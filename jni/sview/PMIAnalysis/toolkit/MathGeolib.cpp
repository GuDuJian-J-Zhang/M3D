/**********************************************************
*		Created on 2014/01/20 By gongcy
*	All Rights Reserved. Copyright(C) 2014 Sinovation.com
*
*	Function:
*		数学库函数
*	更新：
		
**********************************************************/

#include "StdAfx.h"
#include "MathGeolib.h"
#include "MathVector.h"
#include <stdlib.h>
#include <math.h>
#include "m3d/base/MathDefs.h"

#define Sdistol		(0.01)
#define Sradtol		(0.00026)
#define	EPS	    (1.0e-5)	
#define		CTOL	(1.0E-5)	
#define	GOSA		(1.0e-10)
#define ZDIM3		(3)

#define	ZWONPLN		(1)	
#define	ZNORMAL		(0)
#define M_PI       3.14159265358979323846


CMathGeolib::CMathGeolib(void)
{
}

CMathGeolib::~CMathGeolib(void)
{
}

int CMathGeolib::CalPedalPntToLine( int dim , double	pnt[3] , int	dimlin , 
						 double	pntln[3],double dirln[3] , double told,double tola,
						 double	hpnt[3], double	*hprm_p, double	*hdst_p  ) 
{
			
	double	norm = 0.0;		
	double	qp[3] = {0.0,0.0,0.0};     	
	double	inner_vqp = 0.0;	
	double	inner_vv = 0.0;	
	double	eps = 0.0;		
	int		i = 0;		


	//eps = min( told / 100.0,EPS );
	eps =  (told / 100.0<EPS) ? (told / 100.0):EPS;
	norm = 0.0;
	CMathVector::Length( dim,dirln,&norm );
	if( norm <= eps )
	{
		if( dim == 2 && dimlin == 3 )
		{
			hpnt[0] = pntln[0];
			hpnt[1] = pntln[1];
			hpnt[2] = pntln[2];
			*hprm_p = 0.0;
			Cal2PntDis(dim,pnt,hpnt,hdst_p);
			return 1;
			
		}
		else  return -9;
		
	}
	CMathVector::Minus( dim,pnt,pntln,qp );
	CMathVector::Inner( dim,dirln,qp,&inner_vqp );
	CMathVector::Inner( dim,dirln,dirln,&inner_vv );
	*hprm_p = inner_vqp / inner_vv;

	for( i = 0; i < dimlin; i++ )
		hpnt[i] = pntln[i] + (*hprm_p)*dirln[i];

	Cal2PntDis( dim,pnt,hpnt,hdst_p );
	return 0;
}


// ------------------------------------------------
/**
 *	@brief	     [求指定点到指定线段的垂足]
 *	@param[in]	  [输入参数功能描述]
 *	@param[out]	  [输出参数功能描述]
 *	@retval		 [返回值说明]
 *	@note        [详细说明]	
 *	@attention   [注意事项说明]
 */
// ------------------------------------------------

int	CMathGeolib::CalPedalPntToSegment(int	dim,
		   double	pnt[3],		
		   int	dimln,			
		   double	spntln[3],	
		   double	epntln[3],	
		   double	told,		
		   double	tola,		
		   double	hpnt[3],	
		   double	*hprm_p,	
		   double	*hdst_p	)	
{
	int		stat;		
	double		length; 	
	double		unitvt[3];	

	stat = 1;

	stat = CMathVector::MinusLengthAndUnit( dimln, epntln, spntln, &length, unitvt );
	if( stat != 0 ) 
	{
		if( stat == 1 ) 
		{
			*hprm_p = 0.0;
			hpnt[0] = spntln[0];
			hpnt[1] = spntln[1];
			if( dim==3 )  hpnt[2] = spntln[2];
			
			stat = Cal2PntDis( dim, pnt, hpnt, hdst_p );
			if( stat != 0 )
			{
				stat = -9;
				return stat;
			}
			stat = 2;
			return stat;
		}
		else
		{
			stat = -9;
			return stat;
		}
	}
	stat = CalPedalPntToLine( dim, pnt, dimln, spntln, unitvt,told, tola, hpnt, hprm_p, hdst_p );
	if( stat == 0 )
	{
		stat = IsPntInSegment1( dim, pnt, dimln, spntln, epntln,told, tola, hpnt, hprm_p, hdst_p );
		if( stat == 1 ) stat = 1;
		else if( stat != 0 )
		{
			stat = -9;
			return stat;
		}
	}
	else if( stat == 1 )
	{
		*hprm_p = 0.0;
		stat = 2;
	}
	else stat = -9;
	return( stat );
}


