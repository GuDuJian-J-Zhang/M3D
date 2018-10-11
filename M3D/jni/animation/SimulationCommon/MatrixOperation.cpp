// MatrixOperation.cpp
// 包含矩阵转置、求逆、相乘几个通用操作
// 武汉大学.遥感信息工程学院.卢昊
#include "MatrixOperation.h"
#include "math.h"
#include "string.h"

#define		ZNORMAL	(0)
#define		ZEROW	(-1)
#define		ZECOL	(-2)

#define		ZEROW1	(-1)
#define		ZECOL1	(-2)
#define		ZEROW2	(-3)
#define		ZECOL2	(-4)
#define		ZENOTEQ	(-5)



#define		ZEDIM	(-1) 	
#define		ZENOINV	(-2) 	
#define		ZEALLOC	(-10) 	


#define		EPS	(1.0E-18)

//////////////////////////////////////////////////////////////////////////
// 矩阵转置
// 参数说明：原始矩阵MatrixOrigin为m×n
void MatrixOperation::MatrixTranspose(double* MatrixOrigin, double* MatrixNew, int m, int n)
{
	int i;
	int j;
	for (i = 0; i != n; i ++)
	{
		for (j = 0; j != m; j ++)
		{
			MatrixNew[i*m+j] = MatrixOrigin[j*n+i];
		}
	}
}

