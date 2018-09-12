// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		AnimationAPI.h 
 *
 *	@brief	动画仿真相关操作接口类
 *
 *	@par	历史:
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
	CaptureRecPosRot,			//位置
	CaptureRecVisible,			//显隐
	CaptureRecClipPlane,		//剖面
	CaptureRecCamera,			//场景
	CaptureRecAll				//全部
};

class SA_API CAnimationAPI
{
public:
	CAnimationAPI();
	~CAnimationAPI(void);
	static CAnimationAPI* GetInstance(RecCB* pRecCB = NULL, void * pRecCBUserData = NULL);

	//外部调用接口
	void SetSAManager(CSimulationAnimationManager * pSAManager);	//设置动画管理类
	void GetSAManager(vector<CSimulationAnimationManager*>&);
	NS_SimulationAnimation::CSimulationAnimationManager *GetSAManager(){ return m_pSAManager;};
	CSAnimation* GetActiveAnimationFolder() {return m_pActiveAnimationFolder;};
	void SetActiveAnimationFolder(CSAnimation* pActiveAnimationFolder) { m_pActiveAnimationFolder = pActiveAnimationFolder;};
private:
	//动画管理类的全局变量
	NS_SimulationAnimation::CSimulationAnimationManager * m_pSAManager;
	CSAnimation* m_pActiveAnimationFolder;
	RecCB* m_pRecCB;
	void * m_pRecCBUserData;
	RecCB* m_pPlayCB;
	void * m_pPlayCBUserData;
	int					m_iCaptureRecType;//捕获录制类型
public:
	static CAnimationAPI* m_pInstance;
	//录制相关变量
	bool m_bShowRecUI;		//是否处于录制状态
	bool m_bRec;			//是否处于录制状态
	bool m_bAutoKey;		//是否启用关键帧
	int m_numAutoKeyFrame;	//自动画关键帧增加帧数
	bool m_blinear;			//是否启用曲线模式
	bool m_bEvaluateFlag;	//是否启用多级动画
	bool m_bAutoSave;		//录制动画时，是否自动保存
	bool m_bRecCamera;		//是否录制摄像机
	bool m_bAutoCreateFolder;//多选录制时，是否自动创建分组
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
	//录制相关接口
	static void ConvertPlcPathToSAPlcPath(const char *inPlcPath,char *outPlcPath,int radix = 16, int objType = TARGETOBJECT_TYPE_INS);
	static void ConvertSAPlcPathToDisPlcPath(const char *oldPlcPath, char *newPlcPath, bool bPre = false);
	bool AutoSaveSAFile();
	int SaveSAFile();
	int SaveAsSAFile(const wchar_t* strPathName);
	bool SAFilePathNameIsEmpty();
	//设置录制回调函数
	void SetRecCB(RecCB* pRecCB,void * pRecCBUserData){ m_pRecCB = pRecCB; m_pRecCBUserData = pRecCBUserData;};
	void SetPlayCB(PlayCB* pPlayCB,void * pPlayCBUserData){ m_pPlayCB = pPlayCB; m_pPlayCBUserData = pPlayCBUserData;};
	//判断是否处于可录制状态
	bool IsEnableRec();
	//判断是否有步骤
	bool HasBehaviorAction(bool bShowMsg);
	//获取指定对象的旋转重心，如果没有该对象，返回false
	bool GetPivot(const char* strPlcID,AniPoint& pntPivot);
	//获取要录制对象的默认分组
	CSAnimation* GetActiveFolder(const string strPlcPath,CSBehaviorAction* pBehaviorAction);
	//多选录制时，自动创建分组
	CSAnimation* AutoCreateFolder(VStringArray& aInsPlcPath,CSBehaviorAction* pBehaviorAction);
	//更新帧编辑器
	void UpdateKeyEditor();
	void UpdateKeyEditorPlay();
	//判断模型是否可录制旋转动画
	bool RecInsRotCheck(const char* strPlcID, AniPoint pntCenterPnt);
	//判断录制动画时，要录制内容与现有时间线冲突是否有冲突
	bool IsTLRangeImpact(CSAnimation* pAnimation,int keyNum ,int& ImpactType,CTimeLineRange** ppTLRange);
	bool IsTLRangeImpact(VStringArray& aInsPlcPath,const char* interpolatorType,int Numkey ,int& ImpactType,VArray<CTimeLineRange*>& pTLRangeA );
	//录制动画时，自动处理要录制内容与现有时间线冲突
	void TLRangeImpactAutoProcess(CSAnimation* pAnimation,int keyNum );
	void TLRangeImpactAutoProcess(VStringArray& aInsPlcPath,const char* interpolatorType,int keyNum );
	//同时录制平移和旋转动画
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

