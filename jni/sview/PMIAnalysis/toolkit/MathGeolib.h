/**********************************************************
*		Created on 2014/01/20 By gongcy
*	All Rights Reserved. Copyright(C) 2014 Sinovation.com
*
*	Function:
*		数学库函数
*	更新：
		
**********************************************************/


#ifndef _MATH_GEOLIB_INCLUDE_H_
#define _MATH_GEOLIB_INCLUDE_H_
#include "sview/pmiAnalysis/PMIDef.h"

class SVIEWCOMMON_API CMathGeolib
{
public:
	CMathGeolib(void);
	virtual ~CMathGeolib(void);
public:
	/*
	*求指定点对指定无限直线的垂足
	*/
	static int CalPedalPntToLine( int	dim , double	pnt[3] , int	dimlin , double	pntln[3] , double	dirln[3] , double	told , double	tola,
		double	hpnt[3], double	*hprm_p, double	*hdst_p  ); 


	/*
	*求指定点到指定线段的垂足
	*/
	static int CalPedalPntToSegment(int	dim,
		double	pnt[3],		
		int	dimln,			
		double	spntln[3],	
		double	epntln[3],	
		double	told,		
		double	tola,		
		double	hpnt[3],	
		double	*hprm_p,	
		double	*hdst_p	);
	/*
	*点到线有效范围判断
	*/
	static int	IsPntInSegment1( 
		int	dim,			
		double	pnt[3],		
		int	dimln,			
		double	lin_sp[3],	
		double	lin_ep[3],	
		double	told,		
		double	tola,		
		double	hpnt[3],	
		double	*hprm_p,	
		double	*hdst_p);

	/*
	*计算两无限直线的最小距离
	*/
	static int	Cal2LineDis(int nstdim, int dim1, double pnt1[3], double dir1[3], int dim2, double pnt2[3], double dir2[3], double told, double tola, double nstinf[9]);
	
	/*
	*计算两无限直线的交点
	*/
	static int	Cal2LineIntersection( int xdim, int dim1, double pnt1[3], double dir1[3], 
		int dim2, double pnt2[3], double dir2[3], 
		double told, double tola, double xpinf[9] );
	/*
	求无限直线与无限平面的交点。
	*/
	static int	CalLineToPlaneIntersection(double lin_pnt[3], double lin_dir[3], double pln_pnt[3], double pln_nrm[3], double told, double tola, double xpnt[3], double *xprm_p);

	/*
		求点到无限平面的垂点及垂向量
	*/
	static int CalPedalPntToPlane( double pnt[3], double plpnt[3], double plnrm[3], double told, double hpnt[3], double *hdst_p );

	/*
	*点到线有效范围判断
	*/
	static int	IsPntInSegment(int dim, double prm, double lin_sp[3], double lin_ep[3], double *oprm_p);

	/*
	*求两点的距离。
	*/
	static int	Cal2PntDis(int dim, double point1[3], double point2[3], double *distance_p);

	/*
	*二元一次方程求解。
	*/
	static int	BinaryLinearEquation(double equ[2][3], double solv[2]);

	/*
	*计算点沿指定直线移动指定距离后的点。
	*/
	static int	CalNewPntByDirAndDis(int dim, double lin_pnt[3], double lin_dir[3], double pnt1[3], double dist, double pnt2[3], double *prm2_p);
	//计算点沿着向量移动一定的距离后的点
	static int  movPnt(int dim, /*VECTOR*/double *ilnvecP, double/*POINT3*/ *pntP, double ilndst, double/*POINT3*/ *movpntP);
/****************************************************************************
 *
 *	@brief	     [根据两要素信息获取角度尺寸延长线与特殊线的点列]
 *	@param[in]	  [输入参数功能描述]
 *	@param[out]	  [输出参数功能描述]
 *	@retval		 [返回值说明]
 *	@note        [详细说明]	
 *	@attention   [注意事项说明]
 *	@history add by yuting 2015.9.16
****************************************************************************/
	static int GetAngleDimData(
		double i_Element1[2][3],	/* 要素1的始终点 */
		double i_Element2[2][3],	/* 要素2的始终点 */
		double i_prjPlnpnt[3],		/* PMI平面的拾取点的坐标值 */
		int &o_ExtendLineFlag,		/* 延长线的数量 */
		double o_ExtendLine[4][3],	/* 延长线点列 */
		double &o_ComTexValue,
		double o_dInnterPnt[3]);

	static int GetAngleDimSpecialLineData(
		double i_Element1[2][3],			// 要素1的始终点
		double i_Element2[2][3],			// 要素2的始终点
		int &o_SpecialLineFlag,				// 特殊线标志 = 0 没有特殊线
		// = 1有一条特殊线且是第一条;
		// = 2有一条特殊线且是第二条；
		// = 3有两条特殊线
		double o_SpecialLine[4][3]			// 特殊线点列
	);
};
#endif
