// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_View.h
*
*	@brief	��ͼ��
*
*	@par	��ʷ:
*		2017/06/10	������qinyp@HOTEAMSOFT
****************************************************************************/


#ifndef _SVLLIB_PMI_H_
#define _SVLLIB_PMI_H_

#pragma once
#include "Stk_Object.h"
#include "Stk_PMIEntity.h"
#include "Stk_CompositeText.h"
#include "Stk_UserData.h"
#include "Stk_Image.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_PMI : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_PMI)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMI };	//!< ���
		public:
			Stk_PMI();
			virtual ~Stk_PMI();
		public:
			void Init(Stk_PMIDocument* i_PMIDocP, int i_iModelID);
		public:
			/****************************************************************************
			*	@brief		����PMIΨһ��ʶ
			*	@param[in]	i_Identity	��ͼΨһ��ʶ
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			//void						SetID(STK_ID i_ID);

			/****************************************************************************
			*	@brief		��ȡPMIΨһ��ʶ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��ͼ��Ψһ��ʶ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			//STK_ID			GetID();

			/****************************************************************************
			*	@brief		����PMI����
			*	@param[in]	i_strName		PMI����
			*	@param[out]	��
			*	@retval		��
			*	@note		Ϊ����Node��Ҫ��������ͬ����ΪҪ����������ʱ��ͬʱ����Node��
			*	@attention	��
			****************************************************************************/
			void						SetName(const wstring &i_strName);

			/****************************************************************************
			*	@brief		��ȡPMI����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_wstrPMIName		PMI����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			wstring						GetName();
			
			/****************************************************************************
			*	@brief		��ȡ����ƽ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_mtxDefPlane		����ƽ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			const STK_MTX32&			GetDefPlane();

			/****************************************************************************
			*	@brief		���ö���ƽ��
			*	@param[in]	i_mtxDefPlane		����ƽ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetDefPlane(const STK_MTX32& i_mtxDefPlane);
			
			/****************************************************************************
			*	@brief		��ȡPMI��ɫ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_rgbaPMI		PMI��ɫ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			const STK_RGBA32&			GetColor();

			/****************************************************************************
			*	@brief		����PMI��ɫ
			*	@param[in]	i_rgbaPMI		PMI��ɫ
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetColor(const STK_RGBA32& i_rgbaPMI);

			/****************************************************************************
			*	@brief		��ȡPMI����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eType			PMI����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkPMITypeEnum				GetType();

			/****************************************************************************
			*	@brief		����PMI����
			*	@param[in]	i_eType			PMI����
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetType(StkPMITypeEnum i_nuType);

			/****************************************************************************
			*	@brief		��ȡPMI��ʽ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_uiStyle			PMI����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			const STK_UINT32&           GetStyle();

			/****************************************************************************
			*	@brief		����PMI��ʽ
			*	@param[in]	i_uiStyle			PMI����
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS                 SetStyle(const STK_UINT32& i_uiStyle);

			/****************************************************************************
			*	@brief		��ȡʹ�÷�ʽ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_uiUserType			PMI����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			const STK_INT32&           GetUseType();

			/****************************************************************************
			*	@brief		����PMI��ʽ
			*	@param[in]	i_uiUserType			PMI����
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS                 SetUseType(const STK_UINT32& i_uiUseType);

			/****************************************************************************
			*	@brief		����PMI��Դ
			*	@param[in]	i_ePMISource		PMI��Դ
			*	@param[out]	��
			*	@retval		STK_SUCCESS
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetPMISource(StkPMISourceEnum i_ePMISource);

			/****************************************************************************
			*	@brief		��ȡPMI��Դ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_ePMISource	PMI��Դ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkPMISourceEnum			GetPMISource();

			/****************************************************************************
			*	@brief		����PMI��X��������
			*	@param[in]	i_dirX			X��������
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetAxisX(const STK_DIR32& i_dirX);

			/****************************************************************************
			*	@brief		��ȡPMI��X��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		X��������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DIR32&					GetAxisX();

			/****************************************************************************
			*	@brief		����PMI��Y��������
			*	@param[in]	i_dirY			Y��������
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetAxisY(const STK_DIR32& i_dirY);

			/****************************************************************************
			*	@brief		��ȡPMI��Y��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		Y��������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DIR32&					GetAxisY();

			/****************************************************************************
			*	@brief		���÷��Ż�׼��λ��
			*	@param[in]	i_pntSymBase		���Ż�׼��λ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetSymBasePoint(const STK_PNT32& i_pntSymBase);

			/****************************************************************************
			*	@brief		��ȡ���Ż�׼��λ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		���Ż�׼��λ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32&					GetSymBasePoint();

			/****************************************************************************
			*	@brief		�ж�PMI����״̬
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		����״̬
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL					IsDisplay();

			/****************************************************************************
			*	@brief		����PMI����״̬
			*	@param[in]	i_bIsDisplay	����״̬
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void						SetDisplay(STK_BOOL i_bIsDisplay);

			/****************************************************************************
			*	@brief		��ȡ��������������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vcLeaders		����������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			const vector<Stk_LeaderPtr>&	GetLeaders();

			/****************************************************************************
			*	@brief		������������������
			*	@param[in]	i_vcLeaders		����������
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetLeaders(const vector<Stk_LeaderPtr>& i_vcLeaders);

			/****************************************************************************
			*	@brief		���������
			*	@param[in]	i_leaderP		������
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					AddLeader(Stk_LeaderPtr i_leaderP);
			
			/****************************************************************************
			*	@brief		��ȡ�ӳ�����������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vcExLines		�ӳ�������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			const vector<Stk_ExtensionlinePtr>& GetExLines();

			/****************************************************************************
			*	@brief		����ӳ���
			*	@param[in]	i_exlineP �ӳ��߶���
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS             AddExline(Stk_ExtensionlinePtr i_exlineP);

			/****************************************************************************
			*	@brief		�����ӳ�����������
			*	@param[in]	i_vcExLines		�ӳ�������
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetExLines(const vector<Stk_ExtensionlinePtr>& i_vcExLines);

			/****************************************************************************
			*	@brief		��ȡPMI�и����ı���������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vcComTexts	�����ı���������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			const vector<Stk_ComTextPtr>& GetComText();

			/****************************************************************************
			*	@brief		��PMI����Ӹ����ı�����
			*	@param[in]	i_ComTextP		�����ı�
			*	@param[out]	��
			*	@retval		STK_SUCCESS		��ӳɹ�
			*	@retval		STK_ERROR		���ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					AddComText(Stk_ComTextPtr i_ComTextP);

			/****************************************************************************
			*	@brief		��ȡPMI�������������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vcOutFrames	�����������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			const vector<Stk_OutFramePtr>& GetOutFrame();

			/****************************************************************************
			*	@brief		PMI�������������
			*	@param[in]	i_OutFrameP		ָ������ָ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@retval		STK_ERROR		����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetOutFrame(Stk_OutFramePtr i_OutFrameP);

			/****************************************************************************
			*	@brief		��ȡPMI����������������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vcSpecialLines	��������������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			const vector<Stk_SpecialLinePtr>& GetSpecialLines();

			/****************************************************************************
			*	@brief		��PMI���������������
			*	@param[in]	i_SpecLineP		����������
			*	@param[out]	��
			*	@retval		STK_SUCCESS		��ӳɹ�
			*	@retval		STK_ERROR		���ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					AddSpecialLine(Stk_SpecialLinePtr i_SpecLineP);

			/****************************************************************************
			*	@brief		���ͼƬ(ͼ��)
			*	@param[in]	i_stkPictureP		ͼƬ(ͼ��)
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ӳɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					AddImage(Stk_ImagePtr i_stkImageP);

			/****************************************************************************
			*	@brief		��ȡͼƬ(ͼ��)������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		ͼƬ(ͼ��)������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_UINT32					GetImageNum();

			/****************************************************************************
			*	@brief		��ȡָ����ͼƬ(ͼ��)
			*	@param[in]	i_nuIndex			����
			*	@param[out]	��
			*	@retval		ָ����ͼƬ(ͼ��)
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_ImagePtr				GetImageByIndex(STK_UINT32 i_nuIndex);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		�����û�����
			*	@param[in]	i_stkUserDataP		�û�����ָ��
			*	@retval		STK_SUCCESS			�ɹ�
			*	@retval		����				ʧ��
			*	@note		
			*				ͨ�����û��������ơ�+���û�����ID���ķ�ʽ����ΪΨһ��ʶ
			*				  ���硰������+��1�� ��ʾ��һ������ ��������+��2�� ��ʾ�ڶ�������
			*				  ���������õ�Ψһ��ʶ��ͬʱ�����Զ����ǵ�ǰ�ε�����
			*	@attention	��
			****************************************************************************/
			STK_STATUS					AddUserData(Stk_UserDataPtr i_stkUserDataP);

			/****************************************************************************
			*	@brief		ͨ�����ƻ�ȡ�û������б�
			*	@param[in]	�û���������
			*	@param[out]	��
			*	@retval		�û������б�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			map<STK_ID, Stk_UserDataPtr>* GetUserDataByName(wstring i_wstrDataName);

			/****************************************************************************
			*	@brief		ɾ��ָ�����û�����
			*	@param[in]	i_wstrDataName		�û���������
			*	@param[in]	i_id				�û�����ID
			*	@param[out]	��
			*	@retval		STK_SUCCESS			�ҵ����ݣ�ɾ���ɹ�
			*	@retval		STK_ERROR			û���ҵ����ݣ�ɾ��ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					DelUserData(wstring i_wstrDataName, STK_ID i_id);

			/****************************************************************************
			*	@brief		�ж�PMI�����Ƿ����ƽ����Ļ��ʾ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_bIsParallelScreen	�Ƿ�ƽ������Ļ
			*	@note		== true ƽ����Ļ��ʾ <br>
			*				== false ��ƽ����Ļ��ʾ
			*	@attention	��
			****************************************************************************/
			STK_BOOL					IsParallelScreen();

			/****************************************************************************
			*	@brief		����PMI�����Ƿ����ƽ����Ļ��ʾ
			*	@param[in]	i_bIsParallelScreen	�Ƿ�ƽ������Ļ
			*	@param[out]	��
			*	@retval		��
			*	@note		== true ƽ����Ļ��ʾ <br>
			*				== false ��ƽ����Ļ��ʾ
			*	@attention	��
			****************************************************************************/
			void						SetParallelScreen(STK_BOOL i_bIsParallelScreen);

			/****************************************************************************
			*	@brief		�ж�PMI�������Ƿ�̶�λ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_bIsFix		�Ƿ�̶�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL					IsFix();

			/****************************************************************************
			*	@brief		����PMI�������Ƿ�̶�λ��
			*	@param[in]	i_bIsFix		�Ƿ�̶�
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void						SetFix(STK_BOOL i_bIsFix);

			/****************************************************************************
			*	@brief		�Ƿ��Ǿɰ汾��PMI
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		�¾ɰ汾��PMI��־
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL					IsOldVersion();

			/****************************************************************************
			*	@brief		����PMI�¾ɰ汾��־
			*	@param[in]	i_bOldVersion		PMI�¾ɰ汾��־
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetOldVersion(STK_BOOL i_bOldVersion);

			/****************************************************************************
			*	@brief		�Ƿ񱻵�����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_bIsAdjusted	����״̬
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL					IsAdjusted();

			/****************************************************************************
			*	@brief		���õ�����־
			*	@param[in]	i_bIsAdjusted	����״̬
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void						SetAdjusted(STK_BOOL i_bIsAdjusted);

			/****************************************************************************
			*	@brief		�Ƿ�����ǰ����ʾ��PMI
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��ǰ����ʾPMI��ʶ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL					IsFront();

			/****************************************************************************
			*	@brief		�����Ƿ�����ǰ����ʾPMI�ı�ʶ
			*	@param[in]	i_bFront			�Ƿ�����ǰ����ʾPMI�ı�ʶ
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetFront(STK_BOOL i_bFront);
		};
	}
}
#endif
