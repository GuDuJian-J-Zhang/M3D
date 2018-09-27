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
		};
	}
}
#endif