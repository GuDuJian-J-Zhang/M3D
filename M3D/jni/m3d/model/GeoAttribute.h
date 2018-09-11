// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_GeoAttribute.h
 *
 *	@brief	基本属性基类的声明
 *
 *	@par	履历:
 *		2013/12/05	创建。Shaoqy@HOTEAMSOFT
 *		2014/03/04	更新。Gaoqq@HOTEAMSOFT
 ****************************************************************************/

#ifndef M3D_GEOATTRIBUTE_H_
#define M3D_GEOATTRIBUTE_H_
#include "m3d/base/Vector3.h"
#include "m3d/model/Curve.h"
namespace M3D
{
class Line3D;
/** @brief	基本属性类型 */
enum GeoAttrTypeEnum
{
	M3D_GEOATTR_TYPE_UNKNOWN = 0, M3D_GEOATTR_TYPE_PLANEFACE, /*!< 平面							*/
	M3D_GEOATTR_TYPE_REVOLUTIONFACE, /*!< 旋转面							*/
	M3D_GEOATTR_TYPE_CYLINDERFACE, /*!< 圆柱面							*/
	M3D_GEOATTR_TYPE_CONICALFACE, /*!< 圆锥面							*/
	M3D_GEOATTR_TYPE_SPHEREFACE, /*!< 球面							*/
	M3D_GEOATTR_TYPE_TOROIDALFACE, /*!< 轮胎面							*/
	M3D_GEOATTR_TYPE_LINE, //!< 直线
	M3D_GEOATTR_TYPE_ELLIPSE, //!< 椭圆弧
	M3D_GEOATTR_TYPE_REVOLUTIONFACE_CYLINDERFACE_CONICALFACE, //圆柱面和圆锥面
};

/**************************************************************************************
 *	@class	 GeometryAttribute
 *	@brief	基本属性基类
 *
 *	存储基本属性的共同属性。<br>
 ***************************************************************************************/
class M3D_API GeometryAttribute
{
protected:
	int m_ID; //!< 所属要素ID
	GeoAttrTypeEnum m_eGeoAttrType; //!< 所属要素类型

public:
	GeometryAttribute();
	~ GeometryAttribute();

	int GetID();
	M3D_STATUS SetID(int i_ID);
	GeoAttrTypeEnum GetGeoAttrType();
	M3D_STATUS SetGeoAttrType(GeoAttrTypeEnum i_eAttrType);
};

/**************************************************************************************
 *	@class	Stk_PlaneFaceAttribute
 *	@brief	平面属性类
 *
 *	记录平面的基本属性信息。<br>
 *	继承自 GeometryAttribute 类。
 ***************************************************************************************/

class M3D_API PlaneFaceAttribute: public GeometryAttribute
{
private:
	Vector3 m_dirNormal; //!< 平面法向
	Vector3 m_pntOrigin; //!< 平面原点

public:
	PlaneFaceAttribute();
	~ PlaneFaceAttribute();

	M3D_STATUS SetNormal(const Vector3 &i_dirNormal);
	Vector3 GetNormal() const;
	M3D_STATUS SetOrigin(const Vector3 &i_pntOrigin);
	Vector3 GetOrigin() const;
};

/**************************************************************************************
 *	@class	 RevolutionFaceAttribute
 *	@brief	旋转面属性基类
 *
 *	记录旋转面的基本属性信息。<br>
 *	继承自Stk_GeometryAttribute 类。
 ***************************************************************************************/

class M3D_API RevolutionFaceAttribute: public GeometryAttribute
{
protected:
	double m_dRadius; //!< 原点处半径
	Vector3 m_dirRevoAxis; //!< 旋转轴方向
	Vector3 m_pntAxisOrigin; //!< 旋转轴原点

public:
	RevolutionFaceAttribute();
	~ RevolutionFaceAttribute();

