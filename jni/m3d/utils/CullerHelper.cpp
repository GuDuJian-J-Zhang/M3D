#include "m3d/M3D.h"
#include "m3d/utils/CullerHelper.h"
#include "m3d/graphics/CameraNode.h"
#include "sview/views/Parameters.h"

using namespace SVIEW;

namespace M3D
{
CullerHelper::CullerHelper(void)
{
	m_modelLength = 0;
	m_priCameraScale = -1;
	m_drawLimit = 0;
	m_allowCuller = false;
	m_cullerbase = 0;
	m_cullerPercent = 0;
	m_drawLLLimit  = 0;
}

CullerHelper::~CullerHelper()
{

}

void CullerHelper::SetModelLength(float length)
{
	CullerHelper::m_modelLength = length;
}

int CullerHelper::IsLittleModel(const BoundingBox& box,
		CameraNode* camera)
{
	int littleModelState = 0;

	float length = box.Length();
	if(!camera->IsOrthographic())
	{
		float dis = (camera->GetWorldPosition() - box.Center()).Length();
		length = length*1000/dis;
	}

	if(box.Defined())///包围盒定义了，才需要进行判断，如果包围盒没有定义，则认为该模型时需要显示的
	{
		if(length<m_drawLLLimit)
		{
			littleModelState = 2;
		}else if (length < m_drawLimit)
		{
			littleModelState = 1;
		}
	}
	return littleModelState;
}

int CullerHelper::IsLittleBody(const BoundingBox& box,
	CameraNode* camera)
{
	int littleModelState = 0;

	float length = box.Length();
 
	if (box.Defined())///包围盒定义了，才需要进行判断，如果包围盒没有定义，则认为该模型时需要显示的
	{
		if (length < m_drawLLLimit)
		{
			littleModelState = 2;
		}
		else if (length < m_drawLimit)
		{
			littleModelState = 1;
		}
	}
	return littleModelState;
}


Intersection CullerHelper::InViewPort(BoundingBox& box,
		CameraNode* camera)
{
	Intersection intersection = INTERSECTS;
	if(camera)
	{
		const Frustum& cameraFrustum = camera->GetFrustum();
		intersection= cameraFrustum.IsInsideFast(box);
	}
	return intersection;
}

bool CullerHelper::isCameraSacle(CameraNode* camera)
{
	float scale = camera->GetZoom();
	//LOGE("scale  is %f ",scale);

	if (abs(scale - m_priCameraScale) > 0.0001)
	{
		//LOGE("priCameraScale  is %f ",scale);
		m_priCameraScale = scale;
		return true;
	}
	return true;
}
void CullerHelper::AllowCuller(CameraNode* camera, bool allowCuller)
{
	m_allowCuller = allowCuller;
	UpDate(camera);
}

float CullerHelper::GetCullerPercent()
{
	return this->m_cullerPercent;
}

void CullerHelper::AddCullerRatio(float fps)
{
	return;
	if (fps < 2)
	{
		this->m_cullerPercent += 0.1;
		this->m_cullerbase += 10;
	}
	else if (fps < 5)
	{
		this->m_cullerPercent += 0.1;
		this->m_cullerbase += 5;
	}
	else if (fps < 8)
	{
		this->m_cullerPercent += 0.1;
		this->m_cullerbase += 2;
	}

	if (this->m_cullerPercent >0.99) {
		this->m_cullerPercent = 0.99;
	}
	if (this->m_cullerbase >20)
	{
		this->m_cullerbase = 20;
	}
	this->m_cullerPercent = .0f;
	//this->m_cullerbase=0;
}

void CullerHelper::ReduceCullerRatio(float fps)
{
	return;
	if (fps >20)
	{
		this->m_cullerPercent  -= 0.02;
		this->m_cullerbase -= 5;
	}
	else if (fps > 15)
	{
		this->m_cullerPercent-=0.01;
		this->m_cullerbase -= 1;
	}

	if (this->m_cullerPercent <0) {
		this->m_cullerPercent = 0;
	}

	if (this->m_cullerbase < 20)
	{
		this->m_cullerbase = 20;
	}
}
void CullerHelper::UpDate(CameraNode* camera)
{
//bool isCameraScale = isCameraSacle(camera);
//
//	if (/*!isCameraScale ||*/ !m_allowCuller)
//	{
//		m_drawLimit = 0;
//		return;
//	}
	const Viewport& viewport = camera->GetViewPort();
	this->m_cullerbase = Parameters::Instance()->m_RemoveSize;
	float removeSize = this->m_cullerbase;// Parameters::Instance()->m_RemoveSize;
	float removeNeverSeeSize  = this->m_cullerbase/4 ;
	if (Parameters::Instance()->m_RemoveMode == 0)
	{
		m_drawLimit = m_modelLength * removeSize/ 100;
		m_drawLLLimit = 0;
	}
	else if (Parameters::Instance()->m_RemoveMode == 1)
	{
		Vector3 worldPnt0 = viewport.ScreenToWorldPoint(0,0,0.5);
		Vector3 worldPnt1 = viewport.ScreenToWorldPoint(0,removeSize,0.5);
		m_drawLimit = (worldPnt0 -worldPnt1).Length();

		Vector3 worldPnt2 = viewport.ScreenToWorldPoint(0,removeNeverSeeSize,0.5);
		m_drawLLLimit =  (worldPnt0 -worldPnt2).Length();	
	}
}
void CullerHelper::Reset()
{
	m_drawLimit = 0;
	m_drawLLLimit = 0;
}
}

