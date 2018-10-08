#ifndef _H_SIMULATIONANIMATIONMANAGER_H
#define _H_SIMULATIONANIMATIONMANAGER_H
#include "stdafx.h"
#include "SADefine.h"
#include <vector>
#include "../SimulationCommon/SATools.h"
#include "../SimulationCommon/vlist.h"
#include "../SimulationAnimationPlay/AnimationPlayApi.h"

//SA_NAMESPACE_BEGIN
namespace SVIEW{
class View;
}
using SVIEW::View;

class CUtilityXMLGenerator;
class CUtilityXMLTag;
struct vlist_s;

SA_NAMESPACE_BEGIN
class CSBehaviorAction;
class CAnimationStepManager;
class CProcess;
class CProcessTargetObject;

class SA_API CSimulationAnimationManager
{
public:
	CSimulationAnimationManager(const char* CurSVLVersion = 0,const char* CurViewerVersion = 0);
	~CSimulationAnimationManager(void);
	/*! \return The type of CSimulationAnimationManager which is 'CSimulationAnimationManager'. */
	virtual const char * GetType() { return "CSimulationAnimationManager"; } 
	/*!
	Writes the XML data for this SimulationAnimation. 
	\param xmlgen: A pointer to the xml generator that creates correct xml blocks
  	*/
	void Serialize(CUtilityXMLGenerator *xmlgen);
	/*!
	Add a BehaviorAction to SimulationAnimation.
  	*/
	CSBehaviorAction * AddSimAni(int ID, int tps = 10, int delay = 0, const char *name = 0, const char *version = 0, const char *company = 0);
	void AddSimAni(CSBehaviorAction *pbBehaviorAction);
	/*!
	This method return A pointer to the list of BehaviorAction.
  	*/
	const vlist_s * GetBehaviorActionList() { return m_BehaviorActionList; };
	/*!
	 Removes the given CSBehaviorAction from the list of CSBehaviorAction and then deletes it.
     \param pbehaviorManager: Pass a pointer to the CSBehaviorAction you want to delete.
   	*/	
	void DeleteSimAni(CSBehaviorAction *pbehaviorManager);
	/*! This method deletes all CSBehaviorAction and associated objects */
	void DeleteAllSimAni();
	/*! This method find BehaviorAction by id */
	CSBehaviorAction * FindSimAniByID(const int ID);
	/*!
	This method process XML data.
  	*/
	void ProcessXMLData(const char *buffer);
	/*!
	This method convert Animation data from V1.1 to V2.1.
  	*/
	void VersionConvert_1_To_2_1();
	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);
	/*! This method set Current BehaviorAction by ID
	\param ID: BehaviorAction ID*/
	void SetCurSAByID(int SAID){m_CurSAID = SAID;};
	/*! This method get Current BehaviorAction ID.*/
	INT GetCurSAID(){return m_CurSAID;};
	/*! This method get current BehaviorAction.
	\return A pointer to CSBehaviorAction.*/
	CSBehaviorAction* GetCurrentSA();
	/*! This method get max BehaviorAction ID.*/
	int GetMaxID();
	/*! This method register a new BehaviorAction ID.
	\return The ID.*/
	long RegisterBehaviorActionID();
	void StartListSimAniQuery();
	bool GetNextListSimAni(CSBehaviorAction** opBehaviorAction);
	/*! This method check whether the list of CSBehaviorAction is empty. */
	bool HasListSimAni();
	/*! This method get CSBehaviorAction count.
	\return CSBehaviorAction count.*/
	int GetBehaviorActionCount();
	/*! This method get Animation version.*/
	char* GetVersion(){ return m_Version;};
	/*! This method get SVL version when save animation.*/
	char* GetSVLVersion(){ return m_SVLVersion;};
	/*! This method sets SVL version.*/
	void SetSVLVersion(char* SVLVersion){ if(strlen(SVLVersion) < SA_BUFFER_SIZE_SMALL) strcpy(m_SVLVersion,SVLVersion);};
	/*! This method get SView version when save animation.*/
	char* GetSViewVersion(){ return m_ViewerVersion;};
	/*! This method sets SView version.*/
	void SetSViewVersion(char* SViewVersion){ if(strlen(SViewVersion) < SA_BUFFER_SIZE_SMALL) strcpy(m_ViewerVersion,SViewVersion);};
	/*! Return Name of the SimulationAnimation. */
	const char* GetName(){return m_Name;};
	/*! Sets the Name of the SimulationAnimation. */ 
	void SetName(const char* name){if(NULL != name)	strcpy(m_Name, name);};
	/*!
	This method takes the given name and check whether CAnimationStepManager is empty.If is empty, it returns a pointer to CAnimationStepManager else it creates a new
	one with the given name and sets it to SimulationAnimation.
	\param name: The name of the new target object.
	\return A pointer to CAnimationStepManager.
   	*/	
	CAnimationStepManager * CreateAnimationStepManager( const char *name = 0);
	/*!
	This method return A pointer to CAnimationStepManager.
   	*/	
	CAnimationStepManager* GetAnimationStepManager(){return m_pAnimationStepManager;};
	/*!
	This method get current process.
	\return A pointer to Current Process.
   	*/
	CProcess* GetCurrentProcess();
	/*! This method check whether the list of target object has animation. */
	bool HasAnimations();

	/*! This method check is playing. */
	bool IsPlaying();
	/*! This method stop play. */
	void StopAll();
	/*!
	This method return A pointer to AnimationPlayApi object.
   	*/
	AnimationPlayApi * GetAnimationPlayApi(){return m_pAnimationPlayApi;};
	void SetCameraPlay(bool bCameraPlay);
	/*!
	This method return play camera flag.
   	*/
	bool IsCameraPlay() {return m_bCameraPlay;};
	/*!
	This method sets show model move trochoid flag during playing.
   	*/
	void SetShowTrochoid(bool bShowTrochoid){m_bShowTrochoid = bShowTrochoid;}
	bool IsShowTrochoid(){return m_bShowTrochoid;}
	/*!
	This method sets play sound flag.
   	*/
	void SetPlaySoundStatus(bool bPlaySound){m_bPlaySound = bPlaySound;}
	bool IsPlaySound(){return m_bPlaySound;}
	/*!
	This method sets play position and rotation flag.
   	*/
	void SetPlayPosRotStatus(bool bPlayPosRot);
	bool IsPlayPosRot(){return m_bPlayPosRot;}
	/*!
	This method sets play visible flag.
   	*/
	void SetPlayVisibleStatus(bool bPlayVisible);
	bool IsPlayVisible(){return m_bPlayVisible;}
	/*!
	This method sets play picture flag.
   	*/
	void SetPlayPicStatus(bool bPlayImage);
	bool IsPlayImage(){return m_bPlayImage;}
	/*!
	This method sets play clip plane flag.
   	*/
	void SetPlayClipStatus(bool bPlayClip);
	bool IsPlayClip(){return m_bPlayClip;}
	/*!
	This method sets play color flag.
   	*/
	void SetPlayColorStatus(bool bPlayColor);
	bool IsPlayColor(){return m_bPlayColor;}
	/*!
	This method sets collision flag during playing.
   	*/
	void SetPlayCollisionStatus(bool bCollision){m_bCollisionCheck = bCollision;}
	bool IsPlayCollision(){return m_bCollisionCheck;}
	/*!
	This method sets collision intervals(second) during playing.
   	*/
	void SetCollisionDelay(int delay){m_CollisionDelay = delay;}
	int GetCollisionDelay(){return m_CollisionDelay;}
	/*!
	This method sets whether stop play when find collision during playing.
   	*/
	bool IsStopPlayAfterCollision(){return m_bStopPlayAfterCollision;}
	void SetStopPlayAfterCollision(bool bStopPlayAfterCollision){m_bStopPlayAfterCollision = bStopPlayAfterCollision;}
	/*!
	This method sets whether clear clip plane in view when begin play.
   	*/
	bool IsClearClipPlane(){return m_bClearClipPlane;}
	void SetClearClipPlaneStatus(bool bClearClipPlane){m_bClearClipPlane = bClearClipPlane;}
	/*!
	This method sets continuous play flag.
   	*/
	bool GetContinuousPlay() { return m_bContinuousPlay; }
	void SetContinuousPlay(bool bContinuousPlay) { m_bContinuousPlay = bContinuousPlay; }
	/*!
	This method sets reverse play flag.
   	*/
	bool GetReversePlay() { return m_bReversePlay; }
	void SetReversePlay(bool bReversePlay) { m_bReversePlay = bReversePlay; }
	/*!
	This method sets play speed.
   	*/
	float GetPlaySpeed(){return m_fPlaySpeed;}
	void SetPlaySpeed(float fPlaySpeed){m_fPlaySpeed = fPlaySpeed;}

	/*! This method register target object key.
	\return The key.*/
	long RegisterInitTargetObjectKey();
	/*!
	Find init target object by target object path.
  	*/
	CProcessTargetObject* FindInitTargetObjectByPath(const char* targetPath);
	/*!
	Find init target object by target object ID.
  	*/
	CProcessTargetObject * FindInitTargetObjectByByID( const int ID );
	/*!
	 This method find init target object by path and removes it from the list of target object and then deletes it.
     \param targetPath: target object path.
   	*/
	void DeleteInitTargetObject(const char* targetPath);
	/*! This method deletes all target object and associated objects */
	void DeleteAllInitTargetObject();
	/*!
	Add a init target object to simulation animation.
	\param target: target object information.
  	*/
	CProcessTargetObject* AddInitTargetObject(TARGETOBJECTINFO& target);
	/*!
	Add a init target object to simulation animation.
	\param pTargetObject: A pointer to CProcessTargetObject object.
  	*/
	CProcessTargetObject* AddInitTargetObject(CProcessTargetObject *pTargetObject);
	/*!
	This method takes the given name and target and searches through the init target object list.  If a target object 
	corresponding to the name and path exists, it returns that target object. If there are no matches, it creates a new
	one with the given name and path and adds it to the target object list.
	\param name: The name of the new target object.
	\param path: The path of the new target object.
	\return A newly created target object or an already existing target object.
   	*/
	CProcessTargetObject* CreateInitTargetObjectByPath(const char *name, const char *path);

	/*!
	This method clear init target object and insert with the given vector of target object information.
	\param vecTarget: The vector of target object information.*/
	void UpdateInitTargetObjectList(vector<TARGETOBJECTINFO>& vecTarget);
	/*!
	This method return A pointer to the list of target object.
  	*/
	vlist_s * GetInitTargetObjectList() { return m_InitTargetObjectList; }

	/*! This method get init target object count.
	\return init target object count.*/
	int GetInitTargetObjectCount(){return m_InitTargetObjectList->count;}

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback_InitTargetObject(CUtilityXMLTag *xt, bool open, void *m_pExtraData);
	/*!
	Writes the XML data for this InitTargetObject tag. 
	\param xmlgen: A pointer to the xml generator that creates correct xml blocks
  	*/
	void Serialize_InitTargetObject(CUtilityXMLGenerator *xmlgen);

	View* GetView();
	void SetView(View* view);

	/************************************************************************/
	/* 功能：
		将动画保存为XML文件
		
		参数：
		filename：动画文件路径
		返回值：
		 0：成功
		 -1：未知错误
		 -2：文件路径为空
		 -3：文件没有写权限*/
	/************************************************************************/
	int WriteToFile(const wchar_t *filename);
