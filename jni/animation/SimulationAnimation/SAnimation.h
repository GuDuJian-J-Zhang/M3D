// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		SAnimation.h
*
*	@brief	动画类，包含一个时间轴和一个或多个插件器。
*
*	@par	历史:
*		
****************************************************************************/

#ifndef _SANIMATION_H
#define _SANIMATION_H

#ifdef USE_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "../SimulationCommon/SATools.h"
#include "KeyFrame.h"
#include "SADefine.h"
#include "STimeline.h"	//Add By YL 2014.10.23
#include "../SimulationCommon/varray.h"

SA_NAMESPACE_BEGIN
class CSBehaviorAction;
class STargetObject;
SA_NAMESPACE_END

class CSTimeline;
class CSInterpolator;
class CUtilityXMLGenerator;
class CUtilityXMLTag;
class CKeyframe;


using namespace NS_SimulationAnimation;

class SA_API CSAnimation 
{
public:
	/*!	Constructs an CSAnimation object.
		\param name Name of the animation. This name should be unique.
		\param BehaviorAction A pointer to CSBehaviorAction object that this animation belongs to.
		\param parentanimation A pointer to parent animation if it exists.  
		This value can be null if there is not parent animation. */
 	CSAnimation(const char *name, NS_SimulationAnimation::CSBehaviorAction  * BehaviorAction, CSAnimation *parentanimation = 0);

	/*! Destructor of the CSAnimation object. */  
 	virtual ~CSAnimation();   

	/*! \return The type of animation which is 'CSAnimation'. */
	virtual const char * GetType() { return "CSAnimation"; } 

	
	/*! \return Name of the animation. */  	
	const char * GetName() { return m_Name; }

 	/*! Sets the name of the animation. */
	void SetName(const char *name);

	/*! Performs animation based on the given time.  
		\param currenttime Time (in ticks) relative to start time of overall animation. */
  	virtual bool Animate(float currenttime);

	/*! Performs animation based on the given time in relation to given start time.  
		\param currenttime Time (in ticks) relative to given start time (second parameter)
		\param starttime Start time (in ticks) to evaluate animation from */
	virtual bool Animate(float currenttime, float starttime);

	/*! This method deletes a given keyframe in the timeline and interpolators for this animation.
		\param keyframe Number of keyframe
 	*/
	void DeleteKeyframe(int keyframe);

	/*! Copies the next or previous keyframe to the given target keyframe.
 		\param keyframe The keyframe to copy to.
		\param next Pass false for the previous keyframe and true for the next keyframe. */
	void DuplicateNextOrPrevious(int keyframe, bool next, bool bStep = false);

	/*! This method makes a copy of <i>newkeyframe</i> and then replaces <i>oldkeyframe</i> with the copy.
 		\param newkeyframe Keyframe to copy to
		\param oldkeyframe Keyframe to copy from */
	void Duplicate(int newkeyframe, int oldkeyframe, bool bAddTLRange = true);
	
