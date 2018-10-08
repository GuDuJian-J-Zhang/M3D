// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		SBehaviorAction.h
*
*	@brief	活动管理器
*
*	@par	历史:
*
****************************************************************************/

#ifndef _SBEHAVIORMANAGER_H
#define _SBEHAVIORMANAGER_H

#ifdef USE_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "../SimulationCommon/SATools.h"
#include "TimerManager.h"
#include "KeyFrame.h"
#include "SADefine.h"
#include "../SimulationCommon/TypeDef.h"
#include "../SimulationCommon/varray.h"
#include "SimulationAnimationManager.h"
#include "Mutex.h"

//SA_NAMESPACE_BEGIN
namespace SVIEW{
class View;
}
using SVIEW::View;

class CSTimeline;
class CSInterpolator;
class CSAnimation;
class CUtilityXMLGenerator;
class CUtilityXMLTag;

#define DEFAULT_TPS 10

#define	LOCK_MUTEX(_m_)	EnterCriticalSection ((CRITICAL_SECTION *)_m_)
#define	UNLOCK_MUTEX(_m_) LeaveCriticalSection ((CRITICAL_SECTION *)_m_)



SA_NAMESPACE_BEGIN
class CSBehaviorAction;

//动画类型
enum AnimationType
{
	AT_PART,	//单个零件动画
	AT_GROUP,	//分组动画
};
class SA_API STargetObject
{
public:
	/*!
	Constructs an STargetObject object.
    \param behaviourmanager A pointer to CSBehaviorAction object that is associated with this target object.
    \param name The name of target object.
	\param path The target description.  You can use CreateTargetString to generate the path for this target object.
	\param pivot Sets the center of rotation 
  	*/
	STargetObject(CSBehaviorAction *behaviourmanager, const char *name, const char *path, AniPoint *pivot = 0);

 
 	/*! \return The name of this target object. */  	
	const char *GetName() { return m_Name; }

 	/*! \return The path associated with this target object. Aliases are not expanded. */  	
	const char *GetPath() { return m_Path; }
	void SetPath(const char* pPath){ if(strlen(pPath) < SA_BUFFER_SIZE_SMALL) strcpy(m_Path, pPath); }
 	/*! \return The key of target object if applicable. */  	
	long GetTargetKey();

	void SetTargetKey(long key) { m_key = key; }

 	/*! \return The path of target object with aliases fully expanded and no type descriptor.*/  	
	const char *GetResolvedPath() { return m_ResolvedPath; }

 	/*! \return Type of target object.  */  	
	const char *GetType() { return m_Type; }

	/*!
	Compares this target object with another one to determine equality.
 	\param target Pass a target descriptor string.  You can generate this string using the CreateTargetString method.
	\return False if they are not equal or true if they are equal.
 	*/
	bool IsEqual(const char *target);

 	/*! \return The camera type for this target object.*/  	
	SACameraType GetCameraType() { return m_CameraType; }

	/*!
	Given an unexpanded target descriptor, this method returns the corresponding SVIEW segment path, target type and 
	camera type for this target object.
 	\param in The target string to evaluate.
 	\param result Returns the expanded target string.
 	\param targettype Returns the type of target.
	\param ctype Returns the camera type.
  	*/
	static void ResolveTarget(const char *in, char *result, char *targettype, SACameraType &ctype);


	/*!
	This utitlity method combines a target type and path to one target into one string.  The generated name can be
	used as an identifier to pass to the method IsEqual() and the methods CSBehaviorAction::AddAnimation 
	and CSBehaviorAction::FindAnimation to identify a target by a string.
  	\param type The type of the target.
  	\param path the path to the instance. 
	\param result Returns a pointer to the combined target string.
  	\param addquotes Pass true if target string should be enclosed by quotation marks or false if you don't want quotes.
  	*/	static void CreateTargetString(const char *type, const char *path, char *result, bool addquotes = false);

 	/*! Sets the center of rotation for this target object.
	\param pv The pivot point.
	*/  	
 	void SetPivot(AniPoint pv) { m_Pivot = pv; }

