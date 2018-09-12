/*
 * SVL2Reader.cpp
 *
 *  Created on: 2015-6-18
 *      Author: bing
 */
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
#include "SVLLib/Stk_LineSet.h"
#include "SVLLib/Stk_CurveEntity.h"
#include "sview/PMIAnalysis/SVLXPMICreator.h"
#include "writer.h"
#include "SVL2AsynReader.h"


#include "sview/views/View.h"
#include "m3d/SceneManager.h"
#include "m3d/utils/Platform.h"
#include "m3d/utils/MeshHelper.h"
#include "m3d/graphics/Material.h"
#include "m3d/graphics/Texture.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/graphics/TextureCube.h"

#include "m3d/model/Model.h"
#include "m3d/model/Body.h"
#include "m3d/model/ModelShape.h"
#include "m3d/model/Face.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix4.h"
#include "m3d/utils/IDCreator.h"
#include "m3d/utils/FileHelper.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/utils/StringHelper.h"
#include "sview/views/Parameters.h"
#include "m3d/model/PolyLine.h"
#include "m3d/model/Curve.h"
#include "m3d/model/Edge.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/base/Color.h"
#include "m3d/graphics/HardWareVertexBuffer.h"
#include "m3d/graphics/HardWareIndexBuffer.h"
#include "m3d/graphics/SectionPlane.h"
#include "m3d/utils/MemoryHelper.h"
#include "m3d/graphics/DirectionalLight.h"
#include "m3d/model/MeshData.h"
#include "m3d/RenderManager.h"
#include "m3d/graphics/PbrMaterial.h"
#include "m3d/model/ExtendInfoManager.h"
#include "m3d/model/ShapeProperty.h"
#include "m3d/utils/PathHelper.h"
#include "m3d/graphics/MatCapMaterial.h"
using M3D::Model;
using M3D::Model;
using M3D::Model;
using M3D::Body;
using M3D::Face;
using M3D::ModelShape;
using M3D::Body;
using M3D::Face;
using M3D::Vector3;
using M3D::Matrix4;
using M3D::FileHelper;
using M3D::StringHelper;
using M3D::ResourceManager;
using M3D::SceneManager;
using M3D::BaseMaterial;
using M3D::Texture;
using M3D::Color;
using M3D::Texture2D;
using M3D::Platform;
using M3D::Matrix3x4;
using M3D::Quaternion;
using M3D::SModelFileInfo;
using M3D::Quaternion;
using M3D::PolyLine;
using M3D::RefPolyLine;
using M3D::Edge;
using M3D::IDCreator;
 
using namespace HoteamSoft::SVLLib;

#define  ISPARALLEL		0x00000100
namespace SVIEW {

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

	void LogMemoryState(char* appendInfo)
	{
			if (appendInfo == NULL)
			{
				LOGD(("当前内存:" + MemoryHelper::Instance()->ProcessMemoryInfo()).c_str());
			}
			else {
				string strAppendInfo = appendInfo;
				LOGD((strAppendInfo + "当前内存:" + MemoryHelper::Instance()->ProcessMemoryInfo()).c_str());
			}
	}


	SVL2AsynReader::SVL2AsynReader() :
		Reader() {
		m_M3DModel = NULL;
		m_svl2Doc = new Stk_DocumentPtr();

		m_instanceCount = 1;
		m_readerPercent = 0.0f;

		m_fileBuffer = 0;
		m_fileBufferLength = 0;
		m_mergeface = false;

		readInstanceIndex = 0;

		m_usePrototypeProperty = Parameters::Instance()->m_usePrototypeProperty;;
		m_useInstanceProperty = Parameters::Instance()->m_useInstanceProperty;;

		m_SVLIDOffset = IDCreator::Instance()->GetSVLIDOffset().Code();

		this->SetAsynMode(true);
	}

	SVL2AsynReader::~SVL2AsynReader() {
		if (m_M3DModel) {
			this->FillModelColor(m_M3DModel);
			//this->FillModelVisible(m_M3DModel);
			//bool isLoadAttribute = true;//是否加载属性

			//Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
			//if (SVIEW::Parameters::Instance()->m_IsUseGeoData)
			//{
			//	//(*m_svl2Doc)->LoadAttribute();
			//	LOGI("SVL2AsynReader::FillModelAttribute Begin");
			//	this->FillModelAttribute(m_M3DModel, m_svl2Doc);
			//	//(*m_svl2Doc)->UnLoadAttribute();
			//	LOGI("SVL2AsynReader::FillModelAttribute End");
			//}

			//if (isLoadAttribute)
			//{
				//(*m_svl2Doc)->UnLoadAttribute();
			//}
			m_M3DModel->Release();
		}
		m_M3DModel = NULL;

		Stk_DocumentPtr* docPtr = (Stk_DocumentPtr*)m_svl2Doc;
		if (docPtr)
		{		//释放Model
			(*docPtr)->UnloadModel();
			//释放线集-yhp-20180830
			//(*docPtr)->UnloadLine();
			delete docPtr;
			LOGI("AsynReader ok");
			m_svl2Doc = NULL;
		}
	}

	Model* SVL2AsynReader::GetModel(int id) {
		
		if (m_M3DModel == NULL)
		{
			m_M3DModel = GetModelAssembly();
			if (m_M3DModel)
				m_M3DModel->AddRef();
		}
		
		return m_M3DModel;
	}

	void SVL2AsynReader::ReadFromBuffer(char* fileBuffer, int bufferLength)
	{
		this->m_fileBuffer = fileBuffer;
		this->m_fileBufferLength = bufferLength;

		if (this->m_fileBufferLength>0)
		{
			Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
			if (m_svl2Doc->isNotNull())
			{	//打开文件
				(*m_svl2Doc)->OpenFile(this->m_fileBufferLength, this->m_fileBuffer);
			}
		}
	}

	void SVL2AsynReader::FillModelColor(Model* model)
	{
		SModelFileInfo* fileInfo = model->GetFileInfo();
		if (fileInfo) {
			if (/*true||*/fileInfo->GetInstanceMaterialId() > 0)
			{
				Color modelInitColor;
				BaseMaterial* modelMateril = NULL;
				this->GetMatrialColor(fileInfo->GetInstanceMaterialId(), modelInitColor, modelMateril);
				if (modelMateril)
				{
					model->SetMaterial(modelMateril);
					//LOGE("Model has Material,Please!!%s",model->GetName().c_str());
				}
				if (modelInitColor.m_r >= 0.0f
					&& modelInitColor.m_g >= 0.0f
					&& modelInitColor.m_b >= 0.0f)
				{
					model->SetInitColor(modelInitColor);
				}

				if (modelInitColor.m_a>=0)
				{
					model->SetInitAlpha(modelInitColor.m_a);
				}
			}
			model->ClearFileInfo();
		}
		vector<Model*>&subModels = model->GetSubModels();
		for (int i =0;i<subModels.size();i++)
		{
			FillModelColor(subModels.at(i));
		}
	}

	void SVL2AsynReader::FillModelMesh(View* view, Model* model)
	{
		SModelFileInfo* fileInfo = model->GetFileInfo();
		if (fileInfo) {
			Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
			int stk_nodeId = fileInfo->GetPartModelId();
			
			Stk_ModelPtr stkModel = (*m_svl2Doc)->GetModelByID(stk_nodeId-m_SVLIDOffset);
			if (stkModel.isNotNull())
			{
					Model* pModel = FindPrototypeFromCache(stkModel->GetID());
					if (pModel != NULL)
					{
						CopyFormPrototype(static_cast<Model*>(model), static_cast<Model*>(pModel));
					}
					else
					{
						//TODO Xuli 这里需要添加一个顶点索引最大值判断来适配移动端显示的问题
						if (this->m_mergeface)
						{
							//FillModelDrawDataMergeFace(static_cast<Model*>(model), &stkModel);
							FillModelDrawDataMergeFaceWithColor(static_cast<Model*>(model), &stkModel);
						}
						else
						{
							//FillModelDrawDataMergeFaceWithColor(static_cast<Model*>(model), &stkModel);
							FillModelDrawData(static_cast<Model*>(model), &stkModel);
						}
	
						this->AddProtoTypeToCache(stkModel->GetID(), model);
					}	

					model->SetOrigVisible(model->IsVisible(), false);

					//if (fileInfo->GetInstanceMaterialId() > 0)
					//{
					//	Color modelInitColor;
					//	this->GetMatrialColor(fileInfo->GetInstanceMaterialId(), modelInitColor);
					//	model->SetInitColor(modelInitColor);
					//}
//End:
					//model->ClearFileInfo();
			}
		}
	}

	void SVL2AsynReader::FillModelMesh(View* view, vector<Model*>& models)
	{
		for (int i = 0; i < models.size(); i++)
		{
			Model* model = models.at(i);

			this->FillModelMesh(view, model);
		}
	}

	Model* SVL2AsynReader::GetModelAssembly()
	{
		if (m_M3DModel == NULL)
		{
			m_M3DModel = ReadFile(this->GetFile().c_str());
		}
		return m_M3DModel;
	}

	void SVL2AsynReader::Clear()
	{
		Reader::Clear();
		if (m_M3DModel)
			m_M3DModel->Release();
		m_M3DModel = NULL;
		Stk_DocumentPtr* docPtr = (Stk_DocumentPtr*)m_svl2Doc;
		if (docPtr)
		{
			delete docPtr;
			m_svl2Doc = NULL;
		}
	}

	void SVL2AsynReader::MemoryTest()
	{
		LOGI("model Memory Test begin");
		int Testcount = 560000;
		for (int i =0;i<Testcount;i++)
		{
			Model* model = new Model();
		}		
		LOGI("model Memory Model End");
		for (int i = 0; i < Testcount; i++)
		{
			SModelFileInfo* fileInfo = new SModelFileInfo(NULL);
		}
		LOGI("model Memory SModelFileInfo End");
		for (int i = 0; i < Testcount; i++)
		{
			ModelShape* tShapeNode = new ModelShape();
		}
		LOGI("model Memory ModelShape End");
		for (int i = 0; i < Testcount; i++)
		{
			Body* Lbody = new Body();
		}
		LOGI("model Memory Body End");
		for (int i = 0; i < Testcount; i++)
		{
			VertexSet* bodyDrawData = new VertexSet();
		}
		LOGI("model Memory VertexSet End");
		for (int i = 0; i < Testcount; i++)
		{
			void* hardWareVertexBuffer = new HardWareVertexBuffer();
			void* hardWareVertexIndexBuffer = new HardWareIndexBuffer();
		}
		LOGI("model Memory HardWareVertexBuffer End");
		for (int i = 0; i < Testcount; i++)
		{
			Face* lface = new Face();
		}	
		LOGI("model Memory Face End");
		for (int i = 0; i < Testcount; i++)
		{
			Mesh* facemeshData = new Mesh(NULL);
		}
		LOGI("model Memory Mesh End");

		LOGI("model Memory Test begin");
	}

	void SVL2AsynReader::LoadDoc(string path)
	{
		wstring wfilePath = Platform::StringToWString(path, "auto");
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		*m_svl2Doc = Stk_DocumentPtr::CreateObject();
		(*m_svl2Doc)->OpenFile(wfilePath);
	}

	int SVL2AsynReader::GetSVLXFileItem(const std::string& i_strFileName, unsigned int& o_bufSize, char** o_bufP, bool b_ByExt)
	{
		string sFileName = i_strFileName;
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		if (!b_ByExt)
		{
			int iIdx = i_strFileName.find_first_of("\\");
			if (iIdx >= 0)
			{
				sFileName.replace(iIdx, 1, "/");
			}
		}
		return (*m_svl2Doc)->GetSVLXFileItem(sFileName, o_bufSize, o_bufP, b_ByExt);
	}
    int SVL2AsynReader::GetSVLXFileItemToFile(const std::string& i_strFileName, const std::string& i_strOutFilePath, bool bByExt)
    {
        Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
        return(*m_svl2Doc)->GetSVLXFileItemToFile(i_strFileName, i_strOutFilePath, bByExt);
    }
	int SVL2AsynReader::GetSVLXFileItemToFileByReOpen(string & path ,const std::string& i_strFileName, const std::string& i_strOutFilePath, bool bByExt)
	{
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
        *m_svl2Doc = Stk_DocumentPtr::CreateObject();
        wstring wfilePath = Platform::StringToWString(path, "auto");
        //打开文件
        (*m_svl2Doc)->OpenFile(wfilePath);
		return(*m_svl2Doc)->GetSVLXFileItemToFile(i_strFileName, i_strOutFilePath, bByExt);
	}

	void SVL2AsynReader::AddProtoTypeToCache(int partId, Model * model) {
		MutexLock locked(this->m_mutex);
		m_partModelsMap.insert(pair<int, Model *>(partId, model));
	}

	Model*SVL2AsynReader::FindPrototypeFromCache(int id) {
		MutexLock locked(this->m_mutex);
		map<int, Model*>::iterator iter;
		iter = m_partModelsMap.find(id);
		if (iter != m_partModelsMap.end()) {
			return iter->second;
		}
		return NULL;
	}

	void SVL2AsynReader::CopyFormPrototype(Model* model, Model* part_model)
	{
		if (model && part_model && part_model->HasDrawData())
		{
			if (part_model->GetModelShape()&& model->GetModelShape()) {
				model->GetModelShape()->CopyFrom(*(part_model->GetModelShape()));
				model->SetDrawDataPrepared(true);
			}
		}
	}
	void SVL2AsynReader::FillModelDrawDataMergeFace(Model* model, void* pstk_NodePtr)
	{
		Stk_ModelPtr stkPart = *(Stk_ModelPtr*)pstk_NodePtr;
		if (stkPart.isNotNull())
		{
			int meshCount = 1;
			if (meshCount)
			{
				Model* tgroup = dynamic_cast<Model*>(model);
				ModelShape* tShapeNode = tgroup->GetModelShape();
				if (tShapeNode == NULL)
				{
					return;
				}

				stkPart->LoadLOD1Mesh();
				int meshCount = stkPart->GetNbLOD1Mesh();
				//LogMemoryState("SimplyFillPartMergeFace");

				Body* Lbody = new Body();
				Lbody->AddRef();
				VertexSet* bodyDrawData = new VertexSet();
				bodyDrawData->SetUseIndex(true);
				bodyDrawData->AddRef();

				std::vector<Vector3>* bodyPnts = bodyDrawData->GetPositionArray();
				std::vector<Vector3>* bodyNormals = bodyDrawData->GetNormalArray();
				std::vector<M3D_INDEX_TYPE>* indexArray = bodyDrawData->GetIndexArray();
				std::vector<Vector3>* textureCoords = bodyDrawData->GetTextureCoordArray();

				int faceMaterialId = 0;

				int bodyPntSize = 0;
				int bodyNormalSize = 0;
				int bodyIndexSize = 0;
				int bodyTextureSize = 0;

				for (int i = 0; i < meshCount; i++)
				{
					M3D_OFFSET_TYPE faceOffset = 0;
					M3D_OFFSET_TYPE bodyMeshHasDataOffset = 0;
					bodyMeshHasDataOffset = bodyPnts->size();

					Stk_MeshPrimPtr meshPtr = stkPart->GetLOD1Mesh(i);
	
					std::vector<float>& vecVert = meshPtr->GetVertexs();;
					std::vector<float>& vecNormals = meshPtr->GetNormals();;
					std::vector<float>& vecTexture = meshPtr->GetTextuercoordinate();

					if (vecVert.size() == 0)
					{
						continue;
					}

					bool hasNormals = vecNormals.size() > 0;
					bool hasTextureCoords = false;

					Vector3 position;
					Vector3 normal;

					for (int pntIndex = 0; pntIndex < vecVert.size() / 3; pntIndex++)
					{
						position.m_x = vecVert[pntIndex * 3];
						position.m_y = vecVert[pntIndex * 3 + 1];
						position.m_z = vecVert[pntIndex * 3 + 2];

						bodyPnts->push_back(position);
						//Normals
						if (hasNormals)
						{
							normal.m_x = vecNormals[pntIndex * 3];
							normal.m_y = vecNormals[pntIndex * 3 + 1];
							normal.m_z = vecNormals[pntIndex * 3 + 2];

							bodyNormals->push_back(normal);
						}

						//TextureCoords
						if (hasTextureCoords)
						{
							position.m_x = vecTexture[pntIndex * 3];
							position.m_y = vecTexture[pntIndex * 3 + 1];
							position.m_z = vecTexture[pntIndex * 3 + 2];

							textureCoords->push_back(position);
						}
					}

					//READER_LOGI("mesh count:%d 222");
					//color
					//M3D::Color bodyColor;
					//aiMaterial* aiMaterial = scene->mMaterials[curAIMesh->mMaterialIndex];
					//解析边界线数据
					//AddEdgeDataToBody(body, &meshPtr, 0);
					vector<unsigned int> vecTrians;

					std::map<STK_INT32, Stk_MeshFacePtr>& meshFaces = meshPtr->GetMeshFaces();
					std::map<STK_INT32, Stk_MeshFacePtr>::iterator meshIt = meshFaces.begin();

					//获取面的个数
					int numMeshFace = meshPtr->GetNbMeshFace();
					for (; meshIt!= meshFaces.end(); ++meshIt)
					{	//纪录前一次已经存储的数据大小
						//faceOffset = indexArray->size();
						Stk_MeshFacePtr& meshFacePtr = meshIt->second;
						meshFacePtr->GetIndex(vecTrians);
						if (vecTrians.size() > 0) {

							faceMaterialId = meshFacePtr->GetMaterialID();
							for (int ii = 0; ii < vecTrians.size(); ii++)
							{
								unsigned int tmpIndex = vecTrians[ii] + bodyMeshHasDataOffset;
								indexArray->push_back((M3D_INDEX_TYPE)tmpIndex);
							}
						}
					}
				}	
				if (bodyPnts->size() > 0) {
					Face* lface = new Face();
					Color lfaceColor;
					BaseMaterial* faceMaterial = NULL;
					this->GetMatrialColor(faceMaterialId, lfaceColor,faceMaterial);
					lface->SetInitColor(lfaceColor);

					if (faceMaterial != NULL)
					{
						lface->SetMaterial(faceMaterial);
					}

					Mesh* facemeshData = new Mesh(bodyDrawData);
					facemeshData->SetDataOffset(0);
					facemeshData->SetDataLength(indexArray->size());
					lface->SetData(facemeshData);

					Lbody->AddFace(lface);
					tShapeNode->AddBody(Lbody);
	
					bodyPnts->shrink_to_fit();
					bodyNormals->shrink_to_fit();
					indexArray->shrink_to_fit();
					textureCoords->shrink_to_fit();
				}
			 
				bodyDrawData->Release();
				Lbody->Release();

				tgroup->SetDrawDataPrepared(true);
			}
			stkPart->UnloadLOD1Mesh();
		}
	}

