// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		STimeLine.h
*
*	@brief	时间轴定义文件，一个动画中包含一个时间轴
*			时间轴包含一个int类型的关键帧位置数组，该数组记录动画中的动画个关键帧所在的帧位置，数据的大小与所有插值器中关键帧数据大小相同。
*
*	@par	历史:
*
****************************************************************************/

#ifndef _STIMELINE_H
#define _STIMELINE_H

#ifdef USE_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "../SimulationCommon/SATools.h"
#include "../SimulationCommon/varray.h"
#include "SADefine.h"

//SA_NAMESPACE_BEGIN
class CUtilityXMLGenerator;
class CUtilityXMLTag;
class CSAnimation;
class CSTimeline;
 

#ifdef WINDOWS_SYSTEM
template class VArray< int >;
#endif


class SA_API CSTimeline 
{
public:
	CSTimeline(CSAnimation *animation = 0);
 	virtual ~CSTimeline() { };

	/*****************************************************************
	函数名	：SetTimeline
	功能	：更新时间线中关键帧位置内容
	参数	：
			timeline，关键帧位置数组
			length，关键帧的个数
	返回值	：
	更新历史：
	
	*****************************************************************/
	void SetTimeline(const int *timeline, int length);

	/*!
	Sets the animation that this timeline is associated with.
	\param animation Pointer to Animation
 	*/	
	void  SetAnimation(CSAnimation *animation) { m_pAnimation = animation; }

	/*!
	In special cases, you might want to reference this timeline without changing it.  To create a new timeline
	that references this timeline, use this method and pass a pointer to the animation you want associated with
	the new timeline.
 	\param animation Animation that the created timeline instance should reference
 	\return The newly created timeline instance.
 	*/
 	CSTimeline * CreateInstance(CSAnimation *animation);

	/*! \return The timeline array which contains a list of the keyframe times.  Note this is for read-only purposes.  If you want to modify the timeline,
	use the methods provided by this class.*/
	int * GetTimelineArray();

	/*! \return The length of the timeline array.  This is the number of keyframes in the timeline. */
	int GetTimelineArrayLength() { return m_pTimelineArray.Count(); }

	/*!
	Adds a new keyframe to the timeline.
	\param t The keyframe time in ticks.
	\param doesExist Pass by reference. Returns true if a keyframe time already exists in the timeline.
  	\return The number of the newly created keyframe.
 	*/
	int AddKeyframe(int t, bool &doesExist);
	/*!
	Adds a new keyframe to the timeline.
	\param t The keyframe time in ticks.
   	\return  The number of the newly created keyframe.
 	*/
	int AddKeyframe(int t);
	/*!
	This method deletes the keyframe associated with the given time <i>t</i> from the timeline.
	\param t Time in ticks of the keyframe to delete.
   	\return  The position of the deleted keyframe or -1 if no keyframe was found and deleted.
 	*/
	int DeleteKeyframe(int t);
	void DeleteAllKeyframe(){ m_pTimelineArray.TrimSize(0);};
	/*!
	This method searches for keyframe based the given time.
 	\param t Time in ticks of the keyframe to be searched for.
   	\return  The position of the keyframe associated with the given time or -1 if no keyframe could be found.
 	*/
	int CheckKeyframe(int t);

	/*!
	Write XML Data to buffer
  	*/
	void Serialize(CUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*! \return The last time value of keyframe time array. */
	int GetLastTick();

	/*!
	This method find the keyframe interval that is closet to the current time.
	\param currentTick The time that we are searching for the closest interval.
	\param interval Returns interval that is closet to the current time.
	\return True if the currentTick is inside the timeline's range and false if the currentTick is 
	outside the timeline's range.
  	*/
 	bool GetTimelineInterval(int currentTick, int &interval);

	/*!
	Set the start time for the timeline. All keyframes are evaluated based on the start time.
 	\param starttick Start Tick
   	*/
	void SetStartTick(float starttick) { m_StartTick = starttick; }

	/*! \return The start time for this time line. */
	float GetStartTick() { return m_StartTick; }

	/*!
	Calculates the keyframe position and fraction value based on the current time.
	\param currentTick Current Time int tps
	\param interval Returns the keyframe interval which was closest to the currentTick.
	\param fraction Returns fraction to next keyframe which will be a value between 0 and 1.
	\return True if a valid keyframe interval was found and false if no valid keyframe was found.
   	*/
	virtual bool Evaluate (float currentTick, int &interval, float &fraction);

	/*! \return A pointer to the associated animation object. */
	CSAnimation *GetAnimation() { return m_pAnimation; }

	/*! \return The current tick for this timeline. */
	float GetCurrentTick() { return m_CurrentTick; }

	/*!
	Sets the timeline instance.
	\param timeline Instanced Timeline
 	*/	
	void SetInstancedTimeline(CSTimeline *timeline) { m_pInstancedTimeline = timeline; }
	


	/*! \return Current tick relative to the properties in this timeline.  This value takes into account
	start time and looping. */
	float GetCurrentRelativeTick() { return m_CurrentRelativeTick; }

	/*!
	Sets current relative time position.
  	\param tick Relative Time Position
   	*/
	void SetCurrentRelativeTick(float tick) { m_CurrentRelativeTick = tick; }
	void SetCheckRelativeFrame(bool bCheckRelativeFrame = true) {m_bCheckRelativeFrame = bCheckRelativeFrame;}


protected:	
	/*!
	Add keyframe interval to timeline.  This method is for internal use.
	\param t Position where to add interval.
	\param doesExist Returns true if interval already exist or false if interval does not exist.
	\return Position of the newly inserted interval.
 	*/	
	int AddKeyframeInternal(int t, bool *doesExist);	
	/*!
	For the given tick, this method returns a tick that has been adjusted to for the timeline.
	It takes into account factors such as start time and if the animation is looped.  
	\param tick The time in ticks to be adjusted.
	\return The newly adjusted time.
   	*/
	float					AdjustTickToTimeline(float tick);
	VArray< int >			m_pTimelineArray;				//!< This is for internal use only.	

 	CSAnimation*			m_pAnimation;					//!< This is for internal use only.			
	CSTimeline *			m_pInstancedTimeline;			//!< This is for internal use only.		

	float					m_StartTick;					//!< This is for internal use only.			
 	float					m_CurrentTick;					//!< This is for internal use only.			
	float					m_CurrentRelativeTick;				//!< This is for internal use only.	
	bool					m_bCheckRelativeFrame;
};

//SA_NAMESPACE_END

#ifdef USE_PACK_8
#pragma pack(pop)
#endif

#endif



