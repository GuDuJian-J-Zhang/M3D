// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_Document
	*
	*			
**************************************************************************/
#ifndef _SVLLIB_DOCUMENT_H_
#define _SVLLIB_DOCUMENT_H_
#include "Stk_Object.h"
#include <vector>
#include <map>
#include "Stk_Instance.h"
#include "Stk_Node.h"
#include "Stk_MeshPrim.h"
#include "Stk_Material.h"
#include "Stk_Part.h"
#include "Stk_File.h"
#include "Stk_Model.h"
#include "Stk_MeshFace.h"
#include "Stk_MeshEdge.h"
#include "Stk_Image.h"
#include "Stk_Texture.h"
#include "Stk_GeometryAttribute.h"

namespace HoteamSoft {
	namespace SVLLib{
class STK_TOOLKIT_EXPORT Stk_Document : public Stk_Object
{
	STK_DECLARE_CLASS(Stk_Document)
	friend class Stk_Node;
	friend class Stk_Instance;
	friend class Stk_MeshPrim;
	friend class Stk_Part;
	friend class Stk_Model;
	friend class Stk_MetaData;
	friend class Stk_Material;
	friend class Stk_MeshFace;
	friend class Stk_MeshEdge;
protected:
	enum {_typeID = _SVLLIB_TYPE_DOCUMENT};	//!< 类别
public:
	Stk_Document(void);
	virtual ~Stk_Document(void);
	
	bool				IsOpen();
	STK_STATUS			OpenLocal(const std::wstring& fileFullPath);
	STK_STATUS			OpenFile(const unsigned int i_bufferSize,	const char* i_bufP);
	STK_STATUS			OpenFile(const std::wstring& fileFullPath);	
	STK_STATUS			OpenFile(const std::string& strFileFullPathUTF8);
	STK_STATUS			cacheMeshToTempPath();	
	STK_STATUS         OpenSVLX(const std::wstring& fileFullPath);
	STK_STATUS			LoadBom(const std::wstring& bomFullPath);
	STK_STATUS			LoadBom();
	STK_STATUS			LoadBom(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType);
	STK_STATUS			UnloadBom();
	STK_STATUS			LoadModel();
	STK_STATUS			LoadModel(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType);
	STK_STATUS			UnloadModel();
	STK_STATUS			LoadMaterial();
	STK_STATUS			LoadMaterial(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType);
	STK_STATUS          UnloadMaterial();
	STK_STATUS			LoadFileInfo();
	STK_STATUS			LoadFileInfo(const unsigned int i_bufSize, const char* i_bufP, StkMemBlockTypeEnum i_enumBlockType);
	STK_STATUS			LoadLayer();
	STK_STATUS			LoadFeat();
	STK_STATUS			LoadLine();
	STK_STATUS			OpenRemote(const std::string& strIP, int port, 
	const std::string& userName, const std::string& projectCode, const std::string& fileName);
	//统计信息
	unsigned int			GetNbInstance();
	unsigned int			GetNbModel();
	//材质相关
	Stk_MaterialPtr AddMaterial(bool& bHasSet, unsigned int uiID = -1);
	Stk_MaterialPtr GetMaterialByID(unsigned int i_materialID);
	Stk_MaterialPtr FindMaterial(MaterialData materialData);
	void SetGlobalEffect(wstring effectname);
	wstring GetGlobalEffect();

	Stk_ImagePtr AddImage(bool& bHasSet, const string& strImagePath, unsigned int uiID = -1);
	Stk_ImagePtr GetImageByID(unsigned int i_ImageID);
	Stk_NodePtr			AddTopNode(unsigned int uiID = -1);
	int					GetNbTopNode();
	int					GetNbTopInstance();
	vector<Stk_InstancePtr>&	getTopInstances();
	Stk_InstancePtr    AddTopInstance(unsigned int uiID = -1);
	Stk_InstancePtr    GetTopInstance(int iIdx);

	Stk_NodePtr			GetTopNode(int iIdx);
	Stk_NodePtr			GetNodeByID(unsigned int nodeID);
	Stk_PartPtr			GetPartByID(unsigned int partID);
	Stk_MeshPrimPtr		GetMeshPrimByID(unsigned int PrimID);
	void				SetOutputFullPath(const std::wstring& fullPath);
	std::wstring		GetOutputFullPath();
	std::wstring		GetOutputName();
	STK_STATUS			WriteFile();
	STK_STATUS WriteBomFile(const std::wstring& newPath = L"");
	STK_STATUS WriteMaterialFile(const std::wstring& newPath =L"");
	const std::string&	GetBinNameUTF8();

	Stk_ModelPtr GetModelByID( int ModelID);

	void				ListAssemblyBuffer(std::vector<std::string>& outBuffers);
	void				ListPrimBuffer(std::vector<std::string>& outBuffers);
	int					ListTopNodeID(std::vector<unsigned int>& outTopNodeIDs);
	int					ListAssembly(std::vector<Stk_NodePtr>& vecAssembly);
	int					ListMeshPrim(std::vector<Stk_MeshPrimPtr>& vecMeshPrim);
	int					ListPrimID(std::vector<int>& vecPrimIDs);
	int					ListModel(std::vector<Stk_ModelPtr>& vecModel);

