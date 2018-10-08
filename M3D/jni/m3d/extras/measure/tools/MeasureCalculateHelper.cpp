#include "m3d/extras/measure/tools/MeasureCalculateHelper.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/base/Ray.h"
#include "m3d/base/Plane.h"
#include "m3d/model/MeshData.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/model/Curve.h"

namespace M3D
{
MeasureCalculateHelper::MeasureCalculateHelper()
{

}

MeasureCalculateHelper::~MeasureCalculateHelper()
{

}

bool MeasureCalculateHelper::pntInSegment(const Vector3& pntCoordinate,
		const LineAttribute& lineAttribute)
{
	float startPntDistance, endPntDistance;
	Vector3 start = lineAttribute.GetStartPoint();
	Vector3 end = lineAttribute.GetEndPoint();

	startPntDistance = (pntCoordinate - start).Length();
	endPntDistance = (pntCoordinate - end).Length();

	LOGI("start point %s", start.Tostring().c_str());
	LOGI("end point %s", end.Tostring().c_str());
	LOGI("projection point %s", pntCoordinate.Tostring().c_str());
	LOGI(
			"startDistance %f endDistance %f sum %f segement %f", startPntDistance, startPntDistance, startPntDistance+endPntDistance, lineAttribute.GetLength());
	if ((startPntDistance + endPntDistance) > (start - end).Length())
		return false;
	else
		return true;
}
bool MeasureCalculateHelper::PntPntDistance(const Vector3& fistCoordinate,
		const Vector3& secondCoordinate, float& distance)
{
	bool ret = true;

	distance = (fistCoordinate - secondCoordinate).Length();

	return ret;
}

bool MeasureCalculateHelper::PntPntDistance(const Vector3& fistCoordinate,
		const Vector3& secondCoordinate, float& distanceX, float& distanceY,
		float& distanceZ)
{
	bool ret = true;

	Vector3 distance = secondCoordinate - fistCoordinate;

	distanceX = distance.m_x;
	distanceY = distance.m_y;
	distanceZ = distance.m_z;

	return ret;
}

bool MeasureCalculateHelper::PntFaceDistance(const Vector3& pntCoordinate,
		Mesh * meshData, float& distance, float &maxDistance,
		Vector3& projection,Vector3& maxPoint)
{
	VertexSet * pSet = meshData->GetRefMesh();
	unsigned int dataOffect = meshData->GetDataOffset();
	unsigned int dataLength = meshData->GetDataLength();
	vector<Vector3>* positionArray = pSet->GetPositionArray();
	if (pSet->IsUseIndex())
	{
		LOGI("MeasureCalculateHelper::PntFaceDistance INDEX");
		vector<M3D_INDEX_TYPE>* indexArray = pSet->GetIndexArray();
		float min = 0.0f, max = 0.0f;
		//Vector3 minPnt;
		vector<M3D_INDEX_TYPE> tempTriSet;
		M3D_INDEX_TYPE minIndex, maxIndex;
		int count = 0;
		//获得最小距离点索引
		Vector3 startPnt = positionArray->at(
				*(indexArray->begin() + dataOffect));

		projection = startPnt;
		minIndex = *(indexArray->begin() + dataOffect);
		min = (pntCoordinate - startPnt).Length();
		distance = min;

		maxDistance = 0.0f;

		for (vector<M3D_INDEX_TYPE>::iterator it = indexArray->begin()
				+ dataOffect;
				it != indexArray->begin() + dataOffect + dataLength;
				it = it + 3)
		{

			M3D_INDEX_TYPE minIndexTemp, maxIndexTemp;
			float mintemp, maxtemp;

			Vector3 p1 = positionArray->at(*it);
			Vector3 p2 = positionArray->at(*(it + 1));
			Vector3 p3 = positionArray->at(*(it + 2));

			float d1 = (pntCoordinate - p1).Length();
			float d2 = (pntCoordinate - p2).Length();
			float d3 = (pntCoordinate - p3).Length();

			if (d1 > d2)
			{
				if (d1 > d3)
				{
					maxIndexTemp = *it;
					maxtemp = d1;
				}
				else
				{
					maxIndexTemp = *(it + 2);
					maxtemp = d3;
				}
			}
			else if (d2 > d3)
			{
				maxIndexTemp = *(it + 1);
				maxtemp = d2;
			}
			else
			{
				maxIndexTemp = *(it + 2);
				maxtemp = d3;
			}

			if (max < maxtemp)
			{
				max = maxtemp;
				maxIndex = maxIndexTemp;
			}

			Plane plane(p1, p2, p3);
			Vector3 pntInPlane = plane.Project(pntCoordinate);
			Ray ray(pntCoordinate, (pntInPlane - pntCoordinate));
			Vector3 temPro;

			LOGI("p1= %s", p1.Tostring().c_str());
			LOGI("p2= %s", p2.Tostring().c_str());
			LOGI("p3= %s", p3.Tostring().c_str());
			LOGI(
					"pntCoordinate= %s  pntInPlane= %s ", pntCoordinate.Tostring().c_str(), pntInPlane.Tostring().c_str());

			if ((pntCoordinate - pntInPlane).IsZero())
			{
				distance = 0.0f;
				projection = pntInPlane;
			}
			else if (RayPickAction::ISintersectRayAndTriangle(p1, p2, p3, ray,
					temPro))
			{
				float tempDistance = 0.0f;
				tempDistance = (pntCoordinate - temPro).Length();
				if (distance > tempDistance)
				{
					distance = tempDistance;
					projection = temPro;
					//projection = pntInPlane;
					LOGI(
							"Index ,Project Point %s  %s  %f", projection.Tostring().c_str(), pntInPlane.Tostring().c_str(), distance);
				}

			}
			else
			{

				float tempDistance;
				Vector3 tempProjection;
				Vector3 pro1, pro2, pro3;
				MeasureCalculateHelper::pntInSegment(pntCoordinate, p1, p2,
						pro1);
				MeasureCalculateHelper::pntInSegment(pntCoordinate, p1, p3,
						pro2);
				MeasureCalculateHelper::pntInSegment(pntCoordinate, p2, p3,
						pro3);

				float ds1 = (pntCoordinate - pro1).Length();
				float ds2 = (pntCoordinate - pro2).Length();
				float ds3 = (pntCoordinate - pro3).Length();

				if (ds1 < ds2)
				{
					if (ds1 < ds3)
					{
						tempDistance = ds1;
						tempProjection = pro1;
					}
					else
					{
						tempDistance = ds3;
						tempProjection = pro3;
					}
				}
				else if (ds2 < ds3)
				{
					tempDistance = ds2;
					tempProjection = pro2;
				}
				else
				{
					tempDistance = ds3;
					tempProjection = pro3;
				}

				if (distance > tempDistance)
				{
					distance = tempDistance;
					projection = tempProjection;
				}

				LOGI(
						"Index ,Min Point %s  %s  %f", projection.Tostring().c_str(), pntInPlane.Tostring().c_str(), distance);
			}

		}
		LOGI("complete ");
		LOGI("maxIndex = %d", maxIndex);

		maxDistance = max;
		maxPoint = positionArray->at(maxIndex);

	}
	else
	{
		float min = 0.0f, max = 0.0;
		Vector3 startPnt = *(positionArray->begin() + dataOffect);
		projection = startPnt; //初始化
		min = (pntCoordinate - startPnt).Length();
		distance = min; //初始化
		float mintemp, maxtemp;
		Vector3 minPntTemp, maxPntTemp;
		Vector3 minPnt, maxPnt;
		vector<Vector3> tempTriSet;

		for (vector<Vector3>::iterator it = positionArray->begin() + dataOffect;
				it != positionArray->begin() + dataOffect + dataLength;
				it = it + 3)
		{
			float tempDistance = 0.0f;

			Vector3 p1 = *it;
			Vector3 p2 = *(it + 1);
			Vector3 p3 = *(it + 2);

			float d1 = (pntCoordinate - p1).Length();
			float d2 = (pntCoordinate - p2).Length();
			float d3 = (pntCoordinate - p3).Length();

			if (d1 > d2)
			{
				if (d1 > d3)
				{
					maxPntTemp = p1;
					maxtemp = d1;
				}
				else
				{
					maxPntTemp = p3;
					maxtemp = d3;
				}
			}
			else if (d2 > d3)
			{
				maxPntTemp = p2;
				maxtemp = d2;
			}
			else
			{
				maxPntTemp = p3;
				maxtemp = d3;
			}

			if (max < maxtemp)
			{
				max = maxtemp;
				maxPnt = maxPntTemp;
			}

			Plane plane(p1, p2, p3);
			Vector3 pntInPlane = plane.Project(pntCoordinate);
			Ray ray(pntCoordinate, (pntInPlane - pntCoordinate));
			Vector3 temPro;

			if ((pntCoordinate - pntInPlane).IsZero())
			{
				distance = 0.0f;
				projection = pntInPlane;
			}
			else if (RayPickAction::ISintersectRayAndTriangle(p1, p2, p3, ray,
					temPro))

			{
				tempDistance = (pntCoordinate - temPro).Length();
				if (distance > tempDistance)
				{
					distance = tempDistance;
					projection = temPro;
					LOGI(
							"No Index ,Project Point %s  %s  %f", projection.Tostring().c_str(), pntInPlane.Tostring().c_str(), distance);
				}
			}
			else
			{
				float tempDistance;
				Vector3 tempProjection;
				Vector3 pro1, pro2, pro3;
				MeasureCalculateHelper::pntInSegment(pntCoordinate, p1, p2,
						pro1);
				MeasureCalculateHelper::pntInSegment(pntCoordinate, p1, p3,
						pro2);
				MeasureCalculateHelper::pntInSegment(pntCoordinate, p2, p3,
						pro3);

				float ds1 = (pntCoordinate - pro1).Length();
				float ds2 = (pntCoordinate - pro2).Length();
				float ds3 = (pntCoordinate - pro3).Length();

				if (ds1 < ds2)
				{
					if (ds1 < ds3)
					{
						tempDistance = ds1;
						tempProjection = pro1;
					}
					else
					{
						tempDistance = ds3;
						tempProjection = pro3;
					}
				}
				else if (ds2 < ds3)
				{
					tempDistance = ds2;
					tempProjection = pro2;
				}
				else
				{
					tempDistance = ds3;
					tempProjection = pro3;
				}

				if (distance > tempDistance)
				{
					distance = tempDistance;
					projection = tempProjection;
				}

				LOGI(
						"No Index , Min Point %s  %s  %f", projection.Tostring().c_str(), pntInPlane.Tostring().c_str(), distance);
			}

		}

		maxDistance = max;
		maxPoint = maxPnt;

	}

	return true;
}

bool MeasureCalculateHelper::PntLineDistance(const Vector3& pntCoordinate,
		const LineAttribute& lineAttribute, float& distance, int& inLine,
		Vector3& projectPnt)
{
	bool ret = true;
	Ray ray(lineAttribute.GetStartPoint(), lineAttribute.GetDirection());
	projectPnt = ray.Project(pntCoordinate);

	inLine = 0;
	distance = (pntCoordinate - projectPnt).Length();

	return ret;
}

bool MeasureCalculateHelper::PntLineDistance(const Vector3& pntCoordinate,
		const EllipseAttribute& ellipseAttribute, float& distance, int& inLine,
		Vector3& projectPnt,Matrix3x4& worldMatrix,RefPolyLine * m_lineSet)
{
	bool ret = true;
	inLine = 0;//没用
	Vector3 center = ellipseAttribute.GetCenterPoint();
	Vector3 start = ellipseAttribute.GetStartPoint();
	Vector3 end = ellipseAttribute.GetEndPoint();
	Vector3 dirX,dirY,dirZ;
	ellipseAttribute.GetXYZDir(dirX,dirY,dirZ);
	float R = ellipseAttribute.GetMajorRadius();//半径
	//构造圆弧平面
	Plane plane(dirZ,center);
	//求出投影点
	Vector3 projectP = plane.Project(pntCoordinate);

	Vector3 OP = (projectP-center);
	float disTocen = OP.Length();//投影点到圆心距离
	if(disTocen > R)
	{
		LOGI("在圆外");

		SPolyLine * xPolyLine = m_lineSet->GetRefLine();
		vector<Vector3> &pntPoints = xPolyLine->GetPoints();
		unsigned int offset = m_lineSet->GetDataOffset();
		unsigned int dataLength = m_lineSet->GetDataLength();
		bool flag = false;
		//	int countIt =1;
		for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
				it != pntPoints.begin() + offset + dataLength ; it=it+2)
		{


			flag = MeasureCalculateHelper::RaySegmentInsect(center,projectP,
					worldMatrix * (*it),worldMatrix * (*(it + 1)));
			if(flag)
			{
				projectPnt = center+R*(1.0/disTocen)*OP;
				distance = (pntCoordinate-projectPnt).Length();
				LOGI("有交点");
				break;
			}

		}

		if(!flag)
		{
			float CADis = (projectP - start).Length();
			float CBDis = (projectP - end).Length();
			if (CADis >= CBDis)
			{
				distance = (pntCoordinate - end).Length();
				projectPnt = end;
			}
			else
			{
				distance = (pntCoordinate - start).Length();
				projectPnt = start;
			}
		}

	}
	else
	{
		LOGI("在圆内");

		SPolyLine * xPolyLine = m_lineSet->GetRefLine();
		vector<Vector3> &pntPoints = xPolyLine->GetPoints();
		unsigned int offset = m_lineSet->GetDataOffset();
		unsigned int dataLength = m_lineSet->GetDataLength();

		//	int countIt =1;
		bool flag =false;
		for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
				it != pntPoints.begin() + offset + dataLength; it=it+2)
		{


			flag = MeasureCalculateHelper::RaySegmentInsect(center,projectP,
					worldMatrix * (*it),worldMatrix * (*(it + 1)));
			if(flag)
			{
				projectPnt = center+R*(1.0/disTocen)*OP;
				distance = (pntCoordinate-projectPnt).Length();
				break;
			}

		}

