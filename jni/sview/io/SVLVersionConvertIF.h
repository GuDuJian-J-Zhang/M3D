/****************************************************************************
版权所有 (C) 2014  山东山大华天软件有限公司

文件名：	SVLVersionConvertor.h
功能：
提供SVL各版本的数据格式转换接口

更新记录：
****************************************************************************/
#ifndef _SVLVERSIONCONVERTIF_H_
#define _SVLVERSIONCONVERTIF_H_

#include "Stk_ProtoType.h"
#include "SVLLib/Stk_Declare_Ptr.h"
#include "SVLLib/Stk_Document.h"

//Ascii码文件读取结构
class CSVLVersionConvertIF
{
public:
	CSVLVersionConvertIF();
	~CSVLVersionConvertIF();

public:

	/*****************************************************************
	函数名	：SVL1To2
	功能	：将SVL1.0版本的数据转换为SVL2.0格式
	参数	：
	Stk_DocumentManager	*pStkDocMgr, SVL1.0文档管理指针
	Stk_Document* pDocument，SVL2.0文档管理指针
	返回值	：转换结果
	更新历史：
	add by likun 2017-06-23
	*****************************************************************/
	static int SVL1ToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);

	static int SVL1ToSVL2(const string& srcPath, const string &targetPath);

	/*****************************************************************
	函数名	：ProtoType1To2
	功能	：将SVL1.0版本的ProtoType数据转换为SVL2.0格式ProtoType
	参数	：
	Stk_ProtoType *pSrcProtoType, SVL1.0版本ProtoType
	Stk_ModelPtr pNewProtoType，SVL2.0版本ProtoType
	返回值	：转换结果
	更新历史：
	add by likun 2017-06-23
	*****************************************************************/
	static int SVL1InstanceToSVL2(Stk_Instance *pSrcProtoType, HoteamSoft::SVLLib::Stk_InstancePtr pNewInstance, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);

	static int SVL1ProtoTypeToSVL2(Stk_ProtoType *pSrcProtoType, HoteamSoft::SVLLib::Stk_ModelPtr pNewProtoType, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1MeshToSVL2(Stk_Mesh_Com* pSrcMeshCom, HoteamSoft::SVLLib::Stk_MeshPrimPtr pMeshPrim, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);

	/*****************************************************************
	函数名	：SVL1PMIToSVL2
	功能	：将SVL1.0版本的PMI数据转换为SVL2.0格式PMI
	参数	：
	pSrcPMI, SVL1.0版本PMI
	pNewPMI, SVL2.0版本PMI
	pDocument 文档
	返回值	：转换结果
	更新历史：
	add by likun 2017-06-23
	*****************************************************************/
	static int SVL1PMIToSVL2(Stk_PMI* pSrcPMI, HoteamSoft::SVLLib::Stk_PMIPtr pNewPMI, 
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
};


#endif //_SVLVERSIONCONVERTIF_H_
