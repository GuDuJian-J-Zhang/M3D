#include "m3d/SceneManager.h"

#include "m3d/ResourceManager.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/scene/LSceneNode.h"
#include "m3d/scene/LShapeNode.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/scene/AxisNode.h"
#include "m3d/scene/FPSNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/graphics/Light.h"
#include "m3d/scene/BackgroundNode.h"
#include "m3d/scene/ModelNode.h"


#include "m3d/model/Shape.h"
#include "m3d/model/Model.h"
#include "m3d/Handler/Handler.h"
#include "m3d/Handler/HandlerPoint.h"
#include "m3d/extras/note/NoteGroup.h"
#include "m3d/Handler/HandlerRotateCenter.h"

#include "m3d/renderer/gl10/GLDrawer.h"
#include "m3d/renderer/RenderContext.h"

#include "m3d/Handler/HandlerNodeRotateCenter.h"
#include "m3d/Handler/HandlerGroup.h"

#include "sview/views/Parameters.h"

#include "m3d/action/Action.h"
#include "m3d/action/CallBackAction.h"
#include "m3d/action/RayPickAction.h"
#include "sview/views/View.h"

#include "m3d/model/GeoAttribute.h"
#include "m3d/model/ModelView.h"
#include "m3d/graphics/CameraNode.h"

#include "m3d/graphics/SectionPlane.h"
#include "m3d/model/PMIData.h"
#include "m3d/graphics/Section.h"
#include "m3d/graphics/MovedMatrixInfo.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/base/Matrix4.h"

#include "m3d/utils/M3DTools.h"
#include "m3d/utils/AnimationCreator.h"
//#include "m3d/utils/MeasureTools.h"
#include "m3d/utils/CullerHelper.h"

#include "m3d/RenderManager.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/Edge.h"

#include "m3d/extras/measure/MeasureGroup.h"

#include "m3d/base/glue/GlueObj.h"

#include "m3d/base/FileCacheManager.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
using namespace SVIEW;

namespace M3D
{

	SceneManager::SceneManager(void)
	{
		m_pSceneRoot = NULL;
		this->m_RenderMgr = new RenderManager(this);
 
		Init();
	}

	SceneManager::~SceneManager(void)
	{
		if (m_pSceneRoot != NULL)
		{
			m_pSceneRoot->Release();
			m_pSceneRoot = NULL;
		}
 
		if (this->m_RenderMgr)
		{
			delete this->m_RenderMgr;
			this->m_RenderMgr = NULL;
		}

		if (this->m_section)
		{
			delete this->m_section;
			m_section = NULL;
		}

		int lightSize = lights.size();
		for (int i = 0;i<lightSize;i++)
		{
			if (lights[i])
			{
				delete lights[i];
				lights[i] = NULL;
			}
		}
		if (m_camera)
		{
			delete m_camera;
			m_camera = NULL;
		}
		if (m_glueObj)
		{
			delete m_glueObj;
			m_glueObj = NULL;
		}

//		if (this->m_ocTree)
//		{
//			delete this->m_ocTree;
//			this->m_ocTree = NULL;
//		}
	}

	SceneNode * SceneManager::GetSceneRoot(void)
	{
		return m_pSceneRoot;
	}

	void SceneManager::Init(void)
	{
		m_TopModel = NULL;
		this->m_pSceneRoot = new GroupNode;
		this->m_noteGroup = NULL;
		this->m_measureGroup = NULL;
		m_glueObj = new GlueObj();

		this->m_isModelDirty = false;
		m_section = NULL;

		bool isSpeedModel = Parameters::Instance()->m_simplityMode;
//		if (isSpeedModel)
//		{
//			this->m_ocTree = new Octree();
//		}
//		else
//		{
//			this->m_ocTree = NULL;
//		}

		m_resourceMgr = NULL;
	}

void SceneManager::RemoveModel()
{
	MutexLock locked(this->m_mutex);
	this->SetModel(NULL);
	this->ClearModelAndHandles();
	this->GetRenderManager()->ClearGLResource();
    this->GetRenderManager()->RequestRedraw();
}

	void SceneManager::OptimizeScene()
	{
		this->Reset();
		this->m_sceneBox.Clear();
		this->UpdataNodePathMap();
		this->ReIndexIDMapAfterAddModel(this->m_TopModel);

		this->m_RenderMgr->SceneChanged();

		this->m_isModelDirty = true;

		this->GetSceneBox();
	}

	void SceneManager::UpdateHardwareBuffer()
	{
	//文件缓存的方式，暂时屏蔽掉
		if(this->m_isModelDirty && false)
		{
			CFileCacheManager* fileCacheMgr = ResourceManager::Instance->GetFileCacheMgr();
			//创建缓存文件
			fileCacheMgr->bgeinCache();

			CallBackAction* resetAction = new CallBackAction;
			resetAction->SetActionLFun(UpdataHardWareBuffer);
			resetAction->SetActionData(this);
			this->ExecuteAction(resetAction);
			delete resetAction;

			fileCacheMgr->endWriteCache();

			fileCacheMgr->endMapping();

			fileCacheMgr->getFileMapping();

			this->m_isModelDirty = false;
		}
	}

