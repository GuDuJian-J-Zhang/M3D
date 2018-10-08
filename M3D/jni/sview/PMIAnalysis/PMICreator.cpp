// PMICreator.cpp :PMI生成器安卓版
//

/////////////////////////////////////////////

#include "sview/SView.h"
#include "sview/PMIAnalysis/PMICreator.h"
#include "sview/PMIAnalysis/TessCircle.h"

#include "sview/PMIAnalysis/toolkit/PMIESymbol.h"
#include "sview/PMIAnalysis/PMIUtilityLib.h"
#include "prebuilt/svl2/include/Stk_Enum.h"
#include "Utility.h"

//#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <math.h>
//ISSUE:PMI生成器安卓版 define
#ifndef SO_END_FACE_INDEX // also defined in SoVRMLIndexedFaceSet.h
#define SO_END_FACE_INDEX (-1)
#endif // !SO_END_FACE_INDEX

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#define	PI		3.14159265358979323846

///////////////////////////////////////////

#define IDXNUM			(16384)
#define ZDEPS			(1.0e-18)
#define MAXPNTVAL		(1.0e+8)
#define ZTOL			(2e-10)		// 干涉公差
#define ANGLEPARA_TOL	(0.1)	// 角度参数公差
#define ARAAYNUM		(4)
#define PMIDIM2			(2)			// 二维
#define DIVANGLE		(5)			// 离散最小角度
#ifndef CALLBACK
#define CALLBACK
#endif

PMICreator::PMICreator()
{
}

PMICreator::~PMICreator()
{
}

