/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef     M3D_SCENE_NODE_H_
#define     M3D_SCENE_NODE_H_

#include "m3d/M3D.h"
#include "m3d/base/Object.h"
#include "m3d/M3DMacros.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Quaternion.h"
#include "m3d/base/Matrix4.h"
#include "m3d/utils/Trackball.h"
#include "m3d/M3DMacros.h"
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
const int NODE_NAME_LENGTH = 32;

class Action;
class CallBackAction;
class PickAction;
class RayPickAction;
class SerachAction;
class RenderAction;
class SceneNode;
class Quaternion;

/** 
 * @brief Component and child node creation mode for networking. 
 */
enum CreateMode
{
	REPLICATED = 0, LOCAL = 1
};

/** 
 * @brief Transform space for translations and rotations. 
 */
enum TransformSpace
{
	TS_LOCAL = 0, TS_PARENT, TS_WORLD
};

class M3D_API SceneNode: public Object
{
public:
	SceneNode(void);
	virtual ~SceneNode(void);
	/**
	 * @brief 复位节点状态到最初加载状态
	 */
	virtual void Reset();
	/**
	 * 射线拾取，其他子Node从此继承重写此函数
	 * @param action
	 */
	virtual void RayPick(RayPickAction * action);

	void SetParent(SceneNode *node);
	SceneNode * GetParent(void);

	/**
	 * 通过名称查找节点
	 * @param strName
	 * @return
	 */
	virtual SceneNode * Search(const string& strName);

	/**
	 * 通过ID查找节点
	 * @param id
	 * @return
	 */
	virtual SceneNode * Search(const long id);

	void SetID(long id);

	long GetID();

	/**
	 * @brief 获取此node的类型
	 * @return 返回此Node类型
	 */
	virtual int GetType(void);

	/**
	 * 判断该节点是否能够拾取
	 * @return
	 */
	virtual int GetPickType();

	/**
	 * 遍历Action
	 * @param action
	 */
	virtual void Traverse(Action* action);

	void SetName(const string& name);

	string& GetName();
	/** 
	 * @brief 设置最原始的位置，供复位等逻辑使用
	 */
	Vector3& GetOldPosition();
	/** 
	 * @brief 在渲染过程中查找所有可见对象
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	 * 获取原始的旋转位置，共复位等逻辑使用
	 * @return
	 */
	Quaternion& GetOldRotation();

	/**
	 * 重置外部作用的平移旋转和缩放
	 */
	void ResetMovement();
	/** 
	 * @brief 设置配置矩阵
	 */
	void SetPlcMatrix(const Matrix3x4& m);
	/** 
	 * @brief 设置原始的配置矩阵
	 */
	void SetOrigPlcMatrix(const Matrix3x4& m);
	/** 
	 * @brief 获取原始的配置矩阵
	 */
	Matrix3x4& GetOrigPlcMartirx();
	/** 
	 * @brief 获取剔除状态
	 */
	int GetCullerState();
	/** 
	 * @brief 设置剔除状态
	 */
	void SetCullerState(int state);

	/** 
	 * @brief 是否隐藏
	 */
	virtual bool IsHide();
	/** 
	 * @brief 设置隐藏状态
	 */
	void SetHide(bool isHide);
	/** 
	 * @brief 计算包围盒
	 */
	virtual void ComputeBox();
	/** 
	 * @brief 获取包围盒
	 */
	virtual BoundingBox& GetBoundingBox();

