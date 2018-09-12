#pragma once
/*******************************************************************************
*	@file
*		CAnimationCallBack.h
*	@brief
*		CAnimationCallBack 动画播放回调函数类的接口声明
*	@par
*	add by qinyp on 2018-03-29
**********************************************************************************/
#ifndef _H_ANIMATIONCALLBACK_H_
#define _H_ANIMATIONCALLBACK_H_

#include <stdlib.h>
#include <vector>
#include "../SimulationCommon/SATools.h"
#include "../SimulationCommon/Animation_def.h"

//#ifdef WIN32
//typedef void(_stdcall *GetModelPlcMtxCB) (const char plcIdPath[SA_BUFFER_SIZE_BIG], float matrix[4][4], void* pInstance);
//typedef void(_stdcall *SetModelPlcMtxCB) (const char plcIdPath[SA_BUFFER_SIZE_BIG],const float matrix[4][4], void* pInstance);
//typedef void(_stdcall *GetCameraCB) (const char plcIdPath[SA_BUFFER_SIZE_BIG],float scale[3],float matrix[4][4], void* pInstance);
//typedef void(_stdcall *SetCameraCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG],const float scale[3],const float matrix[4][4], void* pInstance);
//typedef void(_stdcall *GetCameraFocalCB)(float& fPosTargetDistance, void* pInstance);
//typedef void(_stdcall *GetCameraTargetPntCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG],float targetPnt[3], void* pInstance);
//typedef void(_stdcall *PlayVisibleCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG], const char name[SA_BUFFER_SIZE_BIG], bool bOnOff, float fTran, void* pInstance);
//typedef void(_stdcall *PlayColorCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG], const char name[SA_BUFFER_SIZE_BIG], const float fColor[3], void* pInstance);
//typedef void(_stdcall *PlaySoundCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG], bool bPlayFlg,float fTime, void* pInstance);
//typedef void(_stdcall *PlayImageCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG], const char name[SA_BUFFER_SIZE_BIG], const float fPos[3], const float fScale[3], bool bOnOffFlg, void* pInstance);
//typedef void(_stdcall *PlayClipPlaneCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG], const char name[SA_BUFFER_SIZE_BIG], const float fNormal[3], const float fPos[3], bool bOnOff, void* pInstance);
//typedef void(_stdcall *PlayToolCB)(int nType, const char plcIdPath[SA_BUFFER_SIZE_BIG], const char strToolPath[SA_BUFFER_SIZE_BIG], const char strParentPath[SA_BUFFER_SIZE_BIG], void* pInstance);
//
////更新视图回调函数
//typedef void(_stdcall *UpdateViewCB) (void* pBehaviorAction, void* pInstance);
////开始播放回调函数
//typedef void(_stdcall *PlayBeginCB) (void* pBehaviorAction, void* pInstance);
////播放结束回调函数
//typedef void(_stdcall *PlayEndCB) (void* pBehaviorAction, void* pInstance);
//
////设置视口内各对象的状态
//typedef void(_stdcall *SetTargetStateCB) (void* targetList, void* pInstance);
//
////锁定视图回调函数
//typedef void(_stdcall *LockViewCB) (bool bLock, void* pInstance);
//#else
typedef void( *GetModelPlcMtxCB) (const char plcIdPath[SA_BUFFER_SIZE_BIG], float matrix[4][4], void* pInstance);
typedef void( *SetModelPlcMtxCB) (const char plcIdPath[SA_BUFFER_SIZE_BIG],const float matrix[4][4], void* pInstance);
typedef void( *GetCameraCB) (const char plcIdPath[SA_BUFFER_SIZE_BIG],float scale[3],float matrix[4][4], void* pInstance);
typedef void( *SetCameraCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG],const float scale[3],const float matrix[4][4], void* pInstance);
typedef void( *GetCameraFocalCB)(float& fPosTargetDistance, void* pInstance);
typedef void( *GetCameraTargetPntCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG],float targetPnt[3], void* pInstance);
typedef void( *PlayVisibleCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG], const char name[SA_BUFFER_SIZE_BIG], bool bOnOff, float fTran, void* pInstance);
typedef void( *PlayColorCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG], const char name[SA_BUFFER_SIZE_BIG], const float fColor[3], void* pInstance);
typedef void( *PlaySoundCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG], bool bPlayFlg,float fTime, void* pInstance);
typedef void( *PlayImageCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG], const char name[SA_BUFFER_SIZE_BIG], const float fPos[3], const float fScale[3], bool bOnOffFlg, void* pInstance);
typedef void( *PlayClipPlaneCB)(const char plcIdPath[SA_BUFFER_SIZE_BIG], const char name[SA_BUFFER_SIZE_BIG], const float fNormal[3], const float fPos[3], bool bOnOff, void* pInstance);
typedef void( *PlayToolCB)(int nType, const char plcIdPath[SA_BUFFER_SIZE_BIG], const char strToolPath[SA_BUFFER_SIZE_BIG], const char strParentPath[SA_BUFFER_SIZE_BIG], void* pInstance);

