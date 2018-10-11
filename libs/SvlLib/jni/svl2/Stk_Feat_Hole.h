// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_Feat_Hole.h
 *
 *	@brief	孔特征类
 *
 *	@par	历史:
 *		2014/11/18	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_FEAT_HOLE_ENTITY_H_
#define _STK_FEAT_HOLE_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Entity.h"
#include "Stk_NodeConnector.h"
#include "Stk_Enum.h"

/****************************************************************************
	 *	@class	Stk_Feat_Axis
	 *	@brief	孔轴特征类
	 *
	 *	创建用于存储Node节点的特征的类库。<br>
	 *	
 ****************************************************************************/

class DATAMANAGER_API Stk_Feat_Axis
{
private:
	STK_DIR32		m_dirAxis;							//!< 轴
	STK_PNT32		m_pntOrigin;						//!< 原点
public:
	/****************************************************************************
	 *	@brief		构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_Axis();

	/****************************************************************************
	 *	@brief		析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Feat_Axis();
public:
	/****************************************************************************
	 *	@brief		设置轴方向
	 *	@param[in]	i_dirAxis		轴方向向量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetDirection(STK_DIR32 i_dirAxis);

	/****************************************************************************
	 *	@brief		获取轴方向
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		轴方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32		GetDirection();

	/****************************************************************************
	 *	@brief		设置原点
	 *	@param[in]	i_pntOrigin		原点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetOrigin(STK_PNT32 i_pntOrigin);

	/****************************************************************************
	 *	@brief		获取原点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		原点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32		GetOrigin();

	/****************************************************************************
	 *	@brief		填写轴特征
	 *	@param[in]	i_stkFileP			文件指针
	 *	@param[in]	i_AxisHeadP			轴记录头
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS			WriteFeatAxis(Stk_File *i_stkFileP, StkSegmentHead* i_AxisHeadP);
};

/****************************************************************************
	 *	@class	Stk_FeatLimit
	 *	@brief	Limit特征类
	 *
	 *	创建用于存储Node节点的特征的类库。<br>
	 *	
 ****************************************************************************/
class DATAMANAGER_API Stk_FeatLimit
{
private:
	StkLimitTypeEnum		m_eLimitType;				//!< Limit类型
	STK_DOUBLE64			m_dValue;					//!< Limit长度
	Stk_NodeConnector*		m_stkNodeConnectorP;		//!< 关联要素
public:
	/****************************************************************************
	 *	@brief		构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_FeatLimit();

	/****************************************************************************
	 *	@brief		构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_FeatLimit();
public:
	/****************************************************************************
	 *	@brief		设置Limit类型
	 *	@param[in]	i_eLimitType		Limit类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetType(StkLimitTypeEnum i_eLimitType);
	
	/****************************************************************************
	 *	@brief		获取Limit类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		Limit类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkLimitTypeEnum		GetType();

	/****************************************************************************
	 *	@brief		设置Limit值
	 *	@param[in]	i_dValue			Limit值
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetValue(STK_DOUBLE64 i_dValue);
	
	/****************************************************************************
	 *	@brief		获取Limit值
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		Limit值
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetValue();

	/****************************************************************************
	 *	@brief		设置关联对象
	 *	@param[in]	i_NodeConnectorP	关联
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetReferenceObject(Stk_NodeConnector* i_NodeConnectorP);

	/****************************************************************************
	 *	@brief		获取关联对象
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		关联
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_NodeConnector*		GetReferenceObject();

	/****************************************************************************
	 *	@brief		填写Limit
	 *	@param[in]	i_stkFileP			文件指针
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS			WriteFile(Stk_File *i_stkFileP);

	/****************************************************************************
	 *	@brief		读取Limit
	 *	@param[in]	i_stkFileP			文件指针
	 *	@param[in]	i_LimitHeadP		Limit段头
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS			ReadFile(Stk_File* i_stkFileP, StkSegmentHead* i_InnerThreadHeadP);
};

/****************************************************************************
	 *	@class	Stk_FeatInnerThread
	 *	@brief	孔螺纹特征类
	 *
	 *	创建用于存储Node节点的特征的类库。<br>
	 *	
 ****************************************************************************/

class DATAMANAGER_API Stk_FeatInnerThread
{
private:
	StkThreadLimitTypeEnum	m_eThreadLimitType;			//!< 底部类型
	StkThreadTypeEnum		m_eThreadType;				//!< 螺纹类型
	STK_DOUBLE64			m_dDepth;					//!< 螺纹深度
	STK_BOOL				m_bHasDepthTol;				//!< 是否有螺纹深度公差
	STK_DOUBLE64			m_dDepthTol;				//!< 螺纹深度公差
	STK_DOUBLE64			m_dDiameter;				//!< 螺纹直径
	STK_BOOL				m_bHasDiameterTol;			//!< 是否有螺纹直径公差
	STK_DOUBLE64			m_dDiameterTol;				//!< 螺纹直径公差
	STK_DOUBLE64			m_dPitch;					//!< 螺距
	STK_BOOL				m_bHasPitchTol;				//!< 是否有螺距公差
	STK_DOUBLE64			m_dPitchTol;				//!< 螺距公差
	STK_BOOL				m_bIsRightThread;			//!< 是否是右旋螺纹
	Stk_NodeConnector*		m_NodeConnectorP;			//!< 只有当m_eThreadLimitType类型为TYPE_THREAD_UP_TO_PLANE时，才会有该属性
public:
	/****************************************************************************
	 *	@brief		构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_FeatInnerThread();

	/****************************************************************************
	 *	@brief		析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_FeatInnerThread();
public:
	/****************************************************************************
	 *	@brief		设置螺纹底部类型
	 *	@param[in]	i_eThreadLimitType	螺纹底部类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetThreadLimitType(StkThreadLimitTypeEnum i_eThreadLimitType);

	/****************************************************************************
	 *	@brief		获取螺纹底部类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		螺纹底部类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkThreadLimitTypeEnum	GetThreadLimitType();

	/****************************************************************************
	 *	@brief		设置螺纹类型
	 *	@param[in]	i_eThreadType	螺纹类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetThreadType(StkThreadTypeEnum i_eThreadType);

	/****************************************************************************
	 *	@brief		获取螺纹类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		螺纹类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkThreadTypeEnum		GetThreadType();

	/****************************************************************************
	 *	@brief		设置螺纹深度
	 *	@param[in]	i_dDepth		螺纹深度
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDepth(STK_DOUBLE64 i_dDepth);

	/****************************************************************************
	 *	@brief		获取螺纹深度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		螺纹深度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetDepth();

	/****************************************************************************
	 *	@brief		设置是否有螺纹深度公差
	 *	@param[in]	i_bHasTol		公差标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDepthHasTol(STK_BOOL i_bHasTol);

	/****************************************************************************
	 *	@brief		是否有螺纹深度公差
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		公差标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsDepthHasTol();

	/****************************************************************************
	 *	@brief		设置螺纹深度公差
	 *	@param[in]	i_dDepthTol		深度公差
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDepthTol(STK_DOUBLE64 i_dDepthTol);

	/****************************************************************************
	 *	@brief		获取螺纹深度公差
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		深度公差
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetDepthTol();

	/****************************************************************************
	 *	@brief		设置螺纹直径
	 *	@param[in]	i_dDiameter		螺纹直径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDiameter(STK_DOUBLE64 i_dDiameter);

	/****************************************************************************
	 *	@brief		获取螺纹直径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		螺纹直径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetDiameter();

	/****************************************************************************
	 *	@brief		设置是否有螺纹直径公差
	 *	@param[in]	i_bDiameterHasTol	螺纹直径公差标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDiameterHasTol(STK_BOOL i_bDiameterHasTol);

	/****************************************************************************
	 *	@brief		是否有螺纹直径公差
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		螺纹直径公差标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsHasDiameterTol();

	/****************************************************************************
	 *	@brief		设置螺距
	 *	@param[in]	i_dPitch		螺距
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetPitch(STK_DOUBLE64 i_dPitch);

	/****************************************************************************
	 *	@brief		获取螺距
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		螺距
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetPitch();

	/****************************************************************************
	 *	@brief		设置是否有螺距公差标识
	 *	@param[in]	i_bPitchHasTol	螺距公差标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetPitchHasTol(STK_BOOL i_bPitchHasTol);

	/****************************************************************************
	 *	@brief		是否有螺距公差标识
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		螺距公差标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsHasPitchTol();

	/****************************************************************************
	 *	@brief		设置螺距公差
	 *	@param[in]	i_dPitchHasTol	螺距公差
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetPitchTol(STK_DOUBLE64 i_dPitchHasTol);

	/****************************************************************************
	 *	@brief		获取螺距公差
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		螺距公差
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetPitchTol();

	/****************************************************************************
	 *	@brief		设置是否是右旋螺纹
	 *	@param[in]	i_bRightThread	右旋螺纹标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetRightThreaded(STK_BOOL i_bRightThread);

	/****************************************************************************
	 *	@brief		是否是右旋螺纹
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		右旋螺纹标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsRightThreaded();

	/****************************************************************************
	 *	@brief		设置螺纹参考面
	 *	@param[in]	i_NodeConnectorP	关联
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		只有当m_eThreadLimitType类型为TYPE_THREAD_UP_TO_PLANE时，才会有该属性
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetThreadLimitReference(Stk_NodeConnector* i_NodeConnectorP);

	/****************************************************************************
	 *	@brief		获取螺纹参考面
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		关联
	 *	@note		只有当m_eThreadLimitType类型为TYPE_THREAD_UP_TO_PLANE时，才会有该属性
	 *	@attention	无
	 ****************************************************************************/
	Stk_NodeConnector*		GetThreadLimitReference();

