// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *	@brief	组件特征类，区别与DTK中有关Node下的特征，该特征目前仅指组、阵列特征、装配特征
 *
 *	@par	历史:
 *		2014/11/11	创建。WangY@HOTEAMSOFT
 *		2016/08/31	更新。WangY@HOTEAMSOFT
 *					API封装
 *		2016/11/22	重构。MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_Document.h"
#include "Stk_DocumentImpl.h"
#include "ChineseCode.h"

using rapidjson::Value;
using rapidjson::Document;
using namespace std;

namespace HoteamSoft {
	namespace SVLLib {

STK_IMPLEMENT_CLASS(Stk_Document, Stk_Object)

 Stk_Document::Stk_Document(void):Stk_Object(new Stk_DocumentImpl())
 {
	 ((Stk_DocumentImpl*)m_pImpl)->m_pViewDoc = Stk_ViewDocumentPtr::CreateObject();
	 ((Stk_DocumentImpl*)m_pImpl)->m_pViewDoc->SetDocument(this);
	 ((Stk_DocumentImpl*)m_pImpl)->m_pPMIDoc = Stk_PMIDocumentPtr::CreateObject();
	 ((Stk_DocumentImpl*)m_pImpl)->m_pPMIDoc->SetDocument(this);
	 ((Stk_DocumentImpl*)m_pImpl)->m_pAttrDoc = Stk_AttributeCollectionPtr::CreateObject();
	 ((Stk_DocumentImpl*)m_pImpl)->m_pAttrDoc->SetDocument(this);
	 ((Stk_DocumentImpl*)m_pImpl)->m_pGeoAttrDoc = Stk_GeometryAttributeCollectionPtr::CreateObject();
	 ((Stk_DocumentImpl*)m_pImpl)->m_pGeoAttrDoc->SetDocument(this);
 }

Stk_Document::~Stk_Document(void)
{
	
}

Stk_NodePtr Stk_Document::AddTopNode(unsigned int uiID/* = -1*/) {
	Stk_NodePtr topNode = Stk_NodePtr::CreateObject();
	if (-1 != uiID) {
		topNode->SetID(uiID);
	}
	else {
		STK_IMPL_CAST(Stk_Document)->AddTopNodeID(topNode->GetID());
		//绑定数据
		topNode->BindingNode(STK_IMPL_CAST(Stk_Document)->GetNode(topNode->GetID()));
	}
	topNode->Init(this, 0, -1 != uiID ? false : true);
	STK_IMPL_CAST(Stk_Document)->m_mapIdToNode.insert(make_pair(topNode->GetID(), topNode));
	return topNode;
}

int Stk_Document::GetNbTopNode(){
	return STK_IMPL_CAST(Stk_Document)->GetNbTopNode();
}

int Stk_Document::GetNbTopInstance()
{
	return STK_IMPL_CAST(Stk_Document)->GetNbTopInstance();
}

vector<HoteamSoft::SVLLib::Stk_InstancePtr>& Stk_Document::getTopInstances()
{
	return STK_IMPL_CAST(Stk_Document)->getTopInstances();
}

//添加顶级实例
Stk_InstancePtr Stk_Document::AddTopInstance(unsigned int uiID /*= -1*/)
{
	map<int, Stk_InstancePtr>::iterator it;
	it = STK_IMPL_CAST(Stk_Document)->m_mapIdToInstance.find(uiID);
	if (it != STK_IMPL_CAST(Stk_Document)->m_mapIdToInstance.end())
	{
		return it->second;
	}
	Stk_InstancePtr topInstance = Stk_InstancePtr::CreateObject();
	if (-1 != uiID) {
		topInstance->SetID(uiID);
	}
	//else {
	//STK_IMPL_CAST(Stk_Document)->AddTopInstanceID(topInstance->GetID());
	//绑定数据
	//topInstance->BindingNode(&STK_IMPL_CAST(Stk_Document)->m_nodeDoc["bom"]["instances"][STK_IMPL_CAST(Stk_Document)->m_nodeDoc["bom"]["instances"].Size()-1]);
	//}
	StkBomBlock tmpBomBlock;
	tmpBomBlock.instanceID = topInstance->GetID();
	tmpBomBlock.materialID = -1;
	tmpBomBlock.modelID = -1;
	tmpBomBlock.parentID = -1;
	tmpBomBlock.plcId = -1;
	tmpBomBlock.visible = -1;
	for (int i = 0; i < 16; i++)
	{
		tmpBomBlock.Matrix[i] = 0;
	}
	tmpBomBlock.Matrix[0] = tmpBomBlock.Matrix[5] = tmpBomBlock.Matrix[10] = 1;
	tmpBomBlock.Matrix[15] = 1;
	topInstance->BindBomBlock(&tmpBomBlock);
	//创建索引
	STK_IMPL_CAST(Stk_Document)->m_mapIdToInstance.insert(make_pair(topInstance->GetID(), topInstance));
	topInstance->Init(this, 0, -1 != uiID ? false : true);
	return topInstance;
}

Stk_InstancePtr Stk_Document::GetTopInstance(int iIdx)
{
	if (iIdx < 0 || iIdx >= GetNbTopInstance()) {
		return nullptr;
	}
	unsigned int InstanceID = STK_IMPL_CAST(Stk_Document)->GetTopInstanceID(iIdx);
	//Stk_InstancePtr topInstance = AddTopInstance(InstanceID);
	map<int, Stk_InstancePtr>::iterator it;
	it = STK_IMPL_CAST(Stk_Document)->m_mapIdToInstance.find(InstanceID);
	if (it != STK_IMPL_CAST(Stk_Document)->m_mapIdToInstance.end())
	{
		return it->second;
	}
	Stk_InstancePtr topInstance = Stk_InstancePtr::CreateObject();
	if (-1 != InstanceID) {
		topInstance->SetID(InstanceID);
	}
	//创建索引
	STK_IMPL_CAST(Stk_Document)->m_mapIdToInstance.insert(make_pair(topInstance->GetID(), topInstance));
	topInstance->Init(this, 0, -1 != InstanceID ? false : true);
	return topInstance;
}

Stk_NodePtr Stk_Document::GetTopNode(int iIdx)
{
	if (iIdx < 0 || iIdx >= GetNbTopNode()) {
		return nullptr;
	}
	unsigned int nodID = STK_IMPL_CAST(Stk_Document)->GetTopNodeID(iIdx);
	Stk_NodePtr topNode = AddTopNode(nodID);
	return topNode;
}

HoteamSoft::SVLLib::Stk_NodePtr Stk_Document::GetNodeByID(unsigned int nodeID)
{
	Stk_NodePtr thisNode = nullptr;
	map<int, Stk_NodePtr>::iterator it;
	it = STK_IMPL_CAST(Stk_Document)->m_mapIdToNode.find(nodeID);
	if (it != STK_IMPL_CAST(Stk_Document)->m_mapIdToNode.end())
	{
		return it->second;
	}
	return thisNode;
}

HoteamSoft::SVLLib::Stk_PartPtr Stk_Document::GetPartByID(unsigned int partID)
{
	Stk_PartPtr thisPart = nullptr;
	map<int, Stk_PartPtr>::iterator it;
	it = STK_IMPL_CAST(Stk_Document)->m_mapIdToStkPart.find(partID);
	if (it != STK_IMPL_CAST(Stk_Document)->m_mapIdToStkPart.end())
	{
		return it->second;
	}
	return thisPart;
}

Stk_MeshPrimPtr Stk_Document::GetMeshPrimByID(unsigned int PrimID)
{
	Stk_MeshPrimPtr thisPrim = nullptr;
	return thisPrim;
}

void Stk_Document::SetOutputFullPath(const std::wstring& fullPath)
{
	STK_IMPL_CAST(Stk_Document)->SetOutputFullPath(fullPath);
}

std::wstring Stk_Document::GetOutputFullPath()
{
	return STK_IMPL_CAST(Stk_Document)->GetOutputFullPath();
}

std::wstring Stk_Document::GetOutputName()
{
	return STK_IMPL_CAST(Stk_Document)->GetOutputName();
}

STK_STATUS Stk_Document::WriteFile()
{
	STK_STATUS	 result = STK_SUCCESS;
	STK_IMPL_CAST(Stk_Document)->WriteFile();
	return result;
}

const std::string& Stk_Document::GetBinNameUTF8(){
	return STK_IMPL_CAST(Stk_Document)->GetBinNameUTF8();
}

Stk_ModelPtr Stk_Document::GetModelByID( int ModelID)
{
	Stk_ModelPtr stkModel = STK_IMPL_CAST(Stk_Document)->GetModelByID(ModelID);
	if (stkModel.isNotNull())
	{
		stkModel->Init(this, 0, false);
	}
	return stkModel;
}

Stk_InstancePtr Stk_Document::GetInstanceByID(int instanceID)
{
	return STK_IMPL_CAST(Stk_Document)->GetInstanceByID(instanceID);
}

bool Stk_Document::IsOpen(){
	return STK_IMPL_CAST(Stk_Document)->m_bOpened;
}

STK_STATUS Stk_Document::OpenLocal(const std::wstring& fileFullPath)
{
	STK_STATUS ststus = STK_IMPL_CAST(Stk_Document)->OpenLocal(fileFullPath);
	printf("完成装配文件解析\r\n");
	if (STK_SUCCESS == ststus) 
	{
		printf("API结构解析完毕\r\n");
	}
	return ststus;
}

STK_STATUS  Stk_Document::OpenSVLX(const std::wstring& fileFullPath)
{
	STK_STATUS iRet = STK_SUCCESS;
	if (fileFullPath.size() == 0)
		return STK_ERROR;
	std::string strFileName;
	strFileName = ChineseCode::WStringToString(fileFullPath);
	//将SVLX数据读取到内存中
	STK_UINT32 mBlockByteSize;
	STK_CHAR* mBlockMemory = NULL;
	Stk_FilePtr filePtr = Stk_FilePtr::CreateObject();
	filePtr->SetName(strFileName);
	filePtr->OpenAsBin("rb");
	mBlockByteSize = filePtr->FileSize();
	mBlockMemory = (STK_CHAR*)malloc(mBlockByteSize);
	filePtr->ReadBuffer(mBlockMemory, mBlockByteSize, 0);
	//打开文件
	OpenFile(mBlockByteSize, mBlockMemory);
	LoadBom();
	LoadModel();
	//LoadAttribute();
	return iRet;
}

STK_STATUS Stk_Document::OpenFile(const unsigned int i_bufferSize, const char* i_bufP)
{
	STK_STATUS status = STK_IMPL_CAST(Stk_Document)->OpenFile(i_bufferSize, i_bufP);
	return status;
}

STK_STATUS Stk_Document::OpenFile(const std::wstring& fileFullPath)
{
	STK_STATUS status = STK_IMPL_CAST(Stk_Document)->OpenFile(fileFullPath);
	return status;
}

STK_STATUS Stk_Document::cacheMeshToTempPath()
{
	STK_STATUS status = STK_IMPL_CAST(Stk_Document)->loadMeshFromBuff();
	return status;
}

HoteamSoft::SVLLib::STK_STATUS Stk_Document::LoadBom(const std::wstring& bomFullPath)
{

	STK_STATUS status = STK_IMPL_CAST(Stk_Document)->LoadBom(bomFullPath);
	printf("完成装配文件解析\r\n");
	if (STK_SUCCESS == status) 
	{
		printf("API结构解析完毕\r\n");
	}
	return status;
}

STK_STATUS	Stk_Document::LoadAttribute(const std::wstring& bomFullPath)
{
	STK_STATUS status = STK_IMPL_CAST(Stk_Document)->LoadAttribute(bomFullPath);
	return status;
}

STK_STATUS Stk_Document::getInstanceNames(map<STK_UINT32, std::wstring>& i_mapInstanceIDtoName)
{
	return STK_IMPL_CAST(Stk_Document)->getInstanceNames(i_mapInstanceIDtoName);
}

STK_STATUS Stk_Document::getAttributesByIds(vector<STK_UINT32> i_vecId, map<STK_UINT32, map<std::wstring, std::wstring>>& i_mapKeyToValue)
{
	return STK_IMPL_CAST(Stk_Document)->getAttributesByIds(i_vecId, i_mapKeyToValue);
}

HoteamSoft::SVLLib::STK_STATUS Stk_Document::loadAttributeStream()
{
	STK_STATUS status = STK_IMPL_CAST(Stk_Document)->loadAttributeStream();
	return status;
}

STK_STATUS Stk_Document::unloadAttributeStream()
{
	STK_STATUS status = STK_IMPL_CAST(Stk_Document)->unloadAttibuteStream();
	return status;
}

STK_STATUS Stk_Document::UnLoadAttribute()
{
	STK_STATUS status = STK_IMPL_CAST(Stk_Document)->unLoadAttribute();
	return status;
}

STK_STATUS  Stk_Document::UnloadGeoAttribute()
{

	STK_STATUS status = STK_IMPL_CAST(Stk_Document)->UnLoadGeoAttribute();
	return status;
}

STK_STATUS Stk_Document::LoadBom()
{
	STK_STATUS status;
	switch (STK_IMPL_CAST(Stk_Document)->m_enumFileOpenMode)
	{
	case STK_FILE_OPENMODE_LOCAL:
		status = STK_IMPL_CAST(Stk_Document)->LoadBom(this);
		break;
	case STK_FILE_OPENMODE_BUFFER:
		status = STK_IMPL_CAST(Stk_Document)->LoadBomFromBuf(this);
		break;
	}
	return status;
}

STK_STATUS Stk_Document::LoadBom(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType)
{
	STK_STATUS status = STK_SUCCESS;
	status = STK_IMPL_CAST(Stk_Document)->LoadBom(this, i_bufSize, i_bufP, i_enumBlockType);
	return status;
}

STK_STATUS Stk_Document::LoadAttribute()
{
	STK_STATUS status;
	switch (STK_IMPL_CAST(Stk_Document)->m_enumFileOpenMode)
	{
	case STK_FILE_OPENMODE_LOCAL:
		status = STK_IMPL_CAST(Stk_Document)->LoadAttribute(this);
		break;
	case STK_FILE_OPENMODE_BUFFER:
		status = STK_IMPL_CAST(Stk_Document)->LoadAttrFromBuf(this);
		break;
	}
	return status;
}

STK_STATUS Stk_Document::LoadGeoAttribute()
{
	STK_STATUS status;
	switch (STK_IMPL_CAST(Stk_Document)->m_enumFileOpenMode)
	{
	case STK_FILE_OPENMODE_LOCAL:
		status = STK_IMPL_CAST(Stk_Document)->LoadGeoAttribute(this);
		break;
	case STK_FILE_OPENMODE_BUFFER:
		status = STK_IMPL_CAST(Stk_Document)->LoadGeoAttrFromBuf(this);
		break;
	}
	return status;
}

STK_STATUS  Stk_Document::LoadPMI()
{
	STK_STATUS status;
	switch (STK_IMPL_CAST(Stk_Document)->m_enumFileOpenMode)
	{
	case STK_FILE_OPENMODE_LOCAL:
		status = STK_IMPL_CAST(Stk_Document)->LoadPMI(this);
		break;
	case STK_FILE_OPENMODE_BUFFER:
		status = STK_IMPL_CAST(Stk_Document)->LoadPMIFromBuf(this);
		break;
	}
	return status;
}

STK_STATUS  Stk_Document::LoadView()
{
	STK_STATUS status;
	switch (STK_IMPL_CAST(Stk_Document)->m_enumFileOpenMode)
	{
	case STK_FILE_OPENMODE_LOCAL:
		status = STK_IMPL_CAST(Stk_Document)->LoadView(this);
		break;
	case STK_FILE_OPENMODE_BUFFER:
		status = STK_IMPL_CAST(Stk_Document)->LoadViewFromBuf(this);
		break;
	}
	return status;
}

STK_STATUS Stk_Document::UnloadBom()
{
	return STK_IMPL_CAST(Stk_Document)->UnloadBom();
}

STK_STATUS Stk_Document::LoadModel()
{
	STK_STATUS status = STK_SUCCESS;
	switch (STK_IMPL_CAST(Stk_Document)->m_enumFileOpenMode)
	{
	case STK_FILE_OPENMODE_LOCAL:
		status = STK_IMPL_CAST(Stk_Document)->LoadModel();
		break;
	case STK_FILE_OPENMODE_BUFFER:
		status = STK_IMPL_CAST(Stk_Document)->loadModelFromBuf();
		break;
	}
	return status;
}

STK_STATUS Stk_Document::LoadModel(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType)
{
	STK_STATUS status = STK_SUCCESS;
	status = STK_IMPL_CAST(Stk_Document)->LoadModel(i_bufSize, i_bufP, i_enumBlockType);
	return status;
}

HoteamSoft::SVLLib::STK_STATUS Stk_Document::UnloadModel()
{
	return STK_IMPL_CAST(Stk_Document)->UnloadModel();
}

STK_STATUS Stk_Document::LoadMaterial()
{
	STK_STATUS status;
	switch (STK_IMPL_CAST(Stk_Document)->m_enumFileOpenMode)
	{
	case STK_FILE_OPENMODE_LOCAL:
		status = STK_IMPL_CAST(Stk_Document)->LoadMaterial();
		break;
	case STK_FILE_OPENMODE_BUFFER:
		status = STK_IMPL_CAST(Stk_Document)->LoadMaterialFromBuf();
		break;
	}
	//对所有材质对象进行初始化操作
	map<int, Stk_MaterialPtr>::iterator it;
	for(  it=STK_IMPL_CAST(Stk_Document)->m_mapIdToStkMaterial.begin(); it!=STK_IMPL_CAST(Stk_Document)->m_mapIdToStkMaterial.end(); it++)
	{
		it->second->Init(this, 0, false);
	}
	return STK_SUCCESS;
}

STK_STATUS Stk_Document::LoadMaterial(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType)
{
	STK_STATUS status = STK_SUCCESS;
	status = STK_IMPL_CAST(Stk_Document)->LoadMaterial(i_bufSize, i_bufP, i_enumBlockType);
	return status;
}

STK_STATUS  Stk_Document::UnloadMaterial()
{
	STK_STATUS status = STK_SUCCESS;
	status = STK_IMPL_CAST(Stk_Document)->UnloadMaterial();
	return status;
}

HoteamSoft::SVLLib::STK_STATUS Stk_Document::LoadFileInfo()
{
	STK_STATUS status;
	switch (STK_IMPL_CAST(Stk_Document)->m_enumFileOpenMode)
	{
	case STK_FILE_OPENMODE_LOCAL:
		status = STK_IMPL_CAST(Stk_Document)->LoadFileInfo();
		break;
	case STK_FILE_OPENMODE_BUFFER:
		status = STK_IMPL_CAST(Stk_Document)->LoadFileFromBuf();
		break;
	}
	return status;
}

STK_STATUS Stk_Document::LoadFileInfo(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType)
{
	STK_STATUS status = STK_SUCCESS;
	status = STK_IMPL_CAST(Stk_Document)->LoadFileInfo(i_bufSize, i_bufP, i_enumBlockType);
	return status;
}

STK_STATUS Stk_Document::LoadLayer()
{
	STK_STATUS status = STK_SUCCESS;
	status = STK_IMPL_CAST(Stk_Document)->LoadLayer(this);
	return status;
}

HoteamSoft::SVLLib::STK_STATUS Stk_Document::LoadFeat()
{
	STK_STATUS status = STK_SUCCESS;
	status = STK_IMPL_CAST(Stk_Document)->LoadFeat(this);
	return status;
}

STK_STATUS Stk_Document::OpenRemote(const std::string& strIP, int port,
	const std::string& userName, const std::string& projectCode, const std::string& fileName)
{
	STK_STATUS ststus = STK_IMPL_CAST(Stk_Document)->OpenRemote(strIP, port, userName, projectCode, fileName);
	return ststus;
}

unsigned int Stk_Document::GetNbInstance()
{
	return  STK_IMPL_CAST(Stk_Document)->m_instanceCount;
}

unsigned int Stk_Document::GetNbModel()
{
	return  STK_IMPL_CAST(Stk_Document)->m_mapIdToStkModel.size(); 
}

Stk_MaterialPtr Stk_Document::AddMaterial(bool& bHasSet, unsigned int uiID/* = -1*/)
{
	bHasSet = false;
	//判断材质对象是否已被添加
	map<int, Stk_MaterialPtr>::iterator it;
	it = STK_IMPL_CAST(Stk_Document)->m_mapIdToStkMaterial.find(uiID);
	if (it != STK_IMPL_CAST(Stk_Document)->m_mapIdToStkMaterial.end())
	{
		bHasSet = true;
		return it->second;
	}
	Stk_MaterialPtr materialPtr = STK_IMPL_CAST(Stk_Document)->AddMaterial(uiID);
	if (materialPtr.isNotNull())
	{
		STK_IMPL_CAST(Stk_Document)->m_mapIdToStkMaterial.insert(make_pair(materialPtr->GetID(), materialPtr));
		materialPtr->Init(this, 0, false);
	}
	return materialPtr;
}

Stk_MaterialPtr Stk_Document::GetMaterialByID(unsigned int i_materialID)
{
	map<int, Stk_MaterialPtr>::iterator it;
	it = STK_IMPL_CAST(Stk_Document)->m_mapIdToStkMaterial.find(i_materialID);
	if (it != STK_IMPL_CAST(Stk_Document)->m_mapIdToStkMaterial.end())
	{
		return it->second;
	}
	return NULL;
}

HoteamSoft::SVLLib::Stk_MaterialPtr Stk_Document::FindMaterial(MaterialData materialData)
{
	return STK_IMPL_CAST(Stk_Document)->FindMaterial(materialData);
}

Stk_ImagePtr Stk_Document::AddImage(bool& bHasSet, const string& strImagePath, unsigned int uiID/* = -1*/)
{
	bHasSet = false;
	
	Stk_ImagePtr imagePtr = STK_IMPL_CAST(Stk_Document)->AddImage(bHasSet, strImagePath, uiID);
	
	return imagePtr;
}

Stk_ImagePtr Stk_Document::GetImageByID(unsigned int i_ImageID)
{
	map<int, Stk_ImagePtr>::iterator it;
	it = STK_IMPL_CAST(Stk_Document)->m_mapIdToStkImage.find(i_ImageID);
	if (it != STK_IMPL_CAST(Stk_Document)->m_mapIdToStkImage.end())
	{
		return it->second;
	}
	return NULL;
}

void Stk_Document::ListAssemblyBuffer(std::vector<std::string>& outBuffers)
{
	
}

void Stk_Document::ListPrimBuffer(std::vector<std::string>& outBuffers)
{
	
}

int Stk_Document::ListTopNodeID(std::vector<unsigned int>& outTopNodeIDs)
{
	return 0;
}

bool Stk_Document::GetAssemblyBuf(unsigned int assID, std::string& outAssemblyBuf)
{
	bool bRet = false;

	return bRet;
}

bool Stk_Document::GetMeshPrimBuf(unsigned int meshID, std::string& outMeshBuf)
{
	bool bRet = false;
	return bRet;
}


bool Stk_Document::GetMeshBufBuf(const std::vector<std::string>& vecAccessorBuf, 
	std::vector<int>& vecVertex, std::vector<int>& vecNormals, std::vector<int>& vecTrians)
{
	return true;
}

bool Stk_Document::GetMeshBufByUrn(const std::string& bufUrn, std::vector<int>& vecVertex, std::vector<int>& vecNormals, std::vector<int>& vecTrians)
{
	return true;
}

int Stk_Document::ListAssembly(std::vector<Stk_NodePtr>& vecAssembly)
{
	return 0;
}

int Stk_Document::ListMeshPrim(std::vector<Stk_MeshPrimPtr>& vecMeshPrim)
{
	return 0;
}

int Stk_Document::ListPrimID(std::vector<int>& vecPrimIDs) {
	return 0;
}

int Stk_Document::ListModel(std::vector<Stk_ModelPtr>& vecModel)
{
	map<int, Stk_ModelPtr>::iterator it = STK_IMPL_CAST(Stk_Document)->m_mapIdToStkModel.begin();
	while(it != STK_IMPL_CAST(Stk_Document)->m_mapIdToStkModel.end())
	{
		vecModel.push_back( it->second);
		it++;
	}
	return STK_SUCCESS;
}

Stk_AttributeCollectionPtr Stk_Document::GetAttributeCollection()
{
	return ((Stk_DocumentImpl*)m_pImpl)->m_pAttrDoc;
}

bool Stk_Document::OpenLocalBuffer(STK_CHAR* fileBuffer, const STK_UINT32 bufferLength)
{
	((Stk_DocumentImpl*)m_pImpl)->m_fileBuffer = fileBuffer;
	((Stk_DocumentImpl*)m_pImpl)->m_bufferLength = bufferLength;

	return true;
}

Stk_FilePtr Stk_Document::GetBomFile()
{
	STK_CHAR* fileBuffer = ((Stk_DocumentImpl*)m_pImpl)->m_fileBuffer;
	STK_UINT32 bufferLength = ((Stk_DocumentImpl*)m_pImpl)->m_bufferLength;

	if (fileBuffer && bufferLength)
	{
		//CExtractSVLFiles::ExtractBufferToSVLFiles(bufferLength,fileBuffer,"test.bom",)
	}

	return NULL;
}

Stk_FilePtr Stk_Document::GetLodFile()
{
	STK_CHAR* fileBuffer = ((Stk_DocumentImpl*)m_pImpl)->m_fileBuffer;
	STK_UINT32 bufferLength = ((Stk_DocumentImpl*)m_pImpl)->m_bufferLength;

	if (fileBuffer && bufferLength)
	{

	}


	return NULL;
}

Stk_FilePtr Stk_Document::GetMaterialFile()
{
	STK_CHAR* fileBuffer = ((Stk_DocumentImpl*)m_pImpl)->m_fileBuffer;
	STK_UINT32 bufferLength = ((Stk_DocumentImpl*)m_pImpl)->m_bufferLength;

	if (fileBuffer && bufferLength)
	{

	}

	return NULL;
}

Stk_FilePtr Stk_Document::GetLoadInfoFile()
{
	STK_CHAR* fileBuffer = ((Stk_DocumentImpl*)m_pImpl)->m_fileBuffer;
	STK_UINT32 bufferLength = ((Stk_DocumentImpl*)m_pImpl)->m_bufferLength;

	if (fileBuffer && bufferLength)
	{

	}

	return NULL;
}

Stk_FilePtr Stk_Document::GetModelAttributeFile()
{
	STK_CHAR* fileBuffer = ((Stk_DocumentImpl*)m_pImpl)->m_fileBuffer;
	STK_UINT32 bufferLength = ((Stk_DocumentImpl*)m_pImpl)->m_bufferLength;

	if (fileBuffer && bufferLength)
	{

	}


	return NULL;
}

Stk_FilePtr Stk_Document::GetFile(const string& fileName)
{
	STK_CHAR* fileBuffer = ((Stk_DocumentImpl*)m_pImpl)->m_fileBuffer;
	STK_UINT32 bufferLength = ((Stk_DocumentImpl*)m_pImpl)->m_bufferLength;

	if (fileBuffer && bufferLength)
	{

	}

	return NULL;
}

HoteamSoft::SVLLib::STK_STATUS Stk_Document::AddSVLXFileItem(const std::string& i_strFileName)
{
	return ((Stk_DocumentImpl*)m_pImpl)->AddSVLXFileItem( i_strFileName);
}
STK_STATUS Stk_Document::ClearSVLXFileItem()
{
	return ((Stk_DocumentImpl*)m_pImpl)->ClearSVLXFileItem();
}

HoteamSoft::SVLLib::STK_STATUS Stk_Document::GetSVLXFileItem(const std::string& i_strFileName, unsigned int& o_bufSize, char** o_bufP, bool bByExt)
{
	return ((Stk_DocumentImpl*)m_pImpl)->GetSVLXFileItem( i_strFileName,  o_bufSize,  o_bufP, bByExt);
}

HoteamSoft::SVLLib::STK_STATUS Stk_Document::GetSVLXFileItemToFile(const std::string& i_strFileName, const std::string& i_strOutFilePath, bool bByEx)
{
	return ((Stk_DocumentImpl*)m_pImpl)->GetSVLXFileItemToFile( i_strFileName, i_strOutFilePath, bByEx);
}
STK_STATUS Stk_Document::GetSVLXFolderItemToFolder(const std::string& i_strFileName, const std::string& i_strOutFilePath)
{
	return ((Stk_DocumentImpl*)m_pImpl)->GetSVLXFolderItemToFolder(i_strFileName, i_strOutFilePath);
}

Stk_GeometryAttributePtr  Stk_Document::GetMeshFaceAttribute(int iModelID, int iMeshFaceID)
{
	return ((Stk_DocumentImpl*)m_pImpl)->GetMeshFaceAttribute(iModelID, iMeshFaceID);
}

map<wstring, wstring> Stk_Document::GetMeshFaceAllAttribute(int iModelID, int iMeshFaceID)
{
	return ((Stk_DocumentImpl*)m_pImpl)->GetMeshFaceAllAttribute(iModelID, iMeshFaceID);
}

Stk_GeometryAttributePtr  Stk_Document::GetMeshEdgeAttribute(int iModelID, int iMeshEdgeID)
{
	return ((Stk_DocumentImpl*)m_pImpl)->GetMeshEdgeAttribute(iModelID, iMeshEdgeID);
}

map<wstring, wstring> Stk_Document::GetMeshEdgeAllAttribute(int iModelID, int iMeshEdgeID)
{
	return ((Stk_DocumentImpl*)m_pImpl)->GetMeshEdgeAllAttribute(iModelID, iMeshEdgeID);
}

wstring Stk_Document::GetMeshFaceAttribute(int iModelID, int iMeshFaceID, const std::wstring& i_wstrAttrName)
{
	return ((Stk_DocumentImpl*)m_pImpl)->GetMeshFaceAttribute(iModelID, iMeshFaceID, i_wstrAttrName);
}

STK_STATUS Stk_Document::AddMeshFaceAttribute(int iModelID, int iMeshFaceID, const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue)
{
	return ((Stk_DocumentImpl*)m_pImpl)->AddMeshFaceAttribute(iModelID, iMeshFaceID, i_wstrAttrName, i_wstrAttrValue);
}

STK_STATUS  Stk_Document::AddMeshFaceAttribute(int iModelID, int iMeshFaceID, Stk_GeometryAttributePtr pGeoAttr)
{
	return ((Stk_DocumentImpl*)m_pImpl)->AddMeshFaceAttribute(iModelID, iMeshFaceID, pGeoAttr);
}

wstring Stk_Document::GetMeshEdgeAttribute(int iModelID, int iMeshEdgeID, const std::wstring& i_wstrAttrName)
{
	return ((Stk_DocumentImpl*)m_pImpl)->GetMeshEdgeAttribute(iModelID, iMeshEdgeID, i_wstrAttrName);
}

STK_STATUS Stk_Document::AddMeshEdgeAttribute(int iModelID, int iMeshEdgeID, const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue)
{
	return ((Stk_DocumentImpl*)m_pImpl)->AddMeshEdgeAttribute(iModelID, iMeshEdgeID, i_wstrAttrName, i_wstrAttrValue);
}

STK_STATUS Stk_Document::AddMeshEdgeAttribute(int iModelID, int iMeshFaceID, Stk_GeometryAttributePtr pGeoAttr)
{
	return ((Stk_DocumentImpl*)m_pImpl)->AddMeshEdgeAttribute(iModelID, iMeshFaceID, pGeoAttr);
}
}//svllib
}//hoteamsoft
