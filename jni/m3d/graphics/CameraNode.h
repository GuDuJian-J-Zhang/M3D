/**@file
 *@brief   摄像机节点
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_CAMERA_NODE_H
#define M3D_CAMERA_NODE_H

#include "m3d/M3DMacros.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/base/ControlInfo.h"
#include "m3d/graphics/GraphicsDefs.h"
#include "m3d/base/Plane.h"
#include "m3d/base/Ray.h"
#include "m3d/base/Viewport.h"
#include "m3d/base/Frustum.h"
#include "../model/Model.h"
namespace M3D
{
static const float DEFAULT_NEARCLIP = 0.1f;
static const float DEFAULT_FARCLIP = 1000.0f;
static const float DEFAULT_CAMERA_FOV = 45.0f;
static const float DEFAULT_ORTHOSIZE = 20.0f;

static const unsigned VO_NONE = 0x0;
static const unsigned VO_LOW_MATERIAL_QUALITY = 0x1;
static const unsigned VO_DISABLE_SHADOWS = 0x2;
static const unsigned VO_DISABLE_OCCLUSION = 0x4;

class SignModel;


/**@class Camera
 * @brief Camera类
 *
 * 提供Camera类的相关方法
 */
struct CAMERA_INFO
{
	int matrix_type; //!<矩阵类型 0=normal 1=only Matrix;
	int id;
	Vector3 origin_position; //!< 相机原始坐标
	Vector3 target_vector; //!< 目标向量
	Vector3 up_vector; //!< 正上方向量
	float angle; //!< 倾角
	float focus; //!< 焦距
	float zoom_factor; //!< 缩放因子
	int project_type; //!< 投影类型
	Matrix4 state_matrix; //!<相机矩阵
};


class M3D_API CameraState
{
public:
	Matrix4 mvMatrix;//!<模型视图矩阵
	Matrix4 projectMatrix;//!<投影矩阵
	int viewPort[4];//!<视图矩阵
};

class M3D_API CameraNode: public SignModel
{
public:
	const static int FRUSTUM_INTER;//!<平截头体相交
	const static int FRUSTUM_OUT;//!<平截头体外部
	const static int FRUSTUM_INER;//!<平截头体内部
	const static Matrix4 flipMatrix;//!<裁剪面矩阵
public:
	CameraNode(void);
	virtual ~CameraNode(void);

public:
	static float drawLimit;//!<绘制限制
	/**
	 * @brief 设置缩放
	 * @param scale 缩放变量
	 */
	//virtual void SetScale(float scale);
	/**
	 * @brief 复位
	 */
	virtual void ReSet();

	/**
	 * @brief 当前相机状态转化为Json串
	 * @return
	 */
	virtual string GetStates();

	/**
	 * @brief 设置相机状态
	 * @param states
	 * @return
	 */
    virtual bool SetStates(const string& states);
    /**
    	 * @brief 设置相机位置
    	 * @param states
    	 * @return
    	 */
    virtual bool setPosition(const string& position);

	void SetDirection(const Vector3 & direction);

	/**
	* @brief Return position in parent space.
	*/
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/**
	* @brief Return rotation in parent space.
	*/
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

    /**
    	 * @brief 设置相机方向
    	 * @param states
    	 * @return
    	 */
    virtual bool setDirection(const string& direction);

	virtual void SetPosition(const Vector3& position);

	virtual void SetRotation(const Quaternion& rotation);

	virtual void SetScale(const Vector3& scale);

	Vector3& GetOldPosition();

	Quaternion& GetOldRotation();


	/**
	* @brief Return up direction in parent space. Positive Y axis equals identity rotation.
	*/
	Vector3 GetUp() const;

	/**
	* @brief Return right direction in parent space. Positive X axis equals identity rotation.
	*/

	Vector3 GetRight() const;

	virtual void SetTransform(const Vector3& position, const Quaternion& rotation);