#ifdef _MSC_VER
	int WriteToFile(const unsigned short *filename);
#endif
	int WriteToFile(const char *filename);
	//
	//	/*!
	//	 Reads XML data containing animation tags from a file and loads it into memory.
	//     \param filename Filename
	//   	*/	
	//
	bool ReadFromFile(const wchar_t *filename);
#ifdef _MSC_VER
	bool ReadFromFile(const unsigned short *filename);
#endif
	bool ReadFromFile(const char *filename);
	void ReadFromData(const char *xmlData);

	__wchar_t *GetSAFilePath(){ return m_strSAFilePath;};
	void SetSAFilePath(wchar_t *strSAFilePath){if(wcslen(strSAFilePath) < SA_BUFFER_SIZE_SMALL) wcscpy(m_strSAFilePath, strSAFilePath);};

	void InitData();
	void ClearData();
	//修改路径 by qinyp 2016/01/31
	void ModifyProcessTargetObjectPath(const char* pOldPath, const char* pNewPath);
	//删除闲置的动画 by qinyp 2016/03/21
	void DeleteSpareAnimation();
	
	

protected:
	struct vlist_s *	m_BehaviorActionList;
	int					m_CurSAID;
	__wchar_t			m_strSAFilePath[SA_BUFFER_SIZE_SMALL];

	char				m_Version[SA_BUFFER_SIZE_SMALL];				/*!< The version string. */	
	char				m_Company[SA_BUFFER_SIZE_SMALL];
	char				m_Name[SA_BUFFER_SIZE_SMALL];

	char				m_CurViewerVersion[SA_BUFFER_SIZE_SMALL];	/*!< The version of current SView . */	
	char				m_CurSVLVersion[SA_BUFFER_SIZE_SMALL];		/*!< The version of current SVL . */
	char				m_SVLVersion[SA_BUFFER_SIZE_SMALL];			/*!< The version of SView when save animatin. */
	char				m_ViewerVersion[SA_BUFFER_SIZE_SMALL];		/*!< The version of SVL when save animatin. */

	CAnimationStepManager*		m_pAnimationStepManager;						/*!< A pointer to CAnimationStepManager object. */

	bool				m_bCameraPlay;							/*!< Is play camera. */	
	bool				m_bShowTrochoid;						/*!< Is show trochoid. */	
	bool				m_bPlaySound;							/*!< Is play sound. */	
	bool				m_bPlayImage;							/*!< Is play picture. */
	bool				m_bPlayPosRot;							/*!< Is play postion and rotation. */
	bool				m_bPlayVisible;							/*!< Is play visible. */
	bool				m_bPlayClip;							/*!< Is play clip plane. */
	bool				m_bPlayColor;							/*!< Is play color. */
	bool				m_bCollisionCheck;						/*!< Is collision during playing. */
	int					m_CollisionDelay;						/*!< collision intervals(second) during playing. */

	bool				m_bStopPlayAfterCollision;				/*!< Is stop play when collison. */
	bool				m_bClearClipPlane;						/*!< Is clear clip plane in view when begin play. */
	AnimationPlayApi*	m_pAnimationPlayApi;						/*!< A pointer to AnimationPlayApi object. */

	bool				m_bContinuousPlay;						/*!< Is continuous play flag. */
	bool				m_bReversePlay;							/*!< Is reverse play flag. */
	float				m_fPlaySpeed;							/*!< Play speed. */

	struct vlist_s *	m_InitTargetObjectList;					/*!< A pointer to init target object list. */

	View* view;

};
SA_NAMESPACE_END
#endif
