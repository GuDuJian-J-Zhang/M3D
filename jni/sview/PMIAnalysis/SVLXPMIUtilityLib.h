/**********************************************************
*		Created on 2014/04/21 By gongcy
*	All Rights Reserved. Copyright(C) 2014 Sinovation.com
*
*	Function:
*		PMI ʵ�ÿ�
*
*   history
*   1.update on 2017/08/30 by huangderong
*   ���UpdateEndSymData�ӿڣ�����ĩ�˷��ŵ�����
**********************************************************/
#pragma once
#ifndef	___MEASUREANNOTATAION_SVLX_PMI_UTILITY_LIB_DIALOG_H__
#define	___MEASUREANNOTATAION_SVLX_PMI_UTILITY_LIB_DIALOG_H__

#include "SVLLib/Stk_PMIEntity.h"

using HoteamSoft::SVLLib::Stk_LeaderPtr;
using HoteamSoft::SVLLib::STK_UINT32;
using HoteamSoft::SVLLib::STK_PNT32;
using HoteamSoft::SVLLib::STK_BOX32;
using HoteamSoft::SVLLib::STK_BOOL;
#define SVIEWCOMMON_API
class SVIEWCOMMON_API CSVLXPMIUtilityLib
{
public:
	CSVLXPMIUtilityLib(void);
	virtual ~CSVLXPMIUtilityLib(void);

	// ���߶���
	static bool GetLineCrossPoint(
		STK_UINT32	dim,				/*(i) ά��						*/
		STK_PNT32	inLin1_PntA[2],		/*(i) ��1���˵�					*/
		STK_PNT32	inLin2_PntA[2],		/*(i) ��2���˵�					*/
		STK_UINT32	&crossFlg,			/*(o) ƽ��/�غ�/�ཻ			*/
										/*   ==0 ƽ��   ==1 �غ�   ==2 �ཻ		*/
		STK_PNT32	&crossPnt			/*(o) ��������/�غ�ʱ�����1��������	*/
	);
	// ���ĩ�˷��Ż��Ʒ���
	static bool GetEndSymbolDirection(
		vector<Stk_LeaderPtr>		leaders,		/*(i) PMI����������				*/
		vector<HoteamSoft::SVLLib::STK_INT32>		&drawDir		/*(o) Բ�����Ʒ���				*/
												/*  ==  1 ԭ����				*/
												/*  == -1 ������				*/
	);
	// Leader �ü�
	static bool DivideLeader(
		STK_UINT32			dim,		/*(i) ά��						*/
		vector<STK_BOX32> TextBox,	    /*(i) �ı��İ�Χ��������						*/
		vector<STK_PNT32>	inPnt,		/*(i) Leader ��������			*/
		vector<STK_PNT32>	&outPnt//,	/*(o) Leader �ü���ĵ�������	*/
								   //bool bConverted = true
	);
	// �жϵ���������Ƿ����
	static bool CheckPntFrmInter(
		STK_UINT32	dim,				/*(i) ά��						*/
		vector<STK_PNT32> inPntA,		/*(i) ��������					*/
		STK_BOX32	inBox,				/*(i) ���BOX					*/
		vector<STK_BOOL> &outChk		/*(o) ��������				*/
	);
	static STK_BOX32 ResetTextBox(STK_BOX32 inbox);
};
#endif