	void SceneManager::FoucusView(BoundingBox& foucusBox, bool useAni)
	{
		////根据包围盒的大小，构建聚焦的动画
		CameraNode* camera = GetCamera();
		if (camera)
		{
			Vector3 center = foucusBox.Center();
			IntVector2 scrPnt = camera->GetViewPort().WorldToScreenPoint(center);
			IntVector2 scrCenter = camera->GetViewPort().GetRect().Center();

			//从屏幕上的一点移动到另一点
			Vector3 m_cacheCurPointNear = camera->GetView()* camera->GetViewPort().ScreenToWorldPoint(
				scrPnt.m_x, scrPnt.m_y, 0.5f);
			Vector3 m_cachePriPoint = camera->GetView()* camera->GetViewPort().ScreenToWorldPoint(
				scrCenter.m_x, scrCenter.m_y, 0.5f);

			//设置位置到合理值
			Vector3 moveVector = m_cacheCurPointNear - m_cachePriPoint;
			camera->Translate(moveVector);

			//设置缩放比例到合理值
			int screenHeight = this->m_RenderMgr->GetWindowHeight();
			int screenWidth = this->m_RenderMgr->GetWindowWidth();
			this->GetRenderManager()->WindowSizeChanged(screenWidth, screenHeight);
			float defaultZoom = Parameters::Instance()->m_DefaultZoomFactor;
			defaultZoom = defaultZoom * screenHeight / screenWidth;
			if (screenHeight > screenWidth)
			{
				defaultZoom = defaultZoom *0.5*(screenWidth*1.0 / screenHeight);
			}
			BoundingBox& sceneBox = GetSceneBox();
			camera->SetZoom(0.5f*defaultZoom*sceneBox.Length() / foucusBox.Length());
		}
	}

	void SceneManager::Reset()
	{
		CallBackAction* resetAction = new CallBackAction;
		resetAction->SetActionFun(ResetModelNodesCallback);
		resetAction->SetActionData(this);
		this->ExecuteAction(resetAction);
		delete resetAction;

		//this->ResetCamera();

		this->GetRenderManager()->RequestRedraw();
	}

	void SceneManager::ExecuteAction(Action* action)
	{
		MutexLock locked(this->m_mutex);
		if (action != NULL)
		{
			SceneNode *group = (SceneNode*)m_pSceneRoot->Search(MAINMODELROOT);
			if (action->GetApplyType() == Action::ONCE) //不需要遍历场景节点的action
			{
				action->ApplyOnce();
				return;
			}
			else ///需要遍历场景节点的Action
			{
				if (group != NULL)
				{
					group->Traverse(action);
				}
			}
		}
	}


	bool SceneManager::RayOctreeAction(RayPickAction* rayPickAction)
	{
		bool ret = false;

//		if (rayPickAction && this->m_ocTree)
//		{
//			Octree* octree = this->m_ocTree;
//			const Ray& pickRay = rayPickAction->GetData()->GetCameraRay();
//
//			vector<LModelShape*> fastResult;
//			RayOctreeQuery query(fastResult,pickRay, RayQueryLevel::RAY_AABB,0.001f);
//
//			octree->RaycastSingleFast(query);
//
//			for (int i = 0;i<fastResult.size();i++)
//			{
//				LModelShape* modelShape = fastResult.at(i);
//				if (modelShape)
//				{
//					modelShape->RayPick(rayPickAction);
//				}
//			}
//
//			ret = true;
//		}

		return ret;
	}

	bool SceneManager::FrameOctreeAction(RayPickAction* rayPickAction)
	{
		bool ret = false;

//		if (rayPickAction && this->m_ocTree)
//		{
//			Octree* octree = this->m_ocTree;
//			const Ray& pickRay = rayPickAction->GetData()->GetCameraRay();
//
//			const Frustum& pickFrustum = rayPickAction->GetData()->GetFramePickFrustum();
//
//			vector<LModelShape*> fastResult;
//			FrustumOctreeQuery query(fastResult, pickFrustum, 0, 0);
//			query.SetRenderAction(NULL);
//			octree->GetDrawables(query);
//
//			for (int i = 0; i < fastResult.size(); i++)
//			{
//				LModelShape* modelShape = fastResult.at(i);
//				if (modelShape)
//				{
//					modelShape->FramePick(rayPickAction);
//				}
//			}
//
//			ret = true;
//		}

		return ret;
	}

	bool SceneManager::FrustumOctreeAction(Action* action)
	{
		bool ret = false;

//		if (action && this->m_ocTree)
//		{
//			CameraNode* camera = this->GetCamera();
//			const Frustum& cameraFrustum = camera->GetFrustum();
//			vector<LModelShape*> fastResult;
//
//			FrustumOctreeQuery query(fastResult, cameraFrustum, 0, 0);
//			//query.SetRenderAction(renderAction);
//			//query.SetCamera(camera);
//
//			this->m_ocTree->GetDrawables(query);
//
//			//立即显示的部分
//			for (int i = 0; i <fastResult.size(); ++i)
//			{
//				LModelShape* modelShape = fastResult.at(i);
//				//modelShape->t
//			}
//		}

		return ret;
	}

	float SceneManager::GetDefaultZoom()
	{
		SceneManager* scene = this;
		CameraNode* camera = scene->GetCamera();

		Viewport viewport = camera->GetViewPort();
		int screenHeight = viewport.GetRect().Height();
		int screenWidth = viewport.GetRect().Width();
		float defaultZoom = Parameters::Instance()->m_DefaultZoomFactor;

		defaultZoom = defaultZoom * screenHeight / screenWidth;

		if (screenHeight > screenWidth)
		{
			defaultZoom = defaultZoom *0.5*(screenWidth*1.0 / screenHeight);
		}

		return defaultZoom;
	}

	float SceneManager::GetDefaultFocusLength()
	{
		BoundingBox& pBoundingBox = this->GetSceneBox();

		return pBoundingBox.Length()*1.5f;
	}

