/**@file
 *@brief	TouchHandler类头文件
 *@author
 *@date
 *@version	1.0
 *
 *提供TouchHandler类的声明
 *
 */
#ifndef M3D_WALKTHROUGHHANDLER_H_
#define M3D_WALKTHROUGHHANDLER_H_
#define BIMSPEED
#include "sview/manipulator/CommonTouchHandler.h"

namespace SVIEW
{

class View;

/**@class
 * @brief 该类用来支持透视投影下的触摸操作。主要用于BIM漫游
 *
 *
 *
 */
class M3D_API WalkthroughHandler: public CommonTouchHandler
{
public:
	WalkthroughHandler();
	virtual ~WalkthroughHandler();

	virtual void InitCamera(bool useAni = true);

	/**
	 * 前后移动
	 * @param speed
	 */
	void MoveStraight(float speed);
	/**
	 * 左右移动
	 * @param speed
	 */
	void MoveSideways(float speed);

	/**
	* 上下移动
	* @param speed
	*/
	void MoveUpAndDown(float speed);

	/**
	 * 虚拟键移动 合并了前后左右移动动作
	 * @param strSpeed
	 * @param sidSpeed
	 */
	void VirtualKeyMove(float strSpeed,float sidSpeed);

	/**
	 * 上下旋转
	 * @param degree
	 */
	void RotateUpAndDown (float degree);
	/**
	 * 左右旋转
	 * @param degree
	 */
	void RotateSideways (float degree);

	/**
	 * 设置透视投影中的fov角度值
	 * @param fov
	 */
	void SetCameraFov(float fov);

	/**
	 * 约束模式
	 * @param value
	 */
	void ConstraintMode(bool value);

	/**
	 * 自由观察模式
	 * @param value
	 */
	virtual void FreeViewMode(bool value);//自由观察模式
	/**
	 * 方向所动模式
	 * @param value
	 */
	void OribitMode(bool value);


	void SetVRMode(bool vrMode);

private:
	//virtual void OnTouchUp(float* p, int n);
	//virtual void OnTouchMove(int moveType, float* p, int n);
	//virtual void OnTouchDown(float* p, int n);

private:
	//virtual void InitModelViewCamera();
	/**
	 * 优化相机
	 */
	virtual void OptimizeCamera();
	/**
	 * 两支抬起调用
	 * @param pos
	 * @param n
	 */
	void TwoPointsUp(float*pos, int n);
	/**
	 * 两指移动时调用
	 * @param
	 * @param n
	 */
	void TwoPointsMove(float*, int n = 2);
	/**
	 * 两指按下时调用
	 * @param
	 * @param n
	 */
	void TwoPointsStart(float*, int n = 2);
	/**
	 * 两指之间的屏幕坐标距离
	 * @param
	 * @param n
	 * @return
	 */
	float TwoPointsDis(float*, int n = 2);

	/**
	 * 两指在屏幕上的旋转角度
	 * @param lastVector1
	 * @param lastVector2
	 * @param currentVector1
	 * @param currentVector2
	 * @param camera
	 * @param axis
	 * @param depth
	 * @param angle
	 */
	void TwoPointsRotateAngle(const IntVector2 & lastVector1,const IntVector2 & lastVector2,
			const IntVector2 & currentVector1,const IntVector2 & currentVector2,const CameraNode * camera
			,Vector3 & axis,float depth,float & angle);

	/**
	 * 双指旋转
	 * @param lastVector1
	 * @param lastVector2
	 * @param currentVector1
	 * @param currentVector2
	 * @param camera
	 * @param target
	 * @param oldPos
	 * @param cameraPositon
	 * @param depth
	 */
	void TwoPointsRotate(const IntVector2 & lastVector1,const IntVector2 & lastVector2,
			const IntVector2 & currentVector1,const IntVector2 & currentVector2,const CameraNode * camera
			,const Vector3 & target,const Vector3 & oldPos,Vector3 &cameraPositon,float depth);

	/**
	 * 获取双指旋转矩阵
	 * @param lastVector1
	 * @param lastVector2
	 * @param currentVector1
	 * @param currentVector2
	 * @param scene
	 * @param target
	 * @param mat
	 * @param depth
	 */
	void GetTwoPointsRotateMatrix(const IntVector2 & lastVector1,const IntVector2 & lastVector2,
			const IntVector2 & currentVector1,const IntVector2 & currentVector2, SceneManager* scene
			,const Vector3 & target,Matrix4 & mat,float depth);


	/**
	 * 两指旋转
	 * @param camera
	 * @param pos
	 * @param currentPnt
	 * @param priPnt
	 */
	void TwoPointsRotate(CameraNode * camera,float * pos,const IntVector2 &currentPnt,const IntVector2 &priPnt);
 
	/**
	 * @brief 限制相机旋转
	 */
    void LimitCamera();


#ifdef BIMSPEED
    /**
     * 控制BIM漫游速度的改进函数
     */
    float GetDirectionDistance(Vector3& direction,int mark);
    void VirtualKeyControlSpeed();
    ///回掉函数
    static void* VirtualKeyControlTask(void * data);
    void  StartVirtualKeyControlTask();
    void  EndVirtualKeyControlTask();

    /**
     * 根据旋转的方向角度，判断是否需要更新目标(target)位置
     */
    bool IsNeedUpdateSpeed(Vector3& walkDirection);

#endif
 
private:

	void OrbitControl();

	void LockPointer();

	/**
	 * @brief 绕坐标轴选准
	 */
	void RotateAroundAxis();

	/**
	 * 自由旋转
	 */
	void FreeViewRotate();

	void OnUpDataTouchIntent();

	/**
	 * 控制靠近目标时的速度
	 * @param currentPnt
	 */
	void ControlCloseRangeSpeed(const IntVector2& currentPnt);
    
	/**
	 * 使用屏幕中心点来控制接近目标时的速度
	 */
    void ControlCloseRangeByScreenCenter();

	/**
	 *更新orbit control的目标
	 */
	void updateOribitControlTarget();


private:
	IntVector2 m_PriPointTwo1;//!<上一次保存的点
	IntVector2 m_PriPointTwo2;//!<上一次保存的点
	float m_PirDistance;//!<上一次两点距离
	float m_CurrDistance;//!<当前的距离

	Vector3 cachePriPoint;//!<缓存的上次的点
	Vector3 cacheCurPointNear;//!<缓存的当前近点
	Vector3 cacheCurPointFar;//!<缓存的当前远点


	float m_cameraToCenterDis;//!<摄像机距离包围盒的距离

	float m_movSpeed;//!<移动速度

	CTimer m_RotateOnFixedPointTimer;//!<TODO
	CTimer m_virtualKeyControlTimer;//!<TODO

	Vector3 m_oldDirectionVector;//!<上次的方向向量

	bool m_insideBoundingBox;//!<是否在包围盒内

	float m_speedControlValue;//!<速度控制

	bool m_isUpDirection;//!<是否调整向上朝向

	Matrix4 m_transMat; //!<TODO

	float m_lastDepthPos; //!<TODO

	Vector3 m_needMoveValue;//!<需要移动的大小

	bool m_useOribit;//!<使用轨道相机

	Vector3 m_oribitControlOffset;//!<

	bool m_isOribteRotateState;

	float m_deltaPhi;//!<oribi相关变量

	Vector3 m_insectPoint;//!<交点

	float m_phi;
	Vector3 m_virtualKeyTarget;//!<虚拟键中的target，用屏幕中心点求得


	bool m_vrModel;
};

}

#endif /*M3D_WALKTHROUGHHANDLER_H_*/