/*
 * 功能：创建PMI引出线

 * 参数：inTopInstance    (I)实例对象指针

 *       outPntNum        (O)输出引出线离散点个数

 *        lpnt            (O)引出线离散点

 *       outIndexNum      (O)引出线索引个数

 *        lIndices     	  (O)输出引出线索引		

 * 返回值： 是否执行成功

 * 2013-07-25   xsn   create
 * 2013-08-01   yangxin 修改参数
 * 2013-08-15   yangxin 裁剪引出线
 * 2013-09-03   Liutp 裁剪引出线,删除冗余代码
 * 2013-09-04   Liutp 修改圆弧折线化的处理
 * 2013-09-05   Liutp 逻辑模块化
 * 2013-09-07   Liutp 圆弧折线化接口分割
 * 2014-01-06   gongcy 平行于屏幕的注释，对引线部分作处理
*/
//STK_UINT32 PMICreator::CreateLeaders(
//		 Stk_PMI				*pmi,		/*(i) 当前实例					*/
//		 vector<STK_PNT32>		&lpnt,		/*(o) 输出引出线离散点			*/
//		 vector<STK_UINT32>		&lIndices	/*(o) 输出引出线索引			*/
//){
//	STK_INT32			pntCnt = 0;		//点的个数
//	vector<STK_INT32>	drawFlg;
//	vector<STK_DOUBLE64>	beginPara, endPara;
//	STK_DOUBLE64		arcAngle = 0.0;
//	STK_BOOL			drawRtn = false;
//
//	STK_BOOL ParallelScreenflg = pmi->IsParallelScreen();
//
//	/*
//	 *	获得PMI及引出线数据
//	 */
//	vector<Stk_Leader*>  leaders = pmi->GetLeaders();
//	vector<Stk_ComText*> ComTexts = pmi->GetComText();
//
//	/*
//	 *	判断圆弧引出线绘制的方向
//	 */
//	beginPara.clear();
//	endPara.clear();
//	drawFlg.clear();
//	arcAngle = 0.0;
//	drawRtn = CTessCircle::GetCircleDrawData(leaders, beginPara, endPara, arcAngle, drawFlg);
//
//	/*
//	 *	获取引出线的点列数据
//	 */
//	for(int i=0;i<leaders.size();i++){
//		Stk_Leader * leader = leaders.at(i);		//获得引出线数据
//		vector<Stk_CurveEntity*>polylines = leader->GetAllCurve();
//
//		for (int k=0;k<polylines.size();k++){
//			vector<STK_PNT32> newPnt;
//			vector<STK_PNT32> pnt;
//			vector<STK_PNT32> notepnts;
//			STK_PNT32 tmpPnt1,tmpPnt2;
//			Stk_PolyLine *polyline = (Stk_PolyLine*)polylines.at(k);	//获得引出线的线条数据
//			StkCurveTypeEnum CurveType = polyline->GetCurveType();
//			StkCurveUsageEnum UsageType = polyline->GetUsageType();
//
//			/*leader的类型为引出线*/
//			if (UsageType == CURVE_USAGE_LEADER_LINE)
//			{
//				// 折线时，获取点列
//				if(CurveType==CURVE_TYPE_POLYLINE)
//				{
//					pnt = polyline->GetPoints();
//					notepnts = polyline->GetPoints();
//					//获得离散点数据
//					if(ParallelScreenflg)
//					{
//						pnt.clear();
//						tmpPnt1 = notepnts.at(0);
//						pnt.push_back(tmpPnt1);
//						for(int icount=1;icount<notepnts.size();icount++)
//						{
//							tmpPnt2 =notepnts.at(icount);
//							if((tmpPnt1.X==tmpPnt2.X)&&(tmpPnt1.Y==tmpPnt2.Y)&&(tmpPnt1.Z==tmpPnt2.Z))
//								continue;
//							else
//							{
//								pnt.push_back(tmpPnt2);
//								break;
//							}
//						}
//					}
//
//				}
//				// 圆弧时，离散点列
//				else if(CurveType ==CURVE_TYPE_ELLIPSE){
//					Stk_Ellipse *ellipse = (Stk_Ellipse *)polyline;
//
//					// 圆弧离散计算
//					if ( drawRtn ) {
//						STK_DOUBLE64 BeginPara = beginPara.at(i);
//						STK_DOUBLE64 EndPara   = endPara.at(i);
//						STK_INT32    DrawFlg   = drawFlg.at(i);
//						CTessCircle::GetCircleDiscretization(ellipse, BeginPara, EndPara, 0.0, DrawFlg, pnt);
//					} else {
//						CTessCircle::GetCircleDiscretization(ellipse, 0, pnt);
//					}
//				}
//				// 裁剪引出线 Add by Liutp on 2013.09.02
//				//CPMIUtilityLib::DivideLeader(PMIDIM2, pnt, newPnt);
//			}
//			/*leader的类型为末端符号*/
//			else if (UsageType == CURVE_USAGE_LEADER_TERM){
//				pnt = polyline->GetPoints();						//获得离散点数据
//			}
//			// 调整逻辑 by Liutp on 2013.09.03
//			if (!pnt.empty()) {
//				if ( newPnt.empty() ) {
//					for(int j=0;j<pnt.size();j++){
//						lpnt.push_back(pnt[j]);
//						lIndices.push_back(pntCnt);
//						pntCnt++;
//					}
//				} else {
//					for(int j=0;j<newPnt.size();j++){
//						lpnt.push_back(newPnt[j]);
//						lIndices.push_back(pntCnt);
//						pntCnt++;
//					}
//				}
//				//设置分隔符
//				lIndices.push_back(SO_END_FACE_INDEX);
//			}
//		}
//	}
//
//	return TRUE;
//}
/*
 * 功能：创建PMI延长线

 * 参数：inTopInstance    (I)实例对象指针

 *       outPntNum        (O)输出延长线离散点个数

 *        epnt            (O)延长线离散点

 *       outIndexNum      (O)延长线索引个数

 *        eIndices     	  (O)输出延长线索引		

 * 返回值： 是否执行成功

 * 2013-07-25   xsn   create
 * 2013-08-01   yangxin 修改参数
 * 2013-08-14	yangxin 增加延长线终点的处理
 * 2013-09-10	修改圆弧折线化处理
*/
STK_UINT32 PMICreator::CreateExtensionLines( 
		 Stk_PMI *pmi,					/*(i) 当前实例					*/
		 vector<STK_PNT32> &epnt,		/*(o) 输出延长线离散点			*/
		 vector<STK_UINT32> &eIndices	/*(o) 输出延长线索引		*/
){

	STK_INT32 pntCnt = 0; //点的个数
	STK_DOUBLE64 VecA[3];	//延长线方向向量
	STK_DOUBLE64 length = 0;	
	STK_DOUBLE64 sum = 0;
	memset(VecA, 0, sizeof(STK_DOUBLE64)*3);
	
	//获得PMI及延长线数据
	vector<Stk_Extensionline*>  lines = pmi->GetExLines();

	//对所有延长线进行解析
	for(int i=0;i<lines.size();i++){
		Stk_Extensionline * line = lines.at(i);   //获得一条延长线数据
		StkExtensTypeEnum type = line->GetExLineType();  //获得延长线类型
		vector<STK_PNT32> pnt;
		//如果是折线
		if(type==EXTENS_TYPE_LENGTH){
			Stk_PolyLine *polyline = (Stk_PolyLine *)line->GetCurve();  //获得延长线的线条数据
			pnt = polyline->GetPoints();   //获得离散点数据

			//计算延长线的方向
			VecA[0] = pnt[pnt.size()-1].X-pnt[0].X;
			VecA[1] = pnt[pnt.size()-1].Y-pnt[0].Y;
			VecA[2] = 0.0;

			//计算延长线终点
			sum = VecA[0]*VecA[0] + VecA[1]*VecA[1];
			if (sum >= ZDEPS){
				length = sqrt(sum);		//延长线长度
				VecA[0] /= length*2;
				VecA[1] /= length*2;
			}
			VecA[0] *= length*0.05;
			VecA[1] *= length*0.05;

			pnt[pnt.size()-1].X += (STK_FLOAT32)VecA[0];
			pnt[pnt.size()-1].Y += (STK_FLOAT32)VecA[1];
			pnt[pnt.size()-1].Z = 0.0;

		}
		//如果是圆弧
		else if(type==EXTENS_TYPE_ANGULAR){
			Stk_Ellipse *ellipse = (Stk_Ellipse *)line->GetCurve();

			// 圆弧离散计算
			CTessCircle::GetCircleDiscretization(ellipse, 0, pnt);
		}

		// 将点坐标和索引存入目标数据
		if (!pnt.empty()) {
			for(int j=0;j<pnt.size();j++){
				epnt.push_back(pnt[j]);
				eIndices.push_back(pntCnt);
				pntCnt++;
			}
			//设置分隔符
			eIndices.push_back(SO_END_FACE_INDEX);
		}
	}
	return TRUE;
}

BOOL PMICreator::createExtensionLines( const vector<Stk_Extensionline*>& inExtline,vector<FPOINT3>& vecPolylinePnts)
{

	BOOL bRet = FALSE;
	vector<Stk_Extensionline*>::size_type i, lineCount = inExtline.size();
	if(0 == lineCount)
	{
		return bRet;
	}

	vector<Stk_CurveEntity*> vecStkExtLine;
	Stk_Extensionline  * line = NULL;
	Stk_CurveEntity* pCruve = NULL;
	Stk_PolyLine *polyline  = NULL;
	Stk_Ellipse *ellipse        = NULL;
	STK_BOOL			drawRtn = false;
	StkCurveTypeEnum typeEnum = CURVE_TYPE_UNKNOWN;

	line = inExtline.at(0);
	if(!line) return bRet;

	pCruve = line->GetCurve();
	if(!pCruve) return bRet;

	typeEnum = pCruve->GetCurveType();
	if(typeEnum == CURVE_TYPE_ELLIPSE) return bRet;



	vecPolylinePnts.clear();

	for(i=0;i<lineCount;i++)
	{
		line = inExtline[i];   //获得一条延长线数据
		if(!line)	continue;

		pCruve = line->GetCurve();
		if(!pCruve) 	continue;
		typeEnum = pCruve->GetCurveType();

		if(CURVE_TYPE_POLYLINE ==  typeEnum)
		{
				polyline = (Stk_PolyLine *)pCruve;  //获得特殊线的线条数据
				if(!polyline) continue;
				const vector<STK_PNT32>& plPnts = polyline->GetPoints();
				vecPolylinePnts.push_back(plPnts);
		}
		else if(CURVE_TYPE_LINE== typeEnum)
		{
				FPOINT3 linePnts;
				Stk_Line *stkline = (Stk_Line *)pCruve;  //获得特殊线的线条数据
				linePnts.push_back(stkline->GetStartPoint());   //获得始点坐标数据
				linePnts.push_back(stkline->GetEndPoint());	//获得终点坐标数据
				vecPolylinePnts.push_back(linePnts);
		}
	}


	return TRUE;
}

