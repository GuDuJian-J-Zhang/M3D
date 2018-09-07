/*
 * AssembleManager.cpp
 *
 *  Created on: 2015-12-24
 *      Author: CDJ
 */

#include "m3d/Section/SectionManager.h"
#include "m3d/handler/DraggerManager.h"
#include "../graphics/SectionPlane.h"
#include "sview/views/View.h"
#include "../SceneManager.h"
#include "../graphics/Section.h"
#include "m3d/scene/SectionNode.h"
#include "../handler/dragger.h"
#include "../handler/TranslateAxisDragger.h"
#include "../extras/modelmanager/DraggerCallbacks.h"
#include "../handler/RotateCylinderAxisDragger.h"
#include "m3d/base/Vector3.h"
#include "../handler/TranslateMinusAxisDragger.h"
using namespace M3D;
using namespace SVIEW;
namespace M3D
{

	SectionManager::SectionManager(SVIEW::View * view)
	{
		this->m_view = view;
		_enableXY = true;
		_enableXZ = true;
		_enableYZ = true;
		_ShowPlaneRect = true;
		_ShowClipPlane = true;
		_ShowCappingPlane = true;

		_xyPlaneID = 1001;
		_xzPlaneID = 1002;
		_yzPlaneID = 1003;
		bindAxisDragger = 0;
		bindRotationDragger = 0;
		_firstOpen = true;
	}

	SectionManager::~SectionManager()
	{

	}

	void SectionManager::OpenThreePlaneSection()
	{
		SectionPlane* outPlane = new SectionPlane();
		outPlane->AddRef();
		outPlane->SetID(_xyPlaneID);
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		pSection->AddPlane(outPlane);
		outPlane->Release();

		outPlane = new SectionPlane();
		outPlane->AddRef();
		outPlane->SetID(_yzPlaneID);

		pSection->AddPlane(outPlane);
		outPlane->Release();

		outPlane = new SectionPlane();
		outPlane->AddRef();
		outPlane->SetID(_xzPlaneID);
		pSection->AddPlane(outPlane);
		outPlane->Release();

		this->ResetThreePlaneSection();
	}

	SectionPlane* SectionManager::CreateOneSectionPlane(const Vector3 &normalVector, const Vector3 &point, int planeID)
	{
		SectionPlane* outPlane = new SectionPlane();
		outPlane->AddRef();
		outPlane->SetID(planeID);
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		//Section* pSection = m_view->GetSceneManager()->CreateSingleSectionNode(planeID)->GetSection();

		list<SectionPlane*> planeList = pSection->GetSectionPlaneList();
		if (planeList.size() > 0)
		{
			list<SectionPlane*>::iterator iter;
			for (iter = planeList.begin(); iter != planeList.end(); ++iter)
			{
				SectionPlane* iPlane = *iter;
				if (iPlane->GetEnable())
				{
					iPlane->SetEnable(false);
					SetDraggerVisiable(iPlane->GetID(), false);
				}
			}
		}

		pSection->AddPlane(outPlane);
		outPlane->Release();

		SectionPlane* onePlane = pSection->GetPlaneById(planeID);
		float D = -(normalVector.DotProduct(point));
		bool val = true;
		if (onePlane)
		{
			onePlane->SetPlaneParam(normalVector.m_x, normalVector.m_y, normalVector.m_z, D);
			onePlane->SetEnable(val);
			BindOneSectionDragger(normalVector, onePlane);
		}
		return onePlane;
	}

