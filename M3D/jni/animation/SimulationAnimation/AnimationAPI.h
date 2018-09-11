// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		AnimationAPI.h 
 *
 *	@brief	����������ز����ӿ���
 *
 *	@par	��ʷ:
 *		2014/09/26	created by yhp
****************************************************************************/
#ifndef _H_ANIMATIONAPI_H
#define _H_ANIMATIONAPI_H

#pragma once
#include "../SimulationCommon/SATools.h"
#include "KeyFrame.h"
#include "SBehaviorAction.h"
#include "SAnimation.h"
#include "SimulationAnimationManager.h"
#include "STimeline.h"
#include "SInterpolator.h"
#include "AnimationSound.h"
#include <stdio.h>
#include <string.h>
#include "../SimulationCommon/varray.h"
#include "../SimulationCommon/vlist.h"

using namespace std;

typedef void RecCB(void * userData);
typedef void PlayCB(void * userData);

typedef VArray<string> VStringArray;
typedef VArray<AniPoint> VHPointArray;
typedef VArray<AniQuat> VHQuatArray;
typedef	VArray<bool> VBoolArray;

enum CaptureRecType
{
	CaptureRecPosRot,			//λ��
	CaptureRecVisible,			//����
	CaptureRecClipPlane,		//����
	CaptureRecCamera,			//����
	CaptureRecAll				//ȫ��
};

class SA_API CAnimationAPI
{
public:
	CAnimationAPI();
	~CAnimationAPI(void);
	static CAnimationAPI* GetInstance(RecCB* pRecCB = NULL, void * pRecCBUserData = NULL);

	//�ⲿ���ýӿ�
	void SetSAManager(CSimulationAnimationManager * pSAManager);	//���ö���������
	void GetSAManager(vector<CSimulationAnimationManager*>&);
	NS_SimulationAnimation::CSimulationAnimationManager *GetSAManager(){ return m_pSAManager;};
	CSAnimation* GetActiveAnimationFolder() {return m_pActiveAnimationFolder;};
	void SetActiveAnimationFolder(CSAnimation* pActiveAnimationFolder) { m_pActiveAnimationFolder = pActiveAnimationFolder;};
private:
	//�����������ȫ�ֱ���
	NS_SimulationAnimation::CSimulationAnimationManager * m_pSAManager;
	CSAnimation* m_pActiveAnimationFolder;
	RecCB* m_pRecCB;
	void * m_pRecCBUserData;
	RecCB* m_pPlayCB;
	void * m_pPlayCBUserData;
	int					m_iCaptureRecType;//����¼������
public:
	static CAnimationAPI* m_pInstance;
	//¼����ر���
	bool m_bShowRecUI;		//�Ƿ���¼��״̬
	bool m_bRec;			//�Ƿ���¼��״̬
	bool m_bAutoKey;		//�Ƿ����ùؼ�֡
	int m_numAutoKeyFrame;	//�Զ����ؼ�֡����֡��
	bool m_blinear;			//�Ƿ���������ģʽ
	bool m_bEvaluateFlag;	//�Ƿ����ö༶����
	bool m_bAutoSave;		//¼�ƶ���ʱ���Ƿ��Զ�����
	bool m_bRecCamera;		//�Ƿ�¼�������
	bool m_bAutoCreateFolder;//��ѡ¼��ʱ���Ƿ��Զ���������
	bool m_bRecording;

