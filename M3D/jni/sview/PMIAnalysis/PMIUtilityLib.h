/**********************************************************
*		Created on 2014/04/21 By gongcy
*	All Rights Reserved. Copyright(C) 2014 Sinovation.com
*
*	Function:
*		PMI 实用库
*	
*   history
*   1.update on 2014/08/26 by gongcy
*   添加UpdateEndSymData接口，更新末端符号的数据		
**********************************************************/
#pragma once
#ifndef	___MEASUREANNOTATAION_PMI_UTILITY_LIB_DIALOG_H__
#define	___MEASUREANNOTATAION_PMI_UTILITY_LIB_DIALOG_H__
//#include "Stk_Instance.h"
//#include "Stk_PMIEntity.h"
//#include "Stk_CurveEntity.h"
#include <prebuilt/svl2/include/Stk_Instance.h>
#include <prebuilt/svl2/include/Stk_CurveEntity.h>
#include <prebuilt/svl2/include/Stk_PMIEntity.h>
/*
#ifdef SVIEWCOMMON_EXPORTS
#define SVIEWCOMMON_API __declspec(dllexport)
#else
#define SVIEWCOMMON_API __declspec(dllimport)
#endif
*/
#define SVIEWCOMMON_API
class SVIEWCOMMON_API CPMIUtilityLib
{
public:
	CPMIUtilityLib(void);
	virtual ~CPMIUtilityLib(void);
	// 两线段求交
	static STK_BOOL GetLineCrossPoint(
		STK_UINT32	dim,				/*(i) 维度						*/
		STK_PNT32	inLin1_PntA[2],		/*(i) 线1两端点					*/
		STK_PNT32	inLin2_PntA[2],		/*(i) 线2两端点					*/
		STK_UINT32	&crossFlg,			/*(o) 平行/重合/相交			*/
		/*   ==0 平行   ==1 重合   ==2 相交		*/
		STK_PNT32	&crossPnt			/*(o) 交点坐标/重合时输出线1顶点坐标	*/
		);
	// 获得末端符号绘制方向
	static STK_BOOL GetEndSymbolDirection(
		vector<Stk_Leader*>		leaders,		/*(i) PMI引出线数据				*/
		vector<STK_INT32>		&drawDir		/*(o) 圆弧绘制方向				*/
		/*  ==  1 原方向				*/
		/*  == -1 反方向				*/
		);
	// Leader 裁剪
	static STK_BOOL DivideLeader(
		STK_UINT32			dim,		/*(i) 维度						*/
		vector<STK_BOX32> TextBox,	    /*(i) 文本的包围盒子数组						*/
		vector<STK_PNT32>	inPnt,		/*(i) Leader 点列数据			*/
		vector<STK_PNT32>	&outPnt//,	/*(o) Leader 裁剪后的点列数据	*/
		//bool bConverted = true
		);
	// 判断点列与外框是否干涉
	static STK_BOOL CheckPntFrmInter(
		STK_UINT32	dim,				/*(i) 维度						*/
		vector<STK_PNT32> inPntA,		/*(i) 点列坐标					*/
		STK_BOX32	inBox,				/*(i) 外框BOX					*/
		vector<STK_BOOL> &outChk		/*(o) 干涉检查结果				*/
		);


	static STK_BOX32 ResetTextBox( STK_BOX32 inbox );
	static vector<STK_BOX32> GetTextBox(Stk_ComText *i_pComText);		/*	获取文字包围盒的坐标点		*/
	static void UpdateEndSymData(
		Stk_PMI* ipDimSym,
		Stk_Leader* ipStkLeader,
		StkTermTypeEnum&	oTermType,		//末端符号类型
		STK_FLOAT32			oSize[2],		//末端符号宽和高
		STK_PNT32&			oLoc,			//末端符号起始位置
		STK_DIR32&			oDir		//末端符号方向
		);
	static void ResetEndSymData(Stk_Leader* ipStkLeader,
								STK_PNT32&			ioLoc,			//末端符号起始位置
								STK_DIR32&			ioDir);
};
#endif
