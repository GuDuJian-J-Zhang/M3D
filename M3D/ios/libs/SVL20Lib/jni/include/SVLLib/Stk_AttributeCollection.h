#pragma once
// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_AttributeDocument
*	@brief	�����ĵ���
*	@par ��ʷ��¼
*				2017/07/03 ������qinyp(Hoteamsoft)
**************************************************************************/

#ifndef _SVLLIB_ATTRIBUTEDOCUMENT_H_
#define _SVLLIB_ATTRIBUTEDOCUMENT_H_
#include "Stk_BaseDocument.h"


namespace HoteamSoft {
	namespace SVLLib {

		class STK_TOOLKIT_EXPORT Stk_AttributeCollection : public Stk_BaseDocument
		{
			STK_DECLARE_CLASS(Stk_AttributeCollection)
		public:
			Stk_AttributeCollection(void);
			virtual ~Stk_AttributeCollection(void);
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_ATTRIBUTEDOCUMENT
			};	//!< ���

		public:
			void            SetVersion(const wstring wstrVersion);
			const wstring&  GetVersion();
			/****************************************************************************
			*	@brief		���ʵ������
			*	@param[in]  i_iOwnerID          ����ID
			*	@param[in]  i_strName           ��������
			*	@param[in]  i_strValue          ����ֵ
			*	@param[out]	��
			*	@retval		STK_STATUS �ɹ����
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS AddInstanceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);
			
			/****************************************************************************
			*	@brief		��ȡʵ������
			*	@param[in]  i_iOwnerID          ����ID
			*	@param[in]  i_strName           ��������
			*	@param[out]	��
			*	@retval		����ֵ
			*	@note		��
			*	@attention
			****************************************************************************/
			wstring GetInstanceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);

			/****************************************************************************
			*	@brief		��ȡʵ����������
			*	@param[in]  i_iOwnerID          ����ID
			*	@param[in]  i_strName           ��������
			*	@param[out]	��
			*	@retval		����ֵ
			*	@note		��
			*	@attention
			****************************************************************************/
			map<wstring, wstring>   GetInstanceAllAttributes(const STK_ID& i_iOwnerID);
			
			/****************************************************************************
			*	@brief		���ģ������
			*	@param[in]  i_iOwnerID          ����ID
			*	@param[in]  i_strName           ��������
			*	@param[in]  i_strValue          ����ֵ
			*	@param[out]	��
			*	@retval		STK_STATUS �ɹ����
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS AddModelAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);

			/****************************************************************************
			*	@brief		��ȡԭ������
			*	@param[in]  i_iOwnerID          ����ID
			*	@param[in]  i_strName           ��������
			*	@param[out]	��
			*	@retval		����ֵ
			*	@note		��
			*	@attention
			****************************************************************************/
			wstring GetModelAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);

			/****************************************************************************
			*	@brief		��ȡʵ����������
			*	@param[in]  i_iOwnerID          ����ID
			*	@param[in]  i_strName           ��������
			*	@param[out]	��
			*	@retval		����ֵ
			*	@note		��
			*	@attention
			****************************************************************************/
			map<wstring, wstring>   GetModelAllAttributes(const STK_ID& i_iOwnerID);

			/****************************************************************************
			*	@brief		�������ͺ�id�������Զ���
			*	@param[in]	i_eAttrOwnerType ��������
			*	@param[in]	i_iOwnerID ����ID
			*	@retval		Stk_AttributePtr ���Զ���
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_BOOL FindAttribute(const StkAttributeOwnerType& i_eAttrOwnerType, const STK_ID& i_iOwnerID);
#if 0
			/****************************************************************************
			*	@brief		��ȡָ��ID��MeshFace������
			*	@param[in]	i_iOwnerID ����ID
			*	@retval		Stk_AttributePtr ���Զ���
			*	@note		��
			*	@attention
			****************************************************************************/
			map<wstring, wstring>     GetMeshFaceAllAttribute(const STK_ID& i_iOwnerID);

			/****************************************************************************
			*	@brief		��ȡMeshFace����
			*	@param[in]  i_iOwnerID          ����ID
			*	@param[in]  i_strName           ��������
			*	@param[out]	��
			*	@retval		����ֵ
			*	@note		��
			*	@attention
			****************************************************************************/
			wstring GetMeshFaceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);

			/****************************************************************************
			*	@brief		��ȡMeshEdge����
			*	@param[in]  i_iOwnerID          ����ID
			*	@param[in]  i_strName           ��������
			*	@param[out]	��
			*	@retval		����ֵ
			*	@note		��
			*	@attention
			****************************************************************************/
			wstring GetMeshEdgeAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);

			/****************************************************************************
			*	@brief	   ���MeshFace������
			*	@param[in]	
			*	@retval		�ɹ����
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS               AddMeshFaceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);
			
			/****************************************************************************
			*	@brief		��ȡָ��ID��MeshEdge������
			*	@param[in]	i_iOwnerID ����ID
			*	@retval		Stk_AttributePtr ���Զ���
			*	@note		��
			*	@attention
			****************************************************************************/
			map<wstring, wstring>      GetMeshEdgeAllAttribute(const STK_ID& i_iOwnerID);

			/****************************************************************************
			*	@brief	    ����MeshEdge������
			*	@param[in]	
			*	@retval		�ɹ����
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS                AddMeshEdgeAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);
#endif
			void unLoad();
		};
	}
}

#endif