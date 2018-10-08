/*
 * AssimpReader.cpp
 *
 *  Created on: 2015-6-18
 *      Author: bing
 */

#include "AssimpReader.h"

#include "m3d/ResourceManager.h"

#include "m3d/graphics/Material.h"
#include "m3d/graphics/Texture.h"

#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix4.h"
#include "m3d/utils/FileHelper.h"
#include "m3d/utils/LoggerHelper.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/utils/StringHelper.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "m3d/SceneManager.h"
#include "sview/views/View.h"

#include <fstream>
#include <sstream>   // Post processing flags

using M3D::Body;
using M3D::Face;
using M3D::Vector3;
using M3D::Matrix4;
using M3D::FileHelper;
using M3D::LoggerHelper;
using M3D::StringHelper;
using M3D::ResourceManager;
using M3D::Material;
using M3D::Texture;
using M3D::Color;
using M3D::Texture2D;
using Assimp::Importer;

namespace SVIEW {

string AssimpReader::GetObjFileMtlPath(const string filePath)
{
	string ret = "";
	if (FileHelper::FileExist(filePath))
	{
		string objFilePath = filePath;
		objFilePath = FileHelper::GetUnionStylePath(objFilePath);
		string basePath = FileHelper::GetDirName(objFilePath) + "/";
		std::stringstream err;
		std::ifstream ifs(objFilePath);

		if (!ifs)
		{
			LOGE("ifs error");
			err << "Cannot open file [" << filePath << "]" << std::endl;
			return err.str();
		}

		int maxchars = 8192; // Alloc enough size.
		std::vector<char> buf(maxchars); // Alloc enough size.
		int lineNum = 0;

		while (ifs.peek() != -1 && lineNum < 1000)
		{
			lineNum++;
			ifs.getline(&buf[0], maxchars);

			std::string linebuf(&buf[0]);

			// Trim newline '\r\n' or '\n'
			if (linebuf.size() > 0)
			{
				if (linebuf[linebuf.size() - 1] == '\n')
					linebuf.erase(linebuf.size() - 1);
			}
			if (linebuf.size() > 0)
			{
				if (linebuf[linebuf.size() - 1] == '\r')
					linebuf.erase(linebuf.size() - 1);
			}

			// Skip if empty line.
			if (linebuf.empty())
			{
				continue;
			}

			// Skip leading space.
			const char *token = linebuf.c_str();
			token += strspn(token, " \t");

			assert(token);
			if (token[0] == '\0')
				continue; // empty line

			if (token[0] == '#')
				continue; // comment line

			// load mtl
			if ((0 == strncmp(token, "mtllib", 6))
					&& StringHelper::IsSpace((token[6])))
			{
				char namebuf[4096];
				token += 7;
				sscanf(token, "%s", namebuf);

				ret = basePath + namebuf;
				break;
			}
		}
	}
	return ret;
}

AssimpReader::AssimpReader() :
		Reader() {
	m_M3DModel = NULL;
}

AssimpReader::~AssimpReader() {
	if(m_M3DModel)
		m_M3DModel->Release();
	m_M3DModel = NULL;
}

Model* AssimpReader::GetModel(int id) {
	if (m_M3DModel == NULL)
	{
		m_M3DModel = ReadFile(this->GetFile().c_str());
		if(m_M3DModel)
		m_M3DModel->AddRef();
	}

	return m_M3DModel;
}

Model* AssimpReader::ReadFile(const char* thePath) {
	this->SetPercent(0.0f/100);
	GetMtlPath(thePath);
	this->SetPercent(5.0f/100);
	READER_LOGI("AssimpReader::ReadFile :%s begin",thePath);
//	m_ModelMap.clear();
	Model* topModel = NULL;

	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT,65535);