	void SVL2AsynReader::FillModelDrawDataMergeFaceWithColor(Model* model, void* pstk_NodePtr)
	{
		Stk_ModelPtr stkPart = *(Stk_ModelPtr*)pstk_NodePtr;
		if (stkPart.isNotNull())
		{
			int meshCount = 1;
			if (meshCount)
			{
				Model* tgroup = dynamic_cast<Model*>(model);
				ModelShape* tShapeNode = tgroup->GetModelShape();
				if (tShapeNode == NULL)
				{
					return;
				}
				stkPart->LoadLOD1Mesh();
				int meshCount = stkPart->GetNbLOD1Mesh();
				LogMemoryState("SimplyFillPartMergeFace");

				Body* Lbody = new Body();
				Lbody->AddRef();
				VertexSet* bodyDrawData = new VertexSet();
				bodyDrawData->SetUseIndex(true);
				bodyDrawData->AddRef();

				//用来处理面合并逻辑
				int priFaceMaterialIndex = -100;

				std::vector<Vector3>* bodyPnts = bodyDrawData->GetPositionArray();
				std::vector<Vector3>* bodyNormals = bodyDrawData->GetNormalArray();
				std::vector<M3D_INDEX_TYPE>* indexArray = bodyDrawData->GetIndexArray();
				std::vector<Vector3>* textureCoords = bodyDrawData->GetTextureCoordArray();

				int faceMaterialId = 0;
				Face* lface = NULL;
				Mesh* facemeshData = NULL;
				M3D_OFFSET_TYPE faceOffset = 0;
				M3D_OFFSET_TYPE bodyMeshHasDataOffset = 0;
				int bodyPntSize = 0;
				int bodyNormalSize = 0;
				int bodyIndexSize = 0;
				int bodyTextureSize = 0;

				for (int i = 0; i < meshCount; i++)
				{
					bodyMeshHasDataOffset = bodyPnts->size();

					Stk_MeshPrimPtr meshPtr = stkPart->GetLOD1Mesh(i);
					//void GetQuaternion(float& w, float& x, float& y, float& z);
					//void GetTranslation(float& x, float& y, float& z);
					std::vector<float>& vecVert = meshPtr->GetVertexs();
					std::vector<float>& vecNormals = meshPtr->GetNormals();
					std::vector<float>& vecTexture = meshPtr->GetTextuercoordinate();
					
					if (vecVert.size() == 0)
					{
						continue;
					}

					bool hasNormals = !vecNormals.empty();
					bool hasTextureCoords = !vecTexture.empty();

					Vector3 position;
					Vector3 normal;

					for (int pntIndex = 0; pntIndex < vecVert.size() / 3; pntIndex++)
					{
						position.m_x = vecVert[pntIndex * 3];
						position.m_y = vecVert[pntIndex * 3 + 1];
						position.m_z = vecVert[pntIndex * 3 + 2];

						bodyPnts->push_back(position);
						//Normals
						if (hasNormals)
						{
							normal.m_x = vecNormals[pntIndex * 3];
							normal.m_y = vecNormals[pntIndex * 3 + 1];
							normal.m_z = vecNormals[pntIndex * 3 + 2];

							bodyNormals->push_back(normal);
						}

						//TextureCoords
						if (hasTextureCoords)
						{
							position.m_x = vecTexture[pntIndex * 3];
							position.m_y = vecTexture[pntIndex * 3 + 1];
							position.m_z = vecTexture[pntIndex * 3 + 2];

							textureCoords->push_back(position);
						}
					}

					//解析边界线数据
					if (SVIEW::Parameters::Instance()->m_readEdge)
					{
						AddMergeEdgeDataToBody(Lbody, &meshPtr, 0);
					}
					
					vector<unsigned int> vecTrians;
					//获取面的个数
					std::map<STK_INT32, Stk_MeshFacePtr>& meshFacesMap = meshPtr->GetMeshFaces();
					std::map<STK_INT32, Stk_MeshFacePtr>::iterator facesIt = meshFacesMap.begin();

					for (; facesIt != meshFacesMap.end(); facesIt++)
					{
						Stk_MeshFacePtr meshFacePtr = facesIt->second;
						meshFacePtr->GetIndex(vecTrians);
						if (vecTrians.size() > 0) {
							if (priFaceMaterialIndex != meshFacePtr->GetMaterialID()) {
								priFaceMaterialIndex = meshFacePtr->GetMaterialID();
								//纪录前一次已经存储的数据大小
								faceOffset = indexArray->size();
								lface = new Face(); 
								//lface->SetColor(*firstFace->GetInitColor());
								facemeshData = new Mesh(bodyDrawData);
								facemeshData->SetDataOffset(faceOffset);
 
								Color lfaceColor; BaseMaterial* faceMaterial = NULL;
								this->GetMatrialColor(meshFacePtr->GetMaterialID(), lfaceColor,faceMaterial);
								lface->SetInitColor(lfaceColor);
								if (faceMaterial != NULL)
								{
									lface->SetMaterial(faceMaterial);
								}

								lface->SetData(facemeshData);
								Lbody->AddFace(lface);
							}

							for (int ii = 0; ii < vecTrians.size(); ii++)
							{
								unsigned int tmpIndex = vecTrians[ii] + bodyMeshHasDataOffset;
								indexArray->push_back((M3D_INDEX_TYPE)tmpIndex);
							}

							facemeshData->SetDataLength(indexArray->size()- faceOffset);
						}
					}

				}
				if (bodyPnts->size() > 0) {
 
					tShapeNode->AddBody(Lbody);
	
					bodyPnts->shrink_to_fit();
					bodyNormals->shrink_to_fit();
					indexArray->shrink_to_fit();
					textureCoords->shrink_to_fit();
				}

				bodyDrawData->Release();
				Lbody->Release();

				tgroup->SetDrawDataPrepared(true);
			}
			stkPart->UnloadLOD1Mesh();
		}
	}

	void SVL2AsynReader::FillLineSetDrawData(Model* i_model, void* i_stkModelPtr)
	{
		Stk_ModelPtr stkModelPtr = *(Stk_ModelPtr*)i_stkModelPtr;
		if (stkModelPtr.isNull())
		{
			return;
		}
		Model* tgroup = dynamic_cast<Model*>(i_model);
		ModelShape* tShapeNode = tgroup->GetModelShape();
		if (tShapeNode == NULL)
		{
			return;
		}
		//获取线集数据
		map<int, Stk_LineSetPtr> mapStkLinsetPtr = stkModelPtr->GetAllLineSet();
		map<int, Stk_LineSetPtr>::iterator it;
		for (it= mapStkLinsetPtr.begin(); it != mapStkLinsetPtr.end(); it++)
		{
			Stk_LineSetPtr stkLineSetPtr = it->second;
			if (stkLineSetPtr.isNull())
			{
				continue;
			}
			vector<Stk_CurveEntityPtr> vecCurveEntity = stkLineSetPtr->GetAllLines();
			if (vecCurveEntity.size() == 0)
			{
				continue;
			}
			//创建Body
			Body* pBody = new Body();
			pBody->AddRef();
			SPolyLine* bodyPolyLine = new SPolyLine();
			pBody->SetPolyLine(bodyPolyLine);
			for (int lineIndex = 0; lineIndex < vecCurveEntity.size(); lineIndex++)
			{
				Stk_CurveEntityPtr curLinePtr = vecCurveEntity.at(lineIndex);
				RefPolyLine * edgeLine = new RefPolyLine(bodyPolyLine);
				if (FillCurve(curLinePtr, edgeLine, bodyPolyLine))
				{
					Edge* edge = new Edge();
					//设置线的显隐状态
					edge->SetOrigVisible(true);
					STK_RGBA32 lineClr = curLinePtr->GetColor();
					Color edgeLineColor(lineClr.Red, lineClr.Green, lineClr.Blue,
						1 - lineClr.Alpha);
					edge->SetColor(edgeLineColor);
					edge->AddData(edgeLine, 0);
					pBody->AddEdge(edge);
				}
				else
				{
					//不存在显示数据，释放空间
					edgeLine->Release();
				}
			}
			tShapeNode->AddBody(pBody);
			pBody->Release();
			tgroup->SetDrawDataPrepared(true);
		}
	}

	bool SVL2AsynReader::FillCurve(Stk_CurveEntityPtr curCurve, RefPolyLine * edgeLine, SPolyLine * polyLine)
	{
		bool ret = false;
		switch (curCurve->GetCurveType())
		{
		case CURVE_TYPE_UNKNOWN:
			break;
		case CURVE_TYPE_POLYLINE:
		{
			Stk_PolyLinePtr curPolyLine = Stk_PolyLinePtr::DynamicCast(curCurve);

			const vector<STK_PNT32>& pnts = curPolyLine->GetPoints();

			edgeLine->SetDataOffset(polyLine->GetPoints().size());
			unsigned int dataLength = pnts.size();
			edgeLine->SetDataLength(2 * (dataLength - 1));

			if (dataLength >= 2)
			{
				//为了实现线合并的显示效果，按照下面这种方法存储边界线顶点
				//首先加入第一个点
				polyLine->AddPoints(Vector3(pnts[0].X, pnts[0].Y, pnts[0].Z));
				//加入中间部分点
				for (int j = 1; j < dataLength - 1; j++)
				{
					Vector3 pnt(pnts[j].X, pnts[j].Y, pnts[j].Z);
					//中间的点，每个点要添加两次
					polyLine->AddPoints(pnt);
					polyLine->AddPoints(pnt);
				}
				//在加入最后一个点
				polyLine->AddPoints(
					Vector3(pnts[dataLength - 1].X, pnts[dataLength - 1].Y,
						pnts[dataLength - 1].Z));
				ret = true;
			}
			break;
		}
		case CURVE_TYPE_ELLIPSE:
			break;
		case CURVE_TYPE_LINE:
		{
			Stk_LinePtr stk_line = Stk_LinePtr::DynamicCast(curCurve);

			STK_PNT32 startPnt = stk_line->GetStartPoint();
			STK_PNT32 endPnt = stk_line->GetEndPoint();

			edgeLine->SetDataOffset(polyLine->GetPoints().size());
			unsigned int dataLength = 2;
			edgeLine->SetDataLength(2 * (dataLength - 1));

			if (dataLength >= 2)
			{
				//为了实现线合并的显示效果，按照下面这种方法存储边界线顶点
				//首先加入第一个点
				polyLine->AddPoints(Vector3(startPnt.X, startPnt.Y, startPnt.Z));
				//加入中间部分点
				//在加入最后一个点
				polyLine->AddPoints(
					Vector3(endPnt.X, endPnt.Y, endPnt.Z));
				ret = true;
			}
			break;
		}
		case CURVE_TYPE_NURBSCURVE:
			break;
		case CURVE_TYPE_HYPERBOLA:
			break;
		case CURVE_TYPE_PARABOLA:
			break;
		}
		return ret;
	}

	void SVL2AsynReader::FillModelDrawData(Model* model, void* pstk_NodePtr)
	{
		Stk_ModelPtr stkPart = *(Stk_ModelPtr*)pstk_NodePtr;
		if (stkPart.isNotNull())
		{
			int meshCount = 1;
			if (meshCount)
			{
				Model* tgroup = dynamic_cast<Model*>(model);
				ModelShape* tShapeNode = tgroup->GetModelShape();
				if (tShapeNode == NULL)
				{
					return;
				}
				stkPart->LoadLOD1Mesh();
				int meshCount = stkPart->GetNbLOD1Mesh();
				//LogMemoryState("SimplyFillPart");

				for (int i = 0; i < meshCount; i++)
				{
					Body* body = new Body();
					body->AddRef();
					VertexSet* bodyDrawData = new VertexSet();
					bodyDrawData->AddRef();
					bodyDrawData->SetUseIndex(true);

					std::vector<Vector3>* bodyPnts = bodyDrawData->GetPositionArray();
					std::vector<Vector3>* bodyNormals = bodyDrawData->GetNormalArray();
					std::vector<M3D_INDEX_TYPE>* indexArray = bodyDrawData->GetIndexArray();
					std::vector<Vector3>* textureCoords = bodyDrawData->GetTextureCoordArray();

					M3D_OFFSET_TYPE faceOffset = 0;
					M3D_OFFSET_TYPE bodyMeshHasDataOffset = 0;
					bodyMeshHasDataOffset = bodyPnts->size();


					Stk_MeshPrimPtr meshPtr = stkPart->GetLOD1Mesh(i);
					body->SetSVLId(meshPtr->GetID()+ m_SVLIDOffset);
#ifdef WIN32					
					IDCreator::Instance()->UpdateSVLID(SVLGlobalID(meshPtr->GetID()+ m_SVLIDOffset));
#else
					SVLGlobalID temp(meshPtr->GetID()+ m_SVLIDOffset);
					IDCreator::Instance()->UpdateSVLID(temp);
#endif					
					//void GetQuaternion(float& w, float& x, float& y, float& z);
					//void GetTranslation(float& x, float& y, float& z);
					std::vector<float>& vecVert = meshPtr->GetVertexs();
					std::vector<float>& vecNormals = meshPtr->GetNormals();
					std::vector<float>& vecTexture = meshPtr->GetTextuercoordinate();

					bool useIndex = this->CanUseIndex(vecVert.size() / 3);
					bodyDrawData->SetUseIndex(useIndex);

					if (vecVert.size() == 0)
					{
						body->Release();
						bodyDrawData->Release();
						continue;
					}

					bool hasNormals = !vecNormals.empty();
					bool hasTextureCoords = !vecTexture.empty();

					Vector3 position;
					Vector3 normal;

					if (useIndex)
					{
						for (int pntIndex = 0; pntIndex < vecVert.size() / 3; pntIndex++)
						{
							position.m_x = vecVert[pntIndex * 3];
							position.m_y = vecVert[pntIndex * 3 + 1];
							position.m_z = vecVert[pntIndex * 3 + 2];

							bodyPnts->push_back(position);
							//Normals
							if (hasNormals)
							{
								normal.m_x = vecNormals[pntIndex * 3];
								normal.m_y = vecNormals[pntIndex * 3 + 1];
								normal.m_z = vecNormals[pntIndex * 3 + 2];

								bodyNormals->push_back(normal);
							}

							//TextureCoords
							if (hasTextureCoords)
							{
								position.m_x = vecTexture[pntIndex * 3];
								position.m_y = vecTexture[pntIndex * 3 + 1];
								position.m_z = vecTexture[pntIndex * 3 + 2];

								textureCoords->push_back(position);
							}
						}
					}

					//READER_LOGI("mesh count:%d 222");
					//color
					M3D::Color bodyColor;
					//aiMaterial* aiMaterial = scene->mMaterials[curAIMesh->mMaterialIndex];
					//解析边界线数据
					if (SVIEW::Parameters::Instance()->m_readEdge)
					{
						AddEdgeDataToBody(body, &meshPtr, 0);
					}
					vector<unsigned int> vecTrians;
					//获取面的个数
					int numMeshFace = meshPtr->GetNbMeshFace();

					std::map<STK_INT32, Stk_MeshFacePtr>& meshFacesMap = meshPtr->GetMeshFaces();
					std::map<STK_INT32, Stk_MeshFacePtr>::iterator facesIt = meshFacesMap.begin();

					for (; facesIt!= meshFacesMap.end();facesIt++)
					{
						
						Stk_MeshFacePtr meshFacePtr = facesIt->second;
						meshFacePtr->GetIndex(vecTrians);
						if (vecTrians.size() > 0) {
							Face* lface = new Face();	
							//由于边和面在model中独立存在 不需要对它们进行重新编号
							lface->SetSVLId(meshFacePtr->GetID()+ m_SVLIDOffset);
                            SVLGlobalID temp(meshFacePtr->GetID()+ m_SVLIDOffset);
							IDCreator::Instance()->UpdateSVLID(temp);

							//lface->SetColor(*firstFace->GetInitColor());
							if (useIndex)
							{	//lface->SetColor(*firstFace->GetInitColor());
								Mesh* facemeshData = new Mesh(bodyDrawData);
								//纪录前一次已经存储的数据大小
								faceOffset = indexArray->size();
								facemeshData->SetDataOffset(faceOffset);
								for (int ii = 0; ii < vecTrians.size(); ii++)
								{
									unsigned int tmpIndex = vecTrians[ii] + bodyMeshHasDataOffset;
									indexArray->push_back((M3D_INDEX_TYPE)tmpIndex);
								}
								facemeshData->SetDataLength(vecTrians.size());
								lface->SetData(facemeshData);
							}
							else
							{
								Mesh* facemeshData = new Mesh(bodyDrawData);
								faceOffset = bodyPnts->size();
								facemeshData->SetDataOffset(faceOffset);
								for (int ii = 0; ii < vecTrians.size(); ii++)
								{
									int pntIndex = vecTrians[ii];
									position.m_x = vecVert[pntIndex * 3];
									position.m_y = vecVert[pntIndex * 3 + 1];
									position.m_z = vecVert[pntIndex * 3 + 2];
									bodyPnts->push_back(position);
									//Normals
									if (hasNormals)
									{
										normal.m_x = vecNormals[pntIndex * 3];
										normal.m_y = vecNormals[pntIndex * 3 + 1];
										normal.m_z = vecNormals[pntIndex * 3 + 2];
										bodyNormals->push_back(normal);
									}

									//TextureCoords
									if (hasTextureCoords)
									{
										position.m_x = vecTexture[pntIndex * 3];
										position.m_y = vecTexture[pntIndex * 3 + 1];
										position.m_z = vecTexture[pntIndex * 3 + 2];

										textureCoords->push_back(position);
									}
								}
								facemeshData->SetDataLength(vecTrians.size());
								lface->SetData(facemeshData);
							}
							Color lfaceColor;
							BaseMaterial* faceMaterial = NULL;
							this->GetMatrialColor(meshFacePtr->GetMaterialID(), lfaceColor, faceMaterial);
							lface->SetInitColor(lfaceColor);

							if (faceMaterial!= NULL)
							{
								lface->SetMaterial(faceMaterial);
							}

							body->AddFace(lface);
						}
					}
					tShapeNode->AddBody(body);
					bodyDrawData->Release();
					body->Release();
				}
				tgroup->SetDrawDataPrepared(true);
			}
			stkPart->UnloadLOD1Mesh(); 
		}
		//读取线集-yhp-20180830
		//FillLineSetDrawData(model, pstk_NodePtr);
	}

	void SVL2AsynReader::LoadAttribute(Model* model, string & path ,View *m_view)
	{
		if (SVIEW::Parameters::Instance()->GetLoadExternInfo())
		{
			wstring wfilePath = Platform::StringToWString(path, "auto");
			Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
			*m_svl2Doc = Stk_DocumentPtr::CreateObject();
			if (SVIEW::Parameters::Instance()->m_IsUseGeoData)
			{
				//打开文件
				(*m_svl2Doc)->OpenFile(wfilePath);
				(*m_svl2Doc)->LoadAttribute();
				(*m_svl2Doc)->LoadGeoAttribute();
				map<STK_ID, map<STK_ID, map<wstring, wstring>>>* meshFaceAtrrPtr = (*m_svl2Doc)->GetAllMeshFaceAttr();
				if (meshFaceAtrrPtr)
				{
					m_view->GetSceneManager()->GetExtendInfoManager()->SetAllMeshFaceAttr(*meshFaceAtrrPtr);
				}

				map<STK_ID, map<STK_ID, map<wstring, wstring>>>* meshEdgeAtrrPtr = (*m_svl2Doc)->GetAllMeshEdgeAttr();
				if (meshFaceAtrrPtr)
				{
					m_view->GetSceneManager()->GetExtendInfoManager()->SetAllMeshEdgeAttr(*meshEdgeAtrrPtr);
				}

				(*m_svl2Doc)->UnLoadAttribute();
				(*m_svl2Doc)->UnloadGeoAttribute();
				//LOGI("SVL2AsynReader::FillModelAttribute End");
			}
		}
	}

	void SVL2AsynReader::AddGeoAttribute(Model* model, string & path, View *m_view)
	{
		if (SVIEW::Parameters::Instance()->GetLoadExternInfo())
		{
			wstring wfilePath = Platform::StringToWString(path, "auto");
			Stk_DocumentPtr m_svl2Doc = Stk_DocumentPtr::CreateObject();
			if (SVIEW::Parameters::Instance()->m_IsUseGeoData)
			{
				//打开文件
				(m_svl2Doc)->OpenFile(wfilePath);
				(m_svl2Doc)->LoadGeoAttribute();
				//在此添加几何属性
				map<STK_ID, map<STK_ID, map<wstring, wstring>>>* meshFaceAtrrPtr = (m_svl2Doc)->GetAllMeshFaceAttr();
				if (meshFaceAtrrPtr)
				{
					map<STK_ID, map<STK_ID, map<wstring, wstring>>> tmpMeshFaceAtrrPtr;
					map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorFaceMap = meshFaceAtrrPtr->begin();
					while (itorFaceMap != meshFaceAtrrPtr->end())
					{
						int iModelID = itorFaceMap->first;
						iModelID += m_SVLIDOffset;
						tmpMeshFaceAtrrPtr.insert(make_pair(iModelID, itorFaceMap->second));
						itorFaceMap++;
					}
					m_view->GetSceneManager()->GetExtendInfoManager()->AddMeshFaceAttr(tmpMeshFaceAtrrPtr);
					tmpMeshFaceAtrrPtr.clear();
				}

				map<STK_ID, map<STK_ID, map<wstring, wstring>>>* meshEdgeAtrrPtr = (m_svl2Doc)->GetAllMeshEdgeAttr();
				if (meshFaceAtrrPtr)
				{
					map<STK_ID, map<STK_ID, map<wstring, wstring>>> tmpMeshEdgeAtrrPtr;
					map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorEdgeMap = meshEdgeAtrrPtr->begin();
					while (itorEdgeMap != meshEdgeAtrrPtr->end())
					{
						int iModelID = itorEdgeMap->first;
						iModelID += m_SVLIDOffset;
						tmpMeshEdgeAtrrPtr.insert(make_pair(iModelID, itorEdgeMap->second));
						itorEdgeMap++;
					}
					m_view->GetSceneManager()->GetExtendInfoManager()->AddMeshEdgeAttr(tmpMeshEdgeAtrrPtr);
					tmpMeshEdgeAtrrPtr.clear();
				}
				(m_svl2Doc)->UnloadGeoAttribute();
				//LOGI("SVL2AsynReader::FillModelAttribute End");
			}
		}
	}

	void SVL2AsynReader::FillModelAttribute(Model* model, void* svlDoc)
	{
		if (!model)
		{
			return;
		}
		vector<Body*>* bodyList = model->GetBodys();
		if (bodyList)
		{
			for (vector<Body*>::iterator it = bodyList->begin(); it != bodyList->end(); it++)
			{
				if (*it)
				{
					vector<Face*> faceList = (*it)->GetFaces();
					for (int i = 0; i < faceList.size(); i++)
					{
						Face* face = faceList[i];
						if (face && face->GetData())
						{
							Mesh* facemeshData = face->GetData();
							int meshFaceID = face->GetSVLId();
							GetTriMeshGeoAttribute(model->GetProtoTypeId(),facemeshData, meshFaceID, svlDoc);
						}
					}
					vector<Edge*>* EdgeList = (*it)->GetEdges();
					for (int i = 0; i < EdgeList->size(); i++)
					{
						Edge* edge = EdgeList->at(i);
						if (edge && edge->GetLineData())
						{
							RefPolyLine * edgeLine = edge->GetLineData();
							int meshEdgeID = edge->GetSVLId();
							GetEdgeGeoAttribute(model->GetProtoTypeId(),edgeLine, meshEdgeID, svlDoc);
						}
					}
				}
			}
		}

		for (int i = 0; i < model->GetSubModelCount(); i++)
		{
			FillModelAttribute(model->GetSubModels().at(i), svlDoc);
		}	
	}

