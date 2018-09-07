/**********************************************************
*		Created on 2013/12/25 By gongcy
*	All Rights Reserved. Copyright(C) 2013 Sinovation.com
*
*	Function:
*		矩阵数学库函数
*	更新：
		
**********************************************************/
#include "StdAfx.h"
#include "MathMatrix.h"
#include "complex"

#include "math.h"
#include "float.h"

#define		EPS	(1.0E-18)	

#define		ZMAT1(r,c)	\
	(*(inmx1_p+4*r  )) * (*(inmx2_p   +c))+ \
	(*(inmx1_p+4*r+1)) * (*(inmx2_p+ 4+c))+ \
	(*(inmx1_p+4*r+2)) * (*(inmx2_p+ 8+c))+ \
	(*(inmx1_p+4*r+3)) * (*(inmx2_p+12+c))
#define		ZMAT2(i)	\
	(*(inmx1_p+4*i  )) * (*(inmx2_p  ))+ \
	(*(inmx1_p+4*i+1)) * (*(inmx2_p+1))+ \
	(*(inmx1_p+4*i+2)) * (*(inmx2_p+2))+ \
	(*(inmx1_p+4*i+3)) * (*(inmx2_p+3))


CMathMatrix::CMathMatrix(void)
{
}

CMathMatrix::~CMathMatrix(void)
{
}


int	CMathMatrix::Minus(int di, int dj, double *inmx1_p, double *inmx2_p, double *difmx_p)
{
	int		status = 0;
	register int	i = 0,j = 0;

	status = 0;

	if (di < 1) 
	{
		status = -1;
		return status;
	}

	if (dj < 1) {
		status = -2;
		return status;
	}

	for (i=0; i<di; i++) 
	{
		for (j=0; j<dj; j++) 
			difmx_p[dj*i+j] = inmx1_p[dj*i+j] - inmx2_p[dj*i+j];
		
	}
}


int CMathMatrix::Multiply(int di1, int dj1, double *inmx1_p, int di2, int dj2, double *inmx2_p, double *intamx_p)
{
	double		sum = 0;		
	int			status = 0;
	register int	i = 0,j = 0,k = 0;		


	if (di1 < 1) 
	{
		status = -1;
		return status;
	}

	if (dj1 < 1) 
	{
		status = -2;
		return status;
	}

	if (di2 < 1) 
	{
		status = -3;
		return status;
	}

	if (dj2 < 1) 
	{
		status = -4;
		return status;
	}

	if (dj1 != di2)
	{
		status = -5;
		return status;
	}

	if (di1 == 4 && dj1 == 4 && dj2 == 4) 
	{
		intamx_p[0] = ZMAT1(0,0);
		intamx_p[1] = ZMAT1(0,1);
		intamx_p[2] = ZMAT1(0,2);
		intamx_p[3] = ZMAT1(0,3);
		intamx_p[4] = ZMAT1(1,0);
		intamx_p[5] = ZMAT1(1,1);
		intamx_p[6] = ZMAT1(1,2);
		intamx_p[7] = ZMAT1(1,3);
		intamx_p[8] = ZMAT1(2,0);
		intamx_p[9] = ZMAT1(2,1);
		intamx_p[10] = ZMAT1(2,2);
		intamx_p[11] = ZMAT1(2,3);
		intamx_p[12] = ZMAT1(3,0);
		intamx_p[13] = ZMAT1(3,1);
		intamx_p[14] = ZMAT1(3,2);
		intamx_p[15] = ZMAT1(3,3);
	}
	else if (di1 == 4 && dj1 == 4 && dj2 == 1) 
	{
		intamx_p[0] = ZMAT2(0);
		intamx_p[1] = ZMAT2(1);
		intamx_p[2] = ZMAT2(2);
		intamx_p[3] = ZMAT2(3);
	}
	else 
	{
		for (i=0; i<di1; i++) 
		{
			for (j=0; j<dj2; j++) 
			{
				sum = 0.0;
				for(k=0; k<di2; k++) 
					sum += inmx1_p[dj1*i+k] * inmx2_p[dj2*k+j];
				intamx_p[dj2*i+j] = sum;
			}
		}
	}

	return status;
}



