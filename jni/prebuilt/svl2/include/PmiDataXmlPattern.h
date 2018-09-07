#pragma once
#include "tinyxml/tinyxml2.h"
#include "Stk_Define.h"
#include "Stk_File.h"
#include "Stk_PMIEntity.h"
#include "Stk_View.h"

using namespace tinyxml2;

class CPmiDataXmlPattern
{
public:
	CPmiDataXmlPattern(void);
	~CPmiDataXmlPattern(void);
public:
	//add by gongcy begin
	static STK_STATUS		    OpenXml( const char * FilePath,Stk_File * pFile);
	static XMLElement*		GetXmlCurve(XMLElement *pDtk_CurvePtrElem,Stk_CurveEntity **ppStkCurve,bool flg=false);
	static STK_STATUS			GetXmlTerminateorDir(XMLElement *pdirElem,double* refvec);
	static STK_STATUS			GetXmlLead(XMLElement *pLeadersElem,vector<Stk_Leader*>& inLeader);
	static STK_STATUS			GetXmlFrame(XMLElement*pDtk_FrameElem,Stk_OutFrame** ppStkOutFrame);
	static STK_STATUS			GetXmlCompositeText(XMLElement *pDtk_CompositeTextElem, Stk_ComText** ppStkComText);
	static STK_STATUS			GetXmlExt_Line(XMLElement*pDtk_CurvePtrElem,vector<Stk_Extensionline*>&inExLine);
	//获取特殊线的数量
	static STK_STATUS			GetXmlSpecialLines(XMLElement*pExt_linesElem,Stk_SpecialLine **ppStkUserLine);
	static STK_STATUS			GetXmlModelDisplay(XMLElement*pDtk_ModelDisplayElem, Stk_View** ppView);
	static STK_STATUS			GetXmlmatrix(XMLElement*pTransfo_matrixElem,STK_MTX32 *pmiMaxP);
	static STK_STATUS			GetXmlMember(const char * pdata,double*aQueue);
	//字符转化
	static void UTF_8ToUnicode(wstring &wstrOut, const char *pText);

	//add by gongcy end

	//ISSUE:for android 实现ms库函数： stricmp
	static int stricmp(const char* str1,const char* str2);
};
