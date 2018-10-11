// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*			
**************************************************************************/
#ifndef _SVLLIB_NODE_H_
#define _SVLLIB_NODE_H_
#include "Stk_Object.h"
#include <string>
// #include <vector>
// #include <map>

namespace HoteamSoft {
	namespace SVLLib {


class Stk_Document;
class STK_TOOLKIT_EXPORT Stk_Node : public Stk_Object
{
	STK_DECLARE_CLASS(Stk_Node)
	friend class Stk_Document;
protected:
	enum {_typeID = _SVLLIB_TYPE_NODE};	//!< 类别
	StkNodeTypeEnum m_enumNodeType;
	void Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite = true);
	Stk_PartPtr m_pPart;
public:
	Stk_Node(void);
	virtual ~Stk_Node(void);

	void					SetNodeName(const std::wstring& i_wstrNodeName);
	const std::wstring		GetNodeName();

	void					SetNodeType(const StkNodeTypeEnum& i_enumNodetype);
	void					SetColorIdx(unsigned int colorIdx);
	//_stk_rgba32			GetColor();
	STK_RGBA32				GetColor();

	Stk_NodePtr				AddChild(unsigned int uiID = -1);
	unsigned int		GetNbChild();
	Stk_NodePtr				GetChild(int iIdx);

	Stk_MeshPrimPtr			AddMeshPrim(unsigned int uiID = -1);
	int						GetNbMeshPrim();
	Stk_MeshPrimPtr			GetMeshPrim(unsigned int iIdx);
	//节点类型
	StkNodeTypeEnum	GetNodeType(){return m_enumNodeType;}
	void							SetNodeType(StkNodeTypeEnum i_enumNodeType){m_enumNodeType = i_enumNodeType;}
	//矩阵信息
	void SetMatrix(STK_MTX32& iMatrix);
	int GetMatrix(STK_MTX32& iMatrix);
	//包围盒信息
	void	SetBoundBox(const float* minPnt, const float* maxPnt);
	void BindingNode(void* i_bingdedNode);
	//设置引用的零件
	void SetPart(Stk_PartPtr i_pStkPart);
	Stk_PartPtr SetPart(unsigned int uiID = -1);
	Stk_PartPtr GetPart();
};

	}//svllib
}//hoteamsoft

#endif