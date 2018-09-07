/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */

#ifndef M3D_SHAPE_NODE_H_
#define M3D_SHAPE_NODE_H_

#include "m3d/model/IShape.h"
#include "m3d/scene/SceneNode.h"

namespace M3D
{
class M3D_API ShapeNode: public SceneNode
{
public:
	ShapeNode();
	ShapeNode(IShape* shape);
	virtual ~ShapeNode();
	/**
	 * @see SceneNode
	 */
	virtual int GetType(void);

	virtual void OnMarkedDirty();

	/**
	 * @see SceneNode
	 */
	virtual int GetPickType();
	/**
	 * @see SceneNode
	 */
	virtual void DrawForMouseSelect(void);
	/**
	 * 设置关联的shape对象
	 */
	virtual void SetShape(IShape *object);
	/**
	 * 获取关联的shape
	 * 对象
	 */
	IShape * GetShape(void);

	/**
	 * @see SceneNode
	 */
	virtual void ComputeBox();

	/**
	 * @see SceneNode
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

	virtual void Traverse(Action* action);

	/**
	 * @see SceneNode
	 */
	virtual bool UpdateName();
protected:
	/**
	 * @see SceneNode
	 */
	virtual void RayPick(RayPickAction * action);

protected:
	IShape *m_shape; //!< 关联的shape对象
};
}
#endif // SHAPE_NODE_H
