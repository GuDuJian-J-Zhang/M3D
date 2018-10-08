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
#include "PMIUtilityLib.h"
#include <math.h>
#include "sview/PMIAnalysis/toolkit/MathVector.h"
//#include "DimSymUitity.h"
#include <stdlib.h>

#define ARAAYNUM		(4)
#define PMIDIM2			(2)	
#define ZTOL			(2e-10)		// 干涉公差
CPMIUtilityLib::CPMIUtilityLib(void)
{
}

CPMIUtilityLib::~CPMIUtilityLib(void)
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
STK_BOOL CPMIUtilityLib::GetEndSymbolDirection(
		vector<Stk_Leader*>		leaders,		/*(i) PMI引出线数据				*/
		vector<STK_INT32>		&drawDir		/*(o) 圆弧绘制方向				*/
												/*  ==  1 原方向				*/
												/*  == -1 反方向				*/
){
	Stk_PolyLine	*polyline;					// 折线
	vector<STK_PNT32> pnt;
	STK_PNT32	StartPnt[2], EndPnt[2];
	STK_INT32	drawDirA[2];

	if ( leaders.size() != 2 ) {
		return FALSE;
	}
	for (int i=0; i<leaders.size(); i++) {
		Stk_Leader * leader = leaders.at(i);				// 获得一条引出线数据
		STK_UINT32 type = leader->GetLeaderType();			// 获得引出线类型

		if ( type == 1 ) {
			pnt.clear();
			polyline = (Stk_PolyLine *)leader->GetCurve();	// 获得引出线的线条数据
			pnt = polyline->GetPoints();					// 获得离散点数据
			StartPnt[i] = pnt.at(0);
			EndPnt[i]   = pnt.at(1);
		} else {
			return FALSE;
		}
	}

	// 绘制方向重新计算
	drawDirA[0] = drawDirA[1] = +1;
	if ( (EndPnt[0].X == EndPnt[1].X) && (EndPnt[0].Y == EndPnt[1].Y) ) {
		if ( StartPnt[0].X < StartPnt[1].X ) {
			if ( EndPnt[0].X < StartPnt[0].X ) {
				drawDirA[0] = -1;
			} else if ( EndPnt[0].X > StartPnt[1].X ) {
				drawDirA[1] = -1;
			}
		} else if ( StartPnt[0].X > StartPnt[1].X ) {
			if ( EndPnt[0].X < StartPnt[1].X ) {
				drawDirA[1] = -1;
			} else if ( EndPnt[0].X > StartPnt[0].X ) {
				drawDirA[0] = -1;
			}
		} else {
			if ( StartPnt[0].Y < StartPnt[1].Y ) {
				if ( EndPnt[0].Y < StartPnt[0].Y ) {
					drawDirA[0] = -1;
				} else if ( EndPnt[0].Y > StartPnt[1].Y ) {
					drawDirA[1] = -1;
				}
			} else if ( StartPnt[0].Y > StartPnt[1].Y ) {
				if ( EndPnt[0].Y < StartPnt[1].Y ) {
					drawDirA[1] = -1;
				} else if ( EndPnt[0].Y > StartPnt[0].Y ) {
					drawDirA[0] = -1;
				}
			}
		}
	}

	for ( int i=0; i<2; i++ ) {
		drawDir.push_back(drawDirA[i]);
	}

	return TRUE;
}

