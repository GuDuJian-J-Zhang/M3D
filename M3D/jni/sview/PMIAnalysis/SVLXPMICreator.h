// SVLXPMICreator.h :PMI������
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
	vector<HoteamSoft::SVLLib::STK_BOX32> m_TextBox;		// �ı����Χ��

public:

	//�������
	STK_UINT32 CreateOutFrame(
		Stk_PMIPtr pmi,					/*(i) ��ǰʵ��					*/
		vector<HoteamSoft::SVLLib::STK_PNT32> &fpnt,		/*(o) ��������ɢ��			*/
		vector<STK_UINT32> &fIndices	/*(o) ����������				*/
		);
	//������Χ��
	STK_UINT32 CreateAnchorPoints(
		Stk_PMIPtr pmi,					/*(i) ��ǰʵ��					*/
		vector<HoteamSoft::SVLLib::STK_PNT32> &apnt,		/*(o) �����Χ����ɢ��			*/
		vector<STK_UINT32> &aIndices	/*(o) �����Χ������			*/
		);
	//����PMI������
	STK_UINT32 CreateSpeciallLines(
		Stk_PMIPtr pmi,					/*(i) ��ǰʵ��					*/
		vector<HoteamSoft::SVLLib::STK_PNT32> &epnt,		/*(o) �����������ɢ��			*/
		vector<STK_UINT32> &eIndices	/*(o) �������������			*/
		);
public:
	bool createLeader(Stk_LeaderPtr pStkLeader, vector<FPOINT3> vecPolylinePnts, Stk_PMIPtr pSymbol);
	bool createLeader(const vector<Stk_LeaderPtr>& pLeaders, const HoteamSoft::SVLLib::STK_BOX32& textBox, vector<FPOINT3>& vecPolylinePnts, Stk_PMIPtr pDimension);
	bool createExtensionLines(const vector<Stk_ExtensionlinePtr>& inExtline, vector<FPOINT3>& vecPolylinePnts);
	//����ĩ�˷���
	bool  CreateEndSymbol(
			Stk_PMIPtr pmi,					/*(i) ��ǰʵ��					*/
			vector<FPOINT3>& vecPolylinePnts,
			HoteamSoft::SVLLib::StkTermTypeEnum		&oTermType	/*(o) ĩ�˷�������				*/
			);
public:
		void SetTextBox(HoteamSoft::SVLLib::STK_BOX32 inbox);	/*	���ð�Χ��					*/
		void ClearTextBox();				/*	������ְ�Χ�е������		*/
		vector<HoteamSoft::SVLLib::STK_BOX32> GetTextBox();		/*	��ȡ���ְ�Χ�е������		*/
		HoteamSoft::SVLLib::STK_BOX32 ResetTextBox(HoteamSoft::SVLLib::STK_BOX32 inbox);	/*	���¼����Χ��		*/

};
#endif
