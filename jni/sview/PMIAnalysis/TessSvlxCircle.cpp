//update by gongcy on 2014/04/19
//添加接口：
//1.GetArcAngleData
//2.GetCircleDrawData

//update by gongcy on 2014/08/11
//添加接口：RectifyArcData
//临时解决角度尺寸引线的问题，调整角度引线的参数

#include "TessSvlxCircle.h"
#include "toolkit/MathVector.h"
#include "toolkit/MathGeolib.h"
//#include "PMIUtilityLib.h"
//#include <Inventor/SbLine.h>
#include <math.h>
#include<stdlib.h>
#include<wchar.h>


#define IDXNUM			(16384)
#define ZDEPS			(1.0e-18)
#define MAXPNTVAL		(1.0e+8)
#define ZTOL			(2e-10)		// 干涉公差
#define ANGLEPARA_TOL	(0.1)	// 角度参数公差
#define ARAAYNUM		(4)
#define PMIDIM2			(2)			// 二维
#define DIVANGLE		(2)			// 离散最小角度
#define	PI		(3.14159265358979323846)


using namespace std;
using namespace HoteamSoft::SVLLib;

CTessSvlxCircle::CTessSvlxCircle(void)
{

}
CTessSvlxCircle::~CTessSvlxCircle(void)
{

}

/*
* 功能：获取圆弧离散点列

* 参数：

*       ellipse               (I) 圆弧线

*       drawFlg               (I) 圆弧绘制方向

*       outPnt[2]             (O) 分元后的点列数据

* 返回值： 执行成功/失败

* 2013-09-05   Liutp   created
*/
bool CTessSvlxCircle::GetCircleDiscretization(
	Stk_ArcPtr          arc,	/*(i) 圆弧线					*/
	HoteamSoft::SVLLib::STK_INT32			drawFlg,	/*(i) 圆弧绘制方向				*/
									/*  ==  1 逆时针				*/
									/*  == -1 顺时针				*/
	vector<HoteamSoft::SVLLib::STK_PNT32>	&outPnt		/*(o) 分元后的点列数据			*/
) {
	HoteamSoft::SVLLib::STK_FLOAT32			MajorRadius, MinorRadius;	// 短半径、长半径
	HoteamSoft::SVLLib::STK_PNT32			Center;						// 中心点
	HoteamSoft::SVLLib::STK_PNT32			BeginPnt, EndPnt;			// 起始点坐标值
	HoteamSoft::SVLLib::STK_DIR32			inNormal;					// Z方向向量，判断圆弧绘制方向
	STK_DOUBLE64		BeginPara, EndPara;			// 起始点/终点的角度参数
	STK_INT32			neqFlg;						// 绘制方向

	arc->GetRadius(MajorRadius, MinorRadius);
	Center = arc->GetCenterPoint();
	arc->GetCoordinatePnt(BeginPnt, EndPnt);
	inNormal = arc->GetNormal();

	/*
	*	按照参数方程计算角度参数
	*/
	GetCircleAngleParameter(PMIDIM2, BeginPnt, Center, MinorRadius, BeginPara);
	GetCircleAngleParameter(PMIDIM2, EndPnt, Center, MinorRadius, EndPara);

	/*
	*	按照角度参数和实际角度进行圆弧折线化计算
	*/
	neqFlg = drawFlg == 0 ? (inNormal.Z > 0.0 ? +1 : -1) : drawFlg;

	return MakeArcDiscretization(Center, MinorRadius, BeginPara, EndPara, 0.0, neqFlg, outPnt);

}

