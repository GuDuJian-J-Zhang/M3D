// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
 *	@file		Stk_ComponentFeatureImpl.cpp
 *  @brief		组件特征类的实现类
 *	@par 历史记录
 *				2016/11/7 创建。MengYaohui(Hoteamsoft)
**************************************************************************/
#include <memory>
#include <fstream>
#include "Stk_DocumentImpl.h"
#include "Stk_Document.h"
#include "ChineseCode.h"
#include "Stk_ResourceManager.h"
#include "Stk_MeshPrim.h"
#include "Stk_MeshAccessor.h"
#include "Stk_Material.h"
//#include "RVMStruct.pb.h"
//#include <google\protobuf\io\zero_copy_stream_impl.h>
#include "Stk_API.h"
#include "JsonMacro.h"
#include "version.h"
#include "Stk_BomBlock.h"
#include "Stk_ModelBlock.h"
#include "RapidJsonTools.h"
#include "CompressSVLFiles.h"
#include "GetCompressContents.h"
#include "ExtractSVLFiles.h"
#include "Alloc.h"
#include "7zFile.h"
#include <math.h>
#include <stdlib.h>
#include "Stk_LoadInf.h"
#include "StringTools.h"

#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
//using namespace google::protobuf;
#include <algorithm>
using namespace std;

namespace HoteamSoft {
	namespace SVLLib {


#define		F_TOL	0.0001
	
STK_CLASS_IMPL_IMPLEMENT(Stk_Document)

Stk_DocumentImpl::Stk_DocumentImpl(void):
	m_bRemote(false), 
	m_bOpened(false),
	m_pRemoteClient(NULL)
{
	m_nodeDoc.SetObject();
	m_modelDoc.SetObject();
	m_materialDoc.SetObject();
	m_fileInfoDoc.SetObject();
	m_pViewDoc = NULL;
	m_pPMIDoc = NULL;
	m_pAttrDoc = NULL;
	m_pGeoAttrDoc = NULL;

	m_fileBuffer = NULL;
	m_bufferLength = 0;
	m_instanceCount= 0;

	m_enumFileOpenMode = STK_FILE_OPENMODE_LOCAL;
	m_svlxBufferSize = 0;
	m_bufP = nullptr;

	m_meshFilePtr = Stk_FilePtr::CreateObject();
	m_hasExtractMeshData = false;

	m_meshCount = m_meshFaceCount = m_meshEdgeCount = m_triangleCount = 0;
	m_meshDataSize = 0;
	m_isNeedDelSvlxBuf = false;
	m_strAttributeBuf = "";
}

Stk_DocumentImpl::~Stk_DocumentImpl(void)
{
	if (m_pRemoteClient) {
		m_pRemoteClient->Deconnect();
		delete m_pRemoteClient;
	}
	m_vecMeshes.clear();
	m_mapIdToStkModel.clear();
	m_mapIdToInstance.clear();
	m_mapIdToStkMaterial.clear();
	m_vecFileName.clear();
	if (m_meshFilePtr.isNotNull())
	{
		m_meshFilePtr->Close();
		m_meshFilePtr->deleteFile();
	}
	m_meshFilePtr = NULL;
	m_svlxFilePtr = NULL;
	if (m_isNeedDelSvlxBuf && m_bufP)
	{
		free(m_bufP);
		m_bufP = NULL;
	}
}

STK_STATUS Stk_DocumentImpl::WriteFile()
{
	STK_STATUS result = STK_SUCCESS;
	if (m_outputFullPath.empty()) {
		return STK_ERROR_FILEWR;
	}
	//m_vecFileName.clear();
	//保存几何信息
	//WriteGeoFile();
	//保存材质信息
	WriteMaterialFile();
	//保存Mesh信息
	WriteMeshFile();
	//保存视图
	WriteViewFile();
	//保存PMI
	WritePMIFile();
	//保存Bom信息
	//WriteBomFile();
	WriteBomBinFile();
	//保存Model信息
	//WriteModelFile();
	WriteModelBinFile();
	//保存属性文件
	WriteAttrFile();
	//保存几何属性文件
	WriteGeoAttrFile();
	//保存几何属性文件
	//保存点线文件
	WriteLineFile();
	//保存文件信息
	WriteInfoFile();
	//保存图层信息
	WriteLayerFile();
	//保存几何特征信息
	WriteFeatFile();
	//保存svlx文件
	WriteSVLXFile();
	return result;
}

STK_STATUS Stk_DocumentImpl::WriteInfoFile()
{
	//保存统计信息
	Stk_LoadInf::GetInstance()->SetVersion(FileVersion);
	Stk_LoadInf::GetInstance()->SetInstanceCount(m_mapIdToInstance.size());
	Stk_LoadInf::GetInstance()->SetModelCount(m_mapIdToStkModel.size());
	Stk_LoadInf::GetInstance()->setMeshCount(m_meshCount);
	Stk_LoadInf::GetInstance()->SetFaceCount(m_meshFaceCount);
	Stk_LoadInf::GetInstance()->setMeshEdgeCount(m_meshEdgeCount);
	Stk_LoadInf::GetInstance()->SetTriangleCount(m_triangleCount);
	Stk_LoadInf::GetInstance()->setMeshDataSize(m_meshDataSize);
	//保存文件
	wstring outputFile = m_outputFullPath + m_outputName + L".info";
	CRapidJsonTools::WriteLoadInf(outputFile, Stk_LoadInf::GetInstance());
	string strInfoFile = m_strOutputName + ".info";
	m_vecFileName.push_back(strInfoFile);
	return STK_SUCCESS;
}


HoteamSoft::SVLLib::STK_STATUS Stk_DocumentImpl::WriteBomFile()
{
	STK_STATUS result = STK_SUCCESS;
	wstring outputFile = m_outputFullPath + m_outputName + L".bom";


	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	m_nodeDoc.Accept(writer);
	string strJson(buffer.GetString(), buffer.GetSize());
	std::ofstream ofile;
#ifdef __MOBILE__
	string strPath;
	ChineseCode::UnicodeToUTF_8(outputFile.c_str(), strPath);
	ofile.open(strPath.c_str());
#else
	ofile.open(outputFile.c_str());
#endif
	if (ofile.fail())
	{
		return STK_ERROR_FILEWR;
	}
	ofile << strJson;
	ofile.close();
	return result;
}

STK_STATUS Stk_DocumentImpl::WriteBomBinFile()
{
	STK_STATUS result = STK_SUCCESS;
	Stk_ResourceManagerPtr resourceManagerPtr =  Stk_ResourceManagerPtr::CreateObject();
	wstring outputFile = m_outputFullPath + m_outputName + L".bom";
	string GeoFileName;
	//ChineseCode::UnicodeToUTF_8(outputFile.c_str(), GeoFileName);
	GeoFileName = ChineseCode::WStringToString(outputFile);
	Stk_FilePtr stk_filePtr = resourceManagerPtr->GetFile(GeoFileName);
	stk_filePtr->OpenAsBin("wb");
	//遍历所有的Model信息
	map<int, Stk_InstancePtr>::iterator it;
	Stk_BomBlock* pStkBomBlock = new Stk_BomBlock();
	for(  it=m_mapIdToInstance.begin(); it!=m_mapIdToInstance.end(); it++)
	{
		StkBomBlock stkBomBlock;
		Stk_InstancePtr instancePtr = it->second;
		stkBomBlock.instanceID = it->first;
		stkBomBlock.materialID = instancePtr->GetMaterialID();
		stkBomBlock.modelID = instancePtr->GetModelID();
		stkBomBlock.parentID = instancePtr->GetParentID();
		stkBomBlock.plcId = instancePtr->GetPlcID();
		stkBomBlock.visible = instancePtr->GetVisible();
		for (int i = 0; i < 16; i++)
		{
			stkBomBlock.Matrix[i] = 0;
		}
		stkBomBlock.Matrix[0] = stkBomBlock.Matrix[5] = stkBomBlock.Matrix[10] = 1.0;
		stkBomBlock.Matrix[15] = 1;
		instancePtr->GetMatrix(stkBomBlock.Matrix, 16);
		pStkBomBlock->AddInstance(stkBomBlock);
	}
	//创建访问器对象
	Stk_CommonAccessorPtr stkMeshAccessor = Stk_CommonAccessorPtr::CreateObject();
	pStkBomBlock->WrieteFile(stkMeshAccessor, stk_filePtr);
	stk_filePtr->Close();
	delete pStkBomBlock;
	resourceManagerPtr->Clear();
	string strTemp = m_strOutputName + ".bom";
	m_vecFileName.push_back(strTemp);
	return result;
}

HoteamSoft::SVLLib::STK_STATUS Stk_DocumentImpl::WriteModelFile()
{
	STK_STATUS result = STK_SUCCESS;
	wstring outputFile = m_outputFullPath + m_outputName + L".model";

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	m_modelDoc.Accept(writer);
	string strJson(buffer.GetString(), buffer.GetSize());
	std::ofstream ofile;
#ifdef __MOBILE__
	string strPath;
	ChineseCode::UnicodeToUTF_8(outputFile.c_str(), strPath);
	ofile.open(strPath.c_str());
#else
	ofile.open(outputFile.c_str());
#endif
	if (ofile.fail())
	{
		return STK_ERROR_FILEWR;
	}
	ofile << strJson;
	ofile.close();
	return result;
}

STK_STATUS Stk_DocumentImpl::WriteModelBinFile()
{
	STK_STATUS result = STK_SUCCESS;
	Stk_ResourceManagerPtr resourceManagerPtr =  Stk_ResourceManagerPtr::CreateObject();
	//遍历所有Mesh对象
	wstring outputFile = m_outputFullPath + m_outputName + L".lod";
	string binFileName;
	//ChineseCode::UnicodeToUTF_8(outputFile.c_str(), GeoFileName);
	binFileName = ChineseCode::WStringToString(outputFile);
	Stk_FilePtr stk_filePtr = resourceManagerPtr->GetFile(binFileName);
	stk_filePtr->OpenAsBin("wb");
	//遍历所有的Model信息
	map<int, Stk_ModelPtr>::iterator it;
	Stk_ModelBlock* pStkModelBlock = new Stk_ModelBlock();
	for(  it=m_mapIdToStkModel.begin(); it!=m_mapIdToStkModel.end(); it++)
	{
		StkModelBlock stkModelBlock;
		Stk_ModelPtr modelPtr = it->second;
		stkModelBlock.modelID = it->first;
		modelPtr->GetBoundingBox(stkModelBlock.BBox, 6);
		stkModelBlock.numLod = modelPtr->GetLodCount();
		pStkModelBlock->AddModel(stkModelBlock);
		//创建访问器对象
		StkAccessorBlock stkAccessorBlock;
		switch (stkModelBlock.numLod)
		{
		case 1:
			{
				modelPtr->GetLOD1Accessor(stkAccessorBlock.fileIndex, stkAccessorBlock.blockOffset, stkAccessorBlock.blockLength);
				pStkModelBlock->AddAccessor(stkAccessorBlock);
			}
			break;
		case 2:
			{
				modelPtr->GetLOD1Accessor(stkAccessorBlock.fileIndex, stkAccessorBlock.blockOffset, stkAccessorBlock.blockLength);
				pStkModelBlock->AddAccessor(stkAccessorBlock);
				modelPtr->GetLOD2Accessor(stkAccessorBlock.fileIndex, stkAccessorBlock.blockOffset, stkAccessorBlock.blockLength);
				pStkModelBlock->AddAccessor(stkAccessorBlock);
			}
			break;
		}
	}
	//创建访问器对象
	Stk_CommonAccessorPtr stkMeshAccessor = Stk_CommonAccessorPtr::CreateObject();
	pStkModelBlock->WrieteFile(stkMeshAccessor, stk_filePtr);
	stk_filePtr->Close();
	delete pStkModelBlock;
	resourceManagerPtr->Clear();
	string strTemp = m_strOutputName + ".lod";
	m_vecFileName.push_back(strTemp);
	return result;
}

HoteamSoft::SVLLib::STK_STATUS Stk_DocumentImpl::WriteGeoFile()
{
	STK_STATUS result = STK_SUCCESS;
	Stk_ResourceManagerPtr resourceManagerPtr =  Stk_ResourceManagerPtr::CreateObject();
	//遍历所有Mesh对象
	int numMesh = m_vecMeshes.size();
	wstring outputFile = m_outputFullPath + m_wstroutputBinName;
	string GeoFileName;
	ChineseCode::UnicodeToUTF_8(outputFile.c_str(), GeoFileName);
	Stk_FilePtr stk_filePtr = resourceManagerPtr->GetFile(GeoFileName);
	stk_filePtr->OpenAsBin("wb");
	for (int i = 0; i < numMesh; i++)
	{
		Stk_MeshPrimPtr stkMesh = m_vecMeshes[i];
		Value* valMesh = (Value*)stkMesh->GetBindedData();
		Stk_MeshAccessorPtr stk_MeshAccessorPtr = Stk_MeshAccessorPtr::CreateObject();
		stk_MeshAccessorPtr->SetType(ACCESSOR_TYPE_MESH_I_P_N);
		stk_MeshAccessorPtr->WriteFile(stkMesh,stk_filePtr);
		stk_MeshAccessorPtr->SetBinFileName(m_outputBinNameUTF8);
		//构造Value节点
		Value valAccessor;
		stk_MeshAccessorPtr->Encode(valAccessor, m_modelDoc);
	    valMesh->AddMember("Accesor", valAccessor, m_modelDoc.GetAllocator());
	}
	stk_filePtr->Close();
	resourceManagerPtr->Clear();
	return result;
}

HoteamSoft::SVLLib::STK_STATUS Stk_DocumentImpl::WriteMeshFile()
{
	STK_STATUS result = STK_SUCCESS;
	Stk_ResourceManagerPtr resourceManagerPtr =  Stk_ResourceManagerPtr::CreateObject();
	//遍历所有Mesh对象
	int numMesh = m_vecMeshes.size();
	wstring outputFile = m_outputFullPath + m_outputName + L".mesh";
	string GeoFileName;
	//ChineseCode::UnicodeToUTF_8(outputFile.c_str(), GeoFileName);
	GeoFileName = ChineseCode::WStringToString(outputFile);
	Stk_FilePtr stk_filePtr = resourceManagerPtr->GetFile(GeoFileName);
	stk_filePtr->OpenAsBin("wb");
	//遍历所有的Model信息
	map<int, Stk_ModelPtr>::iterator it;
	for(  it=m_mapIdToStkModel.begin(); it!=m_mapIdToStkModel.end(); it++)
	{
		Stk_ModelPtr stkModel = it->second;
		stkModel->WriteMesh(stk_filePtr);
	}
	m_meshDataSize = stk_filePtr->FileSize();
	stk_filePtr->Close();
	resourceManagerPtr->Clear();
	string strTemp = m_strOutputName + ".mesh";
	m_vecFileName.push_back(strTemp);
	return result;
}

STK_STATUS Stk_DocumentImpl::WriteMaterialFile()
{
	STK_STATUS result = STK_SUCCESS;
	wstring outputFile = m_outputFullPath + m_outputName + L".material";


	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	m_materialDoc.Accept(writer);
	string strJson(buffer.GetString(), buffer.GetSize());
	std::ofstream ofile;
#ifdef __MOBILE__
	string strPath;
	ChineseCode::UnicodeToUTF_8(outputFile.c_str(), strPath);
	ofile.open(strPath.c_str());
#else
	ofile.open(outputFile.c_str());
#endif
	if (ofile.fail())
	{
		return STK_ERROR_FILEWR;
	}
	ofile << strJson;
	ofile.close();
	
	string strTemp = m_strOutputName + ".material";
	m_vecFileName.push_back(strTemp);
	return result;
}

STK_STATUS Stk_DocumentImpl::WriteNode( Stk_NodePtr i_stkNodeP )
{
	STK_STATUS result = STK_SUCCESS;
	Document::AllocatorType &allocator = m_nodeDoc.GetAllocator();
	int numChild = i_stkNodeP->GetNbChild();
	string strNodeNameUTF8;
	ChineseCode::UnicodeToUTF_8(i_stkNodeP->GetNodeName().c_str(), strNodeNameUTF8);
	Value children(rapidjson::kArrayType);
	//children.SetString(StringRef(strNodeNameUTF8.c_str()));
	for (int i = 0; i < numChild; i++)
	{
#if 1
		Stk_NodePtr stkChildNodeP = i_stkNodeP->GetChild(i);
		string strChildNodeNameUTF8;
		ChineseCode::UnicodeToUTF_8(stkChildNodeP->GetNodeName().c_str(), strChildNodeNameUTF8);
		//children.AddMember(StringRef(strChildNodeNameUTF8.c_str()), StringRef(strChildNodeNameUTF8.c_str()), allocator);
		rapidjson::Value info_object(rapidjson::kStringType);
		//info_object.SetObject();
		info_object.SetString(strChildNodeNameUTF8.c_str(), strChildNodeNameUTF8.size(), allocator);
		children.PushBack(info_object, allocator);
		WriteNode(stkChildNodeP);
#endif
	}
	rapidjson::Value info_object1(rapidjson::kStringType);
	info_object1.SetString(strNodeNameUTF8.c_str(), strNodeNameUTF8.size(), allocator);
	m_TopNode.AddMember(info_object1, children, allocator);
	return result;
}

STK_STATUS Stk_DocumentImpl::WriteViewFile()
{
	STK_STATUS result = STK_SUCCESS;
	if (m_pViewDoc != nullptr)
	{
		wstring outputFile = m_outputFullPath + m_outputName + L".view";
		string strOutputFile = m_strOutputName + ".view";
		result = m_pViewDoc->WriteFile(outputFile);
		m_vecFileName.push_back(strOutputFile);
	}
	return result;
}

STK_STATUS Stk_DocumentImpl::WritePMIFile()
{
	STK_STATUS result = STK_SUCCESS;
	if (m_pPMIDoc != nullptr)
	{
		wstring outputFile = m_outputFullPath + m_outputName + L".pmi";
		result = m_pPMIDoc->WriteFile(outputFile);
		string strPmiFile = m_strOutputName + ".pmi";
		m_vecFileName.push_back(strPmiFile);
	}
	return result;
}

STK_STATUS Stk_DocumentImpl::WriteAttrFile()
{
	STK_STATUS result = STK_SUCCESS;
	if (m_pAttrDoc != nullptr)
	{
		wstring outputFile = m_outputFullPath + m_outputName + L".model";
		result = m_pAttrDoc->WriteFile(outputFile);
		string strModelFile = m_strOutputName + ".model";
		m_vecFileName.push_back(strModelFile);
	}
	return result;
}

STK_STATUS Stk_DocumentImpl::WriteGeoAttrFile()
{
	STK_STATUS result = STK_SUCCESS;
	if (m_pGeoAttrDoc != nullptr)
	{
		wstring outputFile = m_outputFullPath + m_outputName + L".geo";
		result = m_pGeoAttrDoc->WriteFile(outputFile);
		string strGeoFile = m_strOutputName + ".geo";
		m_vecFileName.push_back(strGeoFile);
	}
	return result;
}

STK_STATUS Stk_DocumentImpl::WriteLineFile()
{
	STK_STATUS result = STK_SUCCESS;
	if (m_pViewDoc != nullptr)
	{
		wstring outputFile = m_outputFullPath + m_outputName + L".line";
		CRapidJsonTools::WriteLineDoc(outputFile, m_pViewDoc->GetDocument());
		string strLineFile = m_strOutputName + ".line";
		m_vecFileName.push_back(strLineFile);
	}
	return result;
}

STK_STATUS Stk_DocumentImpl::WriteLayerFile()
{
	//保存文件
	wstring outputFile = m_outputFullPath + m_outputName + L".layer";
	CRapidJsonTools::WriteLayerDoc(outputFile, m_pViewDoc->GetDocument());
	string strInfoFile = m_strOutputName + ".layer";
	m_vecFileName.push_back(strInfoFile);
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::WriteFeatFile()
{
	//保存文件
	wstring outputFile = m_outputFullPath + m_outputName + L".feat";
	CRapidJsonTools::WriteFeatDoc(outputFile, m_pViewDoc->GetDocument());
	string strInfoFile = m_strOutputName + ".feat";
	m_vecFileName.push_back(strInfoFile);
	return STK_SUCCESS;
}
//保存SVLX文件
STK_STATUS Stk_DocumentImpl::WriteSVLXFile()
{
	string strOutputPath,strZipFileName, strSVLFile, strTemp, strTempFile;
	HANDLE  tempHandle;
	strSVLFile = m_strOutputFullPath +m_strOutputName + ".svlx";
	m_vecFileName.push_back("images");
	//遍历所有数据
	int numFile = m_vecFileName.size();
#ifdef __MOBILE__
	vector<string> vecSVLFiles;
	for (int i = 0; i < numFile; i++)
	{
		strTempFile = m_strOutputFullPath + m_vecFileName[i];
		vecSVLFiles.push_back( strTempFile);
	}
//压缩数据
	CCompressSVLFiles compressSVLFiles;
	bool bCompressFlag = compressSVLFiles.CompressFileNameToSVLFiles(strSVLFile, m_vecFileName, vecSVLFiles, vecSVLFiles.size());
	vecSVLFiles.clear();
#else
#if 0
	HANDLE* fileHandleArr = new HANDLE[numFile];
	for (int i = 0; i < numFile; i++)
	{
		strTempFile = m_strOutputFullPath + m_vecFileName[i];
		tempHandle = CreateFileA(strTempFile.c_str(), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, 0);
		if (INVALID_HANDLE_VALUE == tempHandle)
		{
			continue;
		}
		fileHandleArr[i] = tempHandle;
	}
	//压缩数据
	CCompressSVLFiles compressSVLFiles;
	bool bCompressFlag = compressSVLFiles.CompressHandleToSVLFiles(strSVLFile, m_vecFileName, fileHandleArr, numFile);

	//关闭句柄
	for (int i = 0; i < numFile; i++)
	{
		CloseHandle(fileHandleArr[i]);
	}
	delete fileHandleArr;
#endif
	vector<string>		vecFileName;
	HANDLE* fileHandleArr = new HANDLE[numFile];
	vector<string>		vecFolderName;
	vector<string>		vecFolderPath;
	int iFileNum = 0;
	int iFolderNum = 0;
	for (int i = 0; i < numFile; i++)
	{
		strTempFile = m_strOutputFullPath + m_vecFileName[i];
		WIN32_FIND_DATAA  wfd;
		HANDLE hFind = FindFirstFileA(strTempFile.c_str(), &wfd);
		if ((hFind != INVALID_HANDLE_VALUE))
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				vecFolderName.push_back(m_vecFileName[i]);
				vecFolderPath.push_back(strTempFile);
				iFolderNum++;
			}
			else
			{
				tempHandle = CreateFileA(strTempFile.c_str(), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, 0);
				if (INVALID_HANDLE_VALUE == tempHandle)
				{
					continue;
				}
				vecFileName.push_back(m_vecFileName[i]);
				fileHandleArr[iFileNum] = tempHandle;
				iFileNum++;
			}
		}
	}
	//压缩数据
	CCompressSVLFiles compressSVLFiles;
	bool bCompressFlag = compressSVLFiles.CompressHandleToSVLFiles(strSVLFile, vecFileName, fileHandleArr, iFileNum,vecFolderName,vecFolderPath,iFolderNum);