		if (!flag)
		{
			float CADis = (projectP - start).Length();
			float CBDis = (projectP - end).Length();
			if (CADis >= CBDis)
			{
				distance = (pntCoordinate - end).Length();
				projectPnt = end;
			}
			else
			{
				distance = (pntCoordinate - start).Length();
				projectPnt = start;
			}
		}

	}
	return ret;
}

bool MeasureCalculateHelper::PntFaceDistance(const Vector3& pntCoordinate,
		const PlaneFaceAttribute& planFaceAttribute, float& distance,
		Vector3& projectPnt)
{
	bool ret = true;

	Plane plane(planFaceAttribute.GetNormal(), planFaceAttribute.GetOrigin());
	projectPnt = plane.Project(pntCoordinate);

	distance = (projectPnt - pntCoordinate).Length();

	LOGI("point face distance is =========== %f", distance);

	return ret;
}

bool MeasureCalculateHelper::PntFaceDistance(const Vector3& pntCoordinate,
		const RevolutionFaceAttribute& revolutionFaceAttribute, float& distance,
		Vector3& projectPnt)
{
	bool ret = false;

	return ret;
}

bool MeasureCalculateHelper::PntFaceDistance(const Vector3& pntCoordinate,
		const CylinderFaceAttribute& cylinderFaceAttribute, float& distance,
		Vector3& projectPnt)
{
	bool ret = false;

	return ret;
}