/*
* 功能：圆弧离散化

* 参数：

*       ellipse               (i) 圆弧线

*       beginPara,            (i) 圆弧绘制的起始角度参数

*       endPara,              (i) 圆弧绘制的终止角度参数

*       arcAngle,             (i) 圆弧实际角度

*       drawFlg               (I) 圆弧绘制方向

*       outPnt                (o) 分元后的点列数据

* 返回值： 执行成功/失败

* 2013-09-05   Liutp   created
*/
bool CTessSvlxCircle::GetCircleDiscretization(
	Stk_ArcPtr			arc,	/*(i) 圆弧线					*/
	STK_DOUBLE64		beginPara,	/*(i) 圆弧绘制的起始角度参数	*/
	STK_DOUBLE64		endPara,	/*(i) 圆弧绘制的终止角度参数	*/
	STK_DOUBLE64		arcAngle,	/*(i) 圆弧实际角度				*/
	STK_INT32			drawFlg,	/*(i) 圆弧绘制方向				*/
									/*  ==  1 逆时针				*/
									/*  == -1 顺时针				*/
	vector<STK_PNT32>	&outPnt		/*(o) 分元后的点列数据			*/
) {
	STK_FLOAT32			MajorRadius, MinorRadius;	// 短半径、长半径
	STK_PNT32			Center;						// 中心点
	STK_DIR32			inNormal;					// Z方向向量，判断圆弧绘制方向
	STK_INT32			neqFlg;						// 绘制方向

	arc->GetRadius(MajorRadius, MinorRadius);
	Center = arc->GetCenterPoint();
	inNormal = arc->GetNormal();

	// 获取绘制方向
	neqFlg = drawFlg == 0 ? (inNormal.Z > 0.0 ? +1 : -1) : drawFlg;

	/*
	*	按照绘制方向计算计算点列
	*/
	return MakeArcDiscretization(Center, MinorRadius, beginPara, endPara, arcAngle, neqFlg, outPnt);

}

STK_BOOL CTessSvlxCircle::GetCircleAngleParameter(
	HoteamSoft::SVLLib::STK_UINT32 dim,
	HoteamSoft::SVLLib::STK_PNT32 arcPnt,
	HoteamSoft::SVLLib::STK_PNT32 arcCenter,
	HoteamSoft::SVLLib::STK_FLOAT32 arcRadius,
	HoteamSoft::SVLLib::STK_DOUBLE64 &AnglePara
)
{
	STK_DOUBLE64		cosAngle;

	if (dim != PMIDIM2) {
		return false;
	}

	/*
	*	计算cosθ值
	*/
	cosAngle = (arcPnt.X - arcCenter.X) / arcRadius;

	if (cosAngle > 1) {
		if ((cosAngle - 1) < ANGLEPARA_TOL) {
			// 0°时设定为360°
			AnglePara = PI * 2;
			return true;
		}
		else {
			return false;
		}
	}
	else if (cosAngle < -1) {
		if (fabs(1 + cosAngle) < ANGLEPARA_TOL) {
			// cosθ<-1时，公差范围内作为180°
			AnglePara = PI;
			return true;
		}
		else {
			return false;
		}
	}

	/*
	*	角度参数计算arccosθ(三四象限进行特殊处理)
	*/
	if ((arcPnt.X - arcCenter.X) < 0.0 && (arcPnt.Y - arcCenter.Y) < 0.0 ||
		(arcPnt.X - arcCenter.X) > 0.0 && (arcPnt.Y - arcCenter.Y) < 0.0) {
		AnglePara = PI * 2 - acos(cosAngle);
	}
	else if ((arcPnt.X - arcCenter.X == 0.0) && (arcPnt.Y - arcCenter.Y) < 0.0) {
		AnglePara = PI + acos(cosAngle);
	}
	else {
		AnglePara = acos(cosAngle);
	}

	return true;
}

