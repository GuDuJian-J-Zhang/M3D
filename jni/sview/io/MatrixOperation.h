// MatrixOperation.cpp
#ifndef _MATRIXOPERATION_H
#define _MATRIXOPERATION_H
// 包含矩阵转置、求逆、相乘几个通用操作
// 武汉大学.遥感信息工程学院.卢昊

class MatrixOperation
{
public:
	static void MatrixTranspose(float* MatrixOrigin, float* MatrixNew, int m, int n);
	static void TransformPoint(float fMtxAA[4][4], float fPoint[3]);
	static void MakeIdentity(float dMtxAA[4][4]);
};

#endif