	/*!	This method writes XML data to a buffer. */
	virtual void Serialize(CUtilityXMLGenerator *xmlgen, CUtilityXMLTag *xmlt);
	/*!	This method writes XML data to a buffer. */
	virtual void Serialize(CUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);
	static void *XMLCallback_V1(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

 	/*! \return A pointer to the CSBehaviorAction object associated with this animation. */  	
	NS_SimulationAnimation::CSBehaviorAction  * GetBehaviorAction() { return m_pBehaviorAction; }

 	/*! Sets the CSBehaviorAction Object associated to this animation. */  	
	void SetBehaviorAction(NS_SimulationAnimation::CSBehaviorAction  *behaviourmanager) { m_pBehaviorAction = behaviourmanager; }

 	/*! \return The value of first entry in the timeline. */
  	int GetFirstTick();

 	/*! \return The value of last entry in the timeline. */
	int GetLastTick();

 	/*! \return A value of 1 if the animation will be looped or 0 if the animation stops when it reaches the
	end of the timeline.*/
	int GetLoop(bool bSteps = false);

	/*! Use this method to indicate if animation is looped or plays once.
	\param loop Pass 1 if the animation should loop or 0 if the animation should stop when it reaches the end of
	the timeline.*/
	void SetLoop(int loop, bool bSteps=false);

	void SetLoopChild(int bLock, bool bSteps);
	/*! \return A pointer to the timeline associated to this animation. */
	CSTimeline *GetTimeline() { return m_pTimeline; }

	/*! This method associates the passed timeline with this animation. 
	\param timeline The timeline to be associated with this animation.*/
	void SetTimeline( CSTimeline *timeline);
 
	/*! In special cases, you may want an animation that references this animation without changing it. Use 
	this method to create such an animation.
		\return A pointer to newly created animation that refers to this animation. */
	CSAnimation * CreateInstance();

	/*! \return A pointer to the list of interpolators for this animation.*/
 	struct vlist_s * GetInterpolatorList() { return m_InterpolatorList; }

	/*! \return A pointer to the list of child animations associated to this animation.*/
	struct vlist_s * GetChildAnimationList() { return m_ChildAnimationList; }

	/*! \return The first interpolator in the interpolator list.*/
	CSInterpolator *GetInterpolator();
	CSInterpolator *GetInterpolator(const char *interpolatortype);

	/*! Adds an interpolator to the interpolator list.
	\param interpolator The interpolator to be added.*/
	void AddInterpolator(CSInterpolator *interpolator);

	/*! Adds an animation to the child animation list.
	\param childanimation The child animation to be added.*/
	void AddChildAnimation(CSAnimation *childanimation);
	void DelChildAnimation(CSAnimation *childanimation);

	/*! \return Parent animation for this animation. */
 	CSAnimation *GetParentAnimation() { return m_pParentAnimation; }
	void SetParentAnimation(CSAnimation *pParentAnimation) { m_pParentAnimation = pParentAnimation; }
 
  	/*! Sets initial delay time for this animation. A delay can be used to prevent this animation
	from occuring at exactly the same time as other items in the CSBehaviorAction's animation list.*/
	void SetDelay(int delay) { m_Delay = delay; }

  	/*! \return The initial delay time for this animation. */
	int	 GetDelay() { return m_Delay; }

	/*! \return A pointer to target object of this animation. */
	NS_SimulationAnimation::STargetObject * GetTarget() { return m_Target; } 

 	/*! \return A pointer to the animation that this animation references. */
	CSAnimation * GetInstancedAnimation() { return m_pInstancedAnimation; }

	/*! Sets pointer to animation that you want this animation to reference. 
	\param animation A pointer to the animation to reference.*/
	void SetInstancedAnimation(CSAnimation *animation) { m_pInstancedAnimation = animation; CloneAnimation(); }

	/*! Stop the animation if it is currently running and resets all the interpolators associated with this animation.  */
 	void Reset();

	void SetTargetByKey(long key);

	/*! This method creates a target object using the given target path and then attaches it to this
	animation.
		\param targetpath Segment path to target */  	
 	void SetTargetByPath(const char *targetpath);

	/*! This method creates a target object using the given target path and name. Then it attaches the
	newly created target to this animation.
		\param name Name of newly created target object
		\param targetpath Segment path to target */  	
 	void SetTargetByPath(const char *name, const char *targetpath);

 	/*! Associates a given target object with this animation.*/  	
 	void SetTarget(NS_SimulationAnimation::STargetObject *target);
 
	/*! For a given time, this method finds the closest keyframe and interval in the interpolator.
		\param time Time in which you are looking for the closest keyframe. 
		\param interval Returns the interval in timeline which the given time falls between.
		\param  keyframe Returns an Hkeyframe object for given time.
		\return True if a keyframe and interval were found and false if none was found.	*/
 	bool GetInterpolator(int time, int &interval, CKeyframe **keyframe);

 	/*! Sets default active parameter for this animation. If you pass false, then the animation is
	inactive by default which means that it can only be triggered by a sensor action.
		\param active Pass true if you want this animation to be active by default or false if otherwise. */	
 	void SetDefaultActive(bool active) { m_bDefaultActive = active; }

 	/*! \return True if the animation is active by default and false if otherwise. If the animation is not active by default
	then it can only become activated by a sensor or by calling SetDefaultActive and passing true.*/
	bool GetDefaultActive() { return m_bDefaultActive; }

 	/*! Sets the currently running flag which indicates whether an animation is currently executing.
		\param running Pass true to indicate that this animation is currently running and false if it is not running. */
 	void SetCurrentlyRunning(bool running) { m_bCurrentlyRunning = running; }

 	/*! \return True if the animation is running and false if it is not running */
	bool GetCurrentlyRunning() { return m_bCurrentlyRunning; }

 	/*! \return The current "tick" (i.e. frame) as reported by the behavior manager. */
	float GetCurrentTick(); 
	
 	/*! Adjusts keyframe intervals by the given number of ticks.
		\param keyframe Start keyframe to adjust.
		\param delta Amount of ticks to offset the keyframe time.
		\param doall Pass true to adjust all keyframes after the start keyframe. 
		\param relative Adjust keyframes all keyframes after the given keyframe so that they
		will also have an additional delta offset between the next keyframe. 
	*/
 	void AdjustKeyframe(int keyframe, int delta, bool doall = true, bool relative = false);
	
 	/*! This method set disassociates the current timeline from this animation and clears the interpolator list without
	deleting any of the previous associated timeline or interpolator objects.
 	*/
	void CleanWithoutDelete();


 	/*! \return True if the animation is flagged to only run for one update.*/
	bool ExecuteOnce() { return m_bExecuteOnce; }
 	/*! Use this method to indicate if this animation should only run for one update.
		\param once Pass true if you want the animation to be disabled after the next update 
		or false if you want to the animation to run normally.
 	*/
	void SetExecuteOnce(bool once) {m_bExecuteOnce = once; }

 	void Evaluate(float currenttick, bool &hasPos, AniPoint &pos, bool &hasQuat, AniQuat &quat, bool &hasScale, AniPoint &scale);
	//显示轨迹线
	void DrawTrochoid();
	//动画反转
	int Reversion(bool bStep = true);
	void ReversionTimeLineRange(VIntArray& keyFrameIndexArray);
	bool IsAnimationInterval(int iFirstFrame, int iSecondFrame);//判断两个关键帧中间是否有间隔
	bool IsAniIntervalByHKeyFrame(CSInterpolator * pInterpolator, int iFirstFrame, int iSecondFrame);
	//动画镜像
	int MirrorImageSingle(int nTick);
	int MirrorImage(bool bStep);
	int MirrorImageSelect(CTimeLineRange* pTimeLineRange);
	void ResetFirstLastTickByChild();
	bool IsExpanded(){ return m_bExpanded;};
	void SetExpanded(bool bExpanded){ m_bExpanded = bExpanded;};
	bool IsChild(CSAnimation* pAnimation,bool bSteps = true);
	void SetLockedStatus(bool bLocked, bool bSteps = true);
	void SetLockedStatusChild(bool bLock, bool bSteps);
	bool HasChildLocked(bool bSteps);
	bool IsLocked(bool bSteps = true);

	//////////////////////////////////////////////////////////////////////////
	//剖面的相关方法
	void SetClipPos(AniPoint& pos){m_clipPos.x = pos.x; m_clipPos.y = pos.y; m_clipPos.z = pos.z;}
	AniPoint GetClipPos(){return m_clipPos;}
	//////////////////////////////////////////////////////////////////////////
	//图片相关变量
	void SetImgPos(AniPoint& pos){m_imgPos.x = pos.x; m_imgPos.y = pos.y; m_imgPos.z = pos.z;}
	AniPoint GetImgPos(){return m_imgPos;}
	void SetImgScale(AniPoint& scale){m_imgScale.x = scale.x; m_imgScale.y = scale.y; m_imgScale.z = scale.z;}
	AniPoint GetImgScale(){return m_imgScale;}
	void AddHKeyFrame(bool bCopyPrevious=false, bool bLinear=true);

	//根据偏移量创建动画
	CSAnimation *CreatOffsetAnimation(AniPoint localPos,AniQuat localQuat,AniPoint localScale,AniPoint offsetPos,AniQuat offsetQuat,AniPoint offsetScale,NS_SimulationAnimation::STargetObject *target);
	CSAnimation *GetGroupAnimation(){return m_pGroupAnimation;}
	void SetGroupAnimation(CSAnimation *pGroupAnimation){m_pGroupAnimation=pGroupAnimation;}
	//克隆
	CSAnimation* Clone(CSBehaviorAction* pBehaviorAction);
	//复制关键帧到指定位置
	bool CopyKeyFrame(int srcTickIndex, int targetTick);
	//是否要分割
	bool IsCanSlice();
	bool Slice();

	bool GetCollisionFlg();
	void SetCollisionFlg(bool bCollisionFlg);
protected:

 	/*! This is a helper function for Animate().  It checks that the current
	tick is valid for the associated timeline and then animates the interpolators.
		\param currenttick Current Time in ticks of animation */
	virtual bool AnimateInternal(float currenttick);

 	/*! Creates an animation instance. */
	void CloneAnimation();

	/*!	This method writes XML Data for this animation's target to a buffer. */
 	virtual void SerializeTarget(CUtilityXMLTag *xmlt);

	CSAnimation*			m_pInstancedAnimation; 				/*!< A pointer to the referenced animation (if any). */
	NS_SimulationAnimation::STargetObject*		m_Target;							/*!< A pointer to the target object of this animation.*/
	bool					m_bCurrentlyRunning;				/*!< The value is true if the animation is currently running and false if otherwise.*/
	bool					m_bDefaultActive;					/*!< The value is true if the animation is active by default and false if otherwise. */
	NS_SimulationAnimation::CSBehaviorAction *	m_pBehaviorAction;					/*!< A pointer to the BehaviorAction object that this animation belongs to. */	
 	CSAnimation*			m_pParentAnimation;					/*!< A pointer to the parent animation. */	
	CSTimeline*			m_pTimeline;						/*!< A pointer to the timeline object. */
	struct vlist_s*			m_InterpolatorList;					/*!< A pointer to the interpolator list. */
	struct vlist_s*			m_ChildAnimationList;				/*!< A pointer to the list of child animations.*/
 	char					m_Name[SA_BUFFER_SIZE_SMALL];		/*!< Name of this animation. */
 	int m_Loop; /*!< A value of 1 means that this animation should be looped while a value of 0 means the animation should stop when it reaches the end of the timeline. */
	int m_Delay;	/*!< Initial delay for this animation */
	bool				m_bExecuteOnce;			/*!<  A flag that determines if animation should only run for one update. */
	bool				m_bExpanded; //是否展开，显示下级动画
	bool				m_bLocked;//是否锁定

	//////////////////////////////////////////////////////////////////////////
	//剖面相关变量
	AniPoint				m_clipPos;//位置
	//////////////////////////////////////////////////////////////////////////
	//图片相关变量
	AniPoint m_imgPos;//图片位置
	AniPoint m_imgScale;//图片的缩放比例

	CSAnimation		*m_pGroupAnimation;
};

//SA_NAMESPACE_END

#ifdef USE_PACK_8
#pragma pack(pop)
#endif

#endif