void MatrixOperation::MatrixTranspose(float* MatrixOrigin, float* MatrixNew, int m, int n)
{
	int i;
	int j;
	for (i = 0; i != n; i ++)
	{
		for (j = 0; j != m; j ++)
		{
			MatrixNew[i*m+j] = MatrixOrigin[j*n+i];
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 矩阵求逆
// 参数说明：m：原始矩阵Matrix的行/列数
void MatrixOperation::MatrixInversion(double* Matrix, int m)
{
	double	maximum;	
	double	factor;		
	double	*row, *row1;
	double matinv[4][4];
	double	mat[4][4];	
	double	work[4];	
	int	status;
	int	size;
	int	m1;
	int i, j, k;

	size = m * sizeof(double);

	(void)memcpy((char *)mat, (char *)Matrix, m * size);



	for (i = 0; i < m; i++) {
		for (j = 0; j < m; j++) {
			matinv[i][j] = 0.0;
		}
	}
	for (i = 0; i < m; i++) {
		matinv[i][i] = 1.0;
	}

	for (j = 0; j < m; j++) {
		m1 = j;
		maximum = mat[j][j];
		for (i = j+1; i < m; i++) {
			if (fabs(maximum) < fabs(mat[i][j])) {
				m1 = i;
				maximum = mat[i][j];
			}
		}

		if (fabs(maximum) < EPS) {
			goto rtn;
		}

		if (m1 != j) {
			row = mat[j];
			row1 = mat[m1];
			(void)memcpy((char *)work, (char *)row, size);
			(void)memcpy((char *)row, (char *)row1, size);
			(void)memcpy((char *)row1, (char *)work, size);

			row = matinv[j];
			row1 = matinv[m1];
			(void)memcpy((char *)work, (char *)row, size);
			(void)memcpy((char *)row, (char *)row1, size);
			(void)memcpy((char *)row1, (char *)work, size);
		}

		row = mat[j];
		for (i = j+1; i < m; i++) {
			row[i] /= maximum;
		}

		row = matinv[j];
		for (i = 0; i < m; i++) {
			row[i] /= maximum;
		}

		for (i = 0; i < m; i++) {
			if (i != j) {
				factor = mat[i][j];
				for (k = j+1; k < m; k++) {
					mat[i][k] -= mat[j][k] * factor;
				}
				for (k = 0; k < m; k++) {
					matinv[i][k] -= matinv[j][k] * factor;
				}
			}
		}
	}

	for (i = 0; i < m; i++) {
		for (j = 0; j < m; j++) {
			Matrix[i*m+j] = matinv[i][j];
		}
	}
rtn:
	return;
}

void MatrixOperation::MatrixInversion(float* Matrix, int m)
{
	/*int i, j, k;
    for(k = 0; k != m; k ++)
	{
		for(i = 0; i != m; i ++)
		{
			if(i != k)
				Matrix[i*m+k] = - Matrix[i*m+k] / Matrix[k*m+k];
		}
		Matrix[k*m+k] = 1 / Matrix[k*m+k];

		for(i = 0; i != m; i ++)
		{
			if(i != k)
			{
				for(j = 0; j != m; j ++)
				{
					if(j != k)
						Matrix[i*m+j] += Matrix[k*m+j] * Matrix[i*m+k];
				}
			}
		}

		for(j = 0; j != m; j ++)
		{
			if(j != k)
				Matrix[k*m+j] *= Matrix[k*m+k];
		}
	}*/

	float	maximum;
	float	factor;
	float	*row, *row1;
	float matinv[4][4];
	float	mat[4][4];
	float	work[4];
	int	status;
	int	size;
	int	m1;
	int	i, j, k;

	size = m * sizeof(float);

	memcpy((char *)mat, (char *)Matrix, m * size);



	for (i = 0; i < m; i++) {
		for (j = 0; j < m; j++) {
			matinv[i][j] = 0.0;
		}
	}
	for (i = 0; i < m; i++) {
		matinv[i][i] = 1.0;
	}

	for (j = 0; j < m; j++) {
		m1 = j;
		maximum = mat[j][j];
		for (i = j+1; i < m; i++) {
			if (fabs(maximum) < fabs(mat[i][j])) {
				m1 = i;
				maximum = mat[i][j];
			}
		}

		if (fabs(maximum) < EPS) {
			goto rtn;
		}

		if (m1 != j) {
			row = mat[j];
			row1 = mat[m1];
			(void)memcpy((char *)work, (char *)row, size);
			(void)memcpy((char *)row, (char *)row1, size);
			(void)memcpy((char *)row1, (char *)work, size);

			row = matinv[j];
			row1 = matinv[m1];
			(void)memcpy((char *)work, (char *)row, size);
			(void)memcpy((char *)row, (char *)row1, size);
			(void)memcpy((char *)row1, (char *)work, size);
		}

		row = mat[j];
		for (i = j+1; i < m; i++) {
			row[i] /= maximum;
		}

		row = matinv[j];
		for (i = 0; i < m; i++) {
			row[i] /= maximum;
		}

		for (i = 0; i < m; i++) {
			if (i != j) {
				factor = mat[i][j];
				for (k = j+1; k < m; k++) {
					mat[i][k] -= mat[j][k] * factor;
				}
				for (k = 0; k < m; k++) {
					matinv[i][k] -= matinv[j][k] * factor;
				}
			}
		}
	}

	for (i = 0; i < m; i++) {
		for (j = 0; j < m; j++) {
			Matrix[i*m+j] = matinv[i][j];
		}
	}
rtn:
	return;
}

//////////////////////////////////////////////////////////////////////////
// 矩阵相乘
// 参数说明：C=A×B，ARow：A的行数，AColumn：A的列数，BColumn：B的列数
void MatrixOperation::MatrixMultiply(double* MatrixA, double* MatrixB, double* MatrixC, int ARow, int AColumn, int BColumn)
{
	int i;
	int j;
	int k;
	
	for (i = 0; i != ARow; i ++)
	{
		for (j = 0; j != BColumn; j ++)
		{
			for (k = 0; k !=AColumn; k ++)
			{
				MatrixC[i*BColumn+j] += MatrixA[i*AColumn+k] * MatrixB[k*BColumn+j]; 
			}
		}
	}
}

void MatrixOperation::MatrixMultiply(float* MatrixA, float* MatrixB, float* MatrixC, int ARow, int AColumn, int BColumn)
{
	int i;
	int j;
	int k;

	for (i = 0; i != ARow; i ++)
	{
		for (j = 0; j != BColumn; j ++)
		{
			for (k = 0; k !=AColumn; k ++)
			{
				MatrixC[i*BColumn+j] += MatrixA[i*AColumn+k] * MatrixB[k*BColumn+j];
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 矩阵相加
// 参数说明
void MatrixOperation::MatrixAdd(double* MatrixA, double* MatrixB, double* MatrixC, int Row, int Column)
{
	int i;
	int j;
	for (i = 0; i != Row; i ++)
	{
		for (j = 0; j != Column; j ++)
		{
			MatrixC[i*Column+j] = MatrixA[i*Column+j] + MatrixB[i*Column+j];
		}
	}
}
void MatrixOperation::MatrixAdd(float* MatrixA, float* MatrixB, float* MatrixC, int Row, int Column)
{
	int i;
	int j;
	for (i = 0; i != Row; i ++)
	{
		for (j = 0; j != Column; j ++)
		{
			MatrixC[i*Column+j] = MatrixA[i*Column+j] + MatrixB[i*Column+j];
		}
	}
}

void MatrixOperation::MatrixAdd(double* MatrixA, double* MatrixB, double* MatrixC, int length)
{
	int i;
	for (i = 0; i != length; i ++)
	{
		MatrixC[i] = MatrixA[i] + MatrixB[i];
	}
}
void MatrixOperation::MatrixAdd(float* MatrixA, float* MatrixB, float* MatrixC, int length)
{
	int i;
	for (i = 0; i != length; i ++)
	{
		MatrixC[i] = MatrixA[i] + MatrixB[i];
	}
}

void MatrixOperation::MatrixCopy(double* MatrixA, double* MatrixB, int length)
{
	int i;
	for (i = 0; i != length; i ++)
	{
		MatrixB[i] = MatrixA[i];
	}
}
void MatrixOperation::MatrixCopy(float* MatrixA, float* MatrixB, int length)
{
	int i;
	for (i = 0; i != length; i ++)
	{
		MatrixB[i] = MatrixA[i];
	}
}


void MatrixOperation::Multiply(double *MOrigin1, double *MOrigin2, double *MResult, int m, int n, int l)
{
	int i, j, k;
	for(i=0; i<m; i++)
		for(j=0; j<l; j++)
		{
			MResult[i*l+j] = 0.0;
            for(k=0; k<n; k++)
				MResult[i*l+j] += MOrigin1[i*n+k] * MOrigin2[j+k*l];
		}
}
void MatrixOperation::Multiply(float *MOrigin1, float *MOrigin2, float *MResult, int m, int n, int l)
{
	int i, j, k;
	for(i=0; i<m; i++)
		for(j=0; j<l; j++)
		{
			MResult[i*l+j] = 0.0;
            for(k=0; k<n; k++)
				MResult[i*l+j] += MOrigin1[i*n+k] * MOrigin2[j+k*l];
		}
}
void  MatrixOperation::Multiply(float MOrigin1[4][4], float MOrigin2[4][4], float MResult[4][4])
{
	for (int i=0; i < 4; i++) {
		for (int j=0; j < 4; j++) {
			MResult[i][j] =
				MOrigin1[i][0] * MOrigin2[0][j] +
				MOrigin1[i][1] * MOrigin2[1][j] +
				MOrigin1[i][2] * MOrigin2[2][j] +
				MOrigin1[i][3] * MOrigin2[3][j];
		}
	}
}
void
MatrixOperation::MultDirMatrix(const double* MatrixA,double* srcDir, double* dstDir)
{
	// Checks if the "this" matrix is equal to the identity matrix.  See
	// also code comments at the start of SbMatrix::multRight().
	const double * t0 = &MatrixA[0];
	const double * t1 = &MatrixA[4];
	const double * t2 = &MatrixA[8];
	// Copy the src vector, just in case src and dst is the same vector.
	const double * s = srcDir;

	dstDir[0] = s[0]*t0[0] + s[1]*t1[0] + s[2]*t2[0];
	dstDir[1] = s[0]*t0[1] + s[1]*t1[1] + s[2]*t2[1];
	dstDir[2] = s[0]*t0[2] + s[1]*t1[2] + s[2]*t2[2];
}

void
MatrixOperation::MultDirMatrix(const float * MatrixA,float* srcDir, float* dstDir)
{
	// Checks if the "this" matrix is equal to the identity matrix.  See
	// also code comments at the start of SbMatrix::multRight().
	const float * t0 = &MatrixA[0];
	const float * t1 = &MatrixA[4];
	const float * t2 = &MatrixA[8];
	// Copy the src vector, just in case src and dst is the same vector.
	const float * s = srcDir;

	dstDir[0] = s[0]*t0[0] + s[1]*t1[0] + s[2]*t2[0];
	dstDir[1] = s[0]*t0[1] + s[1]*t1[1] + s[2]*t2[1];
	dstDir[2] = s[0]*t0[2] + s[1]*t1[2] + s[2]*t2[2];
}


void MatrixOperation::DoubleMtxToFloatMtx(double dMtxAA[4][4],float fMtxAA[4][4]) {
	for(int i = 0; i< 4; i++)
	{
		for(int j = 0; j< 4; j++)
		{
			fMtxAA[i][j] = (float)dMtxAA[i][j];
		}
	}
}

void MatrixOperation::FloatMtxToDoubleMtx(float fMtxAA[4][4],double dMtxAA[4][4]) {
	for(int i = 0; i< 4; i++)
	{
		for(int j = 0; j< 4; j++)
		{
			dMtxAA[i][j] = fMtxAA[i][j];
		}
	}
}

void MatrixOperation::MtxTanslationToUniTanslation(float fPivot[3], float fMtxAA[4][4], float fPos[3])
{
	fMtxAA[3][0] = fPos[0];
	fMtxAA[3][1] = fPos[1];
	fMtxAA[3][2] = fPos[2];
	//将当前位置转换为沿旋转中心点旋转至单位矩阵后的坐标位置
	fPos[0] = fPivot[0]* fMtxAA[0][0]+fPivot[1]* fMtxAA[1][0]+fPivot[2]* fMtxAA[2][0]+fMtxAA[3][0];
	fPos[1] = fPivot[0]* fMtxAA[0][1]+fPivot[1]* fMtxAA[1][1]+fPivot[2]* fMtxAA[2][1]+fMtxAA[3][1];
	fPos[2] = fPivot[0]* fMtxAA[0][2]+fPivot[1]* fMtxAA[1][2]+fPivot[2]* fMtxAA[2][2]+fMtxAA[3][2];
	fPos[0] = fPos[0] - fPivot[0];
	fPos[1] = fPos[1] - fPivot[1];
	fPos[2] = fPos[2] - fPivot[2];
}

void MatrixOperation::UniTanslationToMtxTanslation(float fPivot[3], float fMtxAA[4][4], float fPos[3])
{
	fMtxAA[3][0] = fMtxAA[3][1] = fMtxAA[3][2] = 0.0;

	float TanslationMtx[4][4] = {
		1.0, 0, 0, 0,
		0, 1.0, 0, 0,
		0, 0, 1.0, 0,
		fPos[0], fPos[1], fPos[2], 1.0 };

	float PivotMtx[4][4] = {
		1.0, 0, 0, 0,
		0, 1.0, 0, 0,
		0, 0, 1.0, 0,
		fPivot[0], fPivot[1], fPivot[2], 1.0 };

		float PivotInvMtx[4][4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				PivotInvMtx[i][j] = PivotMtx[i][j];
			}
		}
		MatrixOperation::MatrixInversion((float*)PivotInvMtx,4);

		float ResultMtx1[4][4];
		float ResultMtx2[4][4];
		MatrixOperation::Multiply((float*)PivotInvMtx, (float*)fMtxAA,(float*)ResultMtx1);
		MatrixOperation::Multiply((float*)ResultMtx1, (float*)PivotMtx,(float*)ResultMtx2);
		MatrixOperation::Multiply((float*)ResultMtx2, (float*)TanslationMtx,(float*)ResultMtx1);

		fPos[0] = ResultMtx1[3][0];
		fPos[1] = ResultMtx1[3][1];
		fPos[2] = ResultMtx1[3][2];
}

void MatrixOperation::TransformPoint( double dMtxAA[4][4], double dPoint[3])
{
	// Checks if the "this" matrix is equal to the identity matrix.  See
	// also code comments at the start of SbMatrix::multRight().

	const double * t0 = dMtxAA[0];
	const double * t1 = dMtxAA[1];
	const double * t2 = dMtxAA[2];
	const double * t3 = dMtxAA[3];

	double dTempPonit[3];
	dTempPonit[0] = dPoint[0];
	dTempPonit[1] = dPoint[1];
	dTempPonit[2] = dPoint[2];

	dPoint[0] = (dTempPonit[0]*t0[0] + dTempPonit[1]*t1[0] + dTempPonit[2]*t2[0] + t3[0]);
	dPoint[1] = (dTempPonit[0]*t0[1] + dTempPonit[1]*t1[1] + dTempPonit[2]*t2[1] + t3[1]);
	dPoint[2] = (dTempPonit[0]*t0[2] + dTempPonit[1]*t1[2] + dTempPonit[2]*t2[2] + t3[2]);
}

void MatrixOperation::TransformPoint( float fMtxAA[4][4], float fPoint[3])
{
	// Checks if the "this" matrix is equal to the identity matrix.  See
	// also code comments at the start of SbMatrix::multRight().

	const float * t0 = fMtxAA[0];
	const float * t1 = fMtxAA[1];
	const float * t2 = fMtxAA[2];
	const float * t3 = fMtxAA[3];

	float dTempPonit[3];
	dTempPonit[0] = fPoint[0];
	dTempPonit[1] = fPoint[1];
	dTempPonit[2] = fPoint[2];

	fPoint[0] = (dTempPonit[0]*t0[0] + dTempPonit[1]*t1[0] + dTempPonit[2]*t2[0] + t3[0]);
	fPoint[1] = (dTempPonit[0]*t0[1] + dTempPonit[1]*t1[1] + dTempPonit[2]*t2[1] + t3[1]);
	fPoint[2] = (dTempPonit[0]*t0[2] + dTempPonit[1]*t1[2] + dTempPonit[2]*t2[2] + t3[2]);
}


void MatrixOperation::MtxTanslationToUniTanslation(double fPivot[3], double fMtxAA[4][4], double fPos[3])
{
	fMtxAA[3][0] = fPos[0];
	fMtxAA[3][1] = fPos[1];
	fMtxAA[3][2] = fPos[2];
	//将当前位置转换为沿旋转中心点旋转至单位矩阵后的坐标位置
	fPos[0] = fPivot[0]* fMtxAA[0][0]+fPivot[1]* fMtxAA[1][0]+fPivot[2]* fMtxAA[2][0]+fMtxAA[3][0];
	fPos[1] = fPivot[0]* fMtxAA[0][1]+fPivot[1]* fMtxAA[1][1]+fPivot[2]* fMtxAA[2][1]+fMtxAA[3][1];
	fPos[2] = fPivot[0]* fMtxAA[0][2]+fPivot[1]* fMtxAA[1][2]+fPivot[2]* fMtxAA[2][2]+fMtxAA[3][2];
	fPos[0] = fPos[0] - fPivot[0];
	fPos[1] = fPos[1] - fPivot[1];
	fPos[2] = fPos[2] - fPivot[2];
}

void MatrixOperation::UniTanslationToMtxTanslation(double dPivot[3], double dQuat[4][4], double dPos[3])
{
	dQuat[3][0] = dQuat[3][1] = dQuat[3][2] = 0.0;

	double TanslationMtx[4][4] = {
		1.0, 0, 0, 0,
		0, 1.0, 0, 0,
		0, 0, 1.0, 0,
		dPos[0], dPos[1], dPos[2], 1.0 };

		double PivotMtx[4][4] = {
			1.0, 0, 0, 0,
			0, 1.0, 0, 0,
			0, 0, 1.0, 0,
			dPivot[0], dPivot[1], dPivot[2], 1.0 };

			double PivotInvMtx[4][4];
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					PivotInvMtx[i][j] = PivotMtx[i][j];
				}
			}
			MatrixOperation::MatrixInversion((double*)PivotInvMtx,4);

			double ResultMtx1[4][4];
			double ResultMtx2[4][4];
			MatrixOperation::Multiply((double*)PivotInvMtx, (double*)dQuat,(double*)ResultMtx1);
			MatrixOperation::Multiply((double*)ResultMtx1, (double*)PivotMtx,(double*)ResultMtx2);
			MatrixOperation::Multiply((double*)ResultMtx2, (double*)TanslationMtx,(double*)ResultMtx1);

			dPos[0] = ResultMtx1[3][0];
			dPos[1] = ResultMtx1[3][1];
			dPos[2] = ResultMtx1[3][2];
}

void MatrixOperation::MakeIdentity(double dMtxAA[4][4]) {

		for(int i = 0; i< 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				if(i == j)
				{
					dMtxAA[i][j] = 1.0;
				}
				else
				{
					dMtxAA[i][j] = 0.0;
				}
			}
		}
}

void MatrixOperation::MakeIdentity(float fMtxAA[4][4]) {

	for(int i = 0; i< 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(i == j)
			{
				fMtxAA[i][j] = 1.0;
			}
			else
			{
				fMtxAA[i][j] = 0.0;
			}
		}
	}
}

void MatrixOperation::FromQuat(float quat[4],float* fMatrix)
{
	float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;
	s = 2.0f/(quat[0]*quat[0] + quat[1]*quat[1] + quat[2]*quat[2] + quat[3]*quat[3]);

	xs = quat[0]*s;	ys = quat[1]*s;		zs = quat[2]*s;
	wx = quat[3]*xs;	wy = quat[3]*ys;		wz = quat[3]*zs;
	xx = quat[0]*xs;	xy = quat[0]*ys;		xz = quat[0]*zs;
	yy = quat[1]*ys;	yz = quat[1]*zs;		zz = quat[2]*zs;

	fMatrix[0] = 1.0f - (float)(yy+zz); fMatrix[1] = (float)xy + (float)wz; fMatrix[2] = (float)xz - (float)wy; fMatrix[3] = 0.0f;
	fMatrix[4] = (float)xy - (float)wz; fMatrix[5] = 1.0f - (float)(xx + zz); fMatrix[6] = (float)yz + (float)wx; fMatrix[7] = 0.0f;
	fMatrix[8] = (float)xz + (float)wy; fMatrix[9] = (float)yz - (float)wx; fMatrix[10] = 1.0f - (float)(xx + yy); fMatrix[11] = 0.0f;
	fMatrix[12] = 0.0f; fMatrix[13] = 0.0f; fMatrix[14] = 0.0f; fMatrix[15] = 1.0f;
}


void MatrixOperation::setRotate(float matrix[4][4],float fRotate[4])
{
	float l = sqrt(fRotate[0]*fRotate[0]+fRotate[1]*fRotate[1]+fRotate[2]*fRotate[2]+fRotate[3]*fRotate[3]);
	float x,y,z,w;
	if (l > 0.0000001) {
		// normalize it
		x = fRotate[0] / l;
		y = fRotate[1] / l;
		z = fRotate[2] / l;
		w = fRotate[3] / l;
	}
	else {
		// identity
		x = y = z = 0.0f;
		w = 1.0f;
	}
	matrix[0][0] = 1.0f - 2.0f * (y * y + z * z);
	matrix[0][1] = 2.0f * (x * y + z * w);
	matrix[0][2] = 2.0f * (z * x - y * w);
	matrix[0][3] = 0.0f;

	matrix[1][0] = 2.0f * (x * y - z * w);
	matrix[1][1] = 1.0f - 2.0f * (z * z + x * x);
	matrix[1][2] = 2.0f * (y * z + x * w);
	matrix[1][3] = 0;

	matrix[2][0] = 2.0f * (z * x + y * w);
	matrix[2][1] = 2.0f * (y * z - x * w);
	matrix[2][2] = 1.0f - 2.0f * (y * y + x * x);
	matrix[2][3] = 0.0f;

	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;
}
void MatrixOperation::setScale(float fMtxAA[4][4],float fScale[3])
{
	MatrixOperation::MakeIdentity(fMtxAA);
	fMtxAA[0][0] = fScale[0];
	fMtxAA[1][1] = fScale[1];
	fMtxAA[2][2] = fScale[2];
}
void MatrixOperation::setTranslate(float fMtxAA[4][4],float fPos[3])
{
	MatrixOperation::MakeIdentity(fMtxAA);
	fMtxAA[3][0] = fPos[0];
	fMtxAA[3][1] = fPos[1];
	fMtxAA[3][2] = fPos[2];
}

void MatrixOperation::setTransform(float fMtxAA[4][4],float fPos[3], float fRotate[4], float fScale[3])
{
	float tmp[4][4],tmp2[4][4];
	setScale(fMtxAA,fScale);		
	setRotate(tmp,fRotate);
	(void)memcpy(tmp2, fMtxAA, 4*4*sizeof(float));
	Multiply(tmp2,tmp,fMtxAA);		
	setTranslate(tmp,fPos);
	(void)memcpy(tmp2, fMtxAA, 4*4*sizeof(float));
	Multiply(tmp2,tmp,fMtxAA);		
}





class SbMatrixP {
public:
	enum QuatPart {X, Y, Z, W};
	typedef float HMatrix[4][4]; /* Right-handed, for column vectors */

	typedef struct {
		float t[4];    /* Translation components */
		float  q[4];        /* Essential rotation     */
		float  u[4];        /* Stretch rotation       */
		float k[4];    /* Stretch factors        */
		float f;      /* Sign of determinant    */
	} AffineParts;
	static void decomp_affine(HMatrix A, AffineParts * parts);
	static float polar_decomp(SbMatrixP::HMatrix M, SbMatrixP::HMatrix Q, SbMatrixP::HMatrix S);
	static void snuggle(float q[4], float k[4],float out[4]);
	static void spect_decomp(SbMatrixP::HMatrix S, SbMatrixP::HMatrix U,float out[4]);

	static void mat_mult(SbMatrixP::HMatrix A, SbMatrixP::HMatrix B, SbMatrixP::HMatrix AB);
	static float vdot(float * va, float * vb);
	static void vcross(float * va, float * vb, float * v);
	static void adjoint_transpose(SbMatrixP::HMatrix M, SbMatrixP::HMatrix MadjT);
	static float mat_norm(SbMatrixP::HMatrix M, int tpose);
	static float norm_inf(SbMatrixP::HMatrix M) {return mat_norm(M, 0);}
	static float norm_one(SbMatrixP::HMatrix M) {return mat_norm(M, 1);}  static 
	int find_max_col(SbMatrixP::HMatrix M);
	static void make_reflector(float * v, float * u);
	static void reflect_cols(SbMatrixP::HMatrix M, float * u);
	static void reflect_rows(SbMatrixP::HMatrix M, float * u);

	static void do_rank1(SbMatrixP::HMatrix M, SbMatrixP::HMatrix Q);
	static void do_rank2(SbMatrixP::HMatrix M, SbMatrixP::HMatrix MadjT, SbMatrixP::HMatrix Q);

	static void invert(float q[4]);
	static void multiply(float q[4],float p[4],float out[4]);
	static void transpose(float mat[4][4]);
	static void matToRotation(float mat[4][4],float out[4]);
	static HMatrix mat_id;
};

SbMatrixP::HMatrix SbMatrixP::mat_id = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

/******* Matrix Preliminaries *******/

/** Fill out 3x3 matrix to 4x4 **/
#define sp_mat_pad(A) (A[W][X]=A[X][W]=A[W][Y]=A[Y][W]=A[W][Z]=A[Z][W]=0, A[W][W]=1)

/** Copy nxn matrix A to C using "gets" for assignment **/
#define sp_mat_copy(C, gets, A, n) {int i, j; for (i=0;i<n;i++) for (j=0;j<n;j++)\
	C[i][j] gets (A[i][j]);}

/** Copy transpose of nxn matrix A to C using "gets" for assignment **/
#define sp_mat_tpose(AT, gets, A, n) {int i, j; for (i=0;i<n;i++) for (j=0;j<n;j++)\
	AT[i][j] gets (A[j][i]);}

