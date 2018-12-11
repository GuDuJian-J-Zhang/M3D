// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		AnimationAPI.cpp
 *
 *	@brief	����������ز����ӿ���ʵ��
 *
 *	@par	��ʷ:
 *		2014/09/26	created by yhp
 ***********************************************"stdafx.h"*******************/
#include "StdAfx.h"
#include <sstream>
#include "../SimulationCommon/SAUtility.h"
#include "AnimationAPI.h"
#include "../SimulationCommon/MatrixOperation.h"
#include "CProcess.h"
#include "../StringTable.h"

CAnimationAPI *CAnimationAPI::m_pInstance = NULL;
CAnimationAPI::CAnimationAPI()
{
	m_pSAManager = NULL;
	m_pActiveAnimationFolder = NULL;
	m_bShowRecUI = false;
	m_bRec = true;
	m_bRecCamera = false;
	m_bAutoKey = true;
	m_numAutoKeyFrame = 20;
	m_bEvaluateFlag = true;
	m_blinear = true;
	m_pRecCB = NULL;
	m_pPlayCB = NULL;
	m_bAutoSave = true;
	m_bAutoCreateFolder = true;
	m_bRecording = false;
}

CAnimationAPI::~CAnimationAPI(void)
{
}

CAnimationAPI* CAnimationAPI::GetInstance(RecCB* pRecCB,void * pRecCBUserData)
{
	if(!m_pInstance)
	{
		m_pInstance = new CAnimationAPI();
	}
	if(pRecCB)
	{
		m_pInstance->SetRecCB(pRecCB,pRecCBUserData);
	}
	return m_pInstance;
}

void CAnimationAPI::SetSAManager( CSimulationAnimationManager * pSAManager )
{
	if(m_pSAManager != pSAManager)
	{
		m_pSAManager = pSAManager;
		m_pActiveAnimationFolder = NULL;
	}
}

NS_SimulationAnimation::CSBehaviorAction * CAnimationAPI::GetCurBehaviorAction()
{
	if(!m_pSAManager)
	{
		return NULL;
	}
	return m_pSAManager->GetCurrentSA();
}

NS_SimulationAnimation::CSBehaviorAction * CAnimationAPI::GetCurProcessBehaviorAction()
{
	if(!m_pSAManager)
	{
		return NULL;
	}

	CProcess* pCurProcess = m_pSAManager->GetCurrentProcess();
	if(!pCurProcess)
	{
		return NULL;
	}

	return pCurProcess->GetBehaviorAction();
}

bool CAnimationAPI::IsReversePlay()
{
	bool bRet = false;
	if(GetCurBehaviorAction())
	{
		bRet = GetCurBehaviorAction()->IsReversePlay();
	}
	return bRet;
}
void CAnimationAPI::SetReversePlay(bool bFlg)
{
	if(GetCurBehaviorAction())
	{
		GetCurBehaviorAction()->SetReversePlay(bFlg);
	}
}

void CAnimationAPI::ConvertPlcPathToSAPlcPath(const char *inPlcPath,char *outPlcPath,int radix, int objType)
{
	if(objType == TARGETOBJECT_TYPE_PMI)
	{
#ifdef WIN32
		strcpy_s(outPlcPath, SA_BUFFER_SIZE_SMALL,"PMI:");
#else
        strcpy(outPlcPath,"PMI:");
#endif
	}
	else
	{
#ifdef WIN32
		strcpy_s(outPlcPath, SA_BUFFER_SIZE_SMALL,"PLCID:");
#else
        strcpy(outPlcPath,"PLCID:");
#endif
	}

	char cPlcPath[SA_BUFFER_SIZE_SMALL] = {0};
	if(strlen(inPlcPath) < 16 || strstr(inPlcPath,"M3D|MAIN|PATH|0") == NULL)
	{
		if(strcmp(inPlcPath,"0") == 0)
		{
#ifdef WIN32
			strcpy_s(cPlcPath,SA_BUFFER_SIZE_SMALL,"");
#else
            strcpy(cPlcPath,"");
#endif
		}
		else if(strstr(inPlcPath,"M3D|MAIN|PATH|0") == NULL)
		{
#ifdef WIN32
			strcpy_s(cPlcPath,SA_BUFFER_SIZE_SMALL,inPlcPath);
#else
            strcpy(cPlcPath,inPlcPath);
#endif
		}
		else
		{
#ifdef WIN32
			strcpy_s(cPlcPath,SA_BUFFER_SIZE_SMALL,"");
#else
            strcpy(cPlcPath,"");
#endif
		}
	}
	else
	{
#ifdef WIN32
		strcpy_s(cPlcPath,SA_BUFFER_SIZE_SMALL,inPlcPath+16);
#else
        strcpy(cPlcPath,inPlcPath+16);
#endif
	}

	string strPlcPath;
	strPlcPath = cPlcPath;
	int idx = 0;
	while((idx = (int)strPlcPath.find_first_of("|")) >=0)
	{
		strPlcPath.replace(idx,1,"\\");
	}
	if(radix == 16)
	{
		char newPlcIdPath[SA_BUFFER_SIZE_SMALL];
		SATools::PlcPath16To10(strPlcPath.c_str(),newPlcIdPath);
#ifdef WIN32
		strcat_s(outPlcPath,SA_BUFFER_SIZE_SMALL,newPlcIdPath);
#else
        strcat(outPlcPath,newPlcIdPath);

#endif
	}
	else
	{
#ifdef WIN32
		strcat_s(outPlcPath,SA_BUFFER_SIZE_SMALL,strPlcPath.c_str());
#else
        strcat(outPlcPath,strPlcPath.c_str());
#endif
	}
	//if(strcmp(outPlcPath, "PLCID:") == 0)
	//{
	//	strcpy_s(outPlcPath, SA_BUFFER_SIZE_SMALL, "PLCID:0");
	//}
}

void CAnimationAPI::ConvertSAPlcPathToDisPlcPath(const char *oldPlcPath, char *newPlcPath, bool bPre)
{
	char *cPlcIdP = NULL;
	char srcPath[SA_BUFFER_SIZE_SMALL] = {0};
	char targetPath[SA_BUFFER_SIZE_SMALL] = {0};
	INT wiPlcId = 0;
	char *endptr;
	strcpy(newPlcPath,""); 
	if(strstr(oldPlcPath, "PLCID"))
	{
		strcpy(srcPath,oldPlcPath+6);
	}
	else
	{
		strcpy(srcPath,oldPlcPath);
	}
	cPlcIdP = strtok(srcPath,"\\");
	while(cPlcIdP !=NULL)
	{

		wiPlcId = strtoul(cPlcIdP, &endptr, 10);
		wiPlcId &= 0x00FFFFFF;
		if(strcmp(newPlcPath,"") == 0)
		{
			sprintf(targetPath,"%x",wiPlcId);
		}
		else
		{
			sprintf(targetPath,"%s|%x",newPlcPath,wiPlcId);
		}

		strcpy(newPlcPath,targetPath);
		cPlcIdP = strtok(NULL,"\\");
	}
	if(strlen(newPlcPath) > 0 && newPlcPath[0]=='0')
	{
		if(bPre)
		{
			sprintf(targetPath,"PATH|%s",newPlcPath);
		}
		else
		{
			strcpy(targetPath,"");
		}
	}
	else
	{
		if(bPre)
		{
			sprintf(targetPath,"PATH|0|%s",newPlcPath);
		}
		else
		{
			sprintf(targetPath,"%s",newPlcPath);
		}
	}

	strcpy(newPlcPath,targetPath);
}

//����¼�Ƽ��༭ʱ��ͨ���ýӿڽ����Զ�����
bool CAnimationAPI::AutoSaveSAFile()
{
	bool bRet = false;
	if(!m_bAutoSave)
	{
		return bRet;
	}
	if( SATools::VersionCompare("2.2",GetSAManager()->GetVersion()) == 0)
	{
		//���ڶ����汾2.2�Ƿֶζ�����ʾ�汾��û�м�¼���̵ĳ�ʼ״̬����ˣ�����������й��̵�״̬���ٱ�����ܱ�֤����Ч��
		return bRet;
	}
	if(wcslen(m_pSAManager->GetSAFilePath()) > 0)
	{
		if(m_pSAManager->WriteToFile((__wchar_t*)NULL) == 0)
		{
			bRet = true;
		}
	}
	return bRet;
}


int CAnimationAPI::SaveSAFile()
{
	int iRet = -1;
	if(NULL == m_pSAManager)
	{
		return iRet;
	}
	
	iRet = m_pSAManager->WriteToFile((__wchar_t*)NULL);
	return iRet;
}

int CAnimationAPI::SaveAsSAFile(const wchar_t* strPathName)
{
	int iRet = -1;
	if(NULL == m_pSAManager)
	{
		return iRet;
	}

	iRet = m_pSAManager->WriteToFile(strPathName);
	return iRet;
}

bool CAnimationAPI::SAFilePathNameIsEmpty()
{
	bool bRet = true;
	if(0 == m_pSAManager)
	{
		return bRet;
	}

	if(wcslen(m_pSAManager->GetSAFilePath()) >0)
	{
		bRet = false;
	}
	return bRet;
}


bool CAnimationAPI::IsEnableRec()
{
	bool bRet = false;
	if(GetCurBehaviorAction())
	{
		if(m_bShowRecUI && m_bRec && !GetCurBehaviorAction()->IsPlaying())
		{
			bRet = true;
		}
	}
	return bRet;
}

bool CAnimationAPI::HasBehaviorAction(bool bShowMsg)
{
	bool bRet = false;
	if (GetCurBehaviorAction())
	{
		bRet = true;
	}
	else
	{
		if (bShowMsg)
		{
			//CModuleResourceOverride Res;
			////�����԰汾
			//CString strTableText;
			//strTableText.LoadString(IDS_STRING_NO_PROCESS);			//û���ҵ�����
			//AfxMessageBox(strTableText);
		}
	}
	return bRet;
}

bool CAnimationAPI::GetPivot(const char* strPlcID,AniPoint& pntPivot)
{
	bool bRet = false;
	if(!GetCurBehaviorAction())
	{
		return bRet;
	}
	if(GetCurBehaviorAction()->FindAnimation(strPlcID,INTERPOLATOR_POS) ||
		GetCurBehaviorAction()->FindAnimation(strPlcID,INTERPOLATOR_QUATROT))
	{
		//�����ת�㲻һ�£����ܽ���¼��
		pntPivot = GetCurBehaviorAction()->FindTargetObjectByPath(strPlcID)->GetPivot();
		bRet = true;
	}
	return bRet;
}

//��ѡ¼��ʱ���Զ���������
CSAnimation* CAnimationAPI::GetActiveFolder(const string strPlcPath,CSBehaviorAction* pBehaviorAction)
{
	CSAnimation* pFolderAnimation = NULL;
	
	if(!pBehaviorAction)
	{
		pBehaviorAction = GetCurBehaviorAction();
	}
	if(!pBehaviorAction)
	{
		return NULL;
	}

	if(!m_pActiveAnimationFolder ||
		!pBehaviorAction->IsAnimation(m_pActiveAnimationFolder))
	{
		return NULL;
	}

	
	CSAnimation* pTargetObjectTypeFolder = CSACommonAPI::GetTargetObjectTypeFolder(strPlcPath.c_str(),pBehaviorAction);
	//�жϻ�����뵱ǰ¼�ƶ���������Ƿ�һ��
	if(!CSACommonAPI::IsUseTargetObjectTypeFolder() || pTargetObjectTypeFolder == CSACommonAPI::GetTopAmimationFolder(m_pActiveAnimationFolder) )
	{
		pFolderAnimation = m_pActiveAnimationFolder;
	}
	
	return pFolderAnimation;
}
//��ѡ¼��ʱ���Զ���������
CSAnimation* CAnimationAPI::AutoCreateFolder(VStringArray& aInsPlcPath,CSBehaviorAction* pBehaviorAction)
{
	CSAnimation* pFolderAnimation = NULL;
	if(m_pActiveAnimationFolder)
	{
		return NULL;
	}

	if(!m_bAutoCreateFolder)
	{
		return NULL;
	}
	if(!pBehaviorAction)
	{
		pBehaviorAction = GetCurBehaviorAction();
	}
	if(!pBehaviorAction)
	{
		return NULL;
	}

	int nSelectInsCount = aInsPlcPath.Count();
	if(nSelectInsCount <= 1)
	{
		return NULL;
	}
	char cTargetType[SA_BUFFER_SIZE_SMALL] = {0};
	SACameraType cameraType;
	string strPlcPath = "";
	for(int i = 0; i< nSelectInsCount; i++)
	{
		strPlcPath = aInsPlcPath[i];
		char cTempTargetType[SA_BUFFER_SIZE_SMALL] = {0};
		char cTempTarget[SA_BUFFER_SIZE_SMALL] = {0};
		STargetObject::ResolveTarget(strPlcPath.c_str(),cTempTarget,cTempTargetType,cameraType);
		if(i == 0)
		{
#ifdef WIN32
            strcpy_s(cTargetType,SA_BUFFER_SIZE_SMALL,cTempTargetType);
#else
            strcpy(cTargetType,cTempTargetType);
#endif
			
		}
		STargetObject* pTargetObject = pBehaviorAction->FindTargetObjectByPath(strPlcPath.c_str());
		//���ѡ�еĶ����Ѿ����ڣ����Զ���������
		if(pTargetObject || 
			(i > 0 && strcmp(cTempTargetType,cTargetType) != 0))
		{
			return NULL;
		}
	}

	CSAnimation* pTargetObjectTypeFolder = CSACommonAPI::GetTargetObjectTypeFolder(strPlcPath.c_str(),pBehaviorAction);
	pFolderAnimation = CSACommonAPI::CreateAmimationFolder(pTargetObjectTypeFolder,NULL,pBehaviorAction);
	return pFolderAnimation;
}
void CAnimationAPI::UpdateKeyEditor()
{
	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}
}