//更新视图回调函数
typedef void( *UpdateViewCB) (void* pBehaviorAction, void* pInstance);
//开始播放回调函数
typedef void( *PlayBeginCB) (void* pBehaviorAction, void* pInstance);
//播放结束回调函数
typedef void( *PlayEndCB) (void* pBehaviorAction, void* pInstance);

//设置视口内各对象的状态
typedef void( *SetTargetStateCB) (void* targetList, void* pInstance);

//锁定视图回调函数
typedef void( *LockViewCB) (bool bLock, void* pInstance);
//#endif
//Tobject:调用对象的类型，Tparam回调函数参数的类型
template<typename Tfun>
struct  AnimationDelegate
{
	AnimationDelegate(void *pInstance, Tfun pFun)
	{
		m_pInstance = pInstance;
		m_pCbFun = pFun;
	}

	Tfun       m_pCbFun;     //回调函数指针  
	void*   m_pInstance; //调用对象  
};

class SA_API CAnimationCallBack
{
public:
	CAnimationCallBack();
	~CAnimationCallBack();
private:
	//更新状态的回调函数
	std::vector<AnimationDelegate<UpdateViewCB> > m_vecUpdateViewCB;
	//开始播放回调函数
	std::vector<AnimationDelegate<PlayBeginCB> > m_vecPlayBeginCB;
	//结束播放回调函数
	std::vector<AnimationDelegate<PlayEndCB> > m_vecPlayEndCB;

	//锁定视图回调函数
	std::vector<AnimationDelegate<LockViewCB> > m_vecLockViewCB;
	
	//设置对象状态回调函数
	std::vector<AnimationDelegate<SetTargetStateCB> > m_vecSetTargetStateCB;

	//获取配置矩阵回调函数
	std::vector<AnimationDelegate<GetModelPlcMtxCB> > m_vecGetModelPlcMtxCB;

	//设置配置矩阵回调函数
	std::vector<AnimationDelegate<SetModelPlcMtxCB> > m_vecSetModelPlcMtxCB;

	//获取摄像机状态回调函数
	std::vector<AnimationDelegate<GetCameraCB> > m_vecGetCameraCB;

	//设置摄像机状态回调函数
	std::vector<AnimationDelegate<SetCameraCB> > m_vecSetCameraCB;

	//获取摄像机目标点距离回调函数
	std::vector<AnimationDelegate<GetCameraFocalCB> > m_vecGetCameraFocalCB;

	//获取摄像机目标点回调函数
	std::vector<AnimationDelegate<GetCameraTargetPntCB> > m_vecGetCameraTargetPntCB;

	//设置显示隐藏回调函数
	std::vector<AnimationDelegate<PlayVisibleCB> > m_vecPlayVisibleCB;

	//设置颜色回调函数
	std::vector<AnimationDelegate<PlayColorCB> > m_vecPlayColorCB;

	//播放图片回调函数
	std::vector<AnimationDelegate<PlayImageCB> > m_vecPlayImageCB;

	//播放剖面回调函数
	std::vector<AnimationDelegate<PlayClipPlaneCB> > m_vecPlayClipPlaneCB;
	
