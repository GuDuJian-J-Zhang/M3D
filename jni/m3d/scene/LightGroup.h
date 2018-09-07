/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef	M3D_LIGHT_GROUP_H_
#define M3D_LIGHT_GROUP_H_

#include "m3d/M3D.h"
#include "m3d/scene/GroupNode.h"


namespace M3D
{
class CameraNode;
class SceneManager;
class BaseLight;

/**
 * @brief 测量组类，用来存放测量对象
 */
class M3D_API LightGroup: public GroupNode
{
public:
	LightGroup();
	virtual ~LightGroup();

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

	virtual void RayPick(RayPickAction * action);
	virtual void FindVisiableObject(RenderAction* renderAction);
	virtual void OnMarkedDirty();
private:
	SceneManager* m_scene;//!<当前场景指针

};
}
#endif /*M3D_MEASURE_GROUP_H_*/
