#pragma once
/*******************************************************************************
*	@file
*		CAnimationCallBack.h
*	@brief
*		CAnimationCallBack �������Żص�������Ľӿ�����
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
////������ͼ�ص�����
//typedef void(_stdcall *UpdateViewCB) (void* pBehaviorAction, void* pInstance);
////��ʼ���Żص�����
//typedef void(_stdcall *PlayBeginCB) (void* pBehaviorAction, void* pInstance);
////���Ž����ص�����
//typedef void(_stdcall *PlayEndCB) (void* pBehaviorAction, void* pInstance);
//
////�����ӿ��ڸ������״̬
//typedef void(_stdcall *SetTargetStateCB) (void* targetList, void* pInstance);
//
////������ͼ�ص�����
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

//������ͼ�ص�����
typedef void( *UpdateViewCB) (void* pBehaviorAction, void* pInstance);
//��ʼ���Żص�����
typedef void( *PlayBeginCB) (void* pBehaviorAction, void* pInstance);
//���Ž����ص�����
typedef void( *PlayEndCB) (void* pBehaviorAction, void* pInstance);

//�����ӿ��ڸ������״̬
typedef void( *SetTargetStateCB) (void* targetList, void* pInstance);

//������ͼ�ص�����
typedef void( *LockViewCB) (bool bLock, void* pInstance);
//#endif
//Tobject:���ö�������ͣ�Tparam�ص���������������
template<typename Tfun>
struct  AnimationDelegate
{
	AnimationDelegate(void *pInstance, Tfun pFun)
	{
		m_pInstance = pInstance;
		m_pCbFun = pFun;
	}

	Tfun       m_pCbFun;     //�ص�����ָ��  
	void*   m_pInstance; //���ö���  
};

class SA_API CAnimationCallBack
{
public:
	CAnimationCallBack();
	~CAnimationCallBack();
private:
	//����״̬�Ļص�����
	std::vector<AnimationDelegate<UpdateViewCB> > m_vecUpdateViewCB;
	//��ʼ���Żص�����
	std::vector<AnimationDelegate<PlayBeginCB> > m_vecPlayBeginCB;
	//�������Żص�����
	std::vector<AnimationDelegate<PlayEndCB> > m_vecPlayEndCB;

	//������ͼ�ص�����
	std::vector<AnimationDelegate<LockViewCB> > m_vecLockViewCB;
	
	//���ö���״̬�ص�����
	std::vector<AnimationDelegate<SetTargetStateCB> > m_vecSetTargetStateCB;

	//��ȡ���þ���ص�����
	std::vector<AnimationDelegate<GetModelPlcMtxCB> > m_vecGetModelPlcMtxCB;

	//�������þ���ص�����
	std::vector<AnimationDelegate<SetModelPlcMtxCB> > m_vecSetModelPlcMtxCB;

	//��ȡ�����״̬�ص�����
	std::vector<AnimationDelegate<GetCameraCB> > m_vecGetCameraCB;

	//���������״̬�ص�����
	std::vector<AnimationDelegate<SetCameraCB> > m_vecSetCameraCB;

	//��ȡ�����Ŀ������ص�����
	std::vector<AnimationDelegate<GetCameraFocalCB> > m_vecGetCameraFocalCB;

	//��ȡ�����Ŀ���ص�����
	std::vector<AnimationDelegate<GetCameraTargetPntCB> > m_vecGetCameraTargetPntCB;

	//������ʾ���ػص�����
	std::vector<AnimationDelegate<PlayVisibleCB> > m_vecPlayVisibleCB;

	//������ɫ�ص�����
	std::vector<AnimationDelegate<PlayColorCB> > m_vecPlayColorCB;

	//����ͼƬ�ص�����
	std::vector<AnimationDelegate<PlayImageCB> > m_vecPlayImageCB;

	//��������ص�����
	std::vector<AnimationDelegate<PlayClipPlaneCB> > m_vecPlayClipPlaneCB;
	
	//���߻ص�����
	std::vector<AnimationDelegate<PlayToolCB> > m_vecPlayToolCB;
public:
	/*****************************************************************
	������	��AddUpdateViewCB
	����	����Ӹ�����ͼ�ص�����
	����	��
	UpdateViewCB UpdateViewCB���ص�����ָ��
	����ֵ	��void
	������ʷ��
	������2018-03-29 qinyp
	*****************************************************************/
	void AddUpdateViewCB(UpdateViewCB pUpdateViewCB,void* pInstance );
	/*****************************************************************
	������	��RemoveUpdateViewCB
	����	��ɾ�����½���ص�����
	����	��
	UpdateViewCB UpdateViewCB���ص�����ָ��
	����ֵ	��void
	������ʷ��
	������2018-03-29 qinyp
	*****************************************************************/
	void RemoveUpdateViewCB(UpdateViewCB pUpdateViewCB, void* pInstance );
	/*****************************************************************
	������	��InvokeUpdateViewCB
	����	��ִ�и��½���ص�����
	����	��
	pBehaviorAction���ζ���
	����ֵ	��void
	������ʷ��
	������2018-03-29 qinyp
	*****************************************************************/
	void InvokeUpdateViewCB(void* pBehaviorAction);
	/*****************************************************************
	������	��AddPlayBeginCB
	����	����Ӷ�����ʼ���Żص�����
	����	��
	PlayBeginCB PlayBeginCB���ص�����ָ��
	����ֵ	��void
	������ʷ��
	������2018-03-29 qinyp
	*****************************************************************/
	void AddPlayBeginCB(PlayBeginCB pPlayBeginCB, void* pInstance );
	/*****************************************************************
	������	��RemovePlayBeginCB
	����	��ɾ��������ʼ���Żص�����
	����	��
	PlayBeginCB PlayBeginCB���ص�����ָ��
	����ֵ	��void
	������ʷ��
	������2018-03-29 qinyp
	*****************************************************************/
	void RemovePlayBeginCB(PlayBeginCB pPlayBeginCB, void* pInstance );
	/*****************************************************************
	������	��InvokePlayBeginCB
	����	��ִ�п�ʼ���Ŷ����ص�����
	����	��
	
	����ֵ	��void
	������ʷ��
	������2018-03-29 qinyp
	*****************************************************************/
	void InvokePlayBeginCB(void* pBehaviorAction);
	
	//��ӽ������Żص�����
	void AddPlayEndCB(PlayEndCB pPlayEndCB, void* pInstance );
	//ɾ���������Żص��ص�����
	void RemovePlayEndCB(PlayEndCB pPlayEndCB, void* pInstance );
	//���ý������Żص��ص�����
	void InvokePlayEndCB(void* pBehaviorAction);

	//���������ͼ�ص�����
	void AddLockViewCB(LockViewCB pLockViewCB, void* pInstance );
	//ɾ��������ͼ�ص��ص�����
	void RemoveLockViewCB(LockViewCB pLockViewCB, void* pInstance );
	//����������ͼ�ص��ص�����
	void InvokeLockViewCB(bool bLock);
	
	
	/*****************************************************************
	������	��AddSetTargetStateCB
	����	����������ӿڸ�����״̬�ص�����
	����	��
	SetTargetStateCB pSetTargetStateCB���ص�����ָ��
	����ֵ	��void
	������ʷ��
	������2018-04-05 qinyp
	*****************************************************************/
	void AddSetTargetStateCB(SetTargetStateCB pSetTargetStateCB, void* pInstance );
	/*****************************************************************
	������	��RemoveSetTargetStateCB
	����	��ɾ�������ӿڸ�����״̬�ص�����
	����	��
	SetTargetStateCB pSetTargetStateCB���ص�����ָ��
	����ֵ	��void
	������ʷ��
	������2018-04-05 qinyp
	*****************************************************************/
	void RemoveSetTargetStateCB(SetTargetStateCB pSetTargetStateCB, void* pInstance );
	/*****************************************************************
	������	��InvokeSetTargetStateCB
	����	��ִ�������ӿڸ�����״̬�ص�����
	����	��
	p���ζ���
	����ֵ	��void
	������ʷ��
	������2018-04-05 qinyp
	*****************************************************************/
	void InvokeSetTargetStateCB(void* pTargetList);
	
	//��ӻ�ȡ���þ���ص�����
	void AddGetModelPlcMtxCB(GetModelPlcMtxCB pGetModelPlcMtxCB, void* pInstance );
	//ɾ����ȡ���þ���ص�����
	void RemoveGetModelPlcMtxCB(GetModelPlcMtxCB pGetModelPlcMtxCB, void* pInstance );
	//���û�ȡ���þ���ص�����
	void InvokeGetModelPlcMtxCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],float matrix[4][4]);

	//����������þ���ص�����
	void AddSetModelPlcMtxCB(SetModelPlcMtxCB pSetModelPlcMtxCB, void* pInstance );
	//ɾ���������þ���ص�����
	void RemoveSetModelPlcMtxCB(SetModelPlcMtxCB pSetModelPlcMtxCB, void* pInstance );
	//�����������þ���ص�����
	void InvokeSetModelPlcMtxCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],const float matrix[4][4]);

	//��ӻ�ȡ�����״̬�ص�����
	void AddGetCameraCB(GetCameraCB pGetCameraCB, void* pInstance );
	//ɾ����ȡ�����״̬�ص�����
	void RemoveGetCameraCB(GetCameraCB pGetCameraCB, void* pInstance );
	//���û�ȡ�����״̬�ص�����
	void InvokeGetCameraCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],float scale[3],float matrix[4][4]);

	//������������״̬�ص�����
	void AddSetCameraCB(SetCameraCB pSetCameraCB, void* pInstance );
	//ɾ�����������״̬�ص�����
	void RemoveSetCameraCB(SetCameraCB pSetCameraCB, void* pInstance );
	//�������������״̬�ص�����
	void InvokeSetCameraCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],const float scale[3],const float matrix[4][4]);

	//��ӻ�ȡ�����Ŀ������ص�����
	void AddGetCameraFocalCB(GetCameraFocalCB pGetCameraFocalCB, void* pInstance );
	//ɾ����ȡ�����Ŀ������ص�����
	void RemoveGetCameraFocalCB(GetCameraFocalCB pGetCameraFocalCB, void* pInstance);
	//���û�ȡ�����Ŀ������ص�����
	void InvokeGetCameraFocalCB( float& fPosTargetDistance);

	//��ӻ�ȡ�����Ŀ���ص�����
	void AddGetCameraTargetPntCB(GetCameraTargetPntCB pGetCameraTargetPntCB, void* pInstance);
	//ɾ����ȡ�����Ŀ���ص�����
	void RemoveGetCameraTargetPntCB(GetCameraTargetPntCB pGetCameraTargetPntCB, void* pInstance);
	//���û�ȡ�����Ŀ���ص�����
	void InvokeGetCameraTargetPntCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],float targetPnt[3]);

	//���������ʾ���ػص�����
	void AddPlayVisibleCB(PlayVisibleCB pPlayVisibleCB, void* pInstance);
	//ɾ��������ʾ���ػص�����
	void RemovePlayVisibleCB(PlayVisibleCB pPlayVisibleCB, void* pInstance);
	//����������ʾ���ػص�����
	void InvokePlayVisibleCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], bool bOnOff, float fTran);

	//���������ɫ�ص�����
	void AddPlayColorCB(PlayColorCB pPlayColorCB, void* pInstance);
	//ɾ��������ɫ�ص�����
	void RemovePlayColorCB(PlayColorCB pPlayColorCB, void* pInstance);
	//����������ɫ�ص�����
	void InvokePlayColorCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fColor[3]);

	//���ͼƬ���Żص�����
	void AddPlayImageCB(PlayImageCB pPlayImageCB, void* pInstance);
	//ɾ��ͼƬ���Żص�����
	void RemovePlayImageCB(PlayImageCB pPlayImageCB, void* pInstance);
	//����ͼƬ���Żص�����
	void InvokePlayImageCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fPos[3], const float fScale[3], bool bOnOffFlg);

	//������沥�Żص�����
	void AddPlayClipPlaneCB(PlayClipPlaneCB pPlayClipPlaneCB, void* pInstance);
	//ɾ�����沥�Żص�����
	void RemovePlayClipPlaneCB(PlayClipPlaneCB pPlayClipPlaneCB, void* pInstance);
	//�������沥�Żص�����
	void InvokePlayClipPlaneCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fPos[3], const float fScale[3], bool bOnOffFlg);
	
	//��ӹ��߲��Żص�����
	void AddPlayToolCB(PlayToolCB pPlayToolCB, void* pInstance);
	//ɾ�����߲��Żص�����
	void RemovePlayToolCB(PlayToolCB pPlayToolCB, void* pInstance);
	//���ù��߲��Żص�����
	void InvokePlayToolCB(int nType, const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char strToolPath[SA_BUFFER_SIZE_SMALL], const char strParentPath[SA_BUFFER_SIZE_SMALL]);
	
};

#endif //_H_ANIMATIONCALLBACK_H_