STK_BOOL PMICreator::createCompositeText(Stk_ComText* pStkCompText,Stk_PMI*pSymbol)
{
	STK_BOOL bRet  = FALSE;
	if(!pStkCompText) 	return bRet;
	if(!pSymbol)			 	return bRet;
	return bRet;
}


/*
 * 功能：创建PMI外框

 * 参数：inTopInstance    (I)实例对象指针

 *       outPntNum        (O)输出外框离散点个数

 *        fpnt            (O)外框离散点

 *       outIndexNum      (O)外框索引个数

 *        fIndices     	  (O)输出外框索引		

 * 返回值： 是否执行成功

 * 2013-07-25   xsn   create
 * 2013-08-07	yangxin	增加处理内容
 * 2013-09-10	修改圆弧折线化处理
*/
STK_UINT32 PMICreator::CreateOutFrame( 
		 Stk_PMI *pmi,					/*(i) 当前实例					*/
		 vector<STK_PNT32> &fpnt,		/*(o) 输出外框离散点			*/
		 vector<STK_UINT32> &fIndices	/*(o) 输出外框索引				*/
){
	STK_INT32 pntCnt = 0;   //点的个数

	//获得PMI及外框数据
	vector<Stk_OutFrame*>  outframes = pmi->GetOutFrame();

	//对所有延长线进行解析
	for(int i=0;i<outframes.size();i++){
		Stk_OutFrame * outframe = outframes.at(i);   //获得一条延长线数据

		vector<Stk_CurveEntity*> curves = outframe->GetFrameData();  //获得延长线的线条数据
		STK_INT32 outframetype = outframe->GetFrameType();	//获取外框类型

		for (int j=0;j<curves.size();j++){
			Stk_CurveEntity *curve = curves.at(j);
			STK_INT32 type = curve->GetCurveType();
			vector<STK_PNT32> pnt;

			//如果是折线
			if (type == 1){
				Stk_PolyLine *Polyline = (Stk_PolyLine *)curve;
				pnt = Polyline->GetPoints();   //获得离散点数据
			}
			//如果是圆弧
			else if (type == 2){
				Stk_Ellipse *ellipse = (Stk_Ellipse *)curve;

				// 圆弧离散计算
				CTessCircle::GetCircleDiscretization(ellipse, 0, pnt);
			}

			//将点坐标和索引存入目标数据
			if (!pnt.empty()) {
				for(int j=0;j<pnt.size();j++){
					fpnt.push_back(pnt[j]);
					fIndices.push_back(pntCnt);
					pntCnt++;
				}
				//设置分隔符
				fIndices.push_back(SO_END_FACE_INDEX);
			}
		}
	}
	return TRUE;
}
/*
 * 功能：创建PMI包围框

 * 参数：inTopInstance    (I)实例对象指针

 *       outPntNum        (O)输出包围框离散点个数

 *        apnt            (O)包围框离散点

 *       outIndexNum      (O)包围框索引个数

 *       aIndices     	  (O)输出包围框索引		

 * 返回值： 是否执行成功

 * 2013-07-25   xsn   create
 * 2013-08-01   yangxin 修改参数
*/
STK_UINT32 PMICreator::CreateAnchorPoints( 
		 Stk_PMI *pmi,					/*(i) 当前实例					*/
		 vector<STK_PNT32> &apnt,		/*(o) 输出包围框离散点			*/
		 vector<STK_UINT32> &aIndices	/*(o) 输出包围框索引			*/
){
	return TRUE;
}
/*
 * 功能：创建末端符号

 * 参数：inTopInstance    (I)实例对象指针

 *       outPntNum        (O)输出末端符号离散点个数

 *       spnt            (O)末端符号离散点

 *       outIndexNum      (O)末端符号索引个数

 *       sIndices     	  (O)输出末端符号索引		

 * 返回值： 是否执行成功

 * 2013-07-25   xsn   create
 * 2013-08-01   yangxin 修改参数
*/
STK_UINT32 PMICreator::CreateEndSymbol( 
		 Stk_PMI *pmi,					/*(i) 当前实例					*/
		 //double  normalVec[3],
		 vector<STK_PNT32> &spnt,		/*(o) 输出末端符号离散点		*/
		 vector<STK_UINT32> &sIndices,	/*(o) 输出末端符号索引			*/
		 StkTermTypeEnum	&oTermType	/*(o) 末端符号类型				*/
){
	STK_INT32			pntCnt = 0;		//点的个数
	CPMIESymbol			pmiSym;			//创建末端符号的类
	STK_DOUBLE64		pnt1[3];		//末端符号起始点坐标
	STK_DOUBLE64		refvec[3];		//末端符号方向向量
	CPOINT3Array		outPnts,outPnts2;	//末端符号离散点
	//STK_UINT32			oTermType;		//末端符号类型
	STK_FLOAT32			oSize[2];		//末端符号宽和高
	STK_PNT32			oLoc;			//末端符号起始位置
	STK_DIR32			oDir;			//末端符号方向
	STK_PNT32			tmp;			//临时变量
	vector<STK_INT32>	drawDir;		// 圆弧绘制方向
	STK_BOOL			drawRtn=FALSE;



	StkPMIModeEnum  Mode = pmi->GetMode();
	if (Mode == PMI_MODE_POLY){
		return TRUE;
	}
	//获得PMI数据及其引出线数据
	vector<Stk_Leader*>  leaders = pmi->GetLeaders();

	// 纠正末端符号绘制方向 by Liutp 2013.9.11
	STK_UINT32 type = pmi->GetType();
	if ( type != 906 ) {			// 屏蔽多引线几何公差的处理
		drawRtn = CPMIUtilityLib::GetEndSymbolDirection(leaders, drawDir);
	}

	//对每一条引出线进行解析
	for(int i=0;i<leaders.size();i++){
		Stk_Leader * leader = leaders.at(i);  //获得引出线数据
		vector<Stk_CurveEntity*>polylines = leader->GetAllCurve();

		for (int k=0;k<polylines.size();k++){
			outPnts.clear();
			outPnts2.clear();
			Stk_PolyLine* polyline = (Stk_PolyLine*)polylines.at(k);
			StkCurveTypeEnum CurveType = polyline->GetCurveType();
			StkCurveUsageEnum UsageType = polyline->GetUsageType();

			/* 折线引出线时计算末端符号 */
			if (UsageType == CURVE_USAGE_LEADER_LINE){
				leader->GetTerminator(oTermType,oSize,oLoc,oDir);//获得末端符号相关数据

		// 异常点特殊处理		by Liutp on 2013.09.10
		if ( oDir.X != oDir.X || oDir.Y != oDir.Y ) {
			continue;
		}

				// 末端符号方向重新计算 by Liutp 2013.9.11
		if ((CurveType==CURVE_TYPE_POLYLINE) && drawRtn){
					oDir.X = oDir.X*drawDir.at(i);
					oDir.Y = oDir.Y*drawDir.at(i);
					oDir.Z = oDir.Z*drawDir.at(i);
				}

		//获得末端符号起始点坐标
		pnt1[0] = oLoc.X;
		pnt1[1] = oLoc.Y;
		pnt1[2] = oLoc.Z;
		//获得末端符号方向向量
		refvec[0] = oDir.X;
		refvec[1] = oDir.Y;
		refvec[2] = oDir.Z;

		//bing added for new interface of symbol--------------------------
		double		normalVec[3];		//末端符号法线向量？bing added
		STK_DOUBLE64		symvec[3];//temp

		//单位化
		STK_DOUBLE64 refsize = sqrt(((refvec[0])*(refvec[0]))+((refvec[1])*(refvec[1]))+((refvec[2])*(refvec[2])));

		symvec[0] = refvec[0]/refsize;
		symvec[1] = refvec[1]/refsize;
		symvec[2] = refvec[2]/refsize;
		normalVec[0] = (-1)*symvec[1];
		normalVec[1] = symvec[0];
		normalVec[2] = 0;
		//-----------------------------------------end ----------------------

		pmiSym.SetESymbolInfo(oSize[0],oSize[1],pnt1,refvec,normalVec,oTermType,outPnts,outPnts2);
		//对第一部分末端符号离散点进行解析
		//if(outPnts.GetSize()>0){
		//	spnt.reserve(outPnts.GetSize());
		//	sIndices.reserve(outPnts.GetSize()+1);
		//}
		//将点坐标和索引存入目标数据
		STK_BOOL chkflg = FALSE;
		if (oTermType == TERM_PATSMDOUBLEFILLARROW)
		{
			for(int j=0;j<outPnts.size();j++)
			{
				tmp.X = (STK_FLOAT32)outPnts[j].x;
				tmp.Y = (STK_FLOAT32)outPnts[j].y;
				tmp.Z = (STK_FLOAT32)outPnts[j].z;

				// 异常点特殊处理		by Liutp on 2013.09.10
				if ( tmp.X != tmp.X || tmp.Y != tmp.Y ) 
				{
					chkflg = TRUE;
					break;
				}

				spnt.push_back(tmp);
				sIndices.push_back(pntCnt);
				pntCnt++;
				if (j == 2 || j == 6)
				{
					sIndices.push_back(SO_END_FACE_INDEX);
					//pntCnt++;
				}
			}
		}
		else if (oTermType == TERM_PATSMDOUBLETRIANGLE)
		{
			int j=0;
			for(j=0;j<outPnts.size();j++)
			{
				tmp.X = (STK_FLOAT32)outPnts[j].x;
				tmp.Y = (STK_FLOAT32)outPnts[j].y;
				tmp.Z = (STK_FLOAT32)outPnts[j].z;

				// 异常点特殊处理		by Liutp on 2013.09.10
				if ( tmp.X != tmp.X || tmp.Y != tmp.Y ) 
				{
					chkflg = TRUE;
					break;
				}

				spnt.push_back(tmp);
				sIndices.push_back(pntCnt);
				pntCnt++;
				if (j == 2 || j == 6)
				{
					sIndices.push_back(SO_END_FACE_INDEX);
					//pntCnt++;
				}
			}
			

		}
		else
		{
			for(int j=0;j<outPnts.size();j++)
			{
				tmp.X = (STK_FLOAT32)outPnts[j].x;
				tmp.Y = (STK_FLOAT32)outPnts[j].y;
				tmp.Z = (STK_FLOAT32)outPnts[j].z;

				// 异常点特殊处理		by Liutp on 2013.09.10
				if ( tmp.X != tmp.X || tmp.Y != tmp.Y ) 
				{
					chkflg = TRUE;
					break;
				}

				spnt.push_back(tmp);
				sIndices.push_back(pntCnt);
				pntCnt++;
			}
			if ( chkflg ) continue;
			//设置分隔符
			if(outPnts.size()>0)
			{
				sIndices.push_back(SO_END_FACE_INDEX);
			}
		}
		//对于第二部分末端符号离散点进行解析（部分特殊符号使用）
		//if(outPnts2.GetSize()>0){
		//	spnt.reserve(outPnts2.GetSize());
		//	sIndices.reserve(outPnts2.GetSize()+1);
		//}
		//将点坐标和索引存入目标数据
		for(int j=0;j<outPnts2.size();j++)
		{
			tmp.X = (STK_FLOAT32)outPnts2[j].x;
			tmp.Y = (STK_FLOAT32)outPnts2[j].y;
			tmp.Z = (STK_FLOAT32)outPnts2[j].z;

			if ( tmp.X != tmp.X || tmp.Y != tmp.Y ) 
			{
				chkflg = TRUE;
				break;
			}

			spnt.push_back(tmp);
			sIndices.push_back(pntCnt);
			pntCnt++;
		}

		if ( chkflg ) continue;

				//设置分隔符
				if(outPnts2.size()>0)
				{
					sIndices.push_back(SO_END_FACE_INDEX);
				}
			}
		}
	}
	return TRUE;
}

