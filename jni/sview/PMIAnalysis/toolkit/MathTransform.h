/**********************************************************
*		Created on 2013/12/26 By gongcy
*	All Rights Reserved. Copyright(C) 2013 Sinovation.com
*
*	Function:
*		矩阵变换库函数
*	更新：
		
**********************************************************/
#ifndef _MATH_TRANSFORM_INCLUDE_H_
#define _MATH_TRANSFORM_INCLUDE_H_

#ifdef __MOBILE__
#define SVIEWCOMMON_API
#else

#ifdef SVIEWCOMMON_EXPORTS
#define SVIEWCOMMON_API __declspec(dllexport)
#else
#define SVIEWCOMMON_API __declspec(dllimport)
#endif

#endif

class SVIEWCOMMON_API  CMathTransform
{
public:
	CMathTransform(void);
	virtual ~CMathTransform(void);
public:
	/*
	*根据旋转点，旋转方向及旋转角度求旋转矩阵
	*/
	static int	CalMatrixByPntDirAndAngle(int dim, double point[3], double nvec[3], double rad, double matrix[4][4], double matinv[4][4]);
	
	/*
	*计算两矩阵的偏移矩阵
	*/
	static int	CalOffsetMatrix(int dim, double e1[3], double e2[3], double e3[3], double p1[3], double f1[3], double f2[3], double f3[3], double p2[3], double matrix[4][4], double matinv[4][4]);
	
	/*
	*根据原点、X轴、Z轴，创建矩阵
	*/
	static int	CalMatrixByPntXZAxis (double point[3], double nvec[3], double base_x[3], double matrix[4][4], double matinv[4][4]);
	
	/*
	*计算一个矩阵相对于另一个矩阵的偏移矩阵
	*/
	static int	CalMatrixOffset(int dim, double e1[3], double e2[3], double e3[3], double p1[3], double f1[3], double f2[3], double f3[3], double p2[3], double matrix[4][4], double matinv[4][4]);
	
	/*
	*将点的坐标值转换为相对于指定矩阵的坐标值
	*/
	static int	WordPntToLocalPnt(int dim, int num_pnti, int rec_size, double pnti[], double t_mtr[4][4], double pnto[]);
	
	/*
	*将向量转换为相对于指定矩阵的向量
	*/
	static int	WordDirToLocalDir(int dim, int num_vct, int rec_size, double vcti[], double t_mtr[4][4], double vcto[]);
};
#endif
