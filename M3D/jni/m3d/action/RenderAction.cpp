#include "sview/views/Parameters.h"

#include "m3d/action/RenderAction.h"
#include "m3d/renderer/gl20/ShaderManager.h"
#include "m3d/renderer/RenderContext.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/Renderable.h"
#include "m3d/scene/BackgroundNode.h"
#include "m3d/scene/FPSNode.h"
#include "m3d/scene/AxisNode.h"
#include "m3d/model/Model.h"
#include "m3d/model/Body.h"
#include "m3d/model/Point.h"
#include "m3d/model/Face.h"
#include "m3d/model/Edge.h"
#include "m3d/model/Note.h"
#include "m3d/model/MeshData.h"
#include "m3d/model/PMIData.h"
#include "m3d/utils/CullerHelper.h"
#include "m3d/SceneManager.h"
#include "m3d/action/CallBackAction.h"

#include "m3d/RenderManager.h"
#include "m3d/renderer/gl10/GLShapeDrawer.h"
#include "m3d/renderer/GL20/GLDrawer20.h"
#include "m3d/renderer/gl20/GLShapeDrawer20.h"
#include "m3d/graphics/HardWareFrameBuffer.h"
#include "m3d/extras/note/NoteGroup.h"
#include "m3d/graphics/DirectionalLight.h"
#include "m3d/scene/SceneGroundNode.h"
#include "sview/views/Parameters.h"
#include "../model/ModelShape.h"
#include "m3d/renderer/effect/EffectManager.h"
#include "m3d/scene/GroundNode.h"
#include "m3d/graphics/Material.h"
#include "m3d/graphics/PbrMaterial.h"

#include "m3d/graphics/Texture2D.h"

#ifdef WIN32
#include "m3d/UI/GUI.h"
#endif //WIN32

using SVIEW::Parameters;

