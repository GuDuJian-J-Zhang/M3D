// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		SoExam_Configuration.h
 *
 *	@brief	    动画功能调用接口类
 *
 *	@par	历史:
 *		2016/02/24 created by qinyp
****************************************************************************/
#ifdef _MSC_VER

#ifdef SA_EXPORTS
#    define SA_API  __declspec (dllexport)
#else
#    define SA_API  __declspec (dllimport)
#endif
#else
#define SA_API
#endif

#ifndef _H_ANIMATIONIF_H
#define _H_ANIMATIONIF_H
#pragma once
class CAnimationIFP;
class SA_API CAnimationIF
{
public:
	CAnimationIF(void);
	~CAnimationIF(void);
private:
	CAnimationIFP* m_pImpl;
public:
	/*****************************************************************//**
	*	@brief	  播放指定ID的动画
	*  @param[in] nAniID: 指定ID
	*  @param[in] bUpdateModel: 是否更新模型的状态
	*  @param[in] bUpdateCam: 是否更新场景的状态
	*  @param[in] bCamAni: 是否使用更新场景动画
	*  @reval 是否成功播放 true:成功播放
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	bool PlaySingleAnimation(int nAniID, bool bUpdateModel = false, bool bUpdateCam = false, bool bCamAni = false);
	/*****************************************************************//**
	*	@brief	  停止播放动画
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	void Stop();
	/*****************************************************************//**
	*	@brief	  继续播放动画
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	void Continue();
	/*****************************************************************//**
	*	@brief	  暂停播放动画
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	void Pause();
	/*****************************************************************//**
	*	@brief	  单步播放指定ID过程下的指定ID步骤的动画
	*  @param[in] nProManagerID: 指定过程ID
	*  @param[in] nProcessID:指定步骤ID
	*  @param[in] bReverse:正反播 true:正播
	*  @param[in] bChangeProcess:是否是切换过程
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	void PlaySingleProcess(int nProManagerID, int nProcessID, bool bReverse, bool bChangeProcess = false);
	/*****************************************************************//**
	*	@brief	  播放指定ID过程下的全部动画
	*  @param[in] nProManagerID: 指定过程ID
	*  @param[in] bReverse:正反播 true:正播
	*  @param[in] bChangeProcess:是否是切换过程 true:切换过程
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	void PlaySingleProcessManager(int nProManagerID, bool bReverse, bool bChangeProcess = false);
	/*****************************************************************//**
	*	@brief	  播放全部动画
	*  @param[in] bReverse:正反播 true:正播
	*  @param[in] bChangeProcess:是否是切换过程 true:切换过程
  	*  @Creator qinyp 2016.02.25
	********************************************************************/
	void PlayAllProcessManager(bool bReverse, bool bChangeProcess);
	/*****************************************************************//**
	*	@brief	  从指定过程的指定步骤开始播放
	*  @param[in] nProManagerID: 指定过程ID
	*  @param[in] nProcessID:指定步骤
	*  @param[in] bReverse:正反播 true:正播
	*  @param[in] bChangeProcess:是否是切换过程 true:切换过程
  	*  @Creator qinyp 2016.02.25
	********************************************************************/
	void PlayFromCurrentProcess(int nProManagerID, int nProcessID, bool bReverse, bool bChangeProcess = false);
	/*****************************************************************//**
	*	@brief	  从指定过程开始播放
	*  @param[in] nProManagerID: 指定过程ID
	*  @param[in] bReverse:正反播 true:正播
	*  @param[in] bChangeProcess:是否是切换过程 true:切换过程
  	*  @Creator qinyp 2016.02.25
	********************************************************************/
	void PlayFromCurrentProcessManager(int nProManagerID, bool bReserve, bool bChangeProcess = false);
	/*****************************************************************//**
	*	@brief	  复位指定动画
	*  @param[in] nAniID: 指定ID
	*  @Creator qinyp 2016.04.01
	********************************************************************/
	void RewindAnimationByID(int nAniID);
};
#endif