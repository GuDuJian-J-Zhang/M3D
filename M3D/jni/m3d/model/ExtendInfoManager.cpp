#include "sview/io/SVL2AsynReader.h"
#include "SVLLib/Stk_Document.h"

#include "m3d/action/RenderAction.h"
#include "m3d/model/ExtendInfoManager.h"
#include "m3d/model/Face.h"
#include "m3d/model/Body.h"
#include "m3d/model/PMIData.h"
#include "m3d/model/ModelView.h"
#include "m3d/graphics/SectionPlane.h"
#include "m3d/utils/Platform.h"
#include "m3d/utils/FileHelper.h"
#include "m3d/model/Edge.h"
#include "sview/views/Parameters.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/model/Model.h"
#include "m3d/SceneManager.h"

using namespace HoteamSoft::SVLLib;
namespace M3D
{

ExtendInfoManager::ExtendInfoManager()
{
	m_svlxFilePath = "";
	m_svlDoc = NULL;
	m_svlxFileExist = false;
	this->SetScene(NULL);
}

ExtendInfoManager::~ExtendInfoManager()
{
	this->Clear();
	UnSVLXInfo(m_svlDoc);
    }


void ExtendInfoManager::AddModelPMIs(IDTYPE ModelId, const map<int, PMIData*>& pmis)
{
	if (pmis.size()>0)
	{
		for (map<int, PMIData*>::const_iterator PMIDataIt = pmis.begin(); PMIDataIt != pmis.end(); PMIDataIt++)
		{
			PMIDataIt->second->AddRef();
			//这里仅仅添加了，还需要在删除PMI的时候，将此ID删除掉
			this->GetScene()->AddShapeIDToMap(PMIDataIt->second);
		}
	}

	m_AllModelPMIMap[ModelId] = pmis;
}
map<int, PMIData*>* ExtendInfoManager::GetModelPMIs(IDTYPE ModelId)
{
	map<IDTYPE, map<int, PMIData*>>::iterator ite = m_AllModelPMIMap.find(ModelId);
	if (ite != m_AllModelPMIMap.end())
	{
		return &ite->second;
	}
	return NULL;
}
map<IDTYPE, map<int, PMIData*>> ExtendInfoManager::GetAllPMIs()
{
	return  m_AllModelPMIMap;
}

int ExtendInfoManager::GetModelPMIsCount(IDTYPE ModelId)
{
	map<int, PMIData*>* pMap = GetModelPMIs(ModelId);
	return pMap ? pMap->size() : 0;
}

void ExtendInfoManager::AddModelView(IDTYPE ModelId, const vector<ModelView*>& vecViews)
{
	if (vecViews.size() > 0) {
		for (vector<ModelView*>::const_iterator modelViewIt = vecViews.begin(); modelViewIt != vecViews.end(); modelViewIt++)
		{
			(*modelViewIt)->AddRef();
		}
	}
	m_AllModleViewList[ModelId] = vecViews;
}
vector<ModelView*>* ExtendInfoManager::GetModelView(IDTYPE ModelId)
{
	map<IDTYPE, vector<ModelView*>>::iterator ite = m_AllModleViewList.find(ModelId);
	if (ite != m_AllModleViewList.end())
	{
		return &ite->second;
	}
	return NULL;
}

int ExtendInfoManager::GetModelViewCount(IDTYPE ModelId)
{
	vector<ModelView*>* pVec = GetModelView(ModelId);
	return pVec ? pVec->size() : 0;
}

void ExtendInfoManager::AddModelSectionPlane(IDTYPE ModelId, const vector<SectionPlane*>& vecSectionPlanes)
{
	if (vecSectionPlanes.size()>0)
	{
		for (vector<SectionPlane*>::const_iterator sectionPlaneIt = vecSectionPlanes.begin(); sectionPlaneIt != vecSectionPlanes.end(); sectionPlaneIt++)
		{
			(*sectionPlaneIt)->AddRef();
		}
	}

	m_AllSectionPlaneList[ModelId] = vecSectionPlanes;
}
vector<SectionPlane*>* ExtendInfoManager::GetModelSectionPlane(IDTYPE ModelId)
{
	map<IDTYPE, vector<SectionPlane*>>::iterator ite = m_AllSectionPlaneList.find(ModelId);
	if (ite != m_AllSectionPlaneList.end())
	{
		return &ite->second;
	}
	return NULL;
}

void ExtendInfoManager::Clear()
{
	for (map<IDTYPE, map<int, PMIData*>>::iterator it = m_AllModelPMIMap.begin(); it != m_AllModelPMIMap.end(); it++)
	{
		map<int, PMIData*>* mapPMI = &(it->second);
		for (map<int, PMIData*>::iterator PMIDataIt = mapPMI->begin(); PMIDataIt != mapPMI->end(); PMIDataIt++)
		{
			PMIDataIt->second->Release();
		}
		mapPMI->clear();
	}
	m_AllModelPMIMap.clear();

	for (map<IDTYPE, vector<ModelView*>>::iterator it = m_AllModleViewList.begin(); it != m_AllModleViewList.end(); it++)
	{
		vector<ModelView*>* vecView = &(it->second);
		for (vector<ModelView*>::iterator modelViewIt = vecView->begin(); modelViewIt != vecView->end(); modelViewIt++)
		{
			(*modelViewIt)->Release();
		}
		vecView->clear();
	}
	m_AllModleViewList.clear();

	for (map<IDTYPE, vector<SectionPlane*>>::iterator it = m_AllSectionPlaneList.begin(); it != m_AllSectionPlaneList.end(); it++)
	{
		vector<SectionPlane*>* vecSection = &(it->second);
		for (int i = 0; i < vecSection->size(); i++)
		{
			vecSection->at(i)->Release();
		}
		vecSection->clear();
	}
	m_AllSectionPlaneList.clear();
}

void ExtendInfoManager::Clear(IDTYPE ModelId)
{
	map<IDTYPE, map<int, PMIData*>>::iterator PMIiter = m_AllModelPMIMap.find(ModelId);
	if (PMIiter != m_AllModelPMIMap.end())
	{
		map<int, PMIData*>* mapPMI = &(PMIiter->second);
		for (map<int, PMIData*>::iterator it = mapPMI->begin(); it != mapPMI->end(); it++)
		{
			it->second->Release();
		}
		mapPMI->clear();
		m_AllModelPMIMap.erase(PMIiter);
	}
	map<IDTYPE, vector<ModelView*>>::iterator Viewite = m_AllModleViewList.find(ModelId);
	if (Viewite != m_AllModleViewList.end())
	{
		vector<ModelView*>* vecView = &(Viewite->second);
		for (vector<ModelView*>::iterator it = vecView->begin(); it != vecView->end(); it++)
		{
			(*it)->Release();
		}
		vecView->clear();
		m_AllModleViewList.erase(Viewite);
	}
	map<IDTYPE, vector<SectionPlane*>>::iterator Planeite = m_AllSectionPlaneList.find(ModelId);
	if (Planeite != m_AllSectionPlaneList.end())
	{
		vector<SectionPlane*>* vecSection = &(Planeite->second);
		for (int i = 0; i < vecSection->size(); i++)
		{
			 vecSection->at(i)->Release();
		}
		vecSection->clear();
		m_AllSectionPlaneList.erase(Planeite);
	}
}

void ExtendInfoManager::RayPick(RayPickAction * action)
{
	for (map<IDTYPE, map<int, PMIData*>>::iterator it = m_AllModelPMIMap.begin(); it != m_AllModelPMIMap.end(); it++)
	{
		map<int, PMIData*>* modelPMIs = &(it->second);
		 
		if (modelPMIs->size() > 0)
		{
			map<int, PMIData*>::iterator it = modelPMIs->begin();
			PMIData* pmi;
			while (it != modelPMIs->end())
			{
				pmi = it->second;
				if (pmi->IsVisible())
				{
					pmi->RayPick(action);
				}

				it++;
			}
		}
	}
}


void ExtendInfoManager::FindVisiableObject(RenderAction* renderAction)
{
	if (renderAction != NULL)
	{
		RenderEffect* renderEffect = renderAction->GetRenderEffect();
		RenderableTypeGroup& dataType = renderEffect->GetRenderableTypeFilter();

		if (true || dataType.Contain(RenderableType::RGT_PMI))
		{
			if (renderAction->GetScene())
			{
				if (renderAction->GetScene()->GetModel())
				{
					IDTYPE topModelId = renderAction->GetScene()->GetModel()->GetID();

					map<int, PMIData*>* mapPMI = this->GetModelPMIs(topModelId);
					renderAction->PrepareRenderPMIS(mapPMI);
                    //zhanglei-装配中的PMI
                    for (int i = 0; i < renderAction->GetScene()->GetModel()->GetSubModelCount(); i++) {
                        IDTYPE modelId = renderAction->GetScene()->GetModel()->GetSubModels().at(i)->GetID();
                        
                        map<int, PMIData*>* mPMI = this->GetModelPMIs(modelId);
                        renderAction->PrepareRenderPMIS(mPMI);
                    }
				}
			}

			//��PMI������ʾ����
			/*for (map<IDTYPE, map<int, PMIData*>>::iterator it = m_AllModelPMIMap.begin(); it != m_AllModelPMIMap.end(); it++)
			{
				map<int, PMIData*>* mapPMI = &(it->second);
				renderAction->PrepareRenderPMIS(mapPMI);
			}*/
		}
	}
}

void ExtendInfoManager::SetAllMeshFaceAttr(map<unsigned int, map<unsigned int, map<wstring, wstring>>>& allAttr){
    allMeshFaceAttr = allAttr;
}
void ExtendInfoManager::SetAllMeshEdgeAttr(map<unsigned int, map<unsigned int, map<wstring, wstring>>>& allAttr){
    allMeshEdgeAttr = allAttr;
}

void ExtendInfoManager::AddMeshFaceAttr(map<unsigned int, map<unsigned int, map<wstring, wstring>>>& faceAttrs)
{
	allMeshFaceAttr.insert(faceAttrs.begin(), faceAttrs.end());
}

void ExtendInfoManager::AddMeshEdgeAttr(map<unsigned int, map<unsigned int, map<wstring, wstring>>>& edgeAttrs)
{
	allMeshEdgeAttr.insert(edgeAttrs.begin(), edgeAttrs.end());
}