	void SceneManager::UpdateScene()
	{
		//在每一帧中释放需要释放的GL资源
		this->GetResourceManager()->ReleaseGLObjects();

		this->UpdateHardwareBuffer();

//		if (this->m_ocTree)
//		{
//			this->m_ocTree->Update();
//		}
	}

	void SceneManager::SetRenderManager(RenderManager * renderMgr)
	{
		this->m_RenderMgr = renderMgr;
	}

	void SceneManager::setSectionEnable(bool flag)
	{
		//剖切关闭或开启，可能涉及到剖面数据的添加或者删除，应该开启资源同步锁
		MutexLock locked(this->m_mutex);

		//	IModel* curModel = GetModel();
		Section * sectionNode = GetSection();
		//	sectionNode->SetModel(curModel);
		sectionNode->SetVisible(flag);

		//如果是停止剖视，则复位所有剖视图为不启用
		//	if (!flag)
		//	{
		//		for (int i = 0; i < curModel->GetSectionPlaneList().size(); i++)
		//		{
		//			Plane* curPlane = curModel->GetSectionPlaneList().at(i);
		//			curPlane->SetEnable(false);
		//		}
		//		sectionNode->SetModel(NULL);
		//	}
		//	LOGI("SceneManager::setSectionEnable = %d", flag);
	}

	void SceneManager::SetSections(const vector<SectionPlane*> planes)
	{
		MutexLock locked(this->m_mutex);

		if(planes.size()>0)
		{
			Section * section = GetSection();
			section->AddPlane(planes[0]);
		}
	}

	Section * SceneManager::GetSection()
	{
		//从场景树中获得剖面节点
		if (m_section == NULL)
		{
			m_section = new Section;
		}

		///给剖面设置包围盒，如果没设置则在查找显示数据时，会被剔除掉
		m_section->SetBox(this->GetSceneBox());
		BoundingBox tempBox = this->GetSceneBox();
		tempBox.ScaleBox(1.3f);
		m_section->SetDrawRection(tempBox);

		return m_section;
	}

	CameraNode* SceneManager::GetCamera()
	{
		return this->m_camera;
	}

	void SceneManager::SetCamera(CameraNode* camera)
	{
		MutexLock locked(this->m_mutex);
		this->m_camera = camera;
	}

	void SceneManager::AddLight(Light* light)
	{
		MutexLock locked(this->m_mutex);
		if (light != NULL)
		{
			this->lights.push_back(light);
		}
	}

	void SceneManager::RemoveLight(int index)
	{
		MutexLock locked(this->m_mutex);
		LightList::iterator it = this->lights.begin();
		if (index >= 0 && index < this->lights.size())
		{
			Light* light = this->lights[index];
			delete light;

			this->lights.erase(it += index);
		}
	}

	LightList* SceneManager::GetLights()
	{
		return &this->lights;
	}

	Light* SceneManager::GetLight(int index)
	{
		Light* light = NULL;
		if (index >= 0 && index < this->lights.size())
		{
			light = this->lights[index];
		}

		return light;
	}

	Model* SceneManager::GetModel()
	{
		return this->m_TopModel;
	}

	void SceneManager::AddModel(Model* model)
	{
		if(!model)
		{
			return;
		}
 
	}

	void SceneManager::SetModel(Model* model)
	{
		if(this->m_TopModel == model)
		{
			return;
		}

		MutexLock locked(this->m_mutex);

		this->m_TopModel = model;
		if(m_TopModel)
		{
			//TODO 根据模型大小自适应的修改优化算法。
			if(Parameters::Instance()->m_OptimizeBigModel)
			{
				Matrix4 topMatrix = m_TopModel->GetPlaceMatrix();
				topMatrix.SetScale(0.001);//缩小1000倍
				m_TopModel->SetPlaceMatrix(topMatrix);
			}

			if(Parameters::Instance()->m_simplityMode)
			{
//				LSceneNode* modelGroup = CreateLSceneNodes(m_TopModel, M3D::MAINMODELROOT, 1);
//				LGroupNode* modelGroupParent = new LGroupNode();
//				modelGroupParent->SetName(M3D::MAINMODELROOT);
//				GroupNode * mainGroup =(GroupNode *)GetSceneRoot()->Search(M3D::MAINGROUP);
//				modelGroupParent->AddChild(modelGroup);
//				mainGroup->AddChild(modelGroupParent);
//
//				this->SetModel(NULL);
			}else
			{
				GroupNode* modelGroup = CreateModelNodes(m_TopModel, M3D::MAINMODELROOT, 1);
				GroupNode* modelGroupParent = new GroupNode();
				modelGroupParent->SetName(M3D::MAINMODELROOT);
				GroupNode * mainGroup =(GroupNode *)GetSceneRoot()->Search(M3D::MAINGROUP);
				modelGroupParent->AddChild(modelGroup);
				mainGroup->AddChild(modelGroupParent);
			}

			//this->OptimizeScene();
			
			this->m_RenderMgr->RequestRedraw();
		}

	}

	HandlerGroup* SceneManager::GetHandlerGroup()
	{
		SceneNode* node = this->m_pSceneRoot->Search(HANDLER_GROUPNODE);
		return (HandlerGroup*) node;
	}

	NoteGroup* SceneManager::GetNoteGroup()
	{
		if(!this->m_noteGroup)
		{
			GroupNode * groupNode = (GroupNode*) this->GetSceneRoot();
			NoteGroup* noteGroup = new NoteGroup();

			noteGroup->SetName(M3D::NOTE_GROUP_PATH);
			groupNode->AddChild(noteGroup);

			this->m_noteGroup = noteGroup;
		}

		return this->m_noteGroup;
	}

