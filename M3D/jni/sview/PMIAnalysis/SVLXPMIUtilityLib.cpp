#include "SVLLib/Stk_CurveEntity.h"
#include "SVLXPMIUtilityLib.h"
#include <math.h>
#include "sview/PMIAnalysis/toolkit/MathVector.h"
//#include "DimSymUitity.h"
#include <stdlib.h>

#define ARAAYNUM		(4)
#define PMIDIM2			(2)	
#define ZTOL			(2e-10)		// 干涉公差

using namespace HoteamSoft::SVLLib;
CSVLXPMIUtilityLib::CSVLXPMIUtilityLib(void)
{
}

CSVLXPMIUtilityLib::~CSVLXPMIUtilityLib(void)
{
}

/*
* 功能：获取末端符号方向

* 参数：

*       leaders               (i) PMI引出线数据

*       drawDir               (o) 末端符号绘制方向

* 返回值： 执行成功/失败

*
*/
bool CSVLXPMIUtilityLib::GetEndSymbolDirection(
	vector<Stk_LeaderPtr>		leaders,		/*(i) PMI引出线数据				*/
	vector<STK_INT32>		&drawDir		/*(o) 圆弧绘制方向				*/
											/*  ==  1 原方向				*/
											/*  == -1 反方向				*/
) {
	Stk_PolyLinePtr polyline;					// 折线
	vector<STK_PNT32> pnt;
	STK_PNT32	StartPnt[2], EndPnt[2];
	STK_INT32	drawDirA[2];

	if (leaders.size() != 2) {
		return false;
	}
	for (int i = 0; i<leaders.size(); i++) {
		Stk_LeaderPtr leader = leaders.at(i);				// 获得一条引出线数据
		STK_UINT32 type = leader->GetLeaderType();			// 获得引出线类型

		if (type == 1) {
			pnt.clear();
			vector<Stk_CurveEntityPtr> CurveList = leader->GetAllCurve();
			if (CurveList.size() == 0)
			{
				return false;
			}
			Stk_CurveEntityPtr pCurve = CurveList[CurveList.size() - 1];
			polyline = Stk_PolyLinePtr::DynamicCast(pCurve);	// 获得引出线的线条数据
			pnt = polyline->GetPoints();					// 获得离散点数据
			StartPnt[i] = pnt.at(0);
			EndPnt[i] = pnt.at(1);
		}
		else {
			return false;
		}
	}

	// 绘制方向重新计算
	drawDirA[0] = drawDirA[1] = +1;
	if ((EndPnt[0].X == EndPnt[1].X) && (EndPnt[0].Y == EndPnt[1].Y)) {
		if (StartPnt[0].X < StartPnt[1].X) {
			if (EndPnt[0].X < StartPnt[0].X) {
				drawDirA[0] = -1;
			}
			else if (EndPnt[0].X > StartPnt[1].X) {
				drawDirA[1] = -1;
			}
		}
		else if (StartPnt[0].X > StartPnt[1].X) {
			if (EndPnt[0].X < StartPnt[1].X) {
				drawDirA[1] = -1;
			}
			else if (EndPnt[0].X > StartPnt[0].X) {
				drawDirA[0] = -1;
			}
		}
		else {
			if (StartPnt[0].Y < StartPnt[1].Y) {
				if (EndPnt[0].Y < StartPnt[0].Y) {
					drawDirA[0] = -1;
				}
				else if (EndPnt[0].Y > StartPnt[1].Y) {
					drawDirA[1] = -1;
				}
			}
			else if (StartPnt[0].Y > StartPnt[1].Y) {
				if (EndPnt[0].Y < StartPnt[1].Y) {
					drawDirA[1] = -1;
				}
				else if (EndPnt[0].Y > StartPnt[0].Y) {
					drawDirA[0] = -1;
				}
			}
		}
	}

	for (int i = 0; i<2; i++) {
		drawDir.push_back(drawDirA[i]);
	}

	return true;
}
/*
* 功能：Leader 裁剪

* 参数：dim                   (I) 维度

*       inPnt                 (I) Leader 点列数据

*       outPnt                (O) Leader 裁剪后的点列数据

* 返回值： 执行成功/失败

*
*/
STK_BOOL CSVLXPMIUtilityLib::DivideLeader(
	STK_UINT32			dim,		/*(i) 维度						*/
	vector<STK_BOX32> TextBox,
	vector<STK_PNT32>	inPnt,		/*(i) Leader 点列数据			*/
	vector<STK_PNT32>	&outPnt//,		/*(o) Leader 裁剪后的点列数据	*/
							   //bool bConverted
) {
	STK_BOX32 box, newbox;
	vector<STK_BOOL> outChk;
	STK_PNT32 lin1_Pnt[5] = {};
	STK_PNT32 lin2_Pnt[2] = {};
	STK_UINT32 crsFlg = 0;
	STK_PNT32 crsPnt;
	STK_BOOL  blRet = false;
	vector<STK_PNT32> tmpPnt;
	STK_INT32 savidx = 0;

	STK_INT32 j = 0, m = 0, k = 0, n = 0;

	STK_FLOAT32 tmplenA[ARAAYNUM + 1];

	outPnt.clear();

	// 维度检查
	if (dim != PMIDIM2)  return false;
	for (j = 0; j<TextBox.size(); j++)
	{
		box = TextBox.at(j);
		outChk.clear();
		// Box调整
		newbox = ResetTextBox(box);

		/*** (1) 计算点列干涉 ***/
		if (!CheckPntFrmInter(dim, inPnt, newbox, outChk))  continue;


		/*** (2) 取得线框的四个顶点 ***/
		// 顺时针0->1->2->3
		lin1_Pnt[0].X = box.BoundBox[0][0];
		lin1_Pnt[0].Y = box.BoundBox[0][1];
		lin1_Pnt[0].Z = 0.0;

		lin1_Pnt[1].X = box.BoundBox[0][0];
		lin1_Pnt[1].Y = box.BoundBox[1][1];
		lin1_Pnt[1].Z = 0.0;

		lin1_Pnt[2].X = box.BoundBox[1][0];
		lin1_Pnt[2].Y = box.BoundBox[1][1];
		lin1_Pnt[2].Z = 0.0;

		lin1_Pnt[3].X = box.BoundBox[1][0];
		lin1_Pnt[3].Y = box.BoundBox[0][1];
		lin1_Pnt[3].Z = 0.0;

		// 为方便计算额外追加一个始点
		lin1_Pnt[4].X = lin1_Pnt[0].X;
		lin1_Pnt[4].Y = lin1_Pnt[0].Y;
		lin1_Pnt[4].Z = 0.0;

		/*** (3) 计算交点 ***/
		outPnt.push_back(inPnt[0]);
		for (k = 1; k<inPnt.size(); k++)
		{
			if (!outChk[k - 1])
			{
				outPnt.push_back(inPnt[k]);
				continue;
			}
			tmpPnt.clear();
			lin2_Pnt[0] = inPnt[k - 1];
			lin2_Pnt[1] = inPnt[k];
			for (m = 0; m<4; m++)
			{
				blRet = GetLineCrossPoint(dim, &lin1_Pnt[m], lin2_Pnt, crsFlg, crsPnt);
				if (blRet && crsFlg != 0)
				{
					tmpPnt.push_back(crsPnt);
				}
			}
			if (tmpPnt.empty())
			{
				outPnt.push_back(inPnt[k]);
			}
			else
			{
				if (tmpPnt.size() == 1)
				{
					outPnt.push_back(tmpPnt[0]);
				}
				else if (tmpPnt.size() >= 2 && tmpPnt.size() <= ARAAYNUM)
				{
					// 多个交点时需要特殊处理：取距离顶点最近的点
					tmplenA[ARAAYNUM] = 0.0;
					savidx = 0;
					for (n = 0; n<tmpPnt.size(); n++)
					{
						tmplenA[n] = sqrtf((tmpPnt[n].X - lin2_Pnt[0].X)*(tmpPnt[n].X - lin2_Pnt[0].X) + (tmpPnt[n].Y - lin2_Pnt[0].Y)*(tmpPnt[n].Y - lin2_Pnt[0].Y));
						if (n == 0)
						{
							tmplenA[ARAAYNUM] = tmplenA[0];
							savidx = 0;
						}
						else
						{
							if (tmplenA[ARAAYNUM] > tmplenA[n])
							{
								tmplenA[ARAAYNUM] = tmplenA[n];
								savidx = n;
							}
						}
					}
					outPnt.push_back(tmpPnt[savidx]);
				}
				else
				{
					// ERROR
					outPnt.push_back(tmpPnt[0]);
				}
				// 多个文本框裁剪的情况需要进一步的测试
				//if(bConverted)
				break;
			}
		}
	}

	return true;
}

