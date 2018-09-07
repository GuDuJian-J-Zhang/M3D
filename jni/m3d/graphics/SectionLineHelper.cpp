/*
 * SectionLineHelper.cpp
 *
 *  Created on: 2016-5-27
 *      Author: CDJ
 */

#include "m3d/graphics/SectionLineHelper.h"
#include "m3d/graphics/SectionPlane.h"
#include "sview/utils/TriangleLib.h"
#include "m3d/action/CallBackAction.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/base/Plane.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/model/Body.h"
#include "m3d/model/Model.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/model/Face.h"
#include "m3d/model/Face.h"
#include "m3d/model/MeshData.h"
#include "m3d/base/poly2tri/poly2tri.h"
#include "m3d/SceneManager.h"
//using namespace p2t;
#define REAL float
namespace M3D
{

SectionLineHelper::SectionLineHelper()
{
	// TODO Auto-generated constructor stub

}

SectionLineHelper::~SectionLineHelper()
{
	// TODO Auto-generated destructor stub
}

void SectionLineHelper::DoSectionLines(void * data,SceneManager * pScene)
{
	if (pScene && data)
	{
		SectionDataBase * sceneDatas = (SectionDataBase * )data;
		CallBackAction* computeSceneLIneAction = new CallBackAction();
		computeSceneLIneAction->SetActionData(sceneDatas);
		computeSceneLIneAction->SetActionFun(GetSectionLines);
		pScene->ExecuteAction(computeSceneLIneAction);
		delete computeSceneLIneAction;
	}
}

void SectionLineHelper::GetSectionLines(void* data, Model* node)
{

	//	LOGI("begin GetSectionLines");
		if (node != NULL && node->GetType() == SHAPE_MODEL)
		{
			SectionDataBase* sectionData = (SectionDataBase*) data;
			int i = 0;
			float * tempPointer = NULL;
			tempPointer = sectionData->m_sectionPlane->GetTransformPlaneParam();
			BoundingBox& shapeNodeBox = node->GetWorldBoundingBox();
			vector<float> boxVertex; shapeNodeBox.GetVertexs(boxVertex);
			float* points = boxVertex.data();
			float A = tempPointer[0],B = tempPointer[1],C=tempPointer[2],D = tempPointer[3];
			float value0 = A * points[0]+B * points[1]+C * points[2]+D ;
			for(int j = 3;j<24;j =j+3)
			{
				float temp = A* points[j]+B * points[j+1]+C * points[j+2]+D ;
				if(value0*temp<0)
				{
					Model * model = (Model *)node;
					GetInsectModel(data,tempPointer,model);
					break;
				}
			}

		}
	//	LOGI("end GetSectionLines");

}

void SectionLineHelper::GetInsectModel(void* data, const float* factors, Model* model)
{
	if (model)
	{
		{

			Vector4 originFactor(factors[0], factors[1], factors[2], factors[3]);
			Plane clipPlane(originFactor);
			SectionDataBase* sectionDataBase = (SectionDataBase*) data;
			BoundingBox & box = model->GetBoundingBox();

			Matrix3x4 mat = ModelAssemblyHelper::GetWorldMatrix((Model*)model); //TODO

			clipPlane.Transform(mat.Inverse());

			Vector4 transFactor = clipPlane.ToVector4();

			vector<float> boxVertex; box.GetVertexs(boxVertex);
			float* points = boxVertex.data();
			float A = transFactor.m_x;
			float B = transFactor.m_y;
			float C = transFactor.m_z;
			float D = transFactor.m_w;
			float value0 = A * points[0] + B * points[1] + C * points[2] + D;
			for (int j = 3; j < 24; j = j + 3)
			{
				float temp = A * points[j] + B * points[j + 1] + C * points[j + 2] + D;
				if (value0 * temp < 0)
				{
					vector<Body*>* bodies = model->GetBodys();
					int bodiesSize = bodies->size();
					vector<Vector3> lineDatas; //该model中的所有剖切线
//					lineDatas.reserve(20);//预留空间，减少拷贝次数
					//vector<Vector3> *orderLineDatas  = new vector<Vector3>;//创建顺序交点点集
					for (int i = 0; i < bodiesSize; i++)
					{
						GetInsectBody(lineDatas,transFactor.Data(), bodies->at(i));
					}
					//TODO

					vector<Vector3>::reverse_iterator it = lineDatas.rbegin();

					for (; it != lineDatas.rend();)
					{
						vector<Vector3>* orderNum = new vector<Vector3>;
						orderNum->reserve(20);
						orderNum->push_back(lineDatas.at(lineDatas.size() - 1));
						orderNum->push_back(lineDatas.at(lineDatas.size() - 2));

						it = lineDatas.rbegin();
						lineDatas.erase(--(it.base()));

						it = lineDatas.rbegin();
						lineDatas.erase(--(it.base()));

						it = lineDatas.rbegin();

						for (; it != lineDatas.rend() && orderNum->at(0) != ( orderNum->at(orderNum->size() - 1)/*!( Eq(orderNum->at(0), ( orderNum->at(orderNum->size() - 1) ))*/ );)
						{

							if (/*Eq((*it), (orderNum->at(orderNum->size() - 1)))*/(*it) == (orderNum->at(orderNum->size() - 1)))
							{
								if ((it - lineDatas.rbegin()) % 2 == 0)
								{
									orderNum->push_back(*(it + 1));
									it = vector<Vector3>::reverse_iterator(lineDatas.erase(--(it.base())));
									it = vector<Vector3>::reverse_iterator(lineDatas.erase(--(it.base())));

								}
								else
								{
									orderNum->push_back(*(it - 1));
									it = vector<Vector3>::reverse_iterator(lineDatas.erase(--(it.base()) + 1));
									it = vector<Vector3>::reverse_iterator(lineDatas.erase(--(it.base())));

								}

								it = lineDatas.rbegin();
							}
							else
							{
								it++;
							}
						}
						if (/*Eq(orderNum->at(0), (orderNum->at(orderNum->size() - 1) ))*/orderNum->at(0) == (orderNum->at(orderNum->size() - 1) ) && orderNum->size()>4)
						{
							int insectLinesSize = orderNum->size();
							for (int i = 0; i < insectLinesSize; i++)
							{
								Vector4 temp(orderNum->at(i), 1.0);

								orderNum->at(i) = (mat * temp); //转换到世界坐标系
							}
							SectionLine * sectionLine = new SectionLine;
							if(sectionLine)
							{
								LOGI("SectionLine ++");
								sectionLine->SetData(orderNum); ///设置数据
								sectionDataBase->m_scetionLinesData.push_back(sectionLine);///加入总的数据中
							}

						}
						else
						{
							LOGE("section line is not closed");
							delete orderNum;
							orderNum = NULL;
						}
					}
					//TODO
					break;
				}
			}

		}
	}
	//	LOGI("end GetInsectModel");

}

void SectionLineHelper::GetInsectBody(vector<Vector3>& lineDatas, const float* factors, Body* body)
{

	//	LOGI("begin GetInsectBody");
		BoundingBox & box = body->GetBoundingBox();
		vector<float> boxVertex; box.GetVertexs(boxVertex);
		float* points = boxVertex.data();
		float A =factors[0];
		float B = factors[1];
		float C = factors[2];
		float D =factors[3];
		float value0 = A * points[0]+B * points[1]+C * points[2]+D ;
		for(int j = 3;j<24;j =j+3)
		{
			float temp = A* points[j]+B * points[j+1]+C * points[j+2]+D ;
			if(value0*temp<0)
			{
				vector<Face*>&faces = body->GetFaces();
				int facesSize = faces.size();
				for(int i = 0;i<facesSize;i++)
				{
					GetInsectFace(lineDatas,factors,faces[i]);
				}

				break;
			}
		}
	//	LOGI("end GetInsectBody");

}

void SectionLineHelper::GetInsectFace(vector<Vector3>& lineDatas, const float* factors, Face* face)
{

	//	LOGI("begin GetInsectFace");
		BoundingBox & box = face->GetBoundingBox();
		vector<float> boxVertex; box.GetVertexs(boxVertex);
		float* points = boxVertex.data();
		float A =factors[0];
		float B = factors[1];
		float C = factors[2];
		float D =factors[3];
		float value0 = A * points[0]+B * points[1]+C * points[2]+D ;
		for(int j = 3;j<24;j =j+3)
		{
			float temp = A* points[j]+B * points[j+1]+C * points[j+2]+D ;
			if(value0*temp<0)
			{
				//TODO
				Mesh * mesh = face->GetData();//(Mesh*)(((Face*)face)->GetData());
				vector<Vector3> tempLineData;
				GetInsectMesh(tempLineData, factors, mesh);

				if (tempLineData.size() > 0)
				{
					lineDatas.insert(lineDatas.end(), tempLineData.begin(), tempLineData.end());
				}

				break;
			}
		}
	//	LOGI("end GetInsectFace");

}

int SectionLineHelper::IsPointInPlane(const float * factors, Vector3& point)
{
	float A = factors[0];
	float B = factors[1];
	float C = factors[2];
	float D = factors[3];
	Vector3 normal(A, B, C);
	Plane plane(normal, D);
	int ret = 0;

	float value = A*point.m_x + B * point.m_y + C * point.m_z+D;
	if (value>1e-8)
	{
		return 1;
	}
	else if (value>-1e-8 && value<1e-8)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void SectionLineHelper::GetInsectMesh(vector<Vector3>& lineData,const float* factors, Mesh* mesh)
{
	VertexSet * pSet = mesh->GetRefMesh();
	unsigned int dataOffset = mesh->GetDataOffset();
	unsigned int dataLength = mesh->GetDataLength();
	vector<Vector3>* positionArray = pSet->GetPositionArray();
	float A = factors[0];
	float B = factors[1];
	float C = factors[2];
	float D = factors[3];
	Vector3 normal(A, B, C);
	Plane plane(normal, D);
	if (pSet->IsUseIndex())
	{
		vector<M3D_INDEX_TYPE>* indexArray = pSet->GetIndexArray();

		for (vector<M3D_INDEX_TYPE>::iterator it = indexArray->begin() + dataOffset;
			it != indexArray->begin() + dataOffset + dataLength; it = it + 3)
		{
			Vector3 points[3];

			points[0] = positionArray->at(*it);
			points[1] = positionArray->at(*(it + 1));
			points[2] = positionArray->at(*(it + 2));

			Vector3 * pPoints[3];
			pPoints[0] = &(positionArray->at(*it));
			pPoints[1] = &(positionArray->at(*(it + 1)));
			pPoints[2] = &(positionArray->at(*(it + 2)));

			int mark[3];
			mark[0] = IsPointInPlane(factors, points[0]);
			mark[1] = IsPointInPlane(factors, points[1]);
			mark[2] = IsPointInPlane(factors, points[2]);
			if (mark[0]==0 || mark[1] ==0 ||mark[2] == 0)
			{
				if (mark[0] == 0 && mark[1] * mark[2] == -1)
				{
					Ray ray1(points[2],points[1] - points[2]);
					Vector3 insect1;
					bool bret1 = plane.GetInsectPnt(ray1, insect1);
					if (bret1)
					{
						lineData.push_back(insect1);
						lineData.push_back(points[0]);
					}
				}
				else if (mark[1] == 0 && mark[0] * mark[2] == -1)
				{
					Ray ray1(points[2], points[0] - points[2]);
					Vector3 insect1;
					bool bret1 = plane.GetInsectPnt(ray1, insect1);
					if (bret1)
					{
						lineData.push_back(insect1);
						lineData.push_back(points[1]);
					}
				}
				else if (mark[2] == 0 && mark[0] * mark[1] == -1)
				{
					Ray ray1(points[1], points[0] - points[1]);
					Vector3 insect1;
					bool bret1 = plane.GetInsectPnt(ray1, insect1);
					if (bret1)
					{
						lineData.push_back(insect1);
						lineData.push_back(points[2]);
					}
				}
				else if (mark[0] == 0 && mark[1] == 0 && mark[2] != 0)
				{
					lineData.push_back(points[0]);
					lineData.push_back(points[1]);
				}
				else if (mark[0] == 0 && mark[2] == 0 && mark[1] != 0)
				{
					lineData.push_back(points[0]);
					lineData.push_back(points[2]);
				}
				else if (mark[1] == 0 && mark[2] == 0 && mark[0] != 0)
				{
					lineData.push_back(points[1]);
					lineData.push_back(points[2]);
				}
			}
			else if (mark[0] != 0 && mark[1] != 0 && mark[2] != 0)
			{
				if (mark[0]+mark[1] == 0 && mark[0]+mark[2] == 0)
				{
					Ray ray1(points[0], points[1] - points[0]);
					Ray ray2(points[0], points[2] - points[0]);
					Vector3 insect1, insect2;
					if (plane.GetInsectPnt(ray1, insect1) && plane.GetInsectPnt(ray2,insect2))
					{
						lineData.push_back(insect1);
						lineData.push_back(insect2);
					}
				}
				else if (mark[1] + mark[0] ==0 && mark[1] + mark[2] == 0)
				{
					Ray ray1(points[1], points[0] - points[1]);
					Ray ray2(points[1], points[2] - points[1]);
					Vector3 insect1, insect2;
					if (plane.GetInsectPnt(ray1, insect1) && plane.GetInsectPnt(ray2, insect2))
					{
						lineData.push_back(insect1);
						lineData.push_back(insect2);
					}
				}
				else if (mark[2] + mark[0] == 0 && mark[2] + mark[1] == 0)
				{
					Ray ray1(points[2], points[0] - points[2]);
					Ray ray2(points[2], points[1] - points[2]);
					Vector3 insect1, insect2;
					if (plane.GetInsectPnt(ray1, insect1) && plane.GetInsectPnt(ray2, insect2))
					{
						lineData.push_back(insect1);
						lineData.push_back(insect2);
					}
				}
			}

		}
	}
	else
	{
		for (vector<Vector3>::iterator it = positionArray->begin() + dataOffset;
			it != positionArray->begin() + dataOffset + dataLength; it = it + 3)
		{

			Vector3 points[3];
			points[0] = (*it);
			points[1] = (*(it + 1));
			points[2] = (*(it + 2));

			Vector3 * pPoints[3];
			pPoints[0] = &((*it));
			pPoints[1] = &((*(it + 1)));
			pPoints[2] = &((*(it + 2)));
			int mark[3];
			mark[0] = IsPointInPlane(factors, points[0]);
			mark[1] = IsPointInPlane(factors, points[1]);
			mark[2] = IsPointInPlane(factors, points[2]);
			if (mark[0] == 0 || mark[1] == 0 || mark[2] == 0)
			{
				if (mark[0] == 0 && mark[1] * mark[2] == -1)
				{
					Ray ray1(points[2], points[1] - points[2]);
					Vector3 insect1;
					bool bret1 = plane.GetInsectPnt(ray1, insect1);
					if (bret1)
					{
						lineData.push_back(insect1);
						lineData.push_back(points[0]);
					}
				}
				else if (mark[1] == 0 && mark[0] * mark[2] == -1)
				{
					Ray ray1(points[2], points[0] - points[2]);
					Vector3 insect1;
					bool bret1 = plane.GetInsectPnt(ray1, insect1);
					if (bret1)
					{
						lineData.push_back(insect1);
						lineData.push_back(points[1]);
					}
				}
				else if (mark[2] == 0 && mark[0] * mark[1] == -1)
				{
					Ray ray1(points[1], points[0] - points[1]);
					Vector3 insect1;
					bool bret1 = plane.GetInsectPnt(ray1, insect1);
					if (bret1)
					{
						lineData.push_back(insect1);
						lineData.push_back(points[2]);
					}
				}
				else if (mark[0] == 0 && mark[1] == 0 && mark[2] != 0)
				{
					lineData.push_back(points[0]);
					lineData.push_back(points[1]);
				}
				else if (mark[0] == 0 && mark[2] == 0 && mark[1] != 0)
				{
					lineData.push_back(points[0]);
					lineData.push_back(points[2]);
				}
				else if (mark[1] == 0 && mark[2] == 0 && mark[0] != 0)
				{
					lineData.push_back(points[1]);
					lineData.push_back(points[2]);
				}
			}
			else if (mark[0] != 0 && mark[1] != 0 && mark[2] != 0)
			{
				if (mark[0] + mark[1] == 0 && mark[0] + mark[2] == 0)
				{
					Ray ray1(points[0], points[1] - points[0]);
					Ray ray2(points[0], points[2] - points[0]);
					Vector3 insect1, insect2;
					if (plane.GetInsectPnt(ray1, insect1) && plane.GetInsectPnt(ray2, insect2))
					{
						lineData.push_back(insect1);
						lineData.push_back(insect2);
					}
				}
				else if (mark[1] + mark[0] == 0 && mark[1] + mark[2] == 0)
				{
					Ray ray1(points[1], points[0] - points[1]);
					Ray ray2(points[1], points[2] - points[1]);
					Vector3 insect1, insect2;
					if (plane.GetInsectPnt(ray1, insect1) && plane.GetInsectPnt(ray2, insect2))
					{
						lineData.push_back(insect1);
						lineData.push_back(insect2);
					}
				}
				else if (mark[2] + mark[0] == 0 && mark[2] + mark[1] == 0)
				{
					Ray ray1(points[2], points[0] - points[2]);
					Ray ray2(points[2], points[1] - points[2]);
					Vector3 insect1, insect2;
					if (plane.GetInsectPnt(ray1, insect1) && plane.GetInsectPnt(ray2, insect2))
					{
						lineData.push_back(insect1);
						lineData.push_back(insect2);
					}
				}
			}
		}

	}
}

Vector3 SectionLineHelper::GetPlaneInsetPoint(Vector3& planeNormal, Vector3& planePoint, Vector3& lineVector,
		Vector3& linePoint)
{
	Vector3 returnResult;
	double vp1, vp2, vp3, n1, n2, n3, v1, v2, v3, m1, m2, m3, t, vpt;
	vp1 = planeNormal.m_x;
	vp2 = planeNormal.m_y;
	vp3 = planeNormal.m_z;
	n1 = planePoint.m_x;
	n2 = planePoint.m_y;
	n3 = planePoint.m_z;
	v1 = lineVector.m_x;
	v2 = lineVector.m_y;
	v3 = lineVector.m_z;
	m1 = linePoint.m_x;
	m2 = linePoint.m_y;
	m3 = linePoint.m_z;
	vpt = v1 * vp1 + v2 * vp2 + v3 * vp3;
	//首先判断直线是否与平面平行
	if (vpt == 0)
	{
		return returnResult;
	}
	else
	{
		t = ((n1 - m1) * vp1 + (n2 - m2) * vp2 + (n3 - m3) * vp3) / vpt;
		returnResult.m_x = m1 + v1 * t;
		returnResult.m_y = m2 + v2 * t;
		returnResult.m_z = m3 + v3 * t;
//		returnResult.m_x = (int)(returnResult.m_x*100000)/100000.0f;
//		returnResult.m_y = (int)(returnResult.m_y*100000)/100000.0f;
//		returnResult.m_z = (int)(returnResult.m_z*100000)/100000.0f;
//		LOGE("returnResult = %s",returnResult.Tostring().c_str());
	}
	return returnResult;
}

bool SectionLineHelper::Eq(Vector3& v1, Vector3& v2)
{

	float M_EPSILON = 1e-6f;///精度
	return (v1.m_x + M_EPSILON >= v2.m_x  && v1.m_x  - M_EPSILON <= v2.m_x ) &&
			(v1.m_y + M_EPSILON >= v2.m_y && v1.m_y  - M_EPSILON <= v2.m_y ) &&
			(v1.m_z + M_EPSILON >= v2.m_z  && v1.m_z  - M_EPSILON <= v2.m_z );

}

Vector3 SectionLineHelper::CalculatePntsCenter(const std::vector<Vector3>* inPnts)
{
	LOGI("SectionLineHelper::CalculatePntsCenter Begin");
	Vector3  centerPnt(0.0,0.0,0.0);
	int pntNum = (int)inPnts->size();
	for ( int i=0;i<pntNum;i++)
	{
		centerPnt = centerPnt + inPnts->at(i);
	}
	//必须要除以点的个数
	centerPnt.m_x=centerPnt.m_x/(double)pntNum;
	centerPnt.m_y=centerPnt.m_y/(double)pntNum;
	centerPnt.m_z=centerPnt.m_z/(double)pntNum;
	LOGI("SectionLineHelper::CalculatePntsCenter end");
	return centerPnt;
}

bool SectionLineHelper::TransformTo2D(SectionPlane* pCurSectionPlane)
{
	bool ret = true;
	///获取数据
	SectionDataBase * sectionDataBase = &pCurSectionPlane->m_scetionLinesDataBase;

	vector<SectionLine*> & allSection = sectionDataBase->m_scetionLinesData;
	if(allSection.size() == 0)
	{
		ret = false;
		return ret;
	}

	///获取法线
	float * eauqtion = pCurSectionPlane->GetTransformPlaneParam();
	Vector3 normal(eauqtion[0], eauqtion[1], eauqtion[2]);
	normal.Normalize();

	LOGE("plane normal is %s",normal.Tostring().c_str());

	///获取所有的剖切线重心
	vector<Vector3> centers;
	int secsSize = static_cast<int>(allSection.size());
	LOGI("secsSize= %d",secsSize);
	for (int i = 0; i < secsSize; i++)
	{
		LOGI("allSection[%d] == %p",i,allSection[i]);
		vector<Vector3> * pTemp = allSection[i]->GetData();
		LOGI("pTemp = %p",pTemp);
		Vector3 centerPnt = CalculatePntsCenter(pTemp);
		LOGI("CalculatePntsCenter END");
		centers.push_back(centerPnt);
	}

	///获取重心的重心
	if(centers.size()>=2)
	{
		Vector3 center = CalculatePntsCenter(&centers);
	sectionDataBase->m_center = center;
	}
	else
	{
		sectionDataBase->m_center = centers[0];
	}

	LOGI("begin quat");
	Vector3 End(0, 0, 1); ///要转换到的平面法向量，此处为XY平面
	Quaternion tempRotate(normal, End);
	sectionDataBase->m_rotate= tempRotate; ///得到旋转四元数
	LOGI("m_rotate = %s",sectionDataBase->m_rotate.Tostring().c_str());
	LOGI("end quat");
	///将每一个剖切线中的每点都转换到Z平面
	for (int j = 0; j < secsSize; j++)
	{
		LOGI("begin allSection");

		vector<Vector3> * pVec = allSection[j]->GetData();
		int size = pVec->size();

		LOGI("size = %d",size);
		///每个剖切线中每一点
		for (int i= 0; i < size; i++)
		{
			pVec->at(i) = sectionDataBase->m_rotate * (pVec->at(i) - sectionDataBase->m_center); ///需要移到原点
		}
		LOGI("zhuan huan hou de zhi %s",pVec->at(0).Tostring().c_str());
		LOGI("beigin box");
		///已经转换到平面，可以计算包围盒了
		allSection[j]->CaculateBoundingBox(); ///计算包围盒

		LOGE("Section allSection[%d] BoundingBox is %s ",j,allSection[j]->GetBoundingBox().Tostring().c_str());
		LOGI("shangxia");
		///初始化剖切线上下级关系
		LOGI("pCurSectionPlane->m_sectionLineRoot =%p ",pCurSectionPlane->m_sectionLineRoot);

		SectionLine * rootSLine = pCurSectionPlane->m_sectionLineRoot;
		if(!rootSLine)
		{
			ret = false;
			return ret;
		}
		rootSLine->AddChild(allSection[j]);
		allSection[j]->SetParent(rootSLine);
		LOGI("end allSection");
	}

	LOGI("Trans to 2d end");
	return ret;
}

///求两点的垂直平分线的方向向量
Vector3 SectionLineHelper::GetMidnormal(Vector3& p1,Vector3& p2)
{
	Vector3 middle = (p1+p2)/2.0f;
	Vector3 direction = p2-p1;
	Vector3 normal = direction.CrossProduct(Vector3::FORWARD);
    normal.Normalize();
    return normal;
}

///求某一点是否在多边形内部
bool SectionLineHelper::PointIn2DPolygon( Vector3 point,const std::vector<Vector3>& vnodes)
{
    bool isInside = false;
    int i = 0;
    int j = 0;

    int pntNum = static_cast<int>(vnodes.size());
    Vector3 u0,u1;
    for (i = 0, j = pntNum-1;i< pntNum;j=i,i++)
    {
        u0 = vnodes[i];
        u1 = vnodes[j];
        if ((u0.m_y<= point.m_y && point.m_y <u1.m_y) ||
            (u1.m_y <= point.m_y && point.m_y < u0.m_y))
        {
            double x = u0.m_x + (point.m_y - u0.m_y)*(u1.m_x - u0.m_x)/(u1.m_y - u0.m_y);
            if (x>point.m_x)
            {
                isInside = !isInside;
            }
        }
    }

    return isInside;
}

///求出多边形内部一点
Vector3 SectionLineHelper::CalculatePointIn2DPloygon(const std::vector<Vector3>& vnodes)
{
    int pntSize = static_cast<int>(vnodes.size());
    Vector3 ret(0,0,0);
    for(int i = 0;i<pntSize-1;i++)
    {
    	Vector3 p1 = vnodes[i];
    	Vector3 p2 = vnodes[i+1];
    	Vector3 middle = (p1+p2)/2.0;
    	Vector3 normal  = GetMidnormal(p1,p2);
         //qDebug()<<"normal = " << normal;
    	Vector3 testP1 = normal*0.001+middle;
    	Vector3 testP2 = -normal*0.001+middle;
        bool bp1 = PointIn2DPolygon(testP1,vnodes);
        if(bp1)
        {
            ret =  testP1;
            break;
        }
        else
        {
            bool bp2 = PointIn2DPolygon(testP2,vnodes);
            if(bp2)
            {
                 ret = testP2;
                 break;
            }
        }
    }
    return ret;
}

///将所有的section排序升级。使用的时候要建立一个虚拟的sectionLine作为根，
///将所有剖切线作为其孩子，然后将其传入函数。
///
///		假设 A 包含BC ，C包含 B；当它们按照C B A 顺序存储在S1(程序中的temp)时，并建立虚拟根节点R，将他们设为孩子。先拿A依次与B C是否在内部测试，结果A不在B、C
///内部；将A在S1中除去，并放置在S2（程序中的sortV）;S1遍历完后，再拿B依次测试；结果B不包含C；将B在S1中移除；然后将B再和S2
///中的元素比较：B在A中，将B作为A孩子关联，不用再存入S2中；同理，C加入到A孩子中；然后，S2中存储了真正的R的孩子；然后递归处理R中有孩子的孩子。
///
///
///  内部增加准备盖面数据准备逻辑
void SectionLineHelper::SortSectionLines(SectionLine * sectionLine,void * outData)
{
	LOGI("SectionLineHelper::SortSectionLines Begin");
	vector<SectionLine*> &temp = sectionLine->GetAllChildren();
	vector<SectionLine*> sortV;
	int childSize = temp.size();
	LOGI("childSize == %d",childSize);
	if (childSize > 0)
	{
		vector<SectionLine*>::reverse_iterator it = temp.rbegin();
		for (; it != temp.rend();)
		{
			SectionLine * sec = *it;
			LOGI("sec size == %d",sec->GetData()->size());
			int count = 0;
			it++;
			for (; it != temp.rend(); it++)
			{
				SectionLine * linshi = *it;
				if (sec->IsInside(linshi))
				{
					LOGI("S1 inside");
					count++;
					(*it)->AddChild(sec);
					temp.erase(temp.end() - 1);
					it = temp.rbegin();
					break;
				}
			}

			if (!count)
			{
				if (sortV.size() > 0)
				{
					int cnt = 0;
					for (int i = 0; i < sortV.size(); i++)
					{

						if (sec->IsInside(sortV[i]))
						{
							LOGI("S2 inside");
							cnt++;
							sortV[i]->AddChild(sec);
							sec->SetParent(sortV[i]);
							break;
						}
					}

					if (!cnt)
					{
						sortV.push_back(sec);
					}
				}
				else
				{
					LOGI("in S2");
					sortV.push_back(sec);
				}

				temp.erase(temp.end() - 1);
				it = temp.rbegin();
			}
		}
	}
//	LOGI("end for");
	///sortV中存的才是其真正的孩子
	sectionLine->GetAllChildren().clear();

	///获取横截面容器
	vector<CrossSection*>* crossSections = (vector<CrossSection*>*) outData;

//	LOGI("begin get group");
	if (sectionLine->GetDepth() % 2 == 1)
	{
		///当剖切线所在深度为奇数层时，说明其为外部边界，和其孩子剖切线构成内外环，来生成横截面
		CrossSection * pCroS = new CrossSection;

		pCroS->m_outer = sectionLine; ///外环

		LOGI("CrossSection %p m_outer is %p",pCroS,sectionLine);
		for (int i = 0; i < sortV.size(); i++)
		{
			sectionLine->AddChild(sortV[i]);

			sortV[i]->SetParent(sectionLine);

			pCroS->m_inner.push_back(sortV[i]); ///加入内环边界容器
		}

		///TODO 此处要写准备传入triangle中的数据的代码

		crossSections->push_back(pCroS);
	}
	else
	{
		for (int i = 0; i < sortV.size(); i++)
		{
			sectionLine->AddChild(sortV[i]);
			sortV[i]->SetParent(sectionLine);
		}
	}

//	LOGI("end get group");
	///递归。将所有的剖切线插入树中
	for (int i = 0; i < sectionLine->GetAllChildren().size(); i++)
	{
		sectionLine->GetAllChildren()[i]->SetDepth(sectionLine->GetDepth() + 1); ///设置所在树的深度位置

		SortSectionLines(sectionLine->GetAllChildren()[i], outData);
	}

	LOGI("SectionLineHelper::SortSectionLines END");
}

void SectionLineHelper::PrepareTriangulationData(vector<CrossSection*>& crossSections)
{
	LOGI("SectionLineHelper::PrepareTriangulationData Begin");
	int vSize = static_cast<int>(crossSections.size());
	for (int csIndex = 0; csIndex < vSize; csIndex++)
	{
		CrossSection * pCroS = crossSections[csIndex];
		if (pCroS)
		{
//			LOGI("Begin outer");
			{
				SectionLine * outer = pCroS->m_outer;
				vector<Vector3> * outerLineData = outer->GetData();
				for (int otrIndex = 0; otrIndex < outerLineData->size() - 1; otrIndex++)
				{
					pCroS->m_Points.push_back(outerLineData->at(otrIndex));
					pCroS->m_segmentIndex.push_back(otrIndex);
					if (otrIndex == outerLineData->size() - 2)
					{
						pCroS->m_segmentIndex.push_back(0);
					}
					else
					{
						pCroS->m_segmentIndex.push_back(otrIndex + 1);
					}
				}
			}
//			LOGI("end outer");

//			LOGI("Begin inner");
			vector<SectionLine *>& inners = pCroS->m_inner;
//			LOGI("inners.size() = %d",inners.size());
			if (inners.size() > 0)
			{
				for (int inrsIndex = 0; inrsIndex < inners.size(); inrsIndex++)
				{
					SectionLine * inner = inners[inrsIndex];
					vector<Vector3> * innerLineData = inner->GetData();
					pCroS->m_holes.push_back(CalculatePointIn2DPloygon(*innerLineData)); ///计算洞内点

					int currentCount = pCroS->m_Points.size();
					for (int inrIndex = 0; inrIndex < innerLineData->size() - 1; inrIndex++)
					{
						pCroS->m_Points.push_back(innerLineData->at(inrIndex));
						pCroS->m_segmentIndex.push_back(inrIndex + currentCount);
						if (inrIndex == innerLineData->size() - 2)
						{
							pCroS->m_segmentIndex.push_back(currentCount);
						}
						else
						{
							pCroS->m_segmentIndex.push_back(inrIndex + 1);
						}
					}
				}
			}
//			LOGI("end inner");
		}
	}
	LOGI("SectionLineHelper::PrepareTriangulationData END");
}

bool SectionLineHelper::PolyToTriTriangulation(vector<CrossSection*>& crossSections,SectionDataBase& dataBase)
{
    
//	LOGI("SectionLineHelper::PolyToTriTriangulation Begin");
//	bool ret = true;
//	Vector3 center = dataBase.m_center;
//	Quaternion rotate = dataBase.m_rotate;
//	Quaternion invRotate = rotate.Inverse();
//    /// Constrained triangles
//
//	int vSize = static_cast<int>(crossSections.size());
//	for (int csIndex = 0; csIndex < vSize; csIndex++)
//	{
//	    vector<p2t::Triangle*> triangles;
//	    vector<p2t::Point*> polyline;
//	    vector< vector<p2t::Point*> > polylines;
//		CrossSection * pCroS = crossSections[csIndex];
//		if (pCroS)
//		{
////			LOGI("Begin outer");
//			{
//				SectionLine * outer = pCroS->m_outer;
//				vector<Vector3> * outerLineData = outer->GetData();
//				for (int otrIndex = 0; otrIndex < outerLineData->size() - 1; otrIndex++)
//				{
//					if(/*Eq(outerLineData->at(otrIndex),outerLineData->at(otrIndex+1))*/outerLineData->at(otrIndex) == outerLineData->at(otrIndex+1))
//					{
//						LOGE("p1 = %s,P2 = %s",outerLineData->at(otrIndex).Tostring().c_str(),outerLineData->at(otrIndex+1).Tostring().c_str());
//						LOGE("can not repeat point");
//						continue;
//					}
//					polyline.push_back(new p2t::Point(outerLineData->at(otrIndex).m_x,outerLineData->at(otrIndex).m_y));
//					outerLineData->at(otrIndex) = invRotate * outerLineData->at(otrIndex)+center;
//				}
//				outerLineData->at(outerLineData->size() - 1) = invRotate * outerLineData->at(outerLineData->size() - 1)+center;
//				polylines.push_back(polyline);
//			}
//
//			if(polyline.size() == 0)
//			{
//				LOGE("cross section outer is null. false");
//				ret = false;
//				return ret;
//			}
//
//			p2t::CDT* cdt = new p2t::CDT(polyline);
////			LOGI("end outer");
//
////			LOGI("Begin inner");
//			vector<SectionLine *>& inners = pCroS->m_inner;
////			LOGI("inners.size() = %d",inners.size());
//			if (inners.size() > 0)
//			{
//				for (int inrsIndex = 0; inrsIndex < inners.size(); inrsIndex++)
//				{
//					vector<p2t::Point*> tempHole;
//
//					SectionLine * inner = inners[inrsIndex];
//					vector<Vector3> * innerLineData = inner->GetData();
//					for (int inrIndex = 0; inrIndex < innerLineData->size() - 1; inrIndex++)
//					{
//						if(/*Eq(innerLineData->at(inrIndex),innerLineData->at(inrIndex+1))*/innerLineData->at(inrIndex) ==innerLineData->at(inrIndex+1))
//						{
//							LOGE("p1 = %s,P2 = %s",innerLineData->at(inrIndex).Tostring().c_str(),innerLineData->at(inrIndex+1).Tostring().c_str());
//
//							LOGE("can not repeat point");
//							continue;
//						}
//						tempHole.push_back(new p2t::Point(innerLineData->at(inrIndex).m_x,innerLineData->at(inrIndex).m_y));
//						innerLineData->at(inrIndex) = invRotate * innerLineData->at(inrIndex)+center;
//					}
//					cdt->AddHole(tempHole);
//					innerLineData->at(innerLineData->size() - 1) = invRotate * innerLineData->at(innerLineData->size() - 1)+center;
//
//					polylines.push_back(tempHole);
//				}
//			}
//			else
//			{
//				LOGE("this cross section inner line is null");
//			}
//
//			LOGI("beign trigulate");
//			cdt->Triangulate();
//			LOGI("end trigulate");
//			triangles = cdt->GetTriangles();///得到该crosssection的剖分顶点坐标值
//			///将剖分顶点结果存入crosssection中
//
//			int triVerSize =  triangles.size();
//			for (int i = 0; i < triVerSize; i++)
//			{
//				p2t::Point* a = triangles[i]->GetPoint(0);
//				Vector3 p1(a->x,a->y,0.0);
//				p1 = invRotate * p1+center;
//
//				p2t::Point* b = triangles[i]->GetPoint(1);
//				Vector3 p2(b->x,b->y,0.0);
//				p2 = invRotate * p2+center;
//
//				p2t::Point* c = triangles[i]->GetPoint(2);
//				Vector3 p3(c->x,c->y,0.0);
//				p3 = invRotate * p3+center;
//
//				pCroS->m_trianglesVertex.push_back(p1);
//				pCroS->m_trianglesVertex.push_back(p2);
//				pCroS->m_trianglesVertex.push_back(p3);
//			}
//
//			///清除临时数据
//			delete cdt;
//			cdt = NULL;
//			for (int i = 0; i < polylines.size(); i++)
//			{
//				vector<p2t::Point*> &poly = polylines[i];
//				FreeClear(poly);
//			}
////			LOGI("end inner");
//		}
//	}
//	LOGI("SectionLineHelper::PolyToTriTriangulation END");
//	return ret;
    return true;

}

///调用Triangle库
void SectionLineHelper::SectionLinesTriangulation(vector<CrossSection*>& crossSections,SectionDataBase& dataBase)
{
	LOGI("SectionLineHelper::SectionLinesTriangulation Begin");
	Vector3 center = dataBase.m_center;
	Quaternion rotate = dataBase.m_rotate;
	Quaternion invRotate = rotate.Inverse();
	int vSize = static_cast<int>(crossSections.size());
	for(int index = 0;index<vSize;index++)
	{
		CrossSection* crossSection = crossSections[index];

		if(crossSection)
		{
//			struct triangulateio in, mid;
//			in.numberofpoints = crossSection->m_Points.size();
//		    in.numberofpointattributes = 0;
//		    in.pointlist = (REAL *)malloc(in.numberofpoints * 2 * sizeof(REAL));
//		    for(int i = 0;i <in.numberofpoints; i++)
//		    {
//		    	in.pointlist[i*2+0] = crossSection->m_Points[i].m_x;
//		    	in.pointlist[i*2+1] = crossSection->m_Points[i].m_y;
//		    }
//
//		    in.pointattributelist = NULL;
//
//		    in.pointmarkerlist = NULL;
//
//		    in.numberofholes = crossSection->m_holes.size();
//		    LOGE("=========================m_holes size = %d",in.numberofholes);
//		    if(in.numberofholes>0)
//		    {
//		    in.holelist = (REAL *)malloc(in.numberofholes * 2 * sizeof(REAL));
//		    for(int i = 0;i <in.numberofholes; i++)
//		    {
//		    	in.holelist[i*2+0] = crossSection->m_holes[i].m_x;
//		    	in.holelist[i*2+1] = crossSection->m_holes[i].m_y;
//		    }
//		    }
//
//		    in.numberofregions = 0;
////		    in.regionlist = (REAL *)malloc(in.numberofregions * 4 * sizeof(REAL));
////		    in.regionlist[0] = 0.5;
////		    in.regionlist[1] = 5.0;
////		    in.regionlist[2] = 7.0;            /* Regional attribute (for whole mesh). */
////		    in.regionlist[3] = 0.1;          /* Area constraint that will not be used. */
//
//
//		    in.numberofsegments = crossSection->m_segmentIndex.size();
//		    in.segmentlist = (int *)malloc(in.numberofsegments * 2 * sizeof(int));
//		    for(int i = 0;i< in.numberofsegments ;i++)
//		    {
//		    	in.segmentlist[i] =  crossSection->m_segmentIndex[i];
//		    }
//		    in.segmentmarkerlist = NULL;
//
//		    /* Make necessary initializations so that Triangle can return a */
//		     /*   triangulation in `mid' and a voronoi diagram in `vorout'.  */
//
//		     mid.pointlist = (REAL *)NULL;            /* Not needed if -N switch used. */
//		     /* Not needed if -N switch used or number of point attributes is zero: */
//		     mid.pointattributelist = (REAL *)NULL;
//		     mid.pointmarkerlist = (int *)NULL; /* Not needed if -N or -B switch used. */
//		     mid.trianglelist = (int *)NULL;          /* Not needed if -E switch used. */
//		     /* Not needed if -E switch used or number of triangle attributes is zero: */
//		     mid.triangleattributelist = (REAL *)NULL;
//		     mid.neighborlist = (int *)NULL;         /* Needed only if -n switch used. */
//		     /* Needed only if segments are output (-p or -c) and -P not used: */
//		     mid.segmentlist = (int *)NULL;
//		     /* Needed only if segments are output (-p or -c) and -P and -B not used: */
//		     mid.segmentmarkerlist = (int *)NULL;
//		     mid.edgelist = (int *)NULL;             /* Needed only if -e switch used. */
//		     mid.edgemarkerlist = (int *)NULL;   /* Needed if -e used and -B not used. */
//
//		     /* Triangulate the points.  Switches are chosen to read and write a  */
//		     /*   PSLG (p), preserve the convex hull (c), number everything from  */
//		     /*   zero (z), assign a regional attribute to each element (A), and  */
//		     /*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
//		     /*   neighbor list (n).                                              */
//
//		     char pars []  = "pcz";
//		     triangulate(pars, &in, &mid, NULL);//原始pczAevn
//
//		     for(int i = 0;i<mid.numberoftriangles;i++)
//		     {
//		         for(int j = 0;j<mid.numberofcorners;j++)
//		         {
//		        	 crossSection->m_triangleIndex .push_back(mid.trianglelist[i*mid.numberofcorners+j]);
//		         }
//		     }
//		      free(in.pointlist);
//		      if(in.numberofholes>0)
//		      {
//		    	  free(in.holelist);
//		      }
////		      free(in.regionlist);
//		      free(in.segmentlist);
//
//		      ///获取完三角形剖分结果后，要将点转换到世界坐标系中
////		      LOGI("crossSection->m_Points.size() ============%d",crossSection->m_Points.size());
		      for(int i = 0;i < crossSection->m_Points.size();i++)
		      {
		    	  crossSection->m_Points[i] = invRotate * crossSection->m_Points[i] +center; ///转换回去
		      }
		}
	}
	LOGI("SectionLineHelper::SectionLinesTriangulation END");
}

///三角剖分
void SectionLineHelper::Triangulation(SectionPlane* pCurSectionPlane)
{

    
	SectionDataBase * sectionDataBase = &pCurSectionPlane->m_scetionLinesDataBase;

	vector<SectionLine*> & allSection = sectionDataBase->m_scetionLinesData;
	if(allSection.size() == 0)
	{
		return;
	}

	///转换到2D，同时初始化剖切线上下级关系

	if(!TransformTo2D(pCurSectionPlane))
	{
		return;
	}

	///构建剖切线树
	SortSectionLines(pCurSectionPlane->m_sectionLineRoot,&(pCurSectionPlane->m_corssSections));

	///准备三角剖分所需的数据
//	PrepareTriangulationData(pCurSectionPlane->m_corssSections);

	//LOGE("==========================pCurSectionPlane->m_corssSections == %d",pCurSectionPlane->m_corssSections.size());
	///运用Triangle库进行三角剖分
	if(!PolyToTriTriangulation(pCurSectionPlane->m_corssSections,pCurSectionPlane->m_scetionLinesDataBase))
	{
		return;
	}
}


void SectionLineHelper::DoFigureSectionLine(void * data,SceneManager * pScene)
{

	if (pScene && data)
	{
		FigureSectionLine * sceneDatas = (FigureSectionLine * )data;
		CallBackAction* computeSceneLIneAction = new CallBackAction();
		computeSceneLIneAction->SetActionData(sceneDatas);
		computeSceneLIneAction->SetActionFun(GetFigureSectionLines);
		pScene->ExecuteAction(computeSceneLIneAction);
		delete computeSceneLIneAction;
	}

}

void SectionLineHelper::GetFigureSectionLines(void* data, Model* node)
{


	//	LOGI("begin GetSectionLines");
		if (node != NULL && node->GetType() == SHAPE_MODEL)
		{
			FigureSectionLine* sectionData = (FigureSectionLine*) data;
			int i = 0;
			float * tempPointer = NULL;
			tempPointer = sectionData->sectionPlane->GetTransformPlaneParam();
			sectionData->boundingNode = node;
			BoundingBox& shapeNodeBox = node->GetWorldBoundingBox();
			shapeNodeBox.Clear();
			shapeNodeBox = node->GetWorldBoundingBox();
			vector<float> boxVertex; shapeNodeBox.GetVertexs(boxVertex);
			float* points = boxVertex.data();
			float A = tempPointer[0],B = tempPointer[1],C=tempPointer[2],D = tempPointer[3];


#if 0        //方案1
			ShapeNode *shapeNode = (ShapeNode *)node;
			Model * model = (Model *)shapeNode->GetShape();
			BoundingBox & box = model->GetBoundingBox();

			Matrix3x4 mat = ModelAssemblyHelper::GetWorldMatrix((Model*)model); //TODO
			Vector4 originFactor(tempPointer[0], tempPointer[1], tempPointer[2], tempPointer[3]);
			Plane clipPlane(originFactor);
			clipPlane.Transform(mat.Inverse());

			Vector4 transFactor = clipPlane.ToVector4();

			float* pointss = box.GetVertexs();			
			float AA = transFactor.m_x;
			float BB = transFactor.m_y;
			float CC = transFactor.m_z;
			float DD = transFactor.m_w;
//			float value0 = A * points[0]+B * points[1]+C * points[2]+D ;
			float value0 = AA * pointss[0] + BB * pointss[1] + CC * pointss[2] + DD;
			for(int j = 3;j<24;j =j+3)
			{
//				float temp = A* points[j]+B * points[j+1]+C * points[j+2]+D ;
				float temp = AA* pointss[j] + BB * pointss[j + 1] + CC * pointss[j + 2] + DD;
				if(value0*temp<0)
				{
					ShapeNode *shapeNode = (ShapeNode *)node;
					Model * model = (Model *)shapeNode->GetShape();
					GetFigureInsectModel(data,tempPointer,model);
					break;
				}
			}


#else       //方案2
			Vector3 minPoint = shapeNodeBox.m_min;
			Vector3 maxPoint = shapeNodeBox.m_max;
			float value00 = A * minPoint.m_x + B * minPoint.m_y + C * minPoint.m_z + D;
			float temp = A* maxPoint.m_x + B * maxPoint.m_y + C * maxPoint.m_z + D;
			if (value00*temp < 0)
			{
				//ShapeNode *shapeNode = (ShapeNode *)node;
				//Model * model = (Model *)shapeNode->GetShape();
				GetFigureInsectModel(data, tempPointer, node);

			}

#endif

		}
	//	LOGI("end GetSectionLines");


}

void SectionLineHelper::GetFigureInsectModel(void * data, const float * factors, Model * model)
{

	if (model)
	{
		{

			Vector4 originFactor(factors[0], factors[1], factors[2], factors[3]);
			Plane clipPlane(originFactor);
			FigureSectionLine* sectionDataBase = (FigureSectionLine*) data;
			BoundingBox & box = model->GetBoundingBox();

			Matrix3x4 mat = model->GetWorldTransform();//ModelAssemblyHelper::GetWorldMatrix((Model*)model); //TODO

			clipPlane.Transform(mat.Inverse());

			Vector4 transFactor = clipPlane.ToVector4();

			vector<float> boxVertex; box.GetVertexs(boxVertex);
			float* points = boxVertex.data();
			float A = transFactor.m_x;
			float B = transFactor.m_y;
			float C = transFactor.m_z;
			float D = transFactor.m_w;
			float value0 = A * points[0] + B * points[1] + C * points[2] + D;
			for (int j = 3; j < 24; j = j + 3)
			{
				float temp = A * points[j] + B * points[j + 1] + C * points[j + 2] + D;
				if (value0 * temp < 0)
				{
					vector<Body*>* bodies = model->GetBodys();
					int bodiesSize = bodies->size();
					vector<Vector3> lineDatas; //该model中的所有剖切线
//					lineDatas.reserve(20);//预留空间，减少拷贝次数
					//vector<Vector3> *orderLineDatas  = new vector<Vector3>;//创建顺序交点点集
					for (int i = 0; i < bodiesSize; i++)
					{
						GetInsectBody(lineDatas,transFactor.Data(), bodies->at(i));
					}
					int insectLinesSize = lineDatas.size();
					for (int i = 0; i < insectLinesSize; i++)
					{
						Vector4 temp(lineDatas.at(i), 1.0);

						lineDatas.at(i) = (mat * temp); //转换到世界坐标系
					}
					sectionDataBase->linesData.insert(sectionDataBase->linesData.end(),lineDatas.begin(),lineDatas.end());


					break;
				}
			}

		}
	}
	//	LOGI("end GetInsectModel");


}


////************************************
//// Method:    SaveCurrentSectionLine
//// FullName:  M3D::SectionLineHelper::SaveCurrentSectionLine
//// Access:    public static
//// Returns:   void
//// Qualifier:
//// Parameter: SectionPlane * pCurSectionPlane
//// 存储当前剖切面内的剖切线到sectiongroupNode中
////************************************
//void SectionLineHelper::SaveCurrentSectionLine(SceneManager* scene)
//{
//	bool ret = false;
//	if (scene)
//	{
//		///获取剖切
//		Section * pSection = scene->GetSection();
//
//		if (!pSection)
//		{
//			return;
//		}
//
//		list<SectionPlane*>* planeList = pSection->GetPlaneList();
//
//		list<SectionPlane*>::iterator iter;
//		for (iter = planeList->begin(); iter != planeList->end(); ++iter)
//		{
//			Shape * pShape = CreateSectionLineShape(*iter);
//			if (pShape)
//			{
//				///得到剖切线组节点
//				SectionGroupNode * pSectionGroupNode = scene->GetSectionGroupNode();
//
//				///创建一个节点，用来挂在shape对象
//				ShapeNode * node = new ShapeNode;
//				node->SetShape(pShape);
//				///给新创建的Node设置一个名字
//				char strID[10];
//				sprintf(strID, "%d", node->GetID());
//				node->SetName(pSectionGroupNode->GetName() + "|" + strID);
//
//
//				//修改场景结构，加锁
//				scene->Lock();
//
//				//将创建的测量对象加入到剖切组中
//				pSectionGroupNode->AddChild(node);
//
//				scene->AddShapeIDToMap(pShape);
//
//				//完成修改，解锁
//				scene->UnLock();
//
//			}
//
//		}
//
//	}
//}
//
////************************************
//// Method:    CreateSectionLineShape
//// FullName:  M3D::SectionLineHelper::CreateSectionLineShape
//// Access:    public static
//// Returns:   M3D::Shape*
//// Qualifier:
//// Parameter: SectionPlane * pCurSectionPlane
//// 创建剖切线的shape对象，用来挂载到shapenode下面
////************************************
//Shape* SectionLineHelper::CreateSectionLineShape(SectionPlane* pCurSectionPlane)
//{
//	Edge * pedge = new Edge;
//
//	XPolyLine * pXPolyLine = new XPolyLine;
//
//	///获取当前剖切线原始数据
//	vector<Vector3> & refData = pCurSectionPlane->m_figureSectonLine.linesData;
//
//	int refDataSize = static_cast<int>(refData.size());
//
//	///填充数据
//	for (int i = 0; i < refDataSize; i++)
//	{
//		pXPolyLine->AddPoints(refData[i]);
//	}
//
//	///创建RefPolyLine，填充edge
//	RefPolyLine * pRefPolyLine = new RefPolyLine(pXPolyLine);
//
//	pedge->AddData(pRefPolyLine);
//
//	return pedge;
//}

}
 /* namespace M3D */
