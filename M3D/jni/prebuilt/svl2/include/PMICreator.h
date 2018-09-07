// PMICreator.h :PMI生成器
//

#include "Stk_Instance.h"
#include "Stk_PMIEntity.h"
#include "Stk_CurveEntity.h"
#include "Stk_CompositeText.h"

class PMICreator{

public:
	PMICreator();
	~PMICreator();

private:
	vector<STK_BOX32> m_TextBox;		// 文本框包围盒

public:
	//创建引出线
	STK_UINT32 CreateLeaders( 
		Stk_PMI *pmi,					/*(i) 当前实例					*/
		vector<STK_PNT32> &lpnt,		/*(o) 输出引出线离散点			*/
		vector<STK_UINT32> &lIndices	/*(o) 输出引出线索引			*/
		);

	//创建延长线
	STK_UINT32 CreateExtensionLines(
		Stk_PMI *pmi,					/*(i) 当前实例					*/
		vector<STK_PNT32> &epnt,		/*(o) 输出延长线离散点			*/
		vector<STK_UINT32> &eIndices	/*(o) 输出延长线索引			*/
		);
	//创建外框
	STK_UINT32 CreateOutFrame(
		Stk_PMI *pmi,					/*(i) 当前实例					*/
		vector<STK_PNT32> &fpnt,		/*(o) 输出外框离散点			*/
		vector<STK_UINT32> &fIndices	/*(o) 输出外框索引				*/
		);
	//创建包围框
	STK_UINT32 CreateAnchorPoints(
		Stk_PMI *pmi,					/*(i) 当前实例					*/
		vector<STK_PNT32> &apnt,		/*(o) 输出包围框离散点			*/
		vector<STK_UINT32> &aIndices	/*(o) 输出包围框索引			*/
		);
	//创建末端符号
	STK_UINT32 CreateEndSymbol(
		Stk_PMI *pmi,					/*(i) 当前实例					*/
		vector<STK_PNT32> &spnt,		/*(o) 输出末端符号离散点		*/
		vector<STK_UINT32> &sIndices,	/*(o) 输出末端符号索引			*/
		StkTermTypeEnum		&oTermType	/*(o) 末端符号类型				*/
		);
	//创建PMI特殊线
	STK_UINT32 CreateSpeciallLines(
		Stk_PMI *pmi,					/*(i) 当前实例					*/
		vector<STK_PNT32> &epnt,		/*(o) 输出特殊线离散点			*/
		vector<STK_UINT32> &eIndices	/*(o) 输出特殊线索引			*/
		);
/** 圆弧折线化相关接口_STRAT		by Liu TongPeng **/
	// 获得圆弧离散点
	STK_BOOL GetCircleDiscretization(
		Stk_Ellipse			*ellipse,	/*(i) 圆弧线					*/
		STK_INT32			drawFlg,	/*(i) 圆弧绘制方向				*/
										/*  ==  1 逆时针				*/
										/*  == -1 顺时针				*/
		vector<STK_PNT32>	&outPnt		/*(o) 分元后的点列数据			*/
		);
	// 获得圆弧离散点_效率优化版
	STK_BOOL GetCircleDiscretization(
		Stk_Ellipse			*ellipse,	/*(i) 圆弧线					*/
		STK_DOUBLE64		beginPara,	/*(i) 圆弧绘制的起始角度参数	*/
		STK_DOUBLE64		endPara,	/*(i) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64		arcAngle,	/*(i) 圆弧实际角度				*/
		STK_INT32			drawFlg,	/*(i) 圆弧绘制方向				*/
										/*  ==  1 逆时针				*/
										/*  == -1 顺时针				*/
		vector<STK_PNT32>	&outPnt		/*(o) 分元后的点列数据			*/
		);
	// 获得圆弧绘制方向
	STK_BOOL GetCircleDrawData(
		vector<Stk_Leader*>		leaders,	/*(i) PMI引出线数据				*/
		vector<STK_DOUBLE64>	&startPara,	/*(o) 圆弧绘制的起始角度参数	*/
		vector<STK_DOUBLE64>	&endPara,	/*(o) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64			&drawAngle,	/*(o) 圆弧绘制角度				*/
		vector<STK_INT32>		&drawFlg	/*(o) 圆弧绘制方向				*/
											/*  ==  1 逆时针				*/
											/*  == -1 顺时针				*/
		);
	// 获得末端符号绘制方向
	STK_BOOL GetEndSymbolDirection(
		vector<Stk_Leader*>		leaders,		/*(i) PMI引出线数据				*/
		vector<STK_INT32>		&drawDir		/*(o) 圆弧绘制方向				*/
												/*  ==  1 原方向				*/
												/*  == -1 反方向				*/
		);
/** 圆弧折线化相关接口_END		by Liu TongPeng **/


/*** 逻辑计算 ***/
public:
	// 两线段求交
	STK_BOOL GetLineCrossPoint(
		STK_UINT32	dim,				/*(i) 维度						*/
		STK_PNT32	inLin1_PntA[2],		/*(i) 线1两端点					*/
		STK_PNT32	inLin2_PntA[2],		/*(i) 线2两端点					*/
		STK_UINT32	&crossFlg,			/*(o) 平行/重合/相交			*/
										/*   ==0 平行   ==1 重合   ==2 相交		*/
		STK_PNT32	&crossPnt			/*(o) 交点坐标/重合时输出线1顶点坐标	*/
		);
	// 判断点列与外框是否干涉
	STK_BOOL CheckPntFrmInter(
		STK_UINT32	dim,				/*(i) 维度						*/
		vector<STK_PNT32> inPntA,		/*(i) 点列坐标					*/
		STK_BOX32	inBox,				/*(i) 外框BOX					*/
		vector<STK_BOOL> &outChk		/*(o) 干涉检查结果				*/
		);
	// Leader 裁剪
	STK_BOOL DivideLeader(
		STK_UINT32			dim,		/*(i) 维度						*/
		vector<STK_PNT32>	inPnt,		/*(i) Leader 点列数据			*/
		vector<STK_PNT32>	&outPnt		/*(o) Leader 裁剪后的点列数据	*/
		);
	// 圆弧线离散
	STK_BOOL MakeArcDiscretization(
		STK_PNT32			arcCenter,	/*(i) 圆心点坐标				*/
		STK_FLOAT32			arcRadius,	/*(i) 圆弧半径					*/
		STK_DOUBLE64		BeginPara,	/*(i) 圆弧绘制的起始角度参数	*/
		STK_DOUBLE64		EndPara,	/*(i) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64		arcAngle,	/*(i) 圆弧实际角度				*/
		STK_INT32			neqFlg,		/*(i) 圆弧绘制方向				*/
										/*  ==  1 逆时针				*/
										/*  == -1 顺时针				*/
		vector<STK_PNT32>	&outPnt		/*(o) 分元后的点列数据			*/
		);
	// 计算圆弧绘制方向
	STK_BOOL GetArcAngleData(
		vector<STK_DOUBLE64>	vecBeginPar,/*(i) 圆弧绘制的起始角度参数	*/
		vector<STK_DOUBLE64>	vecEndPar,	/*(i) 圆弧绘制的终止角度参数	*/
		vector<STK_FLOAT32>		inNormal_Z,	/*(i) 圆弧绘制的终止角度参数	*/
		STK_DOUBLE64			&drawAngle,	/*(o) 圆弧绘制角度				*/
		vector<STK_INT32>		&drawFlg	/*(o) 圆弧绘制方向				*/
											/*  ==  1 逆时针				*/
											/*  == -1 顺时针				*/
		);
	// 计算圆弧上点对应参数方程的角度参数
	STK_BOOL GetCircleAngleParameter(
		STK_UINT32			dim,		/*(i) 维度						*/
		STK_PNT32			arcPnt,		/*(i) 圆弧线上点				*/
		STK_PNT32			arcCenter,	/*(i) 圆心点坐标				*/
		STK_FLOAT32			arcRadius,	/*(i) 圆弧半径					*/
		double				&AnglePara	/*(o) 对应的角度参数			*/
		);


/*** 包围盒管理 by Liutp on 2013.09.03 ***/
public:
	void SetTextBox( STK_BOX32 inbox );	/*	设置包围盒					*/
	void ClearTextBox();				/*	清除文字包围盒的坐标点		*/
	vector<STK_BOX32> GetTextBox();		/*	获取文字包围盒的坐标点		*/
	STK_BOX32 ResetTextBox( STK_BOX32 inbox );	/*	重新计算包围盒		*/

};
