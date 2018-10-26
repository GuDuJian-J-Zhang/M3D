#include "m3d/ResourceManager.h"
#include "m3d/graphics/MaterialTemplateManager.h"
#include "m3d/action/RenderAction.h"

#include "sview/views/View.h"
#include "sview/views/Selector.h"
#include "sview/views/Parameters.h"

#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"

#include "m3d/scene/GroupNode.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/scene/SectionNode.h"

#include "m3d/graphics/DirectionalLight.h"

#include "m3d/scene/BackgroundNode.h"
#include "m3d/scene/AxisNode.h"
#include "m3d/scene/FPSNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/scene/GroupNode.h"

#include "m3d/Handler/HandlerNode.h"
#include "m3d/Handler/HandlerGroup.h"

#include "m3d/M3DMacros.h"
#include "m3d/utils/M3DTools.h"
#include "sview/extern/PerspectiveData.h"
#include "sview/extern/PerspectiveOperator.h"
#include "sview/extern/SectionOperator.h"

#include "m3d/model/Shape.h"
#include "m3d/model/Model.h"
#include "m3d/model/IShape.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"

#include "sview/io/Reader.h"
#include "sview/io/Writer.h"
#include "sview/io/XmlSceneSerializer.h"
#include "sview/io/SVLXWriter.h"

#include "sview/manipulator/CommonTouchHandler.h"
#include "sview/manipulator/WalkthroughHandler.h"
#include "sview/manipulator/OribitControlHandler.h"
#include "sview/manipulator/DraggerHandler.h"
#include "sview/io/SceneLoader.h"

#include "sview/extern/ExplosiveViewOperator.h"

#include "m3d/graphics/Section.h"
#include "m3d/model/Note.h"
#include "m3d/model/PMIData.h"
#include "m3d/base/Vector3.h"
#include "m3d/utils/IDCreator.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/model/ModelView.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/base/Matrix4.h"
#include "m3d/graphics/MovedMatrixInfo.h"

#include "m3d/utils/FileHelper.h"

#include "m3d/graphics/Renderable.h"
#include "animation/SimulationAnimationPlay/AnimationPlayApi.h"
#include "animation/SimulationAnimation/TickTimer.h"
#include "animation/SimulationAnimation/SBehaviorAction.h"
#include "animation/SimulationAnimation/SimulationAnimationManager.h"
#include "animation/SimulationAnimation/AnimationStepManager.h"
#include "animation/SimulationAnimation/CProcess.h"
#include "animation/SimulationAnimation/ProcessManager.h"
#include "animation/SimulationAnimation/AnimationAPI.h"

#include "m3d/model/PolyLine.h"
#include "Stk_Confige.h"
#include "m3d/base/Viewport.h"

#include "m3d/base/json/json.h"
#include "sview/extern/ViewOperator.h"

#include "m3d/extras/note/NoteGroup.h"
#include "m3d/extras/note/TextNote.h"
#include "m3d/extras/note/VoiceNote.h"
#include "m3d/extras/note/NoteFactory.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/model/ModelView.h"
#include "m3d/Handler/HandlerPoint.h"
#include "m3d/extras/note/SequenceNumberNote.h"
#include "m3d/utils/PathHelper.h"
#include "m3d/extras/modelmanager/ModelManager.h"
#include "m3d/extras/OperationHistoryManager.h"
#include "m3d/scene/SceneGroundNode.h"
#include "m3d/utils/Platform.h"
#include <assert.h>
#include "m3d/graphics/DirectionalLight.h"
#include "m3d/model/Edge.h"
#include "m3d/base/glue/GlueObj.h"
#include "m3d/scene/CameraGroup.h"

#include "m3d/extras/note/ThreeDGesturesNote.h"
#include "m3d/utils/MemoryHelper.h"
#include "m3d/model/ModelShape.h"
#include "m3d/Handler/DraggerManager.h"
#include "m3d/Section/SectionManager.h"
#include "m3d/Section/SectionManager.h"
#include "m3d/renderer/LightManager.h"
#include "m3d/graphics/AmbientLight.h"
#include "m3d/graphics/HemisphereLight.h"
#include "m3d/graphics/PointLight.h"
#include "m3d/graphics/SpotLight.h"
#include "m3d/scene/GroundNode.h"
#include "m3d/extras/annotation/AnnotationGroup.h"
#include "m3d/extras/measure/MeasureGroup.h"
#include "animation/SimulationAnimationPlay/AnimationCallBackFunction.h"
#include "m3d/renderer/gl20/ShaderChunk.h"
#include <time.h>

using namespace M3D;

#ifdef __MOBILE__
using NS_SimulationAnimation::CAnimationStepManager;
using NS_SimulationAnimation::CProcess;
using NS_SimulationAnimation::CProcessManager;
#elif WIN32
using NS_SimulationAnimation::CAnimationStepManager;
using NS_SimulationAnimation::CProcess;
using NS_SimulationAnimation::CProcessManager;
#endif

