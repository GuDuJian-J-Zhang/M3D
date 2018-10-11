// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*			
**************************************************************************/
#ifndef _SVLLIB_MESHPRIM_H_
#define _SVLLIB_MESHPRIM_H_
#include "Stk_Object.h"
#include <vector>

namespace HoteamSoft {
	namespace SVLLib {


class Stk_Document;
class Stk_Node;
class STK_TOOLKIT_EXPORT Stk_MeshPrim : public Stk_Object
{
	STK_DECLARE_CLASS(Stk_MeshPrim)
protected:
	friend class Stk_Node;
	friend class Stk_Document;
	friend class Stk_MeshAccessor;
	friend class Stk_Part;
	friend class Stk_Model;
	friend class Stk_MeshFace;
	friend class Stk_MeshEdge;
	enum {	_typeID = _SVLLIB_MESHPRIM};	//!< 类别
	void Init(Stk_Document* pDoc, STK_UINT32 parentID, bool bForWrite = true);
public:
	Stk_MeshPrim(void);
	virtual ~Stk_MeshPrim(void);
	void	SetQuaternion(const STK_FLOAT32& w, const STK_FLOAT32& x, const STK_FLOAT32& y, const STK_FLOAT32& z);
	void	SetTranslation(const STK_FLOAT32& x, const STK_FLOAT32& y, const STK_FLOAT32& z);
	void	SetBoundBox(const STK_FLOAT32* minPnt, const STK_FLOAT32* maxPnt);
	void	SetMeshBuf(const std::vector<STK_FLOAT32>& vecVert, const std::vector<STK_FLOAT32>& vecNormals, const std::vector<STK_UINT32>& vecTrians);

	STK_INT32 GetQuaternion(STK_FLOAT32& w, STK_FLOAT32& x, STK_FLOAT32& y, STK_FLOAT32& z);
	STK_INT32 GetTranslation(STK_FLOAT32& x, STK_FLOAT32& y, STK_FLOAT32& z);

	STK_STATUS GetMeshBuf(std::vector<STK_FLOAT32>& vecVert, std::vector<STK_FLOAT32>& vecNormals, std::vector<STK_UINT32>& vecTrians);

	void SetVertexs(std::vector<STK_FLOAT32>& vecVert);
	void SetNormals(std::vector<STK_FLOAT32>& vecNormals);
	void SetTextuercoordinate(std::vector<STK_FLOAT32>& vecTextureCoordinate);
	void SetIndex(std::vector<STK_UINT32>& vecTrians);

	std::vector<STK_FLOAT32>&  GetVertexs();
	std::vector<STK_FLOAT32>&  GetNormals();
	std::vector<STK_FLOAT32>&  GetTextuercoordinate();
	std::vector<STK_UINT32>&  GetIndex();

	void BindMesh(void* i_bindedMesh);
	void* GetBindedData();

	//创建MeshFace
	Stk_MeshFacePtr AddMeshFace(STK_UINT32 uiID = -1);
	STK_INT32 GetNbMeshFace();
	std::map<STK_INT32, Stk_MeshFacePtr>&  GetMeshFaces( );
	Stk_MeshFacePtr	GetMeshFaceByIdx(STK_INT32 i_idx);
	//创建MeshEdge
	Stk_MeshEdgePtr AddMeshEdge(STK_UINT32 uiID = -1);
	std::map<STK_INT32, Stk_MeshEdgePtr>& GetMeshEdges();
	STK_INT32 GetNbMeshEdge();
	Stk_MeshEdgePtr	GetMeshEdgeByIdx(STK_INT32 i_idx);
	Stk_MeshEdgePtr GetMeshEdgeByID(STK_INT32 meshEdgeID);
	void WriteToBlock(StkMeshBlock& meshBlock);
	void ReadFromBlock(const StkMeshBlock meshBlock);
	void Clear();
private:
	//顶点
	std::vector<STK_FLOAT32> m_vecVert;
	std::vector<STK_FLOAT32> m_vecNormals;
	std::vector<STK_FLOAT32> m_vecTextureCoordinate;
	std::vector<STK_UINT32> m_vecTrians;
	map<STK_INT32, Stk_MeshFacePtr> m_mapMeshFace;
	map<STK_INT32, Stk_MeshEdgePtr> m_mapMeshEdge;
};
	}//svllib
}//hoteamsoft
#endif