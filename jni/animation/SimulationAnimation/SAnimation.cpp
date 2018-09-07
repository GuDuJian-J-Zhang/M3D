// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		SAnimation.cpp
*
*	@brief	动画类
*
*	@par	历史:
*
****************************************************************************/

#include "stdafx.h"
#include <math.h>
#include <string.h>

#include "../SimulationCommon/SATools.h"
#include "SAnimation.h"
#include "SBehaviorAction.h"
#include "SInterpolator.h"
#include "../SimulationCommon/UtilityXMLParser.h"
#include "../SimulationCommon/vlist.h"
#include "../SimulationCommon/MatrixOperation.h"
#include <typeinfo>

using namespace NS_SimulationAnimation;

//SA_NAMESPACE_BEGIN
CSAnimation::CSAnimation(const char *name, NS_SimulationAnimation::CSBehaviorAction  *BehaviorAction, CSAnimation *outerani)
{
	if (name)
		strcpy(m_Name, name);
	else
		strcpy(m_Name, "");


 	m_Loop = 0;

	m_pParentAnimation = outerani;
	m_pTimeline = 0;
	m_pInstancedAnimation = 0;

	SetBehaviorAction(BehaviorAction);

	m_Delay = 0;
	m_InterpolatorList = new_vlist( malloc, free );
	m_ChildAnimationList = new_vlist( malloc, free );
	m_bCurrentlyRunning = false;
	m_bDefaultActive = true;
 	m_Target = 0;
	m_bExecuteOnce = false;
	m_bExpanded = false;
	m_bLocked = false;

	m_clipPos.x = m_clipPos.y = m_clipPos.z = 0.0;
	m_imgPos.x = m_imgPos.y = m_imgPos.z = 0.0;
	m_imgScale.x = m_imgScale.y = m_imgScale.z = 0.0;
	m_pGroupAnimation=NULL;
}

CSAnimation::~CSAnimation()
{

	delete m_pTimeline;

	START_LIST_ITERATION(CSInterpolator, m_InterpolatorList)
		delete temp;
	END_LIST_ITERATION(m_InterpolatorList)
	delete_vlist(m_InterpolatorList);
	delete_vlist(m_ChildAnimationList);
	
}

void CSAnimation::CleanWithoutDelete()
{
    m_pTimeline = 0;
    vlist_flush(m_InterpolatorList);
}
 
void CSAnimation::AddInterpolator(CSInterpolator *interpolator)
{
 	vlist_add_last(m_InterpolatorList, (void *)interpolator);
	if (interpolator)
		interpolator->SetAnimation(this);
}

void CSAnimation::AddChildAnimation(CSAnimation *animation)
{
 	vlist_add_last(m_ChildAnimationList, (void *)animation);
	animation->SetParentAnimation(this);
	ResetFirstLastTickByChild();
}

void CSAnimation::DelChildAnimation(CSAnimation *animation)
{
	vlist_remove(m_ChildAnimationList, (void *)animation);
	animation->SetParentAnimation(NULL);
	ResetFirstLastTickByChild();
}

CSAnimation * CSAnimation::CreateInstance() 
{ 
	
	CSAnimation *ainst = new CSAnimation(0, GetBehaviorAction(), 0);
	ainst->SetInstancedAnimation(this);
	return ainst;
}



CSInterpolator * CSAnimation::GetInterpolator()
{
	return ((CSInterpolator *)vlist_peek_first(m_InterpolatorList));
}

CSInterpolator * CSAnimation::GetInterpolator(const char *interpolatortype)
{
	CSInterpolator *pCSInterpolator = NULL;
	START_LIST_ITERATION(CSInterpolator, m_InterpolatorList)
		if(strcmp(temp->GetType(),interpolatortype) == 0)
		{
			pCSInterpolator = temp;
			break;
		}
	END_LIST_ITERATION(m_InterpolatorList)
	return pCSInterpolator;
}

void CSAnimation::DeleteKeyframe(int keyframe)
{
	if(IsLocked())
	{
		return;
	}
	CSTimeline *tline = GetTimeline();
	int pos = tline->DeleteKeyframe(keyframe);
	if (pos!=-1)
	{
		vlist_reset_cursor(m_InterpolatorList);
  		while (1)
		{
			CSInterpolator *temp;
			if (!(temp = (CSInterpolator *)vlist_peek_cursor(m_InterpolatorList)))
				break;
			if(pos<temp->GetArrayLength())
			{
				temp->Remove(pos);
			}
  			vlist_advance_cursor(m_InterpolatorList);	
		}
		tline->AddTLRange();
	}
	if (tline->GetTimelineArrayLength() <= 1 &&
		GetParentAnimation() &&
		GetParentAnimation()->GetTimeline())
	{
		GetParentAnimation()->GetTimeline()->AddTLRange();
	}
}

void CSAnimation::AdjustKeyframe(int keyframe, int delta, bool doall, bool relative)
{
	if(IsLocked())
	{
		return;
	}
	CSTimeline *tline = GetTimeline();
	if (relative)
    	    tline->AdjustRelative(keyframe,delta); 
	else 
	    tline->Adjust(keyframe,delta, doall); 
}

void CSAnimation::DuplicateNextOrPrevious(int keyframe, bool next, bool bStep)
{
	if(IsLocked())
	{
		return;
	}
	if (bStep &&
		vlist_count(m_ChildAnimationList) > 0)
	{
		int lastTick = GetLastTick();
		int firstTick = GetFirstTick();
		vlist_s* pChildAniList = new_vlist(malloc, free);
		CSACommonAPI::GetTailChildAnimationList(this, pChildAniList);

		START_LIST_ITERATION(CSAnimation, pChildAniList)
			if (temp && !temp->IsLocked())
			{
				temp->DuplicateNextOrPrevious(keyframe,next,false);
			}
		END_LIST_ITERATION(pChildAniList)
			delete_vlist(pChildAniList);
		return;
	}
	if (GetInterpolator() == NULL)
		return;

	bool replace;
	int pos = GetTimeline()->AddKeyframe(keyframe, replace);
	if (pos!=-1)
	{
		vlist_reset_cursor(m_InterpolatorList);
  		while (1)
		{
			CSInterpolator *temp;
			if (!(temp = (CSInterpolator *)vlist_peek_cursor(m_InterpolatorList)))
				break;
			temp->Duplicate(pos, replace, next);
  			vlist_advance_cursor(m_InterpolatorList);	
		}
		GetTimeline()->AddTLRange();
	}
}

void CSAnimation::Duplicate(int newkf, int oldkf, bool bAddTLRange)
{
	if(IsLocked())
	{
		return;
	}
	bool replace;
	CSTimeline *tl;
	tl = GetTimeline();
 	int pos;
	if ((pos = tl->CheckKeyframe(oldkf))!=-1)
	{
		int adpos = GetTimeline()->AddKeyframe(newkf, replace);
		
		vlist_reset_cursor(m_InterpolatorList);
 		while (1)
		{
			CSInterpolator *temp;
			if (!(temp = (CSInterpolator *)vlist_peek_cursor(m_InterpolatorList)))
				break;
				temp->Copy(pos, adpos, replace);
  			vlist_advance_cursor(m_InterpolatorList);	
		}
		if(bAddTLRange)
			GetTimeline()->AddTLRange();
	}
}