namespace M3D
{
 ShaderProgram* RenderAction::m_currentSP = NULL;
 DirectionalLight RenderAction::m_light;
const int RenderEffect::RENDER_NORMALEFFCT = 0x00000000; //正常渲染
const int RenderEffect::RENDER_OPACITYEDGE = 0x00000100; //不透明边界线渲染
const int RenderEffect::RENDER_SECTION = 0x00000200; //盖面渲

const int RenderableType::RGT_SOLID = 0x00000001;
const int RenderableType::RGT_EDGELINE = 0x00000008;
const int RenderableType::RGT_TRILINE = 0x00000002;
const int RenderableType::RGT_BOX = 0x00000004;
const int RenderableType::RGT_TRANSPARENT = 0x00000010;
const int RenderableType::RGT_TRANSLATEEDGE = 0x00000020;
const int RenderableType::RGT_EDGELINEINTOP = 0x00000040;
const int RenderableType::RGT_INTOP = 0x00000080;
const int RenderableType::RGT_PMI = 0x00000100;
const int RenderableType::RGT_POINT = 0x00000200;

const int RenderableType::RGT_NOTE = 0x00000400;
const int RenderableType::RGT_ANNOTATION = 0x00000600;
const int RenderableType::RGT_MEASURE = 0x00000800;
const int RenderableType::RGT_HANDLER = 0x00001000;

const int RenderableType::RGT_SHADOW = 0x00001200;

const int RenderableType::RGT_SOLID_TRAN = RenderableType::RGT_SOLID
		|RenderableType::RGT_TRANSPARENT;//!<实体透明显示

const int RenderableTypeGroup::RENDERDATA_NORMALEFFCT =
		RenderableType::RGT_POINT|RenderableType::RGT_SOLID | RenderableType::RGT_INTOP
				| RenderableType::RGT_TRANSPARENT|RenderableType::RGT_NOTE
				|RenderableType::RGT_MEASURE|RenderableType::RGT_HANDLER|RenderableType::RGT_EDGELINE; //正常渲染

const int RenderableTypeGroup::RENDERDATA_OPACITYEDGE =
		RenderableType::RGT_SOLID | RenderableType::RGT_INTOP
				| RenderableType::RGT_TRANSPARENT | RenderableType::RGT_EDGELINE; //不透明边界线渲染

const int RenderableTypeGroup::RENDERDATA_SECTION = RenderableType::RGT_SOLID
		| RenderableType::RGT_INTOP | RenderableType::RGT_TRANSPARENT; //盖面渲

const int RenderableTypeGroup::RENDERDATA_SOLIDEDGE = RenderableType::RGT_POINT|RenderableType::RGT_SOLID
		| RenderableType::RGT_INTOP | RenderableType::RGT_TRANSPARENT
		| RenderableType::RGT_TRILINE; //实体和线框

const int RenderableTypeGroup::RENDERDATA_LINE = RenderableType::RGT_POINT|RenderableType::RGT_INTOP
		| RenderableType::RGT_TRANSPARENT | RenderableType::RGT_TRILINE; //线框渲染

const int RenderableTypeGroup::RENDERDATA_TRIANDSOLID =RenderableType::RGT_POINT|
		RenderableType::RGT_SOLID | RenderableType::RGT_INTOP
				| RenderableType::RGT_TRANSPARENT | RenderableType::RGT_TRILINE; //三角线框和实体渲染

const int RenderableTypeGroup::RENDERDATA_RENDERTRISOLIDBOX =RenderableType::RGT_POINT|
		RenderableType::RGT_SOLID | RenderableType::RGT_INTOP
				| RenderableType::RGT_TRANSPARENT | RenderableType::RGT_TRILINE
				| RenderableType::RGT_BOX; //线框和实体，包围盒渲染

const int CullerMode::ALL_LEVEL = 0xFFFFFFFF;
const int CullerMode::MODEL_LEVEL = 0x00000001;
const int CullerMode::BODY_LEVEL = 0x00000002;
const int CullerMode::MESH_LEVEL = 0x00000004;
const int CullerMode::FACE_LEVEL = 0x00000008;

/**
 * 构造函数
 */
CullerMode::CullerMode()
{
	///默认只允许Model级别的剔除
	m_CullerMode = MODEL_LEVEL;
}

CullerMode::~CullerMode()
{
	m_CullerMode = 0x00000000;
}

void CullerMode::SetCullerFace(bool cullFace)
{
	if (cullFace)
	{
		this->m_CullerMode |= FACE_LEVEL;
	}
	else
	{
		this->m_CullerMode &= (FACE_LEVEL & ALL_LEVEL);
	}
}

bool CullerMode::AllowCullerFace()
{
	return (this->m_CullerMode & FACE_LEVEL);
}

void CullerMode::SetCullerModel(bool cullModel)
{
	if (cullModel)
	{
		this->m_CullerMode |= MODEL_LEVEL;
	}
	else
	{
		this->m_CullerMode &= (MODEL_LEVEL & ALL_LEVEL);
	}
}

bool CullerMode::AllowCullerModel()
{
	return (this->m_CullerMode & MODEL_LEVEL);
}

RenderAction::RenderAction(RenderManager* renderMgr) :
		Action(),
	 m_mirrorWidth(971), 
	m_mirrorHeight(591){
	this->m_pCamera = NULL;

	this->m_scene = NULL;

	//构造显示对象存储组
 
	this->InitRenderQueueGroup(m_RenderQueueGroup);
	this->InitRenderQueueGroup(m_DraggerRenderQueueGroup);
	this->SetWorkingRenderQueueGroup(&m_RenderQueueGroup);

	this->m_renderMgr = renderMgr;

	this->m_shaderManager = NULL;
    this->m_backgroundColor = NULL;
	 m_clipPlane.resize(5);
	 m_enableClip.resize(5,0.0f);
	 m_bReverseClip = false;

	this->InitRenderEffects();

	this->Reset();

	
	//this->m_mirrorFBO.Create();
}
RenderAction::~RenderAction()
{
	if(this->m_shaderManager)
	{
		delete m_shaderManager;
		m_shaderManager = NULL;
	}
	if (this->m_mirrorPixels != nullptr) {

		delete[] m_mirrorPixels;
		m_mirrorPixels = nullptr;
	}
	this->ReleaseRenderEffects();
}

void RenderAction::Reset()
{
	this->m_priTypeCache = -1;
	m_minfps = 15;
	m_fps = m_minfps;

	m_delayDrawFlag = false;

	m_delayDrawOnceCount = 1000;

	ResourceManager* resourceMgr = this->m_renderMgr->GetSceneManager()->GetResourceManager();

	m_frameBffer.SetResourceManager(resourceMgr);//!< 帧缓存对象

	m_teethFBO.SetResourceManager(resourceMgr);//!< 牙齿FBO

	m_delayDrawFBO.SetResourceManager(resourceMgr);//!< 渐进显示FBO

	m_depthMapFBO.SetResourceManager(resourceMgr);//!< Maincamera出的相机深度图，用来处理边框线

	m_edgeDetectionFBO.SetResourceManager(resourceMgr);//!< 边缘检测帧缓存对象

	m_mirrorFBO.SetResourceManager(resourceMgr);

	m_clearDelayDrawFlag = true;
}

void RenderAction::Apply(SceneNode * node)
{
	this->OnExecute(node);
}

RenderManager* RenderAction::GetRenderManager()
{
	return this->m_renderMgr;
}

void RenderAction::UpdataRenderPara(Body* body)
{
	int bodyLodCount = body->GetLodCount() - 1;
	int specifyLodLevel = this->m_renderMgr->GetLodLevel();
	//判断当前可用LOD和设定lod等级的关系，选取最佳的可以LOD等级作为绘制时使用的等级
	int currentUseLod =
			bodyLodCount > specifyLodLevel ? specifyLodLevel : bodyLodCount;
	this->SetCurrentSpecifyLod(currentUseLod);
}
void RenderAction::PushRenderable(Renderable* renderable, int type)
{
	//对状态进行缓存，如果没有改变，则使用前一次的队列
	if (this->m_priTypeCache == type)
	{
		this->m_currentRenderCount++;
		this->m_priTypePassGroup->second.Push(renderable);
		return;
	}

	//如果状态改变了，则进行缓存
	map<int, RenderQueue>::iterator it = this->m_WorkingRenderQueueGroup->find(type);
	if (it != this->m_WorkingRenderQueueGroup->end())
	{
		it->second.Push(renderable);
		this->m_priTypeCache = type;
		this->m_priTypePassGroup = it;

		this->m_currentRenderCount++;
	}
}

void RenderAction::Begin()
{
	map<int, RenderQueue>::iterator it = this->m_RenderQueueGroup.begin();
	while (it != this->m_RenderQueueGroup.end())
	{
		it->second.Clear();
		it++;
	}

	//清理拖拽器的显示队列
	it = this->m_DraggerRenderQueueGroup.begin();
	while (it != this->m_DraggerRenderQueueGroup.end())
	{
		it->second.Clear();
		it++;
	}

	this->GetRenderImageboards().clear();

	this->SetSection(NULL);
	this->SetFPSNode(NULL);
	this->SetAxisNode(NULL);
	this->SetBackGroundNode(NULL);
	this->SetNoteGroup(NULL);
	this->SetMeasures(NULL);
	this->ClearRenderBox();
	this->SetHandlerGroupNode(NULL);
	this->SetGroundNode(NULL);
	//清除渐进显示队列中的内容
	this->BeginPrepareDelayDraw();

	this->SetWorkingRenderQueueGroup(&this->m_RenderQueueGroup);
 
	this->m_currentRenderImageQueueIndex = 0;
	m_currentRenderCount = 0;
	this->m_renderUIs.clear();

	this->m_hudImages.clear();
}

void RenderAction::Execute()
{
	int GLVersion = this->m_renderMgr->GetGLVersion();
	if( GLVersion== 1)
	{
//		LOGI("execute es1");
		GLShapeDrawer::InitialGL();
		GLShapeDrawer::DrawBackGround(this->GetBackGroundNode(), this);
		GLShapeDrawer::DrawRenderPassGroup(this);
	//	GLShapeDrawer::DrawHandlerGroup(this);
		GLShapeDrawer::DrawAxis(this->GetAxisNode(), this);
		GLShapeDrawer::DrawFPS(this->GetFPSNode(), this);
	}
	else if( GLVersion== 2)
	{
//		GLint nStereoSupport;
//		glGetIntegerv(GL_STEREO, &nStereoSupport);
//		LOGI("execute es2");
		//this->BeginDelayDraw();

		GLDrawer20::InitialGL();
		CameraNode * camera = this->GetCamera();
		GroundNode * groundNode = this->GetGroundNode();
		CameraNode * mirrorCamera = groundNode->GetMirrorCamera();
		if (Parameters::Instance()->m_isOpenMirror){
			m_mirrorFBO.Create();
			m_mirrorFBO.Bind();
			m_mirrorTexture = m_mirrorFBO.AttachTextureToColor();
		    const IntRect& rect = camera->GetViewPort().GetRect();			
			mirrorCamera->SetViewPort(rect.m_left, rect.m_top, rect.Width(), rect.Height());
			this->SetCamera(mirrorCamera);
		    m_mirrorFBO.RenderToMe();
			
		}	
        ExecuteObjects();
		
		if (Parameters::Instance()->m_isOpenMirror){
		//	m_mirrorPixels = new GLubyte[m_mirrorWidth * m_mirrorHeight];
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glReadPixels(0, 0, m_mirrorWidth, m_mirrorHeight,
				GL_RGBA, GL_UNSIGNED_BYTE, m_mirrorPixels);			
		//	GLubyte pixels[1000];
			//for (int i = 0; i < 1000; ++i)
		//	{
		//		pixels[i] = m_mirrorPixels[i+2000];
		//	}
			
			RenderContext * gl = GetGLContext();
			Matrix4	mirrorMat = mirrorCamera->GetProjection() *	mirrorCamera->GetView().ToMatrix4();
		//	Matrix4	mirrorMat = gl->GetProjectMatrix() * gl->GetViewMatrix();
			m_mirrorFBO.UnBind();
			this->SetCamera(camera);
	        ExecuteObjects();
	    	GLuint mirrorTexture = Texture2D::CreateRGBATexture(m_mirrorPixels, m_mirrorWidth, m_mirrorHeight); 	
	        GLShapeDrawer20::DrawMirrorGroundNode(groundNode, this, mirrorMat, mirrorTexture, m_mirrorBackgroundTexture);
			glBindTexture(GL_TEXTURE_2D, 0);	
			glDeleteTextures(1, &m_mirrorTexture);
		
		//	this->SetCamera(camera);
			
		}else {
			GLShapeDrawer20::DrawGroundNode(groundNode, this);
		}		
		
		//绘制拖拽器
		GLShapeDrawer20::DrawDraggerPass(this);	
		GLShapeDrawer20::DrawAxis(this->GetAxisNode(), this);		
		GLShapeDrawer20::DrawFPS(this->GetFPSNode(), this);
		this->GetScene()->GetCamera()->SetCameraMode(0);
		//this->EndDelayDraw();
	}
}

void RenderAction::ExecuteObjects(){

	if (Parameters::Instance()->m_useStereo)
	{
		GLShapeDrawer20::SetFocalLength(this);
		//float distance = (this->GetScene()->GetCamera()->GetWorldPosition() - this->GetScene()->GetSceneBox().Center()).Length()*0.03;
		//this->GetScene()->GetCamera()->SetPupilDistance(distance);
		////		this->GetScene()->GetCamera()->UpdatePupilCamera(distance);
		this->GetScene()->GetCamera()->SetCameraMode(1);
		if (Parameters::Instance()->m_BackTransparent == false) {
			GLShapeDrawer20::DrawBackGround(this->GetBackGroundNode(), this);
		}
		GLShapeDrawer20::DrawRenderPassGroup(this);
		this->GetScene()->GetCamera()->SetCameraMode(2);
		if (Parameters::Instance()->m_BackTransparent == false) {
			GLShapeDrawer20::DrawBackGround(this->GetBackGroundNode(), this);
		}
		GLShapeDrawer20::DrawRenderPassGroup(this);
	}
	else
	{
		if (Parameters::Instance()->m_BackTransparent == false) {
			GLShapeDrawer20::DrawBackGround(this->GetBackGroundNode(), this);
		}
		GLShapeDrawer20::DrawRenderPassGroup(this);
	}
}

void RenderAction::End()
{
	this->SetSceneBoxChanged(false);
}

void RenderAction::SetWorldMatrix(Matrix3x4* matrix)
{
	this->m_WorldMatrix = matrix;
}

Matrix3x4* RenderAction::GetWorldMatrix()
{
	return this->m_WorldMatrix;
}

/**
 * 获取当前准备显示数据所需的世界矩阵
 * @return 世界矩阵
 */
Matrix4* RenderAction::GetGLWorldMatrix()
{
	return this->m_glworldMatrix;
}

/**
 * 设置准备显示数据所需的世界矩阵
 * @param matrix 世界矩阵
 */
void RenderAction::SetGLWorldMatrix(Matrix4* matrix)
{
	this->m_glworldMatrix = matrix;
}


bool RenderAction::ShowRotationCenter()
{
	return this->m_renderMgr->IsShowRotationCenter();
}

void RenderAction::SetRenderEffect(RenderEffect* renderEffect)
{
	m_renderEffect = renderEffect;
}

RenderEffect* RenderAction::GetRenderEffect()
{
	return this->m_renderEffect;
}

void RenderAction::ReleaseRenderEffects()
{
//	RENDEREFFECTMAP::iterator it = this->renderEffectsMap.begin();
//	while(it != this->renderEffectsMap.end())
//	{
//		delete it->second;
//	}
}

void RenderAction::SetImmediateDrawBegin(bool immediateDraw)
{
	this->m_immediatelyDrawBegin = immediateDraw;
}

bool RenderAction::IsImmediateDrawBegin()
{
	return this->m_immediatelyDrawBegin;
}

void RenderAction::ClearDelayDraw()
{
	m_immediatelyDrawBegin = false;
	m_clearDelayDrawFlag = true;
}

void RenderAction::SetDelayDraw(bool delayDraw)
{
	this->m_delayDrawFlag = delayDraw;
}

bool RenderAction::IsDelayDraw()
{
	return this->m_delayDrawFlag;
}

void RenderAction::AddToDelayDraw(Renderable* renderable)
{
	this->m_delayDrawList.Push(renderable);
}

void RenderAction::AddFaceToDrawQueue(Face* renderable)
{
	RenderEffect* renderType = this->GetRenderEffect();
	RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();

	renderable->SetRenderWorldMatrix(this->GetGLWorldMatrix());

	if (dataType.Contain(RenderableType::RGT_SOLID))
	{
		if (this->m_delayDrawFlag)
		{
			AddToDelayDraw(renderable);
		}
		else
		{
			PrepareFaceMesh(renderable);
		}
	}
}

void RenderAction::InitRenderQueueGroup(map<int, M3D::RenderQueue>& renderQueueGroup)
{
	//构造存在坐标轴渲染的
	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_SHADOW,
			RenderQueue(RenderableType::RGT_SHADOW)));
	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_TRANSPARENT,
			RenderQueue(RenderableType::RGT_TRANSPARENT)));
	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_SOLID,
			RenderQueue(RenderableType::RGT_SOLID)));

	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_INTOP,
			RenderQueue(RenderableType::RGT_INTOP)));

	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_EDGELINE,
			RenderQueue(RenderableType::RGT_EDGELINE)));

	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_TRILINE,
			RenderQueue(RenderableType::RGT_TRILINE)));

	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_EDGELINEINTOP,
			RenderQueue(RenderableType::RGT_EDGELINEINTOP)));

	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_PMI,
			RenderQueue(RenderableType::RGT_PMI)));

	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_BOX,
			RenderQueue(RenderableType::RGT_BOX)));

	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_NOTE,
			RenderQueue(RenderableType::RGT_NOTE)));

	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_ANNOTATION,
			RenderQueue(RenderableType::RGT_ANNOTATION)));

	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_MEASURE,
			RenderQueue(RenderableType::RGT_MEASURE)));

	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_HANDLER,
			RenderQueue(RenderableType::RGT_HANDLER)));

	renderQueueGroup.insert(
		map<int, RenderQueue>::value_type(RenderableType::RGT_POINT,
			RenderQueue(RenderableType::RGT_POINT)));

}

