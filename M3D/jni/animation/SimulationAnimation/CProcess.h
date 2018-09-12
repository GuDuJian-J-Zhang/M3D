/*******************************************************************************
*	@file	
*		CProcess.h
*	@brief		
*		CProcess 过程类，过程为分段动画功能的最小单位，每个过程对应一段动画
*	@par
*	create by likun on 2015-10-12	
*   
**********************************************************************************/

#pragma once
#include "stdafx.h"
#include "../SimulationCommon/SATools.h"
#include "SBehaviorAction.h"
#include "SimulationAnimationManager.h"
#include <vector>

class CUtilityXMLGenerator;
class CUtilityXMLTag;
struct vlist_s;

SA_NAMESPACE_BEGIN

class CProcessManager;
class CProcess;

class SA_API CProcessTargetObject
{
public:
	/*!
	Constructs an CProcessTargetObject object.
    \param pParent: A pointer to CProcess or CSimulationAnimationManager object that is associated with this target object.
    \param name: The name of target object.
	\param path: The target description.  You can use CreateTargetString to generate the path for this target object.
	\param pivot: Sets the center of rotation if the target object is a segment in the SVIEW segment hierachy
  	*/
	CProcessTargetObject(void *pParent, const char *name, const char *path, AniPoint *pos = 0, AniQuat *quat = 0, AniPoint *scale = 0, bool visible = true, float fTrans = -1.0);

 
 	/*! \return The name of this target object. */  	
	const char *GetName() { return m_Name; }

 	/*! \return The path associated with this target object. Aliases are not expanded. */  	
	const char *GetPath() { return m_Path; }
	void SetPath(const char* pPath) {if(strlen(pPath) < SA_BUFFER_SIZE_SMALL) strcpy(m_Path, pPath);}
 	/*! \return The key of target object if applicable. */  	
	long GetTargetKey();

	void SetTargetKey(long key) { m_Key = key; }

 	/*! \return The path of target object with aliases fully expanded and no type descriptor.*/  	
	const char *GetResolvedPath() { return m_ResolvedPath; }

 	/*! \return Type of target object.*/  	
	const char *GetType() { return m_Type; }

	/*!
	Compares this target object with another one to determine equality.
 	\param target: Pass a target descriptor string.  You can generate this string using the CreateTargetString method.
	\return False if they are not equal or true if they are equal.
 	*/
	bool IsEqual(const char *target);

 	/*! \return The camera type for this target object.*/  	
	SACameraType GetCameraType() { return m_CameraType; }

	/*!
	Given an unexpanded target descriptor, this method returns the corresponding SVIEW segment path, target type and 
	camera type for this target object.
 	\param in: The target string to evaluate.
 	\param result: Returns the expanded target string.
 	\param targettype: Returns the type of target.
	\param ctype: Returns the camera type.
  	*/
	static void ResolveTarget(const char *in, char *result, char *targettype, SACameraType &ctype);


	/*!
	This utitlity method combines a target type and path to one target into one string.  The generated name can be
	used as an identifier to pass to the method IsEqual() and the methods CSBehaviorAction::AddAnimation 
	and CSBehaviorAction::FindAnimation to identify a target by a string.
  	\param type: The type of the target.
  	\param path: the path to the model instance.  
	\param result: Returns a pointer to the combined target string.
  	\param addquotes: Pass true if target string should be enclosed by quotation marks or false if you don't want quotes.
  	*/	
	static void CreateTargetString(const char *type, const char *path, char *result, bool addquotes = false);