//根据偏移量创建动画
CSAnimation *CSAnimation::CreatOffsetAnimation(AniPoint localPos,AniQuat localQuat,AniPoint localScale,AniPoint offsetPos,AniQuat offsetQuat,AniPoint offsetScale,NS_SimulationAnimation::STargetObject *target)
{
	CSAnimation *pAnimation=new CSAnimation(m_Name,m_pBehaviorAction,NULL);
	CSTimeline *pTimeline=new CSTimeline;
	memcpy(pTimeline,m_pTimeline,sizeof(m_pTimeline));
	int nTimeCount=m_pTimeline->GetTimelineArrayLength();
	int *pNewTime=new int[nTimeCount];
	memcpy(pNewTime,m_pTimeline->GetTimelineArray(),sizeof(int)*nTimeCount);
	pTimeline->SetTimeline(pNewTime,nTimeCount);

	pAnimation->SetTimeline(pTimeline);
	pAnimation->SetTarget(target); 
	pAnimation->SetLoop(m_Loop);
	pAnimation->SetDelay(m_Delay);

	do
	{
		CSInterpolatorPivot * curPivotInter = NULL;
		CSInterpolatorPosition * curPosInter = NULL;
		CSInterpolatorQuatSquad * curQuatSquadInter = NULL;
		START_LIST_ITERATION(CSInterpolator, m_InterpolatorList)
			if(strcmp(temp->GetType(),INTERPOLATOR_PIVOT) == 0)
			{
				curPivotInter=(CSInterpolatorPivot *)temp;
			}
			else if(strcmp(temp->GetType(),INTERPOLATOR_POS) == 0)
			{
				curPosInter=(CSInterpolatorPosition *)temp;
			}
			else if(strcmp(temp->GetType(),INTERPOLATOR_QUATROT) == 0)
			{
				curQuatSquadInter=(CSInterpolatorQuatSquad *)temp;
			}
		END_LIST_ITERATION(m_InterpolatorList)

		if (!curPivotInter||!curPosInter||!curQuatSquadInter)
		{
			break;
		}
		if(curPivotInter->GetArrayLength()!=curPosInter->GetArrayLength()||curPosInter->GetArrayLength()!=curQuatSquadInter->GetArrayLength())
		{
			break;
		}
		CSInterpolatorPivot * interpolatorPivot = new CSInterpolatorPivot();
		pAnimation->AddInterpolator(interpolatorPivot);
		CSInterpolatorPosition * interpolatorPos = new CSInterpolatorPosition();
		pAnimation->AddInterpolator(interpolatorPos);
		CSInterpolatorQuatSquad * interpolatorQuatSquad = new CSInterpolatorQuatSquad();
		pAnimation->AddInterpolator(interpolatorQuatSquad);
		for (int i=0;i<curPivotInter->GetArrayLength();++i)
		{
			CKeyframe *pFrame=curPivotInter->GetAt(i);
			CKeyframeChannel *pPivotChannel=(CKeyframeChannel *)pFrame->Clone();	
			pFrame=curPosInter->GetAt(i);
			CKeyframeChannel *pPosChannel=(CKeyframeChannel *)pFrame->Clone();
			pFrame=curQuatSquadInter->GetAt(i);
			CKeyframeQuatSquad *pQuatSquad=(CKeyframeQuatSquad *)pFrame->Clone();
			//当前配置矩阵
			float fMtxCur[4][4];
			float fCurPos[3];
			float fCurRotation[4];
			float fCurScale[3];
			fCurPos[0]=pPosChannel->m_cp.x;
			fCurPos[1]=pPosChannel->m_cp.y;
			fCurPos[2]=pPosChannel->m_cp.z;
			fCurRotation[0]=pQuatSquad->m_quat.x;
			fCurRotation[1]=pQuatSquad->m_quat.y;
			fCurRotation[2]=pQuatSquad->m_quat.z;
			fCurRotation[3]=pQuatSquad->m_quat.w;
			fCurScale[0]=1;
			fCurScale[1]=1;
			fCurScale[2]=1;	
			MatrixOperation::setTransform(fMtxCur,fCurPos,fCurRotation,fCurScale);
			
			
			float fMtxLocal[4][4];
			float fLocalPos[3];
			float fLocalRotation[4];
			float fLocalScale[3];
			fLocalPos[0]=localPos.x;
			fLocalPos[1]=localPos.y;
			fLocalPos[2]=localPos.z;
			fLocalRotation[0]=localQuat.x;
			fLocalRotation[1]=localQuat.y;
			fLocalRotation[2]=localQuat.z;
			fLocalRotation[3]=localQuat.w;
			fLocalScale[0]=localScale.x;
			fLocalScale[1]=localScale.y;
			fLocalScale[2]=localScale.z;
			MatrixOperation::setTransform(fMtxLocal,fLocalPos,fLocalRotation,fLocalScale);

			float fMtxOffset[4][4];
			float fOffsetPos[3];
			float fOffsetRotation[4];
			float fOffsetScale[3];
			fOffsetPos[0]=offsetPos.x;
			fOffsetPos[1]=offsetPos.y;
			fOffsetPos[2]=offsetPos.z;
			fOffsetRotation[0]=offsetQuat.x;
			fOffsetRotation[1]=offsetQuat.y;
			fOffsetRotation[2]=offsetQuat.z;
			fOffsetRotation[3]=offsetQuat.w;
			fOffsetScale[0]=offsetScale.x;
			fOffsetScale[1]=offsetScale.y;
			fOffsetScale[2]=offsetScale.z;
			MatrixOperation::setTransform(fMtxOffset,fOffsetPos,fOffsetRotation,fOffsetScale);
			
			float fMtxWorld[4][4];
			MatrixOperation::Multiply(fMtxOffset,fMtxCur,fMtxWorld);

			MatrixOperation::MatrixInversion((float *)fMtxOffset);
			float fMtxWorldToLoack[4][4];
			MatrixOperation::Multiply(fMtxOffset,fMtxLocal,fMtxWorldToLoack);
			MatrixOperation::Multiply(fMtxWorld,fMtxWorldToLoack,fMtxLocal);
			float fSo[4];
			MatrixOperation::getTransform(fMtxLocal,fLocalPos,fLocalRotation,fLocalScale,fSo);
	
			AniPoint pivot=pPivotChannel->m_cp;

			MatrixOperation::MatrixInversion((float *)fMtxWorld);
			float fWorldPivot[4][4],fLocalPivot[4][4];
			MatrixOperation::MakeIdentity(fWorldPivot);
			fWorldPivot[3][0]=pivot.x;
			fWorldPivot[3][1]=pivot.y;
			fWorldPivot[3][2]=pivot.z;
			MatrixOperation::Multiply(fWorldPivot,fMtxWorld,fLocalPivot);
			pPivotChannel->m_cp.x=fLocalPivot[3][0];
			pPivotChannel->m_cp.y=fLocalPivot[3][1];
			pPivotChannel->m_cp.z=fLocalPivot[3][2];
			interpolatorPivot->Append(pPivotChannel);


			pQuatSquad->m_quat.x=fLocalRotation[0];
			pQuatSquad->m_quat.y=fLocalRotation[1];
			pQuatSquad->m_quat.z=fLocalRotation[2];
			pQuatSquad->m_quat.w=fLocalRotation[3];

			interpolatorQuatSquad->Append(pQuatSquad);

			pPosChannel->m_cp.x=fLocalPos[0];
			pPosChannel->m_cp.y=fLocalPos[1];
			pPosChannel->m_cp.z=fLocalPos[2];
			//将当前位置转换为沿旋转中心点旋转至单位矩阵后的坐标位置
			pPosChannel->m_cp = CSACommonAPI::MtxTanslationToUniTanslation(pPivotChannel->m_cp,pQuatSquad->m_quat,pPosChannel->m_cp);
			interpolatorPos->Append(pPosChannel);
		}
	}while(false);

	START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
		CSAnimation *pAni=temp->CreatOffsetAnimation(localPos,localQuat,localScale,offsetPos,offsetQuat,offsetScale,target);
		pAnimation->AddChildAnimation(pAni);
	END_LIST_ITERATION(m_ChildAnimationList)
	return pAnimation;	
}
void CSAnimation::Serialize(CUtilityXMLGenerator *xmlgen, CUtilityXMLTag *xmlt)
{
	
 		xmlt->SetTagname("Animation");
 		xmlt->AddProperty("Name", m_Name, true);	 
  		xmlt->AddPropertyInt("Loop", (int)m_Loop);
		xmlt->AddPropertyInt("Delay", (int)m_Delay);
		xmlt->AddPropertyInt("Expand", (int)m_bExpanded);
 		xmlt->SetOpen(true);
   		xmlgen->AddTag(xmlt);

		GetTimeline()->Serialize(xmlgen);

		START_LIST_ITERATION(CSInterpolator, m_InterpolatorList)
			temp->Serialize(xmlgen);
  		END_LIST_ITERATION(m_InterpolatorList);

 
}


void CSAnimation::SerializeTarget(CUtilityXMLTag *xmlt)
{
	/*if (strcmp(m_Target->GetName(), "") != 0)
	{
		char tname[100];
		STargetObject::CreateTargetString("TOB", m_Target->GetName(), tname);
		xmlt->AddProperty("Target", tname, true);
	}
	else*/
		//xmlt->AddProperty("Target", m_Target->GetPath(), true);
		xmlt->AddPropertyInt("TargetID", m_Target->GetTargetKey());		
}
		



void CSAnimation::Serialize(CUtilityXMLGenerator *xmlgen)
{
		CUtilityXMLTag xmlt;
		xmlt.SetTagname("Animation");
	
		if (!m_bDefaultActive)
			xmlt.AddPropertyInt("Active", m_bDefaultActive);
		
 		SerializeTarget(&xmlt);
		Serialize(xmlgen, &xmlt);
  
 		START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
			temp->Serialize(xmlgen);
 		END_LIST_ITERATION(m_ChildAnimationList);
  		xmlgen->CloseTag(&xmlt);
}




