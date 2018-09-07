// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		STimeLine.cpp
*
*	@brief	时间轴定义文件
*
*	@par	历史:
*
****************************************************************************/

#include "stdafx.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "STimeline.h"
#include "SAnimation.h"
#include "../SimulationCommon/UtilityXMLParser.h"
#include "SBehaviorAction.h"

//SA_NAMESPACE_BEGIN


CSTimeline::CSTimeline(CSAnimation *animation)
{
	m_pAnimation = animation;
	m_pTimelineArray.SetCount(0);
	m_pInstancedTimeline = 0; 
 	m_CurrentTick = 0;
	m_StartTick = 0;
	m_CurrentRelativeTick = 0;
	m_bCheckRelativeFrame = true;
	m_pTLRangeArray.TrimSize(0);
}

 
void CSTimeline::SetTimeline(const int *timeline, int length)
{
//	const int i = m_pTimelineArray2[2];
 	m_pTimelineArray.SetCount(0);
 
	m_pTimelineArray.AppendArray(timeline, length);
}

int CSTimeline::CheckKeyframe(int t)
{
	for (int i=0;i<GetTimelineArrayLength();i++)
	{
		if (m_pTimelineArray[i] == t)
			return i;
	}
	return -1;
}
int CSTimeline::AddKeyframeInternal(int t, bool *doesExist)
{
	if (doesExist)
 		*doesExist = false;

	// Need to check that this binary search works in all cases,
	// not just when keyframes are already sorted
	int low;
	int high;
	int mid = 0;
	int tmp;
	int index = t;
	int orig_array_len = GetTimelineArrayLength();

	tmp = 0;				/* in case none found */
	low = 0;
	high = orig_array_len -1;
	if (high>=0)
	{	

		if (index <= m_pTimelineArray[high] && 
				index>= m_pTimelineArray[low]) {
			mid = low + (high - low)/2;

			while (high >= low) {
				int midindex = m_pTimelineArray[mid];

				if (midindex == index)	 {
					tmp = mid;
					if (doesExist)
						*doesExist = true;
					//return mid;
					break;
				}
				else if (midindex > index)
					high = mid - 1;
				else
					low = mid + 1;
				mid = low + (high - low)/2;
			}
		}
		else if (index>= m_pTimelineArray[high])
			mid = high + 1;
		else if (index<= m_pTimelineArray[low])
			mid = low;
	}
	else if (orig_array_len == 0){
		mid = 0;
	}

	if(doesExist && *doesExist)
		m_pTimelineArray.ReplaceAt(t, mid);
	else
		m_pTimelineArray.InsertAt(t, mid);	
	if(m_pAnimation && m_pAnimation->GetParentAnimation())
	{
		m_pAnimation->GetParentAnimation()->ResetFirstLastTickByChild();
	}
	
	return mid;
}

int CSTimeline::AddKeyframe(int t, bool &doesExist)
{
	return AddKeyframeInternal(t, &doesExist); 
}
int CSTimeline::AddKeyframe(int t)
{
	return AddKeyframeInternal(t, 0); 
}

	

int CSTimeline::DeleteKeyframe(int time)
{
	for (int i=0;i<GetTimelineArrayLength();i++)
	{
		if (m_pTimelineArray[i] == time)
		{
 				m_pTimelineArray.RemoveAt(i);
				if(m_pAnimation && m_pAnimation->GetParentAnimation())
				{
					m_pAnimation->GetParentAnimation()->ResetFirstLastTickByChild();
				}
				return i;
		}
	}
	return -1;
}

void CSTimeline::Adjust(int time, int delta, bool doall)
{
	if(m_pAnimation && m_pAnimation->IsLocked())
	{
		return;
	}
	for (int i=0;i<GetTimelineArrayLength();i++)
	{
		if (m_pTimelineArray[i] >= time)
		{
		    if (doall)
		    {		    
		    for (int j=i;j<GetTimelineArrayLength();j++)		    		    
			m_pTimelineArray[j]+=delta;
		    }
		    else
			m_pTimelineArray[i]+=delta;
		    break;
		}
		
	}

	if(m_pAnimation && m_pAnimation->GetParentAnimation())
	{
		m_pAnimation->GetParentAnimation()->ResetFirstLastTickByChild();
	}
}

