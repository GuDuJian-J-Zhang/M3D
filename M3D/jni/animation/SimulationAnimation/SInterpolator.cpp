// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		SInterpolator.cpp
*
*	@brief	插值器定义文件，包含插值器的基类、位置插值器、旋转插值器、旋转点插值器、缩放插值器、颜色插值器等。
*
*	@par	历史:
*
****************************************************************************/

#include "stdafx.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "SInterpolator.h"
#include "STimeline.h"
#include "KeyFrame.h"
#include "SBehaviorAction.h"
#include "SAnimation.h"
#include "../SimulationCommon/UtilityXMLParser.h"
#include "../SimulationCommon/UtilityTextParser.h"
#include "../SimulationCommon/SAUtility.h"
#include "../SimulationCommon/SATools.h"
#include "../SimulationCommon/MatrixOperation.h"

#define  KEYFRAMESPLIT "|"
//SA_NAMESPACE_BEGIN

CSInterpolator::CSInterpolator(CSAnimation *animation, const char *name) 
{ 
 	m_pTarget = 0; 			
	m_pAnimation = animation; 
	if (name)
		strcpy(m_Name, name);
	else
		strcpy(m_Name, "");
	m_pInterpolatorInstance = NULL;
}


CSInterpolator::~CSInterpolator()
{
	for (int i=0;i<GetArrayLength();i++)
		delete m_pArray[i];
}

void CSInterpolator::SetInstancedInterpolator(CSInterpolator *interpolator)
{
	m_pInterpolatorInstance = interpolator;
}


void CSInterpolator::Duplicate(int pos, bool rep, bool next)
{
	if ((next && pos>=GetArrayLength()) || (!next && pos<=0))
		return;
	int pos2;
	if(next && rep)
		pos2 = pos+1;
	else if (next)
		pos2 = pos;
	else
		pos2 = pos-1;
	if (pos2>=GetArrayLength())
		return;
	CKeyframe * kp = m_pArray[pos2]->Clone();
	if (rep)
		Replace(kp, pos);
	else
		Insert(kp, pos);
}
	

void CSInterpolator::Copy(int pos, int adpos, bool replace)
{
	CKeyframe * kp = m_pArray[pos]->Clone();
	if (replace)
		Replace(kp, adpos);
	else
		Insert(kp, adpos);
}

void CSInterpolator::GetTranslationFromMatrix(AniPoint &t)
{
	AniPoint pivot;
	
	GetAnimation()->GetTarget()->GetPivot(pivot);
 		
	AniPoint op;
 	op.Set(-pivot.x, -pivot.y, -pivot.z);
 		
	t.Set(op.x + pivot.x, op.y + pivot.y, op.z + pivot.z);
}


void CSInterpolator::GetRotationFromMatrix(AniQuat &t)
{

	AniPoint pivot;
	
}



void CSInterpolator::SetTarget()
{

	m_pTarget = GetAnimation()->GetTarget()->GetTargetKey();
}

CSInterpolator* CSInterpolator::Clone()
{
	return NULL;
}

CSInterpolatorPosition::CSInterpolatorPosition(CSAnimation *animation, const char *name) : CSInterpolator(animation, name)
{
 
}

const char * CSInterpolatorPosition::GetType() { return "Pos"; } 

void CSInterpolatorPosition::Serialize(CUtilityXMLGenerator *xmlgen)
{
 		CUtilityXMLTag xmlt;
		char tagname[SA_BUFFER_SIZE_SMALL] = {0};
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);

 		for (int i=0; i< GetArrayLength(); i++)
			((CKeyframeChannel *)m_pArray[i])->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}

static bool FindModifier(const char *text, const char *modifier, char *result, bool skip_bracket = true)
{
	static delimiter_t find_items_td[] = {{"[", true, "]", true, true} ,
											{"\x41-\x7a", false, " |]", true, false}, 
											{"\x2a", false, " |]", true, false}, 
											{"\x30-\x39|-|", false, " |]", true, false},
											{"\"", true, "\"", true, true}};
	static delimiter_t find_end_td[] = {{"]", true, 0, true, true}};


	if (!result)
		return ((strstr(text, modifier) != 0));

 	CUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[' && skip_bracket)
		tp.SkipAhead(1);
	while (1)
	{
	 	int dlum = -1;
 		bool res = tp.ParseBuffer(result, (int &)dlum, find_items_td, 4);
		if (dlum == 1 || dlum == 2)
		{
			if (strcmp(modifier, result) == 0)
				return true;
		}
		else if (dlum == 0)
		{
			CUtilityTextParser tp1;
		 	tp1.InsertBuffer(result, (unsigned int)strlen(result));
			char mdf[SA_BUFFER_SIZE_SMALL] = {0};
 	 		tp1.ParseBuffer(mdf, (int &)dlum, find_items_td, 4);
			tp1.ParseBuffer(result, (int &)dlum, find_end_td, 1);
			if (strcmp(modifier, mdf) == 0)
				return true;
		}
		else
			return false;
		if (!res)
			return false;
	}
}

static void FindData(char *text, char *result)
{
	static delimiter_t find_items_td[] = {{"[", true, "]", true, true},
											{"\x41-\x7a", false, " |]", true, false}, 
											{"\x30-\x39|-|", false, "]", true, false},
											{"\"", true, "\"", true, true}};
  	int dlum;
	CUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
		tp.SkipAhead(1);
	while (1)
	{
 		tp.ParseBuffer(result, (int &)dlum, find_items_td, 4);
		if (dlum == 2)
			break;

	}
}

static void FindStringData(char *text, char *result)
{
	static delimiter_t find_items_td[] = {{"\"", true, "\"", true, false}};
	
  	int dlum;
	CUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
		tp.SkipAhead(1);
	tp.ParseBuffer(result, (int &)dlum, find_items_td, 1, false);			

}



void *CSInterpolatorPosition::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};
 	
 	if (open && NULL != xt->GetXMLParser()->GetStackData())
	{
		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		CSInterpolatorPosition *itp = new CSInterpolatorPosition(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
	
		while (1)
		{
			CUtilityTextParser tp;
			ANIChannelType channelType = ANILinear;
			bool HoldConstant = false;	
			bool easeinout = false;
			char *litem;
			char result[SA_BUFFER_SIZE_BIG] = {0};
  			AniPoint pos(0,0,0);
			bool followpathlinear = false;
 			if (!xt->GetNextListItem(&litem))
				break;
			if (FindModifier(litem, "l", 0))
					channelType = ANILinear;
			if (FindModifier(litem, "b", 0))
					channelType = ANIHermiteSpline;		 
			if (FindModifier(litem, "d", 0))
					channelType = ANIDiscrete;		 
			if (FindModifier(litem, "ea", 0))
					easeinout = true;		 
			if (FindModifier(litem, "f", 0))
					channelType = ANIFollowPath;
			if (FindModifier(litem, "fl", 0))
			{
					channelType = ANIFollowPath;
					followpathlinear = true;
			}
			if (FindModifier(litem, "*", 0))
			{
				HoldConstant = true;
				channelType = ANIHermiteSpline;		 
			}
 			else
			{
				FindData(litem, result);
				CHAR * pcTemp;
				pcTemp = strtok(result,KEYFRAMESPLIT);
				if(pcTemp != NULL)
				{
					CUtilityXMLParser::GetFloatPoint(pcTemp, pos);
				}
			}

 			if (channelType == ANIFollowPath)
			{
				CKeyframeChannelFollowPath * fp = new CKeyframeChannelFollowPath; 
				fp->m_bLinear = followpathlinear;
				fp->m_cp = pos; 
				itp->m_pArray.InsertAt(fp, counter);
 			}
 			else if (channelType == ANILinear)
				itp->InsertLinear(pos, counter);
			else if (channelType == ANIHermiteSpline)
				itp->InsertCurve(pos, counter);
			else if (channelType == ANIDiscrete)
				itp->InsertDiscrete(pos, counter);
			itp->m_pArray[counter]->SetConstant(HoldConstant);
			itp->m_pArray[counter]->SetEaseInOut(easeinout);
			counter++;

		}

   		current_animation->AddInterpolator(itp);
		if (current_animation->GetTimeline() && current_animation->GetInterpolator(INTERPOLATOR_POS))
			current_animation->GetTimeline()->AddTLRange();
 
	}
	return 0;

}

