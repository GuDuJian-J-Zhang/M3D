/**@file
 *@brief    组容器节点
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */

#ifndef   M3D_GROUP_NODE_H_
#define   M3D_GROUP_NODE_H_

#include "m3d/scene/SceneNode.h"
namespace M3D
{
class Action;
class Model;


/**
 * 数组的封装，提供节点的添加删除，解除引用操作
 */
class M3D_API SArrayList
{
public:
	SArrayList(void);
	virtual ~SArrayList(void);
	void SetNode(SceneNode * node);
	void AddNode(SceneNode* node);
	void AddNodeBefore(SceneNode* node, const string& name);
	void AddNodeAfter(SceneNode* node, const string& name);
	void RemoveNode(const string& name);
	void RemoveNode(const SceneNode* node);
	void RemoveNode(int i);
	void DetachChild(SceneNode * child);//CdJ 解除关系
	SceneNode* GetNode(const string& name);

	void Clear(void);
	SceneNode* GetSceneNode(int i);
	int Size();

private:
	int m_Count;
	vector<SceneNode *> m_DataArray;
};

class M3D_API GroupNode: public SceneNode
{
public:
	GroupNode(void);
	virtual ~GroupNode(void);

	virtual int GetType(void);

	virtual void RayPick(RayPickAction * action);

	/**
	 * 根据名称查找所有的子节点，返回第一个满足要求的节点
	 * @param name
	 * @return
	 */
	virtual SceneNode* Search(const string& name);
	/**
	 * 根据id查找所有的子节点，返回第一个满足要求的节点
	 * @param id
	 * @return
	 */
	virtual SceneNode* Search(const long id);

	/**
	 * 遍历所有的子节点
	 * @param action
	 */
	virtual void Traverse(Action* action);

	virtual void FindVisiableObject(RenderAction* renderAction);

	virtual void AddChild(SceneNode *child);

	/**
	 * 在具有指定名称的节点前面插入新节点
	 * @param child
	 * @param nodeName
	 */
	void AddChildInSameLevelBefore(SceneNode *child, const string& nodeName);
	/**
	 * 在具有指定名称的节点后面插入新节点
	 * @param child
	 * @param nodeName
	 */
	virtual void AddChildAfter(SceneNode *child, const string& nodeName);
	virtual void SetChild(int i, SceneNode *child);

	/**
	 *当Scenenode节点发生改变时，继承节点执行相应操作
	 */
	virtual void OnMarkedDirty();

	int Size();
	int GetChildIndex(const string& name);
	SceneNode* GetChild(int i);
	SceneNode* GetChild(const string& name);

	/**
	 * 删除第i位置的节点
	 * @param i
	 */
	void DeleteChild(int i);

	/**
	 * 删除对象
	 * @param node
	 */
	void DeleteChild(const SceneNode* node);

	/**
	 * 根据名称删除
	 * @param name
	 */
	void DeleteChild(const string& name);

	void DeleteAllChildren();

	void DeleteChildInAllSub(int id);

	virtual void ComputeBox();

	/**
	 * 解除子节点关系
	 * @param node
	 */
	virtual void DetachChild(SceneNode* node);

	virtual bool UpdateName();

protected:
	SArrayList* m_pChildrenList; //!< 所有孩子数组
};
}
#endif