int	CMathMatrix::Inverse(int dij, double *inmx_p, double *invmx_p)
{
	int		*ip;		
	double		*wk;	
	double		*wk2;	
	int		in_ip[10];	
	double		in_wk[10]  = {};	
	double		in_wk2[10] = {};	
	double		eps = 0;		
	int		i1 = 0,kp = 0;		
	int		tmp = 0;		
	double		pivot = 0,s,aw = 0;	
	double		amax = 0,diag = 0;	
	int		status = 0;
	register int	i = 0,j = 0 ,k = 0;	
	int		dwi = 0,dwj = 0,dwk = 0,dwi1 = 0;

	eps = DBL_MIN;

	if (dij <= 10) 
	{
		ip = in_ip;
		wk = in_wk;
		wk2 = in_wk2;
	}

	tmp = dij * dij;
	for (i=0; i<tmp; i++)  	invmx_p[i] = inmx_p[i];
	
	for (i=0; i<dij; i++)  wk[i] = 0.0;
	
	for (j=0; j<dij; j++) 
	{
		for (i=0, dwi=0; i<dij; i++, dwi+=dij) 
		{
			if (fabs(invmx_p[dwi+j]) > wk[i]) wk[i] = fabs(invmx_p[dwi+j]);
		}
	}

	for (i=0; i<dij; i++) 
		if(wk[i] != 0.0)  wk[i] = 1.0/wk[i];
	

	for (k=0, dwk=0; k<dij; k++,dwk+=dij) 
	{
		kp = k;
		amax = fabs(invmx_p[dwk+k]) * wk[k];
		if (k != dij-1) 
		{
			for(i=k+1,dwi=(k+1)*dij; i<dij; i++, dwi+=dij) 
			{
				aw = fabs(invmx_p[dwi+k]) * wk[i];
				if (aw > amax) 
				{
					kp = i;
					amax = aw;
				}
			}
		}
		ip[k] = kp;
		if (kp != k) 
		{
			aw = wk[kp];
			wk[kp] = wk[k];
			wk[k] = aw;
			dwi = dij*kp;
			for (j=0; j<dij; j++) 
			{
				aw = invmx_p[dwi+j];
				invmx_p[dwi+j] = invmx_p[dwk+j];
				invmx_p[dwk+j] = aw;
			}
		}
		invmx_p[dwk+k] = 1.0 / invmx_p[dwk+k];
		if (k != dij-1) 
		{
			pivot = invmx_p[dwk+k];
			for (i= k+1,dwi=(k+1)*dij; i<dij; i++,dwi+=dij) 
				invmx_p[dwi+k] = invmx_p[dwi+k] * pivot;
			
			if (k != dij-1) 
			{
				for (j=k+1; j<dij; j++) 
				{
					aw = -invmx_p[dwk+j];
					for (i=k+1,dwi=(k+1)*dij; i<dij; i++,dwi+=dij) 
						invmx_p[dwi+j] = invmx_p[dwi+j] + aw * invmx_p[dwi+k];
				}
			}
		}
	}

	for (k=dij-2,dwk=(dij-2)*dij; k>= 0; k--,dwk -= dij) 
	{
		for (j=k+1; j<dij; j++) wk[j] = 0.0;
		
		for (i=k+1,dwi=(k+1)*dij; i<dij; i++,dwi+=dij) 
		{
			for (j=k+1,dwj=(k+1)*dij; j<dij; j++,dwj+=dij) 
				wk[j] = wk[j] -  invmx_p[dwi+k] * invmx_p[dwj+i];
			wk2[i] = invmx_p[dwk+i];
		}
		for (j=k+1,dwj=(k+1)*dij; j<dij; j++,dwj+=dij)  invmx_p[dwj+k] = wk[j];
		s = 0.0;
		for (i=k+1; i<dij; i++) s = s + wk[i] * wk2[i];
		
		diag = invmx_p[dwk+k];
		invmx_p[dwk+k] = (1.0 - s) * diag;
		for (j=k+1; j<dij; j++) 
		{
			s = 0.0;
			for(i = k+1,dwi=(k+1)*dij; i<dij; i++,dwi+=dij) 	s = s + wk2[i] * invmx_p[dwi+j];
			invmx_p[dwk+j] = -s * diag;
		}
	}

	for (k=dij-2; k>=0; k--) 
	{
		if (ip[k] != k) 
		{
			for (i1=0,dwi1=0; i1<dij; i1++,dwi1+=dij)
			{
				aw = invmx_p[dwi1+ip[k]];
				invmx_p[dwi1+ip[k]] = invmx_p[dwi1+k];
				invmx_p[dwi1+k] = aw;
			}
		}
	}
	return(status);
}	