void RenderAction::BeginPrepareDelayDraw()
{
	this->m_delayDrawList.Clear();
	this->m_delayDrawOnceCount = 0;
}

void RenderAction::SetAllowDelayDraw(bool allowDelayDraw)
{
	this->m_allowDelayDraw = allowDelayDraw;
}

bool RenderAction::ProcessDelayDraw()
{
	if (this->m_clearDelayDrawFlag)
	{
		m_delayDrawList.Clear();
		m_clearDelayDrawFlag = false;
		return false;
	}
	bool processState = false;
	RenderabelArray& delayDrawList =  m_delayDrawList.GetRenderableArray();
	if(delayDrawList.size()>0)
	{
		//分五次绘制完
		if(m_delayDrawOnceCount == 0 ){
			m_delayDrawOnceCount = delayDrawList.size()/5;
		}
		this->BeginDelayDraw();
		m_delayOnceDrawList.Clear();
		//从后向前删除m_delayDrawList里面的内容
		if(delayDrawList.size()>this->m_delayDrawOnceCount && m_delayDrawOnceCount > 0)
		{
			RenderabelArray& onceDrawList = m_delayOnceDrawList.GetRenderableArray();
			onceDrawList.assign(delayDrawList.begin()+(delayDrawList.size()- m_delayDrawOnceCount),delayDrawList.end());
			delayDrawList.erase(delayDrawList.begin()+(delayDrawList.size() - m_delayDrawOnceCount),delayDrawList.end());
		}
		else
		{
			RenderabelArray& onceDrawList = m_delayOnceDrawList.GetRenderableArray();
			onceDrawList.assign(delayDrawList.begin(),delayDrawList.end());
			delayDrawList.clear();
		}
		if(SVIEW::Parameters::Instance()->m_OpenGLESVersion == 1)
		{
			GLShapeDrawer::ApplyCamera(this);
			GLShapeDrawer::DrawSolidRenderPassGroup(this, &(m_delayOnceDrawList));
		}
		else
		{
			map<int, RenderQueue>::iterator it = this->m_RenderQueueGroup.begin();
			while (it != this->m_RenderQueueGroup.end())
			{
				it->second.Clear();
				it++;
			}

			this->ClearDelayDraw();
			this->FillDelayDrawRenderQueue(m_delayOnceDrawList, RenderableType::RGT_SOLID);
			GLShapeDrawer20::DrawRenderPassGroup(this);
		
			/*GLShapeDrawer20::DoSection(this, true);
			GLShapeDrawer20::ApplyCamera(this);
			GLShapeDrawer20::DrawSolidRenderPassGroup(this, &(m_delayOnceDrawList));
			if (delayDrawList.size() == 0)
				GLShapeDrawer20::DoSection(this, false);*/
		}

		processState = true;
	}

	return processState;
}

