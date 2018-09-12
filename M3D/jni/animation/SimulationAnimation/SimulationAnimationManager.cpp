#include "stdafx.h"
#include "SimulationAnimationManager.h"
#include "SBehaviorAction.h"
#include "../SimulationCommon/UtilityXMLParser.h"
#include "../SimulationCommon/vlist.h"
#include "../SimulationCommon/MatrixOperation.h"
#include "../SimulationCommon/utf_utils.h"
#include "SAnimation.h"
#include "SInterpolator.h"
#include "STimeline.h"
#include <wchar.h>
#include "AnimationStepManager.h"
#include "sview/views/View.h"
#include "m3d/utils/Platform.h"


SA_NAMESPACE_BEGIN

CSimulationAnimationManager::CSimulationAnimationManager(const char* CurSVLVersion,const char* CurViewerVersion)
{
	m_BehaviorActionList = new_vlist( malloc, free );
	m_CurSAID = -1;
	wcscpy(m_strSAFilePath, L"");
	strcpy(m_Version, ANIMATION_VERSION);
	strcpy(m_Company, "HoteamSoft");
	strcpy(m_Name, "DefaultName");
	if(CurSVLVersion)
	{
		strcpy(m_CurSVLVersion, CurSVLVersion);
	}
	else
	{
		strcpy(m_CurSVLVersion, "2.356");
	}
	if(CurViewerVersion)
	{
		strcpy(m_CurViewerVersion, CurViewerVersion);
	}
	else
	{
		strcpy(m_CurViewerVersion, "5.0");
	}
	strcpy(m_SVLVersion, "");
	strcpy(m_ViewerVersion, "");
	m_pAnimationStepManager = 0;

	m_bCameraPlay = true;
	m_bShowTrochoid = false;
	m_bPlaySound = false;
	m_bPlayVisible = true;
	m_bPlayImage = false;
	m_bPlayPosRot = true;
	m_bPlayClip = false;
	m_bPlayColor = true;
	m_bCollisionCheck = false;
	m_CollisionDelay = 100;
	m_bStopPlayAfterCollision = false;

	m_bContinuousPlay = false;
	m_bReversePlay = false;
	m_fPlaySpeed = 1.0f;

	m_pAnimationPlayApi = new AnimationPlayApi();

	m_InitTargetObjectList = new_vlist( malloc, free );

}

CSimulationAnimationManager::~CSimulationAnimationManager(void)
{
	if(m_pAnimationStepManager)
	{
		delete m_pAnimationStepManager;
		m_pAnimationStepManager = NULL;
	}

	DeleteAllSimAni();
	delete_vlist(m_BehaviorActionList);

	DeleteAllInitTargetObject();
	delete_vlist(m_InitTargetObjectList);
	
	if(m_pAnimationPlayApi)
	{
		delete m_pAnimationPlayApi;
		m_pAnimationPlayApi = NULL;
	}

}

