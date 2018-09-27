#pragma once
// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_GeometryAttributeDocument
*	@brief	属性文档类
*	@par 历史记录
*				2017/09/27 创建。qinyp(Hoteamsoft)
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
			};	//!< 类别

		public:
			void            SetVersion(const wstring wstrVersion);
			const wstring&  GetVersion();
			/****************************************************************************
			*	@brief		获取指定Model下的MeshFace的全部属性
			*   @param[in]  i_iModelID 模型ID
			*	@param[in]	i_iOwnerID 属主ID
			*   @param[in]  attrType 属性类型
			*	@retval		Stk_AttributePtr 属性对象
			*	@note		无
			*	@attention
			****************************************************************************/
			map<wstring, wstring>     GetMeshFaceAllAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief		获取指定Model下的MeshFace的指定属性
			*   @param[in]  i_iModelID 模型ID
			*	@param[in]	i_iOwnerID 属主ID
			*   @param[in]  i_strName  属性名称
			*   @param[in]  attrType 属性类型
			*	@retval		Stk_AttributePtr 属性对象
			*	@note		无
			*	@attention
			****************************************************************************/
			wstring           GetMeshFaceAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, 
				const wstring& i_strName, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief	   添加MeshFace的属性
			*	@param[in] i_iModel  模型ID
			*	@param[in]	i_iOwnerID 属主ID
			*   @param[in]  i_strName  属性名称
			*   @param[in]  i_strValue 属性值
			*   @param[in]  attrType 属性类型
			*	@retval		成功与否
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS       AddMeshFaceAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, 
				const wstring& i_strName, const wstring& i_strValue, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief		获取指定Model下的MeshEdge的全部属性
			*   @param[in]  i_iModelID 模型ID
			*	@param[in]	i_iOwnerID 属主ID
			*   @param[in]  attrType 属性类型
			*	@retval		Stk_AttributePtr 属性对象
			*	@note		无
			*	@attention
			****************************************************************************/
			map<wstring, wstring>     GetMeshEdgeAllAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief		获取指定Model下的MeshEdge的指定属性
			*   @param[in]  i_iModelID 模型ID
			*	@param[in]	i_iOwnerID 属主ID
			*   @param[in]  i_strName  属性名称
			*   @param[in]  attrType 属性类型
			*	@retval		Stk_AttributePtr 属性对象
			*	@note		无
			*	@attention
			****************************************************************************/
			wstring           GetMeshEdgeAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
				const wstring& i_strName, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief	   添加MeshEdge的属性
			*	@param[in] i_iModel  模型ID
			*	@param[in]	i_iOwnerID 属主ID
			*   @param[in]  i_strName  属性名称
			*   @param[in]  i_strValue 属性值
			*   @param[in]  attrType 属性类型
			*	@retval		成功与否
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS       AddMeshEdgeAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
				const wstring& i_strName, const wstring& i_strValue, StkAttributeType attrType = StkAttributeType_Geo);
			/****************************************************************************
			*	@brief		卸载
			*	@retval		无
			*	@note		无
			*	@attention
			****************************************************************************/
			void     UnLoad();
		};
	}
}
#endif