void *CSInterpolatorPosition::XMLCallback_V1(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{

	char Name[SA_BUFFER_SIZE_SMALL] = {0};

	if (open && NULL != xt->GetXMLParser()->GetStackData())
	{
		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		CSInterpolatorPosition *itpPos = new CSInterpolatorPosition(current_animation, Name);
		CSInterpolatorQuatSquad *itpRot = new CSInterpolatorQuatSquad(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;

		while (1)
		{
			CUtilityTextParser tp;
			ANIChannelType channelType = ANILinear;
			bool HoldConstant = false;	
			bool easeinout = false;
			char *litem;
			char result[SA_BUFFER_SIZE_BIG] = {0};
			AniPoint pos(0,0,0);
			AniQuat quat(0,0,0,0);
			bool followpathlinear = false;
			if (!xt->GetNextListItem(&litem))
				break;
			if (FindModifier(litem, "l", 0))
				channelType = ANILinear;
			if (FindModifier(litem, "b", 0))
				channelType = ANIHermiteSpline;		 
			if (FindModifier(litem, "d", 0))
				channelType = ANIDiscrete;		 
			if (FindModifier(litem, "ea", 0))
				easeinout = true;		 
			if (FindModifier(litem, "f", 0))
				channelType = ANIFollowPath;
			if (FindModifier(litem, "fl", 0))
			{
				channelType = ANIFollowPath;
				followpathlinear = true;
			}
			if (FindModifier(litem, "*", 0))
			{
				HoldConstant = true;
				channelType = ANIHermiteSpline;		 
			}
			else
			{
				FindData(litem, result);
				CHAR * pcTemp;
				pcTemp = strtok(result,KEYFRAMESPLIT);
				if(pcTemp != NULL)
				{
					CUtilityXMLParser::GetFloatPoint(pcTemp, pos);
				}
				pcTemp = strtok(NULL,KEYFRAMESPLIT);
				if(pcTemp != NULL)
				{
					CSACommonAPI::GetQuatPoint(pcTemp, quat);
				}
			}

			if (channelType == ANIFollowPath)
			{
				/*CKeyframeChannelFollowPath * fp = new CKeyframeChannelFollowPath; 
				fp->m_bLinear = followpathlinear;
				fp->m_cp = pos; 
				itp->m_pArray.InsertAt(fp, counter);*/
			}
			else if (channelType == ANILinear)
			{
				itpPos->InsertLinear(pos, counter);
				itpRot->InsertLinear(quat,counter);
			}
			else if (channelType == ANIHermiteSpline)
			{
				itpPos->InsertCurve(pos, counter);
				itpRot->Insert(quat,counter);
			}
			else if (channelType == ANIDiscrete)
			{
				itpPos->InsertDiscrete(pos, counter);
				itpRot->InsertLinear(quat,counter);
				
			}
			itpPos->m_pArray[counter]->SetConstant(HoldConstant);
			itpPos->m_pArray[counter]->SetEaseInOut(easeinout);
			
			//itpRot->m_pArray[counter]->SetConstant(HoldConstant);
			//itpRot->m_pArray[counter]->SetRelative(false);
			//((CKeyframeQuatSquad *)itpRot->m_pArray[counter])->m_ExtraSpins  = 0;
			counter++;

		}

		current_animation->AddInterpolator(itpPos);
		current_animation->AddInterpolator(itpRot);
		if (current_animation->GetTimeline() && current_animation->GetInterpolator(INTERPOLATOR_POS))
			current_animation->GetTimeline()->AddTLRange();

	}
	return 0;

}

CSInterpolator * CSInterpolatorPosition::CreateInstance(CSAnimation *ainst) 
{ 
	
	CSInterpolatorPosition *i = new CSInterpolatorPosition(ainst);
	i->SetInstancedInterpolator((CSInterpolator *)this);
	return ((CSInterpolator *)i);
}


void CSInterpolatorPosition::CalculatePos(int keyframe, float fraction,  AniPoint &res)
{
 	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeChannelCurve **splinearray = (CKeyframeChannelCurve **)GetArray();

	if (!GetAnimation()->GetCurrentlyRunning() && splinearray[0]->m_bConstant)		
 			GetTranslationFromMatrix(splinearray[0]->m_cp);		
 
 
 	if (keyframe >= length-1)
	{
		res = CURVE_ARRAY(length-1)->m_cp;
	}
	else 
	{
	    if (CURVE_ARRAY(keyframe)->m_bEaseInOut)
		fraction = SAUtility::EaseInEaseOut(fraction, 0.49f, 0.02f, 0.49f);

		//Edit by likun 2014-11-04_START
		//如果前后关键帧旋转点不一致，先将前一关键帧的坐标位置转换为沿后一关键帧旋转点旋转至单位矩阵后的坐标坐标
		
		//CURVE_ARRAY(keyframe)->Interpolate((CKeyframeChannel **)splinearray, keyframe, fraction, length, res);
		AniPoint* pPrePivot = m_pAnimation->GetTarget()->GetPrePivot();
		AniPoint* pPivot = m_pAnimation->GetTarget()->GetPivot();
		if (pPrePivot->Equal(*pPivot))
		{
			if(CURVE_ARRAY(keyframe)->m_cp.Equal(CURVE_ARRAY(keyframe+1)->m_cp))
			{
				res = CURVE_ARRAY(keyframe)->m_cp;
			}
			else
			{
				CURVE_ARRAY(keyframe)->Interpolate((CKeyframeChannel **)splinearray, keyframe, fraction, length, res);
			}
		}
		else
		{
			//如果当前帧的前一关键帧和后一关键帧不一致，将前一关键帧的平移量转换为沿后一关键帧旋转点旋转到单位矩阵的位置后再进行插值计算
			CKeyframeChannel * kp1 = NULL;
			CKeyframeChannel * kp2 = NULL;
			
			if (keyframe < length-1)
			{
				if(fraction < 1.0f)
				{
					kp1 = (CKeyframeChannel *)splinearray[keyframe];
					kp2 = (CKeyframeChannel *)splinearray[keyframe+1];
				}
				else
				{
					kp1 = kp2 = (CKeyframeChannel *)splinearray[keyframe+1];
				}
			}
			else
			{
				kp1 = kp2 = (CKeyframeChannel *)splinearray[length-1];
			}

			float fMoveMtx[4][4];
			float fPointA[3];
			float fPrePivot[3];
			fPrePivot[0] = pPrePivot->x; fPrePivot[1] = pPrePivot->y; fPrePivot[2] = pPrePivot->z;

			fPointA[0] = kp1->m_cp.x; fPointA[1] = kp1->m_cp.y; fPointA[2] = kp1->m_cp.z;
			GetAnimation()->GetTarget()->GetPreQuat()->ToMatrix((float*)fMoveMtx);

			//将沿前一关键帧旋转点旋转至单位矩阵的坐标值转换为实际的坐标值
			MatrixOperation::UniTanslationToMtxTanslation(fPrePivot,fMoveMtx,fPointA);


			//将实现的坐标值转换为沿后一关键帧旋转点旋转至单位矩阵后的坐标值
			float fPivot[3];
			fPivot[0] = pPivot->x; fPivot[1] = pPivot->y; fPivot[2] = pPivot->z;

			fMoveMtx[3][0] = fPointA[0]; fMoveMtx[3][1] = fPointA[1]; fMoveMtx[3][2] = fPointA[2]; 
			fPointA[0] = fPivot[0]* fMoveMtx[0][0]+fPivot[1]* fMoveMtx[1][0]+fPivot[2]* fMoveMtx[2][0]+fMoveMtx[3][0];
			fPointA[1] = fPivot[0]* fMoveMtx[0][1]+fPivot[1]* fMoveMtx[1][1]+fPivot[2]* fMoveMtx[2][1]+fMoveMtx[3][1];
			fPointA[2] = fPivot[0]* fMoveMtx[0][2]+fPivot[1]* fMoveMtx[1][2]+fPivot[2]* fMoveMtx[2][2]+fMoveMtx[3][2];
			
			fPointA[0] = fPointA[0] - fPivot[0];
			fPointA[1] = fPointA[1] - fPivot[1];
			fPointA[2] = fPointA[2] - fPivot[2];


			//记录前一关键帧的值
			AniPoint pntRealPos = kp1->m_cp;
			kp1->m_cp.x = fPointA[0];
			kp1->m_cp.y = fPointA[1];
			kp1->m_cp.z = fPointA[2];
			CURVE_ARRAY(keyframe)->Interpolate((CKeyframeChannel **)splinearray, keyframe, fraction, length, res);
			//还原前一帧的值
			kp1->m_cp.Set(pntRealPos);
		}
		//Edit by likun 2014-11-04_END
	}
}


void CSInterpolatorPosition::CalculatePosCamera(int keyframe, float fraction,  AniPoint &res)
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeChannelCurve **splinearray = (CKeyframeChannelCurve **)GetArray();

	//CURVE_ARRAY(keyframe)->Interpolate((CKeyframeChannel **)splinearray, keyframe, fraction, length, res);

	bool hasPos;
	AniPoint pos;
	bool hasQuat;
	AniQuat quat;
	bool hasScale;
	AniPoint scale;

	if (keyframe < length-1)
	{
		int * pTimeLineArray = GetAnimation()->GetTimeline()->GetTimelineArray();	

		AniPoint* pPrePivot = m_pAnimation->GetTarget()->GetPrePivot();
		AniPoint* pPivot = m_pAnimation->GetTarget()->GetPivot();

		if (keyframe < length-1)
		{
			if(fraction < 1.0f)
			{
				//将前一关键帧和后一关键帧沿旋转点旋转到单位矩阵再进行插值计算，计算出的结果再沿旋转点旋转到原角度
				CKeyframeChannel * kp1 = NULL;
				CKeyframeChannel * kp2 = NULL;

				kp1 = (CKeyframeChannel *)splinearray[keyframe];
				kp2 = (CKeyframeChannel *)splinearray[keyframe+1];

				AniPoint pntRealPos1 = kp1->m_cp;
				AniPoint pntRealPos2 = kp2->m_cp;

				kp1->m_cp = CSACommonAPI::MtxTanslationToUniTanslation(*pPrePivot,GetAnimation()->GetTarget()->GetPreQuat(),kp1->m_cp);
				kp2->m_cp = CSACommonAPI::MtxTanslationToUniTanslation(*pPivot,GetAnimation()->GetTarget()->GetQuat(),kp2->m_cp);

				GetAnimation()->Evaluate(GetAnimation()->GetCurrentTick(),hasPos,pos,hasQuat,quat,hasScale,scale);

				//如果前一帧和后一帧的缩放比例不同，要进行特殊处理
				bool hasPos1;
				AniPoint pos1;
				bool hasQuat1;
				AniQuat quat1;
				bool hasScale1;
				AniPoint scale1;
				bool hasPos2;
				AniPoint pos2;
				bool hasQuat2;
				AniQuat quat2;
				bool hasScale2;
				AniPoint scale2;
				int nTick = pTimeLineArray[keyframe];
				GetAnimation()->Evaluate(nTick,hasPos1,pos1,hasQuat1,quat1,hasScale1,scale1);
				nTick = pTimeLineArray[keyframe+1];
				GetAnimation()->Evaluate(nTick,hasPos2,pos2,hasQuat2,quat2,hasScale2,scale2);
				if(!scale1.Equal(scale2))
				{
					double dAspectScale = scale2.x/scale.x;;
					pos.x = pos1.x+ (pos.x -  pos1.x) * dAspectScale;
					pos.y = pos1.y+ (pos.y -  pos1.y) * dAspectScale;
					pos.z = pos1.z+ (pos.z -  pos1.z) * dAspectScale;
				}

				//还原前一帧的值
				kp1->m_cp.Set(pntRealPos1);
				kp2->m_cp.Set(pntRealPos2);

				pos = CSACommonAPI::UniTanslationToMtxTanslation(*pPivot,quat,pos);
			}
			else
			{
				pos = ((CKeyframeChannel *)splinearray[keyframe+1])->m_cp;
			}
		}
	}
	else
	{
		pos = ((CKeyframeChannel *)splinearray[length-1])->m_cp;
	}

	res = pos;
}

void CSInterpolatorPosition::CalculateAllTangents()
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeChannelCurve **splinearray = (CKeyframeChannelCurve **)GetArray();
	for (int i=0;i<length-1;i++)
	{
		if (((CKeyframeChannel *)splinearray[i])->m_channeltype == ANIFollowPath || 
			((CKeyframeChannel *)splinearray[i])->m_channeltype == ANIHermiteSpline)
		{
			
			AniPoint p0, p1, p2, p3;
			p1 = splinearray[i]->m_cp;
			p2 = splinearray[i+1]->m_cp;
			if (i>0)
			{
			    if (splinearray[i-1]->m_channeltype == ANILinear)
				p0 = p1;
			    else
				p0 = splinearray[i - 1]->m_cp;
			}
			else
				p0 = p1;
			if (i<length-2)
			{
    				if (splinearray[i+1]->m_channeltype == ANILinear)
				    p3 = p2;
				else
				    p3 = splinearray[i + 2]->m_cp;
			}
			else
				p3 = p2;
			splinearray[i]->CalculateCurveFactor(GetAnimation()->GetTimeline(), i);
			splinearray[i]->CalculateHermiteTangents(p0, p2, p3);
		}
	}
}

CSInterpolator* CSInterpolatorPosition::Clone()
{
	CSInterpolator* pNewInterpolator = NULL;
	pNewInterpolator = new CSInterpolatorPosition();
	if(!pNewInterpolator)
		return pNewInterpolator;
	pNewInterpolator->SetInstancedInterpolator(NULL);
	for (int i=0; i< GetArrayLength(); i++)
	{
		CKeyframe* pNewKeyFrame = m_pArray[i]->Clone();
		pNewInterpolator->Append(pNewKeyFrame);
	}
	return pNewInterpolator;
}

