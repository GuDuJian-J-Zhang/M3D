//
//  StlWriter.cpp
//  M3D
//
//  Created by administrator on 15/3/2.
//  Copyright (c) 2015年 hoteam. All rights reserved.
//

#include "JsonMacro.h"
#include "document.h"
#include "stringbuffer.h"

#include "prettywriter.h"
#include "allocators.h"
using rapidjson::Value;
using rapidjson::Document;
using rapidjson::StringBuffer;
using rapidjson::PrettyWriter;
using rapidjson::Writer;
using rapidjson::StringRef;
using rapidjson::MemoryPoolAllocator;
#include <fstream>
#include <sstream>   // Post processing flags
#include "m3d/ResourceManager.h"
#include "SVLLib/Stk_API.h"
#include "SVLLib/Stk_Document.h"
#include "SVLLib/Stk_UnitTest.h"
#include "SVLLib/Stk_LoadInf.h"
#include "SVLLib/Stk_CompositeText.h"
#include "SVLLib/Stk_GeometryAttribute.h"
#include "SVLLib/Stk_View.h"
#include "SVLLib/Stk_PlaneEntity.h"
#include "SVLLib/Stk_Define.h"
#include "sview/PMIAnalysis/SVLXPMICreator.h"

#include "SVLXWriter.h"
#include "Writer.h"
#include "Utility.h"

#include "sview/views/View.h"

#include "sview/io/SVLVersionConvertIF.h"

#include "m3d/utils/Platform.h"
#include "m3d/model/Model.h"
#include "m3d/model/Body.h"
#include "m3d/utils/FileHelper.h"
#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/graphics/PbrMaterial.h"
#include "m3d/graphics/Material.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/model/MeshData.h"
#include "m3d/model/Face.h"
#include "m3d/model/Edge.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/model/ShapeProperty.h"
#include "m3d/extras/note/NoteGroup.h"
#include "m3d/extras/annotation/AnnotationGroup.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/extras/note/TextNote.h"
#include "m3d/extras/note/SequenceNumberNote.h"
#include "m3d/extras/note/ThreeDGesturesNote.h"

#include "m3d/base/json/json.h"
#include "m3d/base/json/forwards.h"
#include "m3d/renderer/LightManager.h"
#include "m3d/graphics/PointLight.h"
#include "m3d/graphics/SpotLight.h"
#include "m3d/graphics/AmbientLight.h"
#include "m3d/graphics/HemisphereLight.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/graphics/ShaderMaterial.h"
#include "m3d/graphics/TextureCube.h"
#include "m3d/utils/PathHelper.h"
#include "m3d/graphics/MatCapMaterial.h"

using namespace HoteamSoft::SVLLib;
namespace SVIEW
{
	using HoteamSoft::SVLLib::StkGeoAttrTypeEnum;
	using HoteamSoft::SVLLib::GEOATTR_TYPE_UNKNOWN;
	using HoteamSoft::SVLLib::GEOATTR_TYPE_PLANEFACE;
	using HoteamSoft::SVLLib::GEOATTR_TYPE_REVOLUTIONFACE;
	using HoteamSoft::SVLLib::GEOATTR_TYPE_CYLINDERFACE;
	using HoteamSoft::SVLLib::GEOATTR_TYPE_CONICALFACE;
	using HoteamSoft::SVLLib::GEOATTR_TYPE_SPHEREFACE;
	using HoteamSoft::SVLLib::GEOATTR_TYPE_TOROIDALFACE;
	using HoteamSoft::SVLLib::GEOATTR_TYPE_LINE;
	using HoteamSoft::SVLLib::GEOATTR_TYPE_ELLIPSE;

	SVLXWriter::SVLXWriter(void)
	{
		m_View = NULL;
		m_svl2Doc = new Stk_DocumentPtr();
	}
	SVLXWriter::~SVLXWriter()
	{
		Stk_DocumentPtr* docPtr = (Stk_DocumentPtr*)m_svl2Doc;
		if (docPtr)
		{
			delete docPtr;
			LOGI("SVLXWriter ~~ ok");
			m_svl2Doc = NULL;
		}
	}
	
	int SVLXWriter::SaveAll(View * view, const string& srcFilePath, const string & targetPath)
	{
		//保存全部文件，将顶级模型设置到需要保存的模型组中
		int saveState = -1;
		Model* topModel = view->GetSceneManager()->GetModel();
		string topModelName = topModel->GetName();
		vector<Model*> savedModels;

		if (topModelName == "Root")
		{
			for (int i = 0; i < topModel->GetChildren().size(); i++)
			{
				M3D::Model* subModel = topModel->GetChildren().at(i);
				savedModels.push_back(subModel);
			}
		}
		else
		{
			savedModels.push_back(topModel);
		}

		this->SetSavedModel(savedModels);

		this->UpdateMaterialsSet();

		bool hasSrcSVLXFile = false;
		if (srcFilePath.size()>4)
		{
			if (StringHelper::ToLower(FileHelper::GetExt(srcFilePath)) == "svlx")
			{
				hasSrcSVLXFile = true;
			}
		}

		if (hasSrcSVLXFile)
		{
			return WriteSavedModels(view, srcFilePath, targetPath);
		}
		else
		{
			return WriteSavedModels(view, "", targetPath);
		}
	}

	int SVLXWriter::WriteSavedModels(View * view,const string& srcFilePath, const string & targetPath)
	{
		m_View = view;

		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		*m_svl2Doc = Stk_DocumentPtr::CreateObject();
		HoteamSoft::SVLLib::Stk_DocumentPtr& m_pDoc = *m_svl2Doc;
		wstring wstrSVLXOutFilePath = Platform::StringToWString(targetPath,"auto");
 
		wstring wstrExt = wstrSVLXOutFilePath.substr(wstrSVLXOutFilePath.size() - 5, 5);
		if (wstrExt == L".svlx" || wstrExt == L".SVLX")
			wstrSVLXOutFilePath = wstrSVLXOutFilePath.substr(0, wstrSVLXOutFilePath.length() - 5);

		m_pDoc->SetOutputFullPath(wstrSVLXOutFilePath);
		wstrSVLXOutFilePath = m_pDoc->GetOutputFullPath();

		string strSaveFileName = Platform::WStringToString(m_pDoc->GetOutputName(), "auto");
		string strSVLXOutFilePath = Platform::WStringToString(wstrSVLXOutFilePath, "auto");

		//将目录不能编辑的内容解压，然后重新压缩进SVLX中
		if (srcFilePath.size() > 0)
		{
			m_pDoc->OpenFile(Platform::StringToWString(srcFilePath, "auto"));

			//预览图
			string strPreViewFileName = strSaveFileName;
			strPreViewFileName.append(".bmp");
			string strPreViewFilePath = strSVLXOutFilePath;
			m_pDoc->GetSVLXFileItemToFile(".bmp", strPreViewFilePath, true, strPreViewFileName);
 
			//Images文件夹,由于文件会存储多次，暂时屏蔽掉
			//TODO
            string strImagesFolderName = "images";
            string strImagesFolderPath = strSVLXOutFilePath + "images";
            m_pDoc->GetSVLXFolderItemToFolder(strImagesFolderName, strImagesFolderPath);
            
            string strSaresFolderName = "sares";
            string strSaresFolderPath = strSVLXOutFilePath + "sares";
            m_pDoc->GetSVLXFolderItemToFolder(strSaresFolderName, strSaresFolderPath);
            
            string strManualFolderName = "manual";
            string strManualFolderPath = strSVLXOutFilePath + "manual";
            m_pDoc->GetSVLXFolderItemToFolder(strManualFolderName, strManualFolderPath);
            
            string strHotsFolderName = "hotspotres";
            string strHotsFolderPath = strSVLXOutFilePath + "hotspotres";
            m_pDoc->GetSVLXFolderItemToFolder(strHotsFolderName, strHotsFolderPath);

			//PMI
			string strPMIFileName = strSaveFileName;
			strPMIFileName.append(".pmi");
			string strPMIFilePath = strSVLXOutFilePath;
			m_pDoc->GetSVLXFileItemToFile(".pmi", strPMIFilePath, true, strPMIFileName);


			//图层
			string strLayerFileName = strSaveFileName;
			strLayerFileName.append(".layer");
			string strLayerFilePath = strSVLXOutFilePath;
			m_pDoc->GetSVLXFileItemToFile(".layer", strLayerFilePath, true, strLayerFileName);


			//线集
			string strLineFileName = strSaveFileName;
			strLineFileName.append(".line");
			string strLineFilePath = strSVLXOutFilePath;
			m_pDoc->GetSVLXFileItemToFile(".line", strLineFilePath, true, strLineFileName);

			//箭头
			string strArrowFileName = strSaveFileName;
			strArrowFileName.append(".arrow");
			string strArrowFilePath = strSVLXOutFilePath;
			m_pDoc->GetSVLXFileItemToFile(".arrow", strArrowFilePath, true, strArrowFileName);

			//用户批注
//            string strUserNoteFileName = strSaveFileName;
//            strUserNoteFileName.append(".usernote");
//            string strUserNoteFilePath = strSVLXOutFilePath;
//            m_pDoc->GetSVLXFileItemToFile(".usernote", strUserNoteFilePath, true, strUserNoteFileName);

			//移动端的视图文件
			string strViewFileName = strSaveFileName;
			strViewFileName.append(".view");
			string strViewFilePath = strSVLXOutFilePath;
			m_pDoc->GetSVLXFileItemToFile(".view", strViewFilePath, true, strViewFileName);

			//线缆关联文件
			string strInfFileName = strSaveFileName;
			strInfFileName.append(".inf");
			string strInfFilePath = strSVLXOutFilePath;
			m_pDoc->GetSVLXFileItemToFile(".inf", strInfFilePath, true, strInfFileName);

            //geo文件
            string strGeoFileName = strSaveFileName;
            strGeoFileName.append(".geo");
            string strGeoFilePath = strSVLXOutFilePath;
            m_pDoc->GetSVLXFileItemToFile(".geo", strGeoFilePath, true, strGeoFileName);
            
            //animation文件
            string strAniFileName = strSaveFileName;
            strAniFileName.append(".animation");
            string strAniFilePath = strSVLXOutFilePath;
            m_pDoc->GetSVLXFileItemToFile(".animation", strAniFilePath, true, strAniFileName);
            
            //hotspot文件
            string strHotFileName = strSaveFileName;
            strHotFileName.append(".hotspot");
            string strHotFilePath = strSVLXOutFilePath;
            m_pDoc->GetSVLXFileItemToFile(".hotspot", strHotFilePath, true, strHotFileName);
            
            //manual文件
            string strManFileName = strSaveFileName;
            strManFileName.append(".manual");
            string strManFilePath = strSVLXOutFilePath;
            m_pDoc->GetSVLXFileItemToFile(".manual", strManFilePath, true, strManFileName);
            
            //task文件
            string strTaskFileName = strSaveFileName;
            strTaskFileName.append(".task");
            string strTaskFilePath = strSVLXOutFilePath;
            m_pDoc->GetSVLXFileItemToFile(".task", strTaskFilePath, true, strTaskFileName);
            
			m_pDoc->ClearSVLXFileItem();

			m_pDoc->AddSVLXFileItem(strPreViewFileName);
            m_pDoc->AddSVLXFileItem(strImagesFolderName);
            m_pDoc->AddSVLXFileItem(strSaresFolderName);
            m_pDoc->AddSVLXFileItem(strManualFolderName);
            m_pDoc->AddSVLXFileItem(strHotsFolderName);
			m_pDoc->AddSVLXFileItem(strPMIFileName);
			m_pDoc->AddSVLXFileItem(strLayerFileName);
			m_pDoc->AddSVLXFileItem(strLineFileName);
			m_pDoc->AddSVLXFileItem(strArrowFileName);
//            m_pDoc->AddSVLXFileItem(strUserNoteFileName);
			m_pDoc->AddSVLXFileItem(strViewFileName);
			m_pDoc->AddSVLXFileItem(strInfFileName);
            m_pDoc->AddSVLXFileItem(strGeoFileName);
            m_pDoc->AddSVLXFileItem(strAniFileName);
            m_pDoc->AddSVLXFileItem(strHotFileName);
            m_pDoc->AddSVLXFileItem(strManFileName);
            m_pDoc->AddSVLXFileItem(strTaskFileName);
		}

		SaveMaterial();

		//Model* topModel = view->GetSceneManager()->GetModel();

		//保存Bom
		//根据名称判断顶级是否是多实例
		//string topModelName = topModel->GetName();
		vector<Model*>& savedModels = this->GetSavedModels();
		if (savedModels.size() >1)
		{
			Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
			HoteamSoft::SVLLib::Stk_DocumentPtr& m_pDoc = *m_svl2Doc;

			for (int i = 0;i<savedModels.size();i++)
			{
				M3D::Model* subModel = savedModels.at(i);
				HoteamSoft::SVLLib::Stk_InstancePtr pNewTopInstance = m_pDoc->AddTopInstance(subModel->GetSVLId());
				WriteTopInstance(subModel, &pNewTopInstance);
			}
		}
		else if(savedModels.size() == 1)
		{
			Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
			HoteamSoft::SVLLib::Stk_DocumentPtr& m_pDoc = *m_svl2Doc;

			M3D::Model* subModel = savedModels.at(0);
			HoteamSoft::SVLLib::Stk_InstancePtr pNewTopInstance = m_pDoc->AddTopInstance(subModel->GetSVLId());
			WriteTopInstance(subModel, &pNewTopInstance);
		}

		//灯光
		/*string strPreViewFileName = strSaveFileName;
		strPreViewFileName.append(".lights");
		string strPreViewFilePath = strSVLXOutFilePath;
		SaveLights(strPreViewFilePath + strPreViewFileName);
		m_pDoc->AddSVLXFileItem(strPreViewFileName);*/

        //批注
        string strNoteName = strSaveFileName;
         strNoteName.append(".annotation");
         string strNotePath = strSVLXOutFilePath;
         SaveAnnotations(strNotePath + strNoteName);
         m_pDoc->AddSVLXFileItem(strNoteName);
        
		//保存视图
		SaveView();
		SaveAttribute();

		//保存
		m_pDoc->WriteFile();
		//释放SVLLib
		m_pDoc->UnloadMaterial();
		m_pDoc->UnLoadAttribute();
		m_pDoc->UnloadGeoAttribute();
		m_pDoc->UnloadModel();
		m_pDoc->UnloadBom();
		m_pDoc = nullptr;

		return 0;
	}

