// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_GeoAttribute.cpp
 *
 *	@brief	基本属性基类成员函数的实现
 *
 *	@par	历史:
 *		2013/12/05	创建。Shaoqy@HOTEAMSOFT
 *		2014/03/04	更新。Gaoqq@HOTEAMSOFT
****************************************************************************/
#include "StdAfx.h"
#include "Stk_GeoAttribute.h"
#include "Stk_Enum.h"

/************************************************************************//**
 *	@brief		Stk_GeometryAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_GeometryAttribute::Stk_GeometryAttribute()
{
	m_ID = 0;
	m_eGeoAttrType = GEOATTR_TYPE_UNKNOWN;
}

/************************************************************************//**
 *	@brief		Stk_GeometryAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_GeometryAttribute::~Stk_GeometryAttribute()
{

}

/************************************************************************//**
 *	@brief		获取所属要素的ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID	所属要素的ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_ID Stk_GeometryAttribute::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		设置所属要素的ID
 *	@param[in]	i_ID	所属要素的ID
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_GeometryAttribute::SetID(STK_ID i_ID)
{
	m_ID = i_ID;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取所属要素的类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eGeoAttrType	所属要素的类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkGeoAttrTypeEnum Stk_GeometryAttribute::GetGeoAttrType()
{
	return m_eGeoAttrType;
}

/************************************************************************//**
 *	@brief		设置所属要素的类型
 *	@param[in]	i_eAttrType		所属要素的类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_GeometryAttribute::SetGeoAttrType(StkGeoAttrTypeEnum i_eAttrType)
{
	m_eGeoAttrType = i_eAttrType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		Stk_PlaneFaceAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_PlaneFaceAttribute::Stk_PlaneFaceAttribute():Stk_GeometryAttribute()
{
	m_eGeoAttrType = GEOATTR_TYPE_PLANEFACE;
	m_ID = 0;
	m_dirNormal.X = 0.0f;
	m_dirNormal.Y = 0.0f;
	m_dirNormal.Z = 0.0f;
	m_pntOrigin.X = 0.0f;
	m_pntOrigin.Y = 0.0f;
	m_pntOrigin.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_PlaneFaceAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_PlaneFaceAttribute::~Stk_PlaneFaceAttribute()
{

}

/************************************************************************//**
 *	@brief		设置平面的法向
 *	@param[in]	i_dirNormal		平面法向
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置平面法向成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PlaneFaceAttribute::SetNormal(const STK_DIR32 &i_dirNormal)
{
	m_dirNormal = i_dirNormal;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取平面的法向
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dirNormal		平面法向
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_PlaneFaceAttribute::GetNormal()
{
	return m_dirNormal;
}

/************************************************************************//**
 *	@brief		设置平面的原点
 *	@param[in]	i_pntOrigin		平面原点
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置平面原点成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_PlaneFaceAttribute::SetOrigin(const STK_PNT32 &i_pntOrigin)
{
	m_pntOrigin = i_pntOrigin;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取平面的原点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntOrigin		平面原点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_PlaneFaceAttribute::GetOrigin()
{
	return m_pntOrigin;
}

/************************************************************************//**
 *	@brief		Stk_RevolutionFaceAttribute构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_RevolutionFaceAttribute::Stk_RevolutionFaceAttribute():Stk_GeometryAttribute()
{
	m_eGeoAttrType = GEOATTR_TYPE_REVOLUTIONFACE;
	m_ID = 0;
	m_dRadius = 0.0;
	m_dirRevoAxis.X = 0.0f;
	m_dirRevoAxis.Y = 0.0f;
	m_dirRevoAxis.Z = 0.0f;
	m_pntAxisOrigin.X = 0.0f;
	m_pntAxisOrigin.Y = 0.0f;
	m_pntAxisOrigin.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_RevolutionFaceAttribute析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_RevolutionFaceAttribute::~Stk_RevolutionFaceAttribute()
{

}

/************************************************************************//**
 *	@brief		设置旋转面半径
 *	@param[in]	i_dRadius		旋转面半径
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置旋转面半径成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_RevolutionFaceAttribute::SetRadius(STK_DOUBLE64 i_dRadius)
{
	m_dRadius = i_dRadius;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取旋转面半径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dRadius		旋转面半径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_RevolutionFaceAttribute::GetRadius()
{
	return m_dRadius;
}

/************************************************************************//**
 *	@brief		设置旋转面旋转轴
 *	@param[in]	i_dirRevoAxis		旋转面旋转轴方向向量
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置旋转面旋转轴成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_RevolutionFaceAttribute::SetRevoAxis(const STK_DIR32 &i_dirRevoAxis)
{
	m_dirRevoAxis = i_dirRevoAxis;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取旋转面旋转轴方向向量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dirRevoAxis		旋转面旋转轴方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_RevolutionFaceAttribute::GetRevoAxis()
{
	return m_dirRevoAxis;
}

/************************************************************************//**
 *	@brief		设置旋转面的旋转轴原点
 *	@param[in]	i_pntAxisOrigin		旋转面的旋转轴原点
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置旋转轴的原点成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_RevolutionFaceAttribute::SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin)
{
	m_pntAxisOrigin = i_pntAxisOrigin;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取旋转面的旋转轴原点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntAxisOrigin			旋转轴的原点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_RevolutionFaceAttribute::GetAxisOrigin()
{
	return m_pntAxisOrigin;
}

/************************************************************************//**
 *	@brief		Stk_CylinderFaceAttribute构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_CylinderFaceAttribute::Stk_CylinderFaceAttribute():Stk_RevolutionFaceAttribute()
{
	m_eGeoAttrType = GEOATTR_TYPE_CYLINDERFACE;
	m_ID = 0;
	m_dRadius = 0.0;
	m_dirRevoAxis.X = 0.0f;
	m_dirRevoAxis.Y = 0.0f;
	m_dirRevoAxis.Z = 0.0f;
	m_pntAxisOrigin.X = 0.0f;
	m_pntAxisOrigin.Y = 0.0f;
	m_pntAxisOrigin.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_CylinderFaceAttribute析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_CylinderFaceAttribute::~Stk_CylinderFaceAttribute()
{

}

/************************************************************************//**
 *	@brief		Stk_ConicalFaceAttribute构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_ConicalFaceAttribute::Stk_ConicalFaceAttribute():Stk_RevolutionFaceAttribute()
{
	m_eGeoAttrType = GEOATTR_TYPE_CONICALFACE;
	m_ID = 0;
	m_dirRevoAxis.X = 0.0f;
	m_dirRevoAxis.Y = 0.0f;
	m_dirRevoAxis.Z = 0.0f;
	m_pntAxisOrigin.X = 0.0f;
	m_pntAxisOrigin.Y = 0.0f;
	m_pntAxisOrigin.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_ConicalFaceAttribute析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_ConicalFaceAttribute::~Stk_ConicalFaceAttribute()
{
	
}

/************************************************************************//**
 *	@brief		Stk_SphereFaceAttribute构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_SphereFaceAttribute::Stk_SphereFaceAttribute():Stk_RevolutionFaceAttribute()
{
	m_eGeoAttrType = GEOATTR_TYPE_SPHEREFACE;
	m_ID = 0;
	m_dRadius = 0.0;
	m_dirRevoAxis.X = 0.0f;
	m_dirRevoAxis.Y = 0.0f;
	m_dirRevoAxis.Z = 0.0f;
	m_pntAxisOrigin.X = 0.0f;
	m_pntAxisOrigin.Y = 0.0f;
	m_pntAxisOrigin.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_SphereFaceAttribute析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_SphereFaceAttribute::~Stk_SphereFaceAttribute()
{

}

/************************************************************************//**
 *	@brief		Stk_ToroidalFaceAttribute构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_ToroidalFaceAttribute::Stk_ToroidalFaceAttribute():Stk_RevolutionFaceAttribute()
{
	m_eGeoAttrType = GEOATTR_TYPE_TOROIDALFACE;
	m_ID = 0;
	m_dRadius = 0.0;
	m_dirRevoAxis.X = 0.0f;
	m_dirRevoAxis.Y = 0.0f;
	m_dirRevoAxis.Z = 0.0f;
	m_pntAxisOrigin.X = 0.0f;
	m_pntAxisOrigin.Y = 0.0f;
	m_pntAxisOrigin.Z = 0.0f;
	m_fMajorRadius = 0.0f;
	m_fMinorRadius = 0.0f;
}

/************************************************************************//**
 *	@brief		Stk_ToroidalFaceAttribute析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_ToroidalFaceAttribute::~Stk_ToroidalFaceAttribute()
{

}

/************************************************************************//**
 *	@brief		获取轮胎面的外径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fMajorRadius			轮胎面的外径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_ToroidalFaceAttribute::GetMajorRadius()
{
	return m_fMajorRadius;
}

/************************************************************************//**
 *	@brief		设置轮胎面的外径
 *	@param[in]	i_fRadius			轮胎面的外径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ToroidalFaceAttribute::SetMajorRaius(STK_FLOAT32 i_fRadius)
{
	m_fMajorRadius = i_fRadius;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取轮胎面的内径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fMinorRadius			轮胎面的内径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_ToroidalFaceAttribute::GetMinorRadius()
{
	return m_fMinorRadius;
}

/************************************************************************//**
 *	@brief		设置轮胎面的内径
 *	@param[in]	i_fRadius			轮胎面的内径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_ToroidalFaceAttribute::SetMinorRadius(STK_FLOAT32 i_fRadius)
{
	m_fMinorRadius = i_fRadius;
	return STK_SUCCESS;
}
/*************************************************************************//**
 *	@brief		Stk_LineAttribute构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_LineAttribute::Stk_LineAttribute(): Stk_GeometryAttribute()
{
	m_eGeoAttrType = GEOATTR_TYPE_LINE;
	m_ID = 0;
	m_fLength = 0.0f;
	m_pntStartPoint.X = 0.0f;
	m_pntStartPoint.Y = 0.0f;
	m_pntStartPoint.Z = 0.0f;
	m_pntEndPoint.X = 0.0f;
	m_pntEndPoint.Y = 0.0f;
	m_pntEndPoint.Z = 0.0f;
	m_pntCenterPoint.X = 0.0f;
	m_pntCenterPoint.Y = 0.0f;
	m_pntCenterPoint.Z = 0.0f;
	m_dirDirection.X = 0.0f;
	m_dirDirection.Y = 0.0f;
	m_dirDirection.Z = 0.0f;
}

/*************************************************************************//**
 *	@brief		Stk_LineAttribute析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_LineAttribute::~Stk_LineAttribute()
{

}

/*************************************************************************//**
 *	@brief		获取边界线长度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fLength	边界线长度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_LineAttribute::GetLength()
{
	return m_fLength;
}

/*************************************************************************//**
 *	@brief		设置边界线长度
 *	@param[in]	i_fLength	边界线长度
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_LineAttribute::SetLength(STK_FLOAT32 i_fLength)
{
	m_fLength = i_fLength;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取边界线起始点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntStartPoint	边界线起始点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_LineAttribute::GetStartPoint()
{
	return m_pntStartPoint;
}

/*************************************************************************//**
 *	@brief		设置边界线起始点
 *	@param[in]	i_pntStart	边界线起始点
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_LineAttribute::SetStartPoint(STK_PNT32 &i_pntStart)
{
	m_pntStartPoint = i_pntStart;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取边界线终点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntEndPoint	边界线终点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_LineAttribute::GetEndPoint()
{
	return m_pntEndPoint;
}

/*************************************************************************//**
 *	@brief		设置边界线终点
 *	@param[in]	i_pntEnd	边界线终点
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_LineAttribute::SetEndPoint(STK_PNT32 &i_pntEnd)
{
	m_pntEndPoint = i_pntEnd;
	return STK_SUCCESS;
}
/*************************************************************************//**
 *	@brief		获取直线中点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntCenterPoint	直线中点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_LineAttribute::GetCenterPoint()
{
	return m_pntCenterPoint;
}

/*************************************************************************//**
 *	@brief		设置直线中点
 *	@param[in]	i_pntCenter	直线中点
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_LineAttribute::SetCenterPoint(STK_PNT32 &i_pntCenter)
{
	m_pntCenterPoint = i_pntCenter;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取直线方向向量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dirDirection	直线方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_LineAttribute::GetDirection()
{
	return m_dirDirection;
}

/*************************************************************************//**
 *	@brief		设置直线方向向量
 *	@param[in]	i_dirDirect	直线方向向量
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_LineAttribute::SetDirection(STK_DIR32 &i_dirDirect)
{
	m_dirDirection = i_dirDirect;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		Stk_EllipseAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_EllipseAttribute::Stk_EllipseAttribute():Stk_GeometryAttribute()
{
	m_eGeoAttrType = GEOATTR_TYPE_ELLIPSE;
	m_ID = 0;
	m_pntStartPoint.X = 0.0f;
	m_pntStartPoint.Y = 0.0f;
	m_pntStartPoint.Z = 0.0f;
	m_pntEndPoint.X = 0.0f;
	m_pntEndPoint.Y = 0.0f;
	m_pntEndPoint.Z = 0.0f;
	m_pntCenterPoint.X = 0.0f;
	m_pntCenterPoint.Y = 0.0f;
	m_pntCenterPoint.Z = 0.0f;
	m_fMajorRadius = 0.0f;
	m_fMinorRadius = 0.0f;
	m_dirX.X = 0.0f;
	m_dirX.Y = 0.0f;
	m_dirX.Z = 0.0f;
}

/************************************************************************//**
 *	@brief		获取弧线向量
 *	@param[in]	无
 *	@param[out]	o_dirX				X方向向量
 *	@param[out]	o_dirY				Y方向向量
 *	@param[out]	o_dirZ				Z方向向量
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_EllipseAttribute::GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ)
{
	o_dirX = m_dirX;
	o_dirY = m_dirY;
	o_dirZ = m_dirZ;
}

/************************************************************************//**
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

STK_STATUS Stk_EllipseAttribute::SetXYZDir(const STK_DIR32& i_dirX, const STK_DIR32& i_dirY, const STK_DIR32& i_dirZ)
{
	m_dirX = i_dirX;
	m_dirY = i_dirY;
	m_dirZ = i_dirZ;
	return STK_SUCCESS;
}


/*************************************************************************//**
 *	@brief		Stk_EllipseAttribute析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_EllipseAttribute::~Stk_EllipseAttribute()
{

}

/*************************************************************************//**
 *	@brief		获取椭圆弧中心点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntCenterPoint	椭圆弧中心点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_EllipseAttribute::GetCenterPoint()
{
	return m_pntCenterPoint;
}

/*************************************************************************//**
 *	@brief		设置椭圆弧中心点
 *	@param[in]	i_pntCenter	椭圆弧中心点
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_EllipseAttribute::SetCenterPoint(STK_PNT32 &i_pntCenter)
{
	m_pntCenterPoint = i_pntCenter;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取椭圆弧长轴半径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fMajorRadius	椭圆弧长轴半径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_EllipseAttribute::GetMajorRadius()
{
	return m_fMajorRadius;
}

/*************************************************************************//**
 *	@brief		设置椭圆弧长轴半径
 *	@param[in]	i_fRadius	椭圆弧长轴半径
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_EllipseAttribute::SetMajorRadius(STK_FLOAT32 i_fRadius)
{
	m_fMajorRadius = i_fRadius;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取椭圆弧短轴半径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fMinorRadius	椭圆弧短轴半径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_EllipseAttribute::GetMinorRadius()
{
	return m_fMinorRadius;
}

/*************************************************************************//**
 *	@brief		设置椭圆弧短轴半径
 *	@param[in]	i_fRadius	椭圆弧短轴半径
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_EllipseAttribute::SetMinorRadius(STK_FLOAT32 i_fRadius)
{
	m_fMinorRadius = i_fRadius;
	return STK_SUCCESS;
}


/*************************************************************************//**
 *	@brief		获取边界线起始点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntStartPoint	边界线起始点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_EllipseAttribute::GetStartPoint()
{
	return m_pntStartPoint;
}

/*************************************************************************//**
 *	@brief		设置边界线起始点
 *	@param[in]	i_pntStart	边界线起始点
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_EllipseAttribute::SetStartPoint(STK_PNT32 &i_pntStart)
{
	m_pntStartPoint = i_pntStart;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取边界线终点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntEndPoint	边界线终点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_EllipseAttribute::GetEndPoint()
{
	return m_pntEndPoint;
}

/*************************************************************************//**
 *	@brief		设置边界线终点
 *	@param[in]	i_pntEnd	边界线终点
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_EllipseAttribute::SetEndPoint(STK_PNT32 &i_pntEnd)
{
	m_pntEndPoint = i_pntEnd;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		转换为Stk_Line
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Stk_Line*
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Line* Stk_LineAttribute::ConvertToLine()
{
	Stk_Line* stkLine = new Stk_Line();

	stkLine->SetStartPoint(m_pntStartPoint);
	stkLine->SetEndPoint(m_pntEndPoint);

	return stkLine;
}

/*************************************************************************//**
 *	@brief		转换为Stk_Ellipse
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Stk_Ellipse*
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Ellipse* Stk_EllipseAttribute::ConvertToEllipse()
{
	Stk_Ellipse* stkElps = new Stk_Ellipse();

	stkElps->SetCenterPoint(m_pntCenterPoint);
	stkElps->GetRadius(m_fMajorRadius, m_fMinorRadius);
	stkElps->SetCoordiantePnt(m_pntStartPoint, m_pntEndPoint);

	return stkElps;
}