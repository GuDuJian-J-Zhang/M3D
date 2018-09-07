//update by gongcy on 2014/04/19
//添加接口：
//1.GetArcAngleData
//2.GetCircleDrawData

//update by gongcy on 2014/08/11
//添加接口：RectifyArcData
//临时解决角度尺寸引线的问题，调整角度引线的参数

//update by gongcy on 2014/08/12
//添加接口：RectifyExtLinData
//临时解决延长线的标准问题。
//添加接口：RectifyAngleToleranceTxt
//临时解决角度公差丢失度的问题

//update by gongcy on 2014/08/13
//添加接口：RectifyDimensionTolerance
//临时调整尺寸公差的位置缺陷。

//update by gongcy on 2014/09/04
//添加成员方法：ComputeDist
//临时计算文本平移的方向和距离

//update by goncy on 2014/09/09
//添加成员方法：RectifyRugnessTxt
//临时调整表面粗糙度文本的位置

#ifndef SVIEW_COMMON_SVLX_TESSCIRCLE_H
#define SVIEW_COMMON_SVLX_TESSCIRCLE_H

#include "sview/pmiAnalysis/PMIDef.h"

//#include "Stk_Define.h"
//#include "Stk_CurveEntity.h"
//#include "Stk_PMIEntity.h"
#include "SVLLib/Stk_Define.h"
#include "SVLLib/Stk_CurveEntity.h"
#include "SVLLib/Stk_PMIEntity.h"
using HoteamSoft::SVLLib::Stk_ArcPtr;
using HoteamSoft::SVLLib::Stk_LeaderPtr;
using HoteamSoft::SVLLib::STK_DOUBLE64;
using HoteamSoft::SVLLib::STK_INT32;
using HoteamSoft::SVLLib::STK_FLOAT32;
using HoteamSoft::SVLLib::STK_PNT32;
class  SVIEWCOMMON_API  CTessSvlxCircle
{
public:
	CTessSvlxCircle(void);
	~CTessSvlxCircle(void);
	static bool MakeArcDiscretization(HoteamSoft::SVLLib::STK_PNT32 arcCenter, HoteamSoft::SVLLib::STK_FLOAT32 arcRadius, HoteamSoft::SVLLib::STK_DOUBLE64 BeginPara, HoteamSoft::SVLLib::STK_DOUBLE64 EndPara, HoteamSoft::SVLLib::STK_DOUBLE64 arcAngle, HoteamSoft::SVLLib::STK_INT32 neqFlg, std::vector<HoteamSoft::SVLLib::STK_PNT32> &outPnt);
	static bool GetCircleAngleParameter(HoteamSoft::SVLLib::STK_UINT32 dim, HoteamSoft::SVLLib::STK_PNT32 arcPnt, HoteamSoft::SVLLib::STK_PNT32 arcCenter, HoteamSoft::SVLLib::STK_FLOAT32 arcRadius, HoteamSoft::SVLLib::STK_DOUBLE64 &AnglePara);
	static bool GetCircleDiscretization(Stk_ArcPtr arc, HoteamSoft::SVLLib::STK_INT32 drawFlg, std::vector<HoteamSoft::SVLLib::STK_PNT32> &outPnt);
	static bool GetCircleDiscretization(Stk_ArcPtr arc, STK_DOUBLE64 beginPara, STK_DOUBLE64 endPara, STK_DOUBLE64 arcAngle, STK_INT32 drawFlg, std::vector<STK_PNT32> &outPnt);
	static bool GetCircleDrawData
	(
		vector<Stk_LeaderPtr>		leaders,		/*(i) PMI引出线数据				*/
		vector<STK_DOUBLE64>	&vecBeginPar,	/*(o) 圆弧绘制的起始角度参数	*/
		vector<STK_DOUBLE64>	&vecEndPar,		/*(o) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64			&drawAngle,		/*(o) 圆弧绘制角度				*/
		vector<STK_INT32>		&drawFlg		/*(o) 圆弧绘制方向				*/
												/*  ==  1 逆时针				*/
												/*  == -1 顺时针				*/
	);
	static bool GetArcAngleData(
		vector<STK_DOUBLE64>		vecBeginPar,	/*(i) 圆弧绘制的起始角度参数	*/
		vector<STK_DOUBLE64>		vecEndPar,		/*(i) 圆弧绘制的终止角度参数	*/
		vector<STK_FLOAT32>			inNormal_Z,		/*(i) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64				&drawAngle,		/*(o) 圆弧绘制角度				*/
		vector<STK_INT32>			&drawFlg		/*(o) 圆弧绘制方向				*/
													/*  ==  1 逆时针				*/
													/*  == -1 顺时针				*/
	);
};
#endif
