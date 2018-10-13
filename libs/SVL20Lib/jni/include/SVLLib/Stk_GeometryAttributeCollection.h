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
		};
	}
}
#endif