BOOL  PMICreator::CreateEndSymbol(
			Stk_PMI *pmi,					/*(i) 当前实例					*/
			vector<FPOINT3>& vecPolylinePnts,
			StkTermTypeEnum		&oTermType	/*(o) 末端符号类型				*/
			)
{
	BOOL bRet = FALSE;
	CPMIESymbol			pmiSym;			//创建末端符号的类
	STK_DOUBLE64		pnt1[3]  = {};		//末端符号起始点坐标
	STK_DOUBLE64		refvec[3] = {};		//末端符号方向向量
	CPOINT3Array		outPnts,outPnts2;	//末端符号离散点
	STK_FLOAT32			oSize[2] = {};		//末端符号宽和高
	STK_PNT32			oLoc;			//末端符号起始位置
	STK_DIR32			oDir;			//末端符号方向

	STK_BOOL			endDrawRtn = false;

	if (!pmi) return bRet;
	vector<Stk_Leader*> pLeaders = pmi->GetLeaders();

	vector<STK_INT32>	endDrawDir;		// 圆弧绘制方向
	Stk_Leader* pStkLeader =NULL;

	Stk_CurveEntity *pCruve = NULL;
	StkCurveTypeEnum crvType = CURVE_TYPE_UNKNOWN;

	int i = 0,n = 0,nLead = pLeaders.size();

	if(nLead<=0) return FALSE;
	outPnts.clear();
	outPnts2.clear();
	endDrawDir.clear();
	vecPolylinePnts.clear();

	pStkLeader = pLeaders.at(0);
	if(!pStkLeader)	return bRet;
	pCruve = pStkLeader->GetCurve();
	if(!pCruve)	return bRet;
	crvType = pCruve->GetCurveType();
	endDrawRtn = CPMIUtilityLib::GetEndSymbolDirection(pLeaders,endDrawDir);
	for(n = 0;n < nLead;n++)
	{
		outPnts.clear();
		outPnts2.clear();

		pStkLeader = pLeaders.at(n);
		if(!pStkLeader) return bRet;
		pStkLeader->GetTerminator(oTermType,oSize,oLoc,oDir);//获得末端符号相关数据

		// 异常点特殊处理		by Liutp on 2013.09.10
		if ( oDir.X != oDir.X || oDir.Y != oDir.Y ) return FALSE;


		//调整末端符号
		//CPMIUtilityLib::UpdateEndSymData(pDimension,pStkLeader,oTermType,oSize,oLoc,oDir);

		// 末端符号方向重新计算 by Liutp 2013.9.11
		if ((crvType==CURVE_TYPE_POLYLINE) && endDrawRtn)
		{
				oDir.X = oDir.X*endDrawDir.at(n);
				oDir.Y = oDir.Y*endDrawDir.at(n);
				oDir.Z = oDir.Z*endDrawDir.at(n);
		}

		//获得末端符号起始点坐标
		pnt1[0] = oLoc.X;
		pnt1[1] = oLoc.Y;
		pnt1[2] = oLoc.Z;
		//获得末端符号方向向量
		refvec[0] = oDir.X;
		refvec[1] = oDir.Y;
		refvec[2] = oDir.Z;

		//bing added for new interface of symbol--------------------------
		double normalVec[3]; //末端符号法线向量？bing added
		double symvec[3]; //temp

		//单位化
		double refsize = sqrt(
				((refvec[0]) * (refvec[0])) + ((refvec[1]) * (refvec[1]))
						+ ((refvec[2]) * (refvec[2])));

		symvec[0] = refvec[0] / refsize;
		symvec[1] = refvec[1] / refsize;
		symvec[2] = refvec[2] / refsize;
		normalVec[0] = (-1) * symvec[1];
		normalVec[1] = symvec[0];
		normalVec[2] = 0;
		//-----------------------------------------end ----------------------
		pmiSym.SetESymbolInfo(oSize[0],oSize[1],pnt1,refvec,normalVec,oTermType,outPnts,outPnts2);

		FPOINT3 vecpnt;
		STK_PNT32 pnt;
		int nsize = outPnts.size();
		for( i =0; i<nsize; i++)
		{

			pnt.X = outPnts[i].x;
			pnt.Y = outPnts[i].y;
			pnt.Z = outPnts[i].z;
			vecpnt.push_back(pnt);
		}

		if(nsize>0) vecPolylinePnts.push_back(vecpnt);
		vecpnt.clear();
		nsize = outPnts2.size();

		for( i =0; i<nsize; i++)
		{

				pnt.X = outPnts2[i].x;
				pnt.Y = outPnts2[i].y;
				pnt.Z = outPnts2[i].z;
				vecpnt.push_back(pnt);
		}
		if(nsize>0)	vecPolylinePnts.push_back(vecpnt);
	}

	return TRUE;

}