namespace SVIEW {
ViewReadListener::ViewReadListener() :
		ReaderListener() {
	m_readPercent = 0;
	SetReadFileCount(1);
	SetReadFileIndex(0);
	m_readSegRange = 1;
}

ViewReadListener::~ViewReadListener() {

}

void ViewReadListener::OnProcessPercent(Reader* reader, float percent) {
	//
	SetPercent(
			this->m_readFileIndex * this->m_readSegRange
					+ percent * this->m_readSegRange);
}

void ViewReadListener::OnBegin(Reader* reader) {
	this->m_readPercent = 0.0f;
}

void ViewReadListener::OnEnd(Reader* reader) {
	this->m_readPercent = 1.0f;
}

float ViewReadListener::GetPercent() {
	//MutexLock locked(this->m_mutex);
	return this->m_readPercent;
}

void ViewReadListener::SetPercent(float percent) {
	//MutexLock locked(this->m_mutex);
	this->m_readPercent = percent;
}

void ViewReadListener::SetReadFileCount(int readFileCount) {
	this->m_readFileCount = readFileCount;
	if (this->m_readFileCount > 0) {
		this->m_readSegRange = 1.0f / this->m_readFileCount;
	}
}

void ViewReadListener::SetReadFileIndex(int readFileIndex) {
	this->m_readFileIndex = readFileIndex;
}

CTickTimer* View::pTDriver = NULL;
View::View() :
		IView() {
	//Logger::Instance()->SetLogPath("sview_log.txt");
	memoryLeakDetector();
	m_SceneManager = NULL;
	m_RenderManager = NULL;
	m_modelManager = NULL;
	m_Reader = NULL;
	m_Model = NULL;
	m_TouchEventHandler = NULL;
	m_walkthroughTouchHandler = NULL;
	m_workTouchHandler = NULL;
	m_oribitControlTouchHandler = NULL;

	m_useWalkThrough = HANDLER_COMMON;
	m_isInitialized = false;
	m_Selector = NULL;
	m_IDCreator = NULL;
	m_bHasAni = false;
	this->m_pSAManager = NULL;

	this->SetAnimationXMLData("");
	m_RefreshCallBackClassInstance = NULL;
	m_RefreshFun = NULL;

	m_isAnimationPlayCamera = true;
	m_animationPlaySpeed = 1.0f;
	m_animationContinuousPlay = false;

	m_curDrawMode = 0;

	m_isAnimationShowPMI = false;
	this->m_ExplosiveViewOperator = NULL;
	this->m_isNeedChangeViewAnimation = false;
	this->m_backgroundNode = NULL;
	m_DrawLimit = 0.0f;

	m_draggerInit = false;
	m_isDragger = false;
	m_enableCreateDefaultView = true;
	m_bNewCreate = false;

	m_curModelView = NULL;

	m_readListener = new ViewReadListener();
	m_readListener->AddRef();

	this->m_perpectiveOperator = NULL;

	//m_isSpeedModel = false;
	m_isInterBackground = false;
	_draggerManager = NULL;
	_sectionManager = NULL;
	//郑煤机项目 初始缩放比例
	m_fUnitScale = 1.0f;
	m_pAnimationCallBackFunction = new CAnimationCallBackFunction(this);

	this->m_modelSelectedCB = NULL;
}

View::~View() {
	this->CloseFile();

	if (m_IDCreator) {
		delete m_IDCreator;
		m_IDCreator = NULL;
	}
	if (m_Selector) {
		m_Selector->Clear();
		delete m_Selector;
		m_Selector = NULL;
	}
	if (m_TouchEventHandler) {
		delete m_TouchEventHandler;
		m_TouchEventHandler = NULL;
	}
	if (m_walkthroughTouchHandler) {
		delete m_walkthroughTouchHandler;
		m_walkthroughTouchHandler = NULL;
	}
	if (m_draggerHandler) {
		delete m_draggerHandler;
		m_draggerHandler = NULL;
	}
	LOGI("================delete View===================");
	if (m_Reader != NULL) {
		delete m_Reader;
	}
	LOGI("================delete Reader===================");
	/*if (m_pWorkNodeGroup != NULL)
	 {
	 delete m_pWorkNodeGroup;
	 m_pWorkNodeGroup = NULL;
	 }*/
	LOGI("================delete WorkNode===================");
	if (this->m_modelManager) {
		delete this->m_modelManager;
		this->m_modelManager = NULL;
	}
	LOGI("================delete modelmanager===================");
	m_Model = NULL;
	if (this->m_ExplosiveViewOperator) {
		delete this->m_ExplosiveViewOperator;
	}
	this->m_ExplosiveViewOperator = NULL;

	if (m_SceneManager) {
		delete m_SceneManager;
		m_SceneManager = NULL;
	}
	LOGI("================delete View end===================");

	if (m_readListener) {
		m_readListener->Release();
		m_readListener = NULL;
	}

	if (this->m_perpectiveOperator) {
		delete m_perpectiveOperator;
		m_perpectiveOperator = NULL;
	}

	if (_draggerManager) {
		_draggerManager->Release();
	}
	if (_sectionManager) {
		_sectionManager->Release();
	}
	if (m_pAnimationCallBackFunction) {
		delete m_pAnimationCallBackFunction;
		m_pAnimationCallBackFunction = NULL;
	}

	ShaderChunk::Clear();

	dumpMemoryLeaks();
}

void View::Initial() {
	if (!m_isInitialized) {
		MemoryHelper::Instance();
		LOGI("View::InitializeScene");

		m_isInitialized = true;
		m_Parameters = Parameters::Instance();

		//m_IDCreator = new IDCreator;
		m_TouchEventHandler = new CommonTouchHandler;
		m_walkthroughTouchHandler = new WalkthroughHandler;

		//m_oribitControlTouchHandler = new OribitControlHandler;

		m_draggerHandler = new DraggerHandler;

		m_workTouchHandler = m_TouchEventHandler;

		SceneManager* sceneManager = new SceneManager;

		m_Selector = new Selector(sceneManager);

		GroupNode * groupNode = (GroupNode*) sceneManager->GetSceneRoot();
		groupNode->SetName(M3D::ROOT);

		CameraNode *camera = new CameraNode;
		camera->SetName(M3D::MAINCAMERA);
		//		camera->SetOrthographic(false);
		//		camera->SetProjectionMode(M3D::ORTHO);
		//		camera->SetViewVolume(20, 1.0f, 100.0f, 20.0f);
		//		camera->SetPosition(M3D::Vector3(0, 0, 20.0f));

		sceneManager->SetCamera(camera);

		sceneManager->GetCameraManager()->AddCamera(camera);

		SetSceneManager(sceneManager);
		DirectionalLight* dirLight0 = new DirectionalLight();
		dirLight0->SetType(SHAPE_LIGHT_DIRECTIONAL);
		//dirLight0->SetName(M3D::MAINDIRTLIGHT);
		//		dirLight0->SetColor(0.8f, 0.8f, 0.8f);
		//		dirLight0->SetDiffuseIntensity(0.7f);
		//		dirLight0->SetAmbientIntensity(0.15f);
		//		dirLight0->SetSpecularIntensity(0.745f);
		//dirLight0->SetPosition(M3D::Vector3(-0.8f, -1.0f, 1.0f));
		//dirLight0->TurnOn();

		//		PointLightNode* m_pLight1=new PointLightNode;
		//		m_pLight1->SetName(M3D::MAINPOINTIGHT);
		//		m_pLight1->SetColor(0.8f, 0.8f, 0.8f);
		//		m_pLight1->SetDiffuseIntensity(0.8f);
		//		m_pLight1->SetAmbientIntensity(0.1f);
		//		m_pLight1->SetSpecularIntensity(1.0f);
		//		m_pLight1->SetPosition(M3D::Vector3(-2, -2, 2.0f));
		//		m_pLight1->SetRotation(M3D::Quaternion(0, 1, 0, 0));
		////	m_pLight1->SetScale(1, 1, 1);
		//		m_pLight1->TurnOn();
		//      m_pLight1->ShowShape();

		//		SpotLightNode* m_pLight2=new SpotLightNode;
		//		m_pLight2->SetName(M3D::MAINPOTLIGHT);
		//		m_pLight2->SetColor(0.8f, 0.8f, 0.8f);
		//		m_pLight2->SetDiffuseIntensity(0.8f);
		//		m_pLight2->SetAmbientIntensity(0.1f);
		//		m_pLight2->SetSpecularIntensity(1.0f);
		//		m_pLight2->SetCutoffAngle(60);
		//		m_pLight2->SetPosition(M3D::Vector3(0, 4, 2));
		//		m_pLight2->SetRotation(M3D::Quaternion(-60, 1, 0, 0));
		//		m_pLight2->SetScale(1, 1, 1);
		//		m_pLight2->TurnOn();
		//		m_pLight2->ShowShape();

		//sceneManager->AddLight(dirLight0);

		//mainNode->AddChild(dirLight0);
		//mainNode->AddChild(camera1);

		BackgroundNode* backgroundNode = new BackgroundNode;
		backgroundNode->SetName(M3D::BACKGROUNDCOLOR);
		Color topColor(0.25f, 0.63f, 1.0f, 1.0f);
		Color buttomColor(0.88f, 0.96f, 1.0f, 1.0f);
		backgroundNode->SetBottomColor(buttomColor);
		backgroundNode->SetTopColor(topColor);
		backgroundNode->SetResourceManager(
				this->m_SceneManager->GetResourceManager());
		groupNode->AddChild(backgroundNode);
		SetInterBackgroundState(false);
		this->m_backgroundNode = backgroundNode;

		AxisNode* axisNode = new AxisNode();
		axisNode->SetName(M3D::AXIS);
		groupNode->AddChild(axisNode);

		this->m_axisNode = axisNode;

		FPSNode* fpsNode = new FPSNode();
		fpsNode->SetName(M3D::FPS_FLAG);
		groupNode->AddChild(fpsNode);

		//SceneGroundNode * sceneGroundNode = new SceneGroundNode;
		//sceneGroundNode->SetName(M3D::SCENEGROUND);
		//groupNode->AddChild(sceneGroundNode);
		//this->m_sceneGroundNode = sceneGroundNode;

		GroundNode * sceneGroundNode = new GroundNode;
		sceneGroundNode->SetName(M3D::SCENEGROUND);
		groupNode->AddChild(sceneGroundNode);
		this->m_groundNode = sceneGroundNode;

		HandlerGroup* handlerGroup = new HandlerGroup(sceneManager);
		handlerGroup->SetName(M3D::HANDLER_GROUPNODE);

		//		handlerGroup->GetTransformHandler();
		//旋转中心点
		//		ShapeNode* rotateCenter = new ShapeNode();
		//		rotateCenter->SetName(M3D::ROTATECENTERNODE);
		//
		//		HandlerPoint* rotateCenterShape = new HandlerPoint();
		//		rotateCenter->SetShape(rotateCenterShape);
		//		handlerGroup->AddChild(rotateCenter);

		groupNode->AddChild(handlerGroup);

		//add two groupSceneNode here;
		GroupNode * mainNode = new GroupNode;
		mainNode->SetName(M3D::MAINGROUP);
		groupNode->AddChild(mainNode);
		//初始化viewmode节点
		m_viewModeTouchHandlerType = TouchHandlerType::HANDLER_COMMON;
		m_viewModeUpDirection = Vector3::UP;
		m_viewModeOribit = false;
		m_viewModeFreeLook = true;
		m_viewModeControlLockXY = false;

		LOGI("View::InitializeScene end");
		//TODO 测试金正金材质模板
		//MaterialTemplateManager* materialTemplateManager =  this->m_SceneManager->GetResourceManager()->GetMaterialTemplateManager();
		//materialTemplateManager->Load("D:\\develop\\newstructSView\\SView_Designer\\Source\\SViewDesigner\\Bin\\Win32_Debug\\data\\materialTemplate\\materialTemplate.material");
		//BaseMaterial* ret  =materialTemplateManager->GetMaterialFromTemplateByName("bojin");
		//ret->DisplayName();
	}
}

void View::TouchDown(float* p, int n) {
	MutexLock locked(this->m_mutex);
	if (!this->AllowTouch()) {
		return;
	}
	m_workTouchHandler->HandleTouchEvent(p, n, CommonTouchHandler::TOUCHDOWN);

	////TODO 测试金正金材质模板
	//MaterialTemplateManager* materialTemplateManager = this->m_SceneManager->GetResourceManager()->GetMaterialTemplateManager();
	//materialTemplateManager->Load("D:\\develop\\newstructSView\\SView_Designer\\Source\\SViewDesigner\\Bin\\Win32_Debug\\data\\materialTemplate\\materialTemplate.material");
	//BaseMaterial* ret = materialTemplateManager->GetMaterialFromTemplateByName("baibeimu");
	////ret->DisplayName();

	//vector<Model*>& vt = m_Model->GetSubModels();
	//for (size_t i = 0; i < vt.size(); i++)
	//{
	//	string name = vt[i]->GetName();
	//	if (name.find("fenzhenzhu") != string::npos)
	//	{
	//		//BaseMaterial* tempMaterial = vt[i]->GetMaterial();
	//		//tempMaterial->SetUniformParameter(GetSceneManager()->GetResourceManager(), "normalMap", "Texture2D", "D:\\develop\\newstructSView\\SView_Designer\\Source\\SViewDesigner\\Bin\\Win32_Debug\\data\\materialTemplate\\images\\mosha.png");
	//		vt[i]->SetMaterial(ret);
	//	}
	//}
}

void View::TouchMove(int moveType, float* p, int n) {
	MutexLock locked(this->m_mutex);
	if (!this->AllowTouch()) {
		return;
	}
	/*

	 LOGI("m_isDragger ==%d",m_isDragger);
	 if(m_isDragger)
	 {
	 if(m_workTouchHandler->GetTouchHandlerType() != HANDLER_DRAGGER)
	 {
	 m_draggerInit =false;
	 }

	 if(!m_draggerInit)
	 {
	 LOGI("Dragger Begin");
	 m_workTouchHandler->Close();
	 LOGI("Dragger Close() END");
	 m_workTouchHandler = this->m_draggerHandler;
	 LOGI("Dragger Begin END");
	 m_workTouchHandler->Open();
	 LOGI("Dragger Open() END");
	 LOGI("Dragger END");
	 m_draggerInit =true;

	 }
	 m_workTouchHandler->HandleTouchEvent(p, n, DraggerHandler::TOUCHMOVE,
	 moveType);
	 return;
	 }
	 */

	m_workTouchHandler->HandleTouchEvent(p, n, CommonTouchHandler::TOUCHMOVE,
			moveType);
}

void View::TouchUp(float* p, int n) {
	MutexLock locked(this->m_mutex);
	if (!this->AllowTouch()) {
		return;
	}

	m_workTouchHandler->HandleTouchEvent(p, n, CommonTouchHandler::TOUCHUP);
}

void View::WindowSizeChange(int width, int height) {
	m_SceneManager->GetRenderManager()->WindowSizeChanged(width, height);
}

void View::RebindUIRenderFBO() {
	m_SceneManager->GetRenderManager()->RebindUIRenderFBO();
}

void View::OnDraw() {
	//MutexLock locked(this->m_mutex);

	//TODO 环境纹理
	//	LOGE("ligthingMode %d",Parameters::Instance()->m_LightingMode);
	//	if(Parameters::Instance()->m_LightingMode == 200){
	//		if(!ResourceManager::GetInstance()->GetCubeMappingtexture()){
	//			string filePath = this->m_Reader->GetFile();
	//			string cubeMappingTexturePath = FileHelper::GetDirName(filePath)+"/texture.bmp";
	//			LOGI("Get CubeMap %s",cubeMappingTexturePath.c_str());
	//			ResourceManager::GetInstance()->CreateCubeMappingTexture(cubeMappingTexturePath);
	//		}
	//	}

	//	cubeMapping纹理，加载6个面 TODO
	/*if(Parameters::Instance()->m_LightingMode == 200)
	 {
	 //LOGI("ResourceManager::GetInstance()->CreateCubeMappingTexture BEGIN");
	 if(!ResourceManager::GetInstance()->GetCubeMappingTexture("gold"))
	 {
	 vector<string> imagePathes;
	 string filePath = this->m_Reader->GetFile();
	 for(int  i =0;i<6;i++)
	 {
	 string cubeMappingTexturePath = FileHelper::GetDirName(filePath)+ "gold_"+ M3DTOOLS::IntToString(i)+".png";
	 imagePathes.push_back(cubeMappingTexturePath);
	 }

	 ResourceManager::GetInstance()->CreateCubeMappingTexture("gold",imagePathes);

	 LOGI("ResourceManager::GetInstance()->CreateCubeMappingTexture END");
	 }
	 }*/

	m_SceneManager->GetRenderManager()->OnDraw();
}

void View::RequestDraw() {
	m_SceneManager->GetRenderManager()->SetUseLowQuality(true);
	m_SceneManager->GetRenderManager()->RequestRedraw();
#ifdef __MOBILE__
	if (m_RefreshFun && m_RefreshCallBackClassInstance) {
		m_RefreshFun(m_RefreshCallBackClassInstance);
	}
#else

#endif
}

void View::RequestUILayerDrawCB(int refreshTime) {
	this->GetSceneManager()->GetGlueObj()->RequestUILayerDrawCB(refreshTime);
}

/**
 * 开始渐进显示
 */
void View::RequestDelayDraw() {
	m_SceneManager->GetRenderManager()->SetUseLowQuality(false);
}

M3D_STATUS View::ReadFile(string & path) {
	this->GetPerspectiveOperator()->SetAllowUseAni(false);
	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;

	memoryLeakDetector();

	MutexLock locked(this->m_mutex);

	if (this->m_readListener) {
		this->m_readListener->SetReadFileCount(1);
		this->m_readListener->SetReadFileIndex(0);

		this->m_readListener->OnProcessPercent(NULL, 0);
	}

	try {
		SceneManager *scene = this->GetSceneManager();

		CloseFile();

		SetInterBackgroundState(false);

		LOGI("View::ReadFile input path:%s ", path.c_str());

		m_CurFilePath = M3D::FileHelper::GetUnionStylePath(path);

		LOGI("View::ReadFile path:%s ", m_CurFilePath.c_str());

		LOGI("Reader::GetReader start");
		m_Reader = Reader::GetReader(m_CurFilePath);
		if (!m_Reader) {
			readerState = M3D_STATUS::M_Read_NOT_DEFINE_FILE;
			return readerState;
		}
		if (this->m_readListener) {
			m_Reader->SetListener(this->m_readListener);
		}

		LOGI("Reader::GetReader end");

		//m_Reader->SetFontPath(fontPath);
		//m_Reader->SetIDCreator(this->m_IDCreator);

		m_Reader->SetView(this);

		LOGI("m_Reader->GetModel() start");
		m_Model = m_Reader->GetModel();
		LOGI("m_Reader->GetModel() end");

		if (m_Model) {
			bool checkErrorPoint =
					SVIEW::Parameters::Instance()->m_IsCheckErrorPoint;
			if (checkErrorPoint) {
				this->ChechErrorPoint(m_Model);
			}

			m_Model->AddRef();
		}

		//	LOGI("m_Reader->GetAllModelMap() start");
		//	m_AllModelMap = m_Reader->GetAllModelMap();
		//	LOGI("m_Reader->GetAllModelMap() end");
		LOGI("View::ReadFile getmodel ok.");
		if (m_Reader->IsCancel()) {
			LOGI("Read isCancel!");
			this->CloseFile();
			readerState = M3D_STATUS::M_Read_CANCEL;
			return readerState;
		}
		if (m_Model == NULL) {
			LOGE("Read is Error Model=NULL");
			this->CloseFile();
			readerState = M_Read_ANALYSIS_ERROR;
			return readerState;
		}

		//this->GetSceneManager()->SetUnitScale();

		//缓存动画文件
		this->SetAnimationXMLData(m_Reader->GetXMLAnimationData());
//        LOGI(                "ReadFile -- read animationXML %s sfs", this->m_AnimationXMLData.c_str());
		//	LOGI("SetModel start");
		m_SceneManager->SetModel(m_Model);
		LOGI("View::ReadFile m_SceneManager->SetModel ok.");
		this->SetDefaultWorkNodes();

		m_SceneManager->OptimizeScene(m_Reader->IsAsynMode());

		m_firstReadBox = m_SceneManager->GetSceneBox();

		if (this->m_readListener) {
			this->m_readListener->SetPercent(1.0f);
		}
		this->RestoreView(false);

		m_srcFiles = m_Reader->GetSourceFiles();
		//this->m_isSpeedModel = Parameters::Instance()->m_simplityMode;

		if (/*this->m_isSpeedModel && */!this->m_Reader->IsAsynMode()) {
			//m_Model->Release();
			//m_Model = NULL;

			if (m_Reader != NULL) {
				this->GetPerspectiveOperator()->SetAllowUseAni(true);
				delete m_Reader;
				m_Reader = NULL;
			}
		}

		//this->RestoreView();
		LOGE("readfile end");
	} catch (const std::bad_alloc& e) {
		LOGE("C++ 内存分配失败...%s", e.what());
		readerState = M_Read_OOM;
		//MemoryHelper::Instance()->SetMemoryStateLazzy(M3D_Memory_Alloc_Bad);
		this->CloseFile();
		return readerState;
	} catch (const std::ios_base::failure& e) {
		LOGE("C++ IO 异常...%s", e.what());
		readerState = M_Read_ANALYSIS_ERROR;
		this->CloseFile();
		return readerState;
	} catch (...) {
		LOGE("抱歉..未知错误...请联系开发人员..");
		readerState = M_Read_NO_DEFINE_Error;
		this->CloseFile();
		return readerState;
	}
	readerState = M_Read_OK;
	return readerState;
}

M3D_STATUS View::ReadFiles(vector<string>& paths) {
	MutexLock locked(this->m_mutex);
	memoryLeakDetector();
	//	dumpMemoryLeaks();
	if (this->m_readListener) {
		this->m_readListener->SetReadFileCount(paths.size());
		this->m_readListener->OnProcessPercent(NULL, 0);
	}

	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;

	try {
		SceneManager *scene = this->GetSceneManager();

		CloseFile();

		//if (Parameters::Instance()->m_simplityMode)
		{
			LOGI("load svlFiles begin");
			ReadFilesSpeedMode(paths);
			LOGI("load svlfiles end");
		}
		/*else
		 {
		 ReadFilesNormalMode(paths);
		 }*/

		this->SetDefaultWorkNodes();

		m_SceneManager->OptimizeScene(false);

		if (this->m_readListener) {
			this->m_readListener->SetPercent(1.0f);
		}

		this->RestoreView(false);
		LOGI("readfile end");
		readerState = M_Read_OK;
	} catch (const std::bad_alloc& e) {
		LOGE("C++ 内存分配失败...%s", e.what());
		readerState = M_Read_OOM;
		this->CloseFile();
		return readerState;
	} catch (const std::ios_base::failure& e) {
		LOGE("C++ IO 异常...%s", e.what());
		readerState = M_Read_ANALYSIS_ERROR;
		this->CloseFile();
		return readerState;
	} catch (...) {
		LOGE("抱歉..未知错误...请联系开发人员..");
		readerState = M_Read_NO_DEFINE_Error;
		this->CloseFile();
		return readerState;
	}
	//CloseFile();
	//dumpMemoryLeaks();
	//if (this->m_Model && m_Model->GetSubModels().size()>0)
//	{
//		readerState = M_Read_OK;
//	}

	return readerState;
}

M3D_STATUS View::ReadFile(char * fileBuffer, int length) {
	this->GetPerspectiveOperator()->SetAllowUseAni(false);
	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;

	memoryLeakDetector();

	MutexLock locked(this->m_mutex);

	if (this->m_readListener) {
		this->m_readListener->SetReadFileCount(1);
		this->m_readListener->SetReadFileIndex(0);

		this->m_readListener->OnProcessPercent(NULL, 0);
	}

	try {
		SceneManager *scene = this->GetSceneManager();

		CloseFile();
		//dumpMemoryLeaks();
		//LOGI("View::ReadFile input path:%s ", path.c_str());
		//m_CurFilePath = path;
		m_CurFilePath = "readfrombuffer.svlx";
		LOGI("View::ReadFile path:%s ", m_CurFilePath.c_str());

		LOGI("Reader::GetReader start");
		m_Reader = Reader::GetReader(m_CurFilePath);

		SVL2AsynReader* svl2Reader = static_cast<SVL2AsynReader*>(m_Reader);
		if (svl2Reader) {
			svl2Reader->ReadFromBuffer(fileBuffer, length);
		}

		if (this->m_readListener) {
			m_Reader->SetListener(this->m_readListener);
		}

		LOGI("Reader::GetReader end");

		//m_Reader->SetFontPath(fontPath);
		//m_Reader->SetIDCreator(this->m_IDCreator);

		m_Reader->SetView(this);

		LOGI("m_Reader->GetModel() start");
		m_Model = m_Reader->GetModel();
		LOGI("m_Reader->GetModel() end");

		if (m_Model) {
			m_Model->AddRef();
		}

		//	LOGI("m_Reader->GetAllModelMap() start");
		//	m_AllModelMap = m_Reader->GetAllModelMap();
		//	LOGI("m_Reader->GetAllModelMap() end");
		LOGI("View::ReadFile getmodel ok.");
		if (m_Reader->IsCancel()) {
			LOGE("Read isCancel!");
			this->CloseFile();
			readerState = M3D_STATUS::M_Read_CANCEL;
			return readerState;
		}

		if (m_Model == NULL) {
			LOGE("Read is Error Model=NULL");
			this->CloseFile();
			readerState = M_Read_ANALYSIS_ERROR;
			return readerState;
		}

		//缓存动画文件
		this->SetAnimationXMLData(m_Reader->GetXMLAnimationData());
//        LOGI("read animationXML %s sfs", this->m_AnimationXMLData.c_str());
		//	LOGI("SetModel start");
		m_SceneManager->SetModel(m_Model);
		//	LOGI("View::ReadFile m_SceneManager->SetModel ok.");
		this->SetDefaultWorkNodes();

		m_SceneManager->OptimizeScene(m_Reader->IsAsynMode());

		if (this->m_readListener) {
			this->m_readListener->SetPercent(1.0f);
		}
		this->RestoreView(false);

		//this->m_isSpeedModel = Parameters::Instance()->m_simplityMode;

		if (/*this->m_isSpeedModel && */!this->m_Reader->IsAsynMode()) {
			//m_Model->Release();
			//m_Model = NULL;

			if (m_Reader != NULL) {
				delete m_Reader;
				m_Reader = NULL;
			}
		}

		//this->RestoreView();
		LOGE("readfile end");
	} catch (const std::bad_alloc& e) {
		LOGE("C++ 内存分配失败...%s", e.what());
		readerState = M_Read_OOM;
		this->CloseFile();
		return readerState;
	} catch (const std::ios_base::failure& e) {
		LOGE("C++ IO 异常...%s", e.what());
		readerState = M_Read_ANALYSIS_ERROR;
		this->CloseFile();
		return readerState;
	} catch (...) {
		LOGE("抱歉..未知错误...请联系开发人员..");
		readerState = M_Read_NO_DEFINE_Error;
		this->CloseFile();
		return readerState;
	}

	readerState = M_Read_OK;
	return readerState;
}

M3D_STATUS View::AppendReadFile(string & path) {
	MutexLock locked(this->m_mutex);

	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;

	try {
		SceneManager *scene = this->GetSceneManager();

		LOGI("View::ReadFile input path:%s ", path.c_str());

		m_CurFilePath = path;
		m_CurFilePath = M3D::FileHelper::GetUnionStylePath(m_CurFilePath);

		LOGI("View::ReadFile path:%s ", m_CurFilePath.c_str());

		LOGI("Reader::GetReader start");
		m_Reader = Reader::GetReader(m_CurFilePath);
		LOGI("Reader::GetReader end");

		//    m_Reader->SetFontPath(fontPath);
		//m_Reader->SetIDCreator(this->m_IDCreator);
		m_Reader->SetView(this);
		LOGI("m_Reader->GetModel() start");
		m_Model = m_Reader->GetModel();
		LOGI("m_Reader->GetModel() end");

		if (m_Model) {
			m_Model->AddRef();
		}

		//	LOGI("m_Reader->GetAllModelMap() start");
		//	m_AllModelMap = m_Reader->GetAllModelMap();
		//	LOGI("m_Reader->GetAllModelMap() end");
		LOGI("View::ReadFile getmodel ok.");
		if (m_Reader->IsCancel()) {
			LOGE("Read isCancel!");
			this->CloseFile();
			readerState = M3D_STATUS::M_Read_CANCEL;
			return readerState;
		}

		if (m_Model == NULL) {
			LOGE("Read is Error Model=NULL");
			this->CloseFile();
			readerState = M_Read_ANALYSIS_ERROR;
			return readerState;
		}

		//缓存动画文件
		//this->SetAnimationXMLData(m_Reader->GetXMLAnimationData());
//			LOGI("read animationXML %s sfs", this->m_AnimationXMLData.c_str());
		//	LOGI("SetModel start");
		m_SceneManager->AddModel(m_Model);
		//	LOGI("View::ReadFile m_SceneManager->SetModel ok.");
		//this->SetDefaultWorkNodes();
		this->RestoreView(false);
		//if(Parameters::Instance()->m_simplityMode)
		{
			m_Model->Release();
			m_Model = NULL;

			if (m_Reader != NULL) {
				delete m_Reader;
				m_Reader = NULL;
			}
		}

		LOGE("readfile end");
	} catch (const std::bad_alloc& e) {
		LOGE("C++ 内存分配失败...%s", e.what());
		readerState = M_Read_OOM;
		this->CloseFile();
		return readerState;
	} catch (const std::ios_base::failure& e) {
		LOGE("C++ IO 异常...%s", e.what());
		readerState = M_Read_ANALYSIS_ERROR;
		this->CloseFile();
		return readerState;
	} catch (...) {
		LOGE("抱歉..未知错误...请联系开发人员..");
		readerState = M_Read_NO_DEFINE_Error;
		this->CloseFile();
		return readerState;
	}

	readerState = M_Read_OK;
	return readerState;
}

M3D_STATUS View::SaveFile(string &path) {
	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;
	try {
		MutexLock locked(this->m_mutex);
		if (m_Model == NULL) {
			LOGE("View::SaveFile Error: m_Model==NULL");
			return readerState;
		}
		Writer *writer = Writer::GetWriter(m_CurFilePath);

		int ret = writer->SaveTo(this, path);
		if (ret != 0) {
			LOGE("View::SaveFile Error:  errorCode:%d", ret);
		}
		delete writer;
	} catch (...) {
		LOGE("抱歉..View::SaveFile(string &path)请联系开发人员..");
		readerState = M_Read_NO_DEFINE_Error;
		return readerState;
	}

	readerState = M_Read_OK;
	return readerState;
}

/**
 * @brief SVL文件保存为SVLX文件
 * @param tmpPath 临时路径
 * @param srcPath SVLX路径
 */
M3D_STATUS View::SaveSVLXFile(const string& tmpPath, const string &srcPath,
		const string &srcName) {
	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;
	try {
		MutexLock locked(this->m_mutex);
		if (m_Model == NULL) {
			LOGE("View::SaveFile Error: m_Model==NULL");
			return readerState;
		}
		SVLXWriter *writer = new SVLXWriter();

		int ret = writer->SaveToByIncrease(this, tmpPath, srcPath, srcName);

		if (ret != 0) {
			LOGE("View::SaveFile Error:  errorCode : %d", ret);
		}
		delete writer;
	} catch (...) {
		LOGE("抱歉..View::SaveFile(string &path)请联系开发人员..");
		readerState = M_Read_NO_DEFINE_Error;
		return readerState;
	}

	readerState = M_Read_OK;
	return readerState;
}
/**
 * @brief 添加文件到SVLX文件
 * @param srcPath 源文件路径
 * @param svlxPath SVLX路径
 * @param srcName 文件名称(完整-带后缀)
 */
M3D_STATUS View::AddFileToSVLX(const string& srcPath, const string &svlxPath,
		const string &srcName) {
	SVLXWriter *writer = new SVLXWriter();

	M3D_STATUS ret = (M3D_STATUS) (writer->AddFileToSVLX(this, srcPath,
			svlxPath, srcName));
	return ret;
}

M3D_STATUS View::SaveSVLFileToSVLX(const string& srcPath,
		const string &targetPath) {
	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;
	try {
		LOGI("View::SaveFile path:%s %s", srcPath.c_str(), targetPath.c_str());
		SVLXWriter *writer = new SVLXWriter();
		int ret = writer->SaveTo(this, srcPath, targetPath);

		if (ret != 0) {
			LOGE("View::SaveFile Error:  errorCode:%d", ret);
			ret = false;
		}
		ret = true;
		delete writer;
	} catch (...) {
		LOGE("抱歉..SaveSVLFileToSVLX..请联系开发人员..");
		readerState = M_Read_NO_DEFINE_Error;
		return readerState;
	}
	readerState = M_Read_OK;
	return readerState;
}

void View::CloseFile() {
	try {
		m_userDatas.clear();
		m_allGeasureNoteJsonData.clear();
		m_allTextNoteJsonData.clear();
		m_allSequeNoteJsonData.clear();
		if (m_ExplosiveViewOperator) {
			m_ExplosiveViewOperator->Reset();
		}
		if (this->m_Selector != NULL) {
			this->m_Selector->Clear();
		}

		this->m_TouchEventHandler->Reset();
		this->m_walkthroughTouchHandler->Reset();
		this->m_draggerHandler->Reset();
		//清空动画文件
		SetAnimationXMLData("");
		m_SceneManager->GetNoteGroup()->Clear();
		if (m_Reader != NULL) {
			delete m_Reader;
			m_Reader = NULL;
		}
		if (m_Model) {
			m_Model->Release();
		    m_Model = NULL;
		}
		if(m_curModelView){
			m_curModelView->Release();
            m_curModelView = NULL;
		}
		if (m_Model) {
		m_Model->Release();
		 m_Model = NULL;
		}
		m_SceneManager->RemoveModel();
		//int modelLightSize = m_SceneManager->GetModelLights()->size();
		//for (int i = 0; i < modelLightSize; i++)
		//{
		//	if (m_SceneManager->GetModelLights()->at(i))
		//	{
		//		(m_SceneManager->GetModelLights()->at(i))->Release();
		//		//	delete m_modelLists[i];
		//		//	m_modelLists[i] = NULL;
		//	}
		//}
		if (m_SceneManager->GetRenderManager()->GetRenderAction()->GetBackGroundNode()) {
			m_SceneManager->GetRenderManager()->GetRenderAction()->GetBackGroundNode()->ClearSkyBoxTexture();
		}

		m_SceneManager->GetLightManager()->Clear();
		//m_SceneManager->GetModelLights()->clear();
		if (m_modelManager) {
			m_modelManager->GetCommandHistoryManager()->Clear();
		}
		//关闭文件时，清空资源管理器中的资源
		this->m_SceneManager->GetResourceManager()->Clear();
		//ResourceManager::GetInstance()->Clear();
		IDCreator::Instance()->ResetSVLIDOffset();
	} catch (...) {
		LOGE("View::CloseFile Error()");
	}
	RestoreBackgroundState();
	RestoreViewModeState();

	LOGI("View::CloseFile()");

}

Selector* View::GetSelector() {
	return this->m_Selector;
}

IDCreator* View::GetIDCreator() {
	if (this->m_IDCreator == NULL) {
		//this->m_IDCreator = new IDCreator;
	}
	return this->m_IDCreator;
}

void View::TranslateSelectedShapes(float priX, float priY, float curX,
		float curY) {
	M3D::Vector3 startVec, endVec;
	CameraNode* camera = GetSceneManager()->GetCamera();
	const Viewport& viewPort = camera->GetViewPort();
	startVec = viewPort.ScreenToWorldPoint(priX, priY, camera->GetFitClip());
	endVec = viewPort.ScreenToWorldPoint(curX, curY, camera->GetFitClip());
	for (int i = 0; i < GetSelector()->GetAll().size(); i++) {
		IShape* shape = (IShape*) GetSelector()->GetAll().at(i);
		Model* model = static_cast<Model*>(shape);
		if (model && model->GetType() == SHAPE_MODEL) {
			Vector4 nD(endVec - startVec, 0.0f);
			//对于向量的变化量，应该使用此算法
			//而不是直接用矩阵乘vector3，得到的值
			Vector3 mov_ = model->GetWorldTransform().Inverse() * nD;
			ModelShape* shapeNode = ((Model*) shape)->GetModelShape();
			if (shapeNode) {
				shapeNode->Translate(mov_);
			}
			//model->Translate(mov_, TS_PARENT);
		}
	}
	this->RequestDraw();
}

void View::TranslateShape(float priX, float priY, float curX, float curY,
		int id, int type) {
	M3D::Vector3 startVec, endVec;

	CameraNode* camera = GetSceneManager()->GetCamera();
	const Viewport& viewPort = camera->GetViewPort();
	startVec = viewPort.ScreenToWorldPoint(priX, priY, camera->GetFitClip());
	endVec = viewPort.ScreenToWorldPoint(curX, curY, camera->GetFitClip());

	IShape* shape = GetSceneManager()->GetShape(id);
	if (type == 0) {
		M3D::Vector3 outStart, outEnd;
		if (shape != NULL && shape->GetType() == SHAPE_MODEL) {
			Vector4 nD(endVec - startVec, 0.0f);
			Model* model = static_cast<Model*>(shape);
			Vector3 mov_ = model->GetWorldTransform().Inverse() * nD;
			ModelShape* shapeNode = ((Model*) shape)->GetModelShape();
			if (shapeNode) {
				shapeNode->Translate(mov_);
			}
		}
	}
	//	else if (type == 1)
	//	{
	//		SceneNode *node = shape->GetSceneNode();
	//
	//		if (node != NULL)
	//		{
	//			node->Translate(endVec - startVec, TS_WORLD);
	//		}
	//	}
	this->RequestDraw();
}

void View::ResetSelectedShapes() {
	vector<IShape*> shapes = GetSelector()->GetAll();
	for (int i = 0; i < shapes.size(); i++) {
		Model* model = static_cast<Model*>(shapes[i]);
		if (model) {
			model->ResetMovement();
		}
	}
}

void View::ResetSelectedShapes(M3D::IShape* shape) {
	Model* model = static_cast<Model*>(shape);
	if (model) {
		model->ResetMovement();
	}
}

Parameters* View::GetParameters() {
	return this->m_Parameters;
}

Model * View::GetModel() {
	return this->m_Model;
}

//const map<string, Model*>& View::GetAllModelMap()
//{
//	return this->m_AllModelMap;
//}

map<int, PMIData*>* View::GetPMIMap() {
	return this->GetModel()->GetPMIs();
}

RenderManager *View::GetRenderManager() {
	return this->m_SceneManager->GetRenderManager();
}

SceneManager *View::GetSceneManager(void) {
	return this->m_SceneManager;
}

ModelManager * View::GetModelManager() {
	if (this->m_modelManager == NULL) {
		this->m_modelManager = new ModelManager(this);
	}
	return this->m_modelManager;
}

void View::SetSceneManager(SceneManager * sceneManager) {
	if (this->m_SceneManager != NULL) {
		delete this->m_SceneManager;
	}
	this->m_SceneManager = sceneManager;

	m_TouchEventHandler->SetView(this);

	m_walkthroughTouchHandler->SetView(this);

	m_draggerHandler->SetView(this);
}
void View::SetRenderManager(RenderManager* renderManager) {
	if (this->m_RenderManager != NULL) {
		delete this->m_RenderManager;
	}
	this->m_RenderManager = renderManager;
}

void View::SetModelManager(ModelManager * modelManager) {
	if (this->m_modelManager != NULL) {
		delete this->m_modelManager;
	}

	this->m_modelManager = modelManager;
}

void View::RestoreView() {
	this->RestoreView(true);
}

void View::RestoreView(bool useAni) {
	SceneManager *scene = this->GetSceneManager();
	scene->Lock();
	if (this->m_Model) {
		this->m_Model->Restore(true);
	}

	m_TouchEventHandler->Reset();
	m_walkthroughTouchHandler->Reset();
	m_draggerHandler->Reset();

	this->m_SceneManager->Reset();

	if (this->m_Selector) {
		this->m_Selector->Clear();
	}

	this->InitCamera(useAni);

	//TODO 默认灯光
	if (m_SceneManager) {
		if (m_SceneManager->GetLightManager() && false) {
			LightManager* lightManager = m_SceneManager->GetLightManager();
			BaseLight* light = NULL;
			BaseLight* ambientLight = NULL;
			BaseLight* hemisphereLight = NULL;
			PointLight* pointLight = NULL;
			SpotLight* spotLight = NULL;
			if (lightManager->GetAllLight().size() == 0) {
				////平行光测试
				//light = new DirectionalLight;
				//lightManager->AddLight(light);
				//DirectionalLight* tempLight = static_cast<DirectionalLight*>(light);
				//Vector3 center = m_SceneManager->GetSceneBox().Center();
				//light->SetNodeWorldPosition(center);
				//Vector3 direction(1, 1, 1);
				//direction.Normalize();
				//Quaternion r(45.0, Vector3(-1, 0, 0));
				//light->SetRotation(r);
				//float length = m_SceneManager->GetSceneBox().Length();
				////light->SetTargetPosition(center-direction*length*0.2);
				//tempLight->SetDiffuse(Color(1.0,1.0,1.0));
				//light->SetIntensity(1.0);

				////环境光测试
				////ambientLight = new AmbientLight;
				////ambientLight->SetLightColor(Color(0.2, 0.2, 0.2));
				////lightManager->AddLight(ambientLight);

				////半球光测试
				//hemisphereLight = new HemisphereLight;
				//hemisphereLight->SetNodeWorldPosition(Vector3(0, 1, 0));
				//hemisphereLight->SetIntensity(1.0);
				////lightManager->AddLight(hemisphereLight);
				//hemisphereLight->SetLightColor(Color(0xbbbbff));
				//HemisphereLight* hemisphereLight1 = static_cast<HemisphereLight*>(hemisphereLight);
				//hemisphereLight1->GroundColor(Color(0x444422));

				////点光源测试
				//pointLight = new PointLight;
				//pointLight->SetNodeWorldPosition(center );
				//pointLight->SetLightColor(Color(0xfffffff));
				//
				//pointLight->Distance(2000);
				//pointLight->Decay(1.0);
				//lightManager->AddLight(pointLight);

				////聚光灯测试
				//spotLight = new SpotLight;
				////spotLight->SetTargetPosition(center);
				//spotLight->SetNodeWorldPosition(center + direction*length*0.2);
				//spotLight->SetLightColor(Color(0xfffffff));
				//spotLight->Distance(2000);
				//spotLight->Decay(1.0);
				//spotLight->Angle(30.0);
				//spotLight->Penumbra(15.0);
				//spotLight->SetRotation(r);
				//lightManager->AddLight(spotLight);
			} else {
				//light = lightManager->GetAllLight()[0];
				//DirectionalLight* tempLight = static_cast<DirectionalLight*>(light);
				//Vector3 center = m_SceneManager->GetSceneBox().Center();
				//light->SetNodeWorldPosition(center);
				//Quaternion r(45.0, Vector3(-1, 0, 0));
				//light->SetRotation(r);
				//Vector3 direction(1, 1, 1);
				//direction.Normalize();
				//float length = m_SceneManager->GetSceneBox().Length();
				////light->SetTargetPosition(center -direction*length*0.2);
				//tempLight->SetDiffuse(Color(1.0, 1.0, 1.0));
				//light->SetIntensity(1.0);
				//light->OnMarkedDirty();
			}
		}
	}

	//reset
	if (m_pSAManager) {
		CAnimationStepManager* pStepAni =
				m_pSAManager->GetAnimationStepManager();
		if (pStepAni && pStepAni->GetCurProcessManagerID() != -1) {
			pStepAni->SetCurProcessManagerID(-1);
		}
	}
	scene->UnLock();
}

void View::RequestRestoreViewIfNeed() {
	this->GetPerspectiveOperator()->SetAllowUseAni(true);
	MutexLock locked(this->m_mutex);
	//if (m_firstReadBox != m_SceneManager->GetSceneBox())
	{
		//如果包围盒不是一个数目
		//if (m_SceneManager->GetSceneBox().Length() != m_SceneManager->GetSceneBox().Length())
		{
			m_SceneManager->GetSceneBox().Clear();
		}

		m_TouchEventHandler->Reset();
		m_walkthroughTouchHandler->Reset();
		m_draggerHandler->Reset();

		this->m_SceneManager->Reset();
		this->InitCamera();
	}
}

void View::SetDefaultRotateCenter() {
	if (this->m_SceneManager != NULL) {
		this->m_SceneManager->RestoreRotationCenter();
	}
}

bool View::SetRotationCenter(float x, float y, int centerType) {
	if (this->m_SceneManager != NULL) {
		bool state = this->m_SceneManager->SetRotationCenter(x, y, centerType);
		if (state == true) {
			this->m_SceneManager->GetRenderManager()->RequestRedraw();
			return true;
		}
	}
	return false;
}

IShape* View::GetPickShape(float x, float y, int shapeType, int geoType) {
	if (this->m_SceneManager != NULL) {
		return this->m_SceneManager->GetPickShape(x, y, shapeType, geoType);
	}

	return NULL;
}

vector<IShape*> View::GetFramePickShape(const Vector2& leftTop,
		const Vector2& rightBottom, int shapeType, int geoType,
		int framePickType) {
	if (this->m_SceneManager != NULL) {
		return this->m_SceneManager->GetFramePickShape(leftTop, rightBottom,
				shapeType, geoType, framePickType);
	}

	vector<IShape*> ret;
	return ret;
}

void View::SetSelectType(int type) {
	if (m_SceneManager != NULL) {
		return m_SceneManager->SetSelectType(type);
	}
}

void View::SetClipPlane(int direction, float position, bool isShowClipPlane,
		bool isShowCappingPlane, bool isReverseClip) {
	int sectionPlaneId = 1001;
	M3D::SectionOperator::Show(this, sectionPlaneId, direction, position,
			isShowClipPlane, isShowCappingPlane, isReverseClip);
}

void View::SetMultiClipPlane(int directionX, int directionY, int directionZ,
		float positionX, float positionY, float positionZ, bool isShowClipPlane,
		bool isShowCappingPlane, bool isReverseClip) {
	int dX = directionX, dY = directionY, dZ = directionZ;
	int sectionPlaneId = 1001;
	if (directionX == 1)
		directionX = Direction::X;
	if (directionX == -1)
		directionX = -Direction::X;
	M3D::SectionOperator::Show(this, sectionPlaneId++, directionX, positionX,
			isShowClipPlane, isShowCappingPlane, isReverseClip);

	if (directionY == 1)
		directionY = Direction::Y;
	if (directionY == -1)
		directionY = -Direction::Y;
	M3D::SectionOperator::Show(this, sectionPlaneId++, directionY, positionY,
			isShowClipPlane, isShowCappingPlane, isReverseClip);

	if (directionZ == 1)
		directionZ = Direction::Z;
	if (directionZ == -1)
		directionZ = -Direction::Z;
	M3D::SectionOperator::Show(this, sectionPlaneId, directionZ, positionZ,
			isShowClipPlane, isShowCappingPlane, isReverseClip);

	SectionOperator::Instance->m_Direction = 0;
	SectionOperator::Instance->m_DirectionX = dX;
	SectionOperator::Instance->m_DirectionY = dY;
	SectionOperator::Instance->m_DirectionZ = dZ;
	SectionOperator::Instance->m_fPercentageX = positionX;
	SectionOperator::Instance->m_fPercentageY = positionY;
	SectionOperator::Instance->m_fPercentageZ = positionZ;
}

void View::ClearClipPlane() {
	M3D::SectionOperator::Clear(this);
}

//获得和设置实例的矩阵
void View::SetMatrixByPlcPath(char* plcPath, float matrix[4][4]) {
	if (this->m_SceneManager != NULL) {
		//		SceneNode* node = this->m_SceneManager->GetNode(
		//				"M3D|MAIN|" + string(plcPath));
		//		if (node != NULL)
		//		{
		//			//LOGE("node is not null");
		//			float transPosM[16];
		//			for (int i = 0; i < 4; i++)
		//			{
		//				for (int j = 0; j < 4; j++)
		//				{
		//					transPosM[j * 4 + i] = matrix[i][j];
		//				}
		//			}
		//			node->GetPlcMatrix().Set(transPosM);
		//		}
	}
}
void View::GetMatrixByPlcPath(char* plcPath, float matrix[4][4]) {
	if (this->m_SceneManager != NULL) {
		Model* node = this->m_SceneManager->GetNode(
				"M3D|MAIN|" + string(plcPath));
		//		if (node != NULL)
		//		{
		//			//LOGE("node is not null");
		//			const float * plcMatrix = node->GetPlcMatrix().Data();
		//
		//			for (int i = 0; i < 4; i++)
		//			{
		//				for (int j = 0; j < 4; j++)
		//				{
		//					matrix[i][j] = plcMatrix[j * 4 + i];
		//				}
		//			}
		//		}
	}
}

//修改场景的矩阵实线移动和旋转功能
void View::SetSceneMatrix(float matrix[4][4]) {
	//关闭场景动画，如持续旋转操作
	this->CloseSceneAnimation();

	if (this->m_SceneManager != NULL) {
		Matrix3x4 viewMatrix((float*) matrix);
		//		viewMatrix = viewMatrix.Inverse();
		Quaternion rot = viewMatrix.Rotation();

		CameraNode* camera = this->m_SceneManager->GetCamera();
		camera->SetRotation(rot);
		camera->SetWorldPosition(viewMatrix.Translation());
		//m_pWorkNodeGroup->SetPosition(viewMatrix.Translation());
		//	LOGE("camera matrix %s",viewMatrix.ToString().c_str());
	}
}
void View::SetSceneScale(float scales[3]) {
	float scale = scales[0];

	if (this->m_SceneManager != NULL) {
		CameraNode* camera = this->m_SceneManager->GetCamera();
		camera->SetZoom(1 / scale);
	}
}

//设置视图
void View::SetPerspective(int viewType) {
	this->CloseSceneAnimation();
	this->GetPerspectiveOperator()->Show(this, viewType);
}

int View::GetDrawMode() {
	return m_curDrawMode;
}

void View::SetDrawMode(int drawMode) {
	MutexLock locked(this->m_mutex);
	if (drawMode != -1) {
		m_curDrawMode = drawMode;
	}

	RenderEffect* renderType =
			m_SceneManager->GetRenderManager()->GetRenderEffect();
	if (!renderType) {
		return;
	}
	int currentType;
	RenderableTypeGroup& renderData = renderType->GetRenderableTypeFilter();
	switch (drawMode) {
	case 0: //Solid
			//if (m_Model != NULL)
			//	m_Model->ResetAlpha();
		renderData.SetRenderTypes(RenderableTypeGroup::RENDERDATA_NORMALEFFCT);
		renderData.Close(RenderableType::RGT_EDGELINE);
		if (Parameters::Instance()->m_IsShowTrilateralEdge == true) {
			RenderManager::isShowTriLine = true;
			renderData.Open(RenderableType::RGT_TRILINE);
		}
		break;
	case 1: //Transparent
		if (m_Model != NULL)
			m_Model->SetAlpha(Parameters::Instance()->m_Alpha);
		//renderData.SetRenderTypes(RenderableTypeGroup::RENDERDATA_NORMALEFFCT);
		//renderData.Open(RenderableType::RGT_SOLID_TRAN);

		break;
	case 2: //OnlyShowTrilateralEdge
		renderData.SetRenderTypes(RenderableTypeGroup::RENDERDATA_NORMALEFFCT);
		renderData.Close(RenderableType::RGT_SOLID);
		renderData.Close(RenderableType::RGT_TRANSPARENT);

		break;
	case 3: //WireFrame
		renderData.SetRenderTypes(RenderableTypeGroup::RENDERDATA_NORMALEFFCT);
		renderData.Close(RenderableType::RGT_SOLID);

		break;
	case 4: //Normaleffct
		renderData.SetRenderTypes(RenderableTypeGroup::RENDERDATA_NORMALEFFCT);
		if (Parameters::Instance()->m_IsShowTrilateralEdge == true) {
			RenderManager::isShowTriLine = true;
			renderData.Open(RenderableType::RGT_TRILINE);
		}
		break;
	case 5: //
			//renderData.SetRenderTypes(RenderableTypeGroup::RENDERDATA_NORMALEFFCT);
			//三角网格显示
		Parameters::Instance()->m_IsShowTrilateralEdge =
				!Parameters::Instance()->m_IsShowTrilateralEdge;

		if (Parameters::Instance()->m_IsShowTrilateralEdge == true) {
			RenderManager::isShowTriLine = true;
			renderData.Open(RenderableType::RGT_TRILINE);
		} else if (Parameters::Instance()->m_IsShowTrilateralEdge == false) {
			RenderManager::isShowTriLine = false;
			renderData.Close(RenderableType::RGT_TRILINE);
		}
		break;
	case 6: //TransparentReset
		if (m_Model != NULL)
			m_Model->ResetAlpha();
		break;
	case 7: //
			//包围盒显示
			//renderData.Open(RenderableType::RGT_BOX);
		Parameters::Instance()->m_IsShowBox =
				!Parameters::Instance()->m_IsShowBox;
		if (Parameters::Instance()->m_IsShowBox == true) {
			renderData.Open(RenderableType::RGT_BOX);
		} else if (Parameters::Instance()->m_IsShowBox == false) {
			renderData.Close(RenderableType::RGT_BOX);
		}
		break;
	default:
		break;
	}

	//三角网格显示
	if (Parameters::Instance()->m_IsShowTrilateralEdge == true) {
		RenderManager::isShowTriLine = true;
		renderData.Open(RenderableType::RGT_TRILINE);
	} else if (Parameters::Instance()->m_IsShowTrilateralEdge == false) {
		RenderManager::isShowTriLine = false;
		renderData.Close(RenderableType::RGT_TRILINE);
	}

	//显示边界线控制
	//	if (Parameters::Instance()->m_IsShowEdgeLine == true)
	//	{
	//		renderData.Open(RenderableType::RGT_EDGELINE);
	//	}
	//	else if (Parameters::Instance()->m_IsShowEdgeLine == false)
	//	{
	//		renderData.Close(RenderableType::RGT_EDGELINE);
	//	}

	//半透明显示控制
	//显示包围盒控制
	if (Parameters::Instance()->m_IsShowBox == true) {
		renderData.Open(RenderableType::RGT_BOX);
	} else if (Parameters::Instance()->m_IsShowBox == false) {
		renderData.Close(RenderableType::RGT_BOX);
	}

	//显示PMI控制
	if (Parameters::Instance()->m_IsShowPMIs == true) {
		renderData.Open(RenderableType::RGT_PMI);
	} else if (Parameters::Instance()->m_IsShowPMIs == false) {
		renderData.Close(RenderableType::RGT_PMI);
	}

	//通知一下，显示数据需要改变了
	if (this->GetModel()) {
		this->GetModel()->MarkDrawDataDirty(true);
	}

	this->m_SceneManager->GetRenderManager()->RequestRedraw();
}
//设置PMI可见性
bool View::SetPMIVisible(int pmiID, bool visible) {
	//LOGI("View::SetPMIVisible cnt:%d pmiID:%d",this->GetModel()->m_AllPMIMap.size(),pmiID);
	bool ret = true;
	map<int, PMIData*>* pmis = this->GetModel()->GetPMIs();
	map<int, PMIData*>::iterator it = pmis->find(pmiID);
	if (it != pmis->end()) {
		PMIData* pmi = (*it).second;
		//LOGE("View::SetPMIVisible %s %d: visiable %d",pmi->GetName(),pmiID , visable);
		pmi->SetVisible(visible);
	} else {
		ret = false;
	}

	//LOGI("View::SetPMIVisible cnt:%d  ret:%d",this->GetModel()->m_AllPMIMap.size(),ret);
	return ret;
}
void View::SaveSettingXML() {
	Parameters::Instance()->SaveToXML();
}

int View::AddHandle(float x, float y, int type) {
	int ret = this->GetSceneManager()->AddHandle(x, y, type);
	return ret;
}

Vector3 View::GetSelectedPoint(float x, float y, int type,
		bool inModel/* = true*/) {
	M3D::Vector3 vec = this->GetSceneManager()->GetPickPoint(x, y, inModel);

	return vec;
}

Vector3 View::GetUILayerSelectedPoint(float x, float y) {
	M3D::Vector3 vec = this->GetSceneManager()->GetUILayerPickPoint(x, y);
	return vec;
}

int View::AddHandle(float x, float y, float z, int type) {
	return this->GetSceneManager()->AddShape(x, y, z, type);
}

void View::RemoveHandle(int id) {
	this->GetSceneManager()->RemoveHandle(id);
}

int View::AddShape(float x, float y, int type) {
	int ret = this->GetSceneManager()->AddShape(x, y, type);

	return ret;
}

int View::AddShape(float x, float y, float z, int type) {
	LOGE("View::AddShape x y z ");
	int ret = this->GetSceneManager()->AddShape(x, y, z, type);
	return ret;
}

void View::RemoveShape(int id) {
        bool isRemove = true;
        bool isNote = false;
        //当前视图关联note
	if (m_curModelView) {
		vector<int> ids = m_curModelView->GetNoteList();
		vector<int>::iterator it = find(ids.begin(), ids.end(), id);
		if (it != ids.end()) {
                isRemove = true;
                isNote = true;
		}
	}
        //判断是否和其他视图关联
        vector<ModelView*>* allViews = this->GetSceneManager()->GetModel()->GetModelViewList();
        if (allViews) {
		for (vector<ModelView*>::iterator it = allViews->begin(); it != allViews->end(); it++) {
                if (m_curModelView && (*it) != m_curModelView) {
                    vector<int> ids = (*it)->GetNoteList();
                    vector<int>::iterator result = find( ids.begin( ), ids.end( ), id ); //查找
				if ( result != ids.end( ) ) { //找到
                        isRemove = false;
                        break;
                    }
                }
            }
        }
	LOGE("View::RemoveShape");
        //TODO 视图关联note是否可删除
        if (isRemove == true) {
		this->GetSceneManager()->RemoveShape(id);
        }else if (isNote){
            //显示关联的批注信息
            NoteGroup* pNoteGroup = this->GetSceneManager()->GetNoteGroup();
		for (int j = 0; j < pNoteGroup->Size(); j++) {
                SceneNode* pNode = pNoteGroup->GetChild(j);
                if (!pNode)
                    continue;
                IShape* pShape = ((ShapeNode*)pNode)->GetShape();
			if (pShape && id == pShape->GetID()) {
                    pNode->SetVisible(false);
                    pShape->SetVisible(false);
                    break;
                }
            }
        }
        if (isNote) {
            if (m_curModelView) {
                vector<int> ids = m_curModelView->GetNoteList();
                vector<int>::iterator it = find(ids.begin(),ids.end(),id);
                if (it != ids.end()) {
                    ids.erase(it);
                    m_curModelView->SetNoteIds(ids);
                }
            }
        }
	}

void View::GetShapePos(int id, int type, float* p) {
	M3D::Vector3 pos;
	this->GetSceneManager()->GetShapePos(id, type, pos);

	p[0] = pos.m_x;
	p[1] = pos.m_y;
	p[2] = pos.m_z;
}

int View::AddLineLengthNote(int lineID, float x, float y) {

	return -1;
}
int View::AddRadiusLengthNote(int radiusID, float x, float y) {
	return -1;
}

void View::moveLineLengthNote(int lineNoteID, float x, float y) {

}
void View::moveRadiusLengthNote(int radiusNoteID, float x, float y) {

}

Color View::GetShapeColor(int id, int type) {
	return this->GetSceneManager()->GetShapeColor(id, type);
}

void View::AddPoint(int parentId, int parentType, float pntBuf[]) {
	LOGI("View::addPoint parentId:%d,parentType:%d", parentId, parentType);
	if (pntBuf != NULL) {
		Vector3* pnt = new Vector3(pntBuf[0], pntBuf[1], pntBuf[2]);
		switch (parentType) {
		case SHAPE_NOTE:
			Note* note = (Note*) this->m_SceneManager->GetShape(parentId);
			if (note != NULL) {
				//				note->AddPoint(pnt);
			}
			break;
			//			default:
			//				break;
		}
	}
}

void View::AddLine(int parentId, int parentType, float startPointBuf[],
		float endPointBuf[], int width, int startArrow, int endArrow) {
	Vector3 pnt1(startPointBuf[0], startPointBuf[1], startPointBuf[2]);
	Vector3 pnt2(endPointBuf[0], endPointBuf[1], endPointBuf[2]);
	Line3D* line = new Line3D(&pnt1, &pnt2);
	line->SetStartArrow(startArrow);
	line->SetEndArrow(endArrow);

	switch (parentType) {
	case SHAPE_NOTE:
		Note* note = (Note*) this->m_SceneManager->GetShape(parentId);
		if (note != NULL) {
			note->AddLine(line);
		}
		break;
	}
}

void View::AddPolyLine(int parentId, int parentType, int pointCnt,
		float pointBuf[], int indexCnt, M3D_INDEX_TYPE indexBuf[]) {
	LOGI("View::AddPolyLine");
	PolyLine *pPolyLine = new PolyLine();
	pPolyLine->SetBuffer(pointCnt, pointBuf, indexCnt, indexBuf);

	switch (parentType) {
	case SHAPE_NOTE:
		Note* note = (Note*) this->m_SceneManager->GetShape(parentId);
		if (note != NULL) {
			note->AddPolyLine(pPolyLine);
		}
		break;
	}
}

void View::AddText(int parentId, int parentType, float posBuf[], string text,
		float fontSize) {
	LOGI("View::AddText '%s'", text.c_str());
	//临时这样用
	ComText* ct = new ComText();
	CText* t = new CText;

	//LOGI("View::AddText2 '%s'",wstring2string(ws).c_str());
	t->SetText(text);

	Vector3 loc(posBuf[0], posBuf[1], posBuf[2]);
	t->SetInnerLoc(loc);

	Vector3 xAxis, yAxis;
	xAxis.ToZero();
	t->SetInnerXYAxis(xAxis, yAxis);

	//width height
	t->SetCharWidthHeight(fontSize, fontSize);
	ct->AddCText(t);

	Note* note = (Note*) this->m_SceneManager->GetShape(parentId);
	if (note != NULL) {
		if (note->GetType() == SHAPE_NOTE) {
			note->AddText(ct);
		}
	}

}
//------------------------------------------ModelView----------------------------------------------------
void View::ShowModelView(int viewId, bool isAnni) {
	this->m_SceneManager->Lock();

	LOGI("SceneManager::showModelView viewId:%d", viewId);
	Model* curRootModel = this->m_SceneManager->GetModel();
	//根据id获得当前视图
	ModelView *pView = curRootModel->GetModleView(viewId);
	if (NULL == pView) {
		LOGE("modleViewId:%d not found!", viewId);
		this->m_SceneManager->UnLock();
		return;
	}
	m_curModelView = pView;
	//老版本设计器中创建的剖面也保存为自定义视图，剖面数据不应执行视图切换处理
	if (pView->GetSvlUseType()
			== M3D::ModelView::ViewSvlUseTypeEnum::VIEW_USAGE_USER_CLIPPLANE) {
		this->m_SceneManager->UnLock();
		return;
	}
	if (isAnni) {
		//如果需要更新摄像机
		if (pView->GetUpDataCameraState()) {
			//获得视图中的相机参数
			const CameraNode &camera = pView->GetCamera();

			//先把摄像机复位下，然后仅设置摄像机位置和旋转和缩放信息
			//	GetSceneManager()->ResetCamera();
			//this->ReSetCamera();

			CameraNode *currentCamera = GetSceneManager()->GetCamera();

			Vector3 cameraNewPos = camera.GetPosition();

			currentCamera->SetPosition(cameraNewPos);
			currentCamera->SetRotation(camera.GetRotation());

			currentCamera->SetOrthographic(camera.IsOrthographic());
			if (currentCamera->IsOrthographic()) {
				currentCamera->SetZoom(
						currentCamera->GetOrthoSize() / camera.GetOrthoSize()
								* camera.GetZoom());
			} else {
				//currentCamera->SetZoom(camera.GetZoom());
				float defaultZoom = m_SceneManager->GetDefaultZoom();
				currentCamera->SetZoom(defaultZoom);
			}

			//如果是平行投影，进行显示矫正
			if (camera.IsOrthographic()) {
				BoundingBox& sceneBox = this->m_SceneManager->GetSceneBox();
					Vector3 center = sceneBox.Center();
				Vector3 cameraPos = currentCamera->GetPosition();
				//如果摄像机在模型内部，则进行调整
				if (sceneBox.IsInside(cameraPos) == INSIDE) {
					Matrix3 viewMat = currentCamera->GetView().ToMatrix3();
					Vector3 direction(viewMat.m_m20, viewMat.m_m21,
							viewMat.m_m22);
					direction.Normalize();
					Vector3 newPos = cameraPos + direction * sceneBox.Length();
					currentCamera->SetWorldPosition(newPos);
				}
					else
					{
						//如果超出视景体，进行调整
						Matrix3 viewMat = currentCamera->GetView().ToMatrix3();
						Vector3 direction(viewMat.m_m20, viewMat.m_m21, viewMat.m_m22);
						Plane centerPlane(direction, center);
						Vector3 targetPnt = centerPlane.Project(cameraNewPos);

						float fHalfViewSize = currentCamera->GetHalfViewSize();
						Vector3 viewMin(center.m_x - fHalfViewSize, center.m_y - fHalfViewSize, center.m_z - fHalfViewSize);
						Vector3 viewMax(center.m_x + fHalfViewSize, center.m_y + fHalfViewSize, center.m_z + fHalfViewSize);
						BoundingBox viewBox(viewMin, viewMax);
						if (viewBox.IsInside(targetPnt) != INSIDE)
						{
							cameraNewPos = sceneBox.Center() + direction * sceneBox.Length()* 0.8f;
							currentCamera->SetWorldPosition(cameraNewPos);
						}
						
					}
				}
			}

		if (pView->GetUpDataModelState()) {

			//TODO:相机设置旋转动画

			//insAtt
			const map<int, InstanceAttribute>& insAttMap =
					pView->GetInstanceAttributeMap();
			for (map<int, InstanceAttribute>::const_iterator it =
					insAttMap.begin(); it != insAttMap.end(); it++) {
				const InstanceAttribute &curInsAtt = it->second;

				IShape* shape = this->GetShapeBySVLPath(curInsAtt.path);
				if (shape && shape->GetType() == SHAPE_MODEL) {
					Model *curModel = (Model*) shape;
					curModel->SetVisible(curInsAtt.visible, false);
					if (curInsAtt.hasColor) {
						Color tmpColor = curInsAtt.insColor;
                        //TODO 贴图类型
                        if (tmpColor != Color(0.0, 0.0, 0.0, 1.0)) {
						curModel->SetColor(tmpColor);
					}
					}
					//ModelShape* shapeNode = curModel->GetModelShape();
					//if (shapeNode)
					//{
					//	Matrix3x4& modelTransform = curModel->GetWorldTransform();
					//	Matrix3x4 transform = curInsAtt.placeMatrix;
					//	transform = modelTransform.Inverse()*transform;
					//	shapeNode->SetWorldMatrix(transform);
					//}

					Matrix3x4 transform = curInsAtt.placeMatrix;
					if ((int) transform.m_m00 == 0 && (int) transform.m_m01 == 0
							&& (int) transform.m_m02 == 0
							&& (int) transform.m_m03 == 0
							&& (int) transform.m_m10 == 0
							&& (int) transform.m_m11 == 0
							&& (int) transform.m_m12 == 0
							&& (int) transform.m_m13 == 0
							&& (int) transform.m_m20 == 0
							&& (int) transform.m_m21 == 0
							&& (int) transform.m_m22 == 0
							&& (int) transform.m_m23 == 0) {
						continue;
					}
					ModelShape* modelshape = curModel->GetModelShape();
					if (modelshape) {
						modelshape->SetTargetWorldMatrix(transform);
					}
//                        curModel->SetPlaceMatrix(transform);
				}
			}
		}

	} else {
		ShowModelViewAnimation(pView);
	}
	//	LOGE("SceneManager::showModelView clipPlaneIdList 11" );
	//设置视图中的剖面为启用
//
// TODO 剖面后期完善
	vector<int> clipPlaneIdList = pView->GetSectionPlaneIDList();
	this->m_SceneManager->GetSectionNode()->GetSection()->ClearPlanes();
//    if (clipPlaneIdList.size() > 0) {
//        //        LOGE("SceneManager::showModelView clipPlaneIdList 2" );
//        for (int i = 0; i < curRootModel->GetSectionPlaneList()->size(); i++) {
//            SectionPlane* curPlane = curRootModel->GetSectionPlaneList()->at(i);
//            for (int j = 0; j < clipPlaneIdList.size(); j++) {
//                if (clipPlaneIdList.at(j) == curPlane->GetID()) {
//                    //                    LOGE("SceneManager::showModelView clipPlaneIdList 3" );
//                    this->m_SceneManager->GetSectionNode()->GetSection()->AddPlane(
//                            curPlane);
//                    //                    LOGE("SceneManager::showModelView clipPlaneIdList 4" );
//                    //                    LOGI("SceneManager::showModelView enable plane:%d param:%f,%f,%f,%f", curPlane->GetID(), *curPlane->GetEquation(), *(curPlane->GetEquation()+1), *(curPlane->GetEquation()+2), *(curPlane->GetEquation()+3));
//                    break;
//                }
//            }
//        }
//    }

	if (pView->GetUpDataModelState()) {
		//设置与视图关联的PMI
		string PMIName("");
		int displayPMIId;

		map<int, PMIData*>* pmis = curRootModel->GetPMIs();

		if (pmis != NULL && pmis->size() > 0) {
			//如果视图名为“DEFAULT”则显示所有PMI
			if (pView->GetViewType() == 0) {
				for (map<int, PMIData*>::iterator itPMI = pmis->begin();
						itPMI != pmis->end(); itPMI++) {
					(*itPMI).second->SetVisible(true);
				}
			} else { //否则只显示当前视图关联的PMI
					 //首先所有PMI设为隐藏
				for (map<int, PMIData*>::iterator itPMI = pmis->begin();
						itPMI != pmis->end(); itPMI++) {
					(*itPMI).second->SetVisible(false);
				}

				//显示关联的PMI
				for (int i = 0; i < pView->GetPMIList().size(); i++) {
					displayPMIId = pView->GetPMIList().at(i);
					//循环PMI列表，将指定Id的PMI设置为可见
					map<int, PMIData*>::iterator it2 = pmis->find(displayPMIId);
					if (it2 != pmis->end()) {
						PMIData* curTmpPMI = (*it2).second;
						if (curTmpPMI != NULL) {
							curTmpPMI->SetVisible(true);
						}
					}
				}
			}
		}

		//显示关联的批注信息
		NoteGroup* pNoteGroup = this->GetSceneManager()->GetNoteGroup();
		if (pNoteGroup) {
			int noteCount = pNoteGroup->Size();
			//隐藏所有批注
			for (int i = 0; i < pNoteGroup->Size(); i++) {
				SceneNode* pNode = pNoteGroup->GetChild(i);
				if (!pNode)
					continue;
				pNode->SetVisible(false);
			}

			int iNoteCount = 0;
			vector<int> vecNoteList = pView->GetNoteList();
			iNoteCount = vecNoteList.size();
			for (int i = 0; i < iNoteCount; i++) {
				int iNoteID = vecNoteList[i];
				bool isCreate = true;
				for (int j = 0; j < pNoteGroup->Size(); j++) {
					SceneNode* pNode = pNoteGroup->GetChild(j);
					if (!pNode)
						continue;
					IShape* pShape = ((ShapeNode*) pNode)->GetShape();
					if (pShape && iNoteID == pShape->GetID()) {
						pNode->SetVisible(true);
						pShape->SetVisible(true);
						isCreate = false;
						break;
					}
				}
				if (isCreate
						&& GetTextJsonData(StringHelper::IntToString(iNoteID))
								!= M3D::NO_VALUE) {
					string jsonValue = GetTextJsonData(
							StringHelper::IntToString(iNoteID));
					Note *pNode = NoteFactory::CreateTextNoteFromJSON(
							this->GetSceneManager(), jsonValue);
				}
				if (isCreate
						&& GetSequenceJsonData(
								StringHelper::IntToString(iNoteID))
								!= M3D::NO_VALUE) {
					string jsonValue = GetSequenceJsonData(
							StringHelper::IntToString(iNoteID));
					Note *pNode = NoteFactory::CreateSequenceNoteFromJSON(
							this->GetSceneManager(), jsonValue);
				}
				if (isCreate
						&& GetGestureJsonData(
								StringHelper::IntToString(iNoteID))
								!= M3D::NO_VALUE) {
					string jsonValue = GetGestureJsonData(
							StringHelper::IntToString(iNoteID));
					Note *pNode = NoteFactory::CreateThreeDGestureNoteFromJson(
							this->GetSceneManager(), jsonValue);
				}
			}
		}

		//文本批注
		AnnotationGroup* pAnnotationGroup =
				this->GetSceneManager()->GetAnnotationGroup();
		if (pAnnotationGroup) {
			int iAnnotationCount = pAnnotationGroup->Size();
			//隐藏所有批注
			for (int i = 0; i < iAnnotationCount; i++) {
				SceneNode* pNode = pAnnotationGroup->GetChild(i);
				if (!pNode)
					continue;
				pNode->SetVisible(false);
			}

			int iNoteCount = 0;
			vector<int> vecNoteList = pView->GetNoteList();
			iNoteCount = vecNoteList.size();
			for (int i = 0; i < iNoteCount; i++) {
				int iNoteID = vecNoteList[i];
				for (int j = 0; j < pAnnotationGroup->Size(); j++) {
					SceneNode* pNode = pAnnotationGroup->GetChild(j);
					if (!pNode)
						continue;
					IShape* pShape = ((ShapeNode*) pNode)->GetShape();
					if (pShape && iNoteID == pShape->GetID()) {
						pNode->SetVisible(true);
						break;
					}
				}
			}
		}

		//测量批注
		MeasureGroup* pMeasureGroup =
				this->GetSceneManager()->GetMeasureGroup();
		if (pMeasureGroup) {
			int iMeasureCount = pMeasureGroup->Size();
			//隐藏所有批注
			for (int i = 0; i < iMeasureCount; i++) {
				SceneNode* pNode = pMeasureGroup->GetChild(i);
				if (!pNode)
					continue;
				pNode->SetVisible(false);
			}

			int iNoteCount = 0;
			vector<int> vecNoteList = pView->GetNoteList();
			iNoteCount = vecNoteList.size();
			for (int i = 0; i < iNoteCount; i++) {
				int iNoteID = vecNoteList[i];
				for (int j = 0; j < pMeasureGroup->Size(); j++) {
					SceneNode* pNode = pMeasureGroup->GetChild(j);
					if (!pNode)
						continue;
					IShape* pShape = ((ShapeNode*) pNode)->GetShape();
					if (pShape && iNoteID == pShape->GetID()) {
						pNode->SetVisible(true);
						break;
					}
				}
			}
		}
		this->m_SceneManager->GetRenderManager()->RequestRedraw();
	}

	//	//允许动画
	//	if (isAnni)
	//	{
	//		ViewStateOperateAction::CreateAnnitation(pView, this->m_SceneManager,
	//				10);
	//	}
	this->m_SceneManager->UnLock();
}

void View::ShowModelViewAnimation(ModelView *pView) {
	if (!pView)
		return;
	int iCurrentBehaviorActionID = m_pSAManager->GetCurSAID();
	NS_SimulationAnimation::CSBehaviorAction* pChgViewBehaviorAction =
			m_pSAManager->GetAnimationStepManager()->GetBehaviorActionChgCam();

	if (!pChgViewBehaviorAction) {
		return;
	}

	pChgViewBehaviorAction->DeleteAllAnimations();
	CAnimationAPI* pAnimationAPI = CAnimationAPI::GetInstance();
	bool bTempShowRecUI = pAnimationAPI->m_bShowRecUI;
	//int iTempNumAutoKeyFrame = pAnimationAPI->m_numAutoKeyFrame;
	//pAnimationAPI->m_numAutoKeyFrame = 5;
	pAnimationAPI->m_bShowRecUI = true;
	//如果需要更新摄像机
	if (pView->GetUpDataCameraState()) {
		//获得视图中的相机参数
		const CameraNode &camera = pView->GetCamera();
		CameraNode *currentCamera = GetSceneManager()->GetCamera();

		currentCamera->SetOrthographic(camera.IsOrthographic());

		BoundingBox& sceneBox = this->m_SceneManager->GetSceneBox();
		Vector3 center = sceneBox.Center();

		float fFlocalDistance = sceneBox.Size().Length() * 0.8f;
		Vector3 curCamPos = currentCamera->GetPosition();
		Quaternion curCamRotaion = currentCamera->GetRotation();
		Matrix4 curCamMatrix(curCamPos, curCamRotaion, 1.0f);
		Vector3 curDirection(-curCamMatrix.m_m02, -curCamMatrix.m_m12,
				-curCamMatrix.m_m22);
		curCamPos = curCamPos.Add(curDirection.Multiply(fFlocalDistance));
		//curCamMatrix.SetTranslation(curCamPos);
		curCamMatrix = curCamMatrix.Transpose();

		float fCenter[3];
		fCenter[0] = center.m_x;
		fCenter[1] = center.m_y;
		fCenter[2] = center.m_z;

		float fCurMatrix[4][4];
		float fCurScale = 1.0f;
		float fCurInitWind[2];

		fCurMatrix[0][0] = curCamMatrix.m_m00;
		fCurMatrix[0][1] = curCamMatrix.m_m01;
		fCurMatrix[0][2] = curCamMatrix.m_m02;
		fCurMatrix[0][3] = curCamMatrix.m_m03;
		fCurMatrix[1][0] = curCamMatrix.m_m10;
		fCurMatrix[1][1] = curCamMatrix.m_m11;
		fCurMatrix[1][2] = curCamMatrix.m_m12;
		fCurMatrix[1][3] = curCamMatrix.m_m13;
		fCurMatrix[2][0] = curCamMatrix.m_m20;
		fCurMatrix[2][1] = curCamMatrix.m_m21;
		fCurMatrix[2][2] = curCamMatrix.m_m22;
		fCurMatrix[2][3] = curCamMatrix.m_m23;
		fCurMatrix[3][3] = curCamMatrix.m_m33;
		fCurMatrix[3][0] = curCamPos.m_x;
		fCurMatrix[3][1] = curCamPos.m_y;
		fCurMatrix[3][2] = curCamPos.m_z;

		float fCurOrthoSize = currentCamera->GetOrthoSize();
		if (currentCamera->IsOrthographic()) {
			fCurScale = currentCamera->GetZoom();
			fCurInitWind[0] = fCurOrthoSize * currentCamera->GetAspectRatio();
			fCurInitWind[1] = fCurOrthoSize;
		} else {
			double fHeightAngle = 1.0;
			fCurInitWind[1] = (float) (fFlocalDistance * 2.0
					* tan(fHeightAngle / 2.0));
			fCurInitWind[0] =
					fCurInitWind[1]
							* currentCamera->GetAspectRatio()/* * (cameraOrthoSize.m_x / cameraOrthoSize.m_y)*/;
			fCurScale = 1.0f;
		}

		pAnimationAPI->RecCamera(fCenter, fCurMatrix, fCurScale, fCurInitWind,
				true, pChgViewBehaviorAction);

		//目标状态
		Vector3 newCamPos = camera.GetPosition();
		Quaternion newCamRotaion = camera.GetRotation();
		Matrix4 newCamMatrix(newCamPos, newCamRotaion, 1.0f);

			//如果是平行投影，进行显示矫正
			if (camera.IsOrthographic())
			{
				//如果摄像机在模型内部，则进行调整
				if (sceneBox.IsInside(newCamPos) == INSIDE)
				{
					Matrix3 viewMat = camera.GetView().ToMatrix3();
					//Vector3 direction(viewMat.m_m20, viewMat.m_m21, viewMat.m_m22);
					Vector3 direction(viewMat.m_m20, viewMat.m_m21, viewMat.m_m22);
					newCamPos = newCamPos + direction * sceneBox.Length()* 0.8f;
				}
				else
				{
					//如果超出视景体，进行调整
					Matrix3 viewMat = camera.GetView().ToMatrix3();
					Vector3 direction(viewMat.m_m20, viewMat.m_m21, viewMat.m_m22);
					Plane centerPlane(direction, center);
					Vector3 targetPnt = centerPlane.Project(newCamPos);
					
					float fHalfViewSize = currentCamera->GetHalfViewSize();
					Vector3 viewMin(center.m_x- fHalfViewSize, center.m_y - fHalfViewSize, center.m_z - fHalfViewSize);
					Vector3 viewMax(center.m_x + fHalfViewSize, center.m_y + fHalfViewSize, center.m_z + fHalfViewSize);
					BoundingBox viewBox(viewMin, viewMax);
					if (viewBox.IsInside(targetPnt) != INSIDE)
					{
						newCamPos = center + direction * sceneBox.Length()* 0.8f;
					}
				}
			}

			Vector3 direction(-newCamMatrix.m_m02, -newCamMatrix.m_m12, -newCamMatrix.m_m22);
			newCamPos = newCamPos.Add(direction.Multiply(sceneBox.Size().Length()* 0.8f));
			//newCamMatrix.SetTranslation(newCamPos);
			newCamMatrix = newCamMatrix.Transpose();
			float fNewMatrix[4][4];
			float fNewScale = 1.0f;
			float fNewInitWind[2];
			fNewMatrix[0][0] = newCamMatrix.m_m00;
			fNewMatrix[0][1] = newCamMatrix.m_m01;
			fNewMatrix[0][2] = newCamMatrix.m_m02;
			fNewMatrix[0][3] = newCamMatrix.m_m03;
			fNewMatrix[1][0] = newCamMatrix.m_m10;
			fNewMatrix[1][1] = newCamMatrix.m_m11;
			fNewMatrix[1][2] = newCamMatrix.m_m12;
			fNewMatrix[1][3] = newCamMatrix.m_m13;
			fNewMatrix[2][0] = newCamMatrix.m_m20;
			fNewMatrix[2][1] = newCamMatrix.m_m21;
			fNewMatrix[2][2] = newCamMatrix.m_m22;
			fNewMatrix[2][3] = newCamMatrix.m_m23;
			fNewMatrix[3][3] = newCamMatrix.m_m33;
			fNewMatrix[3][0] = newCamPos.m_x;
			fNewMatrix[3][1] = newCamPos.m_y;
			fNewMatrix[3][2] = newCamPos.m_z;

			float fNewOrthoSize = camera.GetOrthoSize();
			if (fNewOrthoSize <= 0)
			{
				fNewOrthoSize = fCurInitWind[1];
			}
			if (currentCamera->IsOrthographic())
			{
				fNewScale = camera.GetZoom();
				float fAspectRatio = camera.GetAspectRatio();
				if (fAspectRatio <= 0)
				{
					fAspectRatio = 1.0f;
				}
				fNewInitWind[0] = fNewOrthoSize*fAspectRatio;
				fNewInitWind[1] = fNewOrthoSize;

			//调整缩放
			float dx = fNewInitWind[0] / fCurInitWind[0];
			float dy = fNewInitWind[1] / fCurInitWind[1];
			if (dx > dy) {
				fNewScale = fNewScale / dx;
			} else {
				fNewScale = fNewScale / dy;
			}
			fNewInitWind[0] = fCurInitWind[0];
			fNewInitWind[1] = fCurInitWind[1];
		} else {
			fNewScale = camera.GetZoom();
			//透视投影切换视图不用考虑缩放，只要摄像机的位置和方向正确即可
			fNewInitWind[0] = fCurInitWind[0]; //fNewOrthoSize/ fNewScale*camera.GetAspectRatio();
			fNewInitWind[1] = fCurInitWind[1]; //fNewOrthoSize/ fNewScale;
			fNewScale = 1.0f;
		}

		pAnimationAPI->RecCamera(fCenter, fNewMatrix, fNewScale, fNewInitWind,
				true, pChgViewBehaviorAction);
	}

	//更新零件状态
	if (pView->GetUpDataModelState()) {
		this->GetExplosiveView()->SetExplosivePercent(0);
		this->GetExplosiveView()->SetExplosiveStyle(NOEXPLOSIVE);

		//if (pChgViewBehaviorAction->GetCurrentTick() != 0)
		//	pChgViewBehaviorAction->SetCurrentTick(pChgViewBehaviorAction->GetCurrentTick() + 10);
		//insAtt
		const map<int, InstanceAttribute>& insAttMap =
				pView->GetInstanceAttributeMap();

		Vector3 boxCenter = Vector3::ZERO;

		VStringArray aInsPlcPath;
		VStringArray aInsName;
		VHPointArray aInsStartTranslate;
		VHPointArray aInsEndTranslate;
		VHQuatArray aInsStartRotaion;
		VHQuatArray aInsEndRotaion;
		VHPointArray aInsCenter;
		char plcID[1024];
		for (map<int, InstanceAttribute>::const_iterator it = insAttMap.begin();
				it != insAttMap.end(); it++) {
			const InstanceAttribute &curInsAtt = it->second;

			IShape* shape = this->GetShapeBySVLPath(curInsAtt.path);
			if (shape && shape->GetType() == SHAPE_MODEL) {
				Model *pModel = (Model*) shape;
				if (pModel == NULL)
					continue;

				//颜色和显隐藏不使用动画效果
				pModel->SetVisible(curInsAtt.visible, false);
				if (curInsAtt.hasColor) {
					Color tmpColor = curInsAtt.insColor;
					pModel->SetColor(tmpColor);
				}

				string strPlcID, saPlcID, strObjName;
				Matrix3x4 plcMatrix;
				Vector3 trans;
				Quaternion rot;

				AniPoint hCenter;
				AniPoint hPoint;
				AniQuat hQuat;

				plcMatrix = *pModel->GetPlaceMatrix();
				Matrix3x4 transform = curInsAtt.placeMatrix;
				if ((int) transform.m_m00 == 0 && (int) transform.m_m01 == 0
						&& (int) transform.m_m02 == 0
						&& (int) transform.m_m03 == 0
						&& (int) transform.m_m10 == 0
						&& (int) transform.m_m11 == 0
						&& (int) transform.m_m12 == 0
						&& (int) transform.m_m13 == 0
						&& (int) transform.m_m20 == 0
						&& (int) transform.m_m21 == 0
						&& (int) transform.m_m22 == 0
						&& (int) transform.m_m23 == 0) {
					continue;
				}

				//如果没有变化，不需要生成动画
				if (plcMatrix.Equals(transform))
					continue;

				strPlcID = pModel->GetPlcPath();
				pAnimationAPI->ConvertPlcPathToSAPlcPath(strPlcID.c_str(),
						plcID, 16, 0);
				saPlcID = plcID;
				aInsPlcPath.Append(saPlcID);

				strObjName = pModel->GetName();
				aInsName.Append(strObjName);

				Vector3 privot = pModel->GetBoundingBox().Center();
				hCenter.x = boxCenter.m_x;
				hCenter.y = boxCenter.m_y;
				hCenter.z = boxCenter.m_z;
				aInsCenter.Append(hCenter);
				;
				trans = plcMatrix.Translation();
				rot = plcMatrix.Rotation();
				hPoint.x = trans.m_x;
				hPoint.y = trans.m_y;
				hPoint.z = trans.m_z;
				hQuat.x = rot.m_x;
				hQuat.y = rot.m_y;
				hQuat.z = rot.m_z;
				hQuat.w = rot.m_w;
				aInsStartTranslate.Append(hPoint);
				aInsStartRotaion.Append(hQuat);

				trans = transform.Translation();
				rot = transform.Rotation();
				hPoint.x = trans.m_x;
				hPoint.y = trans.m_y;
				hPoint.z = trans.m_z;
				hQuat.x = rot.m_x;
				hQuat.y = rot.m_y;
				hQuat.z = rot.m_z;
				hQuat.w = rot.m_w;
				aInsEndTranslate.Append(hPoint);
				aInsEndRotaion.Append(hQuat);

			}
		}
		if (aInsPlcPath.Size() <= 0)
			return;

		//动画录制

		pAnimationAPI->RecInsPosRot(aInsPlcPath, aInsName, aInsStartTranslate,
				aInsEndTranslate, aInsStartRotaion, aInsEndRotaion, aInsCenter,
				true, false, pChgViewBehaviorAction);
	}
	pAnimationAPI->m_bShowRecUI = bTempShowRecUI;
	//pAnimationAPI->m_numAutoKeyFrame = iTempNumAutoKeyFrame;
	pChgViewBehaviorAction->SetCurrentTick(0);
	pChgViewBehaviorAction->Continue();
	m_pSAManager->SetCurSAByID(iCurrentBehaviorActionID);
}

/*设置当前视图
 *
 * */
void View::SetCurrentModelView(ModelView *curModelView) {

	m_curModelView = curModelView;
}
/*获取当前视图
 *
 * */
ModelView* View::GetCurrentModelView() {
	if (m_curModelView) {
		return m_curModelView;
	}
	return this->GetDefaultModelView();
}

string View::LoadString(ModelView *modelView) {
	XmlSceneSerializer xmlSerializer;
	xmlSerializer.CreateDocument();
	xmlSerializer.CreateModel();
	;
	vector<ModelView*> viewList; //GetAllViewList();

	viewList.push_back(modelView);

	xmlSerializer.CreateViews(&viewList);

	return xmlSerializer.LoadStringFromModelView();
}
ModelView * View::LoadModelView(string &xmlString) {
	XmlSceneSerializer xmlSerializer;
	return xmlSerializer.LoadModelViewFromXml(xmlString);
}

string View::LoadStringNew(ModelView *modelView) {
	LOGI(" View::LoadStringNew() ");
	string jsonStr ="";
//	if (GetModel()) {
//		vector<ModelView*>* allViews = GetModel()->GetModelViewList();
//		if (allViews) {
//			for (vector<ModelView*>::iterator it = allViews->begin();
//					it != allViews->end(); it++) {
////		                	根据ID查找对应的视图
//				if ((*it)->GetID() == modelView->GetID()) {
//					if (this->m_Reader) {
//							SVL2AsynReader* svl2Reader = dynamic_cast<SVL2AsynReader*>(m_Reader);
//							if (svl2Reader) {
//								return svl2Reader->GetViewInfoStr(i_strFileName, o_bufSize, o_bufP,
//										b_ByExt);
//							}
//						}
//					break;
//				}
//			}
//		}
//	}
	return jsonStr;
}
ModelView * View::LoadModelViewNew(string &xmlString) {
	ModelView* modelview =NULL;
//	SVLXWriter *writer = new SVLXWriter();
//	vector<ModelView*> pOutViewData;
//	ModelView* view  = writer->SaveOneUserViews(GetModel(),*it);
	return modelview;
}

//TODO
bool View::LoadAllUserViews(string &xmlPath) {
	LOGI("View::LoadAllUserViews :%s", xmlPath.c_str());
	bool ret = false;
	XmlSceneSerializer xmlSerializer;
	bool success = xmlSerializer.Load(xmlPath, false);

	//解析成功，同时模型的读取过程也成功
	if (success && this->GetModel()) {
		//Model？暂时只处理一个模型

		//pmis

		//load all note
		vector<Note*> allNoteList;
		xmlSerializer.LoadAllNotes(allNoteList);

		//		LOGI("allnote's cnt:%d", allNoteList.size());
		//		for (int i = 0; i < allNoteList.size(); i++)
		//		{
		//			//viewOne->GetModel()->AddShape(allNoteList.at(i));
		//			GetSceneManager()->AddShape(allNoteList.at(i));
		//		}

		//load all views
		vector<ModelView*> xmlViewList;
		xmlSerializer.LoadAllModelView(&xmlViewList);

		for (int i = 0; i < xmlViewList.size(); i++) {
			ModelView* curView = xmlViewList.at(i);
			curView->SetUpDataCamera(true);
			curView->SetUpDataModel(true);
			GetModel()->AddModelView(curView);
			//			if(curView->GetViewType() == ModelView::DefaultView)
			//			{
			//				LOGI("save default view");
			//				ModelView*  tempDafaultView = new ModelView(*curView);
			//				m_tempDefaultView = tempDafaultView;
			//				LOGI("save default view END");
			//			} //开启保存上次视图
		}
		ret = true;
		LOGI("viewCnt:%d", xmlViewList.size());
	} else {
		LOGI("nativeLoadAllViewFromXML :未找到xml:%s", xmlPath.c_str());
	}

	if (this->GetModel()->GetModelViewList()
			&& this->GetModel()->GetModelViewList()->size() > 0) {
		ret = true;
	}

	LOGI("View::LoadAllUserViews end. ");
	return ret;
}

bool View::SaveAllUserViews(string &xmlPath) {
	LOGI("save all user views");
	bool ret = false;
	//首先删除文件
	remove(xmlPath.c_str());

	XmlSceneSerializer xmlSerializer;
	xmlSerializer.CreateDocument();
	xmlSerializer.CreateModel();
	LOGI("xmlSerializer.CreateViews model: %s", GetModel()->GetName().c_str());
	vector<ModelView*>* viewList = GetModel()->GetModelViewList(); //GetAllViewList();
	if (viewList) {
		LOGI("xmlSerializer.CreateViews viewCount:%d", viewList->size());
	}

	//	if(!Parameters::Instance()->m_isModelView)
	//	{
	//		for (int i = 0; i < viewList.size(); i++)
	//		{
	//			if (viewList[i]->GetViewType() == ModelView::DefaultView)
	//			{
	//				viewList[i] = m_tempDefaultView;
	//			}
	//		}
	//	}//开启保存上次视图
	xmlSerializer.CreateViews(viewList);
	//LOGI("SaveModelView end.");

	ret = xmlSerializer.Save(xmlPath);
	return ret;
}
ModelView*
View::GetDefaultModelView() {
	LOGI(" View::GetDefaultModelView() ");
	ModelView* defaultView = NULL;
	if (GetModel() && (m_enableCreateDefaultView)) {
		vector<ModelView*>* allViews = GetModel()->GetModelViewList();
		if (allViews) {
			for (vector<ModelView*>::iterator it = allViews->begin();
					it != allViews->end(); it++) {
				if ((*it)->GetViewType() == ModelView::DefaultView) {
					defaultView = *it;
					LOGI("DefaultModelView found!");
					break;
				}
			}
		}

		if (defaultView == NULL) {
			defaultView = new ModelView();
			defaultView->SetName("DefaultView");
			defaultView->SetViewType(ModelView::DefaultView);
//                GetModel()->AddModelView(defaultView);
		}
	}
	LOGI(" View::GetDefaultModelView() end. viewAddr:%d", defaultView);
	return defaultView;
}

bool View::UpdateViewByCurrentScene(ModelView* newView) {
	LOGI("View::UpdateViewByCurrentScene begin %d", newView);
	bool ret = false;
	if (!newView) {
		return ret;
	}
    newView->AddRef();
	//camera
	CameraNode* pCurCamera = GetSceneManager()->GetCamera();
	CameraNode& newViewCamera = (CameraNode&) newView->GetCamera();
	newViewCamera.SetPosition(pCurCamera->GetPosition());
	newViewCamera.SetRotation(pCurCamera->GetRotation());
	newViewCamera.SetZoom(pCurCamera->GetZoom());
	float fWidth = 1.0f, fHeiht = 1.0f;
	pCurCamera->GetOrthoSize(&fWidth, &fHeiht);
	newViewCamera.SetOrthoSize(Vector2(fWidth, fHeiht));
	newViewCamera.SetOrthographic(pCurCamera->IsOrthographic());
	newViewCamera.SetAspectRatio(pCurCamera->GetAspectRatio());
	newViewCamera.SetFov(pCurCamera->GetFov());
	newViewCamera.SetFarClip(pCurCamera->GetFarClip());
	newViewCamera.SetNearClip(pCurCamera->GetNearClip());
	//newView->SetCamera(*GetSceneManager()->GetCamera());
	newView->SetUpDataCamera(true);
	newView->SetUpDataModel(true);

	if (newView->GetUpDataModelState()) {
		//insAtt
		map<int, InstanceAttribute> insAttMap;

		vector<Model*> allSubModels;
		Model* topModel = this->GetSceneManager()->GetModel();
		allSubModels.push_back(topModel);
		topModel->GetAllSubModels(allSubModels);

		for (int i = 0; i < allSubModels.size(); i++) {
			Model *curModel = (Model*) allSubModels[i];

			InstanceAttribute ia;
			ia.id = curModel->GetInstatnceID();
			ia.materialId = -1;
			ia.visible = curModel->IsVisible();
			//如果存在于颜色修改列表中则保存
			//map<string, Model*>::iterator tmpIt = m_ColorChangedModelMap.find(
			//	curModel->GetPlcPath());
			//if (tmpIt != m_ColorChangedModelMap.end())
			//{
			//	ia.hasColor = true;
			//	ia.insColor = *curModel->GetColor();
			//}
			//else
			//{
			//	ia.hasColor = false;
			//	ia.insColor = Color::GRAY;
			//}
			Color* pColor = curModel->GetColor();
			if (pColor) {
				ia.hasColor = true;
				ia.insColor = *curModel->GetColor();
			} else {
				ia.hasColor = false;
				ia.insColor = Color::GRAY;
			}

			ia.path = PathHelper::GetSVLPath(curModel);

			ModelShape* modelshape = curModel->GetModelShape();
			if (modelshape) {
				Matrix3x4 matrixTran = modelshape->GetWorldTransform();
//                    Matrix3x4 matrixTran = *curModel->GetPlaceMatrix();
				ia.placeMatrix = matrixTran.ToMatrix4();
			} else {
				ia.placeMatrix = Matrix4::IDENTITY;
			}

			insAttMap.insert(pair<int, InstanceAttribute>(ia.id, ia));
		}

		newView->SetInstanceAttributeMap(insAttMap);
	}

	//记录场景中的剖面ID
	if (this->GetSceneManager()->GetSectionNode()) {
		Section* pSection =
				this->GetSceneManager()->GetSectionNode()->GetSection();
		if (pSection) {
			Model* topModel = this->GetSceneManager()->GetModel();
			newView->ClearSectionPlaneId();
			list<SectionPlane*>* pPlaneList = pSection->GetPlaneList();
			if (pPlaneList) {
				list<SectionPlane*>::iterator it;
				for (it = pPlaneList->begin(); it != pPlaneList->end(); it++) {
					SectionPlane* pPlane = *it;
					if (!pPlane)
						continue;
					if (pPlane->GetEnable()) {
						newView->AddSectionPlaneId(pPlane->GetID());
						if (!topModel->GetSectionPlane(pPlane->GetID())) {
							SectionPlane* iPlane = new SectionPlane();
							iPlane->Copy(pPlane);
							topModel->AddSectionPlane(iPlane);
						}
					}
				}
			}
		}
	}

	if (newView->GetUpDataModelState()) {
		Model* topModel = this->GetSceneManager()->GetModel();
		//设置与视图关联的PMI
		vector<int> vecPMIIds;
		map<int, PMIData*>* pmis = topModel->GetPMIs();
		if (pmis != NULL && pmis->size() > 0) {
			for (map<int, PMIData*>::iterator itPMI = pmis->begin();
					itPMI != pmis->end(); itPMI++) {
				if (!(*itPMI).second)
					continue;
				if ((*itPMI).second->IsVisible()) {
					vecPMIIds.push_back((*itPMI).second->GetID());
				}
			}
		}
		newView->SetPMIIds(vecPMIIds);

		/*--------------------------------------------------------------------------------*/
		newView->ClearNoteDataList();
		vector<int> vecNoteIds;

		NoteGroup* noteGroup = this->GetSceneManager()->GetNoteGroup();
		LOGI("UpdateViewByCurrentScene::SHAPE_TEXT_NOTE");

		if (noteGroup->Size() > 0) {
			LOGI("begin update textnote by current scene");
			int noteCount = noteGroup->Size();
			LOGI("noteCount %d", noteCount);

			for (int i = 0; i < noteCount; i++) {
				SceneNode* node = noteGroup->GetChild(i);
				if (node && node->GetType() == SHAPE_NODE) {
					ShapeNode* shapeNode = (ShapeNode*) node;
					IShape* shape = shapeNode->GetShape();
					if (!(node->IsVisible()) || !shape || !(shape->IsVisible()))
						continue;

					if (shape->GetType() == SHAPE_TEXT_NOTE) {

						TextNote* pNote = (TextNote *) shape;

						//在此处填充View中使用的TextNote ID。//TODO
						vecNoteIds.push_back(pNote->GetID());

						string noteData = NoteFactory::TextNoteToXMLElement(
								this->GetSceneManager(), pNote);
						newView->GetNoteDataList(SHAPE_TEXT_NOTE)->push_back(
								noteData);

					} else if (shape->GetType() == SHAPE_VOICE_NOTE) {
						LOGI("UpdateViewByCurrentScene::SHAPE_VOICE_NOTE");
						VoiceNote* pNote = (VoiceNote *) shape;

						//在此处填充View中使用的TextNote ID。//TODO
						vecNoteIds.push_back(pNote->GetID());

						string noteData = NoteFactory::VoiceNoteToXMLElement(
								this->GetSceneManager(), pNote);
						newView->GetNoteDataList(SHAPE_VOICE_NOTE)->push_back(
								noteData);
					} else if (shape->GetType() == SHAPE_SEQUENCE_NUMBER_NOTE) {
						LOGI(
								"UpdateViewByCurrentScene::SHAPE_SEQUENCE_NUMBER_NOTE");
						SequenceNumberNote* pNote = (SequenceNumberNote *) shape;

						//在此处填充View中使用的TextNote ID。//TODO
						vecNoteIds.push_back(pNote->GetID());

						string noteData = NoteFactory::SequenceNoteToXMLElement(
								this->GetSceneManager(), pNote);
						newView->GetNoteDataList(SHAPE_SEQUENCE_NUMBER_NOTE)->push_back(
								noteData);
					} else if (shape->GetType() == SHAPE_THREED_GESTURE_NOTE) {
						LOGI(
								"UpdateViewByCurrentScene::SHAPE_THREED_GESTURE_NOTE");
						ThreeDGesturesNote* pNote = (ThreeDGesturesNote *) shape;

						//在此处填充View中使用的TextNote ID。//TODO
						vecNoteIds.push_back(pNote->GetID());

						string noteData =
								NoteFactory::ThreeDGestureNoteToXMLElement(
										this->GetSceneManager(), pNote);
						newView->GetNoteDataList(SHAPE_THREED_GESTURE_NOTE)->push_back(
								noteData);
					}

				}
			}
		}

		//文本批注
		AnnotationGroup* pAnnotationGroup =
				this->GetSceneManager()->GetAnnotationGroup();
		if (pAnnotationGroup && pAnnotationGroup->Size() > 0) {
			int iAnnotationCount = pAnnotationGroup->Size();
			for (int i = 0; i < iAnnotationCount; i++) {
				SceneNode* node = pAnnotationGroup->GetChild(i);
				if (node && node->GetType() == SHAPE_NODE) {
					ShapeNode* shapeNode = (ShapeNode*) node;
					IShape* shape = shapeNode->GetShape();
					if (shape && node->IsVisible() && shape->IsVisible()) {
						vecNoteIds.push_back(shape->GetID());
					}
				}
			}
		}

		//测量批注
		MeasureGroup* pMeasureGroup =
				this->GetSceneManager()->GetMeasureGroup();
		if (pMeasureGroup && pMeasureGroup->Size() > 0) {
			int iMeasureCount = pMeasureGroup->Size();
			for (int i = 0; i < iMeasureCount; i++) {
				SceneNode* node = pMeasureGroup->GetChild(i);
				if (node && node->GetType() == SHAPE_NODE) {
					ShapeNode* shapeNode = (ShapeNode*) node;
					IShape* shape = shapeNode->GetShape();
					if (shape && node->IsVisible() && shape->IsVisible()) {
						vecNoteIds.push_back(shape->GetID());
					}
				}
			}
		}

		newView->SetNoteIds(vecNoteIds);
	}

	LOGI(
			"voice note number = %d", newView->GetNoteDataList(SHAPE_VOICE_NOTE)->size());

	LOGI("end update textnote by current scene");

	ret = true;
RET:
	return ret;

}

bool View::UpdateSpecialViewByCurrentScene(ModelView* newView) {
	//LOGI("View::UpdateViewByCurrentScene begin %d",newView);
	bool ret = false;
	if (!newView) {
		return ret;
	}

	//camera
	//CameraNode* pCurCamera = GetSceneManager()->GetCamera();
	//CameraNode& newViewCamera = (CameraNode&)newView->GetCamera();
	//newViewCamera.SetPosition(pCurCamera->GetPosition());
	//newViewCamera.SetRotation(pCurCamera->GetRotation());
	//newViewCamera.SetZoom(pCurCamera->GetZoom());
	//float fWidth = 1.0f, fHeiht = 1.0f;
	//pCurCamera->GetOrthoSize(&fWidth, &fHeiht);
	//newViewCamera.SetOrthoSize(Vector2(fWidth, fHeiht));
	//newViewCamera.SetOrthographic(pCurCamera->IsOrthographic());
	//newViewCamera.SetAspectRatio(pCurCamera->GetAspectRatio());
	//newViewCamera.SetFov(pCurCamera->GetFov());
	//newViewCamera.SetFarClip(pCurCamera->GetFarClip());
	//newViewCamera.SetNearClip(pCurCamera->GetNearClip());
	////newView->SetCamera(*GetSceneManager()->GetCamera());
	//newView->SetUpDataCamera(true);

	if (newView->GetUpDataModelState()) {
		int direction = GetExplosiveView()->GetExplosiveStyle();
		float percent = GetExplosiveView()->GetExplosivePercent();
//        newView->setExplosiveType(direction);
//        newView->setExplosivePercent(percent * 50);

		//LOGI("UpVector:%s",outStr);
		//       LOGI("ZoomFactor:%f",GetSceneManager()->GetCamera()->GetZoom());
		//        LOGI("Orthographic:%d",GetSceneManager()->GetCamera()->IsOrthographic());
		//        LOGI("NearClip:%f",GetSceneManager()->GetCamera()->GetNearClip());
		//        LOGI("FarClip:%f",GetSceneManager()->GetCamera()->GetFarClip());
		//        LOGI("FOV:%f",GetSceneManager()->GetCamera()->GetFov());

		//insAtt
		map<int, InstanceAttribute> insAttMap;

		vector<Model*> allSubModels;
		//Model* topModel = this->GetSceneManager()->GetModel();

		std::vector<IShape*> modelVector = this->m_Selector->GetAll();
		//allSubModels.push_back(topModel);
		//topModel->GetAllSubModels(allSubModels);

		for (int i = 0; i < modelVector.size(); i++) {
			if (modelVector[i]->GetType() == SHAPETYPE::SHAPE_MODEL) {
				Model *curModel = (Model*) modelVector[i];

				InstanceAttribute ia;
				ia.id = curModel->GetInstatnceID();
				ia.materialId = -1;
				ia.visible = curModel->IsVisible();
				//如果存在于颜色修改列表中则保存
				//map<string, Model*>::iterator tmpIt = m_ColorChangedModelMap.find(
				//	curModel->GetPlcPath());
				//if (tmpIt != m_ColorChangedModelMap.end())
				//{
				//	ia.hasColor = true;
				//	ia.insColor = *curModel->GetColor();
				//}
				//else
				//{
				//	ia.hasColor = false;
				//	ia.insColor = Color::GRAY;
				//}
				Color* pColor = curModel->GetColor();
				if (pColor) {
					ia.hasColor = true;
					ia.insColor = *curModel->GetColor();
				} else {
					ia.hasColor = false;
					ia.insColor = Color::GRAY;
				}

				ia.path = PathHelper::GetSVLPath(curModel);

				ModelShape* modelshape = curModel->GetModelShape();
				if (modelshape) {
					//Matrix3x4 matrixTran = modelshape->GetWorldTransform();
					Matrix3x4 matrixTran = *curModel->GetPlaceMatrix();
					ia.placeMatrix = matrixTran.ToMatrix4();
				} else {
					ia.placeMatrix = Matrix4::IDENTITY;
				}

				insAttMap.insert(pair<int, InstanceAttribute>(ia.id, ia));
			} else {
				continue;
			}

		}

		newView->SetInstanceAttributeMap(insAttMap);
	}

	//记录场景中的剖面ID
	if (this->GetSceneManager()->GetSectionNode()) {
		Section* pSection =
				this->GetSceneManager()->GetSectionNode()->GetSection();
		if (pSection) {
			Model* topModel = this->GetSceneManager()->GetModel();
			newView->ClearSectionPlaneId();
			list<SectionPlane*>* pPlaneList = pSection->GetPlaneList();
			if (pPlaneList) {
				list<SectionPlane*>::iterator it;
				for (it = pPlaneList->begin(); it != pPlaneList->end(); it++) {
					SectionPlane* pPlane = *it;
					if (!pPlane)
						continue;
					if (pPlane->GetEnable()) {
						newView->AddSectionPlaneId(pPlane->GetID());
						if (!topModel->GetSectionPlane(pPlane->GetID())) {
							SectionPlane* iPlane = new SectionPlane();
							iPlane->Copy(pPlane);
							topModel->AddSectionPlane(iPlane);
						}
					}
				}
			}
		}
	}

	//note id
	//	if (this->GetModel()->m_ShapeList.size() > 0)
	//	{
	//		for (int i = 0; i < this->GetModel()->m_ShapeList.size(); i++)
	//		{
	//			Shape *pShape = this->GetModel()->m_ShapeList.at(i);
	//			if (pShape->GetType() == SHAPE_NOTE)
	//			{
	//				Note *pNote = (Note*) pShape;
	//				//LOGI("curNoteID:%d",pNote->GetID());
	//				newView->AddNoteId(pNote->GetID());
	//			}
	//		}
	//	}

	if (newView->GetUpDataModelState()) {
		Model* topModel = this->GetSceneManager()->GetModel();
		//设置与视图关联的PMI
		vector<int> vecPMIIds;
		map<int, PMIData*>* pmis = topModel->GetPMIs();
		if (pmis != NULL && pmis->size() > 0) {
			for (map<int, PMIData*>::iterator itPMI = pmis->begin();
					itPMI != pmis->end(); itPMI++) {
				if (!(*itPMI).second)
					continue;
				if ((*itPMI).second->IsVisible()) {
					vecPMIIds.push_back((*itPMI).second->GetID());
				}
			}
		}
		newView->SetPMIIds(vecPMIIds);

		/*--------------------------------------------------------------------------------*/
		newView->ClearNoteDataList();
		vector<int> vecNoteIds;

		NoteGroup* noteGroup = this->GetSceneManager()->GetNoteGroup();
		LOGI("UpdateViewByCurrentScene::SHAPE_TEXT_NOTE");
		//vector<string> *noteDataList = newView->GetNoteDataList(SHAPE_TEXT_NOTE);//TODO 需不需要清空？
		//	assert(noteDataList != NULL);
		//	noteDataList->clear();
		//vector<string> noteDataList;
		//noteDataList.clear();

		if (noteGroup->Size() > 0) {
			LOGI("begin update textnote by current scene");
			int noteCount = noteGroup->Size();
			LOGI("noteCount %d", noteCount);

			for (int i = 0; i < noteCount; i++) {
				SceneNode* node = noteGroup->GetChild(i);
				if (node && node->GetType() == SHAPE_NODE) {
					ShapeNode* shapeNode = (ShapeNode*) node;
					IShape* shape = shapeNode->GetShape();
					if (!(node->IsVisible()) || !shape || !(shape->IsVisible()))
						continue;

					if (shape->GetType() == SHAPE_TEXT_NOTE) {

						TextNote* pNote = (TextNote *) shape;

						//在此处填充View中使用的TextNote ID。//TODO
						vecNoteIds.push_back(pNote->GetID());

						string noteData = NoteFactory::TextNoteToXMLElement(
								this->GetSceneManager(), pNote);
						newView->GetNoteDataList(SHAPE_TEXT_NOTE)->push_back(
								noteData);

					} else if (shape->GetType() == SHAPE_VOICE_NOTE) {
						LOGI("UpdateViewByCurrentScene::SHAPE_VOICE_NOTE");
						VoiceNote* pNote = (VoiceNote *) shape;

						//在此处填充View中使用的TextNote ID。//TODO
						vecNoteIds.push_back(pNote->GetID());

						string noteData = NoteFactory::VoiceNoteToXMLElement(
								this->GetSceneManager(), pNote);
						newView->GetNoteDataList(SHAPE_VOICE_NOTE)->push_back(
								noteData);
					} else if (shape->GetType() == SHAPE_SEQUENCE_NUMBER_NOTE) {
						LOGI(
								"UpdateViewByCurrentScene::SHAPE_SEQUENCE_NUMBER_NOTE");
						SequenceNumberNote* pNote = (SequenceNumberNote *) shape;

						//在此处填充View中使用的TextNote ID。//TODO
						vecNoteIds.push_back(pNote->GetID());

						string noteData = NoteFactory::SequenceNoteToXMLElement(
								this->GetSceneManager(), pNote);
						newView->GetNoteDataList(SHAPE_SEQUENCE_NUMBER_NOTE)->push_back(
								noteData);
					} else if (shape->GetType() == SHAPE_THREED_GESTURE_NOTE) {
						LOGI(
								"UpdateViewByCurrentScene::SHAPE_THREED_GESTURE_NOTE");
						ThreeDGesturesNote* pNote = (ThreeDGesturesNote *) shape;

						//在此处填充View中使用的TextNote ID。//TODO
						vecNoteIds.push_back(pNote->GetID());

						string noteData =
								NoteFactory::ThreeDGestureNoteToXMLElement(
										this->GetSceneManager(), pNote);
						newView->GetNoteDataList(SHAPE_THREED_GESTURE_NOTE)->push_back(
								noteData);
					}

				}
			}
		}

		//文本批注
		//AnnotationGroup* pAnnotationGroup = this->GetSceneManager()->GetAnnotationGroup();
		//if (pAnnotationGroup && pAnnotationGroup->Size() > 0)
		//{
		//	int iAnnotationCount = pAnnotationGroup->Size();
		//	for (int i = 0; i < iAnnotationCount; i++)
		//	{
		//		SceneNode* node = pAnnotationGroup->GetChild(i);
		//		if (node && node->GetType() == SHAPE_NODE)
		//		{
		//			ShapeNode* shapeNode = (ShapeNode*)node;
		//			IShape* shape = shapeNode->GetShape();
		//			if (shape && node->IsVisible() && shape->IsVisible())
		//			{
		//				vecNoteIds.push_back(shape->GetID());
		//			}
		//		}
		//	}
		//}

		//测量批注
		//MeasureGroup* pMeasureGroup = this->GetSceneManager()->GetMeasureGroup();
		//if (pMeasureGroup && pMeasureGroup->Size() > 0)
		//{
		//	int iMeasureCount = pMeasureGroup->Size();
		//	for (int i = 0; i < iMeasureCount; i++)
		//	{
		//		SceneNode* node = pMeasureGroup->GetChild(i);
		//		if (node && node->GetType() == SHAPE_NODE)
		//		{
		//			ShapeNode* shapeNode = (ShapeNode*)node;
		//			IShape* shape = shapeNode->GetShape();
		//			if (shape && node->IsVisible() && shape->IsVisible())
		//			{
		//				vecNoteIds.push_back(shape->GetID());
		//			}
		//		}
		//	}
		//}

		newView->SetNoteIds(vecNoteIds);
	}

	LOGI(
			"voice note number = %d", newView->GetNoteDataList(SHAPE_VOICE_NOTE)->size());

	LOGI("end update textnote by current scene");

	//TODO MERGEToMobile
//    if (SectionOperator::Instance != NULL) {
//        ///sectionPlane
//        int direction = SectionOperator::Instance->m_Direction;
//        float percentage = 100 * SectionOperator::Instance->m_fPercentage; //
//        int directionX = SectionOperator::Instance->m_DirectionX;
//        float percentageX = SectionOperator::Instance->m_fPercentageX; //
//        int directionY = SectionOperator::Instance->m_DirectionY;
//        float percentageY = SectionOperator::Instance->m_fPercentageY; //
//        int directionZ = SectionOperator::Instance->m_DirectionZ;
//        float percentageZ = SectionOperator::Instance->m_fPercentageZ; //
//        bool showCutPlane = SVIEW::Parameters::Instance()->m_showSection;
//        bool isCappingPlane =
//                this->GetSceneManager()->GetSectionNode()->GetSection()->IsShowCappingPlane();
//        bool isReverse =
//                this->GetSceneManager()->GetSectionNode()->GetSection()->IsReverseClipping();
////        newView->SetSectionPlaneDirection(direction);
////        newView->SetSectionPlanePercentage(percentage);
////        newView->SetSectionPlaneDirectionAndPercentage(directionX, directionY,
////                directionZ, percentageX, percentageY, percentageZ);
//        newView->SetShowClipSectionPlane(showCutPlane);
//        newView->SetShowSectionCappingPlane(isCappingPlane);
//        newView->SetReverseClipping(isReverse);
//    }

	/*-----------------------------------------------------------------------------*/

	//LOGI("Set note id end");
	//	delete action;
	//LOGI("View::UpdateViewByCurrentScene end.");
	ret = true;
RET:
	return ret;

}

void View::CreateDefaultView(bool value) {
	this->m_enableCreateDefaultView = value;
}
/**
 * 获取当前读取操作的进度
 * @return
 */
float View::GetReadPercent() {
	if (this->m_Reader && this->m_readListener) {
		return this->m_readListener->GetPercent();
	}
	return 0;
}

//void View::GetCurrentState()
//{
//	m_NodesStateMap.clear();
//	ViewStateOperateAction* action = new ViewStateOperateAction(
//			ViewStateOperateAction::GET_MATRIX);
//	this->m_SceneManager->ApplyAction(action);
//	m_NodesStateMap = action->GetModelState();
//	delete action;
//}

//void View::SetCurrentState()
//{
//	ViewStateOperateAction* action = new ViewStateOperateAction(
//			ViewStateOperateAction::SET_MATRIX);
//	action->SetModelState(m_NodesStateMap);
//
//	this->m_SceneManager->ApplyAction(action);
//	delete action;
//}

//------------------------------------------ModelView END----------------------------------------------------

void View::SetDefaultWorkNodes() {
	if (this->m_SceneManager == NULL)
		return;

	//m_pWorkNodeGroup->AddNode(M3D::MAINCAMERA,
	//	this->m_SceneManager->GetCamera());
	//m_pWorkNodeGroup->AddNode(M3D::MAINMODELROOT,
	//	this->m_SceneManager->GetSceneRoot()->Search(M3D::MAINMODELROOT));
	//m_pWorkNodeGroup->AddNode(M3D::AXIS,
	//	this->m_SceneManager->GetSceneRoot()->Search(M3D::AXIS));
	//m_pWorkNodeGroup->AddNode(M3D::FPS_FLAG,
	//	this->m_SceneManager->GetSceneRoot()->Search(M3D::FPS_FLAG));
	//m_pWorkNodeGroup->AddNode(M3D::MAINGROUP,
	//		this->m_SceneManager->GetSceneRoot()->Search(M3D::MAINGROUP));
	//m_pWorkNodeGroup->SetCurrWorkNode(MAINMODELROOT);
}

int View::GetViewWidth() {
	return this->m_SceneManager->GetRenderManager()->GetWindowWidth();
}

int View::GetViewHeight() {
	return this->m_SceneManager->GetRenderManager()->GetWindowHeight();
}

void View::setOrthographic(bool enable) {
	CameraNode* camera = this->m_SceneManager->GetCamera();
	if (camera) {
		camera->SetOrthographic(enable);
	}
}

bool View::GetOrthographic() {
	bool enableOrth = true;
	CameraNode* camera = this->m_SceneManager->GetCamera();
	if (camera) {
		enableOrth = camera->IsOrthographic();
	}
	return enableOrth;
}

void View::setWalkThrough(TouchHandlerType type) {

	if (m_useWalkThrough == type) {
		LOGI("View::setWalkThrough m_useWalkThrough ==%d", (int)type);
		return;
	}
	LOGI("View::setWalkThrough type ==%d", (int)type);
	MutexLock locked(this->m_mutex);

	m_useWalkThrough = type;
	if (m_useWalkThrough == HANDLER_WALKTHROUGH) {
		LOGI("m_walkthroughTouchHandler BEGIN");
		m_workTouchHandler->Close();
		m_workTouchHandler = this->m_walkthroughTouchHandler;
		//this->m_walkthroughTouchHandler->StartRotateOnFixedPoint();//TODO
		this->SetCameraProjectionType(1);
		this->SetPerspective(0);
		//m_workTouchHandler->Open();
		LOGI("m_walkthroughTouchHandler END");
	} else if (m_useWalkThrough == HANDLER_COMMON) {
		//	m_workTouchHandler->Close();
		//m_workTouchHandler = this->m_walkthroughTouchHandler;
		// this->m_walkthroughTouchHandler->EndRotateOnFixedPoint();//TODO
		//	m_workTouchHandler->Open();
		this->SetCameraProjectionType(0);
		//m_workTouchHandler->Close();
		m_workTouchHandler = this->m_TouchEventHandler;
		//m_workTouchHandler->Open();
	} else if (m_useWalkThrough == HANDLER_DRAGGER) {
		m_workTouchHandler->Close();

		m_workTouchHandler = this->m_draggerHandler;

		m_workTouchHandler->Open();
	}

	this->RequestDraw();
}

TouchHandlerType View::GetWalkThrough() {
	return m_useWalkThrough;
}

int View::GetID() {
	return this->m_ID;
}

void View::SetID(int viewID) {
	this->m_ID = viewID;
}

bool View::AnimationOpenXMLData(const string& xmlData) {
	MutexLock locked(this->m_mutex);

	if (m_pSAManager == NULL) {
		m_pSAManager =
				new NS_SimulationAnimation::CSimulationAnimationManager();
	}
	if (pTDriver == NULL) {
		pTDriver = new CTickTimer();
		pTDriver->Init();
	}

	m_pSAManager->ClearData();
	m_bHasAni = false;

	//string tempFile = file;
	//LOGI("AnimationOpen  file is %s", tempFile.c_str());
	//m_pSAManager->ReadFromFile(tempFile.c_str());

	m_pSAManager->ReadFromData(xmlData.c_str());

	//	LOGE("%s", xmlData.c_str());

	//	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
	//			m_pSAManager->GetCurrentSA();
	//
	//	if (pBehaviorAction && pBehaviorAction->HasAnimations())
	//	{
	//		pBehaviorAction->SetView(this);
	//		m_bHasAni = true;
	//	}

	if (m_pSAManager->HasAnimations()) {

		//m_pSAManager->SetView(this);
		InitAinmationPlayCB();
		m_bHasAni = true;
	}
	return m_bHasAni;
}

bool View::AnimationOpen(const string& file) {
	MutexLock locked(this->m_mutex);
	LOGI("AnimationOpen  000000000");
	if (m_pSAManager == NULL) {
		m_pSAManager =
				new NS_SimulationAnimation::CSimulationAnimationManager();
	}
	if (pTDriver == NULL) {
		pTDriver = new CTickTimer();
		pTDriver->Init();
	}
	LOGI("AnimationOpen  1111111111");
	m_pSAManager->ClearData();

	m_bHasAni = false;

	string tempFile = file;
	LOGI("AnimationOpen  file is %s", tempFile.c_str());
	m_pSAManager->ReadFromFile(tempFile.c_str());

	//	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
	//			m_pSAManager->GetCurrentSA();
	//
	//	if (pBehaviorAction && pBehaviorAction->HasAnimations())
	//	{
	//		pBehaviorAction->SetView(this);
	//		m_bHasAni = true;
	//	}

	if (m_pSAManager->HasAnimations()) {
		//m_pSAManager->SetView(this);
		InitAinmationPlayCB();
		m_bHasAni = true;
	} else {
		m_pSAManager->SetSAFilePath(
				(wchar_t*) Platform::StringToWString(tempFile, "auto").c_str());
	}

	m_isNeedChangeViewAnimation = true;

	return m_bHasAni;
}

void View::RestoreShowMode() {
	m_isNeedRotate = true;
	m_excStep1 = false;
	m_excStep2 = false;
	isRotateComplete = false;
	explosivePercent = 0;
	explorsiveSpeed = 1.0;
	SetExplosiveViewWithoutRestore(0, 0);
	GetTouchHandler()->m_rotCircleCount = 0;

}

void View::SetCameraProjectionType(int projectionType) {
	CameraNode * camera = m_SceneManager->GetCamera();
	float cameraFov = camera->GetFov();

	if (projectionType == 1) {
		if (camera->IsOrthographic()) {
			//将缩放转换成透视投影对应的位置
			float zoom = camera->GetZoom();

			float defaultZoom = m_SceneManager->GetDefaultZoom();
			camera->SetZoom(defaultZoom);

			float zoomFactor = defaultZoom / zoom;

			Vector3 cameraWorldPos = camera->GetWorldPosition();

			//获取屏幕中心点的交点作为目标点
			Vector3 rotateCenter;
			IntVector2 intVector = camera->GetViewPort().GetRect().Center();
			Vector2 screenPnt(intVector.m_x, intVector.m_y);

			m_SceneManager->GetPickPoint(screenPnt, rotateCenter, false);

			float lenght = m_SceneManager->GetDefaultFocusLength();

			float dis = (lenght / 90) * zoomFactor
					/ (Tan(cameraFov * 0.5f / 180.f * static_cast<float>(M_PI))
							* 2.f);

			camera->SetWorldPosition(
					rotateCenter
							+ (cameraWorldPos - rotateCenter).Normalized()
									* dis);

			camera->SetOrthographic(false);

			this->RequestDraw();
		}
	} else if (projectionType == 0) {
		if (!camera->IsOrthographic()) {
			//将透视投影对应的位置，转换成平行投影的缩放
			float zoom = camera->GetZoom();

			float defaultZoom = m_SceneManager->GetDefaultZoom();
			camera->SetZoom(defaultZoom);

			Vector3 cameraWorldPos = camera->GetWorldPosition();

			//获取屏幕中心点的交点作为目标点
			Vector3 rotateCenter;
			IntVector2 intVector = camera->GetViewPort().GetRect().Center();
			Vector2 screenPnt(intVector.m_x, intVector.m_y);
			m_SceneManager->GetPickPoint(screenPnt, rotateCenter, false);

			float defaultLength = m_SceneManager->GetDefaultFocusLength();
			float lenght = m_SceneManager->GetDefaultFocusLength();

			float zoomFactor = (rotateCenter - cameraWorldPos).Length()
					/ (lenght / 90)
					* (Tan(cameraFov * 0.5f / 180.f * static_cast<float>(M_PI))
							* 2.f);

			camera->SetWorldPosition(
					rotateCenter
							+ (cameraWorldPos - rotateCenter).Normalized()
									* lenght);

			camera->ZoomView(1 / zoomFactor);
			camera->SetOrthographic(true);
			this->RequestDraw();
		}
	}
}

int View::GetCameraProjectionType() {
	int projectionType = 0;
	CameraNode * camera = m_SceneManager->GetCamera();
	if (camera) {
		projectionType = camera->IsOrthographic() ? 0 : 1;
	}
	return projectionType;
}

PerspectiveOperator* View::GetPerspectiveOperator() {
	if (this->m_perpectiveOperator == NULL) {
		m_perpectiveOperator = new PerspectiveOperator();
	}

	return m_perpectiveOperator;
}

void View::FoucusView(BoundingBox& foucusBox, bool useAni) {
	////根据包围盒的大小，构建聚焦的动画
	SceneManager* scene = this->GetSceneManager();
	scene->Lock();

	CameraNode* camera = scene->GetCamera();

	Vector3 disPosition;
	Quaternion disRotation;
	Vector3 disScale;

	if (camera && camera->IsOrthographic()) { //平行投影聚焦显示
		Vector3 center = foucusBox.Center();
		IntVector2 scrPnt = camera->GetViewPort().WorldToScreenPoint(center);
		IntVector2 scrCenter = camera->GetViewPort().GetRect().Center();

		//从屏幕上的一点移动到另一点
		Vector3 m_cacheCurPointNear =
				/* camera->GetView()* */camera->GetViewPort().ScreenToWorldPoint(
						scrPnt.m_x, scrPnt.m_y, 0.5f);
		Vector3 m_cachePriPoint =
				/*camera->GetView()* */camera->GetViewPort().ScreenToWorldPoint(
						scrCenter.m_x, scrCenter.m_y, 0.5f);

		//设置位置到合理值
		Vector3 moveVector = m_cacheCurPointNear - m_cachePriPoint;
		Vector3 cameraPos = camera->GetPosition();
		cameraPos = cameraPos + moveVector;
		disPosition = cameraPos;

		//设置缩放比例到合理值
		int screenHeight = this->GetRenderManager()->GetWindowHeight();
		int screenWidth = this->GetRenderManager()->GetWindowWidth();
		//this->GetRenderManager()->WindowSizeChanged(screenWidth, screenHeight);
		float defaultZoom = Parameters::Instance()->m_DefaultZoomFactor;
		defaultZoom = defaultZoom * screenHeight / screenWidth;
		if (screenHeight > screenWidth) {
			defaultZoom = defaultZoom * 0.5
					* (screenWidth * 1.0 / screenHeight);
		}
		BoundingBox& sceneBox = scene->GetSceneBox();
		float scale = Parameters::m_foucsScalFactor * defaultZoom
				* sceneBox.Length() / foucusBox.Length();
		disScale = Vector3(scale, scale, scale);
		disRotation = camera->GetRotation();
	} else if (camera && !camera->IsOrthographic()) { //处理透视投影
		Vector3 center = foucusBox.Center();

		scene->SetRotationCenter(center);
		float fitDepth = camera->GetFitClip();

		IntVector2 scrCenter = camera->GetViewPort().GetRect().Center();

		///乘上camera->GetView()将点转换到摄像机eye坐标系，进行求两次的变化量,由于此处摄像机采用继承的方式实现，没有采用节点挂载的方式
		//此处的部分变换处理算法，待调整为节点挂载的方式后重构。。 TODO
		Vector3 m_cachePriPoint = camera->GetViewPort().ScreenToWorldPoint(
				scrCenter.m_x, scrCenter.m_y, fitDepth);
		Vector3 m_cacheCurPointNear = center;

		//设置位置到合理值
		Vector3 moveVector = m_cacheCurPointNear - m_cachePriPoint;

		Vector3 cameraPos = camera->GetPosition();
		cameraPos = cameraPos + moveVector;
		//设置缩放比例到合理值
		BoundingBox& sceneBox = scene->GetSceneBox();

		float moveFactor = foucusBox.Length()
				* (1 + Parameters::m_foucsScalFactor);

		//Vector3
		moveVector = (center - cameraPos).Normalized() * moveFactor;
		float scale = camera->GetZoom();

		disScale = Vector3(scale, scale, scale);
		disRotation = camera->GetRotation();
		disPosition = center - moveVector;
	}

	if (useAni) {
		this->GetPerspectiveOperator()->ExecuteCommonCameraAnimation(this,
				disPosition, disRotation, disScale, true, true, true);
	} else {
		camera->SetRotation(disRotation);
		camera->SetWorldPosition(disPosition);
		camera->SetZoom(disScale.m_x);
	}

	scene->UnLock();
}

bool View::CreateNewReader() {
	if (!m_Reader) {
		m_Reader = Reader::GetReader(m_CurFilePath);
		SVL2AsynReader* svl2Reader = dynamic_cast<SVL2AsynReader*>(m_Reader);
		if (svl2Reader) {
			svl2Reader->LoadDoc(m_CurFilePath);
		}
		m_bNewCreate = true;
		return true;
	}
	return false;
}

void View::CloseReader() {
	if (m_bNewCreate) {
		delete m_Reader;
		m_Reader = NULL;
		m_bNewCreate = false;
	}
}

int View::GetSVLXFileItem(const std::string& i_strFileName,
		unsigned int& o_bufSize, char** o_bufP, bool b_ByExt) {
	if (this->m_Reader) {
		SVL2AsynReader* svl2Reader = dynamic_cast<SVL2AsynReader*>(m_Reader);
		if (svl2Reader) {
			return svl2Reader->GetSVLXFileItem(i_strFileName, o_bufSize, o_bufP,
					b_ByExt);
		}
	}
	return 0;
}

M3D_STATUS View::GetSVLXFileItemToFile(const std::string& i_strFileName,
		const std::string& i_strOutFilePath, bool bByExt) {
	if (this->m_Reader) {
		SVL2AsynReader* svl2Reader = static_cast<SVL2AsynReader*>(m_Reader);
		if (svl2Reader) {
			svl2Reader->GetSVLXFileItemToFile(i_strFileName, i_strOutFilePath,
					bByExt);
			return M_SUCCESS;
		}
	}
	return M_ERROR;
}
M3D_STATUS View::CopySVLXFileItemToFileByReOpen(
		const std::string& i_strFileName, const std::string& i_strOutFilePath,
		bool bByExt) {
	//如果"readfrombufferXXX.svlx" 是远端模型标识
	if (m_CurFilePath != "readfrombuffer.svlx") {
		m_Reader = Reader::GetReader(m_CurFilePath);
		SVL2AsynReader* svl2Reader = static_cast<SVL2AsynReader*>(m_Reader);
		if (svl2Reader) {
			LOGI("GetSVLXFileItemToFileByReOpen %s", i_strFileName.c_str());
			svl2Reader->GetSVLXFileItemToFileByReOpen(m_CurFilePath,
					i_strFileName, i_strOutFilePath, bByExt);
			if (m_Reader) {
				delete m_Reader;
				m_Reader = NULL;
			}
			return M_SUCCESS;
		}
	}
	return M_ERROR;
}
bool View::AsynReadClose() {
	//this->GetPerspectiveOperator()->SetAllowUseAni(true);
	SceneManager *scene = this->GetSceneManager();
	scene->Lock();
	//设置ocTree区域
	this->GetSceneManager()->RequestUpdateWhenSceneBoxChanged();

//	if (this->GetModel()) { //
//		if (this->GetModel()->GetChildren().size() == 1) {
//			this->RestoreView(false);
//		} else {
//			//this->GetTouchHandler()->SetRestoreCamera(false);
//			this->InitCamera();
//			//this->GetTouchHandler()->SetRestoreCamera(true);
//		}
//	}
	if (SVIEW::Parameters::Instance()->GetLoadExternInfo() && this->m_Reader) {
		((SVL2AsynReader*) this->m_Reader)->LoadFileInfo();
	}
	scene->UnLock();
	if (this->m_Reader) {
		delete this->m_Reader;
		this->m_Reader = NULL;
	}
	return true;
}

Model* View::AsynGetModel(char* fileBuffer, int length,
		const string& filePath) {
	Model* retModel = NULL;
	SceneManager *scene = this->GetSceneManager();
	scene->Lock();

	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;

	memoryLeakDetector();

	MutexLock locked(this->m_mutex);

	if (this->m_readListener) {
		this->m_readListener->SetReadFileCount(1);
		this->m_readListener->SetReadFileIndex(0);
		this->m_readListener->OnProcessPercent(NULL, 0);
	}

	try {
		LOGI("AsynGetModel begin");
		if (fileBuffer != NULL && length > 0) {
			m_CurFilePath = "readfrombuffer.svlx";
			m_Reader = Reader::GetReader(m_CurFilePath);

			SVL2AsynReader* svl2Reader = static_cast<SVL2AsynReader*>(m_Reader);
			if (svl2Reader) {
				svl2Reader->ReadFromBuffer(fileBuffer, length);
			}
		} else if (filePath.length() > 0) {
			m_CurFilePath = filePath;
			m_Reader = Reader::GetReader(m_CurFilePath);
		}

		if (this->m_readListener) {
			m_Reader->SetListener(this->m_readListener);
		}

		m_Reader->SetView(this);

		retModel = m_Reader->GetModel();

		if (retModel == NULL) {
			if (m_Reader != NULL) {
				delete m_Reader;
				m_Reader = NULL;
			}
		}

		if (this->m_readListener) {
			this->m_readListener->SetPercent(1.0f);
		}
		//this->RestoreView();
		LOGI("AsynGetModel end");
	} catch (const std::bad_alloc& e) {
		LOGE("C++ 内存分配失败...%s", e.what());
		readerState = M_Read_OOM;
	} catch (const std::ios_base::failure& e) {
		LOGE("C++ IO 异常...%s", e.what());
		readerState = M_Read_ANALYSIS_ERROR;
	} catch (...) {
		LOGE("抱歉..未知错误...请联系开发人员..");
		readerState = M_Read_NO_DEFINE_Error;
	}
	scene->UnLock();

	return retModel;
}

Model* View::AsynCreateTopModel() {
	if (!this->m_Model) {
		this->m_Model = new Model();
		this->m_Model->SetName("Root");
		this->GetSceneManager()->SetModel(this->m_Model);
		this->SetDefaultWorkNodes();
		this->m_Model->AddRef();
	}
	return this->m_Model;
}

bool View::AsynAddModel(Model* parentModel, Model* addModel) {
	bool addState = false;
	addState = this->GetSceneManager()->AsynAddModelToParent(parentModel,
			addModel);
	return addState;
}

bool View::AsynRemoveModel(Model* removeModel) {
	bool removeState = false;
	removeState = this->GetSceneManager()->AsynRemoveModelFromeScene(
			removeModel->GetParent(), removeModel);
	return removeState;
}

bool View::SetAsynFillModelBuffer(char* fileLength, int bufferLength) {
	if (this->m_Reader) {
		((SVL2AsynReader*) this->m_Reader)->ReadFromBuffer(fileLength,
				bufferLength);
	}
	return true;
}

M3D_STATUS View::AsynFillModel(Model* singleModel) {
	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;
	if (this->m_Reader) {
		try {
			SceneManager* scene = this->GetSceneManager();
			scene->Lock();
			((SVL2AsynReader*) this->m_Reader)->FillModelMesh(this,
					singleModel);
			bool checkErrorPoint =
					SVIEW::Parameters::Instance()->m_IsCheckErrorPoint;
			if (checkErrorPoint) {
				this->ChechErrorPoint(singleModel);
			}

			this->GetSceneManager()->AsynUpdateModelCacheInfo(singleModel, true,
					false);
			scene->UnLock();
			readerState = M_Read_OK;
		} catch (const std::bad_alloc& e) {
			LOGE("AsynFillModel error 1");
			readerState = M_Read_OOM;
			//MemoryHelper::Instance()->SetMemoryStateLazzy(M3D_Memory_Alloc_Bad);
			//this->CloseFile();
			return readerState;
		} catch (const std::ios_base::failure& e) {
			LOGE("AsynFillModel error 2");
			readerState = M_Read_ANALYSIS_ERROR;
			//this->CloseFile();
			return readerState;
		} catch (...) {
			LOGE("AsynFillModel error 3");
			readerState = M_Read_NO_DEFINE_Error;
			//this->CloseFile();
			return readerState;
		}
	}
	return readerState;
}

void View::SetShowModeSpeed(float speed) {
	explorsiveSpeed = speed;
	GetTouchHandler()->SetShowModeSpeed(speed);
}

void View::AnimationInit() {
	MutexLock locked(this->m_mutex);
	//LOGI("Java_ht_svbase_natives_Natives_nativeAnimationPlay:");
	//	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
	//			m_pSAManager->GetCurrentSA();
	//	if (NULL == pBehaviorAction)
	//		return;

	// 	//每次播放的开始，指定所使用的播放速度
	//	pBehaviorAction->SetTicksPerSecondByMultiple(m_animationPlaySpeed);

	CAnimationStepManager* pStepAni = m_pSAManager->GetAnimationStepManager();

	// 	//设置播放速度
	m_pSAManager->SetPlaySpeed(m_animationPlaySpeed);

	if (!m_pSAManager->IsPlaying()) {
		ControlInfo cameraState;
		bool isUseCamera = this->GetAnimationDisState(cameraState);
		// 		//TODO 暂时屏蔽场景状态切换到动画播放位置 动画插值
		if (isUseCamera == false) {
			this->GetPerspectiveOperator()->SetAnimationFinishCallback(
					View::AnimationStartCallback, (void*) this);
			this->GetPerspectiveOperator()->Apply(this, cameraState, true,
					false, false);
		}
	}

}

bool View::AnimationContinuousPlayState() {
	MutexLock locked(this->m_mutex);

	bool ret = m_animationContinuousPlay;

	if (m_pSAManager) {
		ret = m_pSAManager->GetContinuousPlay();
	}

	return ret;
}

bool View::AnimationContinuousPlay(bool isLoop) {
	MutexLock locked(this->m_mutex);

	bool ret = false;

	m_animationContinuousPlay = isLoop;
	if (m_pSAManager) {
		//		NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
		//				m_pSAManager->GetCurrentSA();
		//		if (pBehaviorAction)
		//		{
		//			pBehaviorAction->SetContinuousPlay(isLoop);
		//			ret = true;
		//		}
		m_pSAManager->SetContinuousPlay(isLoop);
	}

	return ret;
}

bool View::AnimationPlay() {
	MutexLock locked(this->m_mutex);

	//	LOGI("Java_ht_svbase_natives_Natives_nativeAnimationPlay:");
	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
			m_pSAManager->GetCurrentSA();

	if (NULL == pBehaviorAction)
		return false;
	//	LOGE("return false;");

	// 	//每次播放的开始，指定所使用的播放速度
	m_pSAManager->SetPlaySpeed(m_animationPlaySpeed);

	m_pSAManager->SetContinuousPlay(m_animationContinuousPlay);

	// 	//TODO 根据是否使用手动播放动画场景进行判断

	CAnimationStepManager* pStepAni = m_pSAManager->GetAnimationStepManager();
	if (!m_pSAManager->IsPlaying())
	//&& m_isNeedChangeViewAnimation)
	{
		//		ControlInfo cameraState;
		//		bool isUseCamera = this->GetAnimationDisState(cameraState);

		//	// //		//TODO 暂时屏蔽场景状态切换到动画播放位置 动画插值
		//		if (isUseCamera == true)
		//		{
		//			PerspectiveOperator::GetInstance()->SetAnimationFinishCallback(
		//					View::AnimationStartCallback, (void*) this);
		//			PerspectiveOperator::GetInstance()->Apply(this, cameraState, true,
		//					true, true);
		//		}
		//		else
		{
			//			LOGE("not palying");
			this->SaveAnimatinState();

			pStepAni->SetPlayMode(
					CAnimationStepManager::AnimationPlayMode::PLAY_MODE_PROCESS);

			pStepAni->Pause();
			//			pStepAni->StartChangeCamera(pStepAni->GetCurrentProcess());
			pStepAni->Play(
					CAnimationStepManager::AnimationPlayMode::PLAY_MODE_PROCESS);

			//			LOGE("palying");
			this->UpdateDrawLimit();
		}

		//		m_isNeedChangeViewAnimation = false;
	} else {
		//m_isNeedChangeViewAnimation = true;
		// 		//结束视图插值动画
		//PerspectiveOperator::GetInstance()->FinishAnimation();
		pStepAni->Stop();

		this->UpdateDrawLimit();
	}
	return true;
}

bool View::AnimationRewind() {
	MutexLock locked(this->m_mutex);

	//	LOGI("Java_ht_svbase_natives_Natives_nativeAnimationRewind:");
	if (m_pSAManager == NULL) {
		return false;
	}

	//	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
	//			m_pSAManager->GetCurrentSA();

	//	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
	//			m_pSAManager->GetAnimationStep();//GetCurrentSA();

	CAnimationStepManager* pStepAni = m_pSAManager->GetAnimationStepManager();

	if (NULL == pStepAni)
		return false;

	pStepAni->Rewind(
			CAnimationStepManager::AnimationPlayMode::PLAY_MODE_PROCESS, false);

	this->RestoreAnimationState();
	//	pBehaviorAction->Rewind();
	return true;
}

bool View::AnimationSetTick(float percentage) {
	MutexLock locked(this->m_mutex);

	this->m_SceneManager->Lock();

	//	LOGI("Java1");

	//	LOGI("Java_ht_svbase_natives_Natives_nativeAnimationSetTick:");
	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
			m_pSAManager->GetCurrentSA();

	if (NULL == pBehaviorAction)
		return false;

	if (m_pSAManager->IsPlaying()) {
		m_pSAManager->StopAll();

		pBehaviorAction->SetCurrentTickByPercentage(percentage);
		pBehaviorAction->ExecuteAnimations(pBehaviorAction->GetCurrentTick(),
				0);
		pBehaviorAction->Continue();
	} else {
		pBehaviorAction->SetCurrentTickByPercentage(percentage);
		pBehaviorAction->ExecuteAnimations(pBehaviorAction->GetCurrentTick(),
				0);
	}

	this->m_SceneManager->UnLock();

	return true;
}

float View::AnimationGetTick() {
	MutexLock locked(this->m_mutex);

	float tick = 0.0f;
	//LOGI("Java_ht_svbase_natives_Natives_nativeAnimationGetTick:");
	if (m_pSAManager != NULL) {
		CAnimationStepManager* step = m_pSAManager->GetAnimationStepManager();
		if (step) {
			CProcess* process = step->GetCurrentProcess();
			if (process) {
				NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
						process->GetBehaviorAction();
				if (pBehaviorAction) {
					tick = pBehaviorAction->GetCurrentTickByPercentage();
				}
			}
		}
	}
	return tick;
}

bool View::AnimationIsPlaying() {
	MutexLock locked(this->m_mutex);

	//LOGI("Java_ht_svbase_natives_Natives_nativeAnimationIsPlaying:");
	if (m_pSAManager == NULL) {
		return false;
	}
	//	LOGE("is playing %d",m_pSAManager->IsPlaying());
	return m_pSAManager->IsPlaying();
	//	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
	//			m_pSAManager->GetCurrentSA();
	//	if (NULL == pBehaviorAction)
	//		return false;
	//	return pBehaviorAction->IsPlaying();
}

bool View::AnimationTransitionIsPlaying() {
	MutexLock locked(this->m_mutex);

	if (m_pSAManager == NULL) {
		return false;
	}

	return (m_pSAManager->GetAnimationStepManager()->GetBehaviorActionChgCam()
			!= NULL);
}

bool View::AnimationExecute(float percentage) {
	MutexLock locked(this->m_mutex);

	//LOGI("Java_ht_svbase_natives_Natives_nativeAnimationExecute:");
	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
			m_pSAManager->GetCurrentSA();
	if (NULL == pBehaviorAction)
		return false;

	pBehaviorAction->ScheduleAllAnimations(true);
	pBehaviorAction->SetCurrentTickByPercentage(percentage);
	pBehaviorAction->ExecuteAnimations(pBehaviorAction->GetCurrentTick(), 0);

	return true;
}

bool View::AnimationPlayCamera(bool isPlayCamera) {
	MutexLock locked(this->m_mutex);

	//LOGI("View::AnimationPlayCamera(bool isPlayCamera) %d",isPlayCamera);
	m_isAnimationPlayCamera = isPlayCamera;
	return true;
}

bool View::AnimationPlaySpeed(float speed) {
	MutexLock locked(this->m_mutex);

	bool state = true;

	m_animationPlaySpeed = speed;

	if (speed > 0 && speed < 10 && m_pSAManager != NULL) {
		NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
				m_pSAManager->GetCurrentSA();
		if (NULL == pBehaviorAction)
			return false;

		if (m_pSAManager->IsPlaying()) {
			pBehaviorAction->Stop();
			pBehaviorAction->GetSimulationAnimationManager()->SetPlaySpeed(
					(float) m_animationPlaySpeed);

			bool bReversPlay = pBehaviorAction->IsReversePlay();
			pBehaviorAction->ScheduleAllAnimations(true);
			// 			//修改动画播放进度结束时，如果处于暂停状态，继续播放动画
			if (bReversPlay) {
				pBehaviorAction->ContinueReverse();
			} else {
				pBehaviorAction->Continue();
			}
		} else {
			pBehaviorAction->GetSimulationAnimationManager()->SetPlaySpeed(
					(float) m_animationPlaySpeed);
		}
	} else {
		LOGE("View::AnimationPlaySpeed allowed speed from 0 to 10");
		state = false;
	}

	return state;
}

bool View::AnimationSetActiveStep(int processId, int stepId) {
	MutexLock locked(this->m_mutex);

	bool ret = false;

	CAnimationStepManager* pStepAni = m_pSAManager->GetAnimationStepManager();

	// 	//获取到过程
	CProcessManager * processMgr = pStepAni->FindProcessManagerByID(processId);

	if (processMgr) {
		// 		//设置过程中的激活
		processMgr->SetCurProcessByID(stepId);

		ret = true;
	}

	return ret;
}

bool View::AnimationGetPlayCamera() {
	MutexLock locked(this->m_mutex);

	//LOGE("View::AnimationGetPlayCamera(bool isPlayCamera) %d", m_isAllowPlayCamera);
	return m_isAnimationPlayCamera;
}

string& View::GetAnimationXMLData() {
	return this->m_AnimationXMLData;
}

void View::SetAnimationXMLData(const string& xmlData) {
//    LOGI("SetAnimationXMLData %s ----", xmlData.c_str());
	m_AnimationXMLData = xmlData;
}

string View::GetDataManagerVersion() {
	wstring verStr(SVL_DATAMANAGER_VERSION);
	string retStr = WStringToString(verStr);
	return retStr;
}

IShape* View::GetShapeBySVLPath(const string& path) {
	string m3dPath = PathHelper::SVLPathToM3D(path);
	return GetShapeByM3DPath(m3dPath);
}

IShape* View::GetShapeByM3DPath(const string& path) {
	IShape* shape = NULL;
	shape = this->GetSceneManager()->GetShape(path);
	return shape;
}

IShape* View::GetShape(int shapeId) {
	IShape* shape = NULL;
	shape = this->GetSceneManager()->GetShape(shapeId);
	return shape;
}

bool View::SetBackgroundColor(const Color& topColor, const Color& bottomColor) {
	bool retState = false;
	string backgroundNodePath = BACKGROUNDCOLOR;

	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		backgroundNode->SetTopColor(topColor);
		backgroundNode->SetBottomColor(bottomColor);
	} else {
		retState = false;
	}