    map<wstring, wstring> ExtendInfoManager::GetMeshFaceAllAttribute(const int& i_iModelID, const int& i_MeshFaceID)
    {
        map<wstring, wstring> mapAttribute;
        if (allMeshFaceAttr.size() > 0) {
            map<unsigned int, map<unsigned int, map<wstring, wstring>>>::iterator itorModelMap = allMeshFaceAttr.find(i_iModelID);
            if (itorModelMap == allMeshFaceAttr.end())
            {
                return mapAttribute;
            }
            else
            {
                map<unsigned int, map<wstring, wstring>>::iterator itorMeshFaceMap = itorModelMap->second.find(i_MeshFaceID);
                if (itorMeshFaceMap == itorModelMap->second.end())
                    return mapAttribute;
                else
                    return itorMeshFaceMap->second;
            }
        }
        return mapAttribute;
    }
    map<wstring, wstring> ExtendInfoManager::GetMeshEdgeAllAttribute(const int& i_iModelID, const int& i_MeshEdgeID)
    {
        map<wstring, wstring> mapAttribute;
        if (allMeshEdgeAttr.size() > 0)
        {
            map<unsigned int, map<unsigned int, map<wstring, wstring>>>::iterator itorModelMap = allMeshEdgeAttr.find(i_iModelID);
            if (itorModelMap == allMeshEdgeAttr.end())
            {
                return mapAttribute;
            }
            else
            {
                map<unsigned int, map<wstring, wstring>>::iterator itorMeshEdgeMap = itorModelMap->second.find(i_MeshEdgeID);
                if (itorMeshEdgeMap == itorModelMap->second.end())
                    return mapAttribute;
                else
                    return itorMeshEdgeMap->second;
            }
        }
        return mapAttribute;
    }
GeometryAttribute* ExtendInfoManager::GetFaceGeoAttribute(int iModelID, int iMeshFaceID)
{
	//if (m_svlDoc)
	{
		if (iMeshFaceID > 0)
		{
//            Mesh* facemeshData = face->GetData();
            map<wstring, wstring> mapAttribute = GetMeshFaceAllAttribute(iModelID, iMeshFaceID);
           // Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)m_svlDoc;

            //LOGI("mapAttribute.size() %d ------",mapAttribute.size());
            if (mapAttribute.size() > 0) {
                Stk_GeometryAttributePtr geoAttribute = Stk_Document::GetMeshGeometryAttribute(mapAttribute);
                if (geoAttribute != NULL) {
                    GeometryAttribute* triMeshDataGeoAttribute = SVIEW::SVL2AsynReader::GetGeometryAttributeFromStk(geoAttribute);
                    return triMeshDataGeoAttribute;
                }
            }
//        SVIEW::SVL2AsynReader::GetTriMeshGeoAttribute(iModelID,facemeshData, meshFaceID, m_svlDoc);
			return   NULL;
		}
	}
	return NULL;
}

void ExtendInfoManager::CopyMeshAttr(unsigned int iOldModelID, unsigned int iNewModelID)
{
	if (iOldModelID > 0 && iNewModelID > 0)
	{
		map<unsigned int, map<unsigned int, map<wstring, wstring>>>::iterator itorMeshFace = allMeshFaceAttr.find(iOldModelID);
		if (itorMeshFace != allMeshFaceAttr.end())
		{
			allMeshFaceAttr.insert(make_pair(iNewModelID, itorMeshFace->second));
		}
		map<unsigned int, map<unsigned int, map<wstring, wstring>>>::iterator itorMeshEdge = allMeshEdgeAttr.find(iOldModelID);
		if (itorMeshEdge != allMeshEdgeAttr.end())
		{
			allMeshEdgeAttr.insert(make_pair(iNewModelID, itorMeshEdge->second));
		}
	}
}

GeometryAttribute* ExtendInfoManager::GetEdgeGeoAttribute(int iModelID, int iMeshEdgeID)
{
	//if (m_svlDoc)
	{
		if (iMeshEdgeID > 0 && iModelID > 0)
		{
//            RefPolyLine * edgeLine = edge->GetLineData();
            map<wstring, wstring> mapAttribute = GetMeshEdgeAllAttribute(iModelID, iMeshEdgeID);
            if (mapAttribute.size() > 0) {
                //Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)m_svlDoc;
                Stk_GeometryAttributePtr geoAttribute = Stk_Document::GetMeshGeometryAttribute(mapAttribute);
                if (geoAttribute != NULL) {
                    GeometryAttribute* triMeshDataGeoAttribute =
                    SVIEW::SVL2AsynReader::GetGeometryAttributeFromStk(geoAttribute);
                    return triMeshDataGeoAttribute;
                }
            }
//            SVIEW::SVL2AsynReader::GetEdgeGeoAttribute(iModelID,edgeLine, meshEdgeID, m_svlDoc);
			return   NULL;
		}
	}
	return NULL;
}


void ExtendInfoManager::SetSVLXFilePath(const string& filePath)
{
	this->m_svlxFilePath = filePath;
   /* if (m_svlDoc == NULL) {
        m_Reader = SVIEW::Reader::GetReader(this->m_svlxFilePath);
        SVIEW::SVL2AsynReader* m_SVL2Reader = dynamic_cast<SVIEW::SVL2AsynReader*>(m_Reader);
        m_svlDoc = (Stk_DocumentPtr *)(m_SVL2Reader->m_svl2Doc);
        *m_svlDoc = Stk_DocumentPtr::CreateObject();
    }*/
    }

void ExtendInfoManager::FillModelAttribute(Model* model)
{
	if (SVIEW::Parameters::Instance()->m_IsUseGeoData)
	{	//(*m_svl2Doc)->LoadAttribute();
		LOGI("SVL2AsynReader::FillModelAttribute Begin");
		LoadSVLXInfo(m_svlDoc);
		if (m_svlDoc)
		{
			SVIEW::SVL2AsynReader::FillModelAttribute(model, m_svlDoc);
		}
		UnSVLXInfo(m_svlDoc);
		LOGI("SVL2AsynReader::FillModelAttribute End");
	}

}

M3D::SceneManager* ExtendInfoManager::GetScene() const
{
	return m_scene;
}

void ExtendInfoManager::UnSVLXInfo(void* document)
{
	if (document)
	{
		//Stk_DocumentPtr* docPtr = (Stk_DocumentPtr*)document;
		//if (docPtr)
		//{		//�ͷ�Model
		//	(*docPtr)->UnLoadAttribute();
		//	delete docPtr;
		//	LOGI("AsynReader ok");
		//	document = NULL;
		//}
	}
}

void ExtendInfoManager::LoadSVLXInfo(void* document)
{
	//if (!m_svlxFileExist && FileHelper::FileExist(m_svlxFilePath))
	//{
	//	m_svlxFileExist = true;

	//	Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svlDoc;
	//	*m_svl2Doc = Stk_DocumentPtr::CreateObject();
	//	string filePath = m_svlxFilePath;
	//	wstring wfilePath = Platform::StringToWString(filePath, "auto");
	//	//���ļ�
	//	(*m_svl2Doc)->OpenFile(wfilePath);
	//	(*m_svl2Doc)->LoadAttribute();
	//}
}

void ExtendInfoManager::SetScene(M3D::SceneManager* val)
{
	m_scene = val;
}

}