	void SectionManager::BindOneSectionDragger(const Vector3 &normalVector, SectionPlane* plane)
	{
		m_view->GetSceneManager()->Lock();
		TranslateMinusAxisDragger* translateAixsDragger = NULL;
		SceneManager* scene = m_view->GetSceneManager();
		Vector3 position = scene->GetSceneBox().Center();
		if (m_view)
		{
			translateAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetSingleTransMinusformHandler();
			translateAixsDragger->SetName("OneSectionAxisDragger");
			translateAixsDragger->SetVisible(true);
			translateAixsDragger->SetNeedScale(true);
			translateAixsDragger->SetWorldPosition(plane->GetCenterPointArray());
			translateAixsDragger->SetOrientation(normalVector);
			translateAixsDragger->SetID(plane->GetID());
			translateAixsDragger->SetMinusXAxisVisible(false);
			draggerList.push_back(translateAixsDragger);
			SceneNodeDraggerCallback* draggerCallback = new SceneNodeDraggerCallback();
			//SectionNode* scetionNode = scene->GetSectionNode(plane->GetID());
			//SectionNode* scetionNode = scene->GetSectionNode();

			//scetionNode->Reset();
			draggerCallback->SetSectionPlane(plane);
			draggerCallback->AddRef();
			translateAixsDragger->addDraggerCallback(draggerCallback);
			draggerCallback->Release();
			//scetionNode->MarkDirty();
		}
		translateAixsDragger->SetVisible(true);
		m_view->GetSceneManager()->UnLock();
	}

	bool SectionManager::SetDraggerVisiable(int id, bool val)
	{
		TranslateMinusAxisDragger* dragger = GetTranslateDragger(id);
		if (dragger)
		{
			dragger->SetVisible(val);
			return true;
		}
		return false;
	}

	bool SectionManager::UnBindOneDragger(Dragger * dragger)
	{
		if (dragger)
		{
			return m_view->GetDraggerManager()->UnBindDragger(dragger);
		}
		return false;
	}

	TranslateMinusAxisDragger * SectionManager::GetTranslateDragger(int id)
	{
		list<TranslateMinusAxisDragger*>::iterator iter;
		for (iter = draggerList.begin(); iter != draggerList.end(); ++iter)
		{
			TranslateMinusAxisDragger* dragger = *iter;
			if (dragger->GetID() == id)
			{
				return dragger;
			}
		}
		return nullptr;
	}

	M3D::Dragger* SectionManager::BindSectionDragger(int draggerType)
	{
		m_view->GetSceneManager()->Lock();
		Dragger* dragger = NULL;
		switch (draggerType)
		{
		case 1:
			dragger = BindSectionAxisDragger();
			break;
		case 2:
			dragger = BindSectionRotateDragger();
			break;

		default:
			break;
		}	
		dragger->SetVisible(true);
		m_view->GetSceneManager()->UnLock();
		return dragger;
	}

	M3D::Dragger* SectionManager::InitBindSectionDragger(int draggerType)
	{
		m_view->GetSceneManager()->Lock();
		Dragger* dragger = NULL;
		switch (draggerType)
		{
		case 1:
			dragger = InitBindSectionAxisDragger();
			break;
		case 2:
			dragger = InitBindSectionRotateDragger();
			break;

		default:
			break;
		}
		dragger->SetVisible(true);
		m_view->GetSceneManager()->UnLock();
		return dragger;
	}

	M3D::Dragger* SectionManager::BindSectionAxisDragger()
	{
		TranslateAxisDragger* translateAixsDragger = NULL;
		SceneManager* scene = m_view->GetSceneManager();
		SectionNode* scetionNode = scene->GetSectionNode();
		Vector3 position = scetionNode->GetWorldTransform().Translation();
		if (m_view)
		{
			translateAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetTransformHandler();
			translateAixsDragger->SetName("SectionAxisDragger");
			translateAixsDragger->SetVisible(true);
			//translateAixsDragger->SetOrigScale(Vector3(scale,scale,scale));
			translateAixsDragger->SetNeedScale(true);
			translateAixsDragger->SetWorldPosition(position);
			SceneNodeDraggerCallback* draggerCallback = new SceneNodeDraggerCallback();
			SectionNode* scetionNode = scene->GetSectionNode();
			draggerCallback->AddNode(scetionNode);
			draggerCallback->AddRef();
			translateAixsDragger->addDraggerCallback(draggerCallback);
			draggerCallback->Release();
		}
		return translateAixsDragger;
	}