	bool				GetAssemblyBuf(unsigned int assID, std::string& outAssemblyBuf);
	bool				GetMeshPrimBuf(unsigned int meshID, std::string& outMeshBuf);
	bool				GetMeshBufBuf(const std::vector<std::string>& vecAccessorBuf, 
						std::vector<int>& vecVertex, std::vector<int>& vecNormals, std::vector<int>& vecTrians);
	bool				GetMeshBufByUrn(const std::string& bufUrn,
		std::vector<int>& vecVertex, std::vector<int>& vecNormals, std::vector<int>& vecTrians);
	Stk_AttributeCollectionPtr GetAttributeCollection();

	bool OpenLocalBuffer(STK_CHAR* fileBuffer, const STK_UINT32 bufferLength);
	Stk_FilePtr GetBomFile();
	Stk_FilePtr GetLodFile();
	Stk_FilePtr GetMaterialFile();
	Stk_FilePtr GetLoadInfoFile();
	Stk_FilePtr GetModelAttributeFile();
	Stk_FilePtr GetFile(const string& fileName);
	STK_STATUS			LoadAttribute(const std::wstring& bomFullPath);
	STK_STATUS         LoadAttribute();
	STK_STATUS         LoadGeoAttribute();
	STK_STATUS			getInstanceNames(map<STK_UINT32, std::wstring>& i_mapInstanceIDtoName);
	STK_STATUS			getAttributesByIds(vector<STK_UINT32> i_vecId, map<STK_UINT32, map<std::wstring, std::wstring>>& i_mapKeyToValue);
	STK_STATUS			loadAttributeStream();
	STK_STATUS			unloadAttributeStream();
	STK_STATUS			UnLoadAttribute();
	STK_STATUS          UnloadGeoAttribute();
	STK_STATUS          LoadPMI();
	STK_STATUS          LoadView();
	STK_STATUS AddSVLXFileItem(const std::string& i_strFileName);
	STK_STATUS ClearSVLXFileItem();
	STK_STATUS GetSVLXFileItem(const std::string& i_strFileName, unsigned int& o_bufSize, char** o_bufP, bool bByExt = true);
	STK_STATUS GetSVLXFileItemToFile(const std::string& i_strFileName, const std::string& i_strOutFilePath, bool bByExt = true, const std::string& i_strNewFileName = "");
	STK_STATUS GetSVLXFolderItemToFolder(const std::string& i_strFileName, const std::string& i_strOutFilePath);
	Stk_InstancePtr GetInstanceByID(int instanceID);
    map<STK_ID, map<STK_ID, map<wstring, wstring>>> *GetAllMeshFaceAttr();
    map<STK_ID, map<STK_ID, map<wstring, wstring>>> *GetAllMeshEdgeAttr();
    static Stk_GeometryAttributePtr  GetMeshGeometryAttribute(map<wstring, wstring>& mapAttribute);
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
	void                 ClearInstanceMap();
	void                 AddInstance(unsigned int iParentID, vector<Stk_InstancePtr> vcChild);
	void                 AddInstance(unsigned int iParentID, Stk_InstancePtr instancePtr);
	vector<Stk_InstancePtr>                 FindInstance(unsigned int iParentID);
	void                 RemoveInstance(unsigned int iParentID);
	void                 RemoveInstance(unsigned int iParentID, Stk_InstancePtr instancePtr);

	void					SetInstanceName(unsigned int iInstanceID, const std::wstring& i_wstrNodeName);
	std::wstring            GetInstanceName(unsigned int iInstanceID);

	void                    AddInstanceAttribute(unsigned int iInstanceID, const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue);
	std::wstring            GetInstanceAttribute(unsigned int iInstanceID, const std::wstring& i_wstrAttrName);
	map<wstring, wstring>   GetInstanceAllAttribute(unsigned int iInstanceID);

	wstring     GetModelName(unsigned int iModelID);
	void        SetModelName(unsigned int iModelID, const wstring& i_wstrName);

	void                    AddModelAttribute(unsigned int iModelID, const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue);
	std::wstring            GetModelAttribute(unsigned int iModelID, const std::wstring& i_wstrAttrName);
	map<wstring, wstring>   GetModelAllAttribute(unsigned int iModelID);

	STK_STATUS   UnloadLayer();
	STK_STATUS   UnloadFeat();
	STK_STATUS   UnloadLine();
	STK_STATUS   UnloadPMI();
	STK_STATUS   UnloadView();

	STK_STATUS		addFileToSVLX(const wstring& i_wstrFileName, const wstring& i_wstrSVLXFileName, const wstring& i_wstrItemName = L"", bool i_bDeleteFile = true);

	Stk_LoadInf* GetLoadInf();
	STK_STATUS Stk_Document::WriteViewFile( const std::wstring& outputFile ,const string&  strOutputFile);
};
	}//svllib
}//hoteamsoft

#endif
