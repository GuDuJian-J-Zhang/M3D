#include "m3d/action/RayPickAction.h"

#include "m3d/renderer/RenderContext.h"
#include "m3d/SceneManager.h"
#include "m3d/base/Quaternion.h"
#include "m3d/base/Matrix4.h"
#include "m3d/model/Model.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/Face.h"
#include "m3d/model/Edge.h"
#include "m3d/model/Curve.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/scene/SectionNode.h"
#include "m3d/model/Model.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/base/Vector3.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/graphics/Section.h"
#include <float.h>
#include "m3d/model/Curve.h"
#include "sview/views/Parameters.h"
#include "../model/ModelShape.h"

using SVIEW::Parameters;

namespace M3D
{

float RayPickAction::MINVALUE = FLT_MIN / 3;

bool RayPickAction::PickFeaturePnt(Vector2& screenPnt,SceneManager* scene,Vector3& featureCoordinate)
{
	float pickRadius = 5.0f;
	float featureRadius = 10.0f;
	bool picked = false;

	RayPickAction* rayPickAction = new RayPickAction(scene);

	rayPickAction->SetRadius(pickRadius);
	rayPickAction->SetPickShapeType(SHAPETYPE::SHAPE_EDGE);
	rayPickAction->SetPickGeoType(GeoAttrTypeEnum::M3D_GEOATTR_TYPE_UNKNOWN);

	rayPickAction->SetRay(screenPnt);

	SceneNode* group = scene->GetSceneRoot();
	group->Traverse((Action*) rayPickAction);
	IShape* pickedShape = rayPickAction->GetNearPickShape();
	//如果和模型有交点
	if (pickedShape)
	{
		if (pickedShape->GetType() == SHAPETYPE::SHAPE_EDGE)
		{
			picked = rayPickAction->GetNearPickPoint(featureCoordinate);
			Edge* edge = (Edge*) pickedShape;

			RefPolyLine* polyLine = edge->GetLineData();

			Matrix3x4 worldMatrix = ShapeHelper::GetShapeWorldMatrix(edge);

			if(polyLine->GetDataLength()>2)
			{
				Vector3 startPnt = worldMatrix*polyLine->GetStartPnt();
				Vector3 endPnt = worldMatrix*polyLine->GetEndPnt();

				float stratPntDis = RayPickAction::GetScreenDis(featureCoordinate,startPnt,scene->GetCamera());
				float endPntDis = RayPickAction::GetScreenDis(featureCoordinate,endPnt, scene->GetCamera()) ;

				if(stratPntDis>0 && stratPntDis < featureRadius)
				{
					featureCoordinate = startPnt;
				}

				if(endPntDis>0 && endPntDis < featureRadius && stratPntDis > endPntDis)
				{
					featureCoordinate = endPnt;
				}
			}
		}
	}

	delete rayPickAction;

	return picked;
}

const Ray&  RayPickActionData::GetCameraRay()const
{
	return this->m_cameraRay;
}

const Ray&  RayPickActionData::GetOrigCameraRay()const
{
	return this->m_origCameraRay;
}

const Frustum&  RayPickActionData::GetFramePickFrustum()const
{
	return this->m_framePickCameraFrustum;
}

const Ray&  RayPickActionData::GetModelRay()const
{
	return this->m_modelRay;
}

void RayPickActionData::SetModelMatrix(const Matrix3x4& modelMatrix)
{
	this->m_modelMatrix = modelMatrix;
}

PickTypeFilter::PickTypeFilter()
{
	this->currentShapeType = -1;
	this->currentGeoType = -1;

	memset(pickShapeTyeArray, false, PICKTYPENUM * sizeof(bool));
	memset(pickGeoTyeArray, false, PICKTYPENUM * sizeof(bool));
}

PickTypeFilter::~PickTypeFilter()
{

}

void PickTypeFilter::SetPickShapeType(int shapeType)
{
	if (shapeType == this->currentShapeType)
	{
		return;
	}

	memset(pickShapeTyeArray, false, PICKTYPENUM * sizeof(bool));
	memset(pickGeoTyeArray, true, PICKTYPENUM * sizeof(bool)); //默认对Geo不进行过滤

	if (shapeType == SHAPE_MODEL)
	{
		pickShapeTyeArray[SHAPE_MODEL] = true;
		pickShapeTyeArray[SHAPE_BODY] = true;
		pickShapeTyeArray[SHAPE_FACE] = true;
		pickShapeTyeArray[SHAPE_TRIMESH] = true;
		pickShapeTyeArray[SHAPE_EDGE] = true;
		pickShapeTyeArray[SHAPE_POINT] = true;
		pickShapeTyeArray[SHAPE_IMAGE_MODEL] = true;
//		pickShapeTyeArray[SHAPE_POINT] = true;
	}
	else if (shapeType == SHAPE_NOTCONTAIN_IMAGEMODEL)
	{
		pickShapeTyeArray[SHAPE_MODEL] = true;
		pickShapeTyeArray[SHAPE_BODY] = true;
		pickShapeTyeArray[SHAPE_FACE] = true;
		pickShapeTyeArray[SHAPE_TRIMESH] = true;
		pickShapeTyeArray[SHAPE_EDGE] = true;
		pickShapeTyeArray[SHAPE_POINT] = true;
	}
	else if (shapeType == SHAPE_BODY)
	{
		pickShapeTyeArray[SHAPE_MODEL] = true;
		pickShapeTyeArray[SHAPE_BODY] = true;
		pickShapeTyeArray[SHAPE_FACE] = true;
		pickShapeTyeArray[SHAPE_TRIMESH] = true;
	}
	else if (shapeType == SHAPE_FACE)
	{
		pickShapeTyeArray[SHAPE_MODEL] = true;
		pickShapeTyeArray[SHAPE_BODY] = true;
		pickShapeTyeArray[SHAPE_FACE] = true;
		pickShapeTyeArray[SHAPE_TRIMESH] = true;
	}
	else if (shapeType == SHAPE_EDGE)
	{
		pickShapeTyeArray[SHAPE_MODEL] = true;
		pickShapeTyeArray[SHAPE_BODY] = true;
		pickShapeTyeArray[SHAPE_FACE] = true;
		pickShapeTyeArray[SHAPE_EDGE] = true;
		pickShapeTyeArray[SHAPE_TRIMESH] = true;
	}
	else if (shapeType == SHAPE_POINT) //拾取点操作，首先要允许其上级对象进行拾取操作
	{
		pickShapeTyeArray[SHAPE_MODEL] = true;
		pickShapeTyeArray[SHAPE_BODY] = true;
		pickShapeTyeArray[SHAPE_FACE] = true;
		pickShapeTyeArray[SHAPE_EDGE] = true;
		pickShapeTyeArray[SHAPE_POINT] = true;
		pickShapeTyeArray[SHAPE_TRIMESH] = true;
	}
	else if(shapeType == SHAPE_HANDLE) 	//允许拾取Handler时，轴 旋转 和缩放 handler都允许拾取
	{
		pickShapeTyeArray[SHAPE_AXIS_HANDLE] = true;
		pickShapeTyeArray[SHAPE_ROTATE_HANDLE] = true;
		pickShapeTyeArray[SHAPE_SCALE_HANDLE] = true;
	}
	else if(shapeType == SHAPE_MEASURE_BASE)//拾取测量对象，设置距离，角度和属性测量对象为可拾取对象
	{
		pickShapeTyeArray[SHAPE_MEASURE_BASE] = true;
		pickShapeTyeArray[SHAPE_MEASURE_DISTANCE] = true;
		pickShapeTyeArray[SHAPE_MEASURE_ANGLE] = true;
		pickShapeTyeArray[SHAPE_MEASURE_PROPERTY] = true;

		pickShapeTyeArray[SHAPE_TEXT_NOTE] = true;
		pickShapeTyeArray[SHAPE_VOICE_NOTE] = true;
		pickShapeTyeArray[SHAPE_SEQUENCE_NUMBER_NOTE] = true;
		pickShapeTyeArray[SHAPE_THREED_GESTURE_NOTE] = true;
	}
	else
	{
		pickShapeTyeArray[shapeType] = true;
	}

	this->currentShapeType = shapeType;
}
void PickTypeFilter::SetPickGeoType(int geoType)
{
	if (geoType == this->currentGeoType)
	{
		return;
	}
	this->currentGeoType = geoType;
	memset(pickGeoTyeArray, false, PICKTYPENUM * sizeof(bool));
	if (geoType == M3D_GEOATTR_TYPE_LINE)
	{
		pickShapeTyeArray[SHAPE_MODEL] = true;
		pickShapeTyeArray[SHAPE_BODY] = true;
		pickShapeTyeArray[SHAPE_FACE] = true;
		pickShapeTyeArray[SHAPE_EDGE] = true;

		pickGeoTyeArray[M3D_GEOATTR_TYPE_LINE] = true;
	}
	else if (geoType == M3D_GEOATTR_TYPE_ELLIPSE)
	{
		pickShapeTyeArray[SHAPE_MODEL] = true;
		pickShapeTyeArray[SHAPE_BODY] = true;
		pickShapeTyeArray[SHAPE_FACE] = true;
		pickShapeTyeArray[SHAPE_EDGE] = true;

		pickGeoTyeArray[M3D_GEOATTR_TYPE_ELLIPSE] = true;
	}
	else if (geoType == M3D_GEOATTR_TYPE_PLANEFACE)
	{
		pickShapeTyeArray[SHAPE_FACE] = true;
		pickGeoTyeArray[M3D_GEOATTR_TYPE_PLANEFACE] = true;
	}
	else if (geoType == M3D_GEOATTR_TYPE_REVOLUTIONFACE_CYLINDERFACE_CONICALFACE)
	{
		pickShapeTyeArray[SHAPE_FACE] = true;

		pickGeoTyeArray[M3D_GEOATTR_TYPE_REVOLUTIONFACE] = true;
		pickGeoTyeArray[M3D_GEOATTR_TYPE_CYLINDERFACE] = true;
		pickGeoTyeArray[M3D_GEOATTR_TYPE_CONICALFACE] = true;
	}
	else
	{
		memset(pickGeoTyeArray, true, PICKTYPENUM * sizeof(bool));
	}
}

bool PickTypeFilter::CanPickShape(int shapeType)
{
	return this->pickShapeTyeArray[shapeType];
}
bool PickTypeFilter::CanPickGeo(int geoType)
{
	return this->pickGeoTyeArray[geoType];
}
bool PickTypeFilter::CanPickColor(Color* color)
{
	return true;
}
int PickTypeFilter::GetPickShapeType()
{
	return this->currentShapeType;
}
int PickTypeFilter::GetPickGeoType()
{
	return currentGeoType;
}

RayPickAction::RayPickAction() :
		Action()
{
	this->Init();
}

RayPickAction::RayPickAction(SceneManager* scene):
				Action()
{
	this->SetScene(scene);
    this->Init();
}

RayPickAction::~RayPickAction()
{
	if (state != NULL)
	{
		delete this->state;
		this->state = NULL;
	}
}

void RayPickAction::SetPickRadius(float radius)
{
	this->m_fRadius = radius*radius;
}

float RayPickAction::GetScreenDis(const Vector3& pnt1,const Vector3& pnt2)
{
	const Viewport& viewport = this->GetCamera()->GetViewPort();
	//获取点在屏幕上的投影
	IntVector2 projectPnt1 = viewport.WorldToScreenPoint(pnt1);
	IntVector2 closestPnt2 = viewport.WorldToScreenPoint(pnt2);

	return (projectPnt1 - closestPnt2).Length();
}

void RayPickAction::BeginPickAsGroup(IShape* shape)
{
	this->state->m_Intersect.m_IntersectPnts.clear();
	this->state->m_Intersect.m_IntersectTriglePnts.clear();
	m_pickAsGroupIntersectPnts.clear();
	this->m_pickAsGrroup = true;
}

void RayPickAction::EndPickAsGroup(IShape* shape, int zIndex)
{
	this->m_pickAsGrroup = false;
	if (this->m_pickAsGroupIntersectPnts.size())
	{
		RayPickIntersect tempIntesect;
		tempIntesect.m_IntersectPnts = m_pickAsGroupIntersectPnts;
		tempIntesect.m_zindex = zIndex;
		m_PickShapesMap.insert(
			map<IShape*, RayPickIntersect >::value_type(shape,
				tempIntesect));
	}
}

void RayPickAction::OnExecute(SceneNode* node)
{
	node->RayPick(this);
}

bool RayPickAction::GetNearPickPoint(Vector3& vec)
{
	map<IShape*, RayPickIntersect >::iterator it, pos;
	it = m_PickShapesMap.begin();
	pos = m_PickShapesMap.end();
	//Vector3 nearPoint;
	float z = MAXVALUE;
	float tempZ = 0;
	int pointPos = 0;

	for (; it != m_PickShapesMap.end(); it++)
	{
		RayPickIntersect& intersects = it->second;
		vector<Vector3>& vecArray = intersects.m_IntersectPnts;
		int intersectPntSize = vecArray.size();
		for (int i = 0; i < intersectPntSize; i++)
		{
			if(PointVisiable(vecArray[i]))
			{
				tempZ = (state->m_cameraRay.m_origin - vecArray[i]).Length();
				if (tempZ < z)
				{
					z = tempZ;
					pos = it;
					pointPos =  i;
				}
			}
		}
	}
	Vector3 t;
	Vector3 vect;
	if (pos != m_PickShapesMap.end())
	{
		t = pos->second.m_IntersectPnts[pointPos];
		vec = t;
		Vector3 tempVector;
		if ((pointPos * 3 + 1)< pos->second.m_IntersectTriglePnts.size())
		{
			Vector3 v0 = pos->second.m_IntersectTriglePnts[pointPos * 3];
			Vector3 v1 = pos->second.m_IntersectTriglePnts[pointPos * 3+1];
			Vector3 v2 = pos->second.m_IntersectTriglePnts[pointPos * 3+2];
			//tvect = Vector3(pos->second.m_IntersectTriglePnts[pointPos * 3 + 1] - pos->second.m_IntersectTriglePnts[pointPos * 3]).Multiply(pos->second.m_IntersectTriglePnts[pointPos * 3 + 2] - pos->second.m_IntersectTriglePnts[pointPos * 3 + 1]);
			tempVector = (v1 -v0).CrossProduct(v2 - v1);
			this->normal = tempVector / tempVector.Length();
		}
		return true;
	}
	return false;
}

vector<IShape*>& RayPickAction::GetFramePickShapes()
{
	return this->m_framePickShapesArrary;
}

IShape* RayPickAction::GetFarPickShape()
{
	float length = 0.0f;
	IShape* retShape = NULL;
	map<IShape*, RayPickIntersect >::iterator it;
	for (it = m_PickShapesMap.begin(); it != m_PickShapesMap.end(); it++)
	{
		IShape* shape = it->first;
		RayPickIntersect& intersects = it->second;
		vector<Vector3>& vecArray = intersects.m_IntersectPnts;
		if (!shape)
		{
			continue;
		}

		int pickShapeType = pickTypeFilter.GetPickShapeType();
		if (pickShapeType == SHAPE_MODEL)
		{
			if (shape->GetType() == SHAPE_FACE)
			{
				for (size_t i = 0; i < vecArray.size(); i++)
				{
					float tmp = (state->m_cameraRay.m_origin - vecArray[i]).Length();
					if (tmp > length)
					{
						length = tmp;
						retShape = shape;
					}
				}
			}
		}
	}

	if (retShape)
	{
		Face* face = dynamic_cast<Face*>(retShape);
		retShape = face->GetBody()->GetModel();
	}
	return retShape;
}

IShape* RayPickAction::GetNearPickShape()
{
	map<IShape*, RayPickIntersect >::iterator it, pos;
	it = m_PickShapesMap.begin();
	pos = m_PickShapesMap.end();

	float z = MAXVALUE;
	float tempZ = 0;
	for (; it != m_PickShapesMap.end(); it++)
	{
//		IShape* shape = this->sceneManager->GetShape(
//				pos->first);
		IShape* shape = it->first;

		if(shape)
		{
			//LOGI("shape Type %d",shape->GetType());
		}
		RayPickIntersect& intersects = it->second;
		vector<Vector3>& vecArray = intersects.m_IntersectPnts;
		int pntsize = vecArray.size();
		int zindex = intersects.m_zindex;
		for (int i = 0; i < pntsize; i++)
		{
			if(PointVisiable(vecArray[i]))
			{
				//根据选择模型的zindex，进行一个偏移分段
				tempZ = (state->m_cameraRay.m_origin - vecArray[i]).Length() - zindex*1000000;
				if (tempZ < z)
				{
					z = tempZ;
					pos = it;
					this->SetNearestPickPoint(vecArray[i]);
				}
			}
		}
	}
	if (pos != m_PickShapesMap.end())
	{
//		IShape* shape = this->sceneManager->GetShape(
//				pos->first);
		IShape* shape = pos->first;

		IShape* retShape = NULL;

		if (shape == NULL)
		{
			//LOGI("pick shape is NULL");
			return retShape;
		}

		int pickShapeType = pickTypeFilter.GetPickShapeType();
		if (shape->GetType() == SHAPE_FACE)
		{
//			LOGI("PICK SHAPE_FACE");
			if (pickShapeType == SHAPE_FACE)
			{
				retShape = shape;
			}
			else if (pickShapeType == SHAPE_BODY)
			{
				Face* face = dynamic_cast<Face*>(shape);
				retShape = face->GetBody();
			}
			else if (pickShapeType == SHAPE_MODEL || pickShapeType == SHAPE_NOTCONTAIN_IMAGEMODEL)
			{
				Face* face = dynamic_cast<Face*>(shape);

				retShape = face->GetBody()->GetModel();

				//LOGI("picked node path %s",face->GetBody()->GetModel()->GetName().c_str());
			}
			else
			{
				retShape = NULL;
			}
		}
		else if (shape->GetType() == SHAPE_EDGE)
		{
//			LOGI("PICK SHAPE_EDGE");
			if (pickShapeType == SHAPE_EDGE)
			{
				retShape = shape;
			}
			/*else if (pickShapeType == SHAPE_FACE)
			{
				Edge* edge = (Edge*) shape;
				retShape = edge->GetFace()->GetBody();
			}*/
			/*else if (pickShapeType == SHAPE_BODY)
			{
				Edge* edge = (Edge*) shape;
				retShape = edge->GetBody();
			}*/
			else if (pickShapeType == SHAPE_MODEL || pickShapeType == SHAPE_NOTCONTAIN_IMAGEMODEL)
			{
				Edge* edge = (Edge*) shape;
				retShape = edge->GetBody()->GetModel();
			}
			else
			{
				retShape = NULL;
			}
		}else
		{
			retShape = shape;
		}

		if (retShape)
		{
			//如果需要判断几何类型，进行几何类型属性的过滤
			if (this->GetPickGeoType()>0)
			{
				IShape* geoShape = retShape;
				retShape = NULL;
				//int geoType = this->GetPickGeoType();

				if (geoShape->GetType() == SHAPETYPE::SHAPE_FACE)
				{
					Face* face = static_cast<Face*>(geoShape);
					if (face->GetGeoAttribute() && this->pickTypeFilter.CanPickGeo(face->GetGeoAttribute()->GetGeoAttrType()))
					{
						retShape = geoShape;
					}
				}
				else if (geoShape->GetType() == SHAPETYPE::SHAPE_EDGE)
				{
					Edge* edge = static_cast<Edge*>(geoShape);
					int geoType = this->GetPickGeoType();
					if (edge->GetGeoAttribute() && edge->GetGeoAttribute()->GetGeoAttrType() == geoType)
					{
						retShape = geoShape;
					}
				}
			}
		}

		return retShape;
	}
	return NULL;
}

bool RayPickAction::PointVisiable(const Vector3& point)
{
	bool ret = true;
	if (this->GetUseclipPlane())
	{
		Section * tempSection = GetScene()->GetSectionNode()->GetSection();
		list<SectionPlane*>* planeList = tempSection->GetPlaneList();
		for (list<SectionPlane*>::iterator it = planeList->begin(); it != planeList->end(); it++)
		{
			float * equation = (*it)->GetTransformPlaneParam();
			if ((equation[0] * point.m_x + equation[1] * point.m_y + equation[2] * point.m_z + equation[3]) < 0)
			{
				ret = true;
				return ret;
			}
		}
	}
	return ret;
}

int RayPickAction::GetFarPickNode()
{
	/*map<int, vector<Vector3> >::iterator it, pos;
	 it = pickShapesMap.begin();
	 pos = it;

	 vector<Vector3> vecArray;
	 float z = -99999999;

	 for (; it != pickShapesMap.end(); it++) {
	 vecArray = it->second;
	 int offSet = vecArray.size() / 2;

	 for (int i = 0; i < offSet; i++) {
	 if (vecArray[offSet + i].z > z) {
	 z = vecArray[offSet + i].z;
	 pos = it;
	 }
	 }
	 }
	 if (pos != pickShapesMap.end()) {
	 return pos->first;
	 }*/
	return -1;
}

bool RayPickAction::IsintersectRayAndTriangle(const Vector3& v0,const Vector3& v1,
		const Vector3& v2, Vector3& I)
{
	if (this->GetInterctType() == 0)
	{
		return ISintersectRayAndTriangle(v0, v1, v2, state->m_modelRay, I);
	}else if (this->GetInterctType() == 1)
	{
		if (Intersect(v0,v1,I)|| Intersect(v1, v2, I)|| Intersect(v2, v1, I))
		{
			return true;
		}
	}
	return false;
}

bool RayPickAction::IsintersecFrustumTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
	Vector3 I;
	//判断Frustum的四条射线是否和三角形相交，首先判断右下角的那条，提高匹配几率
	if (ISintersectRayAndTriangle(v0, v1, v2, state->m_frustumRightBottomModelRay, I))
	{
		return true;
	}