void *CSAnimation::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};
	char Target[SA_BUFFER_SIZE_SMALL] = {0};
	int TargetKey = 0;
   	int loop;
	int delay;
	int active = 0;
	int expand = 0;
	if(!xt->GetXMLParser()->GetStackData())
	{
		return NULL;
	}
	

	//NS_SimulationAnimation::CSBehaviorAction  **pSimulationAnimation =(NS_SimulationAnimation::CSBehaviorAction **) m_pExtraData;
	if (!xt->GetProperty("Delay", delay))
				delay = 0;		
	if (!xt->GetProperty("Target", Target))
				strcpy(Target, "");
	if (!xt->GetProperty("TargetID", TargetKey))
		TargetKey = 0;
	if (!xt->GetProperty("Name", Name))
		strcpy(Name, "");
	if (!xt->GetProperty("Expand", expand))
		expand = 0;
	

	if (open)
	{
		if (!xt->GetProperty("Loop", loop))
			loop = 0;
		
		if(NULL == xt->GetXMLParser()->GetStackData())
			return 0;
			
		NS_SimulationAnimation::CSBehaviorAction *tmpBehaviorAction = (NS_SimulationAnimation::CSBehaviorAction *)xt->GetXMLParser()->GetStackData();
		CSAnimation *tmpAnimation = (CSAnimation *)xt->GetXMLParser()->GetStackData();
		CSAnimation* curani = NULL;	
		if(typeid(*tmpAnimation) == typeid(CSAnimation))
		{
			curani = new CSAnimation(Name, tmpAnimation->GetBehaviorAction(), tmpAnimation);
			curani->m_Loop = loop;
			curani->m_Delay = delay;
			curani->m_bExpanded = (bool)expand;
			tmpAnimation->AddChildAnimation(curani);
			tmpAnimation->GetBehaviorAction()->AddAnimation(curani);
		} 			
		else if(typeid(*tmpBehaviorAction) == typeid(NS_SimulationAnimation::CSBehaviorAction))
		{
			curani = new CSAnimation(Name, tmpBehaviorAction, NULL);
			curani->m_Loop = loop;
			curani->m_Delay = delay;
			curani->m_bExpanded = (bool)expand;
 			tmpBehaviorAction->AddAnimation(curani);
		}
		else
		{
			return 0;
		}
		return (curani);
	}
	else
	{
		CSAnimation *outeranim = (CSAnimation *)xt->GetXMLParser()->GetStackData();
		if (outeranim)
		{
 		//	if (outeranim->GetParentAnimation() == 0)
			//{
				CSAnimation *ainst = outeranim;
 
  				if (!xt->GetProperty("Active", active))
						active = true;
				ainst->SetDelay(delay);
				if(strlen(Target) > 0)
				{
					ainst->SetTargetByPath(Target);
				}
				else
				{
					ainst->SetTargetByKey(TargetKey);
				}
				
				ainst->SetDefaultActive(active != 0);
			//} 
		}
	}
	
	return 0;
	
}

void *CSAnimation::XMLCallback_V1(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};
	char Target[SA_BUFFER_SIZE_SMALL] = {0};
	int loop;
	int delay;
	int active = 0;
	if(!xt->GetXMLParser()->GetStackData())
	{
		return NULL;
	}
	//NS_SimulationAnimation::CSBehaviorAction  **pSimulationAnimation =(NS_SimulationAnimation::CSBehaviorAction **) m_pExtraData;
	if (!xt->GetProperty("Delay", delay))
		delay = 0;		
	if (!xt->GetProperty("Target", Target))
		strcpy(Target, "");
	if (!xt->GetProperty("Name", Name))
		strcpy(Name, "");

	if (open)
	{
		if (!xt->GetProperty("Loop", loop))
			loop = 0;

		if(NULL == xt->GetXMLParser()->GetStackData())
			return 0;

		NS_SimulationAnimation::CSBehaviorAction *tmpBehaviorAction = (NS_SimulationAnimation::CSBehaviorAction *)xt->GetXMLParser()->GetStackData();
		CSAnimation *tmpAnimation = (CSAnimation *)xt->GetXMLParser()->GetStackData();
		CSAnimation* curani = NULL;	
		if(typeid(*tmpAnimation) == typeid(CSAnimation))
		{
			curani = new CSAnimation(Name, tmpAnimation->GetBehaviorAction(), tmpAnimation);
			curani->m_Loop = loop;
			curani->m_Delay = delay;
			tmpAnimation->AddChildAnimation(curani);
			tmpAnimation->GetBehaviorAction()->AddAnimation(curani);
		} 			
		else if(typeid(*tmpBehaviorAction) == typeid(NS_SimulationAnimation::CSBehaviorAction))
		{
			curani = new CSAnimation(Name, tmpBehaviorAction, NULL);
			curani->m_Loop = loop;
			curani->m_Delay = delay;
			tmpBehaviorAction->AddAnimation(curani);
		}
		else
		{
			return 0;
		}
		return (curani);
	}
	else
	{
		
		CSAnimation *outeranim = (CSAnimation *)xt->GetXMLParser()->GetStackData();
		if (outeranim)
		{
			if(strstr(Target,"PLCID"))
			{
				char cNewPath10[SA_BUFFER_SIZE_SMALL] = {0};
				if(strlen(Target) >8 && strstr(Target,"PLCID:0"))
				{
					SATools::PlcPath16To10(Target+8,cNewPath10);
				}
				else
				{
					SATools::PlcPath16To10(Target+6,cNewPath10);
				}

				char cNewPath[SA_BUFFER_SIZE_SMALL] = {0};
				strcpy(cNewPath,TARGETOBJECTTYPE_INS);
				strcat(cNewPath,":");
				strcat(cNewPath,cNewPath10);
				strcpy(Target,cNewPath);
			}
			if(strstr(Name,"PMI"))
			{
				int nLastCharIdx = -1;
				for(int i = (int)strlen(Name) -1 ; i >= 0; i--)
				{
					if(Name[i]==':')
					{
						nLastCharIdx = i;
						break;
					}
				}
				char cNewPath[SA_BUFFER_SIZE_SMALL] = {0};
				strcpy(cNewPath,TARGETOBJECTTYPE_PMI);
				strcat(cNewPath,":");
				if(strlen(Target)>8 && strstr(Target,"PLCID:0"))
				{
					char cNewPath10[SA_BUFFER_SIZE_SMALL] = {0};
					SATools::PlcPath16To10(Target+8,cNewPath10);
					
					strcat(cNewPath,cNewPath10);
					strcat(cNewPath,"\\");
				}
				strcat(cNewPath,Name+nLastCharIdx + 1);
				strcpy(Target,cNewPath);
			}

			//	if (outeranim->GetParentAnimation() == 0)
			//{
			CSAnimation *ainst = outeranim;

			if (!xt->GetProperty("Active", active))
				active = true;
			ainst->SetDelay(delay);
			ainst->SetTargetByPath(Target);				
			ainst->SetDefaultActive(active != 0);
			//} 
		}
	}

	return 0;

}

bool CSAnimation::GetInterpolator(int ctime, int &p, CKeyframe **piece)
{

 	int interval;
 	m_pTimeline->SetStartTick(0);
	if (m_pTimeline->GetTimelineInterval(ctime, interval) && GetInterpolator())
	{
		CSTimeline *tl = GetTimeline();
 		int length = tl->GetTimelineArrayLength();
		if (interval >= length)
			return false;

		CKeyframe **kp = GetInterpolator()->GetArray();
		
		p = interval;
		*piece = kp[interval];
		return true;
	}
	return false;
}

 
int CSAnimation::GetFirstTick()
{
	if(GetTimeline() && GetTimeline()->GetTimelineArrayLength() >0)
 		return ((GetTimeline()->GetTimelineArray())[0]);
	else
		return 0;
}

int CSAnimation::GetLastTick()
{
	if(GetTimeline() && GetTimeline()->GetTimelineArrayLength() >0)
		return ((GetTimeline()->GetTimelineArray())[GetTimeline()->GetTimelineArrayLength() - 1]);
	else
		return 0;
}

int CSAnimation::GetLoop(bool bSteps)
{
	bool bRet = m_Loop;
	if (bSteps && !m_Loop && m_ChildAnimationList->count > 0)
	{
		bRet = true;
		START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
			bRet = bRet & temp->GetLoop(bSteps);
		//if(bRet)
		//{
		//	break;
		//}
		END_LIST_ITERATION(m_ChildAnimationList);
	}
	return bRet;
}

void CSAnimation::SetLoop(int loop, bool bSteps)
{
	m_Loop = loop;

	if (bSteps)
	{
		//级连设置子动画的锁定状态
		START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
			temp->SetLoopChild(loop, bSteps);
		END_LIST_ITERATION(m_ChildAnimationList);
	}

	CSAnimation *pParentAnimation = GetParentAnimation();
	if (pParentAnimation)
	{
		if (!loop)
		{
			GetParentAnimation()->SetLoop(loop, false);
		}
	}
}

