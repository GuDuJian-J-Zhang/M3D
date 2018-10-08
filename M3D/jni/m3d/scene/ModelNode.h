/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef     M3D_MODEL_NODE_H_
#define     M3D_MODEL_NODE_H_

#include "m3d/scene/ShapeNode.h"
#include "m3d/scene/GroupNode.h"

namespace M3D
{

class M3D_API ModelNode: public GroupNode
{
public:
	ModelNode(void);
	virtual ~ModelNode(void);

	void Init();

	virtual int GetType(void);
	/**
	 * @see GroupNode
	 */
	virtual SceneNode* Search(const string& name);
	/**
	 * @see GroupNode
	 */
	virtual SceneNode* Search(const long id);

	/**
	 * @see GroupNode
	 */
	virtual void Traverse(Action* action);

	/**
	 * @see GroupNode
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	 * @see GroupNode
	 */
	virtual void OnMarkedDirty();
	/**
	 * 得到模型对应的显示数据节点
	 * @return shapeNode
	 */
	ShapeNode* GetShapeNode();

	/**
	 * 设置模型显示数据节点
	 * @param shapeNode
	 */
	void SetShapeNode(ShapeNode* shapeNode);

	/**
	 * @see GroupNode
	 */
	virtual void ComputeBox();

	/**
	 * @see GroupNode
	 */
	virtual bool UpdateName();
private:
	int GetParentCullerState();

private:
	ShapeNode* shapeNode; //!< 关联的shapeNode对象
};

}
#endif // MODEL_NODE_H
