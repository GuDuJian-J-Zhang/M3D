/**********************************************************
*		Created on 2013/12/26 By gongcy
*	All Rights Reserved. Copyright(C) 2013 Sinovation.com
*
*	Function:
*		矩阵变换库函数
*	更新：
		
**********************************************************/
#include "StdAfx.h"
#include "MathTransform.h"
#include "MathMatrix.h"
#include "MathVector.h"
#include "complex"

#define	X		(0)
#define	Y		(1)
#define	Z		(2)
#define	DIM3		(3)
#define	DIM4		(4)


CMathTransform::CMathTransform(void)
{
}

CMathTransform::~CMathTransform(void)
{
}

int	CMathTransform::CalMatrixByPntDirAndAngle(int dim, double point[3], double nvec[3], double rad, double matrix[4][4], double matinv[4][4])
{
	int	status = 0;
	double	length = 0,p = 0, q = 0, r = 0;
	double	unitvt[3] = {};
	double	n_vec[4][4];
	double	mtx_1[4][4];
	double	mtx_2[4][4];
	double	mtx_3[4][4];
	int	di1 = 0, dj1 = 0, di2 = 0, dj2 = 0;
	int	dij = 0;
	double	s_rad = 0,c_rad = 0;
	int	i = 0, j = 0;

	if((dim != 2) && (dim != 3))
	{
		status = -1;
		return status;
	}

	if(dim == 3)
	{
		if(CMathVector::LengthAndUnit(dim, nvec, &length, unitvt) < 0)
		{
			status = -2;
			return status;
		}
	}
	else
	{ 
		unitvt[0] = unitvt[1] = 0;
		unitvt[2] = 1;
	}

	p = unitvt[0];
	q = unitvt[1];
	r = unitvt[2];
	s_rad = sin(rad);
	c_rad = cos(rad);
	n_vec[0][0] = p*p+(1-p*p)*c_rad;
	n_vec[1][0] = p*q*(1-c_rad)+r*s_rad;
	n_vec[2][0] = p*r*(1-c_rad)-q*s_rad;
	n_vec[3][0] = 0;
	n_vec[0][1] = p*q*(1-c_rad)-r*s_rad;
	n_vec[1][1] = q*q+(1-q*q)*c_rad;
	n_vec[2][1] = q*r*(1-c_rad)+p*s_rad;
	n_vec[3][1] = 0;
	n_vec[0][2] = p*r*(1-c_rad)+q*s_rad;
	n_vec[1][2] = q*r*(1-c_rad)-p*s_rad;
	n_vec[2][2] = r*r+(1-r*r)*c_rad;
	n_vec[3][2] = 0;
	n_vec[0][3] = 0;
	n_vec[1][3] = 0;
	n_vec[2][3] = 0;
	n_vec[3][3] = 1;

	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
			mtx_1[j][i] = mtx_2[j][i] = 0;
		
	}
	for(i=0; i<4; i++)
		mtx_1[i][i] = mtx_2[i][i] = 1;
	
	for(i=0; i<3; i++)
	{
		mtx_1[i][3] = -(point[i]);
		mtx_2[i][3] = point[i];
	}

	di1 = dj1 = di2 = dj2 = 4;
	if(dim == 3)
	{
		CMathMatrix::Multiply(di1, dj1, (double *)&mtx_2[0][0], di2, dj2, (double *)&n_vec[0][0], (double *)&mtx_3[0][0]);
		CMathMatrix::Multiply(di1, dj1, (double *)&mtx_3[0][0], di2, dj2, (double *)&mtx_1[0][0], (double *)&matrix[0][0]);
	}
	else
	{	
		mtx_1[2][3] = mtx_2[2][3] = 0;	/* point[2] == z */
		n_vec[2][0] = n_vec[2][1] = n_vec[0][2] = n_vec[1][2] = 0;
		n_vec[2][2] = 1;
		CMathMatrix::Multiply(di1, dj1, (double *)&mtx_2[0][0], di2, dj2, (double *)&n_vec[0][0], (double *)&mtx_3[0][0]);
		CMathMatrix::Multiply(di1, dj1, (double *)&mtx_3[0][0], di2, dj2, (double *)&mtx_1[0][0], (double *)&matrix[0][0]);
	}
	dij = 4;
	if(CMathMatrix::Inverse(dij, (double *)&matrix[0][0], (double *)&matinv[0][0]) < 0)
	{
		status = -2;
		return status;
	}
	


	return  status ;
}

