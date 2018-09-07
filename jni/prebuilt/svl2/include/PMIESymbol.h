#pragma once

//ISSUE:PMIESymbol for android
//CArray => vector
#include <vector>
#include <jni.h>
using std::vector;

//============================

#include <cmath>
/*
 * PMI末端记号形状
 */
#define			PATSMNO			0	/* 无 */			//TerminatorTypeNone 
#define			PATSMARW		1	/* 箭头 */			//TerminatorTypeOpenArrow 
 
#define			PATSMBLKARW		3	/* 黑色箭头 */		//TerminatorTypeFillArrow 
#define			PATSMWHTARW		4	/* 白色箭头 */		//TerminatorTypeClosedArrow
#define			PATSMWHTCIR		5	/* 空心点 */		//TerminatorTypeCircle 
#define			PATSMBLKCIR		6	/* 实心点*/			//TerminatorTypeFilledCircle 
#define			PATSMWHTSQR		7  /* 白色矩形*/		//TerminatorTypeSquare 
#define			PATSMBLKSQR		8  /* 填充矩形*/		//TerminatorTypeFilledSquare 
#define			PATSMSLASH		9	/* 斜线 */			//TerminatorTypeSlash 
#define         PATSMCROSSEDCIR 10						//TerminatorTypeCrossedCircle 											//TerminatorTypeCrossedCircle 
#define			PATSMDBXCIRCLE		11  /*             TerminatorTypeXCircle */
														//
#define			PATSMWHTTRI3	12  /* 白三角2*/		//TerminatorTypeTriangle 
#define			PATSMBLKTRI2	13	/* 黑三角2*/		//TerminatorTypeFilledTriangle 
#define			PATSMCRSBIG		14	/* 大十字线*/		//TerminatorTypePlus 
#define			PATSMCRSX		15  /* X形十字线*/		//TerminatorTypeXCross 
//#define         PATSMINTEGRAL   16						//TerminatorTypeIntegral 
#define 		PATSMCIRCENT	17						//TerminatorTypeCircleCenter 

#define			PATSMDBOPNARW		18  /* 双开白箭头：TerminatorTypeDoubleOpenArrow */
#define			PATSMDBCLSARW		19  /* 双闭白箭头：TerminatorTypeDoubleCloseArrow */
#define			PATSMDBFILLARW		20  /* 双闭黑箭头：TerminatorTypeDoubleFillArrow  */
#define			PATSMDBFILLTRI		21  /* 双开黑三角：TerminatorTypeDoubleTriangle   */


#define			PATSMWHTTRI		22	/* 白三角 */
#define			PATSMBLKTRI		23	/* 黑三角 */
#define			PATSSPACELIN	24  /* 直线*/


#define			PATSMWHTTRI2	25	/* 没有底边三角 */

#define			PATSMUSR		100	/* 定义 */

#define	PI		3.14159265358979323846

//for pmiESymbol 版本不同 bing 20140121
#define 	PATSMDOUBLEFILLARROW PATSMDBFILLARW
#define 	PATSMDOUBLETRIANGLE PATSMDBFILLTRI


#ifdef PMICMD_DLL_
#define PMI_DLL_EXPORT  JNIEXPORT//__declspec(dllexport)
#else
#define PMI_DLL_EXPORT  JNIIMPORT //__declspec(dllimport)
#endif
typedef	struct	_point	{		/* 3D Point Type	*/
	double	x;		/* X Coordinate		*/
	double	y;		/* Y Coordinate		*/
	double	z;		/* Z Coordinate		*/
} POINT3;
//typedef CArray<POINT3, POINT3>		CPOINT3Array;
typedef vector<POINT3>		CPOINT3Array;

class PMI_DLL_EXPORT CPMIESymbol
{
public:
	CPMIESymbol(void);
	~CPMIESymbol(void);
	
public:
	
	int SetESymbolInfo(double fHight, double fWidth, double* Pnt1, double* Pnt2, float* NormalVec, int symtype, CPOINT3Array&outPnts1,CPOINT3Array&outPnts2);
};
