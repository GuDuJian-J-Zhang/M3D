#pragma once
// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_GeometryAttributeDocument
*	@brief	�����ĵ���
*	@par ��ʷ��¼
*				2017/09/27 ������qinyp(Hoteamsoft)
**************************************************************************/
#ifndef _SVLLIB_GEOMETRYATTRIBUTEDOCUMENT_H_
#define _SVLLIB_GEOMETRYATTRIBUTEDOCUMENT_H_

#include "Stk_BaseDocument.h"

namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_GeometryAttributeCollection : public Stk_BaseDocument
		{
			STK_DECLARE_CLASS(Stk_GeometryAttributeCollection)
		public:
			Stk_GeometryAttributeCollection();
			virtual ~Stk_GeometryAttributeCollection();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_GEOMETRYATTRIBUTECOLLECTION
			};	//!< ���

		public:
			void            SetVersion(const wstring wstrVersion);
			const wstring&  GetVersion();
			/****************************************************************************
			*	@brief		��ȡָ��Model�µ�MeshFace��ȫ������
			*   @param[in]  i_iModelID ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  attrType ��������
			*	@retval		Stk_AttributePtr ���Զ���
			*	@note		��
			*	@attention
			****************************************************************************/
			map<wstring, wstring>     GetMeshFaceAllAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief		��ȡָ��Model�µ�MeshFace��ָ������
			*   @param[in]  i_iModelID ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  i_strName  ��������
			*   @param[in]  attrType ��������
			*	@retval		Stk_AttributePtr ���Զ���
			*	@note		��
			*	@attention
			****************************************************************************/
			wstring           GetMeshFaceAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, 
				const wstring& i_strName, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief	   ���MeshFace������
			*	@param[in] i_iModel  ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  i_strName  ��������
			*   @param[in]  i_strValue ����ֵ
			*   @param[in]  attrType ��������
			*	@retval		�ɹ����
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS       AddMeshFaceAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, 
				const wstring& i_strName, const wstring& i_strValue, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief		��ȡָ��Model�µ�MeshEdge��ȫ������
			*   @param[in]  i_iModelID ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  attrType ��������
			*	@retval		Stk_AttributePtr ���Զ���
			*	@note		��
			*	@attention
			****************************************************************************/
			map<wstring, wstring>     GetMeshEdgeAllAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief		��ȡָ��Model�µ�MeshEdge��ָ������
			*   @param[in]  i_iModelID ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  i_strName  ��������
			*   @param[in]  attrType ��������
			*	@retval		Stk_AttributePtr ���Զ���
			*	@note		��
			*	@attention
			****************************************************************************/
			wstring           GetMeshEdgeAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
				const wstring& i_strName, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief	   ���MeshEdge������
			*	@param[in] i_iModel  ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  i_strName  ��������
			*   @param[in]  i_strValue ����ֵ
			*   @param[in]  attrType ��������
			*	@retval		�ɹ����
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS       AddMeshEdgeAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
				const wstring& i_strName, const wstring& i_strValue, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief		ж��
			*	@retval		��
			*	@note		��
			*	@attention
			****************************************************************************/
			void     UnLoad();
		};
	}
}
#endif