//update by gongcy on 2014/04/19
//��ӽӿڣ�
//1.GetArcAngleData
//2.GetCircleDrawData

//update by gongcy on 2014/08/11
//��ӽӿڣ�RectifyArcData
//��ʱ����Ƕȳߴ����ߵ����⣬�����Ƕ����ߵĲ���

//update by gongcy on 2014/08/12
//��ӽӿڣ�RectifyExtLinData
//��ʱ����ӳ��ߵı�׼���⡣
//��ӽӿڣ�RectifyAngleToleranceTxt
//��ʱ����Ƕȹ��ʧ�ȵ�����

//update by gongcy on 2014/08/13
//��ӽӿڣ�RectifyDimensionTolerance
//��ʱ�����ߴ繫���λ��ȱ�ݡ�

//update by gongcy on 2014/09/04
//��ӳ�Ա������ComputeDist
//��ʱ�����ı�ƽ�Ƶķ���;���

//update by goncy on 2014/09/09
//��ӳ�Ա������RectifyRugnessTxt
//��ʱ��������ֲڶ��ı���λ��

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
		vector<Stk_LeaderPtr>		leaders,		/*(i) PMI����������				*/
		vector<STK_DOUBLE64>	&vecBeginPar,	/*(o) Բ�����Ƶ���ʼ�ǶȲ���	*/
		vector<STK_DOUBLE64>	&vecEndPar,		/*(o) Բ�����Ƶ���ֹ�ǶȲ���	*/
		STK_DOUBLE64			&drawAngle,		/*(o) Բ�����ƽǶ�				*/
		vector<STK_INT32>		&drawFlg		/*(o) Բ�����Ʒ���				*/
												/*  ==  1 ��ʱ��				*/
												/*  == -1 ˳ʱ��				*/
	);
	static bool GetArcAngleData(
		vector<STK_DOUBLE64>		vecBeginPar,	/*(i) Բ�����Ƶ���ʼ�ǶȲ���	*/
		vector<STK_DOUBLE64>		vecEndPar,		/*(i) Բ�����Ƶ���ֹ�ǶȲ���	*/
		vector<STK_FLOAT32>			inNormal_Z,		/*(i) Բ�����Ƶ���ֹ�ǶȲ���	*/
		STK_DOUBLE64				&drawAngle,		/*(o) Բ�����ƽǶ�				*/
		vector<STK_INT32>			&drawFlg		/*(o) Բ�����Ʒ���				*/
													/*  ==  1 ��ʱ��				*/
													/*  == -1 ˳ʱ��				*/
	);
};
#endif
