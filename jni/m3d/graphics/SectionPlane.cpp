/*
 * Plane.cpp
 *
 *  Created on: 2013-11-27
 *      Author: bing
 */

#include "m3d/graphics/SectionPlane.h"
#include "m3d/M3D.h"
#include "m3d/model/Model.h"
#include <math.h>
#include "m3d/base/Quaternion.h"
#include "m3d/base/Matrix3x4.h"
#include "../base/Plane.h"
#include "SectionLineHelper.h"

namespace M3D
{

int SectionPlane::m_MaxId = 0;
void SectionPlane::Init()
{
#ifdef NOCLOTH
	m_sectionLineRoot = new SectionLine;
	m_sectionLineRoot->SetDepth(0);
#endif
	m_scetionLinesDataBase.m_sectionPlane = this;
	m_figureSectonLine.sectionPlane = this;
	this->SetScene(NULL);
}
SectionPlane::SectionPlane():Object()
{

	this->m_Id = m_MaxId++;
	m_GLClipPlaneID = -1;
	m_Enable = false;
//	m_Origin ;
//	m_Normal = NULL;
//	m_UDir = NULL;
//	m_VDir = NULL;

	m_edgeColor = Color::GRAY;
	m_faceColor = Color::GREEN;

	m_faceColor.m_a =0.5f;
	Init();
}

SectionPlane::SectionPlane(const Point3D& origin, const Point3D& normal,const Point3D& uDir,const Point3D& vDir)
{
	this->m_Id = m_MaxId++;
	m_GLClipPlaneID = -1;
	m_Enable = false;
//	m_Origin = NULL;
//	m_Normal = NULL;
//	m_UDir = NULL;
//	m_VDir = NULL;
	m_Origin = origin;
	m_Normal = normal;
	m_UDir = uDir;
	m_VDir = vDir;

	GetParam();
	Init();
}

SectionPlane::SectionPlane(const Point3D& origin, const Point3D& normal)
{
	this->m_Id = m_MaxId++;
	m_GLClipPlaneID = -1;
	m_Enable = false;
//	m_Origin = NULL;
//	m_Normal = NULL;
//	m_UDir = NULL;
//	m_VDir = NULL;
	m_Origin = origin;
	m_Normal = normal;
	GetParam();
	Init();
}


SectionPlane::~SectionPlane()
{
#ifdef NOCLOTH
	MutexLock lock(m_mutex);
	if(m_sectionLineRoot)
	{
		LOGI("=============================delete m_sectionLineRoot");
		delete m_sectionLineRoot;
		LOGI("=============================delete end");
		m_sectionLineRoot = NULL;
	}
#endif
}

Point3D SectionPlane::GetInsectPnt(const Point3D& origin, const Point3D& dir)
{
	Point3D pnt ;
	float vpt = m_Normal.DotProduct(dir);
	if(fabs(vpt) <0.0000001)
	{
		return pnt;
	}
	float t = (m_Origin - origin).DotProduct(m_Normal)/vpt;
	pnt = origin + dir*t;
	return pnt;
}

void SectionPlane::SetTransform(Matrix3x4 & transform)
{
	Plane sectionPlane(Vector3(m_Equation[0], m_Equation[1], m_Equation[2]), m_Equation[3]);
	sectionPlane.Transform(transform);
	float A = sectionPlane.m_normal.m_x;
	float B = sectionPlane.m_normal.m_y;
	float C = sectionPlane.m_normal.m_z;
	float D = sectionPlane.m_d;

	this->SetTransformPlaneParam(A,B,C,D);
	this->m_dirty = true;
	this->UpdateDrawData();
}

void SectionPlane::GetParam()
{
	float A = m_Normal.m_x;
	float B = m_Normal.m_y;
	float C = m_Normal.m_z;
	float D = -(A * m_Origin.m_x + B * m_Origin.m_y + C * m_Origin.m_z);

	m_Equation[0] = A;
	m_Equation[1] = B;
	m_Equation[2] = C;
	m_Equation[3] = D;

	//LOGI("plane:%d normal:%f,%f,%f, origion:%f,%f,%f param:%f,%f,%f,%f", GetID(), m_Normal.x, m_Normal.y, m_Normal.z, m_Origin.x, m_Origin.y, m_Origin.z, A, B, C, D);
}

void SectionPlane::SetPlaneParam(float A, float B, float C, float D)
{
	m_Equation[0] = A;
	m_Equation[1] = B;
	m_Equation[2] = C;
	m_Equation[3] = D;
	Matrix3x4 indenty = Matrix3x4::IDENTITY;
	SetTransform(indenty);
}

void SectionPlane::SetTransformPlaneParam(float A, float B, float C, float D)
{
	m_transformEquation[0] = A;
	m_transformEquation[1] = B;
	m_transformEquation[2] = C;
	m_transformEquation[3] = D;
}
 
void SectionPlane::SetDrawPlane(const Vector3 center,const Vector2 size)
{
	this->m_center = center;
	this->m_size = size;
	this->m_dirty = true;
}

/**
 * TODO
 * @param box
 */
void SectionPlane::SetDrawPlane(const BoundingBox& box)
{
	m_box = box;
	this->m_dirty = true;
	//LOGI("this->UpdateDrawData();");
	this->UpdateDrawData();
}

void SectionPlane::Copy(SectionPlane* sectionPlane)
{
	if(sectionPlane)
	{
		float offset = 0.0f;
		if (m_box.Defined())
		{
			offset = m_box.Length() / 10000;
			if (offset < 0.0001) {
				offset = 0.0001;
			}
		}
       
		m_Equation[0] = sectionPlane-> m_Equation[0];
		m_Equation[1] = sectionPlane-> m_Equation[1];
		m_Equation[2] = sectionPlane-> m_Equation[2];
		m_Equation[3] = sectionPlane-> m_Equation[3] + offset;

		this->m_Id = sectionPlane->m_Id;
		this->m_Enable = sectionPlane->m_Enable;
		this->m_ShowClipPlane = sectionPlane->GetShowClipPlane();
        this->m_ShowCappingPlane = sectionPlane->GetShowCappingPlane();
#ifdef NOCLOTH
		this->m_corssSections = sectionPlane->m_corssSections;
#endif
#ifdef CLOTH
		this->m_figureSectonLine = sectionPlane->m_figureSectonLine;
#endif
	}
}
float* SectionPlane::GetEquation()
{
	return m_Equation;
}

bool SectionPlane::IsPlaneAvaliable()
{
	return fabs(m_Equation[0]) > 0.0001f
		|| fabs(m_Equation[1]) > 0.0001f
		|| fabs(m_Equation[2]) > 0.0001f
		|| fabs(m_Equation[2]) > 0.0001f;
}

Vector3* SectionPlane::GetPointArray()
{
	return m_points.data();
}

Vector3* SectionPlane::GetNormalArray()
{
	return m_normals.data();
}

Vector3* SectionPlane::GetLinePointArray()
{
	return m_edgePoints.data();
}

Color& SectionPlane::GetFaceColor()
{
	return m_faceColor;
}

Color& SectionPlane::GetEdgeColor()
{
	return m_edgeColor;
}

void SectionPlane::UpdateDrawData()
{
	if(this->m_dirty)
	{
		//LOGI("SectionPlane::UpdateDrawData()");
		MutexLock lock(m_mutex);
		//	TODO
		float* equation = this->GetTransformPlaneParam();

		Vector3 normal(equation[0], equation[1], equation[2]);
		//		    LOGI("normal = %s",normal.Normalized().Tostring().c_str());
		Plane projectPlane(normal, equation[3]);
		//			LOGI("equation[3] = %f",equation[3]);

		BoundingBox m_box = m_sceneBox.Projected(projectPlane);
		if (false && this->GetScene())
		{
			this->ClearFigureSectionLines();
			void * data = &(this->m_figureSectonLine); ///获取数据
			SectionLineHelper::DoFigureSectionLine(data, this->GetScene());///经此函数，剖切面中将存储带有数据的sectionline对象
		}
			m_points.clear();///顶点

			m_normals.clear();///法线

			m_uvs.clear();///uv坐标

			m_edgePoints.clear();///边界线顶点
			vector<float> pnts;
			m_box.GetTriangleArray(pnts);
		    Vector3 projMin = m_box.m_min;
		    Vector3 projMax = m_box.m_max;

		//    LOGI("projMin = %s",projMin.Tostring().c_str());
		//    LOGI("projMax = %s",projMax.Tostring().c_str());

		    Vector3 vertices[8];
		    vertices[0] = projMin;
		    vertices[1] = Vector3(projMax.m_x, projMin.m_y, projMin.m_z);
		    vertices[2] = Vector3(projMin.m_x, projMax.m_y, projMin.m_z);
		    vertices[3] = Vector3(projMax.m_x, projMax.m_y, projMin.m_z);
		    vertices[4] = Vector3(projMin.m_x, projMin.m_y, projMax.m_z);
		    vertices[5] = Vector3(projMax.m_x, projMin.m_y, projMax.m_z);
		    vertices[6] = Vector3(projMin.m_x, projMax.m_y, projMax.m_z);
		    vertices[7] = projMax;

			Vector3 normals[6];
			//构造三角形 124 243 213 134
			m_points.push_back(vertices[1]);m_points.push_back(vertices[0]);m_points.push_back(vertices[2]);
			m_points.push_back(vertices[0]);m_points.push_back(vertices[3]);m_points.push_back(vertices[2]);
			normals[0] = ((vertices[3] - vertices[0]).CrossProduct(vertices[1] - vertices[0])).Normalized();

			m_points.push_back(vertices[0]);m_points.push_back(vertices[4]);m_points.push_back(vertices[3]);
			m_points.push_back(vertices[0]);m_points.push_back(vertices[5]);m_points.push_back(vertices[4]);
			normals[1] = Vector3(-1,0,0);

			m_points.push_back(vertices[7]);m_points.push_back(vertices[4]);m_points.push_back(vertices[5]);
			m_points.push_back(vertices[5]);m_points.push_back(vertices[6]);m_points.push_back(vertices[7]);
			normals[2] = -normals[0];

			m_points.push_back(vertices[6]);m_points.push_back(vertices[1]);m_points.push_back(vertices[7]);
			m_points.push_back(vertices[1]);m_points.push_back(vertices[2]);m_points.push_back(vertices[7]);
			normals[3] = -normals[1];

			m_points.push_back(vertices[6]);m_points.push_back(vertices[0]);m_points.push_back(vertices[1]);
			m_points.push_back(vertices[6]);m_points.push_back(vertices[5]);m_points.push_back(vertices[0]);
			normals[4] = ((vertices[1] - vertices[0]).CrossProduct(vertices[5] - vertices[0])).Normalized();

			m_points.push_back(vertices[3]);m_points.push_back(vertices[7]);m_points.push_back(vertices[2]);
			m_points.push_back(vertices[3]);m_points.push_back(vertices[4]);m_points.push_back(vertices[7]);
			normals[5] = -normals[4];


			m_normals.push_back(normals[0]);m_normals.push_back(normals[0]);m_normals.push_back(normals[0]);
			m_normals.push_back(normals[0]);m_normals.push_back(normals[0]);m_normals.push_back(normals[0]);

			m_normals.push_back(normals[1]);m_normals.push_back(normals[1]);m_normals.push_back(normals[1]);
			m_normals.push_back(normals[1]);m_normals.push_back(normals[1]);m_normals.push_back(normals[1]);

			m_normals.push_back(normals[2]); m_normals.push_back(normals[2]); m_normals.push_back(normals[2]);
			m_normals.push_back(normals[2]); m_normals.push_back(normals[2]); m_normals.push_back(normals[2]);

			m_normals.push_back(normals[3]); m_normals.push_back(normals[3]); m_normals.push_back(normals[3]);
			m_normals.push_back(normals[3]); m_normals.push_back(normals[3]); m_normals.push_back(normals[3]);

			m_normals.push_back(normals[4]); m_normals.push_back(normals[4]); m_normals.push_back(normals[4]);
			m_normals.push_back(normals[4]); m_normals.push_back(normals[4]); m_normals.push_back(normals[4]);

			m_normals.push_back(normals[5]); m_normals.push_back(normals[5]); m_normals.push_back(normals[5]);
			m_normals.push_back(normals[5]); m_normals.push_back(normals[5]); m_normals.push_back(normals[5]);

			//构造边界线
			m_edgePoints.push_back(vertices[0]);m_edgePoints.push_back(vertices[1]);
			m_edgePoints.push_back(vertices[1]);m_edgePoints.push_back(vertices[3]);
			m_edgePoints.push_back(vertices[2]);m_edgePoints.push_back(vertices[3]);
			m_edgePoints.push_back(vertices[0]);m_edgePoints.push_back(vertices[2]);

			m_edgePoints.push_back(vertices[4]);m_edgePoints.push_back(vertices[5]);
			m_edgePoints.push_back(vertices[5]);m_edgePoints.push_back(vertices[7]);
			m_edgePoints.push_back(vertices[6]);m_edgePoints.push_back(vertices[7]);
			m_edgePoints.push_back(vertices[4]);m_edgePoints.push_back(vertices[6]);

			m_edgePoints.push_back(vertices[0]);m_edgePoints.push_back(vertices[4]);
			m_edgePoints.push_back(vertices[2]);m_edgePoints.push_back(vertices[6]);
			m_edgePoints.push_back(vertices[3]);m_edgePoints.push_back(vertices[7]);
			m_edgePoints.push_back(vertices[1]);m_edgePoints.push_back(vertices[5]);
		this->m_dirty = false;
	}
}
void SectionPlane::SetName(string name)
{
	m_Name = name;
}

void SectionPlane::SetShowClipPlane(bool flag)
{
	m_ShowClipPlane = flag;
}

bool SectionPlane::GetShowClipPlane()
{
	return m_ShowClipPlane;
}
void SectionPlane::SetShowCappingPlane(bool flag)
{
    m_ShowCappingPlane = flag;
}
    
bool SectionPlane::GetShowCappingPlane()
{
    return m_ShowCappingPlane;
}

void SectionPlane::SetShowPlaneRect(bool flag)
{
	m_ShowPlaneRect = flag;
}

bool SectionPlane::GetShowPlaneRect()
{
	return this->m_ShowPlaneRect;
}

void SectionPlane::SetEnable(bool flag)
{
	m_Enable = flag;
}

void SectionPlane::SetID(int id)
{
	m_Id = id;
}

bool SectionPlane::GetEnable()
{
	return m_Enable;
}

int SectionPlane::GetID()
{
	return m_Id;
}

void  SectionPlane::SetFaceColor(Color & color)
{
	m_faceColor = color;
}

void SectionPlane::ClearSectionLineDataBse()
{
	LOGI("SectionPlane::ClearSectionLineDataBse");
	int vecSize = m_scetionLinesDataBase.m_scetionLinesData.size();
	if (vecSize > 0)
	{
		for (int i = 0; i < vecSize; i++)
		{
			SectionLine * temp = m_scetionLinesDataBase.m_scetionLinesData[i];
			if (temp)
			{
				delete temp;
				temp = NULL;
			}
		}
	}
	 m_scetionLinesDataBase.m_scetionLinesData.clear();

	int sz = m_corssSections.size();
	if (sz > 0)
	{
		for (int i = 0; i < sz; i++)
		{
			CrossSection * temp = m_corssSections[i];
			if (temp)
			{
				delete temp;
				temp = NULL;
			}
		}
		m_corssSections.clear();
	}

	m_sectionLineRoot->GetAllChildren().clear();
}


void SectionPlane::ClearFigureSectionLines()
{
	m_figureSectonLine.linesData.clear();
}
void SectionPlane::GetModelWidthAndLength(void* data)
{
	Model * node = m_figureSectonLine.boundingNode;
	if(node&& data)
	{
		map<string,float>* info = (map<string,float>*)data;
		BoundingBox& shapeNodeBox = node->GetWorldBoundingBox();
		float length = shapeNodeBox.GetXLength();
		float width = shapeNodeBox.GetZLength();
		float height = shapeNodeBox.GetYLength();
		info->insert(std::make_pair("ModelLength",length));
		info->insert(std::make_pair("ModelWidth",width));
		info->insert(std::make_pair("ModelHeight",height));
	}
}
void SectionPlane::GetSectionLineInfo(void* data)
{
	float circumference = 0.f, length = 0.f, width = 0.f;

	BoundingBox box;
	vector<Vector3> & lineDatas = m_figureSectonLine.linesData;
	LOGI("lineDatas.size() = %d",lineDatas.size());
	if (lineDatas.size() > 0 && data)
	{
		map<string,float>* info = (map<string,float>*)data;
		box.Define(&(lineDatas.at(0)), lineDatas.size());
		length = box.GetXLength();
		width = box.GetZLength();
		int size = lineDatas.size();

		for (int i = 0; i < size - 1; i=i+2)
		{
			float sgLength = (lineDatas[i] - lineDatas[i + 1]).Length();
			circumference += sgLength;
		}

		info->insert(std::make_pair("SectionLength",length));
		info->insert(std::make_pair("SectionWidth",width));
		info->insert(std::make_pair("SectionCircumference",circumference));
	}
}


float* SectionPlane::GetTransformPlaneParam()
{
	return this->m_transformEquation;
}

}