	//关闭句柄
	for (int i = 0; i < iFileNum; i++)
	{
		CloseHandle(fileHandleArr[i]);
	}
	delete fileHandleArr;
#endif
	m_vecFileName.clear();

	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::AddSVLXFileItem(const std::string& strFileName)
{
	STK_STATUS result = STK_SUCCESS;
	if (strFileName.empty()) {
		return STK_ERROR_FILEWR;
	}
	for (int i = 0; i < m_vecFileName.size(); i++)
	{
		if (m_vecFileName[i] == strFileName)
		{
			return STK_SUCCESS_EXIST;
		}
	}
	m_vecFileName.push_back(strFileName);
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::ClearSVLXFileItem()
{
	STK_STATUS result = STK_SUCCESS;
	m_vecFileName.clear();
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::GetSVLXFileItem(const std::string& i_strFileName,  unsigned int& o_bufSize, char** o_bufP, bool bByExt)
{
	STK_STATUS status = STK_SUCCESS;
	//从内存中解压文件
	CExtractSVLFiles extractSVLFiles;
	char **bufP;
	std::vector<int> vecBufNum;
    string strFileName;
	if(bByExt)
	{
		getFileNameBySuffix(i_strFileName, strFileName);
	}
	else
	{
		strFileName = i_strFileName;
	}
	bool bExtract = extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP, strFileName, &bufP, vecBufNum);
	if (!bExtract)
		return STK_ERROR;
	int bufsize = 0;
	int numBuf = vecBufNum.size();
	for (int i = 0; i < numBuf; i++)
	{
		bufsize += vecBufNum[i];
	}
	o_bufSize = bufsize;
	*o_bufP = new char[o_bufSize];
	memset(*o_bufP, 0, o_bufSize);
	//释放解压出来的内存
	int iReadBufSize = 0;
	for (int i = 0; i < numBuf; i++)
	{
		if (iReadBufSize + vecBufNum[i] <= o_bufSize)
		{
			memcpy((*o_bufP) + iReadBufSize, bufP[i], vecBufNum[i]);
		}
		iReadBufSize += vecBufNum[i];
		MidFree(bufP[i]);
		bufP[i] = NULL;
	}
	//o_bufP[iReadBufSize] = '\0';
	MidFree(bufP);
	bufP = NULL;
	return status;
}

STK_STATUS Stk_DocumentImpl::GetSVLXFileItemToFile(const std::string& i_strFileName, const std::string& i_strOutFilePath, bool bByExt)
{
	STK_STATUS status = STK_SUCCESS;
	
	unsigned int bufSize =0;
	char* bufP = 0;

	string strOutPath = i_strOutFilePath;
	string strDir = i_strFileName;
	std::vector<std::string> vecDirValue;
	CStringTools::StringSplit(strDir, "/", vecDirValue);
	if (vecDirValue.size() > 1)
	{
		for (int i = 0; i < vecDirValue.size() - 1; i++)
		{
			
#ifdef _WIN32  
			strOutPath.append("\\");
			strOutPath.append(vecDirValue[i]);
			_mkdir(strOutPath.c_str());
#else  // Linux  
			strOutPath.append("/");
			strOutPath.append(vecDirValue[i]);
			mkdir(vecDirValue[i].c_str(), S_IRWXU | S_IRGRP | S_IXGRP);
#endif  
		}
		strOutPath = i_strOutFilePath;
	}
#ifdef _WIN32
	if(strOutPath.find_last_of("\\") < strOutPath.length()-1)
		strOutPath.append("\\");
#else  // Linux
		if(strOutPath.find_last_of("/") < strOutPath.length()-1)
		strOutPath.append("/");
#endif

	
	if (bByExt)
	{
		string strTempFileName = "";
		getFileNameBySuffix(i_strFileName, strTempFileName);
		strOutPath.append(strTempFileName);
	}
	else
	{
		strOutPath.append(i_strFileName);
	}
	GetSVLXFileItem(i_strFileName, bufSize, &bufP, bByExt);
	if (bufSize <= 0)
		return STK_ERROR;

	FILE* pFile = fopen(strOutPath.c_str(), "wb");
	if (pFile)
	{
		if (fwrite(bufP, 1, bufSize, pFile) == 0)//若大于bufferLength则意味着产生了错误
		{
			delete bufP;
			fclose(pFile);
			return STK_ERROR_FILEWR;
		}
		delete bufP;
		fclose(pFile);
	}
	return status;
}

STK_STATUS Stk_DocumentImpl::GetSVLXFolderItemToFolder(const std::string& i_strFileName, const std::string& i_strOutFilePath)
{
	STK_STATUS status = STK_SUCCESS;

	unsigned int bufSize = 0;
	char* bufP = 0;
#ifdef _WIN32  
	_mkdir(i_strOutFilePath.c_str());
#else  // Linux  
    mkdir(i_strOutFilePath.c_str(), S_IRUSR);
#endif  
	string strOutPath = i_strOutFilePath;
	string strDir = i_strFileName;
	std::vector<std::string> vecDirValue;
	CStringTools::StringSplit(strDir, "/", vecDirValue);
	if (vecDirValue.size() > 1)
	{
		for (int i = 0; i < vecDirValue.size()-1; i++)
		{

#ifdef _WIN32  
			strOutPath.append("\\");
			strOutPath.append(vecDirValue[i]);
			_mkdir(strOutPath.c_str());
#else  // Linux  
			strOutPath.append("/");
			strOutPath.append(vecDirValue[i]);
			mkdir(vecDirValue[i].c_str(), S_IRWXU | S_IRGRP | S_IXGRP);
#endif  
		}
		strOutPath = i_strOutFilePath;
	}

	strOutPath.append("\\");

	CExtractSVLFiles extractSVLFiles;
	std::vector<std::string> strFileNameA;
	std::vector<std::string> strFolderNameA;
	bool bExtract = extractSVLFiles.GetFolderFileList(m_svlxBufferSize, m_bufP,  i_strFileName, strFileNameA, strFolderNameA);
	for (int i = 0; i < strFolderNameA.size(); i++)
	{
		string strTempFilePath = strOutPath;
#ifdef _WIN32  
		strTempFilePath.append("\\");
		strTempFilePath.append(strFolderNameA[i]);
		_mkdir(strTempFilePath.c_str());
#else  // Linux  
		strTempFilePath.append("/");
		strTempFilePath.append(vecDirValue[i]);
		mkdir(strTempFilePath.c_str(), S_IRWXU | S_IRGRP | S_IXGRP);
#endif  
	}

	
	for (int i = 0; i < strFileNameA.size(); i++)
	{
		string strTempFilePath = strOutPath;
		strTempFilePath.append(strFileNameA[i].substr(i_strFileName.length()+1));

		GetSVLXFileItem(strFileNameA[i], bufSize, &bufP, false);
		if (bufSize <= 0)
			return STK_ERROR;

		FILE* pFile = fopen(strTempFilePath.c_str(), "wb");
		if (pFile)
		{
			if (fwrite(bufP, 1, bufSize, pFile) == 0)//若大于bufferLength则意味着产生了错误
			{
				delete bufP;
				bufP = 0;
				fclose(pFile);
				return STK_ERROR_FILEWR;
			}
			delete bufP;
			bufP = 0;
			fclose(pFile);
		}
	}
	
	return status;
}


IOStream* Stk_DocumentImpl::OpenFile(std::string path, const char* mode, bool absolute)
{
	if (path.size() < 2) return 0;
	//if (!absolute && path[1] != ':' && path[0] != '/') { // relative?
	//	path = mCurrentAssetDir + path;
	//}
	FILE* f = fopen(path.c_str(), mode);
	return f ? new IOStream(f) : 0;
}

STK_STATUS Stk_DocumentImpl::OpenFile(const unsigned int i_bufferSize, const char* i_bufP)
{
	//设置文件打开的模式
	m_enumFileOpenMode = STK_FILE_OPENMODE_BUFFER;
	m_svlxBufferSize = i_bufferSize;
	m_bufP = (char*)i_bufP;
	//获取压缩文件中的文件名称
	CGetCompressContents fileContent;
	if (!fileContent.GetCompressBufferContents(i_bufferSize, i_bufP, m_vecFileName))
	{
		return STK_ERROR;
	}
	//将Mesh文件解压到系统临时目录中
	//loadMeshFromBuff();
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::OpenFile(const std::wstring& fileFullPath)
{
	string strFilePath = ChineseCode::WStringToString(fileFullPath);
	//将压缩文件缓存到内存中
	STK_UINT32 mBlockByteSize;
	STK_CHAR* mBlockMemory = NULL;
	m_svlxFilePtr = Stk_FilePtr::CreateObject();
	m_svlxFilePtr->SetName(strFilePath);
	m_svlxFilePtr->OpenAsBin("rb");
	mBlockByteSize = m_svlxFilePtr->FileSize();
	mBlockMemory = (STK_CHAR*)malloc(mBlockByteSize);
	m_svlxFilePtr->ReadBuffer(mBlockMemory, mBlockByteSize, 0);
	//打开文件
	this->OpenFile(mBlockByteSize, mBlockMemory);
	m_isNeedDelSvlxBuf = true;
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::OpenLocal(const std::wstring& fileFullPath)
{
	MutexLock locked(this->m_mutexDocRead);
	m_mapIdToNode.clear();
	m_mapNodeIdToStkNode.clear();
	m_mapIdToStkPart.clear();
	m_mapIdToStkModel.clear();
	if (m_bOpened) {
		return STK_SUCCESS;
	}
	m_bRemote = false;
	//去掉扩展名
	int start = fileFullPath.find_last_of('.');
	if (-1 == start) {
		return STK_ERROR_FILERD;
	}
	wstring _fullPath = fileFullPath.substr(0, start);
	SetOutputFullPath(_fullPath);
	//初始化资源管理器
	initBinFile();
#if 0
	//加载Json文件流
	string strStream;
	std::ifstream ifile;
	ifile.open(fileFullPath.c_str(), ios_base::in);
	if (!ifile.is_open())
	{
		return STK_ERROR;
	}
	string strLine;
	while(getline(ifile, strLine))
	{
		strStream.append(strLine + "\n");
	}
	ifile.close();
	m_nodeDoc.Parse<0>(strStream.c_str());

	FILE* f = _wfopen(fileFullPath.c_str(), L"rb");
	if (f == NULL)
	{
		return STK_ERROR;
	}
	IOStream* nodeStream = new IOStream(f);
	size_t mFileLength = nodeStream->FileSize();
	std::vector<char> nodeData(mFileLength + 1);
	nodeData[mFileLength] = '\0';

	if (nodeStream->Read(&nodeData[0], 1, mFileLength) != mFileLength) {
		delete nodeStream;
		return STK_ERROR;
	}
	delete nodeStream;
	//解析Json结构
	m_nodeDoc.ParseInsitu(&nodeData[0]);
	//m_nodeDoc.Parse(&nodeData[0]);

	//初始化Map
	//InitJsonNodeMap();
	//InitJsonPartMap();
	InitJsonInstanceMap();
	InitJsonModelMap();
	initBinFile();
	InitViewFile();
	InitPMIFile();
	InitAttrFile();
	if (m_nodeDoc.HasParseError()) {
		return STK_ERROR;
	}
	if (!m_nodeDoc.IsObject()) {
		return STK_ERROR;
	}
#endif
	m_bOpened = true;
	return STK_SUCCESS;
}

HoteamSoft::SVLLib::STK_STATUS Stk_DocumentImpl::LoadBom(const std::wstring& fileFullPath)
{
	m_mapIdToInstance.clear();
	m_mapNodeIdToInstanceNode.clear();
	//设置打开路径
	int start = fileFullPath.find_last_of('.');
	if (-1 == start) {
		return STK_ERROR_FILERD;
	}
	wstring _fullPath = fileFullPath.substr(0, start);
	SetOutputFullPath(_fullPath);
	//加载Bom的Json串
	string strStream;
	std::ifstream ifile;
#ifdef __MOBILE__
	string strPath;
	ChineseCode::UnicodeToUTF_8(fileFullPath.c_str(), strPath);
	ifile.open(strPath.c_str(), ios_base::in);
#else
	ifile.open(fileFullPath.c_str(), ios_base::in);
#endif
	if (!ifile.is_open())
	{
		return STK_ERROR;
	}
	string strLine;
	while(getline(ifile, strLine))
	{
		strStream.append(strLine + "\n");
	}
	ifile.close();
	m_nodeDoc.Parse<0>(strStream.c_str());
	if (m_nodeDoc.HasParseError()) {
		return STK_ERROR;
	}
	if (!m_nodeDoc.IsObject()) {
		return STK_ERROR;
	}
	//初始化Bom结构
	InitJsonInstanceMap();
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::LoadAttribute(const std::wstring& fileFullPath)
{
	if (m_pAttrDoc != nullptr)
	{
		//wstring strAttrFilePath = m_outputFullPath;
		m_pAttrDoc->OpenFile(fileFullPath);
		return STK_SUCCESS;
	}
	return STK_ERROR;
}

STK_STATUS Stk_DocumentImpl::loadAttributeStream()
{
	STK_STATUS status = STK_SUCCESS;
	//从压缩文件中获取后缀名称为.bom的文件
	string strModelFileName;
	getFileNameBySuffix(".model", strModelFileName);
	//从内存中解压文件
	CExtractSVLFiles extractSVLFiles;
	char **bufP;
	std::vector<int> vecBufNum;
	extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP, strModelFileName, &bufP, vecBufNum);
	//将解压的数据复制到字符串中
	//status = CRapidJsonTools::ReadLoadAttr(&bufP, vecBufNum, m_pAttrDoc);
	int numBuf = vecBufNum.size();
	//int index= 0;
	UINT   niOffsetSize = 0;
	for (int i = 0; i < numBuf; i++)
	{
		m_strAttributeBuf.append((bufP)[i]);
		MidFree((bufP)[i]);
		(bufP)[i] = NULL;
	}
	MidFree (bufP);
	bufP = NULL;
	return status;
}

STK_STATUS Stk_DocumentImpl::unloadAttibuteStream()
{
	STK_STATUS status = STK_SUCCESS;
	m_strAttributeBuf.clear();
	m_strAttributeBuf.shrink_to_fit();
	return status;
}

STK_STATUS Stk_DocumentImpl::getInstanceNames(map<STK_UINT32, std::wstring>& i_mapInstanceIDtoName)
{
	return CRapidJsonTools::getInstanceNames(m_strAttributeBuf, i_mapInstanceIDtoName);
}

STK_STATUS Stk_DocumentImpl::getAttributesByIds(vector<STK_UINT32> i_vecId, map<STK_UINT32, map<std::wstring, std::wstring>>& i_mapKeyToValue)
{
	return CRapidJsonTools::getAttributesByIds(m_strAttributeBuf, i_vecId, i_mapKeyToValue);
}

STK_STATUS Stk_DocumentImpl::unLoadAttribute()
{
	if (m_pAttrDoc != nullptr)
	{
		m_pAttrDoc->unLoad();
		return STK_SUCCESS;
	}
	return STK_ERROR;
}

STK_STATUS Stk_DocumentImpl::UnLoadGeoAttribute()
{
	if (m_pGeoAttrDoc != nullptr)
	{
		m_pGeoAttrDoc->UnLoad();
		return STK_SUCCESS;
	}
	return STK_ERROR;
}

STK_STATUS Stk_DocumentImpl::LoadPMI(const std::wstring& fileName)
{
	if (m_pPMIDoc != nullptr)
	{
		m_pPMIDoc->OpenFile(fileName);
		return STK_SUCCESS;
	}
	return STK_ERROR;
}

STK_STATUS Stk_DocumentImpl::LoadView(const std::wstring& fileName)
{
	if (m_pViewDoc != nullptr)
	{
		m_pViewDoc->OpenFile(fileName);
		return STK_SUCCESS;
	}
	return STK_ERROR;
}

STK_STATUS Stk_DocumentImpl::LoadBom(void* i_docPtr)
{
	STK_STATUS status = STK_SUCCESS;
	//获取文件信息
	string strBomFileName;
	strBomFileName = m_strOutputName + ".bom";
	//获取资源管理器
	Stk_FilePtr stk_filePtr = m_resourceManagerPtr->GetFile(strBomFileName);
	stk_filePtr->OpenAsBin("rb"); 
	//解析Bom
	parseBom(i_docPtr, stk_filePtr);
	return status;
}

STK_STATUS Stk_DocumentImpl::LoadBom(void* i_docPtr, const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType)
{
	STK_STATUS status = STK_SUCCESS;
	Stk_FilePtr bomFilePtr = Stk_FilePtr::CreateObject();
	switch (i_enumBlockType)
	{
	case STK_MEMBLOCK_TYPE_ORIGIN:
		{
			bomFilePtr->OpenAsBuffer(i_bufSize);
			bomFilePtr->WriteBuffer((char*)i_bufP, i_bufSize);
			//解析BOM信息
			status = parseBom(i_docPtr, bomFilePtr);
		}
		break;
	case STK_MEMBLOCK_TYPE_COMPRESS_7Z:
		{
			//解压内存文件
			char ***bufP;
			vector<string> vecFileName;
			vector<vector<int>> vecBufNum;
			CExtractSVLFiles::ExtractBufferToAllSVLFiles(i_bufSize, i_bufP, vecFileName, &bufP, vecBufNum);
			if(1 != vecFileName.size())
				return STK_ERROR;
			//解压文件信息
			Stk_FilePtr bomFilePtr = Stk_FilePtr::CreateObject();
			int bufsize = 0;
			int numBuf = vecBufNum[0].size();
			for (int i =0; i < numBuf; i++)
			{
				bufsize += vecBufNum[0][i];
			}
			bomFilePtr->OpenAsBuffer(bufsize);
			//释放解压出来的内存
			for (int i = 0; i < numBuf; i++)
			{
				bomFilePtr->WriteBuffer(*bufP[i], vecBufNum[0][i]);
				MidFree(*bufP[i]);
				*bufP[i] = NULL;
			}
			MidFree (*bufP);
			*bufP = NULL;
			//解析bom
			status = parseBom(i_docPtr, bomFilePtr);
		}
		break;
	}
	return status;
}

STK_STATUS Stk_DocumentImpl::LoadAttribute(void* i_docPtr)
{
	//获取文件信息
	string strAttrFileName;
	strAttrFileName = m_strOutputFullPath +  m_strOutputName + ".model";
	wstring wstrAttrFileName;
	wstrAttrFileName = ChineseCode::StringToWString(strAttrFileName);
	return LoadAttribute(wstrAttrFileName);
}

STK_STATUS Stk_DocumentImpl::LoadGeoAttribute(void* i_docPtr)
{
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::LoadPMI(void* i_docPtr)
{
	string strPMIFileName;
	strPMIFileName = m_strOutputFullPath + m_strOutputName + ".pmi";
	wstring wstrPMIFileName;
	wstrPMIFileName = ChineseCode::StringToWString(strPMIFileName);
	return LoadPMI(wstrPMIFileName);
}

STK_STATUS Stk_DocumentImpl::LoadView(void* i_docPtr)
{
	string strViewFileName;
	strViewFileName = m_strOutputFullPath + m_strOutputName + ".view";
	wstring wstrViewFileName;
	wstrViewFileName = ChineseCode::StringToWString(strViewFileName);
	return LoadView(wstrViewFileName);
}

STK_STATUS Stk_DocumentImpl::LoadBomFromBuf(void* i_docPtr)
{
	STK_STATUS status = STK_SUCCESS;
	//从压缩文件中获取后缀名称为.bom的文件
	string strBomFileName;
	getFileNameBySuffix(".bom", strBomFileName);
	//从内存中解压文件
	CExtractSVLFiles extractSVLFiles;
	char **bufP;
	std::vector<int> vecBufNum;
	extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP,  strBomFileName, &bufP, vecBufNum);
	//创建文件对象
	Stk_FilePtr bomFilePtr = Stk_FilePtr::CreateObject();
	int bufsize = 0;
	int numBuf = vecBufNum.size();
	for (int i =0; i < numBuf; i++)
	{
		bufsize += vecBufNum[i];
	}
	bomFilePtr->OpenAsBuffer(bufsize);
	//释放解压出来的内存
	for (int i = 0; i < numBuf; i++)
	{
		bomFilePtr->WriteBuffer(bufP[i], vecBufNum[i]);
		MidFree(bufP[i]);
		bufP[i] = NULL;
	}
	MidFree (bufP);
	bufP = NULL;
	//解析BOM信息
	status = parseBom(i_docPtr, bomFilePtr);
	return status;
}

STK_STATUS Stk_DocumentImpl::parseBom(void* i_docPtr, Stk_FilePtr& i_bomFilePtr)
{
	STK_STATUS status = STK_SUCCESS;
	Stk_BomBlock* pStkBomBlock = new Stk_BomBlock();
	pStkBomBlock->ReadFile(i_bomFilePtr);
	//获取Model块的相关信息
	int numBom = pStkBomBlock->getInstanceCount();
	int accessorBlockOffset = 0;
	for (int i = 0; i < numBom; i++)
	{
		StkBomBlock stkBomBlock = pStkBomBlock->getInstanceByIdx(i);
		//创建Model对象
		Stk_InstancePtr stkInstance = Stk_InstancePtr::CreateObject();
		stkInstance->SetID(stkBomBlock.instanceID);
		stkInstance->BindBomBlock(&stkBomBlock);
		stkInstance->SetDocument(i_docPtr);
		//stkInstance->SetModel(stkBomBlock.modelID);
		vector<Stk_InstancePtr>& parentSameChild = m_parentInstanceIdToStkInstance[stkBomBlock.parentID];
		parentSameChild.push_back(stkInstance);
	}
	m_instanceCount = numBom;
	delete pStkBomBlock;
	return status;
}

STK_STATUS Stk_DocumentImpl::LoadFileInfo()
{
	STK_STATUS status = STK_SUCCESS;
	wstring wstrLoadInfoFile = m_outputFullPath + m_outputName + L".info";
	status = CRapidJsonTools::ReadLoadInf(wstrLoadInfoFile, Stk_LoadInf::GetInstance());
	return status;
}

STK_STATUS Stk_DocumentImpl::LoadFileInfo(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType)
{
	STK_STATUS status = STK_SUCCESS;
	switch (i_enumBlockType)
	{
	case STK_MEMBLOCK_TYPE_ORIGIN:
		{
			//解析BOM信息
			status = CRapidJsonTools::ReadLoadInf((char*)i_bufP, i_bufSize, Stk_LoadInf::GetInstance());
		}
		break;
	case STK_MEMBLOCK_TYPE_COMPRESS_7Z:
		{
			//解压内存文件
			char ***bufP;
			vector<string> vecFileName;
			vector<vector<int>> vecBufNum;
			CExtractSVLFiles::ExtractBufferToAllSVLFiles(i_bufSize, i_bufP, vecFileName, &bufP, vecBufNum);
			if(1 != vecFileName.size())
				return STK_ERROR;
			//解压文件信息
			int bufsize = 0;
			int numBuf = vecBufNum[0].size();
			for (int i =0; i < numBuf; i++)
			{
				bufsize += vecBufNum[0][i];
			}
			//解析FileInfo文件
			status = CRapidJsonTools::ReadLoadInf(bufP, vecBufNum[0], Stk_LoadInf::GetInstance());
		}
		break;
	}
	return status;
}

HoteamSoft::SVLLib::STK_STATUS Stk_DocumentImpl::LoadFileFromBuf()
{
	STK_STATUS status = STK_SUCCESS;
	//从压缩文件中获取后缀名称为.info的文件
	string strModelFileName;
	if (getFileNameBySuffix(".info", strModelFileName))
	{
		CExtractSVLFiles extractSVLFiles;
		char **bufP;
		std::vector<int> vecBufNum;
		extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP, strModelFileName, &bufP, vecBufNum);
		//解析压缩后的文件加载数据
		status = CRapidJsonTools::ReadLoadInf(&bufP, vecBufNum, Stk_LoadInf::GetInstance());
	}
	return status;
}

STK_STATUS Stk_DocumentImpl::LoadAttrFromBuf(void* i_docPtr)
{
	STK_STATUS status = STK_SUCCESS;
	//从压缩文件中获取后缀名称为.model的文件
	string strModelFileName;
	if (getFileNameBySuffix(".model", strModelFileName))
	{
		//从内存中解压文件
		CExtractSVLFiles extractSVLFiles;
		char **bufP;
		std::vector<int> vecBufNum;
		extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP, strModelFileName, &bufP, vecBufNum);
		//解析压缩后的文件加载数据
		status = CRapidJsonTools::ReadLoadAttr(&bufP, vecBufNum, m_pAttrDoc);
	}
	return status;
}

STK_STATUS Stk_DocumentImpl::LoadGeoAttrFromBuf(void* i_docPtr)
{
	STK_STATUS status = STK_SUCCESS;
	//从压缩文件中获取后缀名称为.model的文件
	string strGeoAttributeFileName;
	if (getFileNameBySuffix(".geo", strGeoAttributeFileName))
	{
		//从内存中解压文件
		CExtractSVLFiles extractSVLFiles;
		char **bufP;
		std::vector<int> vecBufNum;
		extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP, strGeoAttributeFileName, &bufP, vecBufNum);
		//解析压缩后的文件加载数据
		status = CRapidJsonTools::ReadLoadGeoAttr(&bufP, vecBufNum, m_pGeoAttrDoc);
	}
	return status;
}

STK_STATUS Stk_DocumentImpl::LoadPMIFromBuf(void* i_docPtr)
{
	STK_STATUS status = STK_SUCCESS;
	//从压缩文件中获取后缀名称为.pmi的文件
	string strModelFileName;
	if (getFileNameBySuffix(".pmi", strModelFileName))
	{
		//从内存中解压文件
		CExtractSVLFiles extractSVLFiles;
		char **bufP;
		std::vector<int> vecBufNum;
		extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP, strModelFileName, &bufP, vecBufNum);
		//解析压缩后的文件加载数据
		status = CRapidJsonTools::ReadLoadPMI(&bufP, vecBufNum, m_pPMIDoc);
	}
	return status;
}

STK_STATUS Stk_DocumentImpl::LoadViewFromBuf(void* i_docPtr)
{
	STK_STATUS status = STK_SUCCESS;
	//从压缩文件中获取后缀名称为.view的文件
	string strModelFileName;
	if (getFileNameBySuffix(".view", strModelFileName))
	{
		//从内存中解压文件
		CExtractSVLFiles extractSVLFiles;
		char **bufP;
		std::vector<int> vecBufNum;
		extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP, strModelFileName, &bufP, vecBufNum);
		//解析压缩后的文件加载数据
		status = CRapidJsonTools::ReadLoadView(&bufP, vecBufNum, m_pViewDoc);
	}
	return status;
}

