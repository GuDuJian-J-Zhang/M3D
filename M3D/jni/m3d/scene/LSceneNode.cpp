#include "m3d/M3D.h"
#include "m3d/utils/IDCreator.h"

#include "sview/views/Parameters.h"

#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix4.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/base/Quaternion.h"
#include "m3d/utils/Trackball.h"

#include "m3d/scene/LSceneNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/model/Face.h"

#include "m3d/renderer/GLESHead.h"
#include "m3d/renderer/RenderContext.h"

namespace M3D
{

LSceneNode::LSceneNode(void):Object()
{
	m_strNodeName = "NONAME";
	m_position.ToZero();
	m_rotation.ToZero();
	m_scale.ToOne();
	m_pParent = NULL;
/*	this->SetShape(NULL);*/
 
	m_visible = true;
	this->SetSelected(false);
	/*m_cullerState = CameraNode::FRUSTUM_INTER;*/

	this->m_ID = IDCreator::GetDefaultID();
	this->MarkDirty();
}

LSceneNode::~LSceneNode(void)
{
	for (int i =0;i<m_children.size();i++)
	{
		m_children[i]->Release();
	}

// 	if(this->m_shape)
// 	{
// 		this->m_shape->Release();
// 		this->m_shape = NULL;
// 	}
}

void LSceneNode::SetID(IDTYPE id)
{
	this->m_ID = id;
}

IDTYPE LSceneNode::GetID()
{
	return this->m_ID;
}

void  LSceneNode::Optimize()
{
	//优化内存占用
	this->m_children.shrink_to_fit();
}

SHAPETYPE LSceneNode::GetType(void)
{
	return LSCENE_NODE;
}

void LSceneNode::SetName(const string& name)
{
	m_strNodeName = name;
}

string& LSceneNode::GetName()
{
	return m_strNodeName;
}
 
 
bool LSceneNode::Visible()
{
	return this->m_visible;
}

void LSceneNode::SetVisible(bool m_visible)
{
	this->m_visible = m_visible;
}

void LSceneNode::SetSelected(bool selected)
{
	this->m_selected = selected;

	for (int i = 0; i<m_children.size(); i++)
	{
		m_children[i]->SetSelected(selected);
	}
}

bool LSceneNode::IsSelected()
{
	return this->m_selected;
}

void  LSceneNode::AddChild(LSceneNode* childNode)
{
	childNode->SetParent(this);
	this->m_children.push_back(childNode);
}

LFace * LSceneNode::GetShape(void)
{
	return NULL;
}

BoundingBox LSceneNode::GetBoundingBox()
{
	//根据所包含子模型大小
 
		BoundingBox tempBox;
 
// 		LFace* shape = this->m_shape;
// 		if (shape != NULL)
// 		{
// 			tempBox.Merge(shape->GetBoundingBox());
// 		}

// 		//和所有下级模型的世界包围盒重新进行计算
// 		for (int i = 0; i < m_children.size(); i++)
// 		{
// 			BoundingBox& subBox =
// 				m_children[i]->GetWorldBoundingBox();
// 			if (subBox.Defined())
// 			{
// 				tempBox.Merge(subBox);
// 			}
// 		}
//      return tempBox;
		return BoundingBox();
}
 
BoundingBox& LSceneNode:: GetWorldBoundingBox()
{
// 	if(!m_worldBox.Defined())
// 	{
// 		BoundingBox bdBox = GetBoundingBox();
// 		if(bdBox.Defined())
// 		{
// 			 bdBox.Transform(this->GetWorldTransform());
// 			 m_worldBox = bdBox;
// 		}
// 
// 		 LSceneNode::Optimize();
// 	}

	return BoundingBox::NO_BOX;
}

void LSceneNode::Traverse(Action* action)
{
	if (action->IsFinish())
	{
		return;
	}
	action->Apply(this);

	for(int i= 0;i<this->m_children.size();++i)
	{
		m_children[i]->Traverse(action);
	}
}
 
void LSceneNode::RayPick(RayPickAction * action)
{
// 	LFace* shape = this->m_shape;
// 	if (shape != NULL)
// 	{
// 		//LOGE("shapeNode : %s",this->GetName().c_str());
// 		//先通过经过view变换的的camera射线，求射线是否与显示时的包围盒有交点，如果有交点，则继续与shape对象求交
// 		//LOGE("worldbox: %s",this->GetWorldBoundingBox().Tostring().c_str());
// 		if(action->GetData()->GetCameraRay().HitDistance(this->GetWorldBoundingBox()) != M_INFINITY)
// 		{
// 			//没有网格数据，通过包围盒求交计算
// 			if(SVIEW::Parameters::Instance()->m_simplityMode)
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
// 				//LOGE("shapeNode : %f");
// 				//通过shape对象显示时的ModelMatrix，更新Model射线
// 				action->UpdataModelRay(this->GetWorldTransform());
// 				//用更新后的Mode射线进行求交
// 				action->RayPick(this->m_shape);
// 
// 				action->AddShape(shape);
// 			}
// 		}
// 	}
 
	for(int i= 0;i<this->m_children.size();++i)
	{
		m_children[i]->RayPick(action);
	}
}
 
void LSceneNode::SetLocalTransform(const Matrix3x4& localMatrix)
{
	SetTransform(localMatrix.Translation(), localMatrix.Rotation(), localMatrix.Scale());
	this->MarkDirty();
}

Matrix3x4 LSceneNode::GetLocalTransform()const
{
	return  GetTransform();
}

Matrix3x4 LSceneNode::GetTransform() const
{
	return Matrix3x4(m_position, m_rotation, m_scale);
}


void LSceneNode::SetWorldTransform(const Matrix3x4& worldMatrix)
{
	Matrix3x4 localTransform;
	if (m_pParent == NULL)
	{
		localTransform = worldMatrix;
	}
	else
	{
		localTransform = m_pParent->GetWorldTransform().Inverse() * worldMatrix;
	}
 
	this->SetLocalTransform(localTransform);
}

Matrix3x4& LSceneNode::GetWorldTransform()const
{
	if (this->IsDirty())
	{
		this->UpdateWorldTransform();
	}

	return this->m_worldMatrix;
}

void LSceneNode::UpdateWorldTransform() const
{
    Matrix3x4 transform = GetLocalTransform();
 
    if (m_pParent == NULL)
    {
    	m_worldMatrix = transform;
    }
    else
    {
    	m_worldMatrix = m_pParent->GetWorldTransform() * transform;
    }

   /* this->m_glRenderMatrix = this->m_worldMatrix.ToMatrix4().Transpose();*/

    m_dirty = false;
}

void LSceneNode::FindVisiableObject(RenderAction* renderAction)
{
		//视景体剔除
// 	Intersection cullerState = renderAction->GetCullerHelper().InViewPort(this->GetWorldBoundingBox(),renderAction->GetCamera());
// 	//微小模型剔除
// 	bool littleModelState = renderAction->GetCullerHelper().IsLittleModel(this->GetWorldBoundingBox(),renderAction->GetCamera());
// 
// 	LFace* shape = this->m_shape;
// 	if (shape != NULL)
// 	{
// // 		if(!shape->AllowExculding())
// // 		{
// // 			renderAction->SetWorldMatrix(&GetWorldTransform());
// // 			renderAction->SetGLWorldMatrix(&GetGLWorldTransform());
// // 			shape->FindVisiableObject(renderAction);
// // 		}
// // 		else
// 		if (cullerState != OUTSIDE)
// 		{
// 			renderAction->SetWorldMatrix(&GetWorldTransform());
// 			renderAction->SetGLWorldMatrix(&GetGLWorldTransform());
// 			if(!littleModelState)
// 			{
// 				shape->FindVisiableObject(renderAction);
// 			}else
// 			{
// 				renderAction->SetDelayDraw(true);
// 				shape->FindVisiableObject(renderAction);
// 				renderAction->SetDelayDraw(false);
// 			}
// 		}
// 	}

	for(int i= 0;i<this->m_children.size();++i)
	{
		m_children[i]->FindVisiableObject(renderAction);
	}
}
 
void LSceneNode::SetTransform(const Vector3& position, const Quaternion& rotation)
{
    m_position = position;
    m_rotation = rotation;
    MarkDirty();
}

void LSceneNode::SetTransform(const Vector3& position, const Quaternion& rotation, float scale)
{
    SetTransform(position, rotation, Vector3(scale, scale, scale));
}

void LSceneNode::SetTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
    m_position = position;
    m_rotation = rotation;
    m_scale = scale;
    MarkDirty();
}
 
 
void LSceneNode::MarkDirty()
{
    m_dirty = true;
   /* m_worldBox.Clear();*/
    this->OnMarkedDirty();
}

void LSceneNode::OnMarkedDirty()
{

}
 
vector<LSceneNode*>& LSceneNode::GetChildren()
{
	return m_children;
}

LSceneNode * LSceneNode::GetParent(void)
{
	return m_pParent;
}

void LSceneNode::SetParent(LSceneNode *node)
{
	m_pParent = node;
	//node->m_pSubChildren = this;
}

}
