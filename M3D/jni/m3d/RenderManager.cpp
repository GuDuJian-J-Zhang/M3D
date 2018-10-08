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

#include "m3d/scene/LShapeNode.h"

#include "m3d/graphics/CameraNode.h"

#include "m3d/model/Model.h"
#include "m3d/model/Shape.h"
#include "m3d/model/LODData.h"

#include "m3d/renderer/gl10/GLDrawer.h"
#include "m3d/renderer/GLESHead.h"
#include "m3d/renderer/gl10/GLShapeDrawer.h"
#include "m3d/action/RenderAction.h"
#include "m3d/action/CallbackAction.h"
#include "m3d/graphics/Renderable.h"
#include "sview/views/Parameters.h"
#include "m3d/renderer/gl20/GLShapeDrawer20.h"


using SVIEW::Parameters;

namespace M3D
{
long RenderManager::m_LastLogTime = 0;
long RenderManager::m_FrameCount = 0;

const int RenderActionManager::ITEMSCOUNT = 3;
/// Construct. Does not start the thread yet.
RenderActionManager::RenderActionManager(RenderManager* renderMgr)
{
	this->m_workIndex = 0;
	this->m_renderMgr = renderMgr;
	m_finish = true;

	m_RenderActions[0] = new RenderAction(renderMgr);
	m_RenderActions[1] = new RenderAction(renderMgr);
	m_RenderActions[2] = new RenderAction(renderMgr);

	m_pSceneManager = this->m_renderMgr->m_sceneMgr;
}

RenderActionManager::~RenderActionManager()
{

}

void RenderActionManager::ThreadFunction()
{
	while (m_finish)
	{
		//为了保证安全，Queue中存储Item的个数为m_RenderActions的个数减1，因为有一个Action正在准备中.
		//如果已有item个数超过了限制,则sleep一下，减少资源占用
		if (m_renderActionQueue.size() < ITEMSCOUNT - 1)
		{
			//锁定场景
			RenderAction* workAction = m_RenderActions[this->m_workIndex];

			workAction->SetGLContext(m_renderMgr->m_GLContext);
			workAction->SetCamera(m_pSceneManager->GetCamera());
			workAction->SetLights(m_pSceneManager->GetLights());

			workAction->SetFPS(m_renderMgr->GetFPS());

			workAction->SetRenderEffect(m_renderMgr->m_renderEffect);

			workAction->Begin();

			m_pSceneManager->GetSceneRoot()->FindVisiableObject(workAction);

			MutexLock lock(m_mutex);
			m_renderActionQueue.push(workAction);

			this->m_workIndex = ((this->m_workIndex++)) % ITEMSCOUNT;
		}
		else
		{
#ifdef __MOBILE__
			usleep(10);
#endif
		}
	}
}
void RenderActionManager::KeepFlag(bool finish)
{
	this->m_finish = finish;
}
RenderAction* RenderActionManager::GetAction()
{
	//如果没有使用独立线程去准备显示数据，则使用第一个Action数据。
	if (!this->IsStarted())
	{
		while (!m_renderActionQueue.empty())
		{
			m_renderActionQueue.pop();
		}

		this->m_workIndex = 0;

		this->KeepFlag(true);

		this->Run();
	}

	//等待队列中有数据
	while (m_renderActionQueue.empty())
	{
#ifdef __MOBILE__
		usleep(10);
#endif
	}

	RenderAction* workAction = NULL;

	if (m_renderActionQueue.size() > 0)
	{
		MutexLock lock(m_mutex);
		workAction = m_renderActionQueue.front();
		m_renderActionQueue.pop();
	}
	return workAction;
}

SceneDivide::SceneDivide(SceneManager* sceneMgr)
{
	this->MarkDirty();
	this->m_sceneMgr = sceneMgr;
}

void SceneDivide::AddNode(LShapeNode* node)
{
	this->m_shapeNodesCache.push_back(node);
}

void SceneDivide::Clear()
{
	this->m_shapeNodesCache.clear();
}

void SceneDivide::Optimize()
{
	if (this->Dirty())
	{
		this->m_dirty = false;
		this->Clear();
		CallBackAction* computeBoxAction = new CallBackAction();
		computeBoxAction->SetActionData(this);
		computeBoxAction->SetActionFun(NULL);
		computeBoxAction->SetActionLFun(SceneDivide::CacheNode);
		m_sceneMgr->ExecuteAction(computeBoxAction);
		delete computeBoxAction;

		//按照节点包围盒的大小，从大到小进行排序
		sort(this->m_shapeNodesCache.begin(), this->m_shapeNodesCache.end(),
			CompareByPlcWorldBoxLength);

		this->m_shapeNodesCache.shrink_to_fit();
	}
}

void SceneDivide::FindVisiableObject(RenderAction* renderAction)
{
	size_t sceneNodesLength = this->m_shapeNodesCache.size();

	float dividePercent = 1.0f;

	//立即显示的部分
	for (int i = 0; i < sceneNodesLength*dividePercent; ++i)
	{
		LShapeNode* shapeNode = this->m_shapeNodesCache[i];
		//微小模型剔除 
		int littleModelState = renderAction->GetCullerHelper().IsLittleModel(shapeNode->GetWorldBoundingBox(), renderAction->GetCamera());

		//非常小的模型
		if (littleModelState == 2)
		{
			return;
		} //小件剔除的模型
		else //其他
		{
			shapeNode->FindVisiableObjectFast(renderAction, littleModelState);
		}
	}

	//渐进显示的部分
	for (int j = sceneNodesLength*dividePercent; j < sceneNodesLength; ++j)
	{

	}
}

void SceneDivide::Traverse(Action* action)
{

}

void SceneDivide::RayPick(RayPickAction * action)
{

}

bool SceneDivide::Dirty()
{
	return this->m_dirty;
}

void SceneDivide::MarkDirty()
{
	this->m_dirty = true;
}

void SceneDivide::CacheNode(void* data, LSceneNode* node)
{
	SceneDivide* sceneDivide = (SceneDivide*)data;

	if (sceneDivide && node->GetType() == LSHAPE_NODE)
	{
		sceneDivide->AddNode((LShapeNode*)node);
	}
}

bool SceneDivide::CompareByPlcWorldBoxLength(LShapeNode* node1,
	LShapeNode* node2)
{
	return node1->GetWorldBoundingBox().Length() > node2->GetWorldBoundingBox().Length();
}


bool RenderManager::isShowTriLine = false;
RenderManager::RenderManager(SceneManager* scene) :
		FRAME_BUFFER_COUNT(400)
{
	m_avergFPS = 20;
	this->m_sceneMgr = scene;
	this->m_renderAction = new RenderAction(this);
	this->m_renderAction->SetScene(this->m_sceneMgr);
	m_allowDraw = false;
	RenderManager::isShowTriLine = false;
	this->m_renderEffect = NULL;
	this->SetGLVersion(2);

	this->SetMaxLimitFPS(Parameters::Instance()->m_maxFPS);
	m_allRedrawStart = true;
//	m_startTime = 0;
    m_useDelayDraw = false;
	m_sceneDivide = new SceneDivide(scene);

	Reset();
}

RenderManager::~RenderManager(void)
{
	LOGI(
			"---------------------------------Clear RenderManager--------------------------------------  ");
	if (m_renderAction)
	{
		LOGI(
				"---------------------------------Clear shader program--------------------------------------  ");
		delete m_renderAction;
		m_renderAction = NULL;
		LOGI(
				"--------------------------------Clear shader program end----------------------------------------- ");
	}

	RenderEffectsMap::iterator it = this->m_renderEffectsMap.begin();
	while (it != this->m_renderEffectsMap.end())
	{
		delete it->second;
		it++;
	}
	//场景划分管理释放
	if (m_sceneDivide)
	{
		delete m_sceneDivide;
		m_sceneDivide = NULL;
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
	long currentTime = GetTimeLong();
	m_evenFPS = 60;
#ifdef __MOBILE__
	gettimeofday(&m_endTime, NULL);

	if (this->m_allRedrawStart)
	{
		gettimeofday(&m_endTime, NULL);
		m_fpsstartTime = m_endTime;
		m_evenFPS = 60;
		this->m_allRedrawStart = false;
	}

	dt = (m_endTime.tv_sec - m_fpsstartTime.tv_sec) * 1000000
			+ (m_endTime.tv_usec - m_fpsstartTime.tv_usec);

	if (dt > 0)
	{
		m_evenFPS = 1000000.0f / dt;
	}

	gettimeofday(&m_fpsstartTime, NULL);

	m_FrameCount++;
	if (m_LastLogTime == 0 || (currentTime - m_LastLogTime) > 1000)
	{
		m_avergFPS = m_FrameCount;

		GLShapeDrawer::drawBoxTime = 0;
		GLShapeDrawer::drawMeshTime = 0;
		GLShapeDrawer::drawPMITime = 0;
		m_LastLogTime = GetTimeLong();
		m_FrameCount = 0;
	}
#endif
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
//	LOGI("dt is ===============%d",dt);
	if (dt < 1000000 / m_maxFPS && dt > 0)
	{

		usleep(1000000 / m_maxFPS - dt);

	}
	gettimeofday(&m_startTime, NULL);
#endif
	return ret;
}

float RenderManager::GetEventFPS()
{
	return this->m_evenFPS;
}
void RenderManager::Render()
{
	GroupNode * groupNode = (GroupNode *) m_sceneMgr->GetSceneRoot();
//	MutexLock lock(m_mutex);
//	RenderAction* workRenderAction = this->m_renderActionMgr->GetAction();
	RenderAction* workRenderAction = this->m_renderAction;
	if (workRenderAction != NULL)
	{
		workRenderAction->SetGLContext(m_GLContext);
		workRenderAction->SetCamera(m_sceneMgr->GetCamera());

		workRenderAction->SetLights(m_sceneMgr->GetLights());

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

		if (m_useDelayDraw)
		{
			workRenderAction->Optimize();
			workRenderAction->BeginDelayDraw();
		}
		workRenderAction->SetFPS(this->GetFPS());

		workRenderAction->SetRenderEffect(this->m_renderEffect);

		workRenderAction->Begin();

		workRenderAction->SetSection(m_sceneMgr->GetSection());

		if (false&& Parameters::Instance()->m_simplityMode)
		{
			m_sceneDivide->Optimize();

			for (int i = 0; i < groupNode->Size(); i++)
			{
				SceneNode* node = groupNode->GetChild(i);
				if (node->GetName() != MAINGROUP)//不处理挂载模型的节点
				{
					node->FindVisiableObject(workRenderAction);
				}
			}

			m_sceneDivide->FindVisiableObject(workRenderAction);
		}
		else
		{
			groupNode->FindVisiableObject(workRenderAction);
		}

		workRenderAction->Execute();

		if (m_useDelayDraw)
		{
			workRenderAction->EndDelayDraw();
		}

		workRenderAction->End();
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

	}
	else if (GLVersion == 2)
	{
		m_GLContext = RenderContext::GetContext(2);
	}

	InitRenderEffect();
}

void RenderManager::OnDraw()
{
	//TODO　帧率限制暂时屏蔽掉
	if (true || !Parameters::Instance()->m_IsHighPerformanceView)
	{
		LimitDrawFPS();
	}

	///线程资源锁定
	this->m_sceneMgr->Lock();

	Trackball::KEEPINGSTATETIMES--;

	//在每一帧中释放需要释放的GL资源
	this->m_sceneMgr->GetResourceManager()->ReleaseGLObjects();

	this->m_sceneMgr->UpdateHardwareBuffer();

	CameraNode* camera = this->m_sceneMgr->GetCamera();
	if (camera != NULL)
	{
		this->m_cullerHelper.AllowCuller(camera, m_useLowQuality);
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

	Parameters::Instance()->m_screenWidth = width;
	Parameters::Instance()->m_screenHeight = height;

	this->m_iScreenWidth = width;
	this->m_iScreenHeigh = height;

	CameraNode* camera = this->m_sceneMgr->GetCamera();
	int screenHeight = this->GetWindowHeight();
	int screenWidth = this->GetWindowWidth();
	camera->SetViewPort(0, 0, screenWidth, screenHeight);
	//当窗口大小变化时，需要重新设置viewPort 和 摄像机orth的大小
	if (camera)
	{
		BoundingBox& pBoundingBox = this->m_sceneMgr->GetSceneBox();

		float width = 100;
		float height = 100;
		if (screenHeight <= screenWidth)
		{
			width = pBoundingBox.Length();
			height = width * screenHeight / screenWidth;
		}
		else
		{
			height = pBoundingBox.Length();
			width = height * screenWidth / screenHeight;
		}

		this->GetCullerHelper().SetModelLength(pBoundingBox.Length());

		camera->SetOrthoSize(Vector2(width * 2, height * 2));
	}

	SceneNode* rootNode = m_sceneMgr->GetSceneRoot();
	BackgroundNode* backgroundNode = (BackgroundNode *) rootNode->Search(
			BACKGROUNDCOLOR);
	if (backgroundNode != NULL)
	{
		backgroundNode->SetBackgroundSize(this->m_iScreenWidth,
				this->m_iScreenHeigh);
	}

	AxisNode* axisNode = (AxisNode *) rootNode->Search(AXIS);
	if (axisNode != NULL)
	{
		axisNode->SetViewSize(this->m_iScreenWidth, this->m_iScreenHeigh, 0.25);
	}

	FPSNode* fpsNode = (FPSNode *) rootNode->Search(FPS_FLAG);
	if (fpsNode != NULL)
	{
		fpsNode->SetSceneHeight(this->m_iScreenHeigh);
	}

	if (m_renderAction)
	{
		m_renderAction->ResizeFBOs(width, height); //设置FBO大小
	}

	this->m_sceneMgr->UnLock();

	RequestRedraw();
}

void RenderManager::SceneChanged()
{
	if (this->m_sceneDivide)
	{
		this->m_sceneDivide->MarkDirty();
	}
}

void RenderManager::VRPrepareDrawList()
{
	//TODO　帧率限制暂时屏蔽掉

		///线程资源锁定
		//this->m_sceneMgr->Lock();

		//Trackball::KEEPINGSTATETIMES--;

		//在每一帧中释放需要释放的GL资源
		this->m_sceneMgr->GetResourceManager()->ReleaseGLObjects();
		this->m_sceneMgr->UpdateHardwareBuffer();

		CameraNode* camera = this->m_sceneMgr->GetCamera();
		if (camera != NULL)
		{
			this->m_cullerHelper.AllowCuller(camera, m_useLowQuality);
		}


		GroupNode * groupNode = (GroupNode *) m_sceneMgr->GetSceneRoot();

		RenderAction* workRenderAction = this->m_renderAction;
		if (workRenderAction != NULL)
		{
			workRenderAction->SetGLContext(m_GLContext);
			workRenderAction->SetCamera(m_sceneMgr->GetCamera());
			workRenderAction->SetLights(m_sceneMgr->GetLights());

	//只有手机才走手动刷新逻辑

			workRenderAction->SetFPS(this->GetFPS());

			workRenderAction->SetRenderEffect(this->m_renderEffect);

			workRenderAction->Begin();

			if (false&& Parameters::Instance()->m_simplityMode)
			{
				m_sceneDivide->Optimize();

				for (int i = 0; i < groupNode->Size(); i++)
				{
					SceneNode* node = groupNode->GetChild(i);
					if (node->GetName() != MAINGROUP)//不处理挂载模型的节点
					{
						node->FindVisiableObject(workRenderAction);
					}
				}

				m_sceneDivide->FindVisiableObject(workRenderAction);
			}
			else
			{
				groupNode->FindVisiableObject(workRenderAction);
			}

			//workRenderAction->Execute();

			workRenderAction->End();
		}
		//	LOGE("RenderManager::OnDraw() end");
		//this->m_sceneMgr->UnLock();
}

void RenderManager::VRDraw()
{
	RenderAction* workRenderAction = this->m_renderAction;
	if (workRenderAction != NULL)
	{
	GLShapeDrawer::InitialGL();
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
	if (this->m_sceneDivide)
	{
		this->m_sceneDivide->MarkDirty();
	}

	if (m_renderAction)
	{
		m_renderAction->ClearGLResource();
	}


	//高性能模式浏览时，使用渐进显示方式进行绘制
	m_useDelayDraw = false;
	m_renderAction->SetAllowDelayDraw(m_useDelayDraw);
	if(m_useDelayDraw)
	{
		FRAME_BUFFER_COUNT = 1;
	}else
	{
		FRAME_BUFFER_COUNT = 400;
	}
}

bool RenderManager::UseLowQuality()
{
	return this->m_useLowQuality;
}

}

