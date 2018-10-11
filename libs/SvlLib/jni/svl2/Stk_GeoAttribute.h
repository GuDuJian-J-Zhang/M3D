// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_GeoAttribute.h
 *
 *	@brief	基本属性基类的声明
 *
 *	@par	历史:
 *		2013/12/05	创建。Shaoqy@HOTEAMSOFT
 *		2014/03/04	更新。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_GEOATTRIBUTE_H_
#define _STK_GEOATTRIBUTE_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_CurveEntity.h"
#include "Stk_Define.h"

/**************************************************************************************
	 *	@class	Stk_GeometryAttribute
	 *	@brief	基本属性基类
	 *
	 *	存储基本属性的共同属性。<br>
***************************************************************************************/
class DATAMANAGER_API Stk_GeometryAttribute
{
protected:
	STK_ID				m_ID;				//!< 所属要素ID
	StkGeoAttrTypeEnum	m_eGeoAttrType;		//!< 所属要素类型

public:
	/****************************************************************************
	 *	@brief		Stk_GeometryAttribute构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_GeometryAttribute();

	/****************************************************************************
	 *	@brief		Stk_GeometryAttribute析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_GeometryAttribute();

	/****************************************************************************
	 *	@brief		获取所属要素的ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ID	所属要素的ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID				GetID();

	/****************************************************************************
	 *	@brief		设置所属要素的ID
	 *	@param[in]	i_ID	所属要素的ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetID(STK_ID i_ID);

	/****************************************************************************
	 *	@brief		获取所属要素的类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eGeoAttrType	所属要素的类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkGeoAttrTypeEnum	GetGeoAttrType();

	/****************************************************************************
	 *	@brief		设置所属要素的类型
	 *	@param[in]	i_eAttrType		所属要素的类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetGeoAttrType(StkGeoAttrTypeEnum i_eAttrType);
};

/**************************************************************************************
	 *	@class	Stk_PlaneFaceAttribute
	 *	@brief	平面属性类
	 *
	 *	记录平面的基本属性信息。<br>
	 *	继承自Stk_GeometryAttribute 类。
***************************************************************************************/

class DATAMANAGER_API Stk_PlaneFaceAttribute:
	public Stk_GeometryAttribute
{
private:
	STK_DIR32			m_dirNormal;								//!< 平面法向
	STK_PNT32			m_pntOrigin;								//!< 平面原点

public:
	/****************************************************************************
	 *	@brief		Stk_PlaneFaceAttribute构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_PlaneFaceAttribute();

	/****************************************************************************
	 *	@brief		Stk_PlaneFaceAttribute析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_PlaneFaceAttribute();

	/****************************************************************************
	 *	@brief		设置平面的法向
	 *	@param[in]	i_dirNormal		平面法向
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置平面法向成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetNormal(const STK_DIR32 &i_dirNormal);

	/****************************************************************************
	 *	@brief		获取平面的法向
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dirNormal		平面法向
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32			GetNormal();

	/****************************************************************************
	 *	@brief		设置平面的原点
	 *	@param[in]	i_pntOrigin		平面原点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置平面原点成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetOrigin(const STK_PNT32 &i_pntOrigin);

	/****************************************************************************
	 *	@brief		获取平面的原点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntOrigin		平面原点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32			GetOrigin();
};

/**************************************************************************************
	 *	@class	Stk_RevolutionFaceAttribute
	 *	@brief	旋转面属性基类
	 *
	 *	记录旋转面的基本属性信息。<br>
	 *	继承自Stk_GeometryAttribute 类。
***************************************************************************************/

class DATAMANAGER_API Stk_RevolutionFaceAttribute:
	public Stk_GeometryAttribute
{
protected:
	STK_DOUBLE64		m_dRadius;									//!< 原点处半径
	STK_DIR32			m_dirRevoAxis;								//!< 旋转轴方向
	STK_PNT32			m_pntAxisOrigin;							//!< 旋转轴原点

public:
	/****************************************************************************
	 *	@brief		Stk_RevolutionFaceAttribute构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_RevolutionFaceAttribute();

	/****************************************************************************
	 *	@brief		Stk_RevolutionFaceAttribute析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_RevolutionFaceAttribute();

	/****************************************************************************
	 *	@brief		设置旋转面半径
	 *	@param[in]	i_dRadius		旋转面半径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置旋转面半径成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetRadius(STK_DOUBLE64 i_dRadius);
	
	/****************************************************************************
	 *	@brief		获取旋转面半径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dRadius		旋转面半径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DOUBLE64		GetRadius();

	/****************************************************************************
	 *	@brief		设置旋转面旋转轴
	 *	@param[in]	i_dirRevoAxis		旋转面旋转轴方向向量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置旋转面旋转轴成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetRevoAxis(const STK_DIR32 &i_dirRevoAxis);
	
	/****************************************************************************
	 *	@brief		获取旋转面旋转轴方向向量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dirRevoAxis		旋转面旋转轴方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32			GetRevoAxis();

	/****************************************************************************
	 *	@brief		设置旋转面的旋转轴原点
	 *	@param[in]	i_pntAxisOrigin		旋转面的旋转轴原点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置旋转轴的原点成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS			SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin);
	
	/****************************************************************************
	 *	@brief		获取旋转面的旋转轴原点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntAxisOrigin			旋转轴的原点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32			GetAxisOrigin();
};

/**************************************************************************************
	 *	@class	Stk_CylinderFaceAttribute
	 *	@brief	圆柱面属性类
	 *
	 *	记录圆柱面的基本属性信息。<br>
	 *	继承自Stk_RevolutionFaceAttribute 类。
***************************************************************************************/

class DATAMANAGER_API Stk_CylinderFaceAttribute:
	public Stk_RevolutionFaceAttribute
{
private:

public:
	/****************************************************************************
	 *	@brief		Stk_CylinderFaceAttribute构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_CylinderFaceAttribute();

	/****************************************************************************
	 *	@brief		Stk_CylinderFaceAttribute析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_CylinderFaceAttribute();
};

/**************************************************************************************
	 *	@class	Stk_ConicalFaceAttribute
	 *	@brief	圆锥面属性类
	 *
	 *	记录圆锥面的基本属性信息。<br>
	 *	继承自Stk_RevolutionFaceAttribute 类。
***************************************************************************************/
class DATAMANAGER_API Stk_ConicalFaceAttribute:
	public Stk_RevolutionFaceAttribute
{
private:

public:
	/****************************************************************************
	 *	@brief		Stk_ConicalFaceAttribute构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ConicalFaceAttribute();

	/****************************************************************************
	 *	@brief		Stk_ConicalFaceAttribute析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_ConicalFaceAttribute();
};

/**************************************************************************************
	 *	@class	Stk_SphereFaceAttribute
	 *	@brief	球面属性类
	 *
	 *	记录球面的基本属性信息。<br>
	 *	继承自Stk_RevolutionFaceAttribute 类。
***************************************************************************************/
class DATAMANAGER_API Stk_SphereFaceAttribute:
	public Stk_RevolutionFaceAttribute
{
private:

public:
	/****************************************************************************
	 *	@brief		Stk_SphereFaceAttribute构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_SphereFaceAttribute();

	/****************************************************************************
	 *	@brief		Stk_SphereFaceAttribute析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_SphereFaceAttribute();
};

/**************************************************************************************
	 *	@class	Stk_ToroidalFaceAttribute
	 *	@brief	轮胎面属性类
	 *
	 *	记录轮胎面的基本属性信息。<br>
	 *	继承自Stk_RevolutionFaceAttribute 类。
***************************************************************************************/
class DATAMANAGER_API Stk_ToroidalFaceAttribute:
	public Stk_RevolutionFaceAttribute
{
private:
	STK_FLOAT32		m_fMajorRadius;			//!< 轮胎面的外径
	STK_FLOAT32		m_fMinorRadius;			//!< 轮胎面的内径

public:
	/****************************************************************************
	 *	@brief		Stk_ToroidalFaceAttribute构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_ToroidalFaceAttribute();

	/****************************************************************************
	 *	@brief		Stk_ToroidalFaceAttribute析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_ToroidalFaceAttribute();

	/****************************************************************************
	 *	@brief		获取轮胎面的外径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fMajorRadius			轮胎面的外径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32		GetMajorRadius();

	/****************************************************************************
	 *	@brief		设置轮胎面的外径
	 *	@param[in]	i_fRadius			轮胎面的外径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetMajorRaius(STK_FLOAT32 i_fRadius);

	/****************************************************************************
	 *	@brief		获取轮胎面的内径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fMinorRadius			轮胎面的内径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32		GetMinorRadius();

	/****************************************************************************
	 *	@brief		设置轮胎面的内径
	 *	@param[in]	i_fRadius			轮胎面的内径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetMinorRadius(STK_FLOAT32 i_fRdius);
};

/****************************************************************************************
	 *	@class	Stk_LineAttribute
	 *	@brief	Line属性类
	 *
	 *	记录Line的基本属性信息。<br>
***************************************************************************************/
class DATAMANAGER_API Stk_LineAttribute:public Stk_GeometryAttribute
{
private:
	STK_PNT32		m_pntCenterPoint;				//!< Line的中点
	STK_DIR32		m_dirDirection;					//!< Line的方向向量
	STK_PNT32		m_pntStartPoint;				//!< Line起点
	STK_PNT32		m_pntEndPoint;					//!< Line终点
	STK_FLOAT32		m_fLength;						//!< Line长度

public:

	/*****************************************************************************
	 *	@brief		Stk_LineAttribute构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_LineAttribute();

	/*****************************************************************************
	 *	@brief		Stk_LineAttribute析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_LineAttribute();

	/*****************************************************************************
	 *	@brief		获取直线中点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntCenterPoint	直线中点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32		GetCenterPoint();

	/*****************************************************************************
	 *	@brief		设置直线中点
	 *	@param[in]	i_pntCenter	直线中点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetCenterPoint(STK_PNT32 &i_pntCenter);

	/*****************************************************************************
	 *	@brief		获取直线方向向量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dirDirection	直线方向向量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_DIR32		GetDirection();

	/*****************************************************************************
	 *	@brief		设置直线方向向量
	 *	@param[in]	i_dirDirect	直线方向向量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetDirection(STK_DIR32 &i_dirDirect);

	/*****************************************************************************
	 *	@brief		获取边界线起始点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntStartPoint	边界线起始点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32		GetStartPoint();

	/*****************************************************************************
	 *	@brief		设置边界线起始点
	 *	@param[in]	i_pntStart	边界线起始点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetStartPoint(STK_PNT32 &i_pntStart);

	/*****************************************************************************
	 *	@brief		获取边界线终点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntEndPoint	边界线终点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32		GetEndPoint();

	/*****************************************************************************
	 *	@brief		设置边界线终点
	 *	@param[in]	i_pntEnd	边界线终点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetEndPoint(STK_PNT32 &i_pntEnd);

	/*****************************************************************************
	 *	@brief		获取边界线长度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fLength	边界线长度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32		GetLength();

	/*****************************************************************************
	 *	@brief		设置边界线长度
	 *	@param[in]	i_fLength	边界线长度
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetLength(STK_FLOAT32 i_fLength);

	/*****************************************************************************
	 *	@brief		转换为Stk_Line
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		Stk_Line*
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Line*		ConvertToLine();
};

/****************************************************************************************
	 *	@class	Stk_EllipseAttribute
	 *	@brief	Ellipse属性类
	 *
	 *	记录Ellipse的基本属性信息。<br>
***************************************************************************************/
class DATAMANAGER_API Stk_EllipseAttribute:public Stk_GeometryAttribute
{
private:
	STK_PNT32		m_pntCenterPoint;				//!< Ellipse的中心点
	STK_FLOAT32		m_fMajorRadius;					//!< Ellipse的长轴半径
	STK_FLOAT32		m_fMinorRadius;					//!< Ellipse的短轴半径

	STK_PNT32		m_pntStartPoint;				//!< 边界线起点
	STK_PNT32		m_pntEndPoint;					//!< 边界线终点

	STK_DIR32		m_dirX;							//!< X方向向量
	STK_DIR32		m_dirY;							//!< Y方向向量
	STK_DIR32		m_dirZ;							//!< Z方向向量

public:
	/*****************************************************************************
	 *	@brief		Stk_EllipseAttribute构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_EllipseAttribute();

	/*****************************************************************************
	 *	@brief		Stk_EllipseAttribute析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_EllipseAttribute();

	/*****************************************************************************
	 *	@brief		获取椭圆弧中心点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntCenterPoint	椭圆弧中心点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32		GetCenterPoint();

	/*****************************************************************************
	 *	@brief		设置椭圆弧中心点
	 *	@param[in]	i_pntCenter	椭圆弧中心点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetCenterPoint(STK_PNT32 &i_pntCenter);

	/*****************************************************************************
	 *	@brief		获取椭圆弧长轴半径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fMajorRadius	椭圆弧长轴半径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32		GetMajorRadius();

	/*****************************************************************************
	 *	@brief		设置椭圆弧长轴半径
	 *	@param[in]	i_fRadius	椭圆弧长轴半径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetMajorRadius(STK_FLOAT32 i_fRadius);

	/*****************************************************************************
	 *	@brief		获取椭圆弧短轴半径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_fMinorRadius	椭圆弧短轴半径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32		GetMinorRadius();

	/*****************************************************************************
	 *	@brief		设置椭圆弧短轴半径
	 *	@param[in]	i_fRadius	椭圆弧短轴半径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetMinorRadius(STK_FLOAT32 i_fRadius);

	/*****************************************************************************
	 *	@brief		获取边界线起始点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntStartPoint	边界线起始点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32		GetStartPoint();

	/*****************************************************************************
	 *	@brief		设置边界线起始点
	 *	@param[in]	i_pntStart	边界线起始点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetStartPoint(STK_PNT32 &i_pntStart);

	/*****************************************************************************
	 *	@brief		获取边界线终点
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntEndPoint	边界线终点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32		GetEndPoint();

	/*****************************************************************************
	 *	@brief		设置边界线终点
	 *	@param[in]	i_pntEnd	边界线终点
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetEndPoint(STK_PNT32 &i_pntEnd);

	/****************************************************************************
	 *	@brief		获取弧线向量
	 *	@param[in]	无
	 *	@param[out]	o_dirX				X方向向量
	 *	@param[out]	o_dirY				Y方向向量
	 *	@param[out]	o_dirZ				Z方向向量
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void			GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);
	
	/****************************************************************************
	 *	@brief		设置弧线向量
	 *	@param[in]	i_dirX				X方向向量
	 *	@param[in]	i_dirY				Y方向向量
	 *	@param[in]	i_dirZ				Z方向向量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetXYZDir(const STK_DIR32& i_dirX, const STK_DIR32& i_dirY, const STK_DIR32& i_dirZ);
	
	/*****************************************************************************
	 *	@brief		转换为Stk_Ellipse
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		Stk_Ellipse*
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Ellipse*	ConvertToEllipse();
};
#endif