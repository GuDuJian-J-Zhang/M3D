/**********************************************************
*		Created on 2013/12/28 By gongcy
*	All Rights Reserved. Copyright(C) 2013 Sinovation.com
*
*	Function:
*		矢量数学库函数
*	更新：
		
**********************************************************/
#include "StdAfx.h"
#include "MathVector.h"

#include <complex>
//#include "geolib.h"

#include "math.h"
#include "float.h"


#define		ZDEPS	(1.0e-18)	

#define		M_PI    (3.14159265358979323846)

#define 	R_OK	(0)
#define 	R_ERR	(-1)

#define		DIM		(2)
#define		DIM3	(3)

CMathVector::CMathVector(void)
{
}

CMathVector::~CMathVector(void)
{
}


int CMathVector::Sum(int dim, double invt1[], double invt2[], double univt[])
{

	int	status = R_OK;
	int	i = 0 ;

	if (dim <= 0) 
	{
		status = R_ERR;
		return status;
	}
	for(i = 0; i < dim; i++) 
		univt[i] = invt1[i] + invt2[i];

	return status;
}
int CMathVector::Minus(int dim, double invt1[], double invt2[], double difvt[])
{
	int	status = 0;		
	int	i = 0;		

	if( dim == 3 )
	{
		difvt[0] = invt1[0] - invt2[0];
		difvt[1] = invt1[1] - invt2[1];
		difvt[2] = invt1[2] - invt2[2];
		status = R_OK;
	}
	else if( dim == 2 )
	{
		difvt[0] = invt1[0] - invt2[0];
		difvt[1] = invt1[1] - invt2[1];
		status = R_OK;
	}
	else if( dim > 0 )
	{
		for( i=0; i<dim; i++ )
			difvt[i] = invt1[i] - invt2[i];
		status = R_OK;
	}
	else	status = R_ERR;	
	
	return status;
}


int CMathVector::Inner(int dim, double invt1[], double invt2[], double *inner_p)
{
	register int	i = 0;	
	double	inner   = 0.0;
	int	status = 0;		

	if( dim == 3 )
	{
		*inner_p = invt1[0] * invt2[0] + invt1[1] * invt2[1] + invt1[2] * invt2[2];
		status = R_OK;
	}
	else if( dim == 2 )
	{
		*inner_p = invt1[0] * invt2[0] + invt1[1] * invt2[1];
		status = R_OK;
	}
	else if( dim > 0 )
	{
		inner = 0.0;
		for( i=0; i<dim; i++ )	inner += invt1[i] * invt2[i];
		*inner_p = inner;
		status = R_OK;
	}
	else
	{
		*inner_p = 0.0;
		status = R_ERR;
	}
	return status;
}
int CMathVector::Cross(double invt1[3], double invt2[3], double outer[3])
{
	double	work[3] = {};	
	int	status = R_OK;

	work[0] = invt1[1] * invt2[2] - invt1[2] * invt2[1];
	work[1] = invt1[2] * invt2[0] - invt1[0] * invt2[2];
	work[2] = invt1[0] * invt2[1] - invt1[1] * invt2[0];

	outer[0] = work[0];
	outer[1] = work[1];
	outer[2] = work[2];


	return status ;
}