	void SVL2AsynReader::AddEdgeDataToBody(Body* body, void* pStkMesh, int readLodLevel) {

		Stk_MeshPrimPtr meshPtr = *((Stk_MeshPrimPtr*)pStkMesh);
		std::vector<float>& vecVert = meshPtr->GetVertexs();;
		vector<int> vecMeshEdgeID;
		int stkEdgeCount = meshPtr->GetNbMeshEdge();
		if (stkEdgeCount) {
			//构建一个用来存储所有线数据的线对象，让其较小的线对象从中引用数据段。
			M3D::SPolyLine* bodyPolyLine = body->GetPolyLine();

			if (bodyPolyLine == NULL)
			{
				bodyPolyLine = new M3D::SPolyLine(); //构建一个XPolyLine用于存储点数据
				body->SetPolyLine(bodyPolyLine);
			}

			std::map<STK_INT32, Stk_MeshEdgePtr>& edgesMap = meshPtr->GetMeshEdges();
			std::map<STK_INT32, Stk_MeshEdgePtr>::iterator edgeIt = edgesMap.begin();

			for (; edgeIt!= edgesMap.end();edgeIt++)
			{
				Stk_MeshEdgePtr meshEdge = edgeIt->second;

				if (meshEdge.isNull())
				{
					continue;
				}
				RefPolyLine * edgeLine = new RefPolyLine(bodyPolyLine);
				if (FillEdges(&meshEdge, &vecVert, bodyPolyLine, edgeLine)) {
					Edge* edge = new Edge(); 
					//由于边和面在model中独立存在 不需要对它们进行重新编号
					edge->SetSVLId(meshEdge->GetID()/* + m_SVLIDOffset*/);
					//IDCreator::Instance()->UpdateSVLID(SVLGlobalID(meshEdge->GetID() + m_SVLIDOffset));

					edge->AddData(edgeLine, readLodLevel);
					body->AddEdge(edge);
				}
				else {
					edgeLine->Release();
				}
			}
		}
	}

	void SVL2AsynReader::AddMergeEdgeDataToBody(Body* body, void* pStkMesh, int readLodLevel) {

		Stk_MeshPrimPtr meshPtr = *((Stk_MeshPrimPtr*)pStkMesh);
		std::vector<float>& vecVert = meshPtr->GetVertexs();;
		vector<int> vecMeshEdgeID;
		int stkEdgeCount = meshPtr->GetNbMeshEdge();
		if (stkEdgeCount) {
			//构建一个用来存储所有线数据的线对象，让其较小的线对象从中引用数据段。
			M3D::SPolyLine* bodyPolyLine = body->GetPolyLine();

			if (bodyPolyLine == NULL)
			{
				bodyPolyLine = new M3D::SPolyLine(); //构建一个XPolyLine用于存储点数据
				body->SetPolyLine(bodyPolyLine);
			}

			std::map<STK_INT32, Stk_MeshEdgePtr>& edgesMap = meshPtr->GetMeshEdges();
			std::map<STK_INT32, Stk_MeshEdgePtr>::iterator edgeIt = edgesMap.begin();

			for (; edgeIt != edgesMap.end(); edgeIt++)
			{
				Stk_MeshEdgePtr meshEdge = edgeIt->second;

				if (meshEdge.isNull())
				{
					continue;
				}

				//都合并到第一条边里面
				if (body->GetEdges()->size())
				{
					RefPolyLine * edgeLine = body->GetEdges()->at(0)->GetLineData();
					if (edgeLine)
					{
						FillMergeEdges(&meshEdge, &vecVert, bodyPolyLine, edgeLine);
					}
				}
				else
				{
					RefPolyLine * edgeLine = new RefPolyLine(bodyPolyLine);

					if (FillEdges(&meshEdge, &vecVert, bodyPolyLine, edgeLine)) {
						Edge* edge = new Edge();
						//由于边和面在model中独立存在 不需要对它们进行重新编号
						edge->SetSVLId(meshEdge->GetID()/* + m_SVLIDOffset*/);

						edge->AddData(edgeLine, readLodLevel);
						body->AddEdge(edge);
					}
					else {
						edgeLine->Release();
					}
				}
			}
		}
	}


	bool SVL2AsynReader::FillMergeEdges(void* stk_edge,
		vector<float>* outVertices, SPolyLine* bodyPolyLine,
		RefPolyLine * edgeLine) {
		Stk_MeshEdgePtr meshEdge = *((Stk_MeshEdgePtr*)stk_edge);
		vector<STK_UINT32>edgeIndex;
		meshEdge->GetIndex(edgeIndex);

		edgeLine->SetDataOffset(0);
		unsigned int dataLength = edgeIndex.size();
		edgeLine->SetDataLength(bodyPolyLine->GetPoints().size());

		if (dataLength >= 2) {
			//为了实现线合并的显示效果，按照下面这种方法存储边界线顶点
			//首先加入第一个点
			bodyPolyLine->AddPoints(
				Vector3((*outVertices)[edgeIndex[0] * 3],
				(*outVertices)[edgeIndex[0] * 3 + 1],
					(*outVertices)[edgeIndex[0] * 3 + 2]));
			//纪录一下索引信息
			bodyPolyLine->AddPointsIndex(edgeIndex[0]);

			//加入中间部分点
			for (int j = 1; j < dataLength - 1; j++) {
				Vector3 pnt((*outVertices)[edgeIndex[j] * 3],
					(*outVertices)[edgeIndex[j] * 3 + 1],
					(*outVertices)[edgeIndex[j] * 3 + 2]);
				//中间的点，每个点要添加两次
				bodyPolyLine->AddPoints(pnt);
				bodyPolyLine->AddPoints(pnt);

				//纪录一下索引信息
				bodyPolyLine->AddPointsIndex(edgeIndex[j]);
				bodyPolyLine->AddPointsIndex(edgeIndex[j]);
			}

			//在加入最后一个点
			bodyPolyLine->AddPoints(
				Vector3((*outVertices)[edgeIndex[dataLength - 1] * 3],
				(*outVertices)[edgeIndex[dataLength - 1] * 3 + 1],
					(*outVertices)[edgeIndex[dataLength - 1] * 3 + 2]));

			//纪录一下索引信息
			bodyPolyLine->AddPointsIndex(edgeIndex[dataLength - 1]);

			return true;
		}
		else {
			return false;
		}
	}


	bool SVL2AsynReader::FillEdges(void* stk_edge,
		vector<float>* outVertices, SPolyLine* bodyPolyLine,
		RefPolyLine * edgeLine) {
		Stk_MeshEdgePtr meshEdge = *((Stk_MeshEdgePtr*)stk_edge);
		vector<STK_UINT32>edgeIndex;
		meshEdge->GetIndex(edgeIndex);

		edgeLine->SetDataOffset(bodyPolyLine->GetPoints().size());
		unsigned int dataLength = edgeIndex.size();
		edgeLine->SetDataLength(2 * (dataLength - 1));

		if (dataLength >= 2) {
			//为了实现线合并的显示效果，按照下面这种方法存储边界线顶点
			//首先加入第一个点
			bodyPolyLine->AddPoints(
				Vector3((*outVertices)[edgeIndex[0] * 3],
				(*outVertices)[edgeIndex[0] * 3 + 1],
					(*outVertices)[edgeIndex[0] * 3 + 2]));
			//纪录一下索引信息
			bodyPolyLine->AddPointsIndex(edgeIndex[0]);

			//加入中间部分点
			for (int j = 1; j < dataLength - 1; j++) {
				Vector3 pnt((*outVertices)[edgeIndex[j] * 3],
					(*outVertices)[edgeIndex[j] * 3 + 1],
					(*outVertices)[edgeIndex[j] * 3 + 2]);
				//中间的点，每个点要添加两次
				bodyPolyLine->AddPoints(pnt);
				bodyPolyLine->AddPoints(pnt);

				//纪录一下索引信息
				bodyPolyLine->AddPointsIndex(edgeIndex[j]);
				bodyPolyLine->AddPointsIndex(edgeIndex[j]);
			}

			//在加入最后一个点
			bodyPolyLine->AddPoints(
				Vector3((*outVertices)[edgeIndex[dataLength - 1] * 3],
				(*outVertices)[edgeIndex[dataLength - 1] * 3 + 1],
					(*outVertices)[edgeIndex[dataLength - 1] * 3 + 2]));

			//纪录一下索引信息
			bodyPolyLine->AddPointsIndex(edgeIndex[dataLength - 1]);

			return true;
		}
		else {
			return false;
		}
	}

	void SVL2AsynReader::FillShapeProperty(const map<wstring, wstring>& protoTypeProperties, ShapeProperty* shapeProperty)
	{
		vector<ShapePropertyItem>& shapeProperties = shapeProperty->GetProperties();
		shapeProperties.clear();

		map<wstring, wstring>::const_iterator protoTypePropertiesIt = protoTypeProperties.begin();


		for (; protoTypePropertiesIt != protoTypeProperties.end(); ++ protoTypePropertiesIt)
		{
			//Stk_MetaData* metaData = protoTypePropertiesIt;
			//if (metaData)
			{
				ShapePropertyItem propertyItem;
				propertyItem.m_strTitle = Platform::WStringToString(protoTypePropertiesIt->first, "auto");
				propertyItem.m_eValueType = SHAPE_PROPERTY_TypeValueUnknown;
				//(ShapePropertyValueTypeEnum)metaData->GetType();
				propertyItem.m_strValue = Platform::WStringToString(protoTypePropertiesIt->second, "auto");
				//propertyItem.m_strUnits = Platform::WStringToString(metaData->GetUnits(), "auto");

				shapeProperties.push_back(propertyItem);
			}
		}
	}
	void SVL2AsynReader::FillAssembly(Model* model, void* instancePtr)
	{
		this->updateReadInstancePercent();
		Stk_InstancePtr instance =*(Stk_InstancePtr*)instancePtr;
		if (model)
		{
			Matrix4 plcMatrix;
			float* matrixData = &plcMatrix.m_m00;
			instance->GetMatrix(matrixData, 16);
			model->SetPlaceMatrix(plcMatrix);

			//赋值model name
			model->SetSVLId(instance->GetID()+m_SVLIDOffset);
#ifdef WIN32			
			IDCreator::Instance()->UpdateSVLID(SVLGlobalID(instance->GetID() + m_SVLIDOffset));
#else
            SVLGlobalID temp(instance->GetID() + m_SVLIDOffset);
			IDCreator::Instance()->UpdateSVLID(temp);
#endif
			model->SetName(GetInstanceName(model));
 
			model->SetVisible(instance->GetVisible(), false);

			int modelID = instance->GetModelID();
		
			if (modelID > 0)
			{
				SModelFileInfo* fileInfo = new SModelFileInfo(model);
				fileInfo->SetInstanceMaterialId(instance->GetMaterialID());
				fileInfo->SetPartModelId(modelID + m_SVLIDOffset);
				model->SetProtoTypeId(modelID + m_SVLIDOffset);
                SVLGlobalID temp(modelID + m_SVLIDOffset);
				IDCreator::Instance()->UpdateSVLID(temp);
				model->SetFileInfo(fileInfo);
				m_modelStkModelMap.insert(Model_StkModel_Map::value_type(model, modelID + m_SVLIDOffset));
			}

			vector<Stk_InstancePtr>& vecChildIns = instance->getChildren();
			for (int subModelIndex = 0; subModelIndex < vecChildIns.size(); subModelIndex++)
			{
				Model* childModel = new Model();
				Stk_InstancePtr subInstanceJson = vecChildIns.at(subModelIndex);
				FillAssembly(childModel, &subInstanceJson);
				model->AddSubModel(childModel);
			}
		}
	}

	string SVL2AsynReader::GetInstanceName(Model* model)
	{
		string insName = "";
		map<STK_UINT32, std::wstring>::iterator it = this->i_mapInstanceIDtoName.find(model->GetSVLId() - this->m_SVLIDOffset);
		if (it!= this->i_mapInstanceIDtoName.end())
		{
			insName = Platform::WStringToString(it->second, "auto");
			if (insName.empty())
			{
				insName = "Default Ins";
			}
		}
		else
		{
			insName = "Default Ins";
		}
		return insName;
	}

	Model* SVL2AsynReader::ReadFile(const char* thePath)
	{
		LOGI("load BOM bin begin");
		 
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		*m_svl2Doc = Stk_DocumentPtr::CreateObject();

		SetPercent(0.1f);
		if (this->m_fileBuffer && this->m_fileBufferLength > 0)
		{
			//打开buffer
			(*m_svl2Doc)->OpenFile(this->m_fileBufferLength, this->m_fileBuffer);
		}
		else 
		{
			string filePath = thePath;
			wstring wfilePath = Platform::StringToWString(filePath, "auto");
			//打开文件
			(*m_svl2Doc)->OpenFile(wfilePath);
		}
		(*m_svl2Doc)->cacheMeshToTempPath();
		SetPercent(0.2f);
 
		(*m_svl2Doc)->LoadBom();
		SetPercent(0.3f);
 
		//加载材质信息
		(*m_svl2Doc)->LoadMaterial();

		string effectName = Platform::WStringToString ((*m_svl2Doc)->GetGlobalEffect());
		m_view->GetSceneManager()->GetRenderManager()->SetGlobalEffect(effectName);
 
//        this->LoadFileInfo();
		SetPercent(0.4f);			
		if (SVIEW::Parameters::Instance()->GetLoadExternInfo())
		{
			STK_CHAR* dataBuffer = NULL;
			unsigned int bufSize = 0;
			string strFileName = ".animation";// FileHelper::GetFileName(FileHelper::GetUnionStylePath(thePath));
			(*m_svl2Doc)->GetSVLXFileItem(strFileName, bufSize, &dataBuffer);
			if (dataBuffer != NULL)
			{
				SetXMLAnimationData(dataBuffer);
			}
            //批注
            STK_CHAR* _dataBuffer = NULL;
            unsigned int _bufSize = 0;
            string strName = ".annotation";// FileHelper::GetFileName(FileHelper::GetUnionStylePath(thePath));
            (*m_svl2Doc)->GetSVLXFileItem(strName, _bufSize, &_dataBuffer);
            if (_dataBuffer != NULL)
            {
                ParseAnnotation(_dataBuffer);
            }
		}

		string strSourceFormat = (*m_svl2Doc)->GetLoadInf()->getSourceFormat();
		if (strSourceFormat == "rvm")
		{
			this->m_mergeface = true;
		}
		//如果不适用扩展信息，强制合并面处理
		if (!SVIEW::Parameters::Instance()->GetLoadExternInfo())
		{
			this->m_mergeface = true;
		}

		(*m_svl2Doc)->loadAttributeStream();
		(*m_svl2Doc)->getInstanceNames(i_mapInstanceIDtoName);
		(*m_svl2Doc)->unloadAttributeStream();

		SetPercent(0.5f);
		Model* topModel = new Model();
		{
			//遍历缓存的表，将模型树构建出来
			vector<Stk_InstancePtr>& vecTopInstance = (*m_svl2Doc)->getTopInstances();
			int topNodeCount = vecTopInstance.size();
			//当有多个顶级实例时候，仅仅读取一个顶级
			/*if (topNodeCount > 1)
			{
				topModel->SetName("Root");
				for (int i = 0; i < topNodeCount; i++)
				{
					Stk_InstancePtr jsonInstance = vecTopInstance.at(i);
					Model* subModel = new Model();
					if (subModel)
					{
						topModel->AddSubModel(subModel);
					}
					FillAssembly(subModel, &jsonInstance);
					GetInstanceData(subModel, &jsonInstance);
				}
			}
			else if (topNodeCount == 1)
			{
				Stk_InstancePtr jsonInstance = vecTopInstance.at(0);
				FillAssembly(topModel, &jsonInstance);
				GetInstanceData(topModel, &jsonInstance);
			}*/

			if (topNodeCount > 0)
			{
				Stk_InstancePtr jsonInstance = vecTopInstance.at(0);
				FillAssembly(topModel, &jsonInstance);
				GetInstanceData(topModel, &jsonInstance);
			}

			(*m_svl2Doc)->UnloadBom();
		}

		if (topModel->GetName().length() == 0)
		{
			topModel->SetName("Root");
		}
 
		//加载Model
		(*m_svl2Doc)->LoadModel();
		LOGI("svllib load Model  end");
		//加载线集-yhp-20180830
		//(*m_svl2Doc)->LoadLine();

		if (SVIEW::Parameters::Instance()->m_IsUsePmiData)
		{
			(*m_svl2Doc)->LoadPMI();
			GetPMIsData(topModel);
		}

		if (SVIEW::Parameters::Instance()->m_IsUseModelViewData)
		{
			(*m_svl2Doc)->LoadView();
			GetViewsData(topModel);
		}

		FillAssemblyBBox(topModel);

		ReadModelAttribute(topModel);

        ReadUserData(topModel);
        
		this->ClearPartsMap();
 
		return topModel;
	}

	void  SVL2AsynReader::ParseMaterialParameters(Stk_MaterialPtr materialPtr, BaseMaterial* material)
	{
		if (!materialPtr.isNotNull())
		{
			return;
		}
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		map<string, pair<string, string> > parametersMap;
		parametersMap = materialPtr->GetMaterialParameters();

		map<string, pair<string, string> >::iterator it = parametersMap.begin();
		for (;it != parametersMap.end();it++)
		{
			string name = it->first;
			string type = it->second.first;
			string value = it->second.second;

			if (type == "Texture2D")
			{
				int imageID = StringHelper::StringToInt(value);
				unsigned int buffersize = 0;
				char* data = nullptr;
				string savePath = "";
				GetImageData(imageID, buffersize, data, savePath);

				if (buffersize <= 0)
				{
					LOGE("贴图ID(%u)，图片数据为0");
					return;
				}
				if (buffersize > 0 && data)
				{
					//将数据赋值给图片对象
					string textName = material->GetName();

					Texture2D* texture2d = (Texture2D*)this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateTexture(savePath, Texture::TEXTURE_2D);
					Image* image = new Image();
					image->SetPath(savePath);
					image->SetData((unsigned char*)data, buffersize);
					texture2d->SetImage(image);
					
					Uniform uniform(type, (Texture2D*)texture2d);
					texture2d->AddRef();
					if (!material->SetUniformParameter(name, uniform))
					{
						//Exist this name already
						map<string, Uniform>& allMaps = material->GetUnifomParameters();
						Uniform* oldUniform = material->GetUniformParameter(name);
						Texture2D* oldValue = anyCast<Texture2D*>(oldUniform->value);
						if (oldValue != texture2d)//if the old's pointer not equal the new's
						{
							oldValue->Release();//release the old 
							allMaps[name] = uniform;//replace it by the new 	
						}
						else
						{
							texture2d->Release();
						}
					}
				}

			}
			else if (type == "TextureCube")
			{
				vector<string> imageIDStrArray = StringHelper::Split(value, ",");
				vector<int> imageIDArray;
				 
				vector<Image*> cubeImages;
				string savePathes = "";
				for (int i = 0;i<imageIDStrArray.size();i++)
				{
					int imageID = StringHelper::StringToInt (imageIDStrArray[i]);
					unsigned int buffersize = 0;
					char* data = nullptr;
					string savePath = "";

					GetImageData(imageID, buffersize, data, savePath);
					if (buffersize <= 0)
					{
						LOGE("贴图ID(%u)，图片数据为0");
						return;
					}
					Image* image = this->m_view->GetSceneManager()->GetResourceManager()->GetImage(savePath);
					if (!image)
					{
						image = new Image();
						image->SetPath(savePath);
						image->SetData((unsigned char*)data, buffersize);
					}
					savePathes += savePath;
					cubeImages.push_back(image);
				}

				TextureCube * cubeTexture = (TextureCube*)this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateCubeMappingTexture(savePathes, cubeImages);
				Uniform uniform(type, cubeTexture);
				cubeTexture->AddRef();			
				if (!material->SetUniformParameter(name, uniform))
				{
					//Exist this name already
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					TextureCube* oldValue = anyCast<TextureCube*>(oldUniform->value);
					if (oldValue != cubeTexture)//if the old's pointer not equal the new's
					{
						oldValue->Release();//release the old 
						allMaps[name] = uniform;//replace it by the new 	
					}
					else
					{
						cubeTexture->Release();
					}
				}

			}
			else if (type == "Vector3")
			{
				vector<string> vectorStrArray = StringHelper::Split(value, ",");
				Vector3* vec = new Vector3();
				vec->m_x = StringHelper::StringToFloat(vectorStrArray[0]);
				vec->m_y = StringHelper::StringToFloat(vectorStrArray[1]);
				vec->m_z = StringHelper::StringToFloat(vectorStrArray[2]);				
				Uniform uniform(type, vec);			
				if (!material->SetUniformParameter(name, uniform))
				{
					//Exist this name already
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					Vector3* oldValue = anyCast<Vector3*>(oldUniform->value);
					if (oldValue != vec)//if the old's pointer not equal the new's
					{
						delete oldValue;
						oldValue = NULL;
						allMaps[name] = uniform;//replace it by the new 	
					}
					else
					{
						delete vec;
						vec = NULL;
					}
				}
			}
			else if (type == "Vector4")
			{
				vector<string> vectorStrArray = StringHelper::Split(value, ",");
				Vector4* vec = new Vector4();
				vec->m_x = StringHelper::StringToFloat(vectorStrArray[0]);
				vec->m_y = StringHelper::StringToFloat(vectorStrArray[1]);
				vec->m_z = StringHelper::StringToFloat(vectorStrArray[2]);
				vec->m_w = StringHelper::StringToFloat(vectorStrArray[3]);
				Uniform uniform(type, vec);				
				if (!material->SetUniformParameter(name, uniform))
				{
					//Exist this name already
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					Vector4* oldValue = anyCast<Vector4*>(oldUniform->value);
					if (oldValue != vec)//if the old's pointer not equal the new's
					{
						delete oldValue;
						oldValue = NULL;
						allMaps[name] = uniform;//replace it by the new 	
					}
					else
					{
						delete vec;
						vec = NULL;
					}
				}
			}
			else if (type == "Float")
			{
				float fv ;
				fv = ( StringHelper::StringToFloat(value));
				Uniform uniform(type, fv);			
				if (!material->SetUniformParameter(name, uniform))
				{
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					float oldValue = anyCast<float>(oldUniform->value);
					if (oldValue != fv)//if the old's pointer not equal the new's
					{
						allMaps[name] = uniform;//replace it by the new 	
					}
				}
			}
			else if (type == "Bool")
			{
				
				int v = value == "true" ? 1 : 0;				
				Uniform uniform(type, v);				
				if (!material->SetUniformParameter(name, uniform))
				{
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					int oldValue = anyCast<int>(oldUniform->value);
					if (oldValue != v)//if the old's pointer not equal the new's
					{
						allMaps[name] = uniform;//replace it by the new 	
					}
				}
			}
			else if (type == "Int")
			{
				int fv  ;
				fv = (StringHelper::StringToFloat(value));
				Uniform uniform(type, fv);				
				if (!material->SetUniformParameter(name, uniform))
				{
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					int oldValue = anyCast<int>(oldUniform->value);
					if (oldValue != fv)//if the old's pointer not equal the new's
					{
						allMaps[name] = uniform;//replace it by the new 	
					}
				}
			}
		}
		material->SetNeedsUpdateUniformParamerers(false);
	}

