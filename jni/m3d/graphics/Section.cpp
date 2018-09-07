#include "m3d/graphics/SectionPlane.h"
#include "m3d/graphics/Section.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Quaternion.h"
#include "m3d/base/Plane.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/action/RenderAction.h"
namespace M3D
{

Section::Section(void) :
		Shape(),m_isShowCappingPlane(false)
{
	///默认关闭盖面
//	this->SetVisible(false);
	m_planeList.resize(0);
}

Section::~Section(void)
{

}

list<SectionPlane*>* Section::GetPlaneList()
{
	return &m_planeList;
}

void Section::SetTransform(Matrix3x4 & transform)
{
	list<SectionPlane*>::iterator it;
	for (it = m_planeList.begin(); it != m_planeList.end(); it++) 
	{
		(*it)->SetTransform(transform);
	}
}

//void SectionNode::SetModel(Model* model)
//{
//	m_Model = model;
//}

//void SectionNode::SetEnable(bool flag)
//{
//	m_Enable = flag;
//}
//
//bool SectionNode::GetEnable()
//{
//	return m_Enable;
//}

SHAPETYPE Section::GetType(void)
{
	return (SHAPETYPE)SECTION_NODE;
}

void Section::FindVisiableObject(RenderAction* renderAction)
{
	if (renderAction)
	{
		MutexLock lock(m_mutex);
		list<SectionPlane*>::iterator iter;
		for (iter = m_planeList.begin(); iter != m_planeList.end(); ++iter)
		{

			SectionPlane* iPlane = *iter;
			iPlane->SetScene(renderAction->GetScene());
		}

		if (this->m_planeList.size() > 0)
		{
			//添加Section数据到显示队列中
			renderAction->PrepareRenderSection(this);
		}
		else
		{
			renderAction->PrepareRenderSection(NULL);
		}
	}
}

bool Section::AddPlane(SectionPlane* plane)
{
	MutexLock lock(m_mutex);
	bool addState = true;

	if(!plane)
	{
		return false;
	}
	Color green(0.50f,0.50f,0.0f,0.1f);
	Color edgeColor(0.7f,0.7f,0.70f,1.0f);

	list<SectionPlane*>::iterator iter;

	for(iter = m_planeList.begin();iter != m_planeList.end(); ++iter)
	{

		SectionPlane* iPlane = *iter;
		if(iPlane->GetID() == plane->GetID())
		{
			//TOOD  这里仅仅通过拖拽器来更新剖切平面，不在通过外部来更新 
		 	iPlane->Copy(plane); 

			//TOOD XULI
			Matrix3x4 transMatrix = Matrix3x4::IDENTITY;
			iPlane->SetTransform(transMatrix);
//		//	TODO
			float* equation = plane->GetTransformPlaneParam();
//
			Vector3 normal(equation[0],equation[1],equation[2]);
////		    LOGI("normal = %s",normal.Normalized().Tostring().c_str());
			Plane projectPlane(normal,equation[3]);
////			LOGI("equation[3] = %f",equation[3]);

			BoundingBox box = m_drawBoxSize.Projected(projectPlane);
//
////		    Vector3 projMin = box.min_;
////		    Vector3 projMax = box.max_;
////
////		    LOGI("projMin = %s",projMin.Tostring().c_str());
////		    LOGI("projMax = %s",projMax.Tostring().c_str());
//
//			//SECTION TODO
			iPlane->SetDrawPlane(box);
			iPlane->SetSceneBox(m_drawBoxSize);
			//
			iPlane->SetFaceColor(green);
			iPlane->SetEdgeColor(edgeColor);
			iPlane->UpdateDrawData();
			return false;
		}
	}

	if(this->m_planeList.size()>=3)
	{
		delete *(this->m_planeList.begin());
		this->m_planeList.erase(this->m_planeList.begin());
	}

	SectionPlane *newplane = new SectionPlane();
	 
	newplane->Copy(plane);
	newplane->SetSceneBox(m_drawBoxSize);
	newplane->UpdateDrawData();
	newplane->SetFaceColor(green);
	newplane->SetEdgeColor(edgeColor);
	////TODO

	float* equation = plane->GetEquation();
	Vector3 normal(equation[0], equation[1], equation[2]);
	Plane projectPlane(normal, equation[3]);
	BoundingBox box = m_drawBoxSize.Projected(projectPlane);
	////SECTION TODO
	newplane->SetDrawPlane(box);
	newplane->SetFaceColor(green);
	newplane->SetEdgeColor(edgeColor);
	Matrix3x4 transMatrix = Matrix3x4::IDENTITY;
	newplane->SetTransform(transMatrix);
	this->m_planeList.push_back(newplane);

	return addState;
}

bool  Section::RemovePlane(SectionPlane* plane)
{
	MutexLock lock(m_mutex);
	bool removeState = false;

	list<SectionPlane*>::iterator iter;
	for(iter = m_planeList.begin();iter != m_planeList.end(); ++iter)
	{
		SectionPlane* iPlane = *iter;
		if(iPlane->GetID() == plane->GetID())
		{
			delete iPlane;
			m_planeList.erase(iter);

			return true;
		}
	}

	return removeState;
}


void Section::ClearPlanes()
{
	MutexLock lock(m_mutex);

	list<SectionPlane*>::iterator iter;

	for(iter = m_planeList.begin();iter != m_planeList.end(); ++iter)
	{
		SectionPlane* iPlane = *iter;

		delete iPlane;
	}

	this->m_planeList.clear();
}

void  Section::SetDrawRection(const BoundingBox& box)
{
	m_drawBoxSize = box;
}

void Section::SetIsShowCappingPlane(bool state)
{
	this->m_isShowCappingPlane = state;
 
	list<SectionPlane*>* planeList = this->GetPlaneList();
	list<SectionPlane*>::iterator it;
	for (it = planeList->begin(); it != planeList->end(); it++)
	{
		SectionPlane* iPlane = *it;
		iPlane->SetShowCappingPlane(m_isShowCappingPlane);
	}
}

bool Section::IsShowCappingPlane()
{
	return this->m_isShowCappingPlane;
}

void Section::SetIsReverseClipping(bool state)
{
	this->m_isReverseClipping = state;
}

bool Section::IsReverseClipping()
{
	return this->m_isReverseClipping;
}

void Section::SetShowClipPlane(bool val)
{
	m_IsShowClipPlane = val;
 
	list<SectionPlane*>* planeList = this->GetPlaneList();
	list<SectionPlane*>::iterator it;
	for (it = planeList->begin(); it != planeList->end(); it++)
	{
		SectionPlane* iPlane = *it;
		iPlane->SetShowClipPlane(m_IsShowClipPlane);
	}
}

bool Section::GetShowPlaneRect() const
{
	return m_ShowPlaneRect;
}

bool Section::GetShowClipPlane() const
{
	return m_IsShowClipPlane;
}

void Section::SetShowPlaneRect(bool val)
{
	m_ShowPlaneRect = val;
 
	list<SectionPlane*>* planeList = this->GetPlaneList();
	list<SectionPlane*>::iterator it;
	for (it = planeList->begin(); it != planeList->end(); it++)
	{
		SectionPlane* iPlane = *it;
		iPlane->SetShowPlaneRect(m_ShowPlaneRect);
	}
}

SectionPlane* Section::GetPlaneById(int sectionPlaneId)
{
	list<SectionPlane*>* planeList = this->GetPlaneList();
	list<SectionPlane*>::iterator it;
	for (it = planeList->begin(); it != planeList->end(); it++)
	{
		int planeId = (*it)->GetID();
		if (planeId == sectionPlaneId)
		{
			return (*it);
		}
	}
	return NULL;
}

}

