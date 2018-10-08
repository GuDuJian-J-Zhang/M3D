/**@file
 *@brief	IView接口类头文件
 *@author 	XL
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *提供IView接口类方法的声明
 *
 */

#ifndef M3D_I_VIEW_H_
#define M3D_I_VIEW_H_
#include "sview/SView.h"
#include "m3d/base/Color.h"
#include "m3d/M3DStates.h"

namespace M3D
{
class RenderManager;
class SceneManager;
class Model;
class GroupNode;
class IDCreator;
class ModelView;
class MovedMatrixInfo;
class WorkNodes;
class Shape;
class ExplosiveViewOperator;
class BackgroundNode;
}

using namespace M3D;

namespace SVIEW {

/**@class IView
 * @brief IView接口类
 *
 * IView接口类
 *
 */
class M3D_API IView {
public:
	IView();
	virtual ~IView();

	/**
	 * @brief 初始化方法
	 */
	virtual void Initial();
	//touch operator

	/**
	 * 用户手指按下
	 * @param p
	 * @param n
	 */
	virtual void TouchDown(float* p, int n);


	/**
	 * 用户手指移动
	 * @param p
	 * @param n
	 */
	virtual void TouchMove(float* p, int n);


	/**
	 * 用户手指抬起
	 * @param p
	 * @param n
	 */
	virtual void TouchUp(float* p, int n);

	//render operator


	/**
	 * 窗口大小改变
	 * @param width
	 * @param height
	 */
	virtual void WindowSizeChange(int width, int height);


	/**
	 * @brief 刷新
	 */
	virtual void OnDraw();


	/**
	 * @brief 请求重绘
	 */
	virtual void RequestDraw();
	//file operator


	/**
	 * 读取文件
	 * @param path
	 * @return
	 */
	virtual M3D_STATUS ReadFile(string & path);


	/**
	 * @brief 关闭文件
	 */
	virtual void CloseFile();

	/**
	 * 设置背景颜色，最终颜色为屏幕顶部到屏幕底部颜色的渐变
	 * @param topColor 屏幕顶端颜色
	 * @param bottomColor 屏幕底部颜色
	 */
	virtual bool SetBackgroundColor(float* topColor,float* bottomColor,int ColorNum);
    virtual bool SetBackgroundColor(const M3D::Color& topColor,const M3D::Color& bottomColor);

    /**
     * 获取背景颜色
     * @param topColor
     * @param bottomColor
     * @return
     */
    virtual bool GetBackgroundColor(M3D::Color& topColor,M3D::Color& bottomColor);
    
    /**
     * 动画开启
     * @param file
     * @return
     */
	virtual bool AnimationOpen(const string& file);
	/**
	 * 继续
	 * @return
	 */
	virtual bool AnimationContinuousPlay();
	/**
	 * 播放
	 * @return
	 */
	virtual bool AnimationPlay();
	/**
	 * 重播
	 * @return
	 */
	virtual bool AnimationRewind();
	/**
	 * 设置
	 * @param percent
	 * @return
	 */
	virtual bool AnimationSetTick(float percent);
	virtual float AnimationGetTick();
	/**
	 * 动画是否在播放
	 * @return
	 */
	virtual bool AnimationIsPlaying();
	virtual bool AnimationExecute(float percentage);

	/**
	 *
	 * @return
	 */
	virtual bool AnimationTransitionIsPlaying();

	virtual bool AnimationPlayCamera(bool isPlayCamera);
	/**
	 * 获取当前动画播放时的相机
	 * @return
	 */
	virtual bool AnimationGetPlayCamera();
	/**
	 * 动画播放速度
	 * @param speed
	 * @return
	 */
	virtual bool AnimationPlaySpeed(float speed);

	virtual bool AnimationSetActiveStep(int processId,int stepId);

	/**
	 * 获取DataMananger版本号
	 * @return
	 */
	virtual string GetDataManagerVersion();

	/**
	 * 通过svl模型的路径获取shape
	 * @param path
	 * @return
	 */
	virtual Shape* GetShapeBySVLPath(const string& path);
    virtual Shape* GetShape(int shapeId);

    /**
     * 设置爆炸视图
     * @param stype
     * @param pos
     * @param useAnimation
     * @return true表示成功 false表示失败
     */
    virtual bool SetExplosiveView(int stype,int pos = 100,bool useAnimation = false);

    /**
     * 关闭爆炸图
     * @return true表示成功 false表示失败
     */
    virtual bool CloseExplisiveView();

    /**
     * 结束当前动画，包括持续旋转操作
     * 由于进行视口切换等操作时，
     * 不知道需要结束持续旋转的状态
     * 因此提供此接口
     */
    bool  CloseSceneAnimation();

private:
	static int newID;//!< ID
};

}

#endif /*M3D_I_VIEW_H_*/
