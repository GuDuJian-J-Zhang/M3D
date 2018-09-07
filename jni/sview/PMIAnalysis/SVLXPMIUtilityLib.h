/**********************************************************
*		Created on 2014/04/21 By gongcy
*	All Rights Reserved. Copyright(C) 2014 Sinovation.com
*
*	Function:
*		PMI 实用库
*
*   history
*   1.update on 2017/08/30 by huangderong
*   添加UpdateEndSymData接口，更新末端符号的数据
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

	// 两线段求交
	static bool GetLineCrossPoint(
		STK_UINT32	dim,				/*(i) 维度						*/
		STK_PNT32	inLin1_PntA[2],		/*(i) 线1两端点					*/
		STK_PNT32	inLin2_PntA[2],		/*(i) 线2两端点					*/
		STK_UINT32	&crossFlg,			/*(o) 平行/重合/相交			*/
										/*   ==0 平行   ==1 重合   ==2 相交		*/
		STK_PNT32	&crossPnt			/*(o) 交点坐标/重合时输出线1顶点坐标	*/
	);
	// 获得末端符号绘制方向
	static bool GetEndSymbolDirection(
		vector<Stk_LeaderPtr>		leaders,		/*(i) PMI引出线数据				*/
		vector<HoteamSoft::SVLLib::STK_INT32>		&drawDir		/*(o) 圆弧绘制方向				*/
												/*  ==  1 原方向				*/
												/*  == -1 反方向				*/
	);
	// Leader 裁剪
	static bool DivideLeader(
		STK_UINT32			dim,		/*(i) 维度						*/
		vector<STK_BOX32> TextBox,	    /*(i) 文本的包围盒子数组						*/
		vector<STK_PNT32>	inPnt,		/*(i) Leader 点列数据			*/
		vector<STK_PNT32>	&outPnt//,	/*(o) Leader 裁剪后的点列数据	*/
								   //bool bConverted = true
	);
	// 判断点列与外框是否干涉
	static bool CheckPntFrmInter(
		STK_UINT32	dim,				/*(i) 维度						*/
		vector<STK_PNT32> inPntA,		/*(i) 点列坐标					*/
		STK_BOX32	inBox,				/*(i) 外框BOX					*/
		vector<STK_BOOL> &outChk		/*(o) 干涉检查结果				*/
	);
	static STK_BOX32 ResetTextBox(STK_BOX32 inbox);
};
#endif