void CAnimationAPI::UpdateKeyEditorPlay()
{
	if(m_pPlayCB)
	{
		m_pPlayCB(m_pPlayCBUserData);
	}
}

bool CAnimationAPI::RecInsRotCheck(const char* strPlcID, AniPoint pntCenterPnt)
{
	bool bRet = true;
	if(!IsEnableRec())
	{
		return bRet;
	}
	if(!GetCurBehaviorAction())
	{
		return bRet;
	}
	
	if(GetCurBehaviorAction()->FindAnimation(strPlcID,INTERPOLATOR_POS) ||
		GetCurBehaviorAction()->FindAnimation(strPlcID,INTERPOLATOR_QUATROT))
	{
		NS_SimulationAnimation::STargetObject* pTargetObject = GetCurBehaviorAction()->FindTargetObjectByPath(strPlcID);
		//�����ת�㲻һ�£����ܽ���¼��
		AniPoint pivot = pTargetObject->GetPivot();
		//if(abs(pivot.x - pntCenterPnt.x) > ANIMATION_D_TOL || 
		//	abs(pivot.y - pntCenterPnt.y) > ANIMATION_D_TOL || 
		//	abs(pivot.z - pntCenterPnt.z) > ANIMATION_D_TOL)
		if(!pivot.Equal(pntCenterPnt,(float)ANIMATION_D_TOL))
		{
			bRet = false;
		}
	}
	return bRet;
}

bool CAnimationAPI::IsTLRangeImpact(CSAnimation* pAnimation,int keyNum,int& ImpactType,CTimeLineRange** ppTLRange)
{
	bool bRet = false;
	if(!GetCurBehaviorAction())
	{
		return false;
	}
	ImpactType = TLImpactType_None;
	if(!pAnimation)
	{
		return bRet;
	}
	CSTimeline* pTimeLine = pAnimation->GetTimeline();
	if(!pTimeLine)
	{
		return bRet;
	}
	if(m_bAutoKey)
	{
		int nCurrentTick = GetCurBehaviorAction()->GetCurrentTick();
		if(nCurrentTick <= pAnimation->GetLastTick())
		{
			for(int i = nCurrentTick; i <= nCurrentTick + keyNum; i++)
			{
				if(pTimeLine->GetTLRangeByCurFrame(i,ppTLRange) >=0)
				{
					bRet = true;
					break;
				}
			}

			if(bRet)
			{
				int nTLRangeBeginKeyFrame = 0;
				int nTLRangeEndKeyFrame = 0;

				int nKeyFrameIdx = (*ppTLRange)->GetBeginKeyFrameIndex();
				if(nKeyFrameIdx >=0 && nKeyFrameIdx < pTimeLine->GetTimelineArrayLength())
				{
					nTLRangeBeginKeyFrame = pTimeLine->GetTimelineArray()[nKeyFrameIdx];
				}
				nKeyFrameIdx = (*ppTLRange)->GetEndKeyFrameIndex();
				if(nKeyFrameIdx >=0 && nKeyFrameIdx < pTimeLine->GetTimelineArrayLength())
				{
					nTLRangeEndKeyFrame = pTimeLine->GetTimelineArray()[nKeyFrameIdx];
				}
				//�����ǰ֡�뵱ǰ��ʱ��γ�ͻ���Ӷ��������һ֡��ʼ¼��;���򣬲�����ͻ��ʱ�����Զ�����ƶ�
				if(nCurrentTick == nTLRangeEndKeyFrame)
				{
					ImpactType = TLImpactType_End;
				}
				else if(nCurrentTick >= nTLRangeBeginKeyFrame)
				{
					ImpactType = TLImpactType_Mid;
				}
				else
				{
					ImpactType = TLImpactType_Before;
				}
			}
		}
	}
	return bRet;
}

void CAnimationAPI::TLRangeImpactAutoProcess(CSAnimation* pAnimation,int keyNum )
{
	if(!pAnimation)
	{
		return;
	}
	if(!GetCurBehaviorAction())
	{
		return;
	}
	CSTimeline* pTimeLine = pAnimation->GetTimeline();
	if(!pTimeLine)
	{
		return;
	}

	CTimeLineRange* pTLRange = NULL;
	
	int nImpactType = 0;
	if(IsTLRangeImpact(pAnimation,keyNum,nImpactType,&pTLRange))
	{
		//�����ǰ֡�뵱ǰ��ʱ��γ�ͻ���Ӷ��������һ֡��ʼ¼��;���򣬲�����ͻ��ʱ�����Զ�����ƶ�
		if(nImpactType == TLImpactType_End || nImpactType == TLImpactType_Mid)
		{
			GetCurBehaviorAction()->SetCurrentTick(pAnimation->GetLastTick()+keyNum);
		}
		//else if(nImpactType == TLImpactType_Mid)
		//{
		//	GetCurBehaviorAction()->SetCurrentTick(pAnimation->GetLastTick()+1);
		//}
		else  if(nImpactType == TLImpactType_Before)
		{
			//�����ǰ֡��ʱ����ǰ���Զ�����ƶ���ǰ֮֡��Ĺؼ�֡
			int nTLRangeBeginKeyFrame = 0;
			int nKeyFrameIdx = (pTLRange)->GetBeginKeyFrameIndex();
			if(nKeyFrameIdx >=0 && nKeyFrameIdx < pTimeLine->GetTimelineArrayLength())
			{
				nTLRangeBeginKeyFrame = pTimeLine->GetTimelineArray()[nKeyFrameIdx];
			}
			pAnimation->AdjustKeyframe(nTLRangeBeginKeyFrame,keyNum,true,false);
		}
	}
}

bool CAnimationAPI::IsTLRangeImpact(VStringArray& aInsPlcPath,const char* interpolatorType,int keyNum ,int& ImpactType,VArray<CTimeLineRange*>& pTLRangeA )
{
	bool bRet = false;
	if(!GetCurBehaviorAction())
	{
		return bRet;
	}
	ImpactType = TLImpactType_None;
	CSAnimation* pAnimation = NULL;
	string strPlcPath;
	int nSelectInsCount = aInsPlcPath.Count();
	if(m_bAutoKey && nSelectInsCount > 1)
	{
		int nAllTLRangeBeginKeyFrame = 0;
		int nAllTLRangeEndKeyFrame = 0;

		int nCurrentTick = GetCurBehaviorAction()->GetCurrentTick();
		//�����ǰ�ؼ�֡С��ѡ�ж��󶯻������һ���ؼ�֡�����õ�ǰ�ؼ�֡Ϊ�ִ涯�������ؼ�֡
		for(int i = 0; i< nSelectInsCount; i++)
		{
			CTimeLineRange* pTLRange = NULL;
			strPlcPath = aInsPlcPath[i];
			if(pAnimation = GetCurBehaviorAction()->FindAnimation(strPlcPath.c_str(),interpolatorType))
			{
				int nSingleImpactType = 0;
				if(IsTLRangeImpact(pAnimation,keyNum,nSingleImpactType,&pTLRange))
				{

					//��ȡ����ʱ���߶εĿ�ʼ֡�ͽ���֡������ȷ����ͻ����
					{
						int nTLRangeBeginKeyFrame = 0;
						int nTLRangeEndKeyFrame = 0;

						CSTimeline* pTimeLine = pAnimation->GetTimeline();
						int nKeyFrameIdx = pTLRange->GetBeginKeyFrameIndex();
						if(nKeyFrameIdx >=0 && nKeyFrameIdx < pTimeLine->GetTimelineArrayLength())
						{
							nTLRangeBeginKeyFrame = pTimeLine->GetTimelineArray()[nKeyFrameIdx];
						}
						nKeyFrameIdx = pTLRange->GetEndKeyFrameIndex();
						if(nKeyFrameIdx >=0 && nKeyFrameIdx < pTimeLine->GetTimelineArrayLength())
						{
							nTLRangeEndKeyFrame = pTimeLine->GetTimelineArray()[nKeyFrameIdx];
						}

						if(bRet == false)
						{
							nAllTLRangeBeginKeyFrame = nTLRangeBeginKeyFrame;
							nAllTLRangeEndKeyFrame = nTLRangeEndKeyFrame;
						}
						else
						{
							if(nAllTLRangeBeginKeyFrame > nTLRangeBeginKeyFrame)
							{
								nAllTLRangeBeginKeyFrame = nTLRangeBeginKeyFrame;
							}
							if(nAllTLRangeEndKeyFrame < nTLRangeEndKeyFrame)
							{
								nAllTLRangeEndKeyFrame = nTLRangeEndKeyFrame;
							}
						}
					}

					bRet = true;
				}
			}
			pTLRangeA.Append(pTLRange);
		}
		//�����ǰ֡�뵱ǰ��ʱ��γ�ͻ���Ӷ��������һ֡��ʼ¼��;���򣬲�����ͻ��ʱ�����Զ�����ƶ�
		if(nCurrentTick == nAllTLRangeEndKeyFrame)
		{
			ImpactType = TLImpactType_End;
		}
		else if(nCurrentTick >= nAllTLRangeBeginKeyFrame)
		{
			ImpactType = TLImpactType_Mid;
		}
		else
		{
			ImpactType = TLImpactType_Before;
		}
	}
	return bRet;
}


void CAnimationAPI::TLRangeImpactAutoProcess(VStringArray& aInsPlcPath,const char* interpolatorType,int keyNum )
{
	if(!GetCurBehaviorAction())
	{
		return;
	}
	VArray<CTimeLineRange*> pTLRangeA;
	int nImpactType = 0;
	if(IsTLRangeImpact(aInsPlcPath,interpolatorType,keyNum,nImpactType,pTLRangeA))
	{
		//�����ǰ֡�뵱ǰ��ʱ��γ�ͻ���Ӷ��������һ֡��ʼ¼��;���򣬲�����ͻ��ʱ�����Զ�����ƶ�
		if(nImpactType == TLImpactType_End || nImpactType == TLImpactType_Mid)
		{
			int nCurrentTick = GetAnimationsLastTick(aInsPlcPath,interpolatorType) + keyNum;
			GetCurBehaviorAction()->SetCurrentTick(nCurrentTick);
		}
		//else if(nImpactType == TLImpactType_Mid)
		//{
		//	GetCurBehaviorAction()->SetCurrentTick(pAnimation->GetLastTick()+1);
		//}
		else  if(nImpactType == TLImpactType_Before)
		{
			//�����ǰ֡��ʱ����ǰ���Զ�����ƶ���ǰ֮֡��Ĺؼ�֡
			for(int i = 0; i< pTLRangeA.Count(); i++)
			{
				if(pTLRangeA[i])
				{
					int nTLRangeBeginKeyFrame = 0;
					int nKeyFrameIdx = pTLRangeA[i]->GetBeginKeyFrameIndex();
					CSTimeline* pTimeLine = pTLRangeA[i]->GetTimeline();
					CSAnimation* pAnimation = pTimeLine->GetAnimation();
					if(nKeyFrameIdx >=0 && nKeyFrameIdx < pTimeLine->GetTimelineArrayLength())
					{
						nTLRangeBeginKeyFrame = pTimeLine->GetTimelineArray()[nKeyFrameIdx];
					}
					pAnimation->AdjustKeyframe(nTLRangeBeginKeyFrame,keyNum,true,false);
				}
			}
			
		}
	}
}

