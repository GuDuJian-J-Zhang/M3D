/*
 * display.cpp
 *
 *  Created on: 2013-5-31
 *      Author: billy
 */
#include "m3d/ResourceManager.h"
#include "m3d/M3D.h"
#include "m3d/model/Model.h"
#include "m3d/model/MeshData.h"

#include "m3d/utils/IDCreator.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/utils/FileHelper.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/graphics/Material.h"
#include "m3d/graphics/BaseMaterial.h"

#include "m3d/SceneManager.h"

#include "Reader.h"
#include "Svlreader.h"
#include "SVL2AsynReader.h"

#include "AssimpReader.h"
#include "m3d/utils/FileHelper.h"
#include "sview/views/Parameters.h"

#include "sview/views/View.h"

using M3D::IDCreator;
using M3D::StringHelper;
using M3D::FileHelper;

namespace SVIEW
{

ReaderListener::ReaderListener() :
		M3D::Object()
{
	
}

ReaderListener::~ReaderListener()
{

}

void ReaderListener::OnProcessPercent(Reader* reader, float percent)
{

}

void ReaderListener::OnBegin(Reader* reader)
{

}

void ReaderListener::OnEnd(Reader* reader)
{

}

string Reader::defaultAnimationData = "";
Reader::Reader(void)
{
	m_File = "";
	m_isCancel = false;
	m_IDCreator = NULL;
//	m_FontPath = "";
	m_readPercent = -1;
	InitializeID();
	this->m_readerListener = NULL;
	this->SetAsynMode(false);
	this->SetView(NULL);

	this->SetUseIndex(Parameters::Instance()->m_IsUseIndexMode);
}

Reader::~Reader(void)
{
	Clear();
}

int Reader::GetNewID()
{
	if (this->m_IDCreator != NULL)
	{
		return this->m_IDCreator->GetID(IDCreator::MODEL);
	}
	return 0;
}

int Reader::InitializeID()
{
	if (this->m_IDCreator != NULL)
	{
		this->m_IDCreator->Initialize(IDCreator::MODEL, -1);
		return this->m_IDCreator->GetID(IDCreator::MODEL);

		//M3D::Object::OBJID =0;
	}
	return 0;
}

string Reader::GetFile()
{
	return this->m_File;
}

void Reader::SetFile(string file)
{
	this->m_File = file;
}

void Reader::SetView(View* view)
{
	this->m_view = view;
}

Model*
Reader::GetModel()
{
	return GetModel(-1);
}

set<string>& Reader::GetSourceFiles()
{
	return m_srcFiles;
}

bool Reader::AddSourceFile(const string& filePath)
{
	bool ret =false;
	if(m_srcFiles.find(filePath)== m_srcFiles.end())
	{
		m_srcFiles.insert(filePath);
		ret = true;
	}
	return ret;
}

Model*
Reader::GetModel(int id)
{
	return NULL;
}

IVertexSet*
Reader::GetMeshData(int id)
{
	return NULL;
}

void Reader::Cancel()
{
	this->m_isCancel = true;
}

bool Reader::IsCancel()
{
	return this->m_isCancel;
}

void Reader::Clear()
{
	if (this->m_readerListener)
	{
		this->m_readerListener->Release();
		this->m_readerListener = NULL;
	}
}

Reader*
Reader::GetReader(const string &file)
{
	Reader* reader = NULL;
	string ext = StringHelper::ToLower(FileHelper::GetExt(file));
	if(ext == "")
	{
		return reader;
}
	 if(!FileHelper::FileExist(file))
	 {
	 LOGE("File not exist %s",file.c_str());
	 }
    string defaultSuffix = Parameters::Instance()->GetParameter(Parameters::ITEM_DEFAULT_SUFFIX);
    if ("svl" == ext || defaultSuffix == ext)
	{
		reader = new SvlReader();
	}
	else if ("svlx" == ext || "SVLX" == ext)
	{
		reader = new SVL2AsynReader();
	}
	else if ("dat" == ext || "DAT" == ext)
	{
		reader = new SVL2AsynReader();
	}
//	else	if ("stl" == ext || "STL" == ext)
//	{
//		reader = new StlReader();
//	}
	else if ("fbx" == ext || "dae" == ext || "gltf" == ext 
		|| "glb" == ext || "blend" == ext || "3ds" == ext 
		|| "ase" == ext || "obj" == ext || "ifc" == ext 
		|| "xgl" == ext || "zgl" == ext || "ply" == ext 
		|| "dxf" == ext || "lwo" == ext || "lws" == ext 
		|| "lxo" == ext || "stl" == ext || "x" == ext 
		|| "ac" == ext || "ms3d" == ext || "cob" == ext 
		|| "scn" == ext || "bvh" == ext || "csm" == ext 
		|| "xml" == ext || "irrmesh" == ext || "irr" == ext 
		|| "mdl" == ext || "md2" == ext || "md3" == ext 
		|| "pk3" == ext || "mdc" == ext || "md5*" == ext 
		|| "smd" == ext || "vta" == ext || "ogex" == ext 
		|| "3d" == ext || "b3d" == ext || "q3d" == ext 
		|| "q3s" == ext || "nff" == ext || "off" == ext 
		|| "raw" == ext || "ter" == ext || "" == ext 
		|| "mdl" == ext || "hmp" == ext || "ndo" == ext)
	{
		reader = new AssimpReader();
	}

	if (reader != NULL)
	{
		reader->SetFile(file);
	}

	return reader;
}

const string& Reader::GetXMLAnimationData()
{
	return defaultAnimationData;
}

void Reader::SetXMLAnimationData(const string& xmlData)
{

}

void Reader::SetPercent(float percent)
{
	this->m_readPercent = percent;
	if(this->m_readerListener)
	{
		this->m_readerListener->OnProcessPercent(this,percent);
	}
}

float Reader::GetPercent()
{
	return this->m_readPercent;
}

void Reader::SetListener(ReaderListener* listener)
{
	if(this->m_readerListener)
	{
		this->m_readerListener->Release();
		this->m_readerListener = NULL;
	}

	this->m_readerListener = listener;
	if(this->m_readerListener)
	{
		this->m_readerListener->AddRef();
	}
}

ReaderListener* Reader::GetListener()
{
	return this->m_readerListener;
}

bool Reader::IsAsynMode()
{
	return m_isAnsyMode;
}

void Reader::SetAsynMode(bool asynMode)
{
	this->m_isAnsyMode = asynMode;
}


bool Reader::IsUseIndex() {
	return this->m_isUseIndex;
}

bool Reader::CanUseIndex(int vertexsCount) {
	return (this->IsUseIndex() && (vertexsCount < INDEX_MAX_COUNT));
}

void Reader::SetUseIndex(bool useIndex) {
	this->m_isUseIndex = useIndex;
}


BaseMaterial* Reader::CovertColorToMaterial(Color& srcColor)
{
	//?ù?YColorμ??μ??DD±è??
	char tempBuffer[CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "ColorBaseMaterial--%3.2f %3.2f %3.2f %3.2f", srcColor.m_r, srcColor.m_g, srcColor.m_b, srcColor.m_a);
	string srcColorCode = string(tempBuffer);

	BaseMaterial* material = this->m_view->GetSceneManager()->GetResourceManager()
		->GetMaterial(srcColorCode);
	if (material)
	{
		return material;
	}
	else
	{
		material = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateMaterial(srcColorCode, MaterialType_Phong);
	}

	Material* tempMaterial = static_cast<Material*>(material);	
	tempMaterial->DisplayName(srcColorCode);

	tempMaterial->SetDiffuse(srcColor);
	tempMaterial->Opacity(srcColor.m_a);

	float shininess = 20.0;
	if (shininess >= 0)
	{
		tempMaterial->SetShininess(shininess);
	}
	return material;
}

}
