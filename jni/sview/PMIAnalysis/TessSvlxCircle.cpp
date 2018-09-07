//update by gongcy on 2014/04/19
//��ӽӿڣ�
//1.GetArcAngleData
//2.GetCircleDrawData

//update by gongcy on 2014/08/11
//��ӽӿڣ�RectifyArcData
//��ʱ����Ƕȳߴ����ߵ����⣬�����Ƕ����ߵĲ���

#include "TessSvlxCircle.h"
#include "toolkit/MathVector.h"
#include "toolkit/MathGeolib.h"
//#include "PMIUtilityLib.h"
//#include <Inventor/SbLine.h>
#include <math.h>
#include<stdlib.h>
#include<wchar.h>


#define IDXNUM			(16384)
#define ZDEPS			(1.0e-18)
#define MAXPNTVAL		(1.0e+8)
#define ZTOL			(2e-10)		// ���湫��
#define ANGLEPARA_TOL	(0.1)	// �ǶȲ�������
#define ARAAYNUM		(4)
#define PMIDIM2			(2)			// ��ά
#define DIVANGLE		(2)			// ��ɢ��С�Ƕ�
#define	PI		(3.14159265358979323846)


using namespace std;
using namespace HoteamSoft::SVLLib;

CTessSvlxCircle::CTessSvlxCircle(void)
{

}
CTessSvlxCircle::~CTessSvlxCircle(void)
{

}

/*
* ���ܣ���ȡԲ����ɢ����

* ������

*       ellipse               (I) Բ����

*       drawFlg               (I) Բ�����Ʒ���

*       outPnt[2]             (O) ��Ԫ��ĵ�������

* ����ֵ�� ִ�гɹ�/ʧ��

* 2013-09-05   Liutp   created
*/
bool CTessSvlxCircle::GetCircleDiscretization(
	Stk_ArcPtr          arc,	/*(i) Բ����					*/
	HoteamSoft::SVLLib::STK_INT32			drawFlg,	/*(i) Բ�����Ʒ���				*/
									/*  ==  1 ��ʱ��				*/
									/*  == -1 ˳ʱ��				*/
	vector<HoteamSoft::SVLLib::STK_PNT32>	&outPnt		/*(o) ��Ԫ��ĵ�������			*/
) {
	HoteamSoft::SVLLib::STK_FLOAT32			MajorRadius, MinorRadius;	// �̰뾶�����뾶
	HoteamSoft::SVLLib::STK_PNT32			Center;						// ���ĵ�
	HoteamSoft::SVLLib::STK_PNT32			BeginPnt, EndPnt;			// ��ʼ������ֵ
	HoteamSoft::SVLLib::STK_DIR32			inNormal;					// Z�����������ж�Բ�����Ʒ���
	STK_DOUBLE64		BeginPara, EndPara;			// ��ʼ��/�յ�ĽǶȲ���
	STK_INT32			neqFlg;						// ���Ʒ���

	arc->GetRadius(MajorRadius, MinorRadius);
	Center = arc->GetCenterPoint();
	arc->GetCoordinatePnt(BeginPnt, EndPnt);
	inNormal = arc->GetNormal();

	/*
	*	���ղ������̼���ǶȲ���
	*/
	GetCircleAngleParameter(PMIDIM2, BeginPnt, Center, MinorRadius, BeginPara);
	GetCircleAngleParameter(PMIDIM2, EndPnt, Center, MinorRadius, EndPara);

	/*
	*	���սǶȲ�����ʵ�ʽǶȽ���Բ�����߻�����
	*/
	neqFlg = drawFlg == 0 ? (inNormal.Z > 0.0 ? +1 : -1) : drawFlg;

	return MakeArcDiscretization(Center, MinorRadius, BeginPara, EndPara, 0.0, neqFlg, outPnt);

}