void CSimulationAnimationManager::Serialize(CUtilityXMLGenerator *xmlgen)
{
	
	CUtilityXMLTag xmlt;
	xmlgen->WriteToBuffer("<?xml version=\"1.0\" encoding = \"UTF-8\"?>\n");
	xmlt.SetTagname("SimulationAnimation");
	xmlt.AddProperty("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	strcpy(m_Version,ANIMATION_VERSION);
	xmlt.AddProperty("Version", m_Version, true);
	xmlt.AddProperty("Company", m_Company, true);
	xmlt.AddProperty("Name", m_Name, true);
	xmlt.AddPropertyInt("CurSAID", m_CurSAID);
	xmlt.AddProperty("SVLVersion", m_CurSVLVersion, true);
	xmlt.AddProperty("ViewerVersion", m_CurViewerVersion, true);
	xmlt.SetOpen(true);
	xmlgen->AddTag(&xmlt);

	vlist_reset_cursor(m_BehaviorActionList);
	
	/*CUtilityXMLGenerator xmlgenSA;
	CUtilityXMLTag xmltSA;
	xmltSA.SetTagname("SimulationAnimationList");
	char* strSA = NULL;
	size_t sizeSA = 0;*/
	while (1)
	{
		CSBehaviorAction *temp;
		//xmlgenSA.Reset();
		if (!(temp = (CSBehaviorAction *)vlist_peek_cursor(m_BehaviorActionList)))
			break;
		if(temp == GetAnimationStepManager()->GetBehaviorActionChgCam())
		{
			vlist_advance_cursor(m_BehaviorActionList);
			continue;
		}
		temp->Serialize(xmlgen, true);
		//xmlgenSA.Finalize();
		//sizeSA = xmlgenSA.GetBufferSize();
		//if(0 != sizeSA)
		//{
		//	//sizeSA += sizeof(char);//考虑到以后Unicode问题
		//	strSA = new char[sizeSA];
		//	ZeroMemory(strSA, sizeSA);
		//	memcpy_s(strSA, sizeSA, xmlgenSA.GetBuffer(), xmlgenSA.GetBufferSize());
		//	xmltSA.AddListItem(strSA);
		//	delete[] strSA;
		//}
		vlist_advance_cursor(m_BehaviorActionList);	
	}

	//对象的初始状态
	Serialize_InitTargetObject(xmlgen);

	//分段动画信息
	if(m_pAnimationStepManager)
	{
		m_pAnimationStepManager->Serialize(xmlgen);
	}
	//xmlgen->AddTag(&xmltSA);
	xmlgen->CloseTag(&xmlt);
}

CSBehaviorAction * CSimulationAnimationManager::AddSimAni( int ID, int tps /*= 10*/, int delay /*= 0*/, const char *name /*= 0*/, const char *version /*=0*/, const char *company /*= 0*/ )
{
	///暂时限制，只允许一个动画文件
	CSBehaviorAction* pBehaviorAction = NULL;
	if((pBehaviorAction = FindSimAniByID(ID)))
	{
		m_CurSAID = ID;
		pBehaviorAction->SetName(name);
		pBehaviorAction->SetTicksPerSecond(tps);
		pBehaviorAction->DeleteAllAnimations();
	}
	else
	{
		pBehaviorAction = new CSBehaviorAction(ID, tps, delay, name, version, company);
		AddSimAni(pBehaviorAction);
	}
	
	return pBehaviorAction;
}

void CSimulationAnimationManager::AddSimAni( CSBehaviorAction *pbBehaviorAction )
{
	/////暂时限制，只允许一个动画文件
	//CSBehaviorAction* pBehaviorAction = NULL;
	//StartListSimAniQuery();
	//if(GetNextListSimAni(&pBehaviorAction))
	//{
	//	DeleteSimAni(pBehaviorAction);
	//}

	if(NULL != pbBehaviorAction)
	{
		vlist_add_last(m_BehaviorActionList, pbBehaviorAction);
		pbBehaviorAction->Reference();
		m_CurSAID = pbBehaviorAction->GetID();
		pbBehaviorAction->SetSimulationAnimationManager(this);
	}
}

void CSimulationAnimationManager::DeleteSimAni( CSBehaviorAction *pbehaviorManager )
{
	if(NULL != pbehaviorManager)
	{
		if(pbehaviorManager->IsPlaying())
		{
			pbehaviorManager->Stop();
		}

		if(pbehaviorManager->GetID() == m_CurSAID)
		{
			if(GetBehaviorActionCount()>0)
			{
				m_CurSAID = 0;
			}
			else
			{
				m_CurSAID = -1;
			}
		}
		vlist_remove(m_BehaviorActionList, pbehaviorManager);
		pbehaviorManager->Release();
	}
}

void CSimulationAnimationManager::DeleteAllSimAni()
{
	START_LIST_ITERATION(CSBehaviorAction, m_BehaviorActionList)
		if(temp->IsPlaying())
		{
			temp->Stop();
		}
		temp->Release();
	END_LIST_ITERATION(m_BehaviorActionList)

		vlist_flush(m_BehaviorActionList);
	m_CurSAID = -1;
}

CSBehaviorAction * CSimulationAnimationManager::FindSimAniByID( const int ID )
{
	START_LIST_ITERATION(CSBehaviorAction, m_BehaviorActionList)
		if(ID == temp->GetID())
		{
			//m_CurSAID = ID;
			return temp;
		}
		END_LIST_ITERATION(m_BehaviorActionList)
			return NULL;
}

void CSimulationAnimationManager::ProcessXMLData( const char *buffer )
{
	//如果是UTF-8编码，转换成ANSI编码
	bool isCreateSABuffer = false;
	char	*pSABuffer = NULL;
#ifdef _WIN32
	if (buffer[0] == '\xEF' && buffer[1] == '\xBB' && buffer[2] == '\xBF')
	{
		wstring wsrSA = Platform::StringToWString(buffer+3);
		string srSA = Platform::WStringToString(wsrSA,"auto");

		int size = srSA.size()+1;
		pSABuffer = new char[size];

		memset(pSABuffer, 0, size);
		strcpy(pSABuffer, srSA.c_str());
		isCreateSABuffer = true;
	}
	else
	{
		pSABuffer = (char*)buffer;
	}
#else
	SA_UTF8 utf8Obj;
	if(buffer[0] == '\xEF' && buffer[1] == '\xBB' && buffer[2] == '\xBF')
	{
		pSABuffer = (char*)buffer;
	}
	else
	{
		utf8Obj.append(buffer);
		pSABuffer = (char*)utf8Obj.encodedText();
		isCreateSABuffer = true;
	}
#endif
	ClearData();
	CUtilityXMLParser xp;

	char doctype[SA_BUFFER_SIZE_SMALL] = {0};
	xp.FindDocType(doctype, pSABuffer/*buffer*/);
	xp.SetupParseBuffer(pSABuffer/*buffer*/);

	xp.SetTagCallback("SimulationAnimation", CSimulationAnimationManager::XMLCallback, this);
	xp.SetTagCallback("Animdef", CSBehaviorAction::XMLCallback, this);
	xp.SetTagCallback("BehaviorManager", CSBehaviorAction::XMLCallback, this);
	xp.SetTagCallback("Animation", CSAnimation::XMLCallback, this);
	xp.SetTagCallback("PosInterpolator", CSInterpolatorPosition::XMLCallback, this);
	xp.SetTagCallback("Timeline", CSTimeline::XMLCallback, this);
	xp.SetTagCallback("TargetObject", STargetObject::XMLCallback, this);
	xp.SetTagCallback("QuatRotInterpolator", CSInterpolatorQuatSquad::XMLCallback, this);
	xp.SetTagCallback("ScaleInterpolator", CSInterpolatorScale::XMLCallback, this);
	xp.SetTagCallback("VisibleInterpolator", CSInterpolatorVisible::XMLCallback, this);
	xp.SetTagCallback("ColorInterpolator", CSInterpolatorColor::XMLCallback, this);
	xp.SetTagCallback("PivotInterpolator", CSInterpolatorPivot::XMLCallback, this);
	xp.SetTagCallback("SoundInterpolator", CSInterpolatorSound::XMLCallback, this);
	xp.SetTagCallback("NormalInterpolator", CSInterpolatorNormal::XMLCallback, this);

	xp.SetTagCallback("InitTargetObject", CSimulationAnimationManager::XMLCallback_InitTargetObject, this);
	xp.SetTagCallback("AnimationStep", CAnimationStepManager::XMLCallback, this);
	xp.SetTagCallback("AnimationStepManager", CAnimationStepManager::XMLCallback, this);
	xp.SetTagCallback("ProcessManager", CProcessManager::XMLCallback, this);
	xp.SetTagCallback("Process", CProcess::XMLCallback, this);
	xp.SetTagCallback("ProcessTargetObject", CProcessTargetObject::XMLCallback, this);
	xp.SetTagCallback("InstanceCreateInterpolator", CSInterpolatorInstanceCreate::XMLCallback, this);
	xp.ProcessXMLData();

	VersionConvert_1_To_2_1();

	//如果动画文件中没有动画，创建默认的动画管理器
	if(GetBehaviorActionCount() <=0)
	{
		AddSimAni(0);
	}

	//如果没有过程信息，创建默认过程，以兼容2.1及之前版本的动画
	if(!m_pAnimationStepManager)
	{
		m_pAnimationStepManager = CreateAnimationStepManager();
	}
	CProcessManager* pProcessManager = m_pAnimationStepManager->GetCurrentProcessManager();
	if(!pProcessManager)
	{
		pProcessManager = m_pAnimationStepManager->AddProcessManager();
	}
	CProcess* pProcess = pProcessManager->GetCurrentProcess();
	if(!pProcess)
	{
		//如果当前活动过程管理器没有活动过程，取其它过程管理器的活动作为活动过程
		for(int i = 0; i < m_pAnimationStepManager->GetProcessManagerCount(); i++)
		{
			pProcessManager = m_pAnimationStepManager->GetProcessManagerByIdx(i);
			if(pProcessManager)
			{
				pProcess = pProcessManager->GetCurrentProcess();
				if(pProcess)
				{
					break;
				}
			}
		}
		if(!pProcess)
		{
			//如果当前动画文件中没有过程，创建过程，并将过程与当前活动动画关联
			pProcessManager->AddProcess(0,0,false);
		}
		else{
			//如果从其它过程管理器中找到过程，设置活动的过程
			pProcessManager->SetCurProcessByID(pProcess->GetID(),false);
		}
	}
//    if (isCreateSABuffer)
//    {
//        delete[] pSABuffer;
//    }
}


void * CSimulationAnimationManager::XMLCallback( CUtilityXMLTag *xt, bool open, void *m_pExtraData )
{
	CSimulationAnimationManager *pSA =(CSimulationAnimationManager*) m_pExtraData;
	/*char*	strSABuffer = NULL;*/
	if(open)
	{
		char Version[SA_BUFFER_SIZE_SMALL] = {0};
		if (!xt->GetProperty("Version", Version))
			strcpy(Version, "1.1");
		if (!xt->GetProperty("Company", pSA->m_Company))
			strcpy(pSA->m_Company, "HoteamSoft");
		if (!xt->GetProperty("Name", pSA->m_Name))
			strcpy(pSA->m_Name, "DefaultName");
		if (!xt->GetProperty("CurSAID", pSA->m_CurSAID))
			pSA->m_CurSAID = 0;

		if (!xt->GetProperty("SVLVersion", pSA->m_SVLVersion))
			strcpy(pSA->m_SVLVersion, "");
		if (!xt->GetProperty("ViewerVersion", pSA->m_ViewerVersion))
			strcpy(pSA->m_ViewerVersion, "");

		if(strlen(Version) > 0 )
		{
			strcpy(pSA->m_Version, Version);
		}
		if (SATools::VersionCompare(pSA->m_Version, ANIMATION_VERSION) > 0)
		{
			//AfxMessageBox(_T("动画打开失败（版本不兼容）"));
			return NULL;//版本不兼容
		}

		if(SATools::VersionCompare("2.0",Version) > 0 )
		{
			CUtilityXMLParser* pUtilityXMLParser = xt->GetXMLParser();
			pUtilityXMLParser->SetTagCallback("Animation", CSAnimation::XMLCallback_V1, pSA);
			pUtilityXMLParser->SetTagCallback("PosInterpolator", CSInterpolatorPosition::XMLCallback_V1, pSA);
			pUtilityXMLParser->SetTagCallback("TargetObject", STargetObject::XMLCallback_V1, pSA);
			pUtilityXMLParser->SetTagCallback("AttSwitchInterpolator", CSInterpolatorVisible::XMLCallback, pSA);
		}

		/*xt->StartListItemsQuery();
		while(1 && xt->HasListItems())
		{
			if(!xt->GetNextListItem(&strSABuffer))
				break;
			pSA->AddSimAni(CSBehaviorAction::ProcessXMLData(strSABuffer));
		}*/
		return m_pExtraData;
	}
	else
	{

	}
	return 0;
}

//将之前的动画版本转换为2.0的版本
void CSimulationAnimationManager::VersionConvert_1_To_2_1()
{
	if( SATools::VersionCompare("2.0",m_Version) > 0)
	{
		CSBehaviorAction* pBehaviorAction = NULL;
		STargetObject* pTargetObject = NULL;
		char cName[SA_BUFFER_SIZE_SMALL] = {0};
		for(int i = 0; i < (int)vlist_count(m_BehaviorActionList); i++)
		{
			pBehaviorAction = (CSBehaviorAction*)vlist_nth_item(m_BehaviorActionList,i);
			if(pBehaviorAction)
			{
				vlist_s * pTargetObjectList = pBehaviorAction->GetTargetObjectList();
				for(int j = 0; j < (int)vlist_count(pTargetObjectList); j++)
				{
					CSAnimation* pAnimationTargetObject = NULL;
					CSTimeline* pTimeLineTargetObject = NULL;
					pTargetObject = (STargetObject*)vlist_nth_item(pTargetObjectList,j);
					//创建实例的主动画
					if(pTargetObject && 
						!(pAnimationTargetObject = pBehaviorAction->FindAnimation(pTargetObject->GetPath(),NULL)))
					{
						pAnimationTargetObject = CSACommonAPI::GetTargetMainAmimation(pTargetObject->GetPath(),pTargetObject->GetName(),pBehaviorAction);
						if(pAnimationTargetObject)
						{
							pTimeLineTargetObject = pAnimationTargetObject->GetTimeline();
						}
					}
					if(pTargetObject && strcmp(pTargetObject->GetType(),TARGETOBJECTTYPE_INS)==0)
					{
						//将2.0之前版本的平移、旋转动画合并为一条动画，并添加旋转点插值器__START
						//获取平移信息或旋转信息
						CSAnimation* pAnimationPos = pBehaviorAction->FindAnimation(pTargetObject->GetPath(),INTERPOLATOR_POS);
						CSAnimation* pAnimationRot = pBehaviorAction->FindAnimation(pTargetObject->GetPath(),INTERPOLATOR_QUATROT,true);
						CSTimeline* pTimeLinePos = NULL;
						CSTimeline* pTimeLineRot = NULL;
						CSInterpolatorPosition* pInterpolatorPos = NULL;
						CSInterpolatorQuatSquad* pInterpolatorRot = NULL;
						bool bHasPosAnimation = false;
						bool bHasRotAnimation = false;
						if(pAnimationPos)
						{
							pTimeLinePos = pAnimationPos->GetTimeline();
							pInterpolatorPos = (CSInterpolatorPosition*)pAnimationPos->GetInterpolator();
							pInterpolatorRot = (CSInterpolatorQuatSquad*)pAnimationPos->GetInterpolator(INTERPOLATOR_QUATROT);
							if(pTimeLinePos && pInterpolatorPos && pInterpolatorRot)
							{
								bHasPosAnimation = true;
							}
						}
						if(pAnimationRot)
						{
							pTimeLineRot = pAnimationRot->GetTimeline();
							pInterpolatorRot = (CSInterpolatorQuatSquad*)pAnimationRot->GetInterpolator();
							if(pTimeLineRot && pInterpolatorRot)
							{
								bHasRotAnimation = true;
							}
						}
						if(bHasPosAnimation || bHasRotAnimation)
						{
							bool bReplace = false;
							strcpy(cName,"位置");
							//strcat(cName,pTargetObject->GetName());
							CSAnimation* pAnimationPosRot = CSACommonAPI::AddAnimation(pBehaviorAction,cName,pTargetObject->GetPath(),pTargetObject->GetPivot(),pAnimationTargetObject);
							CSTimeline* pTimeLinePosRot =new CSTimeline();
							CSInterpolatorPosition* pInterpolatorPosRotPos = NULL;
							CSInterpolatorQuatSquad* pInterpolatorPosRotRot = NULL;
							CSInterpolatorPivot* pInterpolatorPosRotPivot = new CSInterpolatorPivot();
							pAnimationPosRot->SetTimeline(pTimeLinePosRot);
							pAnimationPosRot->AddInterpolator(pInterpolatorPosRotPivot);
							if(bHasPosAnimation)
							{
								//如果有平移信息，添加关键帧，并添加平移和旋转插值器
								for(int idx= 0; idx < pTimeLinePos->GetTimelineArrayLength(); idx++)
								{
									pTimeLinePosRot->AddKeyframe(pTimeLinePos->GetTimelineArray()[idx],bReplace);
								}
								pInterpolatorPosRotPos = new CSInterpolatorPosition();
								pAnimationPosRot->AddInterpolator(pInterpolatorPosRotPos);
								pInterpolatorPosRotRot = new CSInterpolatorQuatSquad();
								pAnimationPosRot->AddInterpolator(pInterpolatorPosRotRot);
							}
							if(bHasRotAnimation)
							{
								//如果有旋转动画，添加关键帧，并添加旋转插值器
								for(int idx= 0; idx < pTimeLineRot->GetTimelineArrayLength(); idx++)
								{
									pTimeLinePosRot->AddKeyframe(pTimeLineRot->GetTimelineArray()[idx],bReplace);
								}
								if(!pInterpolatorPosRotRot)
								{
									pInterpolatorPosRotRot = new CSInterpolatorQuatSquad();
									pAnimationPosRot->AddInterpolator(pInterpolatorPosRotRot);
								}
							}

							//设置各关键帧的值
							for(int idx = 0; idx< pTimeLinePosRot->GetTimelineArrayLength();idx++)
							{
								if(pInterpolatorPosRotPivot)
								{
									//添加旋转点插值器值
									pInterpolatorPosRotPivot->Insert(*pTargetObject->GetPivot(),idx);
								}
								if(pInterpolatorPosRotRot)
								{
									//添加转换插值器值
									AniQuat quat(0.0,0.0,0.0,1.0);
									if(bHasRotAnimation)
									{
										//如果有旋转动画，取旋转动画的当前帧为旋转关键帧
										int interval = 0;
										float fraction = 0.0f;
										pTimeLineRot->Evaluate(pTimeLinePosRot->GetTimelineArray()[idx],interval,fraction);
										pInterpolatorRot->CalculateQuat(interval,fraction,quat);
										if(((CKeyframeQuatSquad*)(pAnimationRot->GetInterpolator()->GetArray()[0]))->m_bLinear)
										{
											pInterpolatorPosRotRot->InsertLinear(quat,idx);
										}
										else
										{
											pInterpolatorPosRotRot->Insert(quat,idx);
										}
									}
									else if(bHasPosAnimation)
									{
										//如果没有旋转动画，取平移的参照旋转信息为旋转关键帧
										quat = ((CKeyframeQuatSquad*)pInterpolatorRot->GetAt(idx))->m_quat;
										pInterpolatorPosRotRot->InsertLinear(quat,idx);
									}
								}
								if(pInterpolatorPosRotPos && bHasPosAnimation)
								{
									//添加平移插值器值
									AniPoint pntPos;
									int interval = 0;
									float fraction = 0.0f;
									int iPosTick = pTimeLinePosRot->GetTimelineArray()[idx];
									pTimeLinePos->Evaluate(iPosTick,interval,fraction);
									pInterpolatorPos->CalculatePos(interval,fraction,pntPos);

									//获取当前位置的参照旋转信息
									AniQuat quat(0.0,0.0,0.0,1.0);
									CSInterpolatorQuatSquad* pInterpolatorRot2 = (CSInterpolatorQuatSquad*)pAnimationPos->GetInterpolator(INTERPOLATOR_QUATROT);
									int keyFrameIdx = interval;
									if(interval>=pInterpolatorRot2->GetArrayLength())
									{
										keyFrameIdx = pInterpolatorRot2->GetArrayLength()-1;
									}
									else
									{
										if(fraction == 1.0f)
										{
											keyFrameIdx++;
										}
									}
									quat = ((CKeyframeQuatSquad*)pInterpolatorRot2->GetAt(keyFrameIdx))->m_quat;

									//处理先平移，再旋转，再平移的过渡帧
									if(keyFrameIdx > 0 && fraction == 1.0f)
									{
										AniQuat quat2 = ((CKeyframeQuatSquad*)pInterpolatorRot2->GetAt(keyFrameIdx-1))->m_quat;
										//if(fabs(quat2.x - quat.x) > ANIMATION_D_TOL2 || 
										//	fabs(quat2.y - quat.y) > ANIMATION_D_TOL2 || 
										//	fabs(quat2.z - quat.z) > ANIMATION_D_TOL2 || 
										//	fabs(quat2.w - quat.w) > ANIMATION_D_TOL2)
										if(!quat2.Equal(quat,(float)ANIMATION_D_TOL2))
										{
											int iTempPosTick = pTimeLinePos->GetTimelineArray()[keyFrameIdx-1];
											pTimeLinePosRot->Evaluate(iTempPosTick,interval,fraction);
											int preKeyFrameIdx = interval;
											if(fraction == 1.0f)
											{
												preKeyFrameIdx++;
											}
											pInterpolatorPosRotPos->Append(pInterpolatorPosRotPos->GetAt(preKeyFrameIdx)->Clone());
											continue;
										}
									}

									//将模型实例的坐标位置转换为沿旋转点旋转至单位矩阵的位置
									AniPoint pntPivot = *pTargetObject->GetPivot();
									pntPos = CSACommonAPI::MtxTanslationToUniTanslation(pntPivot,quat,pntPos);

									CKeyframeChannel* kp = (CKeyframeChannel*)pInterpolatorPos->GetAt(0);
									if(kp->m_channeltype == ANILinear)
									{
										pInterpolatorPosRotPos->InsertLinear(pntPos,idx);
									}
									else
									{
										pInterpolatorPosRotPos->InsertCurve(pntPos,idx);
									}
								}
							}
							pTimeLinePosRot->AddTLRange();
							//合并完成后，清除原来的平移及旋转动画
							pBehaviorAction->DeleteAnimation(pAnimationPos);
							pBehaviorAction->DeleteAnimation(pAnimationRot);
						}
						//将2.0之前版本的平移、旋转动画合并为一条动画，并添加旋转点插值器__END
					}
					else if(pTargetObject && pTimeLineTargetObject && strcmp(pTargetObject->GetType(),TARGETOBJECTTYPE_CAM) == 0)
					{
						CSAnimation* pAnimationPos = pBehaviorAction->FindAnimation(pTargetObject->GetPath(),INTERPOLATOR_POS);
						CSAnimation* pAnimationRot = pBehaviorAction->FindAnimation(pTargetObject->GetPath(),INTERPOLATOR_QUATROT,true);
						CSAnimation* pAnimationScale = pBehaviorAction->FindAnimation(pTargetObject->GetPath(),INTERPOLATOR_SCALE);

						//添加新格式动画
						CSInterpolatorPosition * pInterpolatorPos = new CSInterpolatorPosition();
						CSInterpolatorQuatSquad * pInterpolatorRot = new CSInterpolatorQuatSquad();
						CSInterpolatorScale * pInterpolatorScale = new CSInterpolatorScale();
						pAnimationTargetObject->AddInterpolator(pInterpolatorPos);
						pAnimationTargetObject->AddInterpolator(pInterpolatorRot);
						pAnimationTargetObject->AddInterpolator(pInterpolatorScale);
						CSTimeline* pTimeLinePos = pAnimationPos->GetTimeline();
						int numKeyFrame = pTimeLinePos->GetTimelineArrayLength();
						for(int idx=0; idx< numKeyFrame; idx++)
						{
							int currentTick = pTimeLinePos->GetTimelineArray()[idx];
							int keyFrameIdx = pTimeLineTargetObject->AddKeyframe(currentTick);
							pInterpolatorPos->Append(pAnimationPos->GetInterpolator()->GetAt(idx)->Clone());
							pInterpolatorRot->Append(pAnimationRot->GetInterpolator()->GetAt(idx)->Clone());
							pInterpolatorScale->Append(pAnimationScale->GetInterpolator()->GetAt(idx)->Clone());

							//1.1及之前版本没有记录缩放比例为1时的视景体大小，设置为0
							if(SATools::VersionCompare(m_Version,"1.1") == 0 )
							{
								((CKeyframeChannel *)pInterpolatorScale->GetAt(keyFrameIdx))->m_cp.y = 0;
								((CKeyframeChannel *)pInterpolatorScale->GetAt(keyFrameIdx))->m_cp.z = 0;
							}

							////2.0之前版本录制的为摄像机矩阵的逆矩阵，求逆转换为摄像机原始矩阵
							//float matrix[4][4];
							//((CKeyframeQuatSquad *)pInterpolatorRot->GetAt(idx))->m_quat.ToMatrix((float*)matrix);
							//float pos[3];
							//pos[0] = ((CKeyframeChannel *)pInterpolatorPos->GetAt(idx))->m_cp.x;
							//pos[1] = ((CKeyframeChannel *)pInterpolatorPos->GetAt(idx))->m_cp.y;
							//pos[2] = ((CKeyframeChannel *)pInterpolatorPos->GetAt(idx))->m_cp.z;
							//matrix[3][0] = pos[0];
							//matrix[3][1] = pos[1];
							//matrix[3][2] = pos[2];
							//DOUBLE dMatrix[4][4];
							//MatrixOperation::FloatMtxToDoubleMtx(matrix,dMatrix);
							//MatrixOperation::MatrixInversion((DOUBLE*)dMatrix);
							//MatrixOperation::DoubleMtxToFloatMtx(dMatrix,matrix);
							//((CKeyframeQuatSquad *)pInterpolatorRot->GetAt(idx))->m_quat = AniQuat::MatrixToQuaternion((float*)matrix);
							//((CKeyframeChannel *)pInterpolatorPos->GetAt(idx))->m_cp.Set(matrix[3][0],matrix[3][1],matrix[3][2]);
						}
						//删除老格式场景动画
						pBehaviorAction->DeleteAnimation(pAnimationPos);
						pBehaviorAction->DeleteAnimation(pAnimationRot);
						pBehaviorAction->DeleteAnimation(pAnimationScale);
					}
					//将显示隐藏动画添加到TargetObject动画下
					CSAnimation* pAnimationVisible = NULL;
					if(pTargetObject)
					{
						pAnimationVisible = pBehaviorAction->FindAnimation(pTargetObject->GetPath(),INTERPOLATOR_VISIBLE);
					}
					if(pAnimationVisible && pAnimationTargetObject)
					{
						strcpy(cName,"显隐");
						//strcat(cName,pTargetObject->GetName());
						pAnimationVisible->SetName(cName);
						pAnimationTargetObject->AddChildAnimation(pAnimationVisible);
					}
					//将颜色隐藏动画添加到TargetObject动画下
					CSAnimation* pAnimationColor = NULL;
					if(pTargetObject)
					{
						pAnimationColor = pBehaviorAction->FindAnimation(pTargetObject->GetPath(),INTERPOLATOR_COLOR);
					}
					if(pAnimationColor && pAnimationTargetObject)
					{
						strcpy(cName,"颜色");
						//strcat(cName,pTargetObject->GetName());
						pAnimationColor->SetName(cName);
						pAnimationTargetObject->AddChildAnimation(pAnimationColor);
					}
				}
			}
		}
		strcpy(m_Version,ANIMATION_VERSION);
	}
}

CSBehaviorAction* CSimulationAnimationManager::GetCurrentSA()
{
	if(-1 != m_CurSAID)
	{
		return FindSimAniByID(m_CurSAID);
	}
	return NULL;
}

int CSimulationAnimationManager::GetMaxID()
{
	int iMaxID = 0;
	int itmpID = 0;
	START_LIST_ITERATION(CSBehaviorAction, m_BehaviorActionList)
		itmpID = temp->GetID();
	if(itmpID > iMaxID)
		iMaxID = itmpID;
	END_LIST_ITERATION(m_BehaviorActionList)
		return iMaxID;
}

long CSimulationAnimationManager::RegisterBehaviorActionID()
{
	long iRet = 0;
	START_LIST_ITERATION(CSBehaviorAction, m_BehaviorActionList)
		if(temp->GetID() >= iRet)
		{
			iRet = temp->GetID()+1;
		}
		END_LIST_ITERATION(m_BehaviorActionList)
			return iRet;
}

void CSimulationAnimationManager::StartListSimAniQuery()
{
	vlist_reset_cursor(m_BehaviorActionList);
}

bool CSimulationAnimationManager::GetNextListSimAni(CSBehaviorAction** opBehaviorAction)
{
	void *temp;
	if (!(temp = vlist_peek_cursor(m_BehaviorActionList)))
		return false;
	*opBehaviorAction = (CSBehaviorAction *)temp;
	vlist_advance_cursor(m_BehaviorActionList);	
	return true;
}

bool CSimulationAnimationManager::HasListSimAni()
{
	vlist_reset_cursor(m_BehaviorActionList);
	if (!vlist_peek_cursor(m_BehaviorActionList))
		return false;
	else 
		return true;
}
int CSimulationAnimationManager::GetBehaviorActionCount() 
{ 
	return m_BehaviorActionList->count; 
}

bool CSimulationAnimationManager::ReadFromFile(const char *filename){
	return ReadFromFile((wchar_t*)SA_WCS(filename).encodedText());
}

#ifdef _MSC_VER
bool CSimulationAnimationManager::ReadFromFile(const unsigned short *filename)
{
	return ReadFromFile((wchar_t const *) filename);
}
#endif

bool CSimulationAnimationManager::ReadFromFile(const wchar_t *filename)
{
	bool bRet = false;
	ClearData();
	static bool first = true;
	static wchar_t oldfilename[SA_BUFFER_SIZE_SMALL] = {0};
	wchar_t loadfilename[SA_BUFFER_SIZE_SMALL] = {0};
	if (!filename)
	{
		if (first)
			return bRet;
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
		return bRet;
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
	{
		ProcessXMLData(buffer);
	}
	delete [] buffer;
	wcscpy(oldfilename, loadfilename);
	wcscpy(m_strSAFilePath, loadfilename);
	if(GetBehaviorActionCount() > 0)
	{
		bRet = true;
	}
	return bRet;
}

void CSimulationAnimationManager::ReadFromData(const char *xmlData)
{
	if(strlen(xmlData) == 0){
		return;
	}

	ProcessXMLData(xmlData);
}

int CSimulationAnimationManager::WriteToFile(const char *filename){
	return WriteToFile((wchar_t*)SA_WCS(filename).encodedText());
}

#ifdef _MSC_VER
int CSimulationAnimationManager::WriteToFile(const unsigned short *filename)
{
	return WriteToFile((wchar_t const *) filename);
}
#endif

int CSimulationAnimationManager::WriteToFile(const wchar_t *filename)
{
	int bRet= -1;
	if(filename && wcslen(filename) > 0)
	{
		wcscpy(m_strSAFilePath, filename);
	}
	if(wcslen(m_strSAFilePath) <=0)
	{
		return -2;
	}
	CUtilityXMLGenerator xmlgen;
	xmlgen.Reset();
	Serialize(&xmlgen);
	xmlgen.Finalize();
	FILE *fp;
	fp = wfopen(m_strSAFilePath, L"w");
	if(fp == NULL)
	{
		//wchar_t strErrMsg[SA_BUFFER_SIZE_BIG];
		//wsprintf(strErrMsg,L"文件写入失败\n请检查（%s）文件或所属目录是否有写入权限! ",filename);
		//AfxMessageBox(strErrMsg);
		return -3;
	}
	//int nSize = fwrite(xmlgen.GetBuffer(), 1, xmlgen.GetBufferSize()-1, fp);
	//保存为UTF-8编码
	SA_UTF8 utf8Obj(xmlgen.GetBuffer());
	const utf8_char *cUTFBuffer =  utf8Obj.encodedText();

	char UTF8BOM[3]={'\xEF','\xBB','\xBF'};
	size_t nSize = fwrite(UTF8BOM, 1, 3, fp);
	nSize += fwrite((const char*)cUTFBuffer, 1, strlen((const char*)cUTFBuffer), fp);
	

	
	fclose(fp);
	if((int)nSize > 0)
	{
		bRet = 0;
	}
	return bRet;
}

bool CSimulationAnimationManager::GetDataBuffer(char** pBuffer,int& nBufferSize)
{
	bool bRet= true;
	CUtilityXMLGenerator xmlgen;
	xmlgen.Reset();
	Serialize(&xmlgen);
	xmlgen.Finalize();
	
	//int nSize = fwrite(xmlgen.GetBuffer(), 1, xmlgen.GetBufferSize()-1, fp);
	//保存为UTF-8编码
	SA_UTF8 utf8Obj(xmlgen.GetBuffer());
	const utf8_char *cUTFBuffer =  utf8Obj.encodedText();

	char UTF8BOM[4]={'\xEF','\xBB','\xBF','\0'};
	nBufferSize = 3 + (int)utf8Obj.length() +1;
	*pBuffer = new char[nBufferSize];
	strcpy(*pBuffer,UTF8BOM);
	strcat(*pBuffer,(const char*)cUTFBuffer);
	(*pBuffer)[nBufferSize-1] = 0;

	return bRet;
}

bool CSimulationAnimationManager::HasAnimations()
{
	bool bRet = false;
	START_LIST_ITERATION(CSBehaviorAction, m_BehaviorActionList)
		if(temp->HasAnimations())
		{
			bRet = true;
			break;
		}
		END_LIST_ITERATION(m_BehaviorActionList)
	return bRet;
}


CAnimationStepManager * CSimulationAnimationManager::CreateAnimationStepManager( const char *name /*= 0*/)
{
	if(!m_pAnimationStepManager)
	{
		m_pAnimationStepManager = new CAnimationStepManager( name);
		m_pAnimationStepManager->SetSimulationAnimationManager(this);
	}
	else
	{
		m_pAnimationStepManager->SetName(name);
		m_pAnimationStepManager->DeleteAllProcessManager();
	}


	return m_pAnimationStepManager;
}
CProcess * CSimulationAnimationManager::GetCurrentProcess()
{
	if(!m_pAnimationStepManager)
	{
		return NULL;
	}
	else
	{
		return m_pAnimationStepManager->GetCurrentProcess();
	}
}

bool CSimulationAnimationManager::IsPlaying()
{
	bool bRet = false;
	START_LIST_ITERATION(CSBehaviorAction, m_BehaviorActionList)
		if(temp->IsPlaying())
		{
			bRet = true;
			break;
		}
	END_LIST_ITERATION(m_BehaviorActionList)
	return bRet;
}

void CSimulationAnimationManager::StopAll()
{
	START_LIST_ITERATION(CSBehaviorAction, m_BehaviorActionList)
		if(temp->IsPlaying())
		{
			temp->Stop();
		}
		END_LIST_ITERATION(m_BehaviorActionList)
}


void CSimulationAnimationManager::InitData()
{
	ClearData();
	AddSimAni(0);

	//如果没有过程信息，创建默认过程，以兼容2.1及之前版本的动画
	if(!m_pAnimationStepManager)
	{
		m_pAnimationStepManager = CreateAnimationStepManager();
	}
	CProcessManager* pProcessManager = m_pAnimationStepManager->GetCurrentProcessManager();
	if(!pProcessManager)
	{
		pProcessManager = m_pAnimationStepManager->AddProcessManager();
	}
	CProcess* pProcess = pProcessManager->GetCurrentProcess();
	if(!pProcess)
	{
		//创建过程，并将过程与当前活动动画关联
		pProcessManager->AddProcess(0,0,false);
	}

}

void CSimulationAnimationManager::ClearData()
{
	if(m_pAnimationStepManager)
	{
		if (m_pAnimationStepManager->IsPlaying())
		{
			//解决多线程正在播放动画时，清除动画崩溃问题。没有想到更好的办法，暂时这么解决
			m_pAnimationStepManager->Stop();
			Thread::Sleep(500);
		}

		delete m_pAnimationStepManager;
		m_pAnimationStepManager = NULL;
	}
	
	DeleteAllSimAni();
	DeleteAllInitTargetObject();

	if(m_pAnimationPlayApi)
	{
		//清除上次干涉的临时数据
		m_pAnimationPlayApi->ClearColisionData();
	}
}

CAnimationCallBack* CSimulationAnimationManager::GetAnimationCallBack()
{
	return m_pAnimationPlayApi->m_pAnimationCB;
}

void CSimulationAnimationManager::SetCameraPlay(bool bCameraPlay)
{ 
	m_bCameraPlay = bCameraPlay; 

	if (GetCurrentSA() && GetCurrentSA()->IsPlaying())
	{
		GetCurrentSA()->Stop();
		GetCurrentSA()->ScheduleAllAnimations(true);
		GetCurrentSA()->ExecuteAnimations(GetCurrentSA()->GetCurrentTick(), 0.0f);
		GetCurrentSA()->Continue();
	}
}

void CSimulationAnimationManager::SetPlayPosRotStatus(bool bPlayPosRot) 
{ 
	m_bPlayPosRot = bPlayPosRot; 
	
	if (GetCurrentSA() && GetCurrentSA()->IsPlaying())
	{
		GetCurrentSA()->Stop();
		GetCurrentSA()->ScheduleAllAnimations(true);
		GetCurrentSA()->ExecuteAnimations(GetCurrentSA()->GetCurrentTick(), 0.0f);
		GetCurrentSA()->Continue();
	}
}

void CSimulationAnimationManager::SetPlayVisibleStatus(bool bPlayVisible) 
{ 
	m_bPlayVisible = bPlayVisible;
	
	if (GetCurrentSA() && GetCurrentSA()->IsPlaying())
	{
		GetCurrentSA()->Stop();
		GetCurrentSA()->ScheduleAllAnimations(true);
		GetCurrentSA()->ExecuteAnimations(GetCurrentSA()->GetCurrentTick(), 0.0f);
		GetCurrentSA()->Continue();
	}
}

void CSimulationAnimationManager::SetPlayPicStatus(bool bPlayImage) 
{ 
	m_bPlayImage = bPlayImage; 
	
	if (GetCurrentSA() && GetCurrentSA()->IsPlaying())
	{
		GetCurrentSA()->Stop();
		GetCurrentSA()->ScheduleAllAnimations(true);
		GetCurrentSA()->ExecuteAnimations(GetCurrentSA()->GetCurrentTick(), 0.0f);
		GetCurrentSA()->Continue();
	}
}

void CSimulationAnimationManager::SetPlayClipStatus(bool bPlayClip)
{
	m_bPlayClip = bPlayClip;
	
	if (GetCurrentSA() && GetCurrentSA()->IsPlaying())
	{
		GetCurrentSA()->Stop();
		GetCurrentSA()->ScheduleAllAnimations(true);
		GetCurrentSA()->ExecuteAnimations(GetCurrentSA()->GetCurrentTick(), 0.0f);
		GetCurrentSA()->Continue();
	}
}

void CSimulationAnimationManager::SetPlayColorStatus(bool bPlayColor)
{
	m_bPlayColor = bPlayColor;
	
	if (GetCurrentSA() && GetCurrentSA()->IsPlaying())
	{
		GetCurrentSA()->Stop();
		GetCurrentSA()->ScheduleAllAnimations(true);
		GetCurrentSA()->ExecuteAnimations(GetCurrentSA()->GetCurrentTick(), 0.0f);
		GetCurrentSA()->Continue();
	}
}

/************************************************************************/
/* 记录对象的初始状态_START_
修改记录：
	add by likun 2015-11-05
*/
/************************************************************************/
long CSimulationAnimationManager::RegisterInitTargetObjectKey()
{
	long iRet = 0;
	START_LIST_ITERATION(CProcessTargetObject, m_InitTargetObjectList)
		if(temp->GetTargetKey() >= iRet)
		{
			iRet = temp->GetTargetKey()+1;
		}
		END_LIST_ITERATION(m_InitTargetObjectList)
		return iRet;
}

void CSimulationAnimationManager::ModifyProcessTargetObjectPath(const char* pOldPath, const char* pNewPath)
{
	//1 修改动画路径
	vlist_s* listBehaviorAction = (vlist_s* )GetBehaviorActionList();
	for (int iBehaviorAction = 0;iBehaviorAction < (int)vlist_count(listBehaviorAction);iBehaviorAction++)
	{
		CSBehaviorAction* pBehaviorAction = NULL;
		pBehaviorAction = (CSBehaviorAction*)vlist_nth_item(listBehaviorAction, iBehaviorAction);
		if(!pBehaviorAction) continue;
		STargetObject* pTargetObject = NULL;
		if(pTargetObject = pBehaviorAction->FindTargetObjectByPath(pOldPath))
		{
			pTargetObject->SetPath(pNewPath);
		}
	}
	//2 修改初始化对象的路径
	CProcessTargetObject *tInitobject;
	if (tInitobject = FindInitTargetObjectByPath(pOldPath))
		tInitobject->SetPath(pNewPath);
	//3 修改各步骤的对象的路径
	CAnimationStepManager* pAniStepManager = GetAnimationStepManager();
	if(!pAniStepManager) return;
	vlist_s* listProcessManager = pAniStepManager->GetProcessManagerList();
	for (int iProManager = 0;iProManager < (int)vlist_count(listProcessManager);iProManager++)
	{
		CProcessManager* pTmpProManager = NULL;
		pTmpProManager = (CProcessManager*)vlist_nth_item(listProcessManager, iProManager);
		if(!pTmpProManager) continue;
		vlist_s* listProcess =  pTmpProManager->GetProcessList();
		for (int iProcess = 0;iProcess < (int)vlist_count(listProcess);iProcess++)
		{
			CProcess* pTmpProcess = NULL;
			pTmpProcess = (CProcess*)vlist_nth_item(listProcess, iProcess);
			if(!pTmpProcess) continue;
			CProcessTargetObject* tObject;
			if(tObject = pTmpProcess->FindTargetObjectByPath(pOldPath))
				tObject->SetPath(pNewPath);
		}
 	}
}

void CSimulationAnimationManager::DeleteSpareAnimation()
{
	vlist_s* listBehaviorAction = (vlist_s* )GetBehaviorActionList();
	CAnimationStepManager* pAnimationStepManager =  GetAnimationStepManager();
	if(!pAnimationStepManager)
		return;
	vlist_s* ProMangerList = pAnimationStepManager->GetProcessManagerList();
	for (int iBehaviorAction = 0;iBehaviorAction < (int)vlist_count(listBehaviorAction);iBehaviorAction++)
	{
		CSBehaviorAction* pBehaviorAction = NULL;
		pBehaviorAction = (CSBehaviorAction*)vlist_nth_item(listBehaviorAction, iBehaviorAction);
		if(!pBehaviorAction) continue;
		bool bFindedFlag = false;
		for (int iProManager = 0;iProManager < (int)vlist_count(ProMangerList);iProManager++)
		{
			CProcessManager* pProcessManager = (CProcessManager*)vlist_nth_item(ProMangerList, iProManager);
			if(!pProcessManager)
				continue;
			if(pProcessManager->FindProcessByAnimationID(pBehaviorAction->GetID()))
			{
				bFindedFlag = true;
				break;
			}
		}
		if(!bFindedFlag)
		{
			DeleteSimAni(pBehaviorAction);
			iBehaviorAction--;
		}
	}
}

CProcessTargetObject* CSimulationAnimationManager::FindInitTargetObjectByPath(const char* targetPath)
{
	vlist_reset_cursor(m_InitTargetObjectList);
	while (1)
	{
		CProcessTargetObject *temp;
		if (!(temp = (CProcessTargetObject *)vlist_peek_cursor(m_InitTargetObjectList)))
			break;
		if (temp->IsEqual(targetPath))
			return temp;
		vlist_advance_cursor(m_InitTargetObjectList);	
	}
	return 0;
}

CProcessTargetObject * CSimulationAnimationManager::FindInitTargetObjectByByID( const int ID )
{
	START_LIST_ITERATION(CProcessTargetObject, m_InitTargetObjectList)
		if(ID == temp->GetTargetKey())
		{
			return temp;
		}
		END_LIST_ITERATION(m_InitTargetObjectList)
			return NULL;
}

void CSimulationAnimationManager::DeleteAllInitTargetObject()
{
	START_LIST_ITERATION(CProcessTargetObject, m_InitTargetObjectList)
		delete temp;
	END_LIST_ITERATION(m_InitTargetObjectList)

	vlist_flush(m_InitTargetObjectList);
}

void CSimulationAnimationManager::DeleteInitTargetObject(const char* targetPath)
{
	CProcessTargetObject* pTargetObject = FindInitTargetObjectByPath(targetPath);
	if(!pTargetObject)
	{
		return;
	}

	//删除所有过程中的相同对象
	if(m_pAnimationStepManager)
	{
		for(int i = 0; i < m_pAnimationStepManager->GetProcessManagerCount(); i++)
		{
			CProcessManager* pProcessManager = m_pAnimationStepManager->GetProcessManagerByIdx(i);
			if(pProcessManager)
			{
				for(int j = 0; j < pProcessManager->GetProcessCount(); j++ )
				{
					CProcess* pProcess = pProcessManager->GetProcessByIdx(j);
					if(pProcess)
					{
						CProcessTargetObject* pProcessTargetObject = pProcess->FindTargetObjectByID(pTargetObject->GetTargetKey());
						if(pProcessTargetObject)
						{
							pProcess->DeleteTargetObject(pProcessTargetObject);
						}
					}
				}
			}
		}
	}

	//删除该对象的动画
	vlist_reset_cursor(m_BehaviorActionList);
	while (1)
	{
		CSBehaviorAction *temp;
		if (!(temp = (CSBehaviorAction *)vlist_peek_cursor(m_BehaviorActionList)))
			break;
		temp->DeleteAnimation(pTargetObject->GetPath());
		vlist_advance_cursor(m_BehaviorActionList);	
	}

	vlist_remove(m_InitTargetObjectList, pTargetObject);
	delete pTargetObject;
}

CProcessTargetObject* CSimulationAnimationManager::AddInitTargetObject(TARGETOBJECTINFO& target)
{
	AniPoint pntPos(target.m_Pos[0],target.m_Pos[1],target.m_Pos[2]);
	AniPoint pntScale(target.m_Scale[0],target.m_Scale[1],target.m_Scale[2]);
	AniQuat quat(target.m_Quat[0],target.m_Quat[1],target.m_Quat[2],target.m_Quat[3]);

	CProcessTargetObject *pTargetObject = NULL;
	if ((pTargetObject = FindInitTargetObjectByPath(target.m_Path)))
	{
		pTargetObject->SetValue(target.m_Name, &pntPos, &quat, &pntScale, target.m_bVisible, target.m_Trans);
	}
	else
	{
		pTargetObject = new CProcessTargetObject(this, target.m_Name, target.m_Path, &pntPos, &quat, &pntScale, target.m_bVisible, target.m_Trans);
		AddInitTargetObject(pTargetObject);
	}
	return pTargetObject;
}

CProcessTargetObject* CSimulationAnimationManager::AddInitTargetObject(CProcessTargetObject *pTargetObject)
{
	vlist_add_last(m_InitTargetObjectList, (void *)pTargetObject);
	return pTargetObject;
}

CProcessTargetObject * CSimulationAnimationManager::CreateInitTargetObjectByPath(const char *name, const char *path)
{
	CProcessTargetObject *tobject;
	if (!(tobject = FindInitTargetObjectByPath(path)))
	{
		tobject = new CProcessTargetObject(this, name, path);
		AddInitTargetObject(tobject);
	}
	return tobject;
}

void CSimulationAnimationManager::UpdateInitTargetObjectList(std::vector<TARGETOBJECTINFO*>& vecTarget)
{
	DeleteAllInitTargetObject();
	for(int i = 0; i < (int)vecTarget.size(); i++)
	{
		TARGETOBJECTINFO* target = vecTarget[i];
		AniPoint pntPos(vecTarget[i]->m_Pos[0],vecTarget[i]->m_Pos[1],vecTarget[i]->m_Pos[2]);
		AniPoint pntScale(vecTarget[i]->m_Scale[0],vecTarget[i]->m_Scale[1],vecTarget[i]->m_Scale[2]);
		AniQuat quat(vecTarget[i]->m_Quat[0],vecTarget[i]->m_Quat[1],vecTarget[i]->m_Quat[2],vecTarget[i]->m_Quat[3]);
		if (strlen(target->m_Path) == 0 || strcmp(target->m_Path, "PLCID:") == 0)
			continue;
		CProcessTargetObject *ani = new CProcessTargetObject(this,target->m_Name,target->m_Path,&pntPos,&quat,&pntScale,target->m_bVisible);
		vlist_add_last(m_InitTargetObjectList, (void *)ani);
	}
}

void *CSimulationAnimationManager::XMLCallback_InitTargetObject(CUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	if(!xt->GetXMLParser()->GetStackData())
	{
		return NULL;
	}

	CSimulationAnimationManager * pSAManager = (CSimulationAnimationManager*)xt->GetXMLParser()->GetStackData();
	if (open)
	{

	}
	else
	{
		
	}
	return pSAManager;
}

void CSimulationAnimationManager::Serialize_InitTargetObject(CUtilityXMLGenerator *xmlgen)
{

	CUtilityXMLTag xmlt;
	xmlt.SetTagname("InitTargetObject");

	xmlt.SetOpen(true);
	xmlgen->AddTag(&xmlt);

	vlist_reset_cursor(m_InitTargetObjectList);
	while (1)
	{
		CProcessTargetObject *temp;
		if (!(temp = (CProcessTargetObject *)vlist_peek_cursor(m_InitTargetObjectList)))
			break;
		temp->Serialize(xmlgen);
		vlist_advance_cursor(m_InitTargetObjectList);	
	}

	xmlgen->CloseTag(&xmlt);
}
/************************************************************************/
/* 记录对象的初始状态_END_                                                                     */
/************************************************************************/

SA_NAMESPACE_END


