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
			}
		}
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
//SA_NAMESPACE_END

