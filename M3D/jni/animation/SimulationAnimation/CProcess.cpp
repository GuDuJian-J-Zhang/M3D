#include "stdafx.h"
#include "CProcess.h"
#include "../SimulationCommon/UtilityXMLParser.h"
#include "../SimulationCommon/UtilityTextParser.h"
#include "../SimulationCommon/vlist.h"
#include "ProcessManager.h"
#include "AnimationStepManager.h"
#include "SInterpolator.h"
#include "SAnimation.h"
#include "../SimulationCommon/MatrixOperation.h"
#include "KeyFrame.h"
#include <typeinfo>
#include "../../Utility.h"

SA_NAMESPACE_BEGIN

void CProcessTargetObject::Serialize(CUtilityXMLGenerator *xmlgen)
{
	if (strcmp(m_Type, TARGETOBJECTTYPE_CAM) == 0 || 
		strcmp(m_Type, TARGETOBJECTTYPE_INS) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_PMI) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_CLIP) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_FOLDER) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_SOUND) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_IMAGE) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_ZOOM) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_TOOL) == 0)
	{
		CUtilityXMLTag xmlt;
		xmlt.SetTagname("ProcessTargetObject");
		xmlt.AddPropertyInt("ID", m_Key);

		//只有对象的初始化列表中记录对象的名称和路径，以减少文件大小
		CSimulationAnimationManager* pSA = (CSimulationAnimationManager*)m_pParent;
		if(typeid(*pSA) == typeid(CSimulationAnimationManager))
		{
			if (strcmp(m_Name, "") != 0)
				xmlt.AddProperty("Name", m_Name, true);

			if (m_bSerializeFromKey && m_CameraType==NoCamera)
			{
				xmlt.AddProperty("Path", GetPath(), true);				

			}
			else
			{
				xmlt.AddProperty("Path", GetPath(), true);	
			}
		}else if(typeid(*pSA) == typeid(CProcess) && strcmp(m_Type, TARGETOBJECTTYPE_IMAGE) == 0)
		{
			//图片类型
			if (strcmp(m_Name, "") != 0)
				xmlt.AddProperty("Name", m_Name, true);
			
			xmlt.AddProperty("Path", GetPath(), true);	
		}
 		
		char buffer[SA_BUFFER_SIZE_BIG] = {0};

		//目前只有零件和摄像机需要记录配置矩阵
		if(strcmp(m_Type, TARGETOBJECTTYPE_CAM) == 0 || 
			strcmp(m_Type, TARGETOBJECTTYPE_INS) == 0)
		{
			SACLOCALE(sprintf(buffer, "[%f %f %f]", m_Pos.x, m_Pos.y, m_Pos.z));
			xmlt.AddProperty("pos", buffer, true);

			SACLOCALE(sprintf(buffer, "[%f %f %f %f]", m_Quat.x, m_Quat.y, m_Quat.z, m_Quat.w));
			xmlt.AddProperty("quat", buffer, true);

			if(!m_Scale.Equal(AniPoint(1.0f,1.0f,1.0f)))
			{
				SACLOCALE(sprintf(buffer, "[%f %f %f]", m_Scale.x, m_Scale.y, m_Scale.z));
				xmlt.AddProperty("scale", buffer, true);
			}
		}else if(strcmp(m_Type, TARGETOBJECTTYPE_IMAGE) == 0)
		{
			//图片类型
			SACLOCALE(sprintf(buffer, "[%f %f %f]", m_Pos.x, m_Pos.y, m_Pos.z));
			xmlt.AddProperty("pos", buffer, true);
			
			if(!m_Scale.Equal(AniPoint(1.0f,1.0f,1.0f)))
			{
				SACLOCALE(sprintf(buffer, "[%f %f %f]", m_Scale.x, m_Scale.y, m_Scale.z));
				xmlt.AddProperty("scale", buffer, true);
			}
		}

		xmlt.AddPropertyInt("visible",GetVisible());
		xmlt.AddPropertyFloat("trans", GetTrans());
		xmlt.SetOpen(false);
		xmlgen->AddTag(&xmlt);
	}
}
 