void CSInterpolatorPosition::Evaluate(int keyframe, float fraction, bool &hasPos, AniPoint &pos, bool &hasQuat, AniQuat &quat, bool &hasScale, AniPoint &scale) 
{	
 	hasPos = true;
 	CalculatePos(keyframe, fraction, pos);
}

void CSInterpolatorPosition::Interpolate(int keyframe, float fraction)
{
	AniPoint res;
	if ( m_pTarget == INVALID_KEY)
		SetTarget();
	
	CSAnimation* pAnimation = GetAnimation();
	if (pAnimation == NULL)
		return;

	CalculatePos(keyframe, fraction, res);
	
	char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(plcIdPath, m_pAnimation->GetTarget()->GetResolvedPath());

	if (pAnimation->GetTarget()->GetCameraType() != NoCamera)
	{
		CalculatePosCamera(keyframe, fraction, res);
		if (!pAnimation->GetBehaviorAction()->IsCameraPlay())
			return;
		if (pAnimation->GetTarget()->GetCameraType() == CameraTargetFree || pAnimation->GetTarget()->GetCameraType() == CameraPositionFree || pAnimation->GetTarget()->GetCameraType() == CameraPositionTarget)
		    InterpolateCamera2(res);
		else
		    InterpolateCamera(res);
	}
	else if (strcmp(pAnimation->GetTarget()->GetType(),TARGETOBJECTTYPE_CLIP) == 0)
	{
		float fPos[3] = {0};
		float fNormal[3] = {0};
		fPos[0] = res.x; fPos[1] = res.y; fPos[2] = res.z;
		char cName[SA_BUFFER_SIZE_SMALL] = {0};
		strcpy(cName, m_pAnimation->GetName());
		pAnimation->GetBehaviorAction()->TransferClipPlane(0,plcIdPath,cName,fNormal,fPos,true);
		//pAnimation->SetClipPos(res);
	}
	else if (strcmp(pAnimation->GetTarget()->GetType(), TARGETOBJECTTYPE_IMAGE) == 0)
	{
		pAnimation->SetImgPos(res);
	}
	else
	{
		//if( GetAnimation()->GetTarget()->GetGeoArray().GetCount()>0)
		//{			
			float fPivot[3];
			float wMoveMtx[4][4];

			memset(wMoveMtx,0,sizeof(float)*16);
			wMoveMtx[0][0] = 1;
			wMoveMtx[1][1] = 1;
			wMoveMtx[2][2] = 1;
			wMoveMtx[3][3] = 1;
			wMoveMtx[3][0] = res.x; wMoveMtx[3][1] = res.y; wMoveMtx[3][2] = res.z; 

			AniPoint* pPoint = m_pAnimation->GetTarget()->GetPivot();
			fPivot[0] = pPoint->x; fPivot[1] = pPoint->y; fPivot[2] = pPoint->z;

			//UINT plcId =atoi(m_pAnimation->GetTarget()->GetResolvedPath());
			pAnimation->GetBehaviorAction()->Transfer(0,plcIdPath,fPivot, wMoveMtx);
			//if(GetAnimation()->GetTarget()->GetCollision())
			//	GetAnimation()->GetBehaviorAction()->Intervene(GetAnimation()->GetTarget()->GetGeoArray());
		//}
		
		

 		pAnimation->GetTarget()->FlagForCollision();
		 
	}
}
 
void CSInterpolatorPosition::InterpolateCamera2(AniPoint &trans, bool simulate)
{
	if (!simulate)
	{
		GetAnimation()->GetBehaviorAction()->CameraUpdated();
	}

}




void CSInterpolatorPosition::InterpolateCamera(AniPoint &pos, bool simulate)
{
	float wMoveMtx[4][4];
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			wMoveMtx[i][j] = 0.0;
		}
	}
	wMoveMtx[0][0] = 1.0;
	wMoveMtx[1][1] = 1.0;
	wMoveMtx[2][2] = 1.0;
	wMoveMtx[3][3] = 1.0;
	wMoveMtx[3][0] = pos.x; wMoveMtx[3][1] = pos.y; wMoveMtx[3][2] = pos.z; 

	char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(plcIdPath, m_pAnimation->GetTarget()->GetResolvedPath());

	AniPoint* pPoint = m_pAnimation->GetTarget()->GetPivot();
	float fPivot[3];
	fPivot[0] = pPoint->x; fPivot[1] = pPoint->y; fPivot[2] = pPoint->z;
	GetAnimation()->GetBehaviorAction()->TransferCamera(plcIdPath,0,fPivot, wMoveMtx,GetAnimation()->GetTarget()->GetCameraType());

	if (!simulate)
	{
		GetAnimation()->GetBehaviorAction()->CameraUpdated();
	}


}


void CSInterpolatorPosition::Reset()
{
	CalculateAllTangents();
}



  
CSInterpolatorColor::CSInterpolatorColor(CSAnimation *animation, const char *name) : CSInterpolator(animation, name)
{
 	SetGeomType("everything");
 	SetColorComponent("diffuse");
}

const char * CSInterpolatorColor::GetType() { return INTERPOLATOR_COLOR; } 


 
void CSInterpolatorColor::Serialize(CUtilityXMLGenerator *xmlgen)
{
 		CUtilityXMLTag xmlt;
		char tagname[SA_BUFFER_SIZE_SMALL] = {0};
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
		xmlt.AddProperty("Type", m_ColorComponent, true);
		xmlt.AddProperty("GeomType", m_GeomType, true);

		int length = GetArrayLength();
		if (length >0)
		{
			CKeyframeChannel **array = (CKeyframeChannel **)GetArray();
			for (int i = 0; i < GetArrayLength(); i++)
				array[i]->Serialize(&xmlt);
		}

   		xmlgen->AddTag(&xmlt);

}

									
	
void *CSInterpolatorColor::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};	
	static delimiter_t find_items_td[] = {{"l|t|b|d|c|", true, 0, true, false},
											{"\x30-\x39|-|", false, " |]", true, false}};

	if (open && xt->GetXMLParser()->GetStackData())
	{
		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		CSInterpolatorColor *itp = new CSInterpolatorColor(current_animation, Name);

		if (!xt->GetProperty("GeomType", itp->GetGeomType()))
			itp->SetGeomType("everything");
 		if (!xt->GetProperty("Type", itp->GetColorComponent()))
			itp->SetColorComponent("diffuse");
 
		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
			CUtilityTextParser tp;
			char *litem;
			char result[SA_BUFFER_SIZE_BIG] = {0};
 			int dlum;
			AniPoint pos;
 			if (!xt->GetNextListItem(&litem))
				break;
			tp.InsertBuffer(litem, (unsigned int)strlen(litem));
			assert(litem[0] == '[');
			tp.SkipAhead(1);
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			if (dlum == 0)
			{
 				tp.ParseBuffer(result, dlum, find_items_td, 2);
			}
			SACLOCALE(pos.x = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			SACLOCALE(pos.y = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			SACLOCALE(pos.z = (float)atof(result));
 			itp->Insert(pos, counter++);

		}

   		current_animation->AddInterpolator(itp);
		if (current_animation->GetTimeline() && current_animation->GetInterpolator(INTERPOLATOR_COLOR))
			current_animation->GetTimeline()->AddTLRange();
	}
	return 0;

}

void CSInterpolatorColor::SetGeomType (const char *geomtype)
{
	if(geomtype && (int)strlen(geomtype) < SA_BUFFER_SIZE_SMALL)
		strcpy(m_GeomType, geomtype);
}

void CSInterpolatorColor::SetColorComponent (const char *colorcomponent)
{
	if(colorcomponent && (int)strlen(colorcomponent) < SA_BUFFER_SIZE_SMALL)
		strcpy(m_ColorComponent, colorcomponent);
}

 


CSInterpolator * CSInterpolatorColor::CreateInstance(CSAnimation *ainst) 
{ 
	
	CSInterpolatorColor *i = new CSInterpolatorColor(ainst);
	i->SetInstancedInterpolator((CSInterpolator *)this);
	return ((CSInterpolator *)i);

}
 
void CSInterpolatorColor::CalculatePos(int keyframe, float fraction,  AniPoint &res)
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeChannel **colorarray  = (CKeyframeChannel **)GetArray();	
	if (keyframe >= length-1)
		res = colorarray[length-1]->m_cp;
	else 
		colorarray[keyframe]->Interpolate((CKeyframeChannel **)colorarray, keyframe, fraction, length, res);
}

CSInterpolator* CSInterpolatorColor::Clone()
{
	CSInterpolator* pNewInterpolator = NULL;
	pNewInterpolator = new CSInterpolatorColor();
	if(!pNewInterpolator)
		return pNewInterpolator;
	pNewInterpolator->SetInstancedInterpolator(NULL);
	for (int i=0; i< GetArrayLength(); i++)
	{
		CKeyframe* pNewKeyFrame = m_pArray[i]->Clone();
		pNewInterpolator->Append(pNewKeyFrame);
	}
	return pNewInterpolator;
}

void CSInterpolatorColor::Interpolate(int keyframe, float fraction)
{

	static delimiter_t find_items_td[] = {{"\x41-\x7a", false, ",| ", true, false}};
 
	AniPoint res;
	CalculatePos(keyframe, fraction,res);

	char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy( plcIdPath, GetAnimation()->GetTarget()->GetResolvedPath());
	char cName[SA_BUFFER_SIZE_SMALL] = "";
	float fColor[3];
	fColor[0] = res.x;
	fColor[1] = res.y;
	fColor[2] = res.z;

	if(strcmp(GetAnimation()->GetTarget()->GetType(),TARGETOBJECTTYPE_PMI) == 0)
	{
		//将PMI所属实例的配置路径及PMIid从target中分离
		char cPMIId[SA_BUFFER_SIZE_SMALL] = "";
		GetAnimation()->GetTarget()->GetInsPathAndPMIIdByResolvedPath(plcIdPath,cPMIId);
		strcpy(cName,TARGETOBJECTTYPE_PMI);
		strcat(cName,":");
		strcat(cName,cPMIId);
	}

	//解决如果某一帧记录的是空颜色（-1,-1,-1）造成播放不正确的问题
	if(((int)fColor[0] != -1 || (int)fColor[1] != -1 || (int)fColor[2] != -1 ) && 
		(fColor[0] < 0.0f || fColor[1] < 0.0f || fColor[2] < 0.0f) )
	{
		fColor[0] += 1.0;
		fColor[1] += 1.0;
		fColor[2] += 1.0;
	}
	GetAnimation()->GetBehaviorAction()->TransferColor(plcIdPath, (char*)cName,fColor);
}

void CSInterpolatorScale::Reset()
{
	CalculateAllTangents();
}

void CSInterpolatorScale::CalculateAllTangents()
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeChannelCurve **splinearray = (CKeyframeChannelCurve **)GetArray();
	for (int i=0;i<length-1;i++)
	{
		if (((CKeyframeChannel *)splinearray[i])->m_channeltype == ANIHermiteSpline)
		{
			
			AniPoint p0, p1, p2, p3;
			p1 = splinearray[i]->m_cp;
			p2 = splinearray[i+1]->m_cp;
			if (i>0)
				p0 = splinearray[i - 1]->m_cp;
			else
				p0 = p1;
			if (i<length-2)
				p3 = splinearray[i + 2]->m_cp;
			else
				p3 = p2;
			splinearray[i]->CalculateCurveFactor(GetAnimation()->GetTimeline(), i);
			splinearray[i]->CalculateHermiteTangents(p0, p2, p3);
		}
	}
}