	MeasureGroup* SceneManager::GetMeasureGroup()
	{
		if(!this->m_measureGroup)
		{
			GroupNode * groupNode = (GroupNode*) this->GetSceneRoot();

			MeasureGroup* measureGroup = new MeasureGroup();
			measureGroup->SetScene(this);
			measureGroup->SetName(M3D::MEASURE_GROUP_PATH);
			groupNode->AddChild(measureGroup);

			this->m_measureGroup = measureGroup;
		}

		return this->m_measureGroup;
	}

	bool SceneManager::RestoreRotationCenter()
	{
		BoundingBox& pBoundingBox = this->GetSceneBox();
		SetRotationCenter(pBoundingBox.Center());
		return false;
	}

	bool SceneManager::SetRotationCenter(float x, float y)
	{
		static int baseZoom = 3.5f;

		Vector3 intersectPnt;
		Vector2 screenPnt(x, y);

		if (this->GetCamera()->GetZoom() > baseZoom ||
			(this->GetCamera()->GetWorldPosition() - this->GetSceneBox().Center()).Length()
			< this->GetSceneBox().Length()/2)
		{
			if (!GetPickPoint(screenPnt, intersectPnt, true))
			{
				intersectPnt = this->GetSceneBox().Center();
			}
		}
		else
		{
			intersectPnt = this->GetSceneBox().Center();
		}

		SetRotationCenter(intersectPnt);

		return true;
	}

	bool SceneManager::SetRotationCenter(float x, float y, float z)
	{
		return false;
	}

	bool SceneManager::SetRotationCenter(const Vector3& vec)
	{
		CameraNode* camera = this->m_camera;
		if (camera != NULL)
		{
			camera->SetRotateCenter(vec);
			return true;
		}
		this->GetRenderManager()->RequestRedraw();

		return false;
	}

	IShape* SceneManager::GetPickShape(float winx, float winY, int shapeType,
		int geoType)
	{
		RayPickAction* rayPickAction = new RayPickAction(this);
		rayPickAction->SetPickShapeType(shapeType);
		rayPickAction->SetPickGeoType(geoType);

		rayPickAction->SetRay((int) winx, (int)(winY));

		//LOGI("setpoint is ok");
		SceneNode* group = this->m_pSceneRoot;
		group->Traverse((Action*) rayPickAction);
		IShape* shape = rayPickAction->GetNearPickShape();

		delete rayPickAction;

		this->GetRenderManager()->RequestRedraw();

		return shape;
	}

vector<IShape*> SceneManager::GetFramePickShape(const Vector2& leftTop, const Vector2& rightBottom,
		int shapeType,
		int geoType, int framePickType)
	{
		RayPickAction* rayPickAction = new RayPickAction(this);
		rayPickAction->SetPickShapeType(shapeType);
		rayPickAction->SetPickGeoType(geoType);
		rayPickAction->SetFramePickSection(leftTop,rightBottom);
		rayPickAction->SetFramePickType(framePickType);

		if (!FrameOctreeAction(rayPickAction))
		{
			//LOGI("setpoint is ok");
			SceneNode* group = this->m_pSceneRoot;
			group->Traverse((Action*)rayPickAction);
		}

		vector<IShape*> framePickShapes = rayPickAction->GetFramePickShapes();

		delete rayPickAction;

		this->GetRenderManager()->RequestRedraw();

		return framePickShapes;
	}

	IShape* SceneManager::GetPickShape(Vector2& screentPnt, int shapeType, int geoType)
	{
		RayPickAction* rayPickAction = new RayPickAction(this);
		rayPickAction->SetPickShapeType(shapeType);
		rayPickAction->SetPickGeoType(geoType);

		rayPickAction->SetRay(screentPnt);

		//LOGI("setpoint is ok");
		SceneNode* group = this->m_pSceneRoot;
		group->Traverse((Action*) rayPickAction);
		IShape* shape = rayPickAction->GetNearPickShape();

	delete rayPickAction;
    
    this->GetRenderManager()->RequestRedraw();

	return shape;
}

	// bool SceneManager::SetRotationCenter(const Vector3& vec, SceneNode* node)
	// {
	// 	node->SetRotateCenter(vec);
	// 	return false;
	// 
	// }
	bool SceneManager::RestoreRotationCenter(SceneNode* node)
	{
		return false;
	}

	void SceneManager::SetSelectType(int type)
	{
		m_iSelectType = type;
	}

	SceneNode* SceneManager::GetNodes(int type, const string& name)
	{
		map<string, SceneNode*>::iterator it = this->m_NodesMap.find(name);
		if (it != this->m_NodesMap.end())
		{
			SceneNode* tempNode = it->second;
			while (tempNode != NULL)
			{
				if (tempNode->GetType() == type)
				{
					return tempNode;
				}
				tempNode = tempNode->GetParent();
			}
		}
		return NULL;
	}
	//
	void SceneManager::SetMatrixByPlcPath(char* plcPath, float matrix[4][4])
	{
		string nodePath = "M3D|MAIN|" + string(plcPath);
		SceneNode* node = this->GetNode(nodePath);
		if (node != NULL)
		{
			Matrix3x4 plcMatrix((float*)matrix);
			node->SetPlcMatrix(plcMatrix);
			//		if(node->GetName() == string("M3D|MAIN|PATH|0|4d"))
			//		{
			//	LOGE("node : %s plcMatrix: %s",node->GetName().c_str(),plcMatrix.ToString().c_str());
			//		}
		}
		else
		{
			//		 LOGI("SceneManager::SetMatrixByPlcPath node %s not found",nodePath.c_str());
		}
	}
	void SceneManager::GetMatrixByPlcPath(char* plcPath, float matrix[4][4])
	{
		string nodePath = "M3D|MAIN|" + string(plcPath);

		SceneNode* node = this->GetNode(nodePath);
		if (node != NULL)
		{
			Matrix3x4& plcMatrix = node->GetPlcMatrix();
			memcpy(matrix, Matrix4::IDENTITY.Data(), 16 * sizeof(float));
			memcpy(matrix, plcMatrix.Data(), 12 * sizeof(float));
			//		LOGI("SceneManager::GetMatrixByPlcPath placePath:%s",plcPath);
			//		LogFloatArray((float*)matrix,16);
		}
		else
		{
			//		LOGI("SceneManager::GetMatrixByPlcPath node %s not found",nodePath.c_str());
		}

	}