int	CMathGeolib::CalPedalPntToPlane( double pnt[3], double plpnt[3], double plnrm[3],
		double told, double hpnt[3], double *hdst_p )
{
	int		status;
	register int	i;
	bool		debug;

	
 	status = 0;

	
	*hdst_p = 0.0;
	for ( i = 0 ; i < ZDIM3 ; i++ ) {
		*hdst_p += ( pnt[i] - plpnt[i] ) * plnrm[i];
	}

	for ( i = 0 ; i < ZDIM3 ; i++ ) {
		hpnt[i] = pnt[i] + (-(*hdst_p)) * plnrm[i];
	}

	/*
	 *  平面上判定
	 */
	if ( fabs( *hdst_p ) <= told ) {
		status = ZWONPLN;
	}
 	return( status );
}


// ------------------------------------------------
/**
 *	@brief	     [点到线有效范围判断]
 *	@param[in]	  [输入参数功能描述]
 *	@param[out]	  [输出参数功能描述]
 *	@retval		 [返回值说明]
 *	@note        [详细说明]	
 *	@attention   [注意事项说明]
 */
// ------------------------------------------------

int	CMathGeolib::IsPntInSegment1( 
int	dim,			
double	pnt[3],		
int	dimln,			
double	lin_sp[3],	
double	lin_ep[3],	
double	told,		
double	tola,		
double	hpnt[3],	
double	*hprm_p,	
double	*hdst_p)
{
	register int i = 0;	
	int			stat = 0;
	double 		dist_1 = 0;		
	double 		dist_2 = 0;		
	double 		oprm_p = 0;		
	double 		vec_1[3] = {};	
	double 		vec_2[3] = {};	

	stat = IsPntInSegment( dimln, *hprm_p, lin_sp, lin_ep, &oprm_p );
	if( stat == 0 ) *hprm_p = oprm_p;
	else
	{
		Cal2PntDis( dimln, lin_sp, hpnt, &dist_1 ); 
		Cal2PntDis( dimln, lin_ep, hpnt, &dist_2 );

		if( dist_1 <= told )
		{
			for( i=0; i<3; i++ )
				hpnt[i] = lin_sp[i];
			(*hprm_p) = 0.0;
		}
		else if( dist_2 <= told )
		{
			for( i=0; i<3; i++ ) hpnt[i] = lin_ep[i];
			(*hprm_p) = 1.0;
		}
		else return 1;
			
		CMathVector::Minus( dim, lin_ep, lin_sp, vec_1 );
		CMathVector::Minus( dim, pnt, hpnt, vec_2 );
		stat = CMathVector::Vertical( dim, vec_1, vec_2, tola );
 		if( stat == 2 )stat = 0;
		
		else if( stat < 0 )
		{
			stat = -9;
			return stat;
		}
		Cal2PntDis(dim, pnt, hpnt, hdst_p);
	}
	return( stat );
}
	

