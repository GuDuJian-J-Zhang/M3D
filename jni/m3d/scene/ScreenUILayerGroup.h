
#ifndef	M3D_SCREENUILAYER_GROUP_H_
#define M3D_SCREENUILAYER_GROUP_H_


#include "m3d/scene/GroupNode.h"
#include "m3d/M3D.h"
#include "../handler/dragger.h"

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
class TranslateBoxDragger;

class M3D_API ImageModelDraggerCallback :public DraggerCallback
{
public:
	ImageModelDraggerCallback();
	virtual ~ImageModelDraggerCallback();
	/**
	* Receive motion commands. Returns true on success.
	*/
	virtual bool receive(const MotionCommand&);
	virtual bool receive(const TranslateInLineCommand& command);
	virtual bool receive(const TranslateInPlaneCommand& command);
 
	bool AddModel(Model* model);
	void AddModels(vector<Model*> models);
	void ClearModel();

	M3D::TranslateBoxDragger* GetBoxDragger() const { return m_boxDragger; }
	void SetBoxDragger(M3D::TranslateBoxDragger* val) { m_boxDragger = val; }
private:
	vector<Model*> m_draggerModels;

	TranslateBoxDragger* m_boxDragger;
};



class M3D_API ScreenUILayerGroup : public GroupNode
{
public:
	ScreenUILayerGroup(SceneManager* scene);

	virtual ~ScreenUILayerGroup();

	virtual int GetType(void);

	virtual void RayPick(RayPickAction * action);

	/**
	* 遍历所有的子节点
	* @param action
	*/
	virtual void Traverse(Action* action);

	virtual void FindVisiableObject(RenderAction* renderAction);
 
    bool AddHudModel(Model* model);

    void RemoveHudModel(Model* model);
	void RemoveHudModel(std::string strName);
	Model* GetHudModelByName(std::string strName);

	void BindBoxDragger(Model* model,int boxDraggerType);

	void UnBindBoxDragger();
private:
	SceneManager* m_scene; //!<

	vector<Model*> m_svlTools;

	TranslateBoxDragger* m_boxDragger;
};
}
#endif /*M3D_HANDLERGROUP_H_*/