/** Assign nxn matrix C the element-wise combination of A and B using "op" **/
#define sp_mat_binop(C, gets, A, op, B, n) {int i, j; for (i=0;i<n;i++) for (j=0;j<n;j++)\
	C[i][j] gets (A[i][j]) op (B[i][j]);}

/** Multiply the upper left 3x3 parts of A and B to get AB **/
void
SbMatrixP::mat_mult(SbMatrixP::HMatrix A, SbMatrixP::HMatrix B, SbMatrixP::HMatrix AB)
{
	int i, j;
	for (i=0; i<3; i++) for (j=0; j<3; j++)
		AB[i][j] = A[i][0]*B[0][j] + A[i][1]*B[1][j] + A[i][2]*B[2][j];
}
/** Return index of column of M containing maximum abs entry, or -1 if M=0 **/
int
SbMatrixP::find_max_col(SbMatrixP::HMatrix M)
{
	float abs, max;
	int i, j, col;
	max = 0.0; col = -1;
	for (i=0; i<3; i++) for (j=0; j<3; j++) {
		abs = M[i][j]; if (abs<0.0) abs = -abs;
		if (abs>max) {max = abs; col = j;}
	}
	return col;
}

/** Setup u for Household reflection to zero all v components but first **/
void
SbMatrixP::make_reflector(float * v, float * u)
{
	float s = static_cast<float>(sqrt(vdot(v, v)));
	u[0] = v[0]; u[1] = v[1];
	u[2] = v[2] + ((v[2]<0.0) ? -s : s);
	s = static_cast<float>(sqrt(2.0/vdot(u, u)));
	u[0] = u[0]*s; u[1] = u[1]*s; u[2] = u[2]*s;
}