CSInterpolator* CSInterpolatorScale::Clone()
{
	CSInterpolator* pNewInterpolator = NULL;
	pNewInterpolator = new CSInterpolatorScale();
	if(!pNewInterpolator)
		return pNewInterpolator;
	pNewInterpolator->SetInstancedInterpolator(NULL);
	for (int i=0; i< GetArrayLength(); i++)
	{
		CKeyframe* pNewKeyFrame = m_pArray[i]->Clone();
		pNewInterpolator->Append(pNewKeyFrame);
	}
	return pNewInterpolator;
}

CSInterpolatorScale::CSInterpolatorScale(CSAnimation *animation, const char *name) : CSInterpolator(animation, name)
{
 
}

const char * CSInterpolatorScale::GetType() { return "Scale"; } 


void CSInterpolatorScale::Serialize(CUtilityXMLGenerator *xmlgen)
{
 		CUtilityXMLTag xmlt;
		char tagname[SA_BUFFER_SIZE_SMALL] = {0};
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
  
		int length = GetArrayLength();
		if (length > 0)
		{
			CKeyframeChannel **array = (CKeyframeChannel **)GetArray();
			for (int i = 0; i < GetArrayLength(); i++)
				array[i]->Serialize(&xmlt);
		}

   		xmlgen->AddTag(&xmlt);

}

void *CSInterpolatorScale::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};
 	
	static delimiter_t find_items_td[] = {{"l|t|b|d|c|", true, 0, true, false},
											{"\x30-\x39|-|", false, " |]", true, false}};

	if (open && xt->GetXMLParser()->GetStackData())
	{
		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		CSInterpolatorScale *itp = new CSInterpolatorScale(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
			CUtilityTextParser tp;
			char *litem;
			char result[SA_BUFFER_SIZE_BIG] = {0};
 			int dlum;
			AniPoint pos;
 			if (!xt->GetNextListItem(&litem))
				break;
			tp.InsertBuffer(litem, (unsigned int)strlen(litem));
			assert(litem[0] == '[');
			tp.SkipAhead(1);
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			if (dlum == 0)
			{
 				tp.ParseBuffer(result, dlum, find_items_td, 2);
			}
			SACLOCALE(pos.x = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			SACLOCALE(pos.y = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			SACLOCALE(pos.z = (float)atof(result));
 			itp->Insert(pos, counter++);

		}

   		current_animation->AddInterpolator(itp);
		if (current_animation->GetTimeline() && current_animation->GetInterpolator(INTERPOLATOR_SCALE))
			current_animation->GetTimeline()->AddTLRange();
	}
	return 0;

}

CSInterpolator * CSInterpolatorScale::CreateInstance(CSAnimation *ainst) 
{ 
	
	CSInterpolatorScale *i = new CSInterpolatorScale(ainst);
	i->SetInstancedInterpolator((CSInterpolator *)this);
	return ((CSInterpolator *)i);

}
 
void CSInterpolatorScale::CalculatePos(int keyframe, float fraction,  AniPoint &res)
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	 CKeyframeChannelCurve **splinearray = (CKeyframeChannelCurve **)GetArray();

	if (!GetAnimation()->GetCurrentlyRunning() && splinearray[0]->m_bConstant)		
 			GetTranslationFromMatrix(splinearray[0]->m_cp);		
	
	if (keyframe >= length-1)
		res = CURVE_ARRAY(length-1)->m_cp;
	else 
	{
		CURVE_ARRAY(keyframe)->Interpolate((CKeyframeChannel **)splinearray, keyframe, fraction, length, res);
		res.y = CURVE_ARRAY(keyframe)->m_cp.y;
		res.z = CURVE_ARRAY(keyframe)->m_cp.z;
	}
}


void CSInterpolatorScale::InterpolateCamera2(AniPoint &quat, bool simulate)
{
 	
}

void CSInterpolatorScale::Evaluate(int keyframe, float fraction, bool &hasPos, AniPoint &pos, bool &hasQuat, AniQuat &quat, bool &hasScale, AniPoint &scale) 
{
	hasScale = true;
 	CalculatePos(keyframe, fraction, scale);
}


void CSInterpolatorScale::Interpolate(int keyframe, float fraction)
{
 	AniPoint res;

	if ( m_pTarget == INVALID_KEY)
		SetTarget();

	CalculatePos(keyframe, fraction, res);
 
	if (GetAnimation()->GetTarget()->GetCameraType() != NoCamera)
	{
		if (!GetAnimation()->GetBehaviorAction()->IsCameraPlay())
			return;
		if (GetAnimation()->GetTarget()->GetCameraType() == CameraTargetFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)
		    InterpolateCamera2(res);
		else
		    InterpolateCamera(res);
	}
	else if (strcmp(GetAnimation()->GetTarget()->GetType(), TARGETOBJECTTYPE_IMAGE) == 0)
	{
		GetAnimation()->SetImgScale(res);
	}
	else
	{
		return;	 
	}
 	GetAnimation()->GetTarget()->FlagForCollision();
}
 

void CSInterpolatorScale::InterpolateCamera(AniPoint &scale, bool simulate)
{   
	float wMoveMtx[4][4];
	wMoveMtx[3][0] = scale.x; wMoveMtx[3][1] = scale.y; wMoveMtx[3][2] = scale.z; 

	AniPoint* pPoint = m_pAnimation->GetTarget()->GetPivot();
	float fPivot[3];
	fPivot[0] = pPoint->x; fPivot[1] = pPoint->y; fPivot[2] = pPoint->z;

	char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(plcIdPath, m_pAnimation->GetTarget()->GetResolvedPath());
	GetAnimation()->GetBehaviorAction()->TransferCamera(plcIdPath, 2, fPivot,wMoveMtx,GetAnimation()->GetTarget()->GetCameraType());

	if (!simulate)
	{
		GetAnimation()->GetBehaviorAction()->CameraUpdated();
	}
}

CSInterpolatorQuatSquad::CSInterpolatorQuatSquad(CSAnimation *animationinstance, const char *name) : CSInterpolator(animationinstance, name)
{
 
}

CSInterpolator* CSInterpolatorQuatSquad::Clone()
{
	CSInterpolator* pNewInterpolator = NULL;
	pNewInterpolator = new CSInterpolatorQuatSquad();
	if(!pNewInterpolator)
		return pNewInterpolator;
	pNewInterpolator->SetInstancedInterpolator(NULL);
	for (int i=0; i< GetArrayLength(); i++)
	{
		CKeyframe* pNewKeyFrame = m_pArray[i]->Clone();
		pNewInterpolator->Append(pNewKeyFrame);
	}
	return pNewInterpolator;
}

void CSInterpolatorQuatSquad::AdjustQuaternions()
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeQuatSquad **quatarray  = (CKeyframeQuatSquad **)GetArray();
	float q1[4];
	float q2[4];
	AniQuat qres;
	for (int i=0;i<length-1;i++)
	{

	q1[0] = quatarray[i]->m_quat.x;
	q1[1] = quatarray[i]->m_quat.y;
	q1[2] = quatarray[i]->m_quat.z;
	q1[3] = quatarray[i]->m_quat.w;

	q2[0] = quatarray[i+1]->m_quat.x;
	q2[1] = quatarray[i+1]->m_quat.y;
	q2[2] = quatarray[i+1]->m_quat.z;
	q2[3] = quatarray[i+1]->m_quat.w;

	float dot1, dot2;

	    dot1 = ((q1[0] - q2[0])*(q1[0] - q2[0])
			+ (q1[1] - q2[1])*(q1[1] - q2[1])
			+ (q1[2] - q2[2])*(q1[2] - q2[2])
			+ (q1[3] - q2[3])*(q1[3] - q2[3])); 

	    dot2 = ((q1[0] + q2[0])*(q1[0] + q2[0])
			+ (q1[1] + q2[1])*(q1[1] + q2[1])
			+ (q1[2] + q2[2])*(q1[2] + q2[2])
			+ (q1[3] + q2[3])*(q1[3] + q2[3])); 

	 if (dot2 < dot1) {
		   qres.Set(-q2[0],  -q2[1], -q2[2], -q2[3]);
		   //Edit by likun 2014-11-07
			//Replace(qres, i+1);
		   if(quatarray[i+1]->m_bLinear)
		   {
			   ReplaceLinear(qres, i+1);
		   }
		   else
		   {
				Replace(qres, i+1);
		   }
		}

 
	}
}

void CSInterpolatorQuatSquad::Serialize(CUtilityXMLGenerator *xmlgen)
{
 		CUtilityXMLTag xmlt;
		char tagname[SA_BUFFER_SIZE_SMALL] = {0};
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
 
		int length = GetArrayLength();
		if (length > 0)
		{
			CKeyframeQuatSquad **array = (CKeyframeQuatSquad **)GetArray();
			for (int i = 0; i < GetArrayLength(); i++)
				array[i]->Serialize(&xmlt);
		}

   		xmlgen->AddTag(&xmlt);

}


void *CSInterpolatorQuatSquad::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};
// 	static delimiter_t find_items_td[] = {{"l|t|b|d|c|", true, 0, true, false},
//											{"\x30-\x39|-|", false, " |]", true, false}};

	if (open && xt->GetXMLParser()->GetStackData())
	{

		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");

  
		CSInterpolatorQuatSquad *itp = new CSInterpolatorQuatSquad(current_animation, Name);
 
		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
			bool relative = false;
			bool linear = false;
			bool HoldConstant = false;
			int extraSpins = 0;
			CUtilityTextParser tp;
			char *litem;
			char result[SA_BUFFER_SIZE_BIG] = {0};
 			AniQuat pos;
 			if (!xt->GetNextListItem(&litem))
				break;

			if (FindModifier(litem, "es", result))
			{
				extraSpins = atoi(result);			 
			}

				relative = true;

			if (FindModifier(litem, "r", 0))
				relative = true;

			if (FindModifier(litem, "l", 0))
					linear = true;

			if (FindModifier(litem, "*", 0))
				HoldConstant = true;
 			else
			{
				FindData(litem, result);
				CSACommonAPI::GetQuatPoint(result, pos);
			}

 			itp->Insert(pos, counter);			
			itp->m_pArray[counter]->SetRelative(relative);
			((CKeyframeQuatSquad *)itp->m_pArray[counter])->m_bLinear = linear;
			((CKeyframeQuatSquad *)itp->m_pArray[counter])->m_ExtraSpins  = extraSpins;
			itp->m_pArray[counter]->SetConstant(HoldConstant);
			counter++;

		}

   		current_animation->AddInterpolator(itp);
		if (current_animation->GetTimeline() && current_animation->GetInterpolator(INTERPOLATOR_QUATROT))
			current_animation->GetTimeline()->AddTLRange();
	}

	return 0;

}



CSInterpolator * CSInterpolatorQuatSquad::CreateInstance(CSAnimation *ainst) 
{ 
	
	CSInterpolatorQuatSquad *i = new CSInterpolatorQuatSquad(ainst);
	i->SetInstancedInterpolator((CSInterpolator *)this);
	return ((CSInterpolator *)i);

}
 