void *CProcessTargetObject::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char cName[SA_BUFFER_SIZE_SMALL] = {0};
	char cPath[SA_BUFFER_SIZE_SMALL] = {0};
	char cPos[SA_BUFFER_SIZE_SMALL] = {0};
	char cQuat[SA_BUFFER_SIZE_SMALL] = {0};
	char cScale[SA_BUFFER_SIZE_SMALL] = {0};
	char cTrans[SA_BUFFER_SIZE_SMALL] = {0};

	AniPoint pntPos;
	AniQuat quatQuat;
	AniPoint pntScale;
	bool bVisible = false;
	float fTrans = -1.0;
	int key = 0;
	if (open)
	{
		CProcess *outerProcess = (CProcess *)xt->GetXMLParser()->GetStackData();
		if(NULL != outerProcess)
		{
			if (!xt->GetProperty("ID", key))
				key = 0;
			if (!xt->GetProperty("Name", cName))
				strcpy(cName, "");
			if (!xt->GetProperty("Path", cPath))
				strcpy(cPath, "");

			if (!xt->GetProperty("pos", cPos))
				pntPos.Set(0.0f,0.0f,0.0f);
			else
				CUtilityXMLParser::GetFloatPoint(cPos, pntPos);

			if (!xt->GetProperty("quat", cQuat))
				quatQuat.Set(0.0f,0.0f,0.0f,1.0f);
			else
				CSACommonAPI::GetQuatPoint(cQuat, quatQuat);

			if (!xt->GetProperty("scale", cScale))
				pntScale.Set(1.0f,1.0f,1.0f);
			else
				CUtilityXMLParser::GetFloatPoint(cScale, pntScale);

			if (!xt->GetProperty("visible", bVisible))
					 bVisible = false;

			if(!xt->GetProperty("trans", cTrans))
				strcpy(cTrans, "");
			
			sscanf(cTrans, "%f", &fTrans);

			CProcessTargetObject *to = NULL;
			//判断是初始状态对象还是过程中的对象
			if(typeid(*outerProcess) == typeid(CSimulationAnimationManager))
			{
				//图片不放入初始列表
				if(strcmp(string(cName).substr(0, 5).c_str(), TARGETOBJECTTYPE_IMAGE) != 0)
					to = ((CSimulationAnimationManager*)outerProcess)->CreateInitTargetObjectByPath(cName, cPath);
			}
			else if(typeid(*outerProcess) == typeid(CProcess))
			{
				//如果过程中的对象列表没有记录对象名称和路径，取初始对象列表中相同ID的对象名称和路径
				if(strcmp(cName, "") == 0 && strcmp(cPath, "") == 0)
				{
					CSimulationAnimationManager* pSA = (CSimulationAnimationManager*)m_pExtraData;
					if(pSA)
					{
						CProcessTargetObject *initTarget = pSA->FindInitTargetObjectByByID(key);
						if(initTarget)
						{
							strcpy(cName, initTarget->GetName());
							strcpy(cPath, initTarget->GetPath());
						}
					}
				}
				to = outerProcess->CreateTargetObjectByPath(cName, cPath);
			}
			
			if(to)
			{
				to->SetPos(pntPos.x,pntPos.y,pntPos.z);
				to->SetQuat(quatQuat.x,quatQuat.y,quatQuat.z,quatQuat.w);
				to->SetScale(pntScale.x,pntScale.y,pntScale.z);
				to->SetVisible(bVisible);
				to->SetTargetKey(key);
				to->SetTrans(fTrans);
			}
		}
	}
	return 0;
}

CProcessTargetObject::CProcessTargetObject(void *pParent, const char *name, const char *path, AniPoint *pos/* = 0*/, AniQuat *quat/* = 0*/, AniPoint *scale/* = 0*/, bool visible/* = true*/, float fTrans/* = -1.0*/)
{
	m_pParent = pParent;
	m_bSerializeFromKey = true;	
	if (name)
		strcpy(m_Name, name);
	else 
		strcpy(m_Name, "");

	if (path)
		strcpy(m_Path, path);
	else
		strcpy(m_Path, "");

	if (pos)
		m_Pos = pos;
	else 
		m_Pos.Set(0,0,0);

	if (quat)
		m_Quat = quat;
	else 
		m_Quat.Set( 0.0f, 0.0f, 0.0f, 1.0f);

	if (scale)
		m_Scale = scale;
	else 
		m_Scale.Set(1.0f,1.0f,1.0f);

	m_bVisible = visible;
	
	m_Trans = fTrans;

	m_CameraType = NoCamera;

 	ResolveTarget(m_Path, m_ResolvedPath, m_Type, m_CameraType);

	m_Key = 0;
	if(m_pParent)
	{
		//如果是创建初始的对象，生成新的ID号。如果是向过程中添加，ID号取初始对象列表中相同对象的ID
		if(typeid(*(CSimulationAnimationManager*)m_pParent) == typeid(CSimulationAnimationManager))
		{
			m_Key = ((CSimulationAnimationManager*)m_pParent)->RegisterInitTargetObjectKey();
		}
		else if(typeid(*(CProcess*)m_pParent) == typeid(CProcess))
		{
			CProcessTargetObject* pInitProcessTarget = ((CProcess*)m_pParent)->GetProcessManager()->GetAnimationStepManager()->GetSimulationAnimationManager()->FindInitTargetObjectByPath(m_Path);
			if(pInitProcessTarget)
			{
				m_Key = pInitProcessTarget->GetTargetKey();
			}
			else
			{
				//初始对象列表中如果没有要创建的对象，过程中不可以添加
				//assert(0);
				if (strcmp(GetType(), TARGETOBJECTTYPE_IMAGE) == 0)
				{
					m_Key = -1;
				}
				else
				{
					m_Key = ((CProcess*)m_pParent)->GetSimulationAnimationManager()->RegisterInitTargetObjectKey();
				}
			}
		}
	}
}


void CProcessTargetObject::CreateTargetString(const char *type, const char *path, char *result, bool addquotes)
{
	char temppath[SA_BUFFER_SIZE_SMALL] = {0};
	if(path && (int)strlen(path) < SA_BUFFER_SIZE_SMALL)
		strcpy(temppath, path);
	strcpy(result, type);
	strcat(result, ":");

	strcat (result, temppath);
 	if (addquotes)
	{
		char result2[SA_BUFFER_SIZE_BIG] = {0};
		sprintf(result2, "\"%s\"", result);
		strcpy(result, result2);
	}

}
	
void CProcessTargetObject::SetValue(const char *name, AniPoint *pos/* = 0*/, AniQuat *quat/* = 0*/, AniPoint *scale/* = 0*/, bool visible/* = true*/, float fTrans/* = -1.0*/)
{
	m_bSerializeFromKey = true;
	if (name)
		strcpy(m_Name, name);
	else
		strcpy(m_Name, "");

	if (pos)
		m_Pos = pos;
	else
		m_Pos.Set(0, 0, 0);

	if (quat)
		m_Quat = quat;
	else
		m_Quat.Set(0.0f, 0.0f, 0.0f, 1.0f);

	if (scale)
		m_Scale = scale;
	else
		m_Scale.Set(1.0f, 1.0f, 1.0f);

	m_bVisible = visible;

	m_Trans = fTrans;

	m_CameraType = NoCamera;

	ResolveTarget(m_Path, m_ResolvedPath, m_Type, m_CameraType);
}

bool CProcessTargetObject::IsEqual(const char *target)
{
	if(!strcmp(target, m_Path))
		return true;
	else
		return false;
}