void CSAnimation::SetLoopChild(int loop, bool bSteps)
{
	m_Loop = loop;
	if (bSteps)
	{
		//级连设置子动画的锁定状态
		START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
			temp->SetLoopChild(loop, bSteps);
		END_LIST_ITERATION(m_ChildAnimationList);
	}
}

void CSAnimation::CloneAnimation()
{
	
}


void CSAnimation::Reset()
{
	CSInterpolator *temp;
	vlist_reset_cursor(m_InterpolatorList);
 	
  	while ((temp = (CSInterpolator *)vlist_peek_cursor(m_InterpolatorList)) != 0)
	{
		if (strcmp(temp->GetType(), "QuatRot") == 0)
		{
			CSInterpolatorQuatSquad *it = (CSInterpolatorQuatSquad *)temp;
			it->AdjustQuaternions();
		}
		temp->Reset();
  		vlist_advance_cursor(m_InterpolatorList);	
	}
	m_bCurrentlyRunning = false;	 
}


bool CSAnimation::Animate(float currenttick)
{
	m_pTimeline->SetCheckRelativeFrame(true);
	bool result = AnimateInternal(currenttick);
	START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
		if (temp->GetDefaultActive())
			temp->Animate(currenttick, GetTimeline()->GetStartTick());
 	END_LIST_ITERATION(m_ChildAnimationList);
	return result;

}

void CSAnimation::Evaluate(float currenttick, bool &hasPos, AniPoint &pos, bool &hasQuat, AniQuat &quat, bool &hasScale, AniPoint &scale)
{
 	int interval;
	float fraction;
 	if (m_pTimeline->Evaluate(currenttick, interval, fraction))
	{
		if(0 > fraction)
			fraction = 0;
 		START_LIST_ITERATION(CSInterpolator, m_InterpolatorList)
			temp->Evaluate(interval, fraction, hasPos, pos, hasQuat, quat, hasScale, scale);
 		END_LIST_ITERATION(m_InterpolatorList)
	}


}


bool CSAnimation::AnimateInternal(float currenttick)
{
 	int interval;
	float fraction;
	if (GetTarget()&&(strcmp(GetTarget()->GetType(),TARGETOBJECTTYPE_GROUP)==0))
	{
		return false;
	}

	//当前帧没有当前动画的数据时，判断跳帧处理是否跳过该动画的关键帧。如果没有跳过关键帧，不需要处理
	if(GetBehaviorAction() && 
		GetBehaviorAction()->IsPlaying() &&
		!GetLoop())
	{
		float fCurrentTick = GetBehaviorAction()->GetCurrentTick();
		float fPreTick = GetBehaviorAction()->GetPreTick();
		if((fCurrentTick > GetLastTick() && fPreTick > GetLastTick()) ||
			(fCurrentTick < GetFirstTick() && fPreTick < GetFirstTick()))
		{
			return false;
		}
	}

	if (m_pTimeline->Evaluate(currenttick, interval, fraction))
	{
		if(0 > fraction)
			fraction = 0;
		CSTimeline *tl = GetTimeline();
 		int length = tl->GetTimelineArrayLength();
 
		//add by likun 2014-11-04_START
		//播放前先设置前后关键帧的旋转点及旋转信息
		int nPreKeyframe = 0;
		int nNextKeyframe = 0;
		if(interval >= length-1)
		{
			nPreKeyframe = nNextKeyframe = length-1;
		}
		else
		{
			if(fraction < 1.0f)
			{
				nPreKeyframe = interval;
				nNextKeyframe = interval+1;
			}
			else
			{
				nPreKeyframe = nNextKeyframe = interval+1;
			}
		}

		START_LIST_ITERATION(CSInterpolator, m_InterpolatorList)
			if(strcmp(temp->GetType(),INTERPOLATOR_PIVOT) == 0)
			{
				GetTarget()->SetPrePivot(((CKeyframeChannel*)temp->GetAt(nPreKeyframe))->m_cp);
				GetTarget()->SetPivot(((CKeyframeChannel*)temp->GetAt(nNextKeyframe))->m_cp);
			}
			else if(strcmp(temp->GetType(),INTERPOLATOR_QUATROT) == 0)
			{
				AniQuat* pQuat = &(((CKeyframeQuatSquad*)temp->GetAt(nPreKeyframe))->m_quat);
				GetTarget()->SetPreQuat(pQuat->x,pQuat->y,pQuat->z,pQuat->w);
				pQuat = &(((CKeyframeQuatSquad*)temp->GetAt(nNextKeyframe))->m_quat);
				GetTarget()->SetQuat(pQuat->x,pQuat->y,pQuat->z,pQuat->w);
			}
		END_LIST_ITERATION(m_InterpolatorList)
		//add by likun 2014-11-04_END

 		START_LIST_ITERATION(CSInterpolator, m_InterpolatorList)
			temp->Interpolate(interval, fraction);
 		END_LIST_ITERATION(m_InterpolatorList)
		SetCurrentlyRunning(true);
		//Edit by Roc 
 		if (!GetBehaviorAction()->IsReversePlay() && interval == length)
			return true;
	}
	return false;
}


bool CSAnimation::Animate(float currenttick, float startframe)
{
 	m_pTimeline->SetStartTick(startframe);
	m_pTimeline->SetCheckRelativeFrame(false);

	bool result = AnimateInternal(currenttick);
	START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
		if (temp->GetDefaultActive())
			temp->Animate(currenttick, startframe);
 	END_LIST_ITERATION(m_ChildAnimationList);
	return result;
}
void CSAnimation::SetTargetByKey(long key)
{
	if (key!= INVALID_KEY)
		m_Target = GetBehaviorAction()->GetTargetObjectByKey(key);
	else
		m_Target = 0;
}
void CSAnimation::SetTargetByPath(const char *name, const char *target)
{
	if (target)
		m_Target = GetBehaviorAction()->CreateTargetObjectByPath(name, target);
	else
		m_Target = 0;
}

void CSAnimation::SetTargetByPath(const char *target)
{
	SetTargetByPath(0, target);
}

void CSAnimation::SetTarget(NS_SimulationAnimation::STargetObject *target)
{
	m_Target = target;
}

void CSAnimation::SetName(const char *name)
{
	if (name)
		strcpy(m_Name, name);
}

void CSAnimation::SetTimeline( CSTimeline *timeline) 
{
	m_pTimeline = timeline; 
	if (m_pTimeline)
		m_pTimeline->SetAnimation(this); 
}
 
float CSAnimation::GetCurrentTick() { 
	return m_pBehaviorAction->GetCurrentTick(); 
}

/*******************************************************************
*	@functin	镜像动画--单条动画
*	@param[in]	无
*	@retval		无
*	@author		Add By YL 2014.10.21
********************************************************************/
int CSAnimation::MirrorImageSingle(int nTick)
{
	if(IsLocked())
	{
		return -3;
	}
	
	//声音动画不能镜像
	if ((strcmp(GetTarget()->GetType(), TARGETOBJECTTYPE_SOUND) == 0 ||
		strcmp(GetName(), TARGETOBJECTTYPE_NAME_SOUND) == 0))
	{
		return -4;
	}
	int newKeyframe = 0,oldKeyframe = 0;				//镜像帧,原始帧

	CSTimeline *t1 = GetTimeline();			//当前动画时间线对象
	int timeLineArrayLen = t1->GetTimelineArrayLength();
	if(timeLineArrayLen > 100)
	{
		//AfxMessageBox(_T("动画的关键帧数超过100，不能进行镜像"));
		return -1;
	}
	int *timelinearray = 0;
	//if (timeLineArrayLen>0)
	//	timelinearray = t1->GetTimelineArray();	//当前动画时间数组0
	for (int i = timeLineArrayLen-1 ; i >= 0; i--)
	{
		timelinearray = t1->GetTimelineArray();
		oldKeyframe = timelinearray[i];
		if(i == timeLineArrayLen-1)				//镜像第1帧+1
		{
			newKeyframe = nTick;
		}
		else
		{
			newKeyframe = newKeyframe + (timelinearray[i+1] - timelinearray[i]);
		}
		
		Duplicate(newKeyframe,oldKeyframe,false);
	}
	GetTimeline()->AddTLRange();
	return 0;
}