int CMathVector::LengthAndUnit(int dim, double invt[], double *length_p, double unitvt[])
{
	int	status = 0;		
	int	i = 0;		
	double	sum = 0,length = 0;

	if( dim == 3 )
	{
		sum = invt[0] * invt[0] + invt[1] * invt[1] + invt[2] * invt[2];	
		if( sum < ZDEPS )
		{
			*length_p = 0.0;
			unitvt[0] = invt[0];
			unitvt[1] = invt[1];
			unitvt[2] = invt[2];
			status = -2;
		}
		else
		{
			length = sqrt(sum);
			unitvt[0] = invt[0] / length;
			unitvt[1] = invt[1] / length;
			unitvt[2] = invt[2] / length;
			*length_p = length;
			status = 0;
		}
	}
	else if( dim == 2 )
	{
		sum = invt[0] * invt[0] + invt[1] * invt[1];
		if( sum < ZDEPS )
		{
			*length_p = 0.0;
			unitvt[0] = invt[0];
			unitvt[1] = invt[1];
			status = -2;
		}
		else
		{
			length = sqrt(sum);
			unitvt[0] = invt[0] / length;
			unitvt[1] = invt[1] / length;
			*length_p = length;
			status = 0;
		}
	}
	else if( dim > 0 )
	{
		sum = 0.0;
		for(i = 0; i < dim; i++) sum += invt[i] * invt[i];
		
		if( sum < ZDEPS )
		{
			*length_p = 0.0;
			for(i = 0; i < dim; i++) 	unitvt[i] = invt[i];
			status = -2;
		}
		else
		{
			length = sqrt(sum);
			for(i = 0; i < dim; i++)	unitvt[i] = invt[i] / length;
			*length_p = length;	
			status = 0;
		}
	}
	else status = -1;
	return(status);
}

int CMathVector::Angle(int dim, double invt1[], double invt2[], double *rad_p)
{

	double		len1 = 0,len2 = 0;	
	double		inn  = 0,con  = 0;	
	double		wk1  = 0,wk2  = 0;	
	int		status = 0;		
	register int	i = 0;		

	*rad_p = 0.0;

	if (dim < 1) 
	{
		status = -1;
		return status;
	}

	inn = 0.0;
	for (i=0; i<dim; i++) 
		inn += (invt1[i] * invt2[i]);
	

	wk1 = 0.0;wk2 = 0.0;
	for(i=0; i<dim; i++) 
	{
		wk1 += (invt1[i] * invt1[i]);
		wk2 += (invt2[i] * invt2[i]);
	}
	len1 = sqrt(wk1);
	len2 = sqrt(wk2);
	if (len1 <= DBL_MIN || len2 <= DBL_MIN || len1 * len2 <= DBL_MIN) 
	{
		status = -2;
		return status;
	}

	con = inn/(len1 * len2);

	if(con >= 1.0) *rad_p = 0.0;
	else if(con <= -1.0)  *rad_p = M_PI;
	else  *rad_p = acos(con);

	return(status);
}



int	CMathVector::Length(int dim, double invt[], double *length_p)
{
	int		i=0,rvalue = 0;
	double	len = 0.0;
	

	if( dim == 3 )
	{
		len = invt[0] * invt[0] + invt[1] * invt[1] + invt[2] * invt[2];
		*length_p = sqrt( len );
		rvalue = 0;
	}
	else if( dim == 2 )
	{
		len = invt[0] * invt[0] + invt[1] * invt[1];
		*length_p = sqrt( len );
		rvalue = 0;
	}
	else if( dim > 0 )
	{
		len = 0.0;
		for( i=0; i<dim; i++ ) 	len += invt[i] * invt[i];
		
		*length_p = sqrt( len );
		rvalue = 0;
	}
	else rvalue = -1;
	
	return( rvalue );
}




int  CMathVector::CrossLengthAndUnit(double invt1[3], double invt2[3], double *length_p, double nvct[3])
{
	double		outer[3] = {};	
	double		sum		= 0.0;
	double		length	= 0.0;
	int			status	= 0;		

	outer[0] = invt1[1] * invt2[2] - invt1[2] * invt2[1];
	outer[1] = invt1[2] * invt2[0] - invt1[0] * invt2[2];
	outer[2] = invt1[0] * invt2[1] - invt1[1] * invt2[0];

	sum = outer[0] * outer[0] + outer[1] * outer[1] + outer[2] * outer[2];
	if( sum < ZDEPS )
	{
		*length_p = 0.0;
		nvct[0] = outer[0];
		nvct[1] = outer[1];
		nvct[2] = outer[2];
		status = 1;
	}
	else
	{
		length = sqrt( sum );
		*length_p = length;
		nvct[0] = outer[0] / length;
		nvct[1] = outer[1] / length;
		nvct[2] = outer[2] / length;
		status = 0;
	}
	return(status);
}