void RenderAction::Optimize(){

	float fps = this->GetScene()->GetRenderManager()->GetEventFPS();
	if(fps<10){
		this->GetScene()->GetRenderManager()->GetCullerHelper().AddCullerRatio(fps);
	}else if(fps>30)
	{
		this->GetScene()->GetRenderManager()->GetCullerHelper().ReduceCullerRatio(fps);
	}
//	//处理不透明
//	map<int, RenderQueue>::iterator it = this->m_RenderQueueGroup.find(RenderableType::RGT_SOLID);
//	if (it != this->m_RenderQueueGroup.end())
//	{
//		RenderQueue& renderQueue = it->second;
//
//
//	}
//
//	//处理透明
//	map<int, RenderQueue>::iterator it = this->m_RenderQueueGroup.find(RenderableType::RGT_TRANSPARENT);
//	if (it != this->m_RenderQueueGroup.end())
//	{
//		it->second.Push(renderable);
//	}

}

void RenderAction::BeginDelayDraw()
{
#ifdef WIN32
//	if (!RenderContext::IsFramebufferSupported)
//	{
////		SVIEW::Parameters::Instance()->m_msaaNum = 0;
//		return;
//	}
//	this->m_delayDrawFBO.Create();
//	this->m_delayDrawFBO.GetFirstFBO();
////	action->m_teethFBO.SetSize(intRect.m_right, intRect.m_bottom);
//	if (Parameters::Instance()->m_msaaNum == 0)
//	{
//		this->m_delayDrawFBO.SetColorAttachmentAttribute(0, false, true);//不使用多重采样
//	}
//	else
//	{
//		this->m_delayDrawFBO.SetColorAttachmentAttribute(0, true, true);//使用多重采样
//	}
//	this->m_delayDrawFBO.CreateDepthAttachment(true);
//	this->m_delayDrawFBO.LinkTextureColorAttachment(0);//0号挂载点
//	this->m_delayDrawFBO.CheckStatus();
//	this->m_delayDrawFBO.Resize();
//	this->m_delayDrawFBO.Bind();



	if (!RenderContext::IsFramebufferSupported)
	{
		//		SVIEW::Parameters::Instance()->m_msaaNum = 0;
		return;
	}

	if (Parameters::Instance()->m_msaaNum == 0)
	{
		m_delayDrawFBO.SetParameters(1, true, true, true, true, false);
	}
	else
	{
		m_delayDrawFBO.SetParameters(1, true, true, true, true, true);
	}
	m_delayDrawFBO.ReShape();
	this->m_delayDrawFBO.Bind();
	


#else

#endif
}

void RenderAction::EndDelayDraw()
{
	if (!RenderContext::IsFramebufferSupported)
	{
		//SVIEW::Parameters::Instance()->m_msaaNum = 0;
		return;
	}
	//this->m_delayDrawFBO.UnBind();
#ifdef WIN32
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, m_delayDrawFBO.GetObject());
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, this->m_delayDrawFBO.GetOriginalFBO());
	const Viewport temp = m_scene->GetCamera()->GetViewPort();
	glBlitFramebufferEXT(0, 0, temp.GetRect().m_right, temp.GetRect().m_bottom, 
		0, 0, temp.GetRect().m_right, temp.GetRect().m_bottom, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT,0);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
#else
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, m_delayDrawFBO.GetObject());
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->m_delayDrawFBO.GetOriginalFBO());
	const Viewport temp = m_scene->GetCamera()->GetViewPort();
	//glBlitFramebuffer(0, 0, temp.GetRect().m_right, temp.GetRect().m_bottom,
	//	0, 0, temp.GetRect().m_right, temp.GetRect().m_bottom, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	//glBindFramebuffer(GL_READ_FRAMEBUFFER,0);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
#endif
	//m_delayDrawFBO.RestoreOriginalFBO();
	//m_delayDrawFBO.RestoreOriginalFBO();

	//glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	//if(!this->m_delayDrawFBO.HasValue())
	//{
	//	return;
	//}
	//	float debugvertices[] =
	//	{ // Vertices for the square
	//		-1.0f, 1.0f, -1.0f,// 0. left-bottom
	//		-1.0f, -1.0f, -1.0f,// 1. right-bottom
	//		1.0f, 1.0f, -1.0f,// 2. left-top
	//		1.0f, -1.0f, -1.0f// 3. right-top
	//	};

	//	float debugcoord[] = {  // Vertices for the square
	//	             0, 1,   // 0. left-bottom
	//	             0, 0,  // 1. right-bottom
	//	           1,  1,   // 2. left-top
	//	             1,  0  // 3. right-top
	//	         };
	//	CameraNode * camera = this->GetCamera();
	//		const IntRect& intRect = camera->GetViewPort().GetRect();

	//		glViewport(intRect.m_left, intRect.m_top, intRect.m_right, intRect.m_bottom);

	//		glEnable(GL_DEPTH_TEST);
	//	ShaderProgram *shaderEffect = this->GetShaderMananger()->GetEffect(ShaderManager::Quad);
	//	shaderEffect->UseProgram();

	//	ShaderParameter * vertex = shaderEffect->GetShaderAttributeParameter(VSP_POSITION);
	//	shaderEffect->SetVertexAttribPointer(vertex->m_location, 3, GL_FLOAT, 0, (void *) debugvertices);
	//	shaderEffect->EnableAttributeArray(vertex->m_location);

	//	ShaderParameter * texCoords = shaderEffect->GetShaderAttributeParameter(VSP_TEXCOORDS);
	//	shaderEffect->SetVertexAttribPointer(texCoords->m_location, 2, GL_FLOAT, 0, (void *) (debugcoord));
	//	shaderEffect->EnableAttributeArray(texCoords->m_location);
	//	{
	//		int sampler0 = 0;
	//		ShaderParameter * sample0Pra = shaderEffect->GetShaderUniformParameter(FSP_SAMPLER0);
	//		shaderEffect->SetUniformValue(sample0Pra->m_location, sampler0);
	//	}

	//	glActiveTexture(GL_TEXTURE0);
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D,((GeometryBuffer*)(this->m_delayDrawFBO.GetColorAttachment(0)))->GetID());

	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//	glBindTexture(GL_TEXTURE_2D, 0);
	//	glDisable(GL_TEXTURE_2D);
	//	shaderEffect->DisableAttributeArray(vertex->m_location);
	//	shaderEffect->DisableAttributeArray(texCoords->m_location);
	//	shaderEffect->ReleaseShaderProgram();
	//	glEnable(GL_DEPTH_TEST);
}

