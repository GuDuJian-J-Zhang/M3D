#include "sview/views/View.h"
#include "sview/views/Selector.h"
#include "sview/views/Parameters.h"

#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/ResourceManager.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/scene/ShapeNode.h"

#include "m3d/graphics/Light.h"

#include "m3d/scene/BackgroundNode.h"
#include "m3d/scene/AxisNode.h"
#include "m3d/scene/FPSNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/scene/GroupNode.h"

#include "m3d/Handler/HandlerNode.h"
#include "m3d/Handler/HandlerGroup.h"
#include "m3d/Handler/HandlerNodeRotateCenter.h"

#include "m3d/M3DMacros.h"
#include "m3d/utils/M3DTools.h"
#include "sview/extern/PerspectiveData.h"
#include "sview/extern/PerspectiveOperator.h"
#include "sview/extern/SectionOperator.h"

#include "m3d/model/Shape.h"
#include "m3d/model/Model.h"

#include "sview/io/Reader.h"
#include "sview/io/Writer.h"
#include "sview/io/XmlSceneSerializer.h"

#include "sview/manipulator/CommonTouchHandler.h"
#include "sview/manipulator/WalkthroughHandler.h"
#include "sview/manipulator/OribitControlHandler.h"
#include "sview/manipulator/DraggerHandler.h"

#include "sview/extern/ExplosiveViewOperator.h"

#include "m3d/graphics/Section.h"
#include "m3d/model/Note.h"
#include "m3d/model/PMIData.h"
#include "m3d/base/Vector3.h"
#include "m3d/utils/IDCreator.h"
#include "m3d/model/ModelView.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/base/Matrix4.h"
#include "m3d/graphics/MovedMatrixInfo.h"

#include "m3d/utils/FileHelper.h"

#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"

#include "animation/SimulationAnimationPlay/AnimationPlayApi.h"
#include "animation/SimulationAnimation/TickTimer.h"
#include "animation/SimulationAnimation/SBehaviorAction.h"
#include "animation/SimulationAnimation/SimulationAnimationManager.h"
#include "animation/SimulationAnimation/AnimationStepManager.h"
#include "animation/SimulationAnimation/CProcess.h"
#include "animation/SimulationAnimation/ProcessManager.h"

#include "m3d/model/PolyLine.h"
#include "Stk_Confige.h"
#include "m3d/base/Viewport.h"

#include "m3d/base/json/json.h"
#include "sview/extern/ViewOperator.h"

#include "m3d/extras/note/NoteGroup.h"
#include "m3d/extras/note/TextNote.h"
#include "m3d/extras/note/VoiceNote.h"
#include "m3d/extras/note/NoteFactory.h"
#include "m3d/model/ModelView.h"
#include "m3d/handler/HandlerPoint.h"
#include "m3d/extras/note/SequenceNumberNote.h"
#include "m3d/utils/PathHelper.h"
#include "m3d/extras/modelmanager/ModelManager.h"
#include "m3d/extras/OperationHistoryManager.h"
#include "m3d/scene/SceneGroundNode.h"
#include "m3d/utils/Platform.h"
using namespace M3D;
#include "m3d/extras/note/ThreeDGesturesNote.h"

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
	this->m_readPercent = this->m_readFileIndex * this->m_readSegRange
			+ percent * this->m_readSegRange;
}

void ViewReadListener::OnBegin(Reader* reader) {
	this->m_readPercent = 0.0f;
}

void ViewReadListener::OnEnd(Reader* reader) {
	this->m_readPercent = 1.0f;
}

float ViewReadListener::GetPercent() {
	return this->m_readPercent;
}

void ViewReadListener::SetPercent(float percent) {
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

	m_isAnimationPlayCamera = false;
	m_animationPlaySpeed = 1.0f;
	m_animationContinuousPlay = true;

	m_curDrawMode = 0;

	m_isAnimationShowPMI = false;
	this->m_ExplosiveViewOperator = NULL;
	this->m_isNeedChangeViewAnimation = false;
	this->m_backgroundNode = NULL;
	m_DrawLimit = 0.0f;

	m_draggerInit = false;
	m_isDragger = false;
	m_enableCreateDefaultView = false;

	m_curModelView = NULL;

	m_readListener = new ViewReadListener();
	m_readListener->AddRef();

	this->m_perpectiveOperator = NULL;

	m_isSpeedModel = false;
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
	if (m_pWorkNodeGroup != NULL) {
		delete m_pWorkNodeGroup;
		m_pWorkNodeGroup = NULL;
	}
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

	if (this->m_perpectiveOperator)
	{
		delete m_perpectiveOperator;
		m_perpectiveOperator = NULL;
	}

	dumpMemoryLeaks();
}

void View::Initial() {
	if (!m_isInitialized) {
		LOGI("View::InitializeScene");
		m_pWorkNodeGroup = new WorkNodes();

		m_isInitialized = true;
		m_Parameters = Parameters::Instance();

		m_IDCreator = new IDCreator;
		m_TouchEventHandler = new CommonTouchHandler;
		m_walkthroughTouchHandler = new WalkthroughHandler;

//		m_oribitControlTouchHandler = new OribitControlHandler;

		m_draggerHandler = new DraggerHandler;

		m_workTouchHandler = m_TouchEventHandler;

		SceneManager* sceneManager = new SceneManager;
		SetSceneManager(sceneManager);

		m_Selector = new Selector(sceneManager);

		CameraNode *camera = new CameraNode;
		camera->SetName(M3D::MAINCAMERA);
//		camera->SetOrthographic(false);
//		camera->SetProjectionMode(M3D::ORTHO);
//		camera->SetViewVolume(20, 1.0f, 100.0f, 20.0f);
//		camera->SetPosition(M3D::Vector3(0, 0, 20.0f));

		sceneManager->SetCamera(camera);
		//camera1->SetTranslateStep(0.1f);

		Light* dirLight0 = new Light();
		dirLight0->SetType(SHAPE_LIGHT_DIRECTIONAL);
		//dirLight0->SetName(M3D::MAINDIRTLIGHT);
//		dirLight0->SetColor(0.8f, 0.8f, 0.8f);
//		dirLight0->SetDiffuseIntensity(0.7f);
//		dirLight0->SetAmbientIntensity(0.15f);
//		dirLight0->SetSpecularIntensity(0.745f);
		//dirLight0->SetPosition(M3D::Vector3(-0.8f, -1.0f, 1.0f));
		dirLight0->TurnOn();

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

		sceneManager->AddLight(dirLight0);

		//mainNode->AddChild(dirLight0);
		//mainNode->AddChild(camera1);

		GroupNode * groupNode = (GroupNode*) sceneManager->GetSceneRoot();
		groupNode->SetName(M3D::ROOT);

		BackgroundNode* backgroundNode = new BackgroundNode;
		backgroundNode->SetName(M3D::BACKGROUNDCOLOR);
		Color topColor(0.25f, 0.63f, 1.0f, 1.0f);
		Color buttomColor(0.88f, 0.96f, 1.0f, 1.0f);
		backgroundNode->SetBottomColor(buttomColor);
		backgroundNode->SetTopColor(topColor);
		groupNode->AddChild(backgroundNode);

		this->m_backgroundNode = backgroundNode;

		AxisNode* axisNode = new AxisNode();
		axisNode->SetName(M3D::AXIS);
		groupNode->AddChild(axisNode);

		this->m_axisNode = axisNode;

		FPSNode* fpsNode = new FPSNode();
		fpsNode->SetName(M3D::FPS_FLAG);
		groupNode->AddChild(fpsNode);

		SceneGroundNode * sceneGroundNode = new SceneGroundNode;
		sceneGroundNode->SetName(M3D::SCENEGROUND);
		groupNode->AddChild(sceneGroundNode);
		this->m_sceneGroundNode = sceneGroundNode;

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
		LOGI("View::InitializeScene end");
	}
}