bool MeasureCalculateHelper::PntFaceDistance(const Vector3& pntCoordinate,
		const ConicalFaceAttribute& conicalFaceAttribute, float& distance,
		Vector3& projectPnt)
{
	bool ret = false;

	return ret;
}

bool MeasureCalculateHelper::PntFaceDistance(const Vector3& pntCoordinate,
		const SphereFaceAttribute& sphereFaceAttribute, float& distance,
		Vector3& projectPnt)
{
	bool ret = false;

	return ret;
}

bool MeasureCalculateHelper::PntFaceDistance(const Vector3& pntCoordinate,
		const ToroidalFaceAttribute& toroidalFaceAttribute, float& distance,
		Vector3& projectPnt)
{
	bool ret = false;

	return ret;
}

bool MeasureCalculateHelper::LineLineDistance(
		const LineAttribute& lineAttribute0,
		const LineAttribute& lineAttribute1, float& distance,
		Vector3& projectPnt0, Vector3& projectPnt1, bool &parallel)
{
	bool ret = true;

	Vector3 alpha0 = lineAttribute0.GetEndPoint()
			- lineAttribute0.GetStartPoint();
	Vector3 alpha1 = lineAttribute1.GetEndPoint()
			- lineAttribute1.GetStartPoint();

	if (alpha0.CrossProduct(alpha1).Length() >= -0.000001
			&& alpha0.CrossProduct(alpha1).Length() <= 0.000001)
	{
		//两直线平行
		//转化到点到直线的距离
		LOGI("平行线");
		parallel = true;
		Vector3 projectPnt;
		Ray ray(lineAttribute0.GetStartPoint(), lineAttribute0.GetDirection());
//		 Vector3 offset =lineAttribute1.GetStartPoint() -  ray.GetOrigin();
//		 Vector3 m_direction = lineAttribute1.GetDirection();
//		 Vector3 dot = offset.DotProduct(m_direction) * m_direction;
//		 LOGE("offset %s ",offset.Tostring().c_str());
//		 LOGE("GetDirection %s ",lineAttribute0.GetDirection().Tostring().c_str());

		projectPnt = ray.Project(lineAttribute1.GetStartPoint());
		distance = (lineAttribute1.GetStartPoint() - projectPnt).Length();

		float l1 = (lineAttribute0.GetStartPoint() - projectPnt).Length();
		float l2 = (lineAttribute0.GetEndPoint() - projectPnt).Length();

		float delta = lineAttribute0.GetLength() - (l1 + l2);
//		float dirc = lineAttribute0.GetDirection().DotProduct(
//				lineAttribute1.GetDirection())
//				/ (lineAttribute0.GetLength() * lineAttribute1.GetLength());
//		if(delta >= -0.000001 && delta <= 0.000001)
//		{
//			projectPnt0 = projectPnt;
//			projectPnt1 = lineAttribute1.GetStartPoint();
//		}

		//LOGI("投影点 %s", projectPnt.Tostring().c_str());
		//LOGI("1起始点 %s", lineAttribute0.GetStartPoint().Tostring().c_str());
		//LOGI("1终点 %s", lineAttribute0.GetEndPoint().Tostring().c_str());
		//LOGI("2起始点 %s", lineAttribute1.GetStartPoint().Tostring().c_str());
		//LOGI("2终点 %s", lineAttribute1.GetEndPoint().Tostring().c_str());

		projectPnt0 = projectPnt;
		projectPnt1 = lineAttribute1.GetStartPoint();

	}
	else
	{
		LOGI("异面直线或公面相交线");
		parallel = false;
		//空间异面直线
		float Xa, Ya, Za, Xb, Yb, Zb, Xc, Yc, Zc, Xd, Yd, Zd;

		Xa = lineAttribute0.GetStartPoint().m_x;
		Ya = lineAttribute0.GetStartPoint().m_y;
		Za = lineAttribute0.GetStartPoint().m_z;

		Xb = lineAttribute0.GetEndPoint().m_x;
		Yb = lineAttribute0.GetEndPoint().m_y;
		Zb = lineAttribute0.GetEndPoint().m_z;

		Xc = lineAttribute1.GetStartPoint().m_x;
		Yc = lineAttribute1.GetStartPoint().m_y;
		Zc = lineAttribute1.GetStartPoint().m_z;

		Xd = lineAttribute1.GetEndPoint().m_x;
		Yd = lineAttribute1.GetEndPoint().m_y;
		Zd = lineAttribute1.GetEndPoint().m_z;

		float H, I, J, K, L, M;
		H = Xb - Xa;
		I = Yb - Ya;
		J = Zb - Za;
		K = Xd - Xc;
		L = Yd - Yc;
		M = Zd - Zc;

		float N, O, P, Q;

		N = H * I * L - I * I * K - J * J * K + H * J * M;
		O = H * H * L - H * I * K - I * J * M + J * J * L;
		P = H * J * K - H * H * M - I * I * M + I * J * L;
		Q = -Xa * N + Ya * O - Za * P;

		float k = (O * Yc - N * Xc - P * Zc - Q) / (N * K - O * L + P * M);

		Vector3 P1, P2;
		P1.m_x = K * k + Xc;
		P1.m_y = L * k + Yc;
		P1.m_z = M * k + Zc;

		Vector3 projectPntT;
		Ray rayT(lineAttribute0.GetStartPoint(), lineAttribute0.GetDirection());

		projectPntT = rayT.Project(P1);

		projectPnt1 = P1;
		projectPnt0 = projectPntT;
		distance = (projectPntT - P1).Length();

	}

	return ret;
}

