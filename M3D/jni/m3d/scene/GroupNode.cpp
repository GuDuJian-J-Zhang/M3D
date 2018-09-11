#include "m3d/M3D.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/model/Model.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/DirectionalLight.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/Action/RaypickAction.h"


//--------------------------- GroupLink --------------------------
namespace M3D
{
SArrayList::SArrayList(void)
{
	m_DataArray.clear();
}
SArrayList::~SArrayList(void)
{
	for (int i = 0; i < m_DataArray.size(); i++)
	{
		if (m_DataArray[i])
		{
			m_DataArray[i]->Release();
			m_DataArray[i] = NULL;
		}
	}
}

void SArrayList::SetNode(SceneNode * node)
{

}

void SArrayList::AddNode(SceneNode* node)
{
	node->AddRef();
	m_DataArray.push_back(node);
}

void SArrayList::AddNodeBefore(SceneNode* node, const string& name)
{
	for (int i = 0; i < m_DataArray.size(); i++)
	{
		if (name == m_DataArray[i]->GetName())
		{
			node->AddRef();
			m_DataArray.insert(m_DataArray.begin() + i, node);
			break;
		}
	}
}

void SArrayList::AddNodeAfter(SceneNode* node, const string& name)
{
    //TODO
	node->AddRef();
}

void SArrayList::RemoveNode(const string& name)
{
	for (int i = 0; i < m_DataArray.size(); i++)
	{
		if (m_DataArray[i]->GetName() == name)
		{
//			delete m_DataArray[i];
			m_DataArray[i]->Release();
			m_DataArray[i] = NULL;
			m_DataArray.erase(m_DataArray.begin() + i);
		}
	}
}

void SArrayList::RemoveNode(const SceneNode* node)
{
	for (int i = 0; i < m_DataArray.size(); i++)	
		{
			if (m_DataArray[i] == node)
			{
//				delete m_DataArray[i];
				m_DataArray[i]->Release();
				m_DataArray[i] = NULL;
				m_DataArray.erase(m_DataArray.begin() + i);
			}
		}
}


void SArrayList::RemoveNode(int id)
{
	for (int i = 0; i < m_DataArray.size(); i++)
	{
		if (m_DataArray[i]->GetID() == id)
		{
//			delete m_DataArray[i];
			m_DataArray[i]->Release();
			m_DataArray[i] = NULL;
			m_DataArray.erase(m_DataArray.begin() + i);
		}
	}
}
SceneNode* SArrayList::GetNode(const string& name)
{
	for (int i = 0; i < m_DataArray.size(); i++)
	{
		if (m_DataArray[i]->GetName() == name)
		{
			//未做删除指针操作
			return m_DataArray[i];
		}
	}
	return NULL;
}

SceneNode* SArrayList::GetSceneNode(int i)
{
	if (i < m_DataArray.size() && i >= 0)
	{
		return m_DataArray[i];
	}
	return NULL;
}
int SArrayList::Size()
{
	return m_DataArray.size();
}

void SArrayList::Clear(void)
{
	for (int i = 0; i < m_DataArray.size(); i++)
	{
//		delete m_DataArray[i];
		m_DataArray[i]->Release();
		m_DataArray[i] = NULL;
	}
	m_DataArray.clear();
}

void SArrayList::DetachChild(SceneNode* child)
{
	for (int i = 0; i < m_DataArray.size(); i++)
	{
		if (m_DataArray[i] == child)
		{
			child->SetParent((SceneNode *)NULL);
			m_DataArray.erase(m_DataArray.begin() + i);
		}
	}
}

GroupNode::GroupNode(void) :
		SceneNode()
{
	m_pChildrenList = new SArrayList;
}

GroupNode::~GroupNode(void)
{
//	LOGI("Group node begin clear");
	if(m_pChildrenList)
	{
	delete m_pChildrenList;

	m_pChildrenList = NULL;
	}
//	LOGI("Group node end clear");
}

SceneNode*GroupNode::Search(const string& name)
{
	if (m_strNodeName == name)
		return this;
	for (int i = 0; i < m_pChildrenList->Size(); i++)
	{
		SceneNode *node = m_pChildrenList->GetSceneNode(i)->Search(name);
		if (node != NULL)
		{
			return node;
		}
	}
	return NULL;
}

SceneNode*GroupNode::Search(const long id)
{
	if (this->m_ID == id)
		return this;
	for (int i = 0; i < m_pChildrenList->Size(); i++)
	{
		SceneNode *node = m_pChildrenList->GetSceneNode(i)->Search(id);
		if (node != NULL)
		{
			return node;
		}
	}
	return NULL;
}

void GroupNode::Traverse(Action* action)
{
	// LOGI("NODE Name is %s",this->GetName().c_str());
	if (action->IsFinish())
	{
		return;
	}
	action->Apply(this);
	for (int i = 0; i < m_pChildrenList->Size(); i++)
	{
		m_pChildrenList->GetSceneNode(i)->Traverse(action);
	}
}


void GroupNode::FindVisiableObject(RenderAction* renderAction)
{
	if (this->IsVisible())
	{
		for (int i = 0; i < m_pChildrenList->Size(); i++)
		{
			m_pChildrenList->GetSceneNode(i)->FindVisiableObject(renderAction);
		}
	}
}

void GroupNode::AddChild(SceneNode *child)
{
	MutexLock locked(this->m_mutex);
	child->SetParent(this);
	m_pChildrenList->AddNode(child);
}

void GroupNode::AddChildInSameLevelBefore(SceneNode *child,
		const string& nodeName)
{
	MutexLock locked(this->m_mutex);
	child->SetParent(this);
	m_pChildrenList->AddNodeBefore(child, nodeName);
}

void GroupNode::AddChildAfter(SceneNode *child, const string& nodeName)
{
	MutexLock locked(this->m_mutex);
	child->SetParent(this);

	m_pChildrenList->AddNode(child);
}

void GroupNode::SetChild(int index, SceneNode *child)
{

}

void GroupNode::OnMarkedDirty()
{
	for (int i = 0; i < m_pChildrenList->Size(); i++)
	{
		//告诉子节点，更新状态
		SceneNode* node = m_pChildrenList->GetSceneNode(i);
		node->MarkDirty();
	}
}

int GroupNode::Size()
{
	return m_pChildrenList->Size();
}

int GroupNode::GetChildIndex(const string& name)
{
	return -1;
}

SceneNode* GroupNode::GetChild(int index)
{
	return m_pChildrenList->GetSceneNode(index);
}

SceneNode*
GroupNode::GetChild(const string& name)
{
	return NULL;
}

void GroupNode::DeleteChild(int id)
{
	MutexLock locked(this->m_mutex);
	if (m_pChildrenList != NULL)
	{
		m_pChildrenList->RemoveNode(id);
	}
}

void GroupNode::DeleteAllChildren()
{
	MutexLock locked(this->m_mutex);
	if (m_pChildrenList != NULL)
	{
		m_pChildrenList->Clear();
	}
}

void GroupNode::DeleteChildInAllSub(int id)
{
	///线程资源锁定
	MutexLock locked(this->m_mutex);
	SceneNode* node = this->Search(id);
	GroupNode* para = (GroupNode*) node->GetParent();
	if (para != NULL)
	{
		para->DeleteChild(id);
	}
}

void GroupNode::DeleteChild(const string& name)
{
	if (m_pChildrenList != NULL)
	{
		m_pChildrenList->RemoveNode(name);
	}
}

void GroupNode::DeleteChild(const SceneNode* node)
{
	if(node == NULL)
	{
		return;
	}

	if (m_pChildrenList != NULL)
	{
		m_pChildrenList->RemoveNode(node);
	}
}

//void GroupNode::FinalRelease(void)
//{
//	delete m_pChildrenList;
//	Object::FinalRelease(); // Container.h
//
//	//m_pChildrenList->Clear();
//}

void GroupNode::RayPick(RayPickAction * action)
{
	for (int i = 0; i < m_pChildrenList->Size(); i++)
	{
		m_pChildrenList->GetSceneNode(i)->RayPick(action);
	}
}

void GroupNode::ComputeBox()
{
	if (!m_bdBox.Defined())
	{
		BoundingBox tempBox;
		for (int i = 0; i < m_pChildrenList->Size(); i++)
		{
			BoundingBox& subBox =
					m_pChildrenList->GetSceneNode(i)->GetBoundingBox();
			if (subBox.Defined())
			{
				tempBox.Merge(subBox);
			}
		}

		if (tempBox.Defined())
		{
			tempBox.Transform(this->GetWorldTransform());
			m_bdBox = tempBox ;
		}
	}
}

int GroupNode::GetType(void)
{
	return GROUP_NODE;
}

void GroupNode::DetachChild(SceneNode* node)
{
	if(node == NULL)
	{
		return;
	}

	if (m_pChildrenList != NULL)
	{
		m_pChildrenList->DetachChild(node);
	}
}

bool GroupNode::UpdateName()
{
	for(int i=0;i<this->m_pChildrenList->Size();i++)
	{
		this->m_pChildrenList->GetSceneNode(i)->UpdateName();
	}
	return true;
}
}