 	/*! 
	This method allows you to indicate if you want this object to be evaluated for collisions. Any collision between this object and the environment will trigger a 
	HAnimationListener::ObjectCollisionEvent event, which can in turn be received by any class deriving from 
	HAnimationListener.  This allows an application to be aware of inter-object collisions during an animation
	and provide visual feedback on the (typically by highlighting the collided objects).
	\param collision Pass true if you want this object to be evaluated for collision.
	*/  	
 	void SetCollision(bool collision) { m_bCollision = collision; }

	/*! \return Returns true if the target should be evaluated for collisions. */  	
 	bool GetCollision() { return m_bCollision; }

 	/*! Sets the center of rotation for the target object. 
	\param x The x coordinate position of the pivot point.
	\param y The y coordinate position of the pivot point.
	\param z The z coordinate position of the pivot point.
	*/  	
	void SetPivot(float x, float y, float z) { m_Pivot.Set(x,y,z); }
	void SetQuat(float x, float y, float z, float w) { m_Quat.Set(x,y,z,w); }

	void SetPrePivot(AniPoint pv) { m_PrePivot = pv; }
	void SetPrePivot(float x, float y, float z) { m_PrePivot.Set(x,y,z); }
	void SetPreQuat(float x, float y, float z, float w) { m_PreQuat.Set(x,y,z,w); }

 	/*! Returns the center of rotation for the target object.*/  	
	void GetPivot(AniPoint &pv) { pv = m_Pivot; }

 	/*! \return Center of rotation for target object. */  	
	AniPoint * GetPivot() { return &m_Pivot; }
	AniQuat * GetQuat() { return &m_Quat; }
	AniPoint * GetPrePivot() { return &m_PrePivot; }
	AniQuat * GetPreQuat() { return &m_PreQuat; }

	/*!
	Writes the XML data for this target object. 
	\param xmlgen A pointer to the xml generator that creates correct xml blocks
  	*/
	void Serialize(CUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);
	static void *XMLCallback_V1(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

 	/*! Call this method to indicate that this object has moved and should be checked for
	collision with other objects.*/  	
	void FlagForCollision() { m_bHasMoved = true; }
 	/*! Call this method indicates this object has not moved and does not need to be checked
	for collisions with other objects.*/  	
	void ResetForCollision() { m_bHasMoved = false; }
 	/*! \return True if the object has moved during a particular animation slice or false if it has not.*/  	
	bool HasMoved() { return m_bHasMoved; }
	void SetSerializeFromKey( bool sfk ) { m_bSerializeFromKey = sfk; }
	void GetInsPathAndPMIIdByResolvedPath(char* plcIdPath,char* pmiId);	 

	//--------------------------分组动画------------------------
	//Add By hhw,2016-04-07
	AnimationType GetTargetType(){return m_eTargetType;}
	void SetTargetType(AnimationType type){m_eTargetType=type;}
	//--------------------------分组动画------------------------

protected:
	//将之前动画版本转换为2.1的版本,将实例的配置路径从16进制改为10进制
	static void PlcPathToVersion2_1(const char* inPlcPath,char* outPlcPath);

 	char					m_Name[SA_BUFFER_SIZE_SMALL];				/*!< The name of this target object. */
	char					m_Path[SA_BUFFER_SIZE_SMALL];		/*!<  The unresolved path. */
	char					m_ResolvedPath[SA_BUFFER_SIZE_SMALL];/*!<  The resolved path. */ 
	char					m_Type[SA_BUFFER_SIZE_SMALL];				/*!<  The target type. */
	long					m_key;										/*!<  The key of target object. */
	CSBehaviorAction *	m_pBehaviorAction;							/*!<  A pointer to BehaviorAction object that this animation belongs to. */
	SACameraType			m_CameraType;								/*!<  The type of camera. */
	AniPoint					m_Pivot; 									/*!<  The center of rotation. */
	AniQuat					m_Quat; 									/*!<  The center of rotation. */
	bool					m_bCollision;								/*!<  A flag indicating if this object should be evaluated for collisions. */
	bool					m_bHasMoved;								/*!<  A flag indicating if object has moved during one animation slice. */
	bool m_bSerializeFromKey;

	/************************************************************************/
	/* add by likun 2014-11-04,当前帧前一关键帧的旋转点和旋转信息，
	用于将沿前一关键帧的旋转点旋转到单位矩阵时的位置转换沿当前旋转点旋转到单位矩阵的位置                  */
	/************************************************************************/
	AniPoint					m_PrePivot; 								/*!<  The center of rotation. */
	AniQuat					m_PreQuat; 									/*!<  The center of rotation. */

	//动画类型，零件动画或分组动画
	//Add By hhw,2016-04-07
	AnimationType			m_eTargetType;
};



//! The CSBehaviorAction class stores and manages all animation related data.
/*!	
	The CSBehaviorAction class stores a list of animations, sensors and target objects and 
	manages them on a frame by frame basis.
*/ 
class SA_API CSBehaviorAction : public CTimerClient
{
public:
	/*!
	Constructs an CSBehaviorAction object.
	\param tps The ticks per second value for measuring time.
	\param delay The initial delay for this CSBehaviorAction object.
    \param name The name of the behavior manager.
	\param version Additional Version Information.
 	\param company Additional Company Information.
   	*/

