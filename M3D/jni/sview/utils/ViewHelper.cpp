#include "sview/utils/ViewHelper.h"

#include "m3d/model/Point.h"
#include "m3d/model/Edge.h"
#include "m3d/model/Face.h"
#include "m3d/model/Body.h"
#include "m3d/model/Model.h"

#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/base/Viewport.h"

#include "m3d/Handler/HandlerPoint.h"
#include "m3d/Handler/HandlerGroup.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/SceneManager.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/scene/ShapeNode.h"


#include "sview/views/View.h"

using namespace M3D;

namespace SVIEW
{

ViewHelper::ViewHelper()
{

}

ViewHelper::~ViewHelper()
{

}

bool ViewHelper::SetShapePosByScreenPoint(const Vector2& screenPnt,
		Shape* shape, View* view)
{
	bool ret = false;
	if (view)
	{
		M3D::Vector3 worldPnt;
		float depth = 0.5f;
		CameraNode* camera = view->GetSceneManager()->GetCamera();
		const Viewport& viewPort = camera->GetViewPort();
		worldPnt = viewPort.ScreenToWorldPoint(screenPnt.m_x, screenPnt.m_y, depth);

		if(shape)
		{
			SceneNode *node = shape->GetSceneNode();
			if (node != NULL)
			{
				//首先根据包围的中心点和摄像机的朝向，确定一张通过包围盒中心点的平面
				//让平移操作在这张平面上进行，能够实现最佳的移动效果
				BoundingBox& box = node->GetWorldBoundingBox();
				Vector3 center = box.Center();
				Plane plane(camera->GetScreenRay(screenPnt.m_x, screenPnt.m_y).GetDirection(),center);

				//将空间中的点投影到屏幕上
				worldPnt = plane.Project(worldPnt);

				Vector3 localDis; //目标点
				localDis = node->GetParent()->GetWorldTransform().Inverse()
						* worldPnt;
				Vector3 localSrc = node->GetWorldTransform().Translation();//原始点
				Vector3 mov = localDis - localSrc;
				node->Translate(mov, TS_PARENT);
				ret = true;
			}
		}
	}
	return ret;
}

Vector3 ViewHelper::ScreenToWorldPoint(const Vector2& screenPnt, float depth,
		View* view)
{
	Vector3 worldPnt;
	if (view)
	{
		CameraNode* camera = view->GetSceneManager()->GetCamera();
		const Viewport& viewPort = camera->GetViewPort();
		worldPnt = viewPort.ScreenToWorldPoint(screenPnt.m_x, screenPnt.m_y, depth*camera->GetClipDis());
	}
	return worldPnt;
}
    
Vector3 ViewHelper::ScreenToWorldPointByShape(const Vector2& screenPnt,Shape* shape,View* view)
{
	Vector3 worldPnt;
    if (view)
    {
        float depth = 0.5f;
        CameraNode* camera = view->GetSceneManager()->GetCamera();
        const Viewport& viewPort = camera->GetViewPort();
        worldPnt = viewPort.ScreenToWorldPoint(screenPnt.m_x, screenPnt.m_y, depth);
        
        if(shape)
        {
            SceneNode *node = shape->GetSceneNode();
            if (node != NULL)
            {
                //首先根据包围的中心点和摄像机的朝向，确定一张通过包围盒中心点的平面
                //让平移操作在这张平面上进行，能够实现最佳的移动效果
                BoundingBox& box = node->GetWorldBoundingBox();
                Vector3 center = box.Center();
                Plane plane(camera->GetScreenRay(screenPnt.m_x, screenPnt.m_y).GetDirection(),center);
                
                //将空间中的点投影到平面上
                worldPnt = plane.Project(worldPnt);
            }
        }
    }
    return worldPnt;
}
    
Vector3 ViewHelper::ScreenToLocalPointByShape(const Vector2& screenPnt,Shape* shape,View* view)
{
    Vector3 localPnt;
    if (view)
    {
        M3D::Vector3 worldPnt;
        float depth = 0.5f;
        CameraNode* camera = view->GetSceneManager()->GetCamera();
        const Viewport& viewPort = camera->GetViewPort();
        worldPnt = viewPort.ScreenToWorldPoint(screenPnt.m_x, screenPnt.m_y, depth);
        
        if(shape)
        {
            SceneNode *node = shape->GetSceneNode();
            if (node != NULL)
            {
                //首先根据包围的中心点和摄像机的朝向，确定一张通过包围盒中心点的平面
                //让平移操作在这张平面上进行，能够实现最佳的移动效果
                BoundingBox& box = node->GetWorldBoundingBox();
                Vector3 center = box.Center();
                Plane plane(camera->GetScreenRay(screenPnt.m_x, screenPnt.m_y).GetDirection(),center);
                
                //将空间中的点投影到屏幕上
                worldPnt = plane.Project(worldPnt);
 
                localPnt = node->GetParent()->GetWorldTransform().Inverse()
                * worldPnt;
            }
        }
    }
    return localPnt;
}
    
Vector3 ViewHelper::WorldPointToLocal(const Vector3& worldPnt,Shape* shape,View* view)
{
    Vector3 loaclPnt;
    if (view)
    {
        if(shape)
        {
            SceneNode *node = shape->GetSceneNode();
            if (node != NULL)
            {
                loaclPnt = node->GetParent()->GetWorldTransform().Inverse()
                * worldPnt;
            }
        }
    }
    return loaclPnt;
}
    
IntVector2 ViewHelper::WorldPointToScreen(const Vector3& worldPnt,View* view)
{
	IntVector2 scrPnt;
    if (view)
    {
        CameraNode* camera = view->GetSceneManager()->GetCamera();
        const Viewport& viewPort = camera->GetViewPort();
        scrPnt = viewPort.WorldToScreenPoint(worldPnt);
    }
    return scrPnt;
}

float ViewHelper::GetCameraCompareZoom(View* view)
{
	float zoom = 1;
	if (view == NULL)
	{
		return zoom;
	}

	SceneManager* m_SceneManager = view->GetSceneManager();

	CameraNode * camera = m_SceneManager->GetCamera();
	if (camera->IsOrthographic())
	{
		//将缩放转换成透视投影对应的位置
		zoom = camera->GetZoom();
	}
	else
	{
		float defaultZoom = m_SceneManager->GetDefaultZoom();

		Vector3 cameraWorldPos = camera->GetWorldPosition();

		//获取屏幕中心点的交点作为目标点
		Vector3 rotateCenter = m_SceneManager->GetSceneBox().Center();

		float defaultLength = m_SceneManager->GetDefaultFocusLength();
		float lenght = m_SceneManager->GetDefaultFocusLength();

		float zoomFactor = (rotateCenter - cameraWorldPos).Length() / (lenght / 90) * (Tan(90 * 0.5f / 180.f * static_cast<float> (M_PI)) * 2.f);

		zoom = defaultZoom / zoomFactor;
	}
	return zoom;
}

}