	M3D::Dragger* SectionManager::BindSectionRotateDragger()
	{
		RotateCylinderAxisDragger* translateAixsDragger = NULL;
		SceneManager* scene = m_view->GetSceneManager(); 
		SectionNode* scetionNode = scene->GetSectionNode();
		Vector3 position = scetionNode->GetWorldTransform().Translation();
		if (m_view)
		{
			translateAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetRotateCylinderAxisDragger();
			translateAixsDragger->SetName("SectionRotateDragger");
			translateAixsDragger->SetVisible(true);
			translateAixsDragger->SetNeedScale(true);
			//translateAixsDragger->SetOrigScale(Vector3(scale, scale, scale));
			translateAixsDragger->SetWorldPosition(position);
			SceneNodeDraggerCallback* draggerCallback = new SceneNodeDraggerCallback();
			SectionNode* scetionNode = scene->GetSectionNode();
			draggerCallback->AddNode(scetionNode);
			draggerCallback->AddRef();
			translateAixsDragger->addDraggerCallback(draggerCallback);
			draggerCallback->Release();
		}
		return translateAixsDragger;
	}

	M3D::Dragger* SectionManager::InitBindSectionAxisDragger()
	{
		TranslateAxisDragger* translateAixsDragger = NULL;
		SceneManager* scene = m_view->GetSceneManager();
		Vector3 position = scene->GetSceneBox().Center();
		if (m_view)
		{
			translateAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetTransformHandler();
			translateAixsDragger->SetName("SectionAxisDragger");
			translateAixsDragger->SetVisible(true);
			//translateAixsDragger->SetOrigScale(Vector3(scale,scale,scale));
			translateAixsDragger->SetNeedScale(true);
			translateAixsDragger->SetWorldPosition(position);
			SceneNodeDraggerCallback* draggerCallback = new SceneNodeDraggerCallback();
			SectionNode* scetionNode = scene->GetSectionNode();
			scetionNode->Reset();
			draggerCallback->AddNode(scetionNode);
			draggerCallback->AddRef();
			translateAixsDragger->addDraggerCallback(draggerCallback);
			draggerCallback->Release();
			scetionNode->MarkDirty();
		}
		return translateAixsDragger;
	}

	M3D::Dragger* SectionManager::InitBindSectionRotateDragger()
	{
		RotateCylinderAxisDragger* translateAixsDragger = NULL;
		SceneManager* scene = m_view->GetSceneManager();
		Vector3 position = scene->GetSceneBox().Center();
		if (m_view)
		{
			translateAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetRotateCylinderAxisDragger();
			translateAixsDragger->SetName("SectionRotateDragger");
			translateAixsDragger->SetVisible(true);
			translateAixsDragger->SetNeedScale(true);
			//translateAixsDragger->SetOrigScale(Vector3(scale, scale, scale));
			translateAixsDragger->SetWorldPosition(position);
			SceneNodeDraggerCallback* draggerCallback = new SceneNodeDraggerCallback();
			SectionNode* scetionNode = scene->GetSectionNode();
			scetionNode->Reset();
			draggerCallback->AddNode(scetionNode);
			draggerCallback->AddRef();
			translateAixsDragger->addDraggerCallback(draggerCallback);
			draggerCallback->Release();
			scetionNode->MarkDirty();
		}
		return translateAixsDragger;
	}

	void SectionManager::ShowAxisDragger()
	{
		this->UnBindAllDragger();
		if (this->_firstOpen)
		{
			bindAxisDragger = this->InitBindSectionDragger(1);
			this->_firstOpen = false;
		}
		else
		{
			bindAxisDragger = this->BindSectionDragger(1);
		}
	}