void CProcessTargetObject::ResolveTarget(const char *in, char *result, char *TargetType, SACameraType &ctype)
{
	if(strlen(in) == 0)
		return;
	char target[SA_BUFFER_SIZE_SMALL] = {0};
	char temp[SA_BUFFER_SIZE_SMALL] = {0};
	if(in && (int)strlen(in) < SA_BUFFER_SIZE_SMALL)
		strcpy(target, in);
	ctype = NoCamera;
	static delimiter_t find_end_td[] = {{"\x21-\x7a", false, ":", true, false}};
	static delimiter_t find_end_td2[] = {{"\x21-\x7a", false, "\x01", true, false}};

  	CUtilityTextParser tp;
 	tp.InsertBuffer(target, (unsigned int)strlen(target));
	int dlum;
	bool res = tp.ParseBuffer(TargetType, (int &)dlum, find_end_td, 1);
	//res = tp.ParseBuffer(target, (int &)dlum, find_end_td2, 1);
	char seps[4]   = ":";
	char* pTarget = NULL;
	pTarget = strtok(target, seps);
	pTarget = strtok(NULL, seps);
	if(!pTarget)
		return;
	strcpy(target, pTarget);
	if (strcmp(TargetType, TARGETOBJECTTYPE_INS) == 0 || 
		strcmp(TargetType, TARGETOBJECTTYPE_PMI) == 0 || 
		strcmp(TargetType, TARGETOBJECTTYPE_CLIP) == 0 || 
		strcmp(TargetType, TARGETOBJECTTYPE_FOLDER) == 0 || 
		strcmp(TargetType, TARGETOBJECTTYPE_SOUND) == 0 || 
		strcmp(TargetType, TARGETOBJECTTYPE_IMAGE) == 0)
	{
		strcpy(result, target);
		return;
	}
 	
	char targetname[SA_BUFFER_SIZE_SMALL] = {0};
	char extraname[SA_BUFFER_SIZE_SMALL] = {0};

	tp.InsertBuffer(target, (unsigned int)strlen(target));
	static delimiter_t find_end_td3[] = {{"\x21-\x7a", false, "/", true, false}};
	res = tp.ParseBuffer(targetname, (int &)dlum, find_end_td3, 1);
	res = tp.ParseBuffer(extraname, (int &)dlum, find_end_td2, 1);
 	int len2 = 0;
	if (strcmp(extraname, "CAMERA") == 0 || strcmp(extraname, "TARGET") == 0 || strcmp(extraname, "POSITION") == 0 || strcmp(extraname, "TARGETFREE") == 0 || strcmp(extraname, "POSITIONFREE") == 0 || strcmp(extraname, "POSITIONTARGET") == 0)
	{
		if (strcmp(extraname, "POSITION") == 0)
			ctype = CameraPosition;
		else if (strcmp(extraname, "TARGET") == 0)
			ctype = CameraTarget;
		else if (strcmp(extraname, "TARGETFREE") == 0)
			ctype = CameraTargetFree;
		else if (strcmp(extraname, "POSITIONFREE") == 0)
			ctype = CameraPositionFree;
		else if (strcmp(extraname, "POSITIONTARGET") == 0)
			ctype = CameraPositionTarget;

		len2 = (int)(strlen(target) - strlen(extraname) -1);
	}
	else
		ctype = NoCamera;

	strcpy(result, target);
	if (len2)
		result[len2] = 0;

	return;
}

long CProcessTargetObject::GetTargetKey() { 
	if (m_Key == INVALID_KEY && m_CameraType != NoCamera)
		m_Key = 0;
	return m_Key; 
}
void CProcessTargetObject::GetInsPathAndPMIIdByResolvedPath(char* plcIdPath,char* pmiId) { 
	strcpy(pmiId,"");
	if(strcmp(GetType(),TARGETOBJECTTYPE_PMI) == 0)
	{
		//将PMI所属实例的配置路径及PMIid从target中分离
		char tempPlcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
		strcpy(tempPlcIdPath,GetResolvedPath());
		int nLastCharIdx = -1;
		for(int i = (int)strlen(tempPlcIdPath) -1 ; i >= 0; i--)
		{
			if(tempPlcIdPath[i]=='\\')
			{
				nLastCharIdx = i;
				break;
			}
		}

		if(nLastCharIdx>0)
		{
			//strcpy_s(plcIdPath,nLastCharIdx,tempPlcIdPath);
			string strTempPlcPath = tempPlcIdPath;
			strTempPlcPath = strTempPlcPath.substr(0,nLastCharIdx);
			strcpy(plcIdPath,strTempPlcPath.c_str());
			strcat(plcIdPath,"\0");
			strcat(pmiId,&tempPlcIdPath[nLastCharIdx+1]);
		}
		else
		{
			strcpy(plcIdPath,"");
			strcpy(pmiId,tempPlcIdPath);
		}
	}
}



CProcess::CProcess(const int ID,const int nBehaviorActionID,const char *name)
{
	m_TargetObjectList = new_vlist( malloc, free );
	m_ID = ID;
	if(name)
	{
		strcpy(m_Name, name);
	}
	else
	{
		sprintf(m_Name,"步骤%d",ID+1);
#ifndef SVIEW_DESIGNER 
#ifdef WIN32
 		SA_UTF8 strName(m_Name);
		strcpy(m_Name, (const char*)strName.encodedText());
#endif
#endif
	}
	strcpy(m_Desc, "");

	m_nBehaviorActionID = nBehaviorActionID;
	m_bPlay = true;
	////创建摄像机对象
	//CHAR strPlcID[MAX_PATH];
	//strcpy(strPlcID, TARGETOBJECTTYPE_CAM);
	//strcat(strPlcID, ":SCENE/TARGET");
	//CreateTargetObjectByPath("Camera",strPlcID);
	m_ReferenceCount = 0;
	m_pProcessManager = NULL;
	m_pBehaviorAction = NULL;
}