	int SceneManager::AddHandle(float x, float y, int type)
	{
		MutexLock locked(this->m_mutex);
		Vector3 handlePos = GetPickPoint(x, y);
		Shape* handle = CreateShape(handlePos, type);

		this->GetRenderManager()->RequestRedraw();

		return handle->GetID();
	}

	int SceneManager::AddHandle(float x, float y, float z, int type)
	{
		MutexLock locked(this->m_mutex);
		Vector3 pos(x, y, z);
		Shape* handle = CreateShape(pos, type);

		this->GetRenderManager()->RequestRedraw();

		return handle->GetID();
	}

	Vector3 SceneManager::GetPickPoint(float x, float y)
	{
		Vector3 vec;
		RayPickAction* rayPickAction = new RayPickAction(this);
		rayPickAction->SetPickShapeType(SHAPE_MODEL);
		rayPickAction->SetPickGeoType(M3D_GEOATTR_TYPE_UNKNOWN);
		rayPickAction->SetRay(x, y);
		this->m_pSceneRoot->Traverse((Action*)rayPickAction);
		bool state = rayPickAction->GetNearPickPoint(vec);
		if (!state)
		{
			Matrix3x4 viewMatrix = rayPickAction->GetCamera()->GetView();

			const Ray& pickRay = rayPickAction->GetData()->GetCameraRay();
			Vector3 tCenter;
			BoundingBox& pBoundingBox = this->GetSceneBox();
			tCenter = pBoundingBox.Center();

			//求射线与（包围盒中心点深度和摄像机方向所确定）平面交点
			float s = pickRay.m_direction.LengthSquared();
			float k = (tCenter - pickRay.m_origin).DotProduct((pickRay.m_direction)) * (1 / s);
			vec = (pickRay.m_direction) * k + pickRay.m_origin;
		}
		delete rayPickAction;

		this->GetRenderManager()->RequestRedraw();

		rayPickAction = NULL;
		return vec;
	}

	bool SceneManager::GetPickPoint(Vector2& screenPnt, Vector3& pnt, bool inModel)
	{
		bool ret = false;
		Vector3 vec;
		RayPickAction* rayPickAction = new RayPickAction(this);
		rayPickAction->SetPickShapeType(SHAPE_MODEL);
		rayPickAction->SetPickGeoType(M3D_GEOATTR_TYPE_UNKNOWN);
		rayPickAction->SetRay(screenPnt);

		if (!RayOctreeAction(rayPickAction))
		{
			//LOGI("setpoint is ok");
			SceneNode* group = this->m_pSceneRoot;
			group->Traverse((Action*)rayPickAction);
		}
		//this->m_pSceneRoot->Traverse((Action*)rayPickAction);
		bool state = rayPickAction->GetNearPickPoint(vec);
		if (!state && !inModel)
		{
			Matrix3x4 viewMatrix = rayPickAction->GetCamera()->GetView();

			const Ray& pickRay = rayPickAction->GetData()->GetCameraRay();
			Vector3 tCenter;
			BoundingBox& pBoundingBox = this->GetSceneBox();
			tCenter = pBoundingBox.Center();

			//求射线与（包围盒中心点深度和摄像机方向所确定）平面交点
			float s = pickRay.m_direction.LengthSquared();
			float k = (tCenter - pickRay.m_origin).DotProduct((pickRay.m_direction)) * (1 / s);
			vec = (pickRay.m_direction) * k + pickRay.m_origin;

			ret = true;
		}
		else if (state)
		{
			ret = true;
		}

		delete rayPickAction;
		rayPickAction = NULL;

		this->GetRenderManager()->RequestRedraw();

		pnt = vec;

		return ret;
	}

	Handler* SceneManager::CreateHandler(const Vector3& vec, int Type)
	{
		return (Handler*) this->CreateShape(vec, Type);
	}

	void SceneManager::RemoveHandle(int id)
	{
		MutexLock locked(this->m_mutex);
		this->RemoveShape(id);

		this->GetRenderManager()->RequestRedraw();

	}

	int SceneManager::AddShape(float x, float y, int type)
	{
		MutexLock locked(this->m_mutex);
		Vector3 handlePos = GetPickPoint(x, y);
		Shape* shape = CreateShape(handlePos, type);
		if (!shape)
		{
			return -1;
		}
		return shape->GetID();
	}

	int SceneManager::AddShape(float x, float y, float z, int type)
	{
		MutexLock locked(this->m_mutex);
		Vector3 pos(x, y, z);
		Shape* shape = CreateShape(pos, type);
		if (!shape)
		{
			return -1;
		}
		return shape->GetID();
	}

