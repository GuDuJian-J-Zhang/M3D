// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*			
**************************************************************************/
#ifndef _SVLLIB_INSTANCE_H_
#define _SVLLIB_INSTANCE_H_
#include "Stk_Object.h"
#include <string>
// #include <vector>
// #include <map>

namespace HoteamSoft {
	namespace SVLLib {


class Stk_Document;
class STK_TOOLKIT_EXPORT Stk_Instance : public Stk_Object
{
	STK_DECLARE_CLASS(Stk_Instance)
	friend class Stk_Document;
protected:
	enum {_typeID = _SVLLIB_TYPE_INSTANCE};	//!< 类别
	StkNodeTypeEnum m_enumNodeType;
	Stk_ModelPtr m_pModel;
public:
	Stk_Instance(void);
	virtual ~Stk_Instance(void);
	void Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite = true);
	void SetDocument(void* i_DocumentPtr);

	void					SetInstanceName(const std::wstring& i_wstrNodeName);
	std::wstring            GetInstanceName();

	void                    AddAttribute(const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue);
	std::wstring            GetAttribute(const std::wstring& i_wstrAttrName);
	map<wstring, wstring>   GetAllAttribute();

	const std::wstring		GetNodeName();

	void					SetPlcID(unsigned int inPlcId);
	unsigned int     GetPlcID();

	void					SetParentID(STK_INT32 i_parentID);
	STK_INT32					GetParentID();

	void SetMaterialID(STK_UINT32 inMaterialId);
	STK_UINT32 GetMaterialID();
	Stk_MaterialPtr GetMaterial();
	void					SetNodeType(const StkNodeTypeEnum& i_enumNodetype);
	void					SetColorIdx(unsigned int colorIdx);
	
	void					SetMatrix(float* inMatrix);
	//float*				GetMatrix();
	STK_STATUS		GetMatrix(float* o_inMatrix, int i_size);

	void					SetVisible(bool i_bVisible);
	bool					GetVisible();

	//_stk_rgba32			GetColor();
	STK_RGBA32				GetColor();

	Stk_InstancePtr				AddChild(unsigned int uiID = -1);
	unsigned int		GetNbChild();
	Stk_InstancePtr				GetChild(int iIdx);
	vector<Stk_InstancePtr>& getChildren();

	//节点类型
	StkNodeTypeEnum	GetNodeType(){return m_enumNodeType;}
	void							SetNodeType(StkNodeTypeEnum i_enumNodeType){m_enumNodeType = i_enumNodeType;}
	//矩阵信息
	void	SetQuaternion(const float& w, const float& x, const float& y, const float& z);
	void	SetTranslation(const float& x, const float& y, const float& z);
	int GetQuaternion(float& w, float& x, float& y, float& z);
	int GetTranslation(float& x, float& y, float& z);
	//包围盒信息
	void	SetBoundBox(const float* minPnt, const float* maxPnt);
	void BindingNode(void* i_bingdedNode);
	void BindBomBlock(void* i_bindedBomBlock);
	//设置引用的模型
	void SetModel(Stk_ModelPtr i_pStkModel);
	Stk_ModelPtr SetModel(int uiID = -1);
	Stk_ModelPtr GetModel();
	int				  GetModelID();
	Stk_ModelPtr  GetModelByID(int i_ModelID);
	//设置引用的Model

};

	}//svllib
}//hoteamsoft

#endif