void View::TouchDown(float* p, int n) {
	MutexLock locked(this->m_mutex);
	if (!this->AllowTouch()) {
		return;
	}
	m_workTouchHandler->HandleTouchEvent(p, n, CommonTouchHandler::TOUCHDOWN);
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

void View::OnDraw() {
	MutexLock locked(this->m_mutex);

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
	if (m_RefreshFun && m_RefreshCallBackClassInstance) {
		m_RefreshFun(m_RefreshCallBackClassInstance);
	}
}

/**
 * 开始渐进显示
 */
void View::RequestDelayDraw() {
	m_SceneManager->GetRenderManager()->SetUseLowQuality(false);
}

M3D_STATUS View::ReadFile(string & path) {
	memoryLeakDetector();

	MutexLock locked(this->m_mutex);

	if (this->m_readListener) {
		this->m_readListener->SetReadFileCount(1);
		this->m_readListener->SetReadFileIndex(0);

		this->m_readListener->OnProcessPercent(NULL, 0);
	}

	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;

	try {
		SceneManager *scene = this->GetSceneManager();

		CloseFile();

		LOGI("View::ReadFile input path:%s ", path.c_str());

		m_CurFilePath = path;
		m_CurFilePath = M3D::FileHelper::GetUnionStylePath(m_CurFilePath);

		LOGI("View::ReadFile path:%s ", m_CurFilePath.c_str());

		LOGI("Reader::GetReader start");
		m_Reader = Reader::GetReader(m_CurFilePath);

		if (this->m_readListener) {
			m_Reader->SetListener(this->m_readListener);
		}

		LOGI("Reader::GetReader end");

//    m_Reader->SetFontPath(fontPath);
		m_Reader->SetIDCreator(this->m_IDCreator);

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
		LOGI("read animationXML %s sfs", this->m_AnimationXMLData.c_str());
//	LOGI("SetModel start");
		m_SceneManager->SetModel(m_Model);
//	LOGI("View::ReadFile m_SceneManager->SetModel ok.");
		this->SetDefaultWorkNodes();

		m_SceneManager->OptimizeScene();

		if (this->m_readListener) {
			this->m_readListener->SetPercent(1.0f);
		}

		this->m_isSpeedModel = Parameters::Instance()->m_simplityMode;

		if (this->m_isSpeedModel) {
			m_Model->Release();
			m_Model = NULL;

			if (m_Reader != NULL) {
				delete m_Reader;
				m_Reader = NULL;
			}
		}

		this->RestoreView();
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
	//this->CloseFile();
//	dumpMemoryLeaks();
	readerState = M_Read_OK;
	return readerState;
}

M3D_STATUS View::ReadFiles(vector<string>& paths) {
	memoryLeakDetector();
//	dumpMemoryLeaks();
	if (this->m_readListener) {
		this->m_readListener->SetReadFileCount(paths.size());
		this->m_readListener->OnProcessPercent(NULL, 0);
	}

	//MutexLock locked(this->m_mutex);

	M3D_STATUS readerState = M3D_STATUS::M_Read_NO_DEFINE_Error;

	try {
		SceneManager *scene = this->GetSceneManager();

		CloseFile();

		if (Parameters::Instance()->m_simplityMode) {
			ReadFilesSpeedMode(paths);
		} else {
			ReadFilesNormalMode(paths);
		}

		this->SetDefaultWorkNodes();

		m_SceneManager->OptimizeScene();

		if (this->m_readListener) {
			this->m_readListener->SetPercent(1.0f);
		}

		this->RestoreView();
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
	//CloseFile();
//	dumpMemoryLeaks();
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
		m_Reader->SetIDCreator(this->m_IDCreator);

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
		LOGI("read animationXML %s sfs", this->m_AnimationXMLData.c_str());
		//	LOGI("SetModel start");
		m_SceneManager->AddModel(m_Model);
		//	LOGI("View::ReadFile m_SceneManager->SetModel ok.");
		//this->SetDefaultWorkNodes();
		this->RestoreView();
		if (Parameters::Instance()->m_simplityMode) {
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

void View::SaveFile(string &path) {

	if (m_Model == NULL) {
		LOGE("View::SaveFile Error: m_Model==NULL");
		return;
	}
	LOGI("View::SaveFile path:%s ", path.c_str());
	Writer *writer = Writer::GetWriter(m_CurFilePath);

	int ret = writer->SaveTo(this, path);
	if (ret != 0) {
		LOGE("View::SaveFile Error:  errorCode:%d", ret);
	}
	delete writer;
}

void View::CloseFile() {
	try {
		m_userDatas.clear();
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
		if (m_Reader != NULL) {
			delete m_Reader;
			m_Reader = NULL;
		}

		if (m_Model) {
			m_Model->Release();
		}
		m_Model = NULL;

		m_SceneManager->RemoveModel();

		if (m_modelManager) {
			m_modelManager->GetCommandHistoryManager()->Clear();
		}
		LOGE("View::CloseFile()11");
		//关闭文件时，清空资源管理器中的资源
		this->m_SceneManager->GetResourceManager()->Clear();
		//ResourceManager::GetInstance()->Clear();
	} catch (...) {
		LOGE("View::CloseFile()");
	}
	LOGE("View::CloseFile()");
}

Selector* View::GetSelector() {
	return this->m_Selector;
}

IDCreator* View::GetIDCreator() {
	if (this->m_IDCreator == NULL) {
		this->m_IDCreator = new IDCreator;
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
		Shape* shape = (Shape*) GetSelector()->GetAll().at(i);
		SceneNode *node = shape->GetSceneNode();
		//仅仅模型支持选择移动
		if (node != NULL && shape->GetType() == SHAPE_MODEL) {
			Vector4 nD(endVec - startVec, 0.0f);
			//对于向量的变化量，应该使用此算法
			//而不是直接用矩阵乘vector3，得到的值
			Vector3 mov_ = node->GetParent()->GetWorldTransform().Inverse()
					* nD;
			node->Translate(mov_, TS_PARENT);
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

	Shape* shape = GetSceneManager()->GetShape(id);
	if (type == 0) {
		M3D::Vector3 outStart, outEnd;
		if (shape != NULL && shape->GetType() == SHAPE_MODEL) {
			SceneNode *node = shape->GetSceneNode();
			if (node != NULL) {
				Vector4 nD(endVec - startVec, 0.0f);
				Vector3 mov_ = node->GetParent()->GetWorldTransform().Inverse()
						* nD;
				node->Translate(mov_, TS_PARENT);
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
		SceneNode *node = // GetSceneManager()->GetShapeRelativeNode(
				((Shape*) shapes[i])->GetSceneNode();
		//shapes[i]->GetRelevantID());
		if (node != NULL) {
			node->ResetMovement();
		}
	}
}

void View::ResetSelectedShapes(M3D::Shape* shape) {
	SceneNode *node = shape->GetSceneNode();

	if (node != NULL) {
		node->ResetMovement();
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

map<int, PMIData*> View::GetPMIMap() {
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
	MutexLock locked(this->m_mutex);

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

	this->InitCamera();
    //reset
    if (m_pSAManager) {
        CAnimationStepManager* pStepAni = m_pSAManager->GetAnimationStepManager();
        if (pStepAni && pStepAni->GetCurProcessManagerID() != -1) {
            pStepAni->SetCurProcessManagerID(-1);
        }
    }
}

void View::SetDefaultRotateCenter() {
	if (this->m_SceneManager != NULL) {
		this->m_SceneManager->RestoreRotationCenter();
	}
}

bool View::SetRotationCenter(float x, float y) {
	if (this->m_SceneManager != NULL) {
		bool state = this->m_SceneManager->SetRotationCenter(x, y);
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

//vector<IShape*> View::GetFramePickShape(const Vector2& leftTop, const Vector2& rightBottom, int shapeType,
//	int geoType, int framePickType)
//{
//	if (this->m_SceneManager != NULL)
//	{
//		return this->m_SceneManager->GetFramePickShape(leftTop,rightBottom, shapeType, geoType, framePickType);
//	}
//
//	vector<IShape*> ret;
//	return ret;
//}

IShape* View::GetPickShapeObj(float x, float y, int shapeType, int geoType) {
	IShape* pickShape = GetPickShape(x, y, shapeType, geoType);
	return pickShape;
}
void View::SetSelectType(int type) {
	if (m_SceneManager != NULL) {
		return m_SceneManager->SetSelectType(type);
	}
}

void View::SetClipPlane(int direction, float position, bool isShowClipPlane,
		bool isShowCutPlane) {
	M3D::SectionOperator::Show(this, direction, position, isShowClipPlane,
			isShowCutPlane);
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
		SceneNode* node = this->m_SceneManager->GetNode(
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
		m_pWorkNodeGroup->SetRotation(rot);
		m_pWorkNodeGroup->SetPosition(viewMatrix.Translation());
		//	LOGE("camera matrix %s",viewMatrix.ToString().c_str());
	}
}
void View::SetSceneScale(float scales[3]) {
	float scale = scales[0];

	if (this->m_SceneManager != NULL) {
		m_pWorkNodeGroup->SetZoom(scale);
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
	m_curDrawMode = drawMode;

	RenderEffect* renderType =
			m_SceneManager->GetRenderManager()->GetRenderEffect();
	if (!renderType) {
		return;
	}
	int currentType;
	RenderableTypeGroup& renderData = renderType->GetRenderableTypeFilter();
	switch (drawMode) {
	case 0: //Solid
		if (m_Model != NULL)
			m_Model->ResetAlpha();
		renderData.SetRenderTypes(RenderableTypeGroup::RENDERDATA_NORMALEFFCT);
		renderData.Close(RenderableType::RGT_EDGELINE);
		break;
//	case 1://Transparent
//		if (m_Model != NULL)
//			m_Model->SetAlpha(Parameters::Instance()->m_Alpha);
//		renderData.SetRenderTypes(RenderableTypeGroup::RENDERDATA_NORMALEFFCT);
//		break;
	case 2: //OnlyShowTrilateralEdge
		renderData.SetRenderTypes(RenderableTypeGroup::RENDERDATA_NORMALEFFCT);

		renderData.Close(RenderableType::RGT_SOLID);
		renderData.Close(RenderableType::RGT_TRANSPARENT);

		break;
	case 3: //WireFrame
		renderData.SetRenderTypes(RenderableTypeGroup::RENDERDATA_NORMALEFFCT);

		renderData.Close(RenderableType::RGT_SOLID);

		break;
	case 5: //
		renderData.SetRenderTypes(RenderableTypeGroup::RENDERDATA_NORMALEFFCT);

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
	if (Parameters::Instance()->m_IsShowTransparent == true) {
		if (m_Model != NULL)
			m_Model->SetAlpha(Parameters::Instance()->m_Alpha);
	} else if (Parameters::Instance()->m_IsShowTransparent == false) {
		if (m_Model != NULL)
			m_Model->ResetAlpha();
	}

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

	this->m_SceneManager->GetRenderManager()->RequestRedraw();
}
//设置PMI可见性
bool View::SetPMIVisible(int pmiID, bool visible) {
	//LOGI("View::SetPMIVisible cnt:%d pmiID:%d",this->GetModel()->m_AllPMIMap.size(),pmiID);
	bool ret = true;
	map<int, PMIData*>& pmis = this->GetModel()->GetPMIs();
	map<int, PMIData*>::iterator it = pmis.find(pmiID);
	if (it != pmis.end()) {
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
	LOGE("AddHandle begin");
	int ret = this->GetSceneManager()->AddHandle(x, y, type);
	LOGE("AddHandle end");
	return ret;
}

Vector3 View::GetSelectedPoint(float x, float y, int type) {
	M3D::Vector3 vec = this->GetSceneManager()->GetPickPoint(x, y);

	return vec;
}

int View::AddHandle(float x, float y, float z, int type) {
	return this->GetSceneManager()->AddShape(x, y, z, type);
}

void View::RemoveHandle(int id) {
//	LOGE("View::RemoveHandle");
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
	LOGE("View::RemoveShape");
	this->GetSceneManager()->RemoveShape(id);
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

//	LOGI("SceneManager::showModelView viewId:%d", viewId);
	Model* curRootModel = this->m_SceneManager->GetModel();
	//根据id获得当前视图
	ModelView *pView = curRootModel->GetModleView(viewId);
	if (NULL == pView) {
		LOGE("modleViewId:%d not found!", viewId);
		this->m_SceneManager->UnLock();
		return;
	}
    SectionOperator::Clear(this);
    SectionOperator::Show(this, pView->GetSectionPlaneDirection(), pView->GetSectionPlanePercentage(), pView->GetShowSectionCappingPlane(), pView->GetShowCutSectionPlane());
    this->GetSceneManager()->GetSection()->SetIsShowCappingPlane(pView->GetShowSectionCappingPlane());
    Parameters::Instance()->m_showSection = pView->GetShowCutSectionPlane();
    if (pView->GetSectionPlanePercentage() == 0 || pView->GetSectionPlanePercentage() == -1) {
        this->GetSceneManager()->GetSection()->SetIsShowCappingPlane(false);
        Parameters::Instance()->m_showSection = false;
    }
    
    this->GetExplosiveView()->SetPercentWithoutRestore(this,-1,0,false);
    int direction = pView->getExplosiveType();
    float percent = pView->getExplosivePercent();
    if (direction != -1) {
        
        this->GetExplosiveView()->SetPercentWithoutRestore(this,direction,percent,false);
    }
//	LOGI(
// "SceneManager::showModelView allViewCount:%d", curRootModel->GetModelViewList().size());

	//如果需要更新摄像机
	if (pView->GetUpDataCameraState()) {
		//获得视图中的相机参数
		const CameraNode &camera = pView->GetCamera();

		//先把摄像机复位下，然后仅设置摄像机位置和旋转和缩放信息
		//	GetSceneManager()->ResetCamera();
		//this->ReSetCamera();

		CameraNode *currentCamera = GetSceneManager()->GetCamera();

		Vector3 cameraNewPos = camera.GetPosition();

		BoundingBox& sceneBox = this->m_SceneManager->GetSceneBox();

		int screenHeight = Parameters::Instance()->m_screenWidth; // = width;
		int screenWidth = Parameters::Instance()->m_screenHeight; // = height;

		float length = sceneBox.Length();

		float width = 100;
		float height = 100;
		if (screenHeight <= screenWidth) {
			width = sceneBox.Length();
			height = width * screenHeight / screenWidth;
		} else {
			height = sceneBox.Length();
			width = height * screenWidth / screenHeight;
		}
		//currentCamera->SetViewPort(0, 0, screenWidth, screenHeight);
		currentCamera->SetOrthoSize(Vector2(width * 2, height * 2));

		currentCamera->SetPosition(cameraNewPos);
		currentCamera->SetRotation(camera.GetRotation());
		currentCamera->SetZoom(camera.GetZoom());
		currentCamera->SetOrthographic(camera.IsOrthographic());

		//如果是平行投影，进行显示矫正
		if (camera.IsOrthographic()) {
			Vector3 cameraPos = currentCamera->GetPosition();
			//如果摄像机在模型内部，则进行调整
			if (sceneBox.IsInside(cameraPos) == INSIDE) {
				Matrix3 viewMat = currentCamera->GetView().ToMatrix3();
				Vector3 direction(viewMat.m_m20, viewMat.m_m21, viewMat.m_m22);
				direction.Normalize();
				Vector3 newPos = cameraPos + direction * sceneBox.Length();
				currentCamera->SetWorldPosition(newPos);
			}
		}
	}

	//TODO:相机设置旋转动画

	//insAtt
	const map<int, InstanceAttribute>& insAttMap =
			pView->GetInstanceAttributeMap();
	for (map<int, InstanceAttribute>::const_iterator it = insAttMap.begin();
			it != insAttMap.end(); it++) {
		const InstanceAttribute &curInsAtt = it->second;

		Shape* shape = this->GetShapeBySVLPath(curInsAtt.path);
		if (shape && shape->GetType() == SHAPE_MODEL) {
			Model *curModel = (Model*) shape;
			curModel->SetVisible(curInsAtt.visible);
			if (curInsAtt.hasColor) {
				Color tmpColor = curInsAtt.insColor;
				curModel->SetColor(tmpColor);
			}
			curModel->GetSceneNode()->SetLocalTransform(curInsAtt.placeMatrix);
		}
	}
	//设置视图中的剖面为启用

	this->m_SceneManager->GetSection()->ClearPlanes();

	vector<int> clipPlaneIdList = pView->GetSectionPlaneIDList();

	if (clipPlaneIdList.size() > 0) {
//		LOGE("SceneManager::showModelView clipPlaneIdList 2" );
		for (int i = 0; i < curRootModel->GetSectionPlaneList().size(); i++) {
			SectionPlane* curPlane = curRootModel->GetSectionPlaneList().at(i);
			for (int j = 0; j < clipPlaneIdList.size(); j++) {
				if (clipPlaneIdList.at(j) == curPlane->GetID()) {
//					LOGE("SceneManager::showModelView clipPlaneIdList 3" );
					this->m_SceneManager->GetSection()->AddPlane(curPlane);
//					LOGE("SceneManager::showModelView clipPlaneIdList 4" );
//					LOGI("SceneManager::showModelView enable plane:%d param:%f,%f,%f,%f", curPlane->GetID(), *curPlane->GetEquation(), *(curPlane->GetEquation()+1), *(curPlane->GetEquation()+2), *(curPlane->GetEquation()+3));
					break;
				}
			}
		}
	}
    
	
//    爆炸数据的设置

	//	vector<Shape*>& shapeList = curRootModel->GetShapeList(); ///ShapeList
//	//设置视图中的Note为启用
//	if (shapeList.size() > 0)
//	{
//		//LOGI("allshapeCnt:%d", shapeList.size());
//	//	LOGI("viewNoteCnt:%d", pView->GetNoteList().size());
//
//		for (int i = 0; i < shapeList.size(); i++)
//		{
//			Shape *pShape = shapeList.at(i);
//			if (pShape->GetType() == SHAPE_NOTE)
//			{
//				Note *note = (Note*) shapeList.at(i);
//				bool found = false;
//
//			//	LOGI("note id:%d", note->GetID());
//				for (int j = 0; j < pView->GetNoteList().size(); j++)
//				{
//					if (note->GetID() == pView->GetNoteList().at(j))
//					{
//						found = true;
//						break;
//					}
//				}
//				note->SetVisible(found);
//			//	LOGI("noteVisable:%d", found);
//			}
//		}
//	}

	//设置与视图关联的PMI
	string PMIName("");
	int displayPMIId;

	map<int, PMIData*>& pmis = curRootModel->GetPMIs();
	vector<ModelView*> modelViewList = curRootModel->GetModelViewList();
	//如果视图名为“DEFAULT”则显示所有PMI
	if (pView->GetViewType() == 0) {
		for (map<int, PMIData*>::iterator itPMI = pmis.begin();
				itPMI != pmis.end(); itPMI++) {
			(*itPMI).second->SetVisible(false);
		}

	} else { //否则只显示当前视图关联的PMI
		//首先所有PMI设为隐藏
		for (map<int, PMIData*>::iterator itPMI = pmis.begin();
				itPMI != pmis.end(); itPMI++) {
			(*itPMI).second->SetVisible(false);
		}

		//显示关联的PMI
		for (int i = 0; i < pView->GetPMIList().size(); i++) {
			displayPMIId = pView->GetPMIList().at(i);
			//循环PMI列表，将指定Id的PMI设置为可见
			map<int, PMIData*>::iterator it2 = pmis.find(displayPMIId);
			if (it2 != pmis.end()) {
				PMIData* curTmpPMI = (*it2).second;
				if (curTmpPMI != NULL) {
					curTmpPMI->SetVisible(true);
				}
			}
		}
	}
//	if (pView->GetViewType() == 0 && pView->GetPMIList().size() == 0) {
//        for (map<int, PMIData*>::iterator itPMI = pmis.begin();
//             itPMI != pmis.end(); itPMI++) {
//            if ((*itPMI).second->IsVisible() == true) {
//                pView->AddPMIId((*itPMI).second->GetID());
//            }
//        }
//	} else { //否则只显示当前视图关联的PMI
//			 //首先所有PMI设为隐藏
//		for (map<int, PMIData*>::iterator itPMI = pmis.begin();
//				itPMI != pmis.end(); itPMI++) {
//			(*itPMI).second->SetVisible(false);
//		}
//	}
//    //显示视图关联的PMI
//    for (int i = 0; i < pView->GetPMIList().size(); i++) {
//        displayPMIId = pView->GetPMIList().at(i);
//        //循环PMI列表，将指定Id的PMI设置为可见
//        map<int, PMIData*>::iterator it2 = pmis.find(displayPMIId);
//        if (it2 != pmis.end()) {
//            PMIData* curTmpPMI = (*it2).second;
//            if (curTmpPMI != NULL) {
//                curTmpPMI->SetVisible(true);
//            }
//        }
//    }
//	//允许动画
//	if (isAnni)
//	{
//		ViewStateOperateAction::CreateAnnitation(pView, this->m_SceneManager,
//				10);
//	}

	this->m_SceneManager->UnLock();

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

	ModelView* curView = xmlSerializer.LoadModelViewFromXml(xmlString);
	return curView;
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
	} else {
		LOGI("nativeLoadAllViewFromXML :未找到xml:%s", xmlPath.c_str());
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
	vector<ModelView*> viewList = GetModel()->GetModelViewList(); //GetAllViewList();
	LOGI("xmlSerializer.CreateViews viewCount:%d", viewList.size());
//	if(!Parameters::Instance()->m_isModelView)
//	{
	for (int i = 0; i < viewList.size(); i++) {
		LOGI(
				"xmlSerializer.CreateViews type : %d", viewList.at(i)->GetViewType());
	}
//	}//开启保存上次视图
	xmlSerializer.CreateViews(&viewList);
	LOGI("SaveModelView end.");

	xmlSerializer.Save(xmlPath);
	return ret;
}
ModelView*
View::GetDefaultModelView() {
	LOGI(" View::GetDefaultModelView() ");
	ModelView* defaultView = NULL;
	if (true || m_enableCreateDefaultView) {
        if (GetModel() != NULL) {
            vector<ModelView*> allViews = GetModel()->GetModelViewList();
            for (vector<ModelView*>::iterator it = allViews.begin();
                 it != allViews.end(); it++) {
                if ((*it)->GetViewType() == ModelView::DefaultView) {
                    defaultView = *it;
                    LOGI("DefaultModelView found!");
                    break;
                }
            }
            if (defaultView == NULL) {
                defaultView = new ModelView();
                defaultView->SetViewType(ModelView::DefaultView);
                defaultView->SetName("DefaultView");
                GetModel()->AddModelView(defaultView);
            }
        }
	}
	LOGI(" View::GetDefaultModelView() end. viewAddr:%d", defaultView);
	return defaultView;
}

bool View::UpdateViewByCurrentScene(ModelView* newView) {
	//LOGI("View::UpdateViewByCurrentScene begin %d",newView);
	bool ret = false;
	if (!newView) {
		return ret;
	}

	//camera
	newView->SetCamera(*GetSceneManager()->GetCamera());
	newView->SetUpDataCamera(true);

	int direction = GetExplosiveView()->GetExplosiveStyle();
	float percent = GetExplosiveView()->GetExplosivePercent();
	newView->setExplosiveType(direction);
	newView->setExplosivePercent(percent*50);

	//LOGI("UpVector:%s",outStr);
//       LOGI("ZoomFactor:%f",GetSceneManager()->GetCamera()->GetZoom());
//        LOGI("Orthographic:%d",GetSceneManager()->GetCamera()->IsOrthographic());
//        LOGI("NearClip:%f",GetSceneManager()->GetCamera()->GetNearClip());
//        LOGI("FarClip:%f",GetSceneManager()->GetCamera()->GetFarClip());
//        LOGI("FOV:%f",GetSceneManager()->GetCamera()->GetFov());
	//insAtt
	map<int, InstanceAttribute> insAttMap;

	vector<Model*> allSubModels;
	Model* topModel = this->GetSceneManager()->GetModel();
	allSubModels.push_back(topModel);
	topModel->GetAllSubModels(allSubModels);

	for (int i = 0; i < allSubModels.size(); i++) {
		Model *curModel = allSubModels[i];

		InstanceAttribute ia;
		ia.id = curModel->GetInstatnceID();
		ia.materialId = -1;
		ia.visible = curModel->IsVisible();
		//如果存在于颜色修改列表中则保存
		map<string, Model*>::iterator tmpIt = m_ColorChangedModelMap.find(
				curModel->GetPlcPath());
		if (tmpIt != m_ColorChangedModelMap.end()) {
			ia.hasColor = true;
			ia.insColor = curModel->GetColor();
		} else {
			ia.hasColor = false;
			ia.insColor = Color::GRAY;
		}
		ia.path = PathHelper::GetSVLPath(curModel);
		ia.placeMatrix =
				curModel->GetSceneNode()->GetLocalTransform().ToMatrix4();
		insAttMap.insert(pair<int, InstanceAttribute>(ia.id, ia));
	}

	newView->SetInstanceAttributeMap(insAttMap);

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
	/*--------------------------------------------------------------------------------*/
	NoteGroup* noteGroup = this->GetSceneManager()->GetNoteGroup();
	LOGI("UpdateViewByCurrentScene::SHAPE_NOTE");
	//vector<string> *noteDataList = newView->GetNoteDataList(SHAPE_TEXT_NOTE);//TODO 需不需要清空？
//	assert(noteDataList != NULL);
//	noteDataList->clear();
	//vector<string> noteDataList;
	//noteDataList.clear();

	newView->ClearNoteDataList();

	if (noteGroup->Size() > 0) {
		LOGI("begin update note by current scene");
		int noteCount = noteGroup->Size();
		LOGI("noteCount %d", noteCount);

		for (int i = 0; i < noteCount; i++) {
			SceneNode* node = noteGroup->GetChild(i);
			if (node && node->GetType() == SHAPE_NODE) {
				ShapeNode* shapeNode = (ShapeNode*) node;
				Shape* shape = shapeNode->GetShape();

				if (shape->GetType() == SHAPE_TEXT_NOTE) {

					TextNote* pNote = (TextNote *) shape;

					//在此处填充View中使用的TextNote ID。//TODO
					newView->AddNoteId(pNote->GetID());

					string noteData = NoteFactory::TextNoteToXMLElement(
							this->GetSceneManager(), pNote);
					newView->GetNoteDataList(SHAPE_TEXT_NOTE)->push_back(
							noteData);

				} else if (shape->GetType() == SHAPE_VOICE_NOTE) {
					LOGI("UpdateViewByCurrentScene::SHAPE_VOICE_NOTE");
					VoiceNote* pNote = (VoiceNote *) shape;

					//在此处填充View中使用的TextNote ID。//TODO
					newView->AddNoteId(pNote->GetID());

					string noteData = NoteFactory::VoiceNoteToXMLElement(
							this->GetSceneManager(), pNote);
					newView->GetNoteDataList(SHAPE_VOICE_NOTE)->push_back(
							noteData);
				} else if (shape->GetType() == SHAPE_SEQUENCE_NUMBER_NOTE) {
					LOGI(
							"UpdateViewByCurrentScene::SHAPE_SEQUENCE_NUMBER_NOTE");
					SequenceNumberNote* pNote = (SequenceNumberNote *) shape;

					//在此处填充View中使用的TextNote ID。//TODO
					newView->AddNoteId(pNote->GetID());

					string noteData = NoteFactory::SequenceNoteToXMLElement(
							this->GetSceneManager(), pNote);
					newView->GetNoteDataList(SHAPE_SEQUENCE_NUMBER_NOTE)->push_back(
							noteData);
				} else if (shape->GetType() == SHAPE_THREED_GESTURE_NOTE) {
					LOGI("UpdateViewByCurrentScene::SHAPE_THREED_GESTURE_NOTE");
					ThreeDGesturesNote* pNote = (ThreeDGesturesNote *) shape;

					//在此处填充View中使用的TextNote ID。//TODO
					newView->AddNoteId(pNote->GetID());
					LOGI("AddNoteId : %d", pNote->GetID());

					string noteData =
							NoteFactory::ThreeDGestureNoteToXMLElement(
									this->GetSceneManager(), pNote);
					newView->GetNoteDataList(SHAPE_THREED_GESTURE_NOTE)->push_back(
							noteData);
				}
			}
		}

	}

	LOGI(
			"voice note number = %d", newView->GetNoteDataList(SHAPE_VOICE_NOTE)->size());

	LOGI("end update textnote by current scene");

	if (SectionOperator::Instance != NULL) {
		///sectionPlane
		int direction = SectionOperator::Instance->m_Direction;
		float percentage = 100 * SectionOperator::Instance->m_fPercentage; //
		bool showCutPlane = SVIEW::Parameters::Instance()->m_showSection;
		bool isCappingPlane =
				this->GetSceneManager()->GetSection()->IsShowCappingPlane();

		newView->SetSectionPlaneDirection(direction);
		newView->SetSectionPlanePercentage(percentage);
		newView->SetShowCutSectionPlane(showCutPlane);
		newView->SetShowSectionCappingPlane(isCappingPlane);
	}

	/*-----------------------------------------------------------------------------*/

	//LOGI("Set note id end");
//	delete action;
	//LOGI("View::UpdateViewByCurrentScene end.");
	ret = true;
	RET: return ret;

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

	m_pWorkNodeGroup->AddNode(M3D::MAINCAMERA,
			this->m_SceneManager->GetCamera());
	m_pWorkNodeGroup->AddNode(M3D::MAINMODELROOT,
			this->m_SceneManager->GetSceneRoot()->Search(M3D::MAINMODELROOT));
	m_pWorkNodeGroup->AddNode(M3D::AXIS,
			this->m_SceneManager->GetSceneRoot()->Search(M3D::AXIS));
	m_pWorkNodeGroup->AddNode(M3D::FPS_FLAG,
			this->m_SceneManager->GetSceneRoot()->Search(M3D::FPS_FLAG));
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
		LOGE("View::HANDLER_WALKTHROUGH");
		LOGI("m_walkthroughTouchHandler BEGIN");
		m_workTouchHandler->Close();
		m_workTouchHandler = this->m_walkthroughTouchHandler;
		//this->m_walkthroughTouchHandler->StartRotateOnFixedPoint();//TODO
		m_workTouchHandler->Open();
		LOGI("m_walkthroughTouchHandler END");
	} else if (m_useWalkThrough == HANDLER_COMMON) {
		LOGE("View::HANDLER_COMMON");
		//	m_workTouchHandler->Close();
		//m_workTouchHandler = this->m_walkthroughTouchHandler;
		// this->m_walkthroughTouchHandler->EndRotateOnFixedPoint();//TODO
		//	m_workTouchHandler->Open();

		m_workTouchHandler->Close();
		m_workTouchHandler = this->m_TouchEventHandler;
		m_workTouchHandler->Open();
	} else if (m_useWalkThrough == HANDLER_DRAGGER) {
		LOGE("View::HANDLER_DRAGGER");
		m_workTouchHandler->Close();

		m_workTouchHandler = this->m_draggerHandler;

		m_workTouchHandler->Open();
	}

	this->RequestDraw();
}

TouchHandlerType View::GetWalkThrough() {
	return m_useWalkThrough;
}

WorkNodes * View::GetWorkNodes(void) {
	return this->m_pWorkNodeGroup;
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

		m_pSAManager->SetView(this);
		m_bHasAni = true;
	}
	return m_bHasAni;
}

bool View::AnimationOpen(const string& file) {
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
		m_pSAManager->SetView(this);
		m_bHasAni = true;
	}

	m_isNeedChangeViewAnimation = true;

	return m_bHasAni;
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

PerspectiveOperator*  View::GetPerspectiveOperator()
{
	if (this->m_perpectiveOperator == NULL)
	{
		m_perpectiveOperator = new PerspectiveOperator();
	}

	return m_perpectiveOperator;
}

void View::FoucusView(BoundingBox& foucusBox, bool useAni)
{
	////根据包围盒的大小，构建聚焦的动画
	SceneManager* scene = this->GetSceneManager();
	CameraNode* camera = scene->GetCamera();

	Vector3 disPosition;
	Quaternion disRotation;
	Vector3 disScale;

	if (camera && camera->IsOrthographic()) //平行投影聚焦显示
	{
		Vector3 center = foucusBox.Center();
		IntVector2 scrPnt = camera->GetViewPort().WorldToScreenPoint(center);
		IntVector2 scrCenter = camera->GetViewPort().GetRect().Center();

		//从屏幕上的一点移动到另一点
		Vector3 m_cacheCurPointNear =/* camera->GetView()* */camera->GetViewPort().ScreenToWorldPoint(
			scrPnt.m_x, scrPnt.m_y, 0.5f);
		Vector3 m_cachePriPoint = /*camera->GetView()* */camera->GetViewPort().ScreenToWorldPoint(
			scrCenter.m_x, scrCenter.m_y, 0.5f);

		//设置位置到合理值
		Vector3 moveVector = m_cacheCurPointNear - m_cachePriPoint;
		Vector3 cameraPos = camera->GetPosition();
		cameraPos = cameraPos + moveVector;
		disPosition = cameraPos;
	
		//设置缩放比例到合理值
		int screenHeight = this->GetRenderManager()->GetWindowHeight();
		int screenWidth = this->GetRenderManager()->GetWindowWidth();
		this->GetRenderManager()->WindowSizeChanged(screenWidth, screenHeight);
		float defaultZoom = Parameters::Instance()->m_DefaultZoomFactor;
		defaultZoom = defaultZoom * screenHeight / screenWidth;
		if (screenHeight > screenWidth)
		{
			defaultZoom = defaultZoom *0.5*(screenWidth*1.0 / screenHeight);
		}
		BoundingBox& sceneBox = scene->GetSceneBox();
		float scale = 0.5f*defaultZoom*sceneBox.Length() / foucusBox.Length();
		disScale = Vector3(scale, scale, scale);
		disRotation = camera->GetRotation();
	}
	else if (camera && !camera->IsOrthographic())//处理透视投影
	{
		Vector3 center = foucusBox.Center();

		scene->SetRotationCenter(center);
		float fitDepth = camera->GetFitClip();

		IntVector2 scrCenter = camera->GetViewPort().GetRect().Center();

		///乘上camera->GetView()将点转换到摄像机eye坐标系，进行求两次的变化量,由于此处摄像机采用继承的方式实现，没有采用节点挂载的方式
		//此处的部分变换处理算法，待调整为节点挂载的方式后重构。。 TODO
		Vector3 m_cachePriPoint = camera->GetViewPort().ScreenToWorldPoint(scrCenter.m_x, scrCenter.m_y, fitDepth);
		Vector3 m_cacheCurPointNear = center;

		//设置位置到合理值
		Vector3 moveVector = m_cacheCurPointNear - m_cachePriPoint;

		Vector3 cameraPos = camera->GetPosition();
		cameraPos = cameraPos + moveVector;
		//设置缩放比例到合理值
		BoundingBox& sceneBox = scene->GetSceneBox();

		float moveFactor = foucusBox.Length()*1.5;

		//Vector3 
		moveVector = (center - cameraPos).Normalized()
			* moveFactor;
		float scale = camera->GetZoom();

		disScale = Vector3(scale, scale, scale);
		disRotation = camera->GetRotation();
		disPosition = center - moveVector;
	}

	if (useAni)
	{
		this->GetPerspectiveOperator()->ExecuteCommonCameraAnimation(this, disPosition, disRotation, disScale,
			true, true, true);
	}
	else
	{
		camera->SetRotation(disRotation);
		camera->SetWorldPosition(disPosition);
		camera->SetZoom(disScale.m_x);
	}
}

void View::AnimationInit()
{
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
		if (isUseCamera == false)
		{
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
	m_AnimationXMLData = xmlData;
}

string View::GetDataManagerVersion() {
	wstring verStr(SVL_DATAMANAGER_VERSION);
	string retStr = WStringToString(verStr);
	return retStr;
}

Shape* View::GetShapeBySVLPath(const string& path) {
	string m3dPath = PathHelper::SVLPathToM3D(path);
	return GetShapeByM3DPath(m3dPath);
}

Shape* View::GetShapeByM3DPath(const string& path) {
	Shape* shape = NULL;
	shape = this->GetSceneManager()->GetShape(path);
	return shape;
}

Shape* View::GetShape(int shapeId) {
	Shape* shape = NULL;
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

	if (backgroundNode != NULL) {
		state = backgroundNode->IsUseImage();
	}

	return state;
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
	if (GetPerspectiveOperator()->GetAnimationState())
	{
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
	Model* model = NULL;
	string realPath = M3D::FileHelper::GetUnionStylePath(path);
	LOGI("View::GetModel path:%s ", realPath.c_str());

	Reader* reader = Reader::GetReader(realPath);

	if (this->m_readListener) {
		reader->SetListener(this->m_readListener);
	}

	model = reader->GetModel();

	if (model) {
		singleModel = model;
		model->AddRef();
	}

	if (reader) {
		delete reader;
	}

	return true;
}

M3D_STATUS View::ReadFilesNormalMode(vector<string>& paths) {
	//构造顶级模型，用来挂载所有的子模型
	m_Model = new Model();
	m_Model->AddRef();

	m_Model->SetName("TopAssembly");

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
			m_Model->AddSubModel(subModel);
		}
	}

	m_SceneManager->SetModel(m_Model);

	return M3D_STATUS::M_SUCCESS;
}

M3D_STATUS View::ReadFilesSpeedMode(vector<string>& paths) {
	//构造顶级模型，用来挂载所有的子模型
	m_Model = new Model();
	m_Model->AddRef();
	m_Model->SetName("TopAssembly");

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
			this->m_SceneManager->AddModel(subModel);
			//不用保留使用完成就释放掉
			subModel->Release();
		}
	}

	//缓存动画文件
	m_SceneManager->SetModel(m_Model);

	return M3D_STATUS::M_SUCCESS;
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

		LOGE("Animation dis scale:%f", controlInfo.scaleFactor);

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

set<string> View::GetSourceFiles() {
	set<string> srcFiles;
	if (this->m_Reader) {
		return this->m_Reader->GetSourceFiles();
	}
	return srcFiles;
}
string View::GetShapeProperties(int shapeId) {
	string properties;
	Shape* shape = this->GetSceneManager()->GetShape(shapeId);
	if (shape) {
		//properties = shape->GetProperties(properties);
	}
	return properties;
}

bool View::SetShapePorperties(int shapeId, const string& properties) {
	bool state = false;
	Shape* shape = this->GetSceneManager()->GetShape(shapeId);
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
			Shape* shape = this->GetShapeBySVLPath(realPath);

			if (shape) {
				LOGE("shape id %d", shape->GetID());
				retJson["ID"] = shape->GetID();
			} else {
				retJson["ID"] = -1;
			}

		}

		if (shapeValue["GetPath"].asBool()) {

			Shape* shape = this->GetShape(shapeValue["ID"].asInt());
			if (shape) {
				SceneNode* sceneNode = shape->GetSceneNode();
				if (sceneNode) {
					string realPath = sceneNode->GetName();
					retJson["Path"] = string(realPath.begin() + 9,
							realPath.end());
				}
			}
		}

		if (shapeValue["GetClr"].asBool()) {
			Json::Value colorValue;

			Shape* shape = this->GetShape(shapeValue["ID"].asInt());
			if (shape) {
				Color& color = shape->GetColor();
				colorValue["ClrR"] = color.m_r;
				colorValue["ClrG"] = color.m_g;
				colorValue["ClrB"] = color.m_b;
				colorValue["ClrA"] = color.m_a;

				retJson["Clr"] = colorValue;
			}

		}

		if (shapeValue["GetTran"].asBool()) {
			Shape* shape = this->GetShape(shapeValue["ID"].asInt());
			if (shape) {
				SceneNode* sceneNode = shape->GetSceneNode();
				if (sceneNode) {
					Matrix3x4& woldMatrix = sceneNode->GetWorldTransform();

					Json::Value tranValue;

					Vector3 pos = woldMatrix.Translation();

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
		if (shapeValue["GetWorldPosition"].asBool()) {
			Shape* shape = this->GetShape(shapeValue["ID"].asInt());
			if (shape) {
				SceneNode* sceneNode = shape->GetSceneNode();
				if (sceneNode) {
					Vector3 pos = sceneNode->GetWorldBoundingBox().Center();

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

		Shape* shape;
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

			SceneNode* sceneNode = shape->GetSceneNode();
			if (sceneNode) {
				Matrix3x4 transform(pos, rotation, scale);

				sceneNode->SetWorldTransform(transform);
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
	map<int, PMIData*>& pmis = this->GetModel()->GetPMIs();
	map<int, PMIData*>::iterator it = pmis.begin();
	map<int, PMIData*>::iterator end = pmis.end();
	while (it != end) {
		PMIData* pmi = (*it).second;
		pmi->SetVisible(isVisible);
		it++;
	}
}

void View::UpDateAnimationLimit() {
	Trackball::DrawLimit = m_DrawLimit;
}

NS_SimulationAnimation::CSimulationAnimationManager* View::GetSimulationMgr() {
	return this->m_pSAManager;
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

bool View::SetExplosiveView(int stype, int pos, bool useAnimation) {
	this->GetExplosiveView()->SetPercent(this, stype, pos, useAnimation);
	return false;
}

bool View::SetExplosiveViewNoRestore(int stype, int pos, bool useAnimation) {
	this->GetExplosiveView()->SetPercentWithoutRestore(this, stype, pos, useAnimation);

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
	this->RestoreView();

	return false;
}

string& View::GetLogger(int classType) {
	return *(M3D::LoggerHelper::Instance()->Get(classType));
}

bool View::FitScaleView() {
	bool ret = false;
	ret = ViewOperator::FitView(this->GetSceneManager());
	return ret;
}

void View::SetShowAxis(bool show) {
	if (this->m_axisNode) {
		this->m_axisNode->SetHide(!show);
	}
}

bool View::IsShowAxis() {
	bool show = false;

	if (this->m_axisNode) {
		show = (!this->m_axisNode->IsHide());
	}

	return show;
}

void View::InitCamera() {
	this->m_workTouchHandler->Open();
}

//void View::ReSetCamera()
//{
//	this->m_workTouchHandler->RefreshModelViewCamera();
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
    
    
}
///namespace