	Shape* SceneManager::CreateShape(const Vector3& position, int Type)
	{
		MutexLock locked(this->m_mutex);

		Shape* shape = NULL;

		switch (Type)
		{
		case SHAPE_POINT_HANDLE:
		{
			break;
		}

		case SHAPE_VOICE_NOTE:
		{
			//		shape = this->GetNoteGroup()->CreateNote(position,Type);
			break;
		}

		case SHAPE_NOTE:
		{
			break;


		}
		default:
			break;
		}

		if (!shape)
		{
			return NULL;
		}

		this->m_ShapesIDMap.insert(
			map<int, Shape*>::value_type(shape->GetID(), shape));

		//	this->m_NewsShapesIDMap.insert(
		//			map<int, Shape*>::value_type(shape->GetID(), shape));

		return shape;
	}

	bool SceneManager::RemoveShape(int id)
	{
		MutexLock locked(this->m_mutex);

    map<int, Shape*>::iterator it = this->m_ShapesIDMap.find(
    		id);
		if (it != this->m_ShapesIDMap.end())
		{
			Shape* shape = it->second;
			if (shape != NULL)
			{
				GroupNode* group = (GroupNode*)shape->GetSceneNode()->GetParent();
				group->DeleteChild(shape->GetSceneNode());
//				shape->Release();
//				delete shape;
			}
			this->m_ShapesIDMap.erase(it);
		}

//		it = this->m_NewsShapesIDMap.find(id);
//		if (it != this->m_NewsShapesIDMap.end())
//		{
//			this->m_NewsShapesIDMap.erase(it);
//		}
    this->GetRenderManager()->RequestRedraw();

	return true;
}

void SceneManager::GetShapePos(int id, int type, Vector3& pos)
{
	Shape* shape = this->GetShape(id);
	if (shape != NULL)
	{
		//0获得shape的实际位置,1获得对应node的位置
		if (type == 0)
		{
//			pos = shape->GetCenterPos();
		}
		else if (type == 1)
		{
			SceneNode* node = shape->GetSceneNode();
			if (node != NULL)
			{
				pos = node->GetPosition();
			}
		}
	}
}


Color SceneManager::GetShapeColor(int id, int type)
{
	Color color;
	Shape* shape = this->GetShape(id);
	if (shape != NULL)
	{
		color = shape->GetColor(); //or drawColor?
	}
	else
	{
		LOGE("SceneManager::GetShapeColor error: shape is null!");
	}
//	LOGI("SceneManager::GetShapeColor id:%d type:%d %f,%f,%f,%f"
//			,id,type,color.r_,color.G,color.B,color.A);

	return color;
}

	//int SceneManager::GetShapeID(const string& path)
	//{
	//	SceneNode* node = this->GetNode(path);
	//
	//	int retID = -1;
	//
	//	if (node == NULL)
	//	{
	//		return retID;
	//	}
	//
	//	if (node->GetType() == GROUP_NODE)
	//	{
	//		GroupNode* group = (GroupNode*) node;
	//
	//		SceneNode* childNode; //= group->GetChildrenNode();
	//
	//		for (int i = 0; i < group->Size(); i++)
	//		{
	//			childNode = group->GetChild(i);
	//
	//			if (childNode->GetType() == SHAPE_NODE)
	//			{
	//				retID = childNode->GetRelevantID();
	//				break;
	//			}
	//		}
	//
	//	}
	//	else if (node->GetType() == SHAPE_NODE)
	//	{
	//
	//	}
	//	else
	//	{
	//
	//	}
	//
	//	return retID;
	//
	//}
		//获取整个场景的包围盒
	BoundingBox& SceneManager::GetSceneBox()
	{
		if (!m_sceneBox.Defined())
		{
			CallBackAction* computeBoxAction = new CallBackAction();
			computeBoxAction->SetActionData(this);
			computeBoxAction->SetActionFun(SceneManager::ComputeBox);

			computeBoxAction->SetActionLFun(SceneManager::ComputeLBox);
			this->ExecuteAction(computeBoxAction);
			delete computeBoxAction;
		}
		return m_sceneBox;
	}

Shape* SceneManager::GetShape(int shapeID)
{
	map<int, Shape*>::iterator it = m_ShapesIDMap.find(shapeID);
	if (it != m_ShapesIDMap.end())
	{
		return it->second;
	}
	return NULL;
}

