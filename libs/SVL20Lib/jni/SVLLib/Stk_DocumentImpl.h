// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_ComponentFeatureImpl
	*	@brief	组件特征类的实现类
	*			
**************************************************************************/
#ifndef _SVLLIB_DOCUMENT_IMPL_H_
#define _SVLLIB_DOCUMENT_IMPL_H_
#include "Stk_ObjectImpl.h"
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "prettywriter.h"
#include "allocators.h"
//#include "RVMStruct.pb.h"
#include "RemoteClient.h"
#include "Stk_API.h"
#include "Stk_ViewDocument.h"
#include "Stk_PMIDocument.h"
#include "Stk_AttributeCollection.h"
#include "Stk_GeometryAttributeCollection.h"
#include "Mutex.h"
#define RAPIDJSON_HAS_STDSTRING 1

using rapidjson::Value;
using rapidjson::Document;
using rapidjson::StringBuffer;
using rapidjson::PrettyWriter;
using rapidjson::Writer;
using rapidjson::StringRef;
using rapidjson::MemoryPoolAllocator;

//using namespace RVM::RVMStruct;
using namespace std;

//typedef ::google::protobuf::Map<::google::protobuf::uint32, ::RVM::RVMStruct::Assembly> AssemblyMap;
//typedef ::google::protobuf::Map<::google::protobuf::uint32, ::RVM::RVMStruct::MeshPrim> MeshPrimMap;

namespace HoteamSoft {
	namespace SVLLib {

#if 0
enum aiOrigin { aiOrigin_SET = 0, aiOrigin_CUR = 1, aiOrigin_END = 2 };
class IOStream
{
	FILE* f;
public:
	IOStream(FILE* file) : f(file) {}
	~IOStream() { fclose(f); f = 0; }

	size_t Read(void* b, size_t sz, size_t n) { return fread(b, sz, n, f); }
	size_t Write(const void* b, size_t sz, size_t n) { return fwrite(b, sz, n, f); }
	int    Seek(size_t off, aiOrigin orig) { return fseek(f, off, int(orig)); }
	size_t Tell() const { return ftell(f); }

	size_t FileSize() {
		long p = Tell(), len = (Seek(0, aiOrigin_END), Tell());
		return size_t((Seek(p, aiOrigin_SET), len));
	}
};
#endif

class Stk_DocumentImpl : public Stk_ObjectImpl
{
	STK_CLASS_IMPL_DECLARE(Stk_Document)
public:
	Stk_DocumentImpl(void);
	virtual ~Stk_DocumentImpl(void);
	friend class Stk_ObjectImpl;
	Document m_nodeDoc;
	Document m_modelDoc;
	Document m_materialDoc;
	Document m_fileInfoDoc;
	Stk_ViewDocumentPtr m_pViewDoc;
	Stk_PMIDocumentPtr m_pPMIDoc;
	Stk_AttributeCollectionPtr m_pAttrDoc;
	Stk_GeometryAttributeCollectionPtr m_pGeoAttrDoc;
	vector<Stk_MeshPrimPtr> m_vecMeshes;
	map<int, Stk_NodePtr> m_mapIdToNode;
	map<int, Stk_InstancePtr> m_mapIdToInstance;
	map<int, Stk_PartPtr> m_mapIdToStkPart;
	map<int, Stk_ModelPtr> m_mapIdToStkModel;
	map<int, Stk_MaterialPtr> m_mapIdToStkMaterial;
	map<int, Stk_ImagePtr> m_mapIdToStkImage;
	map<int, vector<Stk_InstancePtr>> m_parentInstanceIdToStkInstance;
	Stk_ResourceManagerPtr m_resourceManagerPtr;
	std::string m_strOutputFullPath;
	std::string m_strOutputName;

	STK_CHAR* m_fileBuffer;
	STK_UINT32 m_bufferLength;
	STK_UINT32 m_instanceCount;

	StkFileOpenModeEnum m_enumFileOpenMode;
	bool			 m_hasExtractMeshData;
	Stk_FilePtr m_meshFilePtr;
	HANDLE m_hMeshTempFile;

	//统计信息
	STK_UINT32 m_meshCount;
	STK_UINT32 m_meshFaceCount;
	STK_UINT32 m_meshEdgeCount;
	STK_UINT32 m_triangleCount;
	STK_UINT32 m_meshDataSize;
private:
	Value m_TopNode;
	//RVMDoc m_RVMDoc;
	//BufferSet m_RVMBuffer;
	std::wstring m_outputFullPath;
	std::wstring m_outputName;
	std::string m_outputBinNameUTF8;
	std::wstring m_wstroutputBinName;
	bool			m_bRemote;
	bool			m_bOpened;