	CSBehaviorAction(int ID, int tps = 10, int delay = 0, const char *name = 0, const char *version = 0, const char *company = 0);
	virtual ~CSBehaviorAction();
	
	/*! 
	This method set the number of ticks that will occur per second. 
	\param tps The number of ticks per second. */
 	void  SetTicksPerSecond(int tps) { m_Tps =  tps; }
	void  SetTicksPerSecondByMultiple(float mul) { if(mul > 0.0f){m_Tps =  DEFAULT_TPS * mul;} }
 	/*! 
	\return The number of ticks per second. */
	int  GetTicksPerSecond();

	/*!
	This method writes XML data to a buffer.
	\param xmlgen A pointer to an XML generator that creates valid XML blocks.
  	*/
	void Serialize(CUtilityXMLGenerator *xmlgen, bool bHasParent = false);
 
	/*!
	This method schedules the given animation for immediate execution.
    \param animation A pointer to the animation you want to schedule for execution
	\param currenttime The start time for the animation in ticks.
   	*/	
	void ScheduleAnimation(CSAnimation* animation, float currenttime);
	/*!
	This method schedules the given animation considering group for immediate execution.
    \param animation A pointer to the animation you want to schedule for execution
	\param currenttime The start time for the animation in ticks.
   	*/	
	void ScheduleAnimationWithGroup(CSAnimation* animation, float currenttime);
	/*!
	This method creates a new animation based on given name, target, timeline and interpolator.  Then it adds 
	the new animation to the animation list.
    \param name The name of the new animation.
 	\param target A pointer to the target for the new animation.
	\param timeline A pointer to the timeline for the new animation.
	\param interpolator A pointer to the interpolator for the new animation.
	\return A pointer to the newly created animation. 
   	*/	
 	CSAnimation * AddAnimation(const char *name, const char *target , CSTimeline *timeline, CSInterpolator *interpolator);

	/*!
	This method adds a given animation to the animation list.
    \param animation Animation to add to animation list
   	*/	
	void AddAnimation(CSAnimation *animation);

	/*!
	The timer method that gets called from the CTimerManager::Tick() function.  This method updates the current time and 
	executes animations that are scheduled for the current time.
   	*/		
	virtual bool Tick( float request_time, float actual_time );

 	/*! \return A pointer to the current animation list.  Note this information is for read-only purpose.  To modify any information in
	animation list, please use the methods provided by this class.*/  	
	vlist_s * GetAnimationList() { return m_AnimationList; }

 
 	/*! This utility method finds a unique animation name.  This method only works 
	if you are using an incremented naming scheme.
	\return Unused int value indicating free name for a new animation.*/  	
	int GetFreeName();

 	/*! This method deletes all animations and associated objects */
  	void DeleteAllAnimations();

