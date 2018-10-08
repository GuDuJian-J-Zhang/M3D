// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		SBehaviorAction.cpp
*
*	@brief	活动管理器
*
*	@par	历史:
*
****************************************************************************/

#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <typeinfo>

#include "sview/views/View.h"

#include "SBehaviorAction.h"
#include "SAnimation.h"
#include "STimeline.h"
#include "SInterpolator.h"
#include "../SimulationCommon/UtilityXMLParser.h"
#include "../SimulationCommon/UtilityTextParser.h"
#include "AnimationStepManager.h"

#include "GroupManager.h"
#include "Group.h"
#include "GroupItem.h"
#include "../SimulationCommon/vlist.h"
#include "../SimulationCommon/vhash.h"
#include "SimulationAnimationManager.h"
#include "AnimationSound.h"


SA_NAMESPACE_BEGIN

void STargetObject::Serialize(CUtilityXMLGenerator *xmlgen)
{
	if (strcmp(m_Type, TARGETOBJECTTYPE_CAM) == 0 || 
		strcmp(m_Type, TARGETOBJECTTYPE_INS) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_PMI) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_CLIP) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_FOLDER) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_SOUND) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_IMAGE) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_ZOOM) == 0 ||
		strcmp(m_Type, TARGETOBJECTTYPE_GROUP) == 0 || 
		strcmp(m_Type, TARGETOBJECTTYPE_ARROW) == 0) 
	{
		CUtilityXMLTag xmlt;
		xmlt.SetTagname("TargetObject");
		if (strcmp(m_Name, "") != 0)
			xmlt.AddProperty("Name", m_Name, true);
		xmlt.AddPropertyInt("ID", m_key);

 		if (m_bSerializeFromKey && m_CameraType==NoCamera)
		{
	 		xmlt.AddProperty("Path", GetPath(), true);				

		}
		else
	 		xmlt.AddProperty("Path", GetPath(), true);	
		//char buffer[SA_BUFFER_SIZE_BIG];
		//SACLOCALE(sprintf(buffer, "[%f %f %f]", m_Pivot.x, m_Pivot.y, m_Pivot.z));
		//xmlt.AddProperty("Pivot", buffer, true);
		xmlt.AddPropertyInt("Collision",GetCollision());
		xmlt.SetOpen(false);
		xmlgen->AddTag(&xmlt);
	}
}



 
void *STargetObject::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};
	char Path[SA_BUFFER_SIZE_SMALL] = {0};
	char Pivot[SA_BUFFER_SIZE_SMALL] = {0};
	int key = 0;
	AniPoint pv;
	bool collision = false;
	if (open)
	{
		CSBehaviorAction *outerBehaviorAction = (CSBehaviorAction *)xt->GetXMLParser()->GetStackData();
		if(NULL != outerBehaviorAction)
		{
			if (!xt->GetProperty("ID", key))
				key = 0;
			if (!xt->GetProperty("Name", Name))
				strcpy(Name, "");
			if (!xt->GetProperty("Path", Path))
				strcpy(Path, "");
			if (!xt->GetProperty("Pivot", Pivot))
				pv.Set(0,0,0);
			else
				CUtilityXMLParser::GetFloatPoint(Pivot, pv);

			if (!xt->GetProperty("Collision", collision))
					 collision = false;

			STargetObject *to = NULL;
			if( SATools::VersionCompare("2.1",outerBehaviorAction->GetSimulationAnimationManager()->GetVersion()) > 0)
			{
				char Path10[SA_BUFFER_SIZE_SMALL] = {0};
				PlcPathToVersion2_1(Path,Path10);
				to = outerBehaviorAction->CreateTargetObjectByPath(Name, Path10);
			}
			else
			{
				to = outerBehaviorAction->CreateTargetObjectByPath(Name, Path);
			}
			
			to->SetPivot(pv);
			to->SetCollision(collision);
			
			to->SetQuat(0.0,0.0,0.0,1.0);
			to->SetPrePivot(0,0,0);
			to->SetPreQuat(0.0,0.0,0.0,1.0);
			to->SetTargetKey(key);
		}
	}
	return 0;
}

void *STargetObject::XMLCallback_V1(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};
	char Path[SA_BUFFER_SIZE_SMALL] = {0};
	char Pivot[SA_BUFFER_SIZE_SMALL] = {0};
	AniPoint pv;
	bool collision = false;
	if (open)
	{
		CSBehaviorAction *outerBehaviorAction = (CSBehaviorAction *)xt->GetXMLParser()->GetStackData();
		if(NULL != outerBehaviorAction)
		{
			if (!xt->GetProperty("Name", Name))
				strcpy(Name, "");
			if (!xt->GetProperty("Path", Path))
				strcpy(Path, "");
			if (!xt->GetProperty("Pivot", Pivot))
				pv.Set(0,0,0);
			else
				CUtilityXMLParser::GetFloatPoint(Pivot, pv);

			if (!xt->GetProperty("Collision", collision))
				collision = false;

			//如果是动画是1.1版本，将对象路径转换为新格式路径
			if(SATools::VersionCompare("2.0",outerBehaviorAction->GetVersion()) > 0)
			{
				if(strstr(Path,"PLCID"))
				{
					char cNewPath10[SA_BUFFER_SIZE_SMALL] = {0};
					if(strlen(Path) >8 && strstr(Path,"PLCID:0"))
					{
						SATools::PlcPath16To10(Path+8,cNewPath10);
					}
					else
					{
						SATools::PlcPath16To10(Path+6,cNewPath10);
					}

					char cNewPath[SA_BUFFER_SIZE_SMALL] = {0};
					strcpy(cNewPath,TARGETOBJECTTYPE_INS);
					strcat(cNewPath,":");
					strcat(cNewPath,cNewPath10);
					strcpy(Path,cNewPath);
				}

				if(strstr(Name,"PMI") && 
					!strstr(Path,TARGETOBJECTTYPE_PMI))
				{
					int nLastCharIdx = -1;
					for(int i = (int)strlen(Name) -1 ; i >= 0; i--)
					{
						if(Name[i]==':')
						{
							nLastCharIdx = i;
							break;
						}
					}
					char cNewPath[SA_BUFFER_SIZE_SMALL] = {0};
					strcpy(cNewPath,TARGETOBJECTTYPE_PMI);
					strcat(cNewPath,":");
					if(strlen(Path)>8 && strstr(Path,"PLCID:0"))
					{
						char cNewPath10[SA_BUFFER_SIZE_SMALL] = {0};
						SATools::PlcPath16To10(Path+8,cNewPath10);
						strcat(cNewPath,cNewPath10);
						strcat(cNewPath,"\\");
					}

					strcat(cNewPath,Name+nLastCharIdx + 1);
					strcpy(Path,cNewPath);
				}
			}

			STargetObject *to = outerBehaviorAction->CreateTargetObjectByPath(Name, Path);
			to->SetPivot(pv);
			to->SetCollision(collision);

			to->SetQuat(0.0,0.0,0.0,1.0);
			to->SetPrePivot(0,0,0);
			to->SetPreQuat(0.0,0.0,0.0,1.0);
		}
	}
	return 0;
	
}