const char * CSInterpolatorQuatSquad::GetType() { return "QuatRot"; } 


void CSInterpolatorQuatSquad::CalculateQuat(int keyframe, float fraction,  AniQuat &res)
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeQuatSquad **quatarray  = (CKeyframeQuatSquad **)GetArray();
	AniQuat q0, q1, q2, q3, a, b;
	

	if (!GetAnimation()->GetCurrentlyRunning() && quatarray[0]->m_bConstant)		
 			GetRotationFromMatrix(quatarray[0]->m_quat);

	if (keyframe >= length-1)
		res = quatarray[length-1]->m_quat;
	else 
	{
		q1  = quatarray[keyframe]->m_quat;
		q2 = quatarray[keyframe+1]->m_quat;
				
		if (keyframe>0)
			q0 = quatarray[keyframe-1]->m_quat;
		else
			q0 = q1;		
		if (keyframe<length-2)
			q3 = quatarray[keyframe+2]->m_quat;
		else
			q3 = q2;		
		
		if (quatarray[keyframe]->m_ExtraSpins)
			res = AniQuat::QslerpNoInvertExtraSpins(q1,q2,fraction,quatarray[keyframe]->m_ExtraSpins);
		else
		{		
			
			if (quatarray[keyframe]->m_bLinear)
			{
				float q11[4];
				float q22[4];
				float res2[4];
				q11[0] = q1.x; q11[1] = q1.y; q11[2] = q1.z; q11[3] = q1.w;
				q22[0] = q2.x; q22[1] = q2.y; q22[2] = q2.z; q22[3] = q2.w;			
				SAUtility::TransitionQuaternion(q11, q22, fraction, res2);
				res.x = res2[0]; 	res.y = res2[1]; 		res.z = res2[2]; 		res.w = res2[3];
			}
			else
			{						
				a = AniQuat::Qspline(q0,q1,q2);
				b = AniQuat::Qspline(q1,q2,q3);
				res = AniQuat::Qsquad(q1, q2, a,b, fraction);
			}
		}

	}

}




void CSInterpolatorQuatSquad::Evaluate(int keyframe, float fraction, bool &hasPos, AniPoint &pos, bool &hasQuat, AniQuat &quat, bool &hasScale, AniPoint &scale) 
{
	hasQuat = true;
 	CalculateQuat(keyframe, fraction, quat);
}



void CSInterpolatorQuatSquad::Interpolate(int keyframe, float fraction)
{
 
	AniQuat res;
	if ( m_pTarget == INVALID_KEY)
		SetTarget();

	CalculateQuat(keyframe, fraction, res);

	if (GetAnimation()->GetTarget()->GetCameraType() != NoCamera)
	{
		if (!GetAnimation()->GetBehaviorAction()->IsCameraPlay())
			return;
	        if (GetAnimation()->GetTarget()->GetCameraType() == CameraTargetFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)
		    InterpolateCamera2(res);
		else
		    InterpolateCamera(res);
	}
	else
	{	 	
		float rotation_matrix[4][4];
		res.ToMatrix((float*)rotation_matrix);
		
		NS_SimulationAnimation::STargetObject * pTargetObj = GetAnimation()->GetTarget();
		AniPoint* pPivot = pTargetObj->GetPivot();
		float fPivot[3];
		fPivot[0] = pPivot->x;
		fPivot[1] = pPivot->y;
		fPivot[2] = pPivot->z;

		//UINT plcId = atoi(pTargetObj->GetResolvedPath());
		char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
		strcpy(plcIdPath, m_pAnimation->GetTarget()->GetResolvedPath());

		GetAnimation()->GetBehaviorAction()->Transfer(1,plcIdPath,fPivot, rotation_matrix);
	}
//	GetAnimation()->GetTarget()->AdjustChildTargets();
 	GetAnimation()->GetTarget()->FlagForCollision();

}




void CSInterpolatorQuatSquad::InterpolateCamera2(AniQuat &quat, bool simulate)
{

}


void CSInterpolatorQuatSquad::InterpolateCamera(AniQuat &quat, bool simulate)
{
	float rotation_matrix[4][4];
	quat.ToMatrix((float*)rotation_matrix);
	float MovRot[4][4];
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			MovRot[i][j] = rotation_matrix[i][j];
		}
	}

	AniPoint* pPoint = m_pAnimation->GetTarget()->GetPivot();
	float fPivot[3];
	fPivot[0] = pPoint->x; fPivot[1] = pPoint->y; fPivot[2] = pPoint->z;

	char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(plcIdPath, m_pAnimation->GetTarget()->GetResolvedPath());
	GetAnimation()->GetBehaviorAction()->TransferCamera(plcIdPath, 1,fPivot, MovRot,GetAnimation()->GetTarget()->GetCameraType());
}


CSInterpolatorVisible::CSInterpolatorVisible(CSAnimation *animationinstance, const char *name) 
: CSInterpolator(animationinstance, name)
{

}

const char * CSInterpolatorVisible::GetType() { return "Visible"; } 


void CSInterpolatorVisible::Serialize(CUtilityXMLGenerator *xmlgen)
{
	CUtilityXMLTag xmlt;
	char tagname[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(tagname, GetType());
	strcat(tagname,"Interpolator");
	xmlt.SetTagname(tagname);
	xmlt.AddProperty("Name", m_Name, true);

	int length = GetArrayLength();
	if (length> 0)
	{
		CKeyframeString **array = (CKeyframeString **)GetArray();
		for (int i = 0; i < GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);
	}

	xmlgen->AddTag(&xmlt);

}

void *CSInterpolatorVisible::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};

	if (open && xt->GetXMLParser()->GetStackData())
	{
		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		CSInterpolatorVisible *itp = new CSInterpolatorVisible(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
			char *temp;
			char litem[SA_BUFFER_SIZE_BIG] = {0};
			if (!xt->GetNextListItem(&temp))
				break;
			if (temp[0] == '"')
			{
				strcpy(litem, temp + 1);
				litem[strlen(litem) - 1] = 0;
			}
			else
				strcpy(litem, temp);

			itp->Insert(litem, counter++);
		}

		current_animation->AddInterpolator(itp);
		if (current_animation->GetTimeline() && current_animation->GetInterpolator(INTERPOLATOR_VISIBLE))
			current_animation->GetTimeline()->AddTLRange();
	}
	return 0;

}

CSInterpolator * CSInterpolatorVisible::CreateInstance(CSAnimation *ainst) 
{ 

	CSInterpolatorVisible *i = new CSInterpolatorVisible(ainst);
	i->SetInstancedInterpolator((CSInterpolator *)this);
	return ((CSInterpolator *)i);
}


CSInterpolator* CSInterpolatorVisible::Clone()
{
	CSInterpolator* pNewInterpolator = NULL;
	pNewInterpolator = new CSInterpolatorVisible();
	if(!pNewInterpolator)
		return pNewInterpolator;
	pNewInterpolator->SetInstancedInterpolator(NULL);
	for (int i=0; i< GetArrayLength(); i++)
	{
		CKeyframe* pNewKeyFrame = m_pArray[i]->Clone();
		pNewInterpolator->Append(pNewKeyFrame);
	}
	return pNewInterpolator;
}

