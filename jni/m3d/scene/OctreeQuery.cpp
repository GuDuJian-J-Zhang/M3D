#include "m3d/scene/OctreeQuery.h"
#include "m3d/model/ModelShape.h"
#include "m3d/action/RenderAction.h"
#include "m3d/utils/CullerHelper.h"
#include "m3d/graphics/CameraNode.h"

namespace M3D
{

void OctreeQuery::SetRenderAction(RenderAction* action)
{
	this->m_renderAction = action;
}

RenderAction* OctreeQuery::GetRenderAction()
{
	return this->m_renderAction;
}

void OctreeQuery::SetCamera(CameraNode* camera)
{
	this->m_camera = camera;
}

CameraNode* OctreeQuery::GetCamera()
{
	return this->m_camera;
}

Intersection PointOctreeQuery::TestOctant(const BoundingBox& box, bool inside)
{
    if (inside)
        return INSIDE;
    else
        return box.IsInside(m_point);
}

void PointOctreeQuery::TestDrawables(ModelShape** start, ModelShape** end, bool inside)
{
    while (start != end)
    {
		ModelShape* drawable = *start++;

        //if ((drawable->GetDrawableFlags() & m_drawableFlags) && (drawable->GetViewMask() & m_viewMask))
        //{
        //    if (inside || drawable->GetWorldBoundingBox().IsInside(m_point))
        //        m_result.push_back(drawable);
        //}
    }
}

Intersection SphereOctreeQuery::TestOctant(const BoundingBox& box, bool inside)
{
    if (inside)
        return INSIDE;
    else
        return m_sphere.IsInside(box);
}

void SphereOctreeQuery::TestDrawables(ModelShape** start, ModelShape** end, bool inside)
{
    while (start != end)
    {
		ModelShape* drawable = *start++;

        //if ((drawable->GetDrawableFlags() & m_drawableFlags) && (drawable->GetViewMask() & m_viewMask))
        //{
        //    if (inside || m_sphere.IsInsideFast(drawable->GetWorldBoundingBox()))
        //        m_result.push_back(drawable);
        //}
    }
}

Intersection BoxOctreeQuery::TestOctant(const BoundingBox& box, bool inside)
{
    if (inside)
        return INSIDE;
    else
        return m_box.IsInside(box);
}

void BoxOctreeQuery::TestDrawables(ModelShape** start, ModelShape** end, bool inside)
{
    while (start != end)
    {
		ModelShape* drawable = *start++;

        //if ((drawable->GetDrawableFlags() & m_drawableFlags) && (drawable->GetViewMask() & m_viewMask))
        //{
        //    if (inside || m_box.IsInsideFast(drawable->GetWorldBoundingBox()))
        //        m_result.push_back(drawable);
        //}
    }
}

Intersection FrustumOctreeQuery::TestOctant(const BoundingBox& box, bool inside)
{
    if (inside)
        return INSIDE;
	else
	{
		return m_frustum.IsInside(box);
		
	}
}

void FrustumOctreeQuery::TestDrawables(ModelShape** start, ModelShape** end, bool inside)
{
    while (start != end)
    {
		ModelShape* drawable = *start++;
		if (this->m_renderAction && this->m_camera && drawable->AllowCuller())
		{
			int littleModelState = this->m_renderAction->GetCullerHelper().IsLittleModel(drawable->GetWorldBoundingBox(), this->m_camera);
				////非常小的模型
			if (littleModelState == 2)
			{
				return;
			}
		}

		//if (inside /*|| m_frustum.IsInsideFast(drawable->GetWorldBoundingBox())*/){
			m_result.push_back(drawable);
		//}
    }
}


Intersection AllContentOctreeQuery::TestOctant(const BoundingBox& box, bool inside)
{
    return INSIDE;
}

void AllContentOctreeQuery::TestDrawables(ModelShape** start, ModelShape** end, bool inside)
{
    while (start != end)
    {
		//LModelShape* drawable = *start++;

  //      if ((drawable->GetDrawableFlags() & m_drawableFlags) && (drawable->GetViewMask() & m_viewMask))
  //          m_result.push_back(drawable);
    }
}

}
