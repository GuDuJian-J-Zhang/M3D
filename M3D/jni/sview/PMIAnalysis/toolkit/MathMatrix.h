/**********************************************************
*		Created on 2013/12/25 By gongcy
*	All Rights Reserved. Copyright(C) 2013 Sinovation.com
*
*	Function:
*		矩阵数学库函数
*	更新：
		
**********************************************************/
#ifndef _MATH_MATRIX_INCLUDE_H_
#define _MATH_MATRIX_INCLUDE_H_

#ifdef __MOBILE__
#define SVIEWCOMMON_API
#else

#ifdef SVIEWCOMMON_EXPORTS
#define SVIEWCOMMON_API __declspec(dllexport)
#else
#define SVIEWCOMMON_API __declspec(dllimport)
#endif

#endif

class SVIEWCOMMON_API CMathMatrix
{
public:
	CMathMatrix(void);
	virtual ~CMathMatrix(void);
public:
	/*
	*计算两矩阵之差
	*/
	static int	Minus(int di, int dj, double *inmx1_p, double *inmx2_p, double *difmx_p);
	
	/*
	*计算两矩阵之积
	*/
	static int  Multiply(int di1, int dj1, double *inmx1_p, int di2, int dj2, double *inmx2_p, double *intamx_p);
	
	/*
	*计算逆矩阵
	*/
	static int	Inverse(int dij, double *inmx_p, double *invmx_p);

	/*
	*计算矩阵的行列式
	*/
	static int	Determinant(int dij, double *inmx_p, double *det_p);

	/*
	*矩阵行列互换
	*/
	static int	Transpose(double matrix[4][4], double matinv[4][4]);
};
#endif