	void SVL2AsynReader::GetImageData(unsigned int imageID, unsigned int & o_buffersize, char * &o_data,  string& o_svePath)
	{
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		Stk_ImagePtr imagePtr = (*m_svl2Doc)->GetImageByID(imageID);
		//unsigned int buffersize = 0;
		//char* data;
		wstring strPicPath = imagePtr->GetPictureSavePath();
		int iIdx = strPicPath.find_first_of(L"\\");
		if (iIdx >= 0)
		{
			strPicPath.replace(iIdx, 1, L"/");
		}	
		o_svePath = Platform::WStringToString(strPicPath);
		(*m_svl2Doc)->GetSVLXFileItem(Platform::WStringToString(strPicPath), o_buffersize, &o_data, false);
	}

	bool SVL2AsynReader::GetMatrial(int materialId, Color& difuseColor,BaseMaterial* &material)
	{
		material = NULL;
		difuseColor = Color::GRAY;
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		Stk_MaterialPtr materialPtr = (*m_svl2Doc)->GetMaterialByID(materialId);
		if (materialPtr.isNotNull())
		{
			map<int, BaseMaterial*>::iterator it = m_protoTypeMaterialCache.find(materialId);
			if (it != m_protoTypeMaterialCache.end())
			{
				material = it->second;
			}

			map<int,Color>::iterator itColor =m_protoTypeColorCache.find(materialId);
			if (itColor != m_protoTypeColorCache.end())
			{
				difuseColor = itColor->second;

				return false;
			}

			StkMaterialTypeEnum materialType = materialPtr->GetMaterialType();
			if (materialType == StkMaterialTypeEnum::STK_MATERIALTYPE_SIMPLAE)
			{
				//获取RGB的值
				materialPtr->GetDiffuseColor(&(difuseColor.m_r));
				materialPtr->GetTransparency(difuseColor.m_a);

				if (difuseColor.m_a < 0)
				{
					difuseColor.m_a = 1.0f;
				}

				HoteamSoft::SVLLib::Stk_TexturePtr pStkTexture = materialPtr->GetDiffuseTexture();
				vector<HoteamSoft::SVLLib::Stk_TexturePtr> textures;
				materialPtr->GetReflectiveTexture(textures);
				if (pStkTexture.isNotNull() || textures.size()>0)
				{
					string materialIDStr = GetSVLMaterialID(this->m_M3DModel,materialId);
					//将材质加入全局资源管理器中，进行资源管理
					material = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateMaterial(materialIDStr);
					if (material)
					{
						if (pStkTexture.isNotNull()) 
						{
							SVLXRenderToMaterial(material, &pStkTexture);
							//将material加入缓存列表中 便于快速查找
							m_protoTypeMaterialCache.insert(std::pair<int, BaseMaterial*>(materialId, material));
							m_protoTypeColorCache.insert(std::pair<int, Color>(materialId, difuseColor));
						}

						if (textures.size()>0)
						{
							vector<unsigned char*> buffers;
							vector<int> bufferLengths;
							string name = "";

							string savePathes = "";
							vector<Image*> cubeImages;	
							unsigned int buffersize = 0;
							char* data = nullptr;
							for (int i = 0; i < textures.size(); i++)
							{
								HoteamSoft::SVLLib::Stk_TexturePtr pTexture = textures[i];
								if (pTexture.isNotNull())
								{
									HoteamSoft::SVLLib::Stk_ImagePtr pImage = pTexture->GetTextureImage();
								
									string strPicPath = Platform::WStringToString(pImage->GetPictureSavePath());

									int iIdx = strPicPath.find_first_of("\\");
									if (iIdx >= 0)
									{
										strPicPath.replace(iIdx, 1, "/");
									}

									Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
									name += strPicPath;
									(*m_svl2Doc)->GetSVLXFileItem(strPicPath, buffersize, &data, false);
									//将数据赋值给图片对象
									//buffers.push_back((unsigned char*)data);
									//bufferLengths.push_back(buffersize);
									Image* image = this->m_view->GetSceneManager()->GetResourceManager()->GetImage(strPicPath);
									if (!image)
									{
										image = new Image();
										image->SetPath(strPicPath);
										image->SetData((unsigned char*)data, buffersize);
									}
									cubeImages.push_back(image);
								}
							}

							Texture * cubeTexture = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateCubeMappingTexture(name, cubeImages);

							((Material*)material)->SetReflectiveTexture(cubeTexture);
						}
					}
				}
				else
				{
					m_protoTypeColorCache.insert(std::pair<int, Color>(materialId, difuseColor));
				}
			}
			else 
			{
				StkMaterialTypeEnum materialType = materialPtr->GetMaterialType();
				wstring wname = materialPtr->GetMaterialName();
				string name = Platform::WStringToString(wname);
				string materialIDStr = GetSVLMaterialID(this->m_M3DModel, materialId);
				//将材质加入全局资源管理器中，进行资源管理
				material = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateMaterial(name,MaterialType_Shader);
				if (!material->GetNeedsUpdateUniformParamerers())
				{
					return false;
				}

			//	vector<int> * vec = new vector<int>();
			//	vec->push_back(materialType);
				Uniform uniform("Int", (int)materialType);
				material->SetUniformParameter("type", uniform);
				ParseMaterialParameters(materialPtr, material);
			}
		}

		return true;
	}

	//该函数为测试函数，原版的是GetMatrial，若要还原，将原版的改名为GetMatrialColor
	bool SVL2AsynReader::GetMatrialColor(int materialId, Color& difuseColor, BaseMaterial* &material)
	{
		material = NULL;
		difuseColor = Color::GRAY;
		Color tempColor = Color::GRAY;
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		Stk_MaterialPtr materialPtr = (*m_svl2Doc)->GetMaterialByID(materialId);
		if (materialPtr.isNotNull())
		{

			//更新Reader内部对应的材质ID，防止和上一个读取的文件出现重复
			//materialId = materialId + m_SVLIDOffset;
			//IDCreator::Instance()->UpdateSVLID(SVLGlobalID(materialId));

			map<int, BaseMaterial*>::iterator it = m_protoTypeMaterialCache.find(materialId);
			if (it != m_protoTypeMaterialCache.end())
			{
				material = it->second;
			}

			map<int, Color>::iterator itColor = m_protoTypeColorCache.find(materialId);
			if (itColor != m_protoTypeColorCache.end())
			{
				difuseColor = itColor->second;

				return false;
			}
			//创建材质
			string materialIDStr = GetSVLMaterialID(this->m_M3DModel, materialId);
			
			StkMaterialTypeEnum materialType = materialPtr->GetMaterialType();

			
			if (materialType == StkMaterialTypeEnum::STK_MATERIALTYPE_SIMPLAE&&false)
			{
				material = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateMaterial(materialIDStr, MaterialType_Phong);
				//获取RGB的值
				materialPtr->GetDiffuseColor(&(difuseColor.m_r));
				materialPtr->GetTransparency(difuseColor.m_a);

				if (difuseColor.m_a < 0)
				{
					difuseColor.m_a = 1.0f;
				}

				HoteamSoft::SVLLib::Stk_TexturePtr pStkTexture = materialPtr->GetDiffuseTexture();
				vector<HoteamSoft::SVLLib::Stk_TexturePtr> textures;
				materialPtr->GetReflectiveTexture(textures);
				if (pStkTexture.isNotNull() || textures.size() > 0)
				{
					if (material)
					{
						if (pStkTexture.isNotNull())
						{
							SVLXRenderToMaterial(material, &pStkTexture);
							//将material加入缓存列表中 便于快速查找
							m_protoTypeMaterialCache.insert(std::pair<int, BaseMaterial*>(materialId, material));
							m_protoTypeColorCache.insert(std::pair<int, Color>(materialId, difuseColor));
						}
						string savePathes = "";
						vector<Image*> cubeImages;		
						unsigned int buffersize = 0;
						char* data = nullptr;
						if (textures.size() > 0)
						{
							vector<unsigned char*> buffers;
							vector<int> bufferLengths;
							string name = "";
							for (int i = 0; i < textures.size(); i++)
							{
								HoteamSoft::SVLLib::Stk_TexturePtr pTexture = textures[i];
								if (pTexture.isNotNull())
								{
									HoteamSoft::SVLLib::Stk_ImagePtr pImage = pTexture->GetTextureImage();
									//将数据赋值给图片对象
									buffers.push_back((unsigned char*)data);
									bufferLengths.push_back(buffersize);

									string strPicPath = Platform::WStringToString(pImage->GetPictureSavePath());

									int iIdx = strPicPath.find_first_of("\\");
									if (iIdx >= 0)
									{
										strPicPath.replace(iIdx, 1, "/");
									}

									Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
									name += strPicPath;
									(*m_svl2Doc)->GetSVLXFileItem(strPicPath, buffersize, &data, false);
									//将数据赋值给图片对象
									//buffers.push_back((unsigned char*)data);
									//bufferLengths.push_back(buffersize);
									Image* image = this->m_view->GetSceneManager()->GetResourceManager()->GetImage(strPicPath);
									if (!image)
									{
										image = new Image();
										image->SetPath(strPicPath);
										image->SetData((unsigned char*)data, buffersize);
									}
									cubeImages.push_back(image);
								}
							}

							Texture * cubeTexture = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateCubeMappingTexture(name, cubeImages);

							((Material*)material)->SetReflectiveTexture(cubeTexture);
						}
					}
				}
				else
				{
					m_protoTypeColorCache.insert(std::pair<int, Color>(materialId, difuseColor));
				}

			}
			else if (materialType == StkMaterialTypeEnum::STK_MATERIALTYPE_PHONG|| materialType == StkMaterialTypeEnum::STK_MATERIALTYPE_SIMPLAE)
			{
				material = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateMaterial(materialIDStr, MaterialType_Phong);
				Material* tempMaterial = static_cast<Material*>(material);
				wstring wMaterialName = materialPtr->GetMaterialName();
				string materialName = Platform::WStringToString(wMaterialName, "auto");
				tempMaterial->DisplayName(materialName);
				//获取ambientColor
				//TODO
				materialPtr->GetAmbientColor(&(tempColor.m_r));
				if (tempColor.m_r >= 0 && tempColor.m_g >= 0 && tempColor.m_b >= 0)
				{
					tempMaterial->SetAmbient(tempColor);
				}
				//获取diffuseColor
				materialPtr->GetDiffuseColor(&(difuseColor.m_r));
				materialPtr->GetTransparency(difuseColor.m_a);

				if (difuseColor.m_a < 0)
				{
					difuseColor.m_a = 1.0f;
				}
				tempMaterial->SetDiffuse(difuseColor);
				tempMaterial->Opacity(difuseColor.m_a);
				//获取specularColor
				//TODO
				materialPtr->GetSpecularColor(&(tempColor.m_r));
				if (tempColor.m_r>=0&& tempColor.m_g >= 0&& tempColor.m_b >= 0)
				{
					tempMaterial->setSpecular(tempColor);
				}
				
				materialPtr->GetAmbientColor(&(tempColor.m_r));
				if (tempColor.m_r >= 0 && tempColor.m_g >= 0 && tempColor.m_b >= 0)
				{
					tempMaterial->SetAmbient(tempColor);
				}
				
				materialPtr->GetEmissiveColor(&(tempColor.m_r));
				if (tempColor.m_r >= 0 && tempColor.m_g >= 0 && tempColor.m_b >= 0)
				{
					tempMaterial->SetEmissive(tempColor);
				}
				
				float shininess = 20.0;
				materialPtr->GetShininess(shininess);
				if (shininess>=0)
				{
					tempMaterial->SetShininess(shininess);
				}			
				bool hasTexture = false;
				//获取diffuse
				HoteamSoft::SVLLib::Stk_TexturePtr diffuseTexture = materialPtr->GetDiffuseTexture();
				if (diffuseTexture.isNotNull())
				{
					Texture * diffuseMapping = GetTexture2D(&diffuseTexture);
					tempMaterial->SetDiffuseMap(diffuseMapping);
					float uScale = diffuseTexture->GetUScale();
					float vScale = diffuseTexture->GetVScale();
					hasTexture = true;
					//如果u v 不同时为1
					if (Abs(uScale - 1) < 0.001 && Abs(vScale - 1.0f) < 0.001 || Abs(uScale) < 0.001f || Abs(vScale) < 0.001f)
					{
					}
					else
					{
						//构造变换纹理坐标变换矩阵
						tempMaterial->CreateTexture2DTransform();
						Matrix4* matrix4 = tempMaterial->GetTexture2DTransform();
						if (matrix4)
						{
							matrix4->m_m00 = uScale;
							matrix4->m_m11 = vScale;
						}
					}

				}
				HoteamSoft::SVLLib::Stk_TexturePtr matcapTexture = materialPtr->GetMatcapMap();
				if (matcapTexture.isNotNull())
				{
					Texture * matcapMapping = GetTexture2D(&matcapTexture);
					tempMaterial->MatcapMap(matcapMapping);
					float uScale = matcapTexture->GetUScale();
					float vScale = matcapTexture->GetVScale();
					hasTexture = true;
					//如果u v 不同时为1
					if (Abs(uScale - 1) < 0.001 && Abs(vScale - 1.0f) < 0.001 || Abs(uScale) < 0.001f || Abs(vScale) < 0.001f)
					{

					}
					else
					{
						//构造变换纹理坐标变换矩阵
						tempMaterial->CreateTexture2DTransform();
						Matrix4* matrix4 = tempMaterial->GetTexture2DTransform();
						if (matrix4)
						{
							matrix4->m_m00 = uScale;
							matrix4->m_m11 = vScale;
						}
					}

				}
				//获取normalMap
				{
					HoteamSoft::SVLLib::Stk_TexturePtr normalTexture = materialPtr->GetNormalMap();
					if (normalTexture.isNotNull())
					{
						Texture2D * normalMapping = static_cast<Texture2D*> (GetTexture2D(&normalTexture));
						tempMaterial->SetNormalMap(normalMapping);
						hasTexture = true;
					}
					Vector2  xyScale;
					materialPtr->GetNormalScale(&xyScale.m_x);
					if (xyScale.m_x>0.0 && xyScale.m_y>0.0)
					{
						tempMaterial->NormalMapScale(xyScale);
					}
				}
				//获取发光贴图
				//TODO
				//获取...
				if (hasTexture)
				{
					Vector2  xyScale;
					materialPtr->GetUvTranslate(&xyScale.m_x);

					tempMaterial->SetUvTranslate(xyScale);

					materialPtr->GetUvScale(&xyScale.m_x);

					tempMaterial->SetUvScale(xyScale);

					float tempValue = 0.0;
					materialPtr->GetUvRotate(tempValue);

					tempMaterial->SetUvRotate(tempValue);
				}

				m_protoTypeMaterialCache.insert(std::pair<int, BaseMaterial*>(materialId, material));
				m_protoTypeColorCache.insert(std::pair<int, Color>(materialId, difuseColor));
			}
			else if (materialType == MaterialType_MatCap)
			{
				material = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateMaterial(materialIDStr, MaterialType_MatCap);
				MatCapMaterial* tempMaterial = static_cast<MatCapMaterial*>(material);
				wstring wMaterialName = materialPtr->GetMaterialName();
				string materialName = Platform::WStringToString(wMaterialName, "auto");
				tempMaterial->DisplayName(materialName);

				//获取diffuseColor
				materialPtr->GetDiffuseColor(&(difuseColor.m_r));
				materialPtr->GetTransparency(difuseColor.m_a);

				if (difuseColor.m_a < 0)
				{
					difuseColor.m_a = 1.0f;
				}
				tempMaterial->SetDiffuse(difuseColor);
				tempMaterial->Opacity(difuseColor.m_a);
			
				HoteamSoft::SVLLib::Stk_TexturePtr matcapTexture = materialPtr->GetMatcapMap();
				if (matcapTexture.isNotNull())
				{
					Texture * matcapMapping = GetTexture2D(&matcapTexture);
					tempMaterial->MatcapMap(matcapMapping);
					float uScale = matcapTexture->GetUScale();
					float vScale = matcapTexture->GetVScale();

					//如果u v 不同时为1
					if (Abs(uScale - 1) < 0.001 && Abs(vScale - 1.0f) < 0.001 || Abs(uScale) < 0.001f || Abs(vScale) < 0.001f)
					{

					}
					else
					{
						//构造变换纹理坐标变换矩阵
						tempMaterial->CreateTexture2DTransform();
						Matrix4* matrix4 = tempMaterial->GetTexture2DTransform();
						if (matrix4)
						{
							matrix4->m_m00 = uScale;
							matrix4->m_m11 = vScale;
						}
					}

				}
				//获取normalMap
				{
					HoteamSoft::SVLLib::Stk_TexturePtr normalTexture = materialPtr->GetNormalMap();
					if (normalTexture.isNotNull())
					{
						Texture2D * normalMapping = static_cast<Texture2D*> (GetTexture2D(&normalTexture));
						tempMaterial->SetNormalMap(normalMapping);
						//hasTexture = true;
					}
					Vector2  xyScale;
					materialPtr->GetNormalScale(&xyScale.m_x);
					if (xyScale.m_x > 0.0 && xyScale.m_y > 0.0)
					{
						tempMaterial->NormalMapScale(xyScale);
					}
				}
				//获取发光贴图
				//TODO
				//获取...


				m_protoTypeMaterialCache.insert(std::pair<int, BaseMaterial*>(materialId, material));
				m_protoTypeColorCache.insert(std::pair<int, Color>(materialId, difuseColor));
			}
			else if (materialType == StkMaterialTypeEnum::STK_MATERIALTYPE_PBR)
			{
				material = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateMaterial(materialIDStr, MaterialType_Pbr);
				PbrMaterial* tempMaterial = static_cast<PbrMaterial*>(material);
				//获取ambientColor
				//TODO

				//获取diffuseColor
				materialPtr->GetDiffuseColor(&(difuseColor.m_r));
				materialPtr->GetTransparency(difuseColor.m_a);
				wstring wMaterialName =  materialPtr->GetMaterialName();
				string materialName = Platform::WStringToString(wMaterialName);
				tempMaterial->DisplayName(materialName);

				if (difuseColor.m_a < 0)
				{
					difuseColor.m_a = 1.0f;
				}
				tempMaterial->AlbedoColor(difuseColor);
				tempMaterial->Opacity(difuseColor.m_a);
				//获取金属度 粗糙度
				//TODO
				float tempValue = 0.0;
				materialPtr->GetMetalnessFactor(tempValue);
				if (tempValue>=0.0)
				{
					tempMaterial->MetalnessFactor(tempValue);
				}

				materialPtr->GetRoughnessFactor(tempValue);
				if (tempValue>=0.0)
				{
					tempMaterial->RougthnessFactor(tempValue);
				}

				bool isUseClearCoat = false;
				materialPtr->GetUseClearCoat(isUseClearCoat);
				tempMaterial->UseClearCoat(isUseClearCoat);

				materialPtr->GetClearCoat(tempValue);
				if (tempValue>=0.0)
				{
					tempMaterial->ClearCoat(tempValue);
				}
				materialPtr->GetClearCoatRoughness(tempValue);
				if (tempValue>=0.0)
				{
					tempMaterial->ClearCoatRoughness(tempValue);
				}
				//获取反射贴图
				bool hasTexture = false;
				//TODO
				{
					//获取diffuse
					HoteamSoft::SVLLib::Stk_TexturePtr diffuseTexture = materialPtr->GetDiffuseTexture();
					if (diffuseTexture.isNotNull())
					{
						Texture2D * diffuseMapping = static_cast<Texture2D*> (GetTexture2D(&diffuseTexture));
						tempMaterial->AlbedoMap(diffuseMapping);
						hasTexture = true;
					}
				}
				//获取normalMap
				{
					HoteamSoft::SVLLib::Stk_TexturePtr normalTexture = materialPtr->GetNormalMap();
					if (normalTexture.isNotNull())
					{
						Texture2D * normalMapping = static_cast<Texture2D*> (GetTexture2D(&normalTexture));
						tempMaterial->NormalMap(normalMapping);
						hasTexture = true;
					}
					Vector2  xyScale;
					materialPtr->GetNormalScale(&xyScale.m_x);
					if (xyScale.m_x > 0.0 && xyScale.m_y > 0.0)
					{
						tempMaterial->NormalMapScale(xyScale);
					}
				}
				//获取发光贴图
				//TODO
				//获取...

				if (hasTexture)
				{
					Vector2  xyScale;
					materialPtr->GetUvTranslate(&xyScale.m_x);

					tempMaterial->SetUvTranslate(xyScale);

					materialPtr->GetUvScale(&xyScale.m_x);

					tempMaterial->SetUvScale(xyScale);

					materialPtr->GetUvRotate(tempValue);

					tempMaterial->SetUvRotate(tempValue);
				}

				m_protoTypeMaterialCache.insert(std::pair<int, BaseMaterial*>(materialId, material));
				m_protoTypeColorCache.insert(std::pair<int, Color>(materialId, difuseColor));

			}
			else
			{
				StkMaterialTypeEnum materialType = materialPtr->GetMaterialType();
				wstring wname = materialPtr->GetMaterialName();
				string name = Platform::WStringToString(wname);
				string materialIDStr = GetSVLMaterialID(this->m_M3DModel, materialId);
				//将材质加入全局资源管理器中，进行资源管理
				material = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateMaterial(name, MaterialType_Shader);
				if (!material->GetNeedsUpdateUniformParamerers())
				{
					return false;
				}
				material->DisplayName(name);
				//	vector<int> * vec = new vector<int>();
				//	vec->push_back(materialType);
				Uniform uniform("Int", (int)materialType);
				material->SetUniformParameter("type", uniform);
				ParseMaterialParameters(materialPtr, material);

				material->SetMaterialType((MaterialType)materialType);
			}
			//设置材质原始ID-added by yanghongpeng
			material->SetSvlId(materialPtr->GetID() + m_SVLIDOffset);
#ifdef WIN32
			IDCreator::Instance()->UpdateSVLID(SVLGlobalID(materialPtr->GetID() + m_SVLIDOffset));
#else
            SVLGlobalID temp(materialPtr->GetID() + m_SVLIDOffset);
			IDCreator::Instance()->UpdateSVLID(temp);
#endif
		}
		else 
		{
			//材质为空
			material = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateMaterial(M3D_DEFAULT_MATERIAL, MaterialType_Phong);

		}

		return true;
	}

