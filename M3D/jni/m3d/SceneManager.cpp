#include "m3d/SceneManager.h"

#include "m3d/ResourceManager.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/model/Model.h"
#include "m3d/model/ModelShape.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/scene/AxisNode.h"
#include "m3d/scene/FPSNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/graphics/DirectionalLight.h"
#include "m3d/scene/BackgroundNode.h"

#include "m3d/model/Shape.h"
#include "m3d/model/Model.h"
#include "m3d/Handler/Handler.h"
#include "m3d/Handler/HandlerPoint.h"
#include "m3d/extras/note/NoteGroup.h"
#include "m3d/Handler/HandlerRotateCenter.h"

#include "m3d/renderer/gl10/GLDrawer.h"
#include "m3d/renderer/RenderContext.h"

#include "m3d/Handler/HandlerGroup.h"

#include "sview/views/Parameters.h"

#include "m3d/action/Action.h"
#include "m3d/action/CallBackAction.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/action/RenderAction.h"
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
#include "m3d/utils/MemoryHelper.h"
#include "m3d/utils/PathHelper.h"

#include "m3d/RenderManager.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/Edge.h"
#include "m3d/model/ExtendInfoManager.h"

#include "m3d/extras/measure/MeasureGroup.h"
#include "m3d/base/glue/GlueObj.h"
#include "m3d/base/FileCacheManager.h"
#include "m3d/scene/Octree.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/model/Point.h"
#include "m3d/base/Ray.h"
#include "m3d/scene/SectionNode.h"
#include "m3d/renderer/LightManager.h"
#include "m3d/scene/LightGroup.h"
#include "m3d/scene/CameraGroup.h"
#include "m3d/extras/annotation/AnnotationGroup.h"
#ifdef WIN32
#include "m3d/UI/gui.h"
#endif //WIN32
#include "renderer/UniformHelper.h"
#include "scene/ScreenUILayerGroup.h"
using namespace SVIEW;

namespace M3D
{

	SceneManager::SceneManager(void)
	{
		m_pSceneRoot = NULL;
	
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

		if (this->m_ocTree)
		{
			delete this->m_ocTree;
			this->m_ocTree = NULL;
		}

		if (this->m_extendinfoMgr)
		{
			delete this->m_extendinfoMgr;
			this->m_extendinfoMgr = NULL;
		}

		if (this->m_lightManager)
		{
			delete this->m_lightManager;
			this->m_lightManager = NULL;
		}

		if (this->m_resourceMgr)
		{
			delete this->m_resourceMgr;
			this->m_resourceMgr = NULL;
		}


		UniformHelper::Clear();

		ReleaseMe(this->m_lightGroup);

		ReleaseMe(this->m_hudCamera);
	}

	SceneNode * SceneManager::GetSceneRoot(void)
	{
		return m_pSceneRoot;
	}

	void SceneManager::Init(void)
	{
		m_SceneBoxChanged = true;
		m_TopModel = NULL;
		this->m_pSceneRoot = new GroupNode;
		this->m_noteGroup = NULL;
		this->m_annotationGroup = NULL;
		this->m_measureGroup = NULL;
		this->m_lightGroup = NULL;
		m_sectionNode = NULL;
		m_glueObj = new GlueObj();

		this->m_isModelDirty = false;

		this->m_ocTree = new Octree();
		m_extendinfoMgr = NULL;
		
		m_resourceMgr = NULL;
		this->m_RenderMgr = new RenderManager(this);

		m_cameraManager = NULL;
		this->m_lightManager = NULL;
		this->m_cameraGroup = NULL;

		this->m_hudCamera = NULL;
#ifdef WIN32
		this->m_gui = NULL;
#endif //WIN32		
	}

	void SceneManager::RemoveModel()
	{
		MutexLock locked(this->m_mutex);
		this->GetRenderManager()->ClearDelayDrawList();
		if (this->m_TopModel)
		{
			this->m_TopModel->Release();
			this->m_TopModel = NULL;
		}

		this->SetModel(NULL);

		//如果八插树存在，则清空八叉树
		if (this->m_ocTree)
		{
			this->m_ocTree->Clear();
		}

		this->ClearModelAndHandles();

		this->GetRenderManager()->ClearGLResource();
		this->GetRenderManager()->RequestRedraw();
	}

	void SceneManager::OptimizeScene(bool isAsynMode)
	{
		this->m_sceneBox.Clear();
		this->UpdataNodePathMap();
		if (!isAsynMode)
		{
			//this->Reset();
			this->ReIndexIDMapAfterAddModel(this->m_TopModel);
			this->m_isModelDirty = true;
			this->UpdateHardwareBuffer();
		}
	
		//设置ocTree区域
		this->RequestUpdateWhenSceneBoxChanged();
	}

	void SceneManager::AsynUpdateModelCacheInfo(Model* model, bool add, bool addSub)
	{
		if (model)
		{
			if (!this->GetShape(model->GetID()))
			{
				if (add)
				{
					if (addSub)
					{
						this->ReIndexIDMapAfterAddModel(model);
					}
					else
					{
						ReIndexIDMapAfterAddSingleModel(model);
					}
				}
			}
			else
			{
				if (!add)
				{
					if (addSub)
					{
						this->ReIndexIDMapAfterDeleteModel(model);
					}
					else
					{
						ReIndexIDMapAfterDeleteSingleModel(model);
					}
				}
			}
		}
	}

	bool SceneManager::AsynAddModelToParent(Model* parentModel, Model* model)
	{
		bool addState = false;
		if (parentModel&&model)
		{
			MutexLock locked(this->m_mutex);
			parentModel->AddSubModel(model);

			model->SetModelExtInfo(this->GetExtendInfoManager(), true);

			//if (model->GetWorldBoundingBox().Defined())
			{
				this->AddNodeToOCTree(model);
				this->AsynUpdateModelCacheInfo(model,true);
			}
			addState = true;
		}

		return addState;
	}

	bool SceneManager::AsynRemoveModelFromeScene(Model* parentModel, Model* model)
	{
		bool removeState = false;
		if (parentModel&&model)
		{
			MutexLock locked(this->m_mutex);
			this->GetRenderManager()->ClearDelayDrawList();

			this->AsynUpdateModelCacheInfo(model, false);
			parentModel->RemoveSubModel(model);

			//RemoveModelCachePath(model);
			this->RequestUpdateWhenSceneBoxChanged();

			removeState = true;
		}

		return removeState;
	}

