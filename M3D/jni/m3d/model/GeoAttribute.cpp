// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	 GeoAttribute.cpp
 *
 *	@brief	基本属性基类成员函数的实现
 *
 *	@par	履历:
 *		2013/12/05	创建。Shaoqy@HOTEAMSOFT
 *		2014/03/04	更新。Gaoqq@HOTEAMSOFT
****************************************************************************/
#include "m3d/model/GeoAttribute.h"
#include "m3d/model/Line3D.h"

namespace M3D
{

/****************************************************************************
 *	@brief		 GeometryAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 GeometryAttribute:: GeometryAttribute()
{
	m_ID = 0;
	m_eGeoAttrType = M3D_GEOATTR_TYPE_UNKNOWN;
}

/****************************************************************************
 *	@brief		 GeometryAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 GeometryAttribute::~ GeometryAttribute()
{

}

/****************************************************************************
 *	@brief		获取所属要素的ID
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_ID	所属要素的ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/
int  GeometryAttribute::GetID()
{
	return m_ID;
}

/****************************************************************************
 *	@brief		设置所属要素的ID
 *	@param[in]	i_ID	所属要素的ID
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  GeometryAttribute::SetID(int i_ID)
{
	m_ID = i_ID;
	return  M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取所属要素的类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eGeoAttrType	所属要素的类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
GeoAttrTypeEnum  GeometryAttribute::GetGeoAttrType()
{
	return m_eGeoAttrType;
}

/****************************************************************************
 *	@brief		设置所属要素的类型
 *	@param[in]	i_nuAttrType	所属要素的类型
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  GeometryAttribute::SetGeoAttrType(GeoAttrTypeEnum i_eAttrType)
{
	m_eGeoAttrType = i_eAttrType;
	return  M_SUCCESS;
}

/****************************************************************************
 *	@brief		 PlaneFaceAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 PlaneFaceAttribute:: PlaneFaceAttribute(): GeometryAttribute()
{
	m_eGeoAttrType = M3D_GEOATTR_TYPE_PLANEFACE;
	m_ID = 0;
	m_dirNormal = Vector3::ZERO;
	m_pntOrigin = Vector3::ZERO;
}

/****************************************************************************
 *	@brief		 PlaneFaceAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 PlaneFaceAttribute::~ PlaneFaceAttribute()
{

}

/****************************************************************************
 *	@brief		设置平面的法向
 *	@param[in]	i_dirNormal		平面法向
 *	@retval		 M_SUCCESS		设置平面法向成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  PlaneFaceAttribute::SetNormal(const  Vector3 &i_dirNormal)
{
	m_dirNormal = i_dirNormal;
	return  M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取平面的法向
 *	@param[in]	无
 *	@retval		m_dirNormal		平面法向
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 Vector3  PlaneFaceAttribute::GetNormal()const
{
	return m_dirNormal;
}

/****************************************************************************
 *	@brief		设置平面的原点
 *	@param[in]	i_pntOrigin		平面原点
 *	@retval		 M_SUCCESS		设置平面原点成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  PlaneFaceAttribute::SetOrigin(const Vector3 &i_pntOrigin)
{
	m_pntOrigin = i_pntOrigin;
	return M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取平面的原点
 *	@param[in]	无
 *	@retval		m_pntOrigin		平面原点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3 PlaneFaceAttribute::GetOrigin()const
{
	return m_pntOrigin;
}

/****************************************************************************
 *	@brief		 RevolutionFaceAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
RevolutionFaceAttribute::RevolutionFaceAttribute(): GeometryAttribute()
{
	m_eGeoAttrType = M3D_GEOATTR_TYPE_REVOLUTIONFACE;
	m_ID = 0;
	m_dRadius = 0.0;
	m_dirRevoAxis = Vector3::ZERO;
	m_pntAxisOrigin = Vector3::ZERO;
}

/****************************************************************************
 *	@brief		 RevolutionFaceAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
RevolutionFaceAttribute::~RevolutionFaceAttribute()
{

}

/****************************************************************************
 *	@brief		设置旋转面半径
 *	@param[in]	i_dRadius		旋转面半径
 *	@retval		 M_SUCCESS		设置旋转面半径成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS RevolutionFaceAttribute::SetRadius(double i_dRadius)
{
	m_dRadius = i_dRadius;
	return  M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取旋转面半径
 *	@param[in]	无
 *	@retval		m_dRadius		旋转面半径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
double  RevolutionFaceAttribute::GetRadius()const
{
	return m_dRadius;
}

/****************************************************************************
 *	@brief		设置旋转面旋转轴
 *	@param[in]	i_dirRevoAxis		旋转面旋转轴方向向量
 *	@retval		 M_SUCCESS			设置旋转面旋转轴成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  RevolutionFaceAttribute::SetRevoAxis(const  Vector3 &i_dirRevoAxis)
{
	m_dirRevoAxis = i_dirRevoAxis;
	return  M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取旋转面旋转轴方向向量
 *	@param[in]	无
 *	@retval		m_dirRevoAxis		旋转面旋转轴方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 Vector3  RevolutionFaceAttribute::GetRevoAxis()const
{
	return m_dirRevoAxis;
}

/****************************************************************************
 *	@brief		设置旋转面的旋转轴原点
 *	@param[in]	i_pntAxisOrigin		旋转面的旋转轴原点
 *	@retval		 M_SUCCESS			设置旋转轴的原点成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  RevolutionFaceAttribute::SetAxisOrigin(const Vector3 &i_pntAxisOrigin)
{
	m_pntAxisOrigin = i_pntAxisOrigin;
	return  M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取旋转面的旋转轴原点
 *	@param[in]	无
 *	@retval		m_pntAxisOrigin			旋转轴的原点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3  RevolutionFaceAttribute::GetAxisOrigin()const
{
	return m_pntAxisOrigin;
}

/****************************************************************************
 *	@brief		 CylinderFaceAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 CylinderFaceAttribute:: CylinderFaceAttribute(): RevolutionFaceAttribute()
{
	m_eGeoAttrType = M3D_GEOATTR_TYPE_CYLINDERFACE;
	m_ID = 0;
	m_dRadius = 0.0;
	m_dirRevoAxis = Vector3::ZERO;
	m_pntAxisOrigin = Vector3::ZERO;
}

/****************************************************************************
 *	@brief		 CylinderFaceAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 CylinderFaceAttribute::~ CylinderFaceAttribute()
{

}

/****************************************************************************
 *	@brief		 ConicalFaceAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 ConicalFaceAttribute:: ConicalFaceAttribute(): RevolutionFaceAttribute()
{
	m_eGeoAttrType = M3D_GEOATTR_TYPE_CONICALFACE;
	m_ID = 0;
	m_dirRevoAxis = Vector3::ZERO;
	m_pntAxisOrigin = Vector3::ZERO;
}

/****************************************************************************
 *	@brief		 ConicalFaceAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 ConicalFaceAttribute::~ ConicalFaceAttribute()
{

}

/****************************************************************************
 *	@brief		 SphereFaceAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 SphereFaceAttribute:: SphereFaceAttribute(): RevolutionFaceAttribute()
{
	m_eGeoAttrType = M3D_GEOATTR_TYPE_SPHEREFACE;
	m_ID = 0;
	m_dRadius = 0.0;
	m_dirRevoAxis = Vector3::ZERO;
	m_pntAxisOrigin = Vector3::ZERO;
}

/****************************************************************************
 *	@brief		 SphereFaceAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 SphereFaceAttribute::~ SphereFaceAttribute()
{

}

/****************************************************************************
 *	@brief		 ToroidalFaceAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 ToroidalFaceAttribute:: ToroidalFaceAttribute(): RevolutionFaceAttribute()
{
	m_eGeoAttrType = M3D_GEOATTR_TYPE_TOROIDALFACE;
	m_ID = 0;
	m_dRadius = 0.0;
	m_dirRevoAxis = Vector3::ZERO;
	m_pntAxisOrigin = Vector3::ZERO;
	m_fMajorRadius = 0.0f;
	m_fMinorRadius = 0.0f;
}

/****************************************************************************
 *	@brief		 ToroidalFaceAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 ToroidalFaceAttribute::~ ToroidalFaceAttribute()
{

}

/****************************************************************************
 *	@brief		获取轮胎面的外径
 *	@param[in]	无
 *	@retval		m_fMajorRadius			轮胎面的外径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 float  ToroidalFaceAttribute::GetMajorRadius()const
{
	return m_fMajorRadius;
}

/****************************************************************************
 *	@brief		设置轮胎面的外径
 *	@param[in]	i_fRadius			轮胎面的外径
 *	@retval		 M_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  ToroidalFaceAttribute::SetMajorRaius( float i_fRadius)
{
	m_fMajorRadius = i_fRadius;
	return  M_SUCCESS;
}

/****************************************************************************
 *	@brief		获取轮胎面的内径
 *	@param[in]	无
 *	@retval		m_fMinorRadius			轮胎面的内径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 float  ToroidalFaceAttribute::GetMinorRadius()const
{
	return m_fMinorRadius;
}

/****************************************************************************
 *	@brief		设置轮胎面的内径
 *	@param[in]	i_fRadius			轮胎面的内径
 *	@retval		 M_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  ToroidalFaceAttribute::SetMinorRadius( float i_fRadius)
{
	m_fMinorRadius = i_fRadius;
	return  M_SUCCESS;
}
/*************************************************************************//**
 *	@brief		 LineAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 LineAttribute:: LineAttribute():  GeometryAttribute()
{
	m_eGeoAttrType = M3D_GEOATTR_TYPE_LINE;
	m_ID = 0;
	m_fLength = 0.0f;
	m_pntStartPoint = Vector3::ZERO;
	m_pntEndPoint = Vector3::ZERO;
	m_pntCenterPoint = Vector3::ZERO;
	m_dirDirection = Vector3::ZERO;
}

/*************************************************************************//**
 *	@brief		 LineAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 LineAttribute::~ LineAttribute()
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
 float  LineAttribute::GetLength()const
{
	return m_fLength;
}

/*************************************************************************//**
 *	@brief		设置边界线长度
 *	@param[in]	i_fLength	边界线长度
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  LineAttribute::SetLength( float i_fLength)
{
	m_fLength = i_fLength;
	return  M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取边界线起始点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntStartPoint	边界线起始点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3  LineAttribute::GetStartPoint()const
{
	return m_pntStartPoint;
}

/*************************************************************************//**
 *	@brief		设置边界线起始点
 *	@param[in]	i_pntStart	边界线起始点
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  LineAttribute::SetStartPoint(const Vector3 &i_pntStart)
{
	m_pntStartPoint = i_pntStart;
	return  M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取边界线终点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntEndPoint	边界线终点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3  LineAttribute::GetEndPoint()const
{
	return m_pntEndPoint;
}

/*************************************************************************//**
 *	@brief		设置边界线终点
 *	@param[in]	i_pntEnd	边界线终点
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  LineAttribute::SetEndPoint(const Vector3 &i_pntEnd)
{
	m_pntEndPoint = i_pntEnd;
	return  M_SUCCESS;
}
/*************************************************************************//**
 *	@brief		获取直线中点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntCenterPoint	直线中点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3  LineAttribute::GetCenterPoint()const
{
	return m_pntCenterPoint;
}

/*************************************************************************//**
 *	@brief		设置直线中点
 *	@param[in]	i_pntCenter	直线中点
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  LineAttribute::SetCenterPoint(const Vector3 &i_pntCenter)
{
	m_pntCenterPoint = i_pntCenter;
	return  M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取直线方向向量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_dirDirection	直线方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 Vector3  LineAttribute::GetDirection()const
{
	return m_dirDirection;
}

/*************************************************************************//**
 *	@brief		设置直线方向向量
 *	@param[in]	i_dirDirect	直线方向向量
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  LineAttribute::SetDirection(const  Vector3 &i_dirDirect)
{
	m_dirDirection = i_dirDirect;
	return  M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		 EllipseAttribute构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 EllipseAttribute:: EllipseAttribute(): GeometryAttribute()
{
	m_eGeoAttrType = M3D_GEOATTR_TYPE_ELLIPSE;
	m_ID = 0;
	m_pntStartPoint = Vector3::ZERO;
	m_pntEndPoint = Vector3::ZERO;
	m_pntCenterPoint= Vector3::ZERO;
	m_fMajorRadius = 0.0f;
	m_fMinorRadius = 0.0f;
	m_dirX =Vector3::ZERO;
}

/****************************************************************************
 *	@brief		获取弧线向量
 *	@param[out]	o_dirX				X方向向量
 *	@param[out]	o_dirY				Y方向向量
 *	@param[out]	o_dirZ				Z方向向量
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void  EllipseAttribute::GetXYZDir( Vector3& o_dirX,  Vector3& o_dirY,  Vector3& o_dirZ)const
{
	o_dirX = m_dirX;
	o_dirY = m_dirY;
	o_dirZ = m_dirZ;
}

/****************************************************************************
 *	@brief		设置弧线向量
 *	@param[in]	i_dirX				X方向向量
 *	@param[in]	i_dirY				Y方向向量
 *	@param[in]	i_dirZ				Z方向向量
 *	@retval		 M_SUCCESS			设置成功
 *	@retval		 ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

M3D_STATUS  EllipseAttribute::SetXYZDir(const  Vector3& i_dirX, const  Vector3& i_dirY, const  Vector3& i_dirZ)
{
	m_dirX = i_dirX;
	m_dirY = i_dirY;
	m_dirZ = i_dirZ;
	return  M_SUCCESS;
}


/*************************************************************************//**
 *	@brief		 EllipseAttribute析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 EllipseAttribute::~ EllipseAttribute()
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
Vector3  EllipseAttribute::GetCenterPoint()const
{
	return m_pntCenterPoint;
}

/*************************************************************************//**
 *	@brief		设置椭圆弧中心点
 *	@param[in]	i_pntCenter	椭圆弧中心点
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  EllipseAttribute::SetCenterPoint(const Vector3 &i_pntCenter)
{
	m_pntCenterPoint = i_pntCenter;
	return  M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取椭圆弧长轴半径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fMajorRadius	椭圆弧长轴半径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 float  EllipseAttribute::GetMajorRadius()const
{
	return m_fMajorRadius;
}

/*************************************************************************//**
 *	@brief		设置椭圆弧长轴半径
 *	@param[in]	i_fRadius	椭圆弧长轴半径
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  EllipseAttribute::SetMajorRadius( float i_fRadius)
{
	m_fMajorRadius = i_fRadius;
	return  M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取椭圆弧短轴半径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_fMinorRadius	椭圆弧短轴半径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 float  EllipseAttribute::GetMinorRadius()const
{
	return m_fMinorRadius;
}

/*************************************************************************//**
 *	@brief		设置椭圆弧短轴半径
 *	@param[in]	i_fRadius	椭圆弧短轴半径
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  EllipseAttribute::SetMinorRadius( float i_fRadius)
{
	m_fMinorRadius = i_fRadius;
	return  M_SUCCESS;
}


/*************************************************************************//**
 *	@brief		获取边界线起始点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntStartPoint	边界线起始点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3  EllipseAttribute::GetStartPoint()const
{
	return m_pntStartPoint;
}

/*************************************************************************//**
 *	@brief		设置边界线起始点
 *	@param[in]	i_pntStart	边界线起始点
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  EllipseAttribute::SetStartPoint(const Vector3 &i_pntStart)
{
	m_pntStartPoint = i_pntStart;
	return  M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取边界线终点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_pntEndPoint	边界线终点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Vector3  EllipseAttribute::GetEndPoint()const
{
	return m_pntEndPoint;
}

/*************************************************************************//**
 *	@brief		设置边界线终点
 *	@param[in]	i_pntEnd	边界线终点
 *	@param[out]	无
 *	@retval		 M_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
M3D_STATUS  EllipseAttribute::SetEndPoint(const Vector3 &i_pntEnd)
{
	m_pntEndPoint = i_pntEnd;
	return M_SUCCESS;
}

/*************************************************************************//**
 *	@brief		转换为 Line
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		 Line*
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Line3D*  LineAttribute::ConvertToLine()
{
	Line3D* stkLine = new  Line3D();

	stkLine->m_StartPoint = m_pntStartPoint;
	stkLine->m_EndPoint = m_pntEndPoint;

	return stkLine;
}

/*************************************************************************//**
 *	@brief		转换为 Ellipse
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		 Ellipse*
 *	@note		无
 *	@attention	无
 ****************************************************************************/
 Ellipse*  EllipseAttribute::ConvertToEllipse()
{
	 Ellipse* stkElps = new  Ellipse();

	stkElps->SetCenterPoint(m_pntCenterPoint);
	stkElps->GetRadius(m_fMajorRadius, m_fMinorRadius);
	stkElps->SetCoordiantePnt(m_pntStartPoint, m_pntEndPoint);

	return stkElps;
}

}