	enum TLImpactType
	{
		TLImpactType_None,
		TLImpactType_Before,
		TLImpactType_Mid,
		TLImpactType_End
	};
	
public:
	NS_SimulationAnimation::CSBehaviorAction * GetCurBehaviorAction();
	NS_SimulationAnimation::CSBehaviorAction * GetCurProcessBehaviorAction();
	bool IsReversePlay();
	void SetReversePlay(bool bFlg);
	//¼����ؽӿ�
	static void ConvertPlcPathToSAPlcPath(const char *inPlcPath,char *outPlcPath,int radix = 16, int objType = TARGETOBJECT_TYPE_INS);
	static void ConvertSAPlcPathToDisPlcPath(const char *oldPlcPath, char *newPlcPath, bool bPre = false);
	bool AutoSaveSAFile();
	int SaveSAFile();
	int SaveAsSAFile(const wchar_t* strPathName);
	bool SAFilePathNameIsEmpty();
	//����¼�ƻص�����
	void SetRecCB(RecCB* pRecCB,void * pRecCBUserData){ m_pRecCB = pRecCB; m_pRecCBUserData = pRecCBUserData;};
	void SetPlayCB(PlayCB* pPlayCB,void * pPlayCBUserData){ m_pPlayCB = pPlayCB; m_pPlayCBUserData = pPlayCBUserData;};
	//�ж��Ƿ��ڿ�¼��״̬
	bool IsEnableRec();
	//�ж��Ƿ��в���
	bool HasBehaviorAction(bool bShowMsg);
	//��ȡָ���������ת���ģ����û�иö��󣬷���false
	bool GetPivot(const char* strPlcID,AniPoint& pntPivot);
	//��ȡҪ¼�ƶ����Ĭ�Ϸ���
	CSAnimation* GetActiveFolder(const string strPlcPath,CSBehaviorAction* pBehaviorAction);
	//��ѡ¼��ʱ���Զ���������
	CSAnimation* AutoCreateFolder(VStringArray& aInsPlcPath,CSBehaviorAction* pBehaviorAction);
	//����֡�༭��
	void UpdateKeyEditor();
	void UpdateKeyEditorPlay();
	//�ж�ģ���Ƿ��¼����ת����
	bool RecInsRotCheck(const char* strPlcID, AniPoint pntCenterPnt);
	//�ж�¼�ƶ���ʱ��Ҫ¼������������ʱ���߳�ͻ�Ƿ��г�ͻ
	bool IsTLRangeImpact(CSAnimation* pAnimation,int keyNum ,int& ImpactType,CTimeLineRange** ppTLRange);
	bool IsTLRangeImpact(VStringArray& aInsPlcPath,const char* interpolatorType,int Numkey ,int& ImpactType,VArray<CTimeLineRange*>& pTLRangeA );
	//¼�ƶ���ʱ���Զ�����Ҫ¼������������ʱ���߳�ͻ
	void TLRangeImpactAutoProcess(CSAnimation* pAnimation,int keyNum );
	void TLRangeImpactAutoProcess(VStringArray& aInsPlcPath,const char* interpolatorType,int keyNum );
	//ͬʱ¼��ƽ�ƺ���ת����
	bool RecInsPosRot(const char* strPlcID, 
		const char* strObjName, 
		AniPoint pntCenterPnt, 
		AniPoint pntPos, 
		AniQuat qual, 
		bool bAddNewKey,
		bool bLiner, 
		bool bFollowPath,
		bool bUpdateKeyEditor = true,
		CSBehaviorAction* pBehaviorAction = NULL,
		CSAnimation* pFolderAnimation = NULL);
	bool RecInsPosRot(VStringArray& aInsPlcPath,
		VStringArray& aInsName,
		VHPointArray& aInsEndTranslate,
		VHQuatArray& aInsEndRotaion,
		VHPointArray& aInsCenter,
		bool bAddNewKey,
		bool bUpdateKeyEditor = true,
		CSBehaviorAction* pBehaviorAction = NULL);
	bool RecInsPosRot(VStringArray& aInsPlcPath,
		VStringArray& aInsName,
		VHPointArray& aInsStartTranslate,
		VHPointArray& aInsEndTranslate,
		VHQuatArray& aInsStartRotaion,
		VHQuatArray& aInsEndRotaion,
		VHPointArray& aInsCenter,
		bool bAddNewKey,
		bool bUpdateKeyEditor = true,
		CSBehaviorAction* pBehaviorAction = NULL);

