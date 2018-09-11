// SVLXPMICreator.cpp :PMI��������׿��
//

/////////////////////////////////////////////
#include "sview/SView.h"
#include "sview/PMIAnalysis/SVLXPMICreator.h"
#include "sview/PMIAnalysis/SVLXPMIUtilityLib.h"
#include "sview/pmiAnalysis/TessSvlxCircle.h"

#include "sview/PMIAnalysis/toolkit/PMIESymbol.h"
//#include "sview/PMIAnalysis/PMIUtilityLib.h"
#include "Utility.h"

//#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <math.h>
//ISSUE:PMI��������׿�� define
#ifndef SO_END_FACE_INDEX // also defined in SoVRMLIndexedFaceSet.h
#define SO_END_FACE_INDEX (-1)
#endif // !SO_END_FACE_INDEX

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#define	PI		3.14159265358979323846
#define  ISPARALLEL		0x00000100

///////////////////////////////////////////

#define IDXNUM			(16384)
#define ZDEPS			(1.0e-18)
#define MAXPNTVAL		(1.0e+8)
#define ZTOL			(2e-10)		// ���湫��
#define ANGLEPARA_TOL	(0.1)	// �ǶȲ�������
#define ARAAYNUM		(4)
#define PMIDIM2			(2)			// ��ά
#define DIVANGLE		(5)			// ��ɢ��С�Ƕ�
#ifndef CALLBACK
#define CALLBACK
#endif

using namespace HoteamSoft::SVLLib;
SVLXPMICreator::SVLXPMICreator()
{
}

SVLXPMICreator::~SVLXPMICreator()
{
}

bool SVLXPMICreator::createExtensionLines(const vector<Stk_ExtensionlinePtr>& inExtline, vector<FPOINT3>& vecPolylinePnts)
{
	bool bRet = false;
	vector<Stk_ExtensionlinePtr>::size_type i, lineCount = inExtline.size();
	if (0 == lineCount)
	{
		return bRet;
	}

	//vector<Stk_CurveEntity*> vecStkExtLine;
	Stk_ExtensionlinePtr line = NULL;
	Stk_CurveEntityPtr pCruve = NULL;
	Stk_PolyLinePtr polyline = NULL;
	//STK_BOOL			drawRtn = false;
	HoteamSoft::SVLLib::StkCurveTypeEnum typeEnum = HoteamSoft::SVLLib::CURVE_TYPE_UNKNOWN;

	line = inExtline.at(0);
	if (line == NULL) return bRet;

	pCruve = line->GetCurve();
	if (pCruve == NULL) return bRet;

	typeEnum = pCruve->GetCurveType();
	if (typeEnum == HoteamSoft::SVLLib::CURVE_TYPE_ELLIPSE) return bRet;



	vecPolylinePnts.clear();

	for (i = 0; i < lineCount; i++)
	{
		line = inExtline[i];   //���һ���ӳ�������
		if (line == NULL)	continue;

		pCruve = line->GetCurve();
		if (pCruve == NULL) 	continue;
		typeEnum = pCruve->GetCurveType();

		if (HoteamSoft::SVLLib::CURVE_TYPE_POLYLINE == typeEnum)
		{
			polyline = Stk_PolyLinePtr::DynamicCast(pCruve);  //��������ߵ���������
			if (polyline == NULL) continue;
			const vector<HoteamSoft::SVLLib::STK_PNT32>& plPnts = polyline->GetPoints();
			vecPolylinePnts.push_back(plPnts);
		}
		else if (HoteamSoft::SVLLib::CURVE_TYPE_LINE == typeEnum)
		{
			FPOINT3 linePnts;
			Stk_LinePtr stkline = Stk_LinePtr::DynamicCast(pCruve);  //��������ߵ���������
			linePnts.push_back(stkline->GetStartPoint());   //���ʼ����������
			linePnts.push_back(stkline->GetEndPoint());	//����յ���������
			vecPolylinePnts.push_back(linePnts);
		}
	}

	return true;
}