	/**
	 * 得到世界坐标系的包围盒，供快速剔除，射线查询，八叉树剔除等使用
	 * @return
	 */
	BoundingBox& GetWorldBoundingBox();
	/** 
	 * @brief 设置包围盒
	 */
	void SetBoundingBox(const BoundingBox& boudingBox);
	/** 
	 * @brief 获取配置矩阵
	 */
	Matrix3x4& GetPlcMatrix() const;
	/** 
	 * @brief 设置局部坐标系矩阵
	 */
	void SetLocalTransform(const Matrix3x4& localMatrix);
	/** 
	 * @brief 得到局部坐标系偏移量
	 */
	Matrix3x4 GetLocalTransform() const;
	/** 
	 * @brief 设置世界坐标系矩阵
	 */
	void SetWorldTransform(const Matrix3x4& worldMatrix);
	/**
	 * @brief 得到世界坐标系矩阵
	 */
	Matrix3x4& GetWorldTransform() const;
	/** 
	 * @brief 得到世界坐标系opengl格式矩阵
	 */
	Matrix4& GetGLWorldTransform() const;

	/** 
	 * @brief Set position in parent space. If the scene node is on the root level (is child of the scene itself), this is same as world space.
	 */
	void SetPosition(const Vector3& position);
	/** 
	 * @brief Set position in parent space (for Urho2D).
	 */
	void SetPosition2D(const Vector2& position)
	{
		SetPosition(Vector3(position));
	}
	/** 
	 * @brief Set position in parent space (for Urho2D).
	 */
	void SetPosition2D(float x, float y)
	{
		SetPosition(Vector3(x, y, 0.0f));
	}
	/** 
	 * @brief Set rotation in parent space.
	 */
	void SetRotation(const Quaternion& rotation);
	/** 
	 * @brief Set rotation in parent space (for Urho2D).
	 */
	void SetRotation2D(float rotation)
	{
		SetRotation(Quaternion(rotation));
	}
	/** 
	 * @brief Set forward direction in parent space. Positive Z axis equals identity rotation.
	 */
	void SetDirection(const Vector3& direction);
	/** 
	 * @brief Set uniform scale in parent space.
	 */
	void SetScale(float scale);
	/** 
	 * @brief Set scale in parent space.
	 */
	void SetScale(const Vector3& scale);
	/** 
	 * @brief Set scale in parent space (for Urho2D).
	 */
	void SetScale2D(const Vector2& scale)
	{
		SetScale(Vector3(scale, 1.0f));
	}
	/** 
	 * @brief Set scale in parent space (for Urho2D).
	 */
	void SetScale2D(float x, float y)
	{
		SetScale(Vector3(x, y, 1.0f));
	}
	/** 
	 * @brief Set both position and rotation in parent space as an atomic operation. This is faster than setting position and rotation separately.
	 */
	void SetTransform(const Vector3& position, const Quaternion& rotation);
	/** 
	 * @brief Set both position, rotation and uniform scale in parent space as an atomic operation.
	 */
	void SetTransform(const Vector3& position, const Quaternion& rotation,
			float scale);
	/** 
	 * @brief Set both position, rotation and scale in parent space as an atomic operation.
	 */
	void SetTransform(const Vector3& position, const Quaternion& rotation,
			const Vector3& scale);
	/** 
	 * @brief Set both position and rotation in parent space as an atomic operation (for Urho2D).
	 */
	void SetTransform2D(const Vector2& position, float rotation)
	{
		SetTransform(Vector3(position), Quaternion(rotation));
	}
	/** 
	 * @brief Set both position, rotation and uniform scale in parent space as an atomic operation (for Urho2D).
	 */
	void SetTransform2D(const Vector2& position, float rotation, float scale)
	{
		SetTransform(Vector3(position), Quaternion(rotation), scale);
	}
	/** 
	 * @brief Set both position, rotation and scale in parent space as an atomic operation (for Urho2D).
	 */
	void SetTransform2D(const Vector2& position, float rotation,
			const Vector2& scale)
	{
		SetTransform(Vector3(position), Quaternion(rotation),
				Vector3(scale, 1.0f));
	}
	/** 
	 * @brief Set position in world space.
	 */
	void SetWorldPosition(const Vector3& position);
	/** 
	 * @brief Set position in world space (for Urho2D).
	 */
	void SetWorldPosition2D(const Vector2& position)
	{
		SetWorldPosition(Vector3(position));
	}
	/** 
	 * @brief Set position in world space (for Urho2D).
	 */
	void SetWorldPosition2D(float x, float y)
	{
		SetWorldPosition(Vector3(x, y, 0.0f));
	}
	/** 
	 * @brief Set rotation in world space.
	 */
	void SetWorldRotation(const Quaternion& rotation);
	/** 
	 * @brief Set rotation in world space (for Urho2D).
	 */
	void SetWorldRotation2D(float rotation)
	{
		SetWorldRotation(Quaternion(rotation));
	}
	/** 
	 * @brief Set forward direction in world space.
	 */
	void SetWorldDirection(const Vector3& direction);
	/** 
	 * @brief Set uniform scale in world space.
	 */
	void SetWorldScale(float scale);
	/** 
	 * @brief Set scale in world space.
	 */
	void SetWorldScale(const Vector3& scale);
	/** 
	 * @brief Set scale in world space (for Urho2D).
	 */
	void SetWorldScale2D(const Vector2& scale)
	{
		SetWorldScale(Vector3(scale, 1.0f));
	}
	/** 
	 * @brief Set scale in world space (for Urho2D).
	 */
	void SetWorldScale2D(float x, float y)
	{
		SetWorldScale(Vector3(x, y, 1.0f));
	}
	/** 
	 * @brief Set both position and rotation in world space as an atomic operation.
	 */
	void SetWorldTransform(const Vector3& position, const Quaternion& rotation);
	/** 
	 * @brief Set both position, rotation and uniform scale in world space as an atomic operation.
	 */
	void SetWorldTransform(const Vector3& position, const Quaternion& rotation,
			float scale);
	/** 
	 * @brief Set both position, rotation and scale in world space as an atomic opration.
	 */
	void SetWorldTransform(const Vector3& position, const Quaternion& rotation,
			const Vector3& scale);
	/** 
	 * @brief Set both position and rotation in world space as an atomic operation (for Urho2D).
	 */
	void SetWorldTransform2D(const Vector2& position, float rotation)
	{
		SetWorldTransform(Vector3(position), Quaternion(rotation));
	}
	/** 
	 * @brief Set both position, rotation and uniform scale in world space as an atomic operation (for Urho2D).
	 */
	void SetWorldTransform2D(const Vector2& position, float rotation,
			float scale)
	{
		SetWorldTransform(Vector3(position), Quaternion(rotation), scale);
	}
	/** 
	 * @brief Set both position, rotation and scale in world space as an atomic opration (for Urho2D).
	 */
	void SetWorldTransform2D(const Vector2& position, float rotation,
			const Vector2& scale)
	{
		SetWorldTransform(Vector3(position), Quaternion(rotation),
				Vector3(scale, 1.0f));
	}
	/** 
	 * @brief Move the scene node in the chosen transform space.
	 */
	void Translate(const Vector3& delta, TransformSpace space = TS_LOCAL);
	/** 
	 * @brief Move the scene node in the chosen transform space (for Urho2D).
	 */
	void Translate2D(const Vector2& delta, TransformSpace space = TS_LOCAL)
	{
		Translate(Vector3(delta), space);
	}
	/** 
	 * @brief Rotate the scene node in the chosen transform space.
	 */
	void Rotate(const Quaternion& delta, TransformSpace space = TS_LOCAL);
	/** 
	 * @brief Rotate the scene node in the chosen transform space (for Urho2D).
	 */
	void Rotate2D(float delta, TransformSpace space = TS_LOCAL)
	{
		Rotate(Quaternion(delta), space);
	}
	/** 
	 * @brief Rotate around a point in the chosen transform space.
	 */
	void RotateAround(const Vector3& point, const Quaternion& delta,
			TransformSpace space = TS_LOCAL);
	/** 
	 * @brief Rotate around a point in the chosen transform space (for Urho2D).
	 */
	void RotateAround2D(const Vector2& point, float delta,
			TransformSpace space = TS_LOCAL)
	{
		RotateAround(Vector3(point), Quaternion(delta), space);
	}
	/** 
	 * @brief Rotate around the X axis.
	 */
	void Pitch(float angle, TransformSpace space = TS_LOCAL);
	/** 
	 * @brief Rotate around the Y axis.
	 */
	void Yaw(float angle, TransformSpace space = TS_LOCAL);
	/** 
	 * @brief Rotate around the Z axis.
	 */
	void Roll(float angle, TransformSpace space = TS_LOCAL);
	/** 
	 * @brief Look at a target position in the chosen transform space. Note that the up vector is always specified in world space. Return true if successful, or false if resulted in an illegal rotation, in which case the current rotation remains.
	 */
	bool LookAt(const Vector3& target, const Vector3& up = Vector3::UP,
			TransformSpace space = TS_WORLD);
	/** 
	 * @brief Modify scale in parent space uniformly.
	 */
	void Scale(float scale);
	/** 
	 * @brief Modify scale in parent space.
	 */
	void Scale(const Vector3& scale);
	/** 
	 * @brief Modify scale in parent space (for Urho2D).
	 */
	void Scale2D(const Vector2& scale)
	{
		Scale(Vector3(scale, 1.0f));
	}