/****************************************************************************
*	@brief		 ¼��ƽ����ת����
*	@param[in]	plcID					ģ������·��
*	@param[in]	objName				ģ������
*	@param[in]	centerPnt				��ת���ĵ�
*	@param[in]	Pos						ģ��λ����Ϣ
*	@param[in]	AniQuat						ģ����ת��Ϣ
*	@param[in]	bAddNewKey		�Ƿ����һ���µ�֡
*	@return		NULL
*	@note		��
*	@attention	��
*	@par	��ʷ:
*		2014/09/26	created by yhp
****************************************************************************/
bool CAnimationAPI::RecInsPosRot(const char* strPlcID, const char* strObjName, AniPoint pntCenterPnt, AniPoint pntPos, AniQuat qual, bool bAddNewKey,bool bLiner, bool bFollowPath, bool bUpdateKeyEditor,CSBehaviorAction* pBehaviorAction,CSAnimation* pFolderAnimation )
{
	if (!IsEnableRec())
	{
		return false;
	}
	if(!pBehaviorAction)
	{
		pBehaviorAction = GetCurBehaviorAction();
	}
	if(!pBehaviorAction)
	{
		return false;
	}

	if(!strPlcID || (!strstr(strPlcID,TARGETOBJECTTYPE_INS)&&!strstr(strPlcID,TARGETOBJECTTYPE_GROUP)&&!strstr(strPlcID,TARGETOBJECTTYPE_TOOL)))
	{
		return false;
	}

	if(!pFolderAnimation)
	{
		pFolderAnimation = GetActiveFolder(strPlcID,pBehaviorAction);
	}

	////����Ѿ������������Ķ�����ȡ���ж�������ĵ�
	//GetPivot(strPlcID,pntCenterPnt);

	//����ǰλ��ת��Ϊ����ת���ĵ���ת����λ����������λ��
	if(!strstr(strPlcID,TARGETOBJECTTYPE_GROUP))
	{
		pntPos = CSACommonAPI::MtxTanslationToUniTanslation(pntCenterPnt,qual,pntPos);
	}

	float q0, q1, q2, q3;

	q0 = qual.x;
	q1 = qual.y;
	q2 = qual.z;
	q3 = qual.w;

	CSAnimation *animation ;
	if(!(animation = pBehaviorAction->FindAnimation(strPlcID,INTERPOLATOR_POS)))
	{
		CSAnimation *pParentAnimation = CSACommonAPI::GetTargetMainAmimation(strPlcID,strObjName,pBehaviorAction,pFolderAnimation);
		if(pParentAnimation)
		{
			if(!((animation = pBehaviorAction->FindAnimation(strPlcID,INTERPOLATOR_QUATROT))))
			{
				animation = CSACommonAPI::AddAnimation(pBehaviorAction,strObjName,strPlcID,&pntCenterPnt,pParentAnimation);
			}
			std::string strTmp = IDS_STRING_POSITON;
			animation->SetName(strTmp.c_str());
		}
	}
	else
	{
		if(m_bAutoKey)//�ж�����û��������Զ����ӹؼ��갴ť
		{
			INT keyFrameNum = animation->GetTimeline()->GetTimelineArrayLength();
			//�ж�������һ���ؼ�֡��ֵ�뵱ǰҪ¼�Ƶ�����һ�£����ܽ���¼��
			bool bIsEqualLastKeyFrame = true;
			vlist_s* pInterpolatorList = animation->GetInterpolatorList();
			START_LIST_ITERATION(CSInterpolator, pInterpolatorList)
				if(temp->GetAt(keyFrameNum-1)->m_type == ANIChannel)
				{
					CKeyframeChannel * pKeyFrame = (CKeyframeChannel *)temp->GetAt(keyFrameNum-1);
					AniPoint lastPoint =pKeyFrame->m_cp;
					if(strcmp(temp->GetType(),INTERPOLATOR_POS) == 0)
					{
						//if(abs(lastPoint.x - pntPos.x) > ANIMATION_D_TOL || 
						//	abs(lastPoint.y - pntPos.y) > ANIMATION_D_TOL || 
						//	abs(lastPoint.z - pntPos.z) > ANIMATION_D_TOL)
						if(!lastPoint.Equal(pntPos,(float)ANIMATION_D_TOL) )
						{
							bIsEqualLastKeyFrame = false;
							break;
						}
					}
					else if(strcmp(temp->GetType(),INTERPOLATOR_PIVOT)==0)
					{
						//if(abs(lastPoint.x - pntCenterPnt.x) > ANIMATION_D_TOL || 
						//	abs(lastPoint.y - pntCenterPnt.y) > ANIMATION_D_TOL || 
						//	abs(lastPoint.z - pntCenterPnt.z) > ANIMATION_D_TOL)
						if(!lastPoint.Equal(pntCenterPnt,(float)ANIMATION_D_TOL))
						{
							bIsEqualLastKeyFrame = false;
							break;
						}
					}
				}
				else if(temp->GetAt(keyFrameNum-1)->m_type == ANIRotation)
				{
					AniQuat lastQuat =((CKeyframeQuatSquad *)temp->GetAt(keyFrameNum-1))->m_quat;
					//if(fabs(fabs(lastQuat.x) - fabs(qual.x)) > ANIMATION_D_TOL2 || 
					//	fabs(fabs(lastQuat.y) - fabs(qual.y)) > ANIMATION_D_TOL2 || 
					//	fabs(fabs(lastQuat.z) - fabs(qual.z)) > ANIMATION_D_TOL2 ||
					//	fabs(fabs(lastQuat.w) - fabs(qual.w)) > ANIMATION_D_TOL2 )
					if(!lastQuat.Equal(qual, (float)ANIMATION_D_TOL2))
					{
						bIsEqualLastKeyFrame = false;
						break;
					}
				}
			END_LIST_ITERATION(pInterpolatorList)
			if(bIsEqualLastKeyFrame)
			{
				return false;
			}

			//�Զ����һ���ؼ�֡
			if(bAddNewKey || (animation->GetTimeline() && animation->GetTimeline()->GetTimelineArrayLength()<=1))
			{
				//pBehaviorAction->SetCurrentTick(animation->GetLastTick() + m_numAutoKeyFrame);
				if((int)pBehaviorAction->GetCurrentTick() != animation->GetLastTick())
				{
					//�����ǰ֡���ڵ�ǰ���������һ֡����ģ�͵ĵ�ǰ��ת��Ϣ����ǰһ֡����ת��Ϣʱ������ǰһ֡λ��
					if((int)pBehaviorAction->GetCurrentTick() > animation->GetLastTick())
					{
						if( !bFollowPath)
						{
							animation->DuplicateNextOrPrevious(pBehaviorAction->GetCurrentTick(), false);
							pBehaviorAction->SetCurrentTick(animation->GetLastTick() + m_numAutoKeyFrame);
						}
					}
					else
					{
						pBehaviorAction->SetCurrentTick(animation->GetLastTick() + m_numAutoKeyFrame);
					}
				}
				else
				{
					pBehaviorAction->SetCurrentTick(animation->GetLastTick() + m_numAutoKeyFrame);
				}
			}
		}
		if(!pBehaviorAction->FindAnimation(strPlcID,INTERPOLATOR_QUATROT))
		{
			pBehaviorAction->FindTargetObjectByPath(strPlcID)->SetPivot(pntCenterPnt.x, pntCenterPnt.y, pntCenterPnt.z);//����handle����ƶ���������ԭ�������
		}
	}

	if(animation == NULL)
	{
		return false;
	}
	CSACommonAPI::AddPosRotKeyframe(pBehaviorAction,animation,pBehaviorAction->GetCurrentTick(),pntCenterPnt,pntPos,bLiner,qual,bLiner,bFollowPath);
	if(animation->GetTimeline())
	{
		animation->GetTimeline()->AddTLRange();
	}
	
	if(m_pRecCB && bUpdateKeyEditor)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	return true;
}

int CAnimationAPI::GetAnimationsLastTick(VStringArray& aInsPlcPath,const char* interpolatorType,CSBehaviorAction* pBehaviorAction )
{
	int nLastTick = 0;
	if(!pBehaviorAction)
	{
		pBehaviorAction = GetCurBehaviorAction();
	}
	if(!pBehaviorAction)
	{
		return nLastTick;
	}
	CSAnimation* pAnimation = NULL;
	string strPlcPath;
	int nSelectInsCount = aInsPlcPath.Count();
	if(m_bAutoKey && nSelectInsCount > 1)
	{
		//�����ǰ�ؼ�֡С��ѡ�ж��󶯻������һ���ؼ�֡�����õ�ǰ�ؼ�֡Ϊ�ִ涯�������ؼ�֡
		for(int i = 0; i< nSelectInsCount; i++)
		{
			strPlcPath = aInsPlcPath[i];
			if(pAnimation = pBehaviorAction->FindAnimation(strPlcPath.c_str(),interpolatorType))
			{
				if(nLastTick < pAnimation->GetLastTick())
				{
					nLastTick = pAnimation->GetLastTick();
				}
			}

		}
	}
	return nLastTick;
}


bool CAnimationAPI::RecInsPosRot(VStringArray& aInsPlcPath,
						   VStringArray& aInsName,
						   VHPointArray& aInsEndTranslate,
						   VHQuatArray& aInsEndRotaion,
						   VHPointArray& aInsCenter,
						   bool bAddNewKey,
						   bool bUpdateKeyEditor,
						   CSBehaviorAction* pBehaviorAction)
{
	bool bRecReult = true;
	string strPlcPath = "";
	string strObjName = "";
	AniPoint translation;
	AniQuat rotation;

	if(!IsEnableRec())
	{
		return false;
	}
	if(!pBehaviorAction)
	{
		pBehaviorAction = GetCurBehaviorAction();
	}
	if(!pBehaviorAction)
	{
		return false;
	}

	int nSelectInsCount = aInsPlcPath.Count();
	int nCurrentTick = GetAnimationsLastTick(aInsPlcPath,INTERPOLATOR_POS,pBehaviorAction);
	if(nCurrentTick < (int)pBehaviorAction->GetCurrentTick())
	{
		nCurrentTick = (int)pBehaviorAction->GetCurrentTick();
	}
	CSAnimation* pFolderAnimation = AutoCreateFolder(aInsPlcPath,pBehaviorAction);
	for(int i = 0; i< nSelectInsCount; i++)
	{
		strPlcPath = aInsPlcPath[i];
		strObjName = aInsName[i];

		pBehaviorAction->SetCurrentTick(nCurrentTick);

		bRecReult = RecInsPosRot((char*)strPlcPath.c_str(),(char*)strObjName.c_str(),aInsCenter[i],aInsEndTranslate[i],aInsEndRotaion[i],bAddNewKey,m_blinear,false,false,pBehaviorAction,pFolderAnimation);

		
		//if(!animation)
		//{
		//	continue;
		//}
		////����¼�ƽ����󣬶�����֡������1���ܲ��ţ����ֻ��һ���ؼ�֡���ö�����Ч������ɾ��
		//if(m_bAutoKey && animation->GetTimeline()->GetTimelineArrayLength() == 1)
		//{
		//	pBehaviorAction->DeleteAnimation(animation);
		//}

	}

	if(m_pRecCB && bUpdateKeyEditor)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	return true;
}

bool CAnimationAPI::RecInsPosRot(VStringArray& aInsPlcPath,
								 VStringArray& aInsName,
								 VHPointArray& aInsStartTranslate,
								 VHPointArray& aInsEndTranslate,
								 VHQuatArray& aInsStartRotaion,
								 VHQuatArray& aInsEndRotaion,
								 VHPointArray& aInsCenter,
								 bool bAddNewKey,
								 bool bUpdateKeyEditor,
								 CSBehaviorAction* pBehaviorAction)
{
	bool bRecReult = true;
	string strPlcPath = "";
	string strObjName = "";
	AniPoint translation;
	AniQuat rotation;
	CSAnimation *animation = NULL;

	if(!IsEnableRec())
	{
		return false;
	}
	if(!pBehaviorAction)
	{
		pBehaviorAction = GetCurBehaviorAction();
	}
	if(!pBehaviorAction)
	{
		return false;
	}

	int nSelectInsCount = aInsPlcPath.Count();

	int nCurrentTick = GetAnimationsLastTick(aInsPlcPath,INTERPOLATOR_POS,pBehaviorAction);
	if(nCurrentTick < (int)pBehaviorAction->GetCurrentTick())
	{
		nCurrentTick = (int)pBehaviorAction->GetCurrentTick();
	}
	CSAnimation* pFolderAnimation = AutoCreateFolder(aInsPlcPath,pBehaviorAction);
	for(int i = 0; i< nSelectInsCount; i++)
	{
		strPlcPath = aInsPlcPath[i];
		strObjName = aInsName[i];

		pBehaviorAction->SetCurrentTick(nCurrentTick);

		animation = pBehaviorAction->FindAnimation(strPlcPath.c_str(),INTERPOLATOR_POS);
		//���û�е������Ķ�������¼���϶�ǰλ�õĶ�����Ϊ��һ֡
		if(!animation)
		{
			bRecReult = RecInsPosRot((char*)strPlcPath.c_str(),(char*)strObjName.c_str(),aInsCenter[i],aInsStartTranslate[i],aInsStartRotaion[i],bAddNewKey,m_blinear,false,false,pBehaviorAction,pFolderAnimation);
			animation = pBehaviorAction->FindAnimation(strPlcPath.c_str(),INTERPOLATOR_POS);
		}
		bRecReult = RecInsPosRot((char*)strPlcPath.c_str(),(char*)strObjName.c_str(),aInsCenter[i],aInsEndTranslate[i],aInsEndRotaion[i],bAddNewKey,m_blinear,false,false,pBehaviorAction,pFolderAnimation);


		//if(!animation)
		//{
		//	continue;
		//}
		////����¼�ƽ����󣬶�����֡������1���ܲ��ţ����ֻ��һ���ؼ�֡���ö�����Ч������ɾ��
		//if(m_bAutoKey && animation->GetTimeline()->GetTimelineArrayLength() == 1)
		//{
		//	pBehaviorAction->DeleteAnimation(animation);
		//}
	}

	if(m_pRecCB && bUpdateKeyEditor)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	return true;
}