/*
 * 功能：Leader 裁剪

 * 参数：dim                   (I) 维度
 
 *       inPnt                 (I) Leader 点列数据

 *       outPnt                (O) Leader 裁剪后的点列数据

 * 返回值： 执行成功/失败

 *
*/
STK_BOOL CPMIUtilityLib::DivideLeader(
		STK_UINT32			dim,		/*(i) 维度						*/
		vector<STK_BOX32> TextBox,
		vector<STK_PNT32>	inPnt,		/*(i) Leader 点列数据			*/
		vector<STK_PNT32>	&outPnt//,		/*(o) Leader 裁剪后的点列数据	*/
		//bool bConverted
){
	STK_BOX32 box, newbox;
	vector<STK_BOOL> outChk;
	STK_PNT32 lin1_Pnt[5] = {};
	STK_PNT32 lin2_Pnt[2] = {};
	STK_UINT32 crsFlg = 0;
	STK_PNT32 crsPnt;
	STK_BOOL  blRet = false;
	vector<STK_PNT32> tmpPnt;
	STK_INT32 savidx = 0;

	STK_INT32 j = 0,m = 0,k = 0,n = 0;

	STK_FLOAT32 tmplenA[ARAAYNUM+1];

	outPnt.clear();

	// 维度检查
	if ( dim != PMIDIM2 )  return FALSE;
	for ( j=0; j<TextBox.size(); j++) 
	{
		box = TextBox.at(j);
		outChk.clear();
		// Box调整
		newbox = ResetTextBox(box);

		/*** (1) 计算点列干涉 ***/ 
		if ( !CheckPntFrmInter(dim, inPnt, newbox, outChk) )  continue;
		

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
		for ( k=1; k<inPnt.size(); k++) 
		{
			if ( !outChk[k-1] ) 
			{
				outPnt.push_back(inPnt[k]);
				continue;
			}
			tmpPnt.clear();
			lin2_Pnt[0] = inPnt[k-1];
			lin2_Pnt[1] = inPnt[k];
			for ( m=0; m<4; m++) 
			{
				blRet = GetLineCrossPoint(dim, &lin1_Pnt[m], lin2_Pnt, crsFlg, crsPnt);
				if ( blRet && crsFlg!=0 ) 
				{
					tmpPnt.push_back(crsPnt);
				}
			}
			if ( tmpPnt.empty() ) 
			{
				outPnt.push_back(inPnt[k]);
			} 
			else 
			{
				if ( tmpPnt.size() == 1 ) 
				{
					outPnt.push_back(tmpPnt[0]);
				} 
				else if (tmpPnt.size() >= 2 && tmpPnt.size() <= ARAAYNUM) 
				{
					// 多个交点时需要特殊处理：取距离顶点最近的点
					tmplenA[ARAAYNUM] = 0.0;
					savidx = 0;
					for ( n=0; n<tmpPnt.size(); n++) 
					{
						tmplenA[n] = sqrtf((tmpPnt[n].X-lin2_Pnt[0].X)*(tmpPnt[n].X-lin2_Pnt[0].X)+(tmpPnt[n].Y-lin2_Pnt[0].Y)*(tmpPnt[n].Y-lin2_Pnt[0].Y));
						if( n == 0 ) 
						{
							tmplenA[ARAAYNUM] = tmplenA[0];
							savidx = 0;
						} 
						else 
						{
							if ( tmplenA[ARAAYNUM] > tmplenA[n] ) 
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

	return TRUE;
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
STK_BOOL CPMIUtilityLib::CheckPntFrmInter(
		STK_UINT32		dim,			/*(i) 维度						*/
		vector<STK_PNT32> inPntA,		/*(i) 点列坐标					*/
		STK_BOX32		inBox,			/*(i) 外框BOX					*/
		vector<STK_BOOL> &outChk		/*(o) 干涉检查结果				*/
){
	STK_BOOL		Chk;
	STK_PNT32		pntA[2];
	STK_BOX32		chkBox;
	STK_BOOL		IsUse = FALSE;

	if ( dim != PMIDIM2 ) {
		return FALSE;
	}
	outChk.clear();
	chkBox.BoundBox[0][2] = 0.0;
	chkBox.BoundBox[1][2] = 0.0;
	for (int i=1; i<inPntA.size(); i++) {
		Chk = TRUE;
		pntA[0] = inPntA.at(i-1);
		pntA[1] = inPntA.at(i);

		// 计算点列的BOX
		if ( pntA[0].X < pntA[1].X ) {
			chkBox.BoundBox[0][0] = pntA[0].X;
			chkBox.BoundBox[1][0] = pntA[1].X;
		} else {
			chkBox.BoundBox[0][0] = pntA[1].X;
			chkBox.BoundBox[1][0] = pntA[0].X;
		}

		if ( pntA[0].Y < pntA[1].Y ) {
			chkBox.BoundBox[0][1] = pntA[0].Y;
			chkBox.BoundBox[1][1] = pntA[1].Y;
		} else {
			chkBox.BoundBox[0][1] = pntA[1].Y;
			chkBox.BoundBox[1][1] = pntA[0].Y;
		}

		// 干涉检查
		// X方向判定
		if ( chkBox.BoundBox[1][0] < inBox.BoundBox[0][0] - ZTOL || chkBox.BoundBox[0][0] > inBox.BoundBox[1][0] + ZTOL ) {
			Chk = FALSE;
		}
		// Y方向判定
		if ( chkBox.BoundBox[1][1] < inBox.BoundBox[0][1] - ZTOL || chkBox.BoundBox[0][1] > inBox.BoundBox[1][1] + ZTOL ) {
			Chk = FALSE;
		}
		if (Chk) IsUse = TRUE;
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
STK_BOOL CPMIUtilityLib::GetLineCrossPoint(
		STK_UINT32	dim,				/*(i) 维度						*/
		STK_PNT32	inLin1_PntA[2],		/*(i) 线1两端点(线框点)			*/
		STK_PNT32	inLin2_PntA[2],		/*(i) 线2两端点					*/
		STK_UINT32	&crossFlg,			/*(o) 平行/重合/相交			*/
										/*   ==0 平行   ==1 重合   ==2 相交		*/
		STK_PNT32	&crossPnt			/*(o) 交点坐标/重合时输出线框顶点坐标	*/
){
		STK_BOOL	blRet = TRUE;
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
		memset(&crossPnt,0,sizeof(STK_PNT32));
		memset(&crsPnt, 0, sizeof(STK_PNT32));

		Lin1_minX = Lin2_minX = 0.0;
		Lin1_maxX = Lin2_maxX = 0.0;
		Lin1_minY = Lin2_minY = 0.0;
		Lin1_maxY = Lin2_maxY = 0.0;
		IsLin1_X = IsLin2_X = FALSE;

		// 判断维度
		if ( PMIDIM2 != dim ) {
			return FALSE;
		}

		//###### (1)计算斜率 ######
		// 线1斜率
		if ( inLin1_PntA[1].X == inLin1_PntA[0].X ) {
			// 与坐标轴平行?
			if ( inLin1_PntA[1].Y == inLin1_PntA[0].Y ) {
				// 两点重合
				return FALSE;
			} else {
				// 方程是X=inLin1_PntA[0].X
				IsLin1_X = TRUE;
				Lin1_minY = inLin1_PntA[1].Y > inLin1_PntA[0].Y ? inLin1_PntA[0].Y : inLin1_PntA[1].Y;
				Lin1_maxY = inLin1_PntA[1].Y < inLin1_PntA[0].Y ? inLin1_PntA[0].Y : inLin1_PntA[1].Y;
			}
		} else {
			Lin1_K = (inLin1_PntA[1].Y-inLin1_PntA[0].Y)/(inLin1_PntA[1].X-inLin1_PntA[0].X);
			// 取得线1的x有效区间
			if ( inLin1_PntA[1].X > inLin1_PntA[0].X ) {
				Lin1_minX = inLin1_PntA[0].X;
				Lin1_maxX = inLin1_PntA[1].X;
			} else {
				Lin1_minX = inLin1_PntA[1].X;
				Lin1_maxX = inLin1_PntA[0].X;
			}
		}
		// 线2斜率
		if ( inLin2_PntA[1].X == inLin2_PntA[0].X  ) {
			// 与坐标轴平行?
			if ( inLin2_PntA[1].Y == inLin2_PntA[0].Y ) {
				// 两点重合
				return FALSE;
			} else {
				// 方程是X=inLin2_PntA[0].X
				IsLin2_X = TRUE;
				Lin2_minY = inLin2_PntA[1].Y > inLin2_PntA[0].Y ? inLin2_PntA[0].Y : inLin2_PntA[1].Y;
				Lin2_maxY = inLin2_PntA[1].Y < inLin2_PntA[0].Y ? inLin2_PntA[0].Y : inLin2_PntA[1].Y;
			}
		} else {
			Lin2_K = (inLin2_PntA[1].Y-inLin2_PntA[0].Y)/(inLin2_PntA[1].X-inLin2_PntA[0].X);
			// 取得线2的x有效区间
			if ( inLin2_PntA[1].X > inLin2_PntA[0].X ) {
				Lin2_minX = inLin2_PntA[0].X;
				Lin2_maxX = inLin2_PntA[1].X;
			} else {
				Lin2_minX = inLin2_PntA[1].X;
				Lin2_maxX = inLin2_PntA[0].X;
			}
		}

		//###### (2)计算交点 ######
		// case1: 两线段Y轴平行
		if ( IsLin1_X || IsLin2_X ) {
			crossFlg = 2;
			if ( IsLin1_X && IsLin2_X ) {
				if ( inLin1_PntA[0].X == inLin2_PntA[0].X ) {
					// 重合
					crossFlg = 1;
					crsPnt.X = inLin1_PntA[0].X;
					crsPnt.Y = inLin1_PntA[0].Y <= inLin1_PntA[1].Y ? inLin1_PntA[0].Y : inLin1_PntA[1].Y;
				} else {
					crossFlg = 0;
				}
			} else if ( IsLin1_X ) {
				Lin2_B = inLin2_PntA[0].Y-inLin2_PntA[0].X*Lin2_K;
				crsPnt.X = inLin1_PntA[0].X;
				crsPnt.Y = Lin2_K*crsPnt.X + Lin2_B;
			} else if ( IsLin2_X ) {
				Lin1_B = inLin1_PntA[0].Y-inLin1_PntA[0].X*Lin1_K;
				crsPnt.X = inLin2_PntA[0].X;
				crsPnt.Y = Lin1_K*crsPnt.X + Lin1_B;
			}
		} else {
			// case2: 判断是否平行
			Lin1_B = inLin1_PntA[0].Y-inLin1_PntA[0].X*Lin1_K;
			Lin2_B = inLin2_PntA[0].Y-inLin2_PntA[0].X*Lin2_K;
			if ( Lin1_K == Lin2_K ) {
				crossFlg = 0;
				// 判断是否重合：(k1 == k2) && (b1 == b2)时，重合
				if ( Lin1_B == Lin2_B ) {
					crossFlg = 1;
					//if( Lin1_K > 0.0 ) {
						Lin1_minX = inLin1_PntA[0].X <= inLin1_PntA[1].X ? inLin1_PntA[0].X : inLin1_PntA[1].X;
						Lin2_minX = inLin2_PntA[0].X <= inLin2_PntA[1].X ? inLin2_PntA[0].X : inLin2_PntA[1].X;
						crsPnt.X = Lin1_minX <= Lin2_minX ? Lin1_minX: Lin2_minX;
						crsPnt.Y = Lin1_K*crsPnt.X + Lin1_B;
					//} else if ( Lin1_K < 0.0 ) {
					//	Lin1_minX = inLin1_PntA[0].X <= inLin1_PntA[1].X ? inLin1_PntA[0].X : inLin1_PntA[1].X;
					//	Lin2_minX = inLin2_PntA[0].X <= inLin2_PntA[1].X ? inLin2_PntA[0].X : inLin2_PntA[1].X;
					//	crsPnt.X = Lin1_minX <= Lin2_minX ? Lin1_minX: Lin2_minX;
					//	crsPnt.Y = Lin1_K*crsPnt.X + Lin1_B;
					//}
				}
			} else {
				crossFlg = 2;
				// b1 == b2时，两线段交点为(0,b)
				if ( Lin1_B == Lin2_B ) {
					crsPnt.X = 0.0;
					crsPnt.Y = Lin1_B;
				} else {
					// 交点 x=(b2-b1)/(k1-k2)
					crsPnt.X = (Lin2_B-Lin1_B)/(Lin1_K-Lin2_K);
					crsPnt.Y = Lin1_K*crsPnt.X + Lin1_B;
				}
			}
		}

		//###### (3)判断交点有效性 ######
		if ( crossFlg != 0 ) {
			if ( IsLin1_X ) {
				if ( crsPnt.Y > Lin1_maxY || crsPnt.Y < Lin1_minY ) {
					blRet = FALSE;
				}
			} else if ( crsPnt.X > Lin1_maxX || crsPnt.X < Lin1_minX ) {
				blRet = FALSE;
			}
			if ( IsLin2_X ) {
				if ( crsPnt.Y > Lin2_maxY || crsPnt.Y < Lin2_minY ) {
					blRet = FALSE;
				}
			} else if ( crsPnt.X > Lin2_maxX || crsPnt.X < Lin2_minX ) {
				blRet = FALSE;
			}
		}

		memcpy( &crossPnt, &crsPnt, sizeof(STK_PNT32) );

		return blRet;
}

// 重新计算包围盒数据 
STK_BOX32 CPMIUtilityLib::ResetTextBox( STK_BOX32 inbox )
{
	STK_BOX32 outbox;
	if ( inbox.BoundBox[0][0] < inbox.BoundBox[1][0] ) {
		outbox.BoundBox[0][0] = inbox.BoundBox[0][0];
		outbox.BoundBox[1][0] = inbox.BoundBox[1][0];
	} else {
		outbox.BoundBox[0][0] = inbox.BoundBox[1][0];
		outbox.BoundBox[1][0] = inbox.BoundBox[0][0];
	}
	if ( inbox.BoundBox[0][1] < inbox.BoundBox[1][1] ) {
		outbox.BoundBox[0][1] = inbox.BoundBox[0][1];
		outbox.BoundBox[1][1] = inbox.BoundBox[1][1];
	} else {
		outbox.BoundBox[0][1] = inbox.BoundBox[1][1];
		outbox.BoundBox[1][1] = inbox.BoundBox[0][1];
	}

	outbox.BoundBox[0][2] = 0.0;
	outbox.BoundBox[1][2] = 0.0;

	return outbox;
}

// 获取包围盒数据 
vector<STK_BOX32> CPMIUtilityLib::GetTextBox(Stk_ComText *i_pComText)
{

	int i = 0,j = 0;

	vector<Stk_Text*> Texts = i_pComText->GetTexts();
	STK_BOX32 Box = i_pComText->GetOuterBox();
	float textlen = 0.0f;
	float textlen2 = 0.0f;
	float textlen3 = 0.0f;
	float PosX = 0.0f;
	float PosY = 0.0f;

	vector<STK_BOX32> vecTxtBox;
	vecTxtBox.clear();
	STK_BOX32 inbox;



	///* 设置文字起始点位置 */
	//for (int i=0; i<Texts.size(); i++){
	//	Stk_Text *Text = Texts.at(i);

	//
	//	STK_PNT32 Pos = Text->GetInnerLoc();

	//	//设置两个字符串之间的间隔
	//	if(PMIType==907){	//长度尺寸做特殊处理
	//		if(textlen2 !=0.0f ){
	//			if (i>1){
	//				textlen2 += textlen3;
	//			}
	//			Pos.X = PosX+textlen2;
	//		}
	//	}
	//	else{
	//		if (textlen2 != 0.0f && abs(Pos.X-PosX) < textlen2 && PosY==Pos.Y ){
	//			textlen2 += textlen3;
	//			Pos.X = PosX+textlen2;
	//		}
	//	}
	//
	//	if (i==0){
	//		PosX = Pos.X;
	//		PosY = Pos.Y;
	//	}

	//	//文字方向
	//	STK_DIR32 XAxis, YAxis;
	//	Text->GetInnerXYAxis(XAxis,YAxis);
	//

	//	//文字高度
	//	FLOAT Height,Width,ScaleH,ScaleW;
	//	Text->GetCharWidthHeight(Width, Height);
	//	ScaleW = Width*0.08;
	//	ScaleH = Width*0.08;
	//

	//	/* 文字间隔 */
	//	DOUBLE ShiftX = 0.0,ShiftY = 0.0 ;
	//	FLOAT ChrSpc =0.0f;	// 文字间隔
	//	ChrSpc = Text->GetCharSpacing();
	//	ShiftX = ChrSpc;


	//	//获取文字列
	//	wstring str;
	//	str = Text->GetText();

	//	wstring unicodetext;
	//	float textlen1=0.0f;
	//	for ( j=0; j<str.size(); j++)
	//	{
	//		
	//		//textpnt.clear();
	//
	//		//int	IndexNum = count = 0;
	//		//unicodetext.push_back(str.c_str()[j]);

	//		///* 获取文字的点列数据 */
	//		//MaxVerX=0.0f,MaxVerY=0.0f;
	//		//MinVerX=0.0f,MinVerY=0.0f;
	//	
	//		//if (textpnt.size() > 0)
	//		//{
	//		//	MinVerX = textpnt[0].Xf();
	//		//	MinVerY = textpnt[0].Yf();
	//		//}

	//		///* 计算文字包围盒大小 */
	//		//for (int r=0; r<textpnt.size(); r++)
	//		//{
	//		//	MaxVerX = (MaxVerX < textpnt[r].Xf())?textpnt[r].Xf():MaxVerX;
	//		//	MaxVerY = (MaxVerY < textpnt[r].Yf())?textpnt[r].Yf():MaxVerY;
	//		//	MinVerX = (MinVerX > textpnt[r].Xf())?textpnt[r].Xf():MinVerX;
	//		//	MinVerY = (MinVerY > textpnt[r].Yf())?textpnt[r].Yf():MinVerY;
	//		//}
	//		//lenw = (MaxVerX-MinVerX)/64.0f;
	//		//lenh = (MaxVerY-MinVerY)/64.0f;
	//		//lenw = lenw>baselenw?lenw:baselenw;

	//		//textlen1 += lenw+ShiftX;

	//		
	//		textlen3 = textlen1 *= ScaleW;
	//		textlen += textlen1;
	//		if((_finite(textlen1))&&(_finite(textlen3))&&(_finite(textlen)))
	//		{
	//			if (i==0)
	//			{
	//				textlen2 = textlen;
	//			}
	//			
	//		}
	//	}
	//	if(_finite(textlen))
	//	{
	//		if (Box.BoundBox[1][0]-Box.BoundBox[0][0] < textlen)
	//		{
	//			Box.BoundBox[1][0]=Box.BoundBox[0][0]+textlen;
	//		}
	//		
	//		vecTxtBox.push_back(Box);
	//	}
	//
	return vecTxtBox;
}
void CPMIUtilityLib::UpdateEndSymData(
										Stk_PMI* ipDimSym,
										Stk_Leader* ipStkLeader,
										StkTermTypeEnum&	oTermType,		//末端符号类型
										STK_FLOAT32			oSize[2],		//末端符号宽和高
										STK_PNT32&			oLoc,			//末端符号起始位置
										STK_DIR32&			oDir		//末端符号方向
)
{
	StkPMISourceEnum enumSource = ipDimSym->GetPMISource();
	double fRaduis = 0.0;
	STK_PNT32 centreCirPnt,stkStartPnt,stkEndPnt;
	StkCurveTypeEnum EnumCurveType = CURVE_TYPE_UNKNOWN;
	vector<Stk_CurveEntity*>polylines = ipStkLeader->GetAllCurve();
	Stk_CurveEntity* stkCurveline = NULL;
	Stk_PolyLine* pStkPloyLine  = NULL;
	if(polylines.size() > 0)//Datakit中leader只可能有条引出线
	{	
		stkCurveline = polylines.at(0);
		EnumCurveType = stkCurveline->GetCurveType();
	}
	else return;
		
	if ( oDir.X != oDir.X || oDir.Y != oDir.Y ) 
	{
		
		if(CURVE_TYPE_ELLIPSE == EnumCurveType)
		{
			Stk_Ellipse * pStkEllipse = (Stk_Ellipse*)stkCurveline;
			STK_DIR32 stkNormal = pStkEllipse->GetNormal();
			STK_DIR32 stkSrcVec ={0,0,0};;

			STK_FLOAT32 majRad = 0.0f,minRad = 0.0f;
			STK_PNT32 stkMovPnt ={0,0,0};

			double vecStart[3] ={},vecEnd[3] ={},tmpnormal[3] = {};
			double angle = 0.0;

			centreCirPnt = pStkEllipse->GetCenterPoint();
			pStkEllipse->GetCoordinatePnt(stkStartPnt,stkEndPnt);
			
			pStkEllipse->GetRadius(majRad,minRad);
			fRaduis = majRad;
			if(oSize[0]<=fRaduis/8.0&&oSize[0]>=fRaduis/10.0)
			{
				oSize[0] /= 1.5;
				oSize[1] /= 1.5;
			}
			else if(oSize[0]<=fRaduis/5.0&&oSize[0]>fRaduis/8.0)
			{
				oSize[0] /= 2.25;
				oSize[1] /= 2.25;
			}
			else if(oSize[0]<=fRaduis/3.0&&oSize[0]>fRaduis/5.0)
			{
				oSize[0] /= 3.375;
				oSize[1] /= 3.375;
			}
			else if(oSize[0]<fRaduis&&oSize[0]>fRaduis/3.0)
			{
				oSize[0] /= 5.0625;
				oSize[1] /= 5.0625;
			}
			else if(oSize[0]>=fRaduis)
			{
				oSize[0] = (fRaduis/6.0);
				oSize[1] = (fRaduis/18.0);
			}
			double sinAngle = (oSize[0]/2.0)/fRaduis;

			sinAngle = asin(sinAngle);


			

			stkSrcVec.X = (stkStartPnt.X-centreCirPnt.X)/fRaduis;
			stkSrcVec.Y = (stkStartPnt.Y-centreCirPnt.Y)/fRaduis;
			stkSrcVec.Z = (stkStartPnt.Z-centreCirPnt.Z)/fRaduis;

			vecStart[0] = stkSrcVec.X;
			vecStart[1] = stkSrcVec.Y;
			vecStart[2] = stkSrcVec.Z;


			vecEnd[0] = (stkEndPnt.X - centreCirPnt.X)/fRaduis;
			vecEnd[1] = (stkEndPnt.Y - centreCirPnt.Y)/fRaduis;
			vecEnd[2] = (stkEndPnt.Z - centreCirPnt.Z)/fRaduis;

			tmpnormal[0] = stkNormal.X;
			tmpnormal[1] = stkNormal.Y;
			tmpnormal[2] = stkNormal.Z;

			CMathVector::Angle(vecStart,vecEnd,tmpnormal,&angle);



			//stkMovPnt = CDimSymUitity::ComputeVecByAngle(stkNormal,stkSrcVec,sinAngle,centreCirPnt,stkStartPnt,fRaduis);
			vecStart[0] = (stkMovPnt.X-centreCirPnt.X)/fRaduis;
			vecStart[1] = (stkMovPnt.Y-centreCirPnt.Y)/fRaduis;
			vecStart[2] = (stkMovPnt.Z-centreCirPnt.Z)/fRaduis;

			CMathVector::Angle(vecStart,vecEnd,tmpnormal,&sinAngle);
			oDir.X = (stkMovPnt.X - stkStartPnt.X)/fRaduis;
			oDir.Y = (stkMovPnt.Y - stkStartPnt.Y)/fRaduis;
			oDir.Z = 0.0f;


			if(angle<sinAngle)
			{
				oDir.X *= -1;
				oDir.Y *= -1;
				oDir.Z *= -1;
			}
		}
		else if(CURVE_TYPE_POLYLINE == EnumCurveType)
		{
			pStkPloyLine = (Stk_PolyLine*)stkCurveline;
			vector<STK_PNT32> cuvPoints = pStkPloyLine->GetPoints();
			if(cuvPoints.size() >=2)
			{
				//计算末端记号矢量方向
				double refvec[3] = {};
				double refsize = 0.0;
				refvec[0] = cuvPoints[1].X-cuvPoints[0].X;
				refvec[1] = cuvPoints[1].Y-cuvPoints[0].Y;
				refvec[2] = cuvPoints[1].Z-cuvPoints[0].Z;

				//单位化
				refsize = sqrt(((refvec[0])*(refvec[0]))+((refvec[1])*(refvec[1]))+((refvec[2])*(refvec[2])));
				oDir.X  = (float)(refvec[0]/refsize);
				oDir.Y  = (float)(refvec[1]/refsize);
				oDir.Z  = (float)(refvec[2]/refsize);	
			}
		}	
		
	}
	if(oSize[0]<=0||oSize[1]<=0)
	{
		oSize[0] = 3.36f ;
		oSize[1] = 1.12f ;
	}
	if(fabs(oLoc.Z)>1.0)
	{
		if(CURVE_TYPE_POLYLINE == EnumCurveType)
		{
			pStkPloyLine = (Stk_PolyLine*)stkCurveline;
			vector<STK_PNT32> vecPnts = pStkPloyLine->GetPoints();
			STK_PNT32 stkRefPnt;
			int nsize = vecPnts.size();
			int i = 0;
			for(i = 1;i<nsize;i++)
			{
				STK_PNT32 pnt = vecPnts.at(i);
				if(i==1)
					stkRefPnt = pnt;
				if(fabs(pnt.Z)> 0.000001)
					return;
			}
			if(i==nsize)//说明Z分量都是0
			{
				if(stkRefPnt.X == oLoc.X&&stkRefPnt.Y==oLoc.Y&&stkRefPnt.Z!=oLoc.Z)
				{
					oLoc.Z = stkRefPnt.Z;
					if(nsize>2)
					{
						oDir.X = vecPnts.at(2).X - stkRefPnt.X; 
						oDir.Y = vecPnts.at(2).Y - stkRefPnt.Y;
						oDir.Z = 0;
						double dist = sqrt(oDir.X*oDir.X+oDir.Y*oDir.Y);
						oDir.X /= dist;
						oDir.Y /= dist;
					}
				}
			}
		}
	}
}
void CPMIUtilityLib::ResetEndSymData(Stk_Leader* ipStkLeader,
									 STK_PNT32&			ioLoc,			//末端符号起始位置
									 STK_DIR32&			ioDir)
{
	vector<Stk_CurveEntity*>vecPolyline = ipStkLeader->GetAllCurve();
	int nSize = vecPolyline.size();
	int i = 0;
	if(nSize<=0)	return;
	Stk_PolyLine * pPolyLin = (Stk_PolyLine*)vecPolyline.at(0);
	if(!pPolyLin)	return;
	vector<STK_PNT32> vecPnt = pPolyLin->GetPoints();
	nSize = vecPnt.size();
	if(nSize<=2) return;

	STK_PNT32 startPnt = vecPnt.at(0);
	STK_PNT32 endPnd   = vecPnt.at(1);
	double dist = 0.0;

	dist = sqrt((startPnt.X - endPnd.X)*(startPnt.X - endPnd.X)+
				(startPnt.Y - endPnd.Y)*(startPnt.Y - endPnd.Y)+
				(startPnt.Z - endPnd.Z)*(startPnt.Z - endPnd.Z));

	
	ioLoc.X = startPnt.X;
	ioLoc.Y = startPnt.Y;
	ioLoc.Z = startPnt.Z;
	

	
	ioDir.X = (startPnt.X - endPnd.X)/dist;
	ioDir.Y = (startPnt.Y - endPnd.Y)/dist;
	ioDir.Z = (startPnt.Z - endPnd.Z)/dist;

	
}