	void SVLXWriter::WriteTopInstance(M3D::Model* model, void* svlInstance)
	{
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		HoteamSoft::SVLLib::Stk_DocumentPtr& m_pDoc = *m_svl2Doc;

		HoteamSoft::SVLLib::Stk_InstancePtr& pNewTopInstance =*(HoteamSoft::SVLLib::Stk_InstancePtr*)svlInstance;// m_pDoc->AddTopInstance(/*model->GetSVLId()*/);

		FillInstance(model,model, &pNewTopInstance);

		for (int i = 0;i<model->GetSubModelCount();i++)
		{
			M3D::Model* subModel = model->GetSubModels().at(i);

			HoteamSoft::SVLLib::Stk_InstancePtr pNewInstance = pNewTopInstance->AddChild(subModel->GetSVLId());

			WriteInstance(subModel, model, &pNewInstance);
		}
	}

	void SVLXWriter::FillInstance(M3D::Model* model, M3D::Model* topParentmodel, void* svlInstance)
	{
		HoteamSoft::SVLLib::Stk_InstancePtr& pInstance = *(HoteamSoft::SVLLib::Stk_InstancePtr*)(svlInstance);

		wstring wstrTopInstanceName = Platform::StringToWString(model->GetName(),"auto");
		pInstance->SetInstanceName(wstrTopInstanceName);
		pInstance->SetPlcID(model->GetPlcId());

		M3D::Model* parentModel = model->GetParent();

		if (model == topParentmodel)
		{
			pInstance->SetParentID(-1);
		}
		else
		{
			if (parentModel)
			{
				pInstance->SetParentID(parentModel->GetSVLId());
			}
			else
			{
				pInstance->SetParentID(-1);
			}
		}

		pInstance->SetID(model->GetSVLId());

		int materialId = -1;
		if (model->HasMaterial() && model->GetMaterial())
		{
			materialId = model->GetMaterial()->GetSvlId();
		}

		pInstance->SetMaterialID(materialId);

		pInstance->SetVisible(model->IsVisible());

		Matrix4 instancePlcMatrix = model->GetPlaceMatrix()->ToMatrix4();
		//instancePlcMatrix.Transpose();
		pInstance->SetMatrix((float*)instancePlcMatrix.Data());

		//
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		HoteamSoft::SVLLib::Stk_DocumentPtr& m_pDoc = *m_svl2Doc;

		bool bProtoInited = false;
		int iNewModelID = model->GetProtoTypeId();
		if (m_pDoc->GetModelByID(iNewModelID) != nullptr)
			bProtoInited = true;
		HoteamSoft::SVLLib::Stk_ModelPtr pNewProtoType = pInstance->SetModel(iNewModelID);

		if (!bProtoInited)
		{
			WriteProtoType(model,&pNewProtoType);
		}
	}

	void SVLXWriter::WriteInstance(M3D::Model* model, M3D::Model* topParentmodel, void* svlInstance)
	{
		HoteamSoft::SVLLib::Stk_InstancePtr& pNewInstance = *(HoteamSoft::SVLLib::Stk_InstancePtr*)(svlInstance);

		FillInstance(model, topParentmodel, &pNewInstance);

		for (int i = 0; i < model->GetSubModelCount(); i++)
		{
			M3D::Model* subModel = model->GetSubModels().at(i);
			HoteamSoft::SVLLib::Stk_InstancePtr psubNewInstance = pNewInstance->AddChild(subModel->GetSVLId());
			//递归调用，遍历所有子模型
			WriteInstance(subModel, topParentmodel, &psubNewInstance);
		}
	}