	/*!
	Searches for an animation that matches the given target string and interpolator type.
     \param target The target descriptor to search for in animation instance.  You can use 
	 STargetObject::CreateTargetSting() to generate this string.
     \param interpolatortype The interpolator type to search for in animation instance.
	 \return A pointer to the matching animation or null if no animation was found.
   	*/	
	CSAnimation * FindAnimation(const char *target, const char *interpolatortype,bool bFirstInterpolator = false);

	/*!
	Searches for an animation by name.
    \param name The name of the animation instance.
	 \return A pointer to the matching animation or null if no animation was found.
   	*/	
	CSAnimation * FindAnimationByName(const char *name);

	/*!
	
   	*/
	bool IsAnimation(const CSAnimation *pAnimation);

	/*!
	Places all animations in the scheduled animation list.
   	*/	
	void ScheduleAllAnimations(bool reset = false);

	/*!
	 Removes the given animation from the animation list and then deletes it.
     \param animation Pass a pointer to the animation you want to delete.
   	*/	
	bool DeleteAnimation(CSAnimation *animation);
	void DeleteAnimation(const char *target);
	
	/*!
	This method takes the given name and target and searches through the target object list.  If a target object 
	corresponding to the name and path exists, it returns that target object. If there are no matches, it creates a new
	one with the given name and path and adds it to the target object list.
	\param name The name of the new target object.
	\param path The path of the new target object.
	\return A newly created target object or an already existing target object.
   	*/		
	STargetObject * CreateTargetObjectByPath(const char *name, const char *path);

	/*!
	 This method adds the given target object to the target object list.
     \param targetobject The target object to added.
   	*/	
	void AddTargetObject(STargetObject *targetobject);

	/*!
	This method searches the target object list for target object by name.
     \param name The name of the target object.
     \return A pointer to the target object with the given name or null if no target.
	 object corresponding to the given name was found.
   	*/		
	STargetObject * FindTargetObjectByName(const char *name);

	/*!
	Searches for a target object given a path.
     \param path The path of the target object. 
     \return A pointer to the target object with the given name or null if no target object was found.
   	*/		
	STargetObject * FindTargetObjectByPath(const char *path);

	vlist_s * GetTargetObjectList() { return m_TargetObjectList; }
	/*!
	 Serializes all animation data to buffer and writes the buffer to a file.
     \param filename The filename you want to create.
   	*/	
	void WriteToFile(const __wchar_t *filename);
#ifdef _MSC_VER
	void WriteToFile(const unsigned short *filename);
#endif
	void WriteToFile(const char *filename);
//
//	/*!
//	 Reads XML data containing animation tags from a file and loads it into memory.
//     \param filename Filename
//   	*/	
//
	static CSBehaviorAction* ReadFromFile(const __wchar_t *filename);
#ifdef _MSC_VER
	static CSBehaviorAction* ReadFromFile(const unsigned short *filename);
#endif
	static CSBehaviorAction* ReadFromFile(const char *filename);
//
//	/*!
//	Sets up the XML parser to read animation related xml tags.
//     \param model A pointer to the model object.
//	 \param buffer A pointer to the memory buffer containing XML data.
//   	*/		
	static CSBehaviorAction* ProcessXMLData(const char *buffer);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

 	/*! 
	Sets the current tick based on a given time.
	\param t The time in seconds. */
 	void  SetCurrentTickByTime(float t);

 	/*! Set the current ticks.  If the animation is currently playing, it will make an adjustment
		to m_StartTime based on the difference between old and new tick values.
		\param tick The tick value. 	*/
	void  SetCurrentTick(float tick);

	 /*! Set current tick based on current time. */
	void  SetCurrentTick();

 	/*! \return The current tick value.*/  	
	float GetCurrentTick() { return m_CurrentTick; }	

	/*! \return The previous tick value.*/  	
	float GetPreTick() { return m_PreTick; }	

 	/*! Calculates the current tick based on a given percentage of total length of all animations.
		\param percentage  The percentage value used to calculate the current tick.*/	
	void SetCurrentTickByPercentage(float percentage); 

