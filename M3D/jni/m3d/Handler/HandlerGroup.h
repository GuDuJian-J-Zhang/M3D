/**@file
 *@brief    TODO 存储交互层的所有Handler
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef	M3D_HANDLERGROUP_H_
#define M3D_HANDLERGROUP_H_


#include "m3d/scene/GroupNode.h"
#include "m3d/M3D.h"
namespace M3D
{
class HandlerPoint;
class SceneManager;
class TransformHandlerNode;

class M3D_API HandlerGroup: public GroupNode
{
public:
	HandlerGroup(SceneManager* scene);

	virtual ~HandlerGroup();

	virtual int GetType(void);

	void Initialize();
	/**
	 * 创建点handler
	 * @return
	 */
    HandlerPoint* CreatePointHandler(const Vector3& coordinate,float size,int drawType,
    SceneManager* scene);

    HandlerPoint* CreateRotateCenterHandler(const Vector3& coordinate,float size,
    	    SceneManager* scene);

    void RemoveRotateCenterHandler(HandlerPoint* rotateCenter,SceneManager* scene);

    /**
     * 得到工具
     * @param toolsKey
     * @return
     */
    Model* GetSVLTool(const string& toolsKey);

    /**
     * 添加工具
     * @param model
     * @param toolsKey
     */
    void AddSVLTool(Model* model, const string& toolsKey);

    /**
     * 删除工具
     * @param toolsKey
     */
    void RemoveSVLTool(const string& toolsKey);

	void HideAllSVLTools();

	void ShowSVLTool(const string& toolsKey);
	void HideSVLTool(const string& toolsKey);

    /**
     *
     * @param visible
     */
	void SetRotateCenterVisible(bool visible);
	/**
	 *
	 * @return
	 */
	bool GetRotateCenterVisible();

	/**
	 *
	 * @param pos
	 */
	void UpdateRotateCenterPos(const Vector3& pos);

	/**
	 *
	 * @param size
	 */
	void UpdateRoteateCenterSize(float size);

	/**
	 * 得到旋转中心点
	 * @return
	 */
	HandlerPoint* GetRotateCenter();
    

	/**
	 * 移动  拖拽器
	 * @return
	 */
	TransformHandlerNode* GetTransformHandler();

	/**
	 * 设置移动拖拽器的显隐状态
	 * @param visible
	 */
	void SetTransformHandlerVisible(bool visible);

	/**
	 *
	 */
    void Clear();

private:
    /**
     *
     * @param scene
     * @return
     */
	Vector2 GetHandlerSize(SceneManager* scene);

	/**
	 *
	 */
	void RemoveRotateCenter();

	void RemoveAllTools();

	static void CacheNodeToMap(void* data, SceneNode* node);

private:
	vector<HandlerPoint*> m_PointHandlers; //!<
	SceneManager* m_scene; //!<

	HandlerPoint* m_rotateCenterPoint; //!<

	TransformHandlerNode* m_TransformHandlerNode; //!<

	map<string,Model*> m_svlTools;

	float m_rotateCenterSize; //!<
	Vector3 m_rotateCenterPos; //!<
};
}
#endif /*M3D_HANDLERGROUP_H_*/