	//工具回调函数
	std::vector<AnimationDelegate<PlayToolCB> > m_vecPlayToolCB;
public:
	/*****************************************************************
	函数名	：AddUpdateViewCB
	功能	：添加更新视图回调函数
	参数	：
	UpdateViewCB UpdateViewCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-03-29 qinyp
	*****************************************************************/
	void AddUpdateViewCB(UpdateViewCB pUpdateViewCB,void* pInstance );
	/*****************************************************************
	函数名	：RemoveUpdateViewCB
	功能	：删除更新界面回调函数
	参数	：
	UpdateViewCB UpdateViewCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-03-29 qinyp
	*****************************************************************/
	void RemoveUpdateViewCB(UpdateViewCB pUpdateViewCB, void* pInstance );
	/*****************************************************************
	函数名	：InvokeUpdateViewCB
	功能	：执行更新界面回调函数
	参数	：
	pBehaviorAction，段动画
	返回值	：void
	更新历史：
	创建：2018-03-29 qinyp
	*****************************************************************/
	void InvokeUpdateViewCB(void* pBehaviorAction);
	/*****************************************************************
	函数名	：AddPlayBeginCB
	功能	：添加动画开始播放回调函数
	参数	：
	PlayBeginCB PlayBeginCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-03-29 qinyp
	*****************************************************************/
	void AddPlayBeginCB(PlayBeginCB pPlayBeginCB, void* pInstance );
	/*****************************************************************
	函数名	：RemovePlayBeginCB
	功能	：删除动画开始播放回调函数
	参数	：
	PlayBeginCB PlayBeginCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-03-29 qinyp
	*****************************************************************/
	void RemovePlayBeginCB(PlayBeginCB pPlayBeginCB, void* pInstance );
	/*****************************************************************
	函数名	：InvokePlayBeginCB
	功能	：执行开始播放动画回调函数
	参数	：
	
	返回值	：void
	更新历史：
	创建：2018-03-29 qinyp
	*****************************************************************/
	void InvokePlayBeginCB(void* pBehaviorAction);
	
	//添加结束播放回调函数
	void AddPlayEndCB(PlayEndCB pPlayEndCB, void* pInstance );
	//删除结束播放回调回调函数
	void RemovePlayEndCB(PlayEndCB pPlayEndCB, void* pInstance );
	//调用结束播放回调回调函数
	void InvokePlayEndCB(void* pBehaviorAction);

	//添加锁定视图回调函数
	void AddLockViewCB(LockViewCB pLockViewCB, void* pInstance );
	//删除锁定视图回调回调函数
	void RemoveLockViewCB(LockViewCB pLockViewCB, void* pInstance );
	//调用锁定视图回调回调函数
	void InvokeLockViewCB(bool bLock);
	
	
	/*****************************************************************
	函数名	：AddSetTargetStateCB
	功能	：添加设置视口各对象状态回调函数
	参数	：
	SetTargetStateCB pSetTargetStateCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-04-05 qinyp
	*****************************************************************/
	void AddSetTargetStateCB(SetTargetStateCB pSetTargetStateCB, void* pInstance );
	/*****************************************************************
	函数名	：RemoveSetTargetStateCB
	功能	：删除设置视口各对象状态回调函数
	参数	：
	SetTargetStateCB pSetTargetStateCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-04-05 qinyp
	*****************************************************************/
	void RemoveSetTargetStateCB(SetTargetStateCB pSetTargetStateCB, void* pInstance );
	/*****************************************************************
	函数名	：InvokeSetTargetStateCB
	功能	：执行设置视口各对象状态回调函数
	参数	：
	p，段动画
	返回值	：void
	更新历史：
	创建：2018-04-05 qinyp
	*****************************************************************/
	void InvokeSetTargetStateCB(void* pTargetList);
	