int	CMathVector::MinusLengthAndUnit(int dim, double invt1[], double invt2[], double *length_p, double unitvt[])
{

	int	status;	
	register int	i;

	status = 0;


	if(dim < 1){
		status = -1;
		goto	rtn;
	}


	if(dim == 3) {
		unitvt[0] = invt1[0] - invt2[0];
		unitvt[1] = invt1[1] - invt2[1];
		unitvt[2] = invt1[2] - invt2[2];
		*length_p = sqrt(unitvt[0]*unitvt[0]
		+ unitvt[1]*unitvt[1]
		+ unitvt[2]*unitvt[2]);
	}
	else if(dim == 2) {
		unitvt[0] = invt1[0] - invt2[0];
		unitvt[1] = invt1[1] - invt2[1];
		*length_p = sqrt(unitvt[0]*unitvt[0] + unitvt[1]*unitvt[1]);
	}
	else {
		*length_p = 0.0;
		for(i=0;i<dim;i++) {
			unitvt[i] = invt1[i]-invt2[i];
			*length_p += unitvt[i]*unitvt[i];
		}
		*length_p = sqrt(*length_p);
	}

	if( *length_p < ZDEPS ) {
		status = 1;
		goto rtn;
	}

	if( dim == 3 ) {
		unitvt[0] = unitvt[0]/(*length_p);
		unitvt[1] = unitvt[1]/(*length_p);
		unitvt[2] = unitvt[2]/(*length_p);
	}
	else if( dim == 2 ) {
		unitvt[0] = unitvt[0]/(*length_p);
		unitvt[1] = unitvt[1]/(*length_p);
	}
	else {
		for(i=0;i<dim;i++) {
			unitvt[i] = unitvt[i]/(*length_p);
		}
	}


rtn:
	return(status);
}



int	CMathVector::Angle(double invt1[2], double invt2[2], double *rad_p)
{
	double	outz;	
	int	status;	
	int	ist;
	status = 0;
	*rad_p = 0.0;

	ist = Angle(DIM,invt1,invt2,rad_p);
	if(ist < 0){
		status = 1;
	}	
	else{
		outz = invt1[0]*invt2[1] - invt1[1]*invt2[0];
		if(outz < 0.0){
			*rad_p = 2.0*M_PI - *rad_p;
		}
	}

	return(status);
}



int	CMathVector::Angle(double invt1[3], double invt2[3], double refvt[3], double *rad_p)
{
	double	outer[3];	
	double	inner;		
	int	status;		
	int	ist;
	status = 0;
	*rad_p = 0.0;

	ist = Angle(DIM3,invt1,invt2,rad_p);
	if(ist < 0){
		status = 1;
	}
	else{
		Cross(invt1,invt2,outer);
		Inner(DIM3,refvt,outer,&inner);
		if(inner < 0.0){
			*rad_p = 2.0*M_PI - *rad_p;
		}
	}

	return(status);
}

int	CMathVector::ParallelBidirect(int dim, double unit1[], double unit2[], double tola)
{
	int	status;			
	int	stat2;			
	double	eps;		
	double	norm1;		
	double	norm2;		
	double	theta;		
	status = 1;
	eps    = DBL_MIN;
	if( dim < 1 ){
		status = -1;
		goto errrtn;
	}
	CMathVector::Length(dim, unit1, &norm1);
	CMathVector::Length(dim, unit2, &norm2);
	if( (norm1 < eps) || (norm2 < eps) ){
		status = 2;
		goto	rtn;
	}
	stat2 = CMathVector::Angle(dim, unit1, unit2, &theta);
	if( stat2 == -1 ){
		status = -1;
		goto	errrtn;
	}
	if( stat2 == -2 ){
		status = 2;
		goto	rtn;
	}
	theta = fabs(theta);
	if( (theta <=tola) || (fabs(M_PI - theta) <= tola) ){	
		status = 0;
	}
	else{					
		status = 1;
	}
	goto	rtn;	
errrtn:
	goto rtn;
rtn:
	return( status );
}


