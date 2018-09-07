// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*			
**************************************************************************/
#ifndef _SVLLIB_MESHFACE_H_
#define _SVLLIB_MESHFACE_H_
#include "Stk_Object.h"
#include <vector>

namespace HoteamSoft {
	namespace SVLLib {
class Stk_Document;
class Stk_Node;
class Stk_MeshAttribute;
class STK_TOOLKIT_EXPORT Stk_MeshFace : public Stk_Object
{
	STK_DECLARE_CLASS(Stk_MeshFace)
protected:
	friend class Stk_MeshPrim;
	enum {	_typeID = _SVLLIB_TYPE_MESHFACE};	//!< Àà±ð
	void Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite = true);
private:
		STK_UINT32	m_materialID;
public:
	Stk_MeshFace(void);
	virtual ~Stk_MeshFace(void);
	void SetIndex(std::vector<unsigned int>& vecTrians);
	STK_STATUS GetIndex(std::vector<unsigned int>& vecTrians);
	void AddMeshEdgeID(STK_UINT32 i_meshEdgeID);
	STK_STATUS GetEdges(std::vector<int>& vecMeshEdgeID);

	void SetMaterialID(STK_UINT32 i_materialID){m_materialID = i_materialID;}
	STK_UINT32 GetMaterialID(){return m_materialID;}
	Stk_MaterialPtr GetMaterialByID(unsigned int i_materialID);

	void WriteToBlock(StkMeshFaceBlock& meshFaceBlock);
	void ReadFromBlock(const StkMeshFaceBlock meshFaceBlock);

	void Clear();
private:
	//¶¥µã
	std::vector<STK_UINT32> m_vecTrians;
	map<int, int> m_mapMeshEdgeID;
};
	}//svllib
}//hoteamsoft
#endif