/*******************************************************************
*	@functin	镜像动画--所选动画
*	@param[in]	所选动画
*	@retval		无
*	@author		Add By likun 2018.04.17
********************************************************************/
int CSAnimation::MirrorImage(bool bStep)
{
	bool bRet = 0;
	if (IsLocked())
	{
		return -3;
	}
	//声音动画不能镜像
	if ((strcmp(GetTarget()->GetType(), TARGETOBJECTTYPE_SOUND) == 0 ||
		strcmp(GetName(), TARGETOBJECTTYPE_NAME_SOUND) == 0))
	{
		return -4;
	}
	int lastTick = GetLastTick();
	if (bStep &&
		vlist_count(m_ChildAnimationList) > 0)
	{
		vlist_s* pChildAniList = new_vlist(malloc, free);
		CSACommonAPI::GetTailChildAnimationList(this, pChildAniList);

		START_LIST_ITERATION(CSAnimation, pChildAniList)
			bRet = temp->MirrorImageSingle(lastTick + 1 + (lastTick - temp->GetLastTick()));
		END_LIST_ITERATION(pChildAniList)
			delete_vlist(pChildAniList);
		return bRet;
	}
	else
	{
		if (GetInterpolator() == NULL)
			return bRet;
		bRet = MirrorImageSingle(lastTick + 1 );
	}
	
	return bRet;
}

/*******************************************************************
*	@functin	镜像动画--所选动画
*	@param[in]	CTimeLineRange* pTimeLineRange 所选动画段
*	@retval		无
*	@author		Add By YL 2014.10.23
********************************************************************/
int CSAnimation::MirrorImageSelect(CTimeLineRange* pTimeLineRange)
{
	if(IsLocked())
	{
		return -3;
	}
	//声音动画不能镜像
	if ((strcmp(GetTarget()->GetType(), TARGETOBJECTTYPE_SOUND) == 0 ||
		strcmp(GetName(), TARGETOBJECTTYPE_NAME_SOUND) == 0))
	{
		return -4;
	}
	if (pTimeLineRange == NULL)
		return -2;
	int newKeyframe = 0,oldKeyframe = 0;				//镜像帧,原始帧
	int *timeLineArray = 0;						//当前动画数组
	int *timeLineRangeArray = 0;				//当前动画段数组
	CSTimeline *t1 = GetTimeline();			//当前动画时间线对象
	if(!t1)
	{
		return -4;
	}
	int timeLineArrayLen = t1->GetTimelineArrayLength();
	
	if (timeLineArrayLen<=0)
	{
		return -5;
	}
	timeLineArray = t1->GetTimelineArray();	//当前动画时间数组
	if (!timeLineArray)
	{
		return -6;
	}
	timeLineRangeArray = pTimeLineRange->GetKeyFrameIndexArray();
	if (!timeLineRangeArray)
	{
		return -7;
	}
	int beginIndex = pTimeLineRange->GetBeginKeyFrameIndex();				//动画段第1帧的索引值
	int endIndex = pTimeLineRange->GetEndKeyFrameIndex();					//动画段最后1帧的索引值
	int timeLineRangeArrayLen = pTimeLineRange->GetKeyFrameIndexArrayLength();	//当前动画段长度
	if(timeLineRangeArrayLen > 100)
	{
		//AfxMessageBox(_T("关键帧数超过100，不能进行镜像"));
		return -1;
	}

	//后面如有动画段先将其后移
	if(timeLineArrayLen>(endIndex+1))
	{
		AdjustKeyframe(timeLineArray[endIndex+1],timeLineArray[endIndex] - timeLineArray[beginIndex],true,false);
	}
	//动画镜像
	for (int i = timeLineRangeArrayLen-1 ; i >= 0; i--)
	{
		timeLineArray = t1->GetTimelineArray();	//因数组动态更改地址须再声明
		oldKeyframe = timeLineArray[timeLineRangeArray[i]];
		if(i == timeLineRangeArrayLen-1)		//镜像第1帧+1
		{
			newKeyframe = oldKeyframe + 1;
			Duplicate(newKeyframe,oldKeyframe,false);
			continue;
		}
		newKeyframe = newKeyframe + (timeLineArray[timeLineRangeArray[i+1]] - timeLineArray[timeLineRangeArray[i]]);
		Duplicate(newKeyframe,oldKeyframe,false);
	}
	GetTimeline()->AddTLRange();
	return 0;
}