	const aiScene* scene = importer.ReadFile(thePath,
			aiProcess_CalcTangentSpace
			| aiProcess_Triangulate
			| aiProcess_JoinIdenticalVertices
			| aiProcess_SortByPType
			| aiProcess_SplitLargeMeshes
			| aiProcess_GenSmoothNormals
			| aiProcess_FlipUVs //需要翻转一下，否则纹理坐标不正确。
			);
	this->SetPercent(45.0f/100);
	if (!scene) {
		READER_LOGE("read use assimpReader error!");
		return NULL;
	}

	if(scene->HasAnimations())
	{
		READER_LOGI("scene has Animations!");
	}
	if(scene->HasCameras())
	{
		READER_LOGI("scene has Cameras!");
	}
	if(scene->HasLights())
	{
		READER_LOGI("scene has Lights!");
	}
	if(scene->HasMaterials())
	{
		READER_LOGI("scene has Materials!");
	}
	if(scene->HasMeshes())
	{
		READER_LOGI("scene has Meshes!");
	}

	if(scene->HasTextures())
	{
		READER_LOGI("scene has Textures!");
	}

	string filePath = FileHelper::GetUnionStylePath(thePath);

	//构造顶级model
	if(scene->mRootNode!=NULL)
	{
		//加载所有的材质资源
		this->LoadMaterials(scene,FileHelper::GetDirName(filePath),FileHelper::GetFileName(filePath),"");
		this->SetPercent(60.0f/100);
		topModel = CreateModelByAINode(scene,scene->mRootNode,"",0);
		this->SetPercent(90.0f/100);
		topModel->UpdataInfo();
		this->SetPercent(95.0f/100);
	}

	return topModel;
}

void AssimpReader::GetMtlPath(const string& filePath)
{
	this->AddSourceFile(filePath);

	//OBJ 文件
	string ext = StringHelper::ToLower(FileHelper::GetExt(filePath));
	string  mtlPath;
	if(ext == "obj")
	{
		mtlPath = GetObjFileMtlPath(filePath);
	}

	if(mtlPath.length() > 0)
	{
		this->AddSourceFile(mtlPath);
	}
}