	return retState;
}

bool View::GetBackgroundColor(Color& topColor, Color& bottomColor) {
	bool retState = false;

	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		backgroundNode->GetTopColor(topColor);
		backgroundNode->GetBottomColor(bottomColor);
	} else {
		retState = false;
	}

	return retState;
}

bool View::SetBackgroundImage(const string& path, int mappingStyle) {
	bool setState = false;
	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		backgroundNode->SetImage(path, mappingStyle);

		setState = true;
	}

	return setState;
}

void View::SetBackgroundUseImage(bool useImage) {
	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		backgroundNode->SetUseImage(useImage);
	}

}

bool View::GetBackgroundUseImage() {
	bool state = false;
	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

		if (backgroundNode != NULL)
		{
			state = backgroundNode->IsUseImage();
		}


		return state;
}

	bool View::SetBackgroundWaterMark(const string& path, int mappingStyle)
	{
		bool setState = false;
		BackgroundNode* backgroundNode = (BackgroundNode*)this->GetSceneManager()->GetSceneRoot()->Search(M3D::BACKGROUNDCOLOR);

		if (backgroundNode != NULL)
		{
			backgroundNode->SetWaterMark(path, mappingStyle);

			setState = true;
		}


		return setState;
	}

	void View::SetBackgroundUseWaterMark(bool useWaterMark)
	{
		BackgroundNode* backgroundNode = (BackgroundNode*)this->GetSceneManager()->GetSceneRoot()->Search(M3D::BACKGROUNDCOLOR);

		if (backgroundNode != NULL)
		{
			backgroundNode->SetUseWaterMark(useWaterMark);
		}

	}

	bool View::GetBackgroundUseWaterMark()
	{
		bool state = false;
		BackgroundNode* backgroundNode = (BackgroundNode*)this->GetSceneManager()->GetSceneRoot()->Search(M3D::BACKGROUNDCOLOR);

		if (backgroundNode != NULL)
		{
			state = backgroundNode->IsUseWaterMark();
		}


	return state;
}