/*
* ���ܣ�Բ����ɢ��

* ������

*       ellipse               (i) Բ����

*       beginPara,            (i) Բ�����Ƶ���ʼ�ǶȲ���

*       endPara,              (i) Բ�����Ƶ���ֹ�ǶȲ���

*       arcAngle,             (i) Բ��ʵ�ʽǶ�

*       drawFlg               (I) Բ�����Ʒ���

*       outPnt                (o) ��Ԫ��ĵ�������

* ����ֵ�� ִ�гɹ�/ʧ��

* 2013-09-05   Liutp   created
*/
bool CTessSvlxCircle::GetCircleDiscretization(
	Stk_ArcPtr			arc,	/*(i) Բ����					*/
	STK_DOUBLE64		beginPara,	/*(i) Բ�����Ƶ���ʼ�ǶȲ���	*/
	STK_DOUBLE64		endPara,	/*(i) Բ�����Ƶ���ֹ�ǶȲ���	*/
	STK_DOUBLE64		arcAngle,	/*(i) Բ��ʵ�ʽǶ�				*/
	STK_INT32			drawFlg,	/*(i) Բ�����Ʒ���				*/
									/*  ==  1 ��ʱ��				*/
									/*  == -1 ˳ʱ��				*/
	vector<STK_PNT32>	&outPnt		/*(o) ��Ԫ��ĵ�������			*/
) {
	STK_FLOAT32			MajorRadius, MinorRadius;	// �̰뾶�����뾶
	STK_PNT32			Center;						// ���ĵ�
	STK_DIR32			inNormal;					// Z�����������ж�Բ�����Ʒ���
	STK_INT32			neqFlg;						// ���Ʒ���

	arc->GetRadius(MajorRadius, MinorRadius);
	Center = arc->GetCenterPoint();
	inNormal = arc->GetNormal();

	// ��ȡ���Ʒ���
	neqFlg = drawFlg == 0 ? (inNormal.Z > 0.0 ? +1 : -1) : drawFlg;

	/*
	*	���ջ��Ʒ������������
	*/
	return MakeArcDiscretization(Center, MinorRadius, beginPara, endPara, arcAngle, neqFlg, outPnt);

}

STK_BOOL CTessSvlxCircle::GetCircleAngleParameter(
	HoteamSoft::SVLLib::STK_UINT32 dim,
	HoteamSoft::SVLLib::STK_PNT32 arcPnt,
	HoteamSoft::SVLLib::STK_PNT32 arcCenter,
	HoteamSoft::SVLLib::STK_FLOAT32 arcRadius,
	HoteamSoft::SVLLib::STK_DOUBLE64 &AnglePara
)
{
	STK_DOUBLE64		cosAngle;

	if (dim != PMIDIM2) {
		return false;
	}

	/*
	*	����cos��ֵ
	*/
	cosAngle = (arcPnt.X - arcCenter.X) / arcRadius;

	if (cosAngle > 1) {
		if ((cosAngle - 1) < ANGLEPARA_TOL) {
			// 0��ʱ�趨Ϊ360��
			AnglePara = PI * 2;
			return true;
		}
		else {
			return false;
		}
	}
	else if (cosAngle < -1) {
		if (fabs(1 + cosAngle) < ANGLEPARA_TOL) {
			// cos��<-1ʱ�����Χ����Ϊ180��
			AnglePara = PI;
			return true;
		}
		else {
			return false;
		}
	}

	/*
	*	�ǶȲ�������arccos��(�������޽������⴦��)
	*/
	if ((arcPnt.X - arcCenter.X) < 0.0 && (arcPnt.Y - arcCenter.Y) < 0.0 ||
		(arcPnt.X - arcCenter.X) > 0.0 && (arcPnt.Y - arcCenter.Y) < 0.0) {
		AnglePara = PI * 2 - acos(cosAngle);
	}
	else if ((arcPnt.X - arcCenter.X == 0.0) && (arcPnt.Y - arcCenter.Y) < 0.0) {
		AnglePara = PI + acos(cosAngle);
	}
	else {
		AnglePara = acos(cosAngle);
	}

	return true;
}