STK_STATUS Stk_DocumentImpl::LoadAttr(void* i_docPtr)
{
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::LoadLayer(void* i_docPtr)
{
	STK_STATUS status = STK_SUCCESS;
	//从压缩文件中获取后缀名称为.lod的文件
	string strLayerFileName;
	if(getFileNameBySuffix(".layer", strLayerFileName))
	{
		CExtractSVLFiles extractSVLFiles;
		char **bufP;
		std::vector<int> vecBufNum;
		extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP, strLayerFileName, &bufP, vecBufNum);
		//解析压缩后的文件加载数据
		status = CRapidJsonTools::ReadLayer(&bufP, vecBufNum, i_docPtr);
	}
	return status;
}

STK_STATUS Stk_DocumentImpl::LoadFeat(void* i_docPtr)
{
	STK_STATUS status = STK_SUCCESS;
	//从压缩文件中获取后缀名称为.lod的文件
	string strFeatFileName;
	if (getFileNameBySuffix(".feat", strFeatFileName))
	{
		CExtractSVLFiles extractSVLFiles;
		char **bufP;
		std::vector<int> vecBufNum;
		extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP, strFeatFileName, &bufP, vecBufNum);
		//解析压缩后的文件加载数据
		status = CRapidJsonTools::ReadFeat(&bufP, vecBufNum, i_docPtr);
	}
	return status;
}