bool View::SetBackgroundTexture(Texture * texture) {
	bool setState = false;
	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		backgroundNode->SetTexture(texture);

		setState = true;
	}

	return setState;
}

bool View::AddBackgroundSkyBoxTexture(string name, Texture * texture) {
	bool setState = false;
	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		backgroundNode->AddSkyBoxTexture(name, texture);

		setState = true;
	}

	return setState;
}

void View::SetBackgroundUseSkyBox(bool useSkyBox) {
	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		backgroundNode->SetUseSkyBox(useSkyBox);
	}
}

bool View::GetBackgroundUseSkyBox() {
	bool state = false;
	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		state = backgroundNode->IsUseSkyBox();
	}
	return state;
}
bool View::GetBackgroundUseColor() {
	bool state = false;
	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		state = backgroundNode->IsUseColor();
	}
	return state;
}
void View::SetBackgroundUseColor(bool useColor) {
	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		backgroundNode->SetUseColor(useColor);
	}
}

void View::KeepBackgroundState() {
	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		backgroundNode->KeepBackgroundState();
	}
}

void View::RestoreBackgroundState() {
	BackgroundNode* backgroundNode =
			(BackgroundNode*) this->GetSceneManager()->GetSceneRoot()->Search(
					M3D::BACKGROUNDCOLOR);

	if (backgroundNode != NULL) {
		backgroundNode->RestoreBackgroundState();
	}
}
bool View::GetInterBackgroundState() {
	return this->m_isInterBackground;
}
void View::SetInterBackgroundState(bool isInter) {
	this->m_isInterBackground = isInter;
}