void CSTimeline::AdjustRelative(int time, int delta)
{
 	for (int i=1;i<GetTimelineArrayLength();i++)
	{
		if (m_pTimelineArray[i] >= time)
		{
 		     Adjust(m_pTimelineArray[i],delta);

		    for (int j=i+1;j<GetTimelineArrayLength();j++)
		    {		   
			     //int d= m_pTimelineArray[j] - m_pTimelineArray[j-1];
			     //int delta = percent/100.0f * (float)d;
			     Adjust(m_pTimelineArray[j],delta);
		    }
		    break;
		}
		
	}
}


void CSTimeline::Serialize(CUtilityXMLGenerator *xmlgen)
{
 		CUtilityXMLTag xmlt;	 
		xmlt.SetTagname("Timeline"); 
		int *tarray = GetTimelineArray();

		for (int i=0; i< GetTimelineArrayLength(); i++)
		{
			char text[SA_BUFFER_SIZE_BIG];
			sprintf(text, "%d", tarray[i]);
			xmlt.AddListItem(text);
		}
   		xmlgen->AddTag(&xmlt);
}


void *CSTimeline::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{ 	
	if (open && NULL != xt->GetXMLParser()->GetStackData())
	{
 		CSAnimation *current_animation = (CSAnimation *)xt->GetXMLParser()->GetStackData();
		CSTimeline *itp = new CSTimeline(current_animation);
		current_animation->SetTimeline(itp);

		xt->StartListItemsQuery();
 		while (1)
		{
 			char *litem;
  			if (!xt->GetNextListItem(&litem))
				break;
			bool replace;
			itp->AddKeyframe(atoi(litem), replace);
		}
   		//current_animation->SetTimeline(itp);
		itp->AddTLRange();
	}
	return 0;
}

CSTimeline * CSTimeline::CreateInstance(CSAnimation *ainst) 
{ 
	
	CSTimeline *i = new CSTimeline(ainst);
	i->SetInstancedTimeline((CSTimeline *)this);
	return ((CSTimeline *)i);
}

int* CSTimeline::GetTimelineArray()
{
	if (m_pTimelineArray.Count() > 0)
		return &m_pTimelineArray[0];
	else
		return NULL;
}

int CSTimeline::GetLastTick()
{
	if (m_pTimelineArray.Count())
		return m_pTimelineArray[m_pTimelineArray.Count()-1];
	else return -1;
}

 


float  CSTimeline::AdjustTickToTimeline(float frame)
{
	int loop = GetAnimation()->GetLoop();

	int *timeline = GetTimelineArray();
	int length = GetTimelineArrayLength();

	frame = frame - m_StartTick;

	if (frame> timeline[length-1] && loop)
	{
		int frac = (int)(frame / timeline[length-1]);
		frame = frame - (frac * timeline[length-1]);
	}
	return frame;
}


bool CSTimeline::Evaluate(float currentFrame, int &interval, float &fraction)
{
	int i;
//	int loop = GetAnimation()->GetLoop();
	int length = GetTimelineArrayLength();
	if (length <= 0)
		return false;
	int *timeline = GetTimelineArray();
	
 	float relativeFrame = AdjustTickToTimeline(currentFrame);
	m_CurrentTick = currentFrame;


	int low;
	int high;
	int mid = 0;
	int tmp;

	tmp = 0;				/* in case none found */
	low = 0;
	high = length-1;

	//Edit by Roc
	
	if (m_bCheckRelativeFrame && relativeFrame<timeline[0])
		return false;
	// Need to check that this binary search works in all cases,
	// not just when the keyframes are already sorted
	if (relativeFrame <= (float)timeline[high] && 
			relativeFrame >= (float)timeline[low]) {
		mid = low + (high - low)/2;

		while (high >= low) {
			float midindex = (float)timeline[mid];

			if (midindex == relativeFrame)	 {
				tmp = mid;
				
				i = mid;
				break;
			}
			else if (midindex > relativeFrame)
				high = mid - 1;
			else
				low = mid + 1;
			mid = low + (high - low)/2;
		}
		i = mid;
	}
	else if (relativeFrame < timeline[low]){
		i = low+1;
	}
	else if (relativeFrame > timeline[high]){
		i = length;
	}
	else
		i = 0;
	
	

	if ( i == 0 ){
		if (length >= 1) //Edit by zyp 2012-8-24 如果只有一针也执行动画 原if (length >= 2)
			i = 1;
		else
			return false;
	}
	

	if (i == length)
	{
		interval = length;
		fraction = 1.0f;
		m_CurrentTick = timeline[length -1];
		return true;
	}

	float delta1 = (float)(timeline[i] - timeline[i-1]);

	fraction = 	(relativeFrame-timeline[i-1]) / delta1;
	interval = i - 1;
	m_CurrentRelativeTick = relativeFrame;
	return true;
}

