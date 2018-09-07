#ifndef PMIESYMBOL_H_
#define PMIESYMBOL_H_

#include "sview/pmiAnalysis/PMIDef.h"

#include <cmath>
#include <vector>
/*
 * PMI末端记号形状
 */
#define			TERM_PATSMNO			0	/* 无 */			//TerminatorTypeNone 
#define			TERM_PATSMARW		1	/* 箭头 */			//TerminatorTypeOpenArrow 
#define			TERM_PATSMWHTARW		2	/* 白色箭头 */		//TerminatorTypeClosedArrow 
#define			TERM_PATSMBLKARW		3	/* 黑色箭头 */		//TerminatorTypeFillArrow 
#define         PATCROSSARW		4						//TerminatorTypeCrossArrow
#define			TERM_PATSMWHTCIR		5	/* 空心点 */		//TerminatorTypeCircle 
#define			TERM_PATSMBLKCIR		6   /*实心点*/  /*TerminatorTypeFilledCircle*/
#define			TERM_PATSMWHTSQR		7  /* 白色矩形*/		//TerminatorTypeSquare 
#define			TERM_PATSMBLKSQR		8  /* 填充矩形*/		//TerminatorTypeFilledSquare 
#define			TERM_PATSMSLASH_TYPE		9	/* 斜线 */			//TerminatorTypeSlash
#define         TERM_PATSMCROSSEDCIR 10						//TerminatorTypeCrossedCircle 											//TerminatorTypeCrossedCircle
#define			TERM_PATSMDBXCIRCLE		11  /*             TerminatorTypeXCircle */
														//
#define			TERM_PATSMWHTTRI3	12  /* 白三角2*/		//TerminatorTypeTriangle 
#define			TERM_PATSMBLKTRI2	13	/* 黑三角2*/		//TerminatorTypeFilledTriangle 
#define			TERM_PATSMCRSBIG		14	/* 大十字线*/		//TerminatorTypePlus 
#define			TERM_PATSMCRSX		15  /* X形十字线*/		//TerminatorTypeXCross 
//#define         TERM_PATSMINTEGRAL   16						//TerminatorTypeIntegral 
#define 		TERM_PATSMCIRCENT	17						//TerminatorTypeCircleCenter 

#define			TERM_PATSMDBOPNARW		18  /* 双开白箭头：TerminatorTypeDoubleOpenArrow */
#define			TERM_PATSMDBCLSARW		19  /* 双闭白箭头：TerminatorTypeDoubleCloseArrow */
#define			TERM_PATSMDBFILLARW		20  /* 双闭黑箭头：TerminatorTypeDoubleFillArrow  */
#define			TERM_PATSMDBFILLTRI		21  /* 双开黑三角：TerminatorTypeDoubleTriangle   */


#define			TERM_PATSMWHTTRI_TYPE		22	/* 白三角 */
#define			TERM_PATSMBLKTRI_TYPE		23	/* 黑三角 */
#define			PATSSPACELIN_TYPE 24  /* 直线*/


#define			TERM_PATSMWHTTRI2_TYPE	 25	/* 没有底边三角 */

#define			TERM_PATSMUSR		100	/* 定义 */

#define	PI		3.14159265358979323846
#define  USERNOTEIDSTART  500000

typedef 	struct	_point	{		/* 3D Point Type	*/
	double		x;		/* X Coordinate		*/
	double		y;		/* Y Coordinate		*/
	double		z;		/* Z Coordinate		*/
} POINT3;

typedef std::vector<POINT3>		CPOINT3Array;

class SVIEWCOMMON_API CPMIESymbol
{
public:
	CPMIESymbol(void);
	~CPMIESymbol(void);
	
public:
	
	int SetESymbolInfo(double fHight, double fWidth, double* arrowpnt, double* arrowdir,double *pnormal, int symtype, CPOINT3Array &outPnts1,CPOINT3Array &outPnts2);
};

#endif  //PMIESYMBOL_H_
