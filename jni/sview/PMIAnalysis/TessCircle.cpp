//update by gongcy on 2014/04/19
//添加接口：
//1.GetArcAngleData
//2.GetCircleDrawData

//update by gongcy on 2014/08/11
//添加接口：RectifyArcData
//临时解决角度尺寸引线的问题，调整角度引线的参数

#include "TessCircle.h"
#include "toolkit/MathVector.h"
#include "toolkit/MathGeolib.h"
#include "PMIUtilityLib.h"
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
/*
 * 功能：获取圆弧离散点列

 * 参数：

 *       ellipse               (I) 圆弧线

 *       drawFlg               (I) 圆弧绘制方向

 *       outPnt[2]             (O) 分元后的点列数据

 * 返回值： 执行成功/失败

 * 2013-09-05   Liutp   created
*/
STK_BOOL CTessCircle::GetCircleDiscretization(
		Stk_Ellipse			*ellipse,	/*(i) 圆弧线					*/
		STK_INT32			drawFlg,	/*(i) 圆弧绘制方向				*/
										/*  ==  1 逆时针				*/
										/*  == -1 顺时针				*/
		vector<STK_PNT32>	&outPnt		/*(o) 分元后的点列数据			*/
){
	STK_FLOAT32			MajorRadius,MinorRadius;	// 短半径、长半径
	STK_PNT32			Center;						// 中心点
	STK_PNT32			BeginPnt,EndPnt;			// 起始点坐标值
	STK_DIR32			inNormal;					// Z方向向量，判断圆弧绘制方向
	STK_DOUBLE64		BeginPara,EndPara;			// 起始点/终点的角度参数
	STK_INT32			neqFlg;						// 绘制方向

	ellipse->GetRadius(MajorRadius,MinorRadius);
	Center = ellipse->GetCenterPoint();
	ellipse->GetCoordinatePnt(BeginPnt,EndPnt);
	inNormal = ellipse->GetNormal();

	/*
	 *	按照参数方程计算角度参数
	 */
	GetCircleAngleParameter(PMIDIM2, BeginPnt, Center, MinorRadius, BeginPara);
	GetCircleAngleParameter(PMIDIM2, EndPnt, Center, MinorRadius, EndPara);

	/*
	 *	按照角度参数和实际角度进行圆弧折线化计算
	 */
	neqFlg = drawFlg == 0 ? (inNormal.Z > 0.0 ? +1 : -1) : drawFlg;

	return MakeArcDiscretization( Center, MinorRadius, BeginPara, EndPara, 0.0, neqFlg, outPnt );

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
STK_BOOL CTessCircle::GetCircleDiscretization(
		Stk_Ellipse			*ellipse,	/*(i) 圆弧线					*/
		STK_DOUBLE64		beginPara,	/*(i) 圆弧绘制的起始角度参数	*/
		STK_DOUBLE64		endPara,	/*(i) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64		arcAngle,	/*(i) 圆弧实际角度				*/
		STK_INT32			drawFlg,	/*(i) 圆弧绘制方向				*/
										/*  ==  1 逆时针				*/
										/*  == -1 顺时针				*/
		vector<STK_PNT32>	&outPnt		/*(o) 分元后的点列数据			*/
){
	STK_FLOAT32			MajorRadius,MinorRadius;	// 短半径、长半径
	STK_PNT32			Center;						// 中心点
	STK_DIR32			inNormal;					// Z方向向量，判断圆弧绘制方向
	STK_INT32			neqFlg;						// 绘制方向

	ellipse->GetRadius(MajorRadius,MinorRadius);
	Center = ellipse->GetCenterPoint();
	inNormal = ellipse->GetNormal();

	// 获取绘制方向
	neqFlg = drawFlg == 0 ? (inNormal.Z > 0.0 ? +1 : -1) : drawFlg;

	/*
	 *	按照绘制方向计算计算点列
	 */
	return MakeArcDiscretization( Center, MinorRadius, beginPara, endPara, arcAngle, neqFlg, outPnt );

}

/*
 * 功能：根据中心点、半径计算参数方程中对应的参数

 * 参数方程：x = cosθ+a   y = sinθ+b

 * 参数：dim                   (I) 维度

 *       arcPnt                (I) 圆弧线上点

 *       arcCenter             (I) 圆心点坐标

 *       arcRadius             (I) 圆弧半径

 *       AnglePara             (O) 对应的角度参数

 * 返回值： 执行成功/失败

 * 2013-09-07   Liutp   created
*/

STK_BOOL CTessCircle::GetCircleAngleParameter(
		STK_UINT32			dim,				/*(i) 维度						*/
		STK_PNT32			arcPnt,				/*(i) 圆弧线上点				*/
		STK_PNT32			arcCenter,			/*(i) 圆心点坐标				*/
		STK_FLOAT32			arcRadius,			/*(i) 圆弧半径					*/
		STK_DOUBLE64		&AnglePara			/*(o) 对应的角度参数			*/
	){
	STK_DOUBLE64		cosAngle;

	if ( dim != PMIDIM2 ) {
		return FALSE;
	}

	/*
	 *	计算cosθ值
	 */
	cosAngle = (arcPnt.X - arcCenter.X)/arcRadius;

	if ( cosAngle > 1 ) {
		if ( (cosAngle - 1) < ANGLEPARA_TOL ) {
			// 0°时设定为360°
			AnglePara = PI*2;
			return TRUE;
		} else {
			return FALSE;
		}
	} else if ( cosAngle < -1 ) {
		if ( fabs(1 + cosAngle) < ANGLEPARA_TOL ) {
			// cosθ<-1时，公差范围内作为180°
			AnglePara = PI;
			return TRUE;
		} else {
			return FALSE;
		}
	}

	/*
	 *	角度参数计算arccosθ(三四象限进行特殊处理)
	 */
	if ( (arcPnt.X - arcCenter.X) < 0.0 && (arcPnt.Y - arcCenter.Y) < 0.0  ||
		 (arcPnt.X - arcCenter.X) > 0.0 && (arcPnt.Y - arcCenter.Y) < 0.0  ) {
		AnglePara = PI*2 - acos(cosAngle);
	} else if ( (arcPnt.X - arcCenter.X == 0.0) && (arcPnt.Y - arcCenter.Y) < 0.0 ) {
		AnglePara = PI + acos(cosAngle);
	} else {
		AnglePara = acos(cosAngle);
	}

	return TRUE;
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

STK_BOOL CTessCircle::MakeArcDiscretization(
		STK_PNT32			arcCenter,			/*(i) 圆心点坐标				*/
		STK_FLOAT32			arcRadius,			/*(i) 圆弧半径					*/
		STK_DOUBLE64		BeginPara,			/*(i) 圆弧绘制的起始角度参数	*/
		STK_DOUBLE64		EndPara,			/*(i) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64		arcAngle,			/*(i) 圆弧实际角度				*/
		STK_INT32			neqFlg,				/*(i) 圆弧绘制方向				*/
												/*  ==  1 逆时针				*/
												/*  == -1 顺时针				*/
		vector<STK_PNT32>	&outPnt				/*(o) 分元后的点列数据			*/
){
	STK_PNT32		cpnt;						// 圆弧上的点
	STK_DOUBLE64	pointpar, tmPntPar;
	STK_DOUBLE64	tangleP = 0.0;

	// 计算弧度 通过角度参数计算弧度
	if ( arcAngle > 0.0 ) {
		tangleP = arcAngle;
	} else {
		// 计算弧度
		if ( EndPara > BeginPara ) {
			if ( neqFlg > 0 ) {
				tangleP = EndPara - BeginPara;
			} else {
				tangleP = (PI*2-EndPara)+BeginPara;
			}
		} else if ( EndPara < BeginPara ) {
			if ( neqFlg > 0 ) {
				tangleP = (PI*2-BeginPara)+EndPara;
			} else {
				tangleP = BeginPara - EndPara;
			}
		} else if ( EndPara == BeginPara ) {
			tangleP = PI*2;
		}
	}

	//修改设置角度10°->5°为分元参数		by Liutp on 2013.09.04
	STK_DOUBLE64 DisTangle = DIVANGLE*(PI/180);
	STK_INT32    dotpntnum = (STK_INT32)(tangleP/DisTangle);

	if ( dotpntnum < 4 ) {
		dotpntnum = 4;
		DisTangle = tangleP/4;
	}

	// 按照绘制方向绘制圆弧
	for ( int j = 0; j < dotpntnum+1; j++) {
		pointpar = BeginPara + neqFlg*DisTangle*j;
		if ( pointpar < 0.0 ) {
			tmPntPar = PI*2 + pointpar;
		} else if ( pointpar > PI*2 ) {
			tmPntPar = pointpar - PI*2;
		} else {
			tmPntPar = pointpar;
		}
		// 终点处理
		if ( j == dotpntnum ) {
			tmPntPar = EndPara;
		}
		cpnt.X  = (STK_FLOAT32)(arcCenter.X + arcRadius*cos(tmPntPar));
		cpnt.Y  = (STK_FLOAT32)(arcCenter.Y + arcRadius*sin(tmPntPar));
		cpnt.Z  = 0.0;
		outPnt.push_back(cpnt);
	}

	return TRUE;
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
STK_BOOL CTessCircle::GetCircleDrawData(
		vector<Stk_Leader*>		leaders,		/*(i) PMI引出线数据				*/
		vector<STK_DOUBLE64>	&vecBeginPar,	/*(o) 圆弧绘制的起始角度参数	*/
		vector<STK_DOUBLE64>	&vecEndPar,		/*(o) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64			&drawAngle,		/*(o) 圆弧绘制角度				*/
		vector<STK_INT32>		&drawFlg		/*(o) 圆弧绘制方向				*/
												/*  ==  1 逆时针				*/
												/*  == -1 顺时针				*/
){
	Stk_Ellipse			*ellipse;					// 圆弧线
	STK_PNT32			BeginPnt,EndPnt;			// 起始点坐标值
	STK_FLOAT32			MajorRadius,MinorRadius;	// 短半径、长半径
	STK_PNT32			Center;						// 中心点
	STK_DIR32			inNormal;					// Z方向向量，判断圆弧绘制方向
	STK_DOUBLE64		BeginPara, EndPara;			// 起始点/终点的角度参数
	vector<STK_FLOAT32>	inNormal_Z;

	/*
	 *	按照参数方程计算各圆弧的角度参数
	 */
	for (int i=0; i<leaders.size(); i++) {
		Stk_Leader * leader = leaders.at(i);		//获得一条引出线数据
		STK_UINT32 type = leader->GetLeaderType();	//获得引出线类型

		if(type == 2){
			ellipse = (Stk_Ellipse *)leader->GetCurve();

			ellipse->GetRadius(MajorRadius,MinorRadius);
			ellipse->GetCoordinatePnt(BeginPnt,EndPnt);
			Center   = ellipse->GetCenterPoint();
			inNormal = ellipse->GetNormal();

			GetCircleAngleParameter(PMIDIM2, BeginPnt, Center, MinorRadius, BeginPara);
			GetCircleAngleParameter(PMIDIM2, EndPnt, Center, MinorRadius, EndPara);

			inNormal_Z.push_back(inNormal.Z);
			vecBeginPar.push_back(BeginPara);
			vecEndPar.push_back(EndPara);
		} else {
			return FALSE;
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
STK_BOOL CTessCircle::GetArcAngleData(
		vector<STK_DOUBLE64>		vecBeginPar,	/*(i) 圆弧绘制的起始角度参数	*/
		vector<STK_DOUBLE64>		vecEndPar,		/*(i) 圆弧绘制的终止角度参数	*/
		vector<STK_FLOAT32>			inNormal_Z,		/*(i) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64				&drawAngle,		/*(o) 圆弧绘制角度				*/
		vector<STK_INT32>			&drawFlg		/*(o) 圆弧绘制方向				*/
													/*  ==  1 逆时针				*/
													/*  == -1 顺时针				*/
){
	STK_DOUBLE64		EndPara;					// 起始点/终点的角度参数
	STK_INT32			neqFlg;						// 绘制方向
	STK_INT32			neqFlgA[2];					// 绘制方向
	STK_DOUBLE64		RealTangle;
	STK_DOUBLE64		minPara, maxPara;
	STK_DOUBLE64		tangle1, tangle2;
	STK_BOOL			outFlg=FALSE;

	if ( vecBeginPar.size() != 2 ||
		 vecEndPar.size()   != 2 ||
		 inNormal_Z.size()  != 2) {
			return FALSE;
	}

	/* θ1>θ2时，确定绘制角度 */
	if ( vecBeginPar.at(0) > vecBeginPar.at(1) ) {
		minPara = vecBeginPar.at(1);
		maxPara = vecBeginPar.at(0);
		// 实际角度表达方向θ2 -> θ1		注：以逆时针方向为主，下同
		if ( inNormal_Z.at(0) < 0.0 && inNormal_Z.at(1) > 0.0 ) {
			// 有效区间[θ2,θ1]，实际角度θ1-θ2
			RealTangle = vecBeginPar.at(0) - vecBeginPar.at(1);
			for (int j=0; j<vecEndPar.size(); j++ ) {
				EndPara = vecEndPar.at(j);
				neqFlg  = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ( (EndPara >= minPara) && (EndPara <= maxPara) ) {
					// 按照原方向绘制
					// neqFlg = inNormal_Z.at(j)>0.0 ? +1 : -1;
				} else {
					// 与原方向反向
					// neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = TRUE;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
			// 实际角度表达方向θ1 -> θ2
		} else if ( inNormal_Z.at(0) > 0.0 && inNormal_Z.at(1) < 0.0 ) {
			// 有效区间[θ1,2π] ∪ [0,θ2]，实际角度(2π-θ1)+θ2
			RealTangle = (PI*2 - vecBeginPar.at(0)) + vecBeginPar.at(1);
			for (int j=0; j<vecEndPar.size(); j++ ) {
				EndPara = vecEndPar.at(j);
				neqFlg  = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ( (EndPara >= maxPara) && (EndPara <= PI*2) ||
					 (EndPara >=     0.0) && (EndPara <= minPara) ) {
						// 按照原方向绘制
						// neqFlg = inNormal_Z.at(j);
				} else {
					// 与原方向反向
					//neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = TRUE;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
		} else {
			return FALSE;
		}
		// END点在角度区间之外时特殊处理
		if ( outFlg ) {
			neqFlgA[0] = inNormal_Z.at(0)>0.0 ? +1 : -1;
			neqFlgA[1] = inNormal_Z.at(1)>0.0 ? +1 : -1;
			tangle1 = fabs(EndPara-vecBeginPar.at(0));	
			tangle2 = fabs(EndPara-vecBeginPar.at(1));	// 
			if ( EndPara > maxPara ) {
				if ( tangle1+RealTangle/2 < PI ) {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : +1;
				} else {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : +1;
				}
			} else if ( EndPara < minPara ) {
				if ( tangle2+RealTangle/2 < PI ) {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : +1;
				} else {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : +1;
				}
			}
			drawFlg.clear();
			for (int k=0; k<2; k++) {
				drawFlg.push_back(neqFlgA[k]);
			}
		}
		/* θ1<θ2时，确定绘制角度 */
	} else if ( vecBeginPar.at(0) < vecBeginPar.at(1) ) {
		minPara = vecBeginPar.at(0);
		maxPara = vecBeginPar.at(1);
		// 实际角度表达方向θ2 -> θ1，实际角度θ2-θ1
		RealTangle = vecBeginPar.at(1) - vecBeginPar.at(0);
		if ( inNormal_Z.at(0) > 0.0 && inNormal_Z.at(1) < 0.0 ) {
			// 有效区间[θ1,θ2]
			for (int j=0; j<vecEndPar.size(); j++ ) {
				EndPara = vecEndPar.at(j);
				neqFlg  = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ( (EndPara >= minPara) && (EndPara <= maxPara) ) {
					// 按照原方向绘制
					//neqFlg = inNormal_Z.at(j);
				} else {
					// 与原方向反向
					//neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = TRUE;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
			// 实际角度表达方向θ1 -> θ2
		} else if ( inNormal_Z.at(0) < 0.0 && inNormal_Z.at(1) > 0.0 ) {
			// 有效区间[θ2,2π] ∪ [0,θ1]，实际角度(2π-θ2)+θ1
			RealTangle = (PI*2 - vecBeginPar.at(1)) + vecBeginPar.at(0);
			for (int j=0; j<vecEndPar.size(); j++ ) {
				EndPara = vecEndPar.at(j);
				neqFlg  = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ( (EndPara >= maxPara) && (EndPara <= PI*2) ||
					 (EndPara >=     0.0) && (EndPara <= minPara) ) {
						// 按照原方向绘制
						//neqFlg = inNormal_Z.at(j);
				} else {
					// 与原方向反向
					//neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = TRUE;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
		} else {
			return FALSE;
		}
		// END点在角度区间之外时特殊处理
		if ( outFlg ) {
			neqFlgA[0] = inNormal_Z.at(0)>0.0 ? +1 : -1;
			neqFlgA[1] = inNormal_Z.at(1)>0.0 ? +1 : -1;
			tangle1 = fabs(EndPara-vecBeginPar.at(0));	
			tangle2 = fabs(EndPara-vecBeginPar.at(1));	// 
			if ( EndPara > maxPara ) {
				if ( tangle2+RealTangle/2 < PI ) {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : +1;
				} else {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : +1;
				}
			} else if ( EndPara < minPara ) {
				if ( tangle1+RealTangle/2 < PI ) {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : 1;
				} else {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : 1;
				}
			}
			drawFlg.clear();
			for (int k=0; k<2; k++) {
				drawFlg.push_back(neqFlgA[k]);
			}
		}
	} else {
		return FALSE;
	}

	drawAngle = RealTangle;

	// 结果检查
	return (!drawFlg.empty());

}

//STK_BOOL CTessCircle::ComputeCircumcircle(const SbXfBox3f  & outerBox,const STK_PNT32& innerLoc,Stk_Ellipse & ellipse)
//{
//
//	STK_FLOAT32 f_LeftTop[3]  = {0.0f,0.0f,0.0f};
//	STK_FLOAT32 f_LeftBmt[3]  = {0.0f,0.0f,0.0f};
//
//	STK_FLOAT32 f_RightTop[3] = {0.0f,0.0f,0.0f};
//	STK_FLOAT32 f_RightBmt[3] = {0.0f,0.0f,0.0f};
//
//	STK_PNT32 centerpnt ={0.0f,0.0f,0.0f};
//	STK_PNT32 startpnt = {0.0f,0.0f,0.0f};
//	STK_PNT32 endpnt = {0.0f,0.0f,0.0f};
//
//	STK_FLOAT32 pnt[3] = {0.0f,0.0f,0.0f};
//
//	STK_FLOAT32 fstartparam  = 0.0,fendparam = 0.0f;
//
//
//	STK_DIR32 vecDir,vecNormal;
//
//	STK_FLOAT32 fdist = 0.0f,fLen = 0.0f;
//
//	SbVec3f dst;
//	SbVec3f points[2] = {};
//	SbVec3f corner;
//	SbBox3f newbox;
//
//	outerBox.getBounds(points[0],points[1]);
//	SbMatrix  matrix = outerBox.getTransform();
//
//	corner.setValue(points[0][0], points[0][1], 0.0);
//	matrix.multVecMatrix(corner, dst);
//
//	f_LeftBmt[0] = dst[0];
//	f_LeftBmt[1] = dst[1];
//	f_LeftBmt[2] = 0;
//
//
//	corner.setValue(points[1][0], points[1][1], 0.0);
//	matrix.multVecMatrix(corner, dst);
//
//	f_RightTop[0] = dst[0];
//	f_RightTop[1] = dst[1];
//	f_RightTop[2] = 0;
//
//
//	corner.setValue(points[0][0], points[1][1], 0.0);
//	matrix.multVecMatrix(corner, dst);
//	f_LeftTop[0] = dst[0];
//	f_LeftTop[1] = dst[1];
//	f_LeftTop[2] = 0;
//
//
//	corner.setValue(points[1][0], points[0][1], 0.0);
//	matrix.multVecMatrix(corner, dst);
//	f_RightBmt[0] = dst[0];
//	f_RightBmt[1] = dst[1];
//	f_RightBmt[2] = 0;
//	//计算f_RightTop和f_LeftTop的距离
//	float dist1 = sqrt((f_RightTop[0] - f_LeftTop[0])*(f_RightTop[0] - f_LeftTop[0])+
//				       (f_RightTop[1] - f_LeftTop[1])*(f_RightTop[1] - f_LeftTop[1])+
//				       (f_RightTop[2] - f_LeftTop[2])*(f_RightTop[2] - f_LeftTop[2]));
//
//
//	float dist2 = sqrt((f_LeftBmt[0] - f_LeftTop[0])*(f_LeftBmt[0] - f_LeftTop[0])+
//		               (f_LeftBmt[1] - f_LeftTop[1])*(f_LeftBmt[1] - f_LeftTop[1])+
//		               (f_LeftBmt[2] - f_LeftTop[2])*(f_LeftBmt[2] - f_LeftTop[2]));
//	if(dist1>dist2)
//	{
//		startpnt.X = f_RightTop[0];
//		startpnt.Y = f_RightTop[1];
//		startpnt.Z = 0.0f;
//
//		endpnt.X = f_LeftTop[0];
//		endpnt.Y = f_LeftTop[1];
//		endpnt.Z = 0.0;
//	}
//	else
//	{
//		startpnt.X = f_LeftTop[0];
//		startpnt.Y = f_LeftTop[1];
//		startpnt.Z = 0.0f;
//
//		endpnt.X = f_LeftBmt[0];
//		endpnt.Y = f_LeftBmt[1];
//		endpnt.Z = 0.0;
//	}
//
//	vecNormal.X = 0.0f;
//	vecNormal.Y = 0.0f;
//	vecNormal.Z = 1.0f;
//
//	ellipse.SetCoordiantePnt(startpnt,endpnt);
//	if(dist1>dist2)
//	{
//		fdist = sqrt((f_RightTop[0] - f_LeftTop[0])*(f_RightTop[0] - f_LeftTop[0])+
//					 (f_RightTop[1] - f_LeftTop[1])*(f_RightTop[1] - f_LeftTop[1])+
//					 (f_RightTop[2] - f_LeftTop[2])*(f_RightTop[2] - f_LeftTop[2]));
//
//		vecDir.X = f_RightBmt[0] - f_RightTop[0];
//		vecDir.Y = f_RightBmt[1] - f_RightTop[1];
//		vecDir.Z = 0.0f;
//
//		fLen = sqrt(vecDir.X*vecDir.X+vecDir.Y*vecDir.Y);
//		vecDir.X = vecDir.X / fLen;
//		vecDir.Y = vecDir.Y / fLen;
//		vecDir.Z = 0.0f;
//
//
//		pnt[0] = f_RightTop[0] + fdist * vecDir.X;
//		pnt[1] = f_RightTop[1] + fdist * vecDir.Y;
//		pnt[2] = f_RightTop[2] + fdist * vecDir.Z;
//
//		//计算圆心的位置
//		centerpnt.X = (pnt[0] + f_LeftTop[0])/2.0f;
//		centerpnt.Y = (pnt[1] + f_LeftTop[1])/2.0f;
//		centerpnt.Z = (pnt[2] + f_LeftTop[2])/2.0f;
//
//		ellipse.SetCenterPoint(centerpnt);
//
//		fdist = sqrt((f_LeftTop[0] - centerpnt.X)*(f_LeftTop[0] - centerpnt.X)+
//			(f_LeftTop[1] - centerpnt.Y)*(f_LeftTop[1] - centerpnt.Y)+
//			(f_LeftTop[2] - centerpnt.Z)*(f_LeftTop[2] - centerpnt.Z)
//			);
//		ellipse.SetRadius(fdist,fdist);
//	}
//	else
//	{
//		fdist = sqrt((f_LeftBmt[0] - f_LeftTop[0])*(f_LeftBmt[0] - f_LeftTop[0])+
//			         (f_LeftBmt[1] - f_LeftTop[1])*(f_LeftBmt[1] - f_LeftTop[1])+
//					 (f_LeftBmt[2] - f_LeftTop[2])*(f_LeftBmt[2] - f_LeftTop[2]));
//
//		vecDir.X = f_RightTop[0] - f_LeftTop[0];
//		vecDir.Y = f_RightTop[1] - f_LeftTop[1];
//		vecDir.Z = 0.0f;
//
//		fLen = sqrt(vecDir.X*vecDir.X+vecDir.Y*vecDir.Y);
//		vecDir.X = vecDir.X / fLen;
//		vecDir.Y = vecDir.Y / fLen;
//		vecDir.Z = 0.0f;
//
//
//		pnt[0] = f_LeftTop[0] + fdist * vecDir.X;
//		pnt[1] = f_LeftTop[1] + fdist * vecDir.Y;
//		pnt[2] = f_LeftTop[2] + fdist * vecDir.Z;
//
//		//计算圆心的位置
//		centerpnt.X = (pnt[0] + f_LeftBmt[0])/2.0f;
//		centerpnt.Y = (pnt[1] + f_LeftBmt[1])/2.0f;
//		centerpnt.Z = (pnt[2] + f_LeftBmt[2])/2.0f;
//
//		ellipse.SetCenterPoint(centerpnt);
//
//		fdist = sqrt((f_LeftTop[0] - centerpnt.X)*(f_LeftTop[0] - centerpnt.X)+
//			(f_LeftTop[1] - centerpnt.Y)*(f_LeftTop[1] - centerpnt.Y)+
//			(f_LeftTop[2] - centerpnt.Z)*(f_LeftTop[2] - centerpnt.Z)
//			);
//		ellipse.SetRadius(fdist,fdist);
//	}
//
//	float rx = startpnt.X - centerpnt.X;
//	float ry = startpnt.Y - centerpnt.Y;
//
//
//
//	fstartparam = atan2(ry,rx);
//	if(fstartparam<0)
//		fstartparam = fstartparam+ 2.0*PI;
//
//	rx = endpnt.X - centerpnt.X;
//	ry = endpnt.Y - centerpnt.Y;
//	//引线的终点参数
//	fendparam = atan2(ry,rx);
//	if(fendparam<0)
//		fendparam =fendparam + 2.0*PI;
//
//	if(fendparam>=3.0*PI/2.0&&fendparam<=2.0*PI&&(fendparam-fstartparam>PI))
//	{
//		vecNormal.Z = -1.0f;
//	}
//	if(fendparam>PI&&fendparam<3.0*PI/2.0&&fendparam<fstartparam)
//	{
//		vecNormal.Z = -1.0f;
//	}
//
//	if(fendparam>=0&&fendparam<=PI&&fendparam<fstartparam&&fstartparam<=3.0*PI/2.0&&fstartparam-fendparam<PI)
//
//	{
//		vecNormal.Z = -1.0f;
//	}
//
//	if(fendparam>2.0*PI/3.0&&fendparam<=PI&&fstartparam>=3.0*PI/2.0&&fstartparam<5.0*PI/3.0)
//	{
//		vecNormal.Z = -1.0f;
//	}
//	if(fendparam>=3.0*PI/2.0&&fstartparam>fendparam)
//	{
//		vecNormal.Z = -1.0f;
//	}
//
//	if(fendparam-fstartparam>PI)
//		vecNormal.Z = -1.0f;
//
//
//	ellipse.SetParameter(fstartparam,fendparam);
//	ellipse.SetNormal(vecNormal);
//
//
//
//	return true;
//}
//调整圆弧数据
void CTessCircle::RectifyArcData(Stk_PMI * pStkPMI)
{
	
	double angle  = 0.0;
	vector<Stk_Leader*> vecLeader = pStkPMI->GetLeaders();
	vector<Stk_ComText*>vecComText = pStkPMI->GetComText();
	int nsize = 0,i = 0;
	nsize = vecComText.size();
	if(nsize>0)
	{
		Stk_ComText* pComTxt = vecComText.at(0);
		
		vector<Stk_Text *> vecTxts = pComTxt->GetTexts();
		nsize = vecTxts.size();
		for(i = 0;i< nsize;i++)
		{
			Stk_Text * pTxt  = vecTxts.at(i);
			wstring angleTxt = pTxt->GetText();
			wchar_t ch = 0x00b0;

			int pos = angleTxt.find(ch);
			if( pos >0)
			{
				//angle = _wtof(angleTxt.c_str());
				break;
			}
		}
	}

	nsize = vecLeader.size();
	for(i= 0 ;i<nsize;i++)
	{
		Stk_Leader * pLeader = vecLeader.at(i);
		Stk_CurveEntity *pCruve = pLeader->GetCurve();

		StkCurveTypeEnum crvType = pCruve->GetCurveType();
		if(crvType == CURVE_TYPE_ELLIPSE)
		{
			STK_PNT32 startPnt,endPnt,centerPnt;
			Stk_Ellipse * pEllipse = (Stk_Ellipse*)pCruve;
			STK_DIR32 normalVec = pEllipse->GetNormal();
			pEllipse->GetCoordinatePnt(startPnt,endPnt);
			STK_FLOAT32 maxRadius = 0.0,minRadius = 0.0;
			centerPnt = pEllipse->GetCenterPoint();
			double vecA1[3] = {},vecA2[3] = {},fNormal[3] = {},radp = 0.0;
			pEllipse->GetRadius(maxRadius,minRadius);
			vecA1[0] = (startPnt.X - centerPnt.X)/maxRadius;
			vecA1[1] = (startPnt.Y  - centerPnt.Y)/maxRadius;
			vecA1[2] = (startPnt.Z  - centerPnt.Z)/maxRadius;


			vecA2[0] = (endPnt.X - centerPnt.X)/maxRadius;
			vecA2[1] = (endPnt.Y - centerPnt.Y)/maxRadius;
			vecA2[2] = (endPnt.Z - centerPnt.Z)/maxRadius;

			fNormal[0] = normalVec.X;
			fNormal[1] = normalVec.Y;
			fNormal[2] = normalVec.Z;


			int stat = CMathVector::Angle(vecA1,vecA2,fNormal,&radp);

			radp = radp*180/PI;

			if(radp-angle>1.0)
			{
				normalVec.Z*=-1;
				pEllipse->SetNormal(normalVec);
			}
		}
	}
}
//调整延长线数据
//void CTessCircle::RectifyExtLinData(SbVec3f * pPnt1,SbVec3f *pPnt2)
//{
//
//	double dist = 0; /*= sqrt((pPnt1[0]-pPnt2[0])*(pPnt1[0]-pPnt2[0])+
//					   (pPnt1[1]-pPnt2[1])*(pPnt1[1]-pPnt2[1])+
//					   (pPnt1[2]-pPnt2[2])*(pPnt1[2]-pPnt2[2]))*/
//	double dir[3] = {};
//	float pnt1[3] ={},pnt2[3] = {};
//	pPnt1->getValue(pnt1[0],pnt1[1],pnt1[2]);
//	pPnt2->getValue(pnt2[0],pnt2[1],pnt2[2]);
//
//
//
//	dir[0] = pnt1[0] - pnt2[0];
//	dir[1] = pnt1[1] - pnt2[1];
//	dir[2] = pnt1[2] - pnt2[2];
//
//	double len = sqrt(dir[0]*dir[0] + dir[1]*dir[1] +dir[2]*dir[2]);
//	dist = len;
//	dir[0] /= len;
//	dir[1] /= len;
//	dir[2] /= len;
//	if(dist>=100)
//		len = 2.8;
//	else
//		len = dist*0.025;
//
//	pnt1[0] = pnt1[0] + len * dir[0];
//	pnt1[1] = pnt1[1] + len * dir[1];
//	pnt1[2] = pnt1[2] + len * dir[2];
//
//	pPnt1->setValue(pnt1[0],pnt1[1],pnt1[2]);
//
//
//}
//调整延长线数据
void CTessCircle::RectifyExtLinData(Stk_PMI* pStkPMI)
{
	vector<Stk_Extensionline*>vecExtLin =  pStkPMI->GetExLines();
	int i = 0,j = 0,nsize = 0,ncount = 0;
	nsize = vecExtLin.size();
	Stk_Extensionline* pExtLin = NULL;
	for(i = 0;i < nsize ;i++)
	{
		pExtLin = vecExtLin.at(i);
		StkExtensTypeEnum ntype = pExtLin->GetExLineType();
		if(ntype == EXTENS_TYPE_LENGTH)
		{
			Stk_CurveEntity * pCruve =pExtLin->GetCurve();
			if(pCruve)
			{
				Stk_PolyLine * pPolyLin = (Stk_PolyLine*)pCruve;
				vector<STK_PNT32> vecPnts = pPolyLin->GetPoints();
				ncount = vecPnts.size();
				STK_PNT32 pnt1 = vecPnts.at(ncount-1);
				STK_PNT32 pnt2 = vecPnts.at(ncount-2);
				STK_DIR32 dir;
				double len = 0.0;
				dir.X = pnt1.X - pnt2.X;
				dir.Y = pnt1.Y - pnt2.Y;
				dir.Z = pnt1.Z - pnt2.Z;
				len = sqrt(dir.X*dir.X + dir.Y*dir.Y +dir.Z*dir.Z);
				dir.X /= len;
				dir.Y /= len;
				dir.Z /= len;
				//len *= MULTIPLE;
				len = 2.8;

				pnt1.X = pnt1.X + len * dir.X;
				pnt1.Y = pnt1.Y + len * dir.Y;
				pnt1.Z = pnt1.Z + len * dir.Z;
				vecPnts.pop_back();
				vecPnts.push_back(pnt1);
				pPolyLin->SetPoints(vecPnts);
				
			}
		}
	}

}
//调整角度尺寸上下公差丢失的“°”符号
void CTessCircle::RectifyAngleToleranceTxt(Stk_PMI* pStkPMI)
{
	StkPMITypeEnum ntype = pStkPMI->GetType();
	vector <Stk_Leader*> vecLead = pStkPMI->GetLeaders();
	Stk_Leader* pLead = NULL;
	StkCurveTypeEnum crvType = CURVE_TYPE_UNKNOWN;
	if(vecLead.size()>0)
	{
		pLead = vecLead.at(0);
		Stk_CurveEntity *pCruve = pLead->GetCurve();
		crvType = pCruve->GetCurveType();
		
	}
	if(PMI_TYPE_AngularDimension == ntype||
		(PMI_TYPE_TypeDimension == ntype&&
		CURVE_TYPE_ELLIPSE == crvType))
	{
		vector<Stk_ComText*>vecComText = pStkPMI->GetComText();
		int nsize = 0,i = 0;
		nsize = vecComText.size();
		if(nsize>0)
		{
			Stk_ComText* pComTxt = vecComText.at(0);

			vector<Stk_Text *> vecTxts = pComTxt->GetTexts();
			nsize = vecTxts.size();
			for(i = 0;i< nsize;i++)
			{
				Stk_Text * pTxt  = vecTxts.at(i);
				wstring angleTxt = pTxt->GetText();
				wchar_t ch = 0x00b0;
				//获取文本的类型
				if(text_type_tolerance != pTxt->GetUsageType())
					continue;
				int pos = angleTxt.find(ch);
				if( pos <0)
				{
					angleTxt.append(1,ch);
					
					pTxt->SetText(angleTxt);
					vecTxts[i] = pTxt;
				}
			}
		}
	}
	
}

//double CTessCircle::ComputeDist(const SbVec3f&pnt1,const SbVec3f &pnt2,SbVec3f &oDir)
//{
//	double fpnt1[3] = {},fpnt2[3] = {},dist = 0.0;
//
//	fpnt1[0] = pnt1[0];
//	fpnt1[1] = pnt1[1];
//	fpnt1[2] = pnt1[2];
//
//	fpnt2[0] = pnt2[0];
//	fpnt2[1] = pnt2[1];
//	fpnt2[2] = pnt2[2];
//
//	CMathGeolib::Cal2PntDis(3,fpnt1,fpnt2,&dist);
//	oDir[0] = (fpnt1[0] - fpnt2[0])/dist;
//	oDir[1] = (fpnt1[1] - fpnt2[1])/dist;
//	oDir[2] = (fpnt1[2] - fpnt2[2])/dist;
//
//	return dist;
//}
//
//void CTessCircle::RectifyDimensionTolerance(vector<SbXfBox3f>&vecBoxes,
//											const vector<Stk_Text*>& vecText,
//											vector<STK_DIR32>&vecDir,
//											vector<double>&vecDist,
//											vector<int>&vecIdx
//											)
//{
//	int  i = 0,nsize = 0,iVal = -1,iTolerance = -1;
//	nsize = vecBoxes.size();
//	Stk_Text *pText = NULL;
//	SbXfBox3f valBox,toleranceBox;
//	SbVec3f   bounds1[2] = {},bounds2[2] = {};
//	double    rightbom[3] = {},righttop[3] = {},dirLin[3] ={},pnt[3] ={},tolPnt[3] = {};
//	float fHeight = 0.0,fwidth = 0.0;
//
//	double told = 0.01,tola = 0.00026,paramval = 0.0;
//
//	STK_DIR32 tmpdir;
//	double distval = 0.0;
//
//    //以前的逻辑处理：被“新的逻辑处理”代替
//	/*for(i = 0;i< nsize;i++)
//	{
//		StkTextTypeEnum ntype = text_type_undefined;
//		pText = vecText.at(i);
//		ntype = pText->GetUsageType();
//		pText->GetCharWidthHeight(fwidth,fHeight);
//
//		if(text_type_tolerance == ntype)
//			iTolerance = i;
//
//		else if(text_type_value == ntype)
//			iVal = i;
//	}
//	if(iTolerance<0||iVal<0)
//		return false;*/
//
//
//	//新的逻辑处理
//	valBox = vecBoxes.at(0);
//	valBox.getBounds(bounds1[0],bounds1[1]);
//
//	rightbom[0] = bounds1[1][0];
//	rightbom[1] = bounds1[0][1];
//	rightbom[2] = 0.0;
//
//	righttop[0] = bounds1[1][0];
//	righttop[1] = bounds1[1][1];
//	righttop[2] = 0.0;
//
//
//	dirLin[0] = righttop[0] - rightbom[0];
//	dirLin[1] = righttop[1] - rightbom[1];
//	dirLin[2] = righttop[2] - rightbom[2];
//
//	vecText.at(0)->GetCharWidthHeight(fwidth,fHeight);
//	fwidth = fHeight/9.0;
//	for(i = 1;i< nsize; i++)
//	{
//		toleranceBox = vecBoxes.at(i);
//		toleranceBox.getBounds(bounds2[0],bounds2[1]);
//
//		tolPnt[0] = bounds2[0][0];
//		tolPnt[1] = bounds2[0][1];
//		tolPnt[2] = bounds2[0][2];
//
//
//
//		CMathGeolib::CalPedalPntToLine(3,tolPnt,3,rightbom,dirLin,tola,tola,pnt,&paramval,&distval);
//
//		if(distval>fwidth)
//		{
//			tmpdir.X = (pnt[0] - tolPnt[0])/distval;
//			tmpdir.Y = (pnt[1] - tolPnt[1])/distval;
//			tmpdir.Z = (pnt[2] - tolPnt[2])/distval;
//
//			distval -= fwidth;
//			vecDir.push_back(tmpdir);
//			vecDist.push_back(distval);
//			vecIdx.push_back(i);
//
//		}
//	}
//}

 /*
 * 功能：计算圆弧绘制方向

 * 参数：

 *       const vector<Stk_Leader*>& pLeaders           (i) 引线数

 *       const STK_PNT32& refPnt             (i) 圆弧绘制的终止角度参数

 *       Stk_PMI* pDimension             (o) 圆弧绘制角度

 *       vector<STK_PNT32>&outVecExtPnt               (o) 圆弧绘制方向

 * 返回值： 

 * 2014-08-25   gongcy   created
*/

void  CTessCircle::UpdateTessCricleData(const vector<Stk_Leader*>& pLeaders,const STK_PNT32& refPnt,
										  Stk_PMI* pDimension,vector<STK_PNT32>&outVecExtPnt)
{
	STK_PNT32 extPnt,tmpPnt;
	double outAngle = -1.0;
	StkPMISourceEnum EnumSource = pDimension->GetPMISource();
	StkPMITypeEnum   EnumType   = pDimension->GetType();
	bool bRet = true;
	Stk_Ellipse * pCreEllipse = NULL;
	if(EnumSource == PMI_SOURCE_CONVERT&&
		(
		 EnumType == PMI_TYPE_AngularDimension||
		 EnumType == PMI_TYPE_TypeDimension  
		))
	{
		
		int stat = 0;
		STK_PNT32 stkCenter,stkStartPnt,stkEndPnt;
		STK_DIR32 cirNormalDir;
		STK_FLOAT32 majRad = 0.0,minRad = 0.0; 
		int i = 0, ncount = 0;
		Stk_Leader*  pLead = NULL;
		Stk_CurveEntity * pCurve = NULL;
		Stk_Ellipse * pEllipse = NULL;
		StkCurveTypeEnum nType = CURVE_TYPE_UNKNOWN;
		
		double startDir[3]={},endDir[3] = {},zVec[3] = {},refDir[3] = {};
	
		double refAngle = 0.0;
		double angle = 0.0;
		double maxAngle = 0.0;
		double dist = 0.0;
		ncount = pLeaders.size();
	
		if(ncount>0)
		{
			pLead = pLeaders.at(0);
			pCurve = pLead->GetCurve();
			nType = pCurve->GetCurveType();
			if(CURVE_TYPE_ELLIPSE == nType)
			{
				pEllipse = (Stk_Ellipse *)pCurve;
				pEllipse->GetCoordinatePnt(stkStartPnt,stkEndPnt);
				pEllipse->GetRadius(majRad,minRad);
				stkCenter = pEllipse->GetCenterPoint();
				cirNormalDir = pEllipse->GetNormal();

				startDir[0] = (stkStartPnt.X - stkCenter.X)/majRad;
				startDir[1] = (stkStartPnt.Y - stkCenter.Y)/majRad;
				startDir[2] = (stkStartPnt.Z - stkCenter.Z)/majRad;
				
				refDir[0] = refPnt.X - stkCenter.X;
				refDir[1] = refPnt.Y - stkCenter.Y;
				refDir[2] = refPnt.Z - stkCenter.Z;
				double dist = sqrt(refDir[0]*refDir[0]+refDir[1]*refDir[1]+refDir[2]*refDir[2]);
				refDir[0] /= dist;
				refDir[1] /= dist;
				refDir[2] /= dist; 

				extPnt.X  = stkCenter.X + majRad * refDir[0];
				extPnt.Y  = stkCenter.Y + majRad * refDir[1];
				extPnt.Z  = stkCenter.Z + majRad * refDir[2];

				
				//判断点是否在圆弧上
				float pExtPnt[3]  = {extPnt.X,extPnt.Y,extPnt.Z};
				double fExtPnt[3] = {extPnt.X,extPnt.Y,extPnt.Z};
				double fStartPnt[3] = {stkStartPnt.X,stkStartPnt.Y,stkStartPnt.Z};
				double fEndPnt[3] = {stkEndPnt.X,stkEndPnt.Y,stkEndPnt.Z};
				double fStartDist = 0.0,fEndDist = 0.0;

				bRet = IsPntOnArc(pExtPnt,pEllipse);
				//当目标点不在圆弧上时，计算距离始点近还是距离终点近
				if(!bRet)
				{
					CMathGeolib::Cal2PntDis(3,fExtPnt,fStartPnt,&fStartDist);
					CMathGeolib::Cal2PntDis(3,fExtPnt,fEndPnt,&fEndDist);
					pCreEllipse = new Stk_Ellipse();
					pCreEllipse->SetCenterPoint(stkCenter);
					pCreEllipse->SetRadius(majRad,minRad);
					if(fStartDist<fEndDist)//距离始点近
					{
						pCreEllipse->SetCoordiantePnt(stkStartPnt,extPnt);
						cirNormalDir.X *= -1;
						cirNormalDir.Y *= -1;
						cirNormalDir.Z *= -1;
					}
					else//距离终点近
					{
						pCreEllipse->SetCoordiantePnt(stkEndPnt,extPnt);
					}

					pCreEllipse->SetNormal(cirNormalDir);

					GetCircleDiscretization(pCreEllipse, 0, outVecExtPnt);
					if(pCreEllipse)
						delete pCreEllipse;
					pCreEllipse = NULL;
				}
			}
		}
	}
}
/**
 *  @creator gongcy
 *	@brief	判断点是否在圆弧上。
 *	@param	i_pPnt			( I) 目标点

 *	@param	i_pEllipse			( I) 圆弧
 *	
 */
//
bool CTessCircle::IsPntOnArc(const float* i_pPnt,Stk_Ellipse* i_pEllipse)
{
	//定义局部变量
	bool bRet = false;
	if(!i_pEllipse) return false;
	STK_PNT32 startPnt,endPnt,centerPnt,refPnt;
	STK_DIR32 normalDir;
	double fstartpnt[3] = {},fendpnt[3] = {},fpickpnt[3] = {},fvertPnt[3] = {},fparam = 0.0,fvertdist = 0.0;

	
	float dist = 0,refdist = 0;

	float pnts[3]  = {};
	float vecDir[3] = {};

	double angle = 0.0;
	double fStartVec[3] = {},fEndVec[3] = {},fNoramlDir[3] = {};
	
	memset(&startPnt,0,sizeof(STK_PNT32));
	memset(&endPnt,0,sizeof(STK_PNT32));
	memset(&centerPnt,0,sizeof(STK_PNT32));
	memset(&refPnt,0,sizeof(STK_PNT32));

	float majRaduis = 0.0f,minRaduis = 0.0;
	i_pEllipse->GetCoordinatePnt(startPnt,endPnt);
	centerPnt = i_pEllipse->GetCenterPoint();

	i_pEllipse->GetRadius(majRaduis,minRaduis);

	normalDir = i_pEllipse->GetNormal();
	fNoramlDir[0] = normalDir.X;
	fNoramlDir[1] = normalDir.Y;
	fNoramlDir[2] = normalDir.Z;

	vecDir[0] = i_pPnt[0] - centerPnt.X;
	vecDir[1] = i_pPnt[1] - centerPnt.Y;
	vecDir[2] = i_pPnt[2] - centerPnt.Z;

	dist = sqrt((i_pPnt[0] - centerPnt.X)*(i_pPnt[0] - centerPnt.X)+
		        (i_pPnt[1] - centerPnt.Y)*(i_pPnt[1] - centerPnt.Y)+
				(i_pPnt[2] - centerPnt.Z)*(i_pPnt[2] - centerPnt.Z));

	vecDir[0] = vecDir[0]/dist;
	vecDir[1] = vecDir[1]/dist;
	vecDir[2] = vecDir[2]/dist;

	fStartVec[0] = (startPnt.X-centerPnt.X)/majRaduis;
	fStartVec[1] = (startPnt.Y-centerPnt.Y)/majRaduis;
	fStartVec[2] = (startPnt.Z-centerPnt.Z)/majRaduis;

	fEndVec[0] = (endPnt.X-centerPnt.X)/majRaduis;
	fEndVec[1] = (endPnt.Y-centerPnt.Y)/majRaduis;
	fEndVec[2] = (endPnt.Z-centerPnt.Z)/majRaduis;

	//计算角度
	CMathVector::Angle(fStartVec,fEndVec,fNoramlDir,&angle);


	pnts[0] = centerPnt.X + majRaduis * vecDir[0];
	pnts[1] = centerPnt.Y + majRaduis * vecDir[1];
	pnts[2] = centerPnt.Z + majRaduis * vecDir[2];

	refPnt.X = (startPnt.X + endPnt.X)/2.0f;
	refPnt.Y = (startPnt.Y + endPnt.Y)/2.0f;
	refPnt.Z = (startPnt.Z + endPnt.Z)/2.0f;

	dist =sqrt((refPnt.X - centerPnt.X)*( refPnt.X - centerPnt.X)+
		       (refPnt.Y - centerPnt.Y)*(refPnt.Y - centerPnt.Y)+
		       (refPnt.Z - centerPnt.Z)*(refPnt.Z - centerPnt.Z));
	if(dist<=0.01)
	{
		//当为半圆时，不做任何处理，直接返回。
		return true;
		
	}
	else
	{
		vecDir[0] = refPnt.X - centerPnt.X;
		vecDir[1] = refPnt.Y - centerPnt.Y;
		vecDir[2] = refPnt.Z - centerPnt.Z;

		dist = sqrt((refPnt.X - centerPnt.X)*(refPnt.X - centerPnt.X)+
					(refPnt.Y - centerPnt.Y)*(refPnt.Y - centerPnt.Y)+
					(refPnt.Z - centerPnt.Z)*(refPnt.Z - centerPnt.Z));
		

		vecDir[0] = vecDir[0]/dist;
		vecDir[1] = vecDir[1]/dist;
		vecDir[2] = vecDir[2]/dist;
		if(angle>PI)
		{
			vecDir[0] *= -1;
			vecDir[1] *= -1;
			vecDir[2] *= -1;
		}
	}
	
	

	refPnt.X = centerPnt.X + majRaduis * vecDir[0];
	refPnt.Y = centerPnt.Y + majRaduis * vecDir[1];
	refPnt.Z = centerPnt.Z + majRaduis * vecDir[2];

	//计算中心点到始点的距离
	dist = sqrt((refPnt.X - startPnt.X)*(refPnt.X - startPnt.X)+
				(refPnt.Y - startPnt.Y)*(refPnt.Y - startPnt.Y)+
				(refPnt.Z - startPnt.Z)*(refPnt.Z - startPnt.Z));

	refdist =  sqrt((refPnt.X - pnts[0])*(refPnt.X - pnts[0])+
					(refPnt.Y - pnts[1])*(refPnt.Y - pnts[1])+
					(refPnt.Z - pnts[2])*(refPnt.Z - pnts[2]));
	if(refdist<=dist)
		bRet = true;
	else
		bRet = false;
	return bRet;


}

void   CTessCircle::ComputeVecDirByTxt(const STK_BOX32& txtBox,double dirLin[3])
{
	double linStart[3] = {},linEnd[3] = {};
	linStart[0] = txtBox.BoundBox[0][0];
	linStart[1] = txtBox.BoundBox[0][1];
	linStart[2] = txtBox.BoundBox[0][2];


	linEnd[0] = txtBox.BoundBox[1][0]; 
	linEnd[1] = txtBox.BoundBox[0][1];
	linEnd[2] = 0.0;

	double len = 0.0;
	dirLin[0] = linEnd[0] - linStart[0] ;
	dirLin[1] = linEnd[1] - linStart[1] ;
	dirLin[2] = 0.0 ;
	len =sqrt( dirLin[0]*dirLin[0]+dirLin[1]*dirLin[1] );
	dirLin[0] /= len;
	dirLin[1] /= len;
}

//double CTessCircle::RectifyRugnessTxt2(const vector<Stk_CurveEntity*>& vecCurveList,const STK_BOX32&txtBox,double offsetdir[3])
//{
//	Stk_CurveEntity *pCurve1 = NULL;
//
//	Stk_CurveEntity *pCurve2 = NULL;
//	STK_PNT32 firstPnt,secondPnt;
//	double  linStart[3] = {},linEnd[3] = {};
//	double    pnt[3] = {};
//	Stk_PolyLine *pPolyLin = NULL;
//	Stk_Line * pLin  = NULL;
//
//	StkCurveTypeEnum curveType1 = CURVE_TYPE_UNKNOWN;
//	StkCurveTypeEnum curveType2 = CURVE_TYPE_UNKNOWN;
//
//	vector<STK_BOX32> vecTextBox;
//	vector<STK_PNT32>	vecPnt;
//	vector<STK_PNT32>	outVecPnt;
//	double		tolA[2] = {0.01,0.00026};
//	double	oHpnt[3] = {0.0,0.0,0.0};		/* 垂足的坐标值	     */
//
//	double	oHprm = 0.0;		    /* 参数	     */
//	double	oHdst = 0.0;		    /* 距离　    */
//	double  oHdst2 = 0.0;
//
//	int nsize = vecCurveList.size();
//	int i  = 0 ;
//
//	SbLine horiLin,biasLin;
//	SbVec3f horiStart,horiEnd,biasStart,biasEnd,ptOnThis,ptOnThat;
//
//
//	if(nsize!=2)	return 0.0;
//
//	//从第一条线的第二个点开始，沿着文本框水平方向
//	double tmpdir[3] = {};
//	double tmppnt[3] = {};
//	vector<STK_PNT32>vecfpnts,vecspnts;
//
//	ComputeVecDirByTxt(txtBox,tmpdir);//获取文本框水平方向的矢量
//
//
//
//
//	pCurve1 = vecCurveList.at(0);
//	pCurve2 = vecCurveList.at(1);
//	curveType1 = pCurve1->GetCurveType();
//	curveType2 = pCurve2->GetCurveType();
//	vecTextBox.push_back(txtBox);
//
//	if(
//		!(CURVE_TYPE_POLYLINE == curveType1&&
//		 CURVE_TYPE_POLYLINE == curveType2))
//		return 0.0;
//
//	pPolyLin = (Stk_PolyLine*)pCurve1;
//	vecfpnts = pPolyLin->GetPoints();
//	int npnt = vecfpnts.size();
//	STK_PNT32 startPnt ,endPnt;
//	startPnt = vecfpnts.at(npnt-1);
//	horiStart.setValue(startPnt.X,startPnt.Y,startPnt.Z);
//	horiEnd[0] = startPnt.X+1000.0*tmpdir[0];
//	horiEnd[1] = startPnt.Y+1000.0*tmpdir[1];
//	horiEnd[2] = 0.0;
//	horiLin.setValue(horiStart,horiEnd);
//
//	linStart[0] = startPnt.X;
//	linStart[1] = startPnt.Y;
//	linStart[2] = startPnt.Z;
//
//	pPolyLin = (Stk_PolyLine*)pCurve2;
//	vecspnts = pPolyLin->GetPoints();
//	npnt = vecspnts.size();
//	startPnt = vecspnts.at(0);
//	endPnt   = vecspnts.at(npnt-1);
//
//	biasStart.setValue(startPnt.X,startPnt.Y,startPnt.Z);
//	biasEnd.setValue(endPnt.X,endPnt.Y,endPnt.Z);
//
//	biasLin.setValue(biasStart,biasEnd);
//
//
//	if(!horiLin.getClosestPoints(biasLin,ptOnThis,ptOnThat))return 0.0;
//	startPnt.X = horiStart[0];
//	startPnt.Y = horiStart[1];
//	startPnt.Z = horiStart[2];
//
//	vecPnt.push_back(startPnt);
//
//	endPnt.X = horiEnd[0];
//	endPnt.Y = horiEnd[1];
//	endPnt.Z = horiEnd[2];
//
//	vecPnt.push_back(endPnt);
//
//
//
//	CPMIUtilityLib::DivideLeader(2,vecTextBox,vecPnt,outVecPnt);
//
//
//
//	if(outVecPnt.size()>0)
//	{
//		//计算点到第三边的距离
//		pnt[0] = biasStart[0];
//		pnt[1] = biasStart[1];
//		pnt[2] = biasStart[2];
//
//		CMathGeolib::IsPntInSegment1(3,pnt,3,linStart,linEnd,tolA[0],tolA[1],oHpnt,&oHprm,&oHdst);
//		/*double dirLin[3] = {};
//		ComputeVecDirByTxt(txtBox,dirLin);*/
//		linStart[0] = txtBox.BoundBox[0][0];
//		linStart[1] = txtBox.BoundBox[0][1];
//		linStart[2] = txtBox.BoundBox[0][2];
//		CMathGeolib::CalPedalPntToLine(3,pnt,3,linStart,tmpdir,tolA[0],tolA[1],oHpnt,&oHprm,&oHdst2);
//		if(oHdst2<oHdst)
//		{
//			oHdst -= oHdst2;
//			oHdst += (txtBox.BoundBox[1][1] - txtBox.BoundBox[0][1])/10.0;
//			offsetdir[0] = (oHpnt[0] - pnt[0])/oHdst2;
//			offsetdir[1] = (oHpnt[1] - pnt[1])/oHdst2;
//			offsetdir[2] = (oHpnt[2] - pnt[2])/oHdst2;
//		}
//	}
//
//	return oHdst;
//}

double CTessCircle::RectifyRugnessTxt(const vector<Stk_CurveEntity*>& vecCurveList,const STK_BOX32&txtBox,double offsetdir[3])
{

	Stk_CurveEntity *pCurve1 = NULL;

	Stk_CurveEntity *pCurve3 = NULL;
	STK_PNT32 firstPnt,secondPnt;
	double  linStart[3] = {},linEnd[3] = {};
	double    pnt[3] = {};
	Stk_PolyLine *pPolyLin = NULL;
	Stk_Line * pLin  = NULL;

	StkCurveTypeEnum curveType1 = CURVE_TYPE_UNKNOWN;
	StkCurveTypeEnum curveType3 = CURVE_TYPE_UNKNOWN;

	vector<STK_BOX32> vecTextBox;	   
	vector<STK_PNT32>	vecPnt;	
	vector<STK_PNT32>	outVecPnt;
	double		tolA[2] = {0.01,0.00026};
	double	oHpnt[3] = {0.0,0.0,0.0};		/* 垂足的坐标值	     */ 

	double	oHprm = 0.0;		    /* 参数	     */ 
	double	oHdst = 0.0;		    /* 距离　    */ 
	double  oHdst2 = 0.0;

	int nsize = vecCurveList.size();
	int i  = 0 ;

	if(nsize!=3) return 0.0;
	pCurve1 = vecCurveList.at(0);
	pCurve3 = vecCurveList.at(2);
	curveType1 = pCurve1->GetCurveType();
	curveType3 = pCurve3->GetCurveType();
	vecTextBox.push_back(txtBox);

	if(CURVE_TYPE_POLYLINE == curveType1&&CURVE_TYPE_POLYLINE == curveType3)
	{
		
		pPolyLin = (Stk_PolyLine*)pCurve3;
		vecPnt = pPolyLin->GetPoints();
		if(vecPnt.size())
		{
			firstPnt  = vecPnt.at(0);
			secondPnt = vecPnt.at(1);
			
			linStart[0] = firstPnt.X;
			linStart[1] = firstPnt.Y;
			linStart[2] = firstPnt.Z;

			linEnd[0]   = secondPnt.X;
			linEnd[1]   = secondPnt.Y;
			linEnd[2]   = secondPnt.Z;
		
			CPMIUtilityLib::DivideLeader(2,vecTextBox,vecPnt,outVecPnt);

			pPolyLin = (Stk_PolyLine*)pCurve1;

			firstPnt = pPolyLin->GetPoints().at(0);

			pnt[0] = firstPnt.X;
			pnt[1] = firstPnt.Y;
			pnt[2] = firstPnt.Z;
		}
	}

	else if(CURVE_TYPE_LINE == curveType1&&CURVE_TYPE_LINE == curveType3)
	{
		pLin = (Stk_Line *)pCurve3;
		vecPnt.push_back(pLin->GetStartPoint());
		vecPnt.push_back(pLin->GetEndPoint());
		firstPnt = pLin->GetStartPoint();
		secondPnt = pLin->GetEndPoint();

		linStart[0] = firstPnt.X;
		linStart[1] = firstPnt.Y;
		linStart[2] = firstPnt.Z;

		linEnd[0]   = secondPnt.X;
		linEnd[1]   = secondPnt.Y;
		linEnd[2]   = secondPnt.Z;

		

		CPMIUtilityLib::DivideLeader(2,vecTextBox,vecPnt,outVecPnt);

		pLin = (Stk_Line*)pCurve1;

		firstPnt = pLin->GetStartPoint();

		pnt[0] = firstPnt.X;
		pnt[1] = firstPnt.Y;
		pnt[2] = firstPnt.Z;
	}
	if(outVecPnt.size()>0)
	{
		//计算点到第三边的距离
		
		CMathGeolib::IsPntInSegment1(3,pnt,3,linStart,linEnd,tolA[0],tolA[1],oHpnt,&oHprm,&oHdst);
		double dirLin[3] = {};
		ComputeVecDirByTxt(txtBox,dirLin);
		linStart[0] = txtBox.BoundBox[0][0];
		linStart[1] = txtBox.BoundBox[0][1];
		linStart[2] = txtBox.BoundBox[0][2];
		CMathGeolib::CalPedalPntToLine(3,pnt,3,linStart,dirLin,tolA[0],tolA[1],oHpnt,&oHprm,&oHdst2);
		if(oHdst2<oHdst)
		{
			oHdst -= oHdst2;
			oHdst += (txtBox.BoundBox[1][1] - txtBox.BoundBox[0][1])/10.0;
			offsetdir[0] = (oHpnt[0] - pnt[0])/oHdst2;
			offsetdir[1] = (oHpnt[1] - pnt[1])/oHdst2;
			offsetdir[2] = (oHpnt[2] - pnt[2])/oHdst2;
		}
	}
	
	return oHdst;
}

//bool CTessCircle::CheckOutFrameOffset(const SbBox3f&txtBox,const SbBox3f&FrameBox)
//{
//	/*******************************************************************************
//	 ***************       当两个框的关系为完全包含关系时，            *************
//	 ***************       要判断是否存在偏移，如果偏移，调整文本的位置*************
//	 ******************************************************/
//	//计算两个中心的坐标是否相等，若不相等，调整文本的位置
//}
//bool CTessCircle::CheckOutFrameIntersec(const SbBox3f&txtBox,const SbBox3f&FrameBox)
//{
//	bool bRet = false;
//
//
//	float   fTxtVolum	= txtBox.getVolume();
//	float   fTol        = 0.2f;
//	SbVec3f txtCenterPnt = txtBox.getCenter();
//
//
//
//	float   fFrameVolum =	FrameBox.getVolume();
//	SbVec3f frameCenterPnt = FrameBox.getCenter();
//
//	float   fIntersectVolum = 0.0f;
//
//	SbBox3f* pIntersectBox = NULL;
//
//
//
//	//文本偏移的距离和方向
//	double  offsetLen = 0.0;
//	SbVec3f offsetDir;
//
//	//计算两个包围盒是否相交
//	pIntersectBox = ComputeIntersetBox(txtBox,FrameBox);
//	if(pIntersectBox) //相交
//	{
//		fIntersectVolum = pIntersectBox->getVolume();
//		/*
//		 ___________________
//		|		 _____		|
//		|		|_____|		|  完全包含关系
//		|___________________|
//		*/
//		if(fIntersectVolum == fTxtVolum|| fIntersectVolum == fFrameVolum) //完全包含关系
//		{
//			/******************************************************
//			 ******交集为完全包含关系，不用画框，要做偏移操作。****
//			 ******************************************************/
//			//计算两个中心的坐标是否相等，若不相等，调整文本的位置
//			if(!txtCenterPnt.equals(frameCenterPnt,fTol))
//			{
//				frameCenterPnt -= txtCenterPnt;
//				offsetLen = frameCenterPnt.length();
//				offsetDir.setValue(frameCenterPnt[0]/offsetLen,
//								   frameCenterPnt[1]/offsetLen,
//								   frameCenterPnt[2]/offsetLen);
//
//			}
//
//		}
//		/*
//						   _______
//		 _________________|__	  |
//		|				  |  |	  |
//		|				  |	 |    |         部分包含关系
//		|_________________|__|    |
//		                  |_______|
//		*/
//		else if(fIntersectVolum<fTxtVolum&&fIntersectVolum<fFrameVolum)//部分包含关系
//		{
//			//
//		}
//
//		bRet = true;
//	}
//	else //没有交集，要根据文字来画框。
//	{
//
//	}
//	return bRet;
//
//}
/**
 *  @creator gongcy
 *	@brief	过滤边框。
 *	@param	pStkPMI			( I) PMI要素

 *	@param	vecBox			( O) 外框对应的包围盒
 *	
 */
//void CTessCircle::FilterOutFrame(Stk_PMI * pStkPMI,vector<SbBox3f>&vecBox)
//{
//	StkPMITypeEnum EnumType     = pStkPMI->GetType();
//	StkPMISourceEnum EnumSource = pStkPMI->GetPMISource();
//
//	vector<Stk_OutFrame*>vecOutFrame = pStkPMI->GetOutFrame();
//	vector<Stk_SpecialLine*>vecSpecLin ;
//	vector<Stk_CurveEntity*>vecCurve;
//	vecBox.clear();
//
//	int i = 0,j = 0,nSize =vecOutFrame.size(),n = 0;
//
//
//	if(PMI_SOURCE_CONVERT != EnumSource)
//		return;
//	if(PMI_TYPE_TypeGeometricalTolerance == EnumType||
//		PMI_TYPE_TypeNote == EnumType)
//	{
//		for(i=0;i<nSize;i++) //外框的数量
//		{
//			SbBox3f outFrame;
//			SbVec3f minPnt,maxPnt;
//			Stk_OutFrame * pFrame = vecOutFrame.at(i);
//			/*vecCurve = pFrame->GetCurve(0);
//
//
//			Stk_CurveEntity *pCurve = vecCurve.at(0);*/
//
//			Stk_CurveEntity *pCurve = NULL;
//			//pCurve = pFrame->GetCurve(0);
//			pCurve = pFrame->GetFrameData().at(0);
//			Stk_PolyLine *pPolyLin  = (Stk_PolyLine*)pCurve;
//			int npnt = 0;
//			vector<STK_PNT32> vecPnt = pPolyLin->GetPoints();
//			npnt = vecPnt.size();
//			if(npnt==5)
//			{
//				STK_PNT32 tmppnt = vecPnt.at(3);
//				minPnt.setValue(tmppnt.X,tmppnt.Y,tmppnt.Z);
//				tmppnt = vecPnt.at(1);
//				maxPnt.setValue(tmppnt.X,tmppnt.Y,tmppnt.Z);
//
//				outFrame.setBounds(minPnt,maxPnt);
//				n = vecBox.size();
//				if(n==0)
//					vecBox.push_back(outFrame);
//				else
//				{
//					for(j = 0;j<n;j++)
//					{
//						SbBox3f tmpbox = vecBox.at(j);
//						//SbBox3f *pIntersectBox = ComputeIntersetBox(tmpbox,outFrame);
//						if(!CheckBoxEqual(tmpbox,outFrame))
//							vecBox.push_back(outFrame);
//
//
//					}
//				}
//			}
//		}
//	}
//	vecSpecLin = pStkPMI->GetSpecialLines();
//
//}
//bool CTessCircle::ComputeOffsetBox(const SbBox3f&inBox1,const SbBox3f&inBox2,double &offsetLen,SbVec3f&offsetDir)
//{
//	bool    bOffset = false;
//	float   fTxtVolum	= inBox1.getVolume();
//	float   fTol        = 0.2f;
//	SbVec3f txtCenterPnt = inBox1.getCenter();
//
//
//
//	float   fFrameVolum =	inBox2.getVolume();
//	SbVec3f frameCenterPnt = inBox2.getCenter();
//
//	float   fIntersectVolum = 0.0f;
//
//	SbBox3f* pIntersectBox = NULL;
//
//
//
//	//文本偏移的距离和方向
//	offsetLen = 0.0;
//	offsetDir.setValue(0.0f,0.0f,0.0f);
//
//	//计算两个包围盒是否相交
//	pIntersectBox = ComputeIntersetBox(inBox1,inBox2);
//	if(pIntersectBox) //相交
//	{
//		fIntersectVolum = pIntersectBox->getVolume();
//		/*
//		 ___________________
//		|		 _____		|
//		|		|_____|		|  完全包含关系
//		|___________________|
//		*/
//		if(fIntersectVolum == fTxtVolum|| fIntersectVolum == fFrameVolum) //完全包含关系
//		{
//			/******************************************************
//			 ******交集为完全包含关系，不用画框，要做偏移操作。****
//			 ******************************************************/
//			//计算两个中心的坐标是否相等，若不相等，调整文本的位置
//			if(!txtCenterPnt.equals(frameCenterPnt,fTol))
//			{
//				frameCenterPnt -= txtCenterPnt;
//				offsetLen = frameCenterPnt.length();
//				offsetDir.setValue(frameCenterPnt[0]/offsetLen,
//								   frameCenterPnt[1]/offsetLen,
//								   frameCenterPnt[2]/offsetLen);
//				bOffset = true;
//
//			}
//
//		}
//	}
//	return bOffset;
//}
//
//SbBox3f* CTessCircle::ComputeIntersetBox(const SbBox3f&inBox1,const SbBox3f&inBox2)
//{
//	SbVec3f minBox1	=	inBox1.getMin();
//	SbVec3f maxBox1	=	inBox1.getMax();
//	SbVec3f minBox2 =	inBox2.getMin();
//	SbVec3f maxBox2 =	inBox2.getMax();
//
//	SbVec3f minIntersect,maxIntersect;
//	SbBox3f* pIntersectBox = NULL;
//
//	if(inBox1.intersect(inBox2))//判断两个包围盒是否相交
//	{
//		//计算两个包围盒的相交的包围盒
//
//		for(int i = 0;i< 3; i++)
//		{
//			if(minBox1[i] > minBox2[i])	minIntersect[i] = minBox1[i];
//			else minIntersect[i] = minBox2[i];
//			if(maxBox1[i] > maxBox2[i]) maxIntersect[i] = maxBox2[i];
//			else maxIntersect[i] = maxBox1[i];
//		}
//		pIntersectBox = new SbBox3f;
//		pIntersectBox->getBounds(minIntersect,maxIntersect);
//	}
//	else if(inBox2.intersect(inBox1))
//	{
//		for(int i = 0;i< 3; i++)
//		{
//			if(minBox1[i] > minBox2[i])	minIntersect[i] = minBox1[i];
//			else minIntersect[i] = minBox2[i];
//			if(maxBox1[i] > maxBox2[i]) maxIntersect[i] = maxBox2[i];
//			else maxIntersect[i] = maxBox1[i];
//		}
//		pIntersectBox = new SbBox3f;
//		pIntersectBox->getBounds(minIntersect,maxIntersect);
//	}
//	return pIntersectBox;
//
//}
//
//bool CTessCircle::CheckBoxEqual(const SbBox3f&inBox1,const SbBox3f&inBox2 )
//{
//
//	bool bRet = false;
//
//	SbVec3f minBox1	=	inBox1.getMin();
//	SbVec3f maxBox1	=	inBox1.getMax();
//	SbVec3f minBox2 =	inBox2.getMin();
//	SbVec3f maxBox2 =	inBox2.getMax();
//
//
//	if(minBox2.equals(minBox1,0.001)&&maxBox2.equals(maxBox1,0.001))
//		bRet = true;
//	else
//		bRet = false;
//	return bRet;
//
//}

/*====================================================================*\
FUNCTION :   设置基线方向的标志
PURPOSE  :   对应于SV中的“RdSSetLLin”接口
@parm
const SbVec3f&curPnt (I) 输入的最后一个点
const SbVec3f&oldPnt (I) 输入的倒数第二个点
@return
0:同向
1:反向
create by gongcy on 2014/10/22
\*====================================================================*/
int CTessCircle::SetBasLinDir(double curPntA[3],double oldPntA[3])
{

	//需要对curPnt和oldPnt进行检查：是否是同一点？
	int bLinFlg =  -1;
	double linVec[3] = {};
	/*
	 * 水平方向的设置
	 */
	double hVecA[3]		 = {1.0,0.0,0.0};
	double LLinVecA[3]	 = {0.0,0.0,0.0};
	double flength       = 0.0;

	//设置线的方向矢量
	
	linVec[0] = curPntA[0] - oldPntA[0];
	linVec[1] = curPntA[1] - oldPntA[1];
	linVec[2] = 0.0;

	CMathVector::LengthAndUnit(2,linVec,&flength,LLinVecA);


	//设置基线方向的标志
	double fAng = 0.0;
	
	CMathVector::Angle(hVecA,LLinVecA,&fAng);

	if ( 0.0 <= fAng && fAng <= PI/2.0)
		bLinFlg = 0;
	else if ( PI/2.0 < fAng && fAng < PI*3.0/2.0)
		bLinFlg = 1;
	else
		bLinFlg = 0;
	return bLinFlg;

}
