/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef     M3D_L_LSCENE_NODE_H_
#define     M3D_L_LSCENE_NODE_H_

#include "m3d/M3D.h"
#include "m3d/base/Object.h"
#include "m3d/M3DMacros.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Quaternion.h"
#include "m3d/base/Matrix4.h"
#include "m3d/utils/Trackball.h"
#include "m3d/renderer/GLESHead.h"

#include "m3d/base/BoundingBox.h"
#include "m3d/base/Color.h"
#include "m3d/base/Mutex.h"
#include "m3d/M3DMacros.h"

#include "m3d/action/Action.h"
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"

#include "m3d/renderer/RenderContext.h"

#include "m3d/base/Color.h"

namespace M3D
{
class Action;
class CallBackAction;
class PickAction;
class RayPickAction;
class SerachAction;
class RenderAction;
class SceneNode;
class Quaternion;

class LFace;

class LSceneNode: public Object, public IShape
{
public:
	LSceneNode(void);
	virtual ~LSceneNode(void);

	void SetID(IDTYPE id);

	IDTYPE GetID();

	//优化减少内存占用
	void Optimize();

	virtual SHAPETYPE GetType(void);

	/**
	 * @see SceneNode
	 * @param action
	 */
	virtual void Traverse(Action* action);

	virtual void RayPick(RayPickAction * action);

	void SetName(const string& name);

	string& GetName();
	/**
	 * @see SceneNode
	 * @param action
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

	virtual bool Visible();
	/**
	 * @see SceneNode
	 */
	void SetVisible(bool m_visible);

	virtual void SetSelected(bool selected);

	virtual bool IsSelected();

	void AddChild(LSceneNode* childNode);

	virtual LFace * GetShape(void);

	/**
	 * 得到世界坐标系的包围盒，供快速剔除，射线查询，八叉树剔除等使用
	 * @return
	 */
	virtual BoundingBox& GetWorldBoundingBox();

	virtual BoundingBox GetBoundingBox();
	/**
	 * @see SceneNode
	 */
	void SetLocalTransform(const Matrix3x4& localMatrix);

	Matrix3x4 GetLocalTransform() const;

	/**
	 * Return parent space transform matrix.
	 */
	Matrix3x4 GetTransform() const;
	/**
	 * @see SceneNode
	 */
	void SetWorldTransform(const Matrix3x4& worldMatrix);
	/**
	 * @see SceneNode
	 */
	Matrix3x4& GetWorldTransform() const;
	/**
	 * @see SceneNode
	 */
	void SetTransform(const Vector3& position, const Quaternion& rotation);
	/**
	 * @see SceneNode
	 */
	void SetTransform(const Vector3& position, const Quaternion& rotation,
			float scale);
	/**
	 * @see SceneNode
	 */
	void SetTransform(const Vector3& position, const Quaternion& rotation,
			const Vector3& scale);

	/**
	 *  Mark node and child nodes to need world transform recalculation. Notify listener components.
	 */
	void MarkDirty();

	/**
	 * 在子类中重写此方法，能够实现节点脏，通知需要改变的数据
	 */
	virtual void OnMarkedDirty();

	LSceneNode * GetParent(void);

	void SetParent(LSceneNode *node);

	/**
	 * Return whether transform has changed and world transform needs recalculation.
	 * @return
	 */
	bool IsDirty() const
	{
		return m_dirty;
	}

	/**
	 * 得到所有的孩子节点
	 * @return
	 */
	vector<LSceneNode*>& GetChildren();

	/**
	 * @see SceneNode
	 */
	virtual void UpdateWorldTransform() const;

protected:
	Quaternion m_rotation; //!< @see SceneNode
	Vector3 m_position; //!< @see SceneNode
	Vector3 m_scale; //!< @see SceneNode

	mutable bool m_dirty; //!< @see SceneNode
	/// World-space transform matrix.
	mutable Matrix3x4 m_worldMatrix; //!< @see SceneNode
	IDTYPE m_ID; //!< @see SceneNode
	string m_strNodeName; //!< @see SceneNode
	LSceneNode *m_pParent; //!< @see SceneNode
	bool m_visible; //!< @see SceneNode
	bool m_selected; //!< @see SceneNode 存储是否被选中
	vector<LSceneNode*> m_children; //!< 存储下级对象
};

}
#endif // L_SCENE_NODE_H