 	/*! \return The percentage value of the currrent tick in relation to the total length of all animations.*/  	
	float GetCurrentTickByPercentage();

	/*! Stops the execution of all animations and rewinds to the start. */	 	
	void Rewind();
	void RewindReverse();

 	/*! \return True if animations are currently playing or false if no animations are playing. */
	bool IsPlaying() { return m_bPlaying; }

	/*! \return The key to active scene.  This key will be used for camera manipulation. */	
	long GetActiveSceneKey();

	/*!  \return True if SVIEW will not interpolate between ticks and false if SVIEW will interpolate between ticks. */
  	bool GetRenderEveryTick() { return m_bRenderEveryFrame; }

	/*! Enables/disables interpolation between ticks.
    \param everytick Pass true if you do not want interpolation between ticks and false if you want to interpolate between ticks. */	
	void SetRenderEveryTick(bool everytick) { m_bRenderEveryFrame = everytick; }

    /*!  \return True if SVIEW will update camera during animation and false if SVIEW will not update camera. */
	bool GetUpdateCamera() { return m_bUpdateCamera; }

	/*! \return True if the current tick is greater than or equal to the last tick in the animation timeline.*/
	bool IsAtFinalTick();

	/*! Use this method to indicate if the camera should be updated during animations. 
		\param updatecamera Pass True to update camera or false if otherwise. */	
	void SetUpdateCamera(bool updatecamera) { m_bUpdateCamera = updatecamera; }

	/*! Given a tick number, this method finds the next closest keyframe.	
		\param ticknum The tick to calculate next keyframe from.
		\param forward Pass true to find next closest keyframe or false to find the previous closest keyframe. 
		\return The keyframe location in tick if a keyframe is found or the given ticknum if no keyframe was found.
		*/	
	int GetNextKeyframe(int ticknum, bool forward);

	/*! Removes the specified animation from the animation list.
		\param animation A pointer to the animation that you want removed. */	
	void RemoveScheduledAnimation(CSAnimation *animation);

    /*!  \return True if at least one animation exists or false if no animation exists. */
	bool HasAnimations();

    /*!  \return The last tick of all items in the animation list. */
	int GetFirstTick();
	int GetLastTick();
	int GetLastTickUnIncludeSound();
	int GetDefaultActiveAniFirstTick();
	int GetDefaultActiveAniLastTick();
	int GetAllAniLastTick();
	int GetAllAniFirstTick();


	/*! Executes all the scheduled animations.
		\param currenttick The current time in ticks.
		\param starttick The start time  in ticks.  If you pass -1, <i>currenttick</i> will be the start time. */	
	void ExecuteAnimations(float currenttick, float starttick);

	/*! Enables/disables continuous playing of the animation list.  In other words, if continuous play is on, 
	when the last tick is reached, everything is rewound and starts playing from the beginning again.  Note
	that if infinite play is active, it will override the continuous play behavior.
		\param cont Pass true to enable continuous play or false to play animations once. */		
	void SetContinuousPlay(bool cont) { m_bContinuousPlay = cont; }

	/*! Enables/disables infinite play.  If infinite play is active, then once the animation begins playing,
	it will continue to play pass the last tick of the timeline.  This means that animations that are looped 
	will continue to play while animations that are not looped will stop.  Note that if inifinite play is active,
	it will override continuous play.
		\param inf Pass true to set infinite play to active or false to set infinite play to inactive.*/		
 	void SetInfinitePlay(bool inf) { m_bInfinitePlay = inf; }
	
	/*! \return True if the animation list loops continuously and false if the animation 
	will only play through once and then stop. Note that if infinite play is active, it will override 
	the continuous play behavior.*/		
	bool GetContinuousPlay() { return m_bContinuousPlay; }
	
	/*! Use this method to indicate that camera was updated during animation. */		
	void CameraUpdated() { m_bCameraUpdated = true; }

	/*! \return True if camera has been changed during animation and false if the camera has not been changed. */		
	bool GetCameraUpdated() { return m_bCameraUpdated; }