//动画反转
int CSAnimation::Reversion(bool bStep/* = true*/)
{
	bool bRet = 0;
	if(IsLocked())
	{
		return -1;
	}
	//声音动画不能反转
	if ((strcmp(GetTarget()->GetType(), TARGETOBJECTTYPE_SOUND) == 0 ||
		strcmp(GetName(), TARGETOBJECTTYPE_NAME_SOUND) == 0))
	{
		return bRet;
	}
	if(bStep &&
		vlist_count(m_ChildAnimationList)>0 )
	{
		int lastTick = GetLastTick();
		int firstTick = GetFirstTick();
		vlist_s* pChildAniList = new_vlist(malloc, free);
		CSACommonAPI::GetTailChildAnimationList(this, pChildAniList);

		START_LIST_ITERATION(CSAnimation, pChildAniList)
			if (temp && !temp->IsLocked())
			{
				bRet = temp->Reversion();

				//动画反转后与其它动画对齐
				CSTimeline *t1 = temp->GetTimeline();
				int *timelinearray = 0;
				int tineLineArrayLen = t1->GetTimelineArrayLength();
				if (tineLineArrayLen > 0)
				{
					timelinearray = t1->GetTimelineArray();
					temp->AdjustKeyframe(timelinearray[0], (lastTick - timelinearray[tineLineArrayLen - 1]) - (timelinearray[0] - firstTick), true, false);
				}
			}
		END_LIST_ITERATION(pChildAniList)
			delete_vlist(pChildAniList);
		return bRet;
	}
	if (GetInterpolator() == NULL)
		return bRet;

	CKeyframeChannel tmpKeyFrame;
	CKeyframeQuatSquad tmpKeyFrame2;
	int moveValue;

	CSTimeline *t1 = GetTimeline();
	if(!t1)
	{
		return -2;
	}
	int *timelinearray = 0;
	int *tmptimelinearray = 0;//反转后各帧的位置
	int timeLineArrayLen = t1->GetTimelineArrayLength();
	if (timeLineArrayLen<=0)
	{
		return -2;
	}
	timelinearray = t1->GetTimelineArray();
	if(!timelinearray)
	{
		return -2;
	}
	tmptimelinearray = (int*)malloc(sizeof(int)*timeLineArrayLen);
	if(!tmptimelinearray)
	{
		return -2;
	}
	for (int j=0;j<timeLineArrayLen/2;j++)
	{
		vlist_reset_cursor(m_InterpolatorList);
		while (1)
		{
			CSInterpolator *temp;
			if (!(temp = (CSInterpolator *)vlist_peek_cursor(m_InterpolatorList)))
				break;
			CKeyframe * kp1 = temp->GetArray()[j]->Clone();
			CKeyframe * kp2 = temp->GetArray()[timeLineArrayLen-1-j]->Clone();
			temp->Replace(kp2, j);
			temp->Replace(kp1, timeLineArrayLen-1-j);
			
			vlist_advance_cursor(m_InterpolatorList);	
		}

		//记录反转后各帧的位置
		moveValue = (timelinearray[timeLineArrayLen-1] - timelinearray[timeLineArrayLen-1-j])-(timelinearray[j]-timelinearray[0]);
		tmptimelinearray[j] = timelinearray[j] + moveValue;
		if(j != timeLineArrayLen-1-j)
		{
			tmptimelinearray[timeLineArrayLen-1-j] = timelinearray[timeLineArrayLen-1-j] + moveValue;
		}
	}

	//如果动画的帧数是奇数，移动中间帧的位置
	if(timeLineArrayLen%2!=0)
	{
		int pos= timeLineArrayLen/2;
		moveValue = (timelinearray[timeLineArrayLen-1] - timelinearray[timeLineArrayLen-1-pos])-(timelinearray[pos]-timelinearray[0]);
		tmptimelinearray[timeLineArrayLen-1-pos] = timelinearray[timeLineArrayLen-1-pos] + moveValue;
	}

	for (int i=0;i<timeLineArrayLen;i++)
	{
		timelinearray[i] = tmptimelinearray[i];
	}

	t1->SetTimeline(timelinearray,timeLineArrayLen);
	t1->AddTLRange();
	free(tmptimelinearray);
	tmptimelinearray = 0;
    return bRet;
}/************************************************************************/
/* 动画反转（时间线段）                                                                     */
/************************************************************************/
void CSAnimation::ReversionTimeLineRange(VIntArray& keyFrameIndexArray)
{
	int moveValue;
	CSTimeline *t1 = GetTimeline();
	if (!t1)
	{
		return;
	}
	int *timelinearray = 0;
	int* tmptimelinearray = 0;//反转后各帧的位置
	int timeLineArrayLen = t1->GetTimelineArrayLength();
	if (timeLineArrayLen<=0)
	{
		return;
	}
	timelinearray = t1->GetTimelineArray();
	if(!timelinearray)
	{
		return;
	}
	tmptimelinearray = (int*)malloc(sizeof(int)*timeLineArrayLen);
	if(!tmptimelinearray)
	{
		return;
	}
	int keyFrameCount = keyFrameIndexArray.Count();
	if (keyFrameCount <= 0)
		return;
	int beginIndex = keyFrameIndexArray[0];
	int lastIndex = keyFrameIndexArray[keyFrameCount - 1];
	for (int j = 0;j < keyFrameCount/2; j++)
	{
		int tmpIndex1 = keyFrameIndexArray[j];
		int tmpIndex2 = keyFrameIndexArray[keyFrameCount - 1 - j];
		vlist_reset_cursor(m_InterpolatorList);
		while (1)
		{
			CSInterpolator *temp;
			if (!(temp = (CSInterpolator *)vlist_peek_cursor(m_InterpolatorList)))
				break;
			CKeyframe ** pKeyframeArray = NULL;
			int kfArrayLength = temp->GetArrayLength();
			if (kfArrayLength > 0)
				pKeyframeArray = temp->GetArray();
			if (pKeyframeArray && tmpIndex1 < kfArrayLength && tmpIndex2 < kfArrayLength)
			{
				CKeyframe * kp1 = pKeyframeArray[tmpIndex1]->Clone();
				CKeyframe * kp2 = pKeyframeArray[tmpIndex2]->Clone();
				temp->Replace(kp2, tmpIndex1);
				temp->Replace(kp1, tmpIndex2);
			}
			vlist_advance_cursor(m_InterpolatorList);	
		}

		//记录反转后各帧的位置
		moveValue = (timelinearray[lastIndex] - timelinearray[tmpIndex2])-(timelinearray[tmpIndex1]-timelinearray[beginIndex]);
		tmptimelinearray[tmpIndex1 - beginIndex] = timelinearray[tmpIndex1] + moveValue;
		if(tmpIndex1 != tmpIndex2)
		{
			tmptimelinearray[tmpIndex2 - beginIndex] = timelinearray[tmpIndex2] + moveValue;
		}
	}

	if(keyFrameCount%2!=0)
	{
		int pos = keyFrameCount/2+beginIndex;
		moveValue = (timelinearray[lastIndex] - timelinearray[lastIndex - pos])-(timelinearray[pos]-timelinearray[beginIndex]);
		tmptimelinearray[pos - beginIndex] = timelinearray[lastIndex - pos] + moveValue;
	}

	for (int i=0;i<keyFrameCount;i++)
	{
		timelinearray[i+beginIndex] = tmptimelinearray[i];
	}

	t1->SetTimeline(timelinearray,timeLineArrayLen);
	free(tmptimelinearray);
	tmptimelinearray = 0;
}
/*
*功能：判断当前动画是不是间断的

*参数：int iFirstFrame, 第一帧
	   int iSecondFrame 第二帧
	   int* outAnimationType  (O)帧的类型

*返回值：如果当前动画是间断的，返回true，否则返回false
		
*创建：2014-09-28 zhangcc
*/
bool CSAnimation::IsAnimationInterval(int iFirstFrame, int iSecondFrame)
{
	bool bInterval = true;
	if (m_pTimeline == NULL)
		return bInterval;
	if (!(iFirstFrame < m_pTimeline->GetTimelineArrayLength() && iSecondFrame < m_pTimeline->GetTimelineArrayLength()))
		return bInterval;
	CSTimeline *tl = GetTimeline();
	int length = tl->GetTimelineArrayLength();
	if (iFirstFrame >= length || iSecondFrame >= length)
		return false;
	bool bTmpValue = true;
	START_LIST_ITERATION(CSInterpolator, m_InterpolatorList)
		if (iFirstFrame < temp->GetArrayLength())
		{
			bTmpValue = IsAniIntervalByHKeyFrame(temp,iFirstFrame, iSecondFrame);
			bInterval = bInterval & bTmpValue;
		}
	END_LIST_ITERATION(m_InterpolatorList)

	//当插值器为空时，标记为不间断
	if(vlist_count(m_InterpolatorList)<=0)
	{
		bInterval = false;
	}
	return bInterval;
}
/*
*功能：判断当前动画是不是间断的

*参数：int iFirstFrame, 第一帧
	   int iSecondFrame 第二帧
	   HKeyFrame*  (O)关键帧对象

*返回值：如果当前动画是间断的，返回true，否则返回false
		
*创建：2014-10-24 zhangcc
*/
bool CSAnimation::IsAniIntervalByHKeyFrame(CSInterpolator * pInterpolator, int iFirstFrame, int iSecondFrame)
{
	bool bInterval = false;

	CKeyframe* kp1 = NULL;
	CKeyframe* kp2 = NULL;
	if(!pInterpolator)
	{
		return false;
	}
	CKeyframe** pKeyFrameArray = pInterpolator->GetArray();
	if (iFirstFrame >= 0 && iFirstFrame < pInterpolator->GetArrayLength())
	{
		kp1 = pKeyFrameArray[iFirstFrame];
	}
	if (iSecondFrame >= 0 && iSecondFrame < pInterpolator->GetArrayLength())
	{
		kp2 = pKeyFrameArray[iSecondFrame];
	}
	else
		return bInterval;
	int t1 = kp1->m_type;
	int t2 = -1;
	bool linear = false;
	if (t1 == ANIChannel)
		t2 = ((CKeyframeChannel *)kp1)->m_channeltype;
	if (t1 == ANIRotation)
	{
		t2 = ((CKeyframeRotation *)kp1)->m_rotationtype;
		linear = ((CKeyframeRotation *)kp1)->m_bLinear;
	}
	if ((ANIKeyframeType)t1 == ANIChannel && ((ANIChannelType)t2 == ANILinear || (ANIChannelType)t2 == ANIHermiteSpline || (ANIChannelType)t2 == ANIDiscrete || (ANIChannelType)t2 == ANIFollowPath))					
	{
		AniPoint lastPoint1 =((CKeyframeChannel *)kp1)->m_cp;
		AniPoint lastPoint2 =((CKeyframeChannel *)kp2)->m_cp;
		if(strcmp(pInterpolator->GetType(),INTERPOLATOR_POS) == 0
			|| strcmp(pInterpolator->GetType(),INTERPOLATOR_PIVOT) == 0)
		{
			//if(abs(lastPoint1.x - lastPoint2.x) < ANIMATION_D_TOL && 
			//	abs(lastPoint1.y - lastPoint2.y) < ANIMATION_D_TOL && 
			//	abs(lastPoint1.z - lastPoint2.z) < ANIMATION_D_TOL)
			if(lastPoint1.Equal(lastPoint2,(float)ANIMATION_D_TOL))
			{
				bInterval = true;
			}
		}
		else if(strcmp(pInterpolator->GetType(),INTERPOLATOR_COLOR) == 0 || 
			strcmp(pInterpolator->GetType(), INTERPOLATOR_NORMAL) == 0 || 
			strcmp(pInterpolator->GetType(), INTERPOLATOR_SCALE) == 0)
		{
			//if(abs(lastPoint1.x - lastPoint2.x) < ANIMATION_D_TOL2 && 
			//	abs(lastPoint1.y - lastPoint2.y) < ANIMATION_D_TOL2 && 
			//	abs(lastPoint1.z - lastPoint2.z) < ANIMATION_D_TOL2)
			if(lastPoint1.Equal(lastPoint2,(float)ANIMATION_D_TOL2))
			{
				bInterval = true;
			}
		}
	}
	else if ((ANIKeyframeType)t1 == ANIRotation && (ANIRotationType)t2 == ANIQuatSquadRotation)	
	{
		AniQuat lastQuat1 =((CKeyframeQuatSquad *)pInterpolator->GetAt(iFirstFrame))->m_quat;
		AniQuat lastQuat2 =((CKeyframeQuatSquad *)pInterpolator->GetAt(iSecondFrame))->m_quat;
		//if(abs(lastQuat1.x - lastQuat2.x) < ANIMATION_D_TOL2 && 
		//	abs(lastQuat1.y - lastQuat2.y) < ANIMATION_D_TOL2 && 
		//	abs(lastQuat1.z - lastQuat2.z) < ANIMATION_D_TOL2 && 
		//	abs(lastQuat1.w - lastQuat2.w) < ANIMATION_D_TOL2)
		if(lastQuat1.Equal(lastQuat2,(float)ANIMATION_D_TOL2))
		{
			bInterval = true;
		}
	}
	else if(t1 == ANIString)
	{
		if(strcmp(pInterpolator->GetType(),INTERPOLATOR_VISIBLE) == 0)
		{
			char * lastStr1 =((CKeyframeString *)pInterpolator->GetAt(iFirstFrame))->m_target;
			char * lastStr2 =((CKeyframeString *)pInterpolator->GetAt(iSecondFrame))->m_target;
			if(strcmp(lastStr1,lastStr2) == 0)
			{
				bInterval = true;
			}
		}
	}
	return bInterval;
}