	virtual void SetTransform(const Vector3& position, const Quaternion& rotation, float scale);

	virtual void SetTransform(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

	void Translate(const Vector3& delta, TransformSpace space = TS_LOCAL);

	void Rotate(const Quaternion& delta, TransformSpace space = TS_LOCAL);

	virtual Matrix3x4 GetTransform() const;

	/**
	 * @brief 缩放视图
	 * @param scaleFactor
	 */
	void ZoomView(float scaleFactor = 1.0f);

	/**
	 * @brief 设置平行投影大小
	 * @param fOrthoWidth
	 * @param fOrthoHeight
	 */
	void GetOrthoSize(float *fOrthoWidth, float *fOrthoHeight);

	/**
	 * @brief 设置旋转中心
	 * @param delta
	 * @param space
	 */
	void RotateAroundCenter(const Quaternion& delta,
				TransformSpace space = TS_LOCAL);

	void RotateAround(const Vector3 & point, const Quaternion & delta, TransformSpace space = TS_LOCAL);

	/**
	 * @brief 获取视口
	 * @return
	 */
	const Viewport& GetViewPort() const;
	/**
	 * @brief 设置视口
	 * @param x
	 * @param y
	 * @param width
	 * @param heigh
	 */
	void SetViewPort(int x, int y, int width, int heigh);

	/**
	 * @brief Set near clip distance.
	 * @param nearClip
	 */
	void SetNearClip(float nearClip);

	/**
	 * @brief Set far clip distance.
	 * @param farClip
	 */
	void SetFarClip(float farClip);

	/**
	 * @brief Set vertical field of view in degrees.
	 * @param fov
	 */
	void SetFov(float fov);

	/**
	 * @brief Set orthographic mode view uniform size.
	 * @param orthoSize
	 */
	void SetOrthoSize(float orthoSize);

	/**
	 * @brief Set orthographic mode view non-uniform size. Disables the auto aspect ratio -mode.
	 * @param orthoSize
	 */
	void SetOrthoSize(const Vector2& orthoSize);

	/**
	 * @brief Set aspect ratio manually. Disables the auto aspect ratio -mode.
	 * @param aspectRatio
	 */
	void SetAspectRatio(float aspectRatio);

	/**
	 * @brief Set zoom.
	 * @param zoom
	 */
	void SetZoom(float zoom);

	/**
	 * @brief Set LOD bias.
	 * @param bias
	 */
	void SetLodBias(float bias);

	/**
	 * @brief Set view mask. Will be and'ed with object's view mask to see if the object should be rendered.
	 * @param mask
	 */
	void SetViewMask(unsigned mask);

	/**
	 * @brief Set view override flags.
	 * @param flags
	 */
	void SetViewOverrideFlags(unsigned flags);

	/**
	 * @brief Set orthographic mode enabled/disabled.
	 * @param enable
	 */
	void SetOrthographic(bool enable);

	/**
	 * @brief Set automatic aspect ratio based on viewport dimensions. Enabled by default.
	 * @param enable
	 */
	void SetAutoAspectRatio(bool enable);

	/**
	 * @brief Set projection offset. It needs to be calculated as (offset in pixels) / (viewport dimensions.)
	 * @param offset
	 */
	void SetProjectionOffset(const Vector2& offset);

	/**
	 * @brief Set reflection mode.
	 * @param enable
	 */
	void SetUseReflection(bool enable);

	/**
	 * @brief Set reflection plane in world space for reflection mode.
	 * @param plane
	 */
	void SetReflectionPlane(const Plane& plane);

	/**
	 * @brief  Set whether to use a custom clip plane.
	 * @param enable
	 */
	void SetUseClipping(bool enable);

	/**
	 * @brief Set custom clipping plane in world space.
	 * @param plane
	 */
	void SetClipPlane(const Plane& plane);

	/** Set vertical flipping mode. Called internally by View to resolve OpenGL / Direct3D9 rendertarget sampling differences.
	 * @brief
	 * @param enable
	 */
	void SetFlipVertical(bool enable);


	/**
	 * @brief Return far clip distance.
	 * @return
	 */
	float GetFarClip() const
	{
		return m_farClip;
	}
	/**
	 * @brief 设置旋转中心
	 * @param center
	 */
	void SetRotateCenter(const Vector3& center);
	/**
	 * @brief 设置初始旋转中心
	 * @param center
	 */
	void SetInitRotateCenter(const Vector3& center);

	/**
	 * @brief 获取旋转中心
	 * @return
	 */
	Vector3& GetRotateCenter();
	/**
	 * @brief 获取原始旋转中心
	 * @return
	 */
	Vector3& GetOrigRotateCenter();

	void GetLookAt(Vector3& eye, Vector3& center, Vector3& up, float lookDistance = 1.0f) const;
	/**
	 * @brief Return near clip distance.
	 * @return
	 */
	float GetNearClip()const;

	/**
	* @得到最佳的裁剪面
	* @return
	*/
	float GetFitClip();

	/**
	 * @brief 获取半裁剪面
	 * @return
	 */
	float GetHalfClip()const;
	/**
	 * @brief Return vertical field of view in degrees.
	 * @return
	 */
	float GetFov() const
	{
		return m_fov;
	}

	/**
	 * @brief Return orthographic mode size.
	 * @return
	 */
	float GetOrthoSize() const
	{
		return m_orthoSize;
	}

	/**
	 * @brief Return aspect ratio.
	 * @return
	 */
	float GetAspectRatio() const
	{
		return m_aspectRatio;
	}

	/**
	 * @brief Return zoom.
	 * @return
	 */
	float GetZoom() const
	{
		return m_zoom;
	}
    
	/**
	 * @brief 获取裁剪
	 * @return
	 */
    float GetClipDis()const;
    
	/**
	 * @brief Return LOD bias.
	 * @return
	 */
	float GetLodBias() const
	{
		return m_lodBias;
	}

	/**
	 * @brief Return view mask.
	 */
	unsigned GetViewMask() const
	{
		return m_viewMask;
	}

	/**
	 * @brief Return view override flags.
	 */
	unsigned GetViewOverrideFlags() const
	{
		return m_viewOverrideFlags;
	}
//	/// Return fill mode.
//	FillMode GetFillMode() const { return fillMode_; }

	/**
	 * @brief Return orthographic flag.
	 * @return
	 */
	bool IsOrthographic() const
	{
		return m_orthographic;
	}

	/**
	 * @brief Return auto aspect ratio flag.
	 * @return
	 */
	bool GetAutoAspectRatio() const
	{
		return m_autoAspectRatio;
	}

	/**
	 * @brief Return frustum in world space.
	 * @return
	 */
	const Frustum& GetFrustum() const;

	void SetVRProjectionMatrix(const float* proMat);
	void SetVRViewMatrix(const float * viewMat);
	void SetVR(bool isVR);

	/**
	 * @brief Return API-specific projection matrix. For OGL only
	 * @return
	 */
	const Matrix4& GetProjection() const;

	/**
	 * @brief Return either API-specific or API-independent (D3D convention) projection matrix.
	 * @param apiSpecific
	 * @return
	 */
	Matrix4 GetProjection(bool apiSpecific) const;

	/**
	 * @brief Return view matrix.
	 * @return
	 */
	Matrix3x4& GetView() const;

	/**
	 * @brief Return frustum near and far sizes.
	 * @param near
	 * @param _far
	 */
	void GetFrustumSize(Vector3& near, Vector3& _far) const;

	/**
	 * @brief Return half view size.
	 * @return
	 */
	float GetHalfViewSize() const;

	/**
	 * @brief Return frustum split by custom near and far clip distances.
	 * @param nearClip
	 * @param farClip
	 * @return
	 */
	Frustum GetSplitFrustum(float nearClip, float farClip) const;

	/**
	 * @brief Return frustum in view space.
	 * @return
	 */
	Frustum GetViewSpaceFrustum() const;

	/**
	 * @brief Return split frustum in view space.
	 * @param nearClip
	 * @param farClip
	 * @return
	 */
	Frustum GetViewSpaceSplitFrustum(float nearClip, float farClip) const;

	/**
	 * @brief Return ray corresponding to normalized screen coordinates (0.0 - 1.0).
	 * @param x
	 * @param y
	 * @return
	 */
	Ray GetScreenRay(float x, float y) const;
	// Convert a world space point to normalized screen coordinates (0.0 - 1.0).
/**
 * @brief 世界坐标转到屏幕坐标
 * @param worldPos
 * @return
 */
	Vector2 WorldToScreenPoint(const Vector3& worldPos) const;
	// Convert normalized screen coordinates (0.0 - 1.0) and depth to a world space point.
	/**
	 * @brief 屏幕坐标转换到世界坐标
	 * @param screenPos
	 * @return
	 */
	Vector3 ScreenToWorldPoint(const Vector3& screenPos) const;

	/**
	 * @brief  Return projection offset.
	 * @return
	 */
	const Vector2& GetProjectionOffset() const
	{
		return m_projectionOffset;
	}

	/**
	 * @brief Return whether is using reflection.
	 * @return
	 */
	bool GetUseReflection() const
	{
		return m_useReflection;
	}

	/**
	 * @brief Return the reflection plane.
	 * @return
	 */
	const Plane& GetReflectionPlane() const
	{
		return m_reflectionPlane;
	}

	/**
	 * @brief Return whether is using a custom clipping plane.
	 * @return
	 */
	bool GetUseClipping() const
	{
		return m_useClipping;
	}

	/**
	 * @brief  Return the custom clipping plane.
	 * @return
	 */
	const Plane& GetClipPlane() const
	{
		return m_clipPlane;
	}

	/**
	 * @brief Return vertical flipping mode.
	 * @return
	 */
	bool GetFlipVertical() const
	{
		return m_flipVertical;
	}

	/**
	 * @brief Return whether to reverse culling; affected by vertical flipping and reflection.
	 * @return
	 */
	bool GetReverseCulling() const
	{
		return m_flipVertical ^ m_useReflection;
	}

	/**
	 * @brief Return distance to position. In orthographic mode uses only Z coordinate.
	 * @param worldPos
	 * @return
	 */
	float GetDistance(const Vector3& worldPos) const;

	/**
	* @brief Return forward direction in parent space. Positive Z axis equals identity rotation.
	*/
	Vector3 GetDirection() const;

	/**
	 * @brief Return squared distance to position. In orthographic mode uses only Z coordinate.
	 * @param worldPos
	 * @return
	 */
	float GetDistanceSquared(const Vector3& worldPos) const;

	/**
	 * @brief Return a scene node's LOD scaled distance.
	 * @param distance
	 * @param scale
	 * @param bias
	 * @return
	 */
	float GetLodDistance(float distance, float scale, float bias) const;

	/**
	 * @brief Return a world rotation for facing a camera on certain axes based on the existing world rotation.
	 * @param position
	 * @param rotation
	 * @param mode
	 * @return
	 */
	Quaternion GetFaceCameraRotation(const Vector3& position,
			const Quaternion& rotation, FaceCameraMode mode);

	/**
	 * @brief Get effective world transform for matrix and frustum calculations including reflection but excluding node scaling.
	 * @return
	 */
	Matrix3x4 GetEffectiveWorldTransform() const;

	/**
	 * @brief Return if projection parameters are valid for rendering and raycasting.
	 * @return
	 */
	bool IsProjectionValid() const;


	/**
	 * @brief  Set aspect ratio without disabling the "auto aspect ratio" mode. Called internally by View.
	 * @param aspectRatio
	 */
	void SetAspectRatioInternal(float aspectRatio);

	/**
	 * @brief Set orthographic size attribute without forcing the aspect ratio.
	 * @param orthoSize
	 */
	void SetOrthoSizeAttr(float orthoSize);

	virtual void SetWorldPosition(const Vector3& position);

	/**
	 * @brief Set reflection plane attribute.
	 * @param value
	 */
	void SetReflectionPlaneAttr(const Vector4& value);

	/**
	 * @brief Return reflection plane attribute.
	 * @return
	 */
	Vector4 GetReflectionPlaneAttr() const;

	/**
	 * @brief Set clipping plane attribute.
	 * @param value
	 */
	void SetClipPlaneAttr(const Vector4& value);

	/**
	 * @brief Return clipping plane attribute.
	 * @return
	 */
	Vector4 GetClipPlaneAttr() const;

	/**
	 * @brief 标记
	 */
	virtual void OnMarkedDirty();

	void RayPick(RayPickAction* action);
	void SetCameraMode(int mode);

	void SetPupilDistance(int distance);

	void UpdatePupilCamera(float focalLength);

	void SetSceneManager(SceneManager* sceneMgr);
	SceneManager* GetSceneManager();

	virtual void FindVisiableObject(RenderAction* renderAction);
private:
	mutable Viewport m_viewPort;
	Vector3 m_origRotateCenter;
	Vector3 m_rotateCenter;

	mutable Matrix3x4 m_view;//!< Cached view matrix.

	mutable Matrix4 m_projection;//!< Cached projection matrix.

	mutable Frustum m_frustum;//!< Cached frustum.

	mutable bool m_viewDirty;//!< View matrix dirty flag.

	mutable bool m_projectionDirty;//!< Projection matrix dirty flag.

	mutable bool m_frustumDirty;//!< Frustum dirty flag.

	bool m_orthographic;//!< Orthographic mode flag.

	float m_nearClip;//!< Near clip distance.

	float m_farClip;//!< Far clip distance.

	float m_fov;//!< Field of view.

	float m_orthoSize;//!< Orthographic view size.

	float m_aspectRatio;//!< Aspect ratio.

	float m_zoom;//!< Zoom.

	float m_lodBias;//!< LOD bias.

	unsigned m_viewMask;//!< View mask.

	unsigned m_viewOverrideFlags;//!< View override flags.
	/// Fill mode.
//    FillMode fillMode_;

	Vector2 m_projectionOffset;//!< Projection offset.

	Plane m_reflectionPlane;//!< Reflection plane.

	Plane m_clipPlane;//!< Clipping plane.

	Matrix3x4 m_reflectionMatrix;//!< Reflection matrix calculated from the plane.

	bool m_autoAspectRatio;//!< Auto aspect ratio flag.

	mutable bool m_flipVertical;//!< Flip vertical flag.

	bool m_useReflection;//!< Reflection mode enabled flag.

	bool m_useClipping;//!< Use custom clip plane flag.
	
	Matrix3x4 m_leftEyeMat;
	Matrix3x4 m_rightEyeMat;
	Matrix4 m_leftEyeProjMat;
	Matrix4 m_rightEyeProjMat;
	mutable Matrix3x4 m_tempMatrix;

	SceneManager* m_scene;

	Vector3 m_position;//!<位置
	Vector3 m_oldPosition;

	Quaternion m_rotation;//!<选择
	Quaternion m_oldRotation;

	Vector3 m_scale;//!<缩放
	Vector3 m_oldScale;
	mutable Mutex m_mutex;

	public:
		int m_cameraMode;//!<相机模式：0表示正常；1表示左眼；2表示右眼。
	//VR xiangg
	bool m_isVR;
	Matrix3x4 m_VREyeMat;
	mutable Matrix3x4 m_VREyeMatTemp;
};

}
#endif // Camera_Node_H