CullerHelper& RenderAction::GetCullerHelper()
{
	return this->m_renderMgr->GetCullerHelper();
}

RenderQueueGroup& RenderAction::GetRenderQueueGroup()
{
	if (this->m_WorkingRenderQueueGroup)
	{
		return *this->m_WorkingRenderQueueGroup;
	}
	//如果不存在则返回默认的渲染队列
	return this->m_RenderQueueGroup;
}

void RenderAction::InitRenderEffects()
{
//	RenderOrder* normalRender = new RenderOrder();
//	normalRender->InitialNormalEffect();
//	this->renderEffectsMap.insert(pair<RENDERTYPE,RenderOrder*>(RENDERTYPE::RENDER_NORMALEFFCT,normalRender));
//	RenderOrder* opacityLineRender = new RenderOrder();
//	opacityLineRender->InitialOpacityLineEffect();
//	this->renderEffectsMap.insert(pair<RENDERTYPE,RenderOrder*>(RENDERTYPE::RENDER_OPACITYEDGE,opacityLineRender));

//	LOGI("m_lights.size() == %d",m_lights.size());
//	if (m_lights.size() > 3)
//	{
//		return;
//	}
//	for (int i = 0; i < 1; i++)
//	{
//		Light * tempLight = new Light;
//		tempLight->TurnOff();
//
//		Color lambient(1, 1, 1, 1.0f);
//		Color ldiffuse(1, 1, 1, 1.0f);
//		Color lspecular(1, 1, 1, 1.0f);
//		Vector4 lpos(0.0, 0.0, 10.0, 0.0);
//		Vector3 lspotdir(0.0, 0.0, -1.0);
//		Color lambients(0.1, 0.1, 0.1, 1.0f);
//		float spotExponent = 100.0f, spotCutoff = 60.0f, spotCosCutoff = 0.5f, constantAttenuation = 1.0f,
//				linearAttenuation = 0.0005f, quadraticAttenuation = 0.0001f, intensity = 1.0;
//		tempLight->SetAmbient(lambient);
//		tempLight->SetDiffuse(ldiffuse);
//		tempLight->SetSpecular(lspecular);
//		tempLight->SetPosition(lpos);
//		tempLight->SetLightModelAmbient(lambients);
//		this->AddLight(tempLight);
//	}

//	m_frameBffer.SetSize(1024,1024);
//	m_teethFBO.SetSize(1024,1024);

	DirectionalLight &tempLight = m_light;
	Color lambient(1, 1, 1, 1.0f);
	Color ldiffuse(1, 1, 1, 1.0f);
	Color lspecular(1.0, 1.0, 1.0, 1.0f);
	Vector4 lpos(0.0, 0.0, 10.0, 0.0);
	Vector3 lspotdir(0.0, 0.0, 1.0);
	Color lambients(0.01, 0.10, 0.01, 1.0f);
	float spotExponent = 100.0f, spotCutoff = 60.0f, spotCosCutoff = 0.5f, constantAttenuation = 1.0f,
			linearAttenuation = 0.0005f, quadraticAttenuation = 0.0001f, intensity = 1.0;
	tempLight.SetAmbient(lambient);
	tempLight.SetDiffuse(ldiffuse);
	tempLight.SetSpecular(lspecular);
	tempLight.SetPositionOld(lpos);
	tempLight.SetLightModelAmbient(lambients);
	tempLight.SetSpecularIntensity(intensity);

}

void RenderAction::SetCamera(CameraNode* camera)
{
	this->m_pCamera = camera;
}

CameraNode* RenderAction::GetCamera()
{
	return this->m_pCamera;
}

void RenderAction::PrepareRenderPMIS(Model* model)
{
	map<int, PMIData*>* modelPMIs = model->GetPMIs();
	if (modelPMIs->size() > 0)
	{
		map<int, PMIData*>::iterator it = modelPMIs->begin();
		PMIData* pmi;
		while (it != modelPMIs->end())
		{
			pmi = it->second;
			if (pmi->IsVisible())
			{
				pmi->FindVisiableObject(this);
			}

			it++;
		}
	}
}

void RenderAction::PrepareRenderPMIS(map<int, PMIData*>* modelPMIs)
{
	if (!modelPMIs)
	{
		return;
	}
	if (modelPMIs->size() > 0)
	{
		map<int, PMIData*>::iterator it = modelPMIs->begin();
		PMIData* pmi;
		while (it != modelPMIs->end())
		{
			pmi = it->second;
			if (pmi->IsVisible())
			{
				pmi->FindVisiableObject(this);
			}

			it++;
		}
	}
}

void RenderAction::PrepareRenderBox(ModelShape* modelShape)
{
	if (modelShape->GetWorldBoundingBox().Defined())
	{
		this->MergeRenderBox(modelShape->GetWorldBoundingBox());
		this->PushRenderable(NULL, RenderableType::RGT_BOX);
	}
	//model->SetRenderWorldMatrix(this->GetGLWorldMatrix());
}

void RenderAction::PrepareRenderSection(Section* section)
{
	SetSection(section);
}

void  RenderAction::PrepareRenderNote(Note* note)
{
   this->PushRenderable(note, RenderableType::RGT_NOTE);
}

void RenderAction::PrepareRenderAnnotation(Note* note)
{
	this->PushRenderable(note, RenderableType::RGT_ANNOTATION);
}

void RenderAction::PrepareRenderImage(ImageBoard* imageBoard)
{
	int currentImageQueueIndex = this->GetCurrentRenderImageQueueIndex();
	if (currentImageQueueIndex == 0)
	{
		this->m_renderImageboards.push_back(imageBoard);
	}
	else if (currentImageQueueIndex ==1)
	{
		this->m_renderUIs.push_back(imageBoard);
	}else if (currentImageQueueIndex == 2)
	{
		this->m_hudImages.push_back(imageBoard);
	}
}

void RenderAction::PreapareRenderPoint(Point* point)
{
	//没有做合并处理，如果需要的话，在进行优化
	this->PushRenderable(point, RenderableType::RGT_POINT);
}

void RenderAction::StartMergeFace()
{
	this->m_mergeFaceCache = NULL;
}

void RenderAction::FinishMergeFace()
{

}

void RenderAction::StartMergeEdge()
{
	this->m_mergeEdgeCache = NULL;
}

void RenderAction::FinishMergeEdge()
{

}