/*
* ���ܣ�Բ����ɢ��

* ������

*       arcCenter,            (i) Բ�ĵ�����

*       arcRadius,            (i) Բ���뾶

*       beginPara,            (i) Բ�����Ƶ���ʼ�ǶȲ���

*       endPara,              (i) Բ�����Ƶ���ֹ�ǶȲ���

*       arcAngle,             (i) Բ��ʵ�ʽǶ�

*       drawFlg               (i) Բ�����Ʒ���

*       outPnt                (o) ��Ԫ��ĵ�������

* ����ֵ�� ִ�гɹ�/ʧ��

* 2013-09-09   Liutp   created
*/
bool CTessSvlxCircle::MakeArcDiscretization(
	HoteamSoft::SVLLib::STK_PNT32 arcCenter,      /*(i) Բ�ĵ�����				*/
	HoteamSoft::SVLLib::STK_FLOAT32 arcRadius,    /*(i) Բ���뾶					*/
	HoteamSoft::SVLLib::STK_DOUBLE64 BeginPara,   /*(i) Բ�����Ƶ���ʼ�ǶȲ���	*/
	HoteamSoft::SVLLib::STK_DOUBLE64 EndPara,     /*(i) Բ�����Ƶ���ֹ�ǶȲ���	*/
	HoteamSoft::SVLLib::STK_DOUBLE64 arcAngle,    /*(i) Բ��ʵ�ʽǶ�				*/
	HoteamSoft::SVLLib::STK_INT32 neqFlg,         /*(i) Բ�����Ʒ���				*/
											      /*  ==  1 ��ʱ��				*/
											      /*  == -1 ˳ʱ��				*/
	std::vector<HoteamSoft::SVLLib::STK_PNT32> &outPnt)   /*(o) ��Ԫ��ĵ�������			*/
{
	STK_PNT32		cpnt;						// Բ���ϵĵ�
	STK_DOUBLE64	pointpar, tmPntPar;
	STK_DOUBLE64	tangleP = 0.0;

	// ���㻡�� ͨ���ǶȲ������㻡��
	if (arcAngle > 0.0) {
		tangleP = arcAngle;
	}
	else {
		// ���㻡��
		if (EndPara > BeginPara) {
			if (neqFlg > 0) {
				tangleP = EndPara - BeginPara;
			}
			else {
				tangleP = (PI * 2 - EndPara) + BeginPara;
			}
		}
		else if (EndPara < BeginPara) {
			if (neqFlg > 0) {
				tangleP = (PI * 2 - BeginPara) + EndPara;
			}
			else {
				tangleP = BeginPara - EndPara;
			}
		}
		else if (EndPara == BeginPara) {
			tangleP = PI * 2;
		}
	}

	//�޸����ýǶ�10��->5��Ϊ��Ԫ����		by Liutp on 2013.09.04
	STK_DOUBLE64 DisTangle = DIVANGLE*(PI / 180);
	STK_INT32    dotpntnum = (STK_INT32)(tangleP / DisTangle);

	if (dotpntnum < 4) {
		dotpntnum = 4;
		DisTangle = tangleP / 4;
	}

	// ���ջ��Ʒ������Բ��
	for (int j = 0; j < dotpntnum + 1; j++) {
		pointpar = BeginPara + neqFlg*DisTangle*j;
		if (pointpar < 0.0) {
			tmPntPar = PI * 2 + pointpar;
		}
		else if (pointpar > PI * 2) {
			tmPntPar = pointpar - PI * 2;
		}
		else {
			tmPntPar = pointpar;
		}
		// �յ㴦��
		if (j == dotpntnum) {
			tmPntPar = EndPara;
		}
		cpnt.X = (STK_FLOAT32)(arcCenter.X + arcRadius*cos(tmPntPar));
		cpnt.Y = (STK_FLOAT32)(arcCenter.Y + arcRadius*sin(tmPntPar));
		cpnt.Z = 0.0;
		outPnt.push_back(cpnt);
	}

	return true;
}

/*
* ���ܣ�����Բ�����Ʒ���

* ������

*       leaders               (i) PMI����������

*       vecBeginPar           (o) Բ�����Ƶ���ʼ�ǶȲ���

*       vecEndPar             (o) Բ�����Ƶ���ֹ�ǶȲ���

*       drawAngle             (o) Բ�����ƽǶ�

*       drawFlg               (o) Բ�����Ʒ���

* ����ֵ�� ִ�гɹ�/ʧ��

* 2013-09-05   Liutp   created
*/
STK_BOOL CTessSvlxCircle::GetCircleDrawData(
	vector<Stk_LeaderPtr>		leaders,		/*(i) PMI����������				*/
	vector<STK_DOUBLE64>	&vecBeginPar,	/*(o) Բ�����Ƶ���ʼ�ǶȲ���	*/
	vector<STK_DOUBLE64>	&vecEndPar,		/*(o) Բ�����Ƶ���ֹ�ǶȲ���	*/
	STK_DOUBLE64			&drawAngle,		/*(o) Բ�����ƽǶ�				*/
	vector<STK_INT32>		&drawFlg		/*(o) Բ�����Ʒ���				*/
											/*  ==  1 ��ʱ��				*/
											/*  == -1 ˳ʱ��				*/
) {
	Stk_ArcPtr          arc;                        // Բ����
	//Stk_Ellipse			*ellipse;					// Բ����
	STK_PNT32			BeginPnt, EndPnt;			// ��ʼ������ֵ
	STK_FLOAT32			MajorRadius, MinorRadius;	// �̰뾶�����뾶
	STK_PNT32			Center;						// ���ĵ�
	STK_DIR32			inNormal;					// Z�����������ж�Բ�����Ʒ���
	STK_DOUBLE64		BeginPara, EndPara;			// ��ʼ��/�յ�ĽǶȲ���
	vector<STK_FLOAT32>	inNormal_Z;

	/*
	*	���ղ������̼����Բ���ĽǶȲ���
	*/
	for (int i = 0; i<leaders.size(); i++) {
		Stk_LeaderPtr leader = leaders.at(i);		//���һ������������
		STK_UINT32 type = leader->GetLeaderType();	//�������������

		if (type == 2) {
			vector<Stk_CurveEntityPtr> CurveList = leader->GetAllCurve();
			if (CurveList.size() == 0)
			{
				return false;
			}
			Stk_CurveEntityPtr pCurve = CurveList[CurveList.size() - 1];
			arc = Stk_ArcPtr::DynamicCast(pCurve);

			arc->GetRadius(MajorRadius, MinorRadius);
			arc->GetCoordinatePnt(BeginPnt, EndPnt);
			Center = arc->GetCenterPoint();
			inNormal = arc->GetNormal();

			GetCircleAngleParameter(PMIDIM2, BeginPnt, Center, MinorRadius, BeginPara);
			GetCircleAngleParameter(PMIDIM2, EndPnt, Center, MinorRadius, EndPara);

			inNormal_Z.push_back(inNormal.Z);
			vecBeginPar.push_back(BeginPara);
			vecEndPar.push_back(EndPara);
		}
		else {
			return false;
		}
	}

	/*
	*	������Ʒ����ʵ�ʽǶ�
	*/
	return GetArcAngleData(vecBeginPar, vecEndPar, inNormal_Z, drawAngle, drawFlg);

}

