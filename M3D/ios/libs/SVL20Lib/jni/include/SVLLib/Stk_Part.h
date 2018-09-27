// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*			
**************************************************************************/
#ifndef _SVLLIB_PART_H_
#define _SVLLIB_PART_H_
#include "Stk_Object.h"
#include <vector>

namespace HoteamSoft {
	namespace SVLLib {
class Stk_Document;
class Stk_Node;
class STK_TOOLKIT_EXPORT Stk_Part : public Stk_Object
{
	STK_DECLARE_CLASS(Stk_Part)
protected:
	friend class Stk_Node;
	friend class Stk_Document;
	enum {	_typeID = _SVLLIB_PART};	//!< Àà±ð
	void Init(Stk_Document* pDoc, unsigned int parentID, bool bForWrite = true);
public:
	Stk_Part(void);
	virtual ~Stk_Part(void);
	int Add();
	Stk_MeshPrimPtr			AddMeshPrim(unsigned int uiID = -1);
	STK_STATUS DelViewByID(unsigned int viewID = -1);
	unsigned int			GetNbMesh();
	Stk_MeshPrimPtr			GetMesh(int iIdx);
private:
	void BindPart(void* i_bindedPart);
};
	}//svllib
}//hoteamsoft
#endif