/*
* ���ܣ�����PMI���

* ������inTopInstance    (I)ʵ������ָ��

*       outPntNum        (O)��������ɢ�����

*        fpnt            (O)�����ɢ��

*       outIndexNum      (O)�����������

*        fIndices     	  (O)����������

* ����ֵ�� �Ƿ�ִ�гɹ�

* 2013-07-25   xsn   create
* 2013-08-07	yangxin	���Ӵ�������
* 2013-09-10	�޸�Բ�����߻�����
*/
STK_UINT32 SVLXPMICreator::CreateOutFrame(
	Stk_PMIPtr pmi,					/*(i) ��ǰʵ��					*/
	vector<HoteamSoft::SVLLib::STK_PNT32> &fpnt,		/*(o) ��������ɢ��			*/
	vector<HoteamSoft::SVLLib::STK_UINT32> &fIndices	/*(o) ����������				*/
)
{
	HoteamSoft::SVLLib::STK_INT32 pntCnt = 0;   //��ĸ���

							//���PMI���������
	vector<Stk_OutFramePtr>  outframes = pmi->GetOutFrame();

	//�������ӳ��߽��н���
	for (int i = 0; i < outframes.size(); i++) {
		Stk_OutFramePtr outframe = outframes.at(i);   //���һ���ӳ�������

		vector<Stk_CurveEntityPtr> curves = outframe->GetFrameData();  //����ӳ��ߵ���������
		HoteamSoft::SVLLib::STK_INT32 outframetype = outframe->GetFrameType();	//��ȡ�������

		for (int j = 0; j < curves.size(); j++) {
			Stk_CurveEntityPtr curve = curves.at(j);
			HoteamSoft::SVLLib::STK_INT32 type = curve->GetCurveType();
			vector<HoteamSoft::SVLLib::STK_PNT32> pnt;

			//���������
			if (type == 1) {
				Stk_PolyLinePtr Polyline = Stk_PolyLinePtr::DynamicCast(curve);
				pnt = Polyline->GetPoints();   //�����ɢ������
			}
			//�����Բ��
			else if (type == 2) {
				//Stk_Ellipse *ellipse = (Stk_Ellipse *)curve;
				Stk_ArcPtr Arc = Stk_ArcPtr::DynamicCast(curve);
				// Բ����ɢ����
				CTessSvlxCircle::GetCircleDiscretization(Arc, 0, pnt);
			}

			//�����������������Ŀ������
			if (!pnt.empty()) {
				for (int j = 0; j < pnt.size(); j++) {
					fpnt.push_back(pnt[j]);
					fIndices.push_back(pntCnt);
					pntCnt++;
				}
				//���÷ָ���
				fIndices.push_back(SO_END_FACE_INDEX);
			}
		}
	}
	return true;
}

/*
* ���ܣ�����PMI��Χ��

* ������inTopInstance    (I)ʵ������ָ��

*       outPntNum        (O)�����Χ����ɢ�����

*        apnt            (O)��Χ����ɢ��

*       outIndexNum      (O)��Χ����������

*       aIndices     	  (O)�����Χ������

* ����ֵ�� �Ƿ�ִ�гɹ�

* 2013-07-25   xsn   create
* 2013-08-01   yangxin �޸Ĳ���
*/
STK_UINT32 SVLXPMICreator::CreateAnchorPoints(
	Stk_PMIPtr pmi,					/*(i) ��ǰʵ��					*/
	vector<HoteamSoft::SVLLib::STK_PNT32> &apnt,		/*(o) �����Χ����ɢ��			*/
	vector<STK_UINT32> &aIndices	/*(o) �����Χ������			*/
)
{
	return true;
}
/*
* ���ܣ�����ĩ�˷���

* ������inTopInstance    (I)ʵ������ָ��

*       outPntNum        (O)���ĩ�˷�����ɢ�����

*       spnt            (O)ĩ�˷�����ɢ��

*       outIndexNum      (O)ĩ�˷�����������

*       sIndices     	  (O)���ĩ�˷�������

* ����ֵ�� �Ƿ�ִ�гɹ�

* 2013-07-25   xsn   create
* 2013-08-01   yangxin �޸Ĳ���
*/

