// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*			
**************************************************************************/
#ifndef _SVLLIB_MESHEDGE_H_
#define _SVLLIB_MESHEDGE_H_
#include "Stk_Object.h"
#include <vector>

namespace HoteamSoft {
	namespace SVLLib {
class Stk_Document;
class Stk_Node;
class Stk_MeshAttribute;
class STK_TOOLKIT_EXPORT Stk_MeshEdge : public Stk_Object
{
	STK_DECLARE_CLASS(Stk_MeshEdge)
protected:
	friend class Stk_MeshPrim;
	enum {	_typeID = _SVLLIB_TYPE_MESHEDGE};	//!< Àà±ð
	void Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite = true);
public:
	Stk_MeshEdge(void);
	virtual ~Stk_MeshEdge(void);
	void SetIndex(std::vector<unsigned int>& vecEdgeIndex);
	STK_STATUS GetIndex(std::vector<unsigned int>& vecEdgeIndex);
	void WriteToBlock(StkMeshEdgeBlock& meshEdgeBlock);
	void ReadFromBlock(const StkMeshEdgeBlock meshEdgeBlock);
	void Clear();

	Stk_GeometryAttributePtr  GetAttribute();
	map<wstring, wstring> GetAllAttribute();
	//wstring              GetAttribute(const std::wstring& i_wstrAttrName);
	//STK_STATUS           AddAttribute(const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue);
private:
	//¶¥µã
	std::vector<STK_UINT32> m_vecEdgeIndex;
};
	}//svllib
}//hoteamsoft
#endif