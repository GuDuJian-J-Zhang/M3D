#include "SVLLib/Stk_CurveEntity.h"
#include "SVLXPMIUtilityLib.h"
#include <math.h>
#include "sview/PMIAnalysis/toolkit/MathVector.h"
//#include "DimSymUitity.h"
#include <stdlib.h>

#define ARAAYNUM		(4)
#define PMIDIM2			(2)	
#define ZTOL			(2e-10)		// ���湫��

using namespace HoteamSoft::SVLLib;
CSVLXPMIUtilityLib::CSVLXPMIUtilityLib(void)
{
}

CSVLXPMIUtilityLib::~CSVLXPMIUtilityLib(void)
{
}

/*
* ���ܣ���ȡĩ�˷��ŷ���

* ������

*       leaders               (i) PMI����������

*       drawDir               (o) ĩ�˷��Ż��Ʒ���

* ����ֵ�� ִ�гɹ�/ʧ��

*
*/
bool CSVLXPMIUtilityLib::GetEndSymbolDirection(
	vector<Stk_LeaderPtr>		leaders,		/*(i) PMI����������				*/
	vector<STK_INT32>		&drawDir		/*(o) Բ�����Ʒ���				*/
											/*  ==  1 ԭ����				*/
											/*  == -1 ������				*/
) {
	Stk_PolyLinePtr polyline;					// ����
	vector<STK_PNT32> pnt;
	STK_PNT32	StartPnt[2], EndPnt[2];
	STK_INT32	drawDirA[2];

	if (leaders.size() != 2) {
		return false;
	}
	for (int i = 0; i<leaders.size(); i++) {
		Stk_LeaderPtr leader = leaders.at(i);				// ���һ������������
		STK_UINT32 type = leader->GetLeaderType();			// �������������

		if (type == 1) {
			pnt.clear();
			vector<Stk_CurveEntityPtr> CurveList = leader->GetAllCurve();
			if (CurveList.size() == 0)
			{
				return false;
			}
			Stk_CurveEntityPtr pCurve = CurveList[CurveList.size() - 1];
			polyline = Stk_PolyLinePtr::DynamicCast(pCurve);	// ��������ߵ���������
			pnt = polyline->GetPoints();					// �����ɢ������
			StartPnt[i] = pnt.at(0);
			EndPnt[i] = pnt.at(1);
		}
		else {
			return false;
		}
	}

	// ���Ʒ������¼���
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
* ���ܣ�Leader �ü�

* ������dim                   (I) ά��

*       inPnt                 (I) Leader ��������

*       outPnt                (O) Leader �ü���ĵ�������

* ����ֵ�� ִ�гɹ�/ʧ��

*
*/
STK_BOOL CSVLXPMIUtilityLib::DivideLeader(
	STK_UINT32			dim,		/*(i) ά��						*/
	vector<STK_BOX32> TextBox,
	vector<STK_PNT32>	inPnt,		/*(i) Leader ��������			*/
	vector<STK_PNT32>	&outPnt//,		/*(o) Leader �ü���ĵ�������	*/
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

	// ά�ȼ��
	if (dim != PMIDIM2)  return false;
	for (j = 0; j<TextBox.size(); j++)
	{
		box = TextBox.at(j);
		outChk.clear();
		// Box����
		newbox = ResetTextBox(box);

		/*** (1) ������и��� ***/
		if (!CheckPntFrmInter(dim, inPnt, newbox, outChk))  continue;


		/*** (2) ȡ���߿���ĸ����� ***/
		// ˳ʱ��0->1->2->3
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

		// Ϊ����������׷��һ��ʼ��
		lin1_Pnt[4].X = lin1_Pnt[0].X;
		lin1_Pnt[4].Y = lin1_Pnt[0].Y;
		lin1_Pnt[4].Z = 0.0;

		/*** (3) ���㽻�� ***/
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
					// �������ʱ��Ҫ���⴦��ȡ���붥������ĵ�
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
				// ����ı���ü��������Ҫ��һ���Ĳ���
				//if(bConverted)
				break;
			}
		}
	}

	return true;
}

// ���¼����Χ������ 
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
* ���ܣ��жϵ���������Ƿ����

* ������dim,                  (I) ά��

*       inPntA                (I) ��������

*       inBox                 (I) ���BOX

*       outChk                (O) ����� T:����   F:������

* ����ֵ���ӿ��Ƿ�ִ�гɹ�

*
*/
STK_BOOL CSVLXPMIUtilityLib::CheckPntFrmInter(
	STK_UINT32		dim,			/*(i) ά��						*/
	vector<STK_PNT32> inPntA,		/*(i) ��������					*/
	STK_BOX32		inBox,			/*(i) ���BOX					*/
	vector<STK_BOOL> &outChk		/*(o) ��������				*/
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

		// ������е�BOX
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

		// ������
		// X�����ж�
		if (chkBox.BoundBox[1][0] < inBox.BoundBox[0][0] - ZTOL || chkBox.BoundBox[0][0] > inBox.BoundBox[1][0] + ZTOL) {
			Chk = false;
		}
		// Y�����ж�
		if (chkBox.BoundBox[1][1] < inBox.BoundBox[0][1] - ZTOL || chkBox.BoundBox[0][1] > inBox.BoundBox[1][1] + ZTOL) {
			Chk = false;
		}
		if (Chk) IsUse = true;
		outChk.push_back(Chk);
	}

	return IsUse;
}