bool RenderAction::Merge(Face* faceRenderData)
{
	bool mergeState = false;
	//获取需要合并的face的显示状态，当前仅有Color，后期扩展成Material
	Color* drawColor = faceRenderData->GetDrawColor();
	//为空说明第一次进行合并
	//判断颜色状态是否和前一个Face相同，如果相同则进行面合并，如果不同则不进行合并
	//后期增加获取Hash值，通过Hash值进行高效判断
	//如果不可见，将当前合并缓存设置为空，等待下一次重新开始合并
	if(!faceRenderData->IsVisible())
	{
		m_mergeFaceCache = NULL;
		//由于隐藏的面需要跳过，因此假设合并成功，让其返回
		mergeState = true;
		goto RET;
	}
	else if (m_mergeFaceCache == NULL
			|| *drawColor != m_mergeFaceCache->GetRenderColor())
	{
		//合并失败了，将faceRenderData赋值给m_mergeFace，供下一次合并时使用
		if( faceRenderData->GetRenderMeshData()){
			m_mergeFaceCache = faceRenderData;

			VertexSet* meshData = faceRenderData->GetRenderMeshData()->GetRefMesh();
			if(meshData->IsDirty())
			{
				SceneManager* scene = this->m_scene;
				//显示更新硬件缓存
				meshData->UpdateHardWareBuffer(scene);
			}

			m_mergeFaceCache->SetRenderMesh(faceRenderData->GetRenderMeshData());
		}

	}
	else
	{
		m_mergeFaceCache->MergeRenderMesh(faceRenderData->GetData());
		mergeState = true;
	}

	RET:

	return mergeState;
}

bool RenderAction::Merge(Edge* edge)
{
	bool mergeState = false;
	//对于边界线仅仅在选中时颜色才会改变，因此仅仅通过选中状态进行合并
	//后期增加获取Hash值，通过Hash值进行高效判断

	//获取需要合并的face的显示状态，当前仅有Color，后期扩展成Material
	Color* drawColor = edge->GetDrawColor();

	//如果不可见，将当前合并缓存设置为空，等待下一次重新开始合并
	if(!edge->IsVisible())
	{
		m_mergeEdgeCache = NULL;
		//由于隐藏的线需要跳过，因此假设合并成功，让其返回
		mergeState = true;

		goto RET;
	}
	else if (m_mergeEdgeCache == NULL
			|| *drawColor != *m_mergeEdgeCache->GetDrawColor())
	{
		//合并失败了，将faceRenderData赋值给m_mergeFace，供下一次合并时使用
		m_mergeEdgeCache = edge;
		m_mergeEdgeCache->SetRenderPolyLine(edge);
	}
	else
	{
		m_mergeEdgeCache->MergeRenderPolyLine(edge);
		mergeState = true;
	}
RET:
	return mergeState;
}

void RenderAction::PrepareRenderEdges(Body* body)
{
	RenderEffect* renderType = this->GetRenderEffect();
	RenderableTypeGroup& dataType = renderType->GetRenderableTypeFilter();

	if (dataType.Contain(RenderableType::RGT_EDGELINE))
	{
		vector<Edge*>* bodyEdges = body->GetEdges();
//		LOGE("Mrege body edge begin");
		for (int i = 0; i < bodyEdges->size(); i++)
		{
			Edge* edge = bodyEdges->at(i);
			PrepareRenderEdges(edge);
		}
//		LOGE("Mrege body edge end");
	}
}

void RenderAction::PrepareRenderEdges(Edge* edge)
{
	if (Merge(edge))
	{
		return;
	}
	edge->SetRenderWorldMatrix(this->GetGLWorldMatrix()); 
	this->PushRenderable(edge, RenderableType::RGT_EDGELINE);
}

bool RenderAction::ISFaceHasDrawData(Face* face)
{
	Mesh* meshdata = (Mesh*) face->GetData();

	if (meshdata != NULL)
	{
		face->SetRenderMeshData(meshdata);
		return true;
	}

	return false;
}

void RenderAction::PrepareRenderFace(Face* face)
{
//	LOGE("Face in ");
	//如果不存在显示数据，或者该显示数据能够合并到其他的face中去，则返回
	if (!ISFaceHasDrawData(face))
	{
		face->SetRenderMeshData(NULL);
		return;
	}
	else
	{
		//如果能够进行面合并，合并成功后返回。不将面加入显示队列，如果合并不成功则表示当前面不能够进行合并，当前面应该加入显示队列
		if (Merge(face))
		{
			face->SetRenderMeshData(NULL);
			return;
		}
	}
	//this->AddFaceToDrawQueue(face);

//	if (dataType.IsContain(DrawableType::RGT_INTOP))
//	{
//		FacePrepareSelected(face);
//	}
//	else
//	{
//
//	}

}

void RenderAction::PrepareRenderEdges(Face* face)
{

}

void RenderAction::PrepareFaceMesh(Face* face)
{
	if (face->RecesiveShadow())
	{
		this->PushRenderable(face, RenderableType::RGT_SHADOW);
	}

	bool isTransparent = false;
	if (face->GetMaterial())
	{
		BaseMaterial* material = face->GetMaterial();
		if (material->GetMaterialType() == MaterialType_Phong|| material->GetMaterialType() == MaterialType_MatCap)
		{
			Material* temp = (Material*)material;
			if (temp->Opacity() < 1.0)
			{
				this->PushRenderable(face, RenderableType::RGT_TRANSPARENT);
				isTransparent = true;
			}
		}
		else if (material->GetMaterialType() == MaterialType_Pbr)
		{
			PbrMaterial* temp = (PbrMaterial*)material;
			if (temp->Opacity() < 1.0)
			{
				this->PushRenderable(face, RenderableType::RGT_TRANSPARENT);
				isTransparent = true;
			}
		}
	}
	else if (face->GetDrawColor()->IsTransparent())
	//			&& !face->IsSelected())
	{
		this->PushRenderable(face, RenderableType::RGT_TRANSPARENT);
		isTransparent = true;
	}


	if (!isTransparent)
	{
		this->PushRenderable(face, RenderableType::RGT_SOLID);
	}

	if (!Parameters::Instance()->m_IsShowBox && face->IsHightlight())
	{
		this->PushRenderable(face, RenderableType::RGT_INTOP);
	}
}


void RenderAction::FacePrepareLineMesh(Face* face)
{
	this->PushRenderable(face, RenderableType::RGT_TRILINE);
}

bool RenderAction::GetSceneBoxChanged() const
{
	return m_isSceneBoxChanged;
}

void RenderAction::FacePrepareEdgeLine(Face* face)
{
//	if (face->GetEdges().size() > 0)
//	{
//		Renderable drawData;
//		drawData.setRenderMatrix(this->GetWorldMatrixCache());
//		vector<Edge*>& edges = face->GetEdges();
//		drawData.setRenderEdgesData(&edges);
//		if (face->IsSelected())
//		{
//			drawData.SetRenderColor(*Color::SelectColor);
//		}
//		else
//		{
//			drawData.SetRenderColor(Color(0, 0, 0, 1));
//		}
//
//		bool isEdgeSelected = false;
//
//		//TODO 暂时屏选中边的绘制
////		for (int i = 0; i < edges.size(); i++)
////		{
////			//LOGE("Edge is face");
////			Edge* edge = edges[i];
////			if (edge->IsSelected())
////			{
////				drawData.PushbackSelEdge(edge);
////				isEdgeSelected = true;
////				break;
////			}
////		}
//
//		if (isEdgeSelected)
//		{
//			this->PushbackRenderPass(face,
//					RenderPassGroupType::RGTEDGELINEINTOP);
//		}
//
//		RenderType* renderType = this->GetRenderType();
//		RenderDataFilter& dataType = renderType->GetRenderDataFilter();
//
//		if (dataType.IsContain(RenderPassGroupType::RGTEDGELINE))
//		{
//			this->PushbackRenderPass(face, RenderPassGroupType::RGTEDGELINE);
//		}
//	}
}