void View::LoadScene(const string& sceneInfo) {
	SceneLoader sceneLoader(this);
	sceneLoader.Load(sceneInfo);
}

void View::SetRefreshFun(void* refreshCallBackClassInstance,
		RefreshFun refreshFun) {
	m_RefreshCallBackClassInstance = refreshCallBackClassInstance;
	m_RefreshFun = refreshFun;
}

RefreshFun View::GetRefreshFun() {
	return m_RefreshFun;
}

void View::AnimationFinishCallback(void* viewInstance) {
	if (viewInstance != NULL) {
		View* view = (View*) viewInstance;

		//		view->m_SceneManager->Restore();
		//		view->m_SceneManager->ResetCamera();
		//		view->m_SceneManager->RequestRedraw();
	}
}

bool View::AllowTouch() {
	bool allowTouch = true;
	if (GetPerspectiveOperator()->GetAnimationState()) {
		allowTouch = false;
	}
	return allowTouch;
}

void View::SaveAnimatinState() {
	//存储全局PMI显隐状态,在动画播放完成之后进行恢复
	m_isAnimationShowPMI = Parameters::Instance()->m_IsShowPMIs;
	//允许全局PMI显示
	Parameters::Instance()->m_IsShowPMIs = true;
	//	m_isNeedChangeViewAnimation = false;
	//this->SetAllPMISVisible(false);
}

