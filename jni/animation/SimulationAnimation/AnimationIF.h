// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		SoExam_Configuration.h
 *
 *	@brief	    �������ܵ��ýӿ���
 *
 *	@par	��ʷ:
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
	*	@brief	  ����ָ��ID�Ķ���
	*  @param[in] nAniID: ָ��ID
	*  @param[in] bUpdateModel: �Ƿ����ģ�͵�״̬
	*  @param[in] bUpdateCam: �Ƿ���³�����״̬
	*  @param[in] bCamAni: �Ƿ�ʹ�ø��³�������
	*  @reval �Ƿ�ɹ����� true:�ɹ�����
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	bool PlaySingleAnimation(int nAniID, bool bUpdateModel = false, bool bUpdateCam = false, bool bCamAni = false);
	/*****************************************************************//**
	*	@brief	  ֹͣ���Ŷ���
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	void Stop();
	/*****************************************************************//**
	*	@brief	  �������Ŷ���
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	void Continue();
	/*****************************************************************//**
	*	@brief	  ��ͣ���Ŷ���
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	void Pause();
	/*****************************************************************//**
	*	@brief	  ��������ָ��ID�����µ�ָ��ID����Ķ���
	*  @param[in] nProManagerID: ָ������ID
	*  @param[in] nProcessID:ָ������ID
	*  @param[in] bReverse:������ true:����
	*  @param[in] bChangeProcess:�Ƿ����л�����
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	void PlaySingleProcess(int nProManagerID, int nProcessID, bool bReverse, bool bChangeProcess = false);
	/*****************************************************************//**
	*	@brief	  ����ָ��ID�����µ�ȫ������
	*  @param[in] nProManagerID: ָ������ID
	*  @param[in] bReverse:������ true:����
	*  @param[in] bChangeProcess:�Ƿ����л����� true:�л�����
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	void PlaySingleProcessManager(int nProManagerID, bool bReverse, bool bChangeProcess = false);
	/*****************************************************************//**
	*	@brief	  ����ȫ������
	*  @param[in] bReverse:������ true:����
	*  @param[in] bChangeProcess:�Ƿ����л����� true:�л�����
  	*  @Creator qinyp 2016.02.25
	********************************************************************/
	void PlayAllProcessManager(bool bReverse, bool bChangeProcess);
	/*****************************************************************//**
	*	@brief	  ��ָ�����̵�ָ�����迪ʼ����
	*  @param[in] nProManagerID: ָ������ID
	*  @param[in] nProcessID:ָ������
	*  @param[in] bReverse:������ true:����
	*  @param[in] bChangeProcess:�Ƿ����л����� true:�л�����
  	*  @Creator qinyp 2016.02.25
	********************************************************************/
	void PlayFromCurrentProcess(int nProManagerID, int nProcessID, bool bReverse, bool bChangeProcess = false);
	/*****************************************************************//**
	*	@brief	  ��ָ�����̿�ʼ����
	*  @param[in] nProManagerID: ָ������ID
	*  @param[in] bReverse:������ true:����
	*  @param[in] bChangeProcess:�Ƿ����л����� true:�л�����
  	*  @Creator qinyp 2016.02.25
	********************************************************************/
	void PlayFromCurrentProcessManager(int nProManagerID, bool bReserve, bool bChangeProcess = false);
	/*****************************************************************//**
	*	@brief	  ��λָ������
	*  @param[in] nAniID: ָ��ID
	*  @Creator qinyp 2016.04.01
	********************************************************************/
	void RewindAnimationByID(int nAniID);
};
#endif