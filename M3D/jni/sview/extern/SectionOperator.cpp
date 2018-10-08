#include "sview/extern/SectionOperator.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/scene/GroupNode.h"
#include "sview/extern/WorkNodes.h"
#include "m3d/graphics/Section.h"

#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/model/Model.h"
#include "Utility.h"
#include <cmath>
#include "m3d/graphics/SectionPlane.h"
#include "m3d/graphics/SectionLineHelper.h"

#include "m3d/model/ModelView.h"
#include "m3d/model/PMIData.h"
#include "sview/views/Parameters.h"
#include "sview/views/View.h"

namespace M3D
{

SectionOperator* SectionOperator::Instance = NULL;

void SectionOperator::GetSectionInfo(SVIEW::View* view,int direction, float percentage,
		SectionPlane* outPlane)
{
	SceneManager* scene = view->GetSceneManager();

	BoundingBox& box = scene->GetSceneBox();
		float distance = 0;
		float position = 0;
		float clipPosition = 0;
		Vector3 min = box.m_min;
		float width = 0;
		float height = 0;

		//	sectionNode = GetSectionNode();
		float A = 0;
		float B = 0;
		float C = 0;
		float D = 0;

		//	LOGI("model:%s",model->name.c_str());
		//	LOGI("SectionAction::Handle:  direction:%d,length:%f,width:%f,height:%f,percentage:%f",
		//			direction,box->GetLength(),box->GetWidth(),box->GetHeight(),percentage);
		//	LOGI("boxMin:%f,%f,%f  boxMax:%f,%f,%f"
		//			,box->Min.x,box->Min.y,box->Min.z,box->Max.x,box->Max.y,box->Max.z);

		switch (direction)
		{
		case -Direction::X:
			distance = box.GetXLength() * percentage;
			position = min.m_x + distance;
			A = -1;
			D = position;
			break;
		case Direction::X:
			distance = box.GetXLength() * percentage;
			position = min.m_x + distance;
			A = 1;
			D = -position;
			break;
		case -Direction::Y:
			distance = box.GetYLength() * percentage;
			position = min.m_y + distance;
			B = -1;
			D = position;
			break;
		case Direction::Y:
			distance = box.GetYLength() * percentage;
			position = min.m_y + distance;
			B = 1;
			D = -position;

			break;
		case -Direction::Z:
			distance = box.GetZLength() * percentage;
			position = min.m_z + distance;
			C = -1;
			D = position;

			break;
		case Direction::Z:
			distance = box.GetZLength() * percentage;
			position = min.m_z + distance;
			C = 1;
			D = -position;

			break;
		}

	outPlane->SetPlaneParam(A, B, C, D);
}

void SectionOperator::Show(SVIEW::View* pView, int direction, float percentage,
		bool isShowClipPlane, bool isShowCutPlane)
{
	if (SectionOperator::Instance == NULL)
	{
		SectionOperator::Instance = new SectionOperator();
		LOGI("new SectionAction::Instance");
	}
//	LOGI("SectionOperator::Show step 1");
	SectionOperator::Instance->Init(pView, direction, percentage,
			isShowClipPlane, isShowCutPlane);
//	LOGI("SectionOperator::Show step 2");
	SceneManager* pSceneManager = pView->GetSceneManager();
	pSceneManager->Lock();
	SectionOperator::Instance->Handle();
	pSceneManager->UnLock();
//	LOGI("SectionOperator::Show step 3");
}

void SectionOperator::Clear(SVIEW::View* pView)
{
	SectionOperator* curAction = SectionOperator::Instance;
	SceneManager* pSceneManager = pView->GetSceneManager();
	if (SectionOperator::Instance != NULL)
	{
		pSceneManager->Lock();
        SectionOperator::Instance->m_Direction = 0;
        SectionOperator::Instance->m_fPercentage = 0.0;//
        SVIEW::Parameters::Instance()->m_showSection = false;
        pSceneManager->GetSection()->SetIsShowCappingPlane(false);
		pSceneManager->GetSection()->ClearPlanes();
		pSceneManager->UnLock();
	}

	pSceneManager->GetRenderManager()->RequestRedraw(); //1
}

void SectionOperator::Init(SVIEW::View* pView, int direction, float percentage,
		bool isShowClipPlane, bool isShowCutPlane)
{
	m_pView = pView;
	SceneManager* pSceneManager = pView->GetSceneManager();
	//获得最顶级Model
	m_Model = (Model*) pSceneManager->GetModel();

	this->m_pSceneManager = pSceneManager;
	this->m_Direction = direction;
	this->m_fPercentage = percentage / 100;

	//if percentage == 100
	if (fabs(percentage - 100) < 0.01)
		this->m_fPercentage = 1.01;

	if (fabs(percentage) < 0.01)
		this->m_fPercentage = -0.01;

	this->m_IsShowClipPlane = isShowClipPlane;
	this->m_IsShowCutPlane = isShowCutPlane;

    
	int sectionPlaneId = 1001;
	if (m_pCurSectionPlane == NULL)
	{
		m_pCurSectionPlane = new SectionPlane();
	}
	m_pCurSectionPlane->SetID(sectionPlaneId);

}

void SectionOperator::Handle()
{
	//	LOGI("SectionAction::Handle() begin");
	//获得剖面
	CreateSectionPlane(m_Model, m_Direction, m_fPercentage, m_pCurSectionPlane);

	//添加进model 的剖面列表
//	if (m_Model->GetSectionPlane(m_pCurSectionPlane->GetID()) == NULL)
//	{
//		m_Model->AddSectionPlane(m_pCurSectionPlane);
//	}

	//加入到当前视图中
//	m_CurModelView->AddSectionPlaneId(m_pCurSectionPlane->GetID());

	//添加视图到model的视图列表
//	int periodId = m_CurModelView->GetID();
//	ModelView* periodView = m_Model->GetModleView(periodId);
//	if (NULL == periodView)
//		m_Model->AddModelView(m_CurModelView);

	//-----------------test clipPlane2-------------------------
	//	Plane* plane2 = createSectionPlane(model,(direction+1)%3,percentage);
	//	plane2->setID(sectionPlaneId+1);
	//	model->addSectionPlane(curSectionPlane);
	//	curModelView->addSectionPlaneId(curSectionPlane->getID());
	//------------------------------------------

#ifdef NOCLOTH
	m_pCurSectionPlane->ClearSectionLineDataBse();
	void * data = &(m_pCurSectionPlane->m_scetionLinesDataBase); ///获取数据
	SectionLineHelper::DoSectionLines(data,m_pSceneManager);///经此函数，剖切面中将存储带有数据的sectionline对象
	SectionLineHelper::Triangulation(m_pCurSectionPlane);

	LOGE("m_pCurSectionPlane = %p",m_pCurSectionPlane);
#endif

#ifdef CLOTH
	m_pCurSectionPlane->ClearFigureSectionLines();
	void * data = &(m_pCurSectionPlane->m_figureSectonLine); ///获取数据
	SectionLineHelper::DoFigureSectionLine(data,m_pSceneManager);///经此函数，剖切面中将存储带有数据的sectionline对象
#endif
	m_pView->GetSceneManager()->GetSection()->AddPlane(m_pCurSectionPlane);

//	m_pView->GetSceneManager()->setSectionEnable(true);
//	LOGI("new SectionAction::Instance  step 3");
	//请求绘制
	m_pView->RequestDraw();
	//	LOGI("SectionAction::Handle() end");
}

void SectionOperator::CreateSectionPlane(Model* model, int direction,
		float percentage, SectionPlane* outPlane)
{
	GetSectionInfo(m_pView,direction, percentage,
			outPlane);

	//float length = box.Length()/2;

	//Vector2 size(length,length);

	//outPlane->SetDrawPlane(center,size);
	outPlane->SetEnable(true);

	//是否显示剖视平面
	outPlane->SetShowPlaneRect(m_IsShowClipPlane);

	//是否显示截面
	outPlane->SetShowCutPlane(m_IsShowCutPlane);
}

void SectionOperator::RequestDraw()
{

}
}
