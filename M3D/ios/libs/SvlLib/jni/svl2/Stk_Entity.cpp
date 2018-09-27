// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Entity.cpp
 *
 *	@brief	要素基类
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
 *		2014/10/20	更新。Gaoqq@HOTEAMSOFT
 *					由于Node的使用，统一要素的基类
****************************************************************************/

#include "StdAfx.h"
#include "Stk_Entity.h"
#include "Stk_Node.h"

Stk_Entity::Stk_Entity(void)
{
	m_ID = 0;
	m_stkProtoP = NULL;
}

Stk_Entity::~Stk_Entity(void)
{

}


/************************************************************************//**
 *	@brief		获取PMI ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID			PMI ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_Entity::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		设置PMI ID
 *	@param[in]	i_ID			PMI ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Entity::SetID(STK_ID i_ID)
{
	m_ID = i_ID;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取ProtoType指针
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_stkProtoP		ProtoType指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_ProtoType* Stk_Entity::GetProtoType()
{
	return m_stkProtoP;
}