bool CSAnimation::IsChild(CSAnimation* pSubAnimation,bool bSteps)
{
	bool bRet = false;
	START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
		if(pSubAnimation == temp)
		{
			bRet = true;
		}
		else
		{
			if(bSteps)
			{
				bRet = temp->IsChild(pSubAnimation,bSteps);
			}
		}
		if(bRet)
		{
			break;
		}
	END_LIST_ITERATION(m_ChildAnimationList);
	return bRet;
}

/************************************************************************/
/* add by likun，根据子动画重新设置父动画的时间线长度                                                                     */
/************************************************************************/
void CSAnimation::ResetFirstLastTickByChild()
{
	int nFistTick = 0;
	int nLastTick = 0;

	if(!GetTimeline())
	{
		return;
	}

	bool bChildHasKeyFrame = false;
	START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
		if(temp->GetTimeline() && temp->GetTimeline()->GetTimelineArrayLength()>0)
		{
			bChildHasKeyFrame = true;
			if(m_ChildAnimationList->cursor_index == 0)
			{
				nFistTick = temp->GetFirstTick();
				nLastTick = temp->GetLastTick();
			}
			else
			{
				if(nFistTick >(int)temp->GetFirstTick())
				{
					nFistTick = temp->GetFirstTick();
				}
				if(nLastTick < (int)temp->GetLastTick())
				{
					nLastTick = temp->GetLastTick();
				}
			};
		}
	END_LIST_ITERATION(m_ChildAnimationList);

	int timeLineLen = GetTimeline()->GetTimelineArrayLength();
	if(nFistTick != nLastTick )
	{
		if(timeLineLen >= 2)
		{
			//只保留两个关键帧
			for(int i = 2; i< timeLineLen; i++)
			{
				GetTimeline()->DeleteKeyframe(GetTimeline()->GetTimelineArray()[i]);
			}
		
			GetTimeline()->GetTimelineArray()[0] = nFistTick;
			GetTimeline()->GetTimelineArray()[1] = nLastTick;
			if(m_pParentAnimation)
			{
				m_pParentAnimation->ResetFirstLastTickByChild();
			}
		}
		else
		{
			if(timeLineLen == 1)
			{
				GetTimeline()->GetTimelineArray()[0] = nFistTick;
			}
			else
			{
				GetTimeline()->AddKeyframe(nFistTick);
			}
			
			GetTimeline()->AddKeyframe(nLastTick);
			GetTimeline()->AddTLRange();
		}
	}
	else
	{
		if(!bChildHasKeyFrame)
		{
			GetTimeline()->DeleteAllKeyframe();
			if(m_pParentAnimation)
			{
				m_pParentAnimation->ResetFirstLastTickByChild();
			}
		}
		else
		{
			if(timeLineLen <= 0)
			{
				GetTimeline()->AddKeyframe(nFistTick);
			}
			else
			{
				//只保留一个关键帧
				for(int i = 1; i< timeLineLen; i++)
				{
					GetTimeline()->DeleteKeyframe(GetTimeline()->GetTimelineArray()[i]);
				}

				GetTimeline()->GetTimelineArray()[0] = nFistTick;
				GetTimeline()->AddTLRange();
			}
			
		}
	}	
}
/************************************************************************/
/* 添加帧                                                                 */
/************************************************************************/
void CSAnimation::AddHKeyFrame(bool bCopyPrevious, bool bLinear)
{
	if(IsLocked())
	{
		return;
	}
	vlist_s* pChildAniList = new_vlist( malloc, free );
	CSACommonAPI::GetTailChildAnimationList(this, pChildAniList);
	START_LIST_ITERATION(CSAnimation, pChildAniList)
		if(temp && !temp->IsLocked())
		{
			int iCurFrame = m_pBehaviorAction->GetCurrentTick();
			int interval;
			float fraction;
			CSTimeline* pTimeline = temp->GetTimeline();
			if (pTimeline == NULL)
				return;
			struct vlist_s*	pInterpolatorList = temp->GetInterpolatorList();
			if(!pInterpolatorList)
			{
				return;
			}
			int tlArrayLength = pTimeline->GetTimelineArrayLength();
			int* tlArray = NULL;
			if (tlArrayLength > 0)
				tlArray = pTimeline->GetTimelineArray();
			bool bCanSlice = false;
			//for (int j = 0; j < tlArrayLength - 1; j++)
			//{
			//	if (iCurFrame >= tlArray[j] && iCurFrame <= tlArray[j+1])
			//	{
			//		bCanSlice = true;
			//		break;
			//	}
			//}

			CTimeLineRange* pTLRange = NULL;
			pTimeline->GetTLRangeByCurFrame(iCurFrame,&pTLRange);
			if(pTLRange)
			{
				bCanSlice = true;
			}

			if (bCanSlice && pTimeline->Evaluate(iCurFrame, interval, fraction))
			{
				if(0 > fraction)
					fraction = 0;
				vlist_t* pInterpolatorList = temp->GetInterpolatorList();
				int iPos = pTimeline->AddKeyframe(iCurFrame);

				unsigned int iTempCursorIdx= pInterpolatorList->cursor_index;
				VLIST_RESET_CURSOR(pInterpolatorList);
 				while (1)
				{
					CSInterpolator *pInterpolator = NULL;
					if (!(pInterpolator = (CSInterpolator *)VLIST_PEEK_CURSOR(pInterpolatorList)))
						break;

		
					AniPoint point;
					AniQuat hQuat;
					const char* interpolatorType = pInterpolator->GetType();
					if (strcmp(INTERPOLATOR_PIVOT, interpolatorType) == 0)
					{
						CSInterpolatorPivot* pInterpolatorPivot = (CSInterpolatorPivot*)pInterpolator;
						pInterpolatorPivot->CalculatePos(interval, fraction, point);
						pInterpolatorPivot->Insert(point, iPos);
					}
					else if (strcmp(INTERPOLATOR_POS, interpolatorType) == 0)
					{
						CSInterpolatorPosition* pInterpolatorPos = (CSInterpolatorPosition*)pInterpolator;
						pInterpolatorPos->CalculatePos(interval, fraction, point);
						if(((CKeyframeChannel*)pInterpolatorPos->GetAt(interval))->m_channeltype == ANILinear)
						{
							pInterpolatorPos->InsertLinear(point, iPos);
						}
						else
						{
							pInterpolatorPos->InsertCurve(point, iPos);
						}
						//if (bLinear)
						//	pInterpolatorPos->InsertLinear(point, iPos);
						//else
						//	pInterpolatorPos->InsertCurve(point, iPos);
					}
					else if (strcmp(INTERPOLATOR_NORMAL, interpolatorType) == 0)
					{
						CSInterpolatorNormal* pInterpolatorNormal = (CSInterpolatorNormal*)pInterpolator;
						pInterpolatorNormal->CalculatePos(interval, fraction, point);
						pInterpolatorNormal->Insert(point, iPos);
					}
					else if (strcmp(INTERPOLATOR_QUATROT, interpolatorType) == 0)
					{
						CSInterpolatorQuatSquad* pInterpolatorQuat = (CSInterpolatorQuatSquad*)pInterpolator;
						pInterpolatorQuat->CalculateQuat(interval, fraction, hQuat);
						if(((CKeyframeQuatSquad*)pInterpolatorQuat->GetAt(interval))->m_bLinear)
						{
							pInterpolatorQuat->InsertLinear(hQuat, iPos);
						}
						else
						{
							pInterpolatorQuat->Insert(hQuat, iPos);
						}
						//if (bLinear)
						//	pInterpolatorQuat->InsertLinear(hQuat, iPos);
						//else
						//	pInterpolatorQuat->Insert(hQuat, iPos);
					}
					else if (strcmp(INTERPOLATOR_SCALE, interpolatorType) == 0)
					{
						CSInterpolatorScale* pInterpolatorScale = (CSInterpolatorScale*)pInterpolator;
						pInterpolatorScale->CalculatePos(interval, fraction, point);
						pInterpolatorScale->Insert(point, iPos);
					}
					else if (strcmp(INTERPOLATOR_COLOR, interpolatorType) == 0)
					{
						CSInterpolatorColor* pInterpolatorColor = (CSInterpolatorColor*)pInterpolator;
						pInterpolatorColor->CalculatePos(interval, fraction, point);
						pInterpolatorColor->Insert(point, iPos);
					}
					else if (strcmp(INTERPOLATOR_VISIBLE, interpolatorType) == 0)
					{
						CSInterpolatorVisible* pInterpolatorColor = (CSInterpolatorVisible*)pInterpolator;
						if (iPos > 0)
						{
							CKeyframeString* pKeyFrame = (CKeyframeString*)pInterpolatorColor->GetArray()[iPos - 1];
							pInterpolatorColor->Insert(pKeyFrame->GetTarget(), iPos);
						}
					}
					VLIST_ADVANCE_CURSOR(pInterpolatorList);
				}
				vlist_nth_item(pInterpolatorList, iTempCursorIdx);

				pTimeline->AddTLRange();
				if (bCopyPrevious)
					temp->DuplicateNextOrPrevious(iCurFrame - 1, true);
			}
		}
	END_LIST_ITERATION(pChildAniList)
	delete_vlist(pChildAniList);
}