void RenderAction::PrepareRenderInTop(Face* face)
{
	if (face->IsSelected())
	{
		this->PushRenderable(face, RenderableType::RGT_INTOP);
	}
}

LightList* RenderAction::GetLights()
{
	return &this->m_lights;
}

void RenderAction::AddLight(DirectionalLight* light)
{
	if(light)
	{
		m_lights.push_back(light);
	}
}

void RenderAction::SetLights(LightList* lights)
{
	m_lights = *lights;
}

LightList* RenderAction::GetModelLights()
{
	return &this->m_modellights;
}

void RenderAction::AddModelLight(DirectionalLight* light)
{
	if (light)
	{
		m_modellights.push_back(light);
	}
}

void RenderAction::SetModelLights(LightList* lights)
{
	m_modellights = *lights;
}

void RenderAction::ClearRenderBox()
{
	m_selectBox.Clear();
}

void RenderAction::MergeRenderBox(BoundingBox& mergeBox)
{
	if (mergeBox.Defined())
	{
		this->m_selectBox.Merge(mergeBox);
	}
}

BoundingBox& RenderAction::GetRenderBox()
{
	return m_selectBox;
}

void RenderAction::SetGroundNode(GroundNode* groundNode)
{
	m_groundNode = groundNode;
}

GroundNode* RenderAction::GetGroundNode()
{
	return m_groundNode;
}

void RenderAction::SetBackGroundNode(BackgroundNode* backgroundColor)
{
	this->m_backgroundColor = backgroundColor;
}

CullerMode& RenderAction::GetCullerMode()
{
	return this->m_CullerMode;
}

void RenderAction::SetCullerMode(const CullerMode& cullerMode)
{
	this->m_CullerMode = cullerMode;
}

void RenderAction::SetSection(Section* section)
{
	this->m_section = section;
	//如果为空，则关闭剖切操作
	if (!this->m_section)
	{
		for (int i = 0; i < 3; i++)
		{
			this->m_enableClip[i] = false;
		}
	}
}

Section* RenderAction::GetSection()
{
	return this->m_section;
}

void RenderAction::SetFPSNode(FPSNode* fpsNode)
{
	this->m_fpsNode = fpsNode;
}

void RenderAction::SetAxisNode(AxisNode* axisNode)
{
	this->m_axisNode = axisNode;
}

SceneGroundNode* RenderAction::GetSceneGroundNode()
{
	return m_sceneGroundNode;
}

void RenderAction::SetSceneGroundNode(SceneGroundNode* sceneGroundNode)
{
	m_sceneGroundNode = sceneGroundNode;
}

BackgroundNode* RenderAction::GetBackGroundNode()
{
	return this->m_backgroundColor;
}

FPSNode* RenderAction::GetFPSNode()
{
	return this->m_fpsNode;
}

int RenderAction::GetCurretSpecifyLod()
{
	return this->m_CurretSpecifyLod;
}

void RenderAction::SetCurrentSpecifyLod(int lod)
{
	this->m_CurretSpecifyLod = lod;
}

void RenderAction::SetFPS(int fps)
{
	this->m_fps = fps;
//	if(!m_sceneMoving)
//	{
//		return;
//	}
//
//	if(this->m_fps < this->m_minfps)
//	{
//		m_simplifyFactor+=5;
//	}
//	else
//	{
//		m_simplifyFactor-=5;
//	}
//
//	if(m_simplifyFactor>20)
//	{
//		m_simplifyFactor = 20;
//	}
//	else if(m_simplifyFactor < 0)
//	{
//		m_simplifyFactor = 0;
//	}
}

int RenderAction::GetFPS()
{
	return this->m_fps;
}

void RenderAction::SetMinFPS(int fps)
{
	this->m_minfps = fps;
}

int RenderAction::GetMinFPS()
{
	return this->m_minfps;
}


void RenderAction::SetNoteGroup(NoteGroup* noteGroup)
{
	this->m_noteGroupCache = noteGroup;
}


NoteGroup* RenderAction::GetNoteGroup()
{
	return this->m_noteGroupCache;
}

void RenderAction::SetMeasures(MeasureGroup* measureGroup)
{
	this->m_measureGroupCache = measureGroup;
}

MeasureGroup* RenderAction::GetMeaures()
{
	return this->m_measureGroupCache;
}

void RenderAction::SetRotateCenter(HandlerRotateCenter* rotateCenter)
{
	this->m_rotateCenter = rotateCenter;
}

HandlerRotateCenter* RenderAction::GetRotateCenter()
{
	return this->m_rotateCenter;
}

void RenderAction::SetScene(SceneManager* scene)
{
	this->m_scene = scene;
}

ShaderManager * RenderAction::GetShaderMananger()
{
	if(!m_shaderManager)
	{
		m_shaderManager = new ShaderManager;
		m_shaderManager->SetCurrentAction(this);
		m_shaderManager->Init();
	}
	return this->m_shaderManager;
}

void RenderAction::ReleaseShaderManager()
{
	if(m_shaderManager)
	{
		delete m_shaderManager;
	}
}


SceneManager* RenderAction::GetScene()
{
	return this->m_scene;
}

AxisNode* RenderAction::GetAxisNode()
{
	return this->m_axisNode;
}

void RenderAction::ClearGLResource()
{
	//m_frameBffer.ClearResource();
	//m_teethFBO.ClearResource();
	//m_delayDrawFBO.ClearResource();
	//m_depthMapFBO.ClearResource();
	//m_edgeDetectionFBO.ClearResource();

	//在关闭模型后，清除临时渲染队列
	this->m_delayDrawList.Clear();
	this->m_delayDrawOnceCount = 0;

	this->m_scene->GetRenderManager()->GetEffectManager()->ClearResource();
}

void RenderAction::ResizeFBOs(int width,int height)
{
	LOGI("Resize FBO Size");

	m_frameBffer.SetSize(width,height);
	m_teethFBO.SetSize(width,height);
	m_delayDrawFBO.SetSize(width,height);
	m_depthMapFBO.SetSize(width,height);
	m_edgeDetectionFBO.SetSize(width,height);

	m_mirrorFBO.SetSize(width, height);

	m_mirrorTexture = m_mirrorFBO.AttachTextureToColor();

	m_mirrorWidth = width;
    m_mirrorHeight = height;

	m_mirrorPixels = new GLubyte[m_mirrorWidth * m_mirrorHeight * 4];
	Texture2D texture;
	string mirrorBackgroundPath = SVIEW::Parameters::Instance()->m_appWorkPath + "\\data\\textures\\mirror\\" + string("mirrorBackground.jpg");
	m_mirrorBackgroundTexture = texture.LoadOGLTexture(mirrorBackgroundPath, 4, 0, 1);
	
	//m_mirrorPixels = new GLubyte[m_mirrorWidth * m_mirrorHeight * 4];

	//UI端视口大小改变了
	//m_frameBffer.DeatchFromPriFBO();
	//m_teethFBO.DeatchFromPriFBO();
	//m_delayDrawFBO.DeatchFromPriFBO();
	//m_depthMapFBO.DeatchFromPriFBO();
	//m_edgeDetectionFBO.DeatchFromPriFBO();
	this->m_scene->GetRenderManager()->GetEffectManager()->SetSize(width, height);
}