CProcess::~CProcess(void)
{
	CSBehaviorAction* pBehaviorAction = GetBehaviorAction();
	if(pBehaviorAction && 
		pBehaviorAction->GetSimulationAnimationManager())
	{
		pBehaviorAction->GetSimulationAnimationManager()->DeleteSimAni(pBehaviorAction);
	}
	DeleteAllTargetObject();
	delete_vlist(m_TargetObjectList);
}

void CProcess::Reference()
{
	m_ReferenceCount++;
	if(m_ReferenceCount>1 &&
		GetBehaviorAction())
	{
		GetBehaviorAction()->Reference();
	}
}
void CProcess::Release()
{
	m_ReferenceCount--;
	
	if (m_ReferenceCount <= 0)
	{
		delete this;
	}
	else
	{
		if (GetBehaviorAction())
		{
			GetBehaviorAction()->Release();
		}
	}
}

void CProcess::SetName(const char* name)
{
	if(!name)
	{
		return;
	}
	if(strlen(name)>(SA_BUFFER_SIZE_SMALL-1))
	{
		strcpy(m_Name, name);
	}
	else
	{
		strcpy(m_Name, name);
	}
	if(GetBehaviorAction())
	{
		GetBehaviorAction()->SetName(name);
	}
}
void CProcess::SetDesc(const char* desc)
{
	if(!desc)
	{
		return;
	}
	if(strlen(desc)>(SA_BUFFER_SIZE_BIG-1))
	{
		strcpy(m_Desc,desc);
	}
	else
	{
		strcpy(m_Desc, desc);
	}

}

void CProcess::Serialize(CUtilityXMLGenerator *xmlgen)
{
	CUtilityXMLTag xmlt;
	xmlt.SetTagname("Process");
	xmlt.AddProperty("Name", m_Name, true);
	xmlt.AddProperty("Desc", m_Desc, true);
	xmlt.AddPropertyInt("ID", m_ID);
	xmlt.AddPropertyInt("BehavivorManagerID", m_nBehaviorActionID);
	xmlt.SetOpen(true);
	xmlgen->AddTag(&xmlt);

	vlist_reset_cursor(m_TargetObjectList);
	

	while (1)
	{
		CProcessTargetObject *temp;
		//xmlgenSA.Reset();
		if (!(temp = (CProcessTargetObject *)vlist_peek_cursor(m_TargetObjectList)))
			break;
		temp->Serialize(xmlgen);
		
		vlist_advance_cursor(m_TargetObjectList);	
	}
	//xmlgen->AddTag(&xmltSA);
	xmlgen->CloseTag(&xmlt);
}


CProcessTargetObject * CProcess::CreateTargetObjectByPath(const char *name, const char *path)
{

	CProcessTargetObject *tobject;
	if (!(tobject = FindTargetObjectByPath(path)))
	{
		tobject = new CProcessTargetObject(this, name, path);
		AddTargetObject(tobject);
	}
	return tobject;
}

CProcessTargetObject * CProcess::FindTargetObjectByName(const char *name)
{
	vlist_reset_cursor(m_TargetObjectList);
	while (1)
	{
		CProcessTargetObject *temp;
		if (!(temp = (CProcessTargetObject *)vlist_peek_cursor(m_TargetObjectList)))
			break;
		if (strcmp(temp->GetName(), name) == 0)
			return temp;
		vlist_advance_cursor(m_TargetObjectList);	
	}
	return 0;
}



CProcessTargetObject * CProcess::FindTargetObjectByPath(const char *path)
{
	vlist_reset_cursor(m_TargetObjectList);
	while (1)
	{
		CProcessTargetObject *temp;
		if (!(temp = (CProcessTargetObject *)vlist_peek_cursor(m_TargetObjectList)))
			break;
		if (temp->IsEqual(path))
			return temp;
		vlist_advance_cursor(m_TargetObjectList);	
	}
	return 0;
}


void CProcess::DeleteTargetObject( CProcessTargetObject *tinfo )
{
	if(NULL != tinfo)
	{
		vlist_remove(m_TargetObjectList, tinfo);
		delete tinfo;
	}
}

void CProcess::DeleteAllTargetObject()
{
	START_LIST_ITERATION(CProcessTargetObject, m_TargetObjectList)
		delete temp;
	END_LIST_ITERATION(m_TargetObjectList)

	vlist_flush(m_TargetObjectList);
}

void CProcess::AddTargetObject(CProcessTargetObject *tinfo)
{

	vlist_reset_cursor(m_TargetObjectList);
	vlist_add_last(m_TargetObjectList, (void *)tinfo);
}

