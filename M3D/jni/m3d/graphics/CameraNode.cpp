#include "m3d/graphics/CameraNode.h"
#include "m3d/SceneManager.h"
#include "m3d/base/Frustum.h"
#include "Utility.h"
#include "m3d/base/json/json.h"
namespace M3D {
float CameraNode::drawLimit = 0;
const int CameraNode::FRUSTUM_INTER = 0;
const int CameraNode::FRUSTUM_OUT = 1;
const int CameraNode::FRUSTUM_INER = 2;

const Matrix4 CameraNode::flipMatrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

CameraNode::CameraNode(void) :
		SceneNode(), m_viewDirty(true), m_projectionDirty(true), m_frustumDirty(
				true), m_orthographic(false), m_nearClip(DEFAULT_NEARCLIP), m_farClip(
				DEFAULT_FARCLIP), m_reflectionPlane(Plane::UP), m_clipPlane(
				Plane::UP) {
	m_zoom = 1.0f;
	this->m_viewPort.SetCamera(this);
	//默认使用平行投影
	SetOrthographic(true);
	m_flipVertical = false;
	m_useReflection = false;
	m_reflectionMatrix = m_reflectionPlane.ReflectionMatrix();

	this->m_isVR = false;
	m_VREyeMat = Matrix3x4::IDENTITY;

	m_cameraMode = 0;
}

CameraNode::~CameraNode(void) {

}

void CameraNode::SetScale(float scale) {
	if (scale > 0.001) {
		this->m_zoom *= scale;
	}

}

void CameraNode::ReSet() {
	SceneNode::Reset();
	this->m_zoom = 1.0f;

	m_rotateCenter = m_origRotateCenter;
	SetFov(90.0f);
	this->m_worldMatrix = Matrix3x4::IDENTITY;
}

void CameraNode::SetInitRotateCenter(const Vector3& center) {
	this->m_origRotateCenter = center;
	this->m_rotateCenter = center;
}

void CameraNode::SetRotateCenter(const Vector3& center) {
	this->m_rotateCenter = center;
}

Vector3& CameraNode::GetOrigRotateCenter() {
	return this->m_origRotateCenter;
}

Vector3& CameraNode::GetRotateCenter() {
	return this->m_rotateCenter;
}

void CameraNode::GetOrthoSize(float *orthoWidth, float *orthoHeight) {
	*orthoWidth = m_orthoSize * m_aspectRatio;
	*orthoHeight = m_orthoSize;
}

void CameraNode::UpDataCameraPos() {

}

void CameraNode::Traverse(Action* action) {

}

string CameraNode::GetStates() {
	MutexLock lock(m_mutex);
	string states = "";
	Json::Value camera;
	Vector3 pos = this->GetPosition();
	camera["PosX"] = pos.m_x;
	camera["PosY"] = pos.m_y;
	camera["PosZ"] = pos.m_z;

	Quaternion rotation = this->GetRotation();
	camera["RotX"] = rotation.m_x;
	camera["RotY"] = rotation.m_y;
	camera["RotZ"] = rotation.m_z;
	camera["RotW"] = rotation.m_w;

	camera["Zoom"] = this->GetZoom();
	states = camera.toStyledString();

	return states;
}

bool CameraNode::SetStates(const string& states) {
	MutexLock lock(m_mutex);

	bool ret = false;
	Json::Reader reader;
	Json::Value camera;
	if (reader.parse(states.c_str(), camera)) {
		ret = true;
		Vector3 pos;
		pos.m_x = camera["PosX"].asFloat();
		pos.m_y = camera["PosY"].asFloat();
		pos.m_z = camera["PosZ"].asFloat();

		Quaternion rotation;
		rotation.m_x = camera["RotX"].asFloat();
		rotation.m_y = camera["RotY"].asFloat();
		rotation.m_z = camera["RotZ"].asFloat();
		rotation.m_w = camera["RotW"].asFloat();

		float zoom = camera["Zoom"].asFloat();

		this->SetPosition(pos);
		this->SetRotation(rotation);
		this->SetZoom(zoom);
	}

	return ret;
}

bool CameraNode::setPosition(const string& position) {
	MutexLock lock(m_mutex);
	bool ret = false;
	Json::Reader reader;
	Json::Value camera;
	if (reader.parse(position.c_str(), camera)) {
		Vector3 pos;
		pos.m_x = camera["PosX"].asFloat();
		pos.m_y = camera["PosY"].asFloat();
		pos.m_z = camera["PosZ"].asFloat();
		this->SetPosition(pos);
		ret = true;
	}

	return ret;
}
bool CameraNode::setDirection(const string& direction) {
	MutexLock lock(m_mutex);

	bool ret = false;
	Json::Reader reader;
	Json::Value camera;
	if (reader.parse(direction.c_str(), camera)) {
		Vector3 pos;
		pos.m_x = camera["DirX"].asFloat();
		pos.m_y = camera["DirY"].asFloat();
		pos.m_z = camera["DirZ"].asFloat();
		this->SetDirection(pos);
		ret = true;
	}

	return ret;
}

void CameraNode::ZoomView(float scaleFactor) {
	if (scaleFactor > 0.001f) {
		m_zoom *= scaleFactor;
	}
	m_projectionDirty = true;
	m_frustumDirty = true;
	this->m_viewDirty = true;
}

void CameraNode::RayPick(RayPickAction * action) {

}

int CameraNode::GetType(void) {
	return CAMERA_NODE;
}

void CameraNode::RotateAroundCenter(const Quaternion& delta,
		TransformSpace space) {
	if (delta.IsZero()) {
		return;
	}

	Quaternion nDelta = delta;
	switch (space) {
	case TS_LOCAL:
		break;
	case TS_PARENT:
		break;
	case TS_WORLD:
		//由于此处传入旋转值，为trackball相对于摄像机初始位置求得的。
		//如果要实现相同的旋转效果，首先根据摄像机当前的旋转值，将摄像机调整到为旋转位置，然后乘上旋转值，最后在乘上原有旋转值。
		//注意矩阵与点作用的顺序为从左到右进行的。1.m_rotation.Inverse() 2.delta 3.m_rotation
		nDelta = m_rotation * delta * m_rotation.Inverse();
		break;
	}
	this->RotateAround(this->m_rotateCenter, nDelta, space);
}

const Viewport& CameraNode::GetViewPort() const {
	return this->m_viewPort;
}

void CameraNode::SetViewPort(int x, int y, int width, int heigh) {
	MutexLock lock(m_mutex);
	IntRect rect(x, y, x + width, y + heigh);
	this->m_viewPort.SetRect(rect);
	this->MarkDirty();
}

void CameraNode::SetNearClip(float nearClip) {
	m_nearClip = Max(nearClip, M_MIN_NEARCLIP);
	m_frustumDirty = true;
	m_projectionDirty = true;
}

void CameraNode::SetFarClip(float farClip) {
	m_farClip = Max(farClip, M_MIN_NEARCLIP);
	m_frustumDirty = true;
	m_projectionDirty = true;
}

void CameraNode::SetFov(float fov) {
	m_fov = Clamp(fov, 0.0f, M_MAX_FOV);
	m_frustumDirty = true;
	m_projectionDirty = true;
}

void CameraNode::SetOrthoSize(float orthoSize) {
	m_orthoSize = orthoSize;
	m_aspectRatio = 1.0f;
	m_frustumDirty = true;
	m_projectionDirty = true;
}

void CameraNode::SetOrthoSize(const Vector2& orthoSize) {
	m_autoAspectRatio = false;
	m_orthoSize = orthoSize.m_y;
	m_aspectRatio = orthoSize.m_x / orthoSize.m_y;
	m_frustumDirty = true;
	m_projectionDirty = true;
}

void CameraNode::SetAspectRatio(float aspectRatio) {
	m_autoAspectRatio = false;
	SetAspectRatioInternal(aspectRatio);
}

void CameraNode::SetZoom(float zoom) {
	m_zoom = Max(zoom, M_EPSILON);
	m_viewDirty = true;
	m_frustumDirty = true;
	m_projectionDirty = true;
}

void CameraNode::SetLodBias(float bias) {
	m_lodBias = Max(bias, M_EPSILON);
}

void CameraNode::SetViewMask(unsigned mask) {
	m_viewMask = mask;
}

void CameraNode::SetViewOverrideFlags(unsigned flags) {
	m_viewOverrideFlags = flags;
}

void CameraNode::SetOrthographic(bool enable) {
	m_orthographic = enable;
	m_frustumDirty = true;
	m_projectionDirty = true;
}

void CameraNode::SetAutoAspectRatio(bool enable) {
	m_autoAspectRatio = enable;
}

void CameraNode::SetProjectionOffset(const Vector2& offset) {
	m_projectionOffset = offset;
	m_projectionDirty = true;
}

void CameraNode::SetUseReflection(bool enable) {
	m_useReflection = enable;
	m_viewDirty = true;
	m_frustumDirty = true;
}

void CameraNode::SetReflectionPlane(const Plane& plane) {
	m_reflectionPlane = plane;
	m_reflectionMatrix = m_reflectionPlane.ReflectionMatrix();
	m_viewDirty = true;
	m_frustumDirty = true;
}

void CameraNode::SetUseClipping(bool enable) {
	m_useClipping = enable;
	m_projectionDirty = true;
}

void CameraNode::SetClipPlane(const Plane& plane) {
	m_clipPlane = plane;
	m_projectionDirty = true;
}

void CameraNode::SetFlipVertical(bool enable) {
	MutexLock lock(m_mutex);
	m_flipVertical = enable;
	m_projectionDirty = true;
}

float CameraNode::GetNearClip() const {
	// Orthographic camera has always near clip at 0 to avoid trouble with shader depth parameters,
	// and unlike in perspective mode there should be no depth buffer precision issue
	if (!m_orthographic)
		return m_nearClip;
	else
		return 0.0f;
}

/**
 * @得到最佳的裁剪面
 * @return
 */
float CameraNode::GetFitClip() {
	return (this->GetWorldPosition() - this->GetRotateCenter()).Length()
			- this->GetNearClip();
}

float CameraNode::GetHalfClip() const {
	return (this->GetNearClip() + this->GetFarClip()) / 2;
}

Frustum CameraNode::GetSplitFrustum(float nearClip, float farClip) const {
	Frustum ret;

	Matrix3x4 worldTransform = GetEffectiveWorldTransform();
	nearClip = Max(nearClip, GetNearClip());
	farClip = Min(farClip, m_farClip);
	if (farClip < nearClip)
		farClip = nearClip;

	if (!m_orthographic)
		ret.Define(m_fov, m_aspectRatio, m_zoom, nearClip, farClip,
				worldTransform);
	else
		ret.DefineOrtho(m_orthoSize, m_aspectRatio, m_zoom, nearClip, farClip,
				worldTransform);

	return ret;
}

Frustum CameraNode::GetViewSpaceFrustum() const {
	Frustum ret;

	if (!m_orthographic)
		ret.Define(m_fov, m_aspectRatio, m_zoom, GetNearClip(), m_farClip);
	else
		ret.DefineOrtho(m_orthoSize, m_aspectRatio, m_zoom, GetNearClip(),
				m_farClip);

	return ret;
}

Frustum CameraNode::GetViewSpaceSplitFrustum(float nearClip,
		float farClip) const {
	Frustum ret;

	nearClip = Max(nearClip, GetNearClip());
	farClip = Min(farClip, m_farClip);
	if (farClip < nearClip)
		farClip = nearClip;

	if (!m_orthographic)
		ret.Define(m_fov, m_aspectRatio, m_zoom, nearClip, farClip);
	else
		ret.DefineOrtho(m_orthoSize, m_aspectRatio, m_zoom, nearClip, farClip);

	return ret;
}

Ray CameraNode::GetScreenRay(float x, float y) const {
	Ray ret;

	// If projection is invalid, just return a ray pointing forward
	if (!IsProjectionValid()) {
		ret.m_origin = this->GetWorldPosition();
		ret.m_direction = this->GetWorldDirection();
		return ret;
	}

	Matrix4 viewProjInverse = (GetProjection(false) * GetView()).Inverse();

	// The parameters range from 0.0 to 1.0. Expand to normalized device coordinates (-1.0 to 1.0) & flip Y axis
	x = 2.0f * x - 1.0f;
	y = 1.0f - 2.0f * y;
	Vector3 _near(x, y, 0.0f);
	Vector3 _far(x, y, 1.0f);

	ret.m_origin = viewProjInverse * _near;
	ret.m_direction = ((viewProjInverse * _far) - ret.m_origin).Normalized();
	return ret;
}

Vector2 CameraNode::WorldToScreenPoint(const Vector3& worldPos) const {
	Vector3 eyeSpacePos = GetView() * worldPos;
	Vector2 ret;

	if (eyeSpacePos.m_z < 0.0f) {
		Vector3 screenSpacePos = GetProjection(false) * eyeSpacePos;
		ret.m_x = screenSpacePos.m_x;
		ret.m_y = screenSpacePos.m_y;
	} else {
		ret.m_x = (-eyeSpacePos.m_x > 0.0f) ? -1.0f : 1.0f;
		ret.m_y = (-eyeSpacePos.m_y > 0.0f) ? -1.0f : 1.0f;
	}

	ret.m_x = (ret.m_x / 2.0f) + 0.5f;
	ret.m_y = 1.0f - ((ret.m_y / 2.0f) + 0.5f);
	return ret;
}

Vector3 CameraNode::ScreenToWorldPoint(const Vector3& screenPos) const {
	Ray ray = GetScreenRay(screenPos.m_x, screenPos.m_y);
	return ray.m_origin + ray.m_direction * screenPos.m_z;
}

float CameraNode::GetClipDis() const {
	return GetFarClip() - GetNearClip();
}

void CameraNode::SetVR(bool isVR) {
	this->m_isVR = isVR;
}

void CameraNode::SetVRProjectionMatrix(const float* proMat) {
	Matrix4 mat(proMat);
	this->m_projection = mat;
}

void CameraNode::SetVRViewMatrix(const float * viewMat) {

	Matrix4 mat(viewMat);
	Matrix3x4 mat3x4(mat);
	m_VREyeMat = mat3x4;

	this->m_frustumDirty = true;
}

const Frustum& CameraNode::GetFrustum() const {
	if (m_frustumDirty) {
		Matrix3x4 worldTransform = GetEffectiveWorldTransform();

		if (!m_orthographic)
			m_frustum.Define(m_fov, m_aspectRatio, m_zoom, GetNearClip(),
					m_farClip, worldTransform);
		else
			m_frustum.DefineOrtho(m_orthoSize, m_aspectRatio, m_zoom,
					GetNearClip(), m_farClip, worldTransform);

		m_frustumDirty = false;
	}

	return m_frustum;
}

const Matrix4& CameraNode::GetProjection() const {
	MutexLock lock(m_mutex);
	if (m_projectionDirty && !this->m_isVR) {
		m_projection = GetProjection(true);
		m_projectionDirty = false;
	}

	//if (m_cameraMode == 1)
	//{
	//	return m_leftEyeProjMat;
	//}
	//else if (m_cameraMode == 2)
	//{
	//	return m_rightEyeProjMat;
	//}
	//else
	{
		return m_projection;
	}
}

Matrix4 CameraNode::GetProjection(bool apiSpecific) const {
	MutexLock lock(m_mutex);
	Matrix4 ret(Matrix4::ZERO);
// Whether to construct matrix using OpenGL or Direct3D clip space convention
//	    #ifdef M3D_OPENGL
	bool openGLFormat = apiSpecific;
//	    #else
//	    bool openGLFormat = false;
//	    #endif

	if (!m_orthographic) {
		float nearClip = GetNearClip();
		float h = (1.0f / tanf(m_fov * M_DEGTORAD * 0.5f)) * m_zoom;
		float w = h / m_aspectRatio;
		float q, r;

		if (openGLFormat) {
			q = -(m_farClip + nearClip) / (m_farClip - nearClip);
			r = -2.0f * m_farClip * nearClip / (m_farClip - nearClip);
		} else {
			q = -m_farClip / (m_farClip - nearClip);
			r = q * nearClip;
		}

		ret.m_m00 = w;
		ret.m_m02 = m_projectionOffset.m_x * 2.0f;
		ret.m_m11 = h;
		ret.m_m12 = m_projectionOffset.m_y * 2.0f;
		ret.m_m22 = q;
		ret.m_m23 = r;
		ret.m_m32 = -1.0f;
	} else {
		// Disregard near clip, because it does not affect depth precision as with perspective projection
		float h = (1.0f / (m_orthoSize * 0.5f)) * m_zoom;
		float w = h / m_aspectRatio;
		float q, r;

		if (openGLFormat) {
			q = -2.0f / m_farClip;
			r = -1.0f;
		} else {
			q = -1.0f / m_farClip;
			r = 0.0f;
		}

		ret.m_m00 = w;
		ret.m_m03 = m_projectionOffset.m_x * 2.0f;
		ret.m_m11 = h;
		ret.m_m13 = m_projectionOffset.m_y * 2.0f;
		ret.m_m22 = q;
		ret.m_m23 = r;
		ret.m_m33 = 1.0f;
	}
	if (m_flipVertical)
		ret = flipMatrix * ret;

	return ret;
}

void CameraNode::GetFrustumSize(Vector3& _near, Vector3& _far) const {
	_near.m_z = GetNearClip();
	_far.m_z = m_farClip;

	if (!m_orthographic) {
		float halfViewSize = tanf(m_fov * M_DEGTORAD * 0.5f) / m_zoom;
		_near.m_y = _near.m_z * halfViewSize;
		_near.m_x = _near.m_y * m_aspectRatio;
		_far.m_y = _far.m_z * halfViewSize;
		_far.m_x = _far.m_y * m_aspectRatio;
	} else {
		float halfViewSize = m_orthoSize * 0.5f / m_zoom;
		_near.m_y = _far.m_y = halfViewSize;
		_near.m_x = _far.m_x = _near.m_y * m_aspectRatio;
	}

	if (m_flipVertical) {
		_near.m_y = -_near.m_y;
		_far.m_y = -_far.m_y;
	}
}

float CameraNode::GetHalfViewSize() const {
	if (!m_orthographic)
		return tanf(m_fov * M_DEGTORAD * 0.5f) / m_zoom;
	else
		return m_orthoSize * 0.5f / m_zoom;
}

float CameraNode::GetDistance(const Vector3& worldPos) const {
	if (!m_orthographic) {
		const Vector3& cameraPos = this->GetWorldPosition();
		return (worldPos - cameraPos).Length();
	} else
		return Abs((GetView() * worldPos).m_z);
}

float CameraNode::GetDistanceSquared(const Vector3& worldPos) const {
	if (!m_orthographic) {
		const Vector3& cameraPos = this->GetWorldPosition();
		return (worldPos - cameraPos).LengthSquared();
	} else {
		float distance = (GetView() * worldPos).m_z;
		return distance * distance;
	}
}

float CameraNode::GetLodDistance(float distance, float scale,
		float bias) const {
	float d = Max(m_lodBias * bias * scale * m_zoom, M_EPSILON);
	if (!m_orthographic)
		return distance / d;
	else
		return m_orthoSize / d;
}

Quaternion CameraNode::GetFaceCameraRotation(const Vector3& position,
		const Quaternion& rotation, FaceCameraMode mode) {

	switch (mode) {
	default:
		return rotation;

	case FC_ROTATE_XYZ:
		return this->GetWorldRotation();

	case FC_ROTATE_Y: {
		Vector3 euler = rotation.EulerAngles();
		euler.m_y = this->GetWorldRotation().EulerAngles().m_y;
		return Quaternion(euler.m_x, euler.m_y, euler.m_z);
	}

	case FC_LOOKAT_XYZ: {
		Quaternion lookAt;
		lookAt.FromLookRotation(position - this->GetWorldPosition());
		return lookAt;
	}

	case FC_LOOKAT_Y: {
		// Make the Y-only lookat happen on an XZ plane to make sure there are no unwanted transitions
		// or singularities
		Vector3 lookAtVec(position - this->GetWorldPosition());
		lookAtVec.m_y = 0.0f;

		Quaternion lookAt;
		lookAt.FromLookRotation(lookAtVec);

		Vector3 euler = rotation.EulerAngles();
		euler.m_y = lookAt.EulerAngles().m_y;
		return Quaternion(euler.m_x, euler.m_y, euler.m_z);
	}
	}
}

Matrix3x4 CameraNode::GetEffectiveWorldTransform() const {
	MutexLock lock(m_mutex);
	Matrix3x4 worldTransform =
			this ? Matrix3x4(this->GetWorldPosition(), this->GetWorldRotation(),
							1.0f) :
					Matrix3x4::IDENTITY;
	return m_useReflection ?
			m_reflectionMatrix * worldTransform : worldTransform;
}

bool CameraNode::IsProjectionValid() const {
	return m_farClip > GetNearClip();
}

Matrix3x4& CameraNode::GetView() const {
	if (m_viewDirty) {
		// Note: view matrix is unaffected by node or parent scale
		m_view = GetEffectiveWorldTransform().Inverse();
		m_viewDirty = false;
	}
	if (m_isVR) {
		m_VREyeMatTemp = m_VREyeMat * m_view;
		return m_VREyeMatTemp;
	} else {
		if (m_cameraMode == 1) {
			m_tempMatrix = m_leftEyeMat * m_view;
		} else if (m_cameraMode == 2) {
			m_tempMatrix = m_rightEyeMat * m_view;
		} else {
			m_tempMatrix = m_view;
		}

		return m_tempMatrix;
	}
}

void CameraNode::SetAspectRatioInternal(float aspectRatio) {
	m_aspectRatio = aspectRatio;
	m_frustumDirty = true;
	m_projectionDirty = true;
}

void CameraNode::SetOrthoSizeAttr(float orthoSize) {
	m_orthoSize = orthoSize;
	m_frustumDirty = true;
	m_projectionDirty = true;
}

void CameraNode::SetReflectionPlaneAttr(const Vector4& value) {
	SetReflectionPlane(Plane(value));
}

void CameraNode::SetClipPlaneAttr(const Vector4& value) {
	SetClipPlane(Plane(value));
}

Vector4 CameraNode::GetReflectionPlaneAttr() const {
	return m_reflectionPlane.ToVector4();
}

Vector4 CameraNode::GetClipPlaneAttr() const {
	return m_clipPlane.ToVector4();
}

void CameraNode::OnMarkedDirty() {
	m_frustumDirty = true;
	m_viewDirty = true;
	m_frustumDirty = true;
}
void CameraNode::SetCameraMode(int mode) {
	m_cameraMode = mode;
}

void CameraNode::SetPupilDistance(int distance) {
	m_leftEyeMat.SetTranslation(Vector3(distance, 0, 0));
	m_rightEyeMat.SetTranslation(Vector3(-distance, 0, 0));
}

void CameraNode::UpdatePupilCamera(float focalLengths) {
	float focalLength = focalLengths;
	float fov = m_fov;
	float aspect = GetAspectRatio();
	float near = GetNearClip();
	float far = m_farClip;

	Matrix4 projectionMatrix = GetProjection();
	Matrix4 eyeLeft, eyeRight;
	float eyeSep = 0.064 / 2;
	float eyeSepOnProjection = eyeSep * near / focalLength;

	float ymax = near * M3D::Tan(fov * 0.5);
	float xmin, xmax;

	eyeLeft.m_m03 = -eyeSep;
	eyeRight.m_m03 = eyeSep;

	// 左眼
	xmin = -ymax * aspect + eyeSepOnProjection;
	xmax = ymax * aspect + eyeSepOnProjection;

	projectionMatrix.m_m00 = 2 * near / (xmax - xmin);
	projectionMatrix.m_m02 = (xmax + xmin) / (xmax - xmin);

	m_leftEyeProjMat = projectionMatrix;

	//右眼

	xmin = -ymax * aspect - eyeSepOnProjection;
	xmax = ymax * aspect - eyeSepOnProjection;

	projectionMatrix.m_m00 = 2 * near / (xmax - xmin);
	projectionMatrix.m_m02 = (xmax + xmin) / (xmax - xmin);

	m_rightEyeProjMat = (projectionMatrix);

	m_leftEyeMat.SetTranslation(Vector3(-eyeSep, 0, 0));
	m_rightEyeMat.SetTranslation(Vector3(eyeSep, 0, 0));

}
}