void CSAnimation::SetLockedStatus(bool bLock, bool bSteps)
{
	m_bLocked = bLock;
	
	if(bSteps)
	{
		//级连设置子动画的锁定状态
		START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
			temp->SetLockedStatusChild(bLock,bSteps);
		END_LIST_ITERATION(m_ChildAnimationList);
	}

	CSAnimation *pParentAnimation = GetParentAnimation();
	if(pParentAnimation)
	{
		if(bLock)
		{
			////所有子动画锁定时，锁定父动画
			//if(pParentAnimation->IsLocked(true))
			//{
			//	pParentAnimation->SetLockedStatus(bLock,false);
			//}
		}
		else
		{
			//自动解锁父动画
			//if(!pParentAnimation->HasChildLocked(true))
			//	GetParentAnimation()->SetLockedStatus(bLock,false);
			GetParentAnimation()->SetLockedStatus(bLock,false);
		}
	}
}

void CSAnimation::SetLockedStatusChild(bool bLock, bool bSteps)
{
	m_bLocked = bLock;
	if(bSteps)
	{
		//级连设置子动画的锁定状态
		START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
			temp->SetLockedStatusChild(bLock,bSteps);
		END_LIST_ITERATION(m_ChildAnimationList);
	}
}

bool CSAnimation::IsLocked(bool bSteps)
{
	bool bRet = m_bLocked;
	if(bSteps && !m_bLocked && m_ChildAnimationList->count>0)
	{
		bRet = true;
		START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
			bRet = bRet & temp->IsLocked(bSteps);
			//if(bRet)
			//{
			//	break;
			//}
		END_LIST_ITERATION(m_ChildAnimationList);
	}
	return bRet;
}

bool CSAnimation::HasChildLocked(bool bSteps)
{
	bool bRet = false;
	START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
		bRet = temp->IsLocked(bSteps);
		if(bRet)
		{
			break;
		}
	END_LIST_ITERATION(m_ChildAnimationList);
	return bRet;
}

CSAnimation* CSAnimation::Clone(CSBehaviorAction* pBehaviorAction)
{
	CSAnimation* pNewAnimation = NULL;
	pNewAnimation = new CSAnimation(m_Name, pBehaviorAction, 0);
	if (!pNewAnimation)
		return pNewAnimation;
	pNewAnimation->SetExpanded(m_bExpanded);
	pNewAnimation->SetLockedStatus(m_bLocked);
	pNewAnimation->SetLoop(m_Loop);
	CSTimeline* pNewTimeLine = m_pTimeline->Clone();
	if(pNewTimeLine)
		pNewAnimation->SetTimeline(pNewTimeLine);
	START_LIST_ITERATION(CSAnimation, m_ChildAnimationList)
		if(temp && temp->GetTarget())
		{
			CSAnimation* pNewChildAnimation = temp->Clone(pBehaviorAction);
			if(pNewChildAnimation)
			{
				pNewChildAnimation->SetBehaviorAction(pBehaviorAction);
				pNewChildAnimation->SetTargetByPath(temp->GetName(), temp->GetTarget()->GetPath());
				pNewAnimation->AddChildAnimation(pNewChildAnimation);
				pBehaviorAction->AddAnimation(pNewChildAnimation);
			}
		}
	END_LIST_ITERATION(m_ChildAnimationList);

	START_LIST_ITERATION(CSInterpolator, m_InterpolatorList)
		if (temp)
		{
			CSInterpolator* pNewInterpolator = temp->Clone();
			if (pNewInterpolator)
			{
				//pNewInterpolator->SetAnimation(pNewAnimation);
				pNewAnimation->AddInterpolator(pNewInterpolator);
			}
		}
	END_LIST_ITERATION(m_InterpolatorList)
	return pNewAnimation;
}

bool CSAnimation::CopyKeyFrame(int srcTickIndex, int targetTick)
{
	CSTimeline* pTimeline = GetTimeline();
	if (!pTimeline)
		return false;
	int timelineArrayLength = pTimeline->GetTimelineArrayLength();
	int * timelineArray = pTimeline->GetTimelineArray();
	int tmpKeyFrame = -1;
	if (srcTickIndex < timelineArrayLength)
		tmpKeyFrame = timelineArray[srcTickIndex];
	
	bool replace;
	int pos = pTimeline->AddKeyframe(targetTick, replace);
	
	START_LIST_ITERATION(CSInterpolator, m_InterpolatorList)
		CKeyframe* pKeyFrame = temp->GetAt(srcTickIndex);
	CKeyframe* pCloneKeyFrame = pKeyFrame->Clone();
	if (replace)
	{
		temp->Replace(pCloneKeyFrame, pos);
	}
	else
	{
		temp->Insert(pCloneKeyFrame, pos);
	}
	END_LIST_ITERATION(m_InterpolatorList)
		pTimeline->AddTLRange();
		return true;
}

bool CSAnimation::IsCanSlice()
{
	bool bRet = false;
	int iCurFrame = m_pBehaviorAction->GetCurrentTick();
	vlist_s* pChildAniList = new_vlist(malloc, free);
	CSACommonAPI::GetTailChildAnimationList(this, pChildAniList);
	START_LIST_ITERATION(CSAnimation, pChildAniList)
		CSTimeline* pTempTimeline = temp->GetTimeline();
	if (pTempTimeline == NULL)
		return bRet;

	CTimeLineRange* pTLRange = NULL;
	pTempTimeline->GetTLRangeByCurFrame(iCurFrame, &pTLRange);
	if (pTLRange)
	{
		bRet = true;
		break;
	}
	END_LIST_ITERATION(pChildAniList)
		delete_vlist(pChildAniList);
	return bRet;
}

bool CSAnimation::Slice()
{
	bool bRet = false;
	if (IsCanSlice())
	{
		AddHKeyFrame(true);
		bRet = true;
	}
	return bRet;
}

bool CSAnimation::GetCollisionFlg()
{
	bool bRet = true;
	if (CSACommonAPI::IsAmimationFolder(this, GetBehaviorAction()))
	{
		STargetObject* pTargetObject = NULL;
		vlist_s* pChildAniList = new_vlist(malloc, free);
		CSACommonAPI::GetTailChildAnimationList(this, pChildAniList, true);

		START_LIST_ITERATION(CSAnimation, pChildAniList)
			pTargetObject = temp->GetTarget();
		if (pTargetObject)
		{
			bRet = bRet & pTargetObject->GetCollision();
			if (!bRet)
				break;
		}
		END_LIST_ITERATION(pChildAniList)
			delete_vlist(pChildAniList);
	}
	else
	{
		STargetObject* pTargetObject = GetTarget();
		if (pTargetObject)
		{
			bRet = pTargetObject->GetCollision();
		}
	}
	return bRet;
}

void CSAnimation::SetCollisionFlg(bool bCollisionFlg)
{
	if (CSACommonAPI::IsAmimationFolder(this, GetBehaviorAction()))
	{
		STargetObject* pTargetObject = NULL;
		vlist_s* pChildAniList = new_vlist(malloc, free);
		CSACommonAPI::GetTailChildAnimationList(this, pChildAniList, true);
		
		START_LIST_ITERATION(CSAnimation, pChildAniList)
		pTargetObject = temp->GetTarget();
		if (pTargetObject)
		{
			pTargetObject->SetCollision(bCollisionFlg);
		}
		END_LIST_ITERATION(pChildAniList)
			delete_vlist(pChildAniList);
	}
	else
	{
		STargetObject* pTargetObject = GetTarget();
		if (pTargetObject)
		{
			pTargetObject->SetCollision(bCollisionFlg);
		}
	}
}

//SA_NAMESPACE_END