	/*! Starts the execution of items on the animation list, registers with timer and starts playing at the 
	start time.*/	
	void Play();

	/*! Pauses the execution of the items on the scheduled animation list. */	
  	void Stop(bool bPlaybackIsInterrupted = false);

	void PlaySoundSA(bool bPlay = true);

	/*! Reactivates the execution of items on the schedule animation list.*/	
  	void Continue();
	void ContinueReverse();
	
	/*! Resets all animations. */	
	void Reset();

	/*! This method deactivates all the items in the animation list. If an animation is deactivated, it can only be triggered by
	a sensor.*/	
	void DeactivateAllAnimations();
	/*! This method activates all items on the animation list.  When animations are not active, they can only be 
	triggered by sensors.*/	
	void ActivateAllAnimations();
	/*! This method activates an animation given its partial or full name.  When an 
	animation is deactivated, it can only be triggered by a sensor. 
		\param name The name of the animation.
		\param AllowPartial Pass true if you want to allow a partial name to be used for determining animation. */
	void ActivateAnimationByName(const char *name, bool AllowPartial = true);
	/*! This method deactivates an animation given its full or partial name.  When an animation is deactivated, it can only
	be triggered by a sensor.
		\param name The name of the animation.
		\param AllowPartial Pass true if you want to allow a partial name to be used for determining animation. */
	void DeactivateAnimationByName(const char *name, bool AllowPartial = true);

	
	/*! Sets the animation start time.  The CSBehaviorAction will use this time as a referenced start time for all its
	calculations as it iterates through the timeline's array of keyframes.
		\param time The start time of the animation. */
 	void SetStartTime(float time) { m_StartTime = time; }

	/*! This method creates a new named animation.
 		\param name The name of the new animation
		\param targetpath Segment Path specifying the segment the animation should operate on
		\param pivot Center Point of the target (allows for off-center rotation)
		\return Pointer to newly created Animation Object */
 	CSAnimation * AddAnimation(const char *name, const char *targetpath, AniPoint *pivot);
 	
	void SetReversePlay(bool bReversePlay) {m_bReversePlay = bReversePlay;};
	bool IsReversePlay() {return m_bReversePlay;};
	int		GetID() {return m_ID;};
	void	SetName(const char* strName);
	const char* GetName(){return m_Name;};
	bool Transfer(INT type,const char* plcIdPath, const float fPivot[3],const float fMtxAA[4][4]);
	bool TransferVisible(const char* plcIdPath, const char* name,bool bOnOff, float fTran);
	bool TransferColor(const char* plcIdPath, const char* name,const float fColor[3]);
	bool TransferCamera(const char* plcIdPath,INT TrfType, const float fPivot[3],const float fMtxAA[4][4],INT iCamType);
	bool TransferSound(const char* plcIdPath,bool bPlayFlg,float fTime);
	bool TransferImage(const char* plcIdPath, const char* name, const float fImgPos[3], const float fImgScaleFactor[3], bool bOnOffFlg);
	bool TransferClipPlane(INT type,const char* plcIdPath, const char* name, 
		const float fNormal[3], const float fPos[3], bool bOnOff);
	void Reference();
	void Release();
	bool IsPlaybackInterrupted(){return m_bPlaybackIsInterrupted;};
	bool IsAtStartTick();
	
	char* GetVersion(){return m_Version;};
	STargetObject* GetTargetObjectByKey(long key);
	long RegisterTargetObjectKey();
	void SetPlayRangeSign(bool bSign) {m_bPlayRangeSign = bSign;}
	bool IsSetPlayRangeSign(){return m_bPlayRangeSign;}
	void SetPlayBeginTick(float fBeginTick){m_fPlayBeginTick = fBeginTick;}
	void SetPlayEndTick(float fEndTick){m_fPlayEndTick = fEndTick;}
	float GetPlayBeginTick(){return m_fPlayBeginTick;}
	float GetPlayEndTick(){return m_fPlayEndTick;}