/*
* 功能：圆弧离散化

* 参数：

*       arcCenter,            (i) 圆心点坐标

*       arcRadius,            (i) 圆弧半径

*       beginPara,            (i) 圆弧绘制的起始角度参数

*       endPara,              (i) 圆弧绘制的终止角度参数

*       arcAngle,             (i) 圆弧实际角度

*       drawFlg               (i) 圆弧绘制方向

*       outPnt                (o) 分元后的点列数据

* 返回值： 执行成功/失败

* 2013-09-09   Liutp   created
*/
bool CTessSvlxCircle::MakeArcDiscretization(
	HoteamSoft::SVLLib::STK_PNT32 arcCenter,      /*(i) 圆心点坐标				*/
	HoteamSoft::SVLLib::STK_FLOAT32 arcRadius,    /*(i) 圆弧半径					*/
	HoteamSoft::SVLLib::STK_DOUBLE64 BeginPara,   /*(i) 圆弧绘制的起始角度参数	*/
	HoteamSoft::SVLLib::STK_DOUBLE64 EndPara,     /*(i) 圆弧绘制的终止角度参数	*/
	HoteamSoft::SVLLib::STK_DOUBLE64 arcAngle,    /*(i) 圆弧实际角度				*/
	HoteamSoft::SVLLib::STK_INT32 neqFlg,         /*(i) 圆弧绘制方向				*/
											      /*  ==  1 逆时针				*/
											      /*  == -1 顺时针				*/
	std::vector<HoteamSoft::SVLLib::STK_PNT32> &outPnt)   /*(o) 分元后的点列数据			*/
{
	STK_PNT32		cpnt;						// 圆弧上的点
	STK_DOUBLE64	pointpar, tmPntPar;
	STK_DOUBLE64	tangleP = 0.0;

	// 计算弧度 通过角度参数计算弧度
	if (arcAngle > 0.0) {
		tangleP = arcAngle;
	}
	else {
		// 计算弧度
		if (EndPara > BeginPara) {
			if (neqFlg > 0) {
				tangleP = EndPara - BeginPara;
			}
			else {
				tangleP = (PI * 2 - EndPara) + BeginPara;
			}
		}
		else if (EndPara < BeginPara) {
			if (neqFlg > 0) {
				tangleP = (PI * 2 - BeginPara) + EndPara;
			}
			else {
				tangleP = BeginPara - EndPara;
			}
		}
		else if (EndPara == BeginPara) {
			tangleP = PI * 2;
		}
	}

	//修改设置角度10°->5°为分元参数		by Liutp on 2013.09.04
	STK_DOUBLE64 DisTangle = DIVANGLE*(PI / 180);
	STK_INT32    dotpntnum = (STK_INT32)(tangleP / DisTangle);

	if (dotpntnum < 4) {
		dotpntnum = 4;
		DisTangle = tangleP / 4;
	}

	// 按照绘制方向绘制圆弧
	for (int j = 0; j < dotpntnum + 1; j++) {
		pointpar = BeginPara + neqFlg*DisTangle*j;
		if (pointpar < 0.0) {
			tmPntPar = PI * 2 + pointpar;
		}
		else if (pointpar > PI * 2) {
			tmPntPar = pointpar - PI * 2;
		}
		else {
			tmPntPar = pointpar;
		}
		// 终点处理
		if (j == dotpntnum) {
			tmPntPar = EndPara;
		}
		cpnt.X = (STK_FLOAT32)(arcCenter.X + arcRadius*cos(tmPntPar));
		cpnt.Y = (STK_FLOAT32)(arcCenter.Y + arcRadius*sin(tmPntPar));
		cpnt.Z = 0.0;
		outPnt.push_back(cpnt);
	}

	return true;
}

/*
* 功能：计算圆弧绘制方向

* 参数：

*       leaders               (i) PMI引出线数据

*       vecBeginPar           (o) 圆弧绘制的起始角度参数

*       vecEndPar             (o) 圆弧绘制的终止角度参数

*       drawAngle             (o) 圆弧绘制角度

*       drawFlg               (o) 圆弧绘制方向

* 返回值： 执行成功/失败

* 2013-09-05   Liutp   created
*/
STK_BOOL CTessSvlxCircle::GetCircleDrawData(
	vector<Stk_LeaderPtr>		leaders,		/*(i) PMI引出线数据				*/
	vector<STK_DOUBLE64>	&vecBeginPar,	/*(o) 圆弧绘制的起始角度参数	*/
	vector<STK_DOUBLE64>	&vecEndPar,		/*(o) 圆弧绘制的终止角度参数	*/
	STK_DOUBLE64			&drawAngle,		/*(o) 圆弧绘制角度				*/
	vector<STK_INT32>		&drawFlg		/*(o) 圆弧绘制方向				*/
											/*  ==  1 逆时针				*/
											/*  == -1 顺时针				*/
) {
	Stk_ArcPtr          arc;                        // 圆弧线
	//Stk_Ellipse			*ellipse;					// 圆弧线
	STK_PNT32			BeginPnt, EndPnt;			// 起始点坐标值
	STK_FLOAT32			MajorRadius, MinorRadius;	// 短半径、长半径
	STK_PNT32			Center;						// 中心点
	STK_DIR32			inNormal;					// Z方向向量，判断圆弧绘制方向
	STK_DOUBLE64		BeginPara, EndPara;			// 起始点/终点的角度参数
	vector<STK_FLOAT32>	inNormal_Z;

	/*
	*	按照参数方程计算各圆弧的角度参数
	*/
	for (int i = 0; i<leaders.size(); i++) {
		Stk_LeaderPtr leader = leaders.at(i);		//获得一条引出线数据
		STK_UINT32 type = leader->GetLeaderType();	//获得引出线类型

		if (type == 2) {
			vector<Stk_CurveEntityPtr> CurveList = leader->GetAllCurve();
			if (CurveList.size() == 0)
			{
				return false;
			}
			Stk_CurveEntityPtr pCurve = CurveList[CurveList.size() - 1];
			arc = Stk_ArcPtr::DynamicCast(pCurve);

			arc->GetRadius(MajorRadius, MinorRadius);
			arc->GetCoordinatePnt(BeginPnt, EndPnt);
			Center = arc->GetCenterPoint();
			inNormal = arc->GetNormal();

			GetCircleAngleParameter(PMIDIM2, BeginPnt, Center, MinorRadius, BeginPara);
			GetCircleAngleParameter(PMIDIM2, EndPnt, Center, MinorRadius, EndPara);

			inNormal_Z.push_back(inNormal.Z);
			vecBeginPar.push_back(BeginPara);
			vecEndPar.push_back(EndPara);
		}
		else {
			return false;
		}
	}

	/*
	*	计算绘制方向和实际角度
	*/
	return GetArcAngleData(vecBeginPar, vecEndPar, inNormal_Z, drawAngle, drawFlg);

}

