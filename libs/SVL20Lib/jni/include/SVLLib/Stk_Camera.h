// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Camera.h
 *
 *	@brief	镜头类
 *
 *	@par	历史:
 *		2013/08/14	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_CAMERA_ENTITY_H_
#define _SVLLIB_CAMERA_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_File.h"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_Node;

		/**************************************************************************************
			 *	@class	Stk_Camera
			 *	@brief	镜头类
			 *
			 *	确定视图的查看位置。<br>
			 *	继承自Stk_Entity 类。
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT  Stk_Camera :
			public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Camera)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CAMERA };	//!< 类别
		//protected:
		//	STK_PNT32					m_pntOrigin;
		//	STK_DIR32					m_dirTargetVector;
		//	STK_DIR32					m_dirUpVector;
		//	STK_FLOAT32					m_fAngle;
		//	STK_FLOAT32					m_fFocus;
		//	STK_FLOAT32					m_fZoomFactor;		//!< 缩放因子
		//	STK_UINT32					m_nuProjectType;	//!< 投影类型
		//	STK_BOOL					m_bHasMatrix;		//!< 是否有矩阵
		//	STK_MTX32					m_mtxTransform;		//!< 旋转平移矩阵

		public:
			/****************************************************************************
			 *	@brief		Stk_Camera构造函数
			 *	@param[in]	
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Camera(void);

			/****************************************************************************
			 *	@brief		Stk_Camera析构函数
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
			 *	@attention	无
			 ****************************************************************************/
			~Stk_Camera(void);

			/****************************************************************************
			*	@brief		读取文件中的Body数据
			*	@param[in]	i_pJsonNode			View 父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		STK_ERROR			读取失败
			*	@retval		STK_ERROR_MEMORY	内存申请失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					FromJson(void* i_pJsonNode, void* i_JsonDocP);

			/*****************************************************************************
			*	@brief		写View数据
			*	@param[in]	Value* i_pJsonParentNode			View 父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其它				写入失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					ToJson(void* i_pJsonNode, void* i_JsonDocP);

		public:
			/****************************************************************************
			*	@brief		设置投影类型
			*	@param[in]	i_nuProjectType	投影类型
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetProjectType(const STK_UINT32 &i_nuProjectType);

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
			*	@brief		设置镜头角度
			*	@param[in]	i_fAngle	角度
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetAngle(const STK_FLOAT32 &i_fAngle);

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
			*	@brief		设置镜头原始位置
			*	@param[in]	i_pntOrigin	镜头位置
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetOriginPoint(const STK_PNT32 &i_pntOrigin);

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
			*	@brief		设置Target方向向量
			*	@param[in]	i_dirTargetVector	Target方向向量
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetTargetVector(const STK_DIR32 &i_dirTargetVector);

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
			*	@brief		设置Up方向向量
			*	@param[in]	i_dirUpVector	Up方向向量
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetUpVector(const STK_DIR32 &i_dirUpVector);

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
			*	@brief		设置是否有旋转矩阵标志
			*	@param[in]	i_bHasMatrix	是否有旋转矩阵标识
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void SetMatrixFlag(STK_BOOL i_bHasMatrix);
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
			*	@brief		设置旋转矩阵
			*	@param[in]	i_mtxTransform	4*4旋转矩阵
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetMatrix(STK_MTX32 i_mtxTransform);

			/****************************************************************************
			*	@brief		获取旋转矩阵
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_mtxTransform	旋转矩阵
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_MTX32					GetMatrix();

			/****************************************************************************
			*	@brief		设置视景体的宽高比
			*	@param[in]	i_fAspectRatio	宽高比
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetAspectRatio(const STK_FLOAT32 &i_fAspectRatio);

			/****************************************************************************
			*	@brief		获取视景体的宽高比
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fAspectRatio	宽高比
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32					GetAspectRatio();

			/****************************************************************************
			*	@brief		设置焦距
			*	@param[in]	i_fFocus	焦距
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetFocalDistance(const STK_FLOAT32 &i_fFocalDistance);

			/****************************************************************************
			*	@brief		获取焦距
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fFocalDistance	焦距
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32					GetFocalDistance();

			/****************************************************************************
			*	@brief		设置摄像机与近平面距离
			*	@param[in]	i_fNearDistance	摄像机与近平面距离
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetNearDistance(const STK_FLOAT32 &i_fNearDistance);

			/****************************************************************************
			*	@brief		获取摄像机与近平面距离
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fNearlDistance	摄像机与近平面距离
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32					GetNearDistance();

			/****************************************************************************
			*	@brief		设置摄像机与远平面距离
			*	@param[in]	i_fFarDistance	摄像机与远平面距离
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void						SetFarDistance(const STK_FLOAT32 &i_fFarDistance);

			/****************************************************************************
			*	@brief		获取摄像机与远平面距离
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_fFarDistance	摄像机与远平面距离
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32					GetFarDistance();

			/****************************************************************************
			*	@brief		设置摄像机视景体的高度
			*	@param[in]	i_fHeight	视景体的高度
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void SetHeight(const STK_FLOAT32 &i_fHeight);

			/****************************************************************************
			*	@brief		获取摄像机视景体的高度
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		视景体的高度
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32 GetHeight();
		};
	}//SVLLib
}//HoteamSoft
#endif