	//添加获取配置矩阵回调函数
	void AddGetModelPlcMtxCB(GetModelPlcMtxCB pGetModelPlcMtxCB, void* pInstance );
	//删除获取配置矩阵回调函数
	void RemoveGetModelPlcMtxCB(GetModelPlcMtxCB pGetModelPlcMtxCB, void* pInstance );
	//调用获取配置矩阵回调函数
	void InvokeGetModelPlcMtxCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],float matrix[4][4]);

	//添加设置配置矩阵回调函数
	void AddSetModelPlcMtxCB(SetModelPlcMtxCB pSetModelPlcMtxCB, void* pInstance );
	//删除设置配置矩阵回调函数
	void RemoveSetModelPlcMtxCB(SetModelPlcMtxCB pSetModelPlcMtxCB, void* pInstance );
	//调用设置配置矩阵回调函数
	void InvokeSetModelPlcMtxCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],const float matrix[4][4]);

	//添加获取摄像机状态回调函数
	void AddGetCameraCB(GetCameraCB pGetCameraCB, void* pInstance );
	//删除获取摄像机状态回调函数
	void RemoveGetCameraCB(GetCameraCB pGetCameraCB, void* pInstance );
	//调用获取摄像机状态回调函数
	void InvokeGetCameraCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],float scale[3],float matrix[4][4]);

	//添加设置摄像机状态回调函数
	void AddSetCameraCB(SetCameraCB pSetCameraCB, void* pInstance );
	//删除设置摄像机状态回调函数
	void RemoveSetCameraCB(SetCameraCB pSetCameraCB, void* pInstance );
	//调用设置摄像机状态回调函数
	void InvokeSetCameraCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],const float scale[3],const float matrix[4][4]);

	//添加获取摄像机目标点距离回调函数
	void AddGetCameraFocalCB(GetCameraFocalCB pGetCameraFocalCB, void* pInstance );
	//删除获取摄像机目标点距离回调函数
	void RemoveGetCameraFocalCB(GetCameraFocalCB pGetCameraFocalCB, void* pInstance);
	//调用获取摄像机目标点距离回调函数
	void InvokeGetCameraFocalCB( float& fPosTargetDistance);

	//添加获取摄像机目标点回调函数
	void AddGetCameraTargetPntCB(GetCameraTargetPntCB pGetCameraTargetPntCB, void* pInstance);
	//删除获取摄像机目标点回调函数
	void RemoveGetCameraTargetPntCB(GetCameraTargetPntCB pGetCameraTargetPntCB, void* pInstance);
	//调用获取摄像机目标点回调函数
	void InvokeGetCameraTargetPntCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],float targetPnt[3]);

	//添加设置显示隐藏回调函数
	void AddPlayVisibleCB(PlayVisibleCB pPlayVisibleCB, void* pInstance);
	//删除设置显示隐藏回调函数
	void RemovePlayVisibleCB(PlayVisibleCB pPlayVisibleCB, void* pInstance);
	//调用设置显示隐藏回调函数
	void InvokePlayVisibleCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], bool bOnOff, float fTran);

	//添加设置颜色回调函数
	void AddPlayColorCB(PlayColorCB pPlayColorCB, void* pInstance);
	//删除设置颜色回调函数
	void RemovePlayColorCB(PlayColorCB pPlayColorCB, void* pInstance);
	//调用设置颜色回调函数
	void InvokePlayColorCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fColor[3]);

	//添加图片播放回调函数
	void AddPlayImageCB(PlayImageCB pPlayImageCB, void* pInstance);
	//删除图片播放回调函数
	void RemovePlayImageCB(PlayImageCB pPlayImageCB, void* pInstance);
	//调用图片播放回调函数
	void InvokePlayImageCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fPos[3], const float fScale[3], bool bOnOffFlg);

	//添加剖面播放回调函数
	void AddPlayClipPlaneCB(PlayClipPlaneCB pPlayClipPlaneCB, void* pInstance);
	//删除剖面播放回调函数
	void RemovePlayClipPlaneCB(PlayClipPlaneCB pPlayClipPlaneCB, void* pInstance);
	//调用剖面播放回调函数
	void InvokePlayClipPlaneCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fPos[3], const float fScale[3], bool bOnOffFlg);
	
	//添加工具播放回调函数
	void AddPlayToolCB(PlayToolCB pPlayToolCB, void* pInstance);
	//删除工具播放回调函数
	void RemovePlayToolCB(PlayToolCB pPlayToolCB, void* pInstance);
	//调用工具播放回调函数
	void InvokePlayToolCB(int nType, const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char strToolPath[SA_BUFFER_SIZE_SMALL], const char strParentPath[SA_BUFFER_SIZE_SMALL]);
	
};

#endif //_H_ANIMATIONCALLBACK_H_