int	CMathGeolib::Cal2LineDis(int nstdim, int dim1, double pnt1[3], double dir1[3], int dim2, double pnt2[3], double dir2[3], double told, double tola, double nstinf[9])
{
	double	len1 = 0,len2 = 0,tol_sys = 0,hprm = 0,hdst = 0;		
	double	pp_uvec[3] = {},hpnt[3] = {};;	
	double	inn_a = 0,inn_b = 0,inn_c = 0,inn_d = 0,inn_e = 0;		
	double	equ[2][3];	
	double	solv[2] = {};	
	int	s_flag = 0,h_flag = 0;		
		
	int	status = 0;
	int	i=0,j=0,ist = 0;

	for (i=0;i<9;i++)  nstinf[i] = 0.0;
	

    tol_sys = M3D::Min((float)(told/100),(float)CTOL);
	tol_sys = (told/100<CTOL)?(told/100):CTOL;

	ist = CMathVector::Length(nstdim, dir1, &len1);
	if (ist < 0) 
	{
		status = -9;
		return status;
	}

	ist = CMathVector::Length(nstdim, dir2, &len2);
	if (ist < 0) 
	{
		status = -9;
		return status;
	}

	s_flag = 0;
	if (len1 <= tol_sys)  s_flag += 1;
	
	if (len2 <= tol_sys)  s_flag += 2;
	

	switch (s_flag) 
	{
	case	1 :
		ist = CMathGeolib::CalPedalPntToLine(nstdim, pnt1, dim2, pnt2, dir2, told, tola, hpnt, &hprm, &hdst);
		if (ist < 0) 
		{
			status = -9;
			return status;
		}

		for (i=0;i<dim1;i++) 	nstinf[i] = pnt1[i];
		
		nstinf[3] = 0.0;
		for (i=0;i<dim2;i++)  nstinf[4+i] = hpnt[i];
		
		nstinf[7] = hprm;
		nstinf[8] = hdst;

		status = 0;
		return status;

	case	2 :
		ist = CMathGeolib::CalPedalPntToLine(nstdim, pnt2, dim1,pnt1,dir1,told,tola,hpnt,&hprm,&hdst);
		if (ist < 0) 
		{
			status = -9;
			return status;
		}

		for (i=0;i<dim1;i++)  nstinf[i] = hpnt[i];
		
		nstinf[3] = hprm;
		for (i=0;i<dim2;i++)  nstinf[4+i] = pnt2[i];
		
		nstinf[7] = 0.0;
		nstinf[8] = hdst;

		status = 0;
		return status;

	case	3 :
		hdst = 0.0;
		for( i=0; i<nstdim; i++) {
			hdst += (pnt1[i] - pnt2[i])*(pnt1[i] - pnt2[i]);
		}
		if (hdst <= tol_sys*tol_sys) 	status = 2;
		else 	status = 1;

		return status;

	case	0 :
		h_flag = CMathVector::VerticalBidirect(nstdim, dir1, dir2);
		switch (h_flag) 
		{
			case 0 :
			{
				ist = CMathGeolib::CalPedalPntToLine(nstdim, pnt1, dim2, pnt2, dir2,told, tola, hpnt, &hprm, &hdst);
				if (ist < 0) 
				{
					status = -9;
					return status;
				}

				if (hdst <= told)  status = 2;

				else  status = 1;

				return status;
			}
			

			case 1 :
			{
				ist = CMathVector::Inner(nstdim, dir1, dir1, &inn_a);
				if (ist < 0) 
				{
					status = -9;
					return status;
				}

				ist = CMathVector::Inner(nstdim, dir1, dir2, &inn_b);
				if (ist < 0) {
					status = -9;
					return status;
				}
				ist = CMathVector::Inner(nstdim, dir2, dir2, &inn_c);
				if (ist < 0) 
				{
					status = -9;
					return status;
				}
				ist = CMathVector::Minus(nstdim, pnt1, pnt2, pp_uvec);
				if (ist < 0) {
					status = -9;
					return status;
				}
				ist = CMathVector::Inner(nstdim, dir1, pp_uvec, &inn_d);
				if (ist < 0) 
				{
					status = -9;
					return status;
				}
				ist = CMathVector::Inner(nstdim, dir2, pp_uvec, &inn_e);
				if (ist < 0) 
				{
					status = -9;
					return status;
				}
				for (i=0;i<2;i++)  for (j=0;j<3;j++) equ[i][j] = 0.0;
				

				equ[0][0] = -inn_a;
				equ[0][1] = inn_b;
				equ[0][2] = inn_d;
				equ[1][0] = -inn_b;
				equ[1][1] = inn_c;
				equ[1][2] = inn_e;

				ist = BinaryLinearEquation(equ, solv);
				if (ist != 0) 
				{ 
					ist = CMathGeolib::CalPedalPntToLine(nstdim, pnt1, dim2, pnt2, dir2,told, tola, hpnt, &hprm, &hdst);
					if (ist < 0) 
					{
						status = -9;
						return status;
					}

					if (hdst <= told)  status = 2;
					else   status = 1;

					return status;
				}
				for (i=0;i<dim1;i++)  nstinf[i] = pnt1[i] + solv[0] * dir1[i];

				nstinf[3] = solv[0];
				for (i=0;i<dim2;i++)  nstinf[4+i] = pnt2[i] + solv[1] * dir2[i];

				nstinf[7] = solv[1];

				ist = Cal2PntDis(nstdim, &nstinf[0], &nstinf[4],&nstinf[8]);
				if (ist < 0) 
				{
					status = -9;
					return status;
				}
				break;	
			}	
			default :
			{
				status = -9;
				return status;
			}
				
		}
		break;

	default :
		status = -9;
		return status;
	}

	status = 0;
	return(status);
}



int	CMathGeolib::Cal2LineIntersection( int xdim, int dim1, double pnt1[3], double dir1[3], 
		   int dim2, double pnt2[3], double dir2[3], 
		   double told, double tola, double xpinf[9] )
{
	int	status = 0;		
	int	ist = 0, i = 0;		
	
	for (i=0;i<9;i++) 	xpinf[i] = 0.0;

	ist = CMathGeolib::Cal2LineDis(xdim, dim1, pnt1, dir1, dim2, pnt2, dir2,told, tola, xpinf);
	if (ist < 0) 
	{
		status = -9;
		return status;
	}
	else if (ist == 2) 
	{
		status = 2;
		return status;
	}
	else if (ist == 1) 
	{
		for (i=0;i<dim1;i++) 	xpinf[i] = pnt1[i];
		
		xpinf[3]  = 0.0;
		ist = CMathGeolib::CalPedalPntToLine(xdim, pnt1, dim2, pnt2, dir2, told, tola,&xpinf[4], &xpinf[7], &xpinf[8]);
		if (ist < 0) 
		{
			status = -9;
			return status;
		}
	}

	if (xpinf[8] <= told)  status = 0;
	else  status = 1;

	return(status);
}