	HoteamSoft::SVLLib::Stk_GeometryAttributePtr GeoAttribToStk_GeoAttrib(M3D::GeometryAttribute* pSoSVLGeoAttr)
	{
		HoteamSoft::SVLLib::Stk_GeometryAttributePtr pStkGeoAttr = nullptr;
		if (!pSoSVLGeoAttr)
			return pStkGeoAttr;
		M3D::GeoAttrTypeEnum geoAttr = pSoSVLGeoAttr->GetGeoAttrType();
		if (geoAttr == M3D::GeoAttrTypeEnum::M3D_GEOATTR_TYPE_PLANEFACE)
		{
			pStkGeoAttr = HoteamSoft::SVLLib::Stk_PlaneFaceAttributePtr::CreateObject();
			Vector3 normal = ((PlaneFaceAttribute*)pSoSVLGeoAttr)->GetNormal();
			HoteamSoft::SVLLib::STK_DIR32 dirNormal;
			dirNormal.X = normal.m_x; dirNormal.Y = normal.m_y; dirNormal.Z = normal.m_z;
			HoteamSoft::SVLLib::Stk_PlaneFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetNormal(dirNormal);
			Vector3 origin = ((PlaneFaceAttribute*)pSoSVLGeoAttr)->GetOrigin();
			HoteamSoft::SVLLib::STK_PNT32 pntOrigin;
			pntOrigin.X = origin.m_x; pntOrigin.Y = origin.m_y; pntOrigin.Z = origin.m_z;
			HoteamSoft::SVLLib::Stk_PlaneFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetOrigin(pntOrigin);
		}
		else if (geoAttr == M3D::GeoAttrTypeEnum::M3D_GEOATTR_TYPE_REVOLUTIONFACE)
		{
			pStkGeoAttr = HoteamSoft::SVLLib::Stk_RevolutionFaceAttributePtr::CreateObject();
			Vector3 axisOrigin = ((RevolutionFaceAttribute*)pSoSVLGeoAttr)->GetAxisOrigin();
			HoteamSoft::SVLLib::STK_PNT32 pntAxisOrigin;
			pntAxisOrigin.X = axisOrigin.m_x; pntAxisOrigin.Y = axisOrigin.m_y; pntAxisOrigin.Z = axisOrigin.m_z;
			HoteamSoft::SVLLib::Stk_RevolutionFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			HoteamSoft::SVLLib::Stk_RevolutionFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetOriginRadius(((RevolutionFaceAttribute*)pSoSVLGeoAttr)->GetRadius());
			Vector3 revoAxis = ((RevolutionFaceAttribute*)pSoSVLGeoAttr)->GetRevoAxis();
			HoteamSoft::SVLLib::STK_DIR32 dirRevoAxis;
			dirRevoAxis.X = revoAxis.m_x; dirRevoAxis.Y = revoAxis.m_y; dirRevoAxis.Z = revoAxis.m_z;
			HoteamSoft::SVLLib::Stk_RevolutionFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetRevoAxis(dirRevoAxis);
		}
		else if (geoAttr == M3D::GeoAttrTypeEnum::M3D_GEOATTR_TYPE_CYLINDERFACE)
		{
			pStkGeoAttr = HoteamSoft::SVLLib::Stk_CylinderFaceAttributePtr::CreateObject();
			Vector3 axisOrigin = ((CylinderFaceAttribute*)pSoSVLGeoAttr)->GetAxisOrigin();
			HoteamSoft::SVLLib::STK_PNT32 pntAxisOrigin;
			pntAxisOrigin.X = axisOrigin.m_x; pntAxisOrigin.Y = axisOrigin.m_y; pntAxisOrigin.Z = axisOrigin.m_z;
			HoteamSoft::SVLLib::Stk_CylinderFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			HoteamSoft::SVLLib::Stk_CylinderFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetOriginRadius(((CylinderFaceAttribute*)pSoSVLGeoAttr)->GetRadius());
			Vector3 revoAxis = ((CylinderFaceAttribute*)pSoSVLGeoAttr)->GetRevoAxis();
			HoteamSoft::SVLLib::STK_DIR32 dirRevoAxis;
			dirRevoAxis.X = revoAxis.m_x; dirRevoAxis.Y = revoAxis.m_y; dirRevoAxis.Z = revoAxis.m_z;
			HoteamSoft::SVLLib::Stk_CylinderFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetRevoAxis(dirRevoAxis);
		}
		else if (geoAttr == M3D::GeoAttrTypeEnum::M3D_GEOATTR_TYPE_CONICALFACE)
		{
			pStkGeoAttr = HoteamSoft::SVLLib::Stk_ConeFaceAttributePtr::CreateObject();
			Vector3 axisOrigin = ((ConicalFaceAttribute*)pSoSVLGeoAttr)->GetAxisOrigin();
			HoteamSoft::SVLLib::STK_PNT32 pntAxisOrigin;
			pntAxisOrigin.X = axisOrigin.m_x; pntAxisOrigin.Y = axisOrigin.m_y; pntAxisOrigin.Z = axisOrigin.m_z;
			HoteamSoft::SVLLib::Stk_ConeFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			HoteamSoft::SVLLib::Stk_ConeFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetOriginRadius(((ConicalFaceAttribute*)pSoSVLGeoAttr)->GetRadius());
			Vector3 revoAxis = ((ConicalFaceAttribute*)pSoSVLGeoAttr)->GetRevoAxis();
			HoteamSoft::SVLLib::STK_DIR32 dirRevoAxis;
			dirRevoAxis.X = revoAxis.m_x; dirRevoAxis.Y = revoAxis.m_y; dirRevoAxis.Z = revoAxis.m_z;
			HoteamSoft::SVLLib::Stk_ConeFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetRevoAxis(dirRevoAxis);
		}
		else if (geoAttr == M3D::GeoAttrTypeEnum::M3D_GEOATTR_TYPE_SPHEREFACE)
		{
			pStkGeoAttr = HoteamSoft::SVLLib::Stk_SphereFaceAttributePtr::CreateObject();
			Vector3 axisOrigin = ((SphereFaceAttribute*)pSoSVLGeoAttr)->GetAxisOrigin();
			HoteamSoft::SVLLib::STK_PNT32 pntAxisOrigin;
			pntAxisOrigin.X = axisOrigin.m_x; pntAxisOrigin.Y = axisOrigin.m_y; pntAxisOrigin.Z = axisOrigin.m_z;
			HoteamSoft::SVLLib::Stk_SphereFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			HoteamSoft::SVLLib::Stk_SphereFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetOriginRadius(((SphereFaceAttribute*)pSoSVLGeoAttr)->GetRadius());
			Vector3 revoAxis = ((SphereFaceAttribute*)pSoSVLGeoAttr)->GetRevoAxis();
			HoteamSoft::SVLLib::STK_DIR32 dirRevoAxis;
			dirRevoAxis.X = revoAxis.m_x; dirRevoAxis.Y = revoAxis.m_y; dirRevoAxis.Z = revoAxis.m_z;
			HoteamSoft::SVLLib::Stk_SphereFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetRevoAxis(dirRevoAxis);
		}
		else if (geoAttr == M3D::GeoAttrTypeEnum::M3D_GEOATTR_TYPE_TOROIDALFACE)
		{
			pStkGeoAttr = HoteamSoft::SVLLib::Stk_ToroidalFaceAttributePtr::CreateObject();
			Vector3 axisOrigin = ((ToroidalFaceAttribute*)pSoSVLGeoAttr)->GetAxisOrigin();
			HoteamSoft::SVLLib::STK_PNT32 pntAxisOrigin;
			pntAxisOrigin.X = axisOrigin.m_x; pntAxisOrigin.Y = axisOrigin.m_y; pntAxisOrigin.Z = axisOrigin.m_z;
			HoteamSoft::SVLLib::Stk_ToroidalFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			HoteamSoft::SVLLib::Stk_ToroidalFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetOriginRadius(((ToroidalFaceAttribute*)pSoSVLGeoAttr)->GetRadius());
			Vector3 revoAxis = ((ToroidalFaceAttribute*)pSoSVLGeoAttr)->GetRevoAxis();
			HoteamSoft::SVLLib::STK_DIR32 dirRevoAxis;
			dirRevoAxis.X = revoAxis.m_x; dirRevoAxis.Y = revoAxis.m_y; dirRevoAxis.Z = revoAxis.m_z;
			HoteamSoft::SVLLib::Stk_ToroidalFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetRevoAxis(dirRevoAxis);
			HoteamSoft::SVLLib::Stk_ToroidalFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetMajorRaius(((ToroidalFaceAttribute*)pSoSVLGeoAttr)->GetMajorRadius());
			HoteamSoft::SVLLib::Stk_ToroidalFaceAttributePtr::DynamicCast(pStkGeoAttr)->SetMinorRadius(((ToroidalFaceAttribute*)pSoSVLGeoAttr)->GetMinorRadius());
		}
		else if (geoAttr == M3D::GeoAttrTypeEnum::M3D_GEOATTR_TYPE_LINE)
		{
			pStkGeoAttr = HoteamSoft::SVLLib::Stk_LineAttributePtr::CreateObject();
			Vector3 centerPoint = ((LineAttribute*)pSoSVLGeoAttr)->GetCenterPoint();
			HoteamSoft::SVLLib::STK_PNT32 pntCenterPoint, pntStartPoint, pntEndPoint;
			pntCenterPoint.X = centerPoint.m_x; pntCenterPoint.Y = centerPoint.m_y; pntCenterPoint.Z = centerPoint.m_z;
			HoteamSoft::SVLLib::Stk_LineAttributePtr::DynamicCast(pStkGeoAttr)->SetCenterPoint(pntCenterPoint);
			Vector3 direction = ((LineAttribute*)pSoSVLGeoAttr)->GetDirection();
			HoteamSoft::SVLLib::STK_DIR32 dirDirection;
			dirDirection.X = direction.m_x; dirDirection.Y = direction.m_y; dirDirection.Z = direction.m_z;
			HoteamSoft::SVLLib::Stk_LineAttributePtr::DynamicCast(pStkGeoAttr)->SetDirection(dirDirection);
			Vector3 startPoint = ((LineAttribute*)pSoSVLGeoAttr)->GetStartPoint();
			pntStartPoint.X = startPoint.m_x; pntStartPoint.Y = startPoint.m_y; pntStartPoint.Z = startPoint.m_z;
			HoteamSoft::SVLLib::Stk_LineAttributePtr::DynamicCast(pStkGeoAttr)->SetStartPoint(pntStartPoint);
			Vector3 endPoint = ((LineAttribute*)pSoSVLGeoAttr)->GetEndPoint();
			pntEndPoint.X = endPoint.m_x; pntEndPoint.Y = endPoint.m_y; pntEndPoint.Z = endPoint.m_z;
			HoteamSoft::SVLLib::Stk_LineAttributePtr::DynamicCast(pStkGeoAttr)->SetEndPoint(pntEndPoint);
			HoteamSoft::SVLLib::Stk_LineAttributePtr::DynamicCast(pStkGeoAttr)->SetLength(((LineAttribute*)pSoSVLGeoAttr)->GetLength());
		}
		else if (geoAttr == M3D::GeoAttrTypeEnum::M3D_GEOATTR_TYPE_ELLIPSE)
		{
			pStkGeoAttr = HoteamSoft::SVLLib::Stk_EllipseAttributePtr::CreateObject();
			Vector3 centerPoint = ((EllipseAttribute*)pSoSVLGeoAttr)->GetCenterPoint();
			HoteamSoft::SVLLib::STK_PNT32 pntCenterPoint, pntStartPoint, pntEndPoint;
			pntCenterPoint.X = centerPoint.m_x; pntCenterPoint.Y = centerPoint.m_y; pntCenterPoint.Z = centerPoint.m_z;
			HoteamSoft::SVLLib::Stk_EllipseAttributePtr::DynamicCast(pStkGeoAttr)->SetCenterPoint(pntCenterPoint);
			Vector3 startPoint = ((EllipseAttribute*)pSoSVLGeoAttr)->GetStartPoint();
			pntStartPoint.X = startPoint.m_x; pntStartPoint.Y = startPoint.m_y; pntStartPoint.Z = startPoint.m_z;
			HoteamSoft::SVLLib::Stk_EllipseAttributePtr::DynamicCast(pStkGeoAttr)->SetStartPoint(pntStartPoint);
			Vector3 endPoint = ((EllipseAttribute*)pSoSVLGeoAttr)->GetEndPoint();
			pntEndPoint.X = endPoint.m_x; pntEndPoint.Y = endPoint.m_y; pntEndPoint.Z = endPoint.m_z;
			HoteamSoft::SVLLib::Stk_EllipseAttributePtr::DynamicCast(pStkGeoAttr)->SetEndPoint(pntEndPoint);
			HoteamSoft::SVLLib::Stk_EllipseAttributePtr::DynamicCast(pStkGeoAttr)->SetMajorRadius(((EllipseAttribute*)pSoSVLGeoAttr)->GetMajorRadius());
			HoteamSoft::SVLLib::Stk_EllipseAttributePtr::DynamicCast(pStkGeoAttr)->SetMinorRadius(((EllipseAttribute*)pSoSVLGeoAttr)->GetMinorRadius());
			Vector3 xDir, yDir, zDir;
			((EllipseAttribute*)pSoSVLGeoAttr)->GetXYZDir(xDir, yDir, zDir);
			HoteamSoft::SVLLib::STK_DIR32 dirXDir, dirYDir, dirZDir;
			dirXDir.X = xDir.m_x; dirXDir.Y = xDir.m_y; dirXDir.Z = xDir.m_z;
			dirYDir.X = yDir.m_x; dirYDir.Y = yDir.m_y; dirYDir.Z = yDir.m_z;
			dirZDir.X = zDir.m_x; dirZDir.Y = zDir.m_y; dirZDir.Z = zDir.m_z;
			HoteamSoft::SVLLib::Stk_EllipseAttributePtr::DynamicCast(pStkGeoAttr)->SetXYZDir(dirXDir, dirYDir, dirZDir);
		}
		return pStkGeoAttr;
	}


	void SVLXWriter::WriteProtoType(M3D::Model* model, void* svlProtoType)
	{
		HoteamSoft::SVLLib::Stk_ModelPtr& pNewProtoType = *(HoteamSoft::SVLLib::Stk_ModelPtr*)svlProtoType;
		
		wstring wstrTopInstanceName = Platform::StringToWString(model->GetName(),"auto");
		pNewProtoType->SetModelName(wstrTopInstanceName);
		pNewProtoType->SetID(model->GetProtoTypeId());
		BoundingBox box = model->GetBoundingBox();
		float fBox[6] = {box.m_min.m_x,box.m_min.m_y, box.m_min.m_z, 
			box.m_max.m_x, box.m_max.m_y, box.m_max.m_z};
		pNewProtoType->SetBoundingBox(fBox);

		vector<Body*> *bodies = model->GetBodys();
		if (bodies)
		{		
			for (int i = 0; i < bodies->size(); i++)
			{
				Body* subBody = bodies->at(i);
				//写Mesh
				HoteamSoft::SVLLib::Stk_MeshPrimPtr pMeshPrim = pNewProtoType->AddLOD1Mesh(subBody->GetSVLId());
				writeMeshPrim(&pMeshPrim, subBody);

				////写线集
				writeLineSet(&pMeshPrim, subBody);

				////写Face
				vector<Face*>& m3dFaces = subBody->GetFaces();
				for (int j= 0;j<m3dFaces.size();j++)
				{
					Face* subFace = m3dFaces.at(j);
					writeMeshFace(&pMeshPrim,subFace);
				}

				//边
				vector<Edge*>* m3dEdges = subBody->GetEdges();
				for (int j = 0; j < m3dEdges->size(); j++)
				{
					Edge* subEdge = m3dEdges->at(j);
					writeMeshEdge(&pMeshPrim, subEdge);
				}
				////写点
				//writePoint(pPntMarkerKit, pModel);
			}
		}
	}

	void SVLXWriter::writeMeshPrim(void* meshPrim, M3D::Body* body)
	{
		HoteamSoft::SVLLib::Stk_MeshPrimPtr& pMeshPrim = *(HoteamSoft::SVLLib::Stk_MeshPrimPtr*)meshPrim;
	
		VertexSet* bodyMeshData = body->GetData();
		if (bodyMeshData)
		{
			vector<Vector3>* bodyMeshDataPosArray = bodyMeshData->GetPositionArray();
			vector<Vector3>* bodyMeshDataNormalArray = bodyMeshData->GetNormalArray();
			vector<Vector3>* bodyMeshDataUVArray = bodyMeshData->GetTextureCoordArray();

			std::vector<HoteamSoft::SVLLib::STK_FLOAT32> vecVertices;
			std::vector<HoteamSoft::SVLLib::STK_FLOAT32> vecNormals;
			std::vector<HoteamSoft::SVLLib::STK_FLOAT32> vecUVs;
			int iNumObject = 0;
			iNumObject = bodyMeshDataPosArray->size();
			int i = 0;
			for (; i < iNumObject; i++)
			{
				Vector3& vecPnt = (bodyMeshDataPosArray->at(i));
				vecVertices.push_back(vecPnt.m_x);
				vecVertices.push_back(vecPnt.m_y);
				vecVertices.push_back(vecPnt.m_z);
			}

			pMeshPrim->SetVertexs(vecVertices);

			i = 0;
			iNumObject = bodyMeshDataNormalArray->size();
			for (; i < iNumObject; i++)
			{
				Vector3& vecPnt = (bodyMeshDataNormalArray->at(i));
				vecNormals.push_back(vecPnt.m_x);
				vecNormals.push_back(vecPnt.m_y);
				vecNormals.push_back(vecPnt.m_z);
			}

			pMeshPrim->SetNormals(vecNormals);
			i = 0;
			iNumObject = bodyMeshDataUVArray->size();
			for (; i < iNumObject; i++)
			{
				Vector3& vecPnt = (bodyMeshDataUVArray->at(i));
				vecUVs.push_back(vecPnt.m_x);
				vecUVs.push_back(vecPnt.m_y);
				vecUVs.push_back(0.0);
			}
			pMeshPrim->SetTextuercoordinate(vecUVs);
		}	
	}