bool  SVLXPMICreator::CreateEndSymbol(
	Stk_PMIPtr pmi,					/*(i) ��ǰʵ��					*/
	vector<FPOINT3>& vecPolylinePnts,
	HoteamSoft::SVLLib::StkTermTypeEnum		&oTermType	/*(o) ĩ�˷�������				*/
)
{
	bool bRet = false;
	CPMIESymbol			pmiSym;			//����ĩ�˷��ŵ���
	STK_DOUBLE64		pnt1[3] = {};		//ĩ�˷�����ʼ������
	STK_DOUBLE64		refvec[3] = {};		//ĩ�˷��ŷ�������
	CPOINT3Array		outPnts, outPnts2;	//ĩ�˷�����ɢ��
	STK_FLOAT32			oSize[2] = {};		//ĩ�˷��ſ�͸�
	HoteamSoft::SVLLib::STK_PNT32			oLoc;			//ĩ�˷�����ʼλ��
	HoteamSoft::SVLLib::STK_DIR32			oDir;			//ĩ�˷��ŷ���

	STK_BOOL			endDrawRtn = false;

	if (pmi == NULL) return bRet;
	vector<Stk_LeaderPtr> pLeaders = pmi->GetLeaders();

	vector<STK_INT32>	endDrawDir;		// Բ�����Ʒ���
	Stk_LeaderPtr pStkLeader = NULL;

	vector<Stk_CurveEntityPtr> CurveList;
	Stk_CurveEntityPtr pCruve = NULL;
	HoteamSoft::SVLLib::StkCurveTypeEnum crvType = HoteamSoft::SVLLib::CURVE_TYPE_UNKNOWN;

	int i = 0, n = 0, nLead = pLeaders.size();

	if (nLead <= 0) return false;
	outPnts.clear();
	outPnts2.clear();
	endDrawDir.clear();
	vecPolylinePnts.clear();

	pStkLeader = pLeaders.at(0);
	if (pStkLeader == NULL)	return bRet;
	CurveList = pStkLeader->GetAllCurve();
	if (CurveList.size() == 0) return bRet;
	pCruve = CurveList[CurveList.size() - 1];
	if (pCruve == NULL)	return bRet;
	crvType = pCruve->GetCurveType();
	//endDrawRtn = CPMIUtilityLib::GetEndSymbolDirection(pLeaders, endDrawDir);
	endDrawRtn = CSVLXPMIUtilityLib::GetEndSymbolDirection(pLeaders, endDrawDir);
	for (n = 0; n < nLead; n++)
	{
		outPnts.clear();
		outPnts2.clear();

		pStkLeader = pLeaders.at(n);
		if (pStkLeader == NULL) return bRet;
		pStkLeader->GetTerminator(oTermType, oSize, oLoc, oDir);//���ĩ�˷����������

																// �쳣�����⴦��		by Liutp on 2013.09.10
		if (oDir.X != oDir.X || oDir.Y != oDir.Y) return false;


		//����ĩ�˷���
		//CPMIUtilityLib::UpdateEndSymData(pDimension,pStkLeader,oTermType,oSize,oLoc,oDir);

		// ĩ�˷��ŷ������¼��� by Liutp 2013.9.11
		if ((crvType == HoteamSoft::SVLLib::CURVE_TYPE_POLYLINE) && endDrawRtn)
		{
			oDir.X = oDir.X*endDrawDir.at(n);
			oDir.Y = oDir.Y*endDrawDir.at(n);
			oDir.Z = oDir.Z*endDrawDir.at(n);
		}

		//���ĩ�˷�����ʼ������
		pnt1[0] = oLoc.X;
		pnt1[1] = oLoc.Y;
		pnt1[2] = oLoc.Z;
		//���ĩ�˷��ŷ�������
		refvec[0] = oDir.X;
		refvec[1] = oDir.Y;
		refvec[2] = oDir.Z;

		//bing added for new interface of symbol--------------------------
		double normalVec[3]; //ĩ�˷��ŷ���������bing added
		double symvec[3]; //temp

						  //��λ��
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
		pmiSym.SetESymbolInfo(oSize[0], oSize[1], pnt1, refvec, normalVec, oTermType, outPnts, outPnts2);

		FPOINT3 vecpnt;
		HoteamSoft::SVLLib::STK_PNT32 pnt;
		int nsize = outPnts.size();
		for (i = 0; i < nsize; i++)
		{

			pnt.X = outPnts[i].x;
			pnt.Y = outPnts[i].y;
			pnt.Z = outPnts[i].z;
			vecpnt.push_back(pnt);
		}

		if (nsize > 0) vecPolylinePnts.push_back(vecpnt);
		vecpnt.clear();
		nsize = outPnts2.size();

		for (i = 0; i < nsize; i++)
		{

			pnt.X = outPnts2[i].x;
			pnt.Y = outPnts2[i].y;
			pnt.Z = outPnts2[i].z;
			vecpnt.push_back(pnt);
		}
		if (nsize > 0)	vecPolylinePnts.push_back(vecpnt);
	}

	return true;
}