	/*! set target object value. */
	void SetValue(const char *name, AniPoint *pos/* = 0*/, AniQuat *quat/* = 0*/, AniPoint *scale/* = 0*/, bool visible/* = true*/, float fTrans/* = -1.0*/);
	void SetPos(float x, float y, float z) { m_Pos.Set(x, y, z); }
	void SetQuat(float x, float y, float z, float w) { m_Quat.Set(x,y,z,w); }
	void SetScale(float x, float y, float z) { m_Scale.Set(x,y,z); }
	void SetVisible(bool bVisible) { m_bVisible = bVisible; }
	void SetTrans(float fTrans){ m_Trans = fTrans; }
 	/*! \return data for target object. */  	
	AniPoint * GetPos() { return &m_Pos; }
	AniQuat * GetQuat() { return &m_Quat; }
	AniPoint * GetScale() { return &m_Scale; }
	bool GetVisible() { return m_bVisible; }
	float GetTrans(){ return m_Trans; }
	/*!
	Writes the XML data for this target object. 
	\param xmlgen: A pointer to the xml generator that creates correct xml blocks
  	*/
	void Serialize(CUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

	void SetSerializeFromKey( bool sfk ) { m_bSerializeFromKey = sfk; }

	/*!
	如果是要素对象，返回要素所属的零件及要素ID.
  	*/
	void GetInsPathAndPMIIdByResolvedPath(char* plcIdPath,char* pmiId);
	 
protected:
 	char					m_Name[SA_BUFFER_SIZE_SMALL];				/*!< The name of this target object. */
	char					m_Path[SA_BUFFER_SIZE_SMALL];		/*!<  The unresolved path. */
	char					m_ResolvedPath[SA_BUFFER_SIZE_SMALL];/*!<  The resolved path. */ 
	char					m_Type[SA_BUFFER_SIZE_SMALL];				/*!<  The target type. */
	long					m_Key;										/*!<  The key of target object. */
	void*					m_pParent;									/*!<  A pointer to CProcess object or CSimulationAnimationManager object that this targetobject belongs to. */
	SACameraType			m_CameraType;								/*!<  The type of camera. */
	AniPoint					m_Pos; 										/*!<  The position. */
	AniQuat					m_Quat; 									/*!<  The rotation. */
	AniPoint					m_Scale;									/*!<  The scale. */
	bool					m_bVisible;									/*!<  A flag of visible. */
	float                 m_Trans;                                       /*!<  The transparency. */

	bool m_bSerializeFromKey;

};


class SA_API CProcess
{
public:
	CProcess(const int ID,const int nBehaviorActionID,const char *name = 0);
	virtual ~CProcess(void);
	void Reference();
	void Release();
	/*!	This method get process ID.  	*/
	int		GetID() {return m_ID;};
	void     SetID(int nID){m_ID = nID;}
	/*! \return The type of process which is 'CProcess'. */
	virtual const char * GetType() { return "CProcess"; }
	/*!
	Writes the XML data for this process. 
	\param xmlgen: A pointer to the xml generator that creates correct xml blocks
  	*/
	void Serialize(CUtilityXMLGenerator *xmlgen);
	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);
	/*!
	This method takes the given name and target and searches through the target object list.  If a target object 
	corresponding to the name and path exists, it returns that target object. If there are no matches, it creates a new
	one with the given name and path and adds it to the target object list.
	\param name: The name of the new target object.
	\param path: The path of the new target object.
	\return A newly created target object or an already existing target object.
   	*/	
	CProcessTargetObject * CreateTargetObjectByPath(const char *name, const char *path);
	/*!
	Add a targetobject to Process.
  	*/
	void AddTargetObject(CProcessTargetObject *targetobject);
	/*!
	Find targetobject by targetobject name.
  	*/
	CProcessTargetObject * FindTargetObjectByName(const char *name);
	/*!
	Find targetobject by targetobject path.
  	*/
	CProcessTargetObject * FindTargetObjectByPath(const char *path);
	/*!
	This method return A pointer to the list of targetobject.
  	*/
	vlist_s * GetTargetObjectList() { return m_TargetObjectList; };
	/*!
	This method reset target object list
	\param vecTarget: The vector of target object.
  	*/
	void UpdateTargetList(std::vector<TARGETOBJECTINFO*>& vecTarget);
	/*!
	 Removes the given CProcessTargetObject from the list of target object and then deletes it.
     \param tinfo: Pass a pointer to the CProcessTargetObject you want to delete.
   	*/	
	void DeleteTargetObject(CProcessTargetObject *tinfo);
	/*! This method deletes all target object and associated objects */
	void DeleteAllTargetObject();
	/*! This method find target object by id */
	CProcessTargetObject * FindTargetObjectByID(const int ID);	
	/*! This method get max target object id */
	int GetMaxTargetObjectID();
	/*! This method reset target object list cursor */
	void StartListTargetObjectQuery();
	/*! This method get next target object 
	\param opBehaviorAction(OUT): The next target object.
	\return True if have next target object and false if have not next target object.*/
	bool GetNextListTargetObject(CProcessTargetObject** opBehaviorAction);
	/*! This method check whether the list of target object is empty. */
	bool HasListTargetObject();
	/*! This method get target object count
	\return target object count.*/
	int GetTargetObjectCount();
	/*! This method get target object by key
	\param key: The key of target object.
	\return Pointer to Object.*/
	CProcessTargetObject* GetTargetObjectByKey(long key);
	/*! \return A pointer to the list of target object.*/
	vlist_s * GetProcessTargetObjectList() { return m_TargetObjectList; };