	if (ISintersectRayAndTriangle(v0, v1, v2, state->m_frustumRightTopModelRay, I))
	{
		return true;
	}

	if (ISintersectRayAndTriangle(v0, v1, v2, state->m_frustumLeftBottomModelRay, I))
	{
		return true;
	}

	if (ISintersectRayAndTriangle(v0, v1, v2, state->m_frustumLeftTopModelRay, I))
	{
		return true;
	}

	return false;
}

bool RayPickAction::IsPointInFrustum(const Vector3& v0)
{
	return (state->m_modelframePickFrustum.IsInside(v0) == INSIDE);
}

bool RayPickAction::ISintersectRayAndTriangle(float* tri, const Ray&ray, Vector3& I)
{
	float u, v, t;
	Vector3 v0(tri[0], tri[1], tri[2]);
	Vector3 v1(tri[3], tri[4], tri[5]);
	Vector3 v2(tri[6], tri[7], tri[8]);

	Vector3 dir = ray.m_direction;
	// E1
	Vector3 E1 = v1 - v0;

	// E2
	Vector3 E2 = v2 - v0;

	// P
	Vector3 P = dir.CrossProduct(E2);

	// determinant
	float det = E1.DotProduct(P);

	// keep det > 0, modify T accordingly
	Vector3 T;
	if (det > 0)
	{
		T = ray.m_origin - v0;
	}
	else
	{
		T = v0 - ray.m_origin;
		det = -det;
	}

	// If determinant is near zero, ray lies in plane of triangle
	if (det < MINVALUE)
	{
		I = (v0 + v1 + v2) / 3;
		return false;
	}
	// Calculate u and make sure u <= 1

	u = T.DotProduct(P);
	if (u < 0.0f || u > det)
		return false;

	// Q
	Vector3 Q = T.CrossProduct(E1);

	// Calculate v and make sure u + v <= 1
	v = dir.DotProduct(Q);
	if (v < 0.0f || u + v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	t = E2.DotProduct(Q);

	float fInvDet = 1.0f / det;

	t *= fInvDet;
	u *= fInvDet;
	v *= fInvDet;

	I = v0 * (1 - u - v) + v1 * u + v2 * v;

	Vector3 newDir = I - ray.m_origin;

	if (t < 0)
	{
		return false;
	}

	return true;
}

bool RayPickAction::ISintersectRayAndTriangle(const Vector3& v0,const Vector3& v1,
		const Vector3& v2, const Ray&ray,Vector3& I)
{
	float u, v, t;

	Vector3 dir = ray.m_direction;
	// E1
	Vector3 E1 = v1 - v0;

	// E2
	Vector3 E2 = v2 - v0;

	// P
	Vector3 P = dir.CrossProduct(E2);

	// determinant
	float det = E1.DotProduct(P);

	// keep det > 0, modify T accordingly
	Vector3 T;
	if (det > 0)
	{
		T = ray.m_origin - v0;
	}
	else
	{
		T = v0 - ray.m_origin;
		det = -det;
	}

	// If determinant is near zero, ray lies in plane of triangle
	if (det < MINVALUE)
	{
		I = (v0 + v1 + v2) / 3;
		return false;
	}
	// Calculate u and make sure u <= 1

	u = T.DotProduct(P);
	if (u < 0.0f || u > det)
		return false;

	// Q
	Vector3 Q = T.CrossProduct(E1);

	// Calculate v and make sure u + v <= 1
	v = dir.DotProduct(Q);
	if (v < 0.0f || u + v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	t = E2.DotProduct(Q);

	float fInvDet = 1.0f / det;

	t *= fInvDet;
	u *= fInvDet;
	v *= fInvDet;

	I = v0 * (1 - u - v) + v1 * u + v2 * v;

	Vector3 newDir = I - ray.m_origin;

	if (t < 0)
	{
		return false;
	}
	return true;
}


bool RayPickAction::Intersect(const Vector3 & v0_in, const Vector3 & v1_in,
		Vector3 & closestPnt_out)
{
	///如果v0_in 和 v1_in 为同一个点 ，转入射线与点的求交
	if (v0_in == v1_in)
	{
		closestPnt_out = v0_in;
		bool intersect = Intersect(closestPnt_out);
		closestPnt_out = closestPnt_out + this->state->m_modelRay.m_direction.Multiply(-0.01);
		return intersect;
	}

	//构造一条射线
	Ray inRay (v0_in,v1_in - v0_in);

	//求射线m_modelRay 与 inRay的最近点，最近点在inRay射线上取
	Vector3 closestPnt =inRay.ClosestPoint(this->state->m_modelRay);
//	//判断最近点是否有效，如果无效直接返回
//	if (closestPnt == this->state->m_modelRay.origin_)
//		return false;

	//最近点可能不在 v0_in 和 v1_in 之间需要进行截断
	if ((closestPnt - v0_in).DotProduct(inRay.m_direction) < 0.0)
		closestPnt = v0_in;
	else if ((v1_in - closestPnt).DotProduct(inRay.m_direction) < 0.0)
		closestPnt = v1_in;

	///获取m_modelRay上的投影点
	Vector3 projectPnt =  this->state->m_modelRay.Project(closestPnt);
	const Viewport& viewport = this->GetCamera()->GetViewPort();

	Vector3 tempClosetPnt = closestPnt;

	//将点变换到实际位置
	projectPnt = this->state->m_modelMatrix*projectPnt;
	tempClosetPnt = this->state->m_modelMatrix*closestPnt;


	//获取点在屏幕上的投影
	IntVector2 projectPntSrc = viewport.WorldToScreenPoint(projectPnt);
	IntVector2 closestPntSrc = viewport.WorldToScreenPoint(tempClosetPnt);

	//最近点在屏幕外面，原则上不可见
	if(!viewport.GetRect().IsInside(closestPntSrc))
	{
		return false;
	}

	//判断投影到屏幕后的两个点之间的距离
	float distance  = (projectPntSrc - closestPntSrc).LengthSquared();

//	LOGI("projectPntSrc %s- closestPntSrc %s" , projectPntSrc.Tostring().c_str(), closestPntSrc.Tostring().c_str());

	//如果距离小于给定的阈值，则认为相交
	if (m_fRadius >= distance)
	{
//		LOGI("RayPickAction::Intersect %f",distance);
		closestPnt_out = closestPnt+ this->state->m_modelRay.m_direction.Multiply(-0.01);
		return true;
	}
	return false;
}

bool RayPickAction::Intersect(const Vector3& point_in)
{
	///获取m_modelRay上的投影点
	Vector3 projectPnt =  this->state->m_modelRay.Project(point_in);
	const Viewport& viewport = this->GetCamera()->GetViewPort();

	Vector3 tempClosetPnt = point_in;

	//将点变换到实际位置
	projectPnt = this->state->m_modelMatrix*projectPnt;
	tempClosetPnt = this->state->m_modelMatrix*tempClosetPnt;

	IntVector2 projectPntSrc = viewport.WorldToScreenPoint(projectPnt);
	IntVector2 closestPntSrc = viewport.WorldToScreenPoint(tempClosetPnt);

	//判断投影到屏幕后的两个点之间的距离
	float distance  = (projectPntSrc - closestPntSrc).LengthSquared();
	//LOGI("pnt to pnt dis %f",distance);
	//如果距离小于给定的阈值，则认为相交
	return (m_fRadius >= distance);
}

void RayPickAction::RayPick(IShape* shape)
{
	this->state->m_Intersect.m_IntersectPnts.resize(0);
	shape->RayPick(this);
}

void RayPickAction::FramePick(IShape* shape)
{
	shape->FramePick(this);
}


void RayPickAction::AddShape(IShape* shape, int zIndex)
{
	if (this->state->m_Intersect.m_IntersectPnts.size() > 0) //is picked
	{
		this->state->m_Intersect.m_zindex = zIndex;
		int k = this->state->m_Intersect.m_IntersectPnts.size();
		int j = this->state->m_Intersect.m_IntersectTriglePnts.size();
		//将所有的点，通过modelMatrix变换到显示时对应的位置
		if (!this->m_pickAsGrroup)
		{
			for (int i = 0; i < k; i++)
			{
				Vector3 tempVec = this->state->m_modelMatrix
					* this->state->m_Intersect.m_IntersectPnts[i];
				this->state->m_Intersect.m_IntersectPnts[i] = tempVec;
			}
			for (int i = 0; i < j; i++)
			{
				Vector3 tempVec = this->state->m_modelMatrix
					* this->state->m_Intersect.m_IntersectTriglePnts[i];
				this->state->m_Intersect.m_IntersectTriglePnts[i] = tempVec;
			}
			//将shape所有的交点，和shape对象的id放入表中，供最后查找离摄像机最近的shape
			m_PickShapesMap.insert(
				map<IShape*, RayPickIntersect >::value_type(shape,
					this->state->m_Intersect));
		}
		else
		{
			for (int i = 0; i < k; i++)
			{
				Vector3 tempVec = this->state->m_modelMatrix
					* this->state->m_Intersect.m_IntersectPnts[i];
				this->m_pickAsGroupIntersectPnts.push_back(tempVec);
			}
		}
		this->state->m_Intersect.m_IntersectPnts.resize(0);
		this->state->m_Intersect.m_IntersectTriglePnts.resize(0);
	}
}

void RayPickAction::SetPickAll(const bool flag)
{
	this->m_isPickAll = flag;
	if (this->m_isPickAll)
	{
		this->SetFramePickType(1);
	}
}

bool RayPickAction::IsPickAll(void) const
{
	return this->m_isPickAll;
}

void RayPickAction::SetRay(Ray & ray)
{
	this->SetPickAll(false);
	this->state->m_cameraRay = ray;
	this->state->m_origCameraRay = ray;
	if (this->GetCamera())
	{
		Matrix3x4 viewMatrix = this->GetCamera()->GetView();
		this->state->m_origCameraRay = ray.Transformed(viewMatrix);
	}
}

void RayPickAction::SetRay(int winX, int winY)
{
	SetPoint(Vector2(winX, winY));
}

void RayPickAction::SetRay(const Vector2& screenPnt)
{
	this->SetPoint(screenPnt);
}

void RayPickAction::SetFramePickSection(const Vector2& leftTop, const Vector2& rightBottom)
{
	this->SetPickAll(true);

	Vector2 leftBottom(leftTop.m_x,rightBottom.m_y);
	Vector2 rightTop(rightBottom.m_x,leftTop.m_y);

	CameraNode* camera = GetCamera();
	const Viewport& viewport= camera->GetViewPort();

	float  near = camera->GetNearClip();
	float length = camera->GetFarClip();
	
	//得到椎体八个点
	Vector3 nearRightTop = viewport.ScreenToWorldPoint(rightTop, near);
	Vector3 nearRightBottom = viewport.ScreenToWorldPoint(rightBottom, near);
	Vector3 nearLeftBottom = viewport.ScreenToWorldPoint(leftBottom, near);
	Vector3 nearLeftTop = viewport.ScreenToWorldPoint(leftTop, near);

	Vector3 farRightTop= viewport.ScreenToWorldPoint(rightTop, length);
	Vector3 farRightBottom = viewport.ScreenToWorldPoint(rightBottom, length);
	Vector3 farLeftBottom = viewport.ScreenToWorldPoint(leftBottom, length);
	Vector3 farLeftTop = viewport.ScreenToWorldPoint(leftTop, length);

	this->GetData()->m_framePickCameraFrustum.Define(nearRightTop, nearRightBottom, nearLeftBottom,
		nearLeftTop, farRightTop, farRightBottom, farLeftBottom, farLeftTop);


	//定义视锥的四条射线
	this->GetData()->m_frustumRightTopCameraRay = Ray(nearRightTop,farRightTop - nearRightTop) ;
	this->GetData()->m_frustumRightBottomCameraRay = Ray(nearRightBottom, farRightBottom - nearRightBottom);
	this->GetData()->m_frustumLeftBottomCameraRay = Ray(nearLeftBottom, farLeftBottom - nearLeftBottom);
	this->GetData()->m_frustumLeftTopcameraRay = Ray(nearLeftTop, farLeftTop - nearLeftTop);
}

void RayPickAction::SetFramePickType(int framePickType)
{
	this->m_framePickType = framePickType;
}

int RayPickAction::GetFramePickType()
{
	return this->m_framePickType;
}

void RayPickAction::SetPoint(const Vector2 & screentPoint)
{
	this->SetPickAll(false);
	this->SetScreentPoint(screentPoint);
	CameraNode* camera = GetCamera();
     Ray  temp =camera->GetViewPort().GetScreenRay(screentPoint.m_x,screentPoint.m_y);
	this->SetRay(temp);
}

bool  RayPickAction::Intersect(const BoundingBox& box)
{
	return (this->state->m_modelRay.HitDistance(box) != M_INFINITY);
}

bool RayPickAction::FrustumIntersetWithWorldBox(const BoundingBox& box)
{
	bool ret = false;
	if (box.Defined())
	{
		if (this->GetData()->m_framePickCameraFrustum.IsInside(box) != OUTSIDE)
		{
			ret = true;
		}
	}
	return ret;
}

bool RayPickAction::FrustumIntersetWithModel(ModelShape* modelShape)
{
	bool ret = false;
	const BoundingBox& box = modelShape->GetWorldBoundingBox();
	if (this->FrustumIntersetWithWorldBox(box))
	{
		if (this->m_framePickType == 1)
		{
			if (this->GetData()->m_framePickCameraFrustum.IsInside(box) != OUTSIDE)
			{
				ret = true;
			}else if (this->m_framePickType == 2)
			if (this->GetData()->m_framePickCameraFrustum.IsInside(box) == INSIDE)
			{
				ret = true;
			}
		}
	}

	return ret;
}

void RayPickAction::AddToFramePickCollection(IShape* shape)
{
	if (shape)
	{
		m_framePickShapesArrary.push_back(shape);
	}
}

void RayPickAction::Init()
{
	this->state = new RayPickActionData;
	this->m_fRadius = 900;

	if(this->GetScene())
	{
		int width = this->GetCamera()->GetViewPort().GetRect().Width();

		int heigh = this->GetCamera()->GetViewPort().GetRect().Height();

		int screenPPI = Parameters::Instance()->m_screenPPI;

//		int r = width>heigh?heigh:width;

//		LOGI("width %d heigh %d",width,heigh);

//		LOGI("PPI %d",screenPPI);

		//0.2英寸，一英寸等于2.5厘米
		this->m_fRadius = screenPPI * 0.1*screenPPI* 0.1;

		if(this->m_fRadius > 600)
		{
			this->m_fRadius = 600;
		}
	}
	this->m_onceFramePickMatchCount = 0;

	m_pickAsGrroup = false;
	SetUseclipPlane(true);
	SetInterctType(0);
	this->SetPickAll(false);
}

void RayPickAction::SetScene(SceneManager* pSceneManager)
{
	if(pSceneManager!=NULL)
	{
		sceneManager = pSceneManager;
		this->SetCamera(this->GetScene()->GetCamera());
	}
}

void RayPickAction::SetRadius(const float radiusinpixels)
{

}

void RayPickAction::DrawSomeInfoForTest()
{
	//	LOGI("DrawSomeInfo()");
	/*	glPushMatrix();
	 // glLoadIdentity();
	 glEnableClientState(GL_VERTEX_ARRAY);

	 glVertexPointer(3, GL_FLOAT, 0, pickLine);

	 glLineWidth(10);
	 glDrawArrays(GL_LINES, 0, 2);

	 glVertexPointer(3, GL_FLOAT, 0, DrawTri);
	 glDrawArrays(GL_TRIANGLES, 0, 3);

	 glDisableClientState(GL_VERTEX_ARRAY);

	 glPopMatrix();*/
}

bool RayPickAction::IsRayHitBox(const Ray& ray,const BoundingBox& box)
{
	return (ray.HitDistance(box) != M_INFINITY);
}

bool  RayPickAction::RayIntersectBoxPnt(const Ray& ray,const BoundingBox& box,vector<Vector3>& intersectPnts)
{
	vector<Vector3> boxTriAngles;
	boxTriAngles.reserve(12*3);
	//BoundingBox::triIndex[12][3]

	const Vector3& max_ = box.m_max;
	const Vector3& min_ = box.m_min;

	vector<float> m_points;

	m_points.resize(24);
	m_points[0] = max_.m_x;
	m_points[1] = max_.m_y;
	m_points[2] = max_.m_z;

	m_points[3] = min_.m_x;
	m_points[4] = max_.m_y;
	m_points[5] = max_.m_z;

	m_points[6] = min_.m_x;
	m_points[7] = min_.m_y;
	m_points[8] = max_.m_z;

	m_points[9] = max_.m_x;
	m_points[10] = min_.m_y;
	m_points[11] = max_.m_z;

	m_points[12] = max_.m_x;
	m_points[13] = min_.m_y;
	m_points[14] = min_.m_z;

	m_points[15] = max_.m_x;
	m_points[16] = max_.m_y;
	m_points[17] = min_.m_z;

	m_points[18] = min_.m_x;
	m_points[19] = max_.m_y;
	m_points[20] = min_.m_z;

	m_points[21] = min_.m_x;
	m_points[22] = min_.m_y;
	m_points[23] = min_.m_z;

	float tmpP[4];
	Vector3 pnt;
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int index = BoundingBox::triIndex[i][j] * 3;
			pnt.m_x = m_points[index];
			pnt.m_y = m_points[index + 1];
			pnt.m_z = m_points[index + 2];

			boxTriAngles.push_back(pnt);
		}
	}

	intersectPnts.clear();

	for (int i = 0; i < boxTriAngles.size() / 3; i++)
	{
		Vector3 vec;
		if (ISintersectRayAndTriangle(boxTriAngles[i*3],boxTriAngles[i*3+1],boxTriAngles[i*3+2],ray, vec))
		{
			intersectPnts.push_back(vec);
		}
	}
	if (intersectPnts.size() > 0)
	{
		return true;
	}

	return false;
}