bool MeasureCalculateHelper::LineLineDistance(
		const LineAttribute& lineAttribute,
		const EllipseAttribute& ellipseAttribute, float& distance,
		Vector3& projectPnt)
{
	bool ret = false;

	return ret;
}

bool MeasureCalculateHelper::LineLineDistance(
		const EllipseAttribute& ellipseAttribute0,
		const EllipseAttribute& ellipseAttribute1, float& distance,
		Vector3& projectPnt)
{
	bool ret = false;

	return ret;
}

bool MeasureCalculateHelper::LineFaceDistance(
		const LineAttribute& lineAttribute,
		const PlaneFaceAttribute& planFaceAttribute, float& distance,
		Vector3& projectPnt0, Vector3& projectPnt1)
{
	bool ret = true;

	Vector3 lineDirection = lineAttribute.GetEndPoint()-lineAttribute.GetStartPoint();
	Vector3 faceNormal = planFaceAttribute.GetNormal();
	LOGI("lineDirection %s faceNormal %s",lineDirection.Tostring().c_str(),faceNormal.Tostring().c_str());
	Vector3 projectPnt;
	LOGI("lineDirection.DotProduct(faceNormal) %f",lineDirection.DotProduct(faceNormal));
	if (lineDirection.DotProduct(faceNormal) >= -0.000001
			&& lineDirection.DotProduct(faceNormal) <= 0.000001)
	{
		MeasureCalculateHelper::PntFaceDistance(lineAttribute.GetStartPoint(),
				planFaceAttribute, distance, projectPnt0);
		LOGI("distance1 is ================= %f", distance);
		MeasureCalculateHelper::PntFaceDistance(lineAttribute.GetEndPoint(),
				planFaceAttribute, distance, projectPnt1);
		LOGI("distance2 is ================ %f", distance);
	}
	else
		return false;
	return ret;
}