	Texture * SVL2AsynReader::GetTexture2D(void* stkTexture)
	{
		Texture2D * ret = NULL;
		HoteamSoft::SVLLib::Stk_TexturePtr pStkTexture = *(Stk_TexturePtr*)stkTexture;
		{
			if (pStkTexture == nullptr)
				return NULL;
			HoteamSoft::SVLLib::Stk_ImagePtr pPic = pStkTexture->GetTextureImage();
			if (pPic == nullptr)
				return NULL;
			
				//获取图片数据长度
				STK_UINT32 o_nudataSize;
				STK_CHAR* o_chDataP = NULL;

				wstring strPicPath = pPic->GetPictureSavePath();
				int iIdx = strPicPath.find_first_of(L"\\");
				if (iIdx >= 0)
				{
					strPicPath.replace(iIdx, 1, L"/");
				}
				//将数据赋值给图片对象,当多次打开插入文件时，需要给资源一个偏移量
				string imagePathWithOffset = Platform::WStringToString(strPicPath) + "||" + StringHelper::IntToString(this->m_SVLIDOffset);
				Image* image = this->m_view->GetSceneManager()->GetResourceManager()->GetImage(imagePathWithOffset);
				if (image)
				{
					Texture2D* texture2d = (Texture2D*)this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateTexture(imagePathWithOffset, Texture::TEXTURE_2D);
					texture2d->SetImage(image);
					ret = texture2d;
				}
				else
				{
					Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
					(*m_svl2Doc)->GetSVLXFileItem(Platform::WStringToString(strPicPath), o_nudataSize, &o_chDataP, false);
					if (o_nudataSize <= 0)
					{
						LOGE("贴图ID(%u)，图片数据为0", pStkTexture->GetID());
						return NULL;
					}
					if (o_nudataSize > 0 && o_chDataP)
					{
						Texture2D* texture2d = (Texture2D*)this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateTexture(imagePathWithOffset, Texture::TEXTURE_2D);
						if (!image)
						{
							image = new Image();
							image->SetData((unsigned char*)o_chDataP, o_nudataSize);
							image->SetPath(imagePathWithOffset);
							AddRefMe(image);
							texture2d->SetImage(image);
							this->m_view->GetSceneManager()->GetResourceManager()->AddImage(imagePathWithOffset, image);
							ReleaseMe(image);
						}
						ret = texture2d;
					}
				}

				float uScale = pStkTexture->GetUScale();
				float vScale = pStkTexture->GetVScale();

				////如果u v 不同时为1
				//if (Abs(uScale - 1) < 0.001 && Abs(vScale - 1.0f) < 0.001 || Abs(uScale) < 0.001f || Abs(vScale) < 0.001f)
				//{
				//}
				//else
				//{
				//	//构造变换纹理坐标变换矩阵
				//	tempMaterial->CreateTexture2DTransform();
				//	Matrix4* matrix4 = tempMaterial->GetTexture2DTransform();
				//	if (matrix4)
				//	{
				//		matrix4->m_m00 = uScale;
				//		matrix4->m_m11 = vScale;
				//	}
				//}
		}
		return ret;
	}

	void SVL2AsynReader::LoadFileInfo()
	{
        this->m_view->SetInterBackgroundState(false);
		m_view->KeepBackgroundState();
		m_view->KeepViewModeState();
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
		(*m_svl2Doc)->LoadFileInfo();
		Stk_LoadInf* infoFilePtr = (*m_svl2Doc)->GetLoadInf()/*Stk_LoadInf::GetInstance()*/;
		m_instanceCount = infoFilePtr->GetInstanceCount();
		int numModel = infoFilePtr->GetModelCount();
		//背景信息
		
		int type = infoFilePtr->GetBackgroundType();
		int style = infoFilePtr->GetBackgroundStyle();
		if (type == 1)//颜色
		{
			if ( style == 1)
			{
				map<string, string> items = infoFilePtr->GetBackgroundItems();
				for (map<string, string>::iterator it = items.begin(); it != items.end(); it++)
				{
					string name = it->first;
					string val = it->second;
					vector<string> colors = StringHelper::Split(val, ",");

					vector<string> tops = StringHelper::Split(colors[0]," ");
					Color top(StringHelper::StringToFloat(tops[0]), StringHelper::StringToFloat(tops[1]), StringHelper::StringToFloat(tops[2]));
					vector<string> bottoms = StringHelper::Split(colors[1]," ");
					Color bottom(StringHelper::StringToFloat(bottoms[0]), StringHelper::StringToFloat(bottoms[1]), StringHelper::StringToFloat(bottoms[2]));
					this->m_view->SetBackgroundColor(top, bottom);

				}
				this->m_view->SetBackgroundUseImage(false);
				this->m_view->SetBackgroundUseSkyBox(false);
                this->m_view->SetInterBackgroundState(true);
			}
		}
		else if (type == 2)//图片
		{
			if (style ==1)
			{
				map<string, string> items = infoFilePtr->GetBackgroundItems();
				for (map<string, string>::iterator it = items.begin(); it != items.end(); it++)
				{
					string name = it->first;
					string val = it->second;
					unsigned int buffersize = 0;
					char* data;
					(*m_svl2Doc)->GetSVLXFileItem(val, buffersize, &data,false);
					//将数据赋值给图片对象

					Texture2D* texture2d = (Texture2D*)this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateTexture(val, Texture::TEXTURE_2D);
					texture2d->SetImageParameter(TEXTURE_LOAD_RGBA, TEXTURE_FLAG_MIPMAPS | TEXTURE_FLAG_INVERT_Y);
					//构造image，按道理来说image也应该重用
					Image* image = new Image();
					image->SetData((unsigned char *)(data),buffersize);
					AddRefMe(image);
					texture2d->SetImage(image);
					ReleaseMe(image);
					this->m_view->SetBackgroundTexture(texture2d);

				}
				this->m_view->SetBackgroundUseImage(true);
				this->m_view->SetBackgroundUseSkyBox(false);
                this->m_view->SetInterBackgroundState(true);
			}
		}
		else if( type == 3)//天空盒
		{
			if (style == 1)
			{
				map<string, string> items = infoFilePtr->GetBackgroundItems();
				for (map<string, string>::iterator it = items.begin(); it != items.end(); it++)
				{
					string name = it->first;
					string val = it->second;

					vector<string> pathes = StringHelper::Split(val, ",");
					vector<unsigned char*> buffers;
					vector<int> bufferLengths;	
					string imagePath = "";
					vector<Image*> cubeImages;
					for (int i = 0;i<pathes.size();i++)
					{
						imagePath = pathes[i];
						unsigned int buffersize = 0;
						char* data;
						//去除首尾空格
						if (!imagePath.empty())
					     {
							imagePath.erase(0, imagePath.find_first_not_of(" "));
							imagePath.erase(imagePath.find_last_not_of(" ") + 1);
					     }
						(*m_svl2Doc)->GetSVLXFileItem(imagePath, buffersize, &data,false);
						Image* image = this->m_view->GetSceneManager()->GetResourceManager()->GetImage(imagePath);
						if (!image)
						{
							image = new Image();
							image->SetPath(imagePath);
							image->SetData((unsigned char*)data, buffersize);
						}
						cubeImages.push_back(image);
					}

					Texture * cubeTexture = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateCubeMappingTexture(name, cubeImages);
					this->m_view->AddBackgroundSkyBoxTexture(name, cubeTexture);
				}
				this->m_view->SetBackgroundUseImage(false);
				this->m_view->SetBackgroundUseSkyBox(true);
                this->m_view->SetInterBackgroundState(true);
			}
		}
		/*else {
			this->m_view->SetBackgroundUseImage(false);
			this->m_view->SetBackgroundUseSkyBox(false);
		}*/

		{
			unsigned int infoSize = 0;
			char * infoptr = nullptr;
			(*m_svl2Doc)->GetSVLXFileItem(".info", infoSize, &infoptr);
			if (infoSize > 0)
			{
				string infoStr(infoptr);
				string realSrc = infoStr.substr(0, infoStr.find_last_of("}") + 1);
                realSrc = RemoveExcess(realSrc);
                if (realSrc.length() > 0) {
                    string tab = "}}}}}";
                    if (realSrc.find(tab) != string::npos) {
                        realSrc.erase(realSrc.find(tab.c_str())+tab.length(), realSrc.length());
                    }else{
                        tab = "}}}}";
                        if (realSrc.find(tab) != string::npos) {
                            realSrc.erase(realSrc.find(tab.c_str())+tab.length(), realSrc.length());
                        }else{
                            tab = "}}}";
                            if (realSrc.find(tab) != string::npos) {
                                realSrc.erase(realSrc.find(tab.c_str())+tab.length(), realSrc.length());
                            }
                        }
                    }
                }

				//			LOGI(infoptr);
				Document infoDoc;
				infoDoc.Parse(realSrc.c_str());
				Value::MemberIterator rootIt = infoDoc.FindMember("info");
				if (rootIt != infoDoc.MemberEnd() && rootIt->value.IsObject())
				{
					Value& infoNode = rootIt->value;

					Value::MemberIterator sceneIt = infoNode.FindMember("scene");
					if (sceneIt != infoNode.MemberEnd() && sceneIt->value.IsObject())
					{
						Value& sceneNode = sceneIt->value;

						//读取灯光 TODO
						if (sceneNode.HasMember("lights"))
						{
							int lightsSize = sceneNode["lights"].Size();
							for (int i = 0; i < lightsSize; i++)
							{
								DirectionalLight * m3dlight = new DirectionalLight();
								m3dlight->AddRef();
								Value& light = sceneNode["lights"][i];
								if (light.HasMember("type"))
								{
									int type = light["type"].GetInt();
									m3dlight->SetLightType((SHAPETYPE)type);
								}
								if (light.HasMember("position"))
								{
									string positionStr = light["position"].GetString();
									vector<string> strarray = StringHelper::Split(positionStr, ",");
									m3dlight->SetPosition(Vector3(StringHelper::StringToFloat(strarray[0]),
										StringHelper::StringToFloat(strarray[1]), StringHelper::StringToFloat(strarray[2])
										));
								}
								if (light.HasMember("spotDirection"))
								{
									string directionStr = light["spotDirection"].GetString();
									vector<string> strarray = StringHelper::Split(directionStr, ",");
									m3dlight->SetSpotDirection(Vector3(StringHelper::StringToFloat(strarray[0]),
										StringHelper::StringToFloat(strarray[1]), StringHelper::StringToFloat(strarray[2])));
								}

								if (light.HasMember("ambient"))
								{
									string ambientStr = light["ambient"].GetString();
									vector<string> strarray = StringHelper::Split(ambientStr, ",");
									m3dlight->SetAmbient(Color(StringHelper::StringToFloat(strarray[0]),
										StringHelper::StringToFloat(strarray[1]), StringHelper::StringToFloat(strarray[2]), 1.0));
								}

								if (light.HasMember("diffuse"))
								{
									string diffuseStr = light["diffuse"].GetString();
									vector<string> strarray = StringHelper::Split(diffuseStr, ",");
									m3dlight->SetDiffuse(Color(StringHelper::StringToFloat(strarray[0]),
										StringHelper::StringToFloat(strarray[1]), StringHelper::StringToFloat(strarray[2]), 1.0));
								}

								if (light.HasMember("specular"))
								{
									string specularStr = light["specular"].GetString();
									vector<string> strarray = StringHelper::Split(specularStr, ",");
									m3dlight->SetSpecular(Color(StringHelper::StringToFloat(strarray[0]),
										StringHelper::StringToFloat(strarray[1]), StringHelper::StringToFloat(strarray[2]), 1.0));
								}

								if (light.HasMember("constant"))
								{
									float constant = light["constant"].GetFloat();
									m3dlight->SetConstantAttenuation(constant);
								}

								if (light.HasMember("linear"))
								{
									float linear = light["linear"].GetFloat();
									m3dlight->SetLinearAttenuation(linear);
								}

								if (light.HasMember("quadratic"))
								{
									float quadratic = light["quadratic"].GetFloat();
									m3dlight->SetQuadraticAttenuation(quadratic);
								}

								if (light.HasMember("cutoff"))
								{
									float cutoff = light["cutoff"].GetFloat();
									m3dlight->SetSpotCutoff(cutoff);
								}

								if (light.HasMember("spotCosCutoff"))
								{
									float spotCosCutoff = light["spotCosCutoff"].GetFloat();
									m3dlight->SetSpotCosCutoff(spotCosCutoff);
								}

								if (light.HasMember("spotExponent"))
								{
									float spotExponent = light["spotExponent"].GetFloat();
									m3dlight->SetSpotExponent(spotExponent);
								}

								//this->m_view->GetSceneManager()->AddModelLight(m3dlight);
							}
						}


						//读取viewmode
						Value::MemberIterator viewmoderIt = sceneNode.FindMember("viewmode");
						if (viewmoderIt != sceneNode.MemberEnd() && viewmoderIt->value.IsObject())
						{
							Value& viewmodeNode = viewmoderIt->value;
							if (viewmodeNode.HasMember("operationType"))
							{
								int type = viewmodeNode["operationType"].GetInt();
								switch (type)
								{
								case 1:
									this->m_view->setWalkThrough(HANDLER_WALKTHROUGH);
									break;
								case 2:
									this->m_view->setWalkThrough(HANDLER_COMMON);
									break;
								case 3:
									this->m_view->setWalkThrough(HANDLER_DRAGGER);
									break;
								}

							}

							if (viewmodeNode.HasMember("upDirection"))
							{
								int direction = viewmodeNode["upDirection"].GetInt();
								Vector3 upDirection;
								switch (direction) {
								case 1:
									upDirection = Vector3::RIGHT;
									//                LOGI("Set +X");
									break;
								case 2:
									upDirection = Vector3::LEFT;
									//                LOGI("Set -X");
									break;
								case 3:
									upDirection = Vector3::UP;
									//                LOGI("Set +Y");
									break;
								case 4:
									upDirection = Vector3::DOWN;
									//                LOGI("Set -Y");
									break;
								case 5:
									upDirection = Vector3::FORWARD;
									//                LOGI("Set +Z");
									break;
								case 6:
									upDirection = Vector3::BACK;
									//                LOGI("Set -Z");
									break;
								default:
									break;
								}
								if (m_view) {
									TouchHandler *touchHandler;
									touchHandler = m_view->GetTouchHandler();
									touchHandler->SetUpDirection(upDirection, m_view);
                                    
									//        viewer->RequestDraw();
								}
							}

							if (viewmodeNode.HasMember("observingPattern"))
							{
								int oP = viewmodeNode["observingPattern"].GetInt();
								switch (oP)
								{
								case 1:
									m_view->GetTouchHandler()->FreeViewMode(true);
									break;
								case 2:
									m_view->GetTouchHandler()->OribitMode(true);
									break;
								case 3:
									m_view->GetTouchHandler()->ConstraintMode(true);
									break;
								}
							}
						}

					}
				}
			}
		}

	}
 
	void  SVL2AsynReader::ClearPartsMap()
	{
		this->m_partModelsMap.clear();
	}

	void SVL2AsynReader::SetPercent(float percent)
	{
		if (this->m_readerListener)
		{
			this->m_readerListener->OnProcessPercent(this, percent);
		}
	}
	
	void SVL2AsynReader::updateReadInstancePercent()
	{
		this->readInstanceIndex++;
		//instance 的进度范围从0-1
		if (this->m_instanceCount)
		{
			this->SetPercent(0.5f+0.5f*readInstanceIndex*1.0f / this->m_instanceCount);
		}
		else
		{
			this->SetPercent(0);
		}
	}

	string SVL2AsynReader::GetSVLMaterialID(Model* model, unsigned int renderID)
	{
		string materialID;

		if (renderID >= 0)
		{
			materialID = "SVLXRender" +
				StringHelper::IntToString(model->GetID())
				+ "|"
				+
				StringHelper::IntToString(renderID);
		}

		return materialID;
	}

	void SVL2AsynReader::FillAssemblyBBox(Model* model)
	{
		//填充模型最基本的信息
		this->FillSingleModelBBox(model);

		for (int i = 0; i < model->GetSubModels().size(); i++)
		{
			Model* subModel = static_cast<Model*>(model->GetSubModels().at(i));
			FillAssemblyBBox(subModel);
		}
	}

	void SVL2AsynReader::FillSingleModelBBox(Model* model)
	{
		SModelFileInfo* fileInfo = model->GetFileInfo();
		if (fileInfo) {
			Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
			int stk_nodeId = fileInfo->GetPartModelId();
			Stk_ModelPtr stkModel = (*m_svl2Doc)->GetModelByID(stk_nodeId-m_SVLIDOffset);
			if (stkModel.isNotNull())
			{
				Model* tgroup = dynamic_cast<Model*>(model);

				ModelShape* tShapeNode = new ModelShape();
				tgroup->SetModelShape(tShapeNode);
				tgroup->SetDrawDataPrepared(false);

				float boxData[6] = { 0 };
				stkModel->GetBoundingBox(boxData, 6);
				static float checkLimint = 9.0e+5f;
				if (!IsNaN(boxData[0]) && Abs(boxData[0])<checkLimint
					&&Abs(boxData[1])<checkLimint
					&&Abs(boxData[2])<checkLimint
					&&Abs(boxData[3])<checkLimint
					&&Abs(boxData[4])<checkLimint
					&&Abs(boxData[5])<checkLimint)
				{
					BoundingBox modelBox = BoundingBox(Vector3(boxData[0], boxData[1], boxData[2]), Vector3(boxData[3], boxData[4], boxData[5]));
					tShapeNode->SetBoundingBox(modelBox);
				}
			}
		}
	}