void RenderAction::SetSceneBoxChanged(bool val)
{
	m_isSceneBoxChanged = val;
}

M3D::HandlerGroup* RenderAction::GetHandlerGroupNode() const
{
	return m_handlerGroupNode;
}

void RenderAction::SetHandlerGroupNode(HandlerGroup* val)
{
	m_handlerGroupNode = val;
}

map<int, M3D::RenderQueue>* RenderAction::GetWorkingRenderQueueGroup() const
{
	return m_WorkingRenderQueueGroup;
}

void RenderAction::SetWorkingRenderQueueGroup(map<int, M3D::RenderQueue>* val)
{
	this->m_priTypeCache = 0;
	m_WorkingRenderQueueGroup = val;
}

void RenderQueue::Clear()
{
	m_renderableArray.clear();
}

void RenderQueue::Push(Renderable* state)
{
	m_renderableArray.push_back(state);
}

void RenderQueue::SetRenderableArray(RenderabelArray& statesQueue)
{
	m_renderableArray = statesQueue;
}

RenderabelArray& RenderQueue::GetRenderableArray()
{
	return m_renderableArray;
}

void RenderQueue::setType(RenderableType state)
{
	m_rednerType = state;
}

RenderableType RenderQueue::GetType()
{
	return m_rednerType;
}

RenderQueue::RenderQueue(RenderableType state)
{
	this->m_rednerType = state;
}

RenderQueue::RenderQueue()
{
	this->m_rednerType;
}

void RenderTech::SetRenderGroupType(RenderableType type)
{
	this->m_renderGroupType = type;
}

RenderableType RenderTech::GetRenderGroupType()
{
	return this->m_renderGroupType;
}

void RenderTech::EndEffect()
{
	if (this->IsColorMask())
	{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
}

void RenderTech::BeginEffect()
{
	if (this->IsColorMask())
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	}

}

bool RenderTech::IsColorMask()
{
	return this->m_bColorMask;
}

void RenderTech::SetColorMask(bool state)
{
	m_bColorMask = state;
}

RenderTech::RenderTech()
{
	m_bColorMask = false;
	m_DepthTest = true;
//m_renderQueueType =RGTSOLID;
}

void RenderQueuePriority::InitialSectionEffect()
{

}

void RenderQueuePriority::InitialNormalEffect()
{
	//控制渲染对象显示的优先级
	//阴影
	RenderTech* effectShadow = new RenderTech();
	effectShadow->SetRenderGroupType(RenderableType::RGT_SHADOW);
	RenderTech* effectEDGELINE = new RenderTech();
	effectEDGELINE->SetRenderGroupType(RenderableType::RGT_EDGELINE);
	//线
	RenderTech* effectTRILINE = new RenderTech();
	effectTRILINE->SetRenderGroupType(RenderableType::RGT_TRILINE);
	//实体
	RenderTech* effectSOLID = new RenderTech();
	effectSOLID->SetRenderGroupType(RenderableType::RGT_SOLID);
	//透明
	RenderTech* effectTRANSLATE = new RenderTech();
	effectTRANSLATE->SetRenderGroupType(RenderableType::RGT_TRANSPARENT);
	//实体最前端显示
	RenderTech* effectINTOP = new RenderTech();
	effectINTOP->SetRenderGroupType(RenderableType::RGT_INTOP);
	//边界线最前端显示
	RenderTech* effectEDGELINEINTOP = new RenderTech();
	effectEDGELINEINTOP->SetRenderGroupType(RenderableType::RGT_EDGELINEINTOP);
	//PMI最前端显示
	RenderTech* effectPMI = new RenderTech();
	effectPMI->SetRenderGroupType(RenderableType::RGT_PMI);
	//包围盒
	RenderTech* effectBox = new RenderTech();
	effectBox->SetRenderGroupType(RenderableType::RGT_BOX);
	//批注
	RenderTech* effectNote = new RenderTech();
	effectNote->SetRenderGroupType(RenderableType::RGT_NOTE);

	//批注
	RenderTech* effectAnnotation = new RenderTech();
	effectAnnotation->SetRenderGroupType(RenderableType::RGT_ANNOTATION);
	//测量
	RenderTech* effectMeasure = new RenderTech();
	effectMeasure->SetRenderGroupType(RenderableType::RGT_MEASURE);

	//Handler
	RenderTech* effectHandler = new RenderTech();
	effectHandler->SetRenderGroupType(RenderableType::RGT_HANDLER);

	//点对象
	RenderTech* effectPoint = new RenderTech();
	effectPoint->SetRenderGroupType(RenderableType::RGT_POINT);

	//根据push的顺序依次取队列数据进行渲染
	m_renderQueueTechs.push_back(effectShadow);
	m_renderQueueTechs.push_back(effectEDGELINE);
	m_renderQueueTechs.push_back(effectEDGELINEINTOP);
	m_renderQueueTechs.push_back(effectTRILINE);
	m_renderQueueTechs.push_back(effectSOLID);
	m_renderQueueTechs.push_back(effectPMI);
	m_renderQueueTechs.push_back(effectBox);
	m_renderQueueTechs.push_back(effectTRANSLATE);
	m_renderQueueTechs.push_back(effectPoint);
	m_renderQueueTechs.push_back(effectINTOP);
	m_renderQueueTechs.push_back(effectNote);
	m_renderQueueTechs.push_back(effectAnnotation);
	m_renderQueueTechs.push_back(effectMeasure);
	m_renderQueueTechs.push_back(effectHandler);
}

void RenderQueuePriority::InitialOpacityLineEffect()
{
	RenderTech* effectEDGELINE = new RenderTech();
	effectEDGELINE->SetRenderGroupType(RenderableType::RGT_EDGELINE);

	RenderTech* effectTRILINE = new RenderTech();
	effectTRILINE->SetRenderGroupType(RenderableType::RGT_TRILINE);

	RenderTech* effectSOLID = new RenderTech();
	effectSOLID->SetRenderGroupType(RenderableType::RGT_SOLID);
	effectSOLID->SetColorMask(true);

	RenderTech* effectTRANSLATE = new RenderTech();
	effectTRANSLATE->SetRenderGroupType(RenderableType::RGT_TRANSPARENT);
	effectSOLID->SetColorMask(true);

	RenderTech* effectINTOP = new RenderTech();
	effectINTOP->SetRenderGroupType(RenderableType::RGT_INTOP);
	effectSOLID->SetColorMask(true);

	m_renderQueueTechs.push_back(effectSOLID);
	m_renderQueueTechs.push_back(effectTRANSLATE);
	m_renderQueueTechs.push_back(effectINTOP);
	m_renderQueueTechs.push_back(effectEDGELINE);
	m_renderQueueTechs.push_back(effectTRILINE);

}

vector<RenderTech*>& RenderQueuePriority::GetData()
{
	return m_renderQueueTechs;
}

RenderQueuePriority::~RenderQueuePriority()
{
	for (int i = 0; i < m_renderQueueTechs.size(); i++)
	{
		delete m_renderQueueTechs.at(i);
	}
	m_renderQueueTechs.clear();
}

RenderQueuePriority::RenderQueuePriority()
{

}


}