	RemoteClient*    m_pRemoteClient;
	Mutex			m_mutexBufRead;
	Mutex			m_mutexDocRead;
	map<int, Value*> m_mapNodeIdToStkNode;
	map<int, Value*> m_mapNodeIdToInstanceNode;
	map<int, Value*> m_mapPartIDToPartNode;
	map<int, Value*> m_mapModelIDToModelNode;
	map<int, Value*> m_mapMaterialIDToMaterialNode;
	map<int, Value*> m_mapMaterialIDToImageNode;
	vector<string>		m_vecFileName;
	unsigned int m_svlxBufferSize;
	char*			m_bufP;
	Stk_FilePtr	m_svlxFilePtr;
	bool				m_isNeedDelSvlxBuf;
	string			m_strAttributeBuf;

private:
	STK_STATUS	WriteFile();
	STK_STATUS WriteInfoFile();
	STK_STATUS WriteBomFile();
	STK_STATUS	 WriteBomBinFile();
	STK_STATUS WriteModelFile();
	STK_STATUS WriteModelBinFile();
	STK_STATUS WriteGeoFile();
	STK_STATUS WriteMeshFile();
	STK_STATUS WriteMaterialFile();
	STK_STATUS	WriteNode(Stk_NodePtr i_stkNodeP);
	STK_STATUS WriteViewFile();
	STK_STATUS WritePMIFile();
	STK_STATUS WriteAttrFile();
	STK_STATUS WriteGeoAttrFile();
	STK_STATUS WriteLineFile();
	STK_STATUS WriteLayerFile();
	STK_STATUS WriteFeatFile();
	STK_STATUS WriteSVLXFile();
	STK_STATUS AddSVLXFileItem(const std::string& i_strFileName);
	STK_STATUS ClearSVLXFileItem();
	STK_STATUS GetSVLXFileItem(const std::string& i_strFileName, unsigned int& o_bufSize, char** o_bufP, bool bByExt = true);
	STK_STATUS GetSVLXFileItemToFile(const std::string& i_strFileName, const std::string& i_strOutFilePath, bool bByExt = true);
	STK_STATUS GetSVLXFolderItemToFolder(const std::string& i_strFileName, const std::string& i_strOutFilePath);
	IOStream*	OpenFile(std::string path, const char* mode, bool absolute);
	STK_STATUS	OpenLocal(const std::wstring& fileFullPath);
	STK_STATUS OpenFile(const unsigned int i_bufferSize,	const char* i_bufP);
	STK_STATUS OpenFile(const std::wstring& fileFullPath);
	//BOM
	STK_STATUS LoadBom(const std::wstring& fileFullPath);
	STK_STATUS LoadBom(void* i_docPtr);
	STK_STATUS LoadBom(void* i_docPtr, const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType);
	STK_STATUS LoadBomFromBuf(void* i_docPtr);
	STK_STATUS LoadAttrFromBuf(void* i_docPtr);
	STK_STATUS LoadGeoAttrFromBuf(void* i_docPtr);
	STK_STATUS LoadPMIFromBuf(void* i_docPtr);
	STK_STATUS LoadViewFromBuf(void* i_docPtr);
	STK_STATUS LoadAttr(void* i_docPtr);
	STK_STATUS LoadLayer(void* i_docPtr);
	STK_STATUS LoadFeat(void* i_docPtr);
	STK_STATUS parseBom(void* i_docPtr, Stk_FilePtr& i_bomFilePtr);
	STK_STATUS UnloadBom();
	//Info
	STK_STATUS LoadFileInfo();
	STK_STATUS LoadFileInfo(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType);
	STK_STATUS LoadFileFromBuf();
	//Model
	STK_STATUS LoadModel();
	STK_STATUS LoadModel(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType);
	STK_STATUS loadModelFromBuf();
	STK_STATUS parseModel(Stk_FilePtr& i_modelFilePtr);
	STK_STATUS UnloadModel();
	Stk_ModelPtr GetModelByID( int ModelID);
	//Material
	STK_STATUS LoadMaterial();
	STK_STATUS LoadMaterial(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType);
	STK_STATUS LoadMaterialFromBuf();
	STK_STATUS UnloadMaterial();

	STK_STATUS	OpenRemote(const std::string& strIP, int port, 
		const std::string& userName, const std::string& projectCode, const std::string& fileName);

	void	InitJsonNodeMap();
	void InitJsonPartMap();
	void InitJsonInstanceMap();
	void InitJsonModelMap();
	void InitJsonMaterialMap();
	void InitJsonImageMap();
	//Buffer* LoadLocalBin(const std::wstring& binPath);
	void initBinFile();
	void InitViewFile();
	void InitPMIFile();
	void InitAttrFile();

	void InitModel();