bool MeasureCalculateHelper::FaceFaceDistance(
		const PlaneFaceAttribute& planFaceAttribute0,
		const PlaneFaceAttribute& planFaceAttribute1, float& distance,
		Vector3& projectPnt0, Vector3& projectPnt1)
{
	bool ret = true;

	//TODO

	Vector3 origin0 = planFaceAttribute0.GetOrigin();
	Vector3 origin1 = planFaceAttribute1.GetOrigin();
	Vector3 normal0 = planFaceAttribute0.GetNormal();
	Vector3 normal1 = planFaceAttribute1.GetNormal();
	Vector3 cross = normal0.CrossProduct(normal1);
	if(cross.IsZero())
	{

	Plane plan0 (normal0,origin0);
	Plane plan1 (normal1,origin1);



	projectPnt0 = plan1.Project(origin0);
	projectPnt1 = plan0.Project(origin1);



	distance = (projectPnt0 - origin0).Length();

	return ret;
	}
	else
	{
		distance = -1;//表示无效测量对象
		return false;
	}
}

bool MeasureCalculateHelper::LineLineAngle(const LineAttribute& lineAttribute0,
		const LineAttribute& lineAttribute1, float& angle)
{
	bool ret = true;

	angle = lineAttribute0.GetDirection().Angle(lineAttribute1.GetDirection());

	return ret;
}

bool MeasureCalculateHelper::FaceFaceAngle(
		const PlaneFaceAttribute& planFaceAttribute0,
		const PlaneFaceAttribute& planFaceAttribute1, float& angle)
{
	bool ret = true;

	angle = planFaceAttribute0.GetNormal().Angle(
			planFaceAttribute1.GetNormal());

	return ret;
}
bool MeasureCalculateHelper::LineFaceAngle(const LineAttribute& lineAttribute0,
		const PlaneFaceAttribute& planFaceAttribute1, float& angle)
{
	bool ret = true;

	Vector3 normal = planFaceAttribute1.GetNormal();
	Vector3 direction = lineAttribute0.GetDirection();

	float sinTheta = fabs(
			normal.DotProduct(direction)
					/ (normal.Length() * direction.Length()));

	angle = M3D::Asin(sinTheta);

	return ret;
}

bool MeasureCalculateHelper::LineProperty(const LineAttribute& lineAttribute0,
		float& length)
{
	bool ret = true;

	length = lineAttribute0.GetLength();

	return ret;
}

bool MeasureCalculateHelper::LineProperty(
		const EllipseAttribute& ellipseAttribute0, float& radius,
		Vector3& center)
{
	bool ret = true;

	radius = ellipseAttribute0.GetMajorRadius();

	center = ellipseAttribute0.GetCenterPoint();

	return ret;
}

bool MeasureCalculateHelper::triangleArea(const Vector3 & pnt1,
		const Vector3 & pnt2, const Vector3 &pnt3, float & area)
{
	Vector3 edge1 = pnt2 - pnt1;
	Vector3 edge2 = pnt3 - pnt1;
	//面积为边1和边2叉乘模的1/2
	area = (edge1.CrossProduct(edge2)).Length() / 2;
	//LOGI("triangle area");
	return true;
}