void CProcess::UpdateTargetList(std::vector<TARGETOBJECTINFO*>& vecTarget)
{
	DeleteAllTargetObject();
	//根据传入的对象状态判断哪些是相对初始状态变化过的，记录变化过的对象
	CSimulationAnimationManager* pSAManager = GetSimulationAnimationManager();
	if(!pSAManager) return;
	if(pSAManager->GetInitTargetObjectCount() <= 0) return;

	//遍历对象列表
	for(int i = 0;i < (int)vecTarget.size(); i++)
	{
		TARGETOBJECTINFO* targetObjectInfo = vecTarget[i];
		if(!targetObjectInfo)
			continue;
		CProcessTargetObject* pProcessTargetObject = NULL;
		//对象是否在初始化对象列表中存在
		if((pProcessTargetObject = pSAManager->FindInitTargetObjectByPath(targetObjectInfo->m_Path)))
		{
			AniPoint pntPos(targetObjectInfo->m_Pos[0],targetObjectInfo->m_Pos[1],targetObjectInfo->m_Pos[2]);
			AniPoint pntScale(targetObjectInfo->m_Scale[0],targetObjectInfo->m_Scale[1],targetObjectInfo->m_Scale[2]);
			AniQuat quat(targetObjectInfo->m_Quat[0],targetObjectInfo->m_Quat[1],targetObjectInfo->m_Quat[2],targetObjectInfo->m_Quat[3]);
			//显示状态是否改变的判断
			if(targetObjectInfo->m_bVisible == pProcessTargetObject->GetVisible())
			{
				//显示状态不变
				//隐藏状态下的单独处理
				if(TARGETOBJECT_TYPE_INS == targetObjectInfo->m_Type ||TARGETOBJECT_TYPE_CAM == targetObjectInfo->m_Type)
				{
					//零件或摄像机
					if(pntPos.Equal(pProcessTargetObject->GetPos(), (float)ANIMATION_D_TOL) && 
						quat.Equal(pProcessTargetObject->GetQuat(), (float)ANIMATION_D_TOL2) &&
						pntScale.Equal(pProcessTargetObject->GetScale(), (float)ANIMATION_D_TOL2)&&
						(targetObjectInfo->m_Trans == pProcessTargetObject->GetTrans() || targetObjectInfo->m_Trans == -1.0))
					{
						continue;
					}
				}else if(TARGETOBJECT_TYPE_PMI == targetObjectInfo->m_Type && 
					(targetObjectInfo->m_Trans == pProcessTargetObject->GetTrans() || targetObjectInfo->m_Trans == -1.0))
				{
					//PMI
					continue;
				}
			}

			//创建新的对象 并添加到链表中
			CProcessTargetObject* pNewTargetObject = new CProcessTargetObject(this, 
				targetObjectInfo->m_Name, targetObjectInfo->m_Path, &pntPos, 
				&quat, &pntScale, targetObjectInfo->m_bVisible, targetObjectInfo->m_Trans);
			AddTargetObject(pNewTargetObject);

		}else
		{
			//图片只放入步骤中
			if(targetObjectInfo->m_Type == TARGETOBJECT_TYPE_IMAGE)
			{
				AniPoint pntPos(targetObjectInfo->m_Pos[0],targetObjectInfo->m_Pos[1],targetObjectInfo->m_Pos[2]);
				AniPoint pntScale(targetObjectInfo->m_Scale[0],targetObjectInfo->m_Scale[1],targetObjectInfo->m_Scale[2]);
				AniQuat quat(targetObjectInfo->m_Quat[0],targetObjectInfo->m_Quat[1],targetObjectInfo->m_Quat[2],targetObjectInfo->m_Quat[3]);
				CProcessTargetObject* pNewTargetObject = new CProcessTargetObject(this, 
					targetObjectInfo->m_Name, targetObjectInfo->m_Path, &pntPos, 
					&quat, &pntScale, targetObjectInfo->m_bVisible, targetObjectInfo->m_Trans);
				AddTargetObject(pNewTargetObject);
			}
			else
			{
				//将没有查找到的对象加入到初始化列表
				pSAManager->AddInitTargetObject(*targetObjectInfo);
			}
		}
	}
}

CProcessTargetObject * CProcess::FindTargetObjectByID( const int ID )
{
	START_LIST_ITERATION(CProcessTargetObject, m_TargetObjectList)
	if(ID == temp->GetTargetKey())
	{
		return temp;
	}
	END_LIST_ITERATION(m_TargetObjectList)
	return NULL;
}

void * CProcess::XMLCallback( CUtilityXMLTag *xt, bool open, void *m_pExtraData )
{
	if(!xt->GetXMLParser()->GetStackData())
	{
		return NULL;
	}
	CProcessManager *pProcessManager = (CProcessManager *)xt->GetXMLParser()->GetStackData();
	CProcess *pProcess =0;
	/*char*	strSABuffer = NULL;*/
	if(open)
	{
		char Name[SA_BUFFER_SIZE_SMALL] = {0};
		char Desc[SA_BUFFER_SIZE_BIG] = {0};
		int ID = 0;
		int BehaviorActionID = 0;

		
		if (!xt->GetProperty("ID", ID))
			ID = 0;
		if (!xt->GetProperty("Name", Name))
			sprintf(Name,"步骤%d",ID+1);
		if (!xt->GetProperty("Desc", Desc))
			strcpy(Desc, "");
		if (!xt->GetProperty("BehavivorManagerID", BehaviorActionID))
			BehaviorActionID = 0;
		
		pProcess = pProcessManager->AddProcess(ID,Name,false);
		pProcess->SetBehaviorActionID(BehaviorActionID);
		//将默认步骤名改为动画名称
		if(pProcess->GetBehaviorAction())
		{
			strcpy(Name, pProcess->GetBehaviorAction()->GetName());
			if(strcmp(Name, ""))
			{
				pProcess->SetName(Name);
			}
		}
		pProcess->SetDesc(Desc);
		return pProcess;
	}
	else
	{

	}
	return pProcess;
}

int CProcess::GetMaxTargetObjectID()
{
	int iMaxID = 0;
	int itmpID = 0;
	START_LIST_ITERATION(CProcessTargetObject, m_TargetObjectList)
		itmpID = temp->GetTargetKey();
	if(itmpID > iMaxID)
		iMaxID = itmpID;
	END_LIST_ITERATION(m_TargetObjectList)
		return iMaxID;
}

void CProcess::StartListTargetObjectQuery()
{
	vlist_reset_cursor(m_TargetObjectList);
}

bool CProcess::GetNextListTargetObject(CProcessTargetObject** opTargetObject)
{
	void *temp;
	if (!(temp = vlist_peek_cursor(m_TargetObjectList)))
		return false;
	*opTargetObject = (CProcessTargetObject *)temp;
	vlist_advance_cursor(m_TargetObjectList);	
	return true;
}

