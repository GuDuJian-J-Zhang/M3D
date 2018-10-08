/**@file
 *@brief	实现当前状态到指定的基本视图之间的切换，提供动画开关
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef PERSPECTIVEACTION_H_
#define PERSPECTIVEACTION_H_

#include "m3d/base/CTimer.h"
namespace SVIEW
{
class View;
}

namespace M3D
{

const int DEFAULIT_TIMES = 30;
class WorkNodes;
class SceneManager;
class Quaternion;
class Vector3;
class ControlInfo;

typedef void (*AnimationFinishCallBack)(void*);

/**
 * @class
 * @brief 实现当前状态到指定基本视图切换，提供动画开关
 *
 */
class M3D_API PerspectiveOperator
{
public:
	PerspectiveOperator();

	PerspectiveOperator(SVIEW::View* pView, int viewType, bool allowRotate =
			true, bool allowTran = false, bool allowScale = false);

	PerspectiveOperator(SVIEW::View* pView, float* pMatrix, bool allowRotate =
			true, bool allowTran = false, bool allowScale = false);

	virtual ~PerspectiveOperator();

	void Show(SVIEW::View* pView, int viewType, bool allowRotate = true,
			bool allowTran = false, bool allowScale = false);
	void Show(SVIEW::View* pView, float* pMatrix, bool allowRotate = true,
			bool allowTran = false, bool allowScale = false);
	void Show(SVIEW::View* pView, const ControlInfo& cameraMatrix,
			bool allowRotate = true, bool allowTran = false, bool allowScale =
					false, int keepMsecond = 30);

	void Apply(SVIEW::View* pView, int viewType, bool allowRotate = true,
			bool allowTran = false, bool allowScale = false);

	void Apply(SVIEW::View* pView, float* pMatrix, bool allowRotate = true,
			bool allowTran = false, bool allowScale = false);

	void Apply(SVIEW::View* pView, const ControlInfo& camereMatrix,
			bool allowRotate = true, bool allowTran = false, bool allowScale =
					false, int keepMsecond = 30);

	void HandleStandardView();

	void createMVMatrixAnimation(const ControlInfo& mvMatrix,
			const ControlInfo&to, bool allowRotate = true, bool allowTran =
					false, bool allowScale = false);
	void CreateRotationAnimation(const Quaternion& destRotation,
			const Quaternion& origRotation);
	void ApplyRotationToSelectedNode(const Quaternion& rotation);
	void ApplyMovementToSelectedNode(const Vector3& mov);
	void ApplyScaleToSelectedNode(float & scale);
	void CreatePerspectiveAnimation(const ControlInfo& to, bool allowRotate =
			true, bool allowTran = false, bool allowScale = false);

	void ExecuteCommonCameraAnimation(SVIEW::View* view, const Vector3& disPosition,
		const Quaternion& disRotation, const Vector3& disScale, bool allowRotate =
		false, bool allowTran = false, bool allowScale = false);

	//设置动画标记，如果为真说明正在进行动画。主要为动画线程同步使用
	void SetAnimationState(bool animationState);
	//获得动画标记，如果为真说明正在进行动画
	bool GetAnimationState();

	//开始线程动画
	bool StartAnimation();

	bool StartCommonAnimation();

	//结束线程动画
	bool FinishAnimation();

	void SetAnimationFinishCallback(AnimationFinishCallBack callbackfun,
			void* data);

	static void* ThreadAnimationFun(void* arg);

	static void* timerTask(void* arg);

	static void* commonTimerTask(void* arg);

private:
	void Initialize();
protected:
private:
	WorkNodes* m_pSelectedNodes;
	SVIEW::View* m_pView;

	SceneManager* m_pSceneManager;
	int m_iViewType;

	bool m_bAllowRotate;
	bool m_bAllowTran;
	bool m_bAllowScale;

	float m_pMatrix[16];

	///线程动画所需变量
	bool m_isAnimation; //<? 是否正在进行动画标识

	bool m_isFnishAnimation; //<? 退出线程动画标识

	ControlInfo* m_animationTo;
	ControlInfo* m_animationmvMatrix;
	ControlInfo* m_animationdistimvMatrix;

	//Vector3 m_disPosition;
	//Quaternion m_disRotation;
	//Vector3 m_disScale;

//    bool m_animationallowRotate;
//    bool m_animationallowTran;
//    bool m_animationallowScale;

	int m_currentAniFrame;
	int m_slerpCount;

	//动画持续时间
	int m_animationKeepMsecond;

	Quaternion* m_midRotation;
	float* m_midScale;
	Vector3* m_midTran;

	CTimer m_timer;
	//动画结束时回调函数
	AnimationFinishCallBack animationFinishCallback;
	//动画结束时回调数据
	void* animationFinishCallbackData;

	// pthread_t m_AmimationThredID; //动画线程ID
};
}

#endif //PERSPECTIVEACTION_H_
