// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
 *	@file		Stk_ComponentFeatureImpl.cpp
 *  @brief		◊Èº˛Ãÿ’˜¿‡µƒ µœ÷¿‡
 *	@par ¿˙ ∑º«¬º
 *				2016/11/7 ¥¥Ω®°£MengYaohui(Hoteamsoft)
**************************************************************************/
#include "Stk_NodeImpl.h"
#include "Stk_Node.h"
#include "Stk_DocumentImpl.h"


namespace HoteamSoft {
	namespace SVLLib {

	
STK_CLASS_IMPL_IMPLEMENT(Stk_Node)

Stk_NodeImpl::Stk_NodeImpl(void)
        {
            m_qw = m_qx = m_qy = m_qz = 0.0;
	m_tx = m_ty = m_tz = 0.0;
	m_bbMinX = m_bbMinY = m_bbMinZ = 0.0;
	m_bbMaxX = m_bbMaxY = m_bbMaxZ = 0.0;
	m_pJsonNode = NULL;
}

Stk_NodeImpl::~Stk_NodeImpl(void)
{

}

void Stk_NodeImpl::SetQuaternion( const float& w, const float& x, const float& y, const float& z )
{
	m_qw = w;
	m_qx = x;
	m_qy = y;
	m_qz = z;
}

void Stk_NodeImpl::SetTranslation( const float& x, const float& y, const float& z )
{
	m_tx = x;
	m_ty = y;
	m_tz = z;
}

int Stk_NodeImpl::GetQuaternion( float& w, float& x, float& y, float& z )
{
	w = m_qw;
	x = m_qx;
	y = m_qy;
	z = m_qz;
	return 0;
}

int Stk_NodeImpl::GetTranslation( float& x, float& y, float& z )
{
	x = m_tx;
	y = m_ty;
	z = m_tz; 
	return 0;
}

void Stk_NodeImpl::SetBoundBox( const float* minPnt, const float* maxPnt )
{
	m_bbMinX = minPnt[0];
	m_bbMinY = minPnt[1];
	m_bbMinZ = minPnt[2];
	m_bbMaxX = maxPnt[0];
	m_bbMaxY = maxPnt[1];
	m_bbMaxZ = maxPnt[2];
}

	}
}