void CSInterpolatorVisible::Interpolate(int keyframe, float fraction)
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	if(fraction > (1.0f - ANIMATION_D_TOL2))
	{
		keyframe++;
		fraction = 0.0f;
	}
	CKeyframeString **posarray  = (CKeyframeString **)GetArray();
	char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy( plcIdPath, GetAnimation()->GetTarget()->GetResolvedPath());
	//const char *pName = GetAnimation()->GetInterpolator(INTERPOLATOR_VISIBLE)->GetName();
	bool OnOffFlg = true; 
	char cName[SA_BUFFER_SIZE_SMALL] = "";

	if ( m_pTarget == INVALID_KEY)
		SetTarget();

	char  res[SA_BUFFER_SIZE_BIG] = {0};
	float fTran = 0.0;

	if (strcmp(GetAnimation()->GetTarget()->GetType(),TARGETOBJECTTYPE_CLIP) == 0)
	{
		if (keyframe < (length-1))
		{
			char  res2[SA_BUFFER_SIZE_BIG] = {0};
			strcpy(res2, posarray[keyframe+1]->GetTarget());
			strcpy(res, posarray[keyframe]->GetTarget());
			
			//if(strcmp(res,"off") == 0)
			//	OnOffFlg = false;
			//else if(strcmp(res,"on") == 0)
			//	OnOffFlg = true;
			if(strcmp(res,"on") == 0 || strcmp(res2,"on") == 0)
				OnOffFlg = true;
			else
				OnOffFlg = false;
		}
		else
		{
			OnOffFlg = false;
		}
		int iCurrentTick = m_pAnimation->GetCurrentTick();
		int iFirstTick = m_pAnimation->GetFirstTick();
		if (iCurrentTick < iFirstTick/* || keyframe >= length-1*/)
			OnOffFlg = false;
		//GetAnimation()->SetClipVisible(OnOffFlg);
		float fPos[3] = {0};
		float fNormal[3] = {0};
		m_pAnimation->GetBehaviorAction()->TransferClipPlane(2,plcIdPath, cName, fNormal, fPos, OnOffFlg);
		return;
	}
	else if (strcmp(GetAnimation()->GetTarget()->GetType(),TARGETOBJECTTYPE_ZOOM) == 0)
	{
		strcpy(cName, m_pAnimation->GetName());
		if (keyframe ==0 && fabs(fraction) < ANIMATION_D_TOL2)
		{
			strcpy(res, "off");
		}
		else if (keyframe < length)
		{
			strcpy(res, posarray[keyframe]->GetTarget());
		}
		else
		{
			strcpy(res, posarray[length-1]->GetTarget());
		}
		if(strcmp(res,"off") == 0)
			OnOffFlg = false;
		else if(strcmp(res,"on") == 0)
			OnOffFlg = true;

		GetAnimation()->GetBehaviorAction()->TransferVisible(plcIdPath, (char*)cName, OnOffFlg,fTran);
		return;
	}
	else if (strcmp(m_pAnimation->GetTarget()->GetType(), TARGETOBJECTTYPE_IMAGE) == 0 || 
		strcmp(m_pAnimation->GetTarget()->GetType(),TARGETOBJECTTYPE_ARROW) == 0)
	{
		if (keyframe < (length-1))
		{
			strcpy(res, posarray[keyframe]->GetTarget());
		}
		else
		{
			strcpy(res, posarray[length-1]->GetTarget());
		}

		int iCurrentTick = m_pAnimation->GetCurrentTick();
		int iFirstTick = m_pAnimation->GetFirstTick();
		if (iCurrentTick < iFirstTick)
		{
			OnOffFlg = false;
		}
		else
		{
			if(strcmp(res,"off") == 0 )
			{
				OnOffFlg = false;
			}
			else
			{
				OnOffFlg = true;
			}
		}
		if (strcmp(m_pAnimation->GetTarget()->GetType(), TARGETOBJECTTYPE_IMAGE) == 0)
		{
			strcpy(cName, m_pAnimation->GetName());
			float fPos[3];
			float fScale[3];
			AniPoint imgPos = m_pAnimation->GetImgPos();
			AniPoint imgScale = m_pAnimation->GetImgScale();
			fPos[0] = imgPos.x;
			fPos[1] = imgPos.y;
			fPos[2] = imgPos.z;
			fScale[0] = imgScale.x;
			fScale[1] = imgScale.y;
			fScale[2] = imgScale.z;
			m_pAnimation->GetBehaviorAction()->TransferImage(plcIdPath, cName, fPos, fScale, OnOffFlg);
		}
		else if(strcmp(GetAnimation()->GetTarget()->GetType(),TARGETOBJECTTYPE_ARROW) == 0)
		{
			//将PMI所属实例的配置路径及PMIid从target中分离
			char cPMIId[SA_BUFFER_SIZE_SMALL] = "";
			m_pAnimation->GetTarget()->GetInsPathAndPMIIdByResolvedPath(plcIdPath,cPMIId);
			strcpy(cName, TARGETOBJECTTYPE_ARROW);
			strcat(cName,":");
			strcat(cName,cPMIId);
			m_pAnimation->GetBehaviorAction()->TransferVisible(plcIdPath, (char*)cName, OnOffFlg,fTran);
		}
		return;
	}
	//如果是最后一帧，设置透明度为0
	if (keyframe >= length-1)
	{
		strcpy(res, posarray[length-1]->GetTarget());
		if(strcmp(res,"off") == 0)
		{
			OnOffFlg = false;
		}
		else if(strcmp(res,"on") == 0)
		{
			OnOffFlg = true;
		}
	}
	else 
	{
		char  res2[SA_BUFFER_SIZE_SMALL] = {0};
		strcpy(res, posarray[keyframe]->GetTarget());
		strcpy(res2, posarray[keyframe+1]->GetTarget());
		
		if (strcmp(res,res2) == 0)
		{
			int * pTimeLineArray = GetAnimation()->GetTimeline()->GetTimelineArray();
			int iKeyFrameTick = pTimeLineArray[keyframe];
			float fCurrentTick = GetAnimation()->GetBehaviorAction()->GetCurrentTick();
			float fPreTick = GetAnimation()->GetBehaviorAction()->GetPreTick();
			if (GetAnimation()->GetBehaviorAction()->IsPlaying() && 
				(fCurrentTick > iKeyFrameTick && fPreTick > iKeyFrameTick))
			{
				return;
			}
			else
			{
				if(strcmp(res,"off") == 0 )
				{
					OnOffFlg = false;
				}
				else
				{
					OnOffFlg = true;
				}
			}
		}
		else
		{
			
			if(strcmp(res,"off") == 0 )
			{
				//淡入时，透明度从1至0
				if(fraction == 0)
				{
					OnOffFlg = false;
				}
				else
				{
					OnOffFlg = true;
					fTran = 1.0 - fraction;
				}
			}
			else if(strcmp(res,"on") == 0)
			{
				//淡出时，透明度从0至1
				if(fraction == 1)
				{
					OnOffFlg = false;
				}
				else
				{
					OnOffFlg = true;
					fTran = fraction;
				}
			}
		}
	}

	//if(GetAnimation()->GetFirstTick() >= keyframe && abs(fraction) < 0.001)
	//{
	//	//如果是第一个关键帧，隐藏对象
	//	OnOffFlg = false;
	//}
	if(strcmp(GetAnimation()->GetTarget()->GetType(),TARGETOBJECTTYPE_PMI) == 0)
	{
		//将PMI所属实例的配置路径及PMIid从target中分离
		char cPMIId[SA_BUFFER_SIZE_SMALL] = "";
		GetAnimation()->GetTarget()->GetInsPathAndPMIIdByResolvedPath(plcIdPath,cPMIId);
		strcpy(cName,TARGETOBJECTTYPE_PMI);
		strcat(cName,":");
		strcat(cName,cPMIId);
	}

	GetAnimation()->GetBehaviorAction()->TransferVisible(plcIdPath, (char*)cName, OnOffFlg,fTran);
}



//SA_NAMESPACE_END
/************************************************************************/
/*定义剖面差值器类方法实现 added by yhp 2014-09-24 Start               */
/************************************************************************/
CSInterpolatorClip::CSInterpolatorClip( CSAnimation *animation, const char *name ) : CSInterpolator(animation, name)
{
	
}

const char * CSInterpolatorClip::GetType() {return "Clip";}

CSInterpolator * CSInterpolatorClip::CreateInstance( CSAnimation *ainst )
{
	CSInterpolatorPosition *i = new CSInterpolatorPosition(ainst);
	i->SetInstancedInterpolator((CSInterpolator *)this);
	return ((CSInterpolator *)i);
}

void CSInterpolatorClip::Serialize( CUtilityXMLGenerator *xmlgen )
{
	CUtilityXMLTag xmlt;
	char tagname[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(tagname, GetType());
	strcat(tagname,"Interpolator");
	xmlt.SetTagname(tagname);
	xmlt.AddProperty("Name", m_Name, true);

	for (int i=0; i< GetArrayLength(); i++)
		((CKeyframeChannel *)m_pArray[i])->Serialize(&xmlt);

	xmlgen->AddTag(&xmlt);
}

void * CSInterpolatorClip::XMLCallback( CUtilityXMLTag *xt, bool open, void *m_pExtraData )
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};
 	
 	if (open && NULL != xt->GetXMLParser()->GetStackData())
	{
		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		CSInterpolatorClip *itp = new CSInterpolatorClip(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
	
		while (1)
		{
			CUtilityTextParser tp;
			ANIChannelType channelType = ANILinear;
			bool HoldConstant = false;	
			bool easeinout = false;
			char *litem;
			char result[SA_BUFFER_SIZE_BIG] = {0};
  			AniPoint pos(0,0,0);
			AniPoint normal(0,0,0);
			bool bVisible = false;
			bool followpathlinear = false;
 			if (!xt->GetNextListItem(&litem))
				break;
			if (FindModifier(litem, "l", 0))
					channelType = ANILinear;
			if (FindModifier(litem, "b", 0))
					channelType = ANIHermiteSpline;		 
			if (FindModifier(litem, "d", 0))
					channelType = ANIDiscrete;		 
			if (FindModifier(litem, "ea", 0))
					easeinout = true;		 
			if (FindModifier(litem, "f", 0))
					channelType = ANIFollowPath;
			if (FindModifier(litem, "fl", 0))
			{
					channelType = ANIFollowPath;
					followpathlinear = true;
			}
			if (FindModifier(litem, "*", 0))
			{
				HoldConstant = true;
				channelType = ANIHermiteSpline;		 
			}
 			else
			{
				FindData(litem, result);
				//CUtilityXMLParser::GetFloatPoint(result, pos);
				CHAR * pcTemp;
				pcTemp = strtok(result,KEYFRAMESPLIT);
				if(pcTemp != NULL)
				{
					CUtilityXMLParser::GetFloatPoint(pcTemp, pos);
				}
				pcTemp = strtok(NULL,KEYFRAMESPLIT);
				if(pcTemp != NULL)
				{
					CUtilityXMLParser::GetFloatPoint(pcTemp, normal);
				}
				pcTemp = strtok(NULL,KEYFRAMESPLIT);
				if(pcTemp != NULL)
				{
					bVisible = (bool)atoi(pcTemp) ;
				}
			}

 			if (channelType == ANIFollowPath)
			{
				/*CKeyframeChannelFollowPath * fp = new CKeyframeChannelFollowPath; 
				fp->m_bLinear = followpathlinear;
				fp->m_cp = pos; 
				itp->m_pArray.InsertAt(fp, counter);*/
 			}
 			else if (channelType == ANILinear)
				itp->InsertLinear(pos, normal, bVisible, counter);
			//else if (channelType == ANIHermiteSpline)
				//itp->InsertCurve(pos, counter, quat);
			//else if (channelType == ANIDiscrete)
				//itp->InsertDiscrete(pos, counter, quat);
			itp->m_pArray[counter]->SetConstant(HoldConstant);
			itp->m_pArray[counter]->SetEaseInOut(easeinout);
			counter++;

		}

   		current_animation->AddInterpolator(itp);
		if (current_animation->GetTimeline() && current_animation->GetInterpolator())
			current_animation->GetTimeline()->AddTLRange();
 
	}
	return 0;
}

void CSInterpolatorClip::Interpolate( int keyframe, float fraction )
{
	AniPoint res;
	AniQuat quat;
	if ( m_pTarget == INVALID_KEY)
		SetTarget();


	CalculatePos(keyframe, fraction, res);

	//if( GetAnimation()->GetTarget()->GetGeoArray().GetCount()>0)
	//{			
	float fPivot[3];
	float wMoveMtx[4][4];
	if(fabs(quat.x ) < 0.001
		&& fabs(quat.y ) < 0.001
		&& fabs(quat.z ) < 0.001
		&& fabs(quat.w ) < 0.001)	
	{
		memset(wMoveMtx,0,sizeof(float)*16);
		wMoveMtx[0][0] = 1;
		wMoveMtx[1][1] = 1;
		wMoveMtx[2][2] = 1;
		wMoveMtx[3][3] = 1;
	}
	else
	{
		quat.ToMatrix((float*)wMoveMtx);
	}

	wMoveMtx[3][0] = res.x; wMoveMtx[3][1] = res.y; wMoveMtx[3][2] = res.z; 

	AniPoint* pPoint = m_pAnimation->GetTarget()->GetPivot();
	fPivot[0] = pPoint->x; fPivot[1] = pPoint->y; fPivot[2] = pPoint->z;

	//UINT plcId =atoi(m_pAnimation->GetTarget()->GetResolvedPath());
	char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(plcIdPath, m_pAnimation->GetTarget()->GetResolvedPath());

	GetAnimation()->GetBehaviorAction()->Transfer(0,plcIdPath,fPivot, wMoveMtx);
	//if(GetAnimation()->GetTarget()->GetCollision())
	//	GetAnimation()->GetBehaviorAction()->Intervene(GetAnimation()->GetTarget()->GetGeoArray());
	//}

	GetAnimation()->GetTarget()->FlagForCollision();

}

void CSInterpolatorClip::Evaluate(int keyframe, float fraction, bool &hasPos, AniPoint &pos, bool &hasQuat, AniQuat &quat, bool &hasScale, AniPoint &scale )
{
	hasPos = true;
	CalculatePos(keyframe, fraction, pos);
}

void CSInterpolatorClip::Reset()
{
	CalculateAllTangents();
}

void CSInterpolatorClip::CalculatePos( int keyframe, float fraction, AniPoint &res)
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeChannelCurve **splinearray = (CKeyframeChannelCurve **)GetArray();

	if (!GetAnimation()->GetCurrentlyRunning() && splinearray[0]->m_bConstant)		
		GetTranslationFromMatrix(splinearray[0]->m_cp);		


	if (keyframe >= length-1)
	{
		res = CURVE_ARRAY(length-1)->m_cp;
	}
	else 
	{
		if (CURVE_ARRAY(keyframe)->m_bEaseInOut)
			fraction = SAUtility::EaseInEaseOut(fraction, 0.49f, 0.02f, 0.49f);	

		CURVE_ARRAY(keyframe)->Interpolate((CKeyframeChannel **)splinearray, keyframe, fraction, length, res);
	}
}

