
#include "m3d/M3D.h"
#include "m3d/model/Shape.h"
#include "m3d/base/Matrix4.h"
#include "m3d/SceneManager.h"
#include "m3d/action/RayPickAction.h"

#include "m3d/scene/SectionNode.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/utils/CullerHelper.h"
#include "m3d/action/RenderAction.h"
#include "m3d/model/Model.h"
#include "../graphics/Section.h"

namespace M3D
{

SectionNode::SectionNode() : SceneNode()
{
	m_section = NULL;
	this->SetScene(NULL);
}
 

SectionNode::~SectionNode(void)
{
	if (m_section != NULL)
	{
		m_section->Release();
		this->m_section = NULL;
	}
}

//Note that the order of release
//void ShapeNode::FinalRelease(void)
//{
//	Object::FinalRelease();
//	if (m_shape != NULL)
//	{
//		m_shape->Release();
//	}
//}
 
Section * SectionNode::GetSection()
{
	//从场景树中获得剖面节点
	if (m_section == NULL)
	{
		m_section = new Section;
		m_section->AddRef();
	}

	///给剖面设置包围盒，如果没设置则在查找显示数据时，会被剔除掉
	m_section->SetBox(this->GetScene()->GetSceneBox());
	BoundingBox tempBox = this->GetScene()->GetSceneBox();
	tempBox.ScaleBox(1.3f);
	m_section->SetDrawRection(tempBox);

	return m_section;
}

void SectionNode::RayPick(RayPickAction * action)
{
	if (!m_section)
	{
		return;
	}

	list<SectionPlane*>* planeList = m_section->GetPlaneList();
	if (planeList)
	{
		for (list<SectionPlane*>::iterator it = planeList->begin(); it != planeList->end(); it++)
		{
			SectionPlane* plane = *it;
			if (plane && plane->GetEnable())
			{
				plane->RayPick(action);
			}
		}
	}
}

void SectionNode::ComputeBox()
{
 
}

int SectionNode::GetPickType()
{
	return PICKTYPE_MODEL;
}

int SectionNode::GetType(void)
{
	return SECTION_NODE;
}

void SectionNode::OnMarkedDirty()
{
	if (this->m_section)
	{
		m_section->SetTransform(this->GetWorldTransform());
	}
}

void SectionNode::FindVisiableObject(RenderAction* renderAction)
{
	 if (this->m_section)
	 {
		 m_section->FindVisiableObject(renderAction);
	 }
}

//	//视景体剔除
//	Intersection cullerState = renderAction->GetCullerHelper().InViewPort(this->GetWorldBoundingBox(),renderAction->GetCamera());
//	//微小模型剔除
//	bool littleModelState = renderAction->GetCullerHelper().IsLittleModel(this->GetWorldBoundingBox(),renderAction->GetCamera());
//
//	IShape* shape = this->m_shape;
//	if (shape != NULL)
//	{
//		if(!shape->AllowExculding())
//		{
//			renderAction->SetWorldMatrix(&GetWorldTransform());
//			renderAction->SetGLWorldMatrix(&GetGLWorldTransform());
//			shape->FindVisiableObject(renderAction);
//		}
//		else if (cullerState != OUTSIDE && (!littleModelState))
//		{
//			renderAction->SetWorldMatrix(&GetWorldTransform());
//			renderAction->SetGLWorldMatrix(&GetGLWorldTransform());
//			shape->FindVisiableObject(renderAction);
//		}
//	}
//}
 
}
