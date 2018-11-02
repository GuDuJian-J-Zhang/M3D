#include "m3d/utils/ShapeHelper.h"
#include "m3d/action/RayPickAction.h"


#include "m3d/model/Point.h"
#include "m3d/model/Edge.h"
#include "m3d/model/Face.h"
#include "m3d/model/Body.h"
#include "m3d/model/Model.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/Handler/HandlerPoint.h"
#include "m3d/Handler/HandlerGroup.h"
#include "m3d/graphics/CameraNode.h"
#include "sview/views/Parameters.h"
using SVIEW::Parameters;

namespace M3D
{

ShapeHelper::ShapeHelper() :Object()
{

}

ShapeHelper::~ShapeHelper()
{

}

HandlerPoint* ShapeHelper::AddPointHandler(const Vector3& coordinate,float size,SceneManager* scene)
{
	//创建一个点
	HandlerPoint* handlerPoint = NULL;
	HandlerGroup* handlergroup = scene->GetHandlerGroup();
	if(handlergroup)
	{
		BoundingBox& box = scene->GetSceneBox();

		handlerPoint = handlergroup->CreatePointHandler(coordinate,size,3,scene);
	}

	return handlerPoint;
}
IShape* ShapeHelper::SelectFeaturePnt(Vector2& screenPnt,SceneManager* scene)
{
    IShape* shape = NULL;
        
    Vector3 featureCoordinate;
    if(RayPickAction::PickFeaturePnt(screenPnt,scene,featureCoordinate) == 2)
    {
        //创建临时点对象
        shape = AddPointHandler(featureCoordinate,1.0f,scene);
        ((HandlerPoint*)shape)->GetPoint()->SetSize(2.0);
    }
    
    return shape;
}
IShape* ShapeHelper::SelectShape(Vector2& screenPnt,int shapeType,
		int geoType,SceneManager* scene)
{
	IShape* shape = NULL;

	//如果要选择点，由于不存在点对象，需要创建临时对象
	//TODO 后期如果有点对象则去掉此算法
	if(shapeType == SHAPE_COORDINATE)
	{
		Vector3 coordinate;
		bool intersect = scene->GetPickPoint(screenPnt,coordinate,true);

		//如果和模型有交点
		if(intersect)
		{
			//创建临时点对象
			shape = AddPointHandler(coordinate,1.0f,scene);
		}

	}else if(shapeType == SHAPE_FEATURE_COORDINATE){
		Vector3 featureCoordinate;
		if(RayPickAction::PickFeaturePnt(screenPnt,scene,
				 featureCoordinate) != 0)
		{
			//创建临时点对象
			shape = AddPointHandler(featureCoordinate,1.0f,scene);
		}
	}
	else
	{
		shape = scene->GetPickShape(screenPnt,shapeType,geoType);
	}

	//
	return shape;
}

Matrix3x4 ShapeHelper::GetShapeWorldMatrix(IShape* shape)
{
	Matrix3x4 worldMatrix;

	if (!shape)
	{
		return worldMatrix;
	}

	if (shape->GetType() == SHAPE_EDGE)
	{
		Edge* edge = (Edge*) shape;
		if (edge->GetBody() != NULL)
		{
			worldMatrix =
					edge->GetBody()->GetModel()->GetWorldTransform();
		}
		else if(edge->GetFace() != NULL)
		{
			worldMatrix =
					edge->GetFace()->GetBody()->GetModel()->GetWorldTransform();
		}
	}
	else if (shape->GetType() == SHAPE_FACE)
	{
		Face* face = (Face*) shape;
		worldMatrix =
				face->GetBody()->GetModel()->GetWorldTransform();

	}
	else if (shape->GetType() == SHAPE_BODY)
	{
		Body* body = (Body*) shape;
		worldMatrix = body->GetModel()->GetWorldTransform();
	}
	else if (shape->GetType() == SHAPE_MODEL)
	{
		Model* model = dynamic_cast<Model *>(shape);
		worldMatrix = model->GetWorldTransform();
	}

	return worldMatrix;
}

Vector2 ShapeHelper::GetCommonSize(SceneManager* scene, const Vector2& size)
{
	float width = 100.0f;
	float heigh = 100.0f;
    
#ifdef _WIN32
	float screenPPI = 100;
#else
	float screenPPI =  Parameters::Instance()->m_screenPPI;
#endif

    float screenWidth = Parameters::Instance()->m_screenWidth;
    float screenHeight =  Parameters::Instance()->m_screenHeight;

	if (scene)
	{
		CameraNode* camera = scene->GetCamera();
		float ratio = camera->GetAspectRatio();
		//获取当前ortho宽和高的较小值
		heigh = camera->GetOrthoSize();
 
		if(ratio > 1)
		{
			width = heigh * ratio;
            screenWidth = screenHeight;
		}else
		{
			width = heigh;
//			screenWidth = screenHeight;
		}
        
#ifdef _WIN32
		//屏幕宽度的20分之一
		width =  width /(6* (screenWidth/(screenPPI*2.5)));
#else
		//屏幕宽度的20分之一
		width = width / (20 * (screenWidth / (screenPPI*2.5)));
#endif
	}

	return Vector2(width*size.m_x, width*size.m_y);
}

}