bool CProcess::HasListTargetObject()
{
	vlist_reset_cursor(m_TargetObjectList);
	if (!vlist_peek_cursor(m_TargetObjectList))
		return false;
	else 
		return true;
}
int CProcess::GetTargetObjectCount() 
{ 
	return m_TargetObjectList->count; 
}

CProcessTargetObject* CProcess::GetTargetObjectByKey(long key)
{
	CProcessTargetObject* pTarget = NULL;
	START_LIST_ITERATION(CProcessTargetObject, m_TargetObjectList)
		if(temp->GetTargetKey() == key)
		{
			pTarget = temp;
			break;
		}
		END_LIST_ITERATION(m_TargetObjectList)
			return pTarget;
}

long CProcess::RegisterTargetObjectKey()
{
	long iRet = 0;
	START_LIST_ITERATION(CProcessTargetObject, m_TargetObjectList)
		if(temp->GetTargetKey() >= iRet)
		{
			iRet = temp->GetTargetKey()+1;
		}
		END_LIST_ITERATION(m_TargetObjectList)
			return iRet;
}

void NS_SimulationAnimation::CProcess::SetProcessManager(CProcessManager* pProcessManager)
{
	m_pProcessManager = pProcessManager;
	m_pBehaviorAction = NULL;
	GetBehaviorAction();
}

void NS_SimulationAnimation::CProcess::SetBehaviorActionID(const int ID)
{
	m_nBehaviorActionID = ID;
	m_pBehaviorAction = NULL;
	GetBehaviorAction();
}

CSBehaviorAction* CProcess::GetBehaviorAction()
{
	if(!m_pBehaviorAction &&
		m_pProcessManager &&
		m_pProcessManager->GetAnimationStepManager() &&
		m_pProcessManager->GetAnimationStepManager()->GetSimulationAnimationManager())
	{
		m_pBehaviorAction = m_pProcessManager->GetAnimationStepManager()->GetSimulationAnimationManager()->FindSimAniByID(m_nBehaviorActionID);
	}
	return m_pBehaviorAction;
}

CSimulationAnimationManager* CProcess::GetSimulationAnimationManager()
{
	CAnimationStepManager* pAnimationStepManager = NULL;
	if(GetProcessManager())
		pAnimationStepManager = GetProcessManager()->GetAnimationStepManager();
	if(!pAnimationStepManager)
	{
		return NULL;
	}
	return pAnimationStepManager->GetSimulationAnimationManager();
}

CAnimationStepManager* CProcess::GetAnimationStepManager()
{
	CAnimationStepManager* pAnimationStepManager = NULL;
	if(GetProcessManager())
		pAnimationStepManager = GetProcessManager()->GetAnimationStepManager();
	return pAnimationStepManager;

}