/*
* 功能：计算圆弧绘制方向

* 参数：

*       vecBeginPar           (i) 圆弧绘制的起始角度参数

*       vecEndPar             (i) 圆弧绘制的终止角度参数

*       drawAngle             (o) 圆弧绘制角度

*       drawFlg               (o) 圆弧绘制方向

* 返回值： 执行成功/失败

* 2013-09-09   Liutp   created
*/
STK_BOOL CTessSvlxCircle::GetArcAngleData(
	vector<STK_DOUBLE64>		vecBeginPar,	/*(i) 圆弧绘制的起始角度参数	*/
	vector<STK_DOUBLE64>		vecEndPar,		/*(i) 圆弧绘制的终止角度参数	*/
	vector<STK_FLOAT32>			inNormal_Z,		/*(i) 圆弧绘制的终止角度参数	*/
	STK_DOUBLE64				&drawAngle,		/*(o) 圆弧绘制角度				*/
	vector<STK_INT32>			&drawFlg		/*(o) 圆弧绘制方向				*/
												/*  ==  1 逆时针				*/
												/*  == -1 顺时针				*/
) {
	STK_DOUBLE64		EndPara;					// 起始点/终点的角度参数
	STK_INT32			neqFlg;						// 绘制方向
	STK_INT32			neqFlgA[2];					// 绘制方向
	STK_DOUBLE64		RealTangle;
	STK_DOUBLE64		minPara, maxPara;
	STK_DOUBLE64		tangle1, tangle2;
	STK_BOOL			outFlg = false;

	if (vecBeginPar.size() != 2 ||
		vecEndPar.size() != 2 ||
		inNormal_Z.size() != 2) {
		return false;
	}

	/* θ1>θ2时，确定绘制角度 */
	if (vecBeginPar.at(0) > vecBeginPar.at(1)) {
		minPara = vecBeginPar.at(1);
		maxPara = vecBeginPar.at(0);
		// 实际角度表达方向θ2 -> θ1		注：以逆时针方向为主，下同
		if (inNormal_Z.at(0) < 0.0 && inNormal_Z.at(1) > 0.0) {
			// 有效区间[θ2,θ1]，实际角度θ1-θ2
			RealTangle = vecBeginPar.at(0) - vecBeginPar.at(1);
			for (int j = 0; j<vecEndPar.size(); j++) {
				EndPara = vecEndPar.at(j);
				neqFlg = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ((EndPara >= minPara) && (EndPara <= maxPara)) {
					// 按照原方向绘制
					// neqFlg = inNormal_Z.at(j)>0.0 ? +1 : -1;
				}
				else {
					// 与原方向反向
					// neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = true;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
			// 实际角度表达方向θ1 -> θ2
		}
		else if (inNormal_Z.at(0) > 0.0 && inNormal_Z.at(1) < 0.0) {
			// 有效区间[θ1,2π] ∪ [0,θ2]，实际角度(2π-θ1)+θ2
			RealTangle = (PI * 2 - vecBeginPar.at(0)) + vecBeginPar.at(1);
			for (int j = 0; j<vecEndPar.size(); j++) {
				EndPara = vecEndPar.at(j);
				neqFlg = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ((EndPara >= maxPara) && (EndPara <= PI * 2) ||
					(EndPara >= 0.0) && (EndPara <= minPara)) {
					// 按照原方向绘制
					// neqFlg = inNormal_Z.at(j);
				}
				else {
					// 与原方向反向
					//neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = true;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
		}
		else {
			return false;
		}
		// END点在角度区间之外时特殊处理
		if (outFlg) {
			neqFlgA[0] = inNormal_Z.at(0)>0.0 ? +1 : -1;
			neqFlgA[1] = inNormal_Z.at(1)>0.0 ? +1 : -1;
			tangle1 = fabs(EndPara - vecBeginPar.at(0));
			tangle2 = fabs(EndPara - vecBeginPar.at(1));	// 
			if (EndPara > maxPara) {
				if (tangle1 + RealTangle / 2 < PI) {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : +1;
				}
				else {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : +1;
				}
			}
			else if (EndPara < minPara) {
				if (tangle2 + RealTangle / 2 < PI) {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : +1;
				}
				else {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : +1;
				}
			}
			drawFlg.clear();
			for (int k = 0; k<2; k++) {
				drawFlg.push_back(neqFlgA[k]);
			}
		}
		/* θ1<θ2时，确定绘制角度 */
	}
	else if (vecBeginPar.at(0) < vecBeginPar.at(1)) {
		minPara = vecBeginPar.at(0);
		maxPara = vecBeginPar.at(1);
		// 实际角度表达方向θ2 -> θ1，实际角度θ2-θ1
		RealTangle = vecBeginPar.at(1) - vecBeginPar.at(0);
		if (inNormal_Z.at(0) > 0.0 && inNormal_Z.at(1) < 0.0) {
			// 有效区间[θ1,θ2]
			for (int j = 0; j<vecEndPar.size(); j++) {
				EndPara = vecEndPar.at(j);
				neqFlg = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ((EndPara >= minPara) && (EndPara <= maxPara)) {
					// 按照原方向绘制
					//neqFlg = inNormal_Z.at(j);
				}
				else {
					// 与原方向反向
					//neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = true;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
			// 实际角度表达方向θ1 -> θ2
		}
		else if (inNormal_Z.at(0) < 0.0 && inNormal_Z.at(1) > 0.0) {
			// 有效区间[θ2,2π] ∪ [0,θ1]，实际角度(2π-θ2)+θ1
			RealTangle = (PI * 2 - vecBeginPar.at(1)) + vecBeginPar.at(0);
			for (int j = 0; j<vecEndPar.size(); j++) {
				EndPara = vecEndPar.at(j);
				neqFlg = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ((EndPara >= maxPara) && (EndPara <= PI * 2) ||
					(EndPara >= 0.0) && (EndPara <= minPara)) {
					// 按照原方向绘制
					//neqFlg = inNormal_Z.at(j);
				}
				else {
					// 与原方向反向
					//neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = true;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
		}
		else {
			return false;
		}
		// END点在角度区间之外时特殊处理
		if (outFlg) {
			neqFlgA[0] = inNormal_Z.at(0)>0.0 ? +1 : -1;
			neqFlgA[1] = inNormal_Z.at(1)>0.0 ? +1 : -1;
			tangle1 = fabs(EndPara - vecBeginPar.at(0));
			tangle2 = fabs(EndPara - vecBeginPar.at(1));	// 
			if (EndPara > maxPara) {
				if (tangle2 + RealTangle / 2 < PI) {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : +1;
				}
				else {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : +1;
				}
			}
			else if (EndPara < minPara) {
				if (tangle1 + RealTangle / 2 < PI) {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : 1;
				}
				else {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : 1;
				}
			}
			drawFlg.clear();
			for (int k = 0; k<2; k++) {
				drawFlg.push_back(neqFlgA[k]);
			}
		}
	}
	else {
		return false;
	}

	drawAngle = RealTangle;

	// 结果检查
	return (!drawFlg.empty());

}
