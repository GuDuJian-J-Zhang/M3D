// PMICreator.h :PMI生成器
//
#ifndef PMI_CREATOR_
#define PMI_CREATOR_
#include "m3d/M3D.h"
#include "prebuilt/svl2/include/Stk_Instance.h"
#include "prebuilt/svl2/include/Stk_PMIEntity.h"
#include "prebuilt/svl2/include/Stk_CurveEntity.h"
#include "prebuilt/svl2/include/Stk_CompositeText.h"
typedef bool BOOL;
typedef vector<STK_PNT32> FPOINT3;
class M3D_API PMICreator{

public:
	PMICreator();
	~PMICreator();

private:
	vector<STK_BOX32> m_TextBox;		// 文本框包围盒

public:
	//创建引出线
//STK_UINT32 CreateLeaders(
//		Stk_PMI *pmi,					/*(i) 当前实例					*/
//		vector<STK_PNT32> &lpnt,		/*(o) 输出引出线离散点			*/
//		vector<STK_UINT32> &lIndices	/*(o) 输出引出线索引			*/
//		);

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
public:
	BOOL createLeader(Stk_Leader* pStkLeader,vector<FPOINT3> vecPolylinePnts,Stk_PMI*pSymbol);
	BOOL createLeader( const vector<Stk_Leader*>& pLeaders,const STK_BOX32& textBox,vector<FPOINT3>& vecPolylinePnts,Stk_PMI* pDimension);
	BOOL createExtensionLines( const vector<Stk_Extensionline*>& inExtline,vector<FPOINT3>& vecPolylinePnts);
	BOOL createCompositeText(Stk_ComText* pStkCompText,Stk_PMI*pSymbol);
	//创建末端符号
		BOOL  CreateEndSymbol(
			Stk_PMI *pmi,					/*(i) 当前实例					*/
			vector<FPOINT3>& vecPolylinePnts,
			StkTermTypeEnum		&oTermType	/*(o) 末端符号类型				*/
			);
public:
	void SetTextBox( STK_BOX32 inbox );	/*	设置包围盒					*/
	void ClearTextBox();				/*	清除文字包围盒的坐标点		*/
	vector<STK_BOX32> GetTextBox();		/*	获取文字包围盒的坐标点		*/
	STK_BOX32 ResetTextBox( STK_BOX32 inbox );	/*	重新计算包围盒		*/

};
#endif