	/** 
	 * @brief Set enabled/disabled state without recursion. Components in a disabled node become effectively disabled regardless of their own enable/disable state.
	 */
	void SetEnabled(bool enable);
	/** 
	 * @brief Mark node and child nodes to need world transform recalculation. Notify listener components.
	 */
	void MarkDirty();

	//在子类中重写此方法，能够实现节点脏，通知需要改变的数据
	virtual void OnMarkedDirty();
	/** 
	 * @brief Return position in parent space.
	 */
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/** 
	 * @brief Return position in parent space (for Urho2D).
	 */
	Vector2 GetPosition2D() const
	{
		return Vector2(m_position.m_x, m_position.m_y);
	}
	/** 
	 * @brief Return rotation in parent space.
	 */
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}
	/** 
	 * @brief Return rotation in parent space (for Urho2D).
	 */
	float GetRotation2D() const
	{
		return m_rotation.RollAngle();
	}
	Vector3 GetEulerAngles() const
		{
			return m_rotation.EulerAngles();
		}
	/** 
	 * @brief Return forward direction in parent space. Positive Z axis equals identity rotation.
	 */
	Vector3 GetDirection() const
	{
		return m_rotation * Vector3::FORWARD;
	}
	/** 
	 * @brief Return up direction in parent space. Positive Y axis equals identity rotation.
	 */
	Vector3 GetUp() const
	{
		return m_rotation * Vector3::UP;
	}
	/** 
	 * @brief Return right direction in parent space. Positive X axis equals identity rotation.
	 */
	Vector3 GetRight() const
	{
		return m_rotation * Vector3::RIGHT;
	}

	/** 
	 * @brief Return scale in parent space.
	 */
	const Vector3& GetScale() const
	{
		return m_scale;
	}
	/** 
	 * @brief Return scale in parent space (for Urho2D).
	 */
	Vector2 GetScale2D() const
	{
		return Vector2(m_scale.m_x, m_scale.m_y);
	}

	/** 
	 * @brief Return parent space transform matrix.
	 */
	Matrix3x4 GetTransform() const
	{
		return Matrix3x4(m_position, m_rotation, m_scale);
	}

	/** 
	 * @brief Return position in world space.
	 */
	Vector3 GetWorldPosition() const
	{
		if (m_dirty)
			UpdateWorldTransform();

		return m_worldMatrix.Translation();
	}

	/** 
	 * @brief Return position in world space (for Urho2D).
	 */
	Vector2 GetWorldPosition2D() const
	{
		Vector3 worldPosition = GetWorldPosition();
		return Vector2(worldPosition.m_x, worldPosition.m_y);
	}

	/** 
	 * @brief Return rotation in world space.
	 */
	Quaternion GetWorldRotation() const
	{
		if (m_dirty)
			UpdateWorldTransform();

		return m_worldMatrix.Rotation();
	}

	/** 
	 * @brief Return rotation in world space (for Urho2D).
	 */
	float GetWorldRotation2D() const
	{
		return GetWorldRotation().RollAngle();
	}

	/** 
	 * @brief Return direction in world space.
	 */
	Vector3 GetWorldDirection() const
	{
		if (m_dirty)
			UpdateWorldTransform();
		return m_worldMatrix.Rotation() * Vector3::FORWARD;
	}

	/** 
	 * @brief Return node's up vector in world space.
	 */
	Vector3 GetWorldUp() const
	{
		if (m_dirty)
			UpdateWorldTransform();

		return m_worldMatrix.Rotation() * Vector3::UP;
	}

	/** 
	 * @brief Return node's right vector in world space.
	 */
	Vector3 GetWorldRight() const
	{
		if (m_dirty)
			UpdateWorldTransform();

		return m_worldMatrix.Rotation() * Vector3::RIGHT;
	}

	/** 
	 * @brief Return scale in world space.
	 */
	Vector3 GetWorldScale() const
	{
		if (m_dirty)
			UpdateWorldTransform();

		return m_worldMatrix.Scale();
	}

	/** 
	 * @brief Return scale in world space (for Urho2D).
	 */
	Vector2 GetWorldScale2D() const
	{
		Vector3 worldScale = GetWorldScale();
		return Vector2(worldScale.m_x, worldScale.m_y);
	}

	/** 
	 * @brief Convert a local space position to world space.
	 */
	Vector3 LocalToWorld(const Vector3& position) const;

	/** 
	 * @brief Convert a local space position or rotation to world space (for Urho2D).
	 */
	Vector2 LocalToWorld2D(const Vector2& vector) const;
	/** 
	 * @brief Convert a world space position to local space.
	 */
	Vector3 WorldToLocal(const Vector3& position) const;

	/** 
	 * @brief Convert a world space position or rotation to local space (for Urho2D).
	 */
	Vector2 WorldToLocal2D(const Vector2& vector) const;
	/** 
	 * @brief Return whether transform has changed and world transform needs recalculation.
	 */
	bool IsDirty() const
	{
		return m_dirty;
	}

	virtual bool UpdateName();

