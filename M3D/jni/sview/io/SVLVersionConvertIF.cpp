/****************************************************************************
��Ȩ���� (C) 2014  ɽ��ɽ����������޹�˾

�ļ�����	SVLVersionConvertor.cpp
���ܣ�
�ṩSVL���汾�����ݸ�ʽת���ӿ�

���¼�¼��
****************************************************************************/
 
#include "SVLVersionConvertIF.h"
//#include "SVLVersionConvert.h"
#include <map>
#include "sview/io/SVL1ToSVL2.h"
#include "m3d/utils/Platform.h"

static std::map<int, int> m_mapOldProtoIdToNewProtoId;
static std::map<int, int> m_mapOldInsIdToNewInsId;
CSVLVersionConvertIF::CSVLVersionConvertIF()
{
	
}

CSVLVersionConvertIF::~CSVLVersionConvertIF()
{

}

int CSVLVersionConvertIF::SVL1ToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument)
{
	int iRet = 0;
	if (!pStkDocMgr || pDocument.isNull())
	{
		return -1;
	}

	iRet = CSVL1ToSVL2::SVL1ToSVL2(pStkDocMgr, pDocument);
	return iRet;
}

int CSVLVersionConvertIF::SVL1ToSVL2(const string& srcPath, const string &targetPath)
{
#ifdef WIN32
	Stk_DocumentManager *pStkDocMgr = Stk_DocumentManager::NewDocumentManager();
#else
	Stk_DocumentManager *pStkDocMgr = new Stk_DocumentManager();
#endif
	//pStkDocMgrr->SetListener(this);
	//wstring wstrFile = CStringW(thisP->m_strFilePath).GetBuffer();
	//thisP->m_ctrlProg.SetPos(20);
	wstring wstrFilePath = M3D::Platform::StringToWString(srcPath, "auto");
	STK_STATUS iRet = pStkDocMgr->LoadDocument(wstrFilePath);
	if (STK_SUCCESS != iRet) {
#ifdef WIN32
		Stk_DocumentManager::DeleteDocumentManager(pStkDocMgr);
#else
		pStkDocMgr->~Stk_DocumentManager();
#endif
		return iRet;
	}

	if (pStkDocMgr)
	{
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument = HoteamSoft::SVLLib::Stk_DocumentPtr::CreateObject();
		wstring wstrtargetPath = M3D::Platform::StringToWString(targetPath, "auto");
		pDocument->SetOutputFullPath(wstrtargetPath);

		CSVLVersionConvertIF::SVL1ToSVL2(pStkDocMgr, pDocument);

		pDocument->WriteFile();

		pDocument->UnloadBom();
		pDocument->UnloadModel();

#ifdef WIN32
		Stk_DocumentManager::DeleteDocumentManager(pStkDocMgr);
#else
		pStkDocMgr->~Stk_DocumentManager();
#endif
	}
	return 0;
}