void View::RestoreAnimationState() {
	//结束视图插值动画
	GetPerspectiveOperator()->FinishAnimation();
	m_isNeedChangeViewAnimation = true;
	//this->SetAllPMISVisible(true);
	Parameters::Instance()->m_IsShowPMIs = m_isAnimationShowPMI;
}

void View::AnimationStartCallback(void* data) {
	View* view = (View*) data;

	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
			view->m_pSAManager->GetCurrentSA();
	if (NULL == pBehaviorAction)
		return;
	LOGI(" View::AnimationStartCallback(void* data)");
	view->SaveAnimatinState();
	pBehaviorAction->Continue();
	view->UpdateDrawLimit();
}

bool View::ReadSingleModel(const string& path, Model* & singleModel) {
	bool bRet = false;
	Model* model = NULL;
	string realPath = M3D::FileHelper::GetUnionStylePath(path);

	Reader* reader = Reader::GetReader(realPath);

	if (this->m_readListener) {
		reader->SetListener(this->m_readListener);
	}
	reader->SetView(this);
	model = reader->GetModel();

	if (model) {
		singleModel = model;
		//设置plcID
		int nPlcID = 0;
		srand(time(NULL));
		nPlcID = rand() % 10000;
		singleModel->SetPlcId(nPlcID);
		model->AddRef();
		bRet = true;
	}

	if (reader) {
		delete reader;
	}

	return bRet;
}