bool MeasureCalculateHelper::faceArea(Mesh * meshData, float &area)
{

	VertexSet * pSet = meshData->GetRefMesh();
	unsigned int dataOffect = meshData->GetDataOffset();
	unsigned int dataLength = meshData->GetDataLength();
	vector<Vector3>* positionArray = pSet->GetPositionArray();
	if (pSet->IsUseIndex())
	{
		vector<M3D_INDEX_TYPE>* indexArray = pSet->GetIndexArray();
		for (vector<M3D_INDEX_TYPE>::iterator it = indexArray->begin()
				+ dataOffect;
				it != indexArray->begin() + dataOffect + dataLength;
				it = it + 3)
		{
			Vector3 p1 = positionArray->at(*it);
			Vector3 p2 = positionArray->at(*(it + 1));
			Vector3 p3 = positionArray->at(*(it + 2));
			float ar = 0.0f;

			MeasureCalculateHelper::triangleArea(p1, p2, p3, ar);
			area += ar;

		}

	}
	else
	{
		for (vector<Vector3>::iterator it = positionArray->begin() + dataOffect;
				it != positionArray->begin() + dataOffect + dataLength;
				it = it + 3)
		{
			Vector3 p1 = *it;
			Vector3 p2 = *(it + 1);
			Vector3 p3 = *(it + 2);

			float ar = 0.0f;
			MeasureCalculateHelper::triangleArea(p1, p2, p3, ar);
			area += ar;

		}
	}

	LOGI("any face area");
	return true;
}

bool MeasureCalculateHelper::pntInSegment(const Vector3& pntCoordinate,
		const Vector3& point1, const Vector3 & point2, Vector3 & projecPnt)
{

	float startPntDistance, endPntDistance;

	Vector3 direction = point2 - point1;
	Ray ray(point1, direction);
	Vector3 projection = ray.Project(pntCoordinate);

	startPntDistance = (projection - point1).Length();
	endPntDistance = (projection - point2).Length();
	if ((startPntDistance + endPntDistance) > direction.Length())
	{
		if (startPntDistance > endPntDistance)
		{
			projecPnt = point2;
		}
		else
		{
			projecPnt = point1;
		}

		return false;
	}
	else
	{
		projecPnt = projection;
		return true;
	}
}

bool MeasureCalculateHelper::PntLineDistacne(RefPolyLine * lineSet,
		const Vector3 & point, float & distance, Vector3 & minPoint)
{

	SPolyLine * xPolyLine = lineSet->GetRefLine();
	vector<Vector3> & pntPoints = xPolyLine->GetPoints();
	unsigned int offset = lineSet->GetDataOffset();
	unsigned int dataLength = lineSet->GetDataLength();

	int countIt = 1;
	distance = ( *(pntPoints.begin() + offset)-point).Length();//初始化
	minPoint = *(pntPoints.begin() + offset);//初始化
	for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
			it != pntPoints.begin() + offset + dataLength ; it=it+2)
	{
		float distanceTemp;
		Vector3 minPointTemp;

		Vector3 point1 = (*it);
		Vector3 point2 = (*(it + 1));

		MeasureCalculateHelper::PntLineDistance(point1, point2, point,
				distanceTemp,minPointTemp);
		if (distance > distanceTemp)
		{
			distance = distanceTemp;
			minPoint = minPointTemp;
		}

			LOGI("Add line number: %d", countIt);
	}
	return true;
}

bool MeasureCalculateHelper::PntLineDistance(const Vector3& point1,
		const Vector3& point2, const Vector3& point, float & minDistance,
		Vector3& minPoint)
{

	Vector3 projectPoint;

	MeasureCalculateHelper::pntInSegment(point, point1, point2, projectPoint);
	minDistance = (point - projectPoint).Length();
	minPoint = projectPoint;
	return true;
}

bool MeasureCalculateHelper::RaySegmentInsect(const Vector3& RayPoint0,
		const Vector3& RayPoint1, const Vector3& point0, const Vector3 & point1)
{

	bool ret = true;
	bool parallel;

	Vector3 alpha0 = RayPoint1 - RayPoint0;
	Vector3 alpha1 = point1 - point0;

	LOGI("r1 = %s r2 = %s",RayPoint0.Tostring().c_str(),RayPoint1.Tostring().c_str());
	LOGI("p1 = %s p2 = %s",point0.Tostring().c_str(),point1.Tostring().c_str());
	LOGI("alpha0 = %s alpha1 = %s",alpha0.Tostring().c_str(),alpha1.Tostring().c_str());


	if (alpha0.CrossProduct(alpha1).IsZero())
	{
		//两直线平行
		//转化到点到直线的距离
		LOGI("平行线");
		LOGI("changdu %f",alpha0.CrossProduct(alpha1).Length());
		parallel = true;

		return false;

	}
	else
	{
		LOGI("异面直线或公面相交线");
		parallel = false;
		//空间异面直线
		float Xa, Ya, Za, Xb, Yb, Zb, Xc, Yc, Zc, Xd, Yd, Zd;

		Xa = RayPoint0.m_x;
		Ya = RayPoint0.m_y;
		Za = RayPoint0.m_z;

		Xb = RayPoint1.m_x;
		Yb = RayPoint1.m_y;
		Zb = RayPoint1.m_z;

		Xc = point0.m_x;
		Yc = point0.m_y;
		Zc = point0.m_z;

		Xd = point1.m_x;
		Yd = point1.m_y;
		Zd = point1.m_z;

		float H, I, J, K, L, M;
		H = Xb - Xa;
		I = Yb - Ya;
		J = Zb - Za;
		K = Xd - Xc;
		L = Yd - Yc;
		M = Zd - Zc;

		float N, O, P, Q;

		N = H * I * L - I * I * K - J * J * K + H * J * M;
		O = H * H * L - H * I * K - I * J * M + J * J * L;
		P = H * J * K - H * H * M - I * I * M + I * J * L;
		Q = -Xa * N + Ya * O - Za * P;

		float k = (O * Yc - N * Xc - P * Zc - Q) / (N * K - O * L + P * M);

		Vector3 P1, P2;
		P1.m_x = K * k + Xc;
		P1.m_y = L * k + Yc;
		P1.m_z = M * k + Zc;

		Vector3 projectPntT;
		Ray rayT(RayPoint0, (RayPoint1 - RayPoint0).Normalized());

		projectPntT = rayT.Project(P1);

		Vector3 projectPnt1 = P1;
		Vector3 projectPnt0 = projectPntT;
		float distance = (projectPntT - P1).Length();

		LOGI("projectPnt1 %s projectPnt0 %s ",projectPnt0.Tostring().c_str(),projectPnt1.Tostring().c_str());
		Vector3 pr; //no use

		bool in = MeasureCalculateHelper::pntInSegment(projectPnt1, point0,
				point1, pr);
		LOGI("");
		if (in && (projectPnt1 - projectPnt0).IsZero())
		{
			if ((projectPnt0 - RayPoint0).DotProduct(RayPoint1 - RayPoint0)
					> 0.0)
				return true;
			else
				return false;
		}
		else
			return false;
	}

}