	void SVL2AsynReader::SimplyFillAssembly(Model* model, void* pstk_NodePtr)
	{
		//Stk_InstancePtr stk_Instance = *(Stk_InstancePtr*)pstk_NodePtr;
		//if (model && stk_Instance != NULL)
		//{
		//	//READER_LOGI("mesh count: 1111");
		//	//aiMesh* curAIMesh = scene->mMeshes[curAINode->mMeshes[imesh]];
		//	model->SetName(Platform::WStringToString(stk_Instance->GetNodeName(), "auto"));
		//	
		//	Matrix4 plcMatrix;
		//	float* matrixData = &plcMatrix.m_m00;
		//	stk_Instance->GetMatrix(matrixData, 12);
		//	model->SetPlaceMatrix(plcMatrix);
		//	//int meshCount = stk_Node->GetNbMeshPrim();
		//	//填充模型最基本的信息
		//	//this->SimplyFillModel(this->m_view, model, false);
		//	if (stk_Instance->GetModelID() > 0)
		//	{
		//		SModelFileInfo* fileInfo = new SModelFileInfo(model);
		//		fileInfo->SetPartModelId(stk_Instance->GetModelID());
		//		//fileInfo->SetStkNodePtr(NULL);
		//		model->SetFileInfo(fileInfo);
		//		m_modelStkModelMap.insert(Model_StkModel_Map::value_type(model, stk_Instance->GetModelID()));
		//	}
		//	for (int subModelIndex = 0; subModelIndex < stk_Instance->GetNbChild(); subModelIndex++)
		//	{
		//		Model* childModel = new Model();
		//		Stk_InstancePtr nodePtr = stk_Instance->GetChild(subModelIndex);
		//		SimplyFillAssembly(childModel, &nodePtr);
		//		model->AddSubModel(childModel);
		//	}
		//}
	}

	const string& SVL2AsynReader::GetXMLAnimationData() 
	{
		return this->m_xmlAnimatinData;
	}
	void SVL2AsynReader::SetXMLAnimationData(const string& xmlData) 
	{
		this->m_xmlAnimatinData = xmlData;
		
	}
	string SVL2AsynReader::GetAnimationName(const char* path)
	{
		 string sPath = path;
		 string name = "";
		 std::size_t pos = sPath.find_last_of("\\");
		 std::size_t rpos = sPath.find_last_of('.');

		 if (pos != string::npos && rpos != string::npos && rpos > pos)
		 {
			 name = sPath.substr(pos + 1, rpos - pos);
		 }
		 name += "animation";
		 return name;
	}
	void SVL2AsynReader::GetInstanceData(Model* model, void* instancePtr)
	{
		if (!model)
		{
			return;
		}
		
		Stk_InstancePtr instance = instancePtr != NULL ? *(Stk_InstancePtr*)instancePtr : NULL;
		int PlcID = instance == NULL ? 0 : instance->GetPlcID();
		model->SetPlcId(PlcID);
		model->SetInstanceID(instance->GetID());
		if (model->GetChildren().size() == 0)
		{
			return;
		}
		vector<Stk_InstancePtr>& vecChildIns = instance->getChildren();
		for (int subModelIndex = 0; subModelIndex < vecChildIns.size(); subModelIndex++)
		{
			Model* childModel = model->GetChildren().at(subModelIndex);
			Stk_InstancePtr subInstanceJson = vecChildIns.at(subModelIndex);
			GetInstanceData(childModel, &subInstanceJson);
		}
	}
	void SVL2AsynReader::GetPMIsData(Model* model)
	{
		if (!model)
		{
			return;
		}
		SModelFileInfo* fileInfo = model->GetFileInfo();
		if (fileInfo) {
			Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
			int stk_nodeId = fileInfo->GetPartModelId();
			vector<Stk_PMIPtr> stk_PMIList;
			Stk_ModelPtr stkModel = (*m_svl2Doc)->GetModelByID(stk_nodeId);
			if (stkModel != NULL)
			{
				stkModel->GetAllPMI(stk_PMIList);
			}
			
			int pmiCount = 0;
			vector<PMIData*> pOutPMIData;
			GetPMIInfo(&stk_PMIList, &pmiCount, pOutPMIData);
			if (pOutPMIData.size() > 0)
			{
				map<int, PMIData*> pmis;
				for (int i = 0; i < pOutPMIData.size(); i++) {
					PMIData* pTmpPMI = pOutPMIData.at(i);
					pmis.insert(
						map<int, PMIData*>::value_type(pTmpPMI->GetID(), pTmpPMI));
				}
				model->SetModelExtInfo(m_view->GetSceneManager()->GetExtendInfoManager());
				model->SetPMIs(pmis);
			}
		}

		vector<Model *> SubModelList = model->GetSubModels();
		for (int i = 0; i < SubModelList.size(); i++)
		{
			Model* subModel = SubModelList.at(i);
			GetPMIsData(subModel);
		}

	}

	bool SVL2AsynReader::GetPMIInfo(vector<Stk_PMIPtr>* pStkPMIList, int *outPMINum,
		vector<PMIData*>& outPMIData) {

		int count;
		int Error = FALSE;
		CHAR strID[32];
		int PMINum = 0;
		int MaxNum = 1000;

		//vector<FTPoint> textpnt;
		vector<int> num;
		vector<int> type;

		SFPOINT3 pnts;
		int idx = 0, j = 0, n = 0;

		vector<SFPOINT3> vecPolylinePnts;

		if (pStkPMIList->size() <= 0) {
			return true;
		}

		outPMIData = vector<PMIData*>();
		SVLXPMICreator *pPMICreator = new SVLXPMICreator;

		for (int i = 0; i < pStkPMIList->size(); i++) {

			Stk_PMIPtr pmi = pStkPMIList->at(i);
			STK_ID PMIid = pmi->GetID();
			wstring PMIName = pmi->GetName();
			string pmiName = Platform::WStringToString(PMIName);
			//sprintf(strID, "%d", PMIid);
			//PMIName += strID;

			//保存名称
			PMIData *pPMIData = new PMIData;
			pPMIData->SetID(PMIid);
			pPMIData->SetName(pmiName);

			//设置PMI显隐状态
			pPMIData->SetVisible(pmi->IsDisplay());

			//设置PMIPick状态

			//设置PMI颜色 pmi暂无颜色
			//STK_RGBA32 pmiColor;

			Color blue(0, 0, 1, 1);
			pPMIData->SetInitColor(blue); //设置为蓝色
										  //平行于屏幕类型PMI
			//STK_BOOL ParallelScreenflg = pmi->IsParallelScreen();

			STK_INT32 style = pmi->GetStyle();
			LOGI("PMIStypestype %d", style);

			bool ParallelScreenflg = style & ISPARALLEL ? true : false;
			pPMIData->m_IsParallelScreen = ParallelScreenflg;
			pPMIData->m_Type = pmi->GetType();

			// 设置定义平面
			HoteamSoft::SVLLib::STK_MTX32 DefPlane = pmi->GetDefPlane();
			pPMIData->SetDefPlane(Matrix3x4((float*)DefPlane.PlcMatrix));

			SFPOINT3 pnt;
			vector<STK_UINT32> Indices;

			/*
			* 创建PMI延长线 start
			*/
			//		count = 0;
			//		pnt.clear();
			//		Indices.clear();
			//		Error = pPMICreator->CreateExtensionLines(pmi, pnt, Indices);
			//		if (Error != TRUE) {
			//			continue;
			//		}
			//		if (pnt.size() > 0 && Indices.size() > 0) {
			//			//ISSUE:PMI android 生成延长线顶点列表
			//			SetPolyLineForPMI(pPMIData, pnt, Indices);
			//		}
			vecPolylinePnts.clear();
			vector<Stk_ExtensionlinePtr> vecExtline = pmi->GetExLines();
			if (vecExtline.size()>0)
				pPMICreator->createExtensionLines(vecExtline, vecPolylinePnts);
			n = vecPolylinePnts.size();
			//READER_LOGI("末端符号的数量为：(%d)", n);
			//				for (idx = 0; idx < n; idx++) {
			//					pnts = vecPolylinePnts.at(idx);
			//					for (j = 0; j < pnts.size(); j++)
			//						READER_LOGI(
			//								"末端符号点的坐标为：(%f,%f,%f)", pnts.at(j).X, pnts.at(j).Y, pnts.at(j).Z);
			//				}
			if (n > 0)
			{
				//ISSUE:PMI android 创建末端符号
				SetPolyLineForPMI(pPMIData, vecPolylinePnts);
			}
			vecPolylinePnts.clear();

			/*
			* 创建PMI延长线 end
			*/
			READER_LOGI("创建PMI延长线");
			/*
			* 创建外框 start
			*/
			count = 0;
			pnt.clear();
			Indices.clear();
			Error = pPMICreator->CreateOutFrame(pmi, pnt, Indices);
			if (Error != TRUE) {
				continue;
			}

			if (pnt.size() > 0 && Indices.size() > 0) {
				//ISSUE:PMI android 生成外框顶点列表
				SetPolyLineForPMI(pPMIData, pnt, Indices);
			}
			/*
			* 创建外框 end
			*/
			READER_LOGI("创建外框 end");
			/*
			* 创建包围框 start
			*/
			count = 0;
			pnt.clear();
			Indices.clear();
			Error = pPMICreator->CreateAnchorPoints(pmi, pnt, Indices);
			if (Error != TRUE) {
				continue;
			}
			if (pnt.size() > 0 && Indices.size() > 0) {
				//ISSUE:PMI android 生成包围框顶点列表
				SetPolyLineForPMI(pPMIData, pnt, Indices);
			}
			/*
			* 创建包围框 end
			*/
			READER_LOGI("创建包围框 end");
			/*
			* 创建末端符号 start
			*/
			//		count = 0;
			//		pnt.clear();
			//		Indices.clear();
			HoteamSoft::SVLLib::StkTermTypeEnum nEndSymbol = HoteamSoft::SVLLib::TERM_PATSMNONE;
			//Error = pPMICreator->CreateEndSymbol(pmi, pnt, Indices, nEndSymbol);
			vecPolylinePnts.clear();
			Error = pPMICreator->CreateEndSymbol(pmi, vecPolylinePnts, nEndSymbol);
			//		if (Error != TRUE) {
			//			continue;
			//		}

			n = vecPolylinePnts.size();
			READER_LOGI("末端符号的数量为：(%d)", n);
			for (idx = 0; idx < n; idx++) {
				pnts = vecPolylinePnts.at(idx);
				for (j = 0; j < pnts.size(); j++)
					READER_LOGI(
						"末端符号点的坐标为：(%f,%f,%f)", pnts.at(j).X, pnts.at(j).Y, pnts.at(j).Z);
			}
			if (n > 0) {
				//ISSUE:PMI android 创建末端符号
				SetPolyLineForPMI(pPMIData, vecPolylinePnts);
			}
			vecPolylinePnts.clear();
			/*
			* 创建末端符号 end
			*/
			READER_LOGI("创建末端符号 end");
			/*
			* 创建文字 start
			*/


			HoteamSoft::SVLLib::STK_BOX32 textBox;
			memset(&textBox, 0, sizeof(HoteamSoft::SVLLib::STK_BOX32));

			//ISSUE:PMI android 创建文字
			vector<Stk_ComTextPtr> vecComTexts = pmi->GetComText();
			int nComTextSize = vecComTexts.size();
			READER_LOGI("PMI android 创建文字 start cnt:%d", nComTextSize);

			pPMICreator->ClearTextBox();

			//----------transport from sview2.0 SceneCreator.cpp -----------

			//如果是平行于屏幕类型
			if (ParallelScreenflg) {
				vector<Stk_LeaderPtr> pLeads = pmi->GetLeaders();
				Stk_CurveEntityPtr pCurve = NULL;
				Stk_PolyLinePtr pPolyLinePtr = NULL;
				Stk_LeaderPtr pLeader = NULL;

				//引线相关的某个顶点
				STK_PNT32 stubChrPnt;

				//引线相关的某个标志
				bool bStubChrFlg = false;

				vector<STK_PNT32> leaderPnts;

				float ofWidth = 0.0, ofHeight = 0.0f;
				STK_DIR32 oDirXVec, oDirYVec;
				float oSpecing = 0.0f;
				int nComTextsNum = 0;
				int u = 0;

				READER_LOGI("如果有引线... size:%d", pLeads.size());
				//如果有引线，且类型为CURVE_TYPE_POLYLINE，且顶点个数>2则设置标志和读取pnt。
				if (pLeads.size() > 0) {
					pLeader = pLeads.at(0);
		
					vector<Stk_CurveEntityPtr> CurveList = pLeader->GetAllCurve();
					pCurve = CurveList.size() > 0 ? CurveList[CurveList.size() - 1] : NULL;

					//如果是折线
					if (pCurve != NULL && pCurve->GetCurveType() == 1) { //if (nTypeEnum == CURVE_TYPE_POLYLINE)

						pPolyLinePtr = Stk_PolyLinePtr::DynamicCast(pCurve);
						leaderPnts = pPolyLinePtr->GetPoints();
						if (leaderPnts.size() > 2) {
							bStubChrFlg = true;
							stubChrPnt = leaderPnts.at(1); //第二个顶点
														   //						pPMIData->m_outramLocation = Vector3(stubChrPnt.X,stubChrPnt.Y,stubChrPnt.Z);
						}
					}
				}

				nComTextsNum = vecComTexts.size();
				READER_LOGI("循环每个Stk_ComText文本 nComTextsNum:%d", nComTextsNum);
				//循环每个Stk_ComText文本
				for (u = 0; u < nComTextsNum; u++) {
					int v = 0, w = 0, nTextNum = 0, nSize2 = 0, nLin = 0;

					STK_INT32 nMaxLen = 0;

					Stk_TextPtr Text = NULL , tmpTxtP = NULL;
					STK_PNT32 Pos;
					Stk_ComTextPtr stkComText = vecComTexts.at(u); //当前Stk_ComText

					STK_BOX32 outerBox;
					vector<Stk_TextPtr> Texts = stkComText->GetTexts(); //获取所有Stk_Text
					vector<STK_INT32> aNember;
					//typedef vector<STK_INT32> SameLine;
					map<STK_INT32, vector<STK_INT32> > mapLine;

					vector<Stk_TextPtr> Texts2;
					vector<Stk_TextPtr> useSVLTexts2;
					STK_PNT32 Loc1;
					nTextNum = Texts.size();

					memset(&Pos, 0, sizeof(STK_PNT32));
					memset(&Loc1, 0, sizeof(STK_PNT32));
					memset(&outerBox, 0, sizeof(STK_PNT32));

					outerBox = stkComText->GetOuterBox();
					Loc1.X = outerBox.BoundBox[0][0];
					Loc1.Y = outerBox.BoundBox[1][1];

					READER_LOGI("是否固定位置:%d", pPMIData->m_Type == 8000);
					//如果PMI不是固定位置的类型：比如固定在左下角的“技术要求”
					if (8000 != pPMIData->m_Type) {
						//循环每行文字，根据y坐标判断有多少行，
						//并且将每行对应的文本索引（多个）记录下来
						for (v = 0; v < nTextNum; v++) {
							Text = Texts.at(v);
							Pos = Text->GetInnerLoc();
							bool bFirst = true;

							if (v == 0)
							{
								pPMIData->m_outramLocation = Vector3(Pos.X, Pos.Y, Pos.Z);
							}

							//向aNember中push 当前文本向后
							//y坐标相同的除第二 （[v + 1]）个文本外的索引值？
							for (w = v + 1; w < nTextNum; w++) {
								tmpTxtP = Texts.at(w);
								if (Pos.Y == tmpTxtP->GetInnerLoc().Y) {
									if (bFirst) {
										aNember.push_back(w - 1);
										bFirst = false;
									}
									aNember.push_back(w);
									v = w;
								}
							}

							//如果一个相同y的也没有，则push当前索引
							if (aNember.size() <= 0)
								aNember.push_back(v);

							//将aNember插入到mapLine
							mapLine.insert(pair<STK_INT32, vector<STK_INT32> >(nLin, aNember));
							nLin++; //行号？
							aNember.clear();
						}

						//最终有mapLine行，循环每行
						for (v = 0; v < mapLine.size(); v++) {
							nLin = mapLine.size();
							aNember = mapLine[v];
							int member = aNember.size();
							int linNo = 0;
							wstring tmpWstrs;
							//获取第0个文本的属性？
							Texts.at(0)->GetCharWidthAndHeight(ofWidth, ofHeight);
							Texts.at(0)->GetInnerXYAxis(oDirXVec, oDirYVec);
							oSpecing = Texts.at(0)->GetCharSpace();

							READER_LOGI(
								"faceScreenText :W:%f,H:%f", ofWidth, ofHeight);

							float maxH = 0.8f;
							if (ofWidth > maxH) {
								ofWidth = maxH;
								ofHeight = maxH;
							}

							//循环每个当前行的文本，将文本串加入tmpWstrs
							for (w = 0; w < member; w++) {
								linNo = aNember.at(w);

								Text = Texts.at(linNo);
								tmpWstrs.append(Text->GetTextContent());
							}

							//将文本设置给pTmpstr，并push入Texts2
							if (tmpWstrs.size()) {
								//临时Text
								Stk_TextPtr pTmpstr = Stk_TextPtr::CreateObject();
								pTmpstr->SetCharWidthAndHeight(ofWidth, ofHeight);
								pTmpstr->SetInnerXYAxis(oDirXVec, oDirYVec);
								pTmpstr->SetCharSpace(oSpecing);

								//设置引线相关的顶点
								if (bStubChrFlg)
									pTmpstr->SetInnerLoc(stubChrPnt);
								pTmpstr->SetTextContent(tmpWstrs);
								STK_PNT32 pntTmp =
									Texts.at(aNember.at(0))->GetInnerLoc();
								pTmpstr->SetInnerLoc(pntTmp);
								Texts2.push_back(pTmpstr);
							}
						} //end for (v = 0; v < mapLine.size(); v++) 最终组装了Texts2

						  //如果mapline为空，则复制Texts到Texts2
						if (mapLine.size() <= 0) {
							for (v = 0; v < Texts.size(); v++)
								Texts2.push_back(Texts.at(v));
						}

						//获取最长行
						nSize2 = Texts2.size();
						for (v = 0; v < nSize2; v++) {
							int nSz = Texts2.at(v)->GetTextContent().size();
							if (nMaxLen < nSz)
								nMaxLen = nSz;
						}
						//如果引线参数为真，加入Text2
						if (bStubChrFlg) {
							Stk_TextPtr pStubTxt = Stk_TextPtr::CreateObject();
							pStubTxt->SetTextContent(L"───");

							//画一个和最长行相同长度的--线
							if (nMaxLen > 0) {
								wstring wstr;
								wstr.append(nMaxLen, L'─');
								pStubTxt->SetTextContent(wstr);
							}

							Texts2.at(0)->GetCharWidthAndHeight(ofWidth, ofHeight);
							Texts2.at(0)->GetInnerXYAxis(oDirXVec, oDirYVec);
							oSpecing = Texts2.at(0)->GetCharSpace();

							pStubTxt->SetCharWidthAndHeight(ofWidth, ofHeight);
							pStubTxt->SetInnerXYAxis(oDirXVec, oDirYVec);
							pStubTxt->SetCharSpace(oSpecing);
							pStubTxt->SetInnerLoc(stubChrPnt);

							Texts2.push_back(pStubTxt);
						}
					}
					else {
						//^^ if (8000 != pPMISep->m_Type)  若是8000型则直接赋值。
						useSVLTexts2 = Texts;
					}

					//TODO:设置对齐方式？？
					//Paralleltext->justification = SoText2::Justification::LEFT;
					nSize2 = Texts2.size();
					//文本个数？
					//Paralleltext->string.setNum(nSize2);
					//循环Texts2的每个文本
					vector<Stk_TextPtr> texts;

					if (nSize2>0)
					{

						for (j = 0; j < nSize2; j++) {
							Stk_TextPtr newText = Stk_TextPtr::CreateObject();
							Stk_TextPtr tmpText = Texts2[j];
							STK_FLOAT32 charSpacing = tmpText->GetCharSpace();

							newText->SetCharSpace(charSpacing);

							STK_FLOAT32 charWidth, charHeight;
							tmpText->GetCharWidthAndHeight(charWidth, charHeight);
							newText->SetCharWidthAndHeight(charWidth, charHeight);

							STK_PNT32 innerLoc = tmpText->GetInnerLoc();
							newText->SetInnerLoc(innerLoc);

							STK_DIR32 x, y;
							tmpText->GetInnerXYAxis(x, y);
							newText->SetInnerXYAxis(x, y);

							newText->SetTextContent(tmpText->GetTextContent());
							texts.push_back(newText);

							string tmpStr = Platform::WStringToString(tmpText->GetTextContent());
							READER_LOGI("Text:%s", tmpStr.c_str());
							READER_LOGI(
								"charWidth:%f,charHeight:%f ", charWidth, charHeight);

							pPMIData->fontSize = charHeight;
						}

						//释放Texts2
						Texts2.clear();
					}
					else if (useSVLTexts2.size()>0)
					{
						Texts2 = useSVLTexts2;
						for (j = 0; j < nSize2; j++) {
							Stk_TextPtr newText = Stk_TextPtr::CreateObject();
							Stk_TextPtr tmpText = Texts2[j];
							STK_FLOAT32 charSpacing = tmpText->GetCharSpace();

							newText->SetCharSpace(charSpacing);

							STK_FLOAT32 charWidth, charHeight;
							tmpText->GetCharWidthAndHeight(charWidth, charHeight);
							newText->SetCharWidthAndHeight(charWidth, charHeight);

							STK_PNT32 innerLoc = tmpText->GetInnerLoc();
							newText->SetInnerLoc(innerLoc);

							STK_DIR32 x, y;
							tmpText->GetInnerXYAxis(x, y);
							newText->SetInnerXYAxis(x, y);

							newText->SetTextContent(tmpText->GetTextContent());
							texts.push_back(newText);

							string tmpStr = Platform::WStringToString(tmpText->GetTextContent());
							READER_LOGI("Text:%s", tmpStr.c_str());
							READER_LOGI(
								"charWidth:%f,charHeight:%f ", charWidth, charHeight);

							pPMIData->fontSize = charHeight;
						}
						//引用的是SVL中的数据，不用释放
					}

					Stk_ComTextPtr comTextPtr = Stk_ComTextPtr::CreateObject();
					comTextPtr->SetTexts(texts);

					ComText* myText = ConvertStkComTextToComText(comTextPtr);

					//填充文本
					pPMIData->m_ComTexts.push_back(myText);

				}
			}
			//-----------------------------end transport---------------------------------

			//如果不是面朝屏幕的类型PMI
			for (idx = 0; idx <nComTextSize && !ParallelScreenflg; idx++) {
				//复制Stk_ComText 到 ComText
				Stk_ComTextPtr tmpComText = pmi->GetComText()[idx];
				ComText* comText = ConvertStkComTextToComText(tmpComText);
				pPMIData->m_ComTexts.push_back(comText);
			}

			BoundingBox box = pPMIData->GetComTextsBox();

			//LOGI("Comtexts box %s",box.Tostring().c_str());

			if (box.Defined())
			{
				textBox.BoundBox[0][0] = box.m_min.m_x;
				textBox.BoundBox[0][1] = box.m_min.m_y;
				textBox.BoundBox[0][2] = box.m_min.m_z;

				textBox.BoundBox[1][0] = box.m_max.m_x;
				textBox.BoundBox[1][1] = box.m_max.m_y;
				textBox.BoundBox[1][2] = box.m_max.m_z;
			}

			/*
			* 创建文字 end
			*/
			READER_LOGI("创建文字 end");
			/*
			* 创建PMI引出线 start
			*/
			count = 0;
			pnt.clear();
			Indices.clear();

			//Error = pPMICreator->CreateLeaders(pmi, pnt, Indices);

			//		if (Error != TRUE) {
			//			continue;
			//		}
			//		if (pnt.size() > 0 && Indices.size() > 0) {
			//
			//			//ISSUE:PMI android 生成引出线顶点列表
			//		SetPolyLineForPMI(pPMIData, pnt, Indices);
			//		}
			HoteamSoft::SVLLib::StkPMITypeEnum EnumType = pmi->GetType();
			vector<Stk_LeaderPtr> pLeaders = pmi->GetLeaders();

			//READER_LOGI("PMI的类型为：(%d)",EnumType );
			if (
				//StkPMITypeEnum::PMI_TYPE_AngularDimension==EnumType
				//				||StkPMITypeEnum::PMI_TYPE_DiameterDimension == EnumType
				//				||StkPMITypeEnum::PMI_TYPE_RadiusDimension   == EnumType
				//				||StkPMITypeEnum::PMI_TYPE_DistanceDimension == EnumType
				//				||StkPMITypeEnum::PMI_TYPE_LengthDimension   == EnumType
				//				||StkPMITypeEnum::PMI_TYPE_TypeDimension == EnumType
				1) {


				pPMICreator->createLeader(pLeaders, textBox, vecPolylinePnts, pmi);

			}
			else {
				for (idx = 0; idx < pLeaders.size(); idx++)
					pPMICreator->createLeader(pLeaders[idx], vecPolylinePnts, pmi);
			}

			n = vecPolylinePnts.size();
			READER_LOGI("引出线的数量为：(%d)", n);
			SFPOINT3 _tempPnts;
			for (idx = 0; idx < n; idx++) {
				_tempPnts = vecPolylinePnts.at(idx);
				for (j = 0; j < _tempPnts.size(); j++)
					READER_LOGI(
						"引出线点的坐标为：(%f,%f,%f)", _tempPnts.at(j).X, _tempPnts.at(j).Y, _tempPnts.at(j).Z);
			}
			if (n > 0)
				SetPolyLineForPMI(pPMIData, vecPolylinePnts);
			/*
			* 创建PMI引出线 end
			*/
			READER_LOGI("创建PMI引出线 end");
			/*
			* 创建PMI特殊线 start
			*/
			count = 0;
			pnt.clear();
			Indices.clear();
			Error = pPMICreator->CreateSpeciallLines(pmi, pnt, Indices);
			if (Error != TRUE) {
				continue;
			}
			if (pnt.size() > 0 && Indices.size() > 0) {

				//ISSUE:PMI android 生成特殊线顶点列表
				SetPolyLineForPMI(pPMIData, pnt, Indices);
			}
			/*
			* 创建PMI特殊线 end
			*/

			READER_LOGI("创建PMI特殊线 end");

			outPMIData.push_back(pPMIData);
			PMINum++;
		}

		*outPMINum = PMINum;
		READER_LOGI("PMINum:%d", PMINum);

		delete pPMICreator;
		return TRUE;
	}
	//为PMIData组合添加PolyLine
	void SVL2AsynReader::SetPolyLineForPMI(PMIData* pPMIData,
		vector<SFPOINT3>& vecPolylinePnts) {

		for (int iLine = 0; iLine < vecPolylinePnts.size(); iLine++) {
			SFPOINT3 &curLine = vecPolylinePnts.at(iLine);
			//vertexBuffer
			//转化pnt列为float列
			float * tmpVertexBuf = new float[3 * (curLine.size())];
			M3D_INDEX_TYPE * tmpIndexBuf = new M3D_INDEX_TYPE[curLine.size()];
			memset(tmpIndexBuf, 0, curLine.size());

			for (int j = 0; j < curLine.size(); j++) {
				tmpVertexBuf[j * 3] = curLine[j].X;
				tmpVertexBuf[j * 3 + 1] = curLine[j].Y;
				tmpVertexBuf[j * 3 + 2] = curLine[j].Z;

				//indexBuffer
				tmpIndexBuf[j] = j;
			}
			//
			PolyLine *pLine = new PolyLine();
			pLine->SetBuffer(curLine.size(), tmpVertexBuf, curLine.size(),
				tmpIndexBuf);
			pPMIData->m_Lines.push_back(pLine);

			delete[] tmpVertexBuf;
			delete[] tmpIndexBuf;
		}

	}
	void SVL2AsynReader::SetPolyLineForPMI(PMIData* pPMIData,
		vector<HoteamSoft::SVLLib::STK_PNT32>& vertexBuf, vector<STK_UINT32>& indexBuf) {
		//转化pnt列为float列
		float * tmpVertexBuf = new float[3 * (vertexBuf.size())];
		for (int j = 0; j < vertexBuf.size(); j++) {
			tmpVertexBuf[j * 3] = vertexBuf[j].X;
			tmpVertexBuf[j * 3 + 1] = vertexBuf[j].Y;
			tmpVertexBuf[j * 3 + 2] = vertexBuf[j].Z;
		}

		//处理index列
		int indexNum = 0;
		M3D_INDEX_TYPE * tmpIndexBuf = new M3D_INDEX_TYPE[indexBuf.size()];
		for (int j = 0; j < indexBuf.size(); j++) {
			tmpIndexBuf[indexNum] = indexBuf[j];
			//如果为-1则：
			if (indexBuf[j] == -1 && j != 0) {
				//保存当前折线
				PolyLine *pLine = new PolyLine();
				pLine->SetBuffer(vertexBuf.size(), tmpVertexBuf, indexNum,
					tmpIndexBuf);
				pPMIData->m_Lines.push_back(pLine);

				//从新开始计数
				indexNum = 0;
				//delete[] tmpIndexBuf;
				//tmpIndexBuf = new M3D_INDEX_TYPE[indexBuf.size()];
				continue;
			}
			indexNum++;
		}

		//保存当前折线
		/*PolyLine *pLine = new PolyLine();
		pLine->SetBuffer(vertexBuf.size(), tmpVertexBuf, indexNum, tmpIndexBuf);
		pPMIData->m_Lines.push_back(pLine);*/

		delete[] tmpVertexBuf;
		delete[] tmpIndexBuf;
	}
	ComText* SVL2AsynReader::ConvertStkComTextToComText(Stk_ComTextPtr stkComText) 
	{
		ComText* comText = new ComText();

		STK_BOX32 outerBox = stkComText->GetOuterBox();
		Vector3 minPos(outerBox.BoundBox[0]); //可能不对
		Vector3 maxPos(outerBox.BoundBox[1]);
		BoundingBox box(minPos, maxPos);
		comText->SetBox(box);

		vector<Stk_TextPtr> tmpTexts = stkComText->GetTexts();

		for (int j = 0; j < tmpTexts.size(); j++) {
			CText* newText = new CText();
			Stk_TextPtr tmpText = tmpTexts[j];
			STK_FLOAT32 charSpacing = tmpText->GetCharSpace();

			//newText->SetCharSpacing(charSpacing);

			STK_FLOAT32 charWidth, charHeight;
			tmpText->GetCharWidthAndHeight(charWidth, charHeight);
			newText->SetCharWidthHeight(charWidth, charHeight);

			//STK_WCHAR oText[64] =
			//{ 0 };
			wstring tmpWS = tmpText->GetFontName();
			//		if (NULL != oText)
			//		{
			//			wstring tmpWS = Platform::STKWCHARStrToWString(
			//					(STK_WCHAR *) oText);
			//			string tmpfont;
			//WStringToString(tmpWS, tmpfont);
			//newText->SetFonts(tmpfont.c_str());
			//newText->SetFont(Platform::WStringToString(tmpWS).c_str());
			//			READER_LOGI("font is %s",tmpfont.c_str());
			//}

			STK_PNT32 innerLoc = tmpText->GetInnerLoc();
			Vector3 inLoc(innerLoc.X, innerLoc.Y, innerLoc.Z);
			newText->SetInnerLoc(inLoc);

			STK_DIR32 x, y;
			tmpText->GetInnerXYAxis(x, y);
			Vector3 xAxis(x.X, x.Y, x.Z);
			Vector3 yAxis(y.X, y.Y, y.Z);
			newText->SetInnerXYAxis(xAxis, yAxis);
			string textString = Platform::WStringToString(tmpText->GetTextContent());
			newText->SetText(textString);
			comText->AddCText(newText);

		}
		return comText;
	}