M3D_STATUS View::ReadFilesNormalMode(vector<string>& paths) {
	//构造顶级模型，用来挂载所有的子模型
	m_Model = new Model();
	m_Model->AddRef();

	m_Model->SetName("Root");

	m_CurFilePath = "";

	LOGI("View::ReadFile getmodel ok.");

	for (int i = 0; i < paths.size(); i++) {
		if (this->m_readListener) {
			this->m_readListener->SetReadFileIndex(i);
		}

		string& filePath = paths[i];
		if (filePath.size() > 0) {
			Model* subModel = NULL;
			this->ReadSingleModel(filePath, subModel);
			if (subModel) {
				m_Model->AddSubModel(subModel);
			}
		}
	}

	m_SceneManager->SetModel(m_Model);

	return M3D_STATUS::M_SUCCESS;
}

M3D_STATUS View::ReadFilesSpeedMode(vector<string>& paths) {
	//allowDraw = false;
	//构造顶级模型，用来挂载所有的子模型
	m_Model = new Model();
	m_Model->AddRef();
	m_Model->SetName("Root");

	m_CurFilePath = "";

	LOGI("View::ReadFile getmodel ok.");
	this->m_SceneManager->SetModel(m_Model);

	m_Model->Release();
	m_Model = NULL;

	for (int i = 0; i < paths.size(); i++) {
		if (this->m_readListener) {
			this->m_readListener->SetReadFileIndex(i);
		}

		string& filePath = paths[i];
		if (filePath.size() > 0) {
			Model* subModel = NULL;
			this->ReadSingleModel(filePath, subModel);
			if (subModel) {
				this->m_SceneManager->AddModel(subModel);
				//不用保留使用完成就释放掉
				subModel->Release();
			}
		}
	}

	//缓存动画文件
	//m_SceneManager->SetModel(m_Model);

	//m_SceneManager->OptimizeScene();

	//allowDraw = true;
	return M3D_STATUS::M_SUCCESS;
}

void View::SetUnitScale(float scale) {
	m_fUnitScale = scale;
	if (this->GetTouchHandler()) {
		//this->GetTouchHandler()->SetUpDirection(m_viewModeUpDirection, this);
	}
}

void View::ChechErrorPoint(M3D::Model* model) {
	if (model) {
		vector<Body*>* subBodys = model->GetBodys();
		if (subBodys) {
			for (int bi = 0; bi < subBodys->size(); bi++) {
				Body* subBody = dynamic_cast<Body*>(subBodys->at(bi));
				if (!subBody) {
					continue;
				}

				vector<Face*>& subFaces = subBody->GetFaces();

				for (int fi = 0; fi < subFaces.size(); fi++) {
					Mesh* renderMeshData = subFaces.at(fi)->GetRenderMeshData();
					if (renderMeshData && renderMeshData->GetRefMesh()) {
						VertexSet* vertexSet = renderMeshData->GetRefMesh();
						if (vertexSet) {
							vector<Vector3>* pointArray =
									vertexSet->GetPositionArray();
							if (pointArray != NULL) {
								for (int pi = 0; pi < pointArray->size();
										pi++) {
									static float checkLimint = 9.0e+5f;
									Vector3& checkPoint = pointArray->at(pi);
									if (checkPoint.m_x < -checkLimint
											|| checkPoint.m_x > checkLimint
											|| checkPoint.m_y < -checkLimint
											|| checkPoint.m_y > checkLimint
											|| checkPoint.m_z < -checkLimint
											|| checkPoint.m_z > checkLimint) {
										checkPoint.m_x = 0;
										checkPoint.m_y = 0;
										checkPoint.m_z = 0;
									}

									if (IsNaN(checkPoint.m_x)
											|| IsNaN(checkPoint.m_y)
											|| IsNaN(checkPoint.m_z)) {
										checkPoint.m_x = 0;
										checkPoint.m_y = 0;
										checkPoint.m_z = 0;
									}
								}
							}
						}
						break;;
					}
				}
			}
		}

		for (int i = 0; i < model->GetSubModels().size(); i++) {
			ChechErrorPoint(model->GetSubModels().at(i));
		}
	}
}

bool View::GetAnimationDisState(ControlInfo& controlInfo) {
	MutexLock locked(this->m_mutex);
	bool ret = false;
	ControlInfo cameraState;
	NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction =
			m_pSAManager->GetCurrentSA();
	if (NULL == pBehaviorAction)
		return false;

	float cameraMatrix[16];
	float scales[3];

	bool isCameraUseCamera = pBehaviorAction->GetPlayBeginCamera(cameraMatrix,
			scales);

	if (isCameraUseCamera) {
		NS_SimulationAnimation::CSimulationAnimationManager* simMgr =
				this->GetSimulationMgr();
		string animationVersion = "1.0";
		if (simMgr != NULL) {
			animationVersion = simMgr->GetCurrentSA()->GetVersion();
		}

		float scale = scales[0];

		if (atof(animationVersion.c_str()) >= 1.15f && ((int) scales[1]) > 0
				&& ((int) scales[2]) > 0) {
			float width = 1, height = 1;
			CameraNode* camera = this->GetSceneManager()->GetCamera();
			if (camera != NULL) {
				camera->GetOrthoSize(&width, &height);
				camera->SetRotateCenter(Vector3(0, 0, 0));
			}

			float dx = scales[1] / width;
			float dy = scales[2] / height;

			if (dx > dy) {
				scale = scale / dx;
			} else {
				scale = scale / dy;
			}

			scales[0] = scale * 0.8;
		}
		float tmatrix[4][4] = { 0 };
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				tmatrix[i][j] = cameraMatrix[i * 4 + j];
			}
		}
		tmatrix[3][3] = 1;

		Quaternion rotation((float*) tmatrix);
		M3D::Vector3 vec(cameraMatrix[3 * 4 + 0], cameraMatrix[3 * 4 + 1],
				cameraMatrix[3 * 4 + 2]);

		controlInfo.scaleFactor = 1 / scales[0];

		controlInfo.moveVector = vec;
		controlInfo.rotation = rotation;

		ret = true;
	}

	return ret;
}

const string& View::GetUserData(const string& key) {
	map<string, string>::iterator findKey = m_userDatas.find(key);
	if (findKey != m_userDatas.end()) {
		return findKey->second;
	}

	return M3D::NO_VALUE;
}

void View::AddUserData(const string& key, const string& value) {
	if (key.length() > 0) {
		m_userDatas[key] = value;
	}
}

void View::InitRender(int GLESVersion) {
	GetSceneManager()->GetRenderManager()->SetGLVersion(GLESVersion);
	GetSceneManager()->GetRenderManager()->InitialRender();
}

void View::IsUseStereoMode(bool state) {
	if (state) {
		setWalkThrough(HANDLER_WALKTHROUGH);
		Parameters::Instance()->m_useStereo = true;
	} else {
		setWalkThrough(HANDLER_COMMON);
		Parameters::Instance()->m_useStereo = false;
	}
}

bool View::GetStereoModeState() {
	return Parameters::Instance()->m_useStereo;
}

set<string> View::GetSourceFiles() {
	return m_srcFiles;
}
string View::GetShapeProperties(int shapeId) {
	string properties;
	IShape* shape = this->GetSceneManager()->GetShape(shapeId);
	if (shape) {
		//properties = shape->GetProperties(properties);
	}
	return properties;
}

bool View::SetShapePorperties(int shapeId, const string& properties) {
	bool state = false;
	IShape* shape = this->GetSceneManager()->GetShape(shapeId);
	if (shape) {
		//shape->SetProperties(properties);
	}

	return state;
}