CProcessTargetObject * CProcess::GetCameraTargetObject(bool bCreate)
{
	//创建摄像机对象
	CHAR strPlcID[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(strPlcID, TARGETOBJECTTYPE_CAM);
	strcat(strPlcID, ":SCENE/TARGET");
	CProcessTargetObject * pTargetObject = FindTargetObjectByPath(strPlcID);
	if(!pTargetObject && bCreate)
	{
		pTargetObject = CreateTargetObjectByPath("Camera",strPlcID);
	}
	return pTargetObject;
}

bool CProcess::GetCamera(AniPoint& pos,AniQuat& rotation, AniPoint& scale)
{
	bool bRet = false;
	pos.Set(0.0f,0.0f,0.0f);
	rotation.Set(0.0f,0.0f,0.0f,0.0f);
	scale.Set(0.0f,0.0f,0.0f);

	CSBehaviorAction* pBehaviorAction = GetBehaviorAction();
	if(pBehaviorAction)
	{
		CHAR strPlcID[SA_BUFFER_SIZE_SMALL] = {0};
		strcpy(strPlcID, TARGETOBJECTTYPE_CAM);
		strcat(strPlcID, ":SCENE/TARGET");

		CSAnimation* pAnimation = pBehaviorAction->FindAnimation(strPlcID,INTERPOLATOR_POS);
		if(pAnimation && pAnimation->GetTimeline()  && pAnimation->GetTimeline()->GetTimelineArrayLength()>0)
		{
			int nTick = 0;
			if(GetSimulationAnimationManager()->GetReversePlay())
			{
				nTick = pAnimation->GetTimeline()->GetTimelineArrayLength() - 1;
			}
			
			vlist_t* pInterpolatorList = pAnimation->GetInterpolatorList();
			START_LIST_ITERATION(CSInterpolator, pInterpolatorList)
				if(strcmp(temp->GetType(),INTERPOLATOR_POS) == 0)
				{
					pos = ((CKeyframeChannel*)temp->GetAt(nTick))->m_cp;
				}
				else if(strcmp(temp->GetType(),INTERPOLATOR_QUATROT) == 0)
				{
					rotation = ((CKeyframeQuatSquad*)temp->GetAt(nTick))->m_quat;
				}
				else if(strcmp(temp->GetType(),INTERPOLATOR_SCALE) == 0)
				{
					scale = ((CKeyframeChannel*)temp->GetAt(nTick))->m_cp;
				}
			END_LIST_ITERATION(pInterpolatorList)

			float fCurMtxAA[4][4];
			rotation.ToMatrix((float*)fCurMtxAA);

			fCurMtxAA[3][0] = pos.x;
			fCurMtxAA[3][1] = pos.y;
			fCurMtxAA[3][2] = pos.z;

			MatrixOperation::MatrixInversion((float*)fCurMtxAA);
			rotation = AniQuat::MatrixToQuaternion((float*)fCurMtxAA);
			pos.x = fCurMtxAA[3][0];
			pos.y = fCurMtxAA[3][1];
			pos.z = fCurMtxAA[3][2];

			bRet = true;
		}
	}
	if(!bRet)
	{
		CProcessTargetObject * pTargetObject = GetCameraTargetObject(false);
		if(pTargetObject)
		{
			pos = pTargetObject->GetPos();
			rotation = pTargetObject->GetQuat();
			scale = pTargetObject->GetScale();
			bRet = true;
		}
	}
	
	return bRet;
}

void CProcess::SetCamera(AniPoint& pos,AniQuat& rotation, AniPoint& scale)
{
	CProcessTargetObject * pTargetObject = GetCameraTargetObject(true);
	if(pTargetObject)
	{
		pTargetObject->SetPos(pos.x,pos.y,pos.z);
		pTargetObject->SetQuat(rotation.x,rotation.y,rotation.z,rotation.w);
		pTargetObject->SetScale(scale.x,scale.y,scale.z);
	}
}

void CProcess::UpdateView(bool bUpdateModel/* = true*/, bool bUpdateCam/* = false*/, bool bCamAni/* = false*/)
{
	CAnimationStepManager* pAnimationStepManager = GetProcessManager()->GetAnimationStepManager();
	if(!pAnimationStepManager)
	{
		return;
	}
	CSimulationAnimationManager* pSAManager = GetProcessManager()->GetAnimationStepManager()->GetSimulationAnimationManager();
	//根据过程更新视口时，暂屏蔽播放场景功能，防止场景动画与过程的场景冲突
	bool bPlayCamera = pSAManager->IsCameraPlay();
	pSAManager->SetCameraPlay(false);
	if(pSAManager->IsPlaying())
	{
		pSAManager->StopAll();
	}

	//if(!pSAManager->IsPlaying())
	//{
		//根据当前过程记录的对象状态更新视口
		
		//创建临时vector 用于存储当前步骤的状态信息
		vector<PTARGETOBJECTINFO> vecTargetObjectInfo;

		vlist_s* pInitTargetObjectList = pSAManager->GetInitTargetObjectList();
		//遍历初始化对象列表
		CProcessTargetObject* pProcessTargetObject = NULL;
		for(int i = 0;i < (int)vlist_count(pInitTargetObjectList); i++)
		{
			pProcessTargetObject = (CProcessTargetObject*)vlist_nth_item(pInitTargetObjectList, i);
			SACameraType cameraType = pProcessTargetObject->GetCameraType();
			if((cameraType == NoCamera && !bUpdateModel) || 
				(cameraType != NoCamera && !bUpdateCam) || 
				(cameraType != NoCamera && bUpdateCam && bCamAni))
			{
				//若需用判断具体类型 用以下方法实现
				//if(strcmp(pProcessTargetObject->GetType(),TARGETOBJECTTYPE_INS)==0)
					continue;
			}
			//根据是否能在在步骤中找到该对象 选择初始化的来源
			CProcessTargetObject* pCurProcessTargetObject = NULL;
			if((pCurProcessTargetObject = FindTargetObjectByID(pProcessTargetObject->GetTargetKey())))
			{
				TARGETOBJECTINFO* targetObjectInfo = new TARGETOBJECTINFO;
				TargetObject2StructInfo(pCurProcessTargetObject, targetObjectInfo);
				vecTargetObjectInfo.push_back(targetObjectInfo);
			}
			else
			{
				TARGETOBJECTINFO* targetObjectInfo = new TARGETOBJECTINFO;
				TargetObject2StructInfo(pProcessTargetObject, targetObjectInfo);
				vecTargetObjectInfo.push_back(targetObjectInfo);
			}
		}
		//初始列表中没有的对象 如图片类型
		START_LIST_ITERATION(CProcessTargetObject, m_TargetObjectList)
			if(strcmp(temp->GetType(), TARGETOBJECTTYPE_IMAGE) == 0)
			{
				TARGETOBJECTINFO* targetObjectInfo = new TARGETOBJECTINFO;
				TargetObject2StructInfo(temp, targetObjectInfo);
				vecTargetObjectInfo.push_back(targetObjectInfo);
			}
		END_LIST_ITERATION(m_TargetObjectList)


		//调用更新当前视口中模型状态的回调函数
		if(pSAManager->GetAnimationPlayApi() && vecTargetObjectInfo.size()>0)
		{
			if (bUpdateModel)
			{
				pSAManager->GetAnimationPlayApi()->SetTargetState(vecTargetObjectInfo);
			}
		}

		//清空临时vector
		for(int i = 0;i < (int)vecTargetObjectInfo.size();i++)
			delete vecTargetObjectInfo[i];
		vecTargetObjectInfo.clear();

		if(bUpdateModel || bUpdateCam)
		{
			if(bUpdateCam && !bCamAni)
			{
				pSAManager->SetCameraPlay(true);
			}
			CSBehaviorAction* pBehaviorAction = GetBehaviorAction();
			if(pBehaviorAction && pAnimationStepManager->GetPlayMode() != CAnimationStepManager::PLAY_MODE_NONE)
			{
				if(pSAManager->GetReversePlay())
				{
					pBehaviorAction->RewindReverse();
				}
				else
				{
					pBehaviorAction->Rewind();
				}
			}
		}
	//}
	pSAManager->SetCameraPlay(bPlayCamera);

	//开始切换场景
	if(bUpdateCam && bCamAni)
	{
		pAnimationStepManager->StartChangeCamera(this,true);
	}
}

void CProcess::TargetObject2StructInfo(CProcessTargetObject*pProcessTargetObject, TARGETOBJECTINFO* pTargetObjectInfo)
{
	if(pProcessTargetObject)
	{
		strcpy(pTargetObjectInfo->m_Name, pProcessTargetObject->GetName());
		pTargetObjectInfo->m_ID = (int)pProcessTargetObject->GetTargetKey();
		strcpy(pTargetObjectInfo->m_Path, pProcessTargetObject->GetPath());
		if(strcmp(pProcessTargetObject->GetType(), TARGETOBJECTTYPE_INS)==0)
		{
			strcpy(pTargetObjectInfo->m_InsPath, pProcessTargetObject->GetResolvedPath());
			pTargetObjectInfo->m_EntId = 0;
		}
		else if(strcmp(pProcessTargetObject->GetType(), TARGETOBJECTTYPE_PMI)==0)
		{
			char szPMIId[SA_BUFFER_SIZE_SMALL] = "";
			pProcessTargetObject->GetInsPathAndPMIIdByResolvedPath(pTargetObjectInfo->m_InsPath, szPMIId);
			pTargetObjectInfo->m_EntId = atoi(szPMIId);
		}
		pTargetObjectInfo->m_Pos[0] = pProcessTargetObject->GetPos()->x;
		pTargetObjectInfo->m_Pos[1] = pProcessTargetObject->GetPos()->y;
		pTargetObjectInfo->m_Pos[2] = pProcessTargetObject->GetPos()->z;
		pTargetObjectInfo->m_Quat[0] = pProcessTargetObject->GetQuat()->x;
		pTargetObjectInfo->m_Quat[1] = pProcessTargetObject->GetQuat()->y;
		pTargetObjectInfo->m_Quat[2] = pProcessTargetObject->GetQuat()->z;
		pTargetObjectInfo->m_Quat[3] = pProcessTargetObject->GetQuat()->w;
		pTargetObjectInfo->m_Scale[0] = pProcessTargetObject->GetScale()->x;
		pTargetObjectInfo->m_Scale[1] = pProcessTargetObject->GetScale()->y;
		pTargetObjectInfo->m_Scale[2] = pProcessTargetObject->GetScale()->z;
		pTargetObjectInfo->m_bVisible = pProcessTargetObject->GetVisible();
		pTargetObjectInfo->m_Trans = pProcessTargetObject->GetTrans();
		if(strcmp(pProcessTargetObject->GetType(), TARGETOBJECTTYPE_INS)==0)
			pTargetObjectInfo->m_Type = TARGETOBJECT_TYPE_INS;
		else if(strcmp(pProcessTargetObject->GetType(), TARGETOBJECTTYPE_CAM)==0)
			pTargetObjectInfo->m_Type = TARGETOBJECT_TYPE_CAM;
		else if(strcmp(pProcessTargetObject->GetType(), TARGETOBJECTTYPE_PMI)==0)
			pTargetObjectInfo->m_Type = TARGETOBJECT_TYPE_PMI;
		else if(strcmp(pProcessTargetObject->GetType(), TARGETOBJECTTYPE_IMAGE)==0)
			pTargetObjectInfo->m_Type = TARGETOBJECT_TYPE_IMAGE;
	}
}

CProcess* CProcess::Clone(CProcessManager* pParentProcessManager)
{
	//克隆功能 实现步骤的拷贝复制
	CProcess* pNewProcess = NULL;
	if (!pParentProcessManager)
		return pNewProcess;
	CProcessManager* pProcessManager = GetProcessManager();
	if(!pProcessManager)
		return pNewProcess;
	CAnimationStepManager* pAnimationStepManager = pProcessManager->GetAnimationStepManager();
	if(!pAnimationStepManager)
		return pNewProcess;
	CSimulationAnimationManager* pSAManager =  pAnimationStepManager->GetSimulationAnimationManager();
	if(!pSAManager)
		return pNewProcess;
	pNewProcess = new CProcess(pParentProcessManager->RegisterProcessID(), pSAManager->RegisterBehaviorActionID());
	if(!pNewProcess)
		return pNewProcess;
	pNewProcess->SetDesc(m_Desc);
	pNewProcess->SetProcessManager(pParentProcessManager);

	CProcess* pTmpProcess = pParentProcessManager->FindProcessByName(m_Name);
	int index = 1;
	string strName = m_Name;
	//循环查询
	while (pTmpProcess)
	{
		index++;
		strName = m_Name;
		strName += "(";
		char szIndex[32] = { 0 };
#ifdef WIN32
        _itoa_s(index, szIndex, 10);
#else
        sprintf(szIndex, "%d", index);
#endif

		strName += szIndex;
		strName += ")";
		pTmpProcess = pParentProcessManager->FindProcessByName(strName.c_str());
	}
	pNewProcess->SetName(strName.c_str());
	pParentProcessManager->AddProcess(pNewProcess);
	//动画
	CSBehaviorAction* pBehaviorAction = pSAManager->FindSimAniByID(GetBehaviorActionID());
	if(pBehaviorAction)
	{
		CSBehaviorAction* pNewBehaviorAction = pBehaviorAction->Clone();
		if(pNewBehaviorAction)
		{
			pNewProcess->SetBehaviorActionID(pNewBehaviorAction->GetID());
			pNewBehaviorAction->SetName(pNewProcess->GetName());
		}
	}

	START_LIST_ITERATION(CProcessTargetObject, m_TargetObjectList)
		if(temp)
		{
			CProcessTargetObject* pProcessTargetObject = pNewProcess->CreateTargetObjectByPath(temp->GetName(), temp->GetPath());
			if(pProcessTargetObject)
			{
				AniPoint* pos = temp->GetPos();
				AniQuat* quat = temp->GetQuat();
				AniPoint* scale = temp->GetScale();
				pProcessTargetObject->SetPos(pos->x, pos->y, pos->z);
				pProcessTargetObject->SetQuat(quat->x, quat->y, quat->z, quat->w);
				pProcessTargetObject->SetScale(scale->x, scale->y, scale->z);
				pProcessTargetObject->SetTrans(temp->GetTrans());
				pProcessTargetObject->SetVisible(temp->GetVisible());
			}
		}
		END_LIST_ITERATION(m_TargetObjectList)
	return pNewProcess;
}
SA_NAMESPACE_END