	void SVLXWriter::writeLineSet(void* meshPrim, M3D::Body* body)
	{
		HoteamSoft::SVLLib::Stk_MeshPrimPtr& pMeshPrim = *(HoteamSoft::SVLLib::Stk_MeshPrimPtr*)meshPrim;

	}

	void SVLXWriter::writeMeshFace(void* meshPrim, M3D::Face* face)
	{
		HoteamSoft::SVLLib::Stk_MeshPrimPtr& pMeshPrim = *(HoteamSoft::SVLLib::Stk_MeshPrimPtr*)meshPrim;

		HoteamSoft::SVLLib::Stk_MeshFacePtr pMeshFace = pMeshPrim->AddMeshFace(face->GetSVLId());
		
		M3D::Mesh* faceData = face->GetData();
		vector<M3D_INDEX_TYPE>* bodyMeshDataIndexArray = faceData->GetRefMesh()->GetIndexArray();
		vector<STK_UINT32> facetriangles;
		if (faceData->GetRefMesh()->IsUseIndex())
		{
			for (int j = faceData->GetDataOffset(); j < faceData->GetDataOffset() + faceData->GetDataLength(); j++)
			{
				M3D_INDEX_TYPE faceIndex = bodyMeshDataIndexArray->at(j);
				facetriangles.push_back(faceIndex);
			}
		}
		pMeshFace->SetIndex(facetriangles);

		if (face->GetMaterial())
		{
			pMeshFace->SetMaterialID(face->GetMaterial()->GetSvlId());
		}
		else
		{
			pMeshFace->SetMaterialID(-1);
		}

		if (face->GetGeoAttribute())
		{
			HoteamSoft::SVLLib::Stk_GeometryAttributePtr pGeoAttr = GeoAttribToStk_GeoAttrib(face->GetGeoAttribute());
			if (pGeoAttr != nullptr) {
				Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
				HoteamSoft::SVLLib::Stk_DocumentPtr& m_pDoc = *m_svl2Doc;
				//TODO 分散存储
				m_pDoc->AddMeshFaceAttribute(face->GetBody()->GetModel()->GetProtoTypeId(), face->GetSVLId(), pGeoAttr);
			}
		}
	}

	void SVLXWriter::writeMeshEdge(void* meshPrim, M3D::Edge* edge)
	{
		HoteamSoft::SVLLib::Stk_MeshPrimPtr& pMeshPrim = *(HoteamSoft::SVLLib::Stk_MeshPrimPtr*)meshPrim;

		HoteamSoft::SVLLib::Stk_MeshEdgePtr pMeshEdge = pMeshPrim->AddMeshEdge(edge->GetSVLId());
		if (pMeshEdge == nullptr)
			return;

		vcEdgeInfo  stkEdgeInfo;
		RefPolyLine* lineData = edge->GetLineData();
		if (lineData)
		{
			SPolyLine* allLineData = lineData->GetRefLine();
			if (allLineData)
			{
				vector<M3D_INDEX_TYPE>& pntIndex = allLineData->GetPntIndex();
				if (pntIndex.size()>0)
				{
					unsigned int lineDataOffset = lineData->GetDataOffset();
					unsigned int lineDataLength = lineData->GetDataLength();

					M3D_INDEX_TYPE* pnts = pntIndex.data() + lineDataOffset;
					if (lineDataLength >= 2) {
						stkEdgeInfo.push_back(pnts[0]);
						Vector3 intersectPos;
						//由于点在读入的过程中做了特殊处理，这里求真正点的个数
						int realPntNum = lineDataLength;
						for (int i = 1; i < realPntNum - 1; i += 2)
						{
							stkEdgeInfo.push_back(pnts[i]);
						}
						stkEdgeInfo.push_back(pnts[lineDataLength - 1]);
					}
				}
			}
			pMeshEdge->SetIndex(stkEdgeInfo);
		}
	
		if (edge->GetGeoAttribute())
		{
			HoteamSoft::SVLLib::Stk_GeometryAttributePtr pGeoAttr = GeoAttribToStk_GeoAttrib(edge->GetGeoAttribute());
			if (pGeoAttr != nullptr) {
				Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
				HoteamSoft::SVLLib::Stk_DocumentPtr& m_pDoc = *m_svl2Doc;
				//TODO  分散存储
				m_pDoc->AddMeshEdgeAttribute(edge->GetBody()->GetModel()->GetProtoTypeId(), edge->GetSVLId(), pGeoAttr);
			}
		}
	}

	void SVLXWriter::SaveView()
	{
		if (!m_View ||
			!this->m_svl2Doc)
			return;
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		Model* topModel = m_View->GetModel();
		if (!topModel)
			return;
		SaveUserDefineViews(topModel);
	}
//    string SVLXWriter::SaveViewAndAnnotation(View* view,const string& srcFilePath)
//    {
//        string retstr = "";
//        m_View = view;
//        Model* topModel = view->GetSceneManager()->GetModel();
//        string topModelName = topModel->GetName();
//        vector<Model*> savedModels;
//
//        if (topModelName == "Root")
//        {
//            for (int i = 0; i < topModel->GetChildren().size(); i++)
//            {
//                M3D::Model* subModel = topModel->GetChildren().at(i);
//                savedModels.push_back(subModel);
//            }
//        }
//        else
//        {
//            savedModels.push_back(topModel);
//        }
//
//        this->SetSavedModel(savedModels);
//        Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
//        *m_svl2Doc = Stk_DocumentPtr::CreateObject();
//        HoteamSoft::SVLLib::Stk_DocumentPtr& m_pDoc = *m_svl2Doc;
//        string strFilePath = FileHelper::GetDirName(srcFilePath);
//        wstring wstrSVLXOutFilePath = Platform::StringToWString(srcFilePath,"auto");
//
//        wstring wstrExt = wstrSVLXOutFilePath.substr(wstrSVLXOutFilePath.size() - 5, 5);
//        if (wstrExt == L".svlx" || wstrExt == L".SVLX")
//            wstrSVLXOutFilePath = wstrSVLXOutFilePath.substr(0, wstrSVLXOutFilePath.length() - 5);
//
//        m_pDoc->SetOutputFullPath(wstrSVLXOutFilePath);
//        wstrSVLXOutFilePath = m_pDoc->GetOutputFullPath();
//
//        string strSaveFileName = Platform::WStringToString(m_pDoc->GetOutputName(), "auto");
//        string strSVLXOutFilePath = Platform::WStringToString(wstrSVLXOutFilePath, "auto");
//        //将目录不能编辑的内容解压，然后重新压缩进SVLX中
//        if (srcFilePath.size() > 0)
//        {
//            m_pDoc->OpenFile(Platform::StringToWString(srcFilePath, "auto"));
//            //移动端的视图文件
//            string strViewFileName = strSaveFileName;
//            strViewFileName.append(".view");
//            string strViewFile = strFilePath;
//            strViewFile.append(strViewFileName);
//
//            //保存Bom
//            //根据名称判断顶级是否是多实例
//            //string topModelName = topModel->GetName();
//            vector<Model*>& savedModels = this->GetSavedModels();
//            if (savedModels.size() >1)
//            {
//                Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
//                HoteamSoft::SVLLib::Stk_DocumentPtr& m_pDoc = *m_svl2Doc;
//
//                for (int i = 0;i<savedModels.size();i++)
//                {
//                    M3D::Model* subModel = savedModels.at(i);
//                    HoteamSoft::SVLLib::Stk_InstancePtr pNewTopInstance = m_pDoc->AddTopInstance(subModel->GetSVLId());
//                    WriteTopInstance(subModel, &pNewTopInstance);
//                }
//            }
//            else if(savedModels.size() == 1)
//            {
//                Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
//                HoteamSoft::SVLLib::Stk_DocumentPtr& m_pDoc = *m_svl2Doc;
//
//                M3D::Model* subModel = savedModels.at(0);
//                HoteamSoft::SVLLib::Stk_InstancePtr pNewTopInstance = m_pDoc->AddTopInstance(subModel->GetSVLId());
//                WriteTopInstance(subModel, &pNewTopInstance);
//            }
//
//            SaveView();
//
//            int status = m_pDoc->WriteViewFile(Platform::StringToWString(strViewFile,"auto"), strViewFileName);
//            if (status == 0) {
//                retstr = strViewFile;
//            }
//            //移动端的批注文件
//            string strAnnoFileName = strSaveFileName;
//            strAnnoFileName.append(".annotation");
//            string strAnnoFile = strFilePath;
//            strAnnoFile.append(strAnnoFileName);
//            SaveAnnotations(strAnnoFile);
//            string strAnnoFilePath = strSVLXOutFilePath;
//            m_pDoc->GetSVLXFileItemToFile(".annotation", strAnnoFilePath, true, strAnnoFileName);
//            m_pDoc->AddSVLXFileItem(strAnnoFileName);
//        }
//        m_pDoc = nullptr;
//        return retstr;
//    }
	void SVLXWriter::WriteAllProtoTypes(M3D::Model* model)
	{
		//vector<M3D::Model*> allSubModels;
		//model->GetAllSubModels(allSubModels);

		//map<StringHash, Model*> allProtoTypesModel;

		//for (int i=0;i<allSubModels.size();i++)
		//{
		//	allProtoTypesModel.insert(std::pair<StringHash, Model*>(this->GetModelProtypeCode(model), model));
		//}

		//map<StringHash, Model*>::iterator it = allProtoTypesModel.begin();
		//
		//for (; it!= allProtoTypesModel.end();++it)
		//{
		//	this->WriteProtoType(it->second);
		//}
	}

	M3D::StringHash SVLXWriter::GetModelProtypeCode(M3D::Model* model)
	{
		int key = 0;
		string strKey = "";
		//原型ID
		int protoTypeId = model->GetProtoTypeId();
		//Mesh的个数
		int bodyNum = 0;
		int faceNum = 0;
		int edgeNum = 0;

		vector<Body*> *bodies = model->GetBodys();
		if (bodies)
		{
			bodyNum = model->GetBodys()->size();
			for (int i =0;i<bodies->size();i++)
			{
				Body* body = bodies->at(i);
				faceNum += body->GetFaces().size();
				edgeNum += body->GetEdges()->size();
			}
		}
		strKey += M3D::StringHelper::IntToString(protoTypeId);
		strKey += M3D::StringHelper::IntToString(bodyNum);
		strKey += M3D::StringHelper::IntToString(faceNum);
		strKey += M3D::StringHelper::IntToString(edgeNum);

		return M3D::StringHash(key);
	}

	int SVLXWriter::SaveTo(View* view, const string& srcPath, const string &targetPath)
	{
		CSVLVersionConvertIF::SVL1ToSVL2(srcPath, targetPath);
		return 0;
	}

	int  SVLXWriter::SaveTo(View* view,vector<Model*>& models, const string &targetPath)
	{
		int writeSate = 0;

		this->SetSavedModel(models);
		this->UpdateMaterialsSet();

		return WriteSavedModels(view, "", targetPath);
	}