Model* AssimpReader::CreateModelByAINode(const aiScene* scene,aiNode* curAINode,const string& strParentPlacePath,int sortId)
{
	ResourceManager* resourceMgr = this->m_view->GetSceneManager()->GetResourceManager();

	READER_LOGI("CreateModelByAINode:%s ", curAINode->mName.C_Str());

	Model* curModel = new Model();
	curModel->SetName(curAINode->mName.C_Str());

	curModel->SetPlcId(sortId); //默认设置为0，在外部重新赋值为循环子
	Matrix4 curMtx(curAINode->mTransformation[0]);
	curModel->SetPlaceMatrix(curMtx);

	//获取配置ID路径
	char tmpCurPlcIDStr[200];
	memset(tmpCurPlcIDStr, '\0', sizeof(tmpCurPlcIDStr));
	sprintf(tmpCurPlcIDStr, "%x", sortId); //配置路径按照16进制解析
	string curPlacePath;
	if (strParentPlacePath.length()>0) {
		curPlacePath = strParentPlacePath + "|" + tmpCurPlcIDStr;
	} else {//如果没有设置父级path，则为根节点
		curPlacePath = tmpCurPlcIDStr;
	}
	curModel->SetPlcPath(curPlacePath);

	//mesh
	READER_LOGI("mesh count:%d ", curAINode->mNumMeshes);
	for (int imesh = 0; imesh < curAINode->mNumMeshes; imesh++)
	{
		READER_LOGI("mesh count: 1111");
		aiMesh* curAIMesh = scene->mMeshes[curAINode->mMeshes[imesh]];

		Body* body = new Body();
		VertexSet* bodyDrawData = new VertexSet();
		body->AddData(bodyDrawData);
		curModel->AddBody(body);

		bodyDrawData->SetUseIndex(true);

		READER_LOGI("mesh count:%d 222");
		//color
		M3D::Color bodyColor;
		aiMaterial* aiMaterial = scene->mMaterials[curAIMesh->mMaterialIndex];
		if (aiMaterial != NULL)
		{
			aiColor3D meshColor(0.0f, 0.0f, 0.0f);
			aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, meshColor);
			bodyColor = M3D::Color(meshColor.r, meshColor.g, meshColor.b);
			READER_LOGI(
					"body color:%f %f %f", meshColor.r, meshColor.g, meshColor.b);
		}

		std::vector<Vector3>* bodyPnts = bodyDrawData->GetPositionArray();
		std::vector<Vector3>* bodyNormals = bodyDrawData->GetNormalArray();
		std::vector<M3D_INDEX_TYPE>* indexArray = bodyDrawData->GetIndexArray();
		std::vector<Vector3>* textureCoords =bodyDrawData->GetTextureCoordArray();

		bool hasNormals = curAIMesh->HasNormals();

		bool hasTextureCoords = curAIMesh->HasTextureCoords(0);

		int numVertices = curAIMesh->mNumVertices;

		if (hasNormals)
		{
			bodyNormals->reserve(numVertices);
		}

		if (hasTextureCoords)
		{
			textureCoords->reserve(numVertices);
		}

		//vertices
		READER_LOGI("vertices count:%d ", curAIMesh->mNumVertices);
		bodyPnts->reserve(curAIMesh->mNumVertices);

		for (int iVertx = 0; iVertx < numVertices; iVertx++)
		{
			aiVector3D* tmpAIV = &(curAIMesh->mVertices[iVertx]);
			Vector3 tmpVertex(tmpAIV->x, tmpAIV->y, tmpAIV->z);
			bodyPnts->push_back(tmpVertex);

			//Normals
			if (hasNormals)
			{
				aiVector3D* tmpAIN = &(curAIMesh->mNormals[iVertx]);
				Vector3 tmpVertex(tmpAIN->x, tmpAIN->y, tmpAIN->z);
				bodyNormals->push_back(tmpVertex);
			}

			//TextureCoords
			if (hasTextureCoords)
			{
				aiVector3D* tmpTexCoord =
						&(curAIMesh->mTextureCoords[0][iVertx]);
				Vector3 tmpVertex(tmpTexCoord->x, tmpTexCoord->y,
						tmpTexCoord->z);
				textureCoords->push_back(tmpVertex);
			}
		}

		//构造一个合并面显示
		Face* pFace = new Face();

		Mesh* facemeshData = new Mesh(bodyDrawData);
		facemeshData->SetDataOffset(0);

		//indices
		READER_LOGI("faces count:%d ", curAIMesh->mNumFaces);
		for (int iFace = 0; iFace < curAIMesh->mNumFaces; iFace++)
		{
			aiFace &tmpAIFace = curAIMesh->mFaces[iFace];

			for (int ii = 0; ii < tmpAIFace.mNumIndices; ii++)
			{
				unsigned int tmpIndex = tmpAIFace.mIndices[ii];

				indexArray->push_back((M3D_INDEX_TYPE) tmpIndex);
			}
		}

		facemeshData->SetDataLength(bodyDrawData->GetIndexArray()->size());

		pFace->AddData(facemeshData, 0);

		pFace->SetInitColor(this->m_MaterialList[curAIMesh->mMaterialIndex]->GetDiffuse());
		LOGE("face material index %d",curAIMesh->mMaterialIndex);
		pFace->SetMaterial(this->m_MaterialList[curAIMesh->mMaterialIndex]);
		body->AddFace(pFace);
	}

	//children
	for (int iChild = 0; iChild < curAINode->mNumChildren; iChild++)
	{
		aiNode* childNode = curAINode->mChildren[iChild];
		Model* childModel = CreateModelByAINode(scene, childNode,curPlacePath,iChild);
		curModel->AddSubModel(childModel);
	}

	return curModel;
}