int CMathTransform::CalOffsetMatrix(int dim, double e1[3], double e2[3], double e3[3], double p1[3], double f1[3], double f2[3], double f3[3], double p2[3], double matrix[4][4], double matinv[4][4])
{
	int	status = 0;		
	double	e_mtx[4][4];	
	double	f_mtx[4][4];	
	double	f_inv[4][4];	
	int	di1 = 0, dj1 = 0, di2 = 0, dj2 = 0;
	int	dij = 0;		
	double	det_p = 0;	
	int	i = 0 ;		

	
	if((dim != 2) && (dim != 3))
	{
		status = -1;
		return status;
	}
	for(i=0; i<3; i++)
	{ 
		e_mtx[i][0] = e1[i];
		e_mtx[i][1] = e2[i];
		e_mtx[i][2] = e3[i];
		e_mtx[i][3] = p1[i];
		e_mtx[3][i] = 0;
	}
	e_mtx[3][3] = 1;

	for(i=0; i<3; i++)
	{ 
		f_mtx[i][0] = f1[i];
		f_mtx[i][1] = f2[i];
		f_mtx[i][2] = f3[i];
		f_mtx[i][3] = p2[i];
		f_mtx[3][i] = 0;
	}
	f_mtx[3][3] = 1;
	if(dim == 2)
	{
		for(i=0; i<4; i++)
		{
			e_mtx[2][i] = 0;
			f_mtx[2][i] = 0;
		}
		e_mtx[2][2] = 1;
		e_mtx[0][2] = e_mtx[1][2] = 0;
		f_mtx[2][2] = 1;
		f_mtx[0][2] = f_mtx[1][2] = 0;
	}
	dij = 4;
	if(CMathMatrix::Inverse(dij, (double *)&f_mtx[0][0], (double *)&f_inv[0][0]) < 0)
	{
		status = -2;
		return status;
	}
	di1 = dj1 = di2 =  dj2 = 4;
	CMathMatrix::Multiply(di1, dj1, (double *)&f_inv[0][0], di2, dj2, (double *)&e_mtx[0][0], (double *)&matrix[0][0]);
	dij = 4;
	if(CMathMatrix::Inverse(dij, (double *)&matrix[0][0], (double *)&matinv[0][0]) < 0)
	{
		status = -2;
		return status;
	}
	dij = 4;
	CMathMatrix::Determinant(dij, (double *)&matrix[0][0], &det_p);

	if(det_p < 0.0) status = 1;

	return  status ;
}

int	CMathTransform::CalMatrixByPntXZAxis (double point[3], double nvec[3], double base_x[3], double matrix[4][4], double matinv[4][4])
{

	int		status = 0,stat = 0;		
	BOOL	debug = FALSE;
	int		i= 0,j = 0;		
	double		lbase_x = 0,lnvec = 0;	
	double		ubase_x[DIM3] = {},ubase_y[DIM3] = {},unvec[DIM3] = {};	
	
	for( i=0; i<4; i++ )
	{
		for( j=0; j<4; j++ )
		{
			matrix[i][j] = 0.0;
			matinv[i][j] = 0.0;
		}
	}
	if ( CMathVector::LengthAndUnit( DIM3, nvec, &lnvec, unvec ) != 0 )
	{
		status = -1;
		return status;
	}

	if ( CMathVector::LengthAndUnit( DIM3, base_x, &lbase_x, ubase_x ) != 0 )
	{
		status = -1;
		return status;
	}

	CMathVector::Cross( unvec, ubase_x, ubase_y );

	for( i=0; i<DIM3; i++ )
	{
		matrix[0][i]	= ubase_x[i];
		matrix[1][i]	= ubase_y[i];
		matrix[2][i]	= unvec  [i];
	}
	matrix[0][3]	= -( ubase_x[0]*point[X]+ubase_x[1]*point[Y]+ubase_x[2]*point[Z] );
	matrix[1][3]	= -( ubase_y[0]*point[X]+ubase_y[1]*point[Y]+ubase_y[2]*point[Z] );
	matrix[2][3]	= -( unvec  [0]*point[X]+unvec  [1]*point[Y]+unvec  [2]*point[Z] );
	matrix[3][3]	= 1.0;

	stat	= CMathMatrix::Inverse( DIM4, (double *)&matrix[0][0], (double *)&matinv[0][0] );
	if ( stat < 0 )
	{
		status = -1;
		return status;
	}

	return status ;
}