	//void SetAnimationPlayFun(AnimationPlayFun *animationPlayFun);
	//AnimationPlayFun * GetAnimationPlayFun();
	//void SetCameraPlay(bool bCameraPlay) {m_bCameraPlay = bCameraPlay;};
	//bool IsCameraPlay() {return m_bCameraPlay;};
	//void SetShowTrochoid(bool bShowTrochoid){m_bShowTrochoid = bShowTrochoid;}
	//bool IsShowTrochoid(){return m_bShowTrochoid;}
	//void SetPlaySoundStatus(bool bPlaySound){m_bPlaySound = bPlaySound;}
	//bool IsPlaySound(){return m_bPlaySound;}
	//void SetPlayPosRotStatus(bool bPlayPosRot){m_bPlayPosRot = bPlayPosRot;}
	//bool IsPlayPosRot(){return m_bPlayPosRot;}
	//void SetPlayVisibleStatus(bool bPlayVisible){m_bPlayVisible = bPlayVisible;}
	//bool IsPlayVisible(){return m_bPlayVisible;}
	//void SetPlayPicStatus(bool bPlayImage){m_bPlayImage = bPlayImage;}
	//bool IsPlayImage(){return m_bPlayImage;}
	//void SetPlayClipStatus(bool bPlayClip){m_bPlayClip = bPlayClip;}
	//bool IsPlayClip(){return m_bPlayClip;}
	//void SetPlayColorStatus(bool bPlayColor){m_bPlayColor = bPlayColor;}
	//bool IsPlayColor(){return m_bPlayColor;}
	//void SetPlayCollisionStatus(bool bCollision){m_bCollisionCheck = bCollision;}
	//bool IsPlayCollision(){return m_bCollisionCheck;}
	//void SetCollisionDelay(int delay){m_CollisionDelay = delay;}
	//int GetCollisionDelay(){return m_CollisionDelay;}
	//bool IsStopPlayAfterCollision(){return m_bStopPlayAfterCollision;}
	//void SetStopPlayAfterCollision(bool bStopPlayAfterCollision){m_bStopPlayAfterCollision = bStopPlayAfterCollision;}
	////清除非动画中的剖面
	//bool IsClearClipPlane(){return m_bClearClipPlane;}
	//void SetClearClipPlaneStatus(bool bClearClipPlane){m_bClearClipPlane = bClearClipPlane;}

	AnimationPlayApi * GetAnimationPlayApi(){return m_pSimulationAnimationManager->GetAnimationPlayApi();};
	bool IsCameraPlay() {return m_pSimulationAnimationManager->IsCameraPlay();};
	bool IsShowTrochoid(){return m_pSimulationAnimationManager->IsShowTrochoid();}
	bool IsPlaySound(){return m_pSimulationAnimationManager->IsPlaySound();}
	bool IsPlayPosRot(){return m_pSimulationAnimationManager->IsPlayPosRot();}
	bool IsPlayVisible(){return m_pSimulationAnimationManager->IsPlayVisible();}
	bool IsPlayImage(){return m_pSimulationAnimationManager->IsPlayImage();}
	bool IsPlayClip(){return m_pSimulationAnimationManager->IsPlayClip();}
	bool IsPlayColor(){return m_pSimulationAnimationManager->IsPlayColor();}
	bool IsPlayCollision(){return m_pSimulationAnimationManager->IsPlayCollision();}
	int GetCollisionDelay(){return m_pSimulationAnimationManager->GetCollisionDelay();}
	bool IsStopPlayAfterCollision(){return m_pSimulationAnimationManager->IsStopPlayAfterCollision();}
	//清除非动画中的剖面
	bool IsClearClipPlane(){return m_pSimulationAnimationManager->IsClearClipPlane();}

	CSimulationAnimationManager* GetSimulationAnimationManager() {return m_pSimulationAnimationManager;};
	void SetSimulationAnimationManager(CSimulationAnimationManager*pSimulationAnimationManager) {m_pSimulationAnimationManager = pSimulationAnimationManager;};
	//获取动画播放初始状态的摄像机信息
	bool GetPlayBeginCamera(float *matrix,float* scale);
	bool HasLockedAnimations();
	bool IsAllAnimationsLocked();
	bool TransferTool(int nType, const char* plcIdPath, const char* strToolPath, const char* strParentPath);
	View* GetView();
protected:

