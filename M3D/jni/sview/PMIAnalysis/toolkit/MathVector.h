/**********************************************************
*		Created on 2013/12/28 By gongcy
*
*	Function:
*		矢量数学库函数
*	更新：
		
**********************************************************/
#ifndef _MATH_VECTOR_INCLUDE_H_
#define _MATH_VECTOR_INCLUDE_H_

#include "sview/pmiAnalysis/PMIDef.h"

class SVIEWCOMMON_API CMathVector
{
public:
	CMathVector(void);
	virtual ~CMathVector(void);
public:
	/*
	*计算两向量之和
	*/
	static int Sum(int dim, double invt1[], double invt2[], double univt[]);

	/*
	*计算两向量之差
	*/
	static int Minus(int dim, double invt1[], double invt2[], double difvt[]);

	/*
	*计算两向量的内积
	*/
	static int Inner(int dim, double invt1[], double invt2[], double *inner_p);

	/*
	*计算两向量的外积
	*/
	static int Cross(double invt1[3], double invt2[3], double outer[3]);

	/*
	*计算向量的长度及单位向量
	*/
	static int LengthAndUnit(int dim, double invt[], double *length_p, double unitvt[]);

	/*
	*计算两向量的夹角
	*/
	static int Angle(int dim, double invt1[], double invt2[], double *rad_p);

	/*
	*计算向量的长度
	*/
	static int Length(int dim, double invt[], double *length_p);

	/*
	*计算两向量的外积的长度及外积的单位向量
	*/
	static int CrossLengthAndUnit(double invt1[3], double invt2[3], double *length_p, double nvct[3]);

	/*
	*计算两向量之差的长度及单位向量
	*/
	static int MinusLengthAndUnit(int dim, double invt1[], double invt2[], double *length_p, double unitvt[]);

	/*
	*计算两向量的夹角
	*/
	static int Angle(double invt1[2], double invt2[2], double *rad_p);

	/*
	*计算两向量沿指定参照向量的旋转角度
	*/
	static int Angle(double invt1[3], double invt2[3], double refvt[3], double *rad_p);


	/*
	判定2个单位矢量的平行性（双方向）
	*/
	static int ParallelBidirect(int dim, double unit1[], double unit2[], double tola);

	/*
	判定2个单位矢量的平行性（单方向）
	*/
	static int Parallel(int dim, double unit1[], double unit2[], double tola);

	/*
	判定2个单位矢量的垂直性
	*/
	static int Vertical(int dim, double unit1[], double unit2[], double tola);

	/*
	判定2个单位矢量的平行性（双方向）
	*/
	static int VerticalBidirect(int dim, double vect1[], double vect2[]);
};
#endif
