/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef     M3D_L_SHAPE_NODE_H_
#define     M3D_L_SHAPE_NODE_H_

#include "m3d/M3D.h"
#include "m3d/scene/LSceneNode.h"
namespace M3D
{
class LShapeNode: public LSceneNode
{
public:
	LShapeNode(void);
	virtual ~LShapeNode(void);
	virtual SHAPETYPE GetType(void);
	/**
	 * @see ShapeNode
	 */
	virtual void Traverse(Action* action);
	/**
	 * @see ShapeNode
	 */
	virtual void RayPick(RayPickAction * action);
	/**
	 * @see ShapeNode
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

	virtual void FindVisiableObjectFast(RenderAction* renderAction, int littleModelState);
	/**
	 * @see ShapeNode
	 */
	void AddShape(LFace *face);
	/**
	 * @see ShapeNode
	 */
	virtual vector<LFace*>& GetShapes(void);

	/**
	 * 得到世界坐标系的包围盒，供快速剔除，射线查询，八叉树剔除等使用
	 * @return
	 */
	virtual BoundingBox& GetWorldBoundingBox();
	/**
	 * @see ShapeNode
	 */
	virtual BoundingBox GetBoundingBox();
	/**
	 * @see ShapeNode
	 */
	Matrix4& GetGLWorldTransform()const;
	/**
	 * @see ShapeNode
	 */
	void UpdateWorldTransform() const;

	virtual void SetSelected(bool selected);

	virtual bool IsSelected();
protected:
	mutable Matrix4 m_glRenderMatrix;//!< @see ShapeNode
	//相对于世界坐标的包围盒
	mutable BoundingBox m_worldBox;//!< @see ShapeNode
	vector<LFace*> m_shapes;  //!< @see ShapeNode 支持挂载多个面对象
};

}
#endif // L_SCENE_NODE_H
