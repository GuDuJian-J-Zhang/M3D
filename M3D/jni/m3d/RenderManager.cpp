#ifdef WIN32
#include "m3d/UI/GUI.h"
#endif //WIN32
#include "m3d/RenderManager.h"

#include "m3d/SceneManager.h"
#include "m3d/ResourceManager.h"

#include "m3d/base/CTimer.h"

#include "m3d/scene/GroupNode.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/scene/BackgroundNode.h"
#include "m3d/scene/AxisNode.h"
#include "m3d/scene/FPSNode.h"
#include "m3d/scene/SectionNode.h"

#include "m3d/model/ModelShape.h"

#include "m3d/graphics/CameraNode.h"

#include "m3d/model/Model.h"
#include "m3d/model/Shape.h"
#include "m3d/model/LODData.h"

#include "m3d/renderer/gl10/GLDrawer.h"
#include "m3d/renderer/GLESHead.h"
#include "m3d/renderer/gl10/GLShapeDrawer.h"
#include "m3d/action/RenderAction.h"
#include "m3d/action/CallBackAction.h"
#include "m3d/graphics/Renderable.h"
#include "sview/views/Parameters.h"
#include "m3d/scene/Octree.h"
#include "m3d/renderer/gl20/GLShapeDrawer20.h"
#include "m3d/model/ExtendInfoManager.h"
#include "m3d/renderer/effect/EffectManager.h"
#include "m3d/utils/StringHelper.h"


using SVIEW::Parameters;