/** Apply Householder reflection represented by u to column vectors of M **/
void
SbMatrixP::reflect_cols(SbMatrixP::HMatrix M, float * u)
{
	int i, j;
	for (i=0; i<3; i++) {
		float s = u[0]*M[0][i] + u[1]*M[1][i] + u[2]*M[2][i];
		for (j=0; j<3; j++) M[j][i] -= u[j]*s;
	}
}
/** Apply Householder reflection represented by u to row vectors of M **/
void
SbMatrixP::reflect_rows(SbMatrixP::HMatrix M, float * u)
{
	int i, j;
	for (i=0; i<3; i++) {
		float s = vdot(u, M[i]);
		for (j=0; j<3; j++) M[i][j] -= u[j]*s;
	}
}


/** Find orthogonal factor Q of rank 1 (or less) M **/
void
SbMatrixP::do_rank1(SbMatrixP::HMatrix M, SbMatrixP::HMatrix Q)
{
	float v1[3], v2[3], s;
	int col;
	sp_mat_copy(Q, =, mat_id, 4);
	/* If rank(M) is 1, we should find a non-zero column in M */
	col = find_max_col(M);
	if (col<0) return; /* Rank is 0 */
	v1[0] = M[0][col]; v1[1] = M[1][col]; v1[2] = M[2][col];
	make_reflector(v1, v1); reflect_cols(M, v1);
	v2[0] = M[2][0]; v2[1] = M[2][1]; v2[2] = M[2][2];
	make_reflector(v2, v2); reflect_rows(M, v2);
	s = M[2][2];
	if (s<0.0) Q[2][2] = -1.0;
	reflect_cols(Q, v1); reflect_rows(Q, v2);
}

