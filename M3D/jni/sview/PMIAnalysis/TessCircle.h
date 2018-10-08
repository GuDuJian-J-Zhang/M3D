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

#ifndef SVIEW_COMMON_TESSCIRCLE_H
#define SVIEW_COMMON_TESSCIRCLE_H

#include "sview/pmiAnalysis/PMIDef.h"

#include "Stk_Define.h"
#include "Stk_CurveEntity.h"
//#include "SbXfBox3f.h"
//#include "SbMatrix.h"
#include "Stk_PMIEntity.h"

class  SVIEWCOMMON_API  CTessCircle
{
public:
	CTessCircle(void);
	~CTessCircle(void);
	static STK_BOOL MakeArcDiscretization( STK_PNT32 arcCenter,  STK_FLOAT32 arcRadius,  STK_DOUBLE64 BeginPara,  STK_DOUBLE64 EndPara,  STK_DOUBLE64 arcAngle,  STK_INT32 neqFlg,  std::vector<STK_PNT32> &outPnt );
	static STK_BOOL GetCircleAngleParameter( STK_UINT32 dim,  STK_PNT32 arcPnt,  STK_PNT32 arcCenter,  STK_FLOAT32 arcRadius,  STK_DOUBLE64 &AnglePara );
	static STK_BOOL GetCircleDiscretization( Stk_Ellipse *ellipse,  STK_DOUBLE64 beginPara,  STK_DOUBLE64 endPara,  STK_DOUBLE64 arcAngle,  STK_INT32 drawFlg,  std::vector<STK_PNT32> &outPnt );
	static STK_BOOL GetCircleDiscretization( Stk_Ellipse *ellipse,  STK_INT32 drawFlg,  std::vector<STK_PNT32> &outPnt);
	static STK_BOOL GetCircleDrawData
		(
		vector<Stk_Leader*>		leaders,		/*(i) PMI引出线数据				*/
		vector<STK_DOUBLE64>	&vecBeginPar,	/*(o) 圆弧绘制的起始角度参数	*/
		vector<STK_DOUBLE64>	&vecEndPar,		/*(o) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64			&drawAngle,		/*(o) 圆弧绘制角度				*/
		vector<STK_INT32>		&drawFlg		/*(o) 圆弧绘制方向				*/
		/*  ==  1 逆时针				*/
		/*  == -1 顺时针				*/
		);
	static STK_BOOL GetArcAngleData(
		vector<STK_DOUBLE64>		vecBeginPar,	/*(i) 圆弧绘制的起始角度参数	*/
		vector<STK_DOUBLE64>		vecEndPar,		/*(i) 圆弧绘制的终止角度参数	*/
		vector<STK_FLOAT32>			inNormal_Z,		/*(i) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64				&drawAngle,		/*(o) 圆弧绘制角度				*/
		vector<STK_INT32>			&drawFlg		/*(o) 圆弧绘制方向				*/
		/*  ==  1 逆时针				*/
		/*  == -1 顺时针				*/
		);
	//TODO:移植到移动端，此接口暂时注释掉——gcy
	//static STK_BOOL ComputeCircumcircle(
	//	const SbXfBox3f &outerbox,
	//	const STK_PNT32& innerLoc,
	//	Stk_Ellipse & ellipse);
	//
	static void RectifyArcData(Stk_PMI * pStkPMI);
	static void RectifyExtLinData(Stk_PMI* pStkPMI);
	//TODO:移植到移动端，此接口暂时注释掉——gcy
	//static void RectifyExtLinData(SbVec3f * pPnt1,SbVec3f *pPnt2);
	static void RectifyAngleToleranceTxt(Stk_PMI* pStkPMI);
	//TODO:移植到移动端，此接口暂时注释掉——gcy
	//static void RectifyDimensionTolerance(
	//	vector<SbXfBox3f>&vecBoxes,
	//	const vector<Stk_Text*>& vecText,
	//	vector<STK_DIR32>&vecDir,
	//	vector<double>&vecDist,
	//	vector<int>&vecIdx);
	static double RectifyRugnessTxt(const vector<Stk_CurveEntity*>& vecCurveList,const STK_BOX32&txtBox,double offsetdir[3]);
	static double RectifyRugnessTxt2(const vector<Stk_CurveEntity*>& vecCurveList,const STK_BOX32&txtBox,double offsetdir[3]);

	static void UpdateTessCricleData(const vector<Stk_Leader*>& pLeaders,const STK_PNT32& refPnt,
									   Stk_PMI* pDimension,vector<STK_PNT32>&outVecExtPnt);

	static bool IsPntOnArc(const float* i_pPnt,Stk_Ellipse* i_pEllipse);

	//TODO:移植到移动端，此接口暂时注释掉——gcy
	//static bool CheckOutFrameIntersec(const SbBox3f&txtBox,const SbBox3f&FrameBox);
	//TODO:移植到移动端，此接口暂时注释掉——gcy
	//static void FilterOutFrame(Stk_PMI * pStkPMI,vector<SbBox3f>&vecBox);
	//TODO:移植到移动端，此接口暂时注释掉——gcy
	//static SbBox3f* ComputeIntersetBox(const SbBox3f&inBox1,const SbBox3f&inBox2);
	//TODO:移植到移动端，此接口暂时注释掉——gcy
	//static bool CheckBoxEqual(const SbBox3f&inBox1,const SbBox3f&inBox2 );
	//TODO:移植到移动端，此接口暂时注释掉——gcy
	//static bool ComputeOffsetBox(const SbBox3f&inBox1,const SbBox3f&inBox2,double &offsetLen,SbVec3f&offsetDir);
	//TODO:移植到移动端，此接口暂时注释掉——gcy
	//static double ComputeDist(const SbVec3f&pnt1,const SbVec3f &pnt2,SbVec3f &oDir);
	static void   ComputeVecDirByTxt(const STK_BOX32& txtBox,double dirLin[3]);
	static int SetBasLinDir(double curPntA[3],double oldPntA[3]);
};
#endif