	/*! This method register target object key.
	\return The key.*/
	long RegisterTargetObjectKey();
	/*! \return A pointer to the CProcessManager object associated with this process.*/
	CProcessManager* GetProcessManager(){ return m_pProcessManager;};
	/*! Sets the CProcessManager Object associated to this process. */  
	void SetProcessManager(CProcessManager* pProcessManager);
	/*! Return Name of the process. */  	
	const char* GetName(){return m_Name;};
	/*! Sets the Name of the process. */ 
	void SetName(const char* name);
	/*! Return Describe of the process. */ 
	const char* GetDesc(){return m_Desc;};
	/*! Sets the Describe of the process. */ 
	void SetDesc(const char* desc);
	/*! Return ID of CSBehaviorAction associated with this process. */ 
	const int GetBehaviorActionID(){ return m_nBehaviorActionID;};
	/*! Sets the ID of CSBehaviorAction associated with this process. */ 
	void SetBehaviorActionID(const int ID);
	/*! Return A pointer to the CSBehaviorAction object associated with this process. */ 
	CSBehaviorAction* GetBehaviorAction();
	/*! This method get camera target object
	\param bCreate: Create camera target object if not exist.
	\return Pointer to object.*/
	CProcessTargetObject * GetCameraTargetObject(bool bCreate);
	/*! This method get camera target object information 
	\param pos(out): The translation of camera.
	\param rotation(out): The rotation of camera.
	\param scale(out): The scaleFactor of camera.
	\return True if have camera target object and false if have not camera target object */
	bool GetCamera(AniPoint& pos,AniQuat& rotation, AniPoint& scale);
	/*! This method set camera target object information 
	\param pos: The translation of camera.
	\param rotation: The rotation of camera.
	\param scale: The scaleFactor of camera.*/
	void SetCamera(AniPoint& pos,AniQuat& rotation, AniPoint& scale);

	/*! Return A pointer to the CSimulationAnimationManager object associated with this process. */ 
	CSimulationAnimationManager* GetSimulationAnimationManager();
	/*! Return A pointer to the CAnimationStepManager object associated with this process. */ 
	CAnimationStepManager* GetAnimationStepManager();
	/*! This method update view state by target object
	\param bUpdateModel: Whether update ins and entity target object state to view.
	\param bUpdateCam: Whether update camara target object state to view.
	\param bCamAni: Whether use camara animation effect.Only usefull when bUpdateCam = true.*/ 
	void UpdateView(bool bUpdateModel = true, bool bUpdateCam = false, bool bCamAni = false);
	/*! 转变CProcessTargetObject为TARGETOBJECTINFO. 
	\param pProcessTargetObject(IN): A point to CProcessTargetObject object.
	\param targetObjectInfo(OUT): A point to TARGETOBJECTINFO object.*/ 
	void TargetObject2StructInfo(CProcessTargetObject*pProcessTargetObject, TARGETOBJECTINFO* targetObjectInfo);
	//克隆
	CProcess* Clone(CProcessManager* pParent);

	bool IsPlay() { return m_bPlay; }
	void SetIsPlay(bool bPlay) { m_bPlay = bPlay; }
protected:
	struct vlist_s *	m_TargetObjectList;					/*!< A pointer to the list of TargetObject. */
	int					m_ID;								/*!< ID of this Process. */
	UINT				m_ReferenceCount;
	char				m_Name[SA_BUFFER_SIZE_SMALL];		/*!< Name of this Process. */
	char				m_Desc[SA_BUFFER_SIZE_BIG];			/*!< Describe of this Process. */
	int					m_nBehaviorActionID;				/*!< Relation CSBehaviorAction of this ProcessManager. */
	CSBehaviorAction*   m_pBehaviorAction;
	CProcessManager *	m_pProcessManager;					/*!< A pointer to relation CProcessManager of this Process. */
	bool				m_bPlay;							/*!< Is play this processManager. */
	
};
SA_NAMESPACE_END