	SceneNode* SceneManager::GetNode(const string& name)
	{
		map<string, SceneNode*>::iterator it = this->m_NodesMap.find(name);
		if (it != this->m_NodesMap.end())
		{
			return it->second;
		}
		return NULL;
	}

void SceneManager::ReIndexIDMapAfterAddModel(Model* shape)
{
	if (shape != NULL)
	{
		if (shape->GetType() == SHAPE_MODEL)
		{
			Model* model = (Model*) shape;

			AddShapeIDToMap(model);
			vector<Body*>& bodys = model->GetBodys();

			for(int i=0;i<bodys.size();i++)
			{
				Body* body = bodys[i];
				AddShapeIDToMap(body);
				vector<Face*>& faces = body->GetFaces();
				for(int j=0;j<faces.size();j++)
				{
					Face* face = faces[j];
					AddShapeIDToMap(face);
					vector<Edge*>& edges = face->GetEdges();

					for(int k=0;k<edges.size();k++)
					{
						Edge* edge = edges[k];
						AddShapeIDToMap(edge);
					}
				}

				vector<Edge*>& edges = body->GetEdges();

				for(int k=0;k<edges.size();k++)
				{
					Edge* edge = edges[k];
					AddShapeIDToMap(edge);
				}

				vector<Point*>& points = body->GetPoints();
				for(int k = 0;k<points.size();k++)
				{
					Point* point = points[k];
					AddShapeIDToMap(point);
				}
			}

		    vector<Model*>& subModels = model->GetSubModels();
			for(int i=0;i<subModels.size();i++)
			{
				this->ReIndexIDMapAfterAddModel(subModels[i]);
			}
		}
        
        this->GetRenderManager()->RequestRedraw();
	}
}

void SceneManager::ReIndexIDMapAfterDeleteModel(Model* shape)
{
	if (shape != NULL)
	{
		if (shape->GetType() == SHAPE_MODEL)
		{
			Model* model = (Model*) shape;

			RemoveShapeIDFromMap(model);
			vector<Body*>& bodys = model->GetBodys();

			for(int i=0;i<bodys.size();i++)
			{
				Body* body = bodys[i];

				RemoveShapeIDFromMap(body);
				vector<Face*>& faces = body->GetFaces();
				for(int j=0;j<faces.size();j++)
				{
					Face* face = faces[j];

					RemoveShapeIDFromMap(face);
					vector<Edge*>& edges = face->GetEdges();

					for(int k=0;k<edges.size();k++)
					{
						Edge* edge = edges[k];

						RemoveShapeIDFromMap(edge);
					}
				}

				vector<Edge*>& edges = body->GetEdges();

				for(int k=0;k<edges.size();k++)
				{
					Edge* edge = edges[k];

					RemoveShapeIDFromMap(edge);
				}

				vector<Point*>& points = body->GetPoints();
				for(int k = 0;k<points.size();k++)
				{
					Point* point = points[k];

					RemoveShapeIDFromMap(point);
				}
			}

		    vector<Model*>& subModels = model->GetSubModels();
			for(int i=0;i<subModels.size();i++)
			{
				this->ReIndexIDMapAfterAddModel(subModels[i]);
			}
            shape->Release();
		}
	}
}


ResourceManager* SceneManager::GetResourceManager()
{
	if(!this->m_resourceMgr)
	{
		this->m_resourceMgr  = new ResourceManager();
	}

	return this->m_resourceMgr;
	//return ResourceManager::GetInstance();
}

void SceneManager::RemoveShapeIDFromMap(Shape* shape)
{
	if (shape)
	{
		m_ShapesIDMap.erase(shape->GetID());
	}
}

void SceneManager::AddShapeIDToMap(Shape* shape)
{
	if (shape)
	{
		m_ShapesIDMap.insert(
				map<int, Shape*>::value_type(shape->GetID(), shape));
	}
}

void SceneManager::SetGluObj(GlueObj* glueObj)
{
	this->m_glueObj = glueObj;
}

GlueObj* SceneManager::GetGlueObj()
{
	return this->m_glueObj;
}

void SceneManager::ComputeBox(void* data, SceneNode* node)
{
	if(node!= NULL && node->GetType() == SHAPE_NODE)
	{
		SceneManager* scene =(SceneManager*)data;
		BoundingBox& sceneBox = scene->m_sceneBox;
		BoundingBox& shapeNodeBox = node->GetWorldBoundingBox();
		if(shapeNodeBox.Defined())
		{
			sceneBox.Merge(shapeNodeBox);
		}
	}
}

	void SceneManager::ComputeLBox(void* data, LSceneNode* node)
	{
		if(node!= NULL && node->GetType() == LSHAPE_NODE)
		{
			SceneManager* scene =(SceneManager*)data;
			BoundingBox& sceneBox = scene->m_sceneBox;
			BoundingBox& shapeNodeBox = node->GetWorldBoundingBox();
			if(shapeNodeBox.Defined())
			{
				sceneBox.Merge(shapeNodeBox);
			}
		}
	}

	void SceneManager::UpdataHardWareBuffer(void* data, LSceneNode* node)
	{
		if(node!= NULL && node->GetType() == LSHAPE_NODE)
		{
			LFace* face = node->GetShape();
			if(face)
			{
				Mesh* mesh = face->GetRenderMeshData();
				if(mesh)
				{
					VertexSet* meshData = mesh->GetRefMesh();
					if(meshData->IsDirty())
					{
						SceneManager* scene =(SceneManager*)data;
						//显示更新硬件缓存
						meshData->UpdateHardWareBuffer(scene);
					}
				}
			}
		}
	}

	GroupNode* SceneManager::CreateModelNodes(Model* model, string currPath, int n)
	{
		char strID[10];
		sprintf(strID, "%x", model->GetPlcId());
		string tName = currPath + "|" + strID;
		//	LOGI("CreateFromModel   %s",tName.c_str());
		ModelNode* tgroup = new ModelNode;

	tgroup->SetName(tName);
	tgroup->SetOrigPlcMatrix(model->GetPlaceMatrix());

	ShapeNode * shapeNode = new ShapeNode;
	shapeNode->SetName(tName + "|" + "ShapeModel");

//	model->SetRelevantID(shapeNode->GetID());
	model->SetSceneNode(shapeNode);

	shapeNode->SetShape((Shape*) model);
	model->SetScene(this);

	tgroup->SetShapeNode(shapeNode);

	const vector<Model*>& subModels = model->GetSubModels();

	for (int i = 0; i < subModels.size(); i++)
	{
		tgroup->AddChild((SceneNode*) CreateModelNodes(subModels[i],
							tName, model->GetID()));
	}

	return tgroup;
}
 