/** Find orthogonal factor Q of rank 2 (or less) M using adjoint transpose **/
void
SbMatrixP::do_rank2(SbMatrixP::HMatrix M, SbMatrixP::HMatrix MadjT, SbMatrixP::HMatrix Q)
{
	float v1[3], v2[3];
	float w, x, y, z, c, s, d;
	int col;
	/* If rank(M) is 2, we should find a non-zero column in MadjT */
	col = find_max_col(MadjT);
	if (col<0) {do_rank1(M, Q); return;} /* Rank<2 */
	v1[0] = MadjT[0][col]; v1[1] = MadjT[1][col]; v1[2] = MadjT[2][col];
	make_reflector(v1, v1); reflect_cols(M, v1);
	vcross(M[0], M[1], v2);
	make_reflector(v2, v2); reflect_rows(M, v2);
	w = M[0][0]; x = M[0][1]; y = M[1][0]; z = M[1][1];
	if (w*z>x*y) {
		c = z+w; s = y-x; d = static_cast<float>(sqrt(c*c+s*s)); c = c/d; s = s/d;
		Q[0][0] = Q[1][1] = c; Q[0][1] = -(Q[1][0] = s);
	}
	else {
		c = z-w; s = y+x; d = static_cast<float>(sqrt(c*c+s*s)); c = c/d; s = s/d;
		Q[0][0] = -(Q[1][1] = c); Q[0][1] = Q[1][0] = s;
	}
	Q[0][2] = Q[2][0] = Q[1][2] = Q[2][1] = 0.0; Q[2][2] = 1.0;
	reflect_cols(Q, v1); reflect_rows(Q, v2);
}



/** Return dot product of length 3 vectors va and vb **/
float
SbMatrixP::vdot(float * va, float * vb)
{
	return (va[0]*vb[0] + va[1]*vb[1] + va[2]*vb[2]);
}

/** Set v to cross product of length 3 vectors va and vb **/
void
SbMatrixP::vcross(float * va, float * vb, float * v)
{
	v[0] = va[1]*vb[2] - va[2]*vb[1];
	v[1] = va[2]*vb[0] - va[0]*vb[2];
	v[2] = va[0]*vb[1] - va[1]*vb[0];
}

/** Set MadjT to transpose of inverse of M times determinant of M **/
void
SbMatrixP::adjoint_transpose(SbMatrixP::HMatrix M, SbMatrixP::HMatrix MadjT)
{
	vcross(M[1], M[2], MadjT[0]);
	vcross(M[2], M[0], MadjT[1]);
	vcross(M[0], M[1], MadjT[2]);
}


/** Compute either the 1 or infinity norm of M, depending on tpose **/
float
SbMatrixP::mat_norm(SbMatrixP::HMatrix M, int tpose)
{
	int i;
	float sum, max;
	max = 0.0;
	for (i=0; i<3; i++) {
		if (tpose) sum = static_cast<float>(fabs(M[0][i])+fabs(M[1][i])+fabs(M[2][i]));
		else sum = static_cast<float>((fabs(M[i][0])+fabs(M[i][1])+fabs(M[i][2])));
		if (max<sum) max = sum;
	}
	return max;
}


/* Helper function for the snuggle() function below. */
static inline void
cycle(float * a, bool flip)
{
	if (flip) {
		a[3]=a[0]; a[0]=a[1]; a[1]=a[2]; a[2]=a[3];
	}
	else {
		a[3]=a[2]; a[2]=a[1]; a[1]=a[0]; a[0]=a[3];
	}
}
void  SbMatrixP::invert(float q[4])
{
	float length = sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);
	if (length<0.000001f)
	{
	}
	float inv = 1.0f / length;

	q[0] = -q[0] * inv;
	q[1] = -q[1] * inv;
	q[2] = -q[2] * inv;
	q[3] = q[3] * inv;
}
void SbMatrixP::multiply(float q[4],float p[4],float out[4])
{
	out[0]=p[3]*q[0] + p[0]*q[3] + p[1]*q[2] - p[2]*q[1];
	out[1]=p[3]*q[1] - p[0]*q[2] + p[1]*q[3] + p[2]*q[0];
	out[2]=p[3]*q[2] + p[0]*q[1] - p[1]*q[0] + p[2]*q[3];
	out[3]=p[3]*q[3] - p[0]*q[0] - p[1]*q[1] - p[2]*q[2];
}
/******* Spectral Axis Adjustment *******/

