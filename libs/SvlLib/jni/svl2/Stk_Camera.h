// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Camera.h
 *
 *	@brief	镜头类
 *
 *	@par	历史:
 *		2013/08/14	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_CAMERA_ENTITY_H_
#define _Stk_CAMERA_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_Entity.h"
#include "Stk_File.h"

class Stk_Node;

/**************************************************************************************
	 *	@class	Stk_Camera
	 *	@brief	镜头类
	 *
	 *	确定视图的查看位置。<br>
	 *	继承自Stk_Entity 类。
***************************************************************************************/

class DATAMANAGER_API  Stk_Camera :
	public Stk_Entity
{
private:

protected:
	STK_PNT32					m_pntOrigin;
	STK_DIR32					m_dirTargetVector;
	STK_DIR32					m_dirUpVector;
	STK_FLOAT32					m_fAngle;
	STK_FLOAT32					m_fFocus;
	STK_FLOAT32					m_fZoomFactor;		//!< 缩放因子
	STK_UINT32					m_nuProjectType;	//!< 投影类型
	STK_BOOL					m_bHasMatrix;		//!< 是否有矩阵
	STK_MTX32					m_mtxTransform;		//!< 旋转平移矩阵
	Stk_Node*					m_NodeP;			//!< 所属Node

public:
	/****************************************************************************
	 *	@brief		Stk_Camera构造函数
	 *	@param[in]	i_stkProtoTypeP		组件对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Camera(Stk_ProtoType* i_stkProtoTypeP);

	/****************************************************************************
	 *	@brief		Stk_Camera析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Camera(void);

public:
	/****************************************************************************
	 *	@brief		设置镜头ID
	 *	@param[in]	i_ID	镜头ID
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetID(STK_ID i_ID);

	/****************************************************************************
	 *	@brief		获取镜头ID
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetID();

	/****************************************************************************
	 *	@brief		设置镜头原始位置
	 *	@param[in]	i_pntOrigin	镜头位置
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetOriginPoint(const STK_PNT32 &i_pntOrigin);

	/****************************************************************************
	 *	@brief		设置Target方向向量
	 *	@param[in]	i_dirTargetVector	Target方向向量
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetTargetVector(const STK_DIR32 &i_dirTargetVector);

	/****************************************************************************
	 *	@brief		设置焦点
	 *	@param[in]	i_fFocus	焦点
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetFocus(const STK_FLOAT32 &i_fFocus);

	/****************************************************************************
	 *	@brief		设置Up方向向量
	 *	@param[in]	i_dirUpVector	Up方向向量
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetUpVector(const STK_DIR32 &i_dirUpVector);

	/****************************************************************************
	 *	@brief		设置镜头角度
	 *	@param[in]	i_fAngle	角度
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetAngle(const STK_FLOAT32 &i_fAngle);

	/****************************************************************************
	 *	@brief		设置缩放因子
	 *	@param[in]	i_fZoomFactor	缩放因子
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetZoomFactor(const STK_FLOAT32 &i_fZoomFactor);

	/****************************************************************************
	 *	@brief		设置投影类型
	 *	@param[in]	i_nuProjectType	投影类型
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetProjectType(const STK_UINT32 &i_nuProjectType);

	/****************************************************************************
	 *	@brief		设置旋转矩阵
	 *	@param[in]	i_mtxTransform	4*4旋转矩阵
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetMatrix(STK_DOUBLE64 i_mtxTransform[4][4]);

	/****************************************************************************
	 *	@brief		获取镜头原始位置
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntOrigin		镜头原始位置
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32					GetOriginPoint();

	/****************************************************************************
	 *	@brief		获取Target方向向量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dirTargetVector	Target方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32					GetTargetVector();

	/****************************************************************************
	 *	@brief		获取Up方向向量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dirUpVector	Up方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32					GetUpVector();

	/****************************************************************************
	 *	@brief		获取焦点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fFocus	焦点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32					GetFocus();

	/****************************************************************************
	 *	@brief		获取镜头角度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fAngle	镜头角度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32					GetAngle();

	/****************************************************************************
	 *	@brief		获取缩放因子
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fZoomFactor	缩放因子
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32					GetZoomFactor();

	/****************************************************************************
	 *	@brief		获取投影类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuProjectType	投影类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32					GetProjectType();

	/****************************************************************************
	 *	@brief		获取是否有旋转矩阵标志
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bHasMatrix	是否有旋转矩阵标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					GetMatrixFlag();

	/****************************************************************************
	 *	@brief		获取旋转矩阵
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_mtxTransform	旋转矩阵
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_MTX32					GetMatrix();

	/*****************************************************************************
	 *	@brief		设置所对应的Node
	 *	@param[in]	i_NodeP				所对应的Node
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					SetNode(Stk_Node* i_NodeP);

	/*****************************************************************************
	 *	@brief		获取所对应的Node
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		所对应的Node
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	Stk_Node*					GetNode();
};

#endif