	void SceneManager::ClearModelAndHandles()
	{
		LOGE("clear start");
		GroupNode * mainScene = (GroupNode *) this->GetSceneRoot()->Search(MAINGROUP);
		HandlerGroup* intergroup =
			(HandlerGroup*)this->GetSceneRoot()->Search(HANDLER_GROUPNODE);
		if (intergroup != NULL)
		{
			intergroup->Clear();
		}

		//	map<int, Shape*>::iterator it = this->m_NewsShapesIDMap.begin();
		//	vector<int> newShapIDS;
		//	while (it != m_NewsShapesIDMap.end())
		//	{
		//		newShapIDS.push_back(it->first);
		//		it++;
		//	}

		//	for (int i = 0; i < newShapIDS.size(); i++)
		//	{
		//		this->RemoveShape(newShapIDS.at(i));
		//	}

			//LOGE("ClearnModelRoot 222");
		//	this->m_NewsShapesIDMap.clear();
		this->m_NodesMap.clear();
		//	m_Scene->m_NodesMgr->m_NodesIDMap.clear();
		mainScene->DeleteAllChildren();
		LOGE("clear end");
	}

	void SceneManager::UpdataNodePathMap()
	{
		this->m_NodesMap.clear();
		this->m_ShapesIDMap.clear();
		//	this->m_NewsShapesIDMap.clear();

		CallBackAction* cacheNameAction = new CallBackAction;
		cacheNameAction->SetActionFun(SetNodeToMap);
		cacheNameAction->SetActionData(&this->m_NodesMap);
		m_pSceneRoot->Traverse((Action*)cacheNameAction);
		delete cacheNameAction;
	}

	BoundingBox SceneManager::GetFitViewSceneBox()
	{
		if (!m_sceneBox.Defined())
		{
			CallBackAction* computeBoxAction = new CallBackAction();
			computeBoxAction->SetActionData(this);
			computeBoxAction->SetActionFun(SceneManager::ComputeBox);
			this->ExecuteAction(computeBoxAction);
			delete computeBoxAction;
		}
		this->GetRenderManager()->RequestRedraw();

		return m_sceneBox;
	}

	void SceneManager::ResetModelNodesCallback(void* data, SceneNode* node)
	{
		node->Reset();
	}

	void SceneManager::SetNodeToMap(void* data, SceneNode* node)
	{
		//	LOGI("SetNodeToMap  is  ok %s",node->GetName().c_str());
		map<string, SceneNode*>* nodesMap = (map<string, SceneNode*>*) data;
		map<string, SceneNode*>::iterator it = nodesMap->find(node->GetName());
		if (it != nodesMap->end())
		{
			it->second = node;
		}
		else
		{
			nodesMap->insert(
				map<string, SceneNode*>::value_type(node->GetName(), node));
		}
	}

Shape* SceneManager::GetShape(const string& path)
{
	Shape* shape = NULL;
	SceneNode* node = this->GetNode(path);

	if (node == NULL)
	{
		LOGE("GetShape NULL");
		return NULL;
	}

	if (node->GetType() == SHAPE_NODE  )
	{
		ShapeNode* shapeNode = (ShapeNode*) node;
		shape = shapeNode->GetShape();
	}else if(node->GetType() == MODEL_NODE){
		ModelNode* shapeNode = (ModelNode*) node;
		shape = shapeNode->GetShapeNode()->GetShape();
	}

	return shape;
}

	void SceneManager::Lock()
	{
		m_mutex.Acquire();
		//	this->m_RenderMgr->AllowDraw(false && this->m_TopModel);
	}

	void SceneManager::UnLock()
	{
		m_mutex.Release();
		//	this->m_RenderMgr->AllowDraw(true && this->m_TopModel);
	}

	RenderManager*  SceneManager::GetRenderManager()
	{
		return this->m_RenderMgr;
	}

	void  SceneManager::ShowRotationCenter(bool flag)
	{
		if (this->m_RenderMgr != NULL)
		{
			m_RenderMgr->ShowRotationCenter(flag);

			this->GetRenderManager()->RequestRedraw();
		}
	}
    void SceneManager::UpdateSceneByModel(Model * model)
    {
        if (!model)
        {
            return;
        }
        ModelNode * node = ModelAssemblyHelper::GetModelNode(model);
        BoundingBox modelBox;
        if (node)
        {
            modelBox = node->GetParent()->GetWorldBoundingBox();//node->GetWorldBoundingBox();
            
            if (m_sceneBox.IsInside(modelBox) != INSIDE)
            {
                m_sceneBox.Merge(modelBox);
            }
        }
        this->GetRenderManager()->RequestRedraw();
    }
    
    void SceneManager::UpdateSceneByModel(vector<Model*> models)
    {
        int size = models.size();
        for (int i = 0;i<size;i++)
        {
            UpdateSceneByModel(models.at(i));
        }
    }
	////************************************
	//// Method:    GetSectionGroupNode
	//// FullName:  M3D::SceneManager::GetSectionGroupNode
	//// Access:    public
	//// Returns:   M3D::SectionGroupNode*
	//// Qualifier:
	//// 将sectionGroupNode加入场景
	////************************************
	//SectionGroupNode* SceneManager::GetSectionGroupNode()
	//{
	//	if (!this->m_sectionGroupNode)
	//	{
	//		GroupNode * groupNode = (GroupNode*) this->GetSceneRoot();
	//
	//		SectionGroupNode* sectionGroupNode = new SectionGroupNode();
	//		sectionGroupNode->SetScene(this);
	////		sectionGroupNode->SetName(M3D::MEASURE_GROUP_PATH);//TODO 还得取名字
	//		groupNode->AddChild(sectionGroupNode);
	//
	//		this->m_sectionGroupNode = sectionGroupNode;
	//	}
	//
	//	return this->m_sectionGroupNode;
	//}
}