	int SVLXWriter::SaveLights(const string& destFilePath)
	{
		int writeSate = 0;

		Json::Value root;
		Json::FastWriter writer;
		Json::Value jsonLights;
		SceneManager* scene = this->m_View->GetSceneManager();
		LightManager* lightMgr = scene->GetLightManager();

		vector<BaseLight*>& alllights = lightMgr->GetAllLight();
		for (int i = 0;i<alllights.size();i++)
		{
			BaseLight* baseLight = alllights.at(i);
			Json::Value jsonBaseLight;
			jsonBaseLight["id"] = baseLight->GetID();
			jsonBaseLight["name"]= baseLight->GetName();
			jsonBaseLight["Intensity"] =baseLight->GetIntensity();

			Color lightColor = baseLight->GetLightColor();
			Json::Value jsonBaseLightColor;
			jsonBaseLightColor[0] = lightColor.m_r;
			jsonBaseLightColor[1] = lightColor.m_g;
			jsonBaseLightColor[2] = lightColor.m_b;
			jsonBaseLightColor[3] = lightColor.m_a;

			jsonBaseLight["lightColor"] = jsonBaseLightColor;

			jsonBaseLight["castShadow"] = baseLight->CastShadow();
			jsonBaseLight["showSimpleSign"] = baseLight->GetShowSimpleSign();
			jsonBaseLight["showAllSign"] = baseLight->GetShowAllSign();

			jsonBaseLight["type"] = baseLight->GetLightSourceType();

			if (baseLight->GetLightSourceType() == LightType::LightType_Directional)
			{

			}
			else if (baseLight->GetLightSourceType() == LightType::LightType_Point)
			{
				PointLight* pointLight = static_cast<PointLight*>(baseLight);
				jsonBaseLight["decay"] = pointLight->Decay();
				jsonBaseLight["distance"] = pointLight->Distance();
			}
			else if (baseLight->GetLightSourceType() == LightType::LightType_Spot)
			{
				SpotLight* pointLight = static_cast<SpotLight*>(baseLight);
				jsonBaseLight["decay"] = pointLight->Decay();
				jsonBaseLight["distance"] = pointLight->Distance();
				jsonBaseLight["angle"] = pointLight->Angle();
				jsonBaseLight["penumbra"] = pointLight->Penumbra();
			}
			else if (baseLight->GetLightSourceType() == LightType::LightType_Hemisphere)
			{
				HemisphereLight* pointLight = static_cast<HemisphereLight*>(baseLight);
				Json::Value jsonBaseLightColor;

				Color lightColor = pointLight->GroundColor();
				jsonBaseLightColor[0] = lightColor.m_r;
				jsonBaseLightColor[1] = lightColor.m_g;
				jsonBaseLightColor[2] = lightColor.m_b;
				jsonBaseLightColor[3] = lightColor.m_a;

				//地面颜色
				jsonBaseLight["groundColor"] = jsonBaseLightColor;
			}
			jsonLights[i] = jsonBaseLight;
		}

		string outJsonLights = writer.write(jsonLights);

		writeSate = FileHelper::SaveFileAssAscii(outJsonLights,destFilePath);

		return writeSate;
	}
    int SVLXWriter::SaveAnnotations(const string& destFilePath)
    {
        int writeSate = 0;
        
        Json::Value root;
        Json::FastWriter writer;
        Json::Value jsonNotes;
        SceneManager* scene = this->m_View->GetSceneManager();
        NoteGroup* noteGroup = scene->GetNoteGroup();
        int index = 0;
        if (noteGroup->Size() > 0)
        {
            LOGI("begin update textnote by current scene");
            int noteCount = noteGroup->Size();
            LOGI("noteCount %d", noteCount);
            
            for (int i = 0; i < noteCount; i++)
            {
                SceneNode* node = noteGroup->GetChild(i);
                if (node && node->GetType() == SHAPE_NODE)
                {
                    ShapeNode* shapeNode = (ShapeNode*)node;
                    IShape* shape = shapeNode->GetShape();
                    if(!shape)
                        continue;
                    
                    if (shape->GetType() == SHAPE_TEXT_NOTE)
                    {
                        Json::Value tjson;
                        TextNote* pNote = (TextNote *)shape;
                        tjson["createID"] = pNote->GetID();
                        tjson["type"] = NoteGroup::ANNOTATION_TYPE_BASE;
                        tjson["text"] = pNote->GetTextValue();
                        tjson["leaderLine"] = true;
                        tjson["envelope"] = true;
                        tjson["stub"] = true;
                        tjson["fixed"] = false;
                        
                        Json::Value frameColor;
                        frameColor.append(0.0);
                        frameColor.append(0.0);
                        frameColor.append(0.0);
                        frameColor.append(1.0);
                        tjson["frameColor"] = frameColor;
                        
                        Json::Value fillColor;
                        fillColor.append(1.0);
                        fillColor.append(1.0);
                        fillColor.append(1.0);
                        fillColor.append(1.0);
                        tjson["fillColor"] = fillColor;
                        
                        Json::Value textPosjson;
                        Vector3 tPos = pNote->GetTextsPos();
                        textPosjson.append(tPos.m_x);
                        textPosjson.append(tPos.m_y);
                        textPosjson.append(tPos.m_z);
                        tjson["annotationPos"] = textPosjson;
                        
                        Json::Value notePosjson;
                        Vector3 nPos = pNote->GetNotePos();
                        notePosjson.append(nPos.m_x);
                        notePosjson.append(nPos.m_y);
                        notePosjson.append(nPos.m_z);
                        tjson["centerPos"] = notePosjson;
                        
                        jsonNotes[index] = tjson;
                        index++;
                    }
                    else if (shape->GetType() == SHAPE_VOICE_NOTE)
                    {
                        LOGI("UpdateViewByCurrentScene::SHAPE_VOICE_NOTE");
//                        VoiceNote* pNote = (VoiceNote *)shape;
                        
                    }
                    else if (shape->GetType() == SHAPE_SEQUENCE_NUMBER_NOTE)
                    {
                        LOGI("UpdateViewByCurrentScene::SHAPE_SEQUENCE_NUMBER_NOTE");
                        SequenceNumberNote* pNote = (SequenceNumberNote *)shape;
                        Json::Value tjson;
                        tjson["createID"] = pNote->GetID();
                        tjson["type"] = NoteGroup::ANNOTATION_TYPE_SEQUENCE;
                        tjson["text"] = pNote->GetTextValue();
                        tjson["leaderLine"] = true;
                        tjson["envelope"] = true;
                        tjson["stub"] = true;
                        tjson["fixed"] = false;
                        
                        Json::Value frameColor;
                        frameColor.append(0.0);
                        frameColor.append(0.0);
                        frameColor.append(0.0);
                        frameColor.append(1.0);
                        tjson["frameColor"] = frameColor;
                        
                        Json::Value fillColor;
                        fillColor.append(1.0);
                        fillColor.append(1.0);
                        fillColor.append(1.0);
                        fillColor.append(1.0);
                        tjson["fillColor"] = fillColor;
                        
                        Json::Value textPosjson;
                        Vector3 tPos = pNote->GetTextsPos();
                        textPosjson.append(tPos.m_x);
                        textPosjson.append(tPos.m_y);
                        textPosjson.append(tPos.m_z);
                        tjson["annotationPos"] = textPosjson;
                        
                        Json::Value notePosjson;
                        Vector3 nPos = pNote->GetNotePos();
                        notePosjson.append(nPos.m_x);
                        notePosjson.append(nPos.m_y);
                        notePosjson.append(nPos.m_z);
                        tjson["centerPos"] = notePosjson;
                        
                        jsonNotes[index] = tjson;
                        index++;
                    }
                    else if (shape->GetType() == SHAPE_THREED_GESTURE_NOTE)
                    {
        
                        ThreeDGesturesNote* pNote = (ThreeDGesturesNote *)shape;
                        //在此处填充View中使用的TextNote ID。//TODO
                        Json::Value jsonValue;
                        jsonValue["createID"] = pNote->GetID();
                        jsonValue["type"] = NoteGroup::ANNOTATION_TYPE_GESTURE;
                        jsonValue["text"] = pNote->GetTextValue();
                        jsonValue["leaderLine"] = true;
                        jsonValue["envelope"] = true;
                        jsonValue["stub"] = true;
                        jsonValue["fixed"] = false;
                        jsonValue["spacepoint"] = pNote->GetIsShowSpacePoint();

                        Json::Value frameColor;
                        frameColor.append(0.0);
                        frameColor.append(0.0);
                        frameColor.append(0.0);
                        frameColor.append(1.0);
                        jsonValue["frameColor"] = frameColor;

                        Json::Value fillColor;
                        fillColor.append(1.0);
                        fillColor.append(1.0);
                        fillColor.append(1.0);
                        fillColor.append(1.0);
                        jsonValue["fillColor"] = fillColor;

                        Json::Value linesValue;
                        vector<Line3D*>& line3DSet = pNote->m_LineList;
                        if (line3DSet.size() > 0)
                        {
                            int allSize = pNote->GetOriginalProjectPns().size();
                            for (int i = 0; i < allSize; i = i + 1)
                            {
                                Json::Value lineValue;
                                Color& originalProjectColor =
                                pNote->GetOriginalProjectColors().at(i);
                                int originalProjectWidth =
                                pNote->GetOriginalProjectWidths().at(i);
                                lineValue["width"] = originalProjectWidth;
                                Json::Value color;
                                color.append(originalProjectColor.m_r);
                                color.append(originalProjectColor.m_g);
                                color.append(originalProjectColor.m_b);
                                color.append(originalProjectColor.m_a);
                                lineValue["color"] = color;

                                Json::Value pointsValue;
                                vector<Vector3>& originalProjectPnts = pNote->GetOriginalProjectPns().at(i);
                                for (int j = 0; j < originalProjectPnts.size(); j++)
                                {
                                    Json::Value point;
                                    point.append(originalProjectPnts.at(j).m_x);
                                    point.append(originalProjectPnts.at(j).m_y);
                                    point.append(originalProjectPnts.at(j).m_z);
                                    pointsValue.append(point);
                                }
                                lineValue["points"] = pointsValue;
                                linesValue.append(lineValue);
                            }
                        }
                        jsonValue["lines"] = linesValue;
                        jsonNotes[index] = jsonValue;
                        index++;
                    }
                    
                }
            }
        }
        
        //文本批注
        AnnotationGroup* pAnnotationGroup = scene->GetAnnotationGroup();
        if (pAnnotationGroup && pAnnotationGroup->Size() > 0)
        {
            int iAnnotationCount = pAnnotationGroup->Size();
            for (int i = 0; i < iAnnotationCount; i++)
            {
                SceneNode* node = pAnnotationGroup->GetChild(i);
                if (node && node->GetType() == SHAPE_NODE)
                {
                    ShapeNode* shapeNode = (ShapeNode*)node;
                    IShape* shape = shapeNode->GetShape();
                    if (shape && node->IsVisible() && shape->IsVisible())
                    {
                        
                    }
                }
            }
        }
        root["annotations"] = jsonNotes;
        string outJsonNotes = writer.write(root);
        
        writeSate = FileHelper::SaveFileAssAscii(outJsonNotes,destFilePath);
        
        return writeSate;
    }
    int SVLXWriter::SaveMeasures(const string& destFilePath)
    {
        int writeSate = 0;
        
        Json::Value root;
        Json::FastWriter writer;
        Json::Value jsonLights;
        SceneManager* scene = this->m_View->GetSceneManager();
        
        string outJsonLights = writer.write(jsonLights);
        
        writeSate = FileHelper::SaveFileAssAscii(outJsonLights,destFilePath);
        
        return writeSate;
    }

	int SVLXWriter::WriteMaterials(const string& destFilePath)
	{
		int writeSate = 0;

		return writeSate;
	}


	int SVLXWriter::SaveToByIncrease(View* view, const string& tempPath, const string &strsrcPath,const string &strsrcName)
	{
		m_View = view;
		m_tempPath = tempPath;
		if (m_View)
		{
			Stk_DocumentPtr& m_svl2Doc = *(Stk_DocumentPtr*)this->m_svl2Doc;
			m_svl2Doc = Stk_DocumentPtr::CreateObject();

			string filePath  = FileHelper::GetUnionStylePath(strsrcPath);
			wstring wfilePath = Platform::StringToWString(filePath, "auto");
			//打开文件
			(m_svl2Doc)->OpenFile(wfilePath);

			(m_svl2Doc)->LoadBom();

			//加载材质信息
			(m_svl2Doc)->LoadMaterial();

			string strTempPath = tempPath;
			//使用传进来的文件名
			strTempPath +=("/"+ strsrcName);
			//保存材质
			SaveMaterial();
			UpdateSVLXFile();

			wstring wtempPath = Platform::StringToWString(strTempPath, "auto");
			(m_svl2Doc)->WriteMaterialFile(wtempPath + L".material");
			(m_svl2Doc)->WriteBomFile(wtempPath + L".bom");

			(m_svl2Doc)->UnloadMaterial();
			(m_svl2Doc)->UnloadBom();
		}

		return -1;
	}

