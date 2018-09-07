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
#include 	"m3d/M3D.h"

#ifdef WIN32
//更新视图回调函数
typedef void(_stdcall UpdateViewCB) (void* pBehaviorAction);
//开始播放回调函数
typedef void(_stdcall PlayBeginCB) ();
//设置视口内各对象的状态
typedef void(_stdcall SetTargetStateCB) (void* targetList);
#else
//更新视图回调函数
typedef void( UpdateViewCB) (void* pBehaviorAction);
//开始播放回调函数
typedef void( PlayBeginCB) ();
//设置视口内各对象的状态
typedef void( SetTargetStateCB) (void* targetList);
#endif
class M3D_API CAnimationCallBack
{
public:
	CAnimationCallBack();
	~CAnimationCallBack();
private:
	std::vector<UpdateViewCB*> m_vecUpdateViewCB;
	std::vector<PlayBeginCB*> m_vecPlayBeginCB;
	std::vector<SetTargetStateCB*> m_vecSetTargetStateCB;
public:
	/*****************************************************************
	函数名	：AddUpdateViewCB
	功能	：添加更新视图回调函数
	参数	：
	UpdateViewCB* UpdateViewCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-03-29 qinyp
	*****************************************************************/
	void AddUpdateViewCB(UpdateViewCB* pUpdateViewCB);
	/*****************************************************************
	函数名	：RemoveUpdateViewCB
	功能	：删除更新界面回调函数
	参数	：
	UpdateViewCB* UpdateViewCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-03-29 qinyp
	*****************************************************************/
	void RemoveUpdateViewCB(UpdateViewCB* pUpdateViewCB);
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
	PlayBeginCB* PlayBeginCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-03-29 qinyp
	*****************************************************************/
	void AddPlayBeginCB(PlayBeginCB* pPlayBeginCB);
	/*****************************************************************
	函数名	：RemovePlayBeginCB
	功能	：删除动画开始播放回调函数
	参数	：
	PlayBeginCB* PlayBeginCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-03-29 qinyp
	*****************************************************************/
	void RemovePlayBeginCB(PlayBeginCB* pPlayBeginCB);
	/*****************************************************************
	函数名	：InvokePlayBeginCB
	功能	：执行开始播放动画回调函数
	参数	：
	
	返回值	：void
	更新历史：
	创建：2018-03-29 qinyp
	*****************************************************************/
	void InvokePlayBeginCB();
	/*****************************************************************
	函数名	：AddSetTargetStateCB
	功能	：添加设置视口各对象状态回调函数
	参数	：
	SetTargetStateCB* pSetTargetStateCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-04-05 qinyp
	*****************************************************************/
	void AddSetTargetStateCB(SetTargetStateCB* pSetTargetStateCB);
	/*****************************************************************
	函数名	：RemoveSetTargetStateCB
	功能	：删除设置视口各对象状态回调函数
	参数	：
	SetTargetStateCB* pSetTargetStateCB，回调函数指针
	返回值	：void
	更新历史：
	创建：2018-04-05 qinyp
	*****************************************************************/
	void RemoveSetTargetStateCB(SetTargetStateCB* pSetTargetStateCB);
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
};

#endif //_H_ANIMATIONCALLBACK_H_