	void  SVL2AsynReader::GetTriMeshGeoAttribute(int iModelID, IVertexSet* pTriMeshData, int MeshID, void* svlDoc)
	{
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)svlDoc;
		//TODO 合并分支
		//Stk_GeometryAttributePtr geoAttribute = (*m_svl2Doc)->GetMeshFaceAttribute(iModelID,MeshID);//pMeshFace->GetAttribute();

		//if (geoAttribute != NULL && !pTriMeshData->GetGeoAttribute()) {
		//	GeometryAttribute* triMeshDataGeoAttribute =
		//		GetGeometryAttributeFromStk(geoAttribute);
		//	pTriMeshData->SetGeoAttribute(triMeshDataGeoAttribute);
		//}
	}

	GeometryAttribute* SVL2AsynReader::GetGeometryAttributeFromStk(Stk_GeometryAttributePtr stk_geo)
	{
		if (stk_geo == NULL) {
			return NULL;
		}
		GeometryAttribute* geoAttribute = NULL;

		StkGeoAttrTypeEnum geoType = stk_geo->GetGeoAttrType();

		switch (geoType) {
		case GEOATTR_TYPE_UNKNOWN:
			break;
		case GEOATTR_TYPE_PLANEFACE: /*!< 平面							*/
			geoAttribute = GetPlanfaceGeoFromStk(stk_geo);
			break;
		case GEOATTR_TYPE_REVOLUTIONFACE:/*!< 旋转面							*/
			geoAttribute = GetRevolutionfaceGeoFromStk(stk_geo);
			break;
		case GEOATTR_TYPE_CYLINDERFACE:/*!< 圆柱面							*/
			geoAttribute = GetCylinderfaceGeoFromStk(stk_geo);
			break;
		case GEOATTR_TYPE_CONICALFACE:/*!< 圆锥面							*/
			geoAttribute = GetConicalfaceGeoFromStk(stk_geo);
			break;
		case GEOATTR_TYPE_SPHEREFACE:/*!< 球面							*/
			geoAttribute = GetSpherefaceGeoFromStk(stk_geo);
			break;
		case GEOATTR_TYPE_TOROIDALFACE:/*!< 轮胎面							*/
			geoAttribute = GetToroidaifaceFromStk(stk_geo);
			break;
		case GEOATTR_TYPE_LINE: //!< 直线
			geoAttribute = GetLineGeoFromStk(stk_geo);
			break;
		case GEOATTR_TYPE_ELLIPSE: //!< 椭圆弧
			geoAttribute = GetEllipseGeoFromStk(stk_geo);
			break;

		}
		return geoAttribute;
	}

	GeometryAttribute* SVL2AsynReader::GetPlanfaceGeoFromStk(
		Stk_GeometryAttributePtr stk_geo) {
		if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_PLANEFACE) {
			return NULL;
		}

		Stk_PlaneFaceAttributePtr stk_geoPlan = Stk_PlaneFaceAttributePtr::DynamicCast(stk_geo);
		PlaneFaceAttribute* geoAttribute = new PlaneFaceAttribute();
		STK_DIR32 dir = stk_geoPlan->GetNormal();
		geoAttribute->SetNormal(Vector3(dir.X, dir.Y, dir.Z));
		STK_PNT32 pnt = stk_geoPlan->GetOrigin();
		geoAttribute->SetOrigin(Vector3(pnt.X, pnt.Y, pnt.Z));

		return geoAttribute;
	}
	GeometryAttribute* SVL2AsynReader::GetRevolutionfaceGeoFromStk(
		Stk_GeometryAttributePtr stk_geo) {
		if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_REVOLUTIONFACE) {
			return NULL;
		}

		Stk_RevolutionFaceAttributePtr stk_geoRevo = Stk_RevolutionFaceAttributePtr::DynamicCast(stk_geo);
		RevolutionFaceAttribute* geoAttribute = new RevolutionFaceAttribute();
		geoAttribute->SetRadius(stk_geoRevo->GetOriginRadius());
		STK_DIR32 axis = stk_geoRevo->GetRevoAxis();
		geoAttribute->SetRevoAxis(Vector3(axis.X, axis.Y, axis.Z));
		STK_PNT32 axisorig = stk_geoRevo->GetAxisOrigin();
		geoAttribute->SetAxisOrigin(Vector3(axisorig.X, axisorig.Y, axisorig.Z));

		return geoAttribute;
	}

	GeometryAttribute* SVL2AsynReader::GetCylinderfaceGeoFromStk(
		Stk_GeometryAttributePtr stk_geo) {
		if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_CYLINDERFACE) {
			return NULL;
		}
		CylinderFaceAttribute* geoAttribute = new CylinderFaceAttribute();

		Stk_CylinderFaceAttributePtr stk_geoRevo = Stk_CylinderFaceAttributePtr::DynamicCast(stk_geo);

		geoAttribute->SetRadius(stk_geoRevo->GetOriginRadius());
		STK_DIR32 axis = stk_geoRevo->GetRevoAxis();
		geoAttribute->SetRevoAxis(Vector3(axis.X, axis.Y, axis.Z));
		STK_PNT32 axisorig = stk_geoRevo->GetAxisOrigin();
		geoAttribute->SetAxisOrigin(Vector3(axisorig.X, axisorig.Y, axisorig.Z));

		return geoAttribute;
	}

	GeometryAttribute* SVL2AsynReader::GetConicalfaceGeoFromStk(
		Stk_GeometryAttributePtr stk_geo) {
		if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_CONICALFACE) {
			return NULL;
		}
		ConicalFaceAttribute* geoAttribute = new ConicalFaceAttribute();
		Stk_ConeFaceAttributePtr stk_geoRevo = Stk_ConeFaceAttributePtr::DynamicCast(stk_geo);
	
		geoAttribute->SetRadius(stk_geoRevo->GetOriginRadius());
		STK_DIR32 axis = stk_geoRevo->GetRevoAxis();
		geoAttribute->SetRevoAxis(Vector3(axis.X, axis.Y, axis.Z));
		STK_PNT32 axisorig = stk_geoRevo->GetAxisOrigin();
		geoAttribute->SetAxisOrigin(Vector3(axisorig.X, axisorig.Y, axisorig.Z));
		return geoAttribute;
	}

	GeometryAttribute* SVL2AsynReader::GetSpherefaceGeoFromStk(
		Stk_GeometryAttributePtr stk_geo) {
		if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_SPHEREFACE) {
			return NULL;
		}
		SphereFaceAttribute* geoAttribute = new SphereFaceAttribute();
		Stk_SphereFaceAttributePtr stk_geoRevo = Stk_SphereFaceAttributePtr::DynamicCast(stk_geo);
	
		geoAttribute->SetRadius(stk_geoRevo->GetOriginRadius());
		STK_DIR32 axis = stk_geoRevo->GetRevoAxis();
		geoAttribute->SetRevoAxis(Vector3(axis.X, axis.Y, axis.Z));
		STK_PNT32 axisorig = stk_geoRevo->GetAxisOrigin();
		geoAttribute->SetAxisOrigin(Vector3(axisorig.X, axisorig.Y, axisorig.Z));
		return geoAttribute;
	}

	GeometryAttribute* SVL2AsynReader::GetToroidaifaceFromStk(
		Stk_GeometryAttributePtr stk_geo) {
		if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_TOROIDALFACE) {
			return NULL;
		}
		Stk_ToroidalFaceAttributePtr stk_geoToroidail = Stk_ToroidalFaceAttributePtr::DynamicCast(stk_geo);
		ToroidalFaceAttribute* geoAttribute = new ToroidalFaceAttribute();
		geoAttribute->SetMajorRaius(stk_geoToroidail->GetMajorRadius());
		geoAttribute->SetMinorRadius(stk_geoToroidail->GetMinorRadius());

		geoAttribute->SetRadius(stk_geoToroidail->GetOriginRadius());
		STK_DIR32 axis = stk_geoToroidail->GetRevoAxis();
		geoAttribute->SetRevoAxis(Vector3(axis.X, axis.Y, axis.Z));
		STK_PNT32 axisorig = stk_geoToroidail->GetAxisOrigin();
		geoAttribute->SetAxisOrigin(Vector3(axisorig.X, axisorig.Y, axisorig.Z));

		return geoAttribute;
	}

	GeometryAttribute* SVL2AsynReader::GetLineGeoFromStk(
		Stk_GeometryAttributePtr stk_geo) {
		if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_LINE) {
			return NULL;
		}
		Stk_LineAttributePtr stk_geoLine = Stk_LineAttributePtr::DynamicCast(stk_geo);
		LineAttribute* geoAttribute = new LineAttribute();

		STK_PNT32 centerPointPnt = stk_geoLine->GetCenterPoint();
		STK_DIR32 dirDirection = stk_geoLine->GetDirection();
		STK_PNT32 pntStartPnt = stk_geoLine->GetStartPoint();
		STK_PNT32 pntEndPnt = stk_geoLine->GetEndPoint();

		geoAttribute->SetCenterPoint(
			Vector3(centerPointPnt.X, centerPointPnt.Y, centerPointPnt.Z));
		geoAttribute->SetDirection(
			Vector3(dirDirection.X, dirDirection.Y, dirDirection.Z));
		geoAttribute->SetStartPoint(
			Vector3(pntStartPnt.X, pntStartPnt.Y, pntStartPnt.Z));
		geoAttribute->SetEndPoint(Vector3(pntEndPnt.X, pntEndPnt.Y, pntEndPnt.Z));
		geoAttribute->SetLength(stk_geoLine->GetLength());

		return geoAttribute;
	}

	GeometryAttribute* SVL2AsynReader::GetEllipseGeoFromStk(
		Stk_GeometryAttributePtr stk_geo) {
		if (stk_geo->GetGeoAttrType() != GEOATTR_TYPE_ELLIPSE) {
			return NULL;
		}
		Stk_EllipseAttributePtr stk_geoEllipse = Stk_EllipseAttributePtr::DynamicCast(stk_geo);
		EllipseAttribute* geoAttribute = new EllipseAttribute();

		STK_PNT32 pntCenterPoint = stk_geoEllipse->GetCenterPoint(); //!< Ellipse的中心点
		float fMajorRadius = stk_geoEllipse->GetMajorRadius(); //!< Ellipse的长轴半径
		float fMinorRadius = stk_geoEllipse->GetMinorRadius(); //!< Ellipse的短轴半径
		STK_PNT32 pntStartPoint = stk_geoEllipse->GetStartPoint(); //!< 边界线起点
		STK_PNT32 pntEndPoint = stk_geoEllipse->GetEndPoint(); //!< 边界线终点
		STK_DIR32 dirX; //!< X方向向量
		STK_DIR32 dirY; //!< Y方向向量
		STK_DIR32 dirZ; //!< Z方向向量

		stk_geoEllipse->GetXYZDir(dirX, dirY, dirZ);
		geoAttribute->SetCenterPoint(
			Vector3(pntCenterPoint.X, pntCenterPoint.Y, pntCenterPoint.Z));

		geoAttribute->SetMajorRadius(fMajorRadius);
		geoAttribute->SetMinorRadius(fMinorRadius);

		geoAttribute->SetStartPoint(
			Vector3(pntStartPoint.X, pntStartPoint.Y, pntStartPoint.Z));
		geoAttribute->SetEndPoint(
			Vector3(pntEndPoint.X, pntEndPoint.Y, pntEndPoint.Z));
		geoAttribute->SetXYZDir(Vector3(dirX.X, dirX.Y, dirX.Z),
			Vector3(dirY.X, dirY.Y, dirY.Z), Vector3(dirZ.X, dirZ.Y, dirZ.Z));

		return geoAttribute;
	}

	void SVL2AsynReader::SVLXRenderToMaterial(BaseMaterial* material, void* stkTexture)
	{
		HoteamSoft::SVLLib::Stk_TexturePtr pStkTexture = *(Stk_TexturePtr*)stkTexture;
		{
			if (pStkTexture == nullptr)
				return;
			HoteamSoft::SVLLib::Stk_ImagePtr pPic = pStkTexture->GetTextureImage();
			if (pPic == nullptr)
				return;
			if (material->GetMaterialType() == MaterialType_Phong)
			{
				Material* tempMaterial = dynamic_cast<Material*>(material);
				//获取图片数据长度
				STK_UINT32 o_nudataSize;
				STK_CHAR* o_chDataP = NULL;

				wstring strPicPath = pPic->GetPictureSavePath();
				int iIdx = strPicPath.find_first_of(L"\\");
				if (iIdx >= 0)
				{
					strPicPath.replace(iIdx, 1, L"/");
				}

				Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
				(*m_svl2Doc)->GetSVLXFileItem(Platform::WStringToString(strPicPath), o_nudataSize, &o_chDataP, false);
				if (o_nudataSize <= 0)
				{
					LOGE("贴图ID(%u)，图片数据为0", pStkTexture->GetID());
					return;
				}
				if (o_nudataSize > 0 && o_chDataP)
				{
					//将数据赋值给图片对象
					string textName = material->GetName();

					Texture2D* texture2d = (Texture2D*)this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateTexture(textName, Texture::TEXTURE_2D);
					
					Image* image = new Image();
					AddRefMe(image);
					image->SetData((unsigned char*)o_chDataP, o_nudataSize);
					texture2d->SetImage(image);
					ReleaseMe(image);
					tempMaterial->SetDiffuseMap(texture2d);
				}

				float uScale = pStkTexture->GetUScale();
				float vScale = pStkTexture->GetVScale();

				//如果u v 不同时为1
				if (Abs(uScale - 1) < 0.001 && Abs(vScale - 1.0f) < 0.001 || Abs(uScale) < 0.001f || Abs(vScale) < 0.001f)
				{
				}
				else
				{
					//构造变换纹理坐标变换矩阵
					tempMaterial->CreateTexture2DTransform();
					Matrix4* matrix4 = tempMaterial->GetTexture2DTransform();
					if (matrix4)
					{
						matrix4->m_m00 = uScale;
						matrix4->m_m11 = vScale;
					}
				}
			}
		}
	}

	void SVL2AsynReader::GetEdgeGeoAttribute(int iModelID, Curve* curve, int EdgeID, void* svlDoc)
	{
		Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)svlDoc;
		//TODO 合并分支
		/*Stk_GeometryAttributePtr geoAttribute = (*m_svl2Doc)->GetMeshEdgeAttribute(iModelID,EdgeID);

		if (geoAttribute != NULL && !curve->GetGeoAttribute()) {
			GeometryAttribute* triMeshDataGeoAttribute =
				GetGeometryAttributeFromStk(geoAttribute);
			curve->SetGeoAttribute(triMeshDataGeoAttribute);
		}*/
	}


	void SVL2AsynReader::ReadModelAttribute(Model* model)
	{
		if (this->m_view)
		{
			Stk_DocumentPtr& svl2Doc = *(Stk_DocumentPtr*)this->m_svl2Doc;

			svl2Doc->LoadAttribute();

			vector<Model*> allSubModels;
			model->GetAllSubModels(allSubModels);
			allSubModels.push_back(model);

			for (int i = 0; i < allSubModels.size(); i++)
			{
				Model* subModel = allSubModels.at(i);
				map<wstring, wstring>  properties;
				if (this->m_useInstanceProperty && subModel->GetSVLId()- this->m_SVLIDOffset > 0) {
					properties = svl2Doc->GetInstanceAllAttribute(subModel->GetInstatnceID());
				}
				else if (this->m_usePrototypeProperty && subModel->GetProtoTypeId() > 0)
				{
					properties = svl2Doc->GetModelAllAttribute(subModel->GetProtoTypeId());
				}

				if (properties.size() > 0)
				{
					ShapeProperty* shapeProperty = subModel->GetShapeProperty();
					if (!shapeProperty)
					{
						shapeProperty = new ShapeProperty();
						subModel->SetShapeProperty(shapeProperty);
					}
					shapeProperty->AddRef();

					FillShapeProperty(properties, shapeProperty);

					shapeProperty->Release();
				}
			}
			svl2Doc->UnLoadAttribute();
		}
	}


    void SVL2AsynReader::ReadUserData(Model* model)
    {
		if (this->m_view)
		{
			Stk_DocumentPtr& svl2Doc = *(Stk_DocumentPtr*)this->m_svl2Doc;
#ifdef __MOBILE__  
            char* m_dataP;
            unsigned int m_dataSize = 0;
            //动画文件
            (*((Stk_DocumentPtr*)m_svl2Doc))->GetSVLXFileItem(".manual", m_dataSize, &m_dataP);

            if (m_dataP != NULL && m_dataSize > 0) {
                string key = "ServiceManual";
                string value = m_dataP;
                this->m_view->AddUserData(key,value);
            }
            char* h_dataP;
            unsigned int h_dataSize = 0;
            (*((Stk_DocumentPtr*)m_svl2Doc))->GetSVLXFileItem(".hotspot", h_dataSize, &h_dataP);
            if (h_dataP != NULL && h_dataSize > 0) {
                string key = "HotSpot";
                string value = h_dataP;
                this->m_view->AddUserData(key,value);
            }
            char* l_dataP;
            unsigned int l_dataSize = 0;
            (*((Stk_DocumentPtr*)m_svl2Doc))->GetSVLXFileItem(".light", l_dataSize, &l_dataP);
            if (l_dataP != NULL && l_dataSize > 0) {
                string key = "Light";
                string value = l_dataP;
                this->m_view->AddUserData(key,value);
            }
#endif 
		}
    }

	void SVL2AsynReader::GetViewsData(Model* model)
	{
		if (!model)
		{
			return;
		}
		SModelFileInfo* fileInfo = model->GetFileInfo();
		if (fileInfo) {
			Stk_DocumentPtr* m_svl2Doc = (Stk_DocumentPtr*)this->m_svl2Doc;
			int stk_nodeId = fileInfo->GetPartModelId();
			vector<Stk_ViewPtr> stk_ViewList;
			Stk_ModelPtr stkModel = (*m_svl2Doc)->GetModelByID(stk_nodeId);
			if (stkModel != NULL)
			{
				stkModel->GetAllView(stk_ViewList);
			}
			model->SetModelExtInfo(m_view->GetSceneManager()->GetExtendInfoManager());
			int viewCount = 0;
			vector<ModelView*> pOutViewData;
			GetViewInfo(&stk_ViewList, model, &viewCount, pOutViewData);

			for (int i = 0; i < viewCount; i++) {
				model->AddModelView(pOutViewData.at(i));
			}
			//            vector<PMIData*> pOutPMIData;
			//            GetPMIInfo(&stk_PMIList, &pmiCount, pOutPMIData);
			//            if (pOutPMIData.size() > 0)
			//            {
			//                map<int, PMIData*> pmis;
			//                for (int i = 0; i < pOutPMIData.size(); i++) {
			//                    PMIData* pTmpPMI = pOutPMIData.at(i);
			//                    pmis.insert(
			//                                map<int, PMIData*>::value_type(pTmpPMI->GetID(), pTmpPMI));
			//                }
			//                model->SetPMIs(pmis);
			//            }
		}

		vector<Model *> SubModelList = model->GetSubModels();
		for (int i = 0; i < SubModelList.size(); i++)
		{
			Model* subModel = SubModelList.at(i);
			GetViewsData(subModel);
		}
	}

	//获取视图信息
	bool SVL2AsynReader::GetViewInfo(vector<Stk_ViewPtr> *pStkViewList, Model* inModel,
		int *outViewNum, vector<ModelView*>& outViewData) {
		READER_LOGI("Svlreader::GetViewInfo");
		int count;
		int Error = FALSE;
		CHAR strID[32];
		int ViewNum = 0;
		Stk_ViewPtr pViewData = NULL;
		Stk_CameraPtr pCameraData = NULL;
		*outViewNum = 0;

		//获取视图个数
		ViewNum = pStkViewList->size();
		READER_LOGI("View Num %d", ViewNum);

		if (ViewNum <= 0) {
			return true;
		}

		//*outViewData = (ModelView **) malloc(sizeof(ModelView*) * ViewNum);

		for (int i = 0; i < ViewNum; i++) {
			pViewData = NULL;
			pCameraData = NULL;

			pViewData = pStkViewList->at(i);

			if (pViewData == NULL) {
				continue;
			}

			wstring viewName = pViewData->GetName();
			bool viewIsActivated = pViewData->IsActivated();
			

			ModelView * pView = new ModelView(); // Stk_View(pProtoType->GetID());
												 //获取视图基本信息
			int oldId = pViewData->GetID();
            HoteamSoft::SVLLib::StkViewUsageEnum viewType = pViewData->GetUsageType();
            switch (viewType)
            {
                case HoteamSoft::SVLLib::VIEW_USAGE_GENERAL_VIEW:
                case HoteamSoft::SVLLib::VIEW_USAGE_PROE_BASE_VIEW:
                case HoteamSoft::SVLLib::VIEW_USAGE_DEFAULT_VIEW:
                    pView->SetViewType(ModelView::DefaultView);
                    break;
                case HoteamSoft::SVLLib::VIEW_USAGE_SV_USER_VIEW:
                case HoteamSoft::SVLLib::VIEW_USAGE_SV_USER_CLIPVIEW:
                case HoteamSoft::SVLLib::VIEW_USAGE_PROE_USER_VIEW:
                    pView->SetViewType(ModelView::UserView);
                    break;
                default:
                    //            continue;
                    break;
            }
			int viewID = i + 1; //pViewData->GetID();
			READER_LOGI("ViewId:%d new:%d", oldId, viewID);

			pView->SetID(oldId);
			string nameString = Platform::WStringToString(viewName, "auto");
			pView->SetName(nameString.c_str());
			READER_LOGI("ViewName:%s", nameString.c_str());
			//pView->setEnable(viewIsActivated);
			//		pView->setHasClippingPlan(viewHasClipPlan);
			pView->SetSvlUseType(pViewData->GetUsageType());
			
			if (viewIsActivated && viewType!=HoteamSoft::SVLLib::VIEW_USAGE_USER_CLIPPLANE)
			{
				pView->SetIsInitView(true);
			}
			else
			{
				pView->SetIsInitView(false);
			}

														//获取Camera信息
			pCameraData = pViewData->GetCamera();
			if (pCameraData != NULL) {
				CameraNode cameraInfo;
				cameraInfo.SetID(pCameraData->GetID());

				//获取镜头原始位置
				Vector3 pos(pCameraData->GetOriginPoint().X,
					pCameraData->GetOriginPoint().Y,
					pCameraData->GetOriginPoint().Z);

				//获取Target方向向量
				Vector3 target(pCameraData->GetTargetVector().X,
					pCameraData->GetTargetVector().Y,
					pCameraData->GetTargetVector().Z);

				Vector3 up(pCameraData->GetUpVector().X,
					pCameraData->GetUpVector().Y, pCameraData->GetUpVector().Z);

				Quaternion rotation;
				if ((target.m_x != -1.0f || target.m_y != -1.0f || target.m_z != -1.0f) &&
					(up.m_x != -1.0f || up.m_y != -1.0f || up.m_z != -1.0f))
				{
					//构造出旋转,参考李坤的算法,貌似和已有的通过direct和up构造的方法不一致.
					Vector3 x = target.CrossProduct(up);
					Vector3 y = up;
					Vector3 z = -target;
					rotation.FromAxes(x, y, z);
				}
				else
				{
					if (pCameraData->GetMatrixFlag())
					{
						HoteamSoft::SVLLib::STK_MTX32 matrix4 = pCameraData->GetMatrix();
						Matrix3 matrix3(matrix4.PlcMatrix[0][0], matrix4.PlcMatrix[1][0], matrix4.PlcMatrix[2][0],
							matrix4.PlcMatrix[0][1], matrix4.PlcMatrix[1][1], matrix4.PlcMatrix[2][1],
							matrix4.PlcMatrix[0][2], matrix4.PlcMatrix[1][2], matrix4.PlcMatrix[2][2]);
						rotation.FromRotationMatrix(matrix3);
						pos.m_x = matrix4.PlcMatrix[3][0];
						pos.m_y = matrix4.PlcMatrix[3][1];
						pos.m_z = matrix4.PlcMatrix[3][2];
					}
				}
				cameraInfo.SetRotation(rotation);
				cameraInfo.SetPosition(pos);


				cameraInfo.SetFov(pCameraData->GetAngle());//镜头角度
				float fHeight = pCameraData->GetHeight();
				//cameraInfo.focus = pCameraData->GetFocus();//焦点
				//cameraInfo.SetZoom(pCameraData->GetZoomFactor());//缩放因子

				//投影类型
				if (pCameraData->GetProjectType() == 1)
				{
					cameraInfo.SetOrthographic(true);
					cameraInfo.SetOrthoSize(Vector2(fHeight*pCameraData->GetAspectRatio(), fHeight));
				}
				else
				{
					cameraInfo.SetOrthographic(false);
				}
				cameraInfo.SetZoom(1.0f);
				
				cameraInfo.SetAspectRatio(pCameraData->GetAspectRatio());
				
				cameraInfo.SetNearClip(pCameraData->GetNearDistance());
				cameraInfo.SetFarClip(pCameraData->GetFarDistance());

				pView->SetCamera(cameraInfo);
				pView->SetUpDataCamera(true);
                pView->SetUpDataModel(true);
				READER_LOGI(
					"viewId:%d ref cameraId:%d", pView->GetID(), cameraInfo.GetID());
			}

			//获取剖面信息
			const vector<Stk_PlaneEntityPtr>& allPlaneList =
				pViewData->GetAllPlaneEntity();
			for (int j = 0; j < allPlaneList.size(); j++) {

				Stk_ClipPlanePtr pPlaneSurfaceData = Stk_ClipPlanePtr::DynamicCast(allPlaneList[j]);

				Vector3 tmpOrigin(pPlaneSurfaceData->GetOrigin().X,
					pPlaneSurfaceData->GetOrigin().Y,
					pPlaneSurfaceData->GetOrigin().Z); //起始范围

													   //法线修正为负值（负值就对了）
				Vector3 tmpNormal(-pPlaneSurfaceData->GetNormal().X,
					-pPlaneSurfaceData->GetNormal().Y,
					-pPlaneSurfaceData->GetNormal().Z); //法线

				STK_DIR32 UDIR, VDIR;
				pPlaneSurfaceData->GetUVDir(UDIR, VDIR);

				Vector3 tmpUDir(UDIR.X, UDIR.Y, UDIR.Z); //U方向
				Vector3 tmpVDir(VDIR.X, VDIR.Y, VDIR.Z); //V方向
				SectionPlane *plane = new SectionPlane(tmpOrigin, tmpNormal, tmpUDir, tmpVDir);

				inModel->AddSectionPlane(plane);
				pView->AddSectionPlaneId(plane->GetID());

			}

			//实例属性
			vector<wstring>	vecInsAttrPlcPath;
			pViewData->GetAllInsAttrPlcPath(vecInsAttrPlcPath);
			int iMaxPlacement = 0;
			for (int k = 0; k < vecInsAttrPlcPath.size(); k++)
			{
				wstring wstrPlcPath = vecInsAttrPlcPath[k];
				InstanceAttribute curInsAtt;

				string strPlcPath = Platform::WStringToString(wstrPlcPath, "auto");
				if ((int)strPlcPath.find("PATH|0") < 0 )
				{
					if (!strPlcPath.empty())
					{
						strPlcPath = "PATH|0|" + strPlcPath;
					}
					else
					{
						strPlcPath = "PATH|0";
					}
				}
				strPlcPath = PathHelper::SVLPathDecToHex((const string)strPlcPath);
				
				curInsAtt.path = strPlcPath;
				HoteamSoft::SVLLib::STK_RGBA32 newColor;
				if (HoteamSoft::SVLLib::STK_SUCCESS == pViewData->GetInsColorByPlcPath(wstrPlcPath, newColor))
				{

					Color dispColor(newColor.Red, newColor.Green, newColor.Blue, newColor.Alpha);
					curInsAtt.hasColor = true;
					curInsAtt.insColor = dispColor;
				}
				HoteamSoft::SVLLib::StkDisplayStateEnum displayState;
				if (HoteamSoft::SVLLib::STK_SUCCESS == pViewData->GetInsDisplayStateByPlcPath(wstrPlcPath, displayState))
				{
					if (HoteamSoft::SVLLib::STK_DISPLAY == displayState)
					{
						curInsAtt.visible = true;
					}
					else
					{
						curInsAtt.visible = false;
					}
				}
				unsigned int idPlacement = -1;
				HoteamSoft::SVLLib::STK_MTX32 newMatrix;
				pViewData->GetInsPlacementByPlcPath(wstrPlcPath, idPlacement, newMatrix);
				if ((int)idPlacement <= 0)
				{
					//兼容实例属性没有记录配置ID的视图
					idPlacement = --iMaxPlacement;
				}
				curInsAtt.id = idPlacement;
				curInsAtt.placeMatrix.Set((const float*)newMatrix.PlcMatrix);

				pView->AddInstanceAttribute(idPlacement, curInsAtt);
				pView->SetUpDataModel(true);
			}

			map<HoteamSoft::SVLLib::StkEntityTypeEnum, vector<STK_ID>> mapViewPMI = pViewData->GetConnector();
			map<HoteamSoft::SVLLib::StkEntityTypeEnum, vector<STK_ID>>::iterator it = mapViewPMI.begin();
			for (; it != mapViewPMI.end(); it++)
			{
				HoteamSoft::SVLLib::StkEntityTypeEnum EntType = it->first;

				if (EntType == ENTITY_TYPE_PMI)
				{
					//获取PMI关联信息
					vector<STK_ID> vcEntityID = it->second;
					for (vector<STK_ID>::iterator ite = vcEntityID.begin(); ite != vcEntityID.end(); ite++)
					{
						pView->AddPMIId(*ite);
					}
				}
				else if (EntType == ENTITY_TYPE_NOTE)
				{
					//获取批注关联信息
					vector<STK_ID> vcEntityID = it->second;
					for (vector<STK_ID>::iterator ite = vcEntityID.begin(); ite != vcEntityID.end(); ite++)
					{
						pView->AddNoteId(*ite);
					}
				}
				else if (EntType == ENTITY_TYPE_SECTION)
				{
					//获取剖面关联信息
					vector<STK_ID> vcEntityID = it->second;
					for (vector<STK_ID>::iterator ite = vcEntityID.begin(); ite != vcEntityID.end(); ite++)
					{
						pView->AddSectionPlaneId(*ite);
					}
				}
			}

			outViewData.push_back(pView);
			(*outViewNum)++;
		}

		return TRUE;
	}
    void SVL2AsynReader::ParseAnnotation(const string& value){
        if (m_view) {
            m_view->ParseAnnotation(value);
        }
    }
    void SVL2AsynReader::FillModelVisible(Model* model)
    {
        model->SetOrigVisible(model->IsVisible(),false);
        vector<Model*>&subModels = model->GetSubModels();
        for (int i = 0; i < subModels.size(); i++)
        {
            FillModelVisible(subModels.at(i));
        }
    }
    /**
     去除多余字符
     
     @param strStream
     @return
     */
    string SVL2AsynReader::RemoveExcess(string& strStream){
        int index = 0;
        if( !strStream.empty())
        {
            while( (index = strStream.find('\r',index)) != string::npos)
            {
                strStream.erase(index,1);
            }
            index = 0;
            while( (index = strStream.find('\n',index)) != string::npos)
            {
                strStream.erase(index,1);
            }
            index = 0;
            while( (index = strStream.find(' ',index)) != string::npos)
            {
                if (index - 1 > -1) {
                    string s = strStream.substr(index-1,1);
                    if (s != "}") {
                        strStream = strStream.replace(index, 1, "$");
                    }else{
                        strStream.erase(index,1);
                    }
                }
            }
        }
        index = 0;
        while( (index = strStream.find('$',index)) != string::npos)
        {
            strStream.replace(index, 1, " ");
        }
        return strStream;
    }
}
