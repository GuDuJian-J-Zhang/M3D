// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Entity.h
 *
 *	@brief	要素基类
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
 *		2014/10/20	更新。Gaoqq@HOTEAMSOFT
 *					由于Node的使用，统一要素的基类
****************************************************************************/

#ifndef _STK_ENTITY_H_
#define _STK_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_Object.h"
class Stk_ProtoType;
class Stk_Node;

class DATAMANAGER_API Stk_Entity
	:public Stk_Object
{
protected:
	STK_ID						m_ID;					//!< ID
	Stk_ProtoType*				m_stkProtoP;			//!< 所属零件
public:
	Stk_Entity(void);
	virtual ~Stk_Entity(void);

	/****************************************************************************
	 *	@brief		获取ProtoType指针
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_stkProtoP		ProtoType指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ProtoType*				GetProtoType();

	/****************************************************************************
	 *	@brief		获取ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ID			PMI ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetID();
	
	/****************************************************************************
	 *	@brief		获取ID
	 *	@param[in]	i_ID			PMI ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetID(STK_ID i_ID);
};

#endif