	void SectionManager::ShowRotateDragger()
	{
		this->UnBindAllDragger();
		if (this->_firstOpen)
		{
			bindRotationDragger = this->InitBindSectionDragger(2);
		}
		else
		{
			bindRotationDragger = this->BindSectionDragger(2);
		}
	}

	void SectionManager::ResetThreePlaneSection()
	{
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		SectionPlane* xyPlane = pSection->GetPlaneById(_xyPlaneID);
		SectionPlane* yzPlane = pSection->GetPlaneById(_yzPlaneID);
		SectionPlane* xzPlane = pSection->GetPlaneById(_xzPlaneID);
		TranslateAxisDragger* translateAixsDragger = dynamic_cast<TranslateAxisDragger*>(this->bindAxisDragger);
		bool val = true;
		if (xyPlane)
		{
			GetSectionInfo(m_view, 3, 0.50f, xyPlane);
			xyPlane->SetEnable(val);
			if (translateAixsDragger)
			{
				translateAixsDragger->SetZAxisVisible(val);
				translateAixsDragger->SetXYPlaneVisible(val);
			}

		}

		if (yzPlane)
		{
			GetSectionInfo(m_view, 1, 0.50f, yzPlane);
			yzPlane->SetEnable(val);
			if (translateAixsDragger)
			{

				translateAixsDragger->SetXAxisVisible(val);
				translateAixsDragger->SetYZPlaneVisible(val);
			}
		}

		if (xzPlane)
		{
			GetSectionInfo(m_view, 2, 0.50f, xzPlane);
			xzPlane->SetEnable(val);
			if (translateAixsDragger)
			{

				translateAixsDragger->SetYAxisVisible(val);
				translateAixsDragger->SetXZPlaneVisible(val);
			}
		}
	}

	void SectionManager::CloseAllSectionPlanes()
	{
		this->UnBindAllDragger();
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		pSection->ClearPlanes();

		_firstOpen = true;
	}