/* Given a unit quaternion, q, and a scale vector, k, find a unit quaternion, p,
 * which permutes the axes and turns freely in the plane of duplicate scale
 * factors, such that q p has the largest possible w component, i.e. the
 * smallest possible angle. Permutes k's components to go with q p instead of q.
 * See Ken Shoemake and Tom Duff. Matrix Animation and Polar Decomposition.
 * Proceedings of Graphics Interface 1992. Details on p. 262-263.
 */
void SbMatrixP::snuggle(float q[4], float k[4],float out[4])
{
#define SQRTHALF (0.7071067811865475244f)
#define sgn(n, v)    ((n)?-(v):(v))
#define swap(a, i, j) {a[3]=a[i]; a[i]=a[j]; a[j]=a[3];}

  float p[4];
  float ka[4];
  int i, turn = -1;
  ka[X] = k[X]; ka[Y] = k[Y]; ka[Z] = k[Z];
  if (ka[X]==ka[Y]) {if (ka[X]==ka[Z]) turn = W; else turn = Z;}
  else {if (ka[X]==ka[Z]) turn = Y; else if (ka[Y]==ka[Z]) turn = X;}
  if (turn>=0) {
    float qtoz[4], qp[4];
    unsigned neg[3], win;
    double mag[3], t;
    static float qxtoz[4]={0.0f, SQRTHALF, 0.0f, SQRTHALF};
    static float qytoz[4]={SQRTHALF, 0.0f, 0.0f, SQRTHALF};
    static float qppmm[4]={0.5f, 0.5f, -0.5f, -0.5f};
    static float qpppp[4]={0.5f, 0.5f, 0.5f, 0.5f};
    static float qmpmm[4]={-0.5f, 0.5f, -0.5f, -0.5f};
    static float qpppm[4]={0.5f, 0.5f, 0.5f, -0.5f};
    static float q0001[4]={0.0f, 0.0f, 0.0f, 1.0f};
    static float q1000[4]={1.0f, 0.0f, 0.0f, 0.0f};
    switch (turn) {
    case X:
		{
			memcpy(qtoz,qxtoz,sizeof(float)*4);
			float tmp[4];
			multiply(qxtoz,q,tmp);
			memcpy(q,tmp,sizeof(float)*4);
			//q = (qtoz = qxtoz) * q; 
			swap(ka, X, Z) break;
		}
    case Y: 
		{
			memcpy(qtoz,qxtoz,sizeof(float)*4);
			float tmp[4];
			multiply(qxtoz,q,tmp);
			memcpy(q,tmp,sizeof(float)*4);
			//q = (qtoz = qytoz) * q;
			swap(ka, Y, Z) break;
		}
	case Z: 
		{
			memcpy(qtoz,q0001,sizeof(float)*4);
			//qtoz = q0001;
			break;
		}
	default:
		{
			invert(q);
			memcpy(out,q,sizeof(float)*4);
			return;
		}
    }
	invert(q);
    mag[0] = static_cast<double>(q[Z]*q[Z])+static_cast<double>(q[W]*q[W])-0.5;
    mag[1] = static_cast<double>(q[X]*q[Z])-static_cast<double>(q[Y]*q[W]);
    mag[2] = static_cast<double>(q[Y]*q[Z])+static_cast<double>(q[X]*q[W]);
    for (i=0; i<3; i++) if ((neg[i] = (mag[i] < 0.0))) mag[i] = -mag[i];
    if (mag[0]>mag[1]) {if (mag[0]>mag[2]) win = 0; else win = 2;}
    else {if (mag[1]>mag[2]) win = 1; else win = 2;}
    switch (win) {
    case 0:
		{
			if (neg[0])
				memcpy(p,q1000,sizeof(float)*4); 
			else
				memcpy(p,q0001,sizeof(float)*4); 
			break;
		}
    case 1: 
		{
			if (neg[1])
				memcpy(p ,qppmm,sizeof(float)*4);  
			else 
				memcpy(p , qpppp,sizeof(float)*4); 
			cycle(ka, false);
			break;
		}
    case 2: 
		{
			if (neg[2]) 
				memcpy(p , qmpmm,sizeof(float)*4); 
			else
				memcpy(p , qpppm,sizeof(float)*4); 
			cycle(ka, true); break;
		}
    }
    multiply(p , q , qp);
    t = sqrt(mag[win]+0.5);
	float tmp1[4],tmp2[4];
    tmp1[0]=0.0;
	tmp1[1]=0.0;
	tmp1[2]=-qp[Z]/static_cast<float>(t) ;
	tmp1[3]=qp[W]/static_cast<float>(t) ;
	memcpy(tmp2,p,sizeof(float)*4);
	multiply(tmp1,tmp2,p);
	memcpy(tmp2,p,sizeof(float)*4);
	invert(tmp2);	
	multiply(tmp2,qtoz,p);
  }
  else {
    float qa[4], pa[4];
    unsigned lo, hi, neg[4], par = 0;
    double all, big, two;
    qa[0] = q[X]; qa[1] = q[Y]; qa[2] = q[Z]; qa[3] = q[W];
    for (i=0; i<4; i++) {
      pa[i] = 0.0;
      if ((neg[i] = (qa[i]<0.0))) qa[i] = -qa[i];
      par ^= neg[i];
    }
    /* Find two largest components, indices in hi and lo */
    if (qa[0]>qa[1]) lo = 0; else lo = 1;
    if (qa[2]>qa[3]) hi = 2; else hi = 3;
    if (qa[lo]>qa[hi]) {
      if (qa[lo^1]>qa[hi]) {hi = lo; lo ^= 1;}
      else {hi ^= lo; lo ^= hi; hi ^= lo;}
    } else {if (qa[hi^1]>qa[lo]) lo = hi^1;}
    all = (qa[0]+qa[1]+qa[2]+qa[3])*0.5;
    two = (qa[hi]+qa[lo])*SQRTHALF;
    big = qa[hi];
    if (all>two) {
      if (all>big) {/*all*/
        {int i; for (i=0; i<4; i++) pa[i] = sgn(neg[i], 0.5f);}
        cycle(ka, par);
      }
      else {/*big*/ pa[hi] = sgn(neg[hi], 1.0f);}
    }
    else {
      if (two>big) {/*two*/
        pa[hi] = sgn(neg[hi], SQRTHALF); pa[lo] = sgn(neg[lo], SQRTHALF);
        if (lo>hi) {hi ^= lo; lo ^= hi; hi ^= lo;}
        if (hi==W) {hi = "\001\002\000"[lo]; lo = 3-hi-lo;}
        swap(ka, hi, lo)
          } else {/*big*/ pa[hi] = sgn(neg[hi], 1.0f);}
    }
    // FIXME: p = conjugate(pa)? 20010114 mortene.
    p[0]=-pa[0],p[1]= -pa[1],p[2]= -pa[2], p[3]=pa[3];
  }
  k[X] = ka[X]; k[Y] = ka[Y]; k[Z] = ka[Z];
  memcpy(out,q,sizeof(float)*4);

#undef SQRTHALF
#undef sgn
#undef swap
}
/******* Spectral Decomposition *******/

