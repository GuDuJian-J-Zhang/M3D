// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Object.h
 *
 *	@brief	Object¿‡
 *
 *	@par	¿˙ ∑:
 *		2016/11/20	¥¥Ω®°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_Object.h"
#include "Stk_ObjectImpl.h"

namespace HoteamSoft {
	namespace SVLLib {

	
Stk_Object::Stk_Object(void):count_(0),m_pImpl(new Stk_ObjectImpl)
{
}

Stk_Object::Stk_Object(Stk_ObjectImpl *pImpl):count_(0),m_pImpl(pImpl)
{
}

Stk_Object::~Stk_Object(void)
        {
            delete m_pImpl;
        }
        

Stk_Runtime* Stk_Object::GetRuntime()
{
	return (Stk_Runtime*)(&classStk_Object);
}

std::string Stk_Object::desc()
{
	return classStk_Object.name;
}

const Stk_Runtime Stk_Object::classStk_Object = {"Stk_Object", NULL};	

bool Stk_Object::isKindOf(const std::string &strClassName)
{
	if (strClassName == "Stk_Object")
	{
		return true;
	}
	return false;
}

Stk_Object * Stk_Object::DynamicCast(Stk_Object *p)
{
	if ( p->isKindOf( Stk_Object::desc() ) )
	{
		return static_cast<Stk_Object*>(p);
	}
	return NULL;
}

std::string Stk_Object::GetUuid()
{
	if (NULL == m_pImpl)
		return "";
	return m_pImpl->_uuid;
}

/************************************************************************//**
 *	@brief		ªÒ»°objID
 *	@param[in]	ŒﬁD
 *	@param[out]	Œﬁ
 *	@retval		objID
 *	@note		Œﬁ
 *	@attention	Œﬁ
 ****************************************************************************/
STK_ID Stk_Object::objectId()
{
	return Stk_ObjectImpl::_gsobjectId++;
}

/************************************************************************//**
 *	@brief		ªÒ»° ID
 *	@param[in]	Œﬁ
 *	@param[out]	Œﬁ
 *	@retval		m_ID			PMI ID
 *	@note		Œﬁ
 *	@attention	Œﬁ
 ****************************************************************************/

STK_ID Stk_Object::GetID()
{
	if (NULL == m_pImpl)
		return 0;
	else
		return m_pImpl->m_ID;
}

/************************************************************************//**
 *	@brief		…Ë÷√PMI ID
 *	@param[in]	i_ID			ID
 *	@param[out]	Œﬁ
 *	@retval		STK_SUCCESS		…Ë÷√≥…π¶
 *	@note		Œﬁ
 *	@attention	Œﬁ
 ****************************************************************************/

STK_STATUS Stk_Object::SetID(STK_ID i_ID)
{
	if (NULL == m_pImpl)
		return STK_ERROR;

	m_pImpl->m_ID = i_ID;
	return STK_SUCCESS;
}

	}//svllib
}//hoteamsoft