CSInterpolator* CSInterpolatorClip::Clone()
{
	CSInterpolator* pNewInterpolator = NULL;
	pNewInterpolator = new CSInterpolatorClip();
	if(!pNewInterpolator)
		return pNewInterpolator;
	pNewInterpolator->SetInstancedInterpolator(NULL);
	for (int i=0; i< GetArrayLength(); i++)
	{
		CKeyframe* pNewKeyFrame = m_pArray[i]->Clone();
		pNewInterpolator->Append(pNewKeyFrame);
	}
	return pNewInterpolator;
}

void CSInterpolatorClip::CalculateAllTangents()
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeChannelCurve **splinearray = (CKeyframeChannelCurve **)GetArray();
	
	for (int i=0;i<length-1;i++)
	{
		if (((CKeyframeChannel *)splinearray[i])->m_channeltype == ANIFollowPath || 
			((CKeyframeChannel *)splinearray[i])->m_channeltype == ANIHermiteSpline)
		{

			AniPoint p0, p1, p2, p3;
			p1 = splinearray[i]->m_cp;
			p2 = splinearray[i+1]->m_cp;
			if (i>0)
			{
				if (splinearray[i-1]->m_channeltype == ANILinear)
					p0 = p1;
				else
					p0 = splinearray[i - 1]->m_cp;
			}
			else
				p0 = p1;
			if (i<length-2)
			{
				if (splinearray[i+1]->m_channeltype == ANILinear)
					p3 = p2;
				else
					p3 = splinearray[i + 2]->m_cp;
			}
			else
				p3 = p2;
			splinearray[i]->CalculateCurveFactor(GetAnimation()->GetTimeline(), i);
			splinearray[i]->CalculateHermiteTangents(p0, p2, p3);
		}
	}
}
/************************************************************************/
/*定义剖面差值器类方法实现 added by yhp 2014-09-24 End               */
/************************************************************************/


/************************************************************************/
/*定义剖面差值器类方法实现(新的类) added by yhp 2014-10-25  Start   */
/************************************************************************/
CSInterpolatorNormal::CSInterpolatorNormal( CSAnimation *animation /*= 0*/, const char *name /*= 0*/ ) : CSInterpolator(animation, name)
{

}

const char * CSInterpolatorNormal::GetType()
{
	return INTERPOLATOR_NORMAL;
}

CSInterpolator * CSInterpolatorNormal::CreateInstance( CSAnimation *ainst )
{
	CSInterpolatorNormal *i = new CSInterpolatorNormal(ainst);
	i->SetInstancedInterpolator((CSInterpolator *)this);
	return ((CSInterpolator *)i);
}

void CSInterpolatorNormal::Serialize( CUtilityXMLGenerator *xmlgen )
{
	CUtilityXMLTag xmlt;
	char tagname[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(tagname, GetType());
	strcat(tagname,"Interpolator");
	xmlt.SetTagname(tagname);
	xmlt.AddProperty("Name", m_Name, true);

	int length = GetArrayLength();
	if (length > 0)
	{
		CKeyframeChannel **array = (CKeyframeChannel **)GetArray();
		for (int i = 0; i < GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);
	}

	xmlgen->AddTag(&xmlt);
}

void * CSInterpolatorNormal::XMLCallback( CUtilityXMLTag *xt, bool open, void *m_pExtraData )
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};	
	static delimiter_t find_items_td[] = {{"l|t|b|d|c|", true, 0, true, false},
	{"\x30-\x39|-|", false, " |]", true, false}};

	if (open && xt->GetXMLParser()->GetStackData())
	{
		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		CSInterpolatorNormal *itp = new CSInterpolatorNormal(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
			CUtilityTextParser tp;
			char *litem;
			char result[SA_BUFFER_SIZE_BIG] = {0};
			int dlum;
			AniPoint pos;
			if (!xt->GetNextListItem(&litem))
				break;
			tp.InsertBuffer(litem, (unsigned int)strlen(litem));
			assert(litem[0] == '[');
			tp.SkipAhead(1);
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			if (dlum == 0)
			{
				tp.ParseBuffer(result, dlum, find_items_td, 2);
			}
			SACLOCALE(pos.x = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			SACLOCALE(pos.y = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			SACLOCALE(pos.z = (float)atof(result));
			itp->Insert(pos, counter++);

		}

		current_animation->AddInterpolator(itp);
		if (current_animation->GetTimeline() && current_animation->GetInterpolator(INTERPOLATOR_NORMAL))
			current_animation->GetTimeline()->AddTLRange();
	}
	return 0;
}

void CSInterpolatorNormal::Interpolate( int keyframe, float fraction )
{
	if (!m_pAnimation)
		return;
	AniPoint res;
	CalculatePos(keyframe, fraction,res);

	char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy( plcIdPath, m_pAnimation->GetTarget()->GetResolvedPath());
	char cName[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(cName, m_pAnimation->GetName());
	float fNormal[3];
	fNormal[0] = res.x;
	fNormal[1] = res.y;
	fNormal[2] = res.z;
	AniPoint pos = m_pAnimation->GetClipPos();
	float fPos[3] = {pos.x, pos.y, pos.z};
	//bool bVisible = m_pAnimation->GetClipVisible();
	bool bVisible = true;
	/*int iCurKeyframe = m_pAnimation->GetCurrentTick();
	CTimeLineRange* pTLRange = NULL;
	int iTLRangeIndex = m_pAnimation->GetTimeline()->GetTLRangeByCurFrame(iCurKeyframe, &pTLRange);
	if (iTLRangeIndex == -1)
		bVisible = false;*/
	int iCurrentTick = m_pAnimation->GetCurrentTick();
	int iFirstTick = m_pAnimation->GetFirstTick();
	if (iCurrentTick <= iFirstTick || keyframe >= GetArrayLength()-1)
		bVisible = false;
	m_pAnimation->GetBehaviorAction()->TransferClipPlane(1,plcIdPath, cName, fNormal, fPos, bVisible);
}

void CSInterpolatorNormal::CalculatePos( int keyframe, float fraction, AniPoint &res )
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeChannel **colorarray  = (CKeyframeChannel **)GetArray();	
	if (keyframe >= length-1)
		res = colorarray[length-1]->m_cp;
	else 
		colorarray[keyframe]->Interpolate((CKeyframeChannel **)colorarray, keyframe, fraction, length, res);
}

CSInterpolator* CSInterpolatorNormal::Clone()
{
	CSInterpolator* pNewInterpolator = NULL;
	pNewInterpolator = new CSInterpolatorNormal();
	if(!pNewInterpolator)
		return pNewInterpolator;
	pNewInterpolator->SetInstancedInterpolator(NULL);
	for (int i=0; i< GetArrayLength(); i++)
	{
		CKeyframe* pNewKeyFrame = m_pArray[i]->Clone();
		pNewInterpolator->Append(pNewKeyFrame);
	}
	return pNewInterpolator;
}


/************************************************************************/
/*定义旋转点差值器类，实现可沿多沿旋转功能 added by likun 2014-11-03 */
/************************************************************************/
CSInterpolatorPivot::CSInterpolatorPivot(CSAnimation *animation, const char *name) : CSInterpolator(animation, name)
{
	
}

const char * CSInterpolatorPivot::GetType() { return INTERPOLATOR_PIVOT; } 



void CSInterpolatorPivot::Serialize(CUtilityXMLGenerator *xmlgen)
{
	CUtilityXMLTag xmlt;
	char tagname[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(tagname, GetType());
	strcat(tagname,"Interpolator");
	xmlt.SetTagname(tagname);
	xmlt.AddProperty("Name", m_Name, true);
	int length = GetArrayLength();
	if (length > 0)
	{
		CKeyframeChannel **array = (CKeyframeChannel **)GetArray();
		for (int i = 0; i < GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);
	}

	xmlgen->AddTag(&xmlt);

}



void *CSInterpolatorPivot::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};	
	static delimiter_t find_items_td[] = {{"l|t|b|d|c|", true, 0, true, false},
	{"\x30-\x39|-|", false, " |]", true, false}};

	if (open && xt->GetXMLParser()->GetStackData())
	{
		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		CSInterpolatorPivot *itp = new CSInterpolatorPivot(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
			CUtilityTextParser tp;
			char *litem;
			char result[SA_BUFFER_SIZE_BIG] = {0};
			int dlum;
			AniPoint pos;
			if (!xt->GetNextListItem(&litem))
				break;
			tp.InsertBuffer(litem, (unsigned int)strlen(litem));
			assert(litem[0] == '[');
			tp.SkipAhead(1);
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			if (dlum == 0)
			{
				tp.ParseBuffer(result, dlum, find_items_td, 2);
			}
			SACLOCALE(pos.x = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			SACLOCALE(pos.y = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			SACLOCALE(pos.z = (float)atof(result));
			itp->Insert(pos, counter++);

		}

		current_animation->AddInterpolator(itp);
		if (current_animation->GetTimeline() && current_animation->GetInterpolator(INTERPOLATOR_PIVOT))
			current_animation->GetTimeline()->AddTLRange();
	}
	return 0;

}


CSInterpolator * CSInterpolatorPivot::CreateInstance(CSAnimation *ainst) 
{ 

	CSInterpolatorPivot *i = new CSInterpolatorPivot(ainst);
	i->SetInstancedInterpolator((CSInterpolator *)this);
	return ((CSInterpolator *)i);

}

void CSInterpolatorPivot::CalculatePos(int keyframe, float fraction,  AniPoint &res)
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeChannel **colorarray  = (CKeyframeChannel **)GetArray();	
	if (keyframe >= length-1)
		res = colorarray[length-1]->m_cp;
	else 
	{
		if (fraction == 0.0f || fraction == 1.0f)
			res.Set(colorarray[keyframe]->m_cp);
		else
			res.Set(colorarray[keyframe+1]->m_cp);
		//colorarray[keyframe]->Interpolate((CKeyframeChannel **)colorarray, keyframe, fraction, length, res);
	}
}

CSInterpolator* CSInterpolatorPivot::Clone()
{
	CSInterpolator* pNewInterpolator = NULL;
	pNewInterpolator = new CSInterpolatorPivot();
	if(!pNewInterpolator)
		return pNewInterpolator;
	pNewInterpolator->SetInstancedInterpolator(NULL);
	for (int i=0; i< GetArrayLength(); i++)
	{
		CKeyframe* pNewKeyFrame = m_pArray[i]->Clone();
		pNewInterpolator->Append(pNewKeyFrame);
	}
	return pNewInterpolator;
}

void CSInterpolatorPivot::Interpolate(int keyframe, float fraction)
{

	static delimiter_t find_items_td[] = {{"\x41-\x7a", false, ",| ", true, false}};

	AniPoint res;
	CalculatePos(keyframe, fraction,res);

	GetAnimation()->GetTarget()->SetPivot(res.x,res.y,res.z);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//声音插值器定义Start... Add By YL 2014.11.8
CSInterpolatorSound::CSInterpolatorSound(CSAnimation *animationinstance, const char *name) 
: CSInterpolator(animationinstance, name)
{
	m_pSound = NULL;
}

CSInterpolatorSound::~CSInterpolatorSound(void)
{
	if(m_pSound)
	{
		delete m_pSound;
		m_pSound = NULL;
	}
}

const char * CSInterpolatorSound::GetType() { return INTERPOLATOR_SOUND; } 


void CSInterpolatorSound::Serialize(CUtilityXMLGenerator *xmlgen)
{
	CUtilityXMLTag xmlt;
	char tagname[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(tagname, GetType());
	strcat(tagname,"Interpolator");
	xmlt.SetTagname(tagname);
	xmlt.AddProperty("Name", m_Name, true);

	int length = GetArrayLength();
	if (length > 0)
	{
		CKeyframeString **array = (CKeyframeString **)GetArray();
		for (int i = 0; i < GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);
	}

	xmlgen->AddTag(&xmlt);

}

void *CSInterpolatorSound::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};

	if (open && xt->GetXMLParser()->GetStackData())
	{
		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		CSInterpolatorSound *itp = new CSInterpolatorSound(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
			char *temp;
			char litem[SA_BUFFER_SIZE_BIG] = {0};
			if (!xt->GetNextListItem(&temp))
				break;
			if (temp[0] == '"')
			{
				strcpy(litem, temp + 1);
				litem[strlen(litem) - 1] = 0;
			}
			else
				strcpy(litem, temp);

			itp->Insert(litem, counter++);
		}

		current_animation->AddInterpolator(itp);
		if (current_animation->GetTimeline() && current_animation->GetInterpolator(INTERPOLATOR_VISIBLE))
			current_animation->GetTimeline()->AddTLRange();
	}
	return 0;

}

CSInterpolator * CSInterpolatorSound::CreateInstance(CSAnimation *ainst) 
{ 

	CSInterpolatorSound *i = new CSInterpolatorSound(ainst);
	i->SetInstancedInterpolator((CSInterpolator *)this);
	return ((CSInterpolator *)i);
}

CSInterpolator* CSInterpolatorSound::Clone()
{
	CSInterpolator* pNewInterpolator = NULL;
	pNewInterpolator = new CSInterpolatorColor();
	if(!pNewInterpolator)
		return pNewInterpolator;
	pNewInterpolator->SetInstancedInterpolator(NULL);
	for (int i=0; i< GetArrayLength(); i++)
	{
		CKeyframe* pNewKeyFrame = m_pArray[i]->Clone();
		pNewInterpolator->Append(pNewKeyFrame);
	}
	return pNewInterpolator;
}

void CSInterpolatorSound::GetFilePathName(char* cSoundFilePath)
{
	strcpy(cSoundFilePath,"");
	CSAnimation* pAnimation = GetAnimation();
	if(pAnimation)
	{
		CSBehaviorAction *pBehaviorAction = pAnimation->GetBehaviorAction();
		if(pBehaviorAction)
		{
			int nPos = 0;
			string sFilePath;	//XML文件Path
			sFilePath = SATools::w2c(pBehaviorAction->GetSimulationAnimationManager()->GetSAFilePath());
			nPos = (int)sFilePath.find_last_of("\\");
			sFilePath = sFilePath.substr(0,nPos+1);
			strcpy(cSoundFilePath,sFilePath.c_str());
			char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
			strcpy( plcIdPath, pAnimation->GetTarget()->GetResolvedPath());
			strcat(cSoundFilePath,plcIdPath);
		}
	}
}
void CSInterpolatorSound::Interpolate(int keyframe, float fraction)
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframeString **posarray  = (CKeyframeString **)GetArray();
	char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy( plcIdPath, GetAnimation()->GetTarget()->GetResolvedPath());

	bool PlayStopFlg = true;
	if ( m_pTarget == INVALID_KEY)
		SetTarget();
	char  res[SA_BUFFER_SIZE_BIG] = {0};
	//如果是最后一帧设置为0
	if (keyframe >= length-1)
	{
		strcpy(res, posarray[length-1]->GetTarget());
	}
	else 
	{
		strcpy(res, posarray[keyframe]->GetTarget());
	}

	if(strcmp(res,"stop") == 0)
	{
		PlayStopFlg = false;
	}
	else if(strcmp(res,"play") == 0)
	{
		PlayStopFlg = true;
	}
	
	//获得播放起始点
	float fTime = 0.0;
	CSAnimation* pAnimation = GetAnimation();
	if(pAnimation)
	{
		CSBehaviorAction *pBehaviorAction = pAnimation->GetBehaviorAction();
		if(pBehaviorAction)
		{
			//获得播放起始点
			int nTicksPerSecond = 10;	//帧/秒
			nTicksPerSecond = pBehaviorAction->GetTicksPerSecond();		

			if(pAnimation->GetFirstTick() > 0)	//从动画中间播放
			{
				fTime = ((pBehaviorAction->GetCurrentTick() - pAnimation->GetFirstTick()) / nTicksPerSecond) * 1000;
			}
			else
			{
				fTime = pBehaviorAction->GetCurrentTick() / nTicksPerSecond * 1000;
			}

			char cSoundFilePath[SA_BUFFER_SIZE_SMALL] = {0}; //声音文件Path
			GetFilePathName(cSoundFilePath);

			//播放声音...
			if(!m_pSound)
			{
				m_pSound = new CAnimationSound();
			}
			if(pBehaviorAction->IsPlaySound() && !pBehaviorAction->IsReversePlay())
			{
				//m_pSound->TransferSound(cSoundFilePath,plcIdPath,PlayStopFlg,fTime);
			}
			else
			{
				//m_pSound->TransferSound(cSoundFilePath,plcIdPath,false,fTime);
			}

		}
	}
}
//声音插值器定义End...