int	CMathGeolib::CalLineToPlaneIntersection(double lin_pnt[3], double lin_dir[3], double pln_pnt[3], double pln_nrm[3], double told, double tola, double xpnt[3], double *xprm_p)
{
	int	stat = 0,zkstat = 0,zlstat = 0;		
	double vect[3] = {};
	double innert = 0,innera = 0,dist = 0;

	*xprm_p = (double)0.0;

	CMathVector::Minus( 3, pln_pnt, lin_pnt, vect );
	CMathVector::Inner( 3, pln_nrm, vect, &innert );

	zkstat = CMathVector::Vertical( 3, lin_dir, pln_nrm, tola );
	if( zkstat == 0 )
	{		
		if( abs (innert) <= told )
		{

			stat = 2;	
			return stat;
		}
		else
		{
			stat = 1;	
			return stat;
		}
	}

	CMathVector::Inner( 3, lin_dir, pln_nrm, &innera );
	dist =  innert / innera ;

	zlstat = CalNewPntByDirAndDis( 3, lin_pnt, lin_dir, lin_pnt, 
		dist, xpnt, xprm_p );
	return  stat ;
}



int	CMathGeolib::IsPntInSegment(int dim, double prm, double lin_sp[3], double lin_ep[3], double *oprm_p)
{
	int	stat = 0;		
	double	lin_dst = 0;	


	Cal2PntDis( dim, lin_sp, lin_ep, &lin_dst );

	if( 0.0 <= prm && prm <= lin_dst )
	{ 	
		if(lin_dst == 0.0) (*oprm_p) = 0.0;
		
		else (*oprm_p) = prm / lin_dst;
		
		stat = 0;		
	}
	else stat = 1;	
	return( stat );
}
int	CMathGeolib::Cal2PntDis(int dim, double point1[3], double point2[3], double *distance_p)
{
	int	status;
	status = 0;
	if ( dim == 2 ) 
	{
		*distance_p = sqrt( 
			(*(point1)  - *(point2))  *( *(point1)  - *(point2)) +
			(*(point1+1)- *(point2+1))*( *(point1+1)- *(point2+1))
			);
	}
	else if ( dim == 3 ) 
	{
#if CRESTAMVER >= 0x0700
		PP1 = *(point1) - *(point2);
		PP2 = *(point1+1)- *(point2+1);
		PP3 = *(point1+2)- *(point2+2);
		work = (PP1 * PP1);
		work += (PP2 * PP2);
		work += (PP3 * PP3);
		*distance_p = Bsqrt( work );
#else
		*distance_p = sqrt( 
			(*(point1)  - *(point2))  *( *(point1)  - *(point2))   +
			(*(point1+1)- *(point2+1))*( *(point1+1)- *(point2+1)) +
			(*(point1+2)- *(point2+2))*( *(point1+2)- *(point2+2))
			);
#endif
	}
	else  status = -1;
	
	return(status);
}

int	CMathGeolib::BinaryLinearEquation(double equ[2][3], double solv[2])
{
	int	status = 0;				
	double	det = 0;		

	det = equ[0][0]*equ[1][1] - equ[0][1]*equ[1][0];
	if( fabs(det) < GOSA )
	{
		status = 1;
		return status; 
	}
	solv[0] = ( equ[1][1]*equ[0][2] - equ[0][1]*equ[1][2] )/det;
	solv[1] = ( equ[0][0]*equ[1][2] - equ[1][0]*equ[0][2] )/det;	
	return( status );
}



int	CMathGeolib::CalNewPntByDirAndDis(int dim, double lin_pnt[3], double lin_dir[3], double pnt1[3], double dist, double pnt2[3], double *prm2_p)
{
	int	stat = 0;		
	int	i = 0;	
	double	vec[3] = {};	
	double	dsts = 0;

			
	(*prm2_p) = (double)0.0;

	if(( stat = CMathVector::Minus( dim, pnt1, lin_pnt, vec )) != 0 )
	{
		stat = -9;
		return stat;
	}
	if(( stat = CMathVector::Inner( dim, vec, lin_dir, &dsts )) != 0 )
	{
		stat = -9;
		return stat;
	}

	(*prm2_p) = dsts + dist;
	for( i = 0; i < dim; i++ )
		pnt2[i] = lin_pnt[i] + ( (*prm2_p) * lin_dir[i] );

	return( stat );
} 

