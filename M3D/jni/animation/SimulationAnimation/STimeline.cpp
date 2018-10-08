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