namespace M3D
{
long RenderManager::m_LastLogTime = 0;
long RenderManager::m_FrameCount = 0;

bool RenderManager::isShowTriLine = false;

M3D::RenderAction* RenderManager::GetRenderAction() const
{
	return m_renderAction;
}

void RenderManager::SetRenderAction(M3D::RenderAction* val)
{
	m_renderAction = val;
}

RenderManager::RenderManager(SceneManager* scene) :
		FRAME_BUFFER_COUNT(20)
{
	m_avergFPS = 20;
	this->m_sceneMgr = scene;
	this->SetRenderAction(new RenderAction(this));
	this->GetRenderAction()->SetScene(this->m_sceneMgr);
	m_allowDraw = false;
	RenderManager::isShowTriLine = false;
	this->m_renderEffect = NULL;
	this->SetGLVersion(2);

	this->SetMaxLimitFPS(Parameters::Instance()->m_maxFPS);
	m_allRedrawStart = true;
//	m_startTime = 0;

	m_useDelayDraw = false;
	m_effectManager = new EffectManager(this->m_renderAction);
	m_globalEffect = "";
	m_isForceNormalEffect = false;
	Reset();
}

RenderManager::~RenderManager(void)
{
	//LOGI(
	//		"---------------------------------Clear RenderManager--------------------------------------  ");
	if (GetRenderAction())
	{
		///LOGI(
		//		"---------------------------------Clear shader program--------------------------------------  ");
		delete GetRenderAction();
		SetRenderAction(NULL);
		//LOGI(
		//		"--------------------------------Clear shader program end----------------------------------------- ");
	}

	RenderEffectsMap::iterator it = this->m_renderEffectsMap.begin();
	while (it != this->m_renderEffectsMap.end())
	{
		delete it->second;
		it++;
	}

	//删除rendercontext
	delete m_GLContext;
	m_GLContext = NULL;

	if (m_effectManager)
	{
		delete m_effectManager;
		m_effectManager = NULL;
	}
}

void RenderManager::ClearDelayDrawList()
{
	if (this->m_renderAction)
	{
		this->m_renderAction->ClearDelayDraw();
	}
}

void RenderManager::Reset()
{
	this->m_lodLevel = -1;
	m_useLowQuality = false;
}

RenderEffect* RenderManager::GetRenderEffect()
{
	return this->m_renderEffect;
}

void RenderManager::SetRenderEffect(RenderEffect* type)
{
	this->m_renderEffect = type;
}

void RenderManager::CalculateFPS()
{
	long currentTime = CTimer::Tick();
	m_evenFPS = 60;
	CTimer::GetTimeofDay(&m_endTime, NULL);

	if (this->m_allRedrawStart)
	{
		CTimer::GetTimeofDay(&m_endTime, NULL);
		m_fpsstartTime = m_endTime;
		m_evenFPS = 20;
		//默认指定帧率，这样就不会对剔除比例做更新了
		m_avergFPS = 20;
		this->m_allRedrawStart = false;
		currentTime = m_LastLogTime;
	}

	dt = (m_endTime.tv_sec - m_fpsstartTime.tv_sec) * 1000000
			+ (m_endTime.tv_usec - m_fpsstartTime.tv_usec);

	if (dt > 0)
	{
		m_evenFPS = 1000000.0f / dt;
	}

	CTimer::GetTimeofDay(&m_fpsstartTime, NULL);

	m_FrameCount++;
	if (m_LastLogTime == 0 || (currentTime - m_LastLogTime) > 1000)
	{
		m_avergFPS = m_FrameCount;

		GLShapeDrawer::drawBoxTime = 0;
		GLShapeDrawer::drawMeshTime = 0;
		GLShapeDrawer::drawPMITime = 0;
		m_LastLogTime = CTimer::Tick();
		m_FrameCount = 0;
	}
}

int RenderManager::GetFPS()
{
	return this->m_avergFPS;
}

void RenderManager::SetGLVersion(int glVersion)
{
	this->m_glVersion = glVersion;
}

int RenderManager::GetGLVersion()
{
	return this->m_glVersion;
}

int RenderManager::GetMaxLimitFPS()
{
	return this->m_maxFPS;
}

void RenderManager::SetMaxLimitFPS(int num)
{
	this->m_maxFPS = num;
}

void RenderManager::InitRenderEffect()
{
	RenderEffect* normalEffect = new RenderEffect(
			RenderEffect::RENDER_NORMALEFFCT);
	normalEffect->GetRenderQueuePriority().InitialNormalEffect();
	//RenderOrder* normalRender = new RenderOrder();
	//normalRender->InitialNormalEffect();
	this->m_renderEffectsMap.insert(
			pair<int, RenderEffect*>(RenderEffect::RENDER_NORMALEFFCT,
					normalEffect));
	//RenderOrder* opacityLineRender = new RenderOrder();
	//opacityLineRender->InitialOpacityLineEffect();
	//this->renderTypesMap.insert(pair<int,RenderOrder*>(RENDERTYPE::RENDER_OPACITYEDGE,opacityLineRender));
	// 
	this->m_renderEffect = normalEffect;
}

bool RenderManager::LimitDrawFPS()
{
	bool ret = false;
#ifdef __MOBILE__
	gettimeofday(&m_endTime, NULL);
	dt = (m_endTime.tv_sec - m_startTime.tv_sec) * 1000000
			+ (m_endTime.tv_usec - m_startTime.tv_usec);
	if (dt < 1000000 / m_maxFPS && dt > 0)
	{

		usleep(1000000 / m_maxFPS - dt);

	}
	gettimeofday(&m_startTime, NULL);
#endif
	return ret;
}

void RenderManager::ResizeHubViewport()
{
	CameraNode* camera = this->m_sceneMgr->GetHudCamera();

	if (!camera)
	{
		camera = new CameraNode();
		this->m_sceneMgr->SetHudCamera(camera);
		camera->SetOrthographic(true);

		BoundingBox uiLayoutBox = this->m_sceneMgr->GetOcTreeWorldBoundingBox();
		float length = this->m_sceneMgr->GetOcTreeWorldBoundingBox().Length();

		Vector3 center = uiLayoutBox.Center();

		center.m_z += uiLayoutBox.Length() * CAMERA_POSFACTOR;

		camera->SetWorldPosition(center);
		length = this->m_sceneMgr->GetDefaultFocusLength();

		camera->SetNearClip(0.1);
		camera->SetFarClip(100);

		camera->SetFov(90);
		camera->LookAt(uiLayoutBox.Center(), Vector3(0, 1, 0), TS_WORLD);
	}

	int screenHeight = this->GetWindowHeight();
	int screenWidth = this->GetWindowWidth();

	Vector3 minPnt(0,0,-10);
	Vector3 maxPnt(screenWidth,screenHeight,10);

	BoundingBox box(minPnt,maxPnt);
	this->m_sceneMgr->SetHudLayerBox(box);

	camera->SetViewPort(0, 0, screenWidth, screenHeight);
	camera->SetOrthoSize(Vector2(screenWidth, screenHeight));
}

float RenderManager::GetEventFPS()
{
	return this->m_evenFPS;
}

void RenderManager::RebindUIRenderFBO()
{
	RenderAction* workRenderAction = this->m_renderAction;
	if (workRenderAction != NULL)
	{
		workRenderAction->m_delayDrawFBO.KeepOriginalFBO();
	}
}


void RenderManager::Render()
{
	GroupNode * groupNode = (GroupNode *) m_sceneMgr->GetSceneRoot();
	RenderAction* workRenderAction = this->m_renderAction;
	if (workRenderAction != NULL)
	{
		workRenderAction->SetGLContext(m_GLContext);
		workRenderAction->SetCamera(m_sceneMgr->GetCamera());

		if (m_useDelayDraw)
		{
			if (workRenderAction->ProcessDelayDraw())
			{
				//进入渐进显示了，就不用再向下走绘制流程了。
				this->m_allRedrawStart = true;
				workRenderAction->EndDelayDraw();
				return;
			}
		}


//只有手机才走手动刷新逻辑
		if (!IsNeedRedraw())
		{
			this->m_allRedrawStart = true;
			if (m_useDelayDraw)
			{
				workRenderAction->EndDelayDraw();
			}
			return;
		}
 
		CalculateFPS();

		//if (m_useDelayDraw)
		{
			//workRenderAction->Optimize();
			workRenderAction->BeginDelayDraw();
		}

		workRenderAction->SetFPS(this->GetFPS());

		workRenderAction->SetRenderEffect(this->m_renderEffect);

		workRenderAction->Begin();
 
		for (int i = 0; i < groupNode->Size(); i++)
		{
			SceneNode* node = groupNode->GetChild(i);
			if (node->GetName() != MAINGROUP)//不处理挂载模型的节点
			{
				node->FindVisiableObject(workRenderAction);
			}
		}

		this->FindVisiableObject(workRenderAction);

		workRenderAction->Execute();

		//if (m_useDelayDraw)
		{
			workRenderAction->EndDelayDraw();
		}

		workRenderAction->End();
	}
}

void RenderManager::FindVisiableObject(RenderAction* renderAction)
{
	if (this->m_sceneMgr->m_ocTree)
	{
		CameraNode* camera = renderAction->GetCamera();
		const Frustum& cameraFrustum = camera->GetFrustum();
		FrustumOctreeQuery query(this->m_sceneMgr->m_frustumQueryResulets, cameraFrustum, 0, 0);
		query.SetRenderAction(renderAction);
		query.SetCamera(camera);

		this->m_sceneMgr->m_ocTree->GetDrawables(query);

		bool showboxFlag = Parameters::Instance()->m_IsShowBox;
		RenderEffect* renderType = renderAction->GetRenderEffect();
		RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();

		//将面显示数据，加入显示队列中
		if (dataType.Contain(RenderableType::RGT_BOX))
		{
			showboxFlag = true;
		}

		//立即显示的部分
		for (int i = 0; i < this->m_sceneMgr->m_frustumQueryResulets.size(); ++i)
		{
			ModelShape* modelShape = this->m_sceneMgr->m_frustumQueryResulets[i];
			if (modelShape->AllowCuller())
			{
				//微小模型剔除
				int littleModelState = renderAction->GetCullerHelper().IsLittleModel(modelShape->GetWorldBoundingBox(), camera);
				////非常小的模型
				if (littleModelState == 2)
				{

				} //小件剔除的模型
				else //其他
				{
					modelShape->FindVisiableObjectFast(renderAction, littleModelState);
				}

				if (showboxFlag && modelShape->GetModel()->IsSelected() && modelShape->GetModel()->IsVisible())
				{
					renderAction->PrepareRenderBox(modelShape);
				}
			}
			else {
				modelShape->FindVisiableObject(renderAction);
			}
			
		}
		//显示PMI
		this->m_sceneMgr->GetExtendInfoManager()->FindVisiableObject(renderAction);
#ifdef WIN32
		this->m_sceneMgr->GetGUI()->Render(renderAction);
#endif //WIN32
	}
}


bool RenderManager::IsExcludeSmallModel()
{
	return m_excludeSmallModel;
}

void RenderManager::ExcludeSmallModel(bool flag)
{
	this->m_excludeSmallModel = flag;
}

void RenderManager::CloseRenderActionThread()
{
//	MutexLock lock(m_mutex);
//	m_allowDraw = allowDraw;
//	if(m_allowDraw)
//	{
//		this->m_renderActionMgr->Run();
//	}
//	else
//	{
//	  LOGE("RenderManager::AllowDraw stop");
//	  this->m_renderActionMgr->KeepFlag(false);
//	  this->m_renderActionMgr->Stop();
//	}
}

void RenderManager::ShowRotationCenter(bool flag)
{
	this->m_showRotateCenter = flag;
}

bool RenderManager::IsShowRotationCenter()
{
	return this->m_showRotateCenter;
}

int RenderManager::GetLodLevel()
{
	return this->m_lodLevel;
}
void RenderManager::SetLodLevel(int level)
{
	this->m_lodLevel = level;
}

void RenderManager::SetUseLowQuality(bool useLowQuality)
{
	m_useLowQuality = useLowQuality;

	CameraNode* camera = this->m_sceneMgr->GetCamera();

	if (m_useLowQuality == true)
	{
		SetLodLevel(LOD::LODMIN);

		this->m_renderAction->ClearDelayDraw();
	}
	else
	{
		SetLodLevel(LOD::LODMAX);
	}
}

CullerHelper& RenderManager::GetCullerHelper()
{
	return this->m_cullerHelper;
}

void RenderManager::InitialRender()
{
	int GLVersion = this->GetGLVersion();
	///初始化渲染上下文
	if (GLVersion == 1)
	{
		m_GLContext = RenderContext::GetContext(1);
		LOGI("GLVersion == 1");

	}
	else if (GLVersion == 2)
	{
		m_GLContext = RenderContext::GetContext(2);
		LOGI("GLVersion == 2");
	}

	InitRenderEffect();
}

void RenderManager::OnDraw()
{
	//如果宽度或者高度为0，则不进行渲染
	if (m_iScreenWidth == 0 || m_iScreenHeigh == 0)
	{
		return ;
	}


	//TODO　帧率限制暂时屏蔽掉
	if (true || !Parameters::Instance()->m_IsHighPerformanceView)
	{
		LimitDrawFPS();
	}

	///线程资源锁定
	this->m_sceneMgr->Lock();

	Trackball::KEEPINGSTATETIMES--;

	this->m_sceneMgr->UpdateScene();

	CameraNode* camera = this->m_sceneMgr->GetCamera();
	if (camera != NULL)
	{
		this->m_cullerHelper.AllowCuller(camera, m_useLowQuality);
	}

	if (this->m_GLContext)
	{
		this->m_GLContext->OptimizeContext();
	}

	this->Render();

	//	LOGE("RenderManager::OnDraw() end");
	this->m_sceneMgr->UnLock();
}

bool RenderManager::IsNeedRedraw()
{
	if (/*!m_AllowRedraw ||*/m_BufferCounter < 1)
	{
		return false;
	}
	m_BufferCounter--;
	return true;
}

void RenderManager::WindowSizeChanged(int width, int height)
{
	this->m_sceneMgr->Lock();
#ifdef WIN32
 
#else
 	Parameters::Instance()->m_screenWidth = width;
	Parameters::Instance()->m_screenHeight = height;
#endif

	bool sizeChanged = false;
	if (this->m_iScreenWidth != width || this->m_iScreenHeigh != height)
	{
		sizeChanged = true;
	}
 
	this->m_iScreenWidth = width;
	this->m_iScreenHeigh = height;
 
	if (sizeChanged)
	{
		this->RequestUpdateWhenSceneBoxChanged();
		SceneNode* rootNode = m_sceneMgr->GetSceneRoot();
		BackgroundNode* backgroundNode = (BackgroundNode *)rootNode->Search(
			BACKGROUNDCOLOR);
		if (backgroundNode != NULL)
		{
			backgroundNode->SetBackgroundSize(this->m_iScreenWidth,
				this->m_iScreenHeigh);
		}

		AxisNode* axisNode = (AxisNode *)rootNode->Search(AXIS);
		if (axisNode != NULL)
		{
			axisNode->SetViewSize(this->m_iScreenWidth, this->m_iScreenHeigh, 0.25);
		}	FPSNode* fpsNode = (FPSNode *)rootNode->Search(FPS_FLAG);
		if (fpsNode != NULL)
		{
			fpsNode->SetSceneHeight(this->m_iScreenHeigh);
		}

		if (m_renderAction)
		{
			m_renderAction->ResizeFBOs(width, height); //设置FBO大小
		}


		//更改Hud的适口
		this->ResizeHubViewport();
	}

	this->m_sceneMgr->UnLock();

	RequestRedraw();
}

void RenderManager::RequestUpdateWhenSceneBoxChanged()
{
	this->m_sceneMgr->Lock();
	CameraNode* camera = this->m_sceneMgr->GetCamera();
	int screenHeight = this->GetWindowHeight();
	int screenWidth = this->GetWindowWidth();
	camera->SetViewPort(0, 0, screenWidth, screenHeight);
	//当窗口大小变化时，需要重新设置viewPort 和 摄像机orth的大小
	if (camera)
	{
		BoundingBox& pBoundingBox = this->m_sceneMgr->GetSceneBox();

		bool boxHasValue = (pBoundingBox.Length() > 0);

		float width = m_iScreenWidth;
		float height = m_iScreenHeigh;
		float boxLength = width;
		if (boxHasValue && screenHeight <= screenWidth)
		{
			width = pBoundingBox.Length();
			height = width * screenHeight / screenWidth;
			boxLength = pBoundingBox.Length();
		}
		else if (boxHasValue)
		{
			height = pBoundingBox.Length();
			width = height * screenWidth / screenHeight;
			boxLength = pBoundingBox.Length();
		}

		this->GetCullerHelper().SetModelLength(boxLength);

		camera->SetOrthoSize(Vector2(width * 2, height * 2));
	}
	this->m_sceneMgr->UnLock();
}

void RenderManager::SceneChanged()
{

}

void RenderManager::VRPrepareDrawList()
{
		RenderAction* workRenderAction = this->m_renderAction;
		if (workRenderAction != NULL)
		{
			workRenderAction->SetGLContext(m_GLContext);
			workRenderAction->SetCamera(m_sceneMgr->GetCamera());
			workRenderAction->SetRenderEffect(this->m_renderEffect);
			workRenderAction->Begin();

			GroupNode * groupNode = (GroupNode *)m_sceneMgr->GetSceneRoot();
			for (int i = 0; i < groupNode->Size(); i++)
			{
				SceneNode* node = groupNode->GetChild(i);
				if (node->GetName() != MAINGROUP)//不处理挂载模型的节点
				{
					node->FindVisiableObject(workRenderAction);
				}
			}

#ifdef WIN32
			this->m_sceneMgr->GetGUI()->Render(workRenderAction);
#endif //WIN32

			this->FindVisiableObject(workRenderAction);
			workRenderAction->End();
		}
}

void RenderManager::VRDraw()
{
	RenderAction* workRenderAction = this->m_renderAction;
	if (workRenderAction != NULL)
	{
	//GLShapeDrawer::InitialGL();

	glClearColor(0.53, 0.807, .98, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//GLShapeDrawer20::DrawVRBackGround(workRenderAction);
	//GLShapeDrawer20::DrawBackGround(workRenderAction->GetBackGroundNode(),workRenderAction);
	GLShapeDrawer20::DrawRenderPassGroup(workRenderAction);
	//GLShapeDrawer20::DrawAxis(workRenderAction->GetAxisNode(), workRenderAction);
	//GLShapeDrawer20::DrawFPS(workRenderAction->GetFPSNode(), workRenderAction);
	}
}


SceneManager* RenderManager::GetSceneManager()
{
	return this->m_sceneMgr;
}

EffectManager * RenderManager::GetEffectManager()
{
	return m_effectManager;
}

string RenderManager::GetGlobalEffect()
{
	if(m_isForceNormalEffect){
		return "";
	}
	return this->m_globalEffect;
}

void RenderManager::SetGlobalEffect(string effectName)
{
	this->m_globalEffect = effectName;
}

void RenderManager::BeginCatchOpenGLError()
{
	glGetError();
}

string RenderManager::EndCatchOpenGLError()
{
	string ret = "";
	for (GLenum err; (err = glGetError()) != GL_NO_ERROR;)
	{
		switch (err)
		{
		case GL_INVALID_ENUM:
			ret += "Error code: "+StringHelper::IntToString(GL_INVALID_ENUM)+" GL_INVALID_ENUM  ";
			break;
		case GL_INVALID_VALUE:
			ret += "Error code: " + StringHelper::IntToString(GL_INVALID_VALUE) + " GL_INVALID_VALUE  ";
			break;
		case GL_INVALID_OPERATION:
			ret += "Error code: " + StringHelper::IntToString(GL_INVALID_OPERATION) + " GL_INVALID_OPERATION  ";
			break;
		case GL_STACK_OVERFLOW:
			ret += "Error code: " + StringHelper::IntToString(GL_STACK_OVERFLOW) + " GL_STACK_OVERFLOW  ";
			break;
		case GL_STACK_UNDERFLOW:
			ret += "Error code: " + StringHelper::IntToString(GL_STACK_UNDERFLOW) + " GL_STACK_UNDERFLOW  ";
			break;
		case GL_OUT_OF_MEMORY:
			ret += "Error code: " + StringHelper::IntToString(GL_OUT_OF_MEMORY) + " GL_OUT_OF_MEMORY  ";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			ret += "Error code: " + StringHelper::IntToString(GL_INVALID_FRAMEBUFFER_OPERATION) + " GL_INVALID_FRAMEBUFFER_OPERATION  ";
			break;
	 #ifdef _WIN32
		case GL_CONTEXT_LOST:
			ret += "Error code: " + StringHelper::IntToString(GL_CONTEXT_LOST) + " GL_CONTEXT_LOST  ";
			break;
			#else
			
	 #endif
		default:
			ret += "  Undefined Error ";
			break;
		}
	}

	return ret;
}

bool RenderManager::IsForceNormalEffect(){
	return this->m_isForceNormalEffect;
}
void RenderManager::SetISForceNormalEffect(bool isForce){
	this->m_isForceNormalEffect = isForce;
}

int RenderManager::GetWindowWidth()
{
	return this->m_iScreenWidth;
}

int RenderManager::GetWindowHeight()
{
	return this->m_iScreenHeigh;
}

void RenderManager::AllowRedraw(bool allow)
{
	this->m_AllowRedraw = allow;
}

void RenderManager::RequestRedraw()
{
	this->m_BufferCounter = FRAME_BUFFER_COUNT;
} 

void RenderManager::ClearGLResource()
{
	if (m_renderAction)
	{
		m_renderAction->ClearGLResource();
	}

	//高性能模式浏览时，使用渐进显示方式进行绘制
#ifdef _WIN32
	m_useDelayDraw = true;// SVIEW::Parameters::Instance()->m_simplityMode;
#else
	m_useDelayDraw = false;// SVIEW::Parameters::Instance()->m_simplityMode;
#endif
	m_renderAction->SetAllowDelayDraw(true);
	if(m_useDelayDraw)
	{
		FRAME_BUFFER_COUNT = 1;
	}else
	{
		FRAME_BUFFER_COUNT = 20;
	}
    this->SetGlobalEffect("");
}

bool RenderManager::UseLowQuality()
{
	return this->m_useLowQuality;
}

}

