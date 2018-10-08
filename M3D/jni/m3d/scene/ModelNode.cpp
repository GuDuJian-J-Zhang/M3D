#include "m3d/M3D.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/scene/ModelNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/utils/CullerHelper.h"
#include "m3d/model/Model.h"
namespace M3D
{

ModelNode::ModelNode(void) :
		GroupNode()
{
	Init();
}

void ModelNode::Init()
{
	this->shapeNode = NULL;
}

ModelNode::~ModelNode(void)
{
	if (shapeNode)
	{
		this->shapeNode->Release();

		this->shapeNode = NULL;
	}
}

ShapeNode* ModelNode::GetShapeNode()
{
	return this->shapeNode;
}

/**
 * 设置模型显示数据节点
 * @param shapeNode
 */
void ModelNode::SetShapeNode(ShapeNode* shapeNode)
{
	shapeNode->AddRef();
	shapeNode->SetParent(this);
	this->shapeNode = shapeNode;
}

int ModelNode::GetType(void)
{
	return MODEL_NODE;
}

void ModelNode::Traverse(Action* action)
{
	// LOGI("NODE Name is %s",this->GetName().c_str());
	if (action->IsFinish())
	{
		return;
	}
	action->Apply(this);

	ShapeNode* shapeNode = this->shapeNode;

	if (shapeNode != NULL)
	{
		shapeNode->Traverse(action);
	}

	for (int i = 0; i < m_pChildrenList->Size(); i++)
	{
		m_pChildrenList->GetSceneNode(i)->Traverse(action);
	}

}

void ModelNode::FindVisiableObject(RenderAction* renderAction)
{
	ShapeNode* shapeNode = this->shapeNode;

	if (shapeNode != NULL)
	{
		shapeNode->FindVisiableObject(renderAction);
	}

	for (int i = 0; i < m_pChildrenList->Size(); i++)
	{
		m_pChildrenList->GetSceneNode(i)->FindVisiableObject(renderAction);
	}
}

void ModelNode::OnMarkedDirty()
{
	ShapeNode* shapeNode = this->shapeNode;

	if (shapeNode != NULL)
	{
		shapeNode->MarkDirty();
	}

	GroupNode::OnMarkedDirty();
}

int ModelNode::GetParentCullerState()
{
	int parentState = CameraNode::FRUSTUM_INTER;
	SceneNode* parent = this->GetParent();
	if (parent != NULL)
	{
		parentState = parent->GetCullerState();
	}
	return parentState;
}

SceneNode* ModelNode::Search(const string& name)
{
	SceneNode* node = GroupNode::Search(name);
	if (node == NULL)
	{
		ShapeNode* shapeNode = this->shapeNode;
		if (shapeNode != NULL)
		{
			node = shapeNode->Search(name);
		}
	}

	return node;
}

SceneNode* ModelNode::Search(const long id)
{
	SceneNode* node = GroupNode::Search(id);
	if (node == NULL)
	{
		ShapeNode* shapeNode = this->shapeNode;
		if (shapeNode != NULL)
		{
			node = shapeNode->Search(id);
		}
	}

	return node;
}

void ModelNode::ComputeBox()
{
	if (!m_bdBox.Defined())
	{
		MutexLock lock(m_mutex);

		BoundingBox tempBox;
		ShapeNode* shapeNode = this->shapeNode;
		if (shapeNode != NULL)
		{
			tempBox.Merge(shapeNode->GetBoundingBox());
		}

		for (int i = 0; i < m_pChildrenList->Size(); i++)
		{
			BoundingBox subBox =
					m_pChildrenList->GetSceneNode(i)->GetBoundingBox();
			if (subBox.Defined())
			{
					tempBox.Merge(subBox.Transformed(m_pChildrenList->GetSceneNode(i)->GetLocalTransform()));
			}
		}

		if(tempBox.Defined())
		{
//			tempBox.Transform(this->GetLocalTransform());
			m_bdBox = tempBox;
		}
	}
}

bool ModelNode::UpdateName()
{
	if(this->shapeNode)
	{
		Shape* shape = shapeNode->GetShape();
		if(shape&& shape->GetType() == SHAPE_MODEL)
		{
			SceneNode* parent = this->GetParent();
			Model* model = (Model*)shape;

			char strID[10];
			sprintf(strID, "%x", model->GetPlcId());
			string tName = parent->GetName() + "|" + strID;
			this->SetName(tName);
			shapeNode->UpdateName();
		}
	}
	GroupNode::UpdateName();
	return true;
}
}