	void SectionManager::GetSectionInfo(SVIEW::View* view, int direction, float percentage, SectionPlane* outPlane)
	{
		SceneManager* scene = view->GetSceneManager();

		BoundingBox& box = scene->GetSceneBox();
		float distance = 0;
		float position = 0;
		float clipPosition = 0;
		Vector3 min = box.m_min;
		float width = 0;
		float height = 0;

		float A = 0;
		float B = 0;
		float C = 0;
		float D = 0;

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

	bool SectionManager::GetEnableXY() const
	{
		return _enableXY;
	}

	void SectionManager::SetEnableXY(bool val)
	{
		_enableXY = val;
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		SectionPlane* sectionPlane = pSection->GetPlaneById(_xyPlaneID);
		if (sectionPlane)
		{
			sectionPlane->SetEnable(val);
			if (bindAxisDragger)
			{
				TranslateAxisDragger* translateAixsDragger = dynamic_cast<TranslateAxisDragger*>(this->bindAxisDragger);
				translateAixsDragger->SetZAxisVisible(val);
				SetDraggerPlaneVisible();

			}
		}
	}

	bool SectionManager::GetEnableXZ() const
	{
		return _enableXZ;
	}

	void SectionManager::SetEnableXZ(bool val)
	{
		_enableXZ = val;
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		SectionPlane* sectionPlane = pSection->GetPlaneById(_xzPlaneID);
		if (sectionPlane)
		{
			sectionPlane->SetEnable(val);
			if (bindAxisDragger)
			{
				TranslateAxisDragger* translateAixsDragger = dynamic_cast<TranslateAxisDragger*>(this->bindAxisDragger);
				translateAixsDragger->SetYAxisVisible(val);
				SetDraggerPlaneVisible();
			}
		}
	}

	void SectionManager::SetDraggerPlaneVisible()
	{
		if (bindAxisDragger)
		{
			TranslateAxisDragger* translateAixsDragger = dynamic_cast<TranslateAxisDragger*>(this->bindAxisDragger);
			bool val = true;
			translateAixsDragger->SetXYPlaneVisible(val);
			translateAixsDragger->SetXZPlaneVisible(val);
			translateAixsDragger->SetYZPlaneVisible(val);
			val = false;
			if (!_enableXY)
			{
				translateAixsDragger->SetXZPlaneVisible(val);
				translateAixsDragger->SetYZPlaneVisible(val);
			}
			if (!_enableXZ)
			{
				translateAixsDragger->SetXYPlaneVisible(val);
				translateAixsDragger->SetYZPlaneVisible(val);
			}
			if (!_enableYZ)
			{
				translateAixsDragger->SetXYPlaneVisible(val);
				translateAixsDragger->SetXZPlaneVisible(val);
			}
		}
	}

	SectionPlane * SectionManager::GetPlaneById(int sectionPlaneId)
	{
		SectionPlane* sectionPlane = nullptr;
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		sectionPlane = pSection->GetPlaneById(sectionPlaneId);
		return sectionPlane;
	}

	list<SectionPlane*> SectionManager::GetPlaneList()
	{
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		return pSection->GetSectionPlaneList();
	}

	bool SectionManager::AddPlane(SectionPlane * plane)
	{
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		return pSection->AddPlane(plane);
	}

	bool SectionManager::RemovePlane(SectionPlane * plane)
	{
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		TranslateMinusAxisDragger* dragger = GetTranslateDragger(plane->GetID());
		if (dragger)
		{
			UnBindOneDragger(dragger);
		}
		return pSection->RemovePlane(plane);
	}

	bool SectionManager::GetEnableYZ() const
	{
		return _enableYZ;
	}

	void SectionManager::SetEnableYZ(bool val)
	{
		_enableYZ = val;
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		SectionPlane* sectionPlane = pSection->GetPlaneById(_yzPlaneID);
		if (sectionPlane)
		{
			sectionPlane->SetEnable(val);
			if (bindAxisDragger)
			{
				TranslateAxisDragger* translateAixsDragger = dynamic_cast<TranslateAxisDragger*>(this->bindAxisDragger);

				translateAixsDragger->SetXAxisVisible(val);
				SetDraggerPlaneVisible();

			}

		}
	}

	bool SectionManager::GetShowPlaneRect() const
	{
		return _ShowPlaneRect;
	}

	void SectionManager::SetShowPlaneRect(bool val)
	{
		_ShowPlaneRect = val;
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		pSection->SetShowPlaneRect(val);
	}

	bool SectionManager::GetShowClipPlane() const
	{
		return _ShowClipPlane;
	}

	void SectionManager::SetShowClipPlane(bool val)
	{
		_ShowClipPlane = val;
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		pSection->SetShowClipPlane(val);
	}

	bool SectionManager::GetShowCappingPlane() const
	{
		return _ShowCappingPlane;
	}

	void SectionManager::SetShowCappingPlane(bool val)
	{
		_ShowCappingPlane = val;
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		pSection->SetIsShowCappingPlane(val);
	}

	bool SectionManager::GetReversePlane()
	{
		return _ReverseClipPlane;
	}

	void SectionManager::SetReversePlane(bool val)
	{
		_ReverseClipPlane = val;
		Section* pSection = m_view->GetSceneManager()->GetSectionNode()->GetSection();
		pSection->SetIsReverseClipping(val);
	}

	void SectionManager::UnBindAllDragger()
	{
		//关闭旋转和拖动剖切
		if (bindAxisDragger)
		{
			m_view->GetDraggerManager()->UnBindDragger(bindAxisDragger);
		}
		if (bindRotationDragger)
		{
			m_view->GetDraggerManager()->UnBindDragger(bindRotationDragger);
		}
		bindAxisDragger = NULL;
		bindRotationDragger = NULL;
	}
}

/* namespace M3D */