	//沿轨迹线录制接口
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
	//获取实例列表动画的最后一个关键帧位置
	int GetAnimationsLastTick(VStringArray& aInsPlcPath,const char* interpolatorType,CSBehaviorAction* pBehaviorAction = NULL );
	//显隐动画录制接口
	bool RecVisible(const char* strPlcID, const char* strObjName, bool bVisible,CSBehaviorAction* pBehaviorAction = NULL,CSAnimation* pFolderAnimation = NULL);
	//淡入淡出录制接口
	bool RecVisibleFade(const char* strPlcID, const char* strObjName, bool bOnOff,CSAnimation* pFolderAnimation = NULL, bool bUpdateKeyEditor = true, CSBehaviorAction* pBehaviorAction = NULL);
	bool RecVisibleFade(VStringArray& aInsPlcPath, VStringArray& aInsName, bool bOnOff,	bool bUpdateKeyEditor = true, CSBehaviorAction* pBehaviorAction = NULL);
	//录制颜色动画接口
	bool RecColor(const char* strPlcID, const char* strObjName, AniPoint& colorStart, AniPoint& colorEnd,CSAnimation* pFolderAnimation = NULL);
	bool RecColor(VStringArray& aInsPlcPath, VStringArray& aInsName,VHPointArray& aColorStart, AniPoint& colorEnd);
	//闪烁效果录制接口
	bool RecColorWink(const char* strPlcID, const char* strObjName, AniPoint& colorStart, AniPoint& colorEnd,int iTimes, float fDelay,CSAnimation* pFolderAnimation = NULL);
	bool RecColorWink(VStringArray& aInsPlcPath, VStringArray& aInsName,VHPointArray& aColorStart, AniPoint pntEndColor, const int iTimes, const float fDelay);
	int RecColorWinkMaxTimes(const float fDelay);
	bool RecVisibleWink(const char* strPlcID, const char* strObjName, bool bVisible, int iTimes, float fDelay, CSAnimation* pFolderAnimation = NULL);
	//摄像机录制接口
	bool RecCamera(float pivot[3], float fMtxAA[4][4], float fScale, float fIntWind[2], bool bUseAutoKey = false,CSBehaviorAction* pBehaviorAction = NULL);
	//录制局部放大接口
	bool RecZoom(const char* strZoomId,const char* strObjName, float fMtxAA[4][4], float fScale, float fIntWind[2],bool bVisible,bool bRelaceVisible = true);
	//录制声音接口
	bool RecSound(const char* strPlcID, const char* strObjName , wstring& cstrFileName);
	//录制图片接口
	bool RecImageVisible(VStringArray& aImgPlcPath, VStringArray& aImgName, VHPointArray& aImgPos, VHPointArray& aImgScale, bool bOnOff);
	bool RecImageVisible(const char* strPlcPath, const char* strImgName, AniPoint& pos, AniPoint& scale, bool bOnOff,CSAnimation* pFolderAnimation);
	/*******************************************************************
	*	@functin	编辑图片
	*	@param[in]	strPlcID-图片的路径("IMAGE:" + 模型名 + "_SARes\" +图片文件名),
	*				AniPoint& pos：图片位置
					AniPoint& scale：缩放比例
	*	@retval		无
	*	@author		Add By zhangcc 2015.3.11
	*   @modify		
	********************************************************************/
	bool RecImagePosScale(const char* strPlcID, AniPoint& pos, AniPoint& scale);
	//录制剖切接口
	bool RecClipPlane(VStringArray& aStrID, VStringArray& aStrName, VHPointArray& aClipPos, VHPointArray& aClipNormal, VBoolArray& aClipVisible, bool bAutoKeyFrame = true);
	bool RecClipPlane(const char* strPlcID, const char* strName, AniPoint& pos, AniPoint& normal, bool bVisible, bool bAutoKeyFrame = true);
	bool CaptureClipPlane(VStringArray& aClipPlaneID, VStringArray& aClipPlaneName, VHPointArray& aClipPlanePos, VHPointArray& aClipPlaneNormal);
	////删除对象及该对象所有动画
	//bool DeleteTarget(const char* strPlcID);		
	////删除一条动画
	//bool DeleteAnimation(const char* strPlcID, const char *interpolatortype);		
	void SetCaptureRecType(int iType){m_iCaptureRecType = iType;}
	int GetCaptureRecType(){return m_iCaptureRecType;}
	//根据指定路径和ID删除初始化列表PMI   Edited by qinyp 2015/11/09
	void DelInitPMI(string strPlcPath, int nPMIid);
	//根据指定路径删除零件初始动画项 radix 进制
	void DelInitIns(std::string strPlcPath, int radix);
	//增加初始对象
	void AddInitTargetObject(TARGETOBJECTINFO targetObjectInfo);
	//修改对象的路径
	void ModifyProcessTargetObject(const char* pOldPath, const char* pNewPath);
	//录制工具相关信息
	bool RecToolInfo(char* strPath, char* strToolPath, char* strParentPath, char* tmpPath, bool bUpdateKeyFrame = false,CSBehaviorAction* pBehaviorAction = NULL,
		CSAnimation* pFolderAnimation = NULL);
};
#endif