int   CMathGeolib::movPnt(int dim, /*VECTOR*/double *ilnvecP, /*POint3*/double *pntP, double ilndst, /*POint3*/double *movpntP)
{
	int         ret = 0;         	
	int         stat = 0;           
			
	double		dummy = 0;			
	double		ilnunitvtA[3] = {};	


	if (( dim != 2 ) && ( dim != 3 )) return -1;

	
	stat = CMathVector::LengthAndUnit(dim, ilnvecP, &dummy, ilnunitvtA);
	if ( stat != 0 ) return -2;

	if ( 2  == dim) 
	{
		movpntP[0] = pntP[0] + ilndst * ilnunitvtA[0];
		movpntP[1] = pntP[1] + ilndst * ilnunitvtA[1];
		movpntP[2] = 0.0;
	} 
	else 
	{
		movpntP[0] = pntP[0] + ilndst * ilnunitvtA[0];
		movpntP[1] = pntP[1] + ilndst * ilnunitvtA[1];
		movpntP[2] = pntP[2] + ilndst * ilnunitvtA[2];
	}

	return 0;

}
int CMathGeolib::GetAngleDimData(
	double i_Element1[2][3],			// 要素1的始终点
	double i_Element2[2][3],			// 要素2的始终点
	double i_prjPlnpnt[3],				// 基于PMI平面的拾取点的坐标值
	int &o_ExtendLineFlag,				// 延长线标志 = 0 没有延长线；
										// = 1 有一条延长线且是第一条；
										// = 2 有一条延长线且是第二条；
										// = 3 有两条延长线
	double o_ExtendLine[4][3],			// 延长线点列
	double &o_ComTexValue,				// 文本数据
	double o_fInnterPnt[3]				// 两要素的交点
)
{
	o_ExtendLineFlag = 0;
	double basePoint1[3], basePoint2[3];
	basePoint1[0] = i_Element1[0][0];
	basePoint1[1] = i_Element1[0][1];
	basePoint1[2] = i_Element1[0][2];

	double vecElenment1[3], vecElenment2[3];
	vecElenment1[0] = i_Element1[1][0] - i_Element1[0][0];
	vecElenment1[1] = i_Element1[1][1] - i_Element1[0][1];
	vecElenment1[2] = i_Element1[1][2] - i_Element1[0][2];
	
	double TempLen = 0.0;
	int ret = 0;
	ret = CMathVector::LengthAndUnit(ZDIM3, vecElenment1, &TempLen, vecElenment1);
	if (ret < 0)
	{
		return ret;
	}

	basePoint2[0] = i_Element2[0][0];
	basePoint2[1] = i_Element2[0][1];
	basePoint2[2] = i_Element2[0][2];
	vecElenment2[0] = i_Element2[1][0] - i_Element2[0][0];
	vecElenment2[1] = i_Element2[1][1] - i_Element2[0][1];
	vecElenment2[2] = i_Element2[1][2] - i_Element2[0][2];
	ret = CMathVector::LengthAndUnit(ZDIM3, vecElenment2, &TempLen, vecElenment2);
	if (ret < 0)
	{
		return ret;
	}
	// 计算两线段的交点
	double innterPnt[9];

	ret = CMathGeolib::Cal2LineIntersection( ZDIM3, ZDIM3, basePoint1, vecElenment1, ZDIM3,basePoint2, vecElenment2,Sdistol, Sradtol, innterPnt );

	o_fInnterPnt[0] = innterPnt[0];
	o_fInnterPnt[1] = innterPnt[1];
	o_fInnterPnt[2] = innterPnt[2];

	double TempDir[3] = {0.0};
	TempDir[0] = i_prjPlnpnt[0] - o_fInnterPnt[0];
	TempDir[1] = i_prjPlnpnt[1] - o_fInnterPnt[1];
	TempDir[2] = i_prjPlnpnt[2] - o_fInnterPnt[2];

	double TempRadius = 0.0;
	ret = CMathVector::LengthAndUnit(ZDIM3, TempDir, &TempRadius, TempDir);
	if (ret < 0)
	{
		return ret;
	}

	double TempRad = 0.0;
	double TempRad1 = 0.0;
	double TempRad2 = 0.0;
	ret = CMathVector::Angle(ZDIM3, vecElenment1, vecElenment2,&TempRad);
	if (ret < 0)
	{
		return ret;
	}
	ret = CMathVector::Angle(ZDIM3, TempDir, vecElenment1,&TempRad1);
	if (ret < 0)
	{
		return ret;
	}
	ret = CMathVector::Angle(ZDIM3, TempDir, vecElenment2,&TempRad2);
	if (ret < 0)
	{
		return ret;
	}
	/************************************************************************/
	/* 获取几何要素信息 */
	/************************************************************************/
	o_ComTexValue = 0.0;
	double Crv1Length = 0.0, Crv2Length = 0.0;	// 两线段距离交点的最远长度
	if (abs(TempRad1+TempRad2 - TempRad) < Sradtol)
	{
		// 第一象限
		o_ComTexValue = TempRad;
		TempDir[0] = i_Element1[1][0] - o_fInnterPnt[0];
		TempDir[1] = i_Element1[1][1] - o_fInnterPnt[1];
		TempDir[2] = i_Element1[1][2] - o_fInnterPnt[2];

		// 单位化
		ret = CMathVector::LengthAndUnit(ZDIM3, TempDir, &Crv1Length, TempDir);
		if (ret < 0)
		{
			TempLen = 0.0;
		}
		else
		{// 判断两向量是否相同
			CMathGeolib::Cal2PntDis(ZDIM3, TempDir, vecElenment1, &TempLen);
		}
		if (TempLen > Sdistol || TempRadius > Crv1Length)
		{
			o_ExtendLineFlag = 1;
			if (TempLen <= Sdistol)
			{
				memcpy(o_ExtendLine[0], i_Element1[1], sizeof(double)*3);
			}
			else
			{
				memcpy(o_ExtendLine[0], o_fInnterPnt, sizeof(double)*3);
			}
		}

		TempDir[0] = i_Element2[1][0] - o_fInnterPnt[0];
		TempDir[1] = i_Element2[1][1] - o_fInnterPnt[1];
		TempDir[2] = i_Element2[1][2] - o_fInnterPnt[2];

		// 单位化
		ret = CMathVector::LengthAndUnit(ZDIM3, TempDir, &Crv2Length, TempDir);
		if (ret < 0)
		{
			TempLen = 0.0;
		}
		else
		{// 判断两向量是否相同
			CMathGeolib::Cal2PntDis(ZDIM3, TempDir, vecElenment2, &TempLen);
		}
		if (TempLen > Sdistol || TempRadius > Crv2Length)
		{
			if (o_ExtendLineFlag == 1)
			{
				o_ExtendLineFlag = 3;
			}
			else
			{
				o_ExtendLineFlag = 2;
			}
			if (TempLen <= Sdistol)
			{
				memcpy(o_ExtendLine[2], i_Element2[1], sizeof(double)*3);
			}
			else
			{
				memcpy(o_ExtendLine[2], o_fInnterPnt, sizeof(double)*3);
			}
		}
	}
	else if(abs(2.0*M_PI - TempRad1 - TempRad2 - TempRad) < Sradtol)
	{

		// 第三象限
		o_ComTexValue = TempRad;
		TempDir[0] = o_fInnterPnt[0] - i_Element1[0][0];
		TempDir[1] = o_fInnterPnt[1] - i_Element1[0][1];
		TempDir[2] = o_fInnterPnt[2] - i_Element1[0][2];

		// 单位化
		ret = CMathVector::LengthAndUnit(ZDIM3, TempDir, &Crv1Length, TempDir);
		if (ret < 0)
		{
			TempLen = 0.0;
		}
		else
		{// 判断两向量是否相同
			CMathGeolib::Cal2PntDis(ZDIM3, TempDir, vecElenment1, &TempLen);
		}

		if (TempLen > Sdistol || TempRadius > Crv1Length)
		{
			o_ExtendLineFlag = 1;
			if (TempLen <= Sdistol)
			{
				memcpy(o_ExtendLine[0], i_Element1[0], sizeof(double)*3);
			}
			else
			{
				memcpy(o_ExtendLine[0], o_fInnterPnt, sizeof(double)*3);
			}
		}

		TempDir[0] = o_fInnterPnt[0] - i_Element2[0][0];
		TempDir[1] = o_fInnterPnt[1] - i_Element2[0][1];
		TempDir[2] = o_fInnterPnt[2] - i_Element2[0][2];
	
		// 单位化
		ret = CMathVector::LengthAndUnit(ZDIM3, TempDir, &Crv2Length, TempDir);
		if (ret < 0)
		{
			TempLen = 0.0;
		}
		else
		{// 判断两向量是否相同
			CMathGeolib::Cal2PntDis(ZDIM3, TempDir, vecElenment2, &TempLen);
		}
		if (TempLen > Sdistol || TempRadius > Crv2Length)
		{
			if (o_ExtendLineFlag == 1)
			{
				o_ExtendLineFlag = 3;
			}
			else
			{
				o_ExtendLineFlag = 2;
			}
			if (TempLen < Sdistol)
			{

				memcpy(o_ExtendLine[2], i_Element2[0], sizeof(double)*3);
			}
			else
			{
				memcpy(o_ExtendLine[2], o_fInnterPnt, sizeof(double)*3);

			}
		}
		
		vecElenment1[0] *= -1.;
		vecElenment1[1] *= -1.;
		vecElenment1[2] *= -1.;
		vecElenment2[0] *= -1.;
		vecElenment2[1] *= -1.;
		vecElenment2[2] *= -1.;
	}
	else if(TempRad1 > TempRad2)
	{
		// 第二象限
		o_ComTexValue = M_PI - TempRad;
		TempDir[0] = o_fInnterPnt[0] - i_Element1[0][0];
		TempDir[1] = o_fInnterPnt[1] - i_Element1[0][1];
		TempDir[2] = o_fInnterPnt[2] - i_Element1[0][2];
		
		// 单位化
		ret = CMathVector::LengthAndUnit(ZDIM3, TempDir, &Crv1Length, TempDir);
		if (ret < 0)
		{
			TempLen = 0.0;
		}
		else
		{// 判断两向量是否相同
			CMathGeolib::Cal2PntDis(ZDIM3, TempDir, vecElenment1, &TempLen);
		}
		if (TempLen > Sdistol || TempRadius > Crv1Length)
		{
			o_ExtendLineFlag = 1;
			if (TempLen <= Sdistol)
			{
				memcpy(o_ExtendLine[0], i_Element1[0], sizeof(double)*3);
			}
			else
			{
				memcpy(o_ExtendLine[0], o_fInnterPnt, sizeof(double)*3);
			}
		}

		TempDir[0] = i_Element2[1][0] - o_fInnterPnt[0];
		TempDir[1] = i_Element2[1][1] - o_fInnterPnt[1];
		TempDir[2] = i_Element2[1][2] - o_fInnterPnt[2];

		// 单位化
		ret = CMathVector::LengthAndUnit(ZDIM3, TempDir, &Crv2Length, TempDir);
		if (ret < 0)
		{
			TempLen = 0.0;
		}
		else
		{// 判断两向量是否相同
			CMathGeolib::Cal2PntDis(ZDIM3, TempDir, vecElenment2, &TempLen);
		}
		if (TempLen > Sdistol || TempRadius > Crv2Length)
		{
			if (o_ExtendLineFlag == 1)
			{
				o_ExtendLineFlag = 3;
			}
			else
			{
				o_ExtendLineFlag = 2;
			}
			if (TempLen < Sdistol)
			{
				memcpy(o_ExtendLine[2], i_Element2[1], sizeof(double)*3);

			}
			else
			{
				memcpy(o_ExtendLine[2], o_fInnterPnt, sizeof(double)*3);
			}
		}
		vecElenment1[0] *= -1.;
		vecElenment1[1] *= -1.;
		vecElenment1[2] *= -1.;

	}
	else if (TempRad1 < TempRad2)
	{
		// 第四象限
		o_ComTexValue = M_PI - TempRad;
		TempDir[0] =i_Element1[1][0] - o_fInnterPnt[0];
		TempDir[1] =i_Element1[1][1] - o_fInnterPnt[1];
		TempDir[2] =i_Element1[1][2] - o_fInnterPnt[2];

		// 单位化
		ret = CMathVector::LengthAndUnit(ZDIM3, TempDir, &Crv1Length, TempDir);
		if (ret < 0)
		{
			TempLen = 0.0;
		}
		else
		{// 判断两向量是否相同
			CMathGeolib::Cal2PntDis(ZDIM3, TempDir, vecElenment1, &TempLen);
		}

		if (TempLen > Sdistol || TempRadius > Crv1Length)
		{
			o_ExtendLineFlag = 1;
			if (TempLen < Sdistol)
			{
				memcpy(o_ExtendLine[0], i_Element1[1], sizeof(double)*3);
			}
			else
			{
				memcpy(o_ExtendLine[0], o_fInnterPnt, sizeof(double)*3);
			}

		}

		TempDir[0] = o_fInnterPnt[0] - i_Element2[0][0];
		TempDir[1] = o_fInnterPnt[1] - i_Element2[0][1];
		TempDir[2] = o_fInnterPnt[2] - i_Element2[0][2];

		// 单位化
		ret = CMathVector::LengthAndUnit(ZDIM3, TempDir, &Crv2Length, TempDir);
		if (ret < 0)
		{
			TempLen = 0.0;
		}
		else
		{// 判断两向量是否相同
			CMathGeolib::Cal2PntDis(ZDIM3, TempDir, vecElenment2, &TempLen);
		}

		if (TempLen > Sdistol || TempRadius > Crv2Length)
		{
			if (o_ExtendLineFlag == 1)
			{
				o_ExtendLineFlag = 3;
			}
			else
			{
				o_ExtendLineFlag = 2;
			}
			if (TempLen < Sdistol)
			{
				memcpy(o_ExtendLine[2], i_Element2[0], sizeof(double)*3);
			}
			else
			{
				memcpy(o_ExtendLine[2], o_fInnterPnt, sizeof(double)*3);
			}
		}
		vecElenment2[0] *= -1.;
		vecElenment2[1] *= -1.;
		vecElenment2[2] *= -1.;
	}

	o_ExtendLine[1][0] = o_fInnterPnt[0] + vecElenment1[0]*TempRadius;
	o_ExtendLine[1][1] = o_fInnterPnt[1] + vecElenment1[1]*TempRadius;
	o_ExtendLine[1][2] = o_fInnterPnt[2] + vecElenment1[2]*TempRadius;
	o_ExtendLine[3][0] = o_fInnterPnt[0] + vecElenment2[0]*TempRadius;
	o_ExtendLine[3][1] = o_fInnterPnt[1] + vecElenment2[1]*TempRadius;
	o_ExtendLine[3][2] = o_fInnterPnt[2] + vecElenment2[2]*TempRadius;
    return 1;
}

