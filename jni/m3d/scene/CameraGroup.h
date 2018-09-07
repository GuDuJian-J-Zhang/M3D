/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef	M3D_CAMERA_GROUPNODE_H_
#define M3D_CAMERA_GROUPNODE_H_

#include "m3d/M3D.h"
#include "m3d/scene/GroupNode.h"


namespace M3D
{
class CameraNode;
class SceneManager;

class M3D_API CameraManager
{
public:
	CameraManager();
	~CameraManager();
	void AddCamera(CameraNode* camera);
	void RemoveCamera(CameraNode* camera);

	void SetSceneManager(SceneManager* sceneManager) { this->m_sceneManager = sceneManager; }
	SceneManager*GetSceneManager() { return this->m_sceneManager; }

	vector<CameraNode*>& GetAllCameras() { return m_sceneCameras; }

private:
	vector<CameraNode*> m_sceneCameras;
	SceneManager* m_sceneManager;
};

class M3D_API CameraGroup: public GroupNode
{
public:
	CameraGroup();
	virtual ~CameraGroup();

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
#endif /*M3D_CAMERA_EVENT_H_*/