STK_STATUS Stk_DocumentImpl::UnloadBom()
{
	m_mapIdToInstance.clear();
	m_mapNodeIdToInstanceNode.clear();
	m_nodeDoc.RemoveAllMembers();
	//清除掉Map内存
	//m_parentInstanceIdToStkInstance.clear();
#if 1
	map<int, vector<Stk_InstancePtr>>::iterator it;
	map<int, vector<Stk_InstancePtr>> emptyMap;
	vector<Stk_InstancePtr>emptyVector;
	for (it = m_parentInstanceIdToStkInstance.begin(); it !=  m_parentInstanceIdToStkInstance.end(); it++)
	{
		it->second.swap(emptyVector);
		it->second.clear();
	}
	m_parentInstanceIdToStkInstance.swap(emptyMap);
#endif
	m_parentInstanceIdToStkInstance.clear();
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::LoadModel()
{
#if 0
	m_mapIdToStkModel.clear();
	wstring wstrModelFilePath;
	string strModelFilePath;
	wstrModelFilePath = m_outputFullPath + m_outputName + L".model";
	strModelFilePath = ChineseCode::WStringToString(wstrModelFilePath);
	string strStream;
	std::ifstream ifile;
	ifile.open(strModelFilePath.c_str(), ios_base::in);
	if (!ifile.is_open())
	{
		return STK_ERROR;
	}
	string strLine;
	while(getline(ifile, strLine))
	{
		strStream.append(strLine + "\n");
	}
	ifile.close();
	m_modelDoc.Parse<0>(strStream.c_str());
	if (m_modelDoc.HasParseError()) {
		return STK_ERROR;
	}
	if (!m_modelDoc.IsObject()) {
		return STK_ERROR;
	}
	//初始化Bom结构
	InitJsonModelMap();
#endif
	//初始化Model对象
	//initBinFile();
	InitModel();
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::LoadModel(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType)
{
	STK_STATUS status = STK_SUCCESS;
	Stk_FilePtr modelFilePtr = Stk_FilePtr::CreateObject();
	switch (i_enumBlockType)
	{
	case STK_MEMBLOCK_TYPE_ORIGIN:
		{
			modelFilePtr->OpenAsBuffer(i_bufSize);
			modelFilePtr->WriteBuffer((char*)i_bufP, i_bufSize);
			//解析BOM信息
			status = parseModel(modelFilePtr);
		}
		break;
	case STK_MEMBLOCK_TYPE_COMPRESS_7Z:
		{
			//解压内存文件
			char ***bufP;
			vector<string> vecFileName;
			vector<vector<int>> vecBufNum;
			CExtractSVLFiles::ExtractBufferToAllSVLFiles(i_bufSize, i_bufP, vecFileName, &bufP, vecBufNum);
			if(1 != vecFileName.size())
				return STK_ERROR;
			//解压文件信息
			//Stk_FilePtr bomFilePtr = Stk_FilePtr::CreateObject();
			int bufsize = 0;
			int numBuf = vecBufNum[0].size();
			for (int i =0; i < numBuf; i++)
			{
				bufsize += vecBufNum[0][i];
			}
			modelFilePtr->OpenAsBuffer(bufsize);
			//释放解压出来的内存
			for (int i = 0; i < numBuf; i++)
			{
				modelFilePtr->WriteBuffer(*bufP[i], vecBufNum[0][i]);
				MidFree(*bufP[i]);
				*bufP[i] = NULL;
			}
			MidFree (*bufP);
			*bufP = NULL;
			//解析bom
			status = parseModel(modelFilePtr);
		}
		break;
	}
	return status;
}

STK_STATUS Stk_DocumentImpl::loadModelFromBuf()
{
	//从压缩文件中获取后缀名称为.lod的文件
	string strModelFileName;
	getFileNameBySuffix(".lod", strModelFileName);
	//从内存中解压文件
	CExtractSVLFiles extractSVLFiles;
	char **bufP;
	std::vector<int> vecBufNum;
	extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP,  strModelFileName, &bufP, vecBufNum);
	//创建文件对象
	Stk_FilePtr modelFilePtr = Stk_FilePtr::CreateObject();
	int bufsize = 0;
	int numBuf = vecBufNum.size();
	for (int i =0; i < numBuf; i++)
	{
		bufsize += vecBufNum[i];
	}
	modelFilePtr->OpenAsBuffer(bufsize);
	//释放解压出来的内存
	for (int i = 0; i < numBuf; i++)
	{
		modelFilePtr->WriteBuffer(bufP[i], vecBufNum[i]);
		MidFree(bufP[i]);
		bufP[i] = NULL;
	}
	MidFree (bufP);
	bufP = NULL;
	//解析Model信息
	parseModel(modelFilePtr);
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::parseModel(Stk_FilePtr& i_modelFilePtr)
{
	STK_STATUS status = STK_SUCCESS;
	Stk_ModelBlock* pStkModelBlock = new Stk_ModelBlock();
	pStkModelBlock->ReadFile(i_modelFilePtr);
	//获取Model块的相关信息
	int numModel = pStkModelBlock->getModelBlockCount();
	int accessorBlockOffset = 0;
	for (int i = 0; i < numModel; i++)
	{
		StkModelBlock stkModelBlock = pStkModelBlock->getModelBlockByIdx(i);
		//创建Model对象
		map<int, Stk_ModelPtr>::iterator it;
		it = m_mapIdToStkModel.find(stkModelBlock.modelID);
		if (it != m_mapIdToStkModel.end())
		{
			continue;
		}
		Stk_ModelPtr stkModel = Stk_ModelPtr::CreateObject();
		stkModel->SetID(stkModelBlock.modelID);
		stkModel->BindModelBlock(&stkModelBlock);
		int numAccessorBlock = stkModelBlock.numLod;
		for (int j =0; j < numAccessorBlock; j++)
		{
			StkAccessorBlock stkAccessorBlock = pStkModelBlock->getAccessorBlockByIdx(accessorBlockOffset + j);
			stkModel->BindLodAccessorBlock(&stkAccessorBlock);
		}
		accessorBlockOffset += numAccessorBlock;
		m_mapIdToStkModel.insert(make_pair(stkModelBlock.modelID, stkModel));
	}
	delete pStkModelBlock;
	return status;
}

STK_STATUS Stk_DocumentImpl::UnloadModel()
{
	m_vecMeshes.clear();
	m_mapIdToStkModel.clear();
	m_modelDoc.RemoveAllMembers();
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::LoadMaterial()
{
	m_mapIdToStkMaterial.clear();
	wstring wstrMateriallFilePath;
	string strMaterialFilePath;
	wstrMateriallFilePath = m_outputFullPath + m_outputName + L".material";
	strMaterialFilePath = ChineseCode::WStringToString(wstrMateriallFilePath);
	string strStream;
	std::ifstream ifile;
	ifile.open(strMaterialFilePath.c_str(), ios_base::in);
	if (!ifile.is_open())
	{
		return STK_ERROR;
	}
	string strLine;
	while(getline(ifile, strLine))
	{
		strStream.append(strLine + "\n");
	}
	ifile.close();
	m_materialDoc.Parse<0>(strStream.c_str());
	if (m_materialDoc.HasParseError()) {
		return STK_ERROR;
	}
	if (!m_materialDoc.IsObject()) {
		return STK_ERROR;
	}
	//初始化Bom结构
	InitJsonMaterialMap();
	InitJsonImageMap();
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::LoadMaterial(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType)
{
	STK_STATUS status = STK_SUCCESS;
	switch (i_enumBlockType)
	{
	case STK_MEMBLOCK_TYPE_ORIGIN:
		{
			//解析BOM信息
			status = CRapidJsonTools::ReadJsonData((char*)i_bufP,  i_bufSize,  m_materialDoc);
		}
		break;
	case STK_MEMBLOCK_TYPE_COMPRESS_7Z:
		{
			//解压内存文件
			char ***bufP;
			vector<string> vecFileName;
			vector<vector<int>> vecBufNum;
			CExtractSVLFiles::ExtractBufferToAllSVLFiles(i_bufSize, i_bufP, vecFileName, &bufP, vecBufNum);
			if(1 != vecFileName.size())
				return STK_ERROR;
			//解压文件信息
			int bufsize = 0;
			int numBuf = vecBufNum[0].size();
			for (int i =0; i < numBuf; i++)
			{
				bufsize += vecBufNum[0][i];
			}
			//解析FileInfo文件
			status = CRapidJsonTools::ReadJsonData(bufP, vecBufNum[0], m_materialDoc);
		}
		break;
	}
	//初始化Bom结构
	InitJsonMaterialMap();
	InitJsonImageMap();
	return status;
}

STK_STATUS Stk_DocumentImpl::LoadMaterialFromBuf()
{
	m_mapIdToStkMaterial.clear();
	//从压缩文件中获取后缀名称为.lod的文件
	string strModelFileName;
	getFileNameBySuffix(".material", strModelFileName);
	CExtractSVLFiles extractSVLFiles;
	char **bufP;
	std::vector<int> vecBufNum;
	extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP,  strModelFileName, &bufP, vecBufNum);
	string strStream;
	int numBuf = vecBufNum.size();
	//int index= 0;
	UINT   niOffsetSize = 0;
	for (int i = 0; i < numBuf; i++)
	{
		strStream.append(bufP[i]);
		MidFree(bufP[i]);
		bufP[i] = NULL;
	}
	MidFree (bufP);
	bufP = NULL;
    #ifdef _WIN32
    
    #else
    strStream = CRapidJsonTools::RemoveExcess(strStream);
	#endif

    if (strStream.length() > 0) {
        string tab = "]}]}";
        if (strStream.find(tab) != string::npos) {
            strStream.erase(strStream.find(tab.c_str())+tab.length(), strStream.length());
        }
    }
    //printf("strStream 1 : %s",strStream.c_str());
    //解析文件结构
    m_materialDoc.Parse<0>(strStream.c_str());
    if (m_materialDoc.HasParseError()) {
        return STK_ERROR;
    }
    if (!m_materialDoc.IsObject()) {
        return STK_ERROR;
    }
    //初始化Bom结构
    InitJsonMaterialMap();
    InitJsonImageMap();
    return STK_SUCCESS;
}

STK_STATUS  Stk_DocumentImpl::UnloadMaterial()
{
	m_mapMaterialIDToMaterialNode.clear();

	map<int, Stk_MaterialPtr> mapIdToStkMaterial;
	m_mapIdToStkMaterial.swap(mapIdToStkMaterial);
	m_mapIdToStkMaterial.clear();

	m_mapMaterialIDToImageNode.clear();
	map<int, Stk_ImagePtr> mapIdToStkImage;
	m_mapIdToStkImage.swap(mapIdToStkImage);
	m_mapIdToStkImage.clear();
	return STK_SUCCESS;
}

STK_STATUS Stk_DocumentImpl::OpenRemote(const std::string& strIP, int port, 
	const std::string& userName, const std::string& projectCode, const std::string& fileName)
{
	
	return STK_SUCCESS;
}

Stk_ModelPtr Stk_DocumentImpl::GetModelByID( int ModelID)
{
	if (-1 == ModelID)
	{
		return nullptr;
	}
	//确认Part是否被添加
	map<int, Stk_ModelPtr>::iterator it;
	it = m_mapIdToStkModel.find(ModelID);
	if (it != m_mapIdToStkModel.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
	//创建model对象
	Stk_ModelPtr stkModel = Stk_ModelPtr::CreateObject();
	stkModel->SetID(ModelID);
	Value* valModel = GetJsonModel(ModelID);
	stkModel->BindModel(valModel);
	m_mapIdToStkModel.insert(make_pair(ModelID, stkModel));
	return stkModel;
}

Stk_InstancePtr Stk_DocumentImpl::GetInstanceByID(int iInstanceID)
{
	if (-1 == iInstanceID)
	{
		return nullptr;
	}
	map<int, Stk_InstancePtr>::iterator itorIdToInstanceMap;
	itorIdToInstanceMap = m_mapIdToInstance.find(iInstanceID);
	if (itorIdToInstanceMap == m_mapIdToInstance.end())
		return nullptr;
	else
		return itorIdToInstanceMap->second;
    return nullptr;
}

void Stk_DocumentImpl::SetOutputFullPath(const std::wstring& fullPath)
{
	if (fullPath.empty()) {
		return;
	}
#ifdef __MOBILE__
	int start = fullPath.find_last_of('/');
#else
	int start = fullPath.find_last_of('\\');
#endif
	start++;
	//int end = fullPath.find_last_of('.');
	int end = fullPath.length();

	if (-1 == start || -1 == end) {
		return;
	}

	m_outputFullPath = fullPath.substr(0, start);
	m_outputName = fullPath.substr(start, end - start);
	m_strOutputFullPath = ChineseCode::WStringToString(m_outputFullPath);
	m_strOutputName = ChineseCode::WStringToString(m_outputName);
	wstring wstrBin = m_outputName + L".bin";
	ChineseCode::UnicodeToUTF_8(wstrBin.c_str(), m_outputBinNameUTF8);
	m_wstroutputBinName = wstrBin;
}

std::wstring Stk_DocumentImpl::GetOutputFullPath()
{
	return m_outputFullPath;
}

std::wstring Stk_DocumentImpl::GetOutputName()
{
	return m_outputName;
}

const std::string& Stk_DocumentImpl::GetBinNameUTF8() {
	return m_outputBinNameUTF8;
}

unsigned int Stk_DocumentImpl::GetNbTopNode()
{
	Value::MemberIterator it = m_nodeDoc.FindMember("TopNodes");
	if ((it != m_nodeDoc.MemberEnd() && it->value.IsObject()))
	{
		Value* valueTopNode = &it->value;
		//获取顶级节点的个数
		return (unsigned int)valueTopNode->MemberCount();
	}
	return 0;
}

unsigned int Stk_DocumentImpl::GetTopNodeID( unsigned int iIdx )
{
	Value::MemberIterator it = m_nodeDoc.FindMember("TopNodes");
	if ((it != m_nodeDoc.MemberEnd() && it->value.IsObject()))
	{
		Value* valueTopNode = &it->value;
		string TopNodeID;
		int index = 0;
		//获取顶级节点的个数
		for (Value::MemberIterator topNodeIter = valueTopNode->MemberBegin();
			topNodeIter != valueTopNode->MemberEnd(); topNodeIter++)
		{
			if (iIdx == index)
			{
				TopNodeID = topNodeIter->name.GetString();
				return atoi(TopNodeID.c_str());
			}
			index++;
		}
	}
	return -1;
}

unsigned int Stk_DocumentImpl::GetNbTopInstance()
{
	return m_nodeDoc[BOM][ATTRIBUTES][TOPID].Size();
}

unsigned int Stk_DocumentImpl::GetTopInstanceID(unsigned int iIdx)
{
	return m_nodeDoc[BOM][ATTRIBUTES][TOPID][iIdx].GetInt();
}

vector<HoteamSoft::SVLLib::Stk_InstancePtr>& Stk_DocumentImpl::getTopInstances()
{
	return m_parentInstanceIdToStkInstance[-1];
}

Stk_MaterialPtr Stk_DocumentImpl::AddMaterial(unsigned int uiID /*= -1*/)
{
	if (!m_materialDoc.HasMember(MATERIALS))
	{
		Value valMaterials;
		valMaterials.SetArray();
		m_materialDoc.AddMember(MATERIALS, valMaterials, m_materialDoc.GetAllocator());
	}
	//创建材质对象
	Stk_MaterialPtr materialPtr = Stk_MaterialPtr::CreateObject();
	if (-1 != uiID) {
		materialPtr->SetID(uiID);
	}
	//创建json节点
	Value valMaterial;
	valMaterial.SetObject();
	valMaterial.AddMember("id", materialPtr->GetID(), m_materialDoc.GetAllocator());
	m_materialDoc[MATERIALS].PushBack(valMaterial, m_materialDoc.GetAllocator());
	//绑定json节点数据
	materialPtr->BindingMaterial(&m_materialDoc[MATERIALS][m_materialDoc[MATERIALS].Size()-1]);
	return materialPtr;
}

Stk_MaterialPtr Stk_DocumentImpl::FindMaterial(MaterialData materialData)
{
	Stk_MaterialPtr retMaterialPtr = nullptr;
	
	map<int, Stk_MaterialPtr>::iterator it;
	for (it = m_mapIdToStkMaterial.begin(); it != m_mapIdToStkMaterial.end(); it++)
	{
		float diffuseColor[3] = { 0 };
		float fTransparency = 0;
		Stk_MaterialPtr curMaterialPtr = it->second;
		if (curMaterialPtr->GetMaterialType() == STK_MATERIALTYPE_SIMPLAE) 
		{
			curMaterialPtr->GetDiffuseColor(diffuseColor);
			curMaterialPtr->GetTransparency(fTransparency);
			Stk_TexturePtr diffuseTexturePtr = curMaterialPtr->GetDiffuseTexture();

			if (fabs(fTransparency - materialData.fTransparency) < F_TOL &&
				fabs(diffuseColor[0] - materialData.DiffuseColor[0]) < F_TOL &&
				fabs(diffuseColor[1] - materialData.DiffuseColor[1]) < F_TOL &&
				fabs(diffuseColor[2] - materialData.DiffuseColor[2]) < F_TOL)
			{
				if (diffuseTexturePtr != nullptr)
				{
					if (diffuseTexturePtr->GetTextureImage() != nullptr)
					{
						if (diffuseTexturePtr->GetTextureImage()->GetPictureSavePath() == materialData.wstrDiffuseTextureImagePath)
						{
							retMaterialPtr = curMaterialPtr;
							break;
						}
						else
						{
							retMaterialPtr = nullptr;
							break;
						}
					}
					else
					{
						retMaterialPtr = curMaterialPtr;
						break;
					}
				}
				else if(materialData.wstrDiffuseTextureImagePath !=L"") 
				{
					retMaterialPtr = nullptr;
					break;
				}
				else
				{
					retMaterialPtr = curMaterialPtr;
					break;
				}
			}
		}
		else if(curMaterialPtr->GetMaterialType() == STK_MATERIALTYPE_JADE || 
			curMaterialPtr->GetMaterialType() == STK_MATERIALTYPE_PERAL ||
			curMaterialPtr->GetMaterialType() == STK_MATERIALTYPE_CRYSTAL||
			curMaterialPtr->GetMaterialType() == STK_MATERIALTYPE_METAL
			)
		{
			wstring curName = curMaterialPtr->GetMaterialName();
			if (curName == materialData.name)
			{
				retMaterialPtr = curMaterialPtr;
				break;
			}
		}
	}
	
	return retMaterialPtr;
}

Stk_ImagePtr Stk_DocumentImpl::AddImage(bool& bHasSet, const string& strImagePath, unsigned int uiID /*= -1*/)
{
	if (!m_materialDoc.HasMember(IMAGES))
	{
		Value valImages;
		valImages.SetArray();
		m_materialDoc.AddMember(IMAGES, valImages, m_materialDoc.GetAllocator());
	}

	//判断材质对象是否已被添加
	map<int, Stk_ImagePtr>::iterator it;
	it = m_mapIdToStkImage.find(uiID);
	if (it != m_mapIdToStkImage.end())
	{
		bHasSet = true;
		return it->second;
	}

	//创建材质对象
	Stk_ImagePtr imagePtr = Stk_ImagePtr::CreateObject();
	if (-1 != uiID) {
		imagePtr->SetID(uiID);
	}
	wstring wstrImagePath = L"";
	wstrImagePath = ChineseCode::StringToWString(strImagePath);
	imagePtr->SetPictureSavePath(wstrImagePath);
	//创建json节点
	Value valImage;
	valImage.SetObject();
	string strImagePathUTF8 = "";
	ChineseCode::UnicodeToUTF_8(wstrImagePath.c_str(), strImagePathUTF8);
	valImage.AddMember(URI, strImagePathUTF8, m_materialDoc.GetAllocator());
	char charID[32];
	sprintf(charID,"%d",imagePtr->GetID());
	//itoa(imagePtr->GetID(), charID, 10);
	string strID = charID;
	valImage.AddMember("id", imagePtr->GetID(), m_materialDoc.GetAllocator());	
	m_materialDoc[IMAGES].PushBack(valImage, m_materialDoc.GetAllocator());

	if (imagePtr.isNotNull())
	{
		m_mapIdToStkImage.insert(make_pair(imagePtr->GetID(), imagePtr));
	}
	return imagePtr;
}

STK_STATUS Stk_DocumentImpl::loadMeshFromBuff()
{
	//从压缩文件中获取后缀名称为.lod的文件
	string strMeshFileName;
	getFileNameBySuffix(".mesh", strMeshFileName);
	//从内存中解压文件
	CExtractSVLFiles extractSVLFiles;
	char **bufP;
	std::vector<int> vecBufNum;
	extractSVLFiles.ExtractBufferToSVLFiles(m_svlxBufferSize, m_bufP,  strMeshFileName, &bufP, vecBufNum);
	int numBuf = vecBufNum.size();
#ifdef __MOBILE__
	/************************************************************************/
	/* 将Mesh解压到内存中                                                                     */
	/************************************************************************/
	int bufsize = 0;
	for (int i =0; i < numBuf; i++)
	{
		bufsize += vecBufNum[i];
	}
	m_meshFilePtr->OpenAsBuffer(bufsize);
	//释放解压出来的内存
	for (int i = 0; i < numBuf; i++)
	{
		m_meshFilePtr->WriteBuffer(bufP[i], vecBufNum[i]);
		MidFree(bufP[i]);
		bufP[i] = NULL;
	}
	MidFree (bufP);
	bufP = NULL;
#else
	/************************************************************************/
	/*将Mesh文件解压到系统临时文件中                                                   */
	/************************************************************************/
	DWORD  dwBytesWritten, dwBufSize=BUFSIZE; 
	char szTempName[MAX_PATH];
	char lpPathBuffer[MAX_PATH]; 
	// 获取临时文件路径  
	GetTempPathA(MAX_PATH,    
		lpPathBuffer); 
	//创建临时文件
	GetTempFileNameA(lpPathBuffer, // 临时文件目录  
		strMeshFileName.c_str(),                    // 临时文件文的前缀  
		0,                        // 创建唯一的名字  
		szTempName);              // 保存名字的缓冲
	m_meshFilePtr->SetName(szTempName);
	m_meshFilePtr->OpenAsBin("wb"); 
	//将数据写入到文件中
	for (int i = 0; i < numBuf; i++)
	{
		m_meshFilePtr->WriteBuffer(bufP[i], vecBufNum[i]);
		MidFree(bufP[i]);
		bufP[i] = NULL;
	}
	MidFree (bufP);
	bufP = NULL;
	m_meshFilePtr->Close();
	m_meshFilePtr->OpenAsBin("rb"); 
#endif	
	
	m_hasExtractMeshData = true;
	return STK_SUCCESS;
}

Value* Stk_DocumentImpl::GetNode( unsigned int nodeID )
{
	char charID[32];
	sprintf(charID,"%d",nodeID);
    //itoa(nodeID, charID, 10);
	string strID = charID;
	//从顶级节点中查找
	Value::MemberIterator topnodeIter = m_nodeDoc.FindMember("TopNodes");
	if ((topnodeIter!= m_nodeDoc.MemberEnd() && topnodeIter->value.IsObject()))
	{
		Value* valueTopNode = &topnodeIter->value;
		Value::MemberIterator topnodeIDIter = valueTopNode->FindMember(strID.c_str());
		if ((topnodeIDIter!= valueTopNode->MemberEnd() && topnodeIDIter->value.IsObject()))
			return &topnodeIDIter->value;
	}
	//从Node节点中查找
	Value::MemberIterator nodeIter = m_nodeDoc.FindMember("Nodes");
	if ((nodeIter!= m_nodeDoc.MemberEnd() && nodeIter->value.IsObject()))
	{
		Value* valueNode = &nodeIter->value;
		Value::MemberIterator nodeIDIter = valueNode->FindMember(strID.c_str());
		if ((nodeIDIter!= valueNode->MemberEnd() && nodeIDIter->value.IsObject()))
			return &nodeIDIter->value;
	}
	return NULL;
}

void Stk_DocumentImpl::AddTopNodeID( unsigned int nodeID )
{
	//判断顶级Node节点是否存在
	Value valTopNode;
	Value valTopNodeRoot;
	Value valNodesRoot;
	valTopNode.SetObject();
	valTopNodeRoot.SetObject();
	valNodesRoot.SetObject();

	char charID[32];
	sprintf(charID,"%d",nodeID);
	//itoa(nodeID, charID, 10);
	string strID = charID;
	//创建基础根节点
	if (!m_nodeDoc.HasMember("TopNodes"))
	{
		m_nodeDoc.AddMember("TopNodes", valTopNodeRoot, m_nodeDoc.GetAllocator());
	}
	if (!m_nodeDoc.HasMember("Nodes"))
	{
		m_nodeDoc.AddMember("Nodes", valNodesRoot, m_nodeDoc.GetAllocator());
	}
	//插入顶级节点
	Value::MemberIterator topnodeIter = m_nodeDoc.FindMember("TopNodes");
	//添加名称类型信息
	valTopNode.AddMember("name", "default", m_nodeDoc.GetAllocator());
	valTopNode.AddMember("type", 0, m_nodeDoc.GetAllocator());
	if ((topnodeIter!= m_nodeDoc.MemberEnd() && topnodeIter->value.IsObject()))
	{
		Value key(StringRef(strID), m_nodeDoc.GetAllocator());
		topnodeIter->value.AddMember(key, valTopNode, m_nodeDoc.GetAllocator());
	}
}

void Stk_DocumentImpl::AddTopInstanceID(unsigned int instanceID)
{
	//判断顶级Node节点是否存在
	Value valBom;
	Value valInstances;
	Value valAttributes;
	valBom.SetObject();
	valInstances.SetArray();
	valAttributes.SetObject();

	char charID[32];
	sprintf(charID,"%d",instanceID);
	//itoa(instanceID, charID, 10);
	string strID = charID;
	//创建基础根节点
	if (!m_nodeDoc.HasMember(BOMVERSION))
	{
		m_nodeDoc.AddMember(BOMVERSION, VERSION_BOM, m_nodeDoc.GetAllocator());
	}
	if (!m_nodeDoc.HasMember(BOM))
	{
		m_nodeDoc.AddMember(BOM, valBom, m_nodeDoc.GetAllocator());
	}
	//创建Instances和Attribute节点
	Value::MemberIterator bomIter = m_nodeDoc.FindMember(BOM);
	if(!bomIter->value.HasMember(ATTRIBUTES))
		bomIter->value.AddMember(ATTRIBUTES, valAttributes, m_nodeDoc.GetAllocator());
	if(!bomIter->value.HasMember(INSTANCES))
		bomIter->value.AddMember(INSTANCES, valInstances, m_nodeDoc.GetAllocator());
	//创建数据对象
	Value valInstance;
	valInstance.SetArray();
	valInstance.PushBack(instanceID, m_nodeDoc.GetAllocator());		//id
	valInstance.PushBack(-1, m_nodeDoc.GetAllocator());				   //plcid
	valInstance.PushBack(-1, m_nodeDoc.GetAllocator());					//modelID
	valInstance.PushBack(1, m_nodeDoc.GetAllocator());			   //显隐状态
	valInstance.PushBack("", m_nodeDoc.GetAllocator());				   //配置矩阵
	Value valInstaceChildren;
	valInstaceChildren.SetArray();
	valInstance.PushBack(valInstaceChildren, m_nodeDoc.GetAllocator());	//子节点信息
	valInstance.PushBack(-1, m_nodeDoc.GetAllocator());				   //材质ID
	valInstance.PushBack(-1, m_nodeDoc.GetAllocator());				   //父节点ID
    bomIter->value[INSTANCES].PushBack(valInstance, m_nodeDoc.GetAllocator());
	//记录顶级实例的ID号
	if (!bomIter->value[ATTRIBUTES].HasMember(TOPID))
	{
		Value valTopIdArry;
		valTopIdArry.SetArray();
		bomIter->value[ATTRIBUTES].AddMember(TOPID, valTopIdArry, m_nodeDoc.GetAllocator());
	}
	bomIter->value[ATTRIBUTES][TOPID].PushBack(instanceID, m_nodeDoc.GetAllocator());
}

void Stk_DocumentImpl::InitJsonNodeMap()
{
	Value::MemberIterator topnodeRootIter = m_nodeDoc.FindMember("TopNodes");
	if ((topnodeRootIter!= m_nodeDoc.MemberEnd() && topnodeRootIter->value.IsObject()))
	{
		Value* valueTopNode = &topnodeRootIter->value;
		Value::MemberIterator topNodeIter = valueTopNode->MemberBegin();
		for ( ; topNodeIter != valueTopNode->MemberEnd(); ++topNodeIter)
		{
			string strNodeID = topNodeIter->name.GetString();
			int nodeID = atoi(strNodeID.c_str());
			m_mapNodeIdToStkNode.insert(make_pair(nodeID, &topNodeIter->value));
		}	
	}
	Value::MemberIterator nodeRootIter = m_nodeDoc.FindMember("Nodes");
	if ((nodeRootIter!= m_nodeDoc.MemberEnd() && nodeRootIter->value.IsObject()))
	{
		Value* valueNode = &nodeRootIter->value;
		Value::MemberIterator nodeIter = valueNode->MemberBegin();
		for ( ; nodeIter != valueNode->MemberEnd(); ++nodeIter)
		{
			string strNodeID = nodeIter->name.GetString();
			int nodeID = atoi(strNodeID.c_str());
			m_mapNodeIdToStkNode.insert(make_pair(nodeID, &nodeIter->value));
		}
	}
}

void Stk_DocumentImpl::InitJsonPartMap()
{
	Value::MemberIterator partRootIter = m_nodeDoc.FindMember("Part");
	if ((partRootIter!= m_nodeDoc.MemberEnd() && partRootIter->value.IsObject()))
	{
		Value* valPartRoot = &partRootIter->value;
		Value::MemberIterator partIter = valPartRoot->MemberBegin();
		for ( ; partIter != valPartRoot->MemberEnd(); ++partIter)
		{
			string strNodeID = partIter->name.GetString();
			int nodeID = atoi(strNodeID.c_str());
			m_mapPartIDToPartNode.insert(make_pair(nodeID, &partIter->value));
		}
	}
}

void Stk_DocumentImpl::InitJsonInstanceMap()
{
	Value::MemberIterator bomIter = m_nodeDoc.FindMember(BOM);
	if ((bomIter!= m_nodeDoc.MemberEnd() && bomIter->value.IsObject()))
	{
		Value* valBom = &bomIter->value;
		Value::MemberIterator instancesIter = valBom->FindMember(INSTANCES);
		if (instancesIter != valBom->MemberEnd())
		{
			Value* valInstanaces = &instancesIter->value;

			for(int i = 0; i < valInstanaces->Size(); i++)
			{
				int instanceID = (*valInstanaces)[i][INDEX_INSTANCEID].GetInt();
				m_mapNodeIdToInstanceNode.insert(make_pair(instanceID, &(*valInstanaces)[i]));
			}
		}
	}
}

void Stk_DocumentImpl::InitJsonModelMap()
{
	Value::MemberIterator modelsIter = m_modelDoc.FindMember(MODELS);
	if ((modelsIter!= m_modelDoc.MemberEnd() && modelsIter->value.IsArray()))
	{
		Value* valModel = &modelsIter->value;
		for(int i = 0; i < valModel->Size(); i++)
		{
			int modelID = (*valModel)[i]["id"].GetInt();
			m_mapModelIDToModelNode.insert(make_pair(modelID, &(*valModel)[i]));
		}
	}
}

void Stk_DocumentImpl::InitJsonMaterialMap()
{
	if(m_materialDoc.HasMember(MATERIALS))
	{
		int sizeMaterial = m_materialDoc[MATERIALS].Size();
		for (int i = 0; i < sizeMaterial; i++)
		{
			Stk_MaterialPtr materialPtr = Stk_MaterialPtr::CreateObject();
			int materialID = m_materialDoc[MATERIALS][i]["id"].GetInt();
			materialPtr->SetID(materialID);
			m_mapMaterialIDToMaterialNode.insert(make_pair(materialID, &m_materialDoc[MATERIALS][i]));
			m_mapIdToStkMaterial.insert(make_pair(materialID, materialPtr));
		}
	}
}

void Stk_DocumentImpl::InitJsonImageMap()
{
	if (!m_materialDoc.HasMember(IMAGES))
		return;
	Value* imagesValue = &m_materialDoc[IMAGES];
	int length = imagesValue->Size();
	for (int i = 0; i<length;i++)
	{
		Value* imageValue = &((*imagesValue)[i]);
		Stk_ImagePtr imagelPtr = Stk_ImagePtr::CreateObject();
		wstring wstrImagePath = ChineseCode::UTF_8ToUnicode((*imageValue)[URI].GetString());
		imagelPtr->SetID((*imageValue)["id"].GetInt());
		imagelPtr->SetPictureSavePath(wstrImagePath);
		m_mapMaterialIDToImageNode.insert(make_pair(imagelPtr->GetID(), imageValue));
		m_mapIdToStkImage.insert(make_pair(imagelPtr->GetID(), imagelPtr));
	}
}

void Stk_DocumentImpl::initBinFile()
{
	string strFilePath;
	strFilePath = ChineseCode::WStringToString(m_outputFullPath);
	m_resourceManagerPtr = Stk_ResourceManagerPtr::CreateObject();
	m_resourceManagerPtr->SetWorkingDir(strFilePath);
}

void Stk_DocumentImpl::InitViewFile()
{
	wstring strViewFilePath = m_outputFullPath + L".view";
	m_pViewDoc->OpenFile(strViewFilePath);
}

void Stk_DocumentImpl::InitPMIFile()
{
	wstring strPMIFilePath = m_outputFullPath + L".PMI";
	m_pPMIDoc->OpenFile(strPMIFilePath);
}

void Stk_DocumentImpl::InitAttrFile()
{
	wstring strAttrFilePath = m_outputFullPath + L".model";
	m_pAttrDoc->OpenFile(strAttrFilePath);
}
void Stk_DocumentImpl::InitModel()
{
	//创建MeshBlock
	Stk_ModelBlock* pStkModelBlock = new Stk_ModelBlock();
	//获取文件信息
	wstring wstrModelFilePath;
	string strModelFilePath;
	wstrModelFilePath = /*m_outputFullPath +*/ m_outputName + L".lod";
	strModelFilePath = ChineseCode::WStringToString(wstrModelFilePath);
	//获取资源管理器
	Stk_FilePtr stk_filePtr = m_resourceManagerPtr->GetFile(strModelFilePath);
	stk_filePtr->OpenAsBin("rb"); 
	Stk_CommonAccessorPtr stkMeshAccessor = Stk_CommonAccessorPtr::CreateObject();
	pStkModelBlock->ReadFile(stk_filePtr);
	//获取Model块的相关信息
	int numModel = pStkModelBlock->getModelBlockCount();
	int accessorBlockOffset = 0;
	for (int i = 0; i < numModel; i++)
	{
		StkModelBlock stkModelBlock = pStkModelBlock->getModelBlockByIdx(i);
		//创建Model对象
		map<int, Stk_ModelPtr>::iterator it;
		it = m_mapIdToStkModel.find(stkModelBlock.modelID);
		if (it == m_mapIdToStkModel.end())
		{
			continue;
		}
		Stk_ModelPtr stkModel = Stk_ModelPtr::CreateObject();
		stkModel->SetID(stkModelBlock.modelID);
		stkModel->BindModelBlock(&stkModelBlock);
		int numAccessorBlock = stkModelBlock.numLod;
		for (int j =0; j < numAccessorBlock; j++)
		{
			StkAccessorBlock stkAccessorBlock = pStkModelBlock->getAccessorBlockByIdx(accessorBlockOffset + j);
			stkModel->BindLodAccessorBlock(&stkAccessorBlock);
		}
		accessorBlockOffset += numAccessorBlock;
		m_mapIdToStkModel.insert(make_pair(stkModelBlock.modelID, stkModel));
	}
	delete pStkModelBlock;
}

bool Stk_DocumentImpl::getFileNameBySuffix(const string& i_strSuffix, string& o_strFileName)
{
	bool bFindResult = false;
	string strSuffix = i_strSuffix;
	transform(strSuffix.begin(), strSuffix.end(), strSuffix.begin(), ::tolower);
	o_strFileName = "";
	int numFileName = m_vecFileName.size();
	for (int i = 0; i < numFileName; i++)
	{
		string strTempFileName = m_vecFileName[i];
		int pos = strTempFileName.find_last_of(".");
		if(pos == string::npos)
		{
			o_strFileName = m_vecFileName[i];
			continue;
		}
		strTempFileName = strTempFileName.substr(strTempFileName.find_last_of("."), strTempFileName.length() - strTempFileName.find_last_of("."));
		transform(strTempFileName.begin(), strTempFileName.end(), strTempFileName.begin(), ::tolower);
		if (strSuffix == strTempFileName)
		{
			o_strFileName = m_vecFileName[i];
			bFindResult = true;
			break;
		}
	}
	return bFindResult;
}

Value* Stk_DocumentImpl::GetJsonNode(unsigned int nodeID)
{
	map<int, Value*>::iterator it;
	it = m_mapNodeIdToStkNode.find(nodeID);
	if (m_mapNodeIdToStkNode.end() == it)
	{
		return NULL;
	} 
	else
	{
		return it->second;
	}
}

Value* Stk_DocumentImpl::GetJsonPart(unsigned int partID)
{
	map<int, Value*>::iterator it;
	it = m_mapPartIDToPartNode.find(partID);
	if (m_mapPartIDToPartNode.end() == it)
	{
		return NULL;
	} 
	else
	{
		return it->second;
	}
}

Value* Stk_DocumentImpl::GetJsonModel(unsigned int modelID)
{
	map<int, Value*>::iterator it;
	it = m_mapModelIDToModelNode.find(modelID);
	if (m_mapModelIDToModelNode.end() == it)
	{
		return NULL;
	} 
	else
	{
		return it->second;
	}
}

Value* Stk_DocumentImpl::GetJsonInstance(unsigned int instanceID)
{
	map<int, Value*>::iterator it;
	it = m_mapNodeIdToInstanceNode.find(instanceID);
	if (m_mapNodeIdToInstanceNode.end() == it)
	{
		return NULL;
	} 
	else
	{
		return it->second;
	}
}

Value* Stk_DocumentImpl::GetJsonMaterial(unsigned int materialID)
{
	map<int, Value*>::iterator it;
	it = m_mapMaterialIDToMaterialNode.find(materialID);
	if (m_mapMaterialIDToMaterialNode.end() == it)
	{
		return NULL;
	} 
	else
	{
		return it->second;
	}
}

Stk_GeometryAttributePtr  Stk_DocumentImpl::GetMeshFaceAttribute(int iModelID, int iMeshFaceID)
{
	Stk_GeometryAttributePtr pGeoAttr = nullptr;
	if (m_pGeoAttrDoc == nullptr)
		return pGeoAttr;
	wstring wstrValue;
	string strValue;
	vector<std::string> vecValue;

	wstring wstrGeoType = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_TYPE);
	if (wcscmp(wstrGeoType.c_str(), L"1") == 0)
	{
		pGeoAttr = Stk_PlaneFaceAttributePtr::CreateObject();
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_NORMAL);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirNormal;
		if (vecValue.size() == 3)
		{
			dirNormal.X = atof(vecValue[0].c_str());
			dirNormal.Y = atof(vecValue[1].c_str());
			dirNormal.Z = atof(vecValue[2].c_str());
			Stk_PlaneFaceAttributePtr::DynamicCast(pGeoAttr)->SetNormal(dirNormal);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_ORIGIN);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntOrigin;
		if (vecValue.size() == 3)
		{
			pntOrigin.X = atof(vecValue[0].c_str());
			pntOrigin.Y = atof(vecValue[1].c_str());
			pntOrigin.Z = atof(vecValue[2].c_str());
			Stk_PlaneFaceAttributePtr::DynamicCast(pGeoAttr)->SetOrigin(pntOrigin);
		}
	}
	else if (wcscmp(wstrGeoType.c_str(), L"2") == 0 || wcscmp(wstrGeoType.c_str(), L"3") == 0
		|| wcscmp(wstrGeoType.c_str(), L"4") == 0 || wcscmp(wstrGeoType.c_str(), L"5") == 0)
	{
		if(wcscmp(wstrGeoType.c_str(), L"2") == 0)
			pGeoAttr = Stk_RevolutionFaceAttributePtr::CreateObject();
		else if(wcscmp(wstrGeoType.c_str(), L"3") == 0)
			pGeoAttr = Stk_CylinderFaceAttributePtr::CreateObject();
		else if (wcscmp(wstrGeoType.c_str(), L"4") == 0)
			pGeoAttr = Stk_ConeFaceAttributePtr::CreateObject();
		else if (wcscmp(wstrGeoType.c_str(), L"5") == 0)
			pGeoAttr = Stk_SphereFaceAttributePtr::CreateObject();
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISORIGIN);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntAxisOrigin;
		if (vecValue.size() == 3)
		{
			pntAxisOrigin.X = atof(vecValue[0].c_str());
			pntAxisOrigin.Y = atof(vecValue[1].c_str());
			pntAxisOrigin.Z = atof(vecValue[2].c_str());
			if (wcscmp(wstrGeoType.c_str(), L"2") == 0)
				Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			else if (wcscmp(wstrGeoType.c_str(), L"3") == 0)
				Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			if (wcscmp(wstrGeoType.c_str(), L"4") == 0)
				Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			if (wcscmp(wstrGeoType.c_str(), L"5") == 0)
				Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirRevoAxis;
		if (vecValue.size() == 3)
		{
			dirRevoAxis.X = atof(vecValue[0].c_str());
			dirRevoAxis.Y = atof(vecValue[1].c_str());
			dirRevoAxis.Z = atof(vecValue[2].c_str());
			if (wcscmp(wstrGeoType.c_str(), L"2") == 0)
				Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
			else if (wcscmp(wstrGeoType.c_str(), L"3") == 0)
				Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
			else if (wcscmp(wstrGeoType.c_str(), L"4") == 0)
				Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
			else if (wcscmp(wstrGeoType.c_str(), L"5") == 0)
				Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_RADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		if (wcscmp(wstrGeoType.c_str(), L"2") == 0)
			Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
		else if (wcscmp(wstrGeoType.c_str(), L"3") == 0)
			Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
		else if (wcscmp(wstrGeoType.c_str(), L"4") == 0)
			Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
		else if (wcscmp(wstrGeoType.c_str(), L"5") == 0)
			Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
	}
	else if (wcscmp(wstrGeoType.c_str(), L"6") == 0)
	{
		pGeoAttr = Stk_ToroidalFaceAttributePtr::CreateObject();
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISORIGIN);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntAxisOrigin;
		if (vecValue.size() == 3)
		{
			pntAxisOrigin.X = atof(vecValue[0].c_str());
			pntAxisOrigin.Y = atof(vecValue[1].c_str());
			pntAxisOrigin.Z = atof(vecValue[2].c_str());
			Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirRevoAxis;
		if (vecValue.size() == 3)
		{
			dirRevoAxis.X = atof(vecValue[0].c_str());
			dirRevoAxis.Y = atof(vecValue[1].c_str());
			dirRevoAxis.Z = atof(vecValue[2].c_str());
			Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_ORIGINRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_MINORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetMinorRadius(atof(strValue.c_str()));
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_MAJORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetMajorRaius(atof(strValue.c_str()));
	}
	else if (wcscmp(wstrGeoType.c_str(), L"7") == 0)
	{
		pGeoAttr = Stk_LineAttributePtr::CreateObject();
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_CENTEREPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntCenterPoint, pntStartPoint, pntEndPoint;
		if (vecValue.size() == 3)
		{
			pntCenterPoint.X = atof(vecValue[0].c_str());
			pntCenterPoint.Y = atof(vecValue[1].c_str());
			pntCenterPoint.Z = atof(vecValue[2].c_str());
			Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetCenterPoint(pntCenterPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_DIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirDirection;
		if (vecValue.size() == 3)
		{
			dirDirection.X = atof(vecValue[0].c_str());
			dirDirection.Y = atof(vecValue[1].c_str());
			dirDirection.Z = atof(vecValue[2].c_str());
			Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetDirection(dirDirection);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_STARTPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			pntStartPoint.X = atof(vecValue[0].c_str());
			pntStartPoint.Y = atof(vecValue[1].c_str());
			pntStartPoint.Z = atof(vecValue[2].c_str());
			Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetStartPoint(pntStartPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_ENDPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			pntEndPoint.X = atof(vecValue[0].c_str());
			pntEndPoint.Y = atof(vecValue[1].c_str());
			pntEndPoint.Z = atof(vecValue[2].c_str());
			Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetEndPoint(pntEndPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_LENGTH);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetLength(atof(strValue.c_str()));
	}
	if (wcscmp(wstrGeoType.c_str(), L"8") == 0)
	{
		pGeoAttr = Stk_EllipseAttributePtr::CreateObject();
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_CENTEREPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntCenterPoint, pntStartPoint, pntEndPoint;
		if (vecValue.size() == 3)
		{
			pntCenterPoint.X = atof(vecValue[0].c_str());
			pntCenterPoint.Y = atof(vecValue[1].c_str());
			pntCenterPoint.Z = atof(vecValue[2].c_str());
			Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetCenterPoint(pntCenterPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_STARTPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			pntStartPoint.X = atof(vecValue[0].c_str());
			pntStartPoint.Y = atof(vecValue[1].c_str());
			pntStartPoint.Z = atof(vecValue[2].c_str());
			Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetStartPoint(pntStartPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_ENDPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			pntEndPoint.X = atof(vecValue[0].c_str());
			pntEndPoint.Y = atof(vecValue[1].c_str());
			pntEndPoint.Z = atof(vecValue[2].c_str());
			Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetEndPoint(pntEndPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_MINORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetMinorRadius(atof(strValue.c_str()));
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_MAJORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetMajorRadius(atof(strValue.c_str()));
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_XDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		bool bFlag = true;
		STK_DIR32 xDirection, yDirection, zDirection;
		if (vecValue.size() == 3)
		{
			xDirection.X = atof(vecValue[0].c_str());
			xDirection.Y = atof(vecValue[1].c_str());
			xDirection.Z = atof(vecValue[2].c_str());
			vecValue.clear();
		}
		else
			bFlag = false;
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_YDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			yDirection.X = atof(vecValue[0].c_str());
			yDirection.Y = atof(vecValue[1].c_str());
			yDirection.Z = atof(vecValue[2].c_str());
			vecValue.clear();
		}
		else
			bFlag = false;
		wstrValue = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, MESH_ZDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			zDirection.X = atof(vecValue[0].c_str());
			zDirection.Y = atof(vecValue[1].c_str());
			zDirection.Z = atof(vecValue[2].c_str());
		}
		else
			bFlag = false;
		if(bFlag)
			Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetXYZDir(xDirection, yDirection, zDirection);
	}

	return pGeoAttr;
}

Stk_GeometryAttributePtr  Stk_DocumentImpl::GetMeshEdgeAttribute(int iModelID, int iMeshFaceID)
{
	Stk_GeometryAttributePtr pGeoAttr = nullptr;
	if (m_pGeoAttrDoc == nullptr)
		return pGeoAttr;
	wstring wstrValue;
	string strValue;
	vector<std::string> vecValue;

	wstring wstrGeoType = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_TYPE);
	if (wcscmp(wstrGeoType.c_str(), L"1") == 0)
	{
		pGeoAttr = Stk_PlaneFaceAttributePtr::CreateObject();
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_NORMAL);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirNormal;
		if (vecValue.size() == 3)
		{
			dirNormal.X = atof(vecValue[0].c_str());
			dirNormal.Y = atof(vecValue[1].c_str());
			dirNormal.Z = atof(vecValue[2].c_str());
			Stk_PlaneFaceAttributePtr::DynamicCast(pGeoAttr)->SetNormal(dirNormal);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_ORIGIN);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntOrigin;
		if (vecValue.size() == 3)
		{
			pntOrigin.X = atof(vecValue[0].c_str());
			pntOrigin.Y = atof(vecValue[1].c_str());
			pntOrigin.Z = atof(vecValue[2].c_str());
			Stk_PlaneFaceAttributePtr::DynamicCast(pGeoAttr)->SetOrigin(pntOrigin);
		}
	}
	else if (wcscmp(wstrGeoType.c_str(), L"2") == 0 || wcscmp(wstrGeoType.c_str(), L"3") == 0
		|| wcscmp(wstrGeoType.c_str(), L"4") == 0 || wcscmp(wstrGeoType.c_str(), L"5") == 0)
	{
		if (wcscmp(wstrGeoType.c_str(), L"2") == 0)
			pGeoAttr = Stk_RevolutionFaceAttributePtr::CreateObject();
		else if (wcscmp(wstrGeoType.c_str(), L"3") == 0)
			pGeoAttr = Stk_CylinderFaceAttributePtr::CreateObject();
		else if (wcscmp(wstrGeoType.c_str(), L"4") == 0)
			pGeoAttr = Stk_ConeFaceAttributePtr::CreateObject();
		else if (wcscmp(wstrGeoType.c_str(), L"5") == 0)
			pGeoAttr = Stk_SphereFaceAttributePtr::CreateObject();
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_AXISORIGIN);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntAxisOrigin;
		if (vecValue.size() == 3)
		{
			pntAxisOrigin.X = atof(vecValue[0].c_str());
			pntAxisOrigin.Y = atof(vecValue[1].c_str());
			pntAxisOrigin.Z = atof(vecValue[2].c_str());
			if (wcscmp(wstrGeoType.c_str(), L"2") == 0)
				Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			else if (wcscmp(wstrGeoType.c_str(), L"3") == 0)
				Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			if (wcscmp(wstrGeoType.c_str(), L"4") == 0)
				Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			if (wcscmp(wstrGeoType.c_str(), L"5") == 0)
				Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_AXISDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirRevoAxis;
		if (vecValue.size() == 3)
		{
			dirRevoAxis.X = atof(vecValue[0].c_str());
			dirRevoAxis.Y = atof(vecValue[1].c_str());
			dirRevoAxis.Z = atof(vecValue[2].c_str());
			if (wcscmp(wstrGeoType.c_str(), L"2") == 0)
				Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
			else if (wcscmp(wstrGeoType.c_str(), L"3") == 0)
				Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
			else if (wcscmp(wstrGeoType.c_str(), L"4") == 0)
				Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
			else if (wcscmp(wstrGeoType.c_str(), L"5") == 0)
				Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_RADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		if (wcscmp(wstrGeoType.c_str(), L"2") == 0)
			Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
		else if (wcscmp(wstrGeoType.c_str(), L"3") == 0)
			Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
		else if (wcscmp(wstrGeoType.c_str(), L"4") == 0)
			Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
		else if (wcscmp(wstrGeoType.c_str(), L"5") == 0)
			Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
	}
	else if (wcscmp(wstrGeoType.c_str(), L"6") == 0)
	{
		pGeoAttr = Stk_ToroidalFaceAttributePtr::CreateObject();
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_AXISORIGIN);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntAxisOrigin;
		if (vecValue.size() == 3)
		{
			pntAxisOrigin.X = atof(vecValue[0].c_str());
			pntAxisOrigin.Y = atof(vecValue[1].c_str());
			pntAxisOrigin.Z = atof(vecValue[2].c_str());
			Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetAxisOrigin(pntAxisOrigin);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_AXISDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirRevoAxis;
		if (vecValue.size() == 3)
		{
			dirRevoAxis.X = atof(vecValue[0].c_str());
			dirRevoAxis.Y = atof(vecValue[1].c_str());
			dirRevoAxis.Z = atof(vecValue[2].c_str());
			Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetRevoAxis(dirRevoAxis);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_ORIGINRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetOriginRadius(atof(strValue.c_str()));
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_MINORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetMinorRadius(atof(strValue.c_str()));
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_MAJORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->SetMajorRaius(atof(strValue.c_str()));
	}
	else if (wcscmp(wstrGeoType.c_str(), L"7") == 0)
	{
		pGeoAttr = Stk_LineAttributePtr::CreateObject();
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_CENTEREPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntCenterPoint, pntStartPoint, pntEndPoint;
		if (vecValue.size() == 3)
		{
			pntCenterPoint.X = atof(vecValue[0].c_str());
			pntCenterPoint.Y = atof(vecValue[1].c_str());
			pntCenterPoint.Z = atof(vecValue[2].c_str());
			Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetCenterPoint(pntCenterPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_DIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_DIR32 dirDirection;
		if (vecValue.size() == 3)
		{
			dirDirection.X = atof(vecValue[0].c_str());
			dirDirection.Y = atof(vecValue[1].c_str());
			dirDirection.Z = atof(vecValue[2].c_str());
			Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetDirection(dirDirection);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_STARTPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			pntStartPoint.X = atof(vecValue[0].c_str());
			pntStartPoint.Y = atof(vecValue[1].c_str());
			pntStartPoint.Z = atof(vecValue[2].c_str());
			Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetStartPoint(pntStartPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_ENDPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			pntEndPoint.X = atof(vecValue[0].c_str());
			pntEndPoint.Y = atof(vecValue[1].c_str());
			pntEndPoint.Z = atof(vecValue[2].c_str());
			Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetEndPoint(pntEndPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_LENGTH);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_LineAttributePtr::DynamicCast(pGeoAttr)->SetLength(atof(strValue.c_str()));
	}
	if (wcscmp(wstrGeoType.c_str(), L"8") == 0)
	{
		pGeoAttr = Stk_EllipseAttributePtr::CreateObject();
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_CENTEREPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		STK_PNT32 pntCenterPoint, pntStartPoint, pntEndPoint;
		if (vecValue.size() == 3)
		{
			pntCenterPoint.X = atof(vecValue[0].c_str());
			pntCenterPoint.Y = atof(vecValue[1].c_str());
			pntCenterPoint.Z = atof(vecValue[2].c_str());
			Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetCenterPoint(pntCenterPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_STARTPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			pntStartPoint.X = atof(vecValue[0].c_str());
			pntStartPoint.Y = atof(vecValue[1].c_str());
			pntStartPoint.Z = atof(vecValue[2].c_str());
			Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetStartPoint(pntStartPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_ENDPOINT);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			pntEndPoint.X = atof(vecValue[0].c_str());
			pntEndPoint.Y = atof(vecValue[1].c_str());
			pntEndPoint.Z = atof(vecValue[2].c_str());
			Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetEndPoint(pntEndPoint);
			vecValue.clear();
		}
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_MINORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetMinorRadius(atof(strValue.c_str()));
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_MAJORRADIUS);
		strValue = ChineseCode::WStringToString(wstrValue);
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetMajorRadius(atof(strValue.c_str()));
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_XDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		bool bFlag = true;
		STK_DIR32 xDirection, yDirection, zDirection;
		if (vecValue.size() == 3)
		{
			xDirection.X = atof(vecValue[0].c_str());
			xDirection.Y = atof(vecValue[1].c_str());
			xDirection.Z = atof(vecValue[2].c_str());
			vecValue.clear();
		}
		else
			bFlag = false;
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_YDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			yDirection.X = atof(vecValue[0].c_str());
			yDirection.Y = atof(vecValue[1].c_str());
			yDirection.Z = atof(vecValue[2].c_str());
			vecValue.clear();
		}
		else
			bFlag = false;
		wstrValue = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshFaceID, MESH_ZDIRECTION);
		strValue = ChineseCode::WStringToString(wstrValue);
		CStringTools::StringSplit(strValue, " ", vecValue);
		if (vecValue.size() == 3)
		{
			zDirection.X = atof(vecValue[0].c_str());
			zDirection.Y = atof(vecValue[1].c_str());
			zDirection.Z = atof(vecValue[2].c_str());
		}
		else
			bFlag = false;
		if (bFlag)
			Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->SetXYZDir(xDirection, yDirection, zDirection);
	}

	return pGeoAttr;
}

map<wstring, wstring> Stk_DocumentImpl::GetMeshFaceAllAttribute(int iModelID, int iMeshFaceID)
{
	map<wstring, wstring> mapAttribute;
	if (m_pAttrDoc == nullptr)
		return mapAttribute;
	mapAttribute = m_pGeoAttrDoc->GetMeshFaceAllAttribute(iModelID, iMeshFaceID);
	return mapAttribute;
}

map<wstring, wstring> Stk_DocumentImpl::GetMeshEdgeAllAttribute(int iModelID, int iMeshEdgeID)
{
	map<wstring, wstring> mapAttribute;
	if (m_pAttrDoc == nullptr)
		return mapAttribute;
	mapAttribute = m_pGeoAttrDoc->GetMeshEdgeAllAttribute(iModelID, iMeshEdgeID);
	return mapAttribute;
}

wstring Stk_DocumentImpl::GetMeshFaceAttribute(int iModelID, int iMeshFaceID, const std::wstring& i_wstrAttrName)
{
	wstring wstrName = L"";
	if (m_pAttrDoc == nullptr)
		return wstrName;
	wstrName = m_pGeoAttrDoc->GetMeshFaceAttribute(iModelID, iMeshFaceID, i_wstrAttrName);
	return wstrName;
}

STK_STATUS Stk_DocumentImpl::AddMeshFaceAttribute(int iModelID, int iMeshFaceID, const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue)
{
	if (m_pAttrDoc == nullptr)
		return STK_ERROR;
	return m_pGeoAttrDoc->AddMeshFaceAttribute(iModelID, iMeshFaceID, i_wstrAttrName, i_wstrAttrValue);
}

STK_STATUS  Stk_DocumentImpl::AddMeshFaceAttribute(int iModelID, int iMeshFaceID, Stk_GeometryAttributePtr pGeoAttr)
{
	if (pGeoAttr == nullptr)
		return STK_ERROR;

	StkGeoAttrTypeEnum geoAttrType = pGeoAttr->GetGeoAttrType();
	if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_PLANEFACE)
	{
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_TYPE, L"1");
		STK_DIR32 srcNormal = Stk_PlaneFaceAttributePtr::DynamicCast(pGeoAttr)->GetNormal();
		string strValue;
		char szValue[32] = { 0 };
		//normal
		sprintf(szValue, "%.8f", srcNormal.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcNormal.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcNormal.Z);
		strValue += szValue;
		wstring wstrValue = ChineseCode::StringToWString(strValue);
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_NORMAL, wstrValue);
		STK_PNT32 srcOrigin = Stk_PlaneFaceAttributePtr::DynamicCast(pGeoAttr)->GetOrigin();
		//origin
		strValue.clear();
		sprintf(szValue, "%.8f", srcOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcOrigin.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_ORIGIN, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_REVOLUTIONFACE)
	{
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_TYPE, L"2");
		STK_PNT32 srcAxisOrigin = Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISORIGIN, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->GetRevoAxis();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		//radius
		sprintf(szValue, "%.8f", Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->GetOriginRadius());
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_RADIUS, ChineseCode::StringToWString(strValue));
	}
	else if(geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_CYLINDERFACE)
	{
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_TYPE, L"3");
		STK_PNT32 srcAxisOrigin = Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISORIGIN, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->GetRevoAxis();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		//radius
		sprintf(szValue, "%.8f", Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->GetOriginRadius());
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_RADIUS, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_CONICALFACE)
	{
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_TYPE, L"4");
		STK_PNT32 srcAxisOrigin = Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISORIGIN, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->GetRevoAxis();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		//radius
		sprintf(szValue, "%.8f", Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->GetOriginRadius());
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_RADIUS, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_SPHEREFACE)
	{
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_TYPE, L"5");
		STK_PNT32 srcAxisOrigin = Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISORIGIN, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->GetRevoAxis();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		//radius
		sprintf(szValue, "%.8f", Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->GetOriginRadius());
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_RADIUS, ChineseCode::StringToWString(strValue));
	}
	else if(geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_TOROIDALFACE)
	{
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_TYPE, L"6");
		STK_PNT32 srcAxisOrigin = Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISORIGIN, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->GetRevoAxis();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_AXISDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->GetOriginRadius());
		strValue += szValue;
		//originRadius
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_ORIGINRADIUS, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->GetMajorRadius());
		strValue += szValue;
		//majorRadius
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_MAJORRADIUS, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->GetMinorRadius());
		strValue += szValue;
		//minorRadius
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_MINORRADIUS, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_LINE)
	{
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_TYPE, L"7");
		string strValue;
		char szValue[32] = { 0 };
		STK_PNT32 srcCenterPoint = Stk_LineAttributePtr::DynamicCast(pGeoAttr)->GetCenterPoint();
		sprintf(szValue, "%.8f", srcCenterPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_CENTEREPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcStartPoint = Stk_LineAttributePtr::DynamicCast(pGeoAttr)->GetStartPoint();
		sprintf(szValue, "%.8f", srcStartPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_STARTPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcEndPoint = Stk_LineAttributePtr::DynamicCast(pGeoAttr)->GetEndPoint();
		sprintf(szValue, "%.8f", srcEndPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_ENDPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_LineAttributePtr::DynamicCast(pGeoAttr)->GetDirection();
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_DIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", Stk_LineAttributePtr::DynamicCast(pGeoAttr)->GetLength());
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_LENGTH, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_ELLIPSE)
	{
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_TYPE, L"8");
		string strValue;
		char szValue[32] = { 0 };
		STK_PNT32 srcCenterPoint = Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetCenterPoint();
		sprintf(szValue, "%.8f", srcCenterPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_CENTEREPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcStartPoint = Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetStartPoint();
		sprintf(szValue, "%.8f", srcStartPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_STARTPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcEndPoint = Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetEndPoint();
		sprintf(szValue, "%.8f", srcEndPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_ENDPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetMajorRadius());
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_MAJORRADIUS, ChineseCode::StringToWString(strValue));
		sprintf(szValue, "%.8f", Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetMinorRadius());
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_MINORRADIUS, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcXDir, srcYDir, srcZDir;
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetXYZDir(srcXDir, srcYDir, srcZDir);
		sprintf(szValue, "%.8f", srcXDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcXDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcXDir.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_XDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear(); sprintf(szValue, "%.8f", srcYDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcYDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcYDir.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_YDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", srcZDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcZDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcZDir.Z);
		strValue += szValue;
		AddMeshFaceAttribute(iModelID, iMeshFaceID, MESH_ZDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
	}

	return STK_SUCCESS;
}

wstring Stk_DocumentImpl::GetMeshEdgeAttribute(int iModelID, int iMeshEdgeID, const std::wstring& i_wstrAttrName)
{
	wstring wstrName = L"";
	if (m_pAttrDoc == nullptr)
		return wstrName;
	wstrName = m_pGeoAttrDoc->GetMeshEdgeAttribute(iModelID, iMeshEdgeID, i_wstrAttrName);
	return wstrName;
}

STK_STATUS Stk_DocumentImpl::AddMeshEdgeAttribute(int iModelID, int iMeshEdgeID, const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue)
{
	if (m_pAttrDoc == nullptr)
		return STK_ERROR;
	return m_pGeoAttrDoc->AddMeshEdgeAttribute(iModelID, iMeshEdgeID, i_wstrAttrName, i_wstrAttrValue);
}

STK_STATUS  Stk_DocumentImpl::AddMeshEdgeAttribute(int iModelID, int iMeshEdgeID, Stk_GeometryAttributePtr pGeoAttr)
{
	if (m_pAttrDoc == nullptr)
		return STK_ERROR;
	StkGeoAttrTypeEnum geoAttrType = pGeoAttr->GetGeoAttrType();
	if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_PLANEFACE)
	{
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_TYPE, L"1");
		STK_DIR32 srcNormal = Stk_PlaneFaceAttributePtr::DynamicCast(pGeoAttr)->GetNormal();
		string strValue;
		char szValue[32] = { 0 };
		//normal
		sprintf(szValue, "%.8f", srcNormal.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcNormal.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcNormal.Z);
		strValue += szValue;
		wstring wstrValue = ChineseCode::StringToWString(strValue);
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_NORMAL, wstrValue);
		STK_PNT32 srcOrigin = Stk_PlaneFaceAttributePtr::DynamicCast(pGeoAttr)->GetOrigin();
		//origin
		strValue.clear();
		sprintf(szValue, "%.8f", srcOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcOrigin.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_ORIGIN, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_REVOLUTIONFACE)
	{
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_TYPE, L"2");
		STK_PNT32 srcAxisOrigin = Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_AXISORIGIN, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->GetRevoAxis();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_AXISDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		//radius
		sprintf(szValue, "%.8f", Stk_RevolutionFaceAttributePtr::DynamicCast(pGeoAttr)->GetOriginRadius());
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_RADIUS, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_CYLINDERFACE)
	{
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_TYPE, L"3");
		STK_PNT32 srcAxisOrigin = Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_AXISORIGIN, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->GetRevoAxis();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_AXISDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		//radius
		sprintf(szValue, "%.8f", Stk_CylinderFaceAttributePtr::DynamicCast(pGeoAttr)->GetOriginRadius());
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_RADIUS, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_CONICALFACE)
	{
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_TYPE, L"4");
		STK_PNT32 srcAxisOrigin = Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_AXISORIGIN, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->GetRevoAxis();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_AXISDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		//radius
		sprintf(szValue, "%.8f", Stk_ConeFaceAttributePtr::DynamicCast(pGeoAttr)->GetOriginRadius());
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_RADIUS, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_SPHEREFACE)
	{
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_TYPE, L"5");
		STK_PNT32 srcAxisOrigin = Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_AXISORIGIN, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->GetRevoAxis();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_AXISDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		//radius
		sprintf(szValue, "%.8f", Stk_SphereFaceAttributePtr::DynamicCast(pGeoAttr)->GetOriginRadius());
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_RADIUS, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_TOROIDALFACE)
	{
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_TYPE, L"6");
		STK_PNT32 srcAxisOrigin = Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->GetAxisOrigin();
		string strValue;
		char szValue[32] = { 0 };
		//axisOrigin
		sprintf(szValue, "%.8f", srcAxisOrigin.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcAxisOrigin.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_AXISORIGIN, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->GetRevoAxis();
		//axisDirection
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_AXISDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->GetOriginRadius());
		strValue += szValue;
		//originRadius
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_ORIGINRADIUS, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->GetMajorRadius());
		strValue += szValue;
		//majorRadius
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_MAJORRADIUS, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", Stk_ToroidalFaceAttributePtr::DynamicCast(pGeoAttr)->GetMinorRadius());
		strValue += szValue;
		//minorRadius
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_MINORRADIUS, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_LINE)
	{
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_TYPE, L"7");
		string strValue;
		char szValue[32] = { 0 };
		STK_PNT32 srcCenterPoint = Stk_LineAttributePtr::DynamicCast(pGeoAttr)->GetCenterPoint();
		sprintf(szValue, "%.8f", srcCenterPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_CENTEREPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcStartPoint = Stk_LineAttributePtr::DynamicCast(pGeoAttr)->GetStartPoint();
		sprintf(szValue, "%.8f", srcStartPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_STARTPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcEndPoint = Stk_LineAttributePtr::DynamicCast(pGeoAttr)->GetEndPoint();
		sprintf(szValue, "%.8f", srcEndPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_ENDPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcDirection = Stk_LineAttributePtr::DynamicCast(pGeoAttr)->GetDirection();
		sprintf(szValue, "%.8f", srcDirection.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcDirection.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_DIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", Stk_LineAttributePtr::DynamicCast(pGeoAttr)->GetLength());
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_LENGTH, ChineseCode::StringToWString(strValue));
	}
	else if (geoAttrType == StkGeoAttrTypeEnum::GEOATTR_TYPE_ELLIPSE)
	{
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_TYPE, L"8");
		string strValue;
		char szValue[32] = { 0 };
		STK_PNT32 srcCenterPoint = Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetCenterPoint();
		sprintf(szValue, "%.8f", srcCenterPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcCenterPoint.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_CENTEREPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcStartPoint = Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetStartPoint();
		sprintf(szValue, "%.8f", srcStartPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcStartPoint.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_STARTPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_PNT32 srcEndPoint = Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetEndPoint();
		sprintf(szValue, "%.8f", srcEndPoint.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcEndPoint.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_ENDPOINT, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetMajorRadius());
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_MAJORRADIUS, ChineseCode::StringToWString(strValue));
		sprintf(szValue, "%.8f", Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetMinorRadius());
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_MINORRADIUS, ChineseCode::StringToWString(strValue));
		strValue.clear();
		STK_DIR32 srcXDir, srcYDir, srcZDir;
		Stk_EllipseAttributePtr::DynamicCast(pGeoAttr)->GetXYZDir(srcXDir, srcYDir, srcZDir);
		sprintf(szValue, "%.8f", srcXDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcXDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcXDir.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_XDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear(); sprintf(szValue, "%.8f", srcYDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcYDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcYDir.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_YDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
		sprintf(szValue, "%.8f", srcZDir.X);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcZDir.Y);
		strValue += szValue;
		strValue += " ";
		sprintf(szValue, "%.8f", srcZDir.Z);
		strValue += szValue;
		AddMeshEdgeAttribute(iModelID, iMeshEdgeID, MESH_ZDIRECTION, ChineseCode::StringToWString(strValue));
		strValue.clear();
	}
	return STK_SUCCESS;
}
	}//svllib
}//hoteamsoft