private:
	/** 
	 * @brief Recalculate the world transform.
	 */
	void UpdateWorldTransform() const;

protected:
	mutable bool m_dirty;//!<脏标示
	mutable Matrix3x4 m_worldMatrix;//!<World-space transform matrix.
	mutable Matrix4 m_glRenderMatrix;//!<opengl格式的世界坐标系矩阵的缓存

	mutable Matrix3x4 m_plcMatrix;//!< 配置矩阵
	mutable Matrix3x4 m_oldPlcMatrix;//!<配置矩阵

	Vector3 m_position;//!<位置
	Vector3 m_oldPosition;

	Quaternion m_rotation;//!<选择
	Quaternion m_oldRotation;

	Vector3 m_scale;//!<缩放
	Vector3 m_oldScale;

	long int m_ID;
	string m_strNodeName;//!<名称

	SceneNode *m_pParent;
	mutable BoundingBox m_bdBox;//!<自身的包围盒
	mutable BoundingBox m_worldBox;//!<相对于世界坐标的包围盒
	bool m_bHideState;//!<隐藏状态

	//0 = 相交
	//1 = 全在外部
	//2 = 全在内部
	int m_cullerState;//!<视景体 剔除的状态
	mutable Mutex m_mutex;
};

}
#endif // SCENE_NODE_H
