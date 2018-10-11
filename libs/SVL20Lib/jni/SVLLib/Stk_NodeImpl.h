// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_NodeImpl
	*	@brief	
	*			
**************************************************************************/
#ifndef _SVLLIB_NODE_IMPL_H_
#define _SVLLIB_NODE_IMPL_H_
#include "Stk_ObjectImpl.h"
#include "Stk_Document.h"
//#include "RVMStruct.pb.h"
#include "Stk_DocumentImpl.h"

namespace HoteamSoft {
	namespace SVLLib {

class Stk_NodeImpl : public Stk_ObjectImpl
{
	STK_CLASS_IMPL_DECLARE(Stk_Node)
public:
    Stk_NodeImpl(void);
	virtual ~Stk_NodeImpl(void);

	Stk_Document* m_pDoc;
	Stk_Document* GetDoc() { return m_pDoc; }
	Value*				m_pJsonNode;

private:
	//≥…‘±
	float m_qw, m_qx, m_qy, m_qz;
	float m_tx, m_ty, m_tz;
	float m_bbMinX, m_bbMinY, m_bbMinZ;
	float m_bbMaxX, m_bbMaxY, m_bbMaxZ;

	//∑Ω∑®
	void	SetQuaternion(const float& w, const float& x, const float& y, const float& z);
	void	SetTranslation(const float& x, const float& y, const float& z);
	int GetQuaternion(float& w, float& x, float& y, float& z);
	int GetTranslation(float& x, float& y, float& z);
	void	SetBoundBox(const float* minPnt, const float* maxPnt);
};

	}//svlib
}//hoteamsoft
#endif