CSTimeline* CSTimeline::Clone()
{
	CSTimeline *pNewTimeline=new CSTimeline;
	memcpy(pNewTimeline,this,sizeof(this));
	int nTimeCount=GetTimelineArrayLength();
	int *pNewTime=new int[nTimeCount];
	memcpy(pNewTime,GetTimelineArray(),sizeof(int)*nTimeCount);
	pNewTimeline->SetTimeline(pNewTime,nTimeCount);
	return pNewTimeline;
}

bool CSTimeline::GetTimelineInterval(int currentTime, int &interval)
{
	int *timeline = GetTimelineArray();
	int length = GetTimelineArrayLength();

	for (int i=0;i<length;i++)
	{
		if (currentTime>=timeline[i])
		{
			if (i>=length-1)
			{
				if (currentTime == timeline[i])
				{
					interval = i;
					return true;
				}
				else
					return false;
			}
			else
			{
				if (currentTime<timeline[i+1])
				{
					interval = i;
					return true;
				}
			}
		}
	}
	return false;
}
/************************************************************************/
/* 功能：添加某个动画中的所有时间线段
/* 参数：无
/* 返回值：无
/* 创建：2014-10-11 zhangcc
/************************************************************************/
void CSTimeline::AddTLRange()
{
	for(int i = 0; i< m_pTLRangeArray.Count(); i++)
	{
		delete m_pTLRangeArray[i];
	}
	m_pTLRangeArray.TrimSize(0);
	int iTimeLineArrayLength = GetTimelineArrayLength();
	CTimeLineRange* ptlRange = new CTimeLineRange(this);
	int i;
	for (i = 0; i < iTimeLineArrayLength - 1; i++)
	{
		ptlRange->AddKeyFrameIndex(i);
		if (m_pAnimation->IsAnimationInterval(i, i+1))
		{
			m_pTLRangeArray.Append(ptlRange);
			ptlRange = new CTimeLineRange(this);
		}
	}
	if (i == iTimeLineArrayLength - 1)
	{
		ptlRange->AddKeyFrameIndex(i);
		m_pTLRangeArray.Append(ptlRange);
	}
}
int CSTimeline::GetTLRangeByCurFrame(int iCurFrame, CTimeLineRange** pTLRange)
{
	int iTLRangeIndex = -1;
	for (int i = 0; i < m_pTLRangeArray.Count(); i++)
	{
		int beginIndex = m_pTLRangeArray[i]->GetBeginKeyFrameIndex();
		int endIndex = m_pTLRangeArray[i]->GetEndKeyFrameIndex();
		int beginFrame = m_pTimelineArray[beginIndex];
		int endFrame = m_pTimelineArray[endIndex];
		if (iCurFrame >=beginFrame && iCurFrame <= endFrame)
		{
			*pTLRange = m_pTLRangeArray[i];
			iTLRangeIndex = i;
			break;
		}
	}
	return iTLRangeIndex;
}
CTimeLineRange* CSTimeline::GetLastTLRange(int iCurTLRange)
{
	CTimeLineRange* pTLRange = NULL;
	if (iCurTLRange < m_pTLRangeArray.Count())
		pTLRange = m_pTLRangeArray[iCurTLRange - 1];
	return pTLRange;
}
CTimeLineRange* CSTimeline::GetNextTLRange(int iCurTLRange)
{
	CTimeLineRange* pTLRange = NULL;
	if (iCurTLRange < m_pTLRangeArray.Count() - 1)
		pTLRange = m_pTLRangeArray[iCurTLRange + 1];
	return pTLRange;
}
/************************************************************************/
/* 调整动画                                                                     */
/************************************************************************/
void CSTimeline::Adjust(int iFrameType, int delta, int iAniType, 
						  VIntArray& keyFrameIndexArray, VIntArray& keyFrameArray, VArray<CTimeLineRange*> tlRangeArray)
{
	if(m_pAnimation && m_pAnimation->IsLocked())
	{
		return;
	}
	if (iAniType == 0)
	{
		for (int i = 0; i < keyFrameIndexArray.Count(); i++)
		{
			int tmpIndex = keyFrameIndexArray[i];
			if (tmpIndex < keyFrameArray.Count() && tmpIndex < m_pTimelineArray.Count())
			{
				m_pTimelineArray[tmpIndex] = keyFrameArray[tmpIndex] + delta;
			}
		}
	}
	else if (iAniType == 1)
	{
		for (int i = 0; i < tlRangeArray.Count(); i++)
		{
			CTimeLineRange* pTLRange = tlRangeArray[i];
			if (pTLRange == NULL)
				continue;
			int iBeginIndex = pTLRange->GetBeginKeyFrameIndex();
			int iEndIndex = pTLRange->GetEndKeyFrameIndex();
			if (iBeginIndex >= keyFrameArray.Count() || iEndIndex >= keyFrameArray.Count())
				continue;
			
			if (iFrameType == 0 || iFrameType == 1)
			{
				bool bBegin = true;
				if (iFrameType == 1)
					bBegin = false;
				AdjustRelative(iBeginIndex, iEndIndex, bBegin, delta, keyFrameArray);
			}
			else
			{
				for (int idx = iBeginIndex; idx <= iEndIndex; idx++)
				{
					if (idx < keyFrameArray.Count() && idx < m_pTimelineArray.Count())
					{
						m_pTimelineArray[idx] = keyFrameArray[idx] + delta;
					}
				}
			}
		}
	}
	else
	{
		if (keyFrameArray.Count() > 0)
		{
			if (iFrameType == 0 || iFrameType == 1)
			{
				bool bBegin = true;
				if (iFrameType == 1)
					bBegin = false;
				AdjustRelative(0, keyFrameArray.Count() - 1, bBegin, delta, keyFrameArray);
			}
			else
			{
				for (int i = 0; i <= keyFrameArray.Count() - 1; i++)
				{
					if (i < keyFrameArray.Count() && i < m_pTimelineArray.Count())
					{
						m_pTimelineArray[i] = keyFrameArray[i] + delta;
					}
				}
			}
		}
	}
}
/************************************************************************/
/* 等比例进行调整  
/************************************************************************/
void CSTimeline::AdjustRelative(int iBeginIndex, int iEndIndex, bool bBegin, int delta, VIntArray& keyFrameArray)
{
	if (iBeginIndex < keyFrameArray.Count() && iEndIndex < keyFrameArray.Count())
	{
		int beginKeyFrame = keyFrameArray[iBeginIndex];
		int endKeyFrame = keyFrameArray[iEndIndex];
		int intervalFrame = endKeyFrame - beginKeyFrame;
		if (intervalFrame == 0)
			return;
		for (int i = iBeginIndex; i <= iEndIndex; i++)
		{
			int tmp = 0;
			if (bBegin)
				tmp = (endKeyFrame - keyFrameArray[i]) * delta / intervalFrame;
			else
				tmp = (keyFrameArray[i] - beginKeyFrame) * delta / intervalFrame;
			if (i < m_pTimelineArray.Count())
			{
				m_pTimelineArray[i] = keyFrameArray[i] + tmp;	
			}
		}
	}
}
//SA_NAMESPACE_END
//动画中时间段的相关类
CTimeLineRange::CTimeLineRange(CSTimeline* pTimeLine)
{
	m_tlKeyFrameIndexArray.TrimSize(0);
	m_pTimeLine = pTimeLine;
}
CTimeLineRange::~CTimeLineRange()
{
	Clear();
}

void CTimeLineRange::AddKeyFrameIndex(int iKeyFrameIndex)
{
	m_tlKeyFrameIndexArray.Append(iKeyFrameIndex);
}

void CTimeLineRange::Clear()
{
	m_tlKeyFrameIndexArray.TrimSize(0);
}

int* CTimeLineRange::GetKeyFrameIndexArray()
{
	if (m_tlKeyFrameIndexArray.Count() > 0)
		return &m_tlKeyFrameIndexArray[0];
	else
		return NULL;
}
int CTimeLineRange::GetBeginKeyFrameIndex()
{
	int iRet = -1;
	if (m_tlKeyFrameIndexArray.Count() > 0)
		iRet = m_tlKeyFrameIndexArray[0];
	return iRet;
}
int CTimeLineRange::GetEndKeyFrameIndex()
{
	int iRet = -1;
	int iCount = m_tlKeyFrameIndexArray.Count();
	if (iCount > 0)
		iRet = m_tlKeyFrameIndexArray[iCount - 1];
	return iRet;
}