// 重新计算包围盒数据 
STK_BOX32 CSVLXPMIUtilityLib::ResetTextBox(STK_BOX32 inbox)
{
	STK_BOX32 outbox;
	if (inbox.BoundBox[0][0] < inbox.BoundBox[1][0]) {
		outbox.BoundBox[0][0] = inbox.BoundBox[0][0];
		outbox.BoundBox[1][0] = inbox.BoundBox[1][0];
	}
	else {
		outbox.BoundBox[0][0] = inbox.BoundBox[1][0];
		outbox.BoundBox[1][0] = inbox.BoundBox[0][0];
	}
	if (inbox.BoundBox[0][1] < inbox.BoundBox[1][1]) {
		outbox.BoundBox[0][1] = inbox.BoundBox[0][1];
		outbox.BoundBox[1][1] = inbox.BoundBox[1][1];
	}
	else {
		outbox.BoundBox[0][1] = inbox.BoundBox[1][1];
		outbox.BoundBox[1][1] = inbox.BoundBox[0][1];
	}

	outbox.BoundBox[0][2] = 0.0;
	outbox.BoundBox[1][2] = 0.0;

	return outbox;
}
/*
* 功能：判断点列与外框是否干涉

* 参数：dim,                  (I) 维度

*       inPntA                (I) 点列坐标

*       inBox                 (I) 外框BOX

*       outChk                (O) 检查结果 T:干涉   F:不干涉

* 返回值：接口是否执行成功

*
*/
STK_BOOL CSVLXPMIUtilityLib::CheckPntFrmInter(
	STK_UINT32		dim,			/*(i) 维度						*/
	vector<STK_PNT32> inPntA,		/*(i) 点列坐标					*/
	STK_BOX32		inBox,			/*(i) 外框BOX					*/
	vector<STK_BOOL> &outChk		/*(o) 干涉检查结果				*/
) {
	STK_BOOL		Chk;
	STK_PNT32		pntA[2];
	STK_BOX32		chkBox;
	STK_BOOL		IsUse = false;

	if (dim != PMIDIM2) {
		return false;
	}
	outChk.clear();
	chkBox.BoundBox[0][2] = 0.0;
	chkBox.BoundBox[1][2] = 0.0;
	for (int i = 1; i<inPntA.size(); i++) {
		Chk = true;
		pntA[0] = inPntA.at(i - 1);
		pntA[1] = inPntA.at(i);

		// 计算点列的BOX
		if (pntA[0].X < pntA[1].X) {
			chkBox.BoundBox[0][0] = pntA[0].X;
			chkBox.BoundBox[1][0] = pntA[1].X;
		}
		else {
			chkBox.BoundBox[0][0] = pntA[1].X;
			chkBox.BoundBox[1][0] = pntA[0].X;
		}

		if (pntA[0].Y < pntA[1].Y) {
			chkBox.BoundBox[0][1] = pntA[0].Y;
			chkBox.BoundBox[1][1] = pntA[1].Y;
		}
		else {
			chkBox.BoundBox[0][1] = pntA[1].Y;
			chkBox.BoundBox[1][1] = pntA[0].Y;
		}

		// 干涉检查
		// X方向判定
		if (chkBox.BoundBox[1][0] < inBox.BoundBox[0][0] - ZTOL || chkBox.BoundBox[0][0] > inBox.BoundBox[1][0] + ZTOL) {
			Chk = false;
		}
		// Y方向判定
		if (chkBox.BoundBox[1][1] < inBox.BoundBox[0][1] - ZTOL || chkBox.BoundBox[0][1] > inBox.BoundBox[1][1] + ZTOL) {
			Chk = false;
		}
		if (Chk) IsUse = true;
		outChk.push_back(Chk);
	}

	return IsUse;
}

