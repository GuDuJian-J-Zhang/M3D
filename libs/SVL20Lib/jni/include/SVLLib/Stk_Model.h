// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*			
**************************************************************************/
#ifndef _SVLLIB_MODEL_H_
#define _SVLLIB_MODEL_H_
#include "Stk_Object.h"
#include <vector>

namespace HoteamSoft {
	namespace SVLLib {
class Stk_Document;
class Stk_Instance;
class Stk_MeshLodBlock;

class STK_TOOLKIT_EXPORT Stk_Model : public Stk_Object
{
	STK_DECLARE_CLASS(Stk_Model)
protected:
	friend class Stk_Instance;
	friend class Stk_Document;
	enum {	_typeID = _SVLLIB_TYPE_MODEL,};	//!< Àà±ð
	void Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite = true);
public:
	Stk_Model(void);
	virtual ~Stk_Model(void);
	int Add();
	STK_UINT32		GetLodCount();
	Stk_MeshPrimPtr			AddMeshPrim(unsigned int uiID = -1);
	Stk_MeshPrimPtr			AddLOD1Mesh(unsigned int uiID = -1);
	Stk_MeshPrimPtr			AddLOD2Mesh(unsigned int uiID = -1);
	STK_STATUS DelViewByID(unsigned int viewID = -1);
	unsigned int			GetNbMesh();
	Stk_MeshPrimPtr			GetMesh(int iIdx);
	unsigned int			GetNbLOD1Mesh();
	unsigned int			GetNbLOD2Mesh();
	Stk_MeshPrimPtr			GetLOD1Mesh(int iIdx);
	Stk_MeshPrimPtr			GetLOD2Mesh(int iIdx);
	STK_STATUS					GetLOD1Accessor(STK_INT32& i_fileIndex, STK_UINT32& i_fileOffset, STK_UINT32& i_fileLength);
	STK_STATUS					GetLOD2Accessor(STK_INT32& i_fileIndex, STK_UINT32& i_fileOffset, STK_UINT32& i_fileLength);
	STK_STATUS LoadLOD1Mesh();
	STK_STATUS LoadLOD2Mesh();
	STK_STATUS UnloadLOD1Mesh();
	STK_STATUS UnloadLOD2Mesh();
	void  SetBoundingBox(float* i_pBBox);
	int  GetBoundingBox(float* o_pBBox, int i_size);
	int  GetBoundingBoxByJson(float* o_pBBox, int i_size);
	STK_STATUS WriteMesh(Stk_FilePtr& binFile);
	STK_STATUS WriteLod1Mesh(Stk_FilePtr& binFile);
	STK_STATUS WriteLod2Mesh(Stk_FilePtr& binFile);
	void BindModel(void* i_bindedModel);
	void BindModelBlock(void* i_bindedModelBlock);
	void BindLodAccessorBlock(void* i_lodAccessorBlock);
	STK_STATUS	AddPMI(Stk_PMIPtr i_StkPMIP);
	STK_STATUS  AddView(Stk_ViewPtr i_StkViewP);

	wstring     GetModelName();
	void        SetModelName(const wstring& i_wstrName);

	void                    AddAttribute(const std::wstring& i_wstrAttrName, const std::wstring& i_wstrAttrValue);
	std::wstring            GetAttribute(const std::wstring& i_wstrAttrName);
	map<wstring, wstring>   GetAllAttribute();

	Stk_LineSetPtr          AddLineSet(unsigned int uiID = -1);
	STK_STATUS				DeleteLineSet(unsigned int uiID);
	map<int, Stk_LineSetPtr>&   GetAllLineSet();
	STK_STATUS AddSubLine(Stk_CurveEntityPtr linePtr);
	STK_STATUS DeleteSubLine(unsigned int uiID);
	map<int, Stk_CurveEntityPtr>& GetAllSubLine();
	Stk_SpacialPointEntityPtr AddSpacialPoint(unsigned int uiID = -1);
	STK_STATUS DeleteSpacialPoint(unsigned int uiID);
	map<int, Stk_SpacialPointEntityPtr>& GetAllSpacialPoint();

	Stk_LayerInfosSetPtr AddLayerInfosSet(unsigned int uiID = -1);
	STK_STATUS DeleteLayerInfosSet(unsigned int uiID);
	map<int, Stk_LayerInfosSetPtr>& GetAllLayerInfosSet();
	Stk_LayerPtr GetLayerById(unsigned int uiID);

	Stk_FeatPtr AddFeat(unsigned int uiID = -1);
	STK_STATUS DeleteFeat(unsigned int uiID);
	map<int, Stk_FeatPtr>& GetAllFeat();
	STK_STATUS GetAllPMI(vector<Stk_PMIPtr>& vcPMI);
	STK_STATUS GetAllView(vector<Stk_ViewPtr>& vcViews);
private:
	

	STK_UINT32 GetMeshLodBlockCount();
	Stk_MeshLodBlock* GetMeshLodBlock(STK_UINT32 lodLevel);

	vector<Stk_MeshLodBlock*> m_lodMeshesBlocks;
};
	}//svllib
}//hoteamsoft
#endif