// 设置包围盒 by YANGXI
void PMICreator::SetTextBox(
	STK_BOX32 inbox					/*(i)设置文字包围盒的坐标点		*/
){
	m_TextBox.push_back(inbox);
}

// 清除包围盒数据 by YANGXI
void PMICreator::ClearTextBox(
){
	m_TextBox.clear();
}

// 获取包围盒数据 by Liutp
vector<STK_BOX32> PMICreator::GetTextBox(
){
	return m_TextBox;
}

// 重新计算包围盒数据 by Liutp
STK_BOX32 PMICreator::ResetTextBox( STK_BOX32 inbox )
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
/*
* 功能：创建PMI特殊线

 * 参数：inTopInstance    (I)实例对象指针

 *        epnt            (O)特殊线离散点

 *        eIndices     	  (O)输出特殊线索引		

 * 返回值： 是否执行成功

 * 2013-09-03	yangxin create
 * 2013-09-10	修改圆弧折线化处理 by Liutp
 * 2013-09-23	修改特殊线的新方法 by Liutp
*/
STK_UINT32 PMICreator::CreateSpeciallLines( 
		 Stk_PMI *pmi,					/*(i) 当前实例					*/
		 vector<STK_PNT32> &epnt,		/*(o) 输出特殊线离散点			*/
		 vector<STK_UINT32> &eIndices	/*(o) 输出特殊线索引			*/
){

	STK_INT32 pntCnt = 0; //点的个数
	
	//获得PMI特殊线数据
	vector<Stk_SpecialLine*>  lines = pmi->GetSpecialLines();

	//对所有延长线进行解析
	for(int i=0;i<lines.size();i++){
		Stk_SpecialLine * line = lines.at(i);   //获得一条特殊线数据
		vector<Stk_CurveEntity*> vecSpecialLine = line->GetCurves();
		Stk_CurveEntity* SpecialLine;
		for ( int j=0; j<vecSpecialLine.size(); j++ ) {
			SpecialLine = vecSpecialLine.at(j);
			StkCurveTypeEnum type = SpecialLine->GetCurveType();	//获得特殊线类型
			vector<STK_PNT32> pnt;
			//如果是折线
			if( type == CURVE_TYPE_POLYLINE ) {
				Stk_PolyLine *polyline = (Stk_PolyLine *)SpecialLine;  //获得特殊线的线条数据
				pnt = polyline->GetPoints();   //获得离散点数据
			}
			//如果是圆弧
			else if ( type == CURVE_TYPE_ELLIPSE ) {
				Stk_Ellipse *ellipse = (Stk_Ellipse *)SpecialLine;  //获得特殊线的线条数据

				// 圆弧离散计算
				CTessCircle::GetCircleDiscretization(ellipse, 0, pnt);
			}
			else if ( type == CURVE_TYPE_LINE ){
				Stk_Line *stkline = (Stk_Line *)SpecialLine;  //获得特殊线的线条数据
				STK_PNT32 startpnt = stkline->GetStartPoint();   //获得始点坐标数据
				STK_PNT32 endpnt = stkline->GetEndPoint();	//获得终点坐标数据

				//将点坐标和索引存入目标数据
				pnt.push_back(startpnt);
				pnt.push_back(endpnt);
			}
			//将点坐标和索引存入目标数据
			if (!pnt.empty()) {
				for(int j=0;j<pnt.size();j++){
					epnt.push_back(pnt[j]);
					eIndices.push_back(pntCnt);
					pntCnt++;
				}
				//设置分隔符
				eIndices.push_back(SO_END_FACE_INDEX);
			}
		}
	}
	return TRUE;
}
BOOL PMICreator::createLeader(Stk_Leader* pStkLeader,vector<FPOINT3> vecPolylinePnts,Stk_PMI*pSymbol)
{
	STK_INT32			pntCnt = 0;		//点的个数
	CPMIESymbol			pmiSym;			//创建末端符号的类
	STK_DOUBLE64		pnt1[3] = {};		//末端符号起始点坐标
	STK_DOUBLE64		refvec[3] = {};		//末端符号方向向量

	STK_PNT32			tmp;			//临时变量
	vector<STK_INT32>	drawDir;		// 圆弧绘制方向
	STK_BOOL			drawRtn=FALSE;

	//用来矫正509平行于屏幕的注释的引线问题。
	STK_BOOL bParallel = false;

	vector<STK_UINT32>		vecIndices;


	StkPMISourceEnum EnumSource = PMISourceUnknown;
	StkPMITypeEnum   EnumType   = PMI_TYPE_UNKNOWN;
	if(pSymbol)
	{
		bParallel = pSymbol->IsParallelScreen();
		EnumSource =  pSymbol->GetPMISource();
		EnumType  =  pSymbol->GetType();
	}


	int i = 0,   j = 0, pntNum=0;
	bool	exceptPnt = false;
	bool    bFlag = true;

	//对第一部分末端符号离散点进行解析
	vector<Stk_CurveEntity*>polylines = pStkLeader->GetAllCurve();
	Stk_CurveEntity* stkPolyline = NULL;
	if(polylines.size() > 0)//Datakit中leader只可能有条引出线
		stkPolyline = polylines[0];

	if(stkPolyline)
	{
		if(CURVE_TYPE_POLYLINE ==  stkPolyline->GetCurveType())
		{
			//用来矫正509平行于屏幕的注释的引线问题。

			Stk_PolyLine *polyline = (Stk_PolyLine *)stkPolyline;  //获得特殊线的线条数据

			vector<STK_PNT32> plPnts = polyline->GetPoints();
			if(bParallel
				&&PMI_SOURCE_CONVERT == EnumSource
				&&PMI_TYPE_TypeNote == EnumType)
			{

				while(plPnts.size()>2)
					plPnts.pop_back();



				if(pSymbol)
				{
					STK_PNT32 minInnerLoc={0, 0, 0};
					Stk_Text* pStkText = NULL;
					STK_PNT32 innerLoc;

					Stk_ComText *pComText = NULL;
					vector<Stk_ComText*> vecComtxt = pSymbol->GetComText();
					int ncomtxt = vecComtxt.size();
					if(ncomtxt>0)
					{
						pComText = vecComtxt.at(0);
						vector<Stk_Text*>vecText = pComText->GetTexts();
						int ntxt = vecText.size();

						if(ntxt>0)
						{
							minInnerLoc = vecText[0]->GetInnerLoc();
							for( i=0; i<ntxt; i++)
							{
								pStkText = vecText[i];
								if(!pStkText)
									continue;

								innerLoc = pStkText->GetInnerLoc();
								if(minInnerLoc.X > innerLoc.X)
									minInnerLoc.X = innerLoc.X;

								if(minInnerLoc.Y > innerLoc.Y)
									minInnerLoc.Y = innerLoc.Y;
							}
							plPnts.pop_back();
							plPnts.push_back(minInnerLoc);
						}
					}
				}
			}

			vecPolylinePnts.push_back(plPnts);   //获得离散点数据
			//clipLeader(m_pLeaderKit->clipBox.getValue(), vecPolylinePnts[vecPolylinePnts.size()-1]);
			pntNum += plPnts.size();
		}
		else if(CURVE_TYPE_ELLIPSE == stkPolyline->GetCurveType())
		{
			FPOINT3 elpPnts;
			Stk_Ellipse *ellipse = (Stk_Ellipse *)stkPolyline;  //获得特殊线的线条数据
			float UMin = 0.f, UMax = 0.0f, arcAngle = 0.0f, arcMinor = 0.0f;
			int drawFlg = 0;
			//clipLeader(m_pLeaderKit->clipBox.getValue(), ellipse, UMin, UMax, arcAngle, drawFlg);
			ellipse->GetDomain(UMin,UMax);
			STK_DIR32 normalDir = ellipse->GetNormal();
			drawFlg = (normalDir.Z>0)?1:-1;
			CTessCircle::GetCircleDiscretization(ellipse, UMin, UMax,  arcAngle, drawFlg, elpPnts);

			vecPolylinePnts.push_back(elpPnts);
			pntNum += elpPnts.size();
		}

		else if(CURVE_TYPE_LINE== stkPolyline->GetCurveType())
		{
			FPOINT3 linePnts;
			Stk_Line *stkline = (Stk_Line *)stkPolyline;  //获得特殊线的线条数据
			linePnts.push_back(stkline->GetStartPoint());   //获得始点坐标数据
			linePnts.push_back(stkline->GetEndPoint());	//获得终点坐标数据
			vecPolylinePnts.push_back(linePnts);
			pntNum += 2;
		}
	}

	return TRUE;
}
//
BOOL PMICreator::createLeader( const vector<Stk_Leader*>& pLeaders,const STK_BOX32& textBox,vector<FPOINT3>& vecPolylinePnts,Stk_PMI* pDimension)
{
	STK_INT32			pntCnt = 0;		//点的个数
	STK_DOUBLE64		refvec[3];		//末端符号方向向量
	STK_PNT32			tmp;			//临时变量
	vector<STK_INT32>	drawDir;		// 圆弧绘制方向
	vector<STK_BOX32>   vecTextBox;     // 文本的包围盒数组

	int i = 0,  j = 0, n = 0,nLead = 0, pntNum=0, numVertice = 0, numLines = 0 ;
	bool	exceptPnt = false;


	Stk_Leader* pStkLeader =NULL;

	vector<STK_INT32>	vecDrawFlg;
	vector<STK_DOUBLE64>	vecBeginPara, vecEndPara;
	STK_DOUBLE64		arcAngle = 0.0;
	STK_BOOL			drawRtn = false;
	STK_BOOL			endDrawRtn = false;

	vector<STK_INT32>	endDrawDir;		// 圆弧绘制方向

	STK_DOUBLE64 fBeginPara = 0.0;
	STK_DOUBLE64 fEndPara   = 0.0;
	STK_INT32    nDrawFlg   = 0;

	Stk_CurveEntity *pCruve = NULL;
	StkCurveTypeEnum crvType = CURVE_TYPE_UNKNOWN;

	double fRealAngle = -1.0;
	STK_PNT32 boxCenterPnt ;

	int idx = -1;

	StkPMISourceEnum EnumSource = pDimension->GetPMISource();
	StkPMITypeEnum   EnumType   = pDimension->GetType();

	vector<STK_PNT32>vecNewPnt;

	vecBeginPara.clear();
	vecEndPara.clear();
	vecDrawFlg.clear();
	vecNewPnt.clear();
	endDrawDir.clear();
	vecTextBox.clear();

	vecTextBox.push_back(textBox);

	if(EnumType==PMI_TYPE_TypeNote)
		vecTextBox.clear();


	nLead = pLeaders.size();
	if(nLead<=0)
		return FALSE;

	pntNum = 0;
	numVertice = 0;
	numLines = 0;


	pStkLeader = pLeaders.at(0);
	pCruve = pStkLeader->GetCurve();

	crvType = pCruve->GetCurveType();
	if(crvType == CURVE_TYPE_ELLIPSE)
	{
		drawRtn = CTessCircle::GetCircleDrawData(pLeaders,vecBeginPara,vecEndPara,arcAngle,vecDrawFlg);

	}

	endDrawRtn = CPMIUtilityLib::GetEndSymbolDirection(pLeaders,endDrawDir);

	boxCenterPnt.X =( textBox.BoundBox[0][0] + textBox.BoundBox[1][0])*0.5;
	boxCenterPnt.Y =( textBox.BoundBox[0][1] + textBox.BoundBox[1][1])*0.5;
	boxCenterPnt.Z =( textBox.BoundBox[0][2] + textBox.BoundBox[1][2])*0.5;

	vector<STK_PNT32> outVecExtPnt;
	vecPolylinePnts.clear();
	for(n = 0;n < nLead;n++)
	{
		vector<STK_PNT32>		tmpVeclpnt;		/*(o) 修剪引出线时用到			*/
		vector<STK_UINT32>		tmpVeclIndices;	/*(o) 修剪引出线时用到			*/
		STK_INT32			    tmpPntCnt = 0;	/*(o) 修剪引出线时用到			*/



		vecNewPnt.clear();

		pStkLeader = pLeaders.at(n);

		//对第一部分末端符号离散点进行解析
		vector<Stk_CurveEntity*>polylines = pStkLeader->GetAllCurve();
		Stk_CurveEntity* stkPolyline = NULL;
		if(polylines.size() > 0)//Datakit中leader只可能有条引出线
			stkPolyline = polylines[0];

		if(stkPolyline)
		{
			if(CURVE_TYPE_POLYLINE ==  stkPolyline->GetCurveType())
			{
				Stk_PolyLine *polyline = (Stk_PolyLine *)stkPolyline;  //获得特殊线的线条数据
				const vector<STK_PNT32>& plPnts = polyline->GetPoints();

				if(EnumSource == PMI_SOURCE_CREATE&&
				  (EnumType == PMI_TYPE_RadiusDimension|| EnumType == PMI_TYPE_DiameterDimension))
				{
					if(plPnts.size()==2) CPMIUtilityLib::DivideLeader(2,vecTextBox,plPnts,vecNewPnt);
				}


				else CPMIUtilityLib::DivideLeader(2,vecTextBox,plPnts,vecNewPnt);

				if (!plPnts.empty())
				{
					if ( vecNewPnt.empty() )
					{
						for(int j=0;j<plPnts.size();j++)
						{
							tmpVeclpnt.push_back(plPnts[j]);
							tmpVeclIndices.push_back(tmpPntCnt);
							tmpPntCnt++;
						}
					}
					else
					{
						for(int j=0;j<vecNewPnt.size();j++)
						{
							tmpVeclpnt.push_back(vecNewPnt[j]);
							tmpVeclIndices.push_back(tmpPntCnt);
							tmpPntCnt++;
						}
					}
					//设置分隔符
				}
				vecPolylinePnts.push_back(tmpVeclpnt);
				pntNum += tmpPntCnt;
			}
			else if(CURVE_TYPE_ELLIPSE == stkPolyline->GetCurveType())
			{
				FPOINT3 elpPnts;
				Stk_Ellipse *ellipse = (Stk_Ellipse *)stkPolyline;  //获得特殊线的线条数据
				float UMin = 0.f, UMax = 0.0f, arcAngle = 0.0f, arcMinor = 0.0f;
				int drawFlg = 0;
				//clipLeader(m_pLeaderKit->clipBox.getValue(), ellipse, UMin, UMax, arcAngle, drawFlg);
				/*ellipse->GetDomain(UMin,UMax);
				STK_DIR32 normalDir = ellipse->GetNormal();
				drawFlg = (normalDir.Z>0)?1:-1;*/
				if(drawRtn)
				{
					fBeginPara = vecBeginPara.at(n);
					fEndPara   = vecEndPara.at(n);
					nDrawFlg   = vecDrawFlg.at(n);
					//CTessCircle::GetCircleDiscretization(ellipse, UMin, UMax,  arcAngle, drawFlg, elpPnts);

					CTessCircle::GetCircleDiscretization(ellipse,fBeginPara,fEndPara,0.0,nDrawFlg,elpPnts);
				}
				else
					CTessCircle::GetCircleDiscretization(ellipse,0,elpPnts);
				CPMIUtilityLib::DivideLeader(2,vecTextBox,elpPnts,vecNewPnt/*,bConverted*/);

				if (!elpPnts.empty()) {
					if ( vecNewPnt.empty() ) {
						for(int j=0;j<elpPnts.size();j++){
							tmpVeclpnt.push_back(elpPnts[j]);
							tmpVeclIndices.push_back(tmpPntCnt);
							tmpPntCnt++;
						}
					} else {
						for(int j=0;j<vecNewPnt.size();j++){
							tmpVeclpnt.push_back(vecNewPnt[j]);
							tmpVeclIndices.push_back(tmpPntCnt);
							tmpPntCnt++;
						}
					}
					//设置分隔符

				}



				/*vecPolylinePnts.push_back(elpPnts);
				pntNum += elpPnts.size();*/
				vecPolylinePnts.push_back(tmpVeclpnt);
				pntNum += tmpPntCnt;

				if(i==0&&outVecExtPnt.size()>0)
				{
					vecNewPnt.clear();
					CPMIUtilityLib::DivideLeader(2,vecTextBox,outVecExtPnt,vecNewPnt/*,bConverted*/);
					if(vecNewPnt.empty())
					{
						vecPolylinePnts.push_back(outVecExtPnt);
						pntNum+=outVecExtPnt.size();
					}
					else
					{
						vecPolylinePnts.push_back(vecNewPnt);
						pntNum+=vecNewPnt.size();
					}
				}
			}

			else if(CURVE_TYPE_LINE== stkPolyline->GetCurveType())
			{
				FPOINT3 linePnts;
				Stk_Line *stkline = (Stk_Line *)stkPolyline;  //获得特殊线的线条数据
				linePnts.push_back(stkline->GetStartPoint());   //获得始点坐标数据
				linePnts.push_back(stkline->GetEndPoint());	//获得终点坐标数据
				vecPolylinePnts.push_back(linePnts);
				pntNum += 2;
			}
		}
	}
	BOOL bRet = TRUE;
	return bRet;
}

