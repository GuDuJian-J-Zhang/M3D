
#include "m3d/M3D.h"
#include "m3d/model/Shape.h"
#include "m3d/base/Matrix4.h"
#include "m3d/action/RayPickAction.h"

#include "m3d/scene/ShapeNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/utils/CullerHelper.h"
#include "m3d/action/RenderAction.h"
#include "m3d/model/Model.h"

namespace M3D
{

ShapeNode::ShapeNode() :
		SceneNode()
{
	m_shape = NULL;
}

ShapeNode::ShapeNode(IShape* shape) :
		SceneNode()
{
	this->SetShape(shape);
}

ShapeNode::~ShapeNode(void)
{
	if (m_shape != NULL)
	{
		m_shape->Release();
		this->m_shape = NULL;
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

void ShapeNode::SetShape(IShape *shape)
{
	m_shape = shape;
	if (m_shape)
	{
		m_shape->AddRef();
		m_shape->SetSceneNode(this);
		this->MarkDirty();
	}
}

IShape *
ShapeNode::GetShape(void)
{
	return m_shape;
}

void ShapeNode::DrawForMouseSelect(void)
{

}

void ShapeNode::RayPick(RayPickAction * action)
{
	if (this->m_shape != NULL)
	{
		//框选
		if (action->IsPickAll())
		{
			//if (!m_shape->AllowExculding())
			{
				action->FramePick(this->m_shape);
			}
		}
		else
		{
			//		LOGE("shapeNode : %s",this->GetName().c_str());
			//先通过经过view变换的的camera射线，求射线是否与显示时的包围盒有交点，如果有交点，则继续与shape对象求交
			//		LOGE("worldbox: %s",this->GetWorldBoundingBox().Tostring().c_str());
			if (!m_shape->AllowExculding() || action->GetData()->GetCameraRay().HitDistance(this->GetWorldBoundingBox()) != M_INFINITY)
			{
				//			LOGE("shapeNode : %f");
				//通过shape对象显示时的ModelMatrix，更新Model射线
				action->UpdataModelRay(this->GetWorldTransform());
				//用更新后的Mode射线进行求交
				action->RayPick(this->m_shape);
			}
		}
	}
}

void ShapeNode::ComputeBox()
{
	if (!m_bdBox.Defined()) //位置改变了
	{
		IShape* shape = this->GetShape();
		if (shape != NULL ) //存在shape对象
		{
//			LOGE("ShapeNode::ComputeBox()");
			BoundingBox& tempBox = shape->GetBoundingBox();
//			LOGE("tempBox box %s",tempBox.Tostring().c_str());
			if (tempBox.Defined())
			{
				m_bdBox = tempBox;
			}
		}
	}
}

int ShapeNode::GetPickType()
{
	return PICKTYPE_MODEL;
}

int ShapeNode::GetType(void)
{
	return SHAPE_NODE;
}

void ShapeNode::OnMarkedDirty()
{
	if (this->m_shape)
	{
		this->m_shape->GetBoundingBox().Clear();
		if (this->m_shape->GetType() == SHAPE_MODEL || this->m_shape->GetType() == SHAPE_IMAGE_MODEL)
		{
			Model* model = (Model*)this->m_shape;
			model->SetOrigPlcMatrix(this->GetWorldTransform());
		}
	}
}

void ShapeNode::FindVisiableObject(RenderAction* renderAction)
{
	if (!this->IsVisible())
	{
		return;
	}
	IShape* shape = this->m_shape;
	if (shape != NULL)
	{
		if (!shape->AllowExculding())
		{
			shape->SetRenderVisible(true);
			renderAction->SetWorldMatrix(&GetWorldTransform());
			renderAction->SetGLWorldMatrix(&GetGLWorldTransform());
			shape->FindVisiableObject(renderAction);

			return;
		}
	}

	//微小模型剔除
	int littleModelState = renderAction->GetCullerHelper().IsLittleModel(
			this->GetWorldBoundingBox(), renderAction->GetCamera());

	//非常小的模型
	if (littleModelState == 2)
	{
		if (shape != NULL)
		{
			shape->SetRenderVisible(false);
		}
		return;
	} //小件剔除的模型
	else //其他
	{
		//视景体剔除
		Intersection cullerState = renderAction->GetCullerHelper().InViewPort(
				this->GetWorldBoundingBox(), renderAction->GetCamera());
		if (cullerState != OUTSIDE)
		{
			if (shape != NULL)
			{
				renderAction->SetWorldMatrix(&GetWorldTransform());
				renderAction->SetGLWorldMatrix(&GetGLWorldTransform());

				if (littleModelState == 0)
				{
					shape->SetRenderVisible(true);
					shape->FindVisiableObject(renderAction);
				}
				else if (littleModelState == 1)
				{
					shape->SetRenderVisible(true);
					renderAction->SetDelayDraw(true);
					shape->FindVisiableObject(renderAction);
					renderAction->SetDelayDraw(false);
				}
			}
		}
		else
		{
			IShape* shape = this->m_shape;
			if (shape != NULL)
			{
				shape->SetRenderVisible(false);
			}
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
}

void ShapeNode::Traverse(Action* action)
{
	if (this->m_shape != NULL)
	{
		m_shape->Traverse(action);
	}
}

bool ShapeNode::UpdateName()
{
	string tName = this->GetParent()->GetName();
	this->SetName(tName + "|" + "ShapeModel");
	return true;
}

}
