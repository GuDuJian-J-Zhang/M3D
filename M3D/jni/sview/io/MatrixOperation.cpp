// MatrixOperation.cpp
// 包含矩阵转置、求逆、相乘几个通用操作

#include "MatrixOperation.h"
#include "math.h"

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


void MatrixOperation::MatrixTranspose(float* MatrixOrigin, float* MatrixNew, int m, int n)
{
	int i;
	int j;
	for (i = 0; i != n; i++)
	{
		for (j = 0; j != m; j++)
		{
			MatrixNew[i*m + j] = MatrixOrigin[j*n + i];
		}
	}
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