float RayPickAction::GetScreenDis(const Vector3& pnt1,const Vector3& pnt2,CameraNode* camera)
{
	float dis = -1;
	if(camera)
	{
		const Viewport& viewport = camera->GetViewPort();
		//获取点在屏幕上的投影
		IntVector2 projectPnt1 = viewport.WorldToScreenPoint(pnt1);
		IntVector2 closestPnt2 = viewport.WorldToScreenPoint(pnt2);

		dis = (projectPnt1 - closestPnt2).Length();
	}

	return dis;
}

void RayPickAction::UpdataModelRay(const Matrix3x4& modelMatrix)
{
	this->state->SetModelMatrix(modelMatrix);
	//求得模型显示时对应的变换矩阵的逆矩阵，通过对射线做逆矩阵变换，能够避免对模型上所有点乘以正矩阵变换
	Matrix3x4 modelMatrixInverse = state->m_modelMatrix.Inverse();
	state->m_modelRay = state->m_cameraRay.Transformed(modelMatrixInverse);
}

void  RayPickAction::UpdataFramePickFrustum(const Matrix3x4& modelMatrix)
{
	//this->state->SetModelMatrix(modelMatrix);
	//求得模型显示时对应的变换矩阵的逆矩阵，通过对射线做逆矩阵变换，能够避免对模型上所有点乘以正矩阵变换
	this->state->SetModelMatrix(modelMatrix);
	Matrix3x4 modelMatrixInverse = state->m_modelMatrix.Inverse();
	state->m_modelframePickFrustum = state->GetFramePickFrustum();
	state->m_modelframePickFrustum = state->m_modelframePickFrustum.Transformed(modelMatrixInverse);

	//定义视锥的四条射线
	state->m_frustumRightTopModelRay = this->GetData()->m_frustumRightTopCameraRay.Transformed(modelMatrixInverse);
	state->m_frustumRightBottomModelRay = this->GetData()->m_frustumRightBottomCameraRay.Transformed(modelMatrixInverse);
	state->m_frustumLeftBottomModelRay = this->GetData()->m_frustumLeftBottomCameraRay.Transformed(modelMatrixInverse);
	state->m_frustumLeftTopModelRay = this->GetData()->m_frustumLeftTopcameraRay.Transformed(modelMatrixInverse);
}

