/****************************************************************************
��Ȩ���� (C) 2014  ɽ��ɽ����������޹�˾

�ļ�����	SVLVersionConvertor.h
���ܣ�
�ṩSVL���汾�����ݸ�ʽת���ӿ�

���¼�¼��
****************************************************************************/
#ifndef _SVLVERSIONCONVERTIF_H_
#define _SVLVERSIONCONVERTIF_H_

#include "Stk_ProtoType.h"
#include "SVLLib/Stk_Declare_Ptr.h"
#include "SVLLib/Stk_Document.h"

//Ascii���ļ���ȡ�ṹ
class CSVLVersionConvertIF
{
public:
	CSVLVersionConvertIF();
	~CSVLVersionConvertIF();

public:

	/*****************************************************************
	������	��SVL1To2
	����	����SVL1.0�汾������ת��ΪSVL2.0��ʽ
	����	��
	Stk_DocumentManager	*pStkDocMgr, SVL1.0�ĵ�����ָ��
	Stk_Document* pDocument��SVL2.0�ĵ�����ָ��
	����ֵ	��ת�����
	������ʷ��
	add by likun 2017-06-23
	*****************************************************************/
	static int SVL1ToSVL2(Stk_DocumentManager *pStkDocMgr, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);

	static int SVL1ToSVL2(const string& srcPath, const string &targetPath);

	/*****************************************************************
	������	��ProtoType1To2
	����	����SVL1.0�汾��ProtoType����ת��ΪSVL2.0��ʽProtoType
	����	��
	Stk_ProtoType *pSrcProtoType, SVL1.0�汾ProtoType
	Stk_ModelPtr pNewProtoType��SVL2.0�汾ProtoType
	����ֵ	��ת�����
	������ʷ��
	add by likun 2017-06-23
	*****************************************************************/
	static int SVL1InstanceToSVL2(Stk_Instance *pSrcProtoType, HoteamSoft::SVLLib::Stk_InstancePtr pNewInstance, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);

	static int SVL1ProtoTypeToSVL2(Stk_ProtoType *pSrcProtoType, HoteamSoft::SVLLib::Stk_ModelPtr pNewProtoType, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
	static int SVL1MeshToSVL2(Stk_Mesh_Com* pSrcMeshCom, HoteamSoft::SVLLib::Stk_MeshPrimPtr pMeshPrim, HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);

	/*****************************************************************
	������	��SVL1PMIToSVL2
	����	����SVL1.0�汾��PMI����ת��ΪSVL2.0��ʽPMI
	����	��
	pSrcPMI, SVL1.0�汾PMI
	pNewPMI, SVL2.0�汾PMI
	pDocument �ĵ�
	����ֵ	��ת�����
	������ʷ��
	add by likun 2017-06-23
	*****************************************************************/
	static int SVL1PMIToSVL2(Stk_PMI* pSrcPMI, HoteamSoft::SVLLib::Stk_PMIPtr pNewPMI, 
		HoteamSoft::SVLLib::Stk_DocumentPtr pDocument);
};


#endif //_SVLVERSIONCONVERTIF_H_