void AssimpReader::LoadMaterials(const aiScene* scene,const string& fileDir, const string& file_name,const string& preFix)
{
	//store material
	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		aiColor3D dif(0.f, 0.f, 0.f);
		aiColor3D amb(0.f, 0.f, 0.f);
		aiColor3D spec(0.f, 0.f, 0.f);
		float opacity = 1.0f;
		float shininess = 1.0f;
		aiMaterial * the_material = scene->mMaterials[i];

		char file_postfix[100];
		sprintf(file_postfix, "%s_%d", file_name.c_str(), i + 1);

		ResourceManager* resourceMgr = this->m_view->GetSceneManager()->GetResourceManager();

		//从资源管理器中创建材质
		Material * material = resourceMgr->GetOrCreateMaterial(file_postfix);

		the_material->Get(AI_MATKEY_COLOR_AMBIENT, amb);
		the_material->Get(AI_MATKEY_COLOR_DIFFUSE, dif);
		the_material->Get(AI_MATKEY_COLOR_SPECULAR, spec);
		the_material->Get(AI_MATKEY_OPACITY,opacity);
		the_material->Get(AI_MATKEY_SHININESS,shininess);
//LOGI("opacity========%f",opacity);
		float alpha = opacity;
		Color ambient(amb.r, amb.g, amb.b,alpha);
		Color diffuse(dif.r, dif.g, dif.b,alpha);
		Color specular(spec.r, spec.g, spec.b,alpha);

		//填充材质
		material->SetAmbient(ambient);
		material->SetDiffuse(diffuse);
		material->setSpecular(specular);
		material->SetShininess(shininess);

		aiString normalPath;
		the_material->GetTexture(aiTextureType_NORMALS, 0, &normalPath);

		aiString diffuse_Path;
		the_material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuse_Path);

		aiString ambient_Path;
		the_material->GetTexture(aiTextureType_AMBIENT, 0, &ambient_Path);

        string rel_diffuse_texturePath = diffuse_Path.C_Str();
		string diffuseTexturePath = FileHelper::GetUnionStylePath(fileDir+ rel_diffuse_texturePath);

		if (!FileHelper::FileExist(diffuseTexturePath) || rel_diffuse_texturePath.length() == 0)
		{
			LOGE("texture Path %s not exist",diffuseTexturePath.c_str());
		}
		else
		{
			char str[100];
			Texture* tempTexture = resourceMgr->GetOrCreateTexture(diffuseTexturePath,Texture::TEXTURE_2D);
			if(tempTexture && tempTexture->GetType() == Texture::TEXTURE_2D)
			{
				Texture2D* texture2d = (Texture2D*)tempTexture;
				texture2d->SetImagePath(diffuseTexturePath);
				material->SetTexture(tempTexture);
			}

			//将文件路径加入到资源管理中
			this->AddSourceFile(diffuseTexturePath);
		}

		// for ambient
		string rel_ambient_texturePath = ambient_Path.C_Str();
		string ambientTexturePath = FileHelper::GetUnionStylePath(
				fileDir + rel_ambient_texturePath);

		if (!FileHelper::FileExist(ambientTexturePath)
				|| rel_ambient_texturePath.length() == 0)
		{
			LOGE("texture Path %s not exist", ambientTexturePath.c_str());
		}
		else
		{
			char str[100];
			Texture* tempTexture = resourceMgr->GetOrCreateTexture(
					ambientTexturePath, Texture::TEXTURE_2D);
			if (tempTexture && tempTexture->GetType() == Texture::TEXTURE_2D)
			{
				Texture2D* texture2d = (Texture2D*) tempTexture;
				texture2d->SetImagePath(ambientTexturePath);
				material->SetAmbientTexture(tempTexture);
			}

			//将文件路径加入到资源管理中
			this->AddSourceFile(ambientTexturePath);
		}


		this->m_MaterialList.push_back(material);
	}
}

}