////////////////////////////////////////////////////////////////////////////////////////////////
//工具插值器定义
CSInterpolatorInstanceCreate::CSInterpolatorInstanceCreate(CSAnimation *animationinstance, const char *name)
: CSInterpolator(animationinstance, name)
{


}

CSInterpolatorInstanceCreate::~CSInterpolatorInstanceCreate()
{

}



const char * CSInterpolatorInstanceCreate::GetType() { return "InstanceCreate"; }


void CSInterpolatorInstanceCreate::Serialize(CUtilityXMLGenerator *xmlgen)
{
	CUtilityXMLTag xmlt;
	char tagname[SA_BUFFER_SIZE_SMALL];
	strcpy(tagname, GetType());
	strcat(tagname,"Interpolator");
	xmlt.SetTagname(tagname);
	xmlt.AddProperty("Name", m_Name, true);

	int length = GetArrayLength();
	if (length > 0)
	{
		CKeyframeString **array = (CKeyframeString **)GetArray();
		for (int i = 0; i < GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);
	}

	xmlgen->AddTag(&xmlt);
}

void CSInterpolatorInstanceCreate::DeciperString(CSInterpolatorInstanceCreate *itp, char *text, int &counter)
{
	char path2[4096];
	char path[4096];
	char include[4096];
	char include2[4096];
	char color[4096];
	char dummytype[SA_BUFFER_SIZE_SMALL];
	char *pathend = strstr(text,"$$");
	SACameraType dummy;
	if (!pathend)
		return;

	memcpy(path2, text, pathend - text);
	path2[pathend - text] = 0;
	text = pathend+2;
	pathend = strstr(text,"$$");
	if (!pathend)
		return;
	memcpy(include2, text, pathend - text);
	include2[pathend - text] = 0;
	text = pathend+2;
	strcpy(color, text);

	//STargetObject::ResolveTarget(path2, path, dummytype, dummy);
	//STargetObject::ResolveTarget(include2, include, dummytype, dummy);
	itp->Insert(path2, include2, color, counter++);
}


void *CSInterpolatorInstanceCreate::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL];

	if (open)
	{
		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		CSInterpolatorInstanceCreate *itp = new CSInterpolatorInstanceCreate(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
			char *temp;
			char litem[SA_BUFFER_SIZE_BIG];
			if (!xt->GetNextListItem(&temp))
				break;
			if (temp[0] == '"')
			{
				strcpy(litem, temp + 1);
				litem[strlen(litem) - 1] = 0;
			}
			else
				strcpy(litem, temp);

			DeciperString(itp, litem, counter);
		}
        if (current_animation != NULL) {
            current_animation->AddInterpolator(itp);
        }
	}
	return 0;

}

CSInterpolator* CSInterpolatorInstanceCreate::Clone()
{
	CSInterpolator* pNewInterpolator = NULL;
	pNewInterpolator = new CSInterpolatorInstanceCreate();
	if(!pNewInterpolator)
		return pNewInterpolator;
	pNewInterpolator->SetInstancedInterpolator(NULL);
	for (int i=0; i< GetArrayLength(); i++)
	{
		CKeyframe* pNewKeyFrame = m_pArray[i]->Clone();
		pNewInterpolator->Append(pNewKeyFrame);
	}
	return pNewInterpolator;
}


CSInterpolator * CSInterpolatorInstanceCreate::CreateInstance(CSAnimation *ainst)
{
	CSInterpolatorInstanceCreate *i = new CSInterpolatorInstanceCreate(ainst);
	i->SetInstancedInterpolator((CSInterpolator *)this);
	return ((CSInterpolator *)i);
}


void CSInterpolatorInstanceCreate::Reset()
{
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframe3String **posarray  = (CKeyframe3String **)GetArray();
	//for (int i=0;i<length;i++)
	//{
	//	HC_Delete_Segment(posarray[i]->GetTarget1());
	//}
}

void CSInterpolatorInstanceCreate::Interpolate(int keyframe, float fraction)
{
	CSAnimation* pAnimation = GetAnimation();
	if (pAnimation == NULL)
		return;
	CSBehaviorAction *pBehaviorAction = pAnimation->GetBehaviorAction();
	if (pBehaviorAction == NULL)
		return;
	int length = GetArrayLength();
	if (length <= 0)
		return;
	CKeyframe3String **posarray  = (CKeyframe3String **)GetArray();

	char targetsegment[SA_BUFFER_SIZE_SMALL];
	char targetinclude[SA_BUFFER_SIZE_SMALL];
	char targetcolor[SA_BUFFER_SIZE_SMALL];

	if (keyframe >= length-1)
	{
		strcpy(targetsegment, posarray[length-1]->GetTarget1());
		strcpy(targetinclude, posarray[length-1]->GetTarget2());
		strcpy(targetcolor, posarray[length-1]->GetTarget3());
	}
	else
	{
		strcpy(targetsegment, posarray[keyframe]->GetTarget1());
		strcpy(targetinclude, posarray[keyframe]->GetTarget2());
		strcpy(targetcolor, posarray[keyframe]->GetTarget3());
	}
	int nCurTick = pAnimation->GetCurrentTick();
	if (nCurTick > pAnimation->GetFirstTick() && nCurTick < pAnimation->GetLastTick())
		pBehaviorAction->TransferTool(0, targetsegment, targetinclude, targetcolor);//显示工具件
	else
		pBehaviorAction->TransferTool(1, targetsegment, targetinclude, targetcolor);//隐藏工具件
}