string View::GetShapeStates(const string& value) {
	Json::Reader reader;
	Json::Value shapeValue;
	Json::Value retJson;

	if (reader.parse(value.c_str(), shapeValue)) {
		if (shapeValue["GetID"].asBool()) {
			string realPath = shapeValue["Path"].asString();
			IShape* shape = this->GetShapeBySVLPath(realPath);

			if (shape) {

				retJson["ID"] = shape->GetID();
			} else {
				retJson["ID"] = -1;
			}
		}

		if (shapeValue["GetPath"].asBool()) {
			IShape* shape = this->GetShape(shapeValue["ID"].asInt());
			if (shape) {
				Model* model = static_cast<Model*>(shape);
				if (model) {
					string realPath = model->GetPlcPath();
					retJson["Path"] = string(realPath.begin() + 9,
							realPath.end());
				}
			}
		}

		if (shapeValue["GetClr"].asBool()) {
			Json::Value colorValue;

			IShape* shape = this->GetShape(shapeValue["ID"].asInt());
			if (shape) {
				Color* color = shape->GetColor();
				colorValue["ClrR"] = color->m_r;
				colorValue["ClrG"] = color->m_g;
				colorValue["ClrB"] = color->m_b;
				colorValue["ClrA"] = color->m_a;

				retJson["Clr"] = colorValue;
			}

		}

		if (shapeValue["GetTran"].asBool()) {
			IShape* shape = this->GetShape(shapeValue["ID"].asInt());
			if (shape) {
				Model* model = static_cast<Model*>(shape);
				if (model) {

					ModelShape* modelShape = model->GetModelShape();
					if (modelShape) {
						Matrix3x4& woldMatrix = modelShape->GetWorldTransform();
						Json::Value tranValue;

						Vector3 pos = woldMatrix.Translation();
						LOGI("pos  m_x = %f ", pos.m_x);

						tranValue["PosX"] = pos.m_x;
						tranValue["PosY"] = pos.m_y;
						tranValue["PosZ"] = pos.m_z;

						Quaternion rotation = woldMatrix.Rotation();
						tranValue["RotX"] = rotation.m_x;
						tranValue["RotY"] = rotation.m_y;
						tranValue["RotZ"] = rotation.m_z;
						tranValue["RotW"] = rotation.m_w;

						Vector3 scale = woldMatrix.Scale();

						tranValue["SclX"] = scale.m_x;
						tranValue["SclY"] = scale.m_y;
						tranValue["SclZ"] = scale.m_z;

						retJson["Tran"] = tranValue;
					}
				}
			}
		}
		if (shapeValue["GetWorldPosition"].asBool()) {
			IShape* shape = this->GetShape(shapeValue["ID"].asInt());
			if (shape) {
				Model* model = dynamic_cast<Model*>(shape);
				if (model) {
					Vector3 pos = model->GetWorldBoundingBox().Center();

					Json::Value tranValue;

					tranValue["PosX"] = pos.m_x;
					tranValue["PosY"] = pos.m_y;
					tranValue["PosZ"] = pos.m_z;

					retJson["GetWorldPosition"] = tranValue;
				}
			}
		}
	}

	return retJson.toStyledString();
}
bool View::SetShapeStates(const string& value) {
	bool ret = false;

	Json::Reader reader;

	Json::Value shapeValue;

	if (reader.parse(value.c_str(), shapeValue)) {

		IShape* shape;
		int shapeId = shapeValue["ID"].asInt();

		shape = this->GetShape(shapeId);
		if (!shape) {
			return false;
		}

		if (shapeValue["SetClr"].asBool()) {
			Json::Value colorValue = shapeValue["Clr"];

			reader.parse(shapeValue["Clr"].asString().c_str(), colorValue);

			Color color;
			color.m_r = colorValue["ClrR"].asFloat();
			color.m_g = colorValue["ClrG"].asFloat();
			color.m_b = colorValue["ClrB"].asFloat();
			color.m_a = colorValue["ClrA"].asFloat();

			shape->SetColor(color);
		}

		if (shapeValue["SetTran"].asBool()) {
			Json::Value tranValue;
			reader.parse(shapeValue["Tran"].asString().c_str(), tranValue);

			Vector3 pos;

			pos.m_x = tranValue["PosX"].asFloat();
			pos.m_y = tranValue["PosY"].asFloat();
			pos.m_z = tranValue["PosZ"].asFloat();

			Quaternion rotation;
			rotation.m_x = tranValue["RotX"].asFloat();
			rotation.m_y = tranValue["RotY"].asFloat();
			rotation.m_z = tranValue["RotZ"].asFloat();
			rotation.m_w = tranValue["RotW"].asFloat();

			Vector3 scale;
			scale.m_x = tranValue["SclX"].asFloat();
			scale.m_y = tranValue["SclY"].asFloat();
			scale.m_z = tranValue["SclZ"].asFloat();

			Model* model = static_cast<Model*>(shape);
			if (model) {
				Matrix3x4 transform(pos, rotation, scale);
				ModelShape* shapeNode = model->GetModelShape();
				if (shapeNode) {
					Matrix3x4& modelTransform = model->GetWorldTransform();
					Matrix3x4 transform(pos, rotation, scale);
					transform = modelTransform.Inverse() * transform;
					shapeNode->SetWorldMatrix(transform);
				}
			}
		}

		if (shapeValue["SetSel"].asBool()) {
			bool sel = shapeValue["Sel"].asBool();

			if (sel) {
				this->GetSelector()->Add(shape);
			} else {
				this->GetSelector()->Remove(shape);
			}
		}
	}

	return ret;
}

void View::SetAllPMISVisible(bool isVisible) {
	//LOGI("View::SetPMIVisible cnt:%d pmiID:%d",this->GetModel()->m_AllPMIMap.size(),pmiID);
	bool ret = true;
	if (this->GetModel()) {
		map<int, PMIData*>* pmis = this->GetModel()->GetPMIs();
		if (pmis) {
			map<int, PMIData*>::iterator it = pmis->begin();
			map<int, PMIData*>::iterator end = pmis->end();
			while (it != end) {
				PMIData* pmi = (*it).second;
				pmi->SetVisible(isVisible);
				it++;
			}
		}
	}
}

void View::UpDateAnimationLimit() {
	Trackball::DrawLimit = m_DrawLimit;
}

NS_SimulationAnimation::CSimulationAnimationManager* View::GetSimulationMgr() {
	return this->m_pSAManager;
}

void View::SetSimulationMgr(
		NS_SimulationAnimation::CSimulationAnimationManager* pSAManager) {
	this->m_pSAManager = pSAManager;
	if (pTDriver == NULL) {
		pTDriver = new CTickTimer();
		pTDriver->Init();
	}
	//this->m_pSAManager->SetView(this);
	InitAinmationPlayCB();
}

bool View::CloseSceneAnimation() {
	this->m_TouchEventHandler->CloseKeepState();
	this->m_walkthroughTouchHandler->CloseKeepState();
	this->m_draggerHandler->CloseKeepState();
	return true;
}
void View::UpdateDrawLimit() {
	//	m_DrawLimit = 0;
	//	CameraStateAction* action = new CameraStateAction;
	//
	//	Trackball::ISMOVING = true;
	//	Trackball::ISROTATING = true;
	//	Trackball::MOVESTATE = 0;
	//	Trackball::KEEPINGSTATETIMES = Trackball::TIMES;
	//	SceneManager* pSceneManager = this->GetSceneManager();
	//	if (pSceneManager != NULL)
	//	{
	//		CameraStateAction* action = new CameraStateAction;
	//		action->SetPath(MAINCAMERA);
	//		this->GetSceneManager()->ApplyAction(action);
	//		if (Parameters::Instance()->m_RemoveMode == 0)
	//		{
	//			m_DrawLimit = Trackball::CURRENTMODELSIZE
	//					* Parameters::Instance()->m_RemoveSize / 100;
	//		}
	//		else if (Parameters::Instance()->m_RemoveMode == 1)
	//		{
	//			float tempCurPoint1[3];
	//			float tempCurPoint2[3];
	//			gluUnProjectf(0, 0, 0, Matrix::GetIdentityMatrix().GetData(),
	//					action->m_ProjectMatrixCache.GetData(),
	//					action->m_ViewportCache, &tempCurPoint1[0],
	//					&tempCurPoint1[1], &tempCurPoint1[2]);
	//
	//			gluUnProjectf(0, Parameters::Instance()->m_RemoveSize * 3, 0,
	//					Matrix::GetIdentityMatrix().GetData(),
	//					action->m_ProjectMatrixCache.GetData(),
	//					action->m_ViewportCache, &tempCurPoint2[0],
	//					&tempCurPoint2[1], &tempCurPoint2[2]);
	//
	//			m_DrawLimit = VectorMagnitude(
	//					Vector3(tempCurPoint1[0], tempCurPoint1[1], tempCurPoint1[2])
	//							- Vector3(tempCurPoint2[0], tempCurPoint2[1],
	//									tempCurPoint2[2]));
	//
	//		}
	//		delete action;
	//	Trackball::DrawLimit = m_DrawLimit;
	//	}

	}
	void View::StartExplosition(vector<Model*> models)
	{
		this->GetExplosiveView()->startExplosion(models);
	}
	void View::endExplosition()
	{
		this->GetExplosiveView()->endExplosion();
	}
	bool View::SetExplosiveViewWithDirection(vector<Model*> arrayModels,int stype, int pos, Vector3 direction)
	{
		this->GetExplosiveView()->setPercentWithDirection(this, arrayModels,stype, pos, direction);
		return false;
	}
	bool View::SetExplosiveView(int stype, int pos, bool useAnimation)
	{
		this->GetExplosiveView()->SetPercent(this, stype, pos, useAnimation);
//        if (this->GetCurrentModelView()) {
//            this->GetCurrentModelView()->setExplosivePercent(pos);
//        }
	return false;
}
bool View::SetExplosiveView(vector<Model*> arrayModels, int stype, int pos,
		bool useAnimation) {
	this->GetExplosiveView()->SetPercent(this, arrayModels, stype, pos,
			useAnimation);

	return false;
}
bool View::SetExplosiveViewWithoutRestore(int stype, int pos,
		bool useAnimation) {
	this->GetExplosiveView()->SetPercentWithoutRestore(this, stype, pos,
			useAnimation);

	return false;
}

ExplosiveViewOperator* View::GetExplosiveView() {
	if (this->m_ExplosiveViewOperator == NULL) {
		this->m_ExplosiveViewOperator = new ExplosiveViewOperator();
		this->m_ExplosiveViewOperator->SetView(this);
	}
	return this->m_ExplosiveViewOperator;
}

bool View::CloseExplisiveView() {
	if (this->m_ExplosiveViewOperator != NULL) {
		this->m_ExplosiveViewOperator->Close(this);
	}

//        this->RestoreView();

	return false;
}

bool View::FitScaleView() {
	bool ret = false;
	ret = ViewOperator::FitView(this->GetSceneManager());
	return ret;
}

void View::SetShowAxis(bool show) {
	if (this->m_axisNode) {
		this->m_axisNode->SetVisible(show);
	}
}

bool View::IsShowAxis() {
	bool show = false;

	if (this->m_axisNode) {
		show = (this->m_axisNode->IsVisible());
	}

	return show;
}

void View::InitCamera(bool useAni) {
	this->m_workTouchHandler->Open(useAni);
}

//void View::ReSetCamera()
//{
//	//this->m_workTouchHandler->RefreshModelViewCamera();
//}

void View::SetIsDragger(bool isDragger) {
	m_isDragger = isDragger;
}

void View::Lock() {
	m_mutex.Acquire();
}

void View::UnLock() {
	m_mutex.Release();
}

void View::OnAnimationTick() {
	if (pTDriver != NULL) {
		pTDriver->OnTimerTick();
	}
}

void View::StartRotateAndExplosiveTimer() {
	if (m_rotateAndExplosiveTimer.IsStart()) {
		m_rotateAndExplosiveTimer.StopTimer();
	}

	m_rotateAndExplosiveTimer.StartTimer();
	m_rotateAndExplosiveTimer.SetTimer(RotateAndExplosiveTask, this, 0, (41));
}

void View::EndRotateAndExplosiveTimer() {
	if (m_rotateAndExplosiveTimer.IsStart()) {
		m_rotateAndExplosiveTimer.StopTimer();
	}
}

bool View::m_isNeedRotate = true;
bool View::m_excStep1 = false;
bool View::m_excStep2 = false;
bool View::isRotateComplete = false;
float View::explosivePercent = 0;
float View::explorsiveSpeed = 1.0;

void * View::RotateAndExplosiveTask(void * data) {
	View * sview = (View *) data;
	TouchHandler* handler = sview->GetTouchHandler();
	if (m_isNeedRotate) {
		handler->GetRotateOnScreenMiddleLIneFunc(handler);
	}
	if (handler->m_rotCircleCount == 2) {
		m_isNeedRotate = false;
		isRotateComplete = true;
		m_excStep1 = true;
		sview->SetExplosiveViewWithoutRestore(0,
				100 * abs(sin(explosivePercent))) * 0.3;
		explosivePercent += 3.1415926 / 180.0 * 0.3 * explorsiveSpeed;
		if (explosivePercent >= 3.1415926 / 2.0) {
			m_isNeedRotate = true;
			handler->m_rotCircleCount = 0;
		}
	}

	if (handler->m_rotCircleCount == 1 && isRotateComplete) {
		m_isNeedRotate = false;
		handler->OnlyRotateOnAxisFunc(handler);
		sview->SetExplosiveViewWithoutRestore(0,
				100 * abs(sin(explosivePercent))) * 0.3;
		explosivePercent -= 3.1415926 / 180.0 * 0.3 * explorsiveSpeed;
		if (explosivePercent <= 0) {
			m_isNeedRotate = true;
			explosivePercent = 0.0;
			sview->SetExplosiveViewWithoutRestore(0, 0);
			isRotateComplete = false;
			handler->m_rotCircleCount = 0;
		}
	}
	sview->RequestDraw();
	return NULL;
}

map<int, vector<string> > View::GetFixPMIInfo() {
	m_fixPMIInfo.clear();
	bool ret = true;
	map<int, PMIData*>* pmis = this->GetModel()->GetPMIs();
	if (pmis) {
		//        map<int, PMIData*>& pmis=this->GetModel()->GetPMIs();
		map<int, PMIData*>::iterator it = pmis->begin();
		map<int, PMIData*>::iterator end = pmis->end();
		while (it != end) {
			PMIData* pmi = (*it).second;
			vector<string> vecText;
			if (pmi->m_IsParallelScreen && (pmi->m_Lines.size() == 0)) {
				for (int i = 0; i < pmi->m_ComTexts.size(); i++) {
					ComText* comText = pmi->m_ComTexts.at(i);
					for (int j = 0; j < comText->GetCTextList().size(); j++) {
						CText* text = comText->GetCTextList().at(j);
						string uniText = text->GetText();
						//ChinesCode:UTF_8ToUnicode(uniText,text->GetText().c_str());
						//string strText=WStringToString(uniText);
						vecText.push_back(uniText);
					}
				}
				m_fixPMIInfo.insert(
						map<int, vector<string> >::value_type(it->first,
								vecText));
			}
			it++;
		}
	}
	return m_fixPMIInfo;
}

void View::KeepViewModeState() {
	m_viewModeTouchHandlerType =
			this->m_workTouchHandler->GetTouchHandlerType();
	m_viewModeUpDirection = this->m_workTouchHandler->GetCurrentUpDirection();
	this->m_workTouchHandler->GetCurrentObservingPattern(m_viewModeOribit,
			m_viewModeFreeLook, m_viewModeControlLockXY);
}

void View::RestoreViewModeState() {
	this->setWalkThrough(m_viewModeTouchHandlerType);
	this->GetTouchHandler()->SetUpDirection(m_viewModeUpDirection, this);
	if (m_viewModeOribit) {
		GetTouchHandler()->OribitMode(true);
	} else if (m_viewModeFreeLook) {
		GetTouchHandler()->FreeViewMode(true);
	} else if (m_viewModeControlLockXY) {
		GetTouchHandler()->ConstraintMode(true);
	}

}
M3D_STATUS View::LoadAttribute() {
	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;

	//如果"readfrombufferXXX.svlx" 是远端模型标识
	if (m_CurFilePath != "readfrombuffer.svlx") {
		m_Reader = Reader::GetReader(m_CurFilePath);
		SVL2AsynReader* m_SVL2Reader = dynamic_cast<SVL2AsynReader*>(m_Reader);
		if (m_SVL2Reader) {
			try {
				m_SVL2Reader->LoadAttribute(m_Model, m_CurFilePath, this);
			} catch (const std::bad_alloc& e) {
				LOGE("C++ 内存分配失败...%s", e.what());
				readerState = M_Read_OOM;
				return readerState;
			} catch (const std::ios_base::failure& e) {
				LOGE("C++ IO 异常...%s", e.what());
				readerState = M_Read_ANALYSIS_ERROR;
				return readerState;
			} catch (...) {
				LOGE("抱歉..未知错误...请联系开发人员..");
				readerState = M_Read_NO_DEFINE_Error;
				return readerState;
			}
		}
		if (m_Reader) {
			delete m_Reader;
			m_Reader = NULL;
		}
	}

	readerState = M_Read_OK;
	return readerState;
}

bool View::IsLoadAttribute() {
	bool IsHave = false;
	HasModelAttribute(m_Model, IsHave);
	return IsHave;
}

void View::HasModelAttribute(Model* model, bool& bHas) {
	if (!model || bHas) {
		return;
	}
	vector<Body*>* bodyList = model->GetBodys();
	if (bodyList) {
		for (vector<Body*>::iterator it = bodyList->begin();
				it != bodyList->end(); it++) {
			if (*it) {
				vector<Face*> faceList = (*it)->GetFaces();
				for (int i = 0; i < faceList.size(); i++) {
					Face* face = faceList[i];
					if (face && face->GetData()) {
						Mesh* facemeshData = face->GetData();
						if (facemeshData && facemeshData->GetGeoAttribute()) {
							bHas = true;
							return;
						}
					}
				}
				vector<Edge*>* EdgeList = (*it)->GetEdges();
				for (int i = 0; i < EdgeList->size(); i++) {
					Edge* edge = EdgeList->at(i);
					if (edge && edge->GetLineData()) {
						RefPolyLine * edgeLine = edge->GetLineData();
						if (edgeLine && edgeLine->GetGeoAttribute()) {
							bHas = true;
							return;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < model->GetSubModelCount(); i++) {
		HasModelAttribute(model->GetSubModels().at(i), bHas);
		if (bHas) {
			return;
		}
	}
}

void View::OpenForceNormalEffect() {
	this->GetRenderManager()->SetISForceNormalEffect(true);
}
void View::CloseForceNormalEffect() {
	this->GetRenderManager()->SetISForceNormalEffect(false);
}
bool View::GetConstraintMode() {
	return m_workTouchHandler->GetConstraintMode();
}

int View::GetWalkThroughDirection() {
	int iDirection = 2;
	Vector3 upDirection = this->m_workTouchHandler->GetCurrentUpDirection();
	if (upDirection == Vector3::RIGHT) {
		iDirection = 0;
	} else if (upDirection == Vector3::LEFT) {
		iDirection = 1;
	} else if (upDirection == Vector3::UP) {
		iDirection = 2;
	} else if (upDirection == Vector3::DOWN) {
		iDirection = 3;
	} else if (upDirection == Vector3::FORWARD) {
		iDirection = 4;
	} else if (upDirection == Vector3::BACK) {
		iDirection = 5;

	}
	return iDirection;
}

M3D::DraggerManager* View::GetDraggerManager() {
	if (!this->_draggerManager) {
		this->_draggerManager = new DraggerManager(this);
		this->_draggerManager->AddRef();
	}
	return _draggerManager;
}

M3D::SectionManager* View::GetSectionManager() {
	if (!this->_sectionManager) {
		this->_sectionManager = new SectionManager(this);
		this->_sectionManager->AddRef();
	}
	return _sectionManager;
}

void View::UpdateAnimationInitTargetObjects(
		vector<TARGETOBJECTINFO*> &vcObjectInfo) {
	if (!m_pSAManager || vcObjectInfo.size() <= 0)
		return;
	m_pSAManager->UpdateInitTargetObjectList(vcObjectInfo);
}

int View::GetAnimationInitTargetObjectsCount() {
	if (!m_pSAManager)
		return 0;
	return m_pSAManager->GetInitTargetObjectCount();
}

float View::GetUnitScale() {
	return m_fUnitScale;
}

void View::InitAinmationPlayCB() {
	if (m_pAnimationCallBackFunction) {
		m_pAnimationCallBackFunction->InitAinmationPlayCB();
	}
}

void View::SetModelSelectedCB(ModelSelectedCB* modelSelectedCB) {
	this->m_modelSelectedCB = modelSelectedCB;
}

void View::NotifyModelSected(M3D::IShape* shape, bool selected) {
	if (this->m_modelSelectedCB) {
		if (shape) {
			this->m_modelSelectedCB(shape->GetID(), selected);
		} else {
			this->m_modelSelectedCB(-1, selected);
		}
	}
}

Vector3 View::getRayIntersectNormal() {
	return this->GetSceneManager()->rayIntersectNormal;
}
Vector3 View::getRayIntersectPos() {
	return this->GetSceneManager()->rayIntersectPos;
}

void View::updateScreenBox() {
	this->GetSceneManager()->GetSceneBox().Clear();
}
//批注数据解析
void View::ParseAnnotation(const string& value) {
	Json::Reader reader;
	Json::FastWriter writer;
	Json::Value json;
	Json::Value retJson;
	SceneManager *scene = GetSceneManager();
	//清空
	scene->GetNoteGroup()->DeleteAllChildren();
	m_allTextNoteJsonData.clear();
	m_allSequeNoteJsonData.clear();
	m_allGeasureNoteJsonData.clear();
	if (reader.parse(value.c_str(), json)) {
		retJson = json["annotations"];
		if (retJson.isArray()) {
			int iSize = retJson.size();
			for (int i = 0; i < iSize; i++) {
				Json::Value annoValue = retJson[i];
				int type = annoValue["type"].asInt();
				int _id = annoValue["createID"].asInt();
				//判断数据是否有效
				vector<ModelView*>* allViews =
						scene->GetModel()->GetModelViewList();
				bool valid = false;
				if (allViews) {
					for (vector<ModelView*>::iterator it = allViews->begin();
							it != allViews->end(); it++) {
						vector<int> ids = (*it)->GetNoteList();
						vector<int>::iterator result = find(ids.begin(),
								ids.end(), _id); //查找3
						if (result != ids.end()) { //找到
							valid = true;
							break;
						}
					}
				}
				if (valid) {
					switch (type) {
					case 0: { //基本-文本
						string jsonValue = writer.write(annoValue);
						this->AddTextJsonData(StringHelper::IntToString(_id),
								jsonValue);
//                                Note *pNode = NoteFactory::CreateTextNoteFromJSON(scene, jsonValue);
					}
						break;
					case 1: //零组件
						break;
					case 2: { //序号
						string jsonValue = writer.write(annoValue);
						this->AddSequenceJsonData(
								StringHelper::IntToString(_id), jsonValue);
//                                Note *pNode = NoteFactory::CreateSequenceNoteFromJSON(scene, jsonValue);
					}
						break;
					case 1002: { //手势批注
						string jsonValue = writer.write(annoValue);
						this->AddGestureJsonData(StringHelper::IntToString(_id),
								jsonValue);
//                                Note *pNode = NoteFactory::CreateThreeDGestureNoteFromJson(scene, jsonValue);
					}
						break;
					default:
						break;
					}
				}
			}
		}
	}
}
const string& View::GetGestureJsonData(const string& key) {
	map<string, string>::iterator findKey = m_allGeasureNoteJsonData.find(key);
	if (findKey != m_allGeasureNoteJsonData.end()) {
		return findKey->second;
	}

	return M3D::NO_VALUE;
}

void View::AddGestureJsonData(const string& key, const string& value) {
	if (key.length() > 0) {
		m_allGeasureNoteJsonData[key] = value;
	}
}
const string& View::GetTextJsonData(const string& key) {
	map<string, string>::iterator findKey = m_allTextNoteJsonData.find(key);
	if (findKey != m_allTextNoteJsonData.end()) {
		return findKey->second;
	}

	return M3D::NO_VALUE;
}

void View::AddTextJsonData(const string& key, const string& value) {
	if (key.length() > 0) {
		m_allTextNoteJsonData[key] = value;
	}
}
const string& View::GetSequenceJsonData(const string& key) {
	map<string, string>::iterator findKey = m_allSequeNoteJsonData.find(key);
	if (findKey != m_allSequeNoteJsonData.end()) {
		return findKey->second;
	}

	return M3D::NO_VALUE;
}

void View::AddSequenceJsonData(const string& key, const string& value) {
	if (key.length() > 0) {
		m_allSequeNoteJsonData[key] = value;
	}
}
}
///namespace
