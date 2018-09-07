/**@file
 *@brief	
 *@author  CDJ
 *@date    2016-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *
 *
 */
#ifndef M3D_DRAGGERHANDLER_H_
#define M3D_DRAGGERHANDLER_H_

#include "sview/manipulator/TouchHandler.h"
namespace SVIEW
{
/**
 * 操作模式枚举
 */
enum OPEMODE
{
	TRANSLATE = 0,//!< TRANSLATE
	ROTATE = 1,   //!< ROTATE
	SCALE = 2,    //!< SCALE

};
/**
 * 坐标轴枚举
 */
enum TRANSAXIS
{
	AXIS_X = 0,  //!< AXIS_X
	AXIS_Y = 1,  //!< AXIS_Y
	AXIS_Z = 2,  //!< AXIS_Z
	AXIS_FACE = 3//!< AXIS_FACE
};

/**
 * @class
 * @brief 该类用于装配中对模型的一些列操作
 */
class M3D_API DraggerHandler: public TouchHandler
{
public:
	DraggerHandler();
	virtual ~DraggerHandler();
	/**
	 * 设置操作模式
	 * @param mode
	 */
    void SetMode(OPEMODE mode );
    /**
     * 设置坐标轴
     * @param axis
     */
    void SetAxis(TRANSAXIS axis);

	/**
	* 设置偏移量
	* @param fValue
	*/
	void SetMoveValue(float fValue);

	/**
	* 重新设置拖拽偏移信息
	* @param fValue
	*/
	void ResetDragInfo();

	virtual void ResetViewCamera();
private:
    /**
     * 初始化相机
     */
	virtual void InitCamera(bool useAni = true);
	virtual void InitModelViewCamera();
	/**
	 * 优化相机
	 */
	virtual void OptimizeCamera();
	virtual void OnTouchUp(float* p, int n);
	virtual void OnTouchMove(int moveType, float* p, int n);
	virtual void OnTouchDown(float* p, int n);
	virtual void OnUpDataTouchIntent();

	/**
	 * 获得选择的模型
	 * @return
	 */
	Model * GetSelectModel();

	vector<Model*> GetSelectModels();

	void SetIsChangerSceneBox(bool  state);
private:
	OPEMODE m_mode;//!< 操作模式
	TRANSAXIS m_axis;//!<坐标轴
	IntVector2 m_PriPointTwo1;//!<保存的上一次触摸操作时的屏幕点坐标
	bool m_bDragStarted; //拖拽是否开始
	map<Model*, Matrix3x4> m_mapSelectedModelInitWorldMatrix;//选中零件开始拖拽时的配置矩阵
	Vector3 m_moveAxis;//拖拽方向
	float m_fMoveValue;//拖拽量
	bool m_isChangeSceneBox;
	int m_coutChange;//辅助m_isChangeSceneBox，因为会两次调用touchmove,所以要加个计数

};

} /* namespace M3D */
#endif /* M3D_DRAGGERHANDLER_H_ */