int	CMathTransform::CalMatrixOffset(int dim, double e1[3], double e2[3], double e3[3], double p1[3], double f1[3], double f2[3], double f3[3], double p2[3], double matrix[4][4], double matinv[4][4])
{
	int	status = 0;		
	double	e_mtx[3][3];	
	double	e_inv[3][3];	
	double	f_mtx[3][3];	
	double	p1_mtx[3][1];	
	double	p2_mtx[3][1];	
	double	A_mtx[3][3];	
	double	C_mtx[3][1];	
	double	a_mtx[3][1];	
	int	di1 = 0, dj1= 0,di2= 0, dj2= 0;	
	int	dij = 0;		
	double	det_p = 0;	
	int	i = 0, j = 0;		
	
	if((dim != 2) && (dim != 3))
	{
		status = -1;
		return status;
	}

	for(i=0; i<3; i++)
	{
		e_mtx[i][0] = e1[i];
		e_mtx[i][1] = e2[i];
		e_mtx[i][2] = e3[i];
		p1_mtx[i][0] = p1[i];
		f_mtx[i][0] = f1[i];
		f_mtx[i][1] = f2[i];
		f_mtx[i][2] = f3[i];
		p2_mtx[i][0] = p2[i];
	}

	if(dim == 2)
	{
		for(i=0; i<2; i++)
		{
			e_mtx[2][i] = 0;
			e_mtx[i][2] = 0;
			f_mtx[2][i] = 0;
			f_mtx[i][2] = 0;
		}
		e_mtx[2][2] = f_mtx[2][2] = 1;
		p1_mtx[2][0] = p2_mtx[2][0] = 0; 

	}

	dij = 3;
	if(CMathMatrix::Inverse(dij, (double *)&e_mtx[0][0], (double *)&e_inv[0][0]) < 0)
	{
		status = -2;
		return status;
	}
	di1 = dj1 = di2 = dj2 = 3;
	CMathMatrix::Multiply(di1, dj1, (double *)&f_mtx[0][0], di2, dj2, (double *)&e_inv[0][0], (double *)&A_mtx[0][0]);

	di1 = dj1 = di2 = 3;
	dj2 = 1;
	CMathMatrix::Multiply(di1, dj1, (double *)&A_mtx[0][0], di2, dj2, (double *)&p1_mtx[0][0], (double *)&C_mtx[0][0]);

	di1 = 3;
	dj1 = 1;
	CMathMatrix::Minus(di1, dj1, (double *)&p2_mtx[0][0], (double *)&C_mtx[0][0], (double *)&a_mtx[0][0]);

	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)	matrix[i][j] = A_mtx[i][j];
		
	}
	for(i=0; i<3; i++)	matrix[i][3] = a_mtx[i][0];
	
	matrix[3][0] = matrix[3][1] = matrix[3][2] = 0;
	matrix[3][3] = 1;

	dij = 4;
	if(CMathMatrix::Inverse(dij, (double *)&matrix[0][0], (double *)&matinv[0][0]) < 0)
	{
		status = -2;
		return status;
	}

	dij = 4;
	status = CMathMatrix::Determinant(dij, (double *)&matrix[0][0], &det_p);

	if(det_p < 0.0)	status = 1;

	return  status ;
}