/*
* ���ܣ�����Բ�����Ʒ���

* ������

*       vecBeginPar           (i) Բ�����Ƶ���ʼ�ǶȲ���

*       vecEndPar             (i) Բ�����Ƶ���ֹ�ǶȲ���

*       drawAngle             (o) Բ�����ƽǶ�

*       drawFlg               (o) Բ�����Ʒ���

* ����ֵ�� ִ�гɹ�/ʧ��

* 2013-09-09   Liutp   created
*/
STK_BOOL CTessSvlxCircle::GetArcAngleData(
	vector<STK_DOUBLE64>		vecBeginPar,	/*(i) Բ�����Ƶ���ʼ�ǶȲ���	*/
	vector<STK_DOUBLE64>		vecEndPar,		/*(i) Բ�����Ƶ���ֹ�ǶȲ���	*/
	vector<STK_FLOAT32>			inNormal_Z,		/*(i) Բ�����Ƶ���ֹ�ǶȲ���	*/
	STK_DOUBLE64				&drawAngle,		/*(o) Բ�����ƽǶ�				*/
	vector<STK_INT32>			&drawFlg		/*(o) Բ�����Ʒ���				*/
												/*  ==  1 ��ʱ��				*/
												/*  == -1 ˳ʱ��				*/
) {
	STK_DOUBLE64		EndPara;					// ��ʼ��/�յ�ĽǶȲ���
	STK_INT32			neqFlg;						// ���Ʒ���
	STK_INT32			neqFlgA[2];					// ���Ʒ���
	STK_DOUBLE64		RealTangle;
	STK_DOUBLE64		minPara, maxPara;
	STK_DOUBLE64		tangle1, tangle2;
	STK_BOOL			outFlg = false;

	if (vecBeginPar.size() != 2 ||
		vecEndPar.size() != 2 ||
		inNormal_Z.size() != 2) {
		return false;
	}

	/* ��1>��2ʱ��ȷ�����ƽǶ� */
	if (vecBeginPar.at(0) > vecBeginPar.at(1)) {
		minPara = vecBeginPar.at(1);
		maxPara = vecBeginPar.at(0);
		// ʵ�ʽǶȱ�﷽���2 -> ��1		ע������ʱ�뷽��Ϊ������ͬ
		if (inNormal_Z.at(0) < 0.0 && inNormal_Z.at(1) > 0.0) {
			// ��Ч����[��2,��1]��ʵ�ʽǶȦ�1-��2
			RealTangle = vecBeginPar.at(0) - vecBeginPar.at(1);
			for (int j = 0; j<vecEndPar.size(); j++) {
				EndPara = vecEndPar.at(j);
				neqFlg = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ((EndPara >= minPara) && (EndPara <= maxPara)) {
					// ����ԭ�������
					// neqFlg = inNormal_Z.at(j)>0.0 ? +1 : -1;
				}
				else {
					// ��ԭ������
					// neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = true;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
			// ʵ�ʽǶȱ�﷽���1 -> ��2
		}
		else if (inNormal_Z.at(0) > 0.0 && inNormal_Z.at(1) < 0.0) {
			// ��Ч����[��1,2��] �� [0,��2]��ʵ�ʽǶ�(2��-��1)+��2
			RealTangle = (PI * 2 - vecBeginPar.at(0)) + vecBeginPar.at(1);
			for (int j = 0; j<vecEndPar.size(); j++) {
				EndPara = vecEndPar.at(j);
				neqFlg = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ((EndPara >= maxPara) && (EndPara <= PI * 2) ||
					(EndPara >= 0.0) && (EndPara <= minPara)) {
					// ����ԭ�������
					// neqFlg = inNormal_Z.at(j);
				}
				else {
					// ��ԭ������
					//neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = true;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
		}
		else {
			return false;
		}
		// END���ڽǶ�����֮��ʱ���⴦��
		if (outFlg) {
			neqFlgA[0] = inNormal_Z.at(0)>0.0 ? +1 : -1;
			neqFlgA[1] = inNormal_Z.at(1)>0.0 ? +1 : -1;
			tangle1 = fabs(EndPara - vecBeginPar.at(0));
			tangle2 = fabs(EndPara - vecBeginPar.at(1));	// 
			if (EndPara > maxPara) {
				if (tangle1 + RealTangle / 2 < PI) {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : +1;
				}
				else {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : +1;
				}
			}
			else if (EndPara < minPara) {
				if (tangle2 + RealTangle / 2 < PI) {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : +1;
				}
				else {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : +1;
				}
			}
			drawFlg.clear();
			for (int k = 0; k<2; k++) {
				drawFlg.push_back(neqFlgA[k]);
			}
		}
		/* ��1<��2ʱ��ȷ�����ƽǶ� */
	}
	else if (vecBeginPar.at(0) < vecBeginPar.at(1)) {
		minPara = vecBeginPar.at(0);
		maxPara = vecBeginPar.at(1);
		// ʵ�ʽǶȱ�﷽���2 -> ��1��ʵ�ʽǶȦ�2-��1
		RealTangle = vecBeginPar.at(1) - vecBeginPar.at(0);
		if (inNormal_Z.at(0) > 0.0 && inNormal_Z.at(1) < 0.0) {
			// ��Ч����[��1,��2]
			for (int j = 0; j<vecEndPar.size(); j++) {
				EndPara = vecEndPar.at(j);
				neqFlg = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ((EndPara >= minPara) && (EndPara <= maxPara)) {
					// ����ԭ�������
					//neqFlg = inNormal_Z.at(j);
				}
				else {
					// ��ԭ������
					//neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = true;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
			// ʵ�ʽǶȱ�﷽���1 -> ��2
		}
		else if (inNormal_Z.at(0) < 0.0 && inNormal_Z.at(1) > 0.0) {
			// ��Ч����[��2,2��] �� [0,��1]��ʵ�ʽǶ�(2��-��2)+��1
			RealTangle = (PI * 2 - vecBeginPar.at(1)) + vecBeginPar.at(0);
			for (int j = 0; j<vecEndPar.size(); j++) {
				EndPara = vecEndPar.at(j);
				neqFlg = inNormal_Z.at(j)>0.0 ? +1 : -1;
				if ((EndPara >= maxPara) && (EndPara <= PI * 2) ||
					(EndPara >= 0.0) && (EndPara <= minPara)) {
					// ����ԭ�������
					//neqFlg = inNormal_Z.at(j);
				}
				else {
					// ��ԭ������
					//neqFlg = inNormal_Z.at(j)>0.0 ? -1 : +1;
					outFlg = true;
					break;
				}
				drawFlg.push_back(neqFlg);
			}
		}
		else {
			return false;
		}
		// END���ڽǶ�����֮��ʱ���⴦��
		if (outFlg) {
			neqFlgA[0] = inNormal_Z.at(0)>0.0 ? +1 : -1;
			neqFlgA[1] = inNormal_Z.at(1)>0.0 ? +1 : -1;
			tangle1 = fabs(EndPara - vecBeginPar.at(0));
			tangle2 = fabs(EndPara - vecBeginPar.at(1));	// 
			if (EndPara > maxPara) {
				if (tangle2 + RealTangle / 2 < PI) {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : +1;
				}
				else {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : +1;
				}
			}
			else if (EndPara < minPara) {
				if (tangle1 + RealTangle / 2 < PI) {
					neqFlgA[0] = inNormal_Z.at(0)>0.0 ? -1 : 1;
				}
				else {
					neqFlgA[1] = inNormal_Z.at(1)>0.0 ? -1 : 1;
				}
			}
			drawFlg.clear();
			for (int k = 0; k<2; k++) {
				drawFlg.push_back(neqFlgA[k]);
			}
		}
	}
	else {
		return false;
	}

	drawAngle = RealTangle;

	// ������
	return (!drawFlg.empty());

}
