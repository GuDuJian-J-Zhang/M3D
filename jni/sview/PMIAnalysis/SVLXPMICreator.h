// SVLXPMICreator.h :PMI生成器
//
#ifndef SVLX_PMI_CREATOR_
#define SVLX_PMI_CREATOR_

#include "SVLLib/Stk_Declare_Ptr.h"
#include "SVLLib/Stk_Define.h"
#include "SVLLib/Stk_PMIEntity.h"
#include "SVLLib/Stk_PMI.h"
#include "SVLLib/Stk_Enum.h"
#include "m3d/M3D.h"

using HoteamSoft::SVLLib::Stk_PMIPtr;
using HoteamSoft::SVLLib::Stk_LeaderPtr;
using HoteamSoft::SVLLib::Stk_ExtensionlinePtr;
using HoteamSoft::SVLLib::STK_UINT32;

typedef vector<HoteamSoft::SVLLib::STK_PNT32> FPOINT3;
//typedef bool BOOL;
class M3D_API SVLXPMICreator {

public:
	SVLXPMICreator();
	~SVLXPMICreator();

private:
	vector<HoteamSoft::SVLLib::STK_BOX32> m_TextBox;		// 文本框包围盒

public:

	//创建外框
	STK_UINT32 CreateOutFrame(
		Stk_PMIPtr pmi,					/*(i) 当前实例					*/
		vector<HoteamSoft::SVLLib::STK_PNT32> &fpnt,		/*(o) 输出外框离散点			*/
		vector<STK_UINT32> &fIndices	/*(o) 输出外框索引				*/
		);
	//创建包围框
	STK_UINT32 CreateAnchorPoints(
		Stk_PMIPtr pmi,					/*(i) 当前实例					*/
		vector<HoteamSoft::SVLLib::STK_PNT32> &apnt,		/*(o) 输出包围框离散点			*/
		vector<STK_UINT32> &aIndices	/*(o) 输出包围框索引			*/
		);
	//创建PMI特殊线
	STK_UINT32 CreateSpeciallLines(
		Stk_PMIPtr pmi,					/*(i) 当前实例					*/
		vector<HoteamSoft::SVLLib::STK_PNT32> &epnt,		/*(o) 输出特殊线离散点			*/
		vector<STK_UINT32> &eIndices	/*(o) 输出特殊线索引			*/
		);
public:
	bool createLeader(Stk_LeaderPtr pStkLeader, vector<FPOINT3> vecPolylinePnts, Stk_PMIPtr pSymbol);
	bool createLeader(const vector<Stk_LeaderPtr>& pLeaders, const HoteamSoft::SVLLib::STK_BOX32& textBox, vector<FPOINT3>& vecPolylinePnts, Stk_PMIPtr pDimension);
	bool createExtensionLines(const vector<Stk_ExtensionlinePtr>& inExtline, vector<FPOINT3>& vecPolylinePnts);
	//创建末端符号
	bool  CreateEndSymbol(
			Stk_PMIPtr pmi,					/*(i) 当前实例					*/
			vector<FPOINT3>& vecPolylinePnts,
			HoteamSoft::SVLLib::StkTermTypeEnum		&oTermType	/*(o) 末端符号类型				*/
			);
public:
		void SetTextBox(HoteamSoft::SVLLib::STK_BOX32 inbox);	/*	设置包围盒					*/
		void ClearTextBox();				/*	清除文字包围盒的坐标点		*/
		vector<HoteamSoft::SVLLib::STK_BOX32> GetTextBox();		/*	获取文字包围盒的坐标点		*/
		HoteamSoft::SVLLib::STK_BOX32 ResetTextBox(HoteamSoft::SVLLib::STK_BOX32 inbox);	/*	重新计算包围盒		*/

};
#endif