int	CMathVector::Parallel(int dim, double unit1[], double unit2[], double tola)
{
	/*
	 * 内部変数宣言
	 */
	int     status;                 
    int     stat2;                 
    double  eps;                  
	double	norm1;			
	double	norm2;			
	double	theta;		

	/*
	 * 初期設定
	 */
	status = 1;
	eps    = 2.2250738585072014e-308;
	
	if( dim < 1 )
	{
		return -1;
	}
	
	stat2 = Length(dim, unit1, &norm1);
	stat2 = Length(dim, unit2, &norm2);
	if( (norm1 < eps ) || (norm2 < eps))
	{
		return 2;
	}

	stat2 = Angle( dim, unit1,unit2, &theta );
	if( stat2 == -1 )
	{
		return -1;
	}
	if( stat2 == -2 )
	{
		return 2;
	}
	/*
	 *  平行判定（单方向）
	 */
	theta = fabs(theta);
	if( theta <= tola )
	{			
		status = 0;
	}
	else
	{					
		status = 1;
	}
	return status;

}
//判定2个单位矢量的直交性
int	CMathVector::Vertical(int dim, double unit1[], double unit2[], double tola)
{

	int	status = 0;			
	int	stat2  = 0;			
	double	eps = 0.0;		
	double	norm1 = 0.0;		
	double	norm2 = 0.0;		
	double	theta = 0.0;		

	status = 1;
	eps    = DBL_MIN;

	if( dim < 1 ){
		status = -1;
		goto errrtn;
	}
	stat2 = CMathVector::Length(dim, unit1, &norm1);
	stat2 = CMathVector::Length(dim, unit2, &norm2);
	if( (norm1 < eps) || (norm2 < eps) ){
		status = 2;
		goto	rtn;
	}
	stat2 = CMathVector::Angle(dim, unit1, unit2, &theta);
	if( stat2 == -1 ){
		status = -1;
		goto errrtn;
	}
	if( stat2 == -2){
		status = 2;
		goto	rtn;
	}
	theta = fabs( theta );
	if( fabs(M_PI/2 - theta) <= tola ){	
		status = 0;
	}
	else{						
		status = 1;
	}
	goto	rtn;	
errrtn:
	goto rtn;
rtn:
	return( status );
}



int	CMathVector::VerticalBidirect(int dim, double vect1[], double vect2[])
{
	int	status;		
	int	i;			
	double	eps;	

	double	norm1;	
	double	norm2;	
	double	df;		
	double	diff2;	

	double	diff_norm;	
	double	sm;			
	double	sum2;		

	double	sum_norm;	

	status = 1;
	eps    = 1.0e-10;		
	if( dim < 1 ){
		status = -1;
		goto rtn;
	}
	if( (CMathVector::Length(dim, vect1, &norm1)) < 0 ){
		status = -1;
		goto rtn;
	}
	if( (CMathVector::Length(dim, vect2, &norm2)) < 0 ){
		status = -1;
		goto rtn;
	}
	if( (norm1 <= eps) || (norm2 <= eps) ){
		status = 2;
		goto	rtn;
	}
	diff2 = 0.0;
	for( i=0; i < dim; i++ ){
		df = ( ( vect1[i]/norm1 ) - ( vect2[i]/norm2 ) );
		diff2 += df*df;
	}
	diff_norm = sqrt( diff2 );
	if( diff_norm <= eps ){
		status = 0;		 
		goto rtn; 
	}
	sum2 = 0.0;
	for( i=0; i < dim; i++ ){
		sm = ( ( vect1[i]/norm1 ) + ( vect2[i]/norm2 ) );
		sum2 += sm*sm;
	}
	sum_norm = sqrt( sum2 );
	if( sum_norm <= eps ){
		status = 0;		
	}
	else{
		status = 1;		
	}
	goto rtn; 
rtn:
	return( status );
}
