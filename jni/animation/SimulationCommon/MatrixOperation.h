// MatrixOperation.cpp
#ifndef _MATRIXOPERATION_H
#define _MATRIXOPERATION_H
// 包含矩阵转置、求逆、相乘几个通用操作
// 武汉大学.遥感信息工程学院.卢昊
#include "SATools.h"
class SA_API MatrixOperation
{
public:
	//////////////////////////////////////////////////////////////////////////
	// 矩阵转置
	// 参数说明：原始矩阵MatrixOrigin为m×n
	static void MatrixTranspose(double* MatrixOrigin, double* MatrixNew, int m = 4, int n = 4);
	static void MatrixTranspose(float* MatrixOrigin, float* MatrixNew, int m = 4, int n = 4);


	//////////////////////////////////////////////////////////////////////////
	// 矩阵求逆
	// 参数说明：m：原始矩阵Matrix的行/列数
	static void MatrixInversion(double* Matrix, int m = 4);
	static void MatrixInversion(float* Matrix, int m = 4);
	
	//////////////////////////////////////////////////////////////////////////
	// 矩阵相乘
	// 参数说明：C=A×B，ARow：A的行数，AColumn：A的列数，BColumn：B的列数
	static void MatrixMultiply(double* MatrixA, double* MatrixB, double* MatrixC, int ARow, int AColumn, int BColumn);
	static void MatrixMultiply(float* MatrixA, float* MatrixB, float* MatrixC, int ARow, int AColumn, int BColumn);

	//////////////////////////////////////////////////////////////////////////
	// 矩阵相加
	// 参数说明
	static void MatrixAdd(double* MatrixA, double* MatrixB, double* MatrixC, int Row, int Column);
	static void MatrixAdd(float* MatrixA, float* MatrixB, float* MatrixC, int Row, int Column);

	static void MatrixAdd(double* MatrixA, double* MatrixB, double* MatrixC, int length);
	static void MatrixAdd(float* MatrixA, float* MatrixB, float* MatrixC, int length);

	static void MatrixCopy(double* MatrixA, double* MatrixB, int length);
	static void MatrixCopy(float* MatrixA, float* MatrixB, int length);

	static void Multiply(double *MOrigin1, double *MOrigin2, double *MResult, int m = 4, int n = 4, int l = 4);
	static void Multiply(float *MOrigin1, float *MOrigin2, float *MResult, int m = 4, int n = 4, int l = 4);
	static void Multiply(float MOrigin1[4][4], float MOrigin2[4][4], float MResult[4][4]);

	static void MultDirMatrix(const float * MatrixA,float* srcDir, float* dstDir);
	static void MultDirMatrix(const double* MatrixA,double* srcDir, double* dstDir);

	static void DoubleMtxToFloatMtx(double dMtxAA[4][4],float fMtxAA[4][4]);
	static void FloatMtxToDoubleMtx(float fMtxAA[4][4],double dMtxAA[4][4]);

	static void TransformPoint( double dMtxAA[4][4], double dPoint[3]);
	static void TransformPoint( float fMtxAA[4][4], float fPoint[3]);

	//矩阵沿指定旋转点与单位矩阵的互转
	static void MtxTanslationToUniTanslation(float fPivot[3], float fMtxAA[4][4], float fPos[3]);
	static void UniTanslationToMtxTanslation(float fPivot[3], float fMtxAA[4][4], float fPos[3]);

	static void MtxTanslationToUniTanslation(double fPivot[3], double fMtxAA[4][4], double fPos[3]);
	static void UniTanslationToMtxTanslation(double fPivot[3], double fMtxAA[4][4], double fPos[3]);

	static void MakeIdentity(double dMtxAA[4][4]);
	static void MakeIdentity(float dMtxAA[4][4]);

	static void FromQuat(float quat[4],float* fMatrix);
	
	static void setRotate(float fMtxAA[4][4],float fRotate[4]);
	static void setScale(float fMtxAA[4][4],float fScale[3]);
	static void setTranslate(float fMtxAA[4][4],float fPos[3]);
	static void setTransform(float fMtxAA[4][4],float fPos[3], float fRotate[4], float fScale[3]);

	static void getTransform(float fMtxAA[4][4],float fPos[3], float fRotate[4], float fScale[3], float fSo[4]);

};

#endif