	void SVLXWriter::SaveMaterial(Model* model, void* instancePtr)
	{
		if (m_View)
		{
			Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
			Stk_InstancePtr instance = *(Stk_InstancePtr*)instancePtr;
			if (model && model->GetColor())
			{
				Color& difuseColor = *(model->GetColor());
				Stk_MaterialPtr materialPtr = (*m_svl2Doc)->GetMaterialByID(instance->GetMaterialID());

				if (materialPtr.isNotNull())
				{
					StkMaterialTypeEnum materialType = materialPtr->GetMaterialType();
					if (materialType == StkMaterialTypeEnum::STK_MATERIALTYPE_SIMPLAE)
					{
						if (difuseColor.m_a < 0)
						{
							difuseColor.m_a = 1.0f;
						}

						//目前仅支持修改颜色
						materialPtr->SetDiffuseColor(&(difuseColor.m_r));
						materialPtr->SetTransparency(difuseColor.m_a);
					}
					else
					{
						//StkMaterialTypeEnum materialType = materialPtr->GetMaterialType();
						//wstring wname = materialPtr->GetMaterialName();
						//string name = Platform::WStringToString(wname);
						//string materialIDStr = GetSVLMaterialID(this->m_M3DModel, materialId);
						////将材质加入全局资源管理器中，进行资源管理
						//material = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateMaterial(name);
						//if (!material->GetNeedsUpdateUniformParamerers())
						//{
						//	return false;
						//}

						//vector<int> * vec = new vector<int>();
						//vec->push_back(materialType);
						//Uniform uniform("Int", (void*)vec);
						//material->SetUniformParameter("type", uniform);
						//ParseMaterialParameters(materialPtr, material);
					}
				}
				else
				{
					bool bHasSet = false;
					materialPtr = (*m_svl2Doc)->AddMaterial(bHasSet);
					materialPtr->SetMaterialType(HoteamSoft::SVLLib::StkMaterialTypeEnum::STK_MATERIALTYPE_SIMPLAE);
					materialPtr->SetDiffuseColor(&(difuseColor.m_r));
					materialPtr->SetTransparency(difuseColor.m_a);
					instance->SetMaterialID(materialPtr->GetID());
				}
			}
		}
	}

	bool SVLXWriter::SaveTexture2D(Texture2D* texture2d, void* stktexturePtr)
	{
		Texture2D* diffuseMap = (Texture2D*)texture2d;
		if (!diffuseMap)
		{
			return false;
		}

		return SaveImage(texture2d->GetImage(),stktexturePtr);
	}

	bool SVLXWriter::SaveImage(Image* image, void* stktexturePtr)
	{
		Image* diffuseImage = image;
		if (!diffuseImage)
		{
			return false;
		}
		string imagePath = diffuseImage->GetPath();

		imagePath = FileHelper::GetUnionStylePath(imagePath);

		bool mainSVLXFile = StringHelper::Endswith(imagePath, "||0");

		//根据需要去掉后缀
		if (imagePath.find("||") != string::npos)
		{
			imagePath = StringHelper::subString(imagePath, 0, imagePath.rfind("||"));
		}

		string strPicPath = "";
		string strOutputFullPath = "";
		string imageExt = FileHelper::GetExt(imagePath);

		Stk_DocumentPtr& svl2Doc = *(Stk_DocumentPtr*)this->m_svl2Doc;
		strOutputFullPath = Platform::WStringToString(svl2Doc->GetOutputFullPath(), "auto");

		string strImageDir = strOutputFullPath + "/images";
		if (!FileHelper::FileExist(strImageDir))
		{
			FileHelper::make_dir(strImageDir);
		}

		//判断是不是SVLX主文件
		if (mainSVLXFile)
		{
			strPicPath.append("images/");
			strPicPath.append(FileHelper::GetFileName(imagePath));
			//strPicPath.append("_DiffuseTexture.");
			//strPicPath.append(imageExt);
			strOutputFullPath.append(strPicPath);
		}
		else
		{
			strPicPath.append("images/Material_");
			string strMaterial = StringHelper::IntToString(diffuseImage->GetID());

			strPicPath.append(strMaterial);
			strPicPath.append("_DiffuseTexture.");
			strPicPath.append(imageExt);
			strOutputFullPath.append(strPicPath);
		}

		//pStkPicture->GetData(iBufSize, chBuf);

		if (!FileHelper::FileExist(strOutputFullPath))
		{
			FileHelper::SaveFileAsBin((char*)diffuseImage->m_pPixelData, diffuseImage->m_dataLength, strOutputFullPath);
		}

		bool bHasSet;
		HoteamSoft::SVLLib::Stk_ImagePtr imagePtr = svl2Doc->AddImage(bHasSet, strPicPath);
		//imagePtr->SetNC(pStkPicture->GetNC());
		//imagePtr->SetActualHeight(pStkPicture->GetActualHeight());
		//imagePtr->SetActualWidth(pStkPicture->GetActualWidth());
		//imagePtr->SetDisplayHeight(pStkPicture->GetDisplayHeight());
		//imagePtr->SetDisplayWidth(pStkPicture->GetDisplayWidth());
		//imagePtr->SetPictureSaveStyle(HoteamSoft::SVLLib::StkPicSaveStyleEnum::PICTRE_SAVE_STYLE_OUTER);
		imagePtr->SetPictureSavePath(Platform::StringToWString(strPicPath));
		imagePtr->SetScaleX(1.0f);
		imagePtr->SetScaleY(1.0f);
		HoteamSoft::SVLLib::Stk_TexturePtr& texturePtr = *(HoteamSoft::SVLLib::Stk_TexturePtr*)stktexturePtr;//HoteamSoft::SVLLib::Stk_TexturePtr::CreateObject();

		texturePtr->SetTextureImage(imagePtr);
		texturePtr->SetRotateAngle(0.0f);
		texturePtr->SetUOffset(0.0f);
		texturePtr->SetVOffset(0.0f);
		texturePtr->SetUScale(1.0f);
		texturePtr->SetVScale(1.0f);
		texturePtr->SetFlipFlag(false, false);
		texturePtr->SetType(HoteamSoft::SVLLib::StkTextureMappingTypeEnum::MAPPINT_TYPE_STORED_UV);

		//materialPtr->SetDiffuseTexture(texturePtr);
		//m_mapTexturePtr.insert(make_pair(pStkTexture, texturePtr));

		return true;
	}

	bool SVLXWriter::SavePhoneTexture(Material* phoneMaterial, void* stkMaterial)
	{
		Stk_MaterialPtr& materialPtr = *(Stk_MaterialPtr*)stkMaterial;
		bool hastexture = false;
		Texture2D* diffuseMap = (Texture2D*)phoneMaterial->GetDiffuseMap();

		if (diffuseMap)
		{
			HoteamSoft::SVLLib::Stk_TexturePtr texturePtr = HoteamSoft::SVLLib::Stk_TexturePtr::CreateObject();
			SaveTexture2D(diffuseMap, &texturePtr);
			materialPtr->SetDiffuseTexture(texturePtr);
			hastexture = true;
		}

		{
			Texture2D* normalMap = static_cast<Texture2D*>(phoneMaterial->GetNormalMap());
			if (normalMap)
			{
				HoteamSoft::SVLLib::Stk_TexturePtr texturePtr = HoteamSoft::SVLLib::Stk_TexturePtr::CreateObject();
				SaveTexture2D(normalMap, &texturePtr);
				materialPtr->SetNormalMap(texturePtr);
				hastexture = true;
			}

			materialPtr->SetNormalScale(&(phoneMaterial->NormalMapScale().m_x));
		}

		if (hastexture)
		{
			Vector2 temp;
			temp = phoneMaterial->GetUvTranslate();
			materialPtr->SetUvTranslate(&(temp.m_x));
			temp = phoneMaterial->GetUvScale();
			materialPtr->SetUvScale(&(temp.m_x));

			materialPtr->SetUvRotate(phoneMaterial->GetUvRotate());
		}
	
		return true;
	}

	bool SVLXWriter::SavePBRTexture(PbrMaterial* pbrMaterial, void* stkMaterial)
	{
		Stk_MaterialPtr& materialPtr = *(Stk_MaterialPtr*)stkMaterial;
		bool hastexture = false;

		{
			Texture2D* diffuseMap = static_cast<Texture2D*>(pbrMaterial->AlbedoMap());

			if (diffuseMap)
			{
				HoteamSoft::SVLLib::Stk_TexturePtr texturePtr = HoteamSoft::SVLLib::Stk_TexturePtr::CreateObject();
				SaveTexture2D(diffuseMap, &texturePtr);
				materialPtr->SetDiffuseTexture(texturePtr);
				hastexture = true;
			}
		}


		{
			Texture2D* roughmetalMap = static_cast<Texture2D*>(pbrMaterial->MetalnessRoughnessMap());
			if (roughmetalMap)
			{
				HoteamSoft::SVLLib::Stk_TexturePtr texturePtr = HoteamSoft::SVLLib::Stk_TexturePtr::CreateObject();
				SaveTexture2D(roughmetalMap, &texturePtr);
				materialPtr->SetMetalnessRoughnessMap(texturePtr);
				hastexture = true;
			}
		}

		{
			Texture2D* normalMap = static_cast<Texture2D*>(pbrMaterial->NormalMap());
			if (normalMap)
			{
				HoteamSoft::SVLLib::Stk_TexturePtr texturePtr = HoteamSoft::SVLLib::Stk_TexturePtr::CreateObject();
				SaveTexture2D(normalMap, &texturePtr);
				materialPtr->SetNormalMap(texturePtr);
				hastexture = true;
			}
			materialPtr->SetNormalScale(&(pbrMaterial->NormalMapScale().m_x));
		}

		if (hastexture)
		{
			Vector2 temp;
			temp = pbrMaterial->GetUvTranslate();
			materialPtr->SetUvTranslate(&(temp.m_x));
			temp = pbrMaterial->GetUvScale();
			materialPtr->SetUvScale(&(temp.m_x));
			
			materialPtr->SetUvRotate(pbrMaterial->GetUvRotate());
		}

		return true;
	}

	bool SVLXWriter::SaveMatCapTexture(MatCapMaterial* matcapMaterial, void* stkMaterial)
	{
		Stk_MaterialPtr& materialPtr = *(Stk_MaterialPtr*)stkMaterial;
		bool hastexture = false;
		Texture2D* matcapMap = (Texture2D*)matcapMaterial->MatcapMap();

		if (matcapMap)
		{
			HoteamSoft::SVLLib::Stk_TexturePtr texturePtr = HoteamSoft::SVLLib::Stk_TexturePtr::CreateObject();
			SaveTexture2D(matcapMap, &texturePtr);
			materialPtr->SetMatcapMap(texturePtr);
			hastexture = true;
		}

		{
			Texture2D* normalMap = static_cast<Texture2D*>(matcapMaterial->GetNormalMap());
			if (normalMap)
			{
				HoteamSoft::SVLLib::Stk_TexturePtr texturePtr = HoteamSoft::SVLLib::Stk_TexturePtr::CreateObject();
				SaveTexture2D(normalMap, &texturePtr);
				materialPtr->SetNormalMap(texturePtr);
				hastexture = true;
			}

			materialPtr->SetNormalScale(&(matcapMaterial->NormalMapScale().m_x));
		}

		//if (hastexture)
		//{
		//	Vector2 temp;
		//	temp = matcapMaterial->GetUvTranslate();
		//	materialPtr->SetUvTranslate(&(temp.m_x));
		//	temp = matcapMaterial->GetUvScale();
		//	materialPtr->SetUvScale(&(temp.m_x));

		//	materialPtr->SetUvRotate(matcapMaterial->GetUvRotate());
		//}

		return true;
	}

	bool SVLXWriter::SaveShaderTexture(ShaderMaterial* pbrMaterial, void* stkMaterial)
	{
		Stk_MaterialPtr& materialPtr = *(Stk_MaterialPtr*)stkMaterial;

		/*Texture2D* diffuseMap = (Texture2D*)phoneMaterial->GetDiffuseMap();
		if (!diffuseMap)
		{
			return false;
		}

		Image* diffuseImage = diffuseMap->GetImage();
		if (!diffuseImage)
		{
			return false;
		}*/

		return true;
	}