	M3D_STATUS SetRadius(double i_dRadius);
	double GetRadius() const;
	M3D_STATUS SetRevoAxis(const Vector3 &i_dirRevoAxis);
	Vector3 GetRevoAxis() const;
	M3D_STATUS SetAxisOrigin(const Vector3 &i_pntAxisOrigin);
	Vector3 GetAxisOrigin() const;
};

/**************************************************************************************
 *	@class	 CylinderFaceAttribute
 *	@brief	圆柱面属性类
 *
 *	记录圆柱面的基本属性信息。<br>
 *	继承自 RevolutionFaceAttribute 类。
 ***************************************************************************************/

class M3D_API CylinderFaceAttribute: public RevolutionFaceAttribute
{
private:

public:
	CylinderFaceAttribute();
	~ CylinderFaceAttribute();
};

/**************************************************************************************
 *	@class	 ConicalFaceAttribute
 *	@brief	圆锥面属性类
 *
 *	记录圆锥面的基本属性信息。<br>
 *	继承自 RevolutionFaceAttribute 类。
 ***************************************************************************************/
class M3D_API ConicalFaceAttribute: public RevolutionFaceAttribute
{
private:

public:
	ConicalFaceAttribute();
	~ ConicalFaceAttribute();
};

/**************************************************************************************
 *	@class	 SphereFaceAttribute
 *	@brief	球面属性类
 *
 *	记录球面的基本属性信息。<br>
 *	继承自 RevolutionFaceAttribute 类。
 ***************************************************************************************/
class M3D_API SphereFaceAttribute: public RevolutionFaceAttribute
{
private:

public:
	SphereFaceAttribute();
	~ SphereFaceAttribute();
};

/**************************************************************************************
 *	@class	 ToroidalFaceAttribute
 *	@brief	轮胎面属性类
 *
 *	记录轮胎面的基本属性信息。<br>
 *	继承自 RevolutionFaceAttribute 类。
 ***************************************************************************************/
class M3D_API ToroidalFaceAttribute: public RevolutionFaceAttribute
{
private:
	float m_fMajorRadius; //!< 轮胎面的外径
	float m_fMinorRadius; //!< 轮胎面的内径

public:
	ToroidalFaceAttribute();
	~ ToroidalFaceAttribute();

	float GetMajorRadius() const;
	M3D_STATUS SetMajorRaius(float i_fRadius);
	float GetMinorRadius() const;
	M3D_STATUS SetMinorRadius(float i_fRdius);
};

/************************************************************************************//**
 *	@class	 LineAttribute
 *	@brief	Line属性类
 *
 *	记录Line的基本属性信息。<br>
 ***************************************************************************************/
class M3D_API LineAttribute: public GeometryAttribute
{
private:
	Vector3 m_pntCenterPoint; //!< Line的中点
	Vector3 m_dirDirection; //!< Line的方向向量
	Vector3 m_pntStartPoint; //!< Line起点
	Vector3 m_pntEndPoint; //!< Line终点
	float m_fLength; //!< Line长度

public:
	LineAttribute();
	~ LineAttribute();

	Vector3 GetCenterPoint() const;
	M3D_STATUS SetCenterPoint(const Vector3 &i_pntCenter);
	Vector3 GetDirection() const;
	M3D_STATUS SetDirection(const Vector3 &i_dirDirect);
	Vector3 GetStartPoint() const;
	M3D_STATUS SetStartPoint(const Vector3 &i_pntStart);
	Vector3 GetEndPoint() const;
	M3D_STATUS SetEndPoint(const Vector3 &i_pntEnd);
	float GetLength() const;
	M3D_STATUS SetLength(float i_fLength);
	Line3D* ConvertToLine();
};

/************************************************************************************//**
 *	@class	Stk_EllipseAttribute
 *	@brief	Ellipse属性类
 *
 *	记录Ellipse的基本属性信息。<br>
 ***************************************************************************************/
class M3D_API EllipseAttribute: public GeometryAttribute
{
private:
	Vector3 m_pntCenterPoint; //!< Ellipse的中心点
	float m_fMajorRadius; //!< Ellipse的长轴半径
	float m_fMinorRadius; //!< Ellipse的短轴半径

	Vector3 m_pntStartPoint; //!< 边界线起点
	Vector3 m_pntEndPoint; //!< 边界线终点

	Vector3 m_dirX; //!< X方向向量
	Vector3 m_dirY; //!< Y方向向量
	Vector3 m_dirZ; //!< Z方向向量

public:
	EllipseAttribute();
	~ EllipseAttribute();

	Vector3 GetCenterPoint() const;
	M3D_STATUS SetCenterPoint(const Vector3 &i_pntCenter);
	float GetMajorRadius() const;
	M3D_STATUS SetMajorRadius(float i_fRadius);
	float GetMinorRadius() const;
	M3D_STATUS SetMinorRadius(float i_fRadius);

	Vector3 GetStartPoint() const;
	M3D_STATUS SetStartPoint(const Vector3 &i_pntStart);
	Vector3 GetEndPoint() const;
	M3D_STATUS SetEndPoint(const Vector3 &i_pntEnd);

	void GetXYZDir(Vector3& o_dirX, Vector3& o_dirY, Vector3& o_dirZ) const;
	M3D_STATUS SetXYZDir(const Vector3& i_dirX, const Vector3& i_dirY,
			const Vector3& i_dirZ);

	Ellipse* ConvertToEllipse();
};

}
#endif