int CMathGeolib::GetAngleDimSpecialLineData(
	double i_Element1[2][3],			// 要素1的始终点
	double i_Element2[2][3],			// 要素2的始终点
	int &o_SpecialLineFlag,				// 特殊线标志 = 0 没有特殊线
										// = 1有一条特殊线且是第一条;
										// = 2有一条特殊线且是第二条；
										// = 3有两条特殊线
	double o_SpecialLine[4][3]			// 特殊线点列
)
{
	/************************************************************************/
	/* 获取特殊线数据 */
	/************************************************************************/
	double Len1 = 0.0,Len2 = 0.0;
	// 判断交点与线的两端点是否一致
	double dInnterPnt[3] ={0.0};
	// 计算两线段的交点
	double innterPnt[9];
	// 获取两线段交点

	double basePoint1[3], basePoint2[3];
	basePoint1[0] = i_Element1[0][0];
	basePoint1[1] = i_Element1[0][1];
	basePoint1[2] = i_Element1[0][2];

	double vecElenment1[3], vecElenment2[3];
	vecElenment1[0] = i_Element1[1][0] - i_Element1[0][0];
	vecElenment1[1] = i_Element1[1][1] - i_Element1[0][1];
	vecElenment1[2] = i_Element1[1][2] - i_Element1[0][2];

	double TempLen = 0.0;
	int ret = 0;
	ret = CMathVector::LengthAndUnit(ZDIM3, vecElenment1, &TempLen, vecElenment1);
	if (ret < 0)
	{
		return ret;
	}

	basePoint2[0] = i_Element2[0][0];
	basePoint2[1] = i_Element2[0][1];
	basePoint2[2] = i_Element2[0][2];
	vecElenment2[0] = i_Element2[1][0] - i_Element2[0][0];
	vecElenment2[1] = i_Element2[1][1] - i_Element2[0][1];
	vecElenment2[2] = i_Element2[1][2] - i_Element2[0][2];
	ret = CMathVector::LengthAndUnit(ZDIM3, vecElenment2, &TempLen, vecElenment2);
	if (ret < 0)
	{
		return ret;
	}
	ret = CMathGeolib::Cal2LineIntersection( ZDIM3, ZDIM3, basePoint1, vecElenment1, ZDIM3,basePoint2, vecElenment2,Sdistol, Sradtol, innterPnt );
	memcpy(dInnterPnt, innterPnt, sizeof(double)*3);
//	dInnterPnt[0] = dInnterPnt[0];
//	dInnterPnt[1] = dInnterPnt[1];
//	dInnterPnt[2] = dInnterPnt[2];

	double dElement1[2][3];
	dElement1[0][0] = i_Element1[0][0];
	dElement1[0][1] = i_Element1[0][1];
	dElement1[0][2] = i_Element1[0][2];
	dElement1[1][0] = i_Element1[1][0];
	dElement1[1][1] = i_Element1[1][1];
	dElement1[1][2] = i_Element1[1][2];
	Cal2PntDis(ZDIM3, dInnterPnt, dElement1[0], &Len1);
	Cal2PntDis(ZDIM3, dInnterPnt, dElement1[1], &Len2);

	if (Len1 - Sdistol > 0 && Len2 - Sdistol > 0)
	{
		o_SpecialLineFlag = 1;
		memcpy(o_SpecialLine[0], dInnterPnt, sizeof(double)*3);
		if (Len1 > Len2)
		{
			memcpy(o_SpecialLine[1], i_Element1[1], sizeof(double)*3);
		}
		else
		{
			memcpy(o_SpecialLine[1], i_Element1[0], sizeof(double)*3);
		}
	}

	double dElement2[2][3];
	dElement2[0][0] = i_Element2[0][0];
	dElement2[0][1] = i_Element2[0][1];
	dElement2[0][2] = i_Element2[0][2];
	dElement2[1][0] = i_Element2[1][0];
	dElement2[1][1] = i_Element2[1][1];
	dElement2[1][2] = i_Element2[1][2];
	Cal2PntDis(ZDIM3, dInnterPnt, dElement2[0], &Len1);
	Cal2PntDis(ZDIM3, dInnterPnt, dElement2[1], &Len2);
	if (Len1 - Sdistol > 0 && Len2 - Sdistol > 0)
	{
		if (o_SpecialLineFlag == 1)
		{
			o_SpecialLineFlag = 3;
		}
		else
		{
			o_SpecialLineFlag = 2;
		}

		memcpy(o_SpecialLine[2], dInnterPnt, sizeof(double)*3);
		if (Len1 > Len2)
		{
			memcpy(o_SpecialLine[3], i_Element2[1], sizeof(double)*3);
		}
		else
		{
			memcpy(o_SpecialLine[3], i_Element2[0], sizeof(double)*3);
		}
	}
    return 1;
}