	//保存材质数据
	void SVLXWriter::SaveMaterial()
	{
		//遍历所有材质对象
		//ResourceManager* materials = m_View->GetSceneManager()->GetResourceManager();
		//map<string, BaseMaterial *>mapMaterials =  materials->GetAllMaterials();
		//map<string, BaseMaterial *>::iterator it;
		std::set<BaseMaterial *>::iterator it = m_baseMaterials.begin();
		while (it != m_baseMaterials.end())
		{
			BaseMaterial* material = *it;
			Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
			Stk_MaterialPtr materialPtr = (*m_svl2Doc)->GetMaterialByID(material->GetSvlId());
			if (materialPtr.isNull())
			{
				//创建新的材质对象
				bool bHasset = false;
				materialPtr = (*m_svl2Doc)->AddMaterial(bHasset);
				material->SetSvlId(materialPtr->GetID());
			}
			materialPtr->SetMaterialType((StkMaterialTypeEnum)material->GetMaterialType());
			//判断材质类型
			switch (material->GetMaterialType())
			{
				case MaterialType_Phong:
				{
					float fColor[3];
					Color color;
					Material* phongMaterial = ((Material*)material);
					//Ambient
					color = phongMaterial->GetAmbient();
					fColor[0] = color.m_r;
					fColor[1] = color.m_g;
					fColor[2] = color.m_b;
					materialPtr->SetAmbientColor(fColor);
					//Diffuse
					color = phongMaterial->GetDiffuse();
					fColor[0] = color.m_r;
					fColor[1] = color.m_g;
					fColor[2] = color.m_b;
					materialPtr->SetDiffuseColor(fColor);
					materialPtr->SetTransparency(((Material*)material)->Opacity());
					//Emissive
					color = phongMaterial->GetEmissive();
					fColor[0] = color.m_r;
					fColor[1] = color.m_g;
					fColor[2] = color.m_b;
					materialPtr->SetEmissiveColor(fColor);
					//Specular
					color = phongMaterial->GetSpecular();
					fColor[0] = color.m_r;
					fColor[1] = color.m_g;
					fColor[2] = color.m_b;
					materialPtr->SetSpecularColor(fColor);
					//Shininess		
					materialPtr->SetShininess(phongMaterial->GetShininess());

					

					//materialPtr->set

					SavePhoneTexture(phongMaterial, &materialPtr);
				}
				break;
				case MaterialType_Pbr:
				{
					float fColor[3];
					Color color;
					//Diffuse
					color = ((PbrMaterial*)material)->AlbedoColor();
					fColor[0] = color.m_r;
					fColor[1] = color.m_g;
					fColor[2] = color.m_b;
					materialPtr->SetDiffuseColor(fColor);
					materialPtr->SetTransparency(((PbrMaterial*)material)->Opacity());
					//Emissive
					color = ((PbrMaterial*)material)->EmissiveColor();
					fColor[0] = color.m_r;
					fColor[1] = color.m_g;
					fColor[2] = color.m_b;
					materialPtr->SetEmissiveColor(fColor);
					//MetalnessFactor
					materialPtr->SetMetalnessFactor(((PbrMaterial*)material)->MetalnessFactor());
					//RougthnessFactor
					materialPtr->SetRoughnessFactor(((PbrMaterial*)material)->RougthnessFactor());

					PbrMaterial* temp = static_cast<PbrMaterial*>(material);
					//clear coat 相关
					bool isUseClearCoat = temp->UseClearCoat();
					materialPtr->SetUseClearCoat(isUseClearCoat);

					if (isUseClearCoat)
					{
						materialPtr->SetClearCoat(temp->ClearCoat());
						materialPtr->SetClearCoatRoughness(temp->ClearCoatRoughness());
					}
					SavePBRTexture(temp, &materialPtr);
				}
				break;
				case MaterialType_MatCap:
				{
					float fColor[3];
					Color color;
					MatCapMaterial* phongMaterial = ((MatCapMaterial*)material);
	
					//Diffuse
					color = phongMaterial->GetDiffuse();
					fColor[0] = color.m_r;
					fColor[1] = color.m_g;
					fColor[2] = color.m_b;
					materialPtr->SetDiffuseColor(fColor);
					materialPtr->SetTransparency(((Material*)material)->Opacity());
					//materialPtr->set

					SaveMatCapTexture(phongMaterial, &materialPtr);
				}
				break;
				case 101:
				case 102:
				case 103:
				case 104:
				case MaterialType_Shader:
				{
					const static string texture2dKey = "Texture2D";
					const static string textureCubeKey = "TextureCube";
					const static string vector3Key = "Vector3";
					const static string vector4Key = "Vector4";
					const static string floatKey = "Float";
					const static string boolKey = "Bool";
					const static string intKey = "Int";

					ShaderMaterial* shaderMaterial = static_cast<ShaderMaterial*>(material);
					Uniform* typeUniform = shaderMaterial->GetUniformParameter("type");

					materialPtr->SetMaterialName(Platform::StringToWString(shaderMaterial->GetName(),"utf8"));

					shaderMaterial->GetUnifomParameters();

					map<string, Uniform>& alluniformParameters = shaderMaterial->GetUnifomParameters();

					map<string, Uniform>::iterator it = alluniformParameters.begin();
					for (; it != alluniformParameters.end(); ++it)
					{
						string uniformName = it->first;
						Uniform& uniform = it->second;
						string& uniformKey = uniform.type;
						if (texture2dKey == uniformKey)
						{
							Texture2D* uniformValue = anyCast<Texture2D*>(uniform.value);

							HoteamSoft::SVLLib::Stk_TexturePtr texturePtr = HoteamSoft::SVLLib::Stk_TexturePtr::CreateObject();
							SaveTexture2D(uniformValue, &texturePtr);
							materialPtr->SetDiffuseTexture(texturePtr);
                            std::pair<string, Stk_TexturePtr> tempPair(uniformKey, texturePtr);
                            materialPtr->SetMaterialParameters(uniformName, tempPair);
						}
						else if (textureCubeKey == uniformKey)
						{
							TextureCube* uniformValue = anyCast <TextureCube*>(uniform.value);

							vector<Image*>& cubeImages = uniformValue->GetCubeImages();
							vector<Stk_TexturePtr> stkTextures;
							for (int i = 0; i < cubeImages.size(); i++)
							{
								Image* iamge = cubeImages.at(i);
								HoteamSoft::SVLLib::Stk_TexturePtr texturePtr = HoteamSoft::SVLLib::Stk_TexturePtr::CreateObject();
								SaveImage(iamge, &texturePtr);
								stkTextures.push_back(texturePtr);
							}
                            std::pair<string, vector<Stk_TexturePtr> > tempPair (uniformKey, stkTextures);
                            materialPtr->SetMaterialParameters(uniformName, tempPair);
						}
						else if (vector3Key == uniformKey)
						{
							Vector3* uniformValue = anyCast<Vector3*>(uniform.value);

							vector<float> diffuseColor(3);
							diffuseColor[0] = uniformValue->m_x;
							diffuseColor[1] = uniformValue->m_y;
							diffuseColor[2] = uniformValue->m_z;

							pair<string, vector<float> > diffuseColorValue(uniformKey, diffuseColor);

							materialPtr->SetMaterialParameters(uniformName, diffuseColorValue);

						}
						else if (vector4Key == uniformKey)
						{
							Vector4* uniformValue = anyCast <Vector4*>(uniform.value);
							vector<float> diffuseColor(4);
							diffuseColor[0] = uniformValue->m_x;
							diffuseColor[1] = uniformValue->m_y;
							diffuseColor[2] = uniformValue->m_z;
							diffuseColor[3] = uniformValue->m_w;

							pair<string, vector<float> > diffuseColorValue(uniformKey, diffuseColor);

							materialPtr->SetMaterialParameters(uniformName, diffuseColorValue);
						}
						else if (floatKey == uniformKey)
						{
							float uniformValue = anyCast <float>(uniform.value);
							vector<float> diffuseColor(1);
							diffuseColor[0] = uniformValue;

							pair<string, vector<float> > diffuseColorValue(uniformKey, diffuseColor);
							materialPtr->SetMaterialParameters(uniformName, diffuseColorValue);
						}
						else if (boolKey == uniformKey)
						{
							int uniformValue = anyCast <int>(uniform.value);
							pair<string, bool> TrueValue(uniformKey, uniformValue);
							materialPtr->SetMaterialParameters(uniformName, TrueValue);
						}
						else if (intKey == uniformKey)
						{
							int uniformValue = anyCast <int>(uniform.value);

							vector<int> diffuseColor(1);
							diffuseColor[0] = uniformValue;

							pair<string, vector<int> > diffuseColorValue(uniformKey, diffuseColor);
							materialPtr->SetMaterialParameters(uniformName, diffuseColorValue);
						}
					}

					string globalEffect = this->m_View->GetSceneManager()->GetRenderManager()->GetGlobalEffect();
					(*m_svl2Doc)->SetGlobalEffect(Platform::StringToWString(globalEffect));

					this->SaveShaderTexture(shaderMaterial, &materialPtr);
					break;
				}
			default:
				break;
			}
			//通用设置
			//name
			wstring materialName = Platform::StringToWString(material->DisplayName(), "auto");
			materialPtr->SetMaterialName(materialName);
			it++;
		}

	}

	/**
     添加文件到SVLX文件中
     
     @param view
     @param strsrcPath 需添加文件的路径
     @param strsvlxPath svlx文件路径
     @param strsrcName 需添加文件的名称（完整-带后缀）
     @return
     */
	int SVLXWriter::AddFileToSVLX(View* view, const string &strsrcPath,const string &strsvlxPath,const string &strsrcName)
	{
		m_View = view;
		if (m_View)
		{
			Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
            *m_svl2Doc = Stk_DocumentPtr::CreateObject();
			wstring wsrcPath = Platform::StringToWString(strsrcPath, "auto");
			wstring wsvlxPath = Platform::StringToWString(strsvlxPath, "auto");
			wstring wsrcName = Platform::StringToWString(strsrcName, "auto");
			return (*m_svl2Doc)->addFileToSVLX(wsrcPath, wsvlxPath, wsrcName);
		}
		return -1;
	}