//�����ع켣��¼��ʱ��ÿ֡���Ա�ʾ�ľ���
float CAnimationAPI::GetFollowPathLenPerFrame(VHPointArray& aPathPoint)
{
	float fLenPerFrame = 1.0f;
	float fTotlePathLength = 0.0f;

	VIntArray aLength;
	for(int i = 1; i< aPathPoint.Count(); i++)
	{
		
		AniPoint p1= aPathPoint.GetData(i-1);
		AniPoint p2= aPathPoint.GetData(i);
		AniPoint p3= p1 - p2;
		float length = SAUtility::Compute_Vector_Length(p3);
		fTotlePathLength += length;

		//��¼���е��ľ���
		aLength.Append((int)length);
	}

	//�������п��Ƶ��������Լ��
	int nMaxCommonDivisor = 1;
	if(aLength.Count()>1)
	{
		int a= 1;
		int b= 1;
		for(int i = 0 ; i< aLength.Count()-1; i++)
		{
			if(i==0)
			{
				a= aLength[i];
				nMaxCommonDivisor= a;
			}
			else
			{
				a= nMaxCommonDivisor;
			}
			b= aLength[i+1];
			//�������Ϊ3����ֹ��ӵĹؼ�֡���࣬�ֿ�ʵ�ֽ����ٵĲ���Ч��
			while(b>1/*b!=0*/)
			{
				nMaxCommonDivisor= b;
				int c=a%b;
				a=b;
				b=c;
			}
		}
	}
	else if(aLength.Count()==1)
	{
		nMaxCommonDivisor= aLength[0];
	}
	
	//����1֡���ɱ�ʾ�ĳ���
	fLenPerFrame = nMaxCommonDivisor;
	float fFrameNum = fTotlePathLength/fLenPerFrame;
	if(fFrameNum < m_numAutoKeyFrame)
	{
		fLenPerFrame = fLenPerFrame / (m_numAutoKeyFrame/fFrameNum);
	}
	return fLenPerFrame;
}
//�����ع켣��¼��ʱ���������Ƶ�֮��Ĺؼ�֡��࣬��ʵ�����ٲ���
int CAnimationAPI::GetFollowPathAutoKeyNum(VHPointArray& aPathPoint,int pointIdx,float fLenPerFrame)
{
	int numAutoKeyFrame = m_numAutoKeyFrame;
	if(aPathPoint.Count()>2)
	{
		if(pointIdx >0 && pointIdx < aPathPoint.Count())
		{
			AniPoint p1= aPathPoint.GetData(pointIdx-1);
			AniPoint p2= aPathPoint.GetData(pointIdx);
			AniPoint p3= p1 - p2;
			float length = SAUtility::Compute_Vector_Length(p3);
			numAutoKeyFrame = (int)(length/fLenPerFrame);
			if(numAutoKeyFrame <= 0)
			{
				numAutoKeyFrame = 1;
			}
		}
	}
	return numAutoKeyFrame;
}

//���ݹ켣���ϵĿ��Ƶ㼰ģ�͵ĳ�ʼλ�ã�����ģ����ָ�����Ƶ��Ӧ��λ��
AniPoint CAnimationAPI::GetFollowPathInsPos(VHPointArray& aPathPoint,int pointIdx,AniPoint insStartPnt)
{
	AniPoint pntRet = insStartPnt;
	if(pointIdx >0)
	{
		if(pointIdx >= aPathPoint.Count())
		{
			pointIdx = aPathPoint.Count()-1;
		}
		AniPoint pathStartPnt= aPathPoint.GetData(0);
		AniPoint pntOffset= insStartPnt - pathStartPnt;
		AniPoint pathCurPnt= aPathPoint.GetData(pointIdx);
		pntRet = pathCurPnt + pntOffset;
	}
	return pntRet;
}
//�ع켣���ƶ�¼�ƽӿ�
bool CAnimationAPI::RecInsPosRotFollowPath(VStringArray& aInsPlcPath,
								 VStringArray& aInsName,
								 VHPointArray& aInsStartTranslate,
								 VHQuatArray& aInsStartRotaion,
								 VHPointArray& aInsCenter,
								 VHPointArray& aPathPoint,
								 VIntArray& aPntSep,
								 VHQuatArray& aInsParentRotaion,
								 CSBehaviorAction* pBehaviorAction)
{
	bool bRecReult = true;
	string strPlcPath = "";
	string strObjName = "";
	AniPoint translation;
	AniQuat rotation;

	if(!pBehaviorAction)
	{
		pBehaviorAction = GetCurBehaviorAction();
	}
	if(!pBehaviorAction)
	{
		return false;
	}
	if(pBehaviorAction->IsPlaying())
	{
		return false;
	}

	int nSelectInsCount = aInsPlcPath.Count();
	int nCurrentTick = GetAnimationsLastTick(aInsPlcPath,INTERPOLATOR_POS,pBehaviorAction);
	if(nCurrentTick < (int)pBehaviorAction->GetCurrentTick())
	{
		nCurrentTick = (int)pBehaviorAction->GetCurrentTick();
	}
	else if(nCurrentTick > 0 && nSelectInsCount > 1)
	{
		nCurrentTick = nCurrentTick+m_numAutoKeyFrame;
		//�����ǰ�ؼ�֡С��ѡ�ж��󶯻������һ���ؼ�֡�����õ�ǰ�ؼ�֡Ϊ�ִ涯�������ؼ�֡
		for(int i = 0; i< nSelectInsCount; i++)
		{
			CSAnimation* pAnimation = NULL;
			strPlcPath = aInsPlcPath[i];
			if(pAnimation = pBehaviorAction->FindAnimation(strPlcPath.c_str(),INTERPOLATOR_POS))
			{
				pAnimation->DuplicateNextOrPrevious(nCurrentTick, false);
			}

		}
		nCurrentTick = nCurrentTick+1;
	}
	CSAnimation* pFolderAnimation = AutoCreateFolder(aInsPlcPath,pBehaviorAction);


	bool bAutoKey = m_bAutoKey;
	m_bAutoKey = true;
	int nDefNumAutoKeyFrame = m_numAutoKeyFrame;

	float fLenPerFrame = GetFollowPathLenPerFrame(aPathPoint);
	
	for(int i = 0; i< nSelectInsCount; i++)
	{
		pBehaviorAction->SetCurrentTick(nCurrentTick);

		AniQuat parentQuat = aInsParentRotaion[i];
		float mutrix[4][4];	
		parentQuat.ToMatrix((float*)mutrix);		
		MatrixOperation::MatrixInversion((float*)mutrix);
		VHPointArray aTempPathPoint;
		for(int j = 0; j< aPathPoint.Count(); j++) 
		{
			float fPnt[3]; 
			fPnt[0] = aPathPoint[j].x;
			fPnt[1] = aPathPoint[j].y;
			fPnt[2] = aPathPoint[j].z;
			MatrixOperation::TransformPoint(mutrix,fPnt);
			aTempPathPoint.Append(AniPoint(fPnt[0],fPnt[1],fPnt[2]));
		}
		
		strPlcPath = aInsPlcPath[i];
		strObjName = aInsName[i];
		
		int idxPntSep = 0;
		for(int j = 0; j < aPathPoint.Count(); j++)
		{
			bool bLiner = false;
			if(j == aPntSep[idxPntSep])
			{
				bLiner = true;
				idxPntSep++;
			}
			if(j > 0 && aPathPoint[j].Equal(aPathPoint[j-1]))
			{
				CSAnimation* pAnimation = pBehaviorAction->FindAnimation((char*)strPlcPath.c_str(),INTERPOLATOR_POS);
				if(pAnimation)
				{
					pBehaviorAction->SetCurrentTick(pBehaviorAction->GetCurrentTick()+1);
					pAnimation->DuplicateNextOrPrevious(pBehaviorAction->GetCurrentTick(), false);
				}
			}

			m_numAutoKeyFrame = GetFollowPathAutoKeyNum(aPathPoint,j,fLenPerFrame);
			AniPoint pntInsTranslate = GetFollowPathInsPos(aTempPathPoint,j,aInsStartTranslate[i]);
			bRecReult = RecInsPosRot((char*)strPlcPath.c_str(),(char*)strObjName.c_str(),aInsCenter[i],pntInsTranslate,aInsStartRotaion[i],true,bLiner,true,false,pBehaviorAction,pFolderAnimation);
		}
		aTempPathPoint.TrimSize(0);
		
	}
	m_numAutoKeyFrame = nDefNumAutoKeyFrame;
	m_bAutoKey = bAutoKey;


	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	return true;
}

bool CAnimationAPI::RecCamera(float pivot[3], float fMtxAA[4][4], float fScale, float fIntWind[2], bool bUseAutoKey,CSBehaviorAction* pBehaviorAction/* = NULL*/)
{
	//¼���ӿڳ���
	CHAR strPlcID[SA_BUFFER_SIZE_SMALL] = {0};
	CHAR strObjName[SA_BUFFER_SIZE_SMALL] = {0};
	CSAnimation *animation = NULL;
	AniPoint pntCenterPnt(pivot[0],pivot[1],pivot[2]);

	if (!pBehaviorAction)
	{
		pBehaviorAction = GetCurBehaviorAction();
	}
	if(!pBehaviorAction)
	{
		return false;
	}

	if(pBehaviorAction->IsPlaying())
	{
		return false;
	}
	strcpy(strPlcID, TARGETOBJECTTYPE_CAM);
	strcat(strPlcID, ":SCENE/TARGET");	
	std::string strTmp = IDS_STRING_CAMERA;
	strcpy(strObjName, strTmp.c_str());

	if(!(animation = pBehaviorAction->FindAnimation(strPlcID,INTERPOLATOR_POS)))
	{
		animation = CSACommonAPI::AddAnimation(pBehaviorAction,strObjName,strPlcID,&pntCenterPnt,NULL);
	}

	if(animation == NULL)
	{
		return false;
	}

	float fMtxAAInv[4][4];
	MatrixOperation::MatrixCopy((float*)fMtxAA, (float*)fMtxAAInv, 16);
	MatrixOperation::MatrixInversion((float*)fMtxAAInv);
	AniPoint pntPos(fMtxAAInv[3][0], fMtxAAInv[3][1], fMtxAAInv[3][2]);
	AniPoint pntScale(fScale, fIntWind[0], fIntWind[1]);
	AniQuat hQuat = AniQuat::MatrixToQuaternion((float*)fMtxAAInv);

	if(bUseAutoKey && 
		animation->GetTimeline() &&
		animation->GetTimeline()->GetTimelineArrayLength()>0)
	{
		INT keyFrameNum = animation->GetTimeline()->GetTimelineArrayLength();
		//�ж�������һ���ؼ�֡��ֵ�뵱ǰҪ¼�Ƶ�����һ�£����ܽ���¼��
		bool bIsEqualLastKeyFrame = true;
		vlist_s* pInterpolatorList = animation->GetInterpolatorList();
		START_LIST_ITERATION(CSInterpolator, pInterpolatorList)
			if (temp->GetAt(keyFrameNum - 1)->m_type == ANIChannel)
			{
				CKeyframeChannel * pKeyFrame = (CKeyframeChannel *)temp->GetAt(keyFrameNum - 1);
				AniPoint lastPoint = pKeyFrame->m_cp;
				if (strcmp(temp->GetType(), INTERPOLATOR_POS) == 0)
				{
					if (!lastPoint.Equal(pntPos, (float)ANIMATION_D_TOL))
					{
						bIsEqualLastKeyFrame = false;
						break;
					}
				}
				else if (strcmp(temp->GetType(), INTERPOLATOR_PIVOT) == 0)
				{
					if (!lastPoint.Equal(pntCenterPnt, (float)ANIMATION_D_TOL))
					{
						bIsEqualLastKeyFrame = false;
						break;
					}
				}
				else if (strcmp(temp->GetType(), INTERPOLATOR_SCALE) == 0)
				{
					if (!lastPoint.Equal(pntScale, (float)ANIMATION_D_TOL))
					{
						bIsEqualLastKeyFrame = false;
						break;
					}
				}
			}
			else if (temp->GetAt(keyFrameNum - 1)->m_type == ANIRotation)
			{
				AniQuat lastQuat = ((CKeyframeQuatSquad *)temp->GetAt(keyFrameNum - 1))->m_quat;
				if (!lastQuat.Equal(hQuat, (float)ANIMATION_D_TOL2))
				{
					bIsEqualLastKeyFrame = false;
					break;
				}
			}
		END_LIST_ITERATION(pInterpolatorList)
			if (bIsEqualLastKeyFrame)
			{
				return false;
			}

		if((int)pBehaviorAction->GetCurrentTick() <= animation->GetLastTick())
		{
			pBehaviorAction->SetCurrentTick(animation->GetLastTick() + m_numAutoKeyFrame);
		}
		else
		{
			animation->DuplicateNextOrPrevious(pBehaviorAction->GetCurrentTick(),false);
			pBehaviorAction->SetCurrentTick(animation->GetLastTick() + m_numAutoKeyFrame);
		}
	}

	CSACommonAPI::AddCameraKeyframe(pBehaviorAction,animation, pBehaviorAction->GetCurrentTick(),pntCenterPnt,pntPos,hQuat,pntScale,m_blinear);

	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	return true;
}