/*
* ���ܣ���֪�߶ζ˵㣬��ά�߶���

* ���̣�y = kx + b

* ������dim,                  (I) ά��

*       inLin1_PntA[2]        (I) ��1���˵�

*       inLin2_PntA[2]        (I) ��2���˵�

*       crossFlg              (O) ƽ��/���ڽ���

*       crossPnt              (O) ��������

* ����ֵ�� �����Ƿ���Ч/�ӿ��Ƿ�ִ�гɹ�

*
*/
STK_BOOL CSVLXPMIUtilityLib::GetLineCrossPoint(
	STK_UINT32	dim,				/*(i) ά��						*/
	STK_PNT32	inLin1_PntA[2],		/*(i) ��1���˵�(�߿��)			*/
	STK_PNT32	inLin2_PntA[2],		/*(i) ��2���˵�					*/
	STK_UINT32	&crossFlg,			/*(o) ƽ��/�غ�/�ཻ			*/
									/*   ==0 ƽ��   ==1 �غ�   ==2 �ཻ		*/
	STK_PNT32	&crossPnt			/*(o) ��������/�غ�ʱ����߿򶥵�����	*/
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

	// �ж�ά��
	if (PMIDIM2 != dim) {
		return false;
	}

	//###### (1)����б�� ######
	// ��1б��
	if (inLin1_PntA[1].X == inLin1_PntA[0].X) {
		// ��������ƽ��?
		if (inLin1_PntA[1].Y == inLin1_PntA[0].Y) {
			// �����غ�
			return false;
		}
		else {
			// ������X=inLin1_PntA[0].X
			IsLin1_X = true;
			Lin1_minY = inLin1_PntA[1].Y > inLin1_PntA[0].Y ? inLin1_PntA[0].Y : inLin1_PntA[1].Y;
			Lin1_maxY = inLin1_PntA[1].Y < inLin1_PntA[0].Y ? inLin1_PntA[0].Y : inLin1_PntA[1].Y;
		}
	}
	else {
		Lin1_K = (inLin1_PntA[1].Y - inLin1_PntA[0].Y) / (inLin1_PntA[1].X - inLin1_PntA[0].X);
		// ȡ����1��x��Ч����
		if (inLin1_PntA[1].X > inLin1_PntA[0].X) {
			Lin1_minX = inLin1_PntA[0].X;
			Lin1_maxX = inLin1_PntA[1].X;
		}
		else {
			Lin1_minX = inLin1_PntA[1].X;
			Lin1_maxX = inLin1_PntA[0].X;
		}
	}
	// ��2б��
	if (inLin2_PntA[1].X == inLin2_PntA[0].X) {
		// ��������ƽ��?
		if (inLin2_PntA[1].Y == inLin2_PntA[0].Y) {
			// �����غ�
			return false;
		}
		else {
			// ������X=inLin2_PntA[0].X
			IsLin2_X = true;
			Lin2_minY = inLin2_PntA[1].Y > inLin2_PntA[0].Y ? inLin2_PntA[0].Y : inLin2_PntA[1].Y;
			Lin2_maxY = inLin2_PntA[1].Y < inLin2_PntA[0].Y ? inLin2_PntA[0].Y : inLin2_PntA[1].Y;
		}
	}
	else {
		Lin2_K = (inLin2_PntA[1].Y - inLin2_PntA[0].Y) / (inLin2_PntA[1].X - inLin2_PntA[0].X);
		// ȡ����2��x��Ч����
		if (inLin2_PntA[1].X > inLin2_PntA[0].X) {
			Lin2_minX = inLin2_PntA[0].X;
			Lin2_maxX = inLin2_PntA[1].X;
		}
		else {
			Lin2_minX = inLin2_PntA[1].X;
			Lin2_maxX = inLin2_PntA[0].X;
		}
	}

	//###### (2)���㽻�� ######
	// case1: ���߶�Y��ƽ��
	if (IsLin1_X || IsLin2_X) {
		crossFlg = 2;
		if (IsLin1_X && IsLin2_X) {
			if (inLin1_PntA[0].X == inLin2_PntA[0].X) {
				// �غ�
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
		// case2: �ж��Ƿ�ƽ��
		Lin1_B = inLin1_PntA[0].Y - inLin1_PntA[0].X*Lin1_K;
		Lin2_B = inLin2_PntA[0].Y - inLin2_PntA[0].X*Lin2_K;
		if (Lin1_K == Lin2_K) {
			crossFlg = 0;
			// �ж��Ƿ��غϣ�(k1 == k2) && (b1 == b2)ʱ���غ�
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
			// b1 == b2ʱ�����߶ν���Ϊ(0,b)
			if (Lin1_B == Lin2_B) {
				crsPnt.X = 0.0;
				crsPnt.Y = Lin1_B;
			}
			else {
				// ���� x=(b2-b1)/(k1-k2)
				crsPnt.X = (Lin2_B - Lin1_B) / (Lin1_K - Lin2_K);
				crsPnt.Y = Lin1_K*crsPnt.X + Lin1_B;
			}
		}
	}

	//###### (3)�жϽ�����Ч�� ######
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