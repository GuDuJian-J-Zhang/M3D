/*******************************************************************************
*	@file	
*		ProcessManager.h
*	@brief		
*		CProcessManager 过程管理器类
*		过程管理器为分段实现对过程的分组管理，一个过程管理器可包含多个过程。
*	@par
*	create by likun on 2015-10-12	
*   
**********************************************************************************/


#pragma once
#include "../SimulationCommon/SATools.h"
#include "CProcess.h"

class CUtilityXMLGenerator;
class CUtilityXMLTag;
struct vlist_s;

SA_NAMESPACE_BEGIN

class CAnimationStepManager;

class SA_API CProcessManager
{
private:
	void StartListProcessQuery();
	bool GetNextListProcess(CProcess** opProcess);

public:
	CProcessManager(const int ID = 0, const char *name = 0);
	virtual ~CProcessManager(void);
	/*!	This method get processManager ID.  	*/
	int		GetID() {return m_ID;};
	/*! \return The type of processManager which is 'CProcessManager'. */
	virtual const char * GetType() { return "CProcessManager"; } 
	/*!
	Writes the XML data for this processManager. 
	\param xmlgen: A pointer to the xml generator that creates correct xml blocks
  	*/
	void Serialize(CUtilityXMLGenerator *xmlgen);
	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);
	/*!
	Add a process to ProcessManager.
  	*/
	CProcess * AddProcess(const int ID = -1, const char *name = 0, const bool bAddBehaviorAction = true);
	void AddProcess(CProcess* pProcess);
	/*!
	This method return A pointer to the list of Process.
  	*/
	vlist_s * GetProcessList() { return m_ProcessList; };
	/*!
	 Removes the given CProcess from the list of Process and then deletes it.
     \param pProcess: Pass a pointer to the CProcess you want to delete.
   	*/	
	void DeleteProcess(CProcess* pProcess);
	/*! This method deletes all Process and associated objects */
	void DeleteAllProcess();
	/*! This method find process by id */
	CProcess * FindProcessByID(const int ID);
	/*! This method find process by animation id */
	CProcess* FindProcessByAnimationID(const int ID);
	/*! This method find process by name */
	CProcess * FindProcessByName(const char *name);
	/*! This method update view state by process */
	void UpdateViewWithProcess(int nProcessIdx);
	/*! This method set Current Process by ID
	\param ID: Process ID
	\param bUpdateModel: Whether update ins and entity target object state to view.
	\param bUpdateCam: Whether update camara target object state to view.
	\param bCamAni: Whether use camara animation effect.Only usefull when bUpdateCam = true.*/
	void SetCurProcessByID(int ID,bool bUpdateModel = true, bool bUpdateCam = false, bool bCamAni = false);
	/*! This method get Current Process ID.*/
	int GetCurProcessID(){return m_CurProcessID;};
	/*! This method set Current Process by index
	\param idx: Index of Process
	\param bUpdateModel: Whether update ins and entity target object state to view.
	\param bUpdateCam: Whether update camara target object state to view.
	\param bCamAni: Whether use camara animation effect.Only usefull when bUpdateCam = true.*/
	void SetCurProcessByIdx(int idx,bool bUpdateModel = true, bool bUpdateCam = false, bool bCamAni = false);
	/*! This method get Process by index of Process.
	\param idx: Index of Process*/
	CProcess* GetProcessByIdx(int idx);
	/*! This method get index of Process by ID.
	\param ID: ID of Process*/
	int GetProcessIdxByID(const int ID);
	/*! This method get current Process.*/
	CProcess* GetCurrentProcess();
	/*! This method get max Process ID.*/
	int GetMaxProcessID();
	/*! This method check whether the list of process is empty. */
	bool HasListProcess();
	/*! This method get process count
	\return process count.*/
	int GetProcessCount();
	/*! This method Return A pointer to the CAnimationStepManager object associated with this processManager. */ 
	CAnimationStepManager* GetAnimationStepManager(){ return m_pAnimationStepManager;};
	/*! This method Sets the pointer to the CAnimationStepManager object associated with this processManager. */
	void SetAnimationStepManager(CAnimationStepManager* pAnimationStepManager){ m_pAnimationStepManager = pAnimationStepManager;};
	/*! This method move the process form old index to new index.
	\param oldIdx: old index of process
	\param newIdx: new index of process. */
	bool MoveProcess(const int oldIdx,const int newIdx);
	/*! This method move a process to the below of target process.
	\param pProcess: The process to move
	\param pTargetProcess: The target process. */
	bool MoveProcess(CProcess* pProcess,CProcess* pTargetProcess);
	/*! This method return the previous process of a Process.
	\param pProcess: The current process
	\return A point to previous process. */
	CProcess * GetPreProcess(CProcess *pProcess);
	/*! This method return the next process of a Process.
	\param pProcess: The current process
	\return A point to next process. */
	CProcess * GetNextProcess(CProcess *pProcess);

	/*! This method get process by ID
	\param ID: The ID of process.
	\return Pointer to process.*/
	CProcess* GetProcessByID(long ID);
	/*! This method register a new process ID.
	\return The ID.*/
	long RegisterProcessID();
	/*! Return Name of the processManager. */ 
	const char* GetName(){return m_Name;};
	/*! Sets the Name of the processManager. */ 
	void SetName(const char* name);
	/*! Return Describe of the processManager. */ 
	const char* GetDesc(){return m_Desc;};
	/*! Sets the Describe of the processManager. */ 
	void SetDesc(const char* desc);
	/*The method move the pProcess to the first position of the processmanager.
	\param pProcess: the process to move
	\param bUnlink:  the flag if should unlink old link*/
	bool MoveProcessToFirst(CProcess* pProcess, bool bUnlink = true);
	/*The method only unlink the link to the processmanager, don't delete the pProcess.
	\param pProcess: the process to unlink*/
	void EraseProcess(CProcess* pProcess);
	/*The method add the process pProcess after the process pPreProcess. 
	\param pProcess:added process.
	\param pPreProcess:the previous process.*/
	void AddProcess(CProcess* pProcess, CProcess* pPreProcess);


protected:
	struct vlist_s *	m_ProcessList;					/*!< A pointer to the list of process. */
	int					m_CurProcessID;					/*!< Current Process ID. */
	int					m_ID;							/*!< ID of this ProcessManager. */
	char				m_Name[SA_BUFFER_SIZE_SMALL];		/*!< Name of this ProcessManager. */
	char				m_Desc[SA_BUFFER_SIZE_BIG];		/*!< Describe of this ProcessManager. */
	CAnimationStepManager*		m_pAnimationStepManager;				/*!< A pointer to relation AnimationStepManager of this ProcessManager. */
};
SA_NAMESPACE_END
