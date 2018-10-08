#include "m3d/M3D.h"
#include "m3d/utils/IDCreator.h"

#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix4.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/base/Quaternion.h"
#include "m3d/utils/Trackball.h"

#include "m3d/scene/SceneNode.h"
#include "m3d/graphics/CameraNode.h"

#include "m3d/renderer/GLESHead.h"
#include "m3d/renderer/RenderContext.h"

namespace M3D {

SceneNode::SceneNode(void) :
		Object() {
	m_pParent = NULL;
	m_strNodeName = "NONAME";
	m_position.ToZero();
	m_rotation.ToZero();
	m_scale.ToOne();

	m_oldPosition = m_position;
	m_oldRotation = m_rotation;
	m_oldScale = m_scale;

	m_bHideState = false;

	m_cullerState = CameraNode::FRUSTUM_INTER;

	this->m_ID = IDCreator::GetDefaultID();
	this->MarkDirty();
}

SceneNode::~SceneNode(void) {

}

void SceneNode::Reset() {
	m_position = m_oldPosition;
	m_scale = m_oldScale;
	m_rotation = m_oldRotation;

	this->m_plcMatrix = this->m_oldPlcMatrix;
	this->m_worldMatrix = Matrix4::IDENTITY;

	this->MarkDirty();
}

SceneNode *
SceneNode::Search(const string & name) {
	if (name == m_strNodeName)
		return this;
	else
		return NULL;
}

SceneNode * SceneNode::Search(const long id) {
	if (this->m_ID == id) {
		return this;
	}
	return NULL;
}

void SceneNode::SetID(long id) {
	this->m_ID = id;
}

long SceneNode::GetID() {
	return this->m_ID;
}

int SceneNode::GetType(void) {
	return SCENE_NODE;
}

void SceneNode::SetName(const string& name) {
	m_strNodeName = name;
}

string& SceneNode::GetName() {
	return m_strNodeName;
}

Vector3& SceneNode::GetOldPosition() {
	return m_oldPosition;
}

Quaternion& SceneNode::GetOldRotation() {
	return this->m_oldRotation;
}

void SceneNode::SetScale(float scale) {
	if (scale > 0.001) {
		m_scale = m_scale / scale;
		m_oldScale = m_scale;
	}
}

void SceneNode::ResetMovement() {
	m_position.ToZero();
	m_rotation.ToZero();
	m_plcMatrix = m_oldPlcMatrix;
}

// 
// void SceneNode::RotateAround(const Quaternion& rot, const Vector3& ref)
// {
// 	//SetRotateCenter(ref);
// 	Rotate(rot);
// }

void SceneNode::SetPlcMatrix(const Matrix3x4& m) {
	MutexLock locked(this->m_mutex);
	m_plcMatrix = m;
	this->MarkDirty();
}

void SceneNode::SetOrigPlcMatrix(const Matrix3x4& m) {
	m_plcMatrix = m;

	m_oldPlcMatrix = m_plcMatrix;
}

Matrix3x4& SceneNode::GetOrigPlcMartirx() {
	return this->m_oldPlcMatrix;
}

bool SceneNode::IsHide() {
	return this->m_bHideState;
}

void SceneNode::SetHide(bool isHide) {
	this->m_bHideState = isHide;
}

BoundingBox& SceneNode::GetBoundingBox() {
	ComputeBox();
	return m_bdBox;
}

BoundingBox& SceneNode::GetWorldBoundingBox() {
	if (!m_worldBox.Defined()) {
		BoundingBox bdBox = GetBoundingBox();
		if (bdBox.Defined()) {
			bdBox.Transform(this->GetWorldTransform());
			m_worldBox = bdBox;
		}
	}

	return m_worldBox;
}

void SceneNode::Traverse(Action* action) {
	if (action->IsFinish()) {
		return;
	}
	action->Apply(this);
}

void SceneNode::SetBoundingBox(const BoundingBox& boudingBox) {
	this->m_bdBox = boudingBox;
}

void SceneNode::RayPick(RayPickAction * action) {

}

void SceneNode::ComputeBox() {

}

//int SceneNode::GetRelevantID()
//{
//	return this->m_iRelevantID;
//}
//
//void SceneNode::SetRelevantID(int id)
//{
//	this->m_iRelevantID = id;
//}

void SceneNode::SetCullerState(int state) {
	this->m_cullerState = state;
}

int SceneNode::GetCullerState() {
	return this->m_cullerState;
}

int SceneNode::GetPickType() {
	return PICKTYPE_BASE;
}

SceneNode * SceneNode::GetParent(void) {
	return m_pParent;
}

void SceneNode::SetParent(SceneNode *node) {
	m_pParent = node;
	//node->m_pSubChildren = this;
}

void SceneNode::SetLocalTransform(const Matrix3x4& localMatrix) {
	MutexLock locked(this->m_mutex);
	Matrix3x4 delt = GetPlcMatrix().Inverse() * localMatrix;
	SetTransform(delt.Translation(), delt.Rotation(), delt.Scale());
	this->MarkDirty();
}

Matrix3x4 SceneNode::GetLocalTransform() const {
	return GetPlcMatrix() * GetTransform();
}

void SceneNode::SetWorldTransform(const Matrix3x4& worldMatrix) {
	Matrix3x4 localTransform;
	if (m_pParent == NULL) {
		localTransform = worldMatrix;
	} else {
		localTransform = m_pParent->GetWorldTransform().Inverse() * worldMatrix;
	}

	MutexLock locked(this->m_mutex);
	this->SetLocalTransform(localTransform);
}

Matrix3x4& SceneNode::GetWorldTransform() const {
	if (this->IsDirty()) {
		this->UpdateWorldTransform();
	}

	return this->m_worldMatrix;
}

Matrix4& SceneNode::GetGLWorldTransform() const {
	this->GetWorldTransform();
	return this->m_glRenderMatrix;
}

void SceneNode::UpdateWorldTransform() const {
	MutexLock locked(this->m_mutex);

	Matrix3x4 transform = GetLocalTransform();

//  LOGE("transform: %s",GetTransform().Tostring().c_str());

	if (m_pParent == NULL) {
		m_worldMatrix = transform;
	} else {
		m_worldMatrix = m_pParent->GetWorldTransform() * transform;
	}

	this->m_glRenderMatrix = this->m_worldMatrix.ToMatrix4().Transpose();

	m_dirty = false;
}

void SceneNode::FindVisiableObject(RenderAction* renderAction) {

}

Matrix3x4& SceneNode::GetPlcMatrix() const {
	return this->m_plcMatrix;
}
void SceneNode::SetPosition(const Vector3& position) {
	MutexLock lock(m_mutex);
	m_position = position;
	MarkDirty();
}

void SceneNode::SetRotation(const Quaternion& rotation) {
	MutexLock lock(m_mutex);
	m_rotation = rotation;
	MarkDirty();
}

void SceneNode::SetDirection(const Vector3& direction) {
	SetRotation(Quaternion(Vector3::FORWARD, direction));
}

void SceneNode::SetScale(const Vector3& scale) {
	MutexLock lock(m_mutex);
	m_scale = scale.Abs();
	MarkDirty();
}

void SceneNode::SetTransform(const Vector3& position,
		const Quaternion& rotation) {
	m_position = position;
	m_rotation = rotation;
	MarkDirty();
}

void SceneNode::SetTransform(const Vector3& position,
		const Quaternion& rotation, float scale) {
	SetTransform(position, rotation, Vector3(scale, scale, scale));
}

void SceneNode::SetTransform(const Vector3& position,
		const Quaternion& rotation, const Vector3& scale) {
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
	MarkDirty();
}

void SceneNode::SetWorldPosition(const Vector3& position) {
	SetPosition(
			(m_pParent == NULL) ?
					position :
					m_pParent->GetWorldTransform().Inverse() * position);
}

void SceneNode::SetWorldRotation(const Quaternion& rotation) {
	SetRotation(
			(m_pParent == NULL) ?
					rotation :
					m_pParent->GetWorldRotation().Inverse() * rotation);
}

void SceneNode::SetWorldDirection(const Vector3& direction) {
	Vector3 localDirection =
			(m_pParent == NULL) ?
					direction :
					m_pParent->GetWorldRotation().Inverse() * direction;
	SetRotation(Quaternion(Vector3::FORWARD, localDirection));
}

void SceneNode::SetWorldScale(float scale) {
	SetWorldScale(Vector3(scale, scale, scale));
}

void SceneNode::SetWorldScale(const Vector3& scale) {
	SetScale((m_pParent == NULL) ? scale : scale / m_pParent->GetWorldScale());
}

void SceneNode::SetWorldTransform(const Vector3& position,
		const Quaternion& rotation) {
	SetWorldPosition(position);
	SetWorldRotation(rotation);
}

void SceneNode::SetWorldTransform(const Vector3& position,
		const Quaternion& rotation, float scale) {
	SetWorldPosition(position);
	SetWorldRotation(rotation);
	SetWorldScale(scale);
}

void SceneNode::SetWorldTransform(const Vector3& position,
		const Quaternion& rotation, const Vector3& scale) {
	SetWorldPosition(position);
	SetWorldRotation(rotation);
	SetWorldScale(scale);
}

void SceneNode::Translate(const Vector3& delta, TransformSpace space) {
	if (delta.IsZero()) {
		return;
	}

	switch (space) {
	case TS_LOCAL:
		// Note: local space translation disregards local scale for scale-independent movement speed
		m_position += m_rotation * delta;
		break;

	case TS_PARENT:
		m_position += delta;
		break;

	case TS_WORLD:
		m_position +=
				(m_pParent == NULL) ?
						delta :
						m_pParent->GetWorldTransform().Inverse() * delta;
		break;
	}
	MarkDirty();
}

void SceneNode::Rotate(const Quaternion& delta, TransformSpace space) {
	switch (space) {
	case TS_LOCAL:
		m_rotation = (m_rotation * delta).Normalized();
		break;

	case TS_PARENT:
		m_rotation = (delta * m_rotation).Normalized();
		break;

	case TS_WORLD:
		if (m_pParent == NULL)
			m_rotation = (delta * m_rotation).Normalized();
		else {
			Quaternion worldRotation = GetWorldRotation();
			m_rotation = m_rotation * worldRotation.Inverse() * delta
					* worldRotation;
		}
		break;
	}

	MarkDirty();

}

void SceneNode::RotateAround(const Vector3& point, const Quaternion& delta,
		TransformSpace space) {

	MutexLock lock(m_mutex);

	if (delta.IsZero()) {
		return;
	}

	Vector3 parentSpacePoint;
	Quaternion oldRotation = m_rotation;

	switch (space) {
	case TS_LOCAL:
		parentSpacePoint = GetTransform() * point;
		m_rotation = (m_rotation * delta).Normalized();
		break;

	case TS_PARENT:
		parentSpacePoint = point;
		m_rotation = (delta * m_rotation).Normalized();
		break;

	case TS_WORLD:
		if (m_pParent == NULL) {
			parentSpacePoint = point;
			m_rotation = (delta * m_rotation).Normalized();
		} else {
			parentSpacePoint = m_pParent->GetWorldTransform().Inverse() * point;
			Quaternion worldRotation = GetWorldRotation();
			m_rotation = m_rotation * worldRotation.Inverse() * delta
					* worldRotation;
		}
		break;
	}

	Vector3 oldRelativePos = oldRotation.Inverse()
			* (m_position - parentSpacePoint);
	m_position = m_rotation * oldRelativePos + parentSpacePoint;

	MarkDirty();

}

void SceneNode::Yaw(float angle, TransformSpace space) {
	Rotate(Quaternion(angle, Vector3::UP), space);
}

void SceneNode::Pitch(float angle, TransformSpace space) {
	Rotate(Quaternion(angle, Vector3::RIGHT), space);
}

void SceneNode::Roll(float angle, TransformSpace space) {
	Rotate(Quaternion(angle, Vector3::FORWARD), space);
}

bool SceneNode::LookAt(const Vector3& target, const Vector3& up,
		TransformSpace space) {
	Vector3 worldSpaceTarget;

	switch (space) {
	case TS_LOCAL:
		worldSpaceTarget = GetWorldTransform() * target;
		break;

	case TS_PARENT:
		worldSpaceTarget =
				(m_pParent == NULL) ?
						target : m_pParent->GetWorldTransform() * target;
		break;

	case TS_WORLD:
		worldSpaceTarget = target;
		break;
	}

	Vector3 lookDir = worldSpaceTarget - GetWorldPosition();
	// Check if target is very close, in that case can not reliably calculate lookat direction
	if (lookDir.Equals(Vector3::ZERO))
		return false;
	Quaternion newRotation;
	// Do nothing if setting look rotation failed
	if (!newRotation.FromLookRotation(lookDir, up))
		return false;

	SetWorldRotation(newRotation);
	return true;
}

void SceneNode::Scale(float scale) {
	Scale(Vector3(scale, scale, scale));
}

void SceneNode::Scale(const Vector3& scale) {
	m_scale *= scale;
	MarkDirty();
}

void SceneNode::MarkDirty() {
	m_dirty = true;
	m_worldBox.Clear();
	this->OnMarkedDirty();
}

void SceneNode::OnMarkedDirty() {

}

Vector3 SceneNode::LocalToWorld(const Vector3& position) const {
	return GetWorldTransform() * position;
}

Vector2 SceneNode::LocalToWorld2D(const Vector2& vector) const {
	Vector3 result = LocalToWorld(Vector3(vector));
	return Vector2(result.m_x, result.m_y);
}

Vector3 SceneNode::WorldToLocal(const Vector3& position) const {
	return GetWorldTransform().Inverse() * position;
}

Vector2 SceneNode::WorldToLocal2D(const Vector2& vector) const {
	Vector3 result = WorldToLocal(Vector3(vector));
	return Vector2(result.m_x, result.m_y);
}

bool SceneNode::UpdateName() {
	return false;
}
}