/* Compute the spectral decomposition of symmetric positive semi-definite S.
 * Returns rotation in U and scale factors in result, so that if K is a diagonal
 * matrix of the scale factors, then S = U K (U transpose). Uses Jacobi method.
 * See Gene H. Golub and Charles F. Van Loan. Matrix Computations. Hopkins 1983.
 */
void SbMatrixP::spect_decomp(SbMatrixP::HMatrix S, SbMatrixP::HMatrix U,float out[4])
{
  float kv[4];
  double Diag[3], OffD[3]; /* OffD is off-diag (by omitted index) */
  double g, h, fabsh, fabsOffDi, t, theta, c, s, tau, ta, OffDq;
  static char nxt[] = {Y, Z, X};
  int sweep, i, j;
  sp_mat_copy(U, =, mat_id, 4);
  Diag[X] = S[X][X]; Diag[Y] = S[Y][Y]; Diag[Z] = S[Z][Z];
  OffD[X] = S[Y][Z]; OffD[Y] = S[Z][X]; OffD[Z] = S[X][Y];
  for (sweep=20; sweep>0; sweep--) {
    float sm = static_cast<float>((fabs(OffD[X])+fabs(OffD[Y])+fabs(OffD[Z])));
    if (sm==0.0) break;
    for (i=Z; i>=X; i--) {
      int p = nxt[i]; int q = nxt[p];
      fabsOffDi = fabs(OffD[i]);
      g = 100.0*fabsOffDi;
      if (fabsOffDi>0.0) {
        h = Diag[q] - Diag[p];
        fabsh = fabs(h);
        if (fabsh+g==fabsh) {
          t = OffD[i]/h;
        }
        else {
          theta = 0.5*h/OffD[i];
          t = 1.0/(fabs(theta)+sqrt(theta*theta+1.0));
          if (theta<0.0) t = -t;
        }
        c = 1.0/sqrt(t*t+1.0); s = t*c;
        tau = s/(c+1.0);
        ta = t*OffD[i]; OffD[i] = 0.0;
        Diag[p] -= ta; Diag[q] += ta;
        OffDq = OffD[q];
        OffD[q] -= s*(OffD[p] + tau*OffD[q]);
        OffD[p] += s*(OffDq   - tau*OffD[p]);
        for (j=Z; j>=X; j--) {
          // FIXME: this is a very peculiar problem.. if the code in
          // the #else part is activated, Valgrind 1.9.3 reports "Use
          // of uninitialised value of size 8" in this code, when
          // built with g++ 2.95.4. The code under the #if runs
          // without any warnings, even though the only change is some
          // *seemingly* irrelevant casting.
          //
          // (And even more strange: by inserting any call to external
          // code (for instance a line of ''(void)atoi("0");'') below,
          // the Valgrind error vanishes. Could it be some sort of
          // optimalization bug related to stack overwriting or some
          // such?)
          //
          // We also have a report about a crash assumed to be in this
          // code with Coin built with MSVC6.0 with /O2 optimalization
          // (very suspect that there's a problem with both g++ and
          // MSVC++), for the following stand-alone example:
          //
          // ----8<----- [snip] ---------8<----- [snip] -----
          //  #include <Inventor/SoDB.h>
          //  #include <Inventor/SbLinear.h>
          //
          //  int
          //  main( int argc, char** argv )
          //  {
          //    SoDB::init();
          //
          //    SbVec3f translation, scale, axis;
          //    SbRotation rot, scaleRot;
          //    SbMatrix matrix (5.96046e-008f,  1.00000f,       -2.98023e-008f, 0.000000f,
          //                     -2.98023e-008f, 5.96046e-008f,  1.00000f,       0.000000f,
          //                     1.00000f,       -2.98023e-008f, 5.96046e-008f,  0.000000f,
          //                     -162.929f,      -56.2217f,      197.110f,       1.00000f
          //                     );
          //    matrix.getTransform ( translation, rot, scale, scaleRot );
          //    translation.print(stdout); printf(" <- translation\n");
          //    scale.print(stdout); printf(" <- scale\n");
          //    rot.print(stdout); printf(" <- rot\n");
          //    scaleRot.print(stdout); printf(" <- scaleRot\n");
          //
          //    return 0;
          //  }
          // ----8<----- [snip] ---------8<----- [snip] -----
          //
          // I was not able to reproduce the crash, so I got stuck in
          // debugging this.
          //
          // Mailed a patch with the casting changes back to the
          // original reporter of the problem (Tore K at HitecO), and
          // he reports that the crash bug is then also gone! That's
          // the damndest thing.. really weird.
          //
          // It does seem extremely likely to be a compiler problem,
          // although it's very fishy that it mucks something up for
          // both GNU GCC (with -O2) and MS's VisualC++ (with /O2).
          //
          // I'll buy a beer for anyone who can figure out this one
          // and explain it to me.
          //
          // 20030214 mortene.
          float a = U[j][p], b = U[j][q];
#if 1
          U[j][p] -= float(s)*(b + float(tau)*a);
          U[j][q] += float(s)*(a - float(tau)*b);
#else
          U[j][p] -= (float)(s*(b + tau*a));
          U[j][q] += (float)(s*(a - tau*b));
#endif
        }
      }
    }
  }
  kv[X] = static_cast<float>(Diag[X]);
  kv[Y] = static_cast<float>(Diag[Y]);
  kv[Z] = static_cast<float>(Diag[Z]);
  kv[W] = 1.0f;
  memcpy(out,kv,sizeof(float)*4);
}
void SbMatrixP::transpose(float mat[4][4])
{
	for (int i=0; i < 3; i++) {
		for (int j=i+1; j < 4; j++) {
			{
				float fTmp;
				fTmp=mat[i][j];
				mat[i][j]=mat[j][i];
				mat[j][i]=fTmp;
			}
		}
	}
}
void SbMatrixP::matToRotation(float m[4][4],float out[4])
{
	float scalerow = m[0][0] + m[1][1] + m[2][2];

	if (scalerow > 0.0f) {
		float s = static_cast<float>(sqrt(scalerow + m[3][3]));
		out[3] = s * 0.5f;
		s = 0.5f / s;

		out[0] = (m[1][2] - m[2][1]) * s;
		out[1] = (m[2][0] - m[0][2]) * s;
		out[2] = (m[0][1] - m[1][0]) * s;
	}
	else {
		int i = 0;
		if (m[1][1] > m[0][0]) i = 1;
		if (m[2][2] > m[i][i]) i = 2;

		int j = (i+1)%3;
		int k = (j+1)%3;

		float s = static_cast<float>(sqrt((m[i][i] - (m[j][j] + m[k][k])) + m[3][3]));

		out[i] = s * 0.5f;
		s = 0.5f / s;

		out[3] = (m[j][k] - m[k][j]) * s;
		out[j] = (m[i][j] + m[j][i]) * s;
		out[k] = (m[i][k] + m[k][i]) * s;
	}

	if (m[3][3] != 1.0f)
	{
		float fTmp=(1.0f/static_cast<float>(sqrt(m[3][3])));
		out[0]*=fTmp;
		out[1]*=fTmp;
		out[2]*=fTmp;
		out[3]*=fTmp;
	}
}
/******* Decompose Affine Matrix *******/

