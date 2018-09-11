/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef	M3D_MEASURE_GROUP_H_
#define M3D_MEASURE_GROUP_H_

#include "m3d/M3D.h"
#include "m3d/scene/GroupNode.h"


namespace M3D
{
class CameraNode;
class SceneManager;

/**
 * @brief 测量组类，用来存放测量对象
 */
class M3D_API MeasureGroup: public GroupNode
{
public:
	MeasureGroup();
	virtual ~MeasureGroup();

	/**
	 * @brief 获取测量类型
	 * @return 返回测量类型
	 */
	virtual int GetType(void);
	/**
	 * @brief 初始化
	 */
	void Initialize();

	/**
	 * @brief 设置场景
	 * @param scene 场景指针
	 */
	void SetScene(SceneManager* scene);
	/**
	 * @brief 获取场景指针
	 * @return 场景指针
	 */
	SceneManager* GetScene();

private:
	SceneManager* m_scene;//!<当前场景指针

};
}
#endif /*M3D_MEASURE_GROUP_H_*/
