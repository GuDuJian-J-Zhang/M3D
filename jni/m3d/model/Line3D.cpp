#include "Utility.h"
#include "m3d/model/Line3D.h"
#include "m3d/base/BoundingBox.h"
#include "sview/PMIAnalysis/toolkit/PMIESymbol.h"
#include "Stk_CompositeText.h"
#include "m3d/base/Matrix4.h"
#include "m3d/base/Quaternion.h"

namespace M3D
{

void Line3D::Init()
{
	m_StartArrowType = -1;
	m_EndArrowType = -1;
	Color clr = Color::GRAY;
	SetColor(clr);
	m_lineWidth = LINE_WIDTH;
	m_CanDelete = false;
}

Line3D::Line3D()
{
	Init();
}

Line3D::Line3D(Vector3* p1, Vector3* p2)
{
	Init();

	this->SetValue(*p1, *p2);

	ComputeBox();
}

Line3D::Line3D(const Vector3& inStart, const Vector3& inEnd)
{
	Init();
	this->SetValue(inStart, inEnd);
}

Line3D::~Line3D()
{
}

void Line3D::SetValue(const Vector3& inStart, const Vector3& inEnd)
{
	this->m_StartPoint = inStart;
	this->m_EndPoint = inEnd;
}

bool Line3D::GetClosestPoints(const Line3D& line2, Vector3& ptOnThis,
		Vector3& ptOnLine2) const
{
	//LOGI("  Line3D::getClosestPoints1");
	Vector3 p1 = this->m_StartPoint;
	Vector3 p2 = line2.m_StartPoint;

	Vector3 d1 = this->GetDirection();
	Vector3 d2 = line2.GetDirection();

	Vector3 u = p2 - p1;
	float t = d1.DotProduct(d2);
	const float eps = 1.0e-08;
	if (t < -1.0f + eps || t > 1.0f - eps)
	{
		// lines are parallel
		//LOGI("  Line3D::getClosestPoints2");
		return false;
	}
	//LOGI("  Line3D::getClosestPoints2 t is %f", t);
	t = (u.DotProduct(d1) - t * (u.DotProduct(d2))) / (1 - t * t);
	//	LOGI("  Line3D::getClosestPoints2 t is %f", t);
	ptOnThis = p1 + t * d1;
	ptOnLine2 = line2.GetClosestPoint(ptOnThis);
	//	LOGE("ptOnLine2 is %f %f %f", ptOnLine2.x, ptOnLine2.y, ptOnLine2.z);
	return true;
}
Vector3 Line3D::GetClosestPoint(const Vector3& point) const
{
	return this->m_StartPoint
			+ this->GetDirection()
					* (point - this->m_StartPoint).DotProduct(this->GetDirection());
}
const Vector3& Line3D::GetPosition(void) const
{
	return this->m_StartPoint;
}

const Vector3& Line3D::GetEndPoint(void) const
{
	return this->m_EndPoint;
}

const Vector3 Line3D::GetDirection(void) const
{
	Vector3 vec = m_EndPoint - m_StartPoint;
	vec.Normalize();
	return vec;
}
void Line3D::ComputeBox()
{
	m_BoundingBox.Merge(m_StartPoint);
	m_BoundingBox.Merge(m_EndPoint);
	//point
}

void Line3D::SetStartArrow(int startArrow)
{
	m_StartArrowType = startArrow;
	CreateArrowBuffer(m_StartArrowType, m_StartPoint, m_StartPoint - m_EndPoint,
			ARRAW_SIZE, ARRAW_SIZE, m_StartArrowBufferArray);
}

void Line3D::SetEndArrow(int endArrow)
{
	m_EndArrowType = endArrow;
	CreateArrowBuffer(m_EndArrowType, m_EndPoint, m_EndPoint - m_StartPoint,
			ARRAW_SIZE, ARRAW_SIZE, m_EndArrowBufferArray);
}

/**
 * 创建箭头顶点数组缓存
 */
void Line3D::CreateArrowBuffer(int arrowType, Vector3 point, Vector3 direct,
		float width, float height, vector<vector<float> > &outBuf)
{
	CPMIESymbol pmiSym; //创建末端符号的类

	double pnt1[3];
	double refvec[3];
	vector<POINT3> outPnts;
	vector<POINT3> outPnts2;

	if (arrowType != -1)
	{
		//获得末端符号起始点坐标
		pnt1[0] = 0;
		pnt1[1] = 0;
		pnt1[2] = 0;

		//获得末端符号方向向量（需要传入负的向量，可能内部计算有误）
		refvec[0] = -direct.m_x;
		refvec[1] = -direct.m_y;
		refvec[2] = -direct.m_z;

		//bing added for new interface of symbol--------------------------
		double normalVec[3]; //末端符号法线向量？bing added
		double symvec[3]; //temp

		//单位化
		double refsize = sqrt(
				((refvec[0]) * (refvec[0])) + ((refvec[1]) * (refvec[1]))
						+ ((refvec[2]) * (refvec[2])));

		symvec[0] = refvec[0] / refsize;
		symvec[1] = refvec[1] / refsize;
		symvec[2] = refvec[2] / refsize;
		normalVec[0] = (-1) * symvec[1];
		normalVec[1] = symvec[0];
		normalVec[2] = 0;
		//-----------------------------------------end ----------------------

		//生成z=0平面的末端符号数据
		pmiSym.SetESymbolInfo(width, height, pnt1, refvec, normalVec, arrowType,
				outPnts, outPnts2);
//		pmiSym.SetESymbolInfo(width, height, pnt1, refvec, arrowType,
//					outPnts, outPnts2);
//

		//计算从z=0平面变换到直线上需相乘的矩阵
		Matrix4 targetM;
		//1.平移z个单位
		targetM.SetTranslation(Vector3(point.m_x, point.m_y, point.m_z));

		//2.求向量a（x,y,z），b（x,y,0）之间的夹角
		Vector3 a = direct;
		Vector3 b(direct.m_x, direct.m_y, 0);
		float A = acos(
				a.DotProduct(b)/ (a.Length() * b.Length())); //
		float angleA = A * 180 / PI;

		//3.求垂直于a，b的向量c
		Vector3 c = a.CrossProduct(b); //向量叉乘定义

		//		//输出(显示)旋转轴向量
		//		vector<float> cBuf;
		//		cBuf.push_back(point.x);
		//		cBuf.push_back(point.y);
		//		cBuf.push_back(point.z);
		//		cBuf.push_back(point.x + c.x);
		//		cBuf.push_back(point.y + c.y);
		//		cBuf.push_back(point.z + c.z);
		//		outBuf.push_back(cBuf);

		//4.绕向量c旋转A个角度（为何是负角度才正确？）
		targetM = targetM*(Quaternion(-angleA, c));

		//		LOGI("Line3D::createArrowBuffer a:%f,%f,%f  A:%f c:%f,%f,%f"
		//				,a.x,a.y,a.z,angleA,c.x,c.y,c.z);

		// 旋转所有顶点
		for (int i = 0; i < outPnts.size(); i++)
		{
			POINT3* pnt = &outPnts.at(i);
			Vector3 vec(pnt->x, pnt->y, pnt->z);
			vec = targetM*vec;

			//			LOGI("old:%f,%f,%f new :%f,%f,%f"
			//					,pnt->x,pnt->y,pnt->z
			//					,vec.x,vec.y,vec.z);

			pnt->x = vec.m_x;
			pnt->y = vec.m_y;
			pnt->z = vec.m_z;
		}

		if (outPnts2.size() > 0)
		{
			for (int i = 0; i < outPnts2.size(); i++)
			{
				POINT3* pnt = &outPnts2.at(i);
				Vector3 vec(pnt->x, pnt->y, pnt->z);
				vec = targetM*vec;

				//				LOGI("old:%f,%f,%f new :%f,%f,%f"
				//						,pnt->x,pnt->y,pnt->z
				//						,vec.x,vec.y,vec.z);

				pnt->x = vec.m_x;
				pnt->y = vec.m_y;
				pnt->z = vec.m_z;
			}
		}

		//保存顶点数组
		if (outPnts.size() > 0)
		{
			vector<float> tmpList;
			for (int i = 0; i < outPnts.size(); i++)
			{
				POINT3 tmpPnt = outPnts.at(i);
				tmpList.push_back(tmpPnt.x);
				tmpList.push_back(tmpPnt.y);
				tmpList.push_back(tmpPnt.z);
			}
			outBuf.push_back(tmpList);
		}

		if (outPnts2.size() > 0)
		{
			vector<float> tmpList;
			for (int i = 0; i < outPnts2.size(); i++)
			{
				POINT3 tmpPnt = outPnts2.at(i);
				tmpList.push_back(tmpPnt.x);
				tmpList.push_back(tmpPnt.y);
				tmpList.push_back(tmpPnt.z);
			}
			outBuf.push_back(tmpList);
		}
	}
}

const Color & Line3D::GetShapeColor()
{
	return *this->GetColor();
}

void  Line3D::SetLineWidth(int width)
{
	this->m_lineWidth = width;
}
int  Line3D::GetLineWidth()
{
	return this->m_lineWidth;
}

void Line3D::SetCanDelete(bool bValue)
{
	m_CanDelete = bValue;
}
bool Line3D::GetCanDelete()
{
	return m_CanDelete;
}

}