bool CAnimationAPI::RecZoom(const char* strZoomId,const char* strObjName, float fMtxAA[4][4], float fScale, float fIntWind[2],bool bVisible,bool bRelaceVisible)
{
	//¼���ӿڳ���
	CHAR strPlcID[SA_BUFFER_SIZE_SMALL] = {0};
	CSAnimation *animation = NULL;
	AniPoint pntCenterPnt(0.0,0.0,0.0);
	if(!GetCurBehaviorAction())
	{
		return false;
	}

	if(!IsEnableRec())
	{
		return false;
	}
	if(!strZoomId || (int)strlen(strZoomId) > (SA_BUFFER_SIZE_SMALL - 20))
	{
		return false;
	}
	strcpy(strPlcID, TARGETOBJECTTYPE_ZOOM);
	strcat(strPlcID, ":ZOOM");
	strcat(strPlcID, strZoomId);
	strcat(strPlcID, "/TARGET");

	if(!(animation = GetCurBehaviorAction()->FindAnimation(strPlcID,INTERPOLATOR_POS)))
	{
		animation = CSACommonAPI::AddAnimation(GetCurBehaviorAction(),strObjName,strPlcID,&pntCenterPnt,NULL);
	}

	if(animation == NULL)
	{
		return false;
	}

	//if(m_bAutoKey && 
	//	animation->GetTimeline() &&
	//	animation->GetTimeline()->GetTimelineArrayLength()>0)
	//{
	//	if((int)GetCurBehaviorAction()->GetCurrentTick() <= animation->GetLastTick())
	//	{
	//		GetCurBehaviorAction()->SetCurrentTick(animation->GetLastTick() + m_numAutoKeyFrame);
	//	}
	//}
	bool bAutoKey = m_bAutoKey;
	m_bAutoKey = false;
	TLRangeImpactAutoProcess(animation,m_numAutoKeyFrame);
	
	float fMtxAAInv[4][4];
	MatrixOperation::MatrixCopy((float*)fMtxAA,(float*)fMtxAAInv,16);
	MatrixOperation::MatrixInversion((float*)fMtxAAInv);
	AniPoint pntPos (fMtxAAInv[3][0],fMtxAAInv[3][1],fMtxAAInv[3][2]);
	AniPoint pntScale (fScale,fIntWind[0],fIntWind[1]);
	AniQuat hQuat = AniQuat::MatrixToQuaternion((float*)fMtxAAInv);
	char strVisible[SA_BUFFER_SIZE_SMALL] = {0};
	if(bVisible)
	{
		strcpy(strVisible,"on");
	}
	else
	{
		strcpy(strVisible,"off");
	}

	////��һ����ʾ�ؼ�֡Ҫ���һ�����عؼ�֡
	//if(bFirstVisible)
	//{
	//	if(animation->GetTimeline() &&
	//		animation->GetTimeline()->GetTimelineArrayLength()>0 && 
	//		GetCurBehaviorAction()->GetCurrentTick() > animation->GetLastTick())
	//	{
	//		animation->DuplicateNextOrPrevious(GetCurBehaviorAction()->GetCurrentTick(),false);
	//	}
	//	else
	//	{
	//		CSACommonAPI::AddZoomKeyframe(GetCurBehaviorAction(),animation,GetCurBehaviorAction()->GetCurrentTick(),pntPos,hQuat,pntScale,m_blinear,"off");
	//	}
	//	GetCurBehaviorAction()->SetCurrentTick(GetCurBehaviorAction()->GetCurrentTick() + 1);
	//}
	CSACommonAPI::AddZoomKeyframe(GetCurBehaviorAction(),animation,GetCurBehaviorAction()->GetCurrentTick(),pntPos,hQuat,pntScale,m_blinear,strVisible,bRelaceVisible);

	m_bAutoKey = bAutoKey;
	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	return true;
}

//��������¼�ƽӿ�
bool CAnimationAPI::RecVisible(const char* strPlcID, const char* strObjName, bool bVisible,CSBehaviorAction* pBehaviorAction,CSAnimation* pFolderAnimation)
{
	if (!pBehaviorAction)
		pBehaviorAction = GetCurBehaviorAction();
	if (!pBehaviorAction)
		return false;
	if(!m_bShowRecUI || pBehaviorAction->IsPlaying())
	{
		return false;
	}

	CSAnimation* pAnimation = NULL;

	if(!pFolderAnimation)
	{
		pFolderAnimation = GetActiveFolder(strPlcID,pBehaviorAction);
	}
	
	if(!(pAnimation = pBehaviorAction->FindAnimation(strPlcID,INTERPOLATOR_VISIBLE)))
	{
		//�����ɼ��Զ���
		CSAnimation *pParentAnimation = CSACommonAPI::GetTargetMainAmimation(strPlcID,strObjName,pBehaviorAction,pFolderAnimation);
		if(pParentAnimation)
		{
			pAnimation = CSACommonAPI::AddAnimation(pBehaviorAction,strObjName,strPlcID,NULL,pParentAnimation);
			std::string strTmp = IDS_STRING_SHOW_HIDE;
			pAnimation->SetName(strTmp.c_str());
		}
	}

	if(pAnimation == NULL)
		return false;

	int nTick = pBehaviorAction->GetCurrentTick();
	if(bVisible)
		CSACommonAPI::AddVisibleKeyframe(pBehaviorAction, pAnimation, nTick, "on");
	else
		CSACommonAPI::AddVisibleKeyframe(pBehaviorAction,pAnimation,nTick,"off");

	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	return true;
}

bool CAnimationAPI::RecVisibleFade(const char* strPlcID, const char* strObjName, bool bOnOff,CSAnimation* pFolderAnimation, bool bUpdateKeyEditor, CSBehaviorAction* pBehaviorAction)
 {
	CSAnimation *animation = NULL;

	if(!pBehaviorAction)
	{
		pBehaviorAction = GetCurBehaviorAction();
	}
	if(!pBehaviorAction)
	{
		return false;
	}

	if(pBehaviorAction->IsPlaying())
	{
		return false;
	}

	if(!strPlcID || strlen(strPlcID) <= 0)
	{
		return false;
	}

	if(!pFolderAnimation)
	{
		pFolderAnimation = GetActiveFolder(strPlcID,pBehaviorAction);
	}

	bool bAutoKey = m_bAutoKey;
	m_bAutoKey = true;

	if(!(animation = pBehaviorAction->FindAnimation(strPlcID,INTERPOLATOR_VISIBLE)))
	{
		//�����ɼ��Զ���
		CSAnimation *pParentAnimation = CSACommonAPI::GetTargetMainAmimation(strPlcID,strObjName,pBehaviorAction,pFolderAnimation);
		if(pParentAnimation)
		{
			animation = CSACommonAPI::AddAnimation(pBehaviorAction,strObjName,strPlcID,NULL,pParentAnimation);
			std::string strTmp = IDS_STRING_SHOW_HIDE;
			animation->SetName(strTmp.c_str());
		}
	}
	else
	{
		//int nCurrentTick = pBehaviorAction->GetCurrentTick();
		//if(nCurrentTick <= animation->GetLastTick())
		//{
		//	for(int i = nCurrentTick; i <= nCurrentTick + m_numAutoKeyFrame; i++)
		//	{
		//		if(animation->GetTimeline()->CheckKeyframe(i) >= 0)
		//		{
		//			pBehaviorAction->SetCurrentTick(animation->GetLastTick() + 1);
		//		}
		//	}
		//}
		TLRangeImpactAutoProcess(animation,m_numAutoKeyFrame);
	}

	if(animation == NULL)
	{
		return false;
	}

	int nTick = pBehaviorAction->GetCurrentTick();
	if(bOnOff)
	{
		//��ӿ�ʼ�ɼ��Թؼ�֡
		CSACommonAPI::AddVisibleKeyframe(pBehaviorAction,animation,nTick,"off");
		//���Ŀ��ɼ��Թؼ�֡
		nTick += m_numAutoKeyFrame;
		CSACommonAPI::AddVisibleKeyframe(pBehaviorAction,animation,nTick,"on");
	}
	else
	{
		//��ӿ�ʼ�ɼ��Թؼ�֡
		CSACommonAPI::AddVisibleKeyframe(pBehaviorAction,animation,nTick,"on");
		//���Ŀ��ɼ��Թؼ�֡
		nTick += m_numAutoKeyFrame;
		CSACommonAPI::AddVisibleKeyframe(pBehaviorAction,animation,nTick,"off");
	}

	pBehaviorAction->SetCurrentTick(nTick);
	m_bAutoKey = bAutoKey;


	if(m_pRecCB && bUpdateKeyEditor)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	return true;
}

bool CAnimationAPI::RecVisibleFade(VStringArray& aInsPlcPath, VStringArray& aInsName, bool bOnOff, bool bUpdateKeyEditor, CSBehaviorAction* pBehaviorAction)
{
	string strPlcPath = "";
	string strObjName = "";
	
	if(!pBehaviorAction)
	{
		pBehaviorAction = GetCurBehaviorAction();
	}
	if(!pBehaviorAction)
	{
		return false;
	}

	if(pBehaviorAction->IsPlaying())
	{
		return false;
	}


	int nSelectInsCount = aInsPlcPath.Count();
	//int nCurrentTick = GetAnimationsLastTick(aInsPlcPath,INTERPOLATOR_VISIBLE);
	//if(nCurrentTick < (int)pBehaviorAction->GetCurrentTick())
	//{
	//	nCurrentTick = (int)pBehaviorAction->GetCurrentTick();
	//}
	TLRangeImpactAutoProcess(aInsPlcPath,INTERPOLATOR_VISIBLE,m_numAutoKeyFrame);
	int nCurrentTick =pBehaviorAction->GetCurrentTick();
	CSAnimation* pFolderAnimation = AutoCreateFolder(aInsPlcPath,pBehaviorAction);
	bool bResult = false;
	for(int i = 0; i< nSelectInsCount; i++)
	{
		strPlcPath = aInsPlcPath[i];
		strObjName = aInsName[i];

		pBehaviorAction->SetCurrentTick(nCurrentTick);
		if(RecVisibleFade((char*)strPlcPath.c_str(), (char*)strObjName.c_str(),bOnOff,pFolderAnimation,false,pBehaviorAction))
		{
			bResult = true;
		}
	}

	if(m_pRecCB && bUpdateKeyEditor)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	return bResult;
}