	void SVLXWriter::SaveUserDefineViews(Model* pModel)
	{
		if (!pModel||
			!m_View ||
			!this->m_svl2Doc)
			return;
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		HoteamSoft::SVLLib::Stk_DocumentPtr& m_pDoc = *m_svl2Doc;
		
		vector<ModelView*>* pVecModelView = pModel->GetModelViewList();
		if (!pVecModelView)
			return;
		HoteamSoft::SVLLib::Stk_ModelPtr modelP = m_pDoc->GetModelByID((int)pModel->GetProtoTypeId());
		if (modelP == nullptr)
			return;
		vector<Stk_ViewPtr> vcStkViews;
		modelP->GetAllView(vcStkViews);
		vcStkViews.clear();
		LOGI("SaveUserDefineViews pVecModelView .size = %d",(int)pVecModelView->size());
		for (int i = 0; i < (int)pVecModelView->size(); i++)
		{
			ModelView* pModelView = (*pVecModelView)[i];

			if (!pModelView)
				continue;
			HoteamSoft::SVLLib::Stk_ViewPtr viewP = HoteamSoft::SVLLib::Stk_ViewPtr::CreateObject();
			viewP->SetID(pModelView->GetID());
			if (pModelView->IsInitView())
			{
				viewP->SetActivated(true);
			}
			else
			{
				viewP->SetActivated(false);
			}
			wstring wstrViewName = Platform::StringToWString(pModelView->GetName(), "auto");
			viewP->SetName(wstrViewName);
			if(pModelView->GetViewType() == ModelView::DefaultView){
			pModelView->SetSvlUseType((int)HoteamSoft::SVLLib::VIEW_USAGE_DEFAULT_VIEW);
			}else if(pModelView->GetViewType() == ModelView::UserView) {
				pModelView->SetSvlUseType((int)HoteamSoft::SVLLib::VIEW_USAGE_SV_USER_VIEW);
			}else if(pModelView->GetViewType() == ModelView::OrignalView){
				pModelView->SetSvlUseType((int)HoteamSoft::SVLLib::VIEW_USAGE_GENERAL_VIEW);
			}

			viewP->SetUsageType((HoteamSoft::SVLLib::StkViewUsageEnum)(pModelView->GetSvlUseType()));



			//viewP->SetTransparency(pModelView->);
			//if (pModelView->m_bCreateClipPlane)
			//{
			//	HoteamSoft::SVLLib::STK_PNT32 pntOrigin;
			//	HoteamSoft::SVLLib::STK_DIR32 dirNormal;
			//	SbVec3f originPnt = pModelDisplay->getOriginPnt();
			//	pntOrigin.X = originPnt[0]; pntOrigin.Y = originPnt[1]; pntOrigin.Z = originPnt[2];
			//	SbPlane plane = pModelDisplay->getPlane();
			//	SbVec3f normal = plane.getNormal();
			//	float fDis = -plane.getDistance(originPnt);
			//	dirNormal.X = normal[0]; dirNormal.Y = normal[1]; dirNormal.Z = normal[2];
			//	HoteamSoft::SVLLib::Stk_ClipPlanePtr pClipPlane = HoteamSoft::SVLLib::Stk_ClipPlanePtr::CreateObject();
			//	pClipPlane->SetOrigin(pntOrigin);
			//	pClipPlane->SetNormal(dirNormal);
			//	pClipPlane->SetDistance(fDis);
			//	viewP->AddPlaneEntity(pClipPlane);
			//}
			//相机信息
			CameraNode& pCamera = (CameraNode&)pModelView->GetCamera();
			HoteamSoft::SVLLib::Stk_CameraPtr cameraP = HoteamSoft::SVLLib::Stk_CameraPtr::CreateObject();
			HoteamSoft::SVLLib::STK_UINT32 ProjectType = 1;
			if (pCamera.IsOrthographic())
			{
				ProjectType = 1;
				float fOrthoWidth;
				float fOrthoHeight;
				pCamera.GetOrthoSize(&fOrthoWidth, &fOrthoHeight);
				cameraP->SetHeight(fOrthoHeight/ pCamera.GetZoom());
			}
			else
			{
				ProjectType = 2;
				if (m_View)
				{
					float fPosTargetDistance = 1.0f;
					BoundingBox box = m_View->GetSceneManager()->GetSceneBox();
					fPosTargetDistance = box.Length()* CAMERA_POSFACTOR;
					cameraP->SetFocalDistance(fPosTargetDistance);
					float fHeight = (float)(fPosTargetDistance * 2.0 * (tanf(pCamera.GetFov()*M_DEGTORAD * 0.5f)/ pCamera.GetZoom()));
					cameraP->SetHeight(fHeight);
				}
			}

			cameraP->SetProjectType(ProjectType);
			Quaternion rotation = pCamera.GetRotation();
			Vector3 position = pCamera.GetPosition();
			Matrix4 sbMatrix(position, rotation, 1.0f);
			HoteamSoft::SVLLib::STK_MTX32 mtxTransform;
			for (size_t m = 0; m < 4; m++)
			{
				for (size_t n = 0; n < 4; n++)
					mtxTransform.PlcMatrix[m][n] = sbMatrix.Data()[n * 4 + m];
			}
            
			//cameraP->SetMatrixFlag(true);
			cameraP->SetMatrix(mtxTransform);
			cameraP->SetAspectRatio(pCamera.GetAspectRatio());
			cameraP->SetNearDistance(pCamera.GetNearClip());
			cameraP->SetFarDistance(pCamera.GetFarClip());

			viewP->SetCamera(cameraP);

			//实例属性
			map<int, InstanceAttribute> mapInstanceAttribute = pModelView->GetInstanceAttributeMap();

			LOGI("SaveUserDefineViews mapInstanceAttribute .size= %d",(int)mapInstanceAttribute.size());
			for (map<int, InstanceAttribute>::const_iterator it = mapInstanceAttribute.begin();
				it != mapInstanceAttribute.end(); it++)
			{
				const InstanceAttribute &curInsAtt = it->second;
				string strPlcPath = PathHelper::SVLPathHexToDec(curInsAtt.path);
				wstring wstrPlcPath = Platform::StringToWString(strPlcPath, "auto");
				if (curInsAtt.hasColor)
				{
					HoteamSoft::SVLLib::STK_RGBA32 newColor;
					Color dispColor = curInsAtt.insColor;
					newColor.Red = dispColor.m_r;
					newColor.Green = dispColor.m_g;
					newColor.Blue = dispColor.m_b;
					newColor.Alpha = dispColor.m_a;
					viewP->AddInsColor(wstrPlcPath, newColor);
				}
				viewP->AddInsDisplayState(wstrPlcPath, curInsAtt.visible ? HoteamSoft::SVLLib::STK_DISPLAY : HoteamSoft::SVLLib::STK_NO_DISPLAY);
				unsigned int idPlacement = curInsAtt.id;
				HoteamSoft::SVLLib::STK_MTX32 newMatrix;
				for (int m = 0; m < 4; m++)
					for (int n = 0; n < 4; n++)
						newMatrix.PlcMatrix[m][n] = curInsAtt.placeMatrix.Data()[m * 4 + n];

				viewP->AddInsPlacement(wstrPlcPath, idPlacement, newMatrix);
			}

			//关联剖面节点
			vector<int> vecSectionPlane = pModelView->GetSectionPlaneIDList();

			LOGI("SaveUserDefineViews vecSectionPlane .size= %d",(int)vecSectionPlane.size());
			for (int j = 0; j < vecSectionPlane.size(); j++)
			{
				viewP->AddConnector(HoteamSoft::SVLLib::ENTITY_TYPE_SECTION, vecSectionPlane[j]);
			}
			//关联PMI节点
			vector<int> vecPMI = pModelView->GetPMIList();
			for (int j = 0; j < vecPMI.size(); j++)
			{
				viewP->AddConnector(HoteamSoft::SVLLib::ENTITY_TYPE_PMI, vecPMI[j]);
			}
			//关联Note节点
			vector<int> vecNote = pModelView->GetNoteList();
			LOGI("SaveUserDefineViews vecNote .size= %d",(int)vecNote.size());
			for (int j = 0; j < vecNote.size(); j++)
			{
				viewP->AddConnector(HoteamSoft::SVLLib::ENTITY_TYPE_NOTE, vecNote[j]);
			}
			modelP->AddView(viewP);
		}

		for (int i = 0; i < pModel->GetSubModelCount(); i++)
		{
			M3D::Model* subModel = pModel->GetSubModels().at(i);
			//递归调用，遍历所有子模型
			SaveUserDefineViews(subModel);
		}
	}
	void SVLXWriter::SaveUserInfo()
	{
		if (m_View)
		{

		}
	}

	bool SVLXWriter::UpdateSVLXFile()
	{
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		Model* topModel = m_View->GetModel();

		int m_svlId = topModel->GetSVLId();

		//遍历缓存的表，将模型树构建出来
		vector<Stk_InstancePtr>& vecTopInstance = (*m_svl2Doc)->getTopInstances();
		int topNodeCount = vecTopInstance.size();
		if (topModel->GetChildren().size() == topNodeCount && topNodeCount > 1)
		{
			for (int i = 0; i < topNodeCount; i++)
			{
				Stk_InstancePtr jsonInstance = vecTopInstance.at(i);
				Model* subModel = topModel->GetChildren().at(i);
				if (subModel)
				{
					//topModel->AddSubModel(subModel);
				}
				UpdateInstance(subModel, &jsonInstance);
			}
		}
		else if (topNodeCount == 1)
		{
			Stk_InstancePtr jsonInstance = vecTopInstance.at(0);
			UpdateInstance(topModel, &jsonInstance);
		}



		return false;
	}

	bool  SVLXWriter::UpdateInstance(Model* model, void* instancePtr)
	{
		Stk_InstancePtr instance = *(Stk_InstancePtr*)instancePtr;
		if (model)
		{
			if (instance->GetID() != model->GetSVLId())
			{
				return false;
			}

			//this->SaveMaterial(model, instancePtr);
			//更新实例对应的材质ID
			if(model->GetMaterial() != nullptr)
			{
				instance->SetMaterialID(model->GetMaterial()->GetSvlId());
			}
			else
			{
				int id = instance->GetID();
				instance->SetMaterialID(-1);
			}
			//更新装配矩阵

			//更新

			vector<Stk_InstancePtr>& vecChildIns = instance->getChildren();
			if (model->GetChildren().size() == vecChildIns.size())
			{
				int i = 0;
				for (int subModelIndex = 0; subModelIndex < vecChildIns.size(); subModelIndex++)
				{
					Model* childModel = model->GetChildren().at(i++);
					Stk_InstancePtr subInstanceJson = vecChildIns.at(subModelIndex);
					UpdateInstance(childModel, &subInstanceJson);
				}
			}
		}
        return true;
	}

	void SVLXWriter::UpdateMaterialsSet()
	{
		vector<Model*> allsubModels;
		vector<Model*>& savedModels = this->GetSavedModels();
		if (savedModels.size() > 0)
		{
			for (int i = 0; i < savedModels.size(); i++)
			{
				M3D::Model* subModel = savedModels.at(i);

				vector<Model*> subSubModels;
				subModel->GetAllSubModels(subSubModels);
				allsubModels.insert(allsubModels.end(), subSubModels.begin(), subSubModels.end());
				allsubModels.push_back(subModel);
			}
		}

		this->m_baseMaterials.clear();
		for (int i = 0; i < allsubModels.size(); i++)
		{
			Model* subModel = allsubModels.at(i);
			BaseMaterial* baseMaterial = subModel->GetMaterial();
			if (baseMaterial)
			{
				this->m_baseMaterials.insert(baseMaterial);
			}

			if (subModel->GetBodys())
			{
				vector<Body*>& bodies = *subModel->GetBodys();
				for (int j = 0; j < bodies.size(); j++)
				{
					Body* body = bodies.at(j);

					vector<Face*>& faces = body->GetFaces();

					for (int k = 0; k < faces.size(); k++)
					{
						Face* face = faces.at(k);

						baseMaterial = face->GetMaterial();
						if (baseMaterial)
						{
							this->m_baseMaterials.insert(baseMaterial);
						}
					}
				}
			}
		}
	}

	void SVLXWriter::SaveAttribute()
	{
		Stk_DocumentPtr& m_svl2Doc = *(Stk_DocumentPtr*)this->m_svl2Doc;

		vector<Model*> allsubModels;
		vector<Model*>& savedModels = this->GetSavedModels();
		if (savedModels.size() > 0)
		{
			for (int i = 0; i < savedModels.size(); i++)
			{
				M3D::Model* subModel = savedModels.at(i);

				vector<Model*> subSubModels;
				subModel->GetAllSubModels(subSubModels);
				allsubModels.insert(allsubModels.end(), subSubModels.begin(), subSubModels.end());
				allsubModels.push_back(subModel);
			}
		}

		for (int i = 0; i < allsubModels.size(); i++)
		{
			Model* subModel = allsubModels.at(i);
			ShapeProperty* shapeProperty = subModel->GetShapeProperty();

			if (shapeProperty)
			{
				vector<ShapePropertyItem>& shapeProperties = shapeProperty->GetProperties();
				for (int i = 0; i < shapeProperties.size(); i++)
				{
					ShapePropertyItem item = shapeProperties[i];
					wstring attrName = Platform::StringToWString(item.m_strTitle, "auto");
					wstring attrValue = Platform::StringToWString(item.m_strValue, "auto");

					m_svl2Doc->AddModelAttribute(subModel->GetProtoTypeId(), attrName, attrValue);
				}
			}
		}
	}


	unsigned int SVLXWriter::UpdateSVLXID(unsigned int origSVLID)
	{
		if (origSVLID = -1)
		{
			return  ++m_updateSVLID;
		}
		return origSVLID;
	}

	vector<Model*>& SVLXWriter::GetSavedModels()
	{
		return m_savedModels;
	}

	void SVLXWriter::SetSavedModel(vector<Model*>& val)
	{
		m_savedModels = val;
	}

}