	/****************************************************************************
	 *	@brief		填写螺纹特征
	 *	@param[in]	i_stkFileP			文件指针
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS			WriteFile(Stk_File *i_stkFileP);

	/****************************************************************************
	 *	@brief		读取螺纹特征中除NodeConnector的数据
	 *	@param[in]	i_stkFileP			文件指针
	 *	@param[in]	i_InnerThreadHeadP	螺纹段头
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS			ReadFile(Stk_File* i_stkFileP, StkSegmentHead* i_InnerThreadHeadP);
};

/****************************************************************************
	 *	@class	Stk_Feat_Hole
	 *	@brief	孔特征类
	 *
	 *	创建用于存储Node节点的特征的类库。<br>
	 *	
 ****************************************************************************/

class DATAMANAGER_API Stk_Feat_Hole
	: public Stk_Entity
{
private:
	StkHoleAnchorTypeEnum	m_eHoleAnchorType;			//!< 孔的固定类型
	StkHoleTypeEnum			m_eHoleType;				//!< 孔类型
	StkHoleBottomTypeEnum	m_eHoleBottomType;			//!< 孔的底部类型
	Stk_Feat_Axis*			m_FeatAxisP;				//!< 轴
	Stk_FeatInnerThread*	m_FeatInnerThreadP;			//!< 螺纹
	Stk_FeatLimit*			m_FeatLimitP;				//!< Limit特征
	STK_DOUBLE64			m_dDepth;					//!< 孔深度
	STK_BOOL				m_bHasDepthTol;				//!< 深度是否有公差
	STK_DOUBLE64			m_dDepthTol;				//!< 深度公差
	STK_DOUBLE64			m_dDiameter;				//!< 孔直径
	STK_BOOL				m_bHasDiameterTol;			//!< 直径是否有公差
	STK_DOUBLE64			m_dDiamterTol;				//!< 直径公差
	STK_DOUBLE64			m_dBottomAngle;				//!< 孔的底部角度
	STK_BOOL				m_bHasBottomAngleTol;		//!< 底部角度是否有公差
	STK_DOUBLE64			m_dBottomAngleTol;			//!< 底部角度公差

	STK_DOUBLE64			m_dCounterAngle;			//!< 孔的反向角度
	STK_BOOL				m_bHasCounterAngleTol;		//!< 是否有反向角度公差
	STK_DOUBLE64			m_dCounterAngleTol;			//!< 反向角度公差
	STK_DOUBLE64			m_dCounterDepth;			//!< 反向深度
	STK_BOOL				m_bHasCounterDepthTol;		//!< 是否有反向深度公差
	STK_DOUBLE64			m_dCounterDepthTol;			//!< 反向深度公差
	STK_DOUBLE64			m_dCounterDiamter;			//!< 反向直径
	STK_BOOL				m_bHasCounterDiamterTol;	//!< 是否有反向直径公差
	STK_DOUBLE64			m_dCounterDiamterTol;		//!< 反向直径公差
public:
	/****************************************************************************
	 *	@brief		构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_Hole();

	/****************************************************************************
	 *	@brief		析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Feat_Hole();
public:
	/****************************************************************************
	 *	@brief		设置孔的固定类型
	 *	@param[in]	i_eAnchorType		固定类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetAnchorType(StkHoleAnchorTypeEnum i_eAnchorType);

	/****************************************************************************
	 *	@brief		获取孔的固定类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		固定类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkHoleAnchorTypeEnum	GetAnchorType();

	/****************************************************************************
	 *	@brief		设置孔的类型
	 *	@param[in]	i_eType				孔的类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetType(StkHoleTypeEnum i_eType);

	/****************************************************************************
	 *	@brief		获取孔的类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔的类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkHoleTypeEnum			GetType();

	/****************************************************************************
	 *	@brief		设置孔的底部类型
	 *	@param[in]	i_eBottomType		底部类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetBottomType(StkHoleBottomTypeEnum i_eBottomType);

	/****************************************************************************
	 *	@brief		获取孔的底部类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔的底部类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkHoleBottomTypeEnum	GetBottomType();

	/****************************************************************************
	 *	@brief		设置轴
	 *	@param[in]	i_AxisP				轴
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFeatAxis(Stk_Feat_Axis* i_AxisP);

	/****************************************************************************
	 *	@brief		获取轴
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		轴
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat_Axis*			GetFeatAxis();

	/****************************************************************************
	 *	@brief		设置螺纹
	 *	@param[in]	i_InnerThreadP		螺纹
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetInnerThread(Stk_FeatInnerThread* i_InnerThreadP);

	/****************************************************************************
	 *	@brief		获取螺纹
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		螺纹
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_FeatInnerThread*	GetInnerThread();

	/****************************************************************************
	 *	@brief		设置Limit特征
	 *	@param[in]	i_FeatLimitP		Limit特征
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFeatLimit(Stk_FeatLimit* i_FeatLimitP);

	/****************************************************************************
	 *	@brief		获取Limit特征
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		Limit特征
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_FeatLimit*			GetFeatLimit();

	/****************************************************************************
	 *	@brief		设置孔深度
	 *	@param[in]	i_dDepth			孔深度
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDepth(STK_DOUBLE64 i_dDepth);

	/****************************************************************************
	 *	@brief		获取孔深度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔深度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetDepth();

	/****************************************************************************
	 *	@brief		设置是否有孔深度公差
	 *	@param[in]	i_bDepthTol			孔深度公差标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDepthHasTol(STK_BOOL i_bDepthTol);

	/****************************************************************************
	 *	@brief		是否有孔深度公差
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔深度公差标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsHasDepthTol();

	/****************************************************************************
	 *	@brief		设置孔深度公差
	 *	@param[in]	i_dDepthTol			孔深度公差
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDepthWithTol(STK_DOUBLE64 i_dDepthTol);

	/****************************************************************************
	 *	@brief		获取孔深度公差
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔深度公差
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetDepthWithTol();

	/****************************************************************************
	 *	@brief		设置孔直径
	 *	@param[in]	i_dDiameter			孔直径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDiameter(STK_DOUBLE64 i_dDiameter);

	/****************************************************************************
	 *	@brief		获取孔直径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔直径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetDiameter();

	/****************************************************************************
	 *	@brief		设置孔直径公差标识
	 *	@param[in]	i_bDiameterTol		孔直径公差标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDiameterHasTol(STK_BOOL i_bDiameterTol);

	/****************************************************************************
	 *	@brief		获取孔直径公差标识
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔直径公差标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsHasDiameterTol();

	/****************************************************************************
	 *	@brief		设置孔直径公差
	 *	@param[in]	i_dDiameterTol		孔直径公差
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDiameterWithTol(STK_DOUBLE64 i_dDiameterTol);

	/****************************************************************************
	 *	@brief		获取孔直径公差
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔直径公差
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetDiameterWithTol();

	/****************************************************************************
	 *	@brief		设置孔底部角度
	 *	@param[in]	i_dBottomAngle		孔底部角度
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetBottomAngle(STK_DOUBLE64 i_dBottomAngle);

	/****************************************************************************
	 *	@brief		获取孔底部角度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔底部角度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetBottomAngle();

	/****************************************************************************
	 *	@brief		设置孔底部角度公差标识
	 *	@param[in]	i_bBottomAngleTol	孔底部角度公差标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetBottomAngleHasTol(STK_BOOL i_bBottomAngleTol);

	/****************************************************************************
	 *	@brief		获取孔底部角度公差标识
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔底部角度标识公差
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsHasBottomAngleTol();

	/****************************************************************************
	 *	@brief		设置孔底部角度公差
	 *	@param[in]	i_dBottomAngleTol	孔底部角度公差
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetBottomAngleTol(STK_DOUBLE64 i_dBottomAngleTol);

	/****************************************************************************
	 *	@brief		获取孔底部角度公差
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔底部角度公差
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetBottomAngleTol();

	/****************************************************************************
	 *	@brief		设置孔的反向角度
	 *	@param[in]	i_dCounterAngle		孔的反向角度
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCounterAngle(STK_DOUBLE64 i_dCounterAngle);

	/****************************************************************************
	 *	@brief		获取孔的反向角度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔反向角度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64			GetCounterAngle();

	/****************************************************************************
	 *	@brief		设置孔的反向角度公差标识
	 *	@param[in]	i_bCounterAngleTol	孔的反向角度公差标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCounterAngleHasTol(STK_BOOL i_bCounterAngleTol);

	/****************************************************************************
	 *	@brief		孔的反向角度公差标识
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔反向角度公差标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				IsHasCounterAngleTol();

	/****************************************************************************
	 *	@brief		设置孔的反向角度公差
	 *	@param[in]	i_dCounterAngleTol	孔的反向角度公差
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCounterAngleTol(STK_DOUBLE64 i_dCounterAngleTol);

	/****************************************************************************
	 *	@brief		孔的反向角度公差
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔反向角度公差
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64				GetCounterAngleTol();

	/****************************************************************************
	 *	@brief		设置孔的反向深度
	 *	@param[in]	i_dCounterDepth		孔的反向角度深度
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCounterDepth(STK_DOUBLE64 i_dCounterDepth);

	/****************************************************************************
	 *	@brief		孔的反向深度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔反向深度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64				GetCounterDepth();

	/****************************************************************************
	 *	@brief		设置孔的反向深度公差标识
	 *	@param[in]	i_bCounterDepthTol	孔的反向深度公差标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetCounterDepthHasTol(STK_BOOL i_bCounterDepthTol);

	/****************************************************************************
	 *	@brief		孔的反向深度公差标识
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔反向深度公差标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsHasCounterDepthTol();

	/****************************************************************************
	 *	@brief		设置孔的反向深度公差
	 *	@param[in]	i_dCounterDepthTol	孔的反向深度公差
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetCounterDepthTol(STK_DOUBLE64 i_dCounterDepthTol);

	/****************************************************************************
	 *	@brief		孔的反向深度公差
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔反向深度公差
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64				GetCounterDepthTol();

	/****************************************************************************
	 *	@brief		设置孔的反向直径
	 *	@param[in]	i_dCounterDiameter	孔的反向直径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetCounterDiameter(STK_DOUBLE64 i_dCounterDiameter);

	/****************************************************************************
	 *	@brief		孔的反向直径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔反向直径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64				GetCounterDiameter();

	/****************************************************************************
	 *	@brief		设置孔的反向直径公差标识
	 *	@param[in]	i_bCounterDiameterTol	孔的反向直径公差标识
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetCounterDiameterHasTol(STK_BOOL i_bCounterDiameterTol);

	/****************************************************************************
	 *	@brief		孔的反向直径公差标识
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔反向直径公差标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsHasCounterDiameterTol();

	/****************************************************************************
	 *	@brief		设置孔的反向直径公差
	 *	@param[in]	i_dCounterDiameterTol	孔的反向直径公差
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetCounterDiameterTol(STK_DOUBLE64 i_dCounterDiameterTol);

	/****************************************************************************
	 *	@brief		获取孔的反向直径公差
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		孔反向直径公差
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64				GetCounterDiameterTol();

	/****************************************************************************
	 *	@brief		填写孔特征
	 *	@param[in]	i_stkFileP			文件指针
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS			WriteFile(Stk_File *i_stkFileP);

	/****************************************************************************
	 *	@brief		读取孔特征
	 *	@param[in]	i_stkFileP			文件指针
	 *	@param[in]	i_HoleHeadP			孔段头指针
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS			ReadFile(Stk_File *i_stkFileP, StkSegmentHead* i_HoleHeadP);
};

#endif