int	CMathTransform::WordPntToLocalPnt(int dim, int num_pnti, int rec_size, double pnti[], double t_mtr[4][4], double pnto[])
{
	int	status = 0;		
	int	i = 0, j = 0, k = 0;	
	double	wk_mtr_3[4][4];	
	double	work1[4] = {};
	double	work2[4] = {};
	int	di1 = 0, dj1 = 0,di2 = 0, dj2 = 0;	

	if((dim != 2) && (dim != 3))
	{	
		status = -1;
		return  status ;
	}
	if(num_pnti <= 0)
	{		
		status = -2;
		return  status ;
	}
	if(rec_size < dim)
	{		
		status = -3;
		return  status ;
	}
	for(i=0; i<(num_pnti*rec_size); i++)
		pnto[i] = pnti[i];
	
	for(i=0; i<DIM4; i++)
	{
		for(j=0; j<DIM4; j++)	wk_mtr_3[i][j] = t_mtr[i][j];
		
	}
	if( dim==2 )
	{
		wk_mtr_3[0][2] = 0.0;
		wk_mtr_3[1][2] = 0.0;
		wk_mtr_3[2][2] = 1.0;
		wk_mtr_3[2][0] = 0.0;
		wk_mtr_3[2][1] = 0.0;
	}
	j = 0;
	k = num_pnti;
	while((k--) != 0)
	{
		if(dim == 3)
		{
			di1 = dj1 = di2 = 4;
			dj2 = 1;
			work1[0] = pnti[j];
			work1[1] = pnti[j+1];
			work1[2] = pnti[j+2];
			work1[3] = 1.0;
			CMathMatrix::Multiply(di1, dj1, (double *)&wk_mtr_3[0][0], di2, dj2,work1, work2);
			pnto[j]   = work2[0];
			pnto[j+1] = work2[1];
			pnto[j+2] = work2[2];
		}
		else
		{	
			di1 = dj1 = di2 = 4;
			dj2 = 1;
			work1[0] = pnti[j];
			work1[1] = pnti[j+1];
			work1[2] = 0.0;
			work1[3] = 1.0;
			CMathMatrix::Multiply(di1, dj1, (double *)&wk_mtr_3[0][0], di2, dj2,work1, work2);
			pnto[j]   = work2[0];
			pnto[j+1] = work2[1];
		}
		j += rec_size;		
	}

	return ( status );
}


int	CMathTransform::WordDirToLocalDir(int dim, int num_vct, int rec_size, double vcti[], double t_mtr[4][4], double vcto[])
{
	int	status = 0;		
	int	i = 0, j = 0, k = 0;	
	double	wk_mtr_3[3][3];	
	double	wk_mtr_2[2][2];	
	double	wk_vcti_3[3][1];
	double	wk_vcti_2[2][1];
	double	wk_vcto_3[3][1];
	double	wk_vcto_2[2][1];
	int	di1 = 0, dj1 = 0,di2 = 0, dj2 = 0;	
	
	if((dim != 2) && (dim != 3))
	{	
		status = -1;
		return  status ;
	}
	if(num_vct <= 0)
	{
		status = -2;
		return  status ;
	}
	if(rec_size < dim)
	{	
		status = -3;
		return  status ;
	}
	(VOID)memcpy((CHAR *)vcto, (CHAR *)vcti,num_vct * rec_size * sizeof(double));
	for(i=0; i<dim; i++)
	{
		for(j=0; j<dim; j++)
		{
			if(dim == 3) wk_mtr_3[i][j] = t_mtr[i][j];
			else wk_mtr_2[i][j] = t_mtr[i][j];
		}
	}
	j = 0;
	k = num_vct;
	while((k--) != 0)
	{
		for(i=0; i<dim; i++)
		{
			if(dim == 3) wk_vcti_3[i][0] = vcti[j];
			else wk_vcti_2[i][0] = vcti[j];
			j++;
		}
		if(dim == 3)
		{
			di1 = dj1 = di2 = 3;
			dj2 = 1;
			CMathMatrix::Multiply(di1, dj1, (double *)&wk_mtr_3[0][0], di2, dj2,(double *)&wk_vcti_3[0][0], (double *)&wk_vcto_3[0][0]);
		}
		else
		{	
			di1 = dj1 = di2 = 2;
			dj2 = 1;
			CMathMatrix::Multiply(di1, dj1, (double *)&wk_mtr_2[0][0], di2, dj2,(double *)&wk_vcti_2[0][0], (double *)&wk_vcto_2[0][0]);
		}
		j -= dim;	
		for(i=0; i<dim; i++)
		{
			if(dim == 3)	vcto[j] = wk_vcto_3[i][0];
			else vcto[j] = wk_vcto_2[i][0];
			j++;
		}
		j += (rec_size - dim);
	}

	return  status ;
}