//将之前动画版本转换为2.1的版本,将实例的配置路径从16进制改为10进制
void STargetObject::PlcPathToVersion2_1(const char* inPlcPath,char* outPlcPath)
{
	strcpy(outPlcPath,"");
	if(inPlcPath == 0 || 
		strlen(inPlcPath) == 0)
	{
		return;
	}
	char plcPath[SA_BUFFER_SIZE_SMALL] = {0};
	char targetType[SA_BUFFER_SIZE_SMALL] = {0};
	char ResolvedPath[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy( plcPath,  inPlcPath);

	char *cPlcIdP = NULL;
	cPlcIdP = strtok(plcPath,":");
	strcpy(targetType, cPlcIdP);
	cPlcIdP = strtok(NULL,":");
	strcpy(ResolvedPath, cPlcIdP);

	if(strcmp(targetType,TARGETOBJECTTYPE_PMI) == 0)
	{
		int nLastCharIdx = -1;
		for(int i = (int)strlen(ResolvedPath) -1 ; i >= 0; i--)
		{
			if(ResolvedPath[i]=='\\')
			{
				nLastCharIdx = i;
				break;
			}
		}
		
		char insPath16[SA_BUFFER_SIZE_SMALL] = {0};
		char pmiId[SA_BUFFER_SIZE_SMALL] = {0};
		if(nLastCharIdx>=0)
		{
			//strcpy_s(insPath16,nLastCharIdx,ResolvedPath);
			string strTempPlcPath = ResolvedPath;
			strTempPlcPath = strTempPlcPath.substr(0,nLastCharIdx);
			strcpy(insPath16,strTempPlcPath.c_str());
			strcpy(pmiId,&ResolvedPath[nLastCharIdx+1]);
		}
		else
		{
			strcpy(insPath16,"");
			strcpy(pmiId,ResolvedPath);
		}
		
		char insPath10[SA_BUFFER_SIZE_SMALL] = {0};
		SATools::PlcPath16To10(insPath16,insPath10);
		strcpy(outPlcPath, targetType);
		strcat(outPlcPath,":");
		strcat(outPlcPath,insPath10);
		strcat(outPlcPath,pmiId);
	}
	else
	{
		char insPath10[SA_BUFFER_SIZE_SMALL] = {0};
		SATools::PlcPath16To10(ResolvedPath,insPath10);
		strcpy(outPlcPath, targetType);
		strcat(outPlcPath,":");
		strcat(outPlcPath,insPath10);
	}
}

STargetObject::STargetObject(CSBehaviorAction *BehaviorAction, const char *name, const char *path, AniPoint *pivot)
{
	m_pBehaviorAction = BehaviorAction;
	m_bSerializeFromKey = true;	
	if (name)
		strcpy(m_Name, name);
	else 
		strcpy(m_Name, "");

	if (path)
		strcpy(m_Path, path);
	else
		strcpy(m_Path, "");

	if (pivot)
		m_Pivot = pivot;
	else 
		m_Pivot.Set(0,0,0);

	m_Quat.Set(0.0,0.0,0.0,1.0);
	m_PrePivot.Set(0,0,0);
	m_PreQuat.Set(0.0,0.0,0.0,1.0);
	
	m_CameraType = NoCamera;

	if(path && strlen(path) < SA_BUFFER_SIZE_SMALL)
	{
		strcpy(m_Path, path);
	}
 	ResolveTarget(m_Path, m_ResolvedPath, m_Type, m_CameraType);

	m_bHasMoved = false;
	m_bCollision = false;
	m_key = 0;
	if(m_pBehaviorAction)
		m_key = m_pBehaviorAction->RegisterTargetObjectKey();

	m_eTargetType=AT_PART;
}

void STargetObject::CreateTargetString(const char *type, const char *path, char *result, bool addquotes)
{
	char temppath[SA_BUFFER_SIZE_SMALL] = {0};
	if(strlen(path) < SA_BUFFER_SIZE_SMALL)
	{
		strcpy(temppath, path);
	}
	strcpy(result, type);
	strcat(result, ":");

	strcat (result, temppath);
 	if (addquotes)
	{
		char result2[SA_BUFFER_SIZE_SMALL] = {0};
		sprintf(result2, "\"%s\"", result);
		strcpy(result, result2);
	}

}
	

bool STargetObject::IsEqual(const char *target)
{
	if(!strcmp(target, m_Path))
		return true;
	else
		return false;
}


void STargetObject::ResolveTarget(const char *in, char *result, char *TargetType, SACameraType &ctype)
{
	char target[SA_BUFFER_SIZE_SMALL] = {0};
	char temp[SA_BUFFER_SIZE_SMALL] = {0};
	if(in && strlen(in)<SA_BUFFER_SIZE_SMALL)
	{
		strcpy(target, in);
	}
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
	{
		strcpy(target,"");
	}
	else
	{
		strcpy(target,pTarget);
	}
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

long STargetObject::GetTargetKey() { 
	if (m_key == INVALID_KEY && m_CameraType != NoCamera)
		m_key = 0;
	return m_key; 
}
void STargetObject::GetInsPathAndPMIIdByResolvedPath(char* plcIdPath,char* pmiId) { 
	strcpy(pmiId,"");
	if(strcmp(GetType(),TARGETOBJECTTYPE_PMI) == 0 || strcmp(GetType(), TARGETOBJECTTYPE_ARROW) == 0)
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

CSBehaviorAction::CSBehaviorAction(int ID, int tps, int delay,const char *name, const char *version, const char *company) : CTimerClient()
{
	if (name)

		strcpy(m_Name, name);
	else
		strcpy(m_Name, "DefaultName");

 	if (version)
		strcpy(m_Version, version);
	else
		strcpy(m_Version, ANIMATION_VERSION);

	if (company)
		strcpy(m_Company, company);
	else
		strcpy(m_Company, "HoteamSoft");

	if(tps > 0)
		m_Tps = tps;
	else
		m_Tps = DEFAULT_TPS;
	m_Delay = delay;
	m_StartTime = 0.0f;
  	m_AnimationList = new_vlist( malloc, free );
  	m_ScheduledAnimationList = new_vlist( malloc, free );
	m_TargetObjectList = new_vlist(malloc, free);
	m_SensorList = new_vlist(malloc, free);
	//m_ProcessPMIList = new_vlist( malloc, free );
	m_CurrentTick = 0;
	m_PreTick = 0;
 	m_bPlaying = false;
	m_LastTick = 0;
	m_FirstTick = 0;
	m_bRenderEveryFrame = false;
	m_bUpdateCamera = true;
	m_bContinuousPlay = false;
	m_bCameraUpdated = false;
	m_bInfinitePlay = false;
	m_bReversePlay = false;
//	ZeroMemory(&m_AssemblyModelId, sizeof(IDENT));
	m_ID = ID;
	m_ReferenceCount = 0;
	m_bPlaybackIsInterrupted = false;
	m_bPlayRangeSign = false;
	m_fPlayBeginTick = 0.0;
	m_fPlayEndTick = 0.0;
	m_fCollisionTime = 0.0;
	//m_bCameraPlay = true;
	//m_bShowTrochoid = false;
	//IsPlaySound() = false;
	//m_bPlayVisible = true;
	//m_bPlayImage = false;
	//m_bPlayPosRot = true;
	//m_bPlayClip = false;
	//m_bPlayColor = true;
	//m_bCollisionCheck = false;
	//m_CollisionDelay = 100;
	//m_bStopPlayAfterCollision = false;

	//GetAnimationPlayApi() = new AnimationPlayApi();
 }

CSBehaviorAction::~CSBehaviorAction()
{
	DeleteAllAnimations();
	delete_vlist(m_AnimationList);
	delete_vlist(m_ScheduledAnimationList);
	delete_vlist(m_TargetObjectList);
	delete_vlist(m_SensorList);
	//delete_vlist(m_ProcessPMIList);
	//if(GetAnimationPlayApi())
	//{
	//	delete GetAnimationPlayApi();
	//	GetAnimationPlayApi() = NULL;
	//}
}


bool CSBehaviorAction::Transfer(INT type,const char* plcIdPath, const float fPivot[3], const float fMtxAA[4][4])
{
	try
	{
		if (IsPlayPosRot() && GetAnimationPlayApi())
		{
        	GetAnimationPlayApi()->Play(type,plcIdPath, fPivot, fMtxAA,GetView());
		}
		
	}
    catch(...)
    {

    }
    return true;
}

//显示隐藏
bool CSBehaviorAction::TransferVisible(const char* plcIdPath, const char* name,bool bOnOff, float fTran)
{
	try
	{
		if (GetAnimationPlayApi() && 
			((strstr(plcIdPath,"ZOOM")==NULL && IsPlayVisible()) || ( strstr(plcIdPath,"ZOOM") && IsCameraPlay())) )
		{
    		GetAnimationPlayApi()->PlayVisible(plcIdPath, name, bOnOff,fTran,GetView());
		}
	}
    catch(...)
    {

    }
    //

	// 表示更新
	return true;
}

//显示隐藏
bool CSBehaviorAction::TransferColor(const char* plcIdPath, const char* name,const float fColor[3])
{
	try
	{
		if (IsPlayColor() && GetAnimationPlayApi())
		{
			GetAnimationPlayApi()->PlayColor(plcIdPath, name, fColor,GetView());
		}

	}
	catch(...)
	{
	}
	//

	// 表示更新
	return true;
}

////回调主工程声音处理(为获取SVL的绝对Path而采用回调到主工程获得) Add By YL 2014.11.18
//bool CSBehaviorAction::TransferSound(const char* plcIdPath,bool bPlayFlg,float fTime)
//{
//	try
//	{
//		if(IsPlaying() && IsPlaySound() && GetAnimationPlayApi())	//正在播放动画...
//		{
//			GetAnimationPlayApi()->PlayAnimationSound(plcIdPath, bPlayFlg,fTime);
//		}
//
//	}
//	catch(...)
//	{
//	}
//
//	return true;
//}

//回调主工程图片处理(为获取SVL的绝对Path而采用回调到主工程获得)
bool CSBehaviorAction::TransferImage(const char* plcIdPath, const char* name, const float fImgPos[3], const float fImgScaleFactor[3], bool bOnOffFlg)
{
	try
	{
		if(GetAnimationPlayApi())	//正在播放动画...
		{
			GetAnimationPlayApi()->PlayAnimationImage(plcIdPath, name, fImgPos, fImgScaleFactor, bOnOffFlg,GetView());
		}

	}
	catch(...)
	{
	}

	// 表示更新
	return true;
}

bool CSBehaviorAction::TransferCamera(const char* plcIdPath,INT TrfType, const float fPivot[3], const float fMtxAA[4][4],INT iCamType)
{
	try
	{
		if(IsCameraPlay() && GetAnimationPlayApi())
		{
			GetAnimationPlayApi()->PlayCamera(plcIdPath, TrfType, fPivot, fMtxAA,iCamType,GetView());
		}
	}
    catch (...)
    {
	}

	// 表示更新
	return true;
}
/************************************************************************/
/* 创建剖面                                                                 */
/************************************************************************/
bool CSBehaviorAction::TransferClipPlane(INT type,const char* plcIdPath, const char* name, 
											const float fNormal[3], const float fPos[3], bool bOnOff)
{
	try
	{
		if (IsPlayClip() && GetAnimationPlayApi())
		{
			GetAnimationPlayApi()->PlayClipPlane(type,plcIdPath, name, fNormal, fPos, bOnOff,GetView());
		}
	}
	catch (...)
    {
	}

	// 表示更新
	return true;
}

void CSBehaviorAction::Rewind()
{
	if (IsPlaying())
		Stop();
	ScheduleAllAnimations(true);
	SetCurrentTick(GetFirstTick());
 	ExecuteAnimations(GetCurrentTick(), 0.0f);//Edit by ZYP		
	//GetModel()->Update();
}

void CSBehaviorAction::RewindReverse()
{
	if (IsPlaying())
		Stop();
	ScheduleAllAnimations(true);
	SetCurrentTick(GetLastTick());
	ExecuteAnimations(GetCurrentTick(), 0.0f);//Edit by ZYP		
	//GetModel()->Update();
}


void CSBehaviorAction::Play()
{	
	m_pSimulationAnimationManager->SetCurSAByID(m_ID);
	m_bPlaybackIsInterrupted = false;
	if (!IsPlaying())
	{
		ScheduleAllAnimations(true);	
		APP_Show_Time( &m_StartTime );  
		SetInterval( 0.01f );
		SetTimerType( TimerClientType_PeriodicSkip );
		SetNextRequest( m_StartTime + 0.01f );
		//Edit by Roc 
		m_bPlaying = true;
		m_LastTick = GetLastTick();
		m_FirstTick = GetFirstTick();

		CTimerManager::GetTimerManager()->RegisterClient( this );
		m_PreTick = m_CurrentTick;

		m_fCollisionTime = m_CurrentTick*GetTicksPerSecond();
		/*
		m_bPlaying = true;
		m_LastTick = GetLastTick();*/

	}
}

bool CSBehaviorAction::IsAtFinalTick()
{
	if ((GetLastTick()<=GetCurrentTick()) && GetCurrentTick()>0)
		return true;
	else
		return false;
}

bool CSBehaviorAction::IsAtStartTick()
{
	if (GetFirstTick() >= GetCurrentTick() )
		return true;
	else
		return false;
}

bool CSBehaviorAction::GetPlayBeginCamera(float *matrix,float* scale)
{	
	bool bRet = false;
	if (IsAtFinalTick())
		Rewind();
	if (IsPlaying())
		Stop();

	memset(matrix,0,sizeof(float)*16);
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[10] = 1;
	matrix[15] = 1;

	scale[0] = 1.0;
	scale[1] = 0.0;
	scale[2] = 0.0;

	AniPoint pntPos;
	AniQuat quat;
	AniPoint pntScale;

	CSAnimation* pCamAnimation = FindAnimation("CAMERA:SCENE/TARGET",INTERPOLATOR_POS,true);
	if (pCamAnimation)
	{
		int interval;
		float fraction;
		CSTimeline *pTimeLine = pCamAnimation->GetTimeline();
		pTimeLine->Evaluate(m_CurrentTick, interval, fraction);
		if(0 > fraction)
			fraction = 0;

		vlist_s* pInterpolatorList = pCamAnimation->GetInterpolatorList();
		START_LIST_ITERATION(CSInterpolator, pInterpolatorList)
		if(temp)
		{
			if(strcmp(temp->GetType(), INTERPOLATOR_POS) == 0 )
			{
				((CSInterpolatorPosition*)temp)->CalculatePos(interval, fraction, pntPos);
			}
			else if(strcmp(temp->GetType(), INTERPOLATOR_QUATROT) == 0 )
			{
				((CSInterpolatorQuatSquad*)temp)->CalculateQuat(interval, fraction, quat);
			}
			else if(strcmp(temp->GetType(), INTERPOLATOR_SCALE) == 0 )
			{
				((CSInterpolatorScale*)temp)->CalculatePos(interval, fraction, pntScale);
			}
		}
		END_LIST_ITERATION(pInterpolatorList)
		
		bRet = true;
	}

	if(bRet)
	{
		quat.ToMatrix((float*)matrix);
		matrix[12] = pntPos.x; matrix[13] = pntPos.y; matrix[14] = pntPos.z; 
		scale[0] = pntScale.x; scale[1] = pntScale.y; scale[2] = pntScale.z; 
	}

	return bRet;
}


void CSBehaviorAction::Continue()
{	
	m_pSimulationAnimationManager->SetCurSAByID(m_ID);
	if(GetAnimationPlayApi())
	{
		if(IsPlayCollision())
		{
			GetAnimationPlayApi()->InitColisionData(GetView());
		}
		GetAnimationPlayApi()->playBegin(GetView());
	}
	
	m_bPlaybackIsInterrupted = false;
	m_bReversePlay = false;
	if (IsAtFinalTick() || GetFirstTick() > GetCurrentTick() )
		Rewind();
	else if(IsAtStartTick())
	{
		if (IsPlaying())
			Stop();
		ScheduleAllAnimations(true);	
		ExecuteAnimations(GetCurrentTick(), 0.0f);	
	}
 	Play();	
	m_StartTime -= GetCurrentTick()/GetTicksPerSecond(); 
}

void CSBehaviorAction::ContinueReverse()
{	
	m_pSimulationAnimationManager->SetCurSAByID(m_ID);
	if(GetAnimationPlayApi())
	{
		if(IsPlayCollision())
		{
			GetAnimationPlayApi()->InitColisionData(GetView());
		}
		GetAnimationPlayApi()->playBegin(GetView());
	}

	m_bPlaybackIsInterrupted = false;
	m_bReversePlay = true;
	if (IsAtStartTick() || GetLastTick() < GetCurrentTick())
		RewindReverse();
	else if(IsAtFinalTick())
	{
		if (IsPlaying())
			Stop();
		ScheduleAllAnimations(true);
		ExecuteAnimations(GetCurrentTick(), 0.0f);	
	}
	Play();	
	m_StartTime -= (GetLastTick()-GetCurrentTick())/GetTicksPerSecond(); 
}


void CSBehaviorAction::Stop(bool bPlaybackIsInterrupted/* = false*/)
{
	m_bPlaybackIsInterrupted = bPlaybackIsInterrupted;
	if (IsPlaying() && GetAnimationPlayApi())
	{//modify
		CTimerManager::GetTimerManager()->UnRegisterClient( this ); 

		//停止播放声音
		PlaySoundSA(false);

		m_bPlaying = false;
		m_bCameraUpdated = false;
		GetAnimationPlayApi()->playEnd(GetView());
		SetReversePlay(false);
	}
}

void CSBehaviorAction::PlaySoundSA(bool bPlay/* = true*/)
{
		START_LIST_ITERATION(CSAnimation, m_AnimationList)
			if(strcmp(temp->GetTarget()->GetType(), TARGETOBJECTTYPE_SOUND)==0)
			{
				CSInterpolatorSound* pInterpolatorSound = (CSInterpolatorSound*)temp->GetInterpolator(INTERPOLATOR_SOUND);
				if(pInterpolatorSound && pInterpolatorSound->m_pSound)
				{
					char cSoundFilePath[SA_BUFFER_SIZE_SMALL] = {0}; //声音文件Path
					pInterpolatorSound->GetFilePathName(cSoundFilePath);
					//pInterpolatorSound->m_pSound->PlaySound((const __wchar_t*)SA_WCS(cSoundFilePath).encodedText(),bPlay,0);
				}
			}
		END_LIST_ITERATION(m_AnimationList)		
}

STargetObject * CSBehaviorAction::CreateTargetObjectByPath(const char *name, const char *path)
{

	STargetObject *tobject;
	if (!(tobject = FindTargetObjectByPath(path)))
	{
 		tobject = new STargetObject(this, name, path);
		AddTargetObject(tobject);
	}
	return tobject;
}

void CSBehaviorAction::AddTargetObject(STargetObject *tinfo)
{
 	
	vlist_reset_cursor(m_TargetObjectList);
	vlist_add_last(m_TargetObjectList, (void *)tinfo);
}

STargetObject * CSBehaviorAction::FindTargetObjectByName(const char *name)
{
 	
	vlist_reset_cursor(m_TargetObjectList);
 	while (1)
	{
		STargetObject *temp;
		if (!(temp = (STargetObject *)vlist_peek_cursor(m_TargetObjectList)))
				break;
		if (strcmp(temp->GetName(), name) == 0)
			return temp;
   		vlist_advance_cursor(m_TargetObjectList);	
	}
	return 0;
}



STargetObject * CSBehaviorAction::FindTargetObjectByPath(const char *path)
{
  	//Edit by Roc
	vlist_reset_cursor(m_TargetObjectList);
 	while (1)
	{
		STargetObject *temp;
		if (!(temp = (STargetObject *)vlist_peek_cursor(m_TargetObjectList)))
				break;
		if (temp->IsEqual(path))
			return temp;
   		vlist_advance_cursor(m_TargetObjectList);	
	}
	return 0;
}


void CSBehaviorAction::SetCurrentTick()
{
	float ctime;
	APP_Show_Time( &ctime);//must modify
	SetCurrentTickByTime(ctime);
}

void  CSBehaviorAction::SetCurrentTickByTime(float t) 
{ 
	if(!m_bReversePlay)
	{
		m_CurrentTick =  (t - m_StartTime) * GetTicksPerSecond(); 
	}
	else
	{
		m_CurrentTick =  GetLastTick() - (t - m_StartTime) * GetTicksPerSecond(); 
	}
}

void CSBehaviorAction::SetCurrentTick(float tick)
{
	/* if we are currently playing, adjust the start time to match 
	 * what we just set the tick to be */
	if (IsPlaying()) {
		m_StartTime += (m_CurrentTick - tick)/GetTicksPerSecond();
	}
	m_CurrentTick =  tick; 
}



void CSBehaviorAction::ScheduleAnimation(CSAnimation *animation, float currentframe)
{
	CSTimeline *ti = animation->GetTimeline();
	if (animation->GetLoop())
	    ti->SetStartTick( currentframe + animation->GetDelay() - ti->GetCurrentRelativeTick());
	else
	    ti->SetStartTick( currentframe + animation->GetDelay());

	animation->Reset();
	animation->SetDefaultActive(true);
	animation->SetExecuteOnce(false);
	vlist_remove(m_ScheduledAnimationList, animation);
 	vlist_add_last(m_ScheduledAnimationList, (void *)animation);
 
}
void CSBehaviorAction::ScheduleAnimationWithGroup(CSAnimation *animation, float currentframe)
{
	if (!animation)
	{
		return;
	}	
	STargetObject *pTarget=animation->GetTarget();
	if (pTarget&&strcmp(pTarget->GetType(), TARGETOBJECTTYPE_GROUP)==0)
	{
		//组动画
		CGroupManager *pGroupManager=CGroupManager::GetInstance();
		if (!pGroupManager)
		{
			return;
		}
		int nGroupID=atoi(pTarget->GetResolvedPath());
		CGroup *pGroup=pGroupManager->GetGroupByID(nGroupID);
		if (!pGroup)
		{
			return;
		}		
		{
			for (size_t nItemIndex=0;nItemIndex<pGroup->GetItemCount();++nItemIndex)
			{
				bool bExist=false;
				CGroupItem *pItem=pGroup->GetItem(nItemIndex);
				vlist_reset_cursor(m_ScheduledAnimationList);
				while (1)
				{
					CSAnimation *temp;
					//xmlgenSA.Reset();
					if (!(temp = (CSAnimation *)vlist_peek_cursor(m_ScheduledAnimationList)))
						break;
					if (temp->GetGroupAnimation()==animation&&temp->GetTarget()->GetPath()==pItem->GetPlcPath())
					{
						bExist=true;
						break;
					}
					vlist_advance_cursor(m_ScheduledAnimationList);	
				}
				if(!bExist)
				{
					//TODO:delete target and ani
					STargetObject *tobject = new STargetObject(this, NULL, pItem->GetPlcPath().c_str());
					CSAnimation *ani = animation->CreatOffsetAnimation(pItem->m_LocalPos,pItem->m_LocalQuat,pItem->m_LocalScale,
						pItem->m_OffsetGroupPos,pItem->m_OffsetGroupQuat,pItem->m_OffsetGroupScale,tobject);			
					ani->SetGroupAnimation(animation);
					CSTimeline *ti = ani->GetTimeline();
					if (ani->GetLoop())
						ti->SetStartTick( currentframe + ani->GetDelay() - ti->GetCurrentRelativeTick());
					else
						ti->SetStartTick( currentframe + ani->GetDelay());

					ani->Reset();
					ani->SetDefaultActive(true);
					ani->SetExecuteOnce(false);
					vlist_remove(m_ScheduledAnimationList, ani);
					vlist_add_last(m_ScheduledAnimationList, (void *)ani);

				}		
			}
		}
		//CSAnimation *ani = new CSAnimation(name, this, 0);
		//ani->SetTimeline(tl);
		//ani->AddInterpolator(interpolator);
		//tl->SetAnimation(ani);
		//interpolator->SetAnimation(ani);

		////	CSAnimation *aninst = ani->CreateInstance();
		//ani->SetTargetByPath(target);

		//CSTimeline *ti = animation->GetTimeline();
		//if (animation->GetLoop())
		//	ti->SetStartTick( currentframe + animation->GetDelay() - ti->GetCurrentRelativeTick());
		//else
		//	ti->SetStartTick( currentframe + animation->GetDelay());

		//animation->Reset();
		//animation->SetDefaultActive(true);
		//animation->SetExecuteOnce(false);
		//vlist_remove(m_ScheduledAnimationList, animation);
		//vlist_add_last(m_ScheduledAnimationList, (void *)animation);

	}else
	{
		//非组动画
		CSTimeline *ti = animation->GetTimeline();
		if (animation->GetLoop())
			ti->SetStartTick( currentframe + animation->GetDelay() - ti->GetCurrentRelativeTick());
		else
			ti->SetStartTick( currentframe + animation->GetDelay());

		animation->Reset();
		animation->SetDefaultActive(true);
		animation->SetExecuteOnce(false);
		vlist_remove(m_ScheduledAnimationList, animation);
		vlist_add_last(m_ScheduledAnimationList, (void *)animation);
	}

	
}

void CSBehaviorAction::RemoveScheduledAnimation(CSAnimation *animation)
{
 
	vlist_remove(m_ScheduledAnimationList, animation);
	 
}

 

void CSBehaviorAction::ScheduleAllAnimations(bool reset)
{
	CSBehaviorAction::Lock();
	delete_vlist(m_ScheduledAnimationList);
	m_ScheduledAnimationList = new_vlist( malloc, free );
	vlist_reset_cursor(m_AnimationList);
 	while (1)
	{
		CSAnimation *temp;
		if (!(temp = (CSAnimation *)vlist_peek_cursor(m_AnimationList)))
				break;
		if (reset)
		    temp->GetTimeline()->SetCurrentRelativeTick(0);
		if (temp->GetDefaultActive()/* && !temp->GetParentAnimation()*/)
		{
			//ScheduleAnimation(temp, 0);
			ScheduleAnimationWithGroup(temp, 0);
		}
  		vlist_advance_cursor(m_AnimationList);	
	}
 	CSBehaviorAction::UnLock();
}





CSAnimation * CSBehaviorAction::AddAnimation(const char *name, const char *target , CSTimeline *tl, CSInterpolator *interpolator)
{
	CSAnimation *ani = new CSAnimation(name, this, 0);
	ani->SetTimeline(tl);
	ani->AddInterpolator(interpolator);
	tl->SetAnimation(ani);
	interpolator->SetAnimation(ani);

//	CSAnimation *aninst = ani->CreateInstance();
	ani->SetTargetByPath(target);
 	//vlist_add_last(m_AnimationList, ani);
	AddAnimation(ani);
//	vlist_add_last(m_AnimationList, aninst);
	return ani;
}


void CSBehaviorAction::AddAnimation(CSAnimation *animation)
{
  	vlist_add_last(m_AnimationList, (void *)animation);
}

 

int CSBehaviorAction::GetFreeName()
{
		vlist_reset_cursor(m_AnimationList);
 		int number = 0;
		while (1)
		{
			CSAnimation *temp;
			if (!(temp = (CSAnimation *)vlist_peek_cursor(m_AnimationList)))
				break;
			int nnum = atoi(temp->GetName());
			if (nnum>=number)
				number = nnum + 1;
 			vlist_advance_cursor(m_AnimationList);	
		}
		return number;
}

bool CSBehaviorAction::Tick( float request_time, float actual_time )
{
	m_PreTick = m_CurrentTick;
    SVIEW::View* view = GetView();
    if(view)
    {
        view->Lock();
    }
    
	if(!m_bReversePlay)
	{
		//if (m_CurrentTick >= m_LastTick && !vlist_peek_first(m_SensorList) && !m_bInfinitePlay)
		if (m_CurrentTick >= m_LastTick && !vlist_peek_first(m_SensorList) && !m_bInfinitePlay)
		{
			if (m_bContinuousPlay)
			{
				ScheduleAllAnimations();//Edit by ZYP
				m_CurrentTick = GetFirstTick();
				ExecuteAnimations(m_CurrentTick, 0.0f);
				APP_Show_Time( &m_StartTime );  
				m_StartTime -= GetCurrentTick()/GetTicksPerSecond(); 
			}
			else
			{
				m_CurrentTick = m_LastTick;
				Stop();
				if(m_pSimulationAnimationManager->GetAnimationStepManager() && 
					m_pSimulationAnimationManager->GetAnimationStepManager()->GetPlayMode() != CAnimationStepManager::PLAY_MODE_NONE && //
					m_pSimulationAnimationManager->GetAnimationStepManager()->GetPlayMode() != CAnimationStepManager::PLAY_MODE_RANDOM) // edit by likun 16/10/18
				{
					m_pSimulationAnimationManager->GetAnimationStepManager()->PlayFinishCB(this);
				}
                
                if(view)
                {
                    view->UnLock();
                }
				
				return true;
			}
		}
		else
		{
			if (m_bRenderEveryFrame)
				m_CurrentTick++;
			else
				SetCurrentTickByTime(actual_time);
		}
	}
	else
	{
		if (m_CurrentTick <= m_FirstTick && !vlist_peek_first(m_SensorList) && !m_bInfinitePlay)
		{
			if (m_bContinuousPlay)
			{
				ScheduleAllAnimations();//Edit by ZYP
				m_CurrentTick = GetLastTick();
				ExecuteAnimations(m_CurrentTick, 0.0f);
				APP_Show_Time( &m_StartTime );  
				m_StartTime -= (GetLastTick()-GetCurrentTick())/GetTicksPerSecond(); 

			}
			else
			{
				m_CurrentTick = m_FirstTick;
				Stop();
				if(m_pSimulationAnimationManager->GetAnimationStepManager())
				{
					m_pSimulationAnimationManager->GetAnimationStepManager()->PlayFinishCB(this);
				}
                
                if(view)
                {
                    view->UnLock();
                }
				
				return true;
			}
		}
		else
		{
			if (m_bRenderEveryFrame)
				m_CurrentTick--;
			else
				SetCurrentTickByTime(actual_time);
		}
	}
	//if(m_bReversePlay)
 //		ExecuteAnimations(m_LastTick - GetCurrentTick(),-1);	
	//else
    ExecuteAnimations(GetCurrentTick(),-1);
    
    if(view)
    {
        view->UnLock();
    }
    
  	return true;

} //end function Tick
//need sv interface
void CSBehaviorAction::Reset()
{
    START_LIST_ITERATION(STargetObject, m_TargetObjectList);
		temp->ResetForCollision();
	END_LIST_ITERATION(m_TargetObjectList);

}
void CSBehaviorAction::Lock()
{
    m_mutex.Acquire();
}

void CSBehaviorAction::UnLock()
{
//    pthread_mutex_unlock(m_Mutex);
    m_mutex.Release();
}
 
void CSBehaviorAction::ExecuteAnimations(float currentframe, float startframe)
{
	CSBehaviorAction::Lock();
	struct vlist_s *	deletionlist;		
	deletionlist = new_vlist(malloc, free);
	Reset();
	m_bCameraUpdated = false;
	START_LIST_ITERATION(CSAnimation, m_ScheduledAnimationList)
		bool animfinished = false;		
		if (!temp->GetParentAnimation())
		{
			if (startframe>-1)
				temp->Animate(currentframe, startframe);
			else
				animfinished = temp->Animate(currentframe);
		
			if ((animfinished && !temp->GetLoop()) || temp->ExecuteOnce())
			{
				temp->SetExecuteOnce(false);
				vlist_add_last(deletionlist,temp);
//					vlist_remove(m_ScheduledAnimationList, temp);
				temp->SetCurrentlyRunning(false);
			}
		}
	END_LIST_ITERATION(m_ScheduledAnimationList)
	START_LIST_ITERATION(CSAnimation, deletionlist)
	    vlist_remove(m_ScheduledAnimationList,temp);
	END_LIST_ITERATION(deletionlist);
	delete_vlist(deletionlist);
	CSBehaviorAction::UnLock();

	if(GetAnimationPlayApi())
	{
		if(IsPlayCollision())
		{
			float collistionDelay = m_CurrentTick*GetTicksPerSecond() - m_fCollisionTime;
			if(collistionDelay > GetCollisionDelay() || !IsPlaying())
			{
				if(!IsPlaying())
				{
					GetAnimationPlayApi()->InitColisionData(GetView());
				}
				bool bCollisionFound = false;
				START_LIST_ITERATION(STargetObject, m_TargetObjectList);
				if (/*m_bCollisionCheck &&*/
					strcmp(temp->GetType(),TARGETOBJECTTYPE_INS)==0 && 
					temp->GetCollision() && 
					temp->HasMoved())
				{
					bCollisionFound = bCollisionFound | EvaluateCollision(temp);
				}
					
				END_LIST_ITERATION(m_TargetObjectList);

				if(bCollisionFound)
				{
					if(IsStopPlayAfterCollision())
					{
						Stop();
					}
					//记录干涉信息
				}

				m_fCollisionTime = m_CurrentTick*GetTicksPerSecond();
			}
		}

		GetAnimationPlayApi()->updateView(this,GetView());
	}
}

CSAnimation * CSBehaviorAction::FindAnimation(const char *target, const char *interpolatortype,bool bFirstInterpolator)
{
 		START_LIST_ITERATION(CSAnimation, m_AnimationList)
			if(interpolatortype == NULL)
			{
				if(!temp->GetInterpolator() && temp->GetTarget()->IsEqual(target))
				{
					return temp;
				}
			}
			else
			{
				if(temp->GetTarget()->IsEqual(target))
				{
					//遍历当前动画的所有插值器，判断是否包含指定类型的插值器
					bool bFinded = false;
					
					//vlist_s* pInterpolatorList = temp->GetInterpolatorList();
					//START_LIST_ITERATION(CSInterpolator, pInterpolatorList)
					//	if (temp &&
					//		strcmp(temp->GetType(), interpolatortype) == 0)
					//	{
					//		bFinded = true;
					//		break;
					//	}
					//	if(bFirstInterpolator)
					//	{
					//		break;
					//	}
					//END_LIST_ITERATION(pInterpolatorList)
					if(!bFirstInterpolator && temp->GetInterpolator(interpolatortype))
					{
						bFinded = true;
					}
					else if(bFirstInterpolator )
					{
						 CSInterpolator *pCSInterpolator = temp->GetInterpolator();
						 if(pCSInterpolator && strcmp(pCSInterpolator->GetType(), interpolatortype) == 0)
						 {
							 bFinded = true;
						 }
					}
					if(bFinded)
					{
						return temp;
					}
				}
				
			}
		END_LIST_ITERATION(m_AnimationList)
		return 0;
}

CSAnimation * CSBehaviorAction::FindAnimationByName(const char *name)
{

 		START_LIST_ITERATION(CSAnimation, m_AnimationList)
 		if (temp->GetName() && strcmp(temp->GetName(), name) == 0)
  					return temp;							
		END_LIST_ITERATION(m_AnimationList)
		return 0;
}

bool CSBehaviorAction::IsAnimation(const CSAnimation *pAnimation)
{
	bool bRet = false;
	START_LIST_ITERATION(CSAnimation, m_AnimationList)
		if(pAnimation == temp)
		{
			bRet = true;
			break;
		}
	END_LIST_ITERATION(m_AnimationList)
	return bRet;
}

void CSBehaviorAction::DeleteAllAnimations()
{

	if (IsPlaying())
		Stop();	

	if(HasLockedAnimations())
	{
		return;
	}

	START_LIST_ITERATION(CSAnimation, m_AnimationList)
		delete temp;
	END_LIST_ITERATION(m_AnimationList)

	vlist_flush(m_AnimationList);


	vlist_flush(m_ScheduledAnimationList);


	START_LIST_ITERATION(STargetObject, m_TargetObjectList)
		delete temp;
	END_LIST_ITERATION(m_TargetObjectList)
	vlist_flush(m_TargetObjectList);
	
	vlist_flush(m_SensorList);

	m_CurrentTick = 0;
	m_LastTick = 0;
	m_FirstTick = 0;
	m_fPlayBeginTick = 0;
	m_fPlayEndTick = 0;
}

bool CSBehaviorAction::DeleteAnimation(CSAnimation *animation)
{
	bool bRet = false;
	bool bHaveRelationTag = false;
	bool bFindAnimation = false;

	if(!animation)
	{
		return false;
	}
	
	if (IsPlaying())
		Stop();

	if(!animation->IsLocked(true))
	{
		//判断要删除的动画是否有效
		START_LIST_ITERATION(CSAnimation, m_AnimationList)
			if(temp == animation)
			{
				bFindAnimation = true;
				break;
			}
		END_LIST_ITERATION(m_AnimationList)
		if(!bFindAnimation)
		{
			return false;
		}

		//删除子动画
		vlist_s * listChildAnimation  = animation->GetChildAnimationList();
		CSAnimation* pTempAnimation = NULL;
		for(int i = 0; i< (int)vlist_count(listChildAnimation); i++)
		{
			pTempAnimation = (CSAnimation*)vlist_nth_item(listChildAnimation,i);
			if(pTempAnimation)
			{
				if(DeleteAnimation(pTempAnimation))
				{
					i--;
				}
			}
		}

		//删除动画时，如果是PMI动画，将对应的PMI隐藏
		if (animation->GetTarget() && strcmp(animation->GetTarget()->GetType(),TARGETOBJECTTYPE_PMI) == 0)
		{
			char plcIdPath[SA_BUFFER_SIZE_SMALL] = "";
			char cPMIId[SA_BUFFER_SIZE_SMALL] = "";
			char cName[SA_BUFFER_SIZE_SMALL] = "";
			animation->GetTarget()->GetInsPathAndPMIIdByResolvedPath(plcIdPath,cPMIId);
			strcpy(cName,TARGETOBJECTTYPE_PMI);
			strcat(cName,":");
			strcat(cName,cPMIId);
			TransferVisible(plcIdPath, (char*)cName, false,0.0f);
		}

		START_LIST_ITERATION(CSAnimation, m_AnimationList)
			if(temp != animation)
			{
				if(temp->GetTarget() == animation->GetTarget())
				{
					bHaveRelationTag = true;
					break;
				}
			}
		END_LIST_ITERATION(m_AnimationList)
		
		if(!bHaveRelationTag)
		{
			START_LIST_ITERATION(STargetObject, m_TargetObjectList)
			if(temp == animation->GetTarget())
			{
				vlist_remove(m_TargetObjectList, temp);
				delete temp;
				break;
			}
			END_LIST_ITERATION(m_TargetObjectList)
		}

		vlist_remove(m_AnimationList, animation);
		vlist_remove(m_ScheduledAnimationList, animation);

		//add by likun 2014-10-27，删除动画前，将该动画从父动画的子动画列表中删除
		CSAnimation* pParentAnimation = animation->GetParentAnimation();
		if(pParentAnimation)
		{
			vlist_remove(pParentAnimation->GetChildAnimationList(),animation);
			pParentAnimation->ResetFirstLastTickByChild();
		}
	
		delete animation;
		bRet = true;
	
		if(m_fPlayEndTick > GetAllAniLastTick() )
		{
			m_fPlayEndTick = GetAllAniLastTick();
		}

		//清空干涉信息
		if(IsPlayCollision())
		{
			GetAnimationPlayApi()->ClearColisionData(GetView());
		}
	}
	return bRet;
}

void CSBehaviorAction::DeleteAnimation(const char *target)
{
	START_LIST_ITERATION(CSAnimation, m_AnimationList)
		if ( temp->GetTarget()->IsEqual(target) )
		{
			DeleteAnimation(temp);	
		}
	END_LIST_ITERATION(m_AnimationList)
}


void CSBehaviorAction::WriteToFile(const char *filename){
  WriteToFile((wchar_t*)SA_WCS(filename).encodedText());
}

#ifdef _MSC_VER
void CSBehaviorAction::WriteToFile(const unsigned short *filename)
{
	WriteToFile((wchar_t const *) filename);
}
#endif

void CSBehaviorAction::WriteToFile(const wchar_t *filename)
{
	CUtilityXMLGenerator xmlgen;
	xmlgen.Reset();
	Serialize(&xmlgen);
	xmlgen.Finalize();
	FILE *fp;
 	fp = wfopen(filename, L"w");
	fwrite(xmlgen.GetBuffer(), 1, xmlgen.GetBufferSize(), fp);
	fclose(fp);
}

void *CSBehaviorAction::XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[SA_BUFFER_SIZE_SMALL] = {0};
	char Version[SA_BUFFER_SIZE_SMALL] = {0};
	char Company[SA_BUFFER_SIZE_SMALL] = {0};
 	int fps;
	int delay;
	int ID;

	if(!xt->GetXMLParser()->GetStackData())
	{
		return NULL;
	}

	CSimulationAnimationManager * pSAManager = (CSimulationAnimationManager*)xt->GetXMLParser()->GetStackData();
	CSBehaviorAction *pSimulationAnimation = NULL;
	if (open)
	{
		/*if (!xt->GetProperty("AssemblyPath", AssemblyPath))
			strcpy(AssemblyPath, "");*/
		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		//if (!xt->GetProperty("Version", Version))
		//	strcpy(Version, "");
		strcpy(Version, pSAManager->GetVersion());
		if (!xt->GetProperty("Company", Company))
			strcpy(Company, "HoteamSoft");
		if (!xt->GetProperty("TicksPerSecond", fps))
			fps = 10;
		if (!xt->GetProperty("Delay", delay))
			delay = 10;

		if (!xt->GetProperty("ID", ID))
			ID = 0;

		//if(strcmp(Version, VERSION))
		//{
		//	AfxMessageBox(_T("动画打开失败（版本不兼容）"));
		//	return NULL;//版本不兼容
		//}
		if(SATools::VersionCompare("2.0",Version) > 0)
		{
			CUtilityXMLParser* pUtilityXMLParser = xt->GetXMLParser();
			pUtilityXMLParser->SetTagCallback("Animation", CSAnimation::XMLCallback_V1, pSAManager);
			pUtilityXMLParser->SetTagCallback("PosInterpolator", CSInterpolatorPosition::XMLCallback_V1, pSAManager);
			pUtilityXMLParser->SetTagCallback("TargetObject", STargetObject::XMLCallback_V1, pSAManager);
			pUtilityXMLParser->SetTagCallback("AttSwitchInterpolator", CSInterpolatorVisible::XMLCallback, pSAManager);
		}

		if(NULL == pSAManager)
		{
			CSimulationAnimationManager *ppSAManager = (CSimulationAnimationManager*) m_pExtraData;
			if(typeid(*ppSAManager) != typeid(CSimulationAnimationManager))
			{
				pSimulationAnimation = new CSBehaviorAction(ID, fps, delay, Name, Version, Company);
				*(CSBehaviorAction**) m_pExtraData = pSimulationAnimation;
			}
				
		}
		else
		{
			pSimulationAnimation = pSAManager->AddSimAni(ID, fps, delay, Name, Version, Company);
		}

	}
	else
	{
		
	}
	return pSimulationAnimation;
}

CSBehaviorAction* CSBehaviorAction::ReadFromFile(const char *filename){
  return ReadFromFile((wchar_t*)SA_WCS(filename).encodedText());
}

#ifdef _MSC_VER
CSBehaviorAction* CSBehaviorAction::ReadFromFile(const unsigned short *filename)
{
	return ReadFromFile((wchar_t const *) filename);
}
#endif

CSBehaviorAction* CSBehaviorAction::ReadFromFile(const wchar_t *filename)
{
	CSBehaviorAction* pSimulationAnimation = NULL;
	static bool first = true;
	static wchar_t oldfilename[SA_BUFFER_SIZE_SMALL] = {0};
	wchar_t loadfilename[SA_BUFFER_SIZE_SMALL] = {0};
	if (!filename)
	{
		if (first)
			return pSimulationAnimation;
		wcscpy(loadfilename, oldfilename);
	}		
	else
		wcscpy(loadfilename, filename);

	int size = 10000;
	first = false;

	char *buffer = new char[size];
 	int c = 0;
    FILE * fp = wfopen(loadfilename, L"r");
	if(fp == 0)
	{
		return pSimulationAnimation;
	}
	while (1)
	{
		int rr = (int)fread( &buffer[c], 1, 1, fp);
		if (!rr)
			break;
 		c++;
		if (c >=size)
		{
			size+=1000;
			char *temp = new char[size];
			memcpy(temp, buffer, c);
			delete [] buffer;
			buffer = temp;
		}
 
	}
	fclose(fp);
	buffer[c + 1] = 0;
	if(0 != c)
		pSimulationAnimation = ProcessXMLData(buffer);
	delete [] buffer;
	wcscpy(oldfilename, loadfilename);
	return pSimulationAnimation;

}

int CSBehaviorAction::GetTicksPerSecond()
{
	if(m_pSimulationAnimationManager)
	{
		return m_Tps* m_pSimulationAnimationManager->GetPlaySpeed();
	}
	else
	{
		return m_Tps;
	}
}

CSBehaviorAction* CSBehaviorAction::ProcessXMLData(const char *buffer)
{
 	CUtilityXMLParser xp;

	CSBehaviorAction* pSimulationAnimation = NULL;
	char doctype[SA_BUFFER_SIZE_SMALL] = {0};
	xp.FindDocType(doctype, buffer);
	xp.SetupParseBuffer(buffer);
 	
	//xp.SetTagCallback("BehaviorAction", CSBehaviorAction::XMLCallback, (void*)pAssemblyModelId);
	xp.SetTagCallback("Animdef", CSBehaviorAction::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("BehaviorManager", CSBehaviorAction::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("Animation", CSAnimation::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("PosInterpolator", CSInterpolatorPosition::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("Timeline", CSTimeline::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("TargetObject", STargetObject::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("QuatRotInterpolator", CSInterpolatorQuatSquad::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("VisibleInterpolator", CSInterpolatorVisible::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("ColorInterpolator", CSInterpolatorColor::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("PivotInterpolator", CSInterpolatorPivot::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("SoundInterpolator", CSInterpolatorSound::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("NormalInterpolator", CSInterpolatorNormal::XMLCallback, &pSimulationAnimation);
	xp.SetTagCallback("InstanceCreateInterpolator", CSInterpolatorInstanceCreate::XMLCallback, &pSimulationAnimation);
	xp.ProcessXMLData();

	return pSimulationAnimation;
}

void CSBehaviorAction::Serialize(CUtilityXMLGenerator *xmlgen, bool bHasParent)
{
	
 	CUtilityXMLTag xmlt;
	if(!bHasParent)
		xmlgen->WriteToBuffer("<?xml version=\"1.0\" encoding = \"GBK\"?>\n");
	xmlt.SetTagname("BehaviorManager");
	if(!bHasParent)
	{
		xmlt.AddProperty("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	}

	xmlt.AddPropertyInt("ID", (int)m_ID);
	xmlt.AddPropertyInt("Delay", (int)m_Delay);
	xmlt.AddPropertyInt("TicksPerSecond", (int)m_Tps);
	xmlt.AddProperty("Name", m_Name, true);
	//xmlt.AddProperty("AssemblyPath", m_AssemblyPath, true);
	//xmlt.AddProperty("Version", m_Version, true);
	//xmlt.AddProperty("Company", m_Company, true);
	

	xmlt.SetOpen(true);
   	xmlgen->AddTag(&xmlt);

	vlist_reset_cursor(m_TargetObjectList);
	while (1)
	{
		STargetObject *temp;
		if (!(temp = (STargetObject *)vlist_peek_cursor(m_TargetObjectList)))
			break;
		temp->Serialize(xmlgen);
		vlist_advance_cursor(m_TargetObjectList);	
	}


	vlist_reset_cursor(m_AnimationList);
	while (1)
	{
		CSAnimation *temp;
		if (!(temp = (CSAnimation *)vlist_peek_cursor(m_AnimationList)))
			break;
		if(!temp->GetParentAnimation())
			temp->Serialize(xmlgen);
		vlist_advance_cursor(m_AnimationList);	
	}

	xmlgen->CloseTag(&xmlt);
}


int CSBehaviorAction::GetNextKeyframe(int ticknum, bool forward)
{
	bool found = false;
	int ftick = 1000000;
	if (!forward)
		ftick = -ftick;
	START_LIST_ITERATION(CSAnimation, m_ScheduledAnimationList)
  	if (/*!*/temp->GetParentAnimation())
	{
		CSTimeline *ti = temp->GetTimeline();
		int *ta = ti->GetTimelineArray();
		int talength = ti->GetTimelineArrayLength();
		for (int i=0;i<talength; i++)
		{
			if (forward)
			{
				if (ta[i] > ticknum)
				{
					found = true;
					if ((ta[i] - ticknum) < (ftick - ticknum))
						ftick = ta[i];
				}
			}
			else
			{
				if (ta[i] < ticknum)
				{
					found = true;
					if ((ticknum - ta[i]) < (ticknum - ftick))
						ftick = ta[i];
				}
			}
		}
 
	}
 	END_LIST_ITERATION(m_ScheduledAnimationList);		
	if (found)
		return ftick;
	else
		return ticknum;

}
 

 
void CSBehaviorAction::SetCurrentTickByPercentage(float percentage)
{
 
	//int dist = GetLastTick();
	//if (dist == 0)
	//	dist = 1;
	//m_CurrentTick = (percentage / 100.0f) * dist;	
	if(vlist_count(m_ScheduledAnimationList) <= 0)
	{
		ScheduleAllAnimations(true);
	}
	
	int dist = GetLastTick() - GetFirstTick();
	if (dist == 0)
		dist = 1;
	m_CurrentTick = (percentage / 100.0f) * dist + GetFirstTick();

}
 
 
float CSBehaviorAction::GetCurrentTickByPercentage()
{
	//if(m_LastTick > 0.0 &&
	//	m_CurrentTick> 0.0 &&
	//	m_CurrentTick >= m_LastTick)
	//{
	//	return 100.0f;
	//}
	//int dist = GetLastTick();
	//if( dist <= 0 )
	//	return 0;
	//else
	//	return ((m_CurrentTick / dist) * 100.0f);

	if(m_LastTick > 0.0 &&
		m_CurrentTick> 0.0 &&
		m_CurrentTick >= m_LastTick)
	{
		return 100.0f;
	}

	if(vlist_count(m_ScheduledAnimationList) <= 0 )
	{
		ScheduleAllAnimations(true);
		m_FirstTick = GetFirstTick();
	}
	m_LastTick = GetLastTick();

	int dist = m_LastTick - m_FirstTick;
	if( dist <= 0 )
		return 0;
	else
		return (((m_CurrentTick - m_FirstTick) / dist) * 100.0f);


}

int CSBehaviorAction::GetLastTick()
{
		int dist = 0;
		CSBehaviorAction::Lock();
		START_LIST_ITERATION(CSAnimation, m_ScheduledAnimationList)
  		if (!temp->GetParentAnimation())
		{
			CSTimeline *ti = temp->GetTimeline();
			int lasttick = ti->GetLastTick();
			if (lasttick != -1)
			{
				if (lasttick > dist)
					dist = lasttick;
			}
		}
 		END_LIST_ITERATION(m_ScheduledAnimationList);	
 		CSBehaviorAction::UnLock();

		//如果启用播放范围，以播放范围的最后帧为准
		if (m_bPlayRangeSign/* && dist > GetPlayEndTick()*/)
		{
			dist = GetPlayEndTick();
		}

		return dist;
}

int CSBehaviorAction::GetLastTickUnIncludeSound()
{
	int dist = 0;
	CSBehaviorAction::Lock();
	START_LIST_ITERATION(CSAnimation, m_ScheduledAnimationList)
		if(strcmp(temp->GetTarget()->GetType(),TARGETOBJECTTYPE_SOUND) !=0 && 
			strcmp(temp->GetName(),TARGETOBJECTTYPE_NAME_SOUND) != 0)
		{
			if (!temp->GetParentAnimation())
			{
				CSTimeline *ti = temp->GetTimeline();
				int lasttick = ti->GetLastTick();
				if (lasttick != -1)
				{
					if (lasttick > dist)
						dist = lasttick;
				}
			}
		}
		
		END_LIST_ITERATION(m_ScheduledAnimationList);
		CSBehaviorAction::UnLock();

		//如果启用播放范围，以播放范围的最后帧为准
		if (m_bPlayRangeSign/* && dist > GetPlayEndTick()*/)
		{
			dist = GetPlayEndTick();
		}

		return dist;		
}

int CSBehaviorAction::GetFirstTick()
{
	int dist = INT_MAX;
	if(0 == m_ScheduledAnimationList->count)
		return 0;
	CSBehaviorAction::Lock();
	START_LIST_ITERATION(CSAnimation, m_ScheduledAnimationList)
		if (!temp->GetParentAnimation())
		{
			int firsttick = temp->GetFirstTick();
			if (firsttick != -1)
			{
				if (firsttick < dist )
					dist = firsttick;
			}
		}
	END_LIST_ITERATION(m_ScheduledAnimationList);	
	CSBehaviorAction::UnLock();

	//如果启用播放范围，以播放范围的第一帧
	if (m_bPlayRangeSign/* && dist < GetPlayBeginTick()*/)
	{
		dist = GetPlayBeginTick();
	}
	return dist;		
}

int CSBehaviorAction::GetDefaultActiveAniLastTick()
{
	int dist = 0;
	if(0 == m_AnimationList->count)
		return dist;

	START_LIST_ITERATION(CSAnimation, m_AnimationList)
		if (temp->GetDefaultActive() && !temp->GetParentAnimation())
		{
			CSTimeline *ti = temp->GetTimeline();
			int lasttick = ti->GetLastTick();
			if (lasttick != -1)
			{
				if (lasttick > dist)
					dist = lasttick;
			}
		}
	END_LIST_ITERATION(m_AnimationList);	
	//如果启用播放范围，以播放范围的最后帧为准
	if (m_bPlayRangeSign/* && dist > GetPlayEndTick()*/)
	{
		dist = GetPlayEndTick();
	}
	return dist;	
}

int CSBehaviorAction::GetDefaultActiveAniFirstTick()
{
	int dist = INT_MAX;
	if(0 == m_AnimationList->count)
		return 0;

	START_LIST_ITERATION(CSAnimation, m_AnimationList)
		if (temp->GetDefaultActive() && !temp->GetParentAnimation())
		{
			int firsttick = temp->GetFirstTick();
			if (firsttick != -1)
			{
				if (firsttick < dist)
					dist = firsttick;
			}
		}
		END_LIST_ITERATION(m_AnimationList);	
		//如果启用播放范围，以播放范围的第一帧
		if (m_bPlayRangeSign/* && dist < GetPlayBeginTick()*/)
		{
			dist = GetPlayBeginTick();
		}
		return dist;	
}

int CSBehaviorAction::GetAllAniLastTick()
{
	int dist = 0;
	if(0 == m_AnimationList->count)
		return dist;

	START_LIST_ITERATION(CSAnimation, m_AnimationList)
		if (!temp->GetParentAnimation())
		{
			CSTimeline *ti = temp->GetTimeline();
			int lasttick = ti->GetLastTick();
			if (lasttick != -1)
			{
				if (lasttick > dist)
					dist = lasttick;
			}
		}
		END_LIST_ITERATION(m_AnimationList);	
		return dist;		
}
 
int CSBehaviorAction::GetAllAniFirstTick()
{
	int dist = INT_MAX;
	if(0 == m_AnimationList->count)
		return 0;
	
	START_LIST_ITERATION(CSAnimation, m_AnimationList)
		if (!temp->GetParentAnimation())
		{
			int firsttick = temp->GetFirstTick();
			if (firsttick != -1)
			{
				if (firsttick < dist)
					dist = firsttick;
			}
		}
		END_LIST_ITERATION(m_AnimationList);	
		return dist;		
}
 
long CSBehaviorAction::GetActiveSceneKey()
{
	return INVALID_KEY; 
}


bool CSBehaviorAction::HasAnimations()
{ 
	return (vlist_peek_first(m_AnimationList) != 0);
}



void CSBehaviorAction::DeactivateAllAnimations()
{
	START_LIST_ITERATION(CSAnimation, m_AnimationList)
	    temp->SetDefaultActive(false);
  	END_LIST_ITERATION(m_AnimationList)

 
}
void CSBehaviorAction::ActivateAllAnimations()
{
	START_LIST_ITERATION(CSAnimation, m_AnimationList)
	    temp->SetDefaultActive(true);
  	END_LIST_ITERATION(m_AnimationList)

 
}

void CSBehaviorAction::ActivateAnimationByName(const char *name, bool AllowPartial)
{
	START_LIST_ITERATION(CSAnimation, m_AnimationList)
	    if (AllowPartial)
	    {	    
		if (strstr(temp->GetName(),name) == temp->GetName())
			temp->SetDefaultActive(true);
	    }
	    else
	    {	    
		if (strcmp(temp->GetName(),name) == 0)
			temp->SetDefaultActive(true);
	    }

  	END_LIST_ITERATION(m_AnimationList)
}
void CSBehaviorAction::DeactivateAnimationByName(const char *name, bool AllowPartial)
{
	START_LIST_ITERATION(CSAnimation, m_AnimationList)
	    if (AllowPartial)
	    {	    
		if (strstr(temp->GetName(),name) == temp->GetName())
			temp->SetDefaultActive(false);
	    }
	    else
	    {	    
		if (strcmp(temp->GetName(),name) == 0)
			temp->SetDefaultActive(false);
	    }

  	END_LIST_ITERATION(m_AnimationList)
}

bool CSBehaviorAction::EvaluateCollision(STargetObject* pTargetObject)
{
	bool bCollisionFound = false;
	if(!pTargetObject)
	{
		return bCollisionFound;
	}
	
	char plcIdPath[SA_BUFFER_SIZE_SMALL] = {0};
	strcpy(plcIdPath, pTargetObject->GetResolvedPath());
	if(GetAnimationPlayApi())
	{
		bCollisionFound = GetAnimationPlayApi()->Collision(plcIdPath,GetView());
	}

	return bCollisionFound;;
}

CSAnimation * CSBehaviorAction::AddAnimation(const char *name, const char *targetpath, AniPoint *pivot)
{
 	CSAnimation *ani = new CSAnimation(name, this, 0);
 	if (targetpath)
	{
		ani->SetTargetByPath(targetpath);
		if (pivot)
			ani->GetTarget()->SetPivot(pivot->x, pivot->y, pivot->z);
	}
	AddAnimation(ani);
	return ani;
}

void CSBehaviorAction::SetName(const char* strName)
{
	if(NULL != strName)
		strcpy(m_Name, strName);
}

void CSBehaviorAction::Reference()
{
	m_ReferenceCount++;
}
void CSBehaviorAction::Release()
{
	m_ReferenceCount--;
	if(m_ReferenceCount <= 0) 
	{
		delete this;
	}
}

STargetObject* CSBehaviorAction::GetTargetObjectByKey(long key)
{
	STargetObject* pTarget = NULL;
	START_LIST_ITERATION(STargetObject, m_TargetObjectList)
		if(temp->GetTargetKey() == key)
		{
			pTarget = temp;
			break;
		}
	END_LIST_ITERATION(m_TargetObjectList)
	return pTarget;
}

long CSBehaviorAction::RegisterTargetObjectKey()
{
	long iRet = 0;
	START_LIST_ITERATION(STargetObject, m_TargetObjectList)
		if(temp->GetTargetKey() >= iRet)
		{
			iRet = temp->GetTargetKey()+1;
		}
	END_LIST_ITERATION(m_TargetObjectList)
	return iRet;
}

bool CSBehaviorAction::HasLockedAnimations()
{
	bool bHasLocked = false;
	START_LIST_ITERATION(CSAnimation, m_AnimationList)
		if(temp->IsLocked())
		{
			bHasLocked = true;
			break;
		}
		END_LIST_ITERATION(m_AnimationList)
			return bHasLocked;
}
bool CSBehaviorAction::IsAllAnimationsLocked()
{
	bool bAllLocked = true;
	START_LIST_ITERATION(CSAnimation, m_AnimationList)
		if(!temp->IsLocked())
		{
			bAllLocked = false;
			break;
		}
	END_LIST_ITERATION(m_AnimationList)
	return bAllLocked;
}

bool  CSBehaviorAction::TransferTool(int nType, const char* plcIdPath, const char* strToolPath, const char* strParentPath)
{
	if(GetAnimationPlayApi())	//正在播放动画...
	{
		GetAnimationPlayApi()->PlayToolAnimation(nType, plcIdPath, strToolPath, strParentPath,GetView());
	}
	// 表示更新
	return true;
}


View* CSBehaviorAction::GetView()
{
	if(GetSimulationAnimationManager())
	{
		return GetSimulationAnimationManager()->GetView();
	}
	else
	{
		return NULL;
	}
}

/*void CSBehaviorAction::SetCBTran(PlayOccShapesFun CBFun, UpdateOccViewFun CBUpdateViewFun, PlayOccShapesOnOffFun CBOnOffFun)
{
    m_cbOccPlayFun = CBFun;
    m_cbOccUpdateView = CBUpdateViewFun;
    m_cbOccPlayOnOffFun = CBOnOffFun;
}*/
SA_NAMESPACE_END