bool CAnimationAPI::RecColor(const char* strPlcID, const char* strObjName, AniPoint& colorStart, AniPoint& colorEnd,CSAnimation* pFolderAnimation)
{
	CSAnimation *animation = NULL;
	
	if(!GetCurBehaviorAction())
	{
		return false;
	}

	if(GetCurBehaviorAction()->IsPlaying())
	{
		return false;
	}

	if(!strPlcID || strlen(strPlcID) <= 0)
	{
		return false;
	}

	//�����ʼ��ɫ�������ɫ��ͬ����¼��
	if(colorStart.Equal(colorEnd))
	{
		return false;
	}
	if(!pFolderAnimation)
	{
		pFolderAnimation = GetActiveFolder(strPlcID,GetCurBehaviorAction());
	}

	bool bAutoKey = m_bAutoKey;
	m_bAutoKey = true;

	if(!(animation = GetCurBehaviorAction()->FindAnimation(strPlcID,INTERPOLATOR_COLOR)))
	{
		//������ɫ����
		CSAnimation *pParentAnimation = CSACommonAPI::GetTargetMainAmimation(strPlcID,strObjName,GetCurBehaviorAction(),pFolderAnimation);
		if(pParentAnimation)
		{
			animation = CSACommonAPI::AddAnimation(GetCurBehaviorAction(),strObjName,strPlcID,NULL,pParentAnimation);
			std::string strTmp = IDS_STRING_COLOR;
			animation->SetName(strTmp.c_str());
		}
	}
	else
	{
		//int nCurrentTick = GetCurBehaviorAction()->GetCurrentTick();
		//if(nCurrentTick <= animation->GetLastTick())
		//{
		//	for(int i = nCurrentTick; i <= nCurrentTick + m_numAutoKeyFrame; i++)
		//	{
		//		if(animation->GetTimeline()->CheckKeyframe(i) >= 0)
		//		{
		//			GetCurBehaviorAction()->SetCurrentTick(animation->GetLastTick() + 1);
		//		}
		//	}
		//}
		TLRangeImpactAutoProcess(animation,m_numAutoKeyFrame);
	}

	if(animation == NULL)
	{
		return false;
	}
	
	//��ӿ�ʼ��ɫ�ؼ�֡
	int nTick = GetCurBehaviorAction()->GetCurrentTick();
	CSACommonAPI::AddColorKeyframe(GetCurBehaviorAction(),animation,nTick,colorStart,false);
	//���Ŀ����ɫ�ؼ�֡
	nTick += m_numAutoKeyFrame;
	CSACommonAPI::AddColorKeyframe(GetCurBehaviorAction(),animation,nTick,colorEnd,false);

	GetCurBehaviorAction()->SetCurrentTick(nTick);
	m_bAutoKey = bAutoKey;

	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	return true;
}


bool CAnimationAPI::RecColor(VStringArray& aInsPlcPath, VStringArray& aInsName,VHPointArray& aColorStart, AniPoint& colorEnd)
{
	string strPlcPath = "";
	string strObjName = "";
	AniPoint colorStart;

	if(!GetCurBehaviorAction())
	{
		return false;
	}

	if(GetCurBehaviorAction()->IsPlaying())
	{
		return false;
	}


	int nSelectInsCount = aInsPlcPath.Count();
	//int nCurrentTick = GetAnimationsLastTick(aInsPlcPath,INTERPOLATOR_COLOR);
	//if(nCurrentTick < (int)GetCurBehaviorAction()->GetCurrentTick())
	//{
	//	nCurrentTick = (int)GetCurBehaviorAction()->GetCurrentTick();
	//}
	TLRangeImpactAutoProcess(aInsPlcPath,INTERPOLATOR_COLOR,m_numAutoKeyFrame);
	int nCurrentTick = (int)GetCurBehaviorAction()->GetCurrentTick();
	CSAnimation* pFolderAnimation = AutoCreateFolder(aInsPlcPath,GetCurBehaviorAction());
	bool bResult = false;
	for(int i = 0; i< nSelectInsCount; i++)
	{
		strPlcPath = aInsPlcPath[i];
		strObjName = aInsName[i];
		colorStart= aColorStart[i];
		
		GetCurBehaviorAction()->SetCurrentTick(nCurrentTick);
		//�����ɫ�ؼ�֡
		if(RecColor((char*)strPlcPath.c_str(), (char*)strObjName.c_str(),colorStart,colorEnd,pFolderAnimation))
		{
			bResult = true;
		}
	}
	if(!bResult)
	{
		return false;
	}

	return true;
}
//¼����˸Ч��
bool CAnimationAPI::RecColorWink(const char* strPlcID, const char* strObjName, AniPoint& colorStart, AniPoint& colorEnd,int iTimes, float fDelay,CSAnimation* pFolderAnimation)
{
	CSAnimation *animation = NULL;

	if(!GetCurBehaviorAction())
	{
		return false;
	}

	if(GetCurBehaviorAction()->IsPlaying())
	{
		return false;
	}

	if(!strPlcID || strlen(strPlcID) <= 0)
	{
		return false;
	}
	if(iTimes<=0)
	{
		return false;
	}

	//�����ʼ��ɫ�������ɫ��ͬ����¼��
	if(colorStart.Equal(colorEnd))
	{
		return false;
	}
	
	if (iTimes <= 0)
	{
		return false;
	}

	if(!pFolderAnimation)
	{
		pFolderAnimation = GetActiveFolder(strPlcID,GetCurBehaviorAction());
	}

	int numAutoKeyFrame = m_numAutoKeyFrame;
	int TickCount = fDelay * GetCurBehaviorAction()->GetTicksPerSecond();
	int keyFrameCount = iTimes*2 +1;
	numAutoKeyFrame = TickCount / (keyFrameCount-1);
	bool bAutoKey = m_bAutoKey;
	m_bAutoKey = true;

	if(!(animation = GetCurBehaviorAction()->FindAnimation(strPlcID,INTERPOLATOR_COLOR)))
	{
		//������ɫ����
		CSAnimation *pParentAnimation = CSACommonAPI::GetTargetMainAmimation(strPlcID,strObjName,GetCurBehaviorAction(),pFolderAnimation);
		if(pParentAnimation)
		{
			animation = CSACommonAPI::AddAnimation(GetCurBehaviorAction(),strObjName,strPlcID,NULL,pParentAnimation);
			std::string strTmp = IDS_STRING_COLOR;
			animation->SetName(strTmp.c_str());
		}
	}
	else
	{
		//int nCurrentTick = GetCurBehaviorAction()->GetCurrentTick();
		//if(nCurrentTick <= animation->GetLastTick())
		//{
		//	for(int i = nCurrentTick; i <= nCurrentTick + numAutoKeyFrame; i++)
		//	{
		//		if(animation->GetTimeline()->CheckKeyframe(i) >= 0)
		//		{
		//			GetCurBehaviorAction()->SetCurrentTick(animation->GetLastTick() + 1);
		//		}
		//	}
		//}
		TLRangeImpactAutoProcess(animation,numAutoKeyFrame);
	}

	if(animation == NULL)
	{
		return false;
	}

	//��ӿ�ʼ��ɫ�ؼ�֡
	int nTick = GetCurBehaviorAction()->GetCurrentTick();
	CSACommonAPI::AddColorKeyframe(GetCurBehaviorAction(),animation,nTick,colorStart,true);

	for(int i = 0; i< iTimes; i++)
	{
		//���Ŀ����ɫ�ؼ�֡
		nTick += numAutoKeyFrame;
		CSACommonAPI::AddColorKeyframe(GetCurBehaviorAction(),animation,nTick,colorEnd,true);
		nTick += numAutoKeyFrame;
		CSACommonAPI::AddColorKeyframe(GetCurBehaviorAction(),animation,nTick,colorStart,true);
	}
	GetCurBehaviorAction()->SetCurrentTick(nTick);
	m_bAutoKey = bAutoKey;

	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	return true;
}

//¼����˸Ч��
bool CAnimationAPI::RecColorWink(VStringArray& aInsPlcPath, VStringArray& aInsName,VHPointArray& aColorStart, AniPoint pntEndColor, const int iTimes, const float fDelay)
{
	string strPlcPath = "";
	string strObjName = "";
	AniPoint colorStart;

	if (iTimes <= 0)
	{
		return false;
	}
	if(!GetCurBehaviorAction())
	{
		return false;
	}

	if(GetCurBehaviorAction()->IsPlaying())
	{
		return false;
	}

	int maxtimes = RecColorWinkMaxTimes(fDelay);
	if(iTimes > maxtimes)
	{
		//CString strErrMsg;
		//strErrMsg.Format(_T("��˸�������ܴ���%d"), maxtimes);
		//AfxMessageBox(strErrMsg);
		return false;
	}


	int nSelectInsCount = aInsPlcPath.Count();
	//int nCurrentTick = GetAnimationsLastTick(aInsPlcPath,INTERPOLATOR_COLOR);
	//if(nCurrentTick < (int)GetCurBehaviorAction()->GetCurrentTick())
	//{
	//	nCurrentTick = (int)GetCurBehaviorAction()->GetCurrentTick();
	//}
	int TickCount = fDelay * GetCurBehaviorAction()->GetTicksPerSecond();
	TLRangeImpactAutoProcess(aInsPlcPath,INTERPOLATOR_COLOR,TickCount);
	int nCurrentTick = (int)GetCurBehaviorAction()->GetCurrentTick();

	CSAnimation* pFolderAnimation = AutoCreateFolder(aInsPlcPath,GetCurBehaviorAction());
	bool bResult = false;
	for(int i = 0; i< nSelectInsCount; i++)
	{
		strPlcPath = aInsPlcPath[i];
		strObjName = aInsName[i];
		colorStart = aColorStart[i];

		GetCurBehaviorAction()->SetCurrentTick(nCurrentTick+1);
		//�����ɫ�ؼ�֡
		if(RecColorWink((char*)strPlcPath.c_str(), (char*)strObjName.c_str(),colorStart,pntEndColor,iTimes,fDelay,pFolderAnimation))
		{
			bResult = true;
		}
	}
	if(!bResult)
	{
		return false;
	}

	return true;
}

int CAnimationAPI::RecColorWinkMaxTimes(const float fDelay)
{
	int tps = GetCurBehaviorAction()->GetTicksPerSecond();
	int maxtimes = (int)(fDelay* tps - 1)/2;
	return maxtimes;
}

bool CAnimationAPI::RecVisibleWink(const char* strPlcID, const char* strObjName, bool bVisible, int iTimes, float fDelay, CSAnimation* pFolderAnimation /*= NULL*/)
{
	CSAnimation *animation = NULL;
	CSBehaviorAction* pBehaviorManager = pBehaviorManager = GetCurBehaviorAction();
	if(!pBehaviorManager)
	{
		return false;
	}

	if(pBehaviorManager->IsPlaying())
	{
		return false;
	}

	if(!strPlcID || strlen(strPlcID) <= 0)
	{
		return false;
	}

	if(iTimes<=0)
	{
		return false;
	}
	if(!pFolderAnimation)
	{
		pFolderAnimation = GetActiveFolder(strPlcID,pBehaviorManager);
	}

	int numAutoKeyFrame = m_numAutoKeyFrame;
	int TickCount = fDelay * pBehaviorManager->GetTicksPerSecond();
	int keyFrameCount = iTimes*2 +1;
	numAutoKeyFrame = TickCount / (keyFrameCount-1);
	bool bAutoKey = m_bAutoKey;
	m_bAutoKey = true;

	if(!(animation = pBehaviorManager->FindAnimation(strPlcID,INTERPOLATOR_VISIBLE)))
	{
		//�����ɼ��Զ���
		CSAnimation *pParentAnimation = CSACommonAPI::GetTargetMainAmimation(strPlcID,strObjName,pBehaviorManager,pFolderAnimation);
		if(pParentAnimation)
		{
			animation = CSACommonAPI::AddAnimation(pBehaviorManager,strObjName,strPlcID,NULL,pParentAnimation);
			animation->SetName("����");
		}
	}
	else
	{
		TLRangeImpactAutoProcess(animation,numAutoKeyFrame);
	}

	if(animation == NULL)
	{
		return false;
	}

	int nTick = pBehaviorManager->GetCurrentTick();
	//��ӿ�ʼ�ɼ��Թؼ�֡
	CSACommonAPI::AddVisibleKeyframe(pBehaviorManager,animation,nTick,"on");

	for(int i = 0; i< iTimes; i++)
	{
		//��ӿɼ��Թؼ�֡
		nTick += numAutoKeyFrame;
		CSACommonAPI::AddVisibleKeyframe(pBehaviorManager,animation,nTick,"off");
		nTick += numAutoKeyFrame;
		CSACommonAPI::AddVisibleKeyframe(pBehaviorManager,animation,nTick,"on");
	}
	GetCurBehaviorAction()->SetCurrentTick(nTick);
	m_bAutoKey = bAutoKey;

	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	return true;
}