	//�ع켣��¼�ƽӿ�
	float GetFollowPathLenPerFrame(VHPointArray& aPathPoint);
	int GetFollowPathAutoKeyNum(VHPointArray& aPathPoint,int pointIdx,float fLenPerFrame);
	AniPoint GetFollowPathInsPos(VHPointArray& aPathPoint,int pointIdx,AniPoint insStartPnt);
	bool RecInsPosRotFollowPath(VStringArray& aInsPlcPath,
		VStringArray& aInsName,
		VHPointArray& aInsStartTranslate,
		VHQuatArray& aInsStartRotaion,
		VHPointArray& aInsCenter,
		VHPointArray& aPathPoint,
		VIntArray& aPntSep,
		VHQuatArray& aInsParentRotaion,
		CSBehaviorAction* pBehaviorAction = NULL);
	//��ȡʵ���б��������һ���ؼ�֡λ��
	int GetAnimationsLastTick(VStringArray& aInsPlcPath,const char* interpolatorType,CSBehaviorAction* pBehaviorAction = NULL );
	//��������¼�ƽӿ�
	bool RecVisible(const char* strPlcID, const char* strObjName, bool bVisible,CSBehaviorAction* pBehaviorAction = NULL,CSAnimation* pFolderAnimation = NULL);
	//���뵭��¼�ƽӿ�
	bool RecVisibleFade(const char* strPlcID, const char* strObjName, bool bOnOff,CSAnimation* pFolderAnimation = NULL, bool bUpdateKeyEditor = true, CSBehaviorAction* pBehaviorAction = NULL);
	bool RecVisibleFade(VStringArray& aInsPlcPath, VStringArray& aInsName, bool bOnOff,	bool bUpdateKeyEditor = true, CSBehaviorAction* pBehaviorAction = NULL);
	//¼����ɫ�����ӿ�
	bool RecColor(const char* strPlcID, const char* strObjName, AniPoint& colorStart, AniPoint& colorEnd,CSAnimation* pFolderAnimation = NULL);
	bool RecColor(VStringArray& aInsPlcPath, VStringArray& aInsName,VHPointArray& aColorStart, AniPoint& colorEnd);
	//��˸Ч��¼�ƽӿ�
	bool RecColorWink(const char* strPlcID, const char* strObjName, AniPoint& colorStart, AniPoint& colorEnd,int iTimes, float fDelay,CSAnimation* pFolderAnimation = NULL);
	bool RecColorWink(VStringArray& aInsPlcPath, VStringArray& aInsName,VHPointArray& aColorStart, AniPoint pntEndColor, const int iTimes, const float fDelay);
	int RecColorWinkMaxTimes(const float fDelay);
	bool RecVisibleWink(const char* strPlcID, const char* strObjName, bool bVisible, int iTimes, float fDelay, CSAnimation* pFolderAnimation = NULL);
	//�����¼�ƽӿ�
	bool RecCamera(float pivot[3], float fMtxAA[4][4], float fScale, float fIntWind[2], bool bUseAutoKey = false,CSBehaviorAction* pBehaviorAction = NULL);
	//¼�ƾֲ��Ŵ�ӿ�
	bool RecZoom(const char* strZoomId,const char* strObjName, float fMtxAA[4][4], float fScale, float fIntWind[2],bool bVisible,bool bRelaceVisible = true);
	//¼�������ӿ�
	bool RecSound(const char* strPlcID, const char* strObjName , wstring& cstrFileName);
	//¼��ͼƬ�ӿ�
	bool RecImageVisible(VStringArray& aImgPlcPath, VStringArray& aImgName, VHPointArray& aImgPos, VHPointArray& aImgScale, bool bOnOff);
	bool RecImageVisible(const char* strPlcPath, const char* strImgName, AniPoint& pos, AniPoint& scale, bool bOnOff,CSAnimation* pFolderAnimation);
	/*******************************************************************
	*	@functin	�༭ͼƬ
	*	@param[in]	strPlcID-ͼƬ��·��("IMAGE:" + ģ���� + "_SARes\" +ͼƬ�ļ���),
	*				AniPoint& pos��ͼƬλ��
					AniPoint& scale�����ű���
	*	@retval		��
	*	@author		Add By zhangcc 2015.3.11
	*   @modify		
	********************************************************************/
	bool RecImagePosScale(const char* strPlcID, AniPoint& pos, AniPoint& scale);
	//¼�����нӿ�
	bool RecClipPlane(VStringArray& aStrID, VStringArray& aStrName, VHPointArray& aClipPos, VHPointArray& aClipNormal, VBoolArray& aClipVisible, bool bAutoKeyFrame = true);
	bool RecClipPlane(const char* strPlcID, const char* strName, AniPoint& pos, AniPoint& normal, bool bVisible, bool bAutoKeyFrame = true);
	bool CaptureClipPlane(VStringArray& aClipPlaneID, VStringArray& aClipPlaneName, VHPointArray& aClipPlanePos, VHPointArray& aClipPlaneNormal);
	////ɾ�����󼰸ö������ж���
	//bool DeleteTarget(const char* strPlcID);		
	////ɾ��һ������
	//bool DeleteAnimation(const char* strPlcID, const char *interpolatortype);		
	void SetCaptureRecType(int iType){m_iCaptureRecType = iType;}
	int GetCaptureRecType(){return m_iCaptureRecType;}
	//����ָ��·����IDɾ����ʼ���б�PMI   Edited by qinyp 2015/11/09
	void DelInitPMI(string strPlcPath, int nPMIid);
	//����ָ��·��ɾ�������ʼ������ radix ����
	void DelInitIns(std::string strPlcPath, int radix);
	//���ӳ�ʼ����
	void AddInitTargetObject(TARGETOBJECTINFO targetObjectInfo);
	//�޸Ķ����·��
	void ModifyProcessTargetObject(const char* pOldPath, const char* pNewPath);
	//¼�ƹ��������Ϣ
	bool RecToolInfo(char* strPath, char* strToolPath, char* strParentPath, char* tmpPath, bool bUpdateKeyFrame = false,CSBehaviorAction* pBehaviorAction = NULL,
		CSAnimation* pFolderAnimation = NULL);
};
#endif
