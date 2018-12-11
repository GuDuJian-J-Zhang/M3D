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
class TouchEvent;
class TranslateAxisDragger;
class Translate1DDragger;
class RotateCylinderAxisDragger;
class ScaleAxisDragger;
class BaseLight;
class CameraNode;
class TranslateMinusAxisDragger;
class TranslateBoxDragger;
class Translate3DDragger;
class RotateCylinderDragger;
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

	virtual void RayPick(RayPickAction * action);


	/**
	* 遍历所有的子节点
	* @param action
	*/
	virtual void Traverse(Action* action);

	void HandleDragger(TouchEvent& touchEvent);

	virtual void FindVisiableObject(RenderAction* renderAction);
    /**
     * 得到工具
     * @param toolsKey
     * @return
     */
    Model* GetSVLTool(const string& toolsKey);

	/**
	* 得到工具
	* @param toolsKey
	* @return
	*/
	Model* GetDraggerTip(const string& toolsKey);

    /**
     * 添加工具
     * @param model
     * @param toolsKey
     */
    bool AddSVLTool(Model* model, const string& toolsKey);

    /**
     * 删除工具
     * @param toolsKey
     */
    void RemoveSVLTool(const string& toolsKey);

	bool AddDraggerTip(Model* model, const string& toolsKey);

	/**
	* 删除工具
	* @param toolsKey
	*/
	void RemoveDraggerTip(const string& toolsKey);

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
	TranslateAxisDragger* GetTransformHandler();

	TranslateMinusAxisDragger* GetTransMinusformHandler();

	TranslateMinusAxisDragger* GetSingleTransMinusformHandler();

	RotateCylinderAxisDragger* GetRotateCylinderAxisDragger();
	ScaleAxisDragger* GetScaleAxisDragger();

	Translate3DDragger* GetTranslate3DHandle();
	Translate3DDragger* GetTranslate3DPntHandle();
	/**
	 *
	 */
    void Clear();

	M3D::TranslateBoxDragger* GetTranslateBoxDragger();
	M3D::RotateCylinderDragger* GetRotateCylinderDragger();

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

	void RemoveLightHandle();

	void RemoveSectionHandle();

	static void CacheNodeToMap(void* data, Model* node);

private:
	vector<HandlerPoint*> m_PointHandlers; //!<
	SceneManager* m_scene; //!<

	HandlerPoint* m_rotateCenterPoint; //!<
	TranslateMinusAxisDragger* m_TransMinusformHandlerNode;
	TranslateAxisDragger* m_TransformHandlerNode; //!<
	RotateCylinderAxisDragger* m_RotateCylinderAxisDragger;
	ScaleAxisDragger* m_ScaleAxisDragger;
	RotateCylinderDragger* m_RotateCylinderDragger;

	TranslateBoxDragger* m_translateBoxDragger;
	map<string,Model*> m_svlTools;
	map<string, Model*> m_draggerTips;


	float m_rotateCenterSize; //!<
	Vector3 m_rotateCenterPos; //!<
	list<TranslateMinusAxisDragger*> miusMinusAxisDraggerList;
	vector<Translate3DDragger*> m_TransLightHandleNodes;//光源拖拽器
};
}
#endif /*M3D_HANDLERGROUP_H_*/
