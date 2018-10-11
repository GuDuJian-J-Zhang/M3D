// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Camera.cpp
 *
 *	@brief	镜头类
 *
 *	@par	历史:
 *		2013/08/14	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_Camera.h"
#include "Stk_Manager.h"
#include "Stk_Utility.h"

/************************************************************************//**
 *	@brief		Stk_Camera构造函数
 *	@param[in]	i_stkProtoTypeP		组件对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Camera::Stk_Camera(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_ID = 0;
	m_pntOrigin.X = 0.0f;
	m_pntOrigin.Y = 0.0f;
	m_pntOrigin.Z = 0.0f;
	m_dirTargetVector.X = -1.0f;
	m_dirTargetVector.Y = -1.0f;
	m_dirTargetVector.Z = -1.0f;
	m_dirUpVector.X = -1.0f;
	m_dirUpVector.Y = -1.0f;
	m_dirUpVector.Z = -1.0f;
	m_fAngle = 0.0f;
	m_fFocus = 0.0f;
	m_fZoomFactor = 0.0f;
	m_nuProjectType = 0;
	m_bHasMatrix = false;
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
		{
			m_mtxTransform.PlcMatrix[ii][jj] = -1.0f;
		}
	}
	m_NodeP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_Camera析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
 *	@attention	无
 ****************************************************************************/

Stk_Camera::~Stk_Camera(void)
{
	// 删除所从属的Node节点
	if (m_NodeP!=NULL)
	{
		m_NodeP->SetCamera(NULL); //防止析构Node时再次析构Camera造成死循环
		delete m_NodeP;
		m_NodeP = NULL;
	}
}

/************************************************************************//**
 *	@brief		设置镜头ID
 *	@param[in]	i_ID	镜头ID
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
void Stk_Camera::SetID(STK_ID i_ID)
{
	m_ID = i_ID;
}

/************************************************************************//**
 *	@brief		获取镜头ID
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_Camera::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		设置镜头原始位置
 *	@param[in]	i_pntOrigin	镜头位置
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Camera::SetOriginPoint(const STK_PNT32 &i_pntOrigin)
{
	m_pntOrigin = i_pntOrigin;
}

/************************************************************************//**
 *	@brief		设置Target方向向量
 *	@param[in]	i_dirTargetVector	Target方向向量
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Camera::SetTargetVector(const STK_DIR32 &i_dirTargetVector)
{
	m_dirTargetVector = i_dirTargetVector;
}

/************************************************************************//**
 *	@brief		设置焦点
 *	@param[in]	i_fFocus	焦点
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Camera::SetFocus(const STK_FLOAT32 &i_fFocus)
{
	m_fFocus = i_fFocus;
}

/************************************************************************//**
 *	@brief		设置Up方向向量
 *	@param[in]	i_dirUpVector	Up方向向量
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Camera::SetUpVector(const STK_DIR32 &i_dirUpVector)
{
	m_dirUpVector = i_dirUpVector;
}

/************************************************************************//**
 *	@brief		设置镜头角度
 *	@param[in]	i_fAngle	角度
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Camera::SetAngle(const STK_FLOAT32 &i_fAngle)
{
	m_fAngle = i_fAngle;
}

/************************************************************************//**
 *	@brief		设置缩放因子
 *	@param[in]	i_fZoomFactor	缩放因子
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Camera::SetZoomFactor(const STK_FLOAT32 &i_fZoomFactor)
{
	m_fZoomFactor = i_fZoomFactor;
}

/************************************************************************//**
 *	@brief		设置投影类型
 *	@param[in]	i_nuProjectType	投影类型
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Camera::SetProjectType(const STK_UINT32 &i_nuProjectType)
{
	m_nuProjectType = i_nuProjectType;
}

/************************************************************************//**
 *	@brief		设置旋转矩阵
 *	@param[in]	i_mtxTransform	4*4旋转矩阵
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Camera::SetMatrix(STK_DOUBLE64 i_mtxTransform[4][4])
{
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
		{
			m_mtxTransform.PlcMatrix[ii][jj] = i_mtxTransform[ii][jj];
		}
	}
	m_bHasMatrix = true;

	Stk_Utility::TransMatrixToUV(i_mtxTransform, m_dirUpVector, m_dirTargetVector);
}

/************************************************************************//**
 *	@brief		获取镜头原始位置
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntOrigin		镜头原始位置
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_PNT32 Stk_Camera::GetOriginPoint()
{
	return m_pntOrigin;
}

/************************************************************************//**
 *	@brief		获取Target方向向量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dirTargetVector	Target方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_DIR32 Stk_Camera::GetTargetVector()
{
	return m_dirTargetVector;
}

/************************************************************************//**
 *	@brief		获取Up方向向量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dirUpVector	Up方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_DIR32 Stk_Camera::GetUpVector()
{
	return m_dirUpVector;
}

/************************************************************************//**
 *	@brief		获取焦点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fFocus	焦点
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_FLOAT32 Stk_Camera::GetFocus()
{
	return m_fFocus;
}

/************************************************************************//**
 *	@brief		获取镜头角度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fAngle	镜头角度
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_FLOAT32 Stk_Camera::GetAngle()
{
	return m_fAngle;
}

/************************************************************************//**
 *	@brief		获取缩放因子
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fZoomFactor	缩放因子
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_FLOAT32 Stk_Camera::GetZoomFactor()
{
	return m_fZoomFactor;
}

/************************************************************************//**
 *	@brief		获取投影类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuProjectType	投影类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_UINT32 Stk_Camera::GetProjectType()
{
	return m_nuProjectType;
}

/************************************************************************//**
 *	@brief		获取是否有旋转矩阵标志
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_bHasMatrix	是否有旋转矩阵标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Camera::GetMatrixFlag()
{
	return m_bHasMatrix;
}

/************************************************************************//**
 *	@brief		获取旋转矩阵
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_mtxTransform	旋转矩阵
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_MTX32 Stk_Camera::GetMatrix()
{
	if (m_bHasMatrix)
	{
		return m_mtxTransform;
	}
	else
	{
		STK_DIR32 VVec;
		VVec.X = m_dirTargetVector.Y * m_dirUpVector.Z - m_dirTargetVector.Z * m_dirUpVector.Y;
		VVec.Y = m_dirTargetVector.Z * m_dirUpVector.X - m_dirTargetVector.X * m_dirUpVector.Z;
		VVec.Z = m_dirTargetVector.X * m_dirUpVector.Y - m_dirTargetVector.Y * m_dirUpVector.X;

		STK_MTX32 mtxTmp = {
			VVec.X,VVec.Y,VVec.Z,0.0f,
			m_dirUpVector.X,m_dirUpVector.Y,m_dirUpVector.Z,0.0f,
			-m_dirTargetVector.X,-m_dirTargetVector.Y,-m_dirTargetVector.Z,0.0f,
			0.0f,0.0f,0.0f,1.0f
		};

		memcpy(&m_mtxTransform, &mtxTmp, sizeof(STK_MTX32));
		return m_mtxTransform;
	}
}

/*************************************************************************//**
 *	@brief		设置所对应的Node
 *	@param[in]	i_NodeP				所对应的Node
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Camera::SetNode(Stk_Node* i_NodeP)
{
	m_NodeP = i_NodeP;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取所对应的Node
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		所对应的Node
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
Stk_Node* Stk_Camera::GetNode()
{
	return m_NodeP;
}