	/*! Determine if any collisions have occurred for specified target object.
		\param tob A pointer to the target object. 
	*/
	bool EvaluateCollision(STargetObject *tob);
	

	char				m_Name[SA_BUFFER_SIZE_SMALL];				/*!< The name of the behavior manager object.*/	
	char				m_Version[SA_BUFFER_SIZE_SMALL];				/*!< The version string. */	
	char				m_Company[SA_BUFFER_SIZE_SMALL];				/*!< The additional company information.*/
	int					m_Tps;						/*!< The number of ticks per second.*/
	int					m_Delay;					/*!< Initial execution delay for this behaviour block*/
	float				m_StartTime;				/*!< Start of execution.*/
 	float				m_CurrentTick;				/*!< The current time.*/
	float				m_PreTick;					/*!< The Previous Tick,used for check is frameskip.*/
 	int					m_LastTick;					/*!< Final tick of all animations.*/
	int					m_FirstTick;					/*!< Start tick of all animations.*/

 	struct vlist_s *	m_AnimationList;			/*!< The list of all animations.*/
  	struct vlist_s *	m_ScheduledAnimationList;	/*!< The list of animations scheduled for execution.*/
	struct vlist_s *	m_TargetObjectList;			/*!< The list of all target objects.*/
	struct vlist_s *	m_SensorList;				/*!< The list of all sensors.*/

	bool				m_bPlaying;					/*!< The value is true if animations are currently playing or false if no animation is playing.*/
	bool				m_bRenderEveryFrame;		/*!< If true, SVIEW will display every frame of animations.  If false, SVIEW will display animations with interpolation.*/
	bool				m_bUpdateCamera;			/*!< The value is true if camera manipulations will be performed or false to not execute camera manipulations*/
	bool				m_bContinuousPlay;			/*!< The value is true if the animation list will rewound and played agained whenever the timeline reaches the end or false if the animation list will be played through once and stops at the end.*/
	bool				m_bCameraUpdated;			/*!< The value is true if camera has been changed during animation, false if camera has not been changed.*/	
	bool				m_bInfinitePlay;			/*!< The flag that determines if animation should run infinitely.  In otherwords, the ticks continue to increment.*/
	bool				m_bReversePlay;
	//char				m_AssemblyPath[MAX_PATH];
	int					m_ID;
	UINT  m_ReferenceCount;
	bool				m_bPlaybackIsInterrupted;
	
	bool				m_bPlayRangeSign;
	float				m_fPlayBeginTick;
	float				m_fPlayEndTick;
	float				m_fCollisionTime;//最后一次计算干涉的时间

    
	mutable Mutex m_mutex;
    
    void Lock();
    void UnLock();
	//bool				m_bCameraPlay;
	//bool				m_bShowTrochoid;
	//bool				m_bPlaySound;//声音
	//bool				m_bPlayImage;//图片
	//bool				m_bPlayPosRot;//位置
	//bool				m_bPlayVisible;//显隐
	//bool				m_bPlayClip;//剖切
	//bool				m_bPlayColor;//颜色
	//bool				m_bCollisionCheck;//是否启用干涉检查
	//int					m_CollisionDelay;//干涉计算间隔时间(毫秒)
	//
	//bool				m_bStopPlayAfterCollision;//是否启用干涉检查
	//bool				m_bClearClipPlane;//是否清除非动画中的剖面
	//AnimationPlayApi * m_pAnimationPlayApi;

	CSimulationAnimationManager* m_pSimulationAnimationManager;

	
};

SA_NAMESPACE_END

#ifdef USE_PACK_8
#pragma pack(pop)
#endif

#ifndef USER_MSG_PLAYANIMATION
#define USER_MSG_PLAYANIMATION WM_USER+107
#endif
#ifndef USER_MSG_UPDATEVIEW
#define USER_MSG_UPDATEVIEW WM_USER+108
#endif

#endif



