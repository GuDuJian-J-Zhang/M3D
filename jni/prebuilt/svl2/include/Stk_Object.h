// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Object.h
 *
 *	@brief	Stk_ProtoType组件类
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_OBJECT_H_
#define _STK_OBJECT_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Define.h"
#include "DataManager.h"

/**************************************************************************************
	 *	@class	Stk_Object
	 *	@brief	Object类
	 *
***************************************************************************************/

class DATAMANAGER_API Stk_Object
{

protected:
	Stk_type_define m_idType; 
public:
	Stk_Object(void);
	~Stk_Object(void);
};

#endif