/* Decompose 4x4 affine matrix A as TFRUK(U transpose), where t contains the
 * translation components, q contains the rotation R, u contains U, k contains
 * scale factors, and f contains the sign of the determinant.
 * Assumes A transforms column vectors in right-handed coordinates.
 * See Ken Shoemake and Tom Duff. Matrix Animation and Polar Decomposition.
 * Proceedings of Graphics Interface 1992.
 */
void SbMatrixP::decomp_affine(SbMatrixP::HMatrix A, SbMatrixP::AffineParts * parts)
{
	SbMatrixP::HMatrix Q, S, U;
	float p[4];
	parts->t[0]=A[X][W];
	parts->t[1]=A[Y][W];
	parts->t[2]=A[Z][W];
	parts->t[3]=0;
	float det = polar_decomp(A, Q, S);
	if (det<0.0) {
		sp_mat_copy(Q, =, -Q, 3);
		parts->f = -1;
	}
	else parts->f = 1;

	// Transpose for our code (we use OpenGL's convention for numbering
	// rows and columns).
	float TQ[4][4];
	memcpy(TQ,Q,sizeof(float)*4*4);
	//(reinterpret_cast<const SbMat *>(&Q));
	transpose(TQ);
	matToRotation(TQ,parts->q);
	spect_decomp(S, U,parts->k );
	// Transpose for our code (we use OpenGL's convention for numbering
	// rows and columns).
	float TU[4][4];
	memcpy(TU,U,sizeof(float)*4*4);
	//(reinterpret_cast<const SbMat *>(&U));
	transpose(TU);
	matToRotation(TU,parts->u);
	snuggle(parts->u, parts->k,p);
	float tmp[4];
	multiply(p , parts->u,tmp);
	memcpy(parts->u ,tmp,sizeof(float)*4); 
}
/******* Polar Decomposition *******/

/* Polar Decomposition of 3x3 matrix in 4x4,
 * M = QS.  See Nicholas Higham and Robert S. Schreiber,
 * Fast Polar Decomposition of An Arbitrary Matrix,
 * Technical Report 88-942, October 1988,
 * Department of Computer Science, Cornell University.
 */
float
SbMatrixP::polar_decomp(SbMatrixP::HMatrix M, SbMatrixP::HMatrix Q, SbMatrixP::HMatrix S)
{
#define TOL 1.0e-6
  SbMatrixP::HMatrix Mk, MadjTk, Ek;
  float det, M_one, M_inf, MadjT_one, MadjT_inf, E_one, gamma, g1, g2;
  int i, j;
  sp_mat_tpose(Mk, =, M, 3);
  M_one = norm_one(Mk);  M_inf = norm_inf(Mk);
  do {
    adjoint_transpose(Mk, MadjTk);
    det = vdot(Mk[0], MadjTk[0]);
    if (det==0.0) {do_rank2(Mk, MadjTk, Mk); break;}
    MadjT_one = norm_one(MadjTk); MadjT_inf = norm_inf(MadjTk);
    gamma = static_cast<float>(sqrt(sqrt((MadjT_one*MadjT_inf)/(M_one*M_inf))/fabs(det)));
    g1 = gamma*0.5f;
    g2 = 0.5f/(gamma*det);
    sp_mat_copy(Ek, =, Mk, 3);
    sp_mat_binop(Mk, =, g1*Mk, +, g2*MadjTk, 3);
    sp_mat_copy(Ek, -=, Mk, 3);
    E_one = norm_one(Ek);
    M_one = norm_one(Mk);  M_inf = norm_inf(Mk);
  } while (E_one>(M_one*TOL));
  sp_mat_tpose(Q, =, Mk, 3); sp_mat_pad(Q);
  SbMatrixP::mat_mult(Mk, M, S);    sp_mat_pad(S);
  for (i=0; i<3; i++) for (j=i; j<3; j++)
    S[i][j] = S[j][i] = 0.5f*(S[i][j]+S[j][i]);
  return (det);

#undef TOL
}

#undef sp_mat_pad
#undef sp_mat_copy
#undef sp_mat_tpose
#undef sp_mat_binop
void MatrixOperation::getTransform(float fMtxAA[4][4],float fPos[3], float fRotate[4], float fScale[3], float fSo[4]) 
{
	SbMatrixP::AffineParts parts;
	SbMatrixP::HMatrix hmatrix;

	// transpose-copy
	hmatrix[0][0] = fMtxAA[0][0];
	hmatrix[0][1] = fMtxAA[1][0];
	hmatrix[0][2] = fMtxAA[2][0];
	hmatrix[0][3] = fMtxAA[3][0];

	hmatrix[1][0] = fMtxAA[0][1];
	hmatrix[1][1] = fMtxAA[1][1];
	hmatrix[1][2] = fMtxAA[2][1];
	hmatrix[1][3] = fMtxAA[3][1];

	hmatrix[2][0] = fMtxAA[0][2];
	hmatrix[2][1] = fMtxAA[1][2];
	hmatrix[2][2] = fMtxAA[2][2];
	hmatrix[2][3] = fMtxAA[3][2];

	hmatrix[3][0] = fMtxAA[0][3];
	hmatrix[3][1] = fMtxAA[1][3];
	hmatrix[3][2] = fMtxAA[2][3];
	hmatrix[3][3] = fMtxAA[3][3];

	SbMatrixP::decomp_affine(hmatrix, &parts);

	float mul = 1.0f;
	if (parts.t[SbMatrixP::W] != 0.0f) mul = 1.0f / parts.t[SbMatrixP::W];
	fPos[0] = parts.t[SbMatrixP::X] * mul;
	fPos[1] = parts.t[SbMatrixP::Y] * mul;
	fPos[2] = parts.t[SbMatrixP::Z] * mul;

	memcpy(fRotate,parts.q,sizeof(float)*4);
	mul = 1.0f;
	if (parts.k[SbMatrixP::W] != 0.0f) mul = 1.0f / parts.k[SbMatrixP::W];
	// mul be sign of determinant to support negative scales.
	mul *= parts.f;
	fScale[0] = parts.k[SbMatrixP::X] * mul;
	fScale[1] = parts.k[SbMatrixP::Y] * mul;
	fScale[2] = parts.k[SbMatrixP::Z] * mul;

	memcpy(fSo,parts.u,sizeof(float)*4);
}