	M3D::ExtendInfoManager* SceneManager::GetExtendInfoManager()
	{
		if (this->m_extendinfoMgr == NULL)
		{
			m_extendinfoMgr = new ExtendInfoManager();
			m_extendinfoMgr->SetScene(this);
		}
		return this->m_extendinfoMgr;
	}        

	void SceneManager::BeginDiskCache()
	{
			CFileCacheManager* fileCacheMgr = GetResourceManager()->GetFileCacheMgr();
			if (fileCacheMgr)
			{
				//创建缓存文件
				fileCacheMgr->bgeinCache();

				CallBackAction* resetAction = new CallBackAction;
				resetAction->SetActionFun(UpdataHardWareBuffer);
				resetAction->SetActionData(this);
				this->ExecuteAction(resetAction);
				delete resetAction;
			}
	}

	void SceneManager::EndDiskCache()
	{
		CFileCacheManager* fileCacheMgr = GetResourceManager()->GetFileCacheMgr();
		if (fileCacheMgr)
		{
			fileCacheMgr->endWriteCache();
			fileCacheMgr->endMapping();
			fileCacheMgr->getFileMapping();
		}
	}

	void SceneManager::UpdateHardwareBuffer()
	{
		if (this->m_isModelDirty)
		{
			this->EndDiskCache();
			//CFileCacheManager* fileCacheMgr = m_resourceMgr->GetFileCacheMgr();
			////创建缓存文件
			//fileCacheMgr->bgeinCache();

			//CallBackAction* resetAction = new CallBackAction;
			//resetAction->SetActionLFun(UpdataHardWareBuffer);
			//resetAction->SetActionData(this);
			//this->ExecuteAction(resetAction);
			//delete resetAction;

			//fileCacheMgr->endWriteCache();

			//fileCacheMgr->endMapping();

			//fileCacheMgr->getFileMapping();

			this->m_isModelDirty = false;
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
		this->m_sceneBox.Clear();
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

		if (rayPickAction && this->m_ocTree)
		{
			Octree* octree = this->m_ocTree;
			const Ray& pickRay = rayPickAction->GetData()->GetCameraRay();

			vector<ModelShape*> fastResult;
			RayOctreeQuery query(fastResult,pickRay, RayQueryLevel::RAY_AABB,0.001f);

			octree->RaycastSingleFast(query);

			for (int i = 0;i<fastResult.size();i++)
			{
				ModelShape* modelShape = fastResult.at(i);
				if (modelShape)
				{
					modelShape->RayPick(rayPickAction);
				}
			}

			//this->GetNoteGroup()->RayPick(rayPickAction);
			//this->GetMeasureGroup()->RayPick(rayPickAction);
			//this->GetLightGroup()->RayPick(rayPickAction);

			this->GetExtendInfoManager()->RayPick(rayPickAction);

			ret = true;
		}

		return ret;
	}

	bool SceneManager::FrameOctreeAction(RayPickAction* rayPickAction)
	{
		bool ret = false;

		if (rayPickAction && this->m_ocTree)
		{
			Octree* octree = this->m_ocTree;
			const Ray& pickRay = rayPickAction->GetData()->GetCameraRay();

			const Frustum& pickFrustum = rayPickAction->GetData()->GetFramePickFrustum();

			vector<ModelShape*> fastResult;
			FrustumOctreeQuery query(fastResult, pickFrustum, 0, 0);
			octree->GetDrawables(query);

			for (int i = 0; i < fastResult.size(); i++)
			{
				ModelShape* modelShape = fastResult.at(i);
				if (modelShape)
				{
					modelShape->FramePick(rayPickAction);
				}
			}

			ret = true;
		}

		return ret;
	}

	bool SceneManager::FrustumOctreeAction(Action* action)
	{
		bool ret = false;

		if (action && this->m_ocTree)
		{
			CameraNode* camera = this->GetCamera();
			const Frustum& cameraFrustum = camera->GetFrustum();
			vector<ModelShape*> fastResult;

			FrustumOctreeQuery query(fastResult, cameraFrustum, 0, 0);
			//query.SetRenderAction(renderAction);
			//query.SetCamera(camera);

			this->m_ocTree->GetDrawables(query);

			//立即显示的部分
			for (int i = 0; i <fastResult.size(); ++i)
			{
				ModelShape* modelShape = fastResult.at(i);
				//modelShape->t
			}
		}

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

		this->OnRequestUpdateWhenSceneBoxChanged();

		if (this->m_ocTree)
		{
			this->m_ocTree->Update();
		}
	}

	void SceneManager::SetRenderManager(RenderManager * renderMgr)
	{
		this->m_RenderMgr = renderMgr;
	}

	void SceneManager::setSectionEnable(bool flag)
	{
		//剖切关闭或开启，可能涉及到剖面数据的添加或者删除，应该开启资源同步锁
		MutexLock locked(this->m_mutex);

		//	Model* curModel = GetModel();
		Section * sectionNode = GetSectionNode()->GetSection();
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

	//void SceneManager::SetSections(const vector<SectionPlane*> planes)
	//{
	//	MutexLock locked(this->m_mutex);

	//	if (planes.size() > 0)
	//	{
	//		Section * section = GetSectionNode()->GetSection();
	//		section->AddPlane(planes[0]);
	//	}
	//}


	M3D::SectionNode* SceneManager::GetSectionNode()
	{
		if (!this->m_sectionNode)
		{
			GroupNode * groupNode = (GroupNode*) this->GetSceneRoot();
			SectionNode* sectionNode = new SectionNode();

			//根据场景的大小默认构造三个按照X，Y，和Z方向的拖拽器
			sectionNode->SetScene(this);
			sectionNode->SetName(M3D::SECTION_GROUP_PATH);
			groupNode->AddChild(sectionNode);

			this->m_sectionNode = sectionNode;
			m_sectionNode->AddRef();
		}

		return this->m_sectionNode;

	}

	CameraNode* SceneManager::GetCamera()
	{
		return this->m_camera;
	}

	void SceneManager::SetCamera(CameraNode* camera)
	{
		MutexLock locked(this->m_mutex);
		this->m_camera = camera;
		if (this->m_camera) {
			this->m_camera->SetSceneManager(this);
		}
	}


	LightGroup * SceneManager::GetLightGroup()
	{
		if (!this->m_lightGroup)
		{
			GroupNode* groupNode = (GroupNode*)this->GetSceneRoot();
			this->m_lightGroup = new LightGroup();
			m_lightGroup->SetName(LIGHT_GROUP_PATH);
			groupNode->AddChild(m_lightGroup);
			this->m_lightGroup->SetScene(this);

			AddRefMe(this->m_lightGroup);
		}
		return m_lightGroup;
	}

	CameraGroup * SceneManager::GetCameraGroup()
	{
		if (!this->m_cameraGroup)
		{
			GroupNode* groupNode = (GroupNode*)this->GetSceneRoot();
			this->m_cameraGroup = new CameraGroup();
			m_cameraGroup->SetName(CAMERA_GROUP_PATH);
			groupNode->AddChild(m_cameraGroup);
			m_cameraGroup->SetScene(this);
		}
		return m_cameraGroup;
	}

	Model* SceneManager::GetModel()
	{
		return this->m_TopModel;
	}

	void SceneManager::AddModel(Model* model)
	{
		if (!model)
		{
			return;
		}

		MutexLock locked(this->m_mutex);

		if (model)
		{
			//if(Parameters::Instance()->m_simplityMode)
			//{
			//	GroupNode * mainGroup = (GroupNode *)GetSceneRoot()->Search(M3D::MAINGROUP);

			//	LGroupNode* modelGroupParent = (LGroupNode*)mainGroup->Search(M3D::MAINMODELROOT);

			//	if (modelGroupParent)
			//	{
			//		Model* modelGroup = static_cast<Model*>(CreateLSceneNodes(model, M3D::MAINMODELROOT, modelGroupParent->Size()));
			//		if (modelGroup->GetChildren().size() > 0)
			//		{
			//			modelGroupParent->GetChildren().at(0)->AddChild(modelGroup);
			//		}
			//	}
			//}
			this->BeginDiskCache();
		}
	}

	void SceneManager::SetModel(Model* model)
	{
		if (this->m_TopModel == model)
		{
			return;
		}

		MutexLock locked(this->m_mutex);

		this->m_TopModel = model; 
 
		if (m_TopModel)
		{
			this->m_TopModel->AddRef();

			//TODO 根据模型大小自适应的修改优化算法。
			if(Parameters::Instance()->m_OptimizeBigModel)
			{
				Matrix3x4 topMatrix = *m_TopModel->GetPlaceMatrix();
				topMatrix.SetScale(0.001);//缩小1000倍
				m_TopModel->SetPlaceMatrix(topMatrix);
			}

			//if(Parameters::Instance()->m_simplityMode)
			{
				//LModel* modelGroup = static_cast<LModel*>(CreateLSceneNodes(m_TopModel, M3D::MAINMODELROOT, 1));
				//LGroupNode* modelGroupParent = new LGroupNode();
				//modelGroupParent->SetName(M3D::MAINMODELROOT);
				//GroupNode * mainGroup = (GroupNode *)GetSceneRoot()->Search(M3D::MAINGROUP);
				//modelGroupParent->AddChild(modelGroup);
				//mainGroup->AddChild(modelGroupParent);
				//this->m_TopModel = modelGroup;

				//LModel* modelGroup = static_cast<LModel*>(CreateLSceneNodes(m_TopModel, M3D::MAINMODELROOT, 1));
				ShapeNode* modelGroupParent = new ShapeNode();
				modelGroupParent->SetName(M3D::MAINMODELROOT);
				GroupNode * mainGroup = (GroupNode *)GetSceneRoot()->Search(M3D::MAINGROUP);
				modelGroupParent->SetShape(m_TopModel);
				mainGroup->AddChild(modelGroupParent);
				m_TopModel->SetModelExtInfo(this->GetExtendInfoManager(), true);
				//this->m_TopModel = modelGroup;
				//this->SetModel(modelGroup);
			}
			//else
			//{
			//	GroupNode* modelGroup = CreateModelNodes(m_TopModel, M3D::MAINMODELROOT, 1);
			//	GroupNode* modelGroupParent = new GroupNode();
			//	modelGroupParent->SetName(M3D::MAINMODELROOT);
			//	GroupNode * mainGroup = (GroupNode *)GetSceneRoot()->Search(M3D::MAINGROUP);
			//	modelGroupParent->AddChild(modelGroup);
			//	mainGroup->AddChild(modelGroupParent);
			//}
			this->BeginDiskCache();
		}

		//VRGUI Test
	//	this->GetGUI()->TestCode();
	}

	HandlerGroup* SceneManager::GetHandlerGroup()
	{
		SceneNode* node = this->m_pSceneRoot->Search(HANDLER_GROUPNODE);
		return (HandlerGroup*)node;
	}

	NoteGroup* SceneManager::GetNoteGroup()
	{
		if (!this->m_noteGroup)
		{
			GroupNode * groupNode = (GroupNode*) this->GetSceneRoot();
			NoteGroup* noteGroup = new NoteGroup();

			noteGroup->SetName(M3D::NOTE_GROUP_PATH);
			groupNode->AddChild(noteGroup);

			this->m_noteGroup = noteGroup;
		}

		return this->m_noteGroup;
	}

	AnnotationGroup* SceneManager::GetAnnotationGroup()
	{
		if (!this->m_annotationGroup)
		{
			GroupNode * groupNode = (GroupNode*) this->GetSceneRoot();
			AnnotationGroup* annotationGroup = new AnnotationGroup();
			annotationGroup->SetScene(this);
			annotationGroup->SetName(M3D::ANNOTATION_GROUP_PATH);
			groupNode->AddChild(annotationGroup);

			m_annotationGroup = annotationGroup;
		}
		return m_annotationGroup;
	}

	MeasureGroup* SceneManager::GetMeasureGroup()
	{
		if (!this->m_measureGroup)
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

	bool SceneManager::SetRotationCenter(float x, float y,int centerType)
	{
		static int baseZoom = 3.5f;

		Vector3 intersectPnt;
		Vector2 screenPnt(x, y);
		if (centerType ==0)
		{
			if (this->GetCamera()->GetZoom() > baseZoom ||
				(this->GetCamera()->GetWorldPosition() - this->GetSceneBox().Center()).Length()
				< this->GetSceneBox().Length() / 2)
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
		}
		else if (centerType == 1)
		{
			intersectPnt = this->GetCamera()->GetWorldPosition();
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

	bool SceneManager::UpdateSceneByModel(Model * model)
	{
		bool ret = false;
		if (!model)
		{
			return ret;
		}
	 
		BoundingBox modelBox;
		modelBox = model->GetWorldBoundingBox();

		if (m_sceneBox.IsInside(modelBox) != INSIDE)
		{
			m_sceneBox.Merge(modelBox);
			this->RequestUpdateWhenSceneBoxChanged();
			ret = true;
		}
 
		this->GetRenderManager()->RequestRedraw();
		return ret;
	}

	void SceneManager::UpdateSceneByModel(vector<Model*> models)
	{
		int size = models.size();
		for (int i = 0;i<size;i++)
		{
			UpdateSceneByModel(models.at(i));
		}
	}

	void  SceneManager::OnRequestUpdateWhenSceneBoxChanged()
	{
		if (m_SceneBoxChanged)
		{
			m_SceneBoxChanged = false;
			if (this->m_ocTree)
			{
				MutexLock locked(this->m_mutex);
				this->m_sceneBox.Clear();
				BoundingBox octreeBox = this->m_ocTree->GetWorldBoundingBox();
				BoundingBox newoctreeBox = this->GetOcTreeWorldBoundingBox();
				//if (octreeBox != newoctreeBox)
				{
					this->m_ocTree->Clear();
					this->m_ocTree->SetSize(newoctreeBox, 8);

					CallBackAction* computeBoxAction = new CallBackAction();
					computeBoxAction->SetActionData(this);
					computeBoxAction->SetActionFun(NULL);
					computeBoxAction->SetActionFun(SceneManager::AddNodeToOCTree);
					ExecuteAction(computeBoxAction);
					delete computeBoxAction;
					LOGI("End Create OCTree");

					//设置场景改变，通知重新构建显示的批注等图片的大小
					this->m_RenderMgr->GetRenderAction()->SetSceneBoxChanged(true);
				}
			}
		}
	}

	void SceneManager::RequestUpdateWhenSceneBoxChanged()
	{
		MutexLock locked(this->m_mutex);
		m_SceneBoxChanged = true;
		//if (this->m_ocTree)
		//{
		//	this->GetSceneBox().Clear(); 
		//	BoundingBox octreeBox = this->m_ocTree->GetWorldBoundingBox();
		//	BoundingBox newoctreeBox = this->GetOcTreeWorldBoundingBox();
		//	//if (octreeBox != newoctreeBox)
		//	{
		//		this->m_ocTree->Clear();
		//		this->m_ocTree->SetSize(newoctreeBox, 8);
		//		
		//		CallBackAction* computeBoxAction = new CallBackAction();
		//		computeBoxAction->SetActionData(this);
		//		computeBoxAction->SetActionFun(NULL);
		//		computeBoxAction->SetActionFun(SceneManager::AddNodeToOCTree);
		//		ExecuteAction(computeBoxAction);
		//		delete computeBoxAction;
		//		LOGI("End Create OCTree");

		//		//设置场景改变，通知重新构建显示的批注等图片的大小
		//		this->m_RenderMgr->GetRenderAction()->SetSceneBoxChanged(true);
		//	}
		//}
		//OptimizeCameraView();
	}	
	
	void SceneManager::OptimizeCameraView(bool fitShowView)
	{
		CameraNode* camera = this->GetCamera();
		camera->MarkDirty();
		if (fitShowView)
		{
			this->GetRenderManager()->RequestUpdateWhenSceneBoxChanged();
		}

		//BoundingBox& pBoundingBox = this->GetSceneBox();
		//float length = pBoundingBox.Length();
		////Vector3 center = pBoundingBox.Center();
		////m_oribitControlTarget = center;
		////center.m_z += pBoundingBox.Length() * CAMERA_POSFACTOR;

		////camera->SetWorldPosition(center);
		//length = this->GetDefaultFocusLength();
		////		camera->SetZoom(this->m_SceneManager->GetDefaultZoom());

		//if (camera->IsOrthographic()) {
		//	BoundingBox& sceneBox = this->GetSceneBox();
		//	Vector3 cameraPos = camera->GetPosition();	
		//	//如果摄像机在模型内部，则进行调整
		//	if (camera->GetFrustum().IsInside(sceneBox) == INTERSECTS)
		//	{
		//		Matrix3 viewMat = camera->GetView().ToMatrix3();
		//		Vector3 direction(viewMat.m_m20, viewMat.m_m21, viewMat.m_m22);
		//		direction.Normalize();
		//		Vector3 newPos = cameraPos + direction * sceneBox.Length();
		//		CameraNode* tCamera = const_cast<CameraNode*>(camera);
		//		tCamera->SetWorldPosition(newPos);
		//	}
		//}
		//camera->SetNearClip(length * NEAR_CLIP_PLANE_FACTOR);
		//camera->SetFarClip(length * FAR_CLIP_PLANE_FACTOR);
		//camera->SetInitRotateCenter(pBoundingBox.Center());
		//camera->SetFov(90)
		//camera->LookAt(pBoundingBox.Center(), Vector3(0, 1, 0), TS_WORLD);

		//this->m_TrackBall.SetRotateSpeed(2.0f);

		//const IntRect& rect = camera->GetViewPort().GetRect();
		//int width = rect.Width() > rect.Height() ? rect.Height() : rect.Width();
		//this->m_TrackBall.SetTrackWindow(width, width);
	}

	IShape* SceneManager::GetFarPickShape(M3D::Ray& ray, int shapeType, int geoType)
	{
		MutexLock locked(this->m_mutex);
		RayPickAction* rayPickAction = new RayPickAction(this);
		rayPickAction->SetPickShapeType(shapeType);
		rayPickAction->SetPickGeoType(geoType);

		rayPickAction->SetRay(ray);

		if (!RayOctreeAction(rayPickAction))
		{
			//LOGI("setpoint is ok");
			SceneNode* group = this->m_pSceneRoot;
			group->Traverse((Action*)rayPickAction);
		}
		IShape* shape = rayPickAction->GetFarPickShape();

		delete rayPickAction;
		rayPickAction = nullptr;
		
		return shape;
	}

	IShape * SceneManager::GetPickShape(M3D::Ray& ray, int shapeType, int geoType)
	{
		MutexLock locked(this->m_mutex);
		RayPickAction* rayPickAction = new RayPickAction(this);
		rayPickAction->SetPickShapeType(shapeType);
		rayPickAction->SetPickGeoType(geoType);

		rayPickAction->SetRay(ray);

		if (!RayOctreeAction(rayPickAction))
		{
			//LOGI("setpoint is ok");
			SceneNode* group = this->m_pSceneRoot;
			group->Traverse((Action*)rayPickAction);
		}

		GroupNode * groupNode = (GroupNode *)this->GetSceneRoot();
		for (int i = 0; i < groupNode->Size(); i++)
		{
			SceneNode* node = groupNode->GetChild(i);
			if (node->GetName() != MAINGROUP)//不处理挂载模型的节点
			{
				node->RayPick(rayPickAction);
			}
		}

		IShape* shape = rayPickAction->GetNearPickShape();

		delete rayPickAction;

		//this->GetRenderManager()->RequestRedraw();

		return shape;
	}
	
	IShape* SceneManager::GetPickShape(float winx, float winY, int shapeType,
		int geoType)
	{
		MutexLock locked(this->m_mutex);
		RayPickAction* rayPickAction = new RayPickAction(this);
		rayPickAction->SetPickShapeType(shapeType);
		rayPickAction->SetPickGeoType(geoType);

		rayPickAction->SetRay((int)winx, (int)(winY));

		if (!RayOctreeAction(rayPickAction))
		{
			//LOGI("setpoint is ok");
			SceneNode* group = this->m_pSceneRoot;
			group->Traverse((Action*)rayPickAction);
		}

		GroupNode * groupNode = (GroupNode *)this->GetSceneRoot();
		for (int i = 0; i < groupNode->Size(); i++)
		{
			SceneNode* node = groupNode->GetChild(i);
			if (node->GetName() != MAINGROUP)//不处理挂载模型的节点
			{
				node->RayPick(rayPickAction);
			}
		}

		IShape* shape = rayPickAction->GetNearPickShape();

		delete rayPickAction;

		//this->GetRenderManager()->RequestRedraw();

		return shape;
	}

	bool SceneManager::ProcessVREvent()
	{
		bool processState = false;




		return processState;
	}

vector<IShape*> SceneManager::GetFramePickShape(const Vector2& leftTop, const Vector2& rightBottom,
		int shapeType,
		int geoType, int framePickType)
{
	MutexLock locked(this->m_mutex);
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

		GroupNode * groupNode = (GroupNode *)this->GetSceneRoot();
		for (int i = 0; i < groupNode->Size(); i++)
		{
			SceneNode* node = groupNode->GetChild(i);
			if (node->GetName() != MAINGROUP)//不处理挂载模型的节点
			{
				//node->pi(rayPickAction);
			}
		}

		vector<IShape*> framePickShapes = rayPickAction->GetFramePickShapes();

		delete rayPickAction;

		this->GetRenderManager()->RequestRedraw();

		return framePickShapes;
	}

IShape* SceneManager::GetPickShape(Vector2& screentPnt, int shapeType, int geoType)
{
	MutexLock locked(this->m_mutex);
		RayPickAction* rayPickAction = new RayPickAction(this);
		rayPickAction->SetPickShapeType(shapeType);
		rayPickAction->SetPickGeoType(geoType);

		rayPickAction->SetRay(screentPnt);

		if (!RayOctreeAction(rayPickAction))
		{
			SceneNode* group = this->m_pSceneRoot;
			group->Traverse((Action*)rayPickAction);
		}

		GroupNode * groupNode = (GroupNode *)this->GetSceneRoot();
		for (int i = 0; i < groupNode->Size(); i++)
		{
			SceneNode* node = groupNode->GetChild(i);
			if (node->GetName() != MAINGROUP)//不处理挂载模型的节点
			{
				node->RayPick(rayPickAction);
			}
		}
	 
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

	//
	void SceneManager::SetMatrixByPlcPath(char* plcPath, float matrix[4][4])
	{
		string nodePath = "M3D|MAIN|" + string(plcPath);
		Model* node = this->GetNode(nodePath);
		if (node != NULL)
		{
			Matrix3x4 plcMatrix((float*)matrix);
			node->SetPlaceMatrix(plcMatrix);
			UpdateSceneByModel(node);
		}
 
	}
	void SceneManager::GetMatrixByPlcPath(char* plcPath, float matrix[4][4])
	{
		string nodePath = "M3D|MAIN|" + string(plcPath);

		Model* node = this->GetNode(nodePath);
		if (node != NULL)
		{
			Matrix3x4* plcMatrix = node->GetPlaceMatrix();
			memcpy(matrix, Matrix4::IDENTITY.Data(), 16 * sizeof(float));
			memcpy(matrix, plcMatrix->Data(), 12 * sizeof(float));
		}
	}

	int SceneManager::AddHandle(float x, float y, int type)
	{
		MutexLock locked(this->m_mutex);
		Vector3 handlePos = GetPickPoint(x, y);
		IShape* handle = CreateShape(handlePos, type);

		this->GetRenderManager()->RequestRedraw();

		return handle->GetID();
	}

	int SceneManager::AddHandle(float x, float y, float z, int type)
	{
		MutexLock locked(this->m_mutex);
		Vector3 pos(x, y, z);
		IShape* handle = CreateShape(pos, type);

		this->GetRenderManager()->RequestRedraw();

		return handle->GetID();
	}

	Vector3 SceneManager::GetPickPoint(float x, float y, bool inModel/* = true*/, bool inExcludeEdge)
	{
		Vector3 vec;
		Vector2 srcPnt(x, y);
		this->GetPickPoint(srcPnt, vec, inModel, inExcludeEdge);
		return vec;
	}

	Vector3 SceneManager::GetUILayerPickPoint(float x, float y)
	{
		Vector3 vec;
		Vector2 srcPnt(x, y);
		RayPickAction* rayPickAction = new RayPickAction(this);

		rayPickAction->SetCamera(this->GetHudCamera());

		rayPickAction->SetPickShapeType(SHAPE_MODEL);
		rayPickAction->SetPickGeoType(M3D_GEOATTR_TYPE_UNKNOWN);
		rayPickAction->SetRay(srcPnt);

		Matrix3x4 viewMatrix = rayPickAction->GetCamera()->GetView();
		const Ray& pickRay = rayPickAction->GetData()->GetCameraRay();
		Vector3 tCenter;
		BoundingBox& pBoundingBox = this->GetHudLayerBox();
		tCenter = pBoundingBox.Center();

		//求射线与（包围盒中心点深度和摄像机方向所确定）平面交点
		float s = pickRay.m_direction.LengthSquared();
		float k = (tCenter - pickRay.m_origin).DotProduct((pickRay.m_direction)) * (1 / s);
		vec = (pickRay.m_direction) * k + pickRay.m_origin;


		delete rayPickAction;
		rayPickAction = NULL;

		this->GetRenderManager()->RequestRedraw();

		return vec;
	}

	bool SceneManager::GetPickPoint(Ray & ray, Vector3 & pnt, bool inModel)
	{
		bool ret = false;
		Vector3 vec;
		RayPickAction* rayPickAction = new RayPickAction(this);
		rayPickAction->SetPickShapeType(SHAPE_MODEL);
		rayPickAction->SetPickGeoType(M3D_GEOATTR_TYPE_UNKNOWN);
		rayPickAction->SetRay(ray);

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

	bool SceneManager::GetPickPoint(Vector2& screenPnt, Vector3& pnt, bool inModel,  bool inExcludeEdge)
	{
		bool ret = false;
		Vector3 vec;
		RayPickAction* rayPickAction = new RayPickAction(this);
		if (inExcludeEdge)
		{
			rayPickAction->SetPickShapeType(SHAPE_MODEL, SHAPE_EDGE);
		} 
		else
		{
			rayPickAction->SetPickShapeType(SHAPE_MODEL);
		}		
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
		//拿到action中计算出来的法向量
		rayIntersectNormal = rayPickAction->normal;
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
	Vector3 SceneManager::GetPickNormal()
	{
		return this->rayIntersectNormal;
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
		IShape* shape = CreateShape(handlePos, type);
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
		IShape* shape = CreateShape(pos, type);
		if (!shape)
		{
			return -1;
		}
		return shape->GetID();
	}

	IShape* SceneManager::CreateShape(const Vector3& position, int Type)
	{
		MutexLock locked(this->m_mutex);

		IShape* shape = NULL;

		switch (Type)
		{
		case SHAPE_POINT_HANDLE:
		{
			break;
		}

		case SHAPE_VOICE_NOTE:
		{
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
			map<IDTYPE, IShape*>::value_type(shape->GetID(), shape));
		return shape;
	}

	bool SceneManager::RemoveShape(IDTYPE id)
	{
		MutexLock locked(this->m_mutex);

		map<IDTYPE, IShape*>::iterator it = this->m_ShapesIDMap.find(
			id);
		if (it != this->m_ShapesIDMap.end())
		{
			IShape* shape = it->second;
			if (shape != NULL)
			{
				if (shape->GetType()== MODEL_SHAPE)
				{

				}
				else
				{
					GroupNode* group = (GroupNode*)shape->GetSceneNode()->GetParent();
					group->DeleteChild(shape->GetSceneNode());
				}
			}
			this->m_ShapesIDMap.erase(it);
		}
		this->GetRenderManager()->RequestRedraw();

		return true;
	}

	void SceneManager::GetShapePos(IDTYPE id, int type, Vector3& pos)
	{
		IShape* shape = this->GetShape(id);
		if (shape != NULL)
		{
			//0获得shape的实际位置,1获得对应node的位置
			if (type == 0)
			{
				//			pos = shape->GetCenterPos();
			}
			else if (type == 1)
			{
				if (shape->GetType() == MODEL_SHAPE)
				{
					Model* model = static_cast<Model*>(shape);
					if (model)
					{
						pos = model->GetPlaceMatrix()->Translation();
					}
				}
			}
		}
	}


	Color SceneManager::GetShapeColor(IDTYPE id, int type)
	{
		Color color;
		IShape* shape = this->GetShape(id);
		if (shape != NULL)
		{
			color = *shape->GetColor(); //or drawColor?
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
			m_sceneBox.Clear();
			CallBackAction* computeBoxAction = new CallBackAction();
			computeBoxAction->SetActionData(this);
			//computeBoxAction->SetActionLFun(SceneManager::ComputeBox);
			computeBoxAction->SetActionFun(SceneManager::ComputeLBox);
			this->ExecuteAction(computeBoxAction);
			delete computeBoxAction;
		}
		return m_sceneBox;
	}

	IShape* SceneManager::GetShape(IDTYPE shapeID)
	{
		map<IDTYPE, IShape*>::iterator it = m_ShapesIDMap.find(shapeID);
		if (it != m_ShapesIDMap.end())
		{
			return it->second;
		}
		return NULL;
	}

	Model* SceneManager::GetNode(const string& name)
	{
		if (!SVIEW::Parameters::Instance()->m_useSimplePath)
		{
			map<string, Model*>::iterator it = this->m_NodesMap.find(name);
			if (it != this->m_NodesMap.end())
			{
				return it->second;
			}
		}
		return NULL;
	}

	void SceneManager::AddModelCachePath(Model* model)
	{
		if (model && !GetNode(model->GetPlcPath())) {
			m_NodesMap.insert(pair<string, Model*>(model->GetPlcPath(), model));
		}
	}

    void SceneManager::RemoveModelCachePath(Model* model)
    {
		if (model && GetNode(model->GetPlcPath())) {
			//先删子
			map<string, Model*>::iterator it = m_NodesMap.find(model->GetPlcPath());
			m_NodesMap.erase(it);
		}
    }
	void SceneManager::ReIndexIDMapAfterAddModel(Model* shape)
	{
		if (shape != NULL)
		{
			if (shape->GetType() == SHAPE_MODEL || shape->GetType() == IMAGEMODEL_SHAPE)
			{
				Model* model = (Model*)shape;
				ReIndexIDMapAfterAddSingleModel(model);

				vector<Model*>& subModels = model->GetSubModels();
				for (int i = 0; i < subModels.size(); i++)
				{
					this->ReIndexIDMapAfterAddModel(subModels[i]);
				}
			}

			this->GetRenderManager()->RequestRedraw();
		}
	}

	void SceneManager::ReIndexIDMapAfterAddSingleModel(Model* shape)
	{
		if (shape != NULL)
		{
			if (shape->GetType() == SHAPE_MODEL || shape->GetType() == IMAGEMODEL_SHAPE)
			{
				Model* model = (Model*)shape;

				AddShapeIDToMap(model);

				if (!SVIEW::Parameters::Instance()->m_useSimplePath)
				{
					this->AddModelCachePath(model);
				}

				if (SVIEW::Parameters::Instance()->m_useBodyObject)
				{
					vector<Body*>* bodys = model->GetBodys();

					if (bodys)
					{
						for (int i = 0; i < bodys->size(); i++)
						{
							Body* body = bodys->at(i);
							AddShapeIDToMap(body);
							vector<Face*>& faces = body->GetFaces();

							for (int j = 0; j < faces.size(); j++)
							{
								Face* face = faces[j];
								AddShapeIDToMap(face);
								vector<Edge*>* edges = face->GetEdges();
								if (edges)
								{
									for (int k = 0; k < edges->size(); k++)
									{
										Edge* edge = edges->at(k);
										AddShapeIDToMap(edge);
									}
								}
							}

							vector<Edge*>* edges = body->GetEdges();
							if (edges) {
								for (int k = 0; k < edges->size(); k++)
								{
									Edge* edge = edges->at(k);
									AddShapeIDToMap(edge);
								}
							}

							vector<Point*>* points = body->GetPoints();
							if (points)
							{
								for (int k = 0; k < points->size(); k++)
								{
									Point* point = points->at(k);
									AddShapeIDToMap(point);
								}
							}
						}
					}
				}
			}

			this->GetRenderManager()->RequestRedraw();
		}
	}

	void SceneManager::ReIndexIDMapAfterDeleteModel(Model* shape)
	{
		if (shape != NULL)
		{
			{
				Model* model = (Model*)shape;
				ReIndexIDMapAfterDeleteSingleModel(model);
				
				vector<Model*>& subModels = model->GetSubModels();
				for (int i = 0; i < subModels.size(); i++)
				{
					this->ReIndexIDMapAfterDeleteModel(subModels[i]);
				}
			}
		}
	}

	void  SceneManager::ReIndexIDMapAfterDeleteSingleModel(Model* model)
	{
		RemoveShapeIDFromMap(model);
		if (!SVIEW::Parameters::Instance()->m_useSimplePath)
		{
			this->RemoveModelCachePath(model);
		}

		if (SVIEW::Parameters::Instance()->m_useBodyObject)
		{
			vector<Body*>* bodys = model->GetBodys();
			if (bodys)
			{
				for (int i = 0; i < bodys->size(); i++)
				{
					Body* body = bodys->at(i);

					RemoveShapeIDFromMap(body);
					vector<Face*>& faces = body->GetFaces();
					for (int j = 0; j < faces.size(); j++)
					{
						Face* face = faces[j];

						RemoveShapeIDFromMap(face);
						vector<Edge*>* edges = face->GetEdges();
						if (edges) {
							for (int k = 0; k < edges->size(); k++)
							{
								Edge* edge = edges->at(k);
								RemoveShapeIDFromMap(edge);
							}
						}
					}

					vector<Edge*>* edges = body->GetEdges();
					if (edges)
					{
						for (int k = 0; k < edges->size(); k++)
						{
							Edge* edge = edges->at(k);
							RemoveShapeIDFromMap(edge);
						}
					}

					vector<Point*>* points = body->GetPoints();
					if (points)
					{
						for (int k = 0; k < points->size(); k++)
						{
							Point* point = points->at(k);
							RemoveShapeIDFromMap(point);
						}
					}
				}
			}
		}
	}


	ResourceManager* SceneManager::GetResourceManager()
	{
		if (!this->m_resourceMgr)
		{
			this->m_resourceMgr = new ResourceManager();
		}

		return this->m_resourceMgr;
	}

	void SceneManager::RemoveShapeIDFromMap(IShape* shape)
	{
		if (shape)
		{
			m_ShapesIDMap.erase(shape->GetID());
		}
	}

	void SceneManager::AddShapeIDToMap(IShape* shape)
	{
		if (shape)
		{
			m_ShapesIDMap.insert(
				map<IDTYPE, IShape*>::value_type(shape->GetID(), shape));
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

	void SceneManager::ComputeBox(void* data, Model* node)
	{
		if (node != NULL && node->GetType() == SHAPE_NODE)
		{
			SceneManager* scene = (SceneManager*)data;
			BoundingBox& sceneBox = scene->m_sceneBox;
			BoundingBox& shapeNodeBox = node->GetWorldBoundingBox();
			if (shapeNodeBox.Defined())
			{
				sceneBox.Merge(shapeNodeBox);
			}
		}
	}

	void SceneManager::ComputeLBox(void* data, Model* node)
	{
		if (node != NULL && (node->GetType() == SHAPE_MODEL || node->GetType() == IMAGEMODEL_SHAPE))
		{
			SceneManager* scene = (SceneManager*)data;
			BoundingBox& sceneBox = scene->m_sceneBox;
			BoundingBox& shapeNodeBox = node->GetWorldBoundingBox();
			if (shapeNodeBox.Defined())
			{
				sceneBox.Merge(shapeNodeBox);
			}
		}
	}

	void  SceneManager::AddNodeToOCTree(Model* model)
	{
		if (model->GetType() == SHAPE_MODEL || model->GetType() == IMAGEMODEL_SHAPE)
		{
			ModelShape* modelShape = model->GetModelShape();
			if (modelShape && this->m_ocTree)
			{
				const BoundingBox& box = modelShape->GetWorldBoundingBox();
				if (box.Defined())
				{
					this->m_ocTree->InsertDrawable(modelShape);
					modelShape->SetOctree(this->m_ocTree);
				}

				//vector<Body*>* bodys = modelShape->GetBodys();
				//for (int i = 0; i < bodys->size(); i++)
				//{
				//	vector<Face*>& subFaces = bodys->at(i)->GetFaces();
				//	for (int j = 0; j < subFaces.size(); j++)
				//	{
				//		Face* shape = subFaces[j];
				//		if (MemoryHelper::IsMemoryRich())
				//		{
				//			shape->GetRenderMeshData()->GetRefMesh()->UnCacheFromDisk(this);
				//		}
				//	}
				//}
			}
		}
	}

	void SceneManager::AddNodeToOCTree(void* data, Model* node)
	{
		SceneManager* scene = (SceneManager*)data;
		
		if (scene && (node->GetType() == SHAPE_MODEL || node->GetType() == IMAGEMODEL_SHAPE))
		{
			scene->AddNodeToOCTree(node);
			/*ModelShape* modelShape = node->GetModelShape();
			if (modelShape)
			{
				scene->m_ocTree->InsertDrawable(modelShape);
				modelShape->SetOctree(scene->m_ocTree);
				if (SVIEW::Parameters::Instance()->m_useBodyObject)
				{
					vector<Body*>* bodys = modelShape->GetBodys();
					for (int i = 0; i < bodys->size(); i++)
					{
						vector<Face*>& subFaces = bodys->at(i)->GetFaces();
						for (int j = 0; j < subFaces.size(); j++)
						{
							Face* shape = subFaces[j];
							if (MemoryHelper::Instance()->IsMemoryRich())
							{
								shape->GetData()->GetRefMesh()->UnCacheFromDisk(scene);
							}
						}
					}
				}
			}*/
		}
	}

	void SceneManager::UpdataHardWareBuffer(void* data, Model* node)
	{
		if (node != NULL && (node->GetType() == SHAPE_MODEL || node->GetType() == IMAGEMODEL_SHAPE))
		{
			SceneManager* scene = (SceneManager*)data;
			ModelShape* shape = node->GetModelShape();
			if (shape)
			{
				vector<Body*>* bodys = shape->GetBodys();
				for (int i = 0; i < bodys->size(); i++)
				{
					vector<Face*>& subFaces = bodys->at(i)->GetFaces();
					for (int j = 0; j < subFaces.size(); j++)
					{
						Face* shape = subFaces[j];
						if (shape->GetData())
						{
							shape->GetData()->GetRefMesh()->UpdateHardWareBuffer(scene);
						}
					}
				}
			}
		}
	}
  
	void SceneManager::ClearModelAndHandles()
	{
		LOGI("clear start");
		GroupNode * mainScene = (GroupNode *) this->GetSceneRoot()->Search(MAINGROUP);
		HandlerGroup* intergroup =
			(HandlerGroup*)this->GetSceneRoot()->Search(HANDLER_GROUPNODE);
		if (intergroup != NULL)
		{
			intergroup->Clear();
		}

		if (this->m_TopModel)
		{
			this->m_TopModel->Release(); this->m_TopModel = NULL;
		}

		//	map<int, IShape*>::iterator it = this->m_NewsShapesIDMap.begin();
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
		this->GetExtendInfoManager()->Clear();
		//	m_Scene->m_NodesMgr->m_NodesIDMap.clear();
		mainScene->DeleteAllChildren();
		LOGI("clear end");
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
#ifdef WIN32
	GUI * SceneManager::GetGUI()
	{
		if (!this->m_gui)
		{
			this->m_gui = new GUI();
			this->m_gui->SetSceneManager(this);
		}

		return m_gui;
	}
#endif //WIN32
	M3D::CameraNode* SceneManager::GetHudCamera() const
	{
		return m_hudCamera;
	}


	BoundingBox SceneManager::GetOcTreeWorldBoundingBox()
	{
		float length = GetSceneBox().Length() / 2;
		Vector3 center = GetSceneBox().Center();
		Vector3 min = center - Vector3(length, length, length);
		Vector3 max = center + Vector3(length, length, length);

		return BoundingBox(min, max);
	}

	CameraManager* SceneManager::GetCameraManager()
	{
		if (!this->m_cameraManager)
		{
			m_cameraManager = new CameraManager();
			m_cameraManager->SetSceneManager(this);

			//显示调用创建灯光渲染组
			this->GetCameraGroup();
		}
		return m_cameraManager;
	}

	LightManager* SceneManager::GetLightManager()
	{
		if (!this->m_lightManager)
		{
			m_lightManager = new LightManager();
			m_lightManager->SetSceneManager(this);

			//显示调用创建灯光渲染组
			this->GetLightGroup();
		}
		return m_lightManager;
	}

	ScreenUILayerGroup* SceneManager::GetScreenUILayerGroup()
	{
		SceneNode* node = this->m_pSceneRoot->Search(SCREENUILAYER_GROUPNODE);

		if (!node)
		{
			ScreenUILayerGroup* screenUILayergroup = new ScreenUILayerGroup(this);
			node = screenUILayergroup;
			screenUILayergroup->SetName(SCREENUILAYER_GROUPNODE);
			((GroupNode*)this->m_pSceneRoot)->AddChild(screenUILayergroup);
		}

		return (ScreenUILayerGroup*)node;
	}

	SectionNode * SceneManager::GetSectionNode(int ID)
	{
		GroupNode * groupNode = (GroupNode*) this->GetSceneRoot();
		list<SectionNode*>::iterator iter;
		for (iter = m_sceneNodeList.begin(); iter != m_sceneNodeList.end(); ++iter)
		{
			SectionNode* sectionNode = *iter;
			if (sectionNode->GetID() == ID)
			{
				return sectionNode;
			}
		}
		return nullptr;
	}

	void SceneManager::AddSectionNode(SectionNode * node)
	{
		m_sceneNodeList.push_back(node);
	}

	void SceneManager::SetUnitScale(float scale)
	{
		if (!m_pSceneRoot) {
			return;
		}
		m_unitScale = scale;
	}
	
	void SceneManager::ResetModelNodesCallback(void* data, Model* node)
	{
		node->Restore();
	}

	void SceneManager::SetNodeToMap(void* data, Model* node)
	{
		//	LOGI("SetNodeToMap  is  ok %s",node->GetName().c_str());
		map<string, Model*>* nodesMap = (map<string, Model*>*) data;
		map<string, Model*>::iterator it = nodesMap->find(node->GetPlcPath());
		if (it != nodesMap->end())
		{
			it->second = node;
		}
		else
		{
			nodesMap->insert(
				map<string, Model*>::value_type(node->GetPlcPath(), node));
		}
	}

	IShape* SceneManager::GetShape(const string& path)
	{
		IShape* shape = NULL;
		Model* node = this->GetNode(path);

		if (node == NULL)
		{
			LOGE("SceneManager::GetShape NULL");
			return NULL;
		}
		return node;
	}

	void SceneManager::Lock()
	{
		m_mutex.Acquire();
		//this->m_RenderMgr->AllowDraw(false && this->m_TopModel);
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

	void SceneManager::SetHudCamera(CameraNode* val)
	{
		if (val!= m_hudCamera)
		{
			ReleaseMe(m_hudCamera);
		}

		m_hudCamera = val;
		AddRefMe(m_hudCamera);
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