bool MeasureCalculateHelper::LineLineDistance(const EllipseAttribute& ellipseAttribute0,
				const EllipseAttribute& ellipseAttribute1, float& distance0,float &distance1,
				Vector3& projectPnt0,Vector3& projectPnt1,
				int & type)
{
	Vector3 center0, center1; //圆心
	float radius0, radius1; //半径
	Vector3 start0, start1; //起点
	Vector3 dirX0, dirY0, dirZ0, dirX1, dirY1, dirZ1;

	ellipseAttribute0.GetXYZDir(dirX0, dirY0, dirZ0);
	ellipseAttribute1.GetXYZDir(dirX1, dirY1, dirZ1);

	center0 = ellipseAttribute0.GetCenterPoint();
	center1 = ellipseAttribute1.GetCenterPoint();
	radius0 = ellipseAttribute0.GetMajorRadius();
	radius1 = ellipseAttribute1.GetMajorRadius();
	start0 = ellipseAttribute0.GetStartPoint();
	start1 = ellipseAttribute1.GetStartPoint();
	LOGI("radius0 %f radius1 %f ", radius0, radius1);
	Vector3 parall = dirZ0.CrossProduct(dirZ1);
	if (parall.IsZero())
	{
		//所在平面平行
		if ((center0 - center1).IsZero())
		{
			//两点相等 同心圆弧
			distance0 = 0.0f;
			distance1 = fabs(radius0 - radius1);
			projectPnt0 = start0;
			projectPnt1 = (start0 - center0)
					* (1.0 / (start0 - center0).Length()) * radius1 + center1;
			//projectPnt1 = start1;
			type = 2;
		}
		else
		{
			//非共面，求轴心，和 圆心距
			Ray ray(center0, dirZ0);
			Vector3 projectPnt = ray.Project(center1);
			distance0 = (projectPnt - center1).Length();
			distance1 = (center0 - center1).Length();
			projectPnt0 = projectPnt;
			projectPnt1 = center1;
			type = 1;
		}
	}
	else
	{
		//所在平面不平行
		type = 0;
		distance0 = -1;
		distance1 = (center0 - center1).Length();
		projectPnt0 = center0;
		projectPnt1 = center1;
	}
	return true;
}