/*******************************************************************
*	@functin	���붯������
*	@param[in]	strPlcID-������·��("SOUND:" + ģ���� + "_SARes\" +�����ļ���),
*				strObjName-�����ļ���
*	@retval		��
*	@author		Add By YL 2014.11.6
*   @modify		
********************************************************************/
bool CAnimationAPI::RecSound(const char* strPlcID, const char* strObjName,wstring& cstrFileName)
{
	CSAnimation *animation = NULL;
	int nTicksPerSecond = 10;	//֡/��
	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction = GetCurBehaviorAction();

	if(!pBehaviorAction)
	{
		return false;
	}
	if(pBehaviorAction->IsPlaying())
	{
		return false;
	}

	if(!strPlcID || strlen(strPlcID) <= 0)
	{
		return false;
	}

	//���Ŀ��ɼ��Թؼ�֡3 = ����ʱ�� * ��/֡
	CAnimationSound *pSound = new CAnimationSound();
	long dwSoundSecond = pSound->GetLength(cstrFileName.c_str());
	
	if(dwSoundSecond <= 0)
	{
		delete pSound;
		return false;
	}
	delete pSound;

	if(!(animation = pBehaviorAction->FindAnimation(strPlcID,INTERPOLATOR_SOUND)))
	{
		AniPoint pntCenterPnt(0.0,0.0,0.0);
		CSAnimation *pParentAnimation = GetActiveFolder(strPlcID,pBehaviorAction);
		if(!pParentAnimation)
		{
			pParentAnimation = CSACommonAPI::GetTargetObjectTypeFolder(strPlcID,pBehaviorAction);
		}
		if(pParentAnimation || !CSACommonAPI::IsUseTargetObjectTypeFolder())
		{
			animation = CSACommonAPI::AddAnimation(pBehaviorAction,strObjName,strPlcID,&pntCenterPnt,pParentAnimation);
			animation->SetName(strObjName);
		}
	}
	if(animation == NULL)
	{
		return false;
	}
	

	
	int nTick = pBehaviorAction->GetCurrentTick();
	//���Ŀ��ɼ��Թؼ�֡1
	CSACommonAPI::AddSoundKeyframe(pBehaviorAction,animation,nTick,"stop");
	//��ӿ�ʼ�ɼ��Թؼ�֡2(������λʱ�����Ź�+1)
	nTick += 1;
	CSACommonAPI::AddSoundKeyframe(pBehaviorAction,animation,nTick,"play");
	nTicksPerSecond = pBehaviorAction->GetTicksPerSecond();
	nTick +=  dwSoundSecond/1000.0 * nTicksPerSecond;	
	CSACommonAPI::AddSoundKeyframe(pBehaviorAction,animation,nTick,"stop");

	pBehaviorAction->SetCurrentTick(nTick);
	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData); 
	}
	AutoSaveSAFile();
	return true;
}
bool CAnimationAPI::RecImageVisible(VStringArray& aImgPlcPath, VStringArray& aImgName, VHPointArray& aImgPos, VHPointArray& aImgScale, bool bOnOff)
{
	if(!IsEnableRec())
	{
		return false;
	}
	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction = GetCurBehaviorAction();
	if(!pBehaviorAction)
	{
		return false;
	}
	int nSelectInsCount = aImgPlcPath.Count();
	int nCurrentTick = GetAnimationsLastTick(aImgPlcPath,INTERPOLATOR_POS);
	if(nCurrentTick < (int)pBehaviorAction->GetCurrentTick())
	{
		nCurrentTick = (int)pBehaviorAction->GetCurrentTick();
	}
	CSAnimation* pFolderAnimation = AutoCreateFolder(aImgPlcPath,pBehaviorAction);
	
	bool bResult = false;
	string strPlcPath = "";
	string strImgName = "";
	AniPoint pos;
	AniPoint scale;
	for(int i = 0; i< nSelectInsCount; i++)
	{
		strPlcPath = aImgPlcPath[i];
		strImgName = aImgName[i];
		pos = aImgPos[i];
		scale = aImgScale[i];
		pBehaviorAction->SetCurrentTick(nCurrentTick);
		if(RecImageVisible((char*)strPlcPath.c_str(), (char*)strImgName.c_str(), pos, scale, bOnOff,pFolderAnimation))
		{
			bResult = true;
		}
	}
	if(!bResult)
	{
		return false;
	}
	return true;
}
/*******************************************************************
*	@functin	���붯��ͼƬ
*	@param[in]	strPlcID-ͼƬ��·��("IMAGE:" + ģ���� + "_SARes\" +ͼƬ�ļ���),
*				strObjName-ͼƬ�ļ���
*	@retval		��
*	@author		Add By YL 2015.1.19
*   @modify		
********************************************************************/
bool CAnimationAPI::RecImageVisible(const char* strPlcPath, const char* strImgName, AniPoint& pos, AniPoint& scale, bool bOnOff,CSAnimation* pFolderAnimation)
{
	if(!IsEnableRec())
	{
		return false;
	}

	if(!strPlcPath || strlen(strPlcPath) <= 0)
	{
		return false;
	}

	CSAnimation *pAnimation = NULL;
	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction = GetCurBehaviorAction();
	if(!pBehaviorAction)
	{
		return false;
	}
	if(!pFolderAnimation)
	{
		pFolderAnimation = GetActiveFolder(strPlcPath,pBehaviorAction);
	}

	pAnimation = pBehaviorAction->FindAnimation(strPlcPath,INTERPOLATOR_POS);
	if (pAnimation == NULL)
	{
		//���û����ʾͼƬ�Ķ���������¼��ɾ��ͼƬ����
		//if(!bOnOff)
		//{
		//	return false;
		//}
		AniPoint pntCenterPnt(0.0,0.0,0.0);
		CSAnimation *pParentAnimation = NULL;
		if(pFolderAnimation)
		{
			pParentAnimation = pFolderAnimation;
		}
		else
		{
			pParentAnimation = CSACommonAPI::GetTargetObjectTypeFolder(strPlcPath,pBehaviorAction);
		}
		if(pParentAnimation || !CSACommonAPI::IsUseTargetObjectTypeFolder())
		{
			pAnimation = CSACommonAPI::AddAnimation(pBehaviorAction, strImgName, strPlcPath, NULL, pParentAnimation);
		}
	}
	if (pAnimation)
	{
		int nCurrentTick =pBehaviorAction->GetCurrentTick();
		if(pAnimation->GetLastTick() != 0 && nCurrentTick <= pAnimation->GetLastTick())
		{
			nCurrentTick = pAnimation->GetLastTick()+1;
			pBehaviorAction->SetCurrentTick(nCurrentTick);
		}
		pAnimation->SetName(strImgName);
		if (bOnOff)
		{
			//ͼƬ¼��_START_ ��ʱ���� ͼƬ�̶���Ļ��ɽ�if�����д������ɾ��
			if (pAnimation->GetTimeline() && pAnimation->GetTimeline()->GetTimelineArrayLength() > 0)
			{
				pAnimation->DuplicateNextOrPrevious(nCurrentTick, false);
				CSACommonAPI::AddVisibleKeyframe(pBehaviorAction, pAnimation, nCurrentTick, "off");
			}
			else
				CSACommonAPI::AddImageKeyframe(pBehaviorAction, pAnimation, nCurrentTick, pos, scale, "off", m_blinear);

			nCurrentTick = pAnimation->GetLastTick() + m_numAutoKeyFrame;
			pBehaviorAction->SetCurrentTick(nCurrentTick);

			if (pAnimation->GetTimeline() && pAnimation->GetTimeline()->GetTimelineArrayLength() > 0)
			{
				pAnimation->DuplicateNextOrPrevious(nCurrentTick, false);
				CSACommonAPI::AddVisibleKeyframe(pBehaviorAction, pAnimation, nCurrentTick, "on");
			}
			else
				CSACommonAPI::AddImageKeyframe(pBehaviorAction, pAnimation, nCurrentTick, pos, scale, "on", m_blinear);

			//CSACommonAPI::AddImageKeyframe(pBehaviorAction, pAnimation, nCurrentTick, pos, scale, "off", m_blinear);
			//nCurrentTick = pAnimation->GetLastTick() + m_numAutoKeyFrame;
			//pBehaviorAction->SetCurrentTick(nCurrentTick);
			//CSACommonAPI::AddImageKeyframe(pBehaviorAction, pAnimation, nCurrentTick, pos, scale, "on", m_blinear);
		}
		else
		{
			//��ǰ����û�йؼ�֡��ǰ��û����ʾͼƬ����ʱ��������¼��ͼƬɾ������
			//CSInterpolatorVisible* pInterpolatorVisible = (CSInterpolatorVisible*)pAnimation->GetInterpolator(INTERPOLATOR_VISIBLE);
			//if(pInterpolatorVisible)
			//{
			//	int nKeyNum = pInterpolatorVisible->GetArrayLength();
			//	if(nKeyNum > 0)
			//	{
			//		CKeyframeString* pKeyframe = (CKeyframeString*)pInterpolatorVisible->GetAt(nKeyNum-1);
			//		if(strcmp(pKeyframe->m_target,"off") == 0)
			//		{
			//			return false;
			//		}
			//	}
			//	else
			//	{
			//		return false;
			//	}
			//}
			//else
			//{
			//	return false;
			//}

			//ͼƬ¼��_START_ ��ʱ���� ͼƬ�̶���Ļ��ɽ�if�����д������ɾ��
			if (pAnimation->GetTimeline() && pAnimation->GetTimeline()->GetTimelineArrayLength() > 0)
			{
				pAnimation->DuplicateNextOrPrevious(nCurrentTick, false);
				CSACommonAPI::AddVisibleKeyframe(pBehaviorAction, pAnimation, nCurrentTick, "on");
			}
			else
				CSACommonAPI::AddImageKeyframe(pBehaviorAction, pAnimation, nCurrentTick, pos, scale, "on", m_blinear);

			nCurrentTick = pAnimation->GetLastTick() + m_numAutoKeyFrame;
			pBehaviorAction->SetCurrentTick(nCurrentTick);

			if (pAnimation->GetTimeline() && pAnimation->GetTimeline()->GetTimelineArrayLength() > 0)
			{
				pAnimation->DuplicateNextOrPrevious(nCurrentTick, false);
				CSACommonAPI::AddVisibleKeyframe(pBehaviorAction, pAnimation, nCurrentTick, "off");
			}
			else
				CSACommonAPI::AddImageKeyframe(pBehaviorAction,pAnimation,pBehaviorAction->GetCurrentTick(), pos, scale, "off", m_blinear);
		}
	}

	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}
	AutoSaveSAFile();
	return true;
}
/*******************************************************************
*	@functin	�༭ͼƬ
*	@param[in]	strPlcID-ͼƬ��·��("IMAGE:" + ģ���� + "_SARes\" +ͼƬ�ļ���),
*				AniPoint& pos��ͼƬλ��
				AniPoint& scale�����ű���
*	@retval		��
*	@author		Add By zhangcc 2015.3.11
*   @modify		
********************************************************************/
bool CAnimationAPI::RecImagePosScale(const char* strPlcID, AniPoint& pos, AniPoint& scale)
{
	if(!IsEnableRec())
	{
		return false;
	}

	if(!strPlcID || strlen(strPlcID) <= 0)
	{
		return false;
	}

	CSAnimation *pAnimation = NULL;
	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction = GetCurBehaviorAction();
	if(!pBehaviorAction)
	{
		return false;
	}
	pAnimation = pBehaviorAction->FindAnimation(strPlcID,INTERPOLATOR_POS);
	if (pAnimation)
	{
		CSTimeline* pTimeline = pAnimation->GetTimeline();
		int* tlArray = NULL;
		int tlArrayLength = pTimeline->GetTimelineArrayLength();
		if (tlArrayLength > 0)
			tlArray = pTimeline->GetTimelineArray();
		for (int i = 0; i < tlArrayLength; i++)
		{
			//ƽ��¼��_START_
			CSACommonAPI::AddPosKeyframe(GetCurBehaviorAction(),pAnimation,tlArray[i],pos,m_blinear);
			CSACommonAPI::AddScaleKeyframe(GetCurBehaviorAction(), pAnimation, tlArray[i], scale);
		}
	}

	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}
	AutoSaveSAFile();
	return true;
}
/************************************************************************/
/* ¼������                                                                     */
/************************************************************************/
bool CAnimationAPI::RecClipPlane(const char* strPlcID, const char* strName, AniPoint& pos, AniPoint& normal, bool bVisible, bool bAutoKeyFrame)
{
	if(!IsEnableRec())
	{
		return false;
	}

	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction = GetCurBehaviorAction();

	if(!pBehaviorAction)
	{
		return false;
	}
	if(pBehaviorAction->IsPlaying())
	{
		return false;
	}

	char strPlcPath[SA_BUFFER_SIZE_SMALL] = {0};
	CSAnimation *pAnimation = NULL;
	
	if (strPlcID == NULL)
	{
		std::string strTmp = IDS_STRING_CLIP_SECTION;
		
#ifdef WIN32
        strcpy_s(strPlcPath, SA_BUFFER_SIZE_SMALL, strTmp.c_str());
#else
        strcpy(strPlcPath,strTmp.c_str());
#endif

	}
	else
#ifdef WIN32
        strcpy_s(strPlcPath, SA_BUFFER_SIZE_SMALL, strPlcID);
#else
    strcpy(strPlcPath,strPlcID);
#endif
	pAnimation = pBehaviorAction->FindAnimation(strPlcPath, INTERPOLATOR_NORMAL);
	char animationName[SA_BUFFER_SIZE_SMALL] = {0};
	if (strName == NULL)
	{
		std::string strTmp = IDS_STRING_SECTION;
#ifdef WIN32
        strcpy_s(animationName, SA_BUFFER_SIZE_SMALL, strTmp.c_str());
#else
        strcpy(animationName,strTmp.c_str());
#endif
		
	}
	else
#ifdef WIN32
        strcpy_s(animationName, SA_BUFFER_SIZE_SMALL, strName);
#else
        strcpy(animationName,strName);
#endif
		
	if (pAnimation == NULL)
	{
		pAnimation = CSACommonAPI::AddAnimation(pBehaviorAction,animationName,strPlcPath,NULL,NULL);
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
		//����¼��_START_
		char cVisible[SA_BUFFER_SIZE_SMALL] = {0};
#ifdef WIN32
        strcpy_s(cVisible, SA_BUFFER_SIZE_SMALL, "on");
#else
        strcpy(cVisible,"on");
#endif
		CSACommonAPI::AddClipKeyframe(pBehaviorAction, pAnimation, pBehaviorAction->GetCurrentTick(), pos, normal, cVisible, m_blinear);
		if (bAutoKeyFrame)
		{
#ifdef WIN32
            strcpy_s(cVisible, SA_BUFFER_SIZE_SMALL, "off");
#else
            strcpy(cVisible,"off");
#endif
			CSACommonAPI::AddClipKeyframe(pBehaviorAction, pAnimation, pBehaviorAction->GetCurrentTick() + m_numAutoKeyFrame, pos, normal, cVisible, m_blinear);
			pBehaviorAction->SetCurrentTick(pAnimation->GetCurrentTick() + m_numAutoKeyFrame);
		}
	}
	else
	{
		CSTimeline* pTimeLine = pAnimation->GetTimeline();
		CSInterpolatorVisible* pInterpolatorVisible = (CSInterpolatorVisible*)pAnimation->GetInterpolator(INTERPOLATOR_VISIBLE);
		if(pTimeLine && pInterpolatorVisible )
		{
			if (bAutoKeyFrame)
			{
				int nCurrentTick = pBehaviorAction->GetCurrentTick();
				if(nCurrentTick > pAnimation->GetLastTick())
					pAnimation->DuplicateNextOrPrevious(pAnimation->GetCurrentTick(), false);
				pBehaviorAction->SetCurrentTick(pAnimation->GetLastTick() + m_numAutoKeyFrame);
			}

			pAnimation->SetName(animationName);
			//����¼��_START_
			char cVisible[SA_BUFFER_SIZE_SMALL] = {0};
            const char *temp = "on";
			if (!bVisible)
                temp = "off";
#ifdef WIN32
            strcpy_s(cVisible, SA_BUFFER_SIZE_SMALL, temp);
#else
            strcpy(cVisible,temp);
#endif
			CSACommonAPI::AddClipKeyframe(pBehaviorAction, pAnimation, pBehaviorAction->GetCurrentTick(), pos, normal, cVisible, m_blinear);
		}
		else
		{
			return false;
		}
		
	}
	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	return true;
}