// ���ð�Χ�� by YANGXI
void SVLXPMICreator::SetTextBox(
	HoteamSoft::SVLLib::STK_BOX32 inbox					/*(i)�������ְ�Χ�е������		*/
) {
	m_TextBox.push_back(inbox);
}

// �����Χ������ by YANGXI
void SVLXPMICreator::ClearTextBox(
) {
	m_TextBox.clear();
}

// ��ȡ��Χ������ by Liutp
vector<HoteamSoft::SVLLib::STK_BOX32> SVLXPMICreator::GetTextBox(
) {
	return m_TextBox;
}

// ���¼����Χ������ by Liutp
HoteamSoft::SVLLib::STK_BOX32 SVLXPMICreator::ResetTextBox(HoteamSoft::SVLLib::STK_BOX32 inbox)
{
	HoteamSoft::SVLLib::STK_BOX32 outbox;
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


//����PMI������
STK_UINT32 SVLXPMICreator::CreateSpeciallLines(
	Stk_PMIPtr pmi,					/*(i) ��ǰʵ��					*/
	vector<HoteamSoft::SVLLib::STK_PNT32> &epnt,		/*(o) �����������ɢ��			*/
	vector<STK_UINT32> &eIndices	/*(o) �������������			*/
)
{

	STK_INT32 pntCnt = 0; //��ĸ���

						  //���PMI����������
	vector<Stk_SpecialLinePtr>  lines = pmi->GetSpecialLines();

	//�������ӳ��߽��н���
	for (int i = 0; i < lines.size(); i++) {
		Stk_SpecialLinePtr line = lines.at(i);   //���һ������������
		vector<Stk_CurveEntityPtr> vecSpecialLine = line->GetCurves();
		Stk_CurveEntityPtr SpecialLine;
		for (int j = 0; j < vecSpecialLine.size(); j++) {
			SpecialLine = vecSpecialLine.at(j);
			StkCurveTypeEnum type = SpecialLine->GetCurveType();	//�������������
			vector<STK_PNT32> pnt;
			//���������
			if (type == CURVE_TYPE_POLYLINE) {
				Stk_PolyLinePtr polyline = Stk_PolyLinePtr::DynamicCast(SpecialLine);
				pnt = polyline->GetPoints();   //�����ɢ������
			}
			//�����Բ��
			else if (type == CURVE_TYPE_ELLIPSE) {
				Stk_ArcPtr arc = Stk_ArcPtr::DynamicCast(SpecialLine);
																	// Բ����ɢ����
				CTessSvlxCircle::GetCircleDiscretization(arc, 0, pnt);
			}
			else if (type == CURVE_TYPE_LINE) {
				Stk_LinePtr stkline = Stk_LinePtr::DynamicCast(SpecialLine);
				STK_PNT32 startpnt = stkline->GetStartPoint();   //���ʼ����������
				STK_PNT32 endpnt = stkline->GetEndPoint();	//����յ���������

															//�����������������Ŀ������
				pnt.push_back(startpnt);
				pnt.push_back(endpnt);
			}
			//�����������������Ŀ������
			if (!pnt.empty()) {
				for (int j = 0; j < pnt.size(); j++) {
					epnt.push_back(pnt[j]);
					eIndices.push_back(pntCnt);
					pntCnt++;
				}
				//���÷ָ���
				eIndices.push_back(SO_END_FACE_INDEX);
			}
		}
	}
	return true;
}

bool SVLXPMICreator::createLeader(Stk_LeaderPtr pStkLeader, vector<FPOINT3> vecPolylinePnts, Stk_PMIPtr pSymbol)
{
	STK_INT32			pntCnt = 0;		//��ĸ���
	CPMIESymbol			pmiSym;			//����ĩ�˷��ŵ���
	STK_DOUBLE64		pnt1[3] = {};		//ĩ�˷�����ʼ������
	STK_DOUBLE64		refvec[3] = {};		//ĩ�˷��ŷ�������

	STK_PNT32			tmp;			//��ʱ����
	vector<STK_INT32>	drawDir;		// Բ�����Ʒ���
	STK_BOOL			drawRtn = false;

	//��������509ƽ������Ļ��ע�͵��������⡣
	STK_BOOL bParallel = false;

	vector<STK_UINT32>		vecIndices;


	StkPMISourceEnum EnumSource = PMISourceUnknown;
	StkPMITypeEnum   EnumType = PMI_TYPE_UNKNOWN;
	if (pSymbol != NULL)
	{
		//bParallel = pSymbol->IsParallelScreen();
		STK_INT32 style = pSymbol->GetStyle();
		bParallel = style & ISPARALLEL == ISPARALLEL ? true : false;
		EnumSource = pSymbol->GetPMISource();
		EnumType = pSymbol->GetType();
	}


	int i = 0, j = 0, pntNum = 0;
	bool	exceptPnt = false;
	bool    bFlag = true;

	//�Ե�һ����ĩ�˷�����ɢ����н���
	vector<Stk_CurveEntityPtr>polylines = pStkLeader->GetAllCurve();
	Stk_CurveEntityPtr stkPolyline = NULL;
	if (polylines.size() > 0)//Datakit��leaderֻ��������������
		stkPolyline = polylines[0];

	if (stkPolyline != NULL)
	{
		if (CURVE_TYPE_POLYLINE == stkPolyline->GetCurveType())
		{
			//��������509ƽ������Ļ��ע�͵��������⡣

			//Stk_PolyLine *polyline = (Stk_PolyLine *)stkPolyline;  //��������ߵ���������
			Stk_PolyLinePtr polyline = Stk_PolyLinePtr::DynamicCast(stkPolyline);

			vector<STK_PNT32> plPnts = polyline->GetPoints();
			if (bParallel
				&&PMI_SOURCE_CONVERT == EnumSource
				&&PMI_TYPE_TypeNote == EnumType)
			{

				while (plPnts.size() > 2)
					plPnts.pop_back();



				if (pSymbol != NULL)
				{
					STK_PNT32 minInnerLoc;
					Stk_TextPtr pStkText = NULL;
					STK_PNT32 innerLoc;

					Stk_ComTextPtr pComText = NULL;
					vector<Stk_ComTextPtr> vecComtxt = pSymbol->GetComText();
					int ncomtxt = vecComtxt.size();
					if (ncomtxt > 0)
					{
						pComText = vecComtxt.at(0);
						vector<Stk_TextPtr>vecText = pComText->GetTexts();
						int ntxt = vecText.size();

						if (ntxt > 0)
						{
							minInnerLoc = vecText[0]->GetInnerLoc();
							for (i = 0; i < ntxt; i++)
							{
								pStkText = vecText[i];
								if (pStkText == NULL)
									continue;

								innerLoc = pStkText->GetInnerLoc();
								if (minInnerLoc.X > innerLoc.X)
									minInnerLoc.X = innerLoc.X;

								if (minInnerLoc.Y > innerLoc.Y)
									minInnerLoc.Y = innerLoc.Y;
							}
							plPnts.pop_back();
							plPnts.push_back(minInnerLoc);
						}
					}
				}
			}

			vecPolylinePnts.push_back(plPnts);   //�����ɢ������
												 //clipLeader(m_pLeaderKit->clipBox.getValue(), vecPolylinePnts[vecPolylinePnts.size()-1]);
			pntNum += plPnts.size();
		}
		else if (CURVE_TYPE_ELLIPSE == stkPolyline->GetCurveType())
		{
			FPOINT3 elpPnts;
			//Stk_Ellipse *ellipse = (Stk_Ellipse *)stkPolyline;  //��������ߵ���������
			Stk_ArcPtr arc = Stk_ArcPtr::DynamicCast(stkPolyline);
			float UMin = 0.f, UMax = 0.0f, arcAngle = 0.0f, arcMinor = 0.0f;
			int drawFlg = 0;
			//clipLeader(m_pLeaderKit->clipBox.getValue(), ellipse, UMin, UMax, arcAngle, drawFlg);
			arc->SetRange(UMin, UMax);
			STK_DIR32 normalDir = arc->GetNormal();
			drawFlg = (normalDir.Z > 0) ? 1 : -1;
			CTessSvlxCircle::GetCircleDiscretization(arc, UMin, UMax, arcAngle, drawFlg, elpPnts);

			vecPolylinePnts.push_back(elpPnts);
			pntNum += elpPnts.size();
		}

		else if (CURVE_TYPE_LINE == stkPolyline->GetCurveType())
		{
			FPOINT3 linePnts;
			Stk_LinePtr stkline = Stk_LinePtr::DynamicCast(stkPolyline);
			linePnts.push_back(stkline->GetStartPoint());   //���ʼ����������
			linePnts.push_back(stkline->GetEndPoint());	//����յ���������
			vecPolylinePnts.push_back(linePnts);
			pntNum += 2;
		}
	}

	return true;
}

bool SVLXPMICreator::createLeader(const vector<Stk_LeaderPtr>& pLeaders, const HoteamSoft::SVLLib::STK_BOX32& textBox, vector<FPOINT3>& vecPolylinePnts, Stk_PMIPtr pDimension)
{
	STK_INT32			pntCnt = 0;		//��ĸ���
	STK_DOUBLE64		refvec[3];		//ĩ�˷��ŷ�������
	STK_PNT32			tmp;			//��ʱ����
	vector<STK_INT32>	drawDir;		// Բ�����Ʒ���
	vector<STK_BOX32>   vecTextBox;     // �ı��İ�Χ������

	int i = 0, j = 0, n = 0, nLead = 0, pntNum = 0, numVertice = 0, numLines = 0;
	bool	exceptPnt = false;


	Stk_LeaderPtr pStkLeader = NULL;

	vector<STK_INT32>	vecDrawFlg;
	vector<STK_DOUBLE64>	vecBeginPara, vecEndPara;
	STK_DOUBLE64		arcAngle = 0.0;
	STK_BOOL			drawRtn = false;
	STK_BOOL			endDrawRtn = false;

	vector<STK_INT32>	endDrawDir;		// Բ�����Ʒ���

	STK_DOUBLE64 fBeginPara = 0.0;
	STK_DOUBLE64 fEndPara = 0.0;
	STK_INT32    nDrawFlg = 0;

	Stk_CurveEntityPtr pCruve = NULL;
	StkCurveTypeEnum crvType = CURVE_TYPE_UNKNOWN;

	double fRealAngle = -1.0;
	STK_PNT32 boxCenterPnt;

	int idx = -1;

	StkPMISourceEnum EnumSource = pDimension->GetPMISource();
	StkPMITypeEnum   EnumType = pDimension->GetType();

	vector<STK_PNT32>vecNewPnt;

	vecBeginPara.clear();
	vecEndPara.clear();
	vecDrawFlg.clear();
	vecNewPnt.clear();
	endDrawDir.clear();
	vecTextBox.clear();

	vecTextBox.push_back(textBox);

	if (EnumType == PMI_TYPE_TypeNote)
		vecTextBox.clear();


	nLead = pLeaders.size();
	if (nLead <= 0)
		return false;

	pntNum = 0;
	numVertice = 0;
	numLines = 0;


	pStkLeader = pLeaders.at(0);

	vector<Stk_CurveEntityPtr> CurveList = pStkLeader->GetAllCurve();
	if (CurveList.size() == 0)
	{
		return false;
	}
	pCruve = CurveList[CurveList.size() - 1];

	crvType = pCruve->GetCurveType();
	if (crvType == CURVE_TYPE_ELLIPSE)
	{
		drawRtn = CTessSvlxCircle::GetCircleDrawData(pLeaders, vecBeginPara, vecEndPara, arcAngle, vecDrawFlg);

	}

	endDrawRtn = CSVLXPMIUtilityLib::GetEndSymbolDirection(pLeaders, endDrawDir);

	boxCenterPnt.X = (textBox.BoundBox[0][0] + textBox.BoundBox[1][0])*0.5;
	boxCenterPnt.Y = (textBox.BoundBox[0][1] + textBox.BoundBox[1][1])*0.5;
	boxCenterPnt.Z = (textBox.BoundBox[0][2] + textBox.BoundBox[1][2])*0.5;

	vector<STK_PNT32> outVecExtPnt;
	vecPolylinePnts.clear();
	for (n = 0; n < nLead; n++)
	{
		vector<STK_PNT32>		tmpVeclpnt;		/*(o) �޼�������ʱ�õ�			*/
		vector<STK_UINT32>		tmpVeclIndices;	/*(o) �޼�������ʱ�õ�			*/
		STK_INT32			    tmpPntCnt = 0;	/*(o) �޼�������ʱ�õ�			*/



		vecNewPnt.clear();

		pStkLeader = pLeaders.at(n);

		//�Ե�һ����ĩ�˷�����ɢ����н���
		vector<Stk_CurveEntityPtr>polylines = pStkLeader->GetAllCurve();
		Stk_CurveEntityPtr stkPolyline = NULL;
		if (polylines.size() > 0)//Datakit��leaderֻ��������������
			stkPolyline = polylines[0];

		if (stkPolyline != NULL)
		{
			if (CURVE_TYPE_POLYLINE == stkPolyline->GetCurveType())
			{
				Stk_PolyLinePtr polyline = Stk_PolyLinePtr::DynamicCast(stkPolyline);
				const vector<STK_PNT32>& plPnts = polyline->GetPoints();

				if (EnumSource == PMI_SOURCE_CREATE &&
					(EnumType == PMI_TYPE_RadiusDimension || EnumType == PMI_TYPE_DiameterDimension))
				{
					if (plPnts.size() == 2) CSVLXPMIUtilityLib::DivideLeader(2, vecTextBox, plPnts, vecNewPnt);
				}
				else
				{
					//�˴������вü�--yhp-20180816
					//CSVLXPMIUtilityLib::DivideLeader(2, vecTextBox, plPnts, vecNewPnt);
				}

				if (!plPnts.empty())
				{
					if (vecNewPnt.empty())
					{
						for (int j = 0; j<plPnts.size(); j++)
						{
							tmpVeclpnt.push_back(plPnts[j]);
							tmpVeclIndices.push_back(tmpPntCnt);
							tmpPntCnt++;
						}
					}
					else
					{
						for (int j = 0; j<vecNewPnt.size(); j++)
						{
							tmpVeclpnt.push_back(vecNewPnt[j]);
							tmpVeclIndices.push_back(tmpPntCnt);
							tmpPntCnt++;
						}
					}
					//���÷ָ���
				}
				vecPolylinePnts.push_back(tmpVeclpnt);
				pntNum += tmpPntCnt;
			}
			else if (CURVE_TYPE_ELLIPSE == stkPolyline->GetCurveType())
			{
				FPOINT3 elpPnts;
				//Stk_Ellipse *ellipse = (Stk_Ellipse *)stkPolyline;  //��������ߵ���������
				Stk_ArcPtr arc = Stk_ArcPtr::DynamicCast(stkPolyline);
				float UMin = 0.f, UMax = 0.0f, arcAngle = 0.0f, arcMinor = 0.0f;
				int drawFlg = 0;
				//clipLeader(m_pLeaderKit->clipBox.getValue(), ellipse, UMin, UMax, arcAngle, drawFlg);
				/*ellipse->GetDomain(UMin,UMax);
				STK_DIR32 normalDir = ellipse->GetNormal();
				drawFlg = (normalDir.Z>0)?1:-1;*/
				if (drawRtn)
				{
					fBeginPara = vecBeginPara.at(n);
					fEndPara = vecEndPara.at(n);
					nDrawFlg = vecDrawFlg.at(n);
					//CTessCircle::GetCircleDiscretization(ellipse, UMin, UMax,  arcAngle, drawFlg, elpPnts);

					CTessSvlxCircle::GetCircleDiscretization(arc, fBeginPara, fEndPara, 0.0, nDrawFlg, elpPnts);
				}
				else
					CTessSvlxCircle::GetCircleDiscretization(arc, 0, elpPnts);
				CSVLXPMIUtilityLib::DivideLeader(2, vecTextBox, elpPnts, vecNewPnt/*,bConverted*/);

				if (!elpPnts.empty()) {
					if (vecNewPnt.empty()) {
						for (int j = 0; j<elpPnts.size(); j++) {
							tmpVeclpnt.push_back(elpPnts[j]);
							tmpVeclIndices.push_back(tmpPntCnt);
							tmpPntCnt++;
						}
					}
					else {
						for (int j = 0; j<vecNewPnt.size(); j++) {
							tmpVeclpnt.push_back(vecNewPnt[j]);
							tmpVeclIndices.push_back(tmpPntCnt);
							tmpPntCnt++;
						}
					}
					//���÷ָ���

				}



				/*vecPolylinePnts.push_back(elpPnts);
				pntNum += elpPnts.size();*/
				vecPolylinePnts.push_back(tmpVeclpnt);
				pntNum += tmpPntCnt;

				if (i == 0 && outVecExtPnt.size()>0)
				{
					vecNewPnt.clear();
					CSVLXPMIUtilityLib::DivideLeader(2, vecTextBox, outVecExtPnt, vecNewPnt/*,bConverted*/);
					if (vecNewPnt.empty())
					{
						vecPolylinePnts.push_back(outVecExtPnt);
						pntNum += outVecExtPnt.size();
					}
					else
					{
						vecPolylinePnts.push_back(vecNewPnt);
						pntNum += vecNewPnt.size();
					}
				}
			}

			else if (CURVE_TYPE_LINE == stkPolyline->GetCurveType())
			{
				FPOINT3 linePnts;
				//Stk_Line *stkline = (Stk_Line *)stkPolyline;  //��������ߵ���������
				Stk_LinePtr stkline = Stk_LinePtr::DynamicCast(stkPolyline);
				linePnts.push_back(stkline->GetStartPoint());   //���ʼ����������
				linePnts.push_back(stkline->GetEndPoint());	//����յ���������
				vecPolylinePnts.push_back(linePnts);
				pntNum += 2;
			}
		}
	}
	return true;
}