void RayPickAction::AddIntersectPnt(const Vector3& pos)
{
	this->state->m_Intersect.m_IntersectPnts.push_back(pos);
}
void RayPickAction::AddIntersectTrianglePnts(const Vector3& normal)
{
	this->state->m_Intersect.m_IntersectTriglePnts.push_back(normal);
}

void RayPickAction::UpdataIntersecPnts(Matrix3x4& modelMatrix)
{
	for (int i =0;i<this->state->m_Intersect.m_IntersectPnts.size();i++)
	{
		Vector3 tempVec = modelMatrix
			* this->state->m_Intersect.m_IntersectPnts[i];
		this->state->m_Intersect.m_IntersectPnts[i] = tempVec;
	}
}

void RayPickAction::UpdateGroupPickPnts()
{
	m_pickAsGroupIntersectPnts = this->state->m_Intersect.m_IntersectPnts;
}

int RayPickAction::GetPickShapeType()
{
	return this->pickTypeFilter.GetPickShapeType();
}
int RayPickAction::GetPickGeoType()
{
	return this->pickTypeFilter.GetPickGeoType();
}
void RayPickAction::SetPickShapeType(int shapeType)
{
	this->pickTypeFilter.SetPickShapeType(shapeType);
}
void RayPickAction::SetPickGeoType(int geoType)
{
	this->pickTypeFilter.SetPickGeoType(geoType);
}
bool RayPickAction::CanPickShape(int shapeType)
{
	return this->pickTypeFilter.CanPickShape(shapeType);
}
bool RayPickAction::CanPickGeo(int geoType)
{
	return this->pickTypeFilter.CanPickGeo(geoType);
}
bool RayPickAction::CanPickColor(Color* color)
{
	return this->pickTypeFilter.CanPickColor(color);
}

void RayPickAction::SetCamera(CameraNode* camera)
{
	this->m_camera = camera;
}

void RayPickAction::BeginOnceFramePick()
{
	this->m_onceFramePickCount = 0;
	this->m_onceFramePickMatchCount =0;
}

void RayPickAction::IncreaseFramePickPntCount()
{
	this->m_onceFramePickCount++;
}

int RayPickAction::GetOnceFramePickCount()
{
	return this->m_onceFramePickCount;
}

void RayPickAction::IncreaseFramePickMatchPntCount()
{
	this->m_onceFramePickMatchCount++;
}

int RayPickAction::GetFramePickMatchPntCount()
{
	return this->m_onceFramePickMatchCount;
}

CameraNode* RayPickAction::GetCamera()
{
	return this->m_camera;
}

RayPickActionData* RayPickAction::GetData()
{
	return this->state;
}

}