bool CAnimationAPI::RecClipPlane( VStringArray& aStrID, VStringArray& aStrName, VHPointArray& aClipPos, VHPointArray& aClipNormal, VBoolArray& aClipVisible, bool bAutoKeyFrame /*= true*/ )
{
	if(!IsEnableRec())
	{
		return false;
	}
	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction = GetCurBehaviorAction();
	if(!pBehaviorAction)
	{
		return false;
	}
	if(pBehaviorAction->IsPlaying())
	{
		return false;
	}
	string strID, strName;
	AniPoint pos, normal;
	bool bVisible;
	int iCurrentTick = pBehaviorAction->GetCurrentTick();
	for (int i = 0; i < aStrID.Count(); i++)
	{
		strID = aStrID[i];
		strName = aStrName[i];
		pos = aClipPos[i];
		normal = aClipNormal[i];
		bVisible = aClipVisible[i];
		pBehaviorAction->SetCurrentTick(iCurrentTick);
		RecClipPlane(strID.c_str(), strName.c_str(), pos, normal, bVisible, bAutoKeyFrame);
	}
	return true;
}

/************************************************************************/
/* ��������                                                                     */
/************************************************************************/
//bool CAnimationAPI::CaptureClipPlane(const char* strPlcID)
//{
//	if(!IsEnableRec())
//	{
//		return false;
//	}
//	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction = GetCurBehaviorAction();
//	if(!pBehaviorAction)
//		return false;
//
//	if(pBehaviorAction->IsPlaying())
//		return false;
//
//	char strPlcPath[RMAX_PATH];
//	CSAnimation *pAnimation = NULL;
//	if (strPlcID == NULL)
//	{
//		strcpy_s(strPlcPath, MAX_PATH, "CLIP:����");
//	}
//	else
//	{
//		strcpy_s(strPlcPath, MAX_PATH, strPlcID);
//	}
//	pAnimation = pBehaviorAction->FindAnimation(strPlcPath, INTERPOLATOR_NORMAL);
//	if (pAnimation)
//	{
//		int nCurrentTick = pBehaviorAction->GetCurrentTick();
//		if(nCurrentTick < pAnimation->GetFirstTick())
//		{
//			pAnimation->DuplicateNextOrPrevious(nCurrentTick,true);
//		}
//		else
//		{
//			pAnimation->DuplicateNextOrPrevious(nCurrentTick,false);
//		}
//		CSTimeline* pTimeLine = pAnimation->GetTimeline();
//		CSInterpolatorVisible* pInterpolator = (CSInterpolatorVisible*)pAnimation->GetInterpolator(INTERPOLATOR_VISIBLE);
//		if(pTimeLine && pInterpolator)
//		{
//			int interval = 0;
//			float fraction = 0.0f;
//			pTimeLine->Evaluate(nCurrentTick,interval,fraction);
//			if(fabs(fraction - 1.0f) < 0.0001 && interval < pInterpolator->GetArrayLength()-1)
//			{
//				interval++;
//			}
//			
//			pInterpolator->Replace("off",interval);
//			pTimeLine->AddTLRange();
//		}
//		//pAnimation->AddHKeyFrame(false, m_blinear);
//	}
//
//	if(m_pRecCB)
//	{
//		m_pRecCB(m_pRecCBUserData);
//	}
//
//	AutoSaveSAFile();
//	return true;
//}

bool CAnimationAPI::CaptureClipPlane(VStringArray& aClipPlaneID, VStringArray& aClipPlaneName, VHPointArray& aClipPlanePos, VHPointArray& aClipPlaneNormal)
{
	bool bResult = false;
	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction = GetCurBehaviorAction();
	if(!pBehaviorAction)
		return bResult;

	if(pBehaviorAction->IsPlaying())
		return bResult;

	string strID, strName;
	AniPoint pos, normal;
	TLRangeImpactAutoProcess(aClipPlaneID,INTERPOLATOR_NORMAL, m_numAutoKeyFrame);
	int nCurrentTick =pBehaviorAction->GetCurrentTick();
	for (int i = 0; i < aClipPlaneID.Count(); i++)
	{
		strID = aClipPlaneID[i];
		strName = aClipPlaneName[i];
		pos = aClipPlanePos[i];
		normal = aClipPlaneNormal[i];
		CSAnimation *pAnimation = pBehaviorAction->FindAnimation(strID.c_str(), INTERPOLATOR_NORMAL);
		bool bCreate = false;
		if (pAnimation == NULL)
		{
			pAnimation = CSACommonAPI::AddAnimation(pBehaviorAction,strName.c_str(),strID.c_str(),NULL,NULL);
			bCreate = true;
		}
		if (pAnimation)
		{
			//��ӿ�ʼ�ɼ��Թؼ�֡
			CSACommonAPI::AddClipKeyframe(pBehaviorAction, pAnimation, nCurrentTick, pos, normal, "on", m_blinear);
			bool bCreateOff = true;//�ж��Ƿ�������عؼ�֡
			if (!bCreate)
			{
				if (nCurrentTick >= pAnimation->GetFirstTick() && nCurrentTick <= pAnimation->GetLastTick())
					bCreateOff = false;
			}
			//���Ŀ��ɼ��Թؼ�֡
			if (bCreateOff)
			{
				int iNextTick = nCurrentTick + m_numAutoKeyFrame;
				CSACommonAPI::AddClipKeyframe(pBehaviorAction, pAnimation, iNextTick, pos, normal, "off", m_blinear);
				if (bCreate)
					pBehaviorAction->SetCurrentTick(iNextTick);
			}
		}
	}

	if(m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData);
	}

	AutoSaveSAFile();
	bResult = true;
	return bResult;
}

void CAnimationAPI::DelInitPMI(string strParentPlcPath, int nPMIid)
{
	CSimulationAnimationManager* pSAManager = GetSAManager();
	if(!pSAManager) return;

	string strPlcPath = "";
	string strNoteId;
	std::stringstream ss;
	ss<<nPMIid;
	ss>>strNoteId;

	strPlcPath = "PMI:";
	if(strPlcPath.size()>0)
		strPlcPath += strParentPlcPath;
	strPlcPath = strPlcPath + strNoteId;

	pSAManager->DeleteInitTargetObject(strPlcPath.c_str());
}

void CAnimationAPI::DelInitIns(std::string strPlcPath, int radix)
{
	CSimulationAnimationManager* pSAManager = GetSAManager();
	if(!pSAManager) return;
	char szPlcPath[SA_BUFFER_SIZE_SMALL] = {0};
	if(strPlcPath.size()>0)
	{
		ConvertPlcPathToSAPlcPath(strPlcPath.c_str(), szPlcPath, radix, TARGETOBJECT_TYPE_INS);
		pSAManager->DeleteInitTargetObject(szPlcPath);
	}
}

void CAnimationAPI::AddInitTargetObject(TARGETOBJECTINFO targetObjectInfo)
{
	CSimulationAnimationManager* pSAManager = GetSAManager();
	if(!pSAManager) return;
	pSAManager->AddInitTargetObject(targetObjectInfo);
}

void CAnimationAPI::ModifyProcessTargetObject(const char* pOldPath, const char* pNewPath)
{
	CSimulationAnimationManager* pSAManager = GetSAManager();
	if(!pSAManager) return;
	pSAManager->ModifyProcessTargetObjectPath(pOldPath, pNewPath);
}

bool CAnimationAPI::RecToolInfo(char* strPath, char* strToolPath, char* strParentPath, char* tmpPath, bool bUpdateKeyFrame, CSBehaviorAction* pBehaviorAction, CSAnimation* pFolderAnimation)
{
	if (!pBehaviorAction)
		pBehaviorAction = GetCurBehaviorAction();

	if(!pBehaviorAction)
	{
		return false;
	}
	if(!IsEnableRec())
	{
		return false;
	}
	if(pBehaviorAction->IsPlaying())
	{
		return false;
	}
	if(!strPath || strlen(strPath) <= 0)
	{
		return false;
	}

	if (pFolderAnimation == NULL)
		pFolderAnimation = GetActiveFolder(strPath,pBehaviorAction);

	CSAnimation *pAnimation = NULL;
	pAnimation = pBehaviorAction->FindAnimation(strPath, INTERPOLATOR_INSTANCECREATE);
	if(!pAnimation)
	{
		pAnimation = CSACommonAPI::AddAnimation(pBehaviorAction, "����", strPath, NULL, pFolderAnimation);
	}
	if(pAnimation == NULL)
	{
		return false;
	}

	int nTick = pBehaviorAction->GetCurrentTick();
	CSACommonAPI::AddToolKeyframe(pBehaviorAction, pAnimation, nTick, strPath, strToolPath, strParentPath);
	if(bUpdateKeyFrame && m_pRecCB)
	{
		m_pRecCB(m_pRecCBUserData); 
	}
	AutoSaveSAFile();
	return true;
}