int	CMathMatrix::Determinant(int dij, double *inmx_p, double *det_p)
{
	double		*wk;		
	double		in_wk[16];	
	double		cof,tmp;	
	double		con,w;		
	int		status;
	register int	i,j,k,m;	

	status = 0;

	if (dij < 1) 
	{
		status = -1;
		return status;
	}

	if (dij <= 4)  wk = in_wk;
	

	tmp = dij * dij;
	for (i=0; i<tmp; i++)  	wk[i] = inmx_p[i];
	

	con = 1.0;
	for (i=0; i<dij; i++) 
	{
		k = i;
		for (m=k+1; m<dij; m++)
			if (fabs(wk[dij*i+k]) < fabs(wk[dij*i+m])) k = m;
		if (fabs(wk[dij*i+k]) > EPS) 
		{
			if (k != i) 
			{	
				for (m=i; m<dij; m++) 
				{
					w =  wk[dij*m+i];
					wk[dij*m+i] = 
						wk[dij*m+k];
					wk[dij*m+k] = w;
				}
				con = -con;
			}
		} 
		for (j=i+1; j<dij; j++) 
		{
			cof = wk[dij*i+j] / wk[dij*i+i];
			for (k=i; k<dij; k++)  wk[dij*k+j] -= cof * wk[dij*k+i];
			
		}
	}

	*det_p = 1.0;
	for (i=0; i<dij;i++)  *det_p *= wk[dij*i+i];
	*det_p *= con;
	return status;
}	



int	CMathMatrix::Transpose(double matrix[4][4], double matinv[4][4])
{
	double	maximum = 0.0;	
	double	factor  = 0.0;		
	double	*row, *row1;
	double	mat[4][4];	
	double	work[4] = {};	
	int	status=0,size = 0,m = 0;		
	register int	i = 0, j = 0, k = 0;	

	size = 4 * sizeof(double);

	(VOID)memcpy((CHAR *)mat, (CHAR *)matrix, 4 * size);

	for (i = 0; i < 4; i++) 
		for (j = 0; j < 4; j++) 	matinv[i][j] = 0.0;
		
	
	for (i = 0; i < 4; i++)	matinv[i][i] = 1.0;
	

	for (j = 0; j < 4; j++) 
	{
		m = j;
		maximum = mat[j][j];
		for (i = j+1; i < 4; i++) 
		{
			if (fabs(maximum) < fabs(mat[i][j])) 
			{
				m = i;
				maximum = mat[i][j];
			}
		}

		if (fabs(maximum) < EPS) 
		{
			status = -2;
			return status;
		}

		if (m != j) 
		{
			row = mat[j];
			row1 = mat[m];
			(VOID)memcpy((CHAR *)work, (CHAR *)row, size);
			(VOID)memcpy((CHAR *)row, (CHAR *)row1, size);
			(VOID)memcpy((CHAR *)row1, (CHAR *)work, size);

			row = matinv[j];
			row1 = matinv[m];
			(VOID)memcpy((CHAR *)work, (CHAR *)row, size);
			(VOID)memcpy((CHAR *)row, (CHAR *)row1, size);
			(VOID)memcpy((CHAR *)row1, (CHAR *)work, size);
		}

		row = mat[j];
		for (i = j+1; i < 4; i++) 	row[i] /= maximum;

		row = matinv[j];
		for (i = 0; i < 4; i++) 	row[i] /= maximum;
		

		for (i = 0; i < 4; i++) 
		{
			if (i != j) 
			{
				factor = mat[i][j];
				for (k = j+1; k < 4; k++) 
					mat[i][k] -= mat[j][k] * factor;
				
				for (k = 0; k < 4; k++) 
					matinv[i][k] -= matinv[j][k] * factor;
				
			}
		}
	}
	return status;
}
