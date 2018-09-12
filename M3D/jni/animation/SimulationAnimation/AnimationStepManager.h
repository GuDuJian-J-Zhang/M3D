/*******************************************************************************
*	@file	
*		AnimationStepManager.h
*	@brief		
*		CAnimationStepManager 分段动画类
*		该类是分段动画的最高管理类，提供分段动画数据管理及播放相关接口
*	@par
*	create by likun on 2015-10-12	
*   
**********************************************************************************/

#pragma once
#include "../SimulationCommon/SATools.h"
#include "ProcessManager.h"
#include "SimulationAnimationManager.h"

class CUtilityXMLGenerator;
class CUtilityXMLTag;
struct vlist_s;
class CGroupManager;

SA_NAMESPACE_BEGIN

class SA_API CAnimationStepManager
{
public:
	enum AnimationPlayMode
	{
		PLAY_MODE_NONE,						//停止播放动画
		PLAY_MODE_PROCESS,					//播放单个过程
		PLAY_MODE_FROM_CURPROCESS,			//从当前过程开始播放
		PLAY_MODE_PROCESSMANAGER,			//播放一个过程管理器
		PLAY_MODE_FROM_CURPROCESSMANAGER,	//从当前过程管理器播放
		PLAY_MODE_ALL,						//播放所有动画
		PLAY_MODE_RANDOM				//随机播放
	};
private:
	void StartListProcessManagerQuery();
	bool GetNextListProcessManager(CProcessManager** opProcessManager);
public:
	CAnimationStepManager(const char *name = 0);
	virtual ~CAnimationStepManager(void);
	/*!
	Writes the XML data for this AnimationStepManager. 
	\param xmlgen: A pointer to the xml generator that creates correct xml blocks
  	*/
	void Serialize(CUtilityXMLGenerator *xmlgen);
	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);
	/*!
	Add a ProcessManager to AnimationStepManager.
  	*/
	CProcessManager * AddProcessManager(const int ID = -1, const char *name = 0);
	void AddProcessManager(CProcessManager* pProcessManager);
	/*!
	This method return A pointer to the list of CProcessManager.
  	*/
	vlist_s * GetProcessManagerList() { return m_ProcessManagerList; };
	/*!
	 Removes the given CProcessManager from the list of CProcessManager and then deletes it.
     \param pProcessManager: Pass a pointer to the CProcessManager you want to delete.
   	*/
	void DeleteProcessManager(CProcessManager* pProcessManager);
	/*! This method deletes all CProcessManager and associated objects */
	void DeleteAllProcessManager();
	/*! This method find CProcessManager by id */
	CProcessManager * FindProcessManagerByID(const int ID);
	/*! This method find CProcessManager by name */
	CProcessManager * FindProcessManagerByName(const char *name);
	/*! This method find CProcess by ProcessManagerID and ProcessID */
	CProcess * FindProcessManagerByManagerIDAndProcessID(const int processManagerID, const int processID);
	/*! This method update view state by processManager */
	void UpdateViewWithProcessManager(int nProcessManagerIdx);
	/*! This method set Current ProcessManager by ID
	\param ID: ID of ProcessManager
	\param bUpdateModel: Whether update ins and entity target object state to view.
	\param bUpdateCam: Whether update camara target object state to view.
	\param bCamAni: Whether use camara animation effect.Only usefull when bUpdateCam = true.*/
	void SetCurProcessManagerByID(int ID,bool bUpdateModel = true, bool bUpdateCam = false, bool bCamAni = false);
	/*! This method get Current Process ID.*/
	int GetCurProcessManagerID(){return m_CurProcessManagerID;};
    void SetCurProcessManagerID(int ID){m_CurProcessManagerID = ID;};
	/*! This method set Current ProcessManager by Index
	\param ID: index of ProcessManager
	\param bUpdateModel: Whether update ins and entity target object state to view.
	\param bUpdateCam: Whether update camara target object state to view.
	\param bCamAni: Whether use camara animation effect.Only usefull when bUpdateCam = true.*/
	void SetCurProcessManagerByIdx(int idx,bool bUpdateModel = true, bool bUpdateCam = false, bool bCamAni = false);
	/*! This method get index of ProcessManager by ID.
	\param ID: ID of ProcessManager*/
	int GetProcessManagerIdxByID(const int ID);
	/*! This method get ProcessManager by index of ProcessManager.
	\param idx: Index of ProcessManager*/
	CProcessManager* GetProcessManagerByIdx(int idx);
	/*! This method get current ProcessManager.*/
	CProcessManager* GetCurrentProcessManager();
	/*! This method get current Process.*/
	CProcess* GetCurrentProcess();
	/*! This method get max ProcessManager ID.*/
	int GetMaxProcessManagerID();
	/*! This method check whether the list of ProcessManager is empty. */
	bool HasListProcessManager();
	/*! This method get ProcessManager count
	\return ProcessManager count.*/
	int GetProcessManagerCount();

	/*! This method Return A pointer to the CSimulationAnimationManager object associated with this animationStep. */ 
	CSimulationAnimationManager* GetSimulationAnimationManager() {return m_pSA;};
	/*! This method Sets the pointer to the CSimulationAnimationManager object associated with this animationStep. */ 
	void SetSimulationAnimationManager(CSimulationAnimationManager*pSimulationAnimationManager) {m_pSA = pSimulationAnimationManager;};
	/*! This method register a new processManager ID.
	\return The ID.*/
	long RegisterProcessManagerID();
	
	/*! Return Name of the animationStep. */ 
	const char* GetName(){return m_Name;};
	/*! Sets the Name of the animationStep. */
	void SetName(const char* name){if(NULL != name)	strcpy(m_Name, name);};

	virtual const char * GetType() { return "CAnimationStepManager"; } 
	/*! This method move the processManager form old index to new index.
	\param oldIdx: old index of processManager
	\param newIdx: new index of processManager. */
	bool MoveProcessManager(const int oldIdx,const int newIdx);
	/*! This method move a processManager to the below of target processManager.
	\param pProcessManager: The processManager to move
	\param pTargetProcessManager: The target processManager. */
	bool MoveProcessManager(CProcessManager* pProcessManager,CProcessManager* pTargetProcessManager);
	/*! This method return the previous processManager of a processManager.
	\param pProcessManager: The current processManager
	\param bSteps: Is return child processmanager
	\param pFocusProcessManager: find in this processmanager.if NULL,find in all processmanager
	\return A point to previous processManager. */
	CProcessManager * GetPreProcessManager(CProcessManager *pProcessManager, bool bSteps = false, CProcessManager* pFocusProcessManager = NULL);
	/*! This method return the next processManager of a processManager.
	\param pProcessManager: The current processManager
	\param bSteps: Is return child processmanager
	\param pFocusProcessManager: find in this processmanager.if NULL,find in all processmanager
	\return A point to next processManager. */
	CProcessManager * GetNextProcessManager(CProcessManager *pProcessManager, bool bSteps = false, CProcessManager* pFocusProcessManager = NULL);
	/*! This method return the previous process of a Process.
	\param pProcess: The current process
	\param bSteps: Is return child process
	\param pFocusProcessManager: find in this processmanager.if NULL,find in all processmanager
	\return A point to previous process. */
	CProcess * GetPreProcess(CProcess *pProcess, bool bSteps = false, CProcessManager* pFocusProcessManager = NULL);
	/*! This method return the next process of a Process.
	\param pProcess: The current process
	\param bSteps: Is return child process
	\param pFocusProcessManager: find in this processmanager.if NULL,find in all processmanager
	\return A point to next process. */
	CProcess * GetNextProcess(CProcess *pProcess, bool bSteps = false, CProcessManager* pFocusProcessManager = NULL);


	/*! This method get PlayMode.*/
	AnimationPlayMode GetPlayMode(){ return m_PlayMode;};
	/*! Sets PlayMode.
	\param nPlayMode: PlayMode(PLAY_NONE,PLAY_PROCESS,PLAY_PROCESSMANAGER,PLAY_ALL...)*/
	void SetPlayMode(AnimationPlayMode nPlayMode){ m_PlayMode = nPlayMode;};
	/*! This method check whether at the start state.*/
	bool IsAtPlayFirst();
	/*! This method check whether at the end state.*/
	bool IsAtPlayEnd();
	/*! This method Stops the execution of all animations and rewinds to the start.*/
	void Rewind(AnimationPlayMode playMode,bool bReverse);
	/*! This method Play Animation.
	\param playMode: PlayMode
	\param bReverse: Is Reverse Play.
	\param bChangeProcess: Is Change Current Process*/
	void Play(AnimationPlayMode playMode,bool bReverse = false, bool bChangeProcess = false);
	/*! \return True if animations are currently playing or false if no animations are playing. */
	bool IsPlaying();
	/*! Stop animation Play. */
	void Stop();
	/*! Pauses animation Play. */
	void Pause();
	/*! Call when a CSBehaviorAction play end.This method auto start next process if not at end*/
	void PlayFinishCB(CSBehaviorAction* pBehaviorAction);
	/*! Start play camera change animation between current view camera state and target process camera state. */
	bool StartChangeCamera(CProcess* pCurProcess,bool bAnimation = false);
	/*! End play camera change animation between current view camera state and target process camera state. */
	void EndChangeCamera();
	/*! Get camera change CSBehaviorAction. */
	CSBehaviorAction* GetBehaviorActionChgCam();
	/*! Set camera change animation time. 
	\param fSecond: The camera change animation play time(fSecond> 0) */
	void SetChgCamTime(float fSecond = 1.0f);
	/*! This method move pSourceProcess to the below of the target process pTargetProcess.
	\param pSourceProcess: the process to move
	\param pTarget: the target object process or processmanager*/
	bool MoveProcess(CProcess* pSourceProcess, void* pTarget);
	/*! This method find process by the param nAnimationID.
	\param nAnimationID: the id of process finding out in the animationstepmanager*/
	/*! This method move pSourceProcess to the below of the target process pTargetProcess.
	\param pSourceProcess: the process to move
	\param pTargetProcess: the target object process*/
	bool MoveProcess(CProcess* pSourceProcess, CProcess* pTargetProcess);
	/*! This method move pSourceProcess to the below of the target processmanager pTargetProcessManager.
	\param pSourceProcess: the process to move
	\param pTargetProcessManager: the target object processmanager*/
	bool MoveProcess(CProcess* pSourceProcess, CProcessManager* pTargetProcessManager);
	CProcess* FindProcessByAnimationID(int nAnimationID);
	/*! This method return the pause state.*/
	bool IsPauseState();
	/*! This method return the first process of the current AnimationStepManager.*/
	CProcess* GetFirstProcess(bool bSteps = false);
	/*! This method return the last process of the current AnimationStepManager.*/
	CProcess* GetLastProcess(bool bSteps = false);
	/*! This method return the first top processmanager of the current AnimationStepManager.*/
	CProcessManager* GetFirstTopProcessManager();
	/*! This method return the last top processmanager of the current AnimationStepManager.*/
	CProcessManager* GetLastTopProcessManager();
	/*! This method return the previous top processmanager of the current AnimationStepManager.*/
	CProcessManager* GetPreTopProcessManager(CProcessManager* pProcessManager);
	/*! This method return the next top processmanager of the current AnimationStepManager.*/
	CProcessManager* GetNextTopProcessManager(CProcessManager* pProcessManager);
	/*! This method return the play child processManager state.*/
	bool IsPlayChildProcessManagerSteps() { return m_bPlayChildProcessManagerSteps; }
	void SetPlayChildProcessManagerSteps(bool bPlayChildProcessManagerSteps) { m_bPlayChildProcessManagerSteps = bPlayChildProcessManagerSteps; }
	/*! This method return the play child processManager state.*/
	CProcessManager* GetFocusProcessManager() { return m_pFocusProcessManager; }
	void SetFocusProcessManager(CProcessManager* pFocusProcessManager) { m_pFocusProcessManager = pFocusProcessManager; }
protected:
	struct vlist_s *	m_ProcessManagerList;			/*!< A pointer to the list of processManager. */
	int					m_CurProcessManagerID;			/*!< Current ProcessManager ID. */
	char				m_Name[SA_BUFFER_SIZE_SMALL];		/*!< Name of this AnimationStepManager. */
	CSimulationAnimationManager* m_pSA;					/*!< A pointer to relation CSimulationAnimationManager of this AnimationStepManager. */
	AnimationPlayMode					m_PlayMode;		/*!< Play Mode. */
	CSBehaviorAction* m_pBehaviorActionChgCam;		/*!< Camera change CSBehaviorAction. */
	int					m_nChgCamTickNum;				/*!< Camera change CSBehaviorAction tick number. */
	bool                m_bIsPause;						/*!< Is pause flag. */
	bool				m_bPlayChildProcessManagerSteps;		/*!< Is play child ProcessManager. */
	CProcessManager*	m_pFocusProcessManager;

};
SA_NAMESPACE_END
