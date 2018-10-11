#pragma once
// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_AttributeDocument
*	@brief	属性文档类
*	@par 历史记录
*				2017/07/03 创建。qinyp(Hoteamsoft)
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
			};	//!< 类别

		public:
			void            SetVersion(const wstring wstrVersion);
			const wstring&  GetVersion();
			/****************************************************************************
			*	@brief		添加实例属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[in]  i_strValue          属性值
			*	@param[out]	无
			*	@retval		STK_STATUS 成功与否
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS AddInstanceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);
			
			/****************************************************************************
			*	@brief		获取实例属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			wstring GetInstanceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);

			/****************************************************************************
			*	@brief		获取实例所有属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			map<wstring, wstring>   GetInstanceAllAttributes(const STK_ID& i_iOwnerID);
			
			/****************************************************************************
			*	@brief		添加模型属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[in]  i_strValue          属性值
			*	@param[out]	无
			*	@retval		STK_STATUS 成功与否
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS AddModelAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);

			/****************************************************************************
			*	@brief		获取原型属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			wstring GetModelAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);

			/****************************************************************************
			*	@brief		获取实例所有属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			map<wstring, wstring>   GetModelAllAttributes(const STK_ID& i_iOwnerID);

			/****************************************************************************
			*	@brief		根据类型和id查找属性对象
			*	@param[in]	i_eAttrOwnerType 属主类型
			*	@param[in]	i_iOwnerID 属主ID
			*	@retval		Stk_AttributePtr 属性对象
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_BOOL FindAttribute(const StkAttributeOwnerType& i_eAttrOwnerType, const STK_ID& i_iOwnerID);
#if 0
			/****************************************************************************
			*	@brief		获取指定ID的MeshFace的属性
			*	@param[in]	i_iOwnerID 属主ID
			*	@retval		Stk_AttributePtr 属性对象
			*	@note		无
			*	@attention
			****************************************************************************/
			map<wstring, wstring>     GetMeshFaceAllAttribute(const STK_ID& i_iOwnerID);

			/****************************************************************************
			*	@brief		获取MeshFace属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			wstring GetMeshFaceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);

			/****************************************************************************
			*	@brief		获取MeshEdge属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			wstring GetMeshEdgeAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);

			/****************************************************************************
			*	@brief	   添加MeshFace的属性
			*	@param[in]	
			*	@retval		成功与否
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS               AddMeshFaceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);
			
			/****************************************************************************
			*	@brief		获取指定ID的MeshEdge的属性
			*	@param[in]	i_iOwnerID 属主ID
			*	@retval		Stk_AttributePtr 属性对象
			*	@note		无
			*	@attention
			****************************************************************************/
			map<wstring, wstring>      GetMeshEdgeAllAttribute(const STK_ID& i_iOwnerID);

			/****************************************************************************
			*	@brief	    设置MeshEdge的属性
			*	@param[in]	
			*	@retval		成功与否
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS                AddMeshEdgeAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);
#endif
			void unLoad();
		};
	}
}

#endif