	bool getFileNameBySuffix(const string& i_strSuffix, string& o_strFileName);
	
public:
	void				SetOutputFullPath(const std::wstring& fullPath);
	std::wstring		GetOutputFullPath();
	std::wstring		GetOutputName();
	const std::string&	GetBinNameUTF8();
	//int					GetAssembleByParentID(unsigned int parentID, std::vector<Assembly>& vecAssemblies);
	//int					GetMeshPrimByParentID(unsigned int parentID, std::vector<MeshPrim>& vecMeshPrims);
	//AssemblyMap*		GetAssemblyMap();
	Value*			GetNode(unsigned int nodeID);
	Value*          GetJsonNode(unsigned int nodeID);
	Value*          GetJsonPart(unsigned int partID);
	Value*			GetJsonModel(unsigned int modelID);
	Value*			GetJsonInstance(unsigned int instanceID);
	Value*			GetJsonMaterial(unsigned int materialID);
	//MeshPrimMap*	GetMeshPrimMap();
	//MeshPrim*		GetMeshPrim(unsigned int meshPrimID);
	int				ListPrimID(std::vector<int>& primIDs);

	//Buffer*			GetRVMBuffer(unsigned int bufIdx);
	//Buffer*			GetRVMBuffer(const std::string& urn);
	//STK_STATUS		GetRVMBuffer(const std::vector<Accessor>& vecAccessor, Buffer& outBuffer);
	//Buffer*			AddRVMBuffer();

	void			AddTopNodeID(unsigned int nodeID);
	void			AddTopInstanceID(unsigned int instanceID);
	//int				ListTopNodeID(std::vector<unsigned int>& outTopNodeIDs);
	unsigned int	GetNbTopNode();
	unsigned int	GetTopNodeID(unsigned int iIdx);
	//int				ListAssemblyID(std::vector<int>& assebmlyID);
	unsigned int GetNbTopInstance();
	unsigned int	GetTopInstanceID(unsigned int iIdx);

	vector<Stk_InstancePtr>&	getTopInstances();

	Stk_MaterialPtr AddMaterial(unsigned int uiID = -1);
	Stk_MaterialPtr FindMaterial(MaterialData materialData);
	Stk_ImagePtr AddImage(bool& bHasSet, const string& strImagePath, unsigned int uiID = -1);

	STK_STATUS loadMeshFromBuff();
	STK_STATUS LoadAttribute(const std::wstring& fileFullPath);
	STK_STATUS LoadAttribute(void* i_docPtr);
	STK_STATUS LoadGeoAttribute(void* i_docPtr);
	STK_STATUS loadAttributeStream();
	STK_STATUS unloadAttibuteStream();
	STK_STATUS getInstanceNames(map<STK_UINT32, std::wstring>& i_mapInstanceIDtoName);
	STK_STATUS getAttributesByIds(vector<STK_UINT32> i_vecId, map<STK_UINT32, map<std::wstring, std::wstring>>& i_mapKeyToValue);
	STK_STATUS unLoadAttribute();
	STK_STATUS UnLoadGeoAttribute();
	STK_STATUS LoadPMI(void* i_docPtr);
	STK_STATUS LoadPMI(const std::wstring& fileName);
	STK_STATUS LoadView(void* i_docPtr);
	STK_STATUS LoadView(const std::wstring& fileName);

	Stk_InstancePtr GetInstanceByID(int iInstanceID);

	Stk_GeometryAttributePtr  GetMeshFaceAttribute(int iModelID, int iMeshFaceID);
	map<wstring, wstring> GetMeshFaceAllAttribute(int iModelID, int iMeshFaceID);
	wstring              GetMeshFaceAttribute(int iModelID, int iMeshFaceID, const std::wstring& i_wstrAttrName);
	STK_STATUS           AddMeshFaceAttribute(int iModelID, int iMeshFaceID, const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue);
	STK_STATUS           AddMeshFaceAttribute(int iModelID, int iMeshFaceID, Stk_GeometryAttributePtr pGeoAttr);

	Stk_GeometryAttributePtr  GetMeshEdgeAttribute(int iModelID, int iMeshEdgeID);
	map<wstring, wstring> GetMeshEdgeAllAttribute(int iModelID, int iMeshEdgeID);
	wstring              GetMeshEdgeAttribute(int iModelID, int iMeshEdgeID, const std::wstring& i_wstrAttrName);
	STK_STATUS           AddMeshEdgeAttribute(int iModelID, int iMeshEdgeID, const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue);
	STK_STATUS           AddMeshEdgeAttribute(int iModelID, int iMeshFaceID, Stk_GeometryAttributePtr pGeoAttr);
};
	}//svllib
}//hoteamsoft

#endif
