#include "m3d/handler/HandlerGroup.h"

#include "m3d/graphics/cameranode.h"
#include "m3d/handler/HandlerPoint.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/SceneManager.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/model/Model.h"
#include "m3d/action/CallBackAction.h"
#include "m3d/handler/dragger.h"
#include "m3d/handler/Translate1DDragger.h"
#include "m3d/handler/Translate3DDragger.h"
#include "m3d/handler/TranslateAxisDragger.h"
#include "m3d/handler/TranslateMinusAxisDragger.h"
#include "RotateCylinderAxisDragger.h"
#include "ScaleAxisDragger.h"
#include "../action/RenderAction.h"
#include "TranslateBoxDragger.h"

namespace M3D
{
    
HandlerGroup::HandlerGroup(SceneManager* scene)
{
	this->m_scene = scene;

	this->Initialize();
}

HandlerGroup::~HandlerGroup()
{
	//不需要释放，已经挂载到父级节点中的对象，在场景节点释放时，会自动释放掉
//	if(m_rotateCenterPoint){
//		delete m_rotateCenterPoint;
//		m_rotateCenterPoint = NULL;
//	}
//
//	if(m_TransformHandlerNode){
//		delete m_TransformHandlerNode;
//		m_TransformHandlerNode = NULL;
//	}
	ReleaseMe(m_TransformHandlerNode);
	ReleaseMe(m_RotateCylinderAxisDragger);
	ReleaseMe(m_ScaleAxisDragger);
}

void HandlerGroup::Initialize()
{
	m_rotateCenterPoint = NULL;
    m_rotateCenterSize=1.0f;
	m_TransformHandlerNode = NULL;
	m_TransMinusformHandlerNode = NULL;
	m_RotateCylinderAxisDragger = NULL;
	m_ScaleAxisDragger = NULL;
	m_translateBoxDragger = NULL;
}

void HandlerGroup::Clear()
{
	ReleaseMe(m_TransformHandlerNode);
	ReleaseMe(m_RotateCylinderAxisDragger);
	ReleaseMe(m_ScaleAxisDragger);
	ReleaseMe(m_TransMinusformHandlerNode);
	ReleaseMe(m_translateBoxDragger);
	this->RemoveLightHandle();
	this->RemoveAllTools();
    this->RemoveRotateCenter();
    this->DeleteAllChildren();
}

int HandlerGroup::GetType( void )
{
	return HANDLER_GROUP_NODE;
}

HandlerPoint* HandlerGroup::CreatePointHandler(const Vector3& coordinate,float size,int drawType,
		SceneManager* scene)
{
	HandlerPoint* handlerPoint = NULL;
	//判断场景是否为空
	if (scene == NULL)
	{
		return handlerPoint;
	}
 
	handlerPoint = new HandlerPoint(coordinate,size);
    handlerPoint->SetDrawType(drawType);
	//创建一个节点，用来挂载创建的测量handlerPoint对象
	ShapeNode* node = new ShapeNode();
	node->SetShape(handlerPoint);

	//给新创建的Node设置一个名字
	char strID[10];
	sprintf(strID, "%d", node->GetID());
	node->SetName(this->GetName() + "|" + strID);

	//修改场景结构，加锁
	scene->Lock();

	//将创建的测量对象加入到测量组中
	this->AddChild(node);

	scene->AddShapeIDToMap(handlerPoint);

	//完成修改，解锁
	scene->UnLock();

	return handlerPoint;
}

HandlerPoint* HandlerGroup::CreateRotateCenterHandler(const Vector3& coordinate,float size,
	    SceneManager* scene){

	int drawType = 0;
	HandlerPoint* handlerPoint = NULL;
		//判断场景是否为空
		if (scene == NULL)
		{
			return handlerPoint;
		}

		handlerPoint = new HandlerPoint(coordinate,size);

	    handlerPoint->SetDrawType(drawType);
		//创建一个节点，用来挂载创建的测量handlerPoint对象
		ShapeNode* node = new ShapeNode();
		node->SetShape(handlerPoint);

		//修改场景结构，加锁
		scene->Lock();

		//将创建的测量对象加入到测量组中
		this->AddChild(node);

		//完成修改，解锁
		scene->UnLock();

		return handlerPoint;
}

void HandlerGroup::RemoveRotateCenterHandler(HandlerPoint* rotateCenter,
		SceneManager* scene)
{
	if (rotateCenter != NULL)
	{ //修改场景结构，加锁
		scene->Lock();

		GroupNode* group =
				(GroupNode*) rotateCenter->GetSceneNode()->GetParent();
		group->DeleteChild(rotateCenter->GetSceneNode());

		//完成修改，解锁
		scene->UnLock();
	}
}

void HandlerGroup::RayPick(RayPickAction * action)
{
	if (this->m_translateBoxDragger)
	{
		CameraNode* currentCamera = this->m_scene->GetCamera();

		CameraNode* hudCamera = this->m_scene->GetHudCamera();

		this->m_scene->SetHudCamera(hudCamera);

		this->m_translateBoxDragger->RayPick(action);
 
		this->m_scene->SetCamera(currentCamera);
	}

	GroupNode::RayPick(action);

	//遍历工具的显示
	if (!this->m_svlTools.empty())
	{
		map<string, Model*>::iterator it = this->m_svlTools.begin();
		while (it != this->m_svlTools.end())
		{
			Model* model = it->second;
			if (model->IsVisible())
			{
				model->RayPick(action);
			}
			it++;
		}
	}
}

void HandlerGroup::Traverse(Action* action)
{
	GroupNode::Traverse(action);
}

void HandlerGroup::HandleDragger(TouchEvent& touchEvent)
{
	if (!touchEvent.getHandled() && this->m_translateBoxDragger && m_translateBoxDragger->IsVisible())
	{
		CameraNode* currentCamera = this->m_scene->GetCamera();

		CameraNode* hudCamera = this->m_scene->GetHudCamera();

		this->m_scene->SetCamera(hudCamera);

		Dragger* dragger = m_translateBoxDragger;
		dragger->handle(touchEvent);

		this->m_scene->SetCamera(currentCamera);
	}
 
	if (!touchEvent.getHandled()&&this->m_TransformHandlerNode && m_TransformHandlerNode->IsVisible())
	{
		Dragger* dragger = m_TransformHandlerNode;
		dragger->handle(touchEvent);
	}
	if (!touchEvent.getHandled() && this->m_TransMinusformHandlerNode && m_TransMinusformHandlerNode->IsVisible())
	{
		Dragger* dragger = m_TransMinusformHandlerNode;
		dragger->handle(touchEvent);
	}

	list<TranslateMinusAxisDragger*>::iterator iter;
	for (iter = miusMinusAxisDraggerList.begin(); iter != miusMinusAxisDraggerList.end(); ++iter)
	{
		TranslateMinusAxisDragger* childDragger = *iter;
		if (!touchEvent.getHandled() && childDragger)// && childDragger->IsVisible())
		{
			if (!childDragger->handle(touchEvent))
			{
				Dragger* dragger = childDragger;
				dragger->handle(touchEvent);
			}
		}
	}

	for (size_t i = 0; i < m_TransLightHandleNodes.size(); i++)
	{
		Translate3DDragger* lightDragger = m_TransLightHandleNodes[i];
		if (lightDragger && !touchEvent.getHandled())
		{
			Dragger* dragger = lightDragger;
			dragger->handle(touchEvent);
		}
	}

	if (!touchEvent.getHandled()&&this->m_RotateCylinderAxisDragger && m_RotateCylinderAxisDragger->IsVisible())
	{
		Dragger* dragger = m_RotateCylinderAxisDragger;
		dragger->handle(touchEvent);
	}


	if (!touchEvent.getHandled() && this->m_ScaleAxisDragger && m_ScaleAxisDragger->IsVisible())
	{
		Dragger* dragger = m_ScaleAxisDragger;
		dragger->handle(touchEvent);
	}
}

void HandlerGroup::FindVisiableObject(RenderAction* renderAction)
{
	//处理拖拽器
	static float scaleFactor = 1.5f;
	if (m_TransformHandlerNode && m_TransformHandlerNode->IsVisible())
	{
		if (renderAction->GetSceneBoxChanged() || m_TransformHandlerNode->GetNeedScale())
		{
			Vector2 size = ShapeHelper::GetCommonSize(renderAction->GetScene(), Vector2(scaleFactor, scaleFactor));
			m_TransformHandlerNode->SetOrigScale(Vector3(size.m_x, size.m_x, size.m_x));
			m_TransformHandlerNode->SetNeedScale(false);
		}
		Vector3 position = m_TransformHandlerNode->GetWorldTransform()* Vector3(0,0,0);
		float scale = Billboard::GetFitShowScale(renderAction, position)/0.4f;
		Vector3 scaleVec3(scale, scale, scale);
		Vector3 OldscaleVec3 = m_TransformHandlerNode->GetOrigScale();
		OldscaleVec3 *= scaleVec3;
		m_TransformHandlerNode->SetScale(OldscaleVec3);
	}
	if (m_TransMinusformHandlerNode && m_TransMinusformHandlerNode->IsVisible())
	{
		if (renderAction->GetSceneBoxChanged() || m_TransMinusformHandlerNode->GetNeedScale())
		{
			Vector2 size = ShapeHelper::GetCommonSize(renderAction->GetScene(), Vector2(scaleFactor, scaleFactor));
			m_TransMinusformHandlerNode->SetOrigScale(Vector3(size.m_x, size.m_x, size.m_x));
			m_TransMinusformHandlerNode->SetNeedScale(false);
		}
		Vector3 position = m_TransMinusformHandlerNode->GetWorldTransform()* Vector3(0, 0, 0);
		float scale = Billboard::GetFitShowScale(renderAction, position) / 0.4f;
		Vector3 scaleVec3(scale, scale, scale);
		Vector3 OldscaleVec3 = m_TransMinusformHandlerNode->GetOrigScale();
		OldscaleVec3 *= scaleVec3;
		m_TransMinusformHandlerNode->SetScale(OldscaleVec3);
	}

	list<TranslateMinusAxisDragger*>::iterator iter;
	for (iter = miusMinusAxisDraggerList.begin(); iter != miusMinusAxisDraggerList.end(); ++iter)
	{
		TranslateMinusAxisDragger* dragger = *iter;
		if (dragger && dragger->IsVisible())
		{
			if (renderAction->GetSceneBoxChanged() || dragger->GetNeedScale())
			{
				Vector2 size = ShapeHelper::GetCommonSize(renderAction->GetScene(), Vector2(scaleFactor, scaleFactor));
				dragger->SetOrigScale(Vector3(size.m_x, size.m_x, size.m_x));
				dragger->SetNeedScale(false);
			}
			Vector3 position = dragger->GetWorldTransform()* Vector3(0, 0, 0);
			float scale = Billboard::GetFitShowScale(renderAction, position) / 0.4f;
			Vector3 scaleVec3(scale, scale, scale);
			Vector3 OldscaleVec3 = dragger->GetOrigScale();
			OldscaleVec3 *= scaleVec3;
			dragger->SetScale(OldscaleVec3);
		}
	}
	for (size_t i = 0; i < m_TransLightHandleNodes.size(); i++)
	{
		Translate3DDragger* dragger = m_TransLightHandleNodes[i];
		if (dragger && dragger->IsVisible())
		{
			if (renderAction->GetSceneBoxChanged() || dragger->GetNeedScale())
			{
				Vector2 size = ShapeHelper::GetCommonSize(renderAction->GetScene(), Vector2(scaleFactor, scaleFactor));
				size.m_x /= 2;
				dragger->SetOrigScale(Vector3(size.m_x, size.m_x, size.m_x));
				dragger->SetNeedScale(false);
			}
			Vector3 position = dragger->GetWorldTransform()* Vector3(0, 0, 0);
			float scale = Billboard::GetFitShowScale(renderAction, position) / 0.4f;
			Vector3 scaleVec3(scale, scale, scale);
			Vector3 OldscaleVec3 = dragger->GetOrigScale();
			OldscaleVec3 *= scaleVec3;
			dragger->SetScale(OldscaleVec3);
		}
	}

	if (m_RotateCylinderAxisDragger && m_RotateCylinderAxisDragger->IsVisible() )
	{
		if (renderAction->GetSceneBoxChanged() || m_RotateCylinderAxisDragger->GetNeedScale())
		{
			Vector2 size = ShapeHelper::GetCommonSize(renderAction->GetScene(), Vector2(scaleFactor, scaleFactor));
			m_RotateCylinderAxisDragger->SetOrigScale(Vector3(size.m_x, size.m_x, size.m_x));
			m_RotateCylinderAxisDragger->SetNeedScale(false);
		}

		Vector3 position = m_RotateCylinderAxisDragger->GetWorldTransform()* Vector3(0, 0, 0);
		float scale = Billboard::GetFitShowScale(renderAction, position) / 0.4f;
		Vector3 scaleVec3(scale, scale, scale);
		Vector3 OldscaleVec3 = m_RotateCylinderAxisDragger->GetOrigScale();
		OldscaleVec3 *= scaleVec3;
		m_RotateCylinderAxisDragger->SetScale(OldscaleVec3);
	}

	if (m_ScaleAxisDragger && m_ScaleAxisDragger->IsVisible())
	{
		if (renderAction->GetSceneBoxChanged() || m_ScaleAxisDragger->GetNeedScale())
		{
			Vector2 size = ShapeHelper::GetCommonSize(renderAction->GetScene(), Vector2(scaleFactor, scaleFactor));
			m_ScaleAxisDragger->SetOrigScale(Vector3(size.m_x,size.m_x,size.m_x));
			m_ScaleAxisDragger->SetNeedScale(false);
		}
		Vector3 position = m_ScaleAxisDragger->GetWorldTransform()* Vector3(0, 0, 0);
		float scale = Billboard::GetFitShowScale(renderAction, position) / 0.4f;
		Vector3 scaleVec3(scale, scale, scale);
		Vector3 OldscaleVec3 = m_ScaleAxisDragger->GetOrigScale();
		OldscaleVec3 *= scaleVec3;
		m_ScaleAxisDragger->SetScale(OldscaleVec3);
	}
	map<int, M3D::RenderQueue>* currentRenderQueue = renderAction->GetWorkingRenderQueueGroup();
	map<int, M3D::RenderQueue>* draggerRenderQueue = &renderAction->GetDraggerRenderQueueGroup();
	renderAction->SetWorkingRenderQueueGroup(draggerRenderQueue);

	GroupNode::FindVisiableObject(renderAction);

	//遍历DraggerTips
	if (!this->m_draggerTips.empty())
	{
		map<string, Model*>::iterator it = this->m_draggerTips.begin();
		while (it != this->m_draggerTips.end())
		{
			Model* model = it->second;
			if (model->IsVisible())
			{
				model->FindVisiableObject(renderAction);
			}
			it++;
		}
	}
	renderAction->SetWorkingRenderQueueGroup(currentRenderQueue);

	//遍历工具的显示
	if (!this->m_svlTools.empty())
	{
		map<string, Model*>::iterator it = this->m_svlTools.begin();
		while (it != this->m_svlTools.end())
		{
			Model* model = it->second;
			if (model->IsVisible())
			{
				model->FindVisiableObject(renderAction);
				//如果是固定屏幕显示的图片
			}
			it++;
		}
	}
	renderAction->SetHandlerGroupNode(this);


	//放到HudIamges组里面去显示
	int priRenderImageQueueIndex = renderAction->GetCurrentRenderImageQueueIndex();
	renderAction->SetCurrentRenderImageQueueIndex(2);
	if (this->m_translateBoxDragger && this->m_translateBoxDragger->IsVisible())
	{
		m_translateBoxDragger->FindVisiableObject(renderAction);
	}
	renderAction->SetCurrentRenderImageQueueIndex(priRenderImageQueueIndex);
}

Model* HandlerGroup::GetSVLTool(const string& toolsKey)
{
	Model* toolsModel = NULL;

	if(this->m_svlTools.find(toolsKey) != this->m_svlTools.end())
	{
		toolsModel = this->m_svlTools.find(toolsKey)->second;
	}

	return toolsModel;
}

Model* HandlerGroup::GetDraggerTip(const string& toolsKey)
{
	Model* toolsModel = NULL;

	if (this->m_draggerTips.find(toolsKey) != this->m_draggerTips.end())
	{
		toolsModel = this->m_draggerTips.find(toolsKey)->second;
	}

	return toolsModel;
}


bool  HandlerGroup::AddSVLTool(Model* model, const string& toolsKey)
{
	bool addState = false;
	map<string,Model*>::iterator it = this->m_svlTools.find(toolsKey);
	if(it == this->m_svlTools.end() && model)
	{
		this->m_svlTools.insert(pair<string,Model*>(toolsKey,model));
		model->AddRef();
		//加入场景中
		CallBackAction* cacheNameAction = new CallBackAction;
		cacheNameAction->SetActionFun(CacheNodeToMap);
		cacheNameAction->SetActionData(&this->m_scene->m_NodesMap);
		model->Traverse((Action*)cacheNameAction);
		delete cacheNameAction;

		m_scene->AsynUpdateModelCacheInfo(model, true);
		addState = true;
	}else
	{
		LOGI("此SVL%s工具已经存在",toolsKey.c_str());
	}
	return addState;
}

void HandlerGroup::RemoveSVLTool(const string& toolsKey)
{
	map<string,Model*>::iterator it = this->m_svlTools.find(toolsKey);
	if(it != this->m_svlTools.end())
	{
		Model* model = it->second;
		m_scene->AsynUpdateModelCacheInfo(model, false);
		model->Release();
		this->m_svlTools.erase(it);
	}
}


bool  HandlerGroup::AddDraggerTip(Model* model, const string& toolsKey)
{
	bool addState = false;
	map<string, Model*>::iterator it = this->m_draggerTips.find(toolsKey);
	if (it == this->m_draggerTips.end() && model)
	{
		this->m_draggerTips.insert(pair<string, Model*>(toolsKey, model));
		model->AddRef();
		//加入场景中	 
		addState = true;
	}
	return addState;
}

void HandlerGroup::RemoveDraggerTip(const string& toolsKey)
{
	map<string, Model*>::iterator it = this->m_draggerTips.find(toolsKey);
	if (it != this->m_draggerTips.end())
	{
		Model* model = it->second;
		model->Release();
		this->m_draggerTips.erase(it);
	}
}

void HandlerGroup::ShowSVLTool(const string& toolsKey)
{
	map<string, Model*>::iterator it = this->m_svlTools.find(toolsKey);
	if (it != this->m_svlTools.end())
	{
		Model* model = it->second;

		model->SetVisible(true);
	}
}

void HandlerGroup::HideAllSVLTools()
{
	map<string, Model*>::iterator it = this->m_svlTools.begin();

	while (it != this->m_svlTools.end())
	{
		Model* model = it->second;

		model->SetVisible(false);
		
		it++;
	}
}

void HandlerGroup::HideSVLTool(const string& toolsKey)
{
	map<string, Model*>::iterator it = this->m_svlTools.find(toolsKey);
	if (it != this->m_svlTools.end())
	{
		Model* model = it->second;

		model->SetVisible(false);
	}
}

void HandlerGroup::SetRotateCenterVisible(bool visible)
{
    MutexLock locked(this->m_mutex);
	if(m_rotateCenterPoint)
	{
        m_rotateCenterPoint->SetVisible(visible);
	}
}

bool HandlerGroup::GetRotateCenterVisible()
{
    MutexLock locked(this->m_mutex);
	bool visible = false;

	if(m_rotateCenterPoint)
	{
		visible = m_rotateCenterPoint->IsVisible();
	}

	return visible;
}

void HandlerGroup::UpdateRotateCenterPos(const Vector3& pos)
{
	if(	m_rotateCenterPos != pos)
	{
        MutexLock locked(this->m_mutex);
        m_rotateCenterPos = pos;

		this->RemoveRotateCenter();

		m_rotateCenterPoint = this->CreateRotateCenterHandler(m_rotateCenterPos,
					m_rotateCenterSize,m_scene);

		//LOGE("m_rotateCenterPoint is%d",m_rotateCenterPoint);
	}
}

void HandlerGroup::UpdateRoteateCenterSize(float size)
{
	m_rotateCenterSize = size;

	this->RemoveRotateCenter();

	m_rotateCenterPoint = this->CreatePointHandler(m_rotateCenterPos,
			m_rotateCenterSize, 2,m_scene);
}

HandlerPoint* HandlerGroup::GetRotateCenter()
{
	return m_rotateCenterPoint;
}

TranslateMinusAxisDragger* HandlerGroup::GetTransMinusformHandler()
{
	if (m_TransMinusformHandlerNode == NULL)
	{
		m_TransMinusformHandlerNode = new TranslateMinusAxisDragger();
		m_TransMinusformHandlerNode->setupDefaultGeometry();
		m_TransMinusformHandlerNode->AddRef();
		m_TransMinusformHandlerNode->SetScene(m_scene);
		this->AddChild(m_TransMinusformHandlerNode);
	}

	return m_TransMinusformHandlerNode;
}

M3D::TranslateBoxDragger* HandlerGroup::GetTranslateBoxDragger()
{
	if (m_translateBoxDragger == NULL)
	{
		m_translateBoxDragger = new TranslateBoxDragger();
		m_translateBoxDragger->setupDefaultGeometry();
		m_translateBoxDragger->AddRef();
		m_translateBoxDragger->SetScene(m_scene);
		//this->AddChild(m_translateBoxDragger);
	}

	return m_translateBoxDragger;
}


TranslateMinusAxisDragger * HandlerGroup::GetSingleTransMinusformHandler()
{
	if (m_TransMinusformHandlerNode == NULL)
	{
		m_TransMinusformHandlerNode = new TranslateMinusAxisDragger();
		m_TransMinusformHandlerNode->setupDefaultGeometry();
		m_TransMinusformHandlerNode->AddRef();
		m_TransMinusformHandlerNode->SetScene(m_scene);
		this->AddChild(m_TransMinusformHandlerNode);
	}
	TranslateMinusAxisDragger* transMinusformHandlerNode = m_TransMinusformHandlerNode;
	miusMinusAxisDraggerList.push_back(transMinusformHandlerNode);
	m_TransMinusformHandlerNode = nullptr;
	return transMinusformHandlerNode;
}


TranslateAxisDragger* HandlerGroup::GetTransformHandler()
{
	if (m_TransformHandlerNode == NULL)
	{
		m_TransformHandlerNode = new TranslateAxisDragger();
		m_TransformHandlerNode->setupDefaultGeometry();
		m_TransformHandlerNode->AddRef();
		m_TransformHandlerNode->SetScene(m_scene);
		this->AddChild(m_TransformHandlerNode);
	}

	return m_TransformHandlerNode;
}

M3D::RotateCylinderAxisDragger* HandlerGroup::GetRotateCylinderAxisDragger()
{
	if (m_RotateCylinderAxisDragger == NULL)
	{
		m_RotateCylinderAxisDragger = new RotateCylinderAxisDragger();
		m_RotateCylinderAxisDragger->SetScene(m_scene);

		m_RotateCylinderAxisDragger->setupDefaultGeometry();
		m_RotateCylinderAxisDragger->AddRef();

		this->AddChild(m_RotateCylinderAxisDragger);
	}

	return m_RotateCylinderAxisDragger;
}

M3D::ScaleAxisDragger* HandlerGroup::GetScaleAxisDragger()
{
	if (m_ScaleAxisDragger == NULL)
	{
		m_ScaleAxisDragger = new ScaleAxisDragger();
		m_ScaleAxisDragger->setupDefaultGeometry();
		m_ScaleAxisDragger->AddRef();
		m_ScaleAxisDragger->SetScene(m_scene);

		this->AddChild(m_ScaleAxisDragger);
	}

	return m_ScaleAxisDragger;
}

Translate3DDragger* HandlerGroup::GetTranslate3DHandle()
{
	Translate3DDragger* translate3DDragger = new Translate3DDragger();
	translate3DDragger->setupDefaultImageGeometry();
	translate3DDragger->AddRef();
	translate3DDragger->SetScene(m_scene);
	this->AddChild(translate3DDragger);
	m_TransLightHandleNodes.push_back(translate3DDragger);
	return translate3DDragger;
}

Translate3DDragger* HandlerGroup::GetTranslate3DPntHandle()
{
	Translate3DDragger* translate3DDragger = new Translate3DDragger();
	translate3DDragger->setupDefaultPntImageGeometry();
	translate3DDragger->AddRef();
	translate3DDragger->SetScene(m_scene);
	this->AddChild(translate3DDragger);
	m_TransLightHandleNodes.push_back(translate3DDragger);
	return translate3DDragger;
}

//TransformHandlerNode* HandlerGroup::GetTransformHandler()
//{
//	if(this->m_TransformHandlerNode == NULL)
//	{
//		this->m_TransformHandlerNode = new TransformHandlerNode();
//		this->AddChild(this->m_TransformHandlerNode);
//
//		this->m_TransformHandlerNode->SetName(TRANSFORM_HANDLER_NODE);
//	}
//
//	return this->m_TransformHandlerNode;
//}
//
//void HandlerGroup::SetTransformHandlerVisible(bool visible)
//{
//	if(this->m_TransformHandlerNode)
//	{
//		this->m_TransformHandlerNode->SetHide(!visible);
//	}
//}

void HandlerGroup::RemoveLightHandle()
{
	for (vector<Translate3DDragger*>::iterator it = m_TransLightHandleNodes.begin(); it != m_TransLightHandleNodes.end(); it++)
	{
		DeleteChild(*it);
		ReleaseMe(*it);
	}
	m_TransLightHandleNodes.clear();
}

void HandlerGroup::RemoveRotateCenter()
{
    MutexLock locked(this->m_mutex);
	if(m_rotateCenterPoint)
	{
		if(this->m_scene)
		{
			this->RemoveRotateCenterHandler(m_rotateCenterPoint,this->m_scene);
		}
		m_rotateCenterPoint = NULL;
	}
}

void HandlerGroup::RemoveAllTools()
{
	map<string,Model*>::iterator it = this->m_svlTools.begin();

	while(it != this->m_svlTools.end())
	{
		Model* model = it->second;
		ReleaseMe(model);
		//TOTO
		//if(model->GetSceneNode())
		//{
		//	ModelNode* modelNode = ShapeHelper::GetModelNode(model);
		//	if(modelNode)
		//	{
		//		((GroupNode*)(modelNode->GetParent()))->DeleteChild(modelNode);
		//	}
		//}
		it++;
	}

	this->m_svlTools.clear();
}

void HandlerGroup::CacheNodeToMap(void* data, Model* node)
{
	//	LOGI("SetNodeToMap  is  ok %s",node->GetName().c_str());
	map<string, Model*>* nodesMap = (map<string, Model*>*) data;
	map<string, Model*>::iterator it = nodesMap->find(node->GetName());
	if (it != nodesMap->end())
	{
		it->second = node;
	}
	else
	{
		nodesMap->insert(
			map<string, Model*>::value_type(node->GetName(), node));
	}
}

Vector2 HandlerGroup::GetHandlerSize(SceneManager* scene)
{
	float width = 1.0f;
	float heigh = 1.0f;

	if (scene)
	{
		CameraNode* camera = scene->GetCamera();
		float ratio = camera->GetAspectRatio();
		//获取当前ortho宽和高的较小值
		heigh = camera->GetOrthoSize();
		width = heigh * ratio;
		//屏幕宽度的20分之一
		width = width / 40;
	}

	return Vector2(width, width);
}

}