bool MeasureCalculateHelper::SegmentSegmentDiatance(const Vector3& segment1Start,const Vector3& segment1End,
			const Vector3& segment2Start,const Vector3& segment2End, Vector3& projectPnt1,Vector3 & projectPnt2,float &minDistance)
{
	LOGI("MeasureCalculateHelper::SegmentSegmentDiatance BEGIN");
	bool ret = true;
	LineAttribute firstLineAttribute;
	LineAttribute secondLineAttribute;
	firstLineAttribute.SetStartPoint(segment1Start);
	firstLineAttribute.SetEndPoint(segment1End);
	firstLineAttribute.SetDirection(segment1End - segment1Start);

	secondLineAttribute.SetStartPoint(segment2Start);
	secondLineAttribute.SetEndPoint(segment2End);
	secondLineAttribute.SetDirection(segment2End - segment2Start);

	float distance;
	Vector3 project1, project2;
	bool parallel = true;
	MeasureCalculateHelper::LineLineDistance(firstLineAttribute,
			secondLineAttribute, distance, project1, project2, parallel);

	Vector3 pntStartPoint1 = segment1Start;
	Vector3 pntEndPoint1 = segment1End;

	Vector3 pntStartPoint2 = segment2Start;
	Vector3 pntEndPoint2 = segment2End;

	//判断得到的垂足是否在两条线段上，只要一条不在，就用四个端点之间的距离来判断。
	float sdis1, edis1, sdis2, edis2;

	bool in1, in2;
	in1 = MeasureCalculateHelper::pntInSegment(project1, firstLineAttribute);
	in2 = MeasureCalculateHelper::pntInSegment(project2, secondLineAttribute);

	float firstLineStartToSecondLineMinDistance = 0.0f;
	Vector3 firstStartProjectPnt;
	MeasureCalculateHelper::PntLineDistance(pntStartPoint2, pntEndPoint2,
			pntStartPoint1, firstLineStartToSecondLineMinDistance,
			firstStartProjectPnt);
	float firstLineEndToSecondLineMinDistance = 0.0f;
	Vector3 firstEndProjectPnt;
	MeasureCalculateHelper::PntLineDistance(pntStartPoint2, pntEndPoint2,
			pntEndPoint1, firstLineEndToSecondLineMinDistance,
			firstEndProjectPnt);
	float secondLineStartToFirstLineMinDistance = 0.0f;
	Vector3 secondStartProjectPnt;
	MeasureCalculateHelper::PntLineDistance(pntStartPoint1, pntEndPoint1,
			pntStartPoint2, secondLineStartToFirstLineMinDistance,
			secondStartProjectPnt);
	float secondLineEndToFirstLineMinDistance = 0.0f;
	Vector3 secondEndProjectPnt;
	MeasureCalculateHelper::PntLineDistance(pntStartPoint1, pntEndPoint1,
			pntEndPoint2, secondLineEndToFirstLineMinDistance,
			secondEndProjectPnt);

	float min = 0;
	Vector3 minPoint1, minPoint2;
	sdis2 = secondLineStartToFirstLineMinDistance;
	edis2 = secondLineEndToFirstLineMinDistance;
	sdis1 = firstLineStartToSecondLineMinDistance;
	edis1 = firstLineEndToSecondLineMinDistance;
	if (sdis2 > edis2)
	{
		min = edis2;
		minPoint1 = secondEndProjectPnt;
		minPoint2 = pntEndPoint2;
	}
	else
	{
		min = sdis2;
		minPoint1 = secondStartProjectPnt;
		minPoint2 = pntStartPoint2;
	}

	if (min > sdis1)
	{
		min = sdis1;
		minPoint1 = firstStartProjectPnt;
		minPoint2 = pntStartPoint1;
	}

	if (min > edis1)
	{
		min = edis1;
		minPoint1 = pntEndPoint1;
		minPoint2 = firstEndProjectPnt;
	}

	minDistance = min;
	projectPnt1 = minPoint1;
	projectPnt2 = minPoint2;

	LOGI("MeasureCalculateHelper::SegmentSegmentDiatance END");

	return ret;
}

void MeasureCalculateHelper::PolylinePolylineDistance(RefPolyLine * polyline1,
		RefPolyLine * polyline2, Vector3& projectPnt1, Vector3 & projectPnt2,
		float &minDistance,Matrix3x4 & mat1,Matrix3x4 & mat2)
{
	LOGI("MeasureCalculateHelper::PolylinePolylineDistance BEGIN");
	SPolyLine * xPolyLine1 = polyline1->GetRefLine();
	vector<Vector3> & pntPoints1 = xPolyLine1->GetPoints();
	unsigned int offset1 = polyline1->GetDataOffset();
	unsigned int dataLength1 = polyline1->GetDataLength();

	SPolyLine * xPolyLine2 = polyline2->GetRefLine();
	vector<Vector3> & pntPoints2 = xPolyLine2->GetPoints();
	unsigned int offset2 = polyline2->GetDataOffset();
	unsigned int dataLength2 = polyline2->GetDataLength();

	//将折线1的每条线段与折线2的最小距离进行比较，找出最小值作为折线之间的最小值

	float distance = 100000.0f;
	Vector3 minProjectPnt1, minProjectPnt2;

	for (vector<Vector3>::iterator it1 = pntPoints1.begin() + offset1;
			it1 != pntPoints1.begin() + offset1 + dataLength1; it1 = it1 + 2)
	{

		Vector3 line1s = mat1*(*it1);
		Vector3 line1e = mat1*(*(it1 + 1));
		for (vector<Vector3>::iterator it2 = pntPoints2.begin() + offset2;
				it2 != pntPoints2.begin() + offset2 + dataLength2;
				it2 = it2 + 2)
		{
			float distanceTemp = 0.0f;
			Vector3 projectPntTemp1, projectPntTemp2;
			Vector3 line2s = mat2*(*it2);
			Vector3 line2e = mat2*(*(it2 + 1));

			MeasureCalculateHelper::SegmentSegmentDiatance(line1s, line1e,
					line2s, line2e, projectPntTemp1, projectPntTemp2,
					distanceTemp);
			if (distance > distanceTemp)
			{
				distance = distanceTemp;
				minProjectPnt1 = projectPntTemp1;
				minProjectPnt2 = projectPntTemp2;
			}
		}
	}

	projectPnt1 = minProjectPnt1;
	projectPnt2 = minProjectPnt2;
	minDistance = distance;

	LOGI("MeasureCalculateHelper::PolylinePolylineDistance END");
}

void MeasureCalculateHelper::PolyLineLength(RefPolyLine * lineSet,float & length)
{
	SPolyLine * xPolyLine = lineSet->GetRefLine();
	vector<Vector3> & pntPoints = xPolyLine->GetPoints();
	unsigned int offset = lineSet->GetDataOffset();
	unsigned int dataLength = lineSet->GetDataLength();

	length = 0.0f;
	for (vector<Vector3>::iterator it = pntPoints.begin() + offset;
			it != pntPoints.begin() + offset + dataLength ; it=it+2)
	{
		float distanceTemp;
		Vector3 minPointTemp;

		Vector3 point1 = (*it);
		Vector3 point2 = (*(it + 1));

		length +=(point2 - point1).Length();

	}

}

}


