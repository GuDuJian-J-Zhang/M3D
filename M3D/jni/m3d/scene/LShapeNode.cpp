#include "m3d/M3D.h"
#include "m3d/utils/IDCreator.h"

#include "sview/views/Parameters.h"

#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix4.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/base/Quaternion.h"
#include "m3d/utils/Trackball.h"

#include "m3d/scene/LShapeNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/model/Face.h"

#include "m3d/renderer/GLESHead.h"
#include "m3d/renderer/RenderContext.h"

namespace M3D
{

LShapeNode::LShapeNode(void):LSceneNode()
{
// 	m_strNodeName = "NONAME";
// 	m_position.ToZero();
// 	m_rotation.ToZero();
// 	m_scale.ToOne();
	this->m_shapes.resize(0);
	//this->AddShape(NULL);
  
/*	m_cullerState = CameraNode::FRUSTUM_INTER;*/
}

LShapeNode::~LShapeNode(void)
{
// 	for (int i =0;i<m_children.size();i++)
// 	{
// 		m_children[i]->Release();
// 	}
 	for (int i =0;i<m_shapes.size();i++)
 	{
		m_shapes[i]->Release();
 	}

	this->m_shapes.resize(0);
}

// void LShapeNode::SetID(long id)
// {
// 	this->m_ID = id;
// }
// 
// long LShapeNode::GetID()
// {
// 	return this->m_ID;
// }

// void  LShapeNode::Optimize()
// {
// 	//优化内存占用
// 	this->m_children.shrink_to_fit();
// }

SHAPETYPE LShapeNode::GetType(void)
{
	return LSHAPE_NODE;
}

// void LShapeNode::SetName(const string& name)
// {
// 	m_strNodeName = name;
// }
// 
// string& LShapeNode::GetName()
// {
// 	return m_strNodeName;
// }
 
 
// bool LShapeNode::Visible()
// {
// 	return this->m_visible;
// }
// 
// void LShapeNode::SetVisible(bool isHide)
// {
// 	this->m_visible = isHide;
// }

// void  LShapeNode::AddChild(LShapeNode* childNode)
// {
// 	childNode->m_pParent = this;
// 	this->m_children.push_back(childNode);
// }

// void LShapeNode::SetShape(Shape *shape)
// {
// 	m_shape = shape;
// 	if (m_shape)
// 	{
// 		m_shape->AddRef();
// 	}
// }
// 
// Shape * LShapeNode::GetShape(void)
// {
// 	return m_shape;
// }

void LShapeNode::AddShape(LFace *shape)
{
	if (shape)
	{
		shape->AddRef();
		shape->SetParent(this);
		m_shapes.push_back(shape);
	}
	
}

vector<LFace*>& LShapeNode::GetShapes(void)
{
	return m_shapes;
}

BoundingBox LShapeNode::GetBoundingBox()
{
	//根据所包含子模型大小
 
		BoundingBox tempBox;

		for (int i = 0; i<m_shapes.size(); i++)
		{
			tempBox.Merge(m_shapes[i]->GetBoundingBox());
		}

		//和所有下级模型的世界包围盒重新进行计算
// 		for (int i = 0; i < m_children.size(); i++)
// 		{
// 			BoundingBox& subBox =
// 				m_children[i]->GetWorldBoundingBox();
// 			if (subBox.Defined())
// 			{
// 				tempBox.Merge(subBox);
// 			}
// 		}
     return tempBox;
}
 
BoundingBox& LShapeNode:: GetWorldBoundingBox()
{
	if(!m_worldBox.Defined())
	{
		BoundingBox bdBox = GetBoundingBox();
		if(bdBox.Defined())
		{
			 bdBox.Transform(this->GetWorldTransform());
			 m_worldBox = bdBox;
		}

		 LShapeNode::Optimize();
	}

	return m_worldBox;
}

void LShapeNode::Traverse(Action* action)
{
	if (action->IsFinish())
	{
		return;
	}
	action->Apply(this);

// 	for(int i= 0;i<this->m_children.size();++i)
// 	{
// 		m_children[i]->Traverse(action);
// 	}
}
 
void LShapeNode::RayPick(RayPickAction * action)
{
	if (m_shapes.size() > 0)
	{
		//LOGE("shapeNode : %s",this->GetName().c_str());
		//先通过经过view变换的的camera射线，求射线是否与显示时的包围盒有交点，如果有交点，则继续与shape对象求交
		//LOGE("worldbox: %s",this->GetWorldBoundingBox().Tostring().c_str());
		if(action->GetData()->GetCameraRay().HitDistance(this->GetWorldBoundingBox()) != M_INFINITY)
		{
			//没有网格数据，通过包围盒求交计算
// 			if(SVIEW::Parameters::Instance()->m_bufferType == GPUObject::GPU_CACHE)
// 			{
// 				vector<Vector3> intersectPnts;
// 				if(RayPickAction::RayIntersectBoxPnt(action->GetData()->GetCameraRay(),this->GetWorldBoundingBox(),intersectPnts))
// 				{
// 					action->UpdataModelRay(Matrix3x4::IDENTITY);
// 					for (int i= 0;i<intersectPnts.size();i++)
// 					{
// 						 action->AddIntersectPnt(intersectPnts[i]);
// 					}
// 					action->AddShape(shape);
// 				}
// 			}
// 			else
// 			{
				//LOGE("shapeNode : %f");
				//通过shape对象显示时的ModelMatrix，更新Model射线
				action->UpdataModelRay(this->GetWorldTransform());
				for (int i = 0; i<m_shapes.size(); i++)
				{
					LFace* shape = m_shapes[i];
					//用更新后的Mode射线进行求交
					action->RayPick(shape);
					action->AddShape(shape);
				}

/*			}*/
		}
	}
 
// 	for(int i= 0;i<this->m_children.size();++i)
// 	{
// 		m_children[i]->RayPick(action);
// 	}
}
 
// void LShapeNode::SetCullerState(int state)
// {
// 	this->m_cullerState = state;
// }

// int LShapeNode::GetCullerState()
// {
// 	return this->m_cullerState;
// }
 
// void LShapeNode::SetLocalTransform(const Matrix3x4& localMatrix)
// {
// 	SetTransform(localMatrix.Translation(), localMatrix.Rotation(), localMatrix.Scale());
// 	this->MarkDirty();
// }
// 
// Matrix3x4 LShapeNode::GetLocalTransform()const
// {
// 	return  GetTransform();
// }

// Matrix3x4 LShapeNode::GetTransform() const
// {
// 	return Matrix3x4(m_position, m_rotation, m_scale);
// }
// 
// 
// void LShapeNode::SetWorldTransform(const Matrix3x4& worldMatrix)
// {
// 	Matrix3x4 localTransform;
// 	if (m_pParent == NULL)
// 	{
// 		localTransform = worldMatrix;
// 	}
// 	else
// 	{
// 		localTransform = m_pParent->GetWorldTransform().Inverse() * worldMatrix;
// 	}
//  
// 	this->SetLocalTransform(localTransform);
// }

// Matrix3x4& LShapeNode::GetWorldTransform()const
// {
// 	if (this->IsDirty())
// 	{
// 		this->UpdateWorldTransform();
// 	}
// 
// 	return this->m_worldMatrix;
// }

Matrix4& LShapeNode::GetGLWorldTransform()const
{
	this->GetWorldTransform();
	return this->m_glRenderMatrix;
}

void LShapeNode::UpdateWorldTransform() const
{
	LSceneNode::UpdateWorldTransform();

    this->m_glRenderMatrix = this->m_worldMatrix.ToMatrix4().Transpose();

    m_dirty = false;
}

void LShapeNode::SetSelected(bool selected)
{
	LSceneNode::SetSelected(selected);

	for (int i = 0; i<m_shapes.size(); i++)
	{
		LFace* shape = m_shapes[i];
		shape->SetSelected(selected);
	}
}

bool LShapeNode::IsSelected()
{
	return LSceneNode::IsSelected();
}

void LShapeNode::FindVisiableObject(RenderAction* renderAction)
{
	//微小模型剔除
	int littleModelState = renderAction->GetCullerHelper().IsLittleModel(this->GetWorldBoundingBox(),renderAction->GetCamera());
	
	//非常小的模型
	if(littleModelState == 2)
	{
		return;
	} //小件剔除的模型
	else //其他
	{
		//视景体剔除
		Intersection cullerState = renderAction->GetCullerHelper().InViewPort(this->GetWorldBoundingBox(),renderAction->GetCamera());
		if (cullerState != OUTSIDE)
		{
			renderAction->SetWorldMatrix(&GetWorldTransform());
			renderAction->SetGLWorldMatrix(&GetGLWorldTransform());

			for (int i = 0; i<m_shapes.size(); i++)
			{
				LFace* shape = m_shapes[i];

				if (littleModelState == 0)
				{
					shape->FindVisiableObject(renderAction);
				}
				else if (littleModelState == 1)
				{
					renderAction->SetDelayDraw(true);
					shape->FindVisiableObject(renderAction);
					renderAction->SetDelayDraw(false);
				}
			}
		}
	}
// 		if(!shape->AllowExculding())
// 		{
// 			renderAction->SetWorldMatrix(&GetWorldTransform());
// 			renderAction->SetGLWorldMatrix(&GetGLWorldTransform());
// 			shape->FindVisiableObject(renderAction);
// 		}
// 		else
		
/*	}*/
// 
// 	for(int i= 0;i<this->m_children.size();++i)
// 	{
// 		m_children[i]->FindVisiableObject(renderAction);
// 	}
}

void LShapeNode::FindVisiableObjectFast(RenderAction* renderAction,int littleModelState)
{
		//视景体剔除
		Intersection cullerState = renderAction->GetCullerHelper().InViewPort(this->GetWorldBoundingBox(), renderAction->GetCamera());
		if (cullerState != OUTSIDE)
		{
			renderAction->SetWorldMatrix(&GetWorldTransform());
			renderAction->SetGLWorldMatrix(&GetGLWorldTransform());

			for (int i = 0; i<m_shapes.size(); i++)
			{
				LFace* shape = m_shapes[i];

				if (littleModelState == 0)
				{
					shape->FindVisiableObject(renderAction);
				}
				else if (littleModelState == 1)
				{
					renderAction->SetDelayDraw(true);
					shape->FindVisiableObject(renderAction);
					renderAction->SetDelayDraw(false);
				}
			}
	}
	// 		if(!shape->AllowExculding())
	// 		{
	// 			renderAction->SetWorldMatrix(&GetWorldTransform());
	// 			renderAction->SetGLWorldMatrix(&GetGLWorldTransform());
	// 			shape->FindVisiableObject(renderAction);
	// 		}
	// 		else

	/*	}*/
	// 
	// 	for(int i= 0;i<this->m_children.size();++i)
	// 	{
	// 		m_children[i]->FindVisiableObject(renderAction);
	// 	}
}
 
// void LShapeNode::SetTransform(const Vector3& position, const Quaternion& rotation)
// {
//     m_position = position;
//     m_rotation = rotation;
//     MarkDirty();
// }

// void LShapeNode::SetTransform(const Vector3& position, const Quaternion& rotation, float scale)
// {
//     SetTransform(position, rotation, Vector3(scale, scale, scale));
// }

// void LShapeNode::SetTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
// {
//     m_position = position;
//     m_rotation = rotation;
//     m_scale = scale;
//     MarkDirty();
// }
 
// void LShapeNode::MarkDirty()
// {
//     m_dirty = true;
//     m_worldBox.Clear();
//     this->OnMarkedDirty();
// }
// 
// void LShapeNode::OnMarkedDirty()
// {
// 
// }
//  
// bool LShapeNode::UpdateName()
// {
// 	return false;
// }
}
