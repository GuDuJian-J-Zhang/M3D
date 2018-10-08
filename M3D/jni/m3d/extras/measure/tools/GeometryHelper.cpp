#include "m3d/extras/measure/tools/GeometryHelper.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Vector4.h"
#include "m3d/model/GeoAttribute.h"

namespace M3D
{

GeometryHelper::GeometryHelper()
{

}

GeometryHelper::~GeometryHelper()
{

}

bool GeometryHelper::Transform(Vector3& coordinate,const Matrix3x4& matrix)
{
	bool ret = true;

	coordinate = matrix*coordinate;

	return ret;
}

bool GeometryHelper::Transform(PlaneFaceAttribute& attribute,const Matrix3x4& matrix)
{
	bool ret = true;

	Vector3	dirNormal = matrix*Vector4(attribute.GetNormal(),0.0f);
	Vector3	pntOrigin = attribute.GetOrigin();

	attribute.SetOrigin(matrix*pntOrigin);
	attribute.SetNormal(dirNormal);

	return ret;
}

bool GeometryHelper::Transform(RevolutionFaceAttribute& attribute,const Matrix3x4& matrix)
{
	bool ret = true;

	Vector3	dirRevoAxis = matrix*Vector4(attribute.GetRevoAxis(),0.0f) ;
	Vector3	pntAxisOrigin = matrix*attribute.GetAxisOrigin() ;

	attribute.SetAxisOrigin(pntAxisOrigin);
	attribute.SetRevoAxis(dirRevoAxis);

	return ret;
}

bool GeometryHelper::Transform(CylinderFaceAttribute& attribute,const Matrix3x4& matrix)
{
	bool ret = true;

	Vector3	dirRevoAxis = matrix*Vector4(attribute.GetRevoAxis(),0.0f) ;
	Vector3	pntAxisOrigin = matrix*attribute.GetAxisOrigin();

	attribute.SetAxisOrigin(pntAxisOrigin);
	attribute.SetRevoAxis(dirRevoAxis);

	return ret;
}

bool GeometryHelper::Transform(ConicalFaceAttribute& attribute,const Matrix3x4& matrix)
{
	bool ret = true;

	Vector3	dirRevoAxis = matrix*Vector4(attribute.GetRevoAxis(),0.0f) ;
	Vector3	pntAxisOrigin = matrix*attribute.GetAxisOrigin() ;

	attribute.SetAxisOrigin(pntAxisOrigin);
	attribute.SetRevoAxis(dirRevoAxis);

	return ret;
}

bool GeometryHelper::Transform(SphereFaceAttribute& attribute,const Matrix3x4& matrix)
{
	bool ret = true;

	Vector3	dirRevoAxis = matrix*Vector4(attribute.GetRevoAxis(),0.0f) ;
	Vector3	pntAxisOrigin = matrix*attribute.GetAxisOrigin() ;

	attribute.SetAxisOrigin(pntAxisOrigin);
	attribute.SetRevoAxis(dirRevoAxis);

	return ret;
}

bool GeometryHelper::Transform(ToroidalFaceAttribute& attribute,const Matrix3x4& matrix)
{
	bool ret = true;

	Vector3	dirRevoAxis = matrix*Vector4(attribute.GetRevoAxis(),0.0f) ;
	Vector3	pntAxisOrigin = matrix*attribute.GetAxisOrigin() ;

	attribute.SetAxisOrigin(pntAxisOrigin);
	attribute.SetRevoAxis(dirRevoAxis);

	return ret;
}

bool GeometryHelper::Transform(LineAttribute& attribute,const Matrix3x4& matrix)
{
	bool ret = true;

	Vector3		pntCenterPoint = matrix*attribute.GetCenterPoint();				//!< Line的中点
	Vector3		dirDirection = matrix*Vector4(attribute.GetDirection(),0.0f);					//!< Line的方向向量
	Vector3		pntStartPoint = matrix*attribute.GetStartPoint() ;				//!< Line起点
	Vector3		pntEndPoint = matrix*attribute.GetEndPoint() ;					//!< Line终点

	attribute.SetCenterPoint(pntCenterPoint);
	attribute.SetDirection(pntEndPoint - pntStartPoint);
	attribute.SetStartPoint(pntStartPoint);
	attribute.SetEndPoint(pntEndPoint);

	return ret;
}

bool GeometryHelper::Transform(EllipseAttribute& attribute,const Matrix3x4& matrix)
{
	bool ret = true;

	Vector3		pntCenterPoint = matrix*attribute.GetCenterPoint();				//!< Ellipse的中心点

	Vector3		pntStartPoint = matrix*attribute.GetStartPoint();				//!< 边界线起点
	Vector3		pntEndPoint = matrix*attribute.GetEndPoint();					//!< 边界线终点

	Vector3		dirX ;							//!< X方向向量
	Vector3		dirY ;							//!< Y方向向量
	Vector3		dirZ;							//!< Z方向向量

	attribute.GetXYZDir(dirX,dirY,dirZ);

	dirX = matrix*Vector4(dirX,0.0f);
	dirY = matrix*Vector4(dirY,0.0f);
	dirZ = matrix*Vector4(dirZ,0.0f);

	attribute.SetCenterPoint(pntCenterPoint);
	attribute.SetStartPoint(pntStartPoint);
	attribute.SetEndPoint(pntEndPoint);
	attribute.SetXYZDir(dirX,dirY,dirZ);

	return ret;
}

}