/*
* 功能：已知线段端点，二维线段求交

* 方程：y = kx + b

* 参数：dim,                  (I) 维度

*       inLin1_PntA[2]        (I) 线1两端点

*       inLin2_PntA[2]        (I) 线2两端点

*       crossFlg              (O) 平行/存在交点

*       crossPnt              (O) 交点坐标

* 返回值： 交点是否有效/接口是否执行成功

*
*/
STK_BOOL CSVLXPMIUtilityLib::GetLineCrossPoint(
	STK_UINT32	dim,				/*(i) 维度						*/
	STK_PNT32	inLin1_PntA[2],		/*(i) 线1两端点(线框点)			*/
	STK_PNT32	inLin2_PntA[2],		/*(i) 线2两端点					*/
	STK_UINT32	&crossFlg,			/*(o) 平行/重合/相交			*/
									/*   ==0 平行   ==1 重合   ==2 相交		*/
	STK_PNT32	&crossPnt			/*(o) 交点坐标/重合时输出线框顶点坐标	*/
) {
	STK_BOOL	blRet = true;
	STK_FLOAT32	Lin1_X = 0.0;
	STK_FLOAT32	Lin1_Y = 0.0;
	STK_FLOAT32	Lin1_K = 0.0;
	STK_FLOAT32	Lin1_B = 0.0;
	STK_FLOAT32	Lin1_minX, Lin1_maxX;
	STK_FLOAT32	Lin1_minY, Lin1_maxY;
	STK_BOOL	IsLin1_X;

	STK_FLOAT32	Lin2_X = 0.0;
	STK_FLOAT32	Lin2_Y = 0.0;
	STK_FLOAT32	Lin2_K = 0.0;
	STK_FLOAT32	Lin2_B = 0.0;
	STK_FLOAT32	Lin2_minX, Lin2_maxX;
	STK_FLOAT32	Lin2_minY, Lin2_maxY;
	STK_BOOL	IsLin2_X;

	STK_PNT32	crsPnt;

	crossFlg = -1;
	//ZeroMemory(&crossPnt, sizeof(STK_PNT32));
	//ZeroMemory(&crsPnt, sizeof(STK_PNT32));
	memset(&crossPnt, 0, sizeof(STK_PNT32));
	memset(&crsPnt, 0, sizeof(STK_PNT32));

	Lin1_minX = Lin2_minX = 0.0;
	Lin1_maxX = Lin2_maxX = 0.0;
	Lin1_minY = Lin2_minY = 0.0;
	Lin1_maxY = Lin2_maxY = 0.0;
	IsLin1_X = IsLin2_X = false;

	// 判断维度
	if (PMIDIM2 != dim) {
		return false;
	}

	//###### (1)计算斜率 ######
	// 线1斜率
	if (inLin1_PntA[1].X == inLin1_PntA[0].X) {
		// 与坐标轴平行?
		if (inLin1_PntA[1].Y == inLin1_PntA[0].Y) {
			// 两点重合
			return false;
		}
		else {
			// 方程是X=inLin1_PntA[0].X
			IsLin1_X = true;
			Lin1_minY = inLin1_PntA[1].Y > inLin1_PntA[0].Y ? inLin1_PntA[0].Y : inLin1_PntA[1].Y;
			Lin1_maxY = inLin1_PntA[1].Y < inLin1_PntA[0].Y ? inLin1_PntA[0].Y : inLin1_PntA[1].Y;
		}
	}
	else {
		Lin1_K = (inLin1_PntA[1].Y - inLin1_PntA[0].Y) / (inLin1_PntA[1].X - inLin1_PntA[0].X);
		// 取得线1的x有效区间
		if (inLin1_PntA[1].X > inLin1_PntA[0].X) {
			Lin1_minX = inLin1_PntA[0].X;
			Lin1_maxX = inLin1_PntA[1].X;
		}
		else {
			Lin1_minX = inLin1_PntA[1].X;
			Lin1_maxX = inLin1_PntA[0].X;
		}
	}
	// 线2斜率
	if (inLin2_PntA[1].X == inLin2_PntA[0].X) {
		// 与坐标轴平行?
		if (inLin2_PntA[1].Y == inLin2_PntA[0].Y) {
			// 两点重合
			return false;
		}
		else {
			// 方程是X=inLin2_PntA[0].X
			IsLin2_X = true;
			Lin2_minY = inLin2_PntA[1].Y > inLin2_PntA[0].Y ? inLin2_PntA[0].Y : inLin2_PntA[1].Y;
			Lin2_maxY = inLin2_PntA[1].Y < inLin2_PntA[0].Y ? inLin2_PntA[0].Y : inLin2_PntA[1].Y;
		}
	}
	else {
		Lin2_K = (inLin2_PntA[1].Y - inLin2_PntA[0].Y) / (inLin2_PntA[1].X - inLin2_PntA[0].X);
		// 取得线2的x有效区间
		if (inLin2_PntA[1].X > inLin2_PntA[0].X) {
			Lin2_minX = inLin2_PntA[0].X;
			Lin2_maxX = inLin2_PntA[1].X;
		}
		else {
			Lin2_minX = inLin2_PntA[1].X;
			Lin2_maxX = inLin2_PntA[0].X;
		}
	}

	//###### (2)计算交点 ######
	// case1: 两线段Y轴平行
	if (IsLin1_X || IsLin2_X) {
		crossFlg = 2;
		if (IsLin1_X && IsLin2_X) {
			if (inLin1_PntA[0].X == inLin2_PntA[0].X) {
				// 重合
				crossFlg = 1;
				crsPnt.X = inLin1_PntA[0].X;
				crsPnt.Y = inLin1_PntA[0].Y <= inLin1_PntA[1].Y ? inLin1_PntA[0].Y : inLin1_PntA[1].Y;
			}
			else {
				crossFlg = 0;
			}
		}
		else if (IsLin1_X) {
			Lin2_B = inLin2_PntA[0].Y - inLin2_PntA[0].X*Lin2_K;
			crsPnt.X = inLin1_PntA[0].X;
			crsPnt.Y = Lin2_K*crsPnt.X + Lin2_B;
		}
		else if (IsLin2_X) {
			Lin1_B = inLin1_PntA[0].Y - inLin1_PntA[0].X*Lin1_K;
			crsPnt.X = inLin2_PntA[0].X;
			crsPnt.Y = Lin1_K*crsPnt.X + Lin1_B;
		}
	}
	else {
		// case2: 判断是否平行
		Lin1_B = inLin1_PntA[0].Y - inLin1_PntA[0].X*Lin1_K;
		Lin2_B = inLin2_PntA[0].Y - inLin2_PntA[0].X*Lin2_K;
		if (Lin1_K == Lin2_K) {
			crossFlg = 0;
			// 判断是否重合：(k1 == k2) && (b1 == b2)时，重合
			if (Lin1_B == Lin2_B) {
				crossFlg = 1;
				//if( Lin1_K > 0.0 ) {
				Lin1_minX = inLin1_PntA[0].X <= inLin1_PntA[1].X ? inLin1_PntA[0].X : inLin1_PntA[1].X;
				Lin2_minX = inLin2_PntA[0].X <= inLin2_PntA[1].X ? inLin2_PntA[0].X : inLin2_PntA[1].X;
				crsPnt.X = Lin1_minX <= Lin2_minX ? Lin1_minX : Lin2_minX;
				crsPnt.Y = Lin1_K*crsPnt.X + Lin1_B;
				//} else if ( Lin1_K < 0.0 ) {
				//	Lin1_minX = inLin1_PntA[0].X <= inLin1_PntA[1].X ? inLin1_PntA[0].X : inLin1_PntA[1].X;
				//	Lin2_minX = inLin2_PntA[0].X <= inLin2_PntA[1].X ? inLin2_PntA[0].X : inLin2_PntA[1].X;
				//	crsPnt.X = Lin1_minX <= Lin2_minX ? Lin1_minX: Lin2_minX;
				//	crsPnt.Y = Lin1_K*crsPnt.X + Lin1_B;
				//}
			}
		}
		else {
			crossFlg = 2;
			// b1 == b2时，两线段交点为(0,b)
			if (Lin1_B == Lin2_B) {
				crsPnt.X = 0.0;
				crsPnt.Y = Lin1_B;
			}
			else {
				// 交点 x=(b2-b1)/(k1-k2)
				crsPnt.X = (Lin2_B - Lin1_B) / (Lin1_K - Lin2_K);
				crsPnt.Y = Lin1_K*crsPnt.X + Lin1_B;
			}
		}
	}

	//###### (3)判断交点有效性 ######
	if (crossFlg != 0) {
		if (IsLin1_X) {
			if (crsPnt.Y > Lin1_maxY || crsPnt.Y < Lin1_minY) {
				blRet = false;
			}
		}
		else if (crsPnt.X > Lin1_maxX || crsPnt.X < Lin1_minX) {
			blRet = false;
		}
		if (IsLin2_X) {
			if (crsPnt.Y > Lin2_maxY || crsPnt.Y < Lin2_minY) {
				blRet = false;
			}
		}
		else if (crsPnt.X > Lin2_maxX || crsPnt.X < Lin2_minX) {
			blRet = false;
		}
	}

	memcpy(&crossPnt, &crsPnt, sizeof(STK_PNT32));

	return blRet;
}