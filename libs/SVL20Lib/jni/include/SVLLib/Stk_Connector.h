// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_Connector.h
*
*	@brief	������
*
*	@par	��ʷ:
*
****************************************************************************/
#ifndef _SVLLIB_CONNECTOR_H_
#define _SVLLIB_CONNECTOR_H_
#pragma once
#include "Stk_Object.h"
namespace HoteamSoft {
	namespace SVLLib {

		typedef struct {
			STK_ID					idEntity;
			StkEntityTypeEnum		nuEntityType;
			STK_PNT32				pntPick;		// Ϊ������Ӧ�ò��ṩ��Pick����
			STK_DIR32				dirPick;		// Ϊ������Ӧ�ò��ṩ��Pick��������
		}SubEntity;

		class STK_TOOLKIT_EXPORT Stk_Connector : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Connector)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CONNECTOR };	//!< ���
		public:
			Stk_Connector();
			virtual ~Stk_Connector();
		public:
			/****************************************************************************
			*	@brief		����Entity����
			*	@param[in]	i_ePointingEnt			ָ������
			*	@param[in]	i_ePointedEnt			��ָ������
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void						SetEntityType(StkEntityTypeEnum i_ePointingEnt, StkEntityTypeEnum i_ePointedEnt);

			/****************************************************************************
			*	@brief		��ȡEntity����
			*	@param[in]	��
			*	@param[out]	o_ePointingEnt			ָ������
			*	@param[out]	o_ePointedEnt			��ָ������
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void						GetEntityType(StkEntityTypeEnum& o_ePointingEnt, StkEntityTypeEnum& o_ePointedEnt);

			/****************************************************************************
			*	@brief		����Proto Link
			*	@param[in]	i_wstrPlcPath			����·��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void						SetLinkProto(const wstring &i_wstrPlcPath);

			/****************************************************************************
			*	@brief		��ȡProto Link
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_wstrPlcPath	����·��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			wstring						GetLinkProto();

			/****************************************************************************
			*	@brief		�ж��Ƿ���Proto Link
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_bIsLinkProto	����·��״̬
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL					IsLinkProto();

			/****************************************************************************
			*	@brief		�����Ƿ��Ƿ�ָ������ProtoType
			*	@param[in]	i_bLinkProto 
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void                       SetIsLinkProto(STK_BOOL i_bLinkProto);

			/****************************************************************************
			*	@brief		����NodeConnector Link
			*	@param[in]	i_idNode		Node ID
			*	@param[out]	��
			*	@retval		��
			*	@note		����PMI��ͼ����Ϣ����Node��
			*	@attention	��
			****************************************************************************/
			void						SetLinkNode(STK_ID i_idNode);

			/****************************************************************************
			*	@brief		�����Ƿ�Connector Link
			*	@param[in]	i_bLinkNode		�Ƿ�����
			*	@param[out]	��
			*	@retval		��
			*	@note		
			*	@attention	��
			****************************************************************************/
			void                        SetIsLinkNode(STK_BOOL i_bLinkNode);

			/****************************************************************************
			*	@brief		��ȡNodeConnector Link
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_idNode		Node ID
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_ID						GetLinkNode();

			/****************************************************************************
			*	@brief		�ж��Ƿ���Node Link
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_bIsLinkNode	Node Link״̬
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL					IsLinkNode();

			/****************************************************************************
			*	@brief		������Entity
			*	@param[in]	i_SubEntity		��Entity
			*	@param[out]	��
			*	@retval		��
			*	@note		����ߡ��桢ͼ���
			*	@attention	��
			****************************************************************************/
			void						AddSubEntity(SubEntity i_SubEntity);

			/****************************************************************************
			*	@brief		��ȡ��Entity
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vcEntities	������Entity
			*	@note		��
			*	@attention	��
			****************************************************************************/
			vector<SubEntity>			GetSubEntity();

			/****************************************************************************
			*	@brief		�ж��Ƿ�����Entity Link
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_bIsLinkEntities	��Entity Link״̬
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL					IsLinkEntities();

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
		};
	}
}
#endif //_SVLLIB_CONNECTOR_H_