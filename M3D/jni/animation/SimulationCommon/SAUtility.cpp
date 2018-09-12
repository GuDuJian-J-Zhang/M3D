// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		SAUtility.cpp
*
*	@brief	定义动画相关的几何算法
*
*
*	@par	历史:
*
****************************************************************************/


// SAUtility.cpp : implementation of the SAUtility class
//
#include "stdafx.h"
#include "SAUtility.h"
#include	<math.h>


const float SAUtility::PI=3.1415926f;
const float SAUtility::Epsilon=0.01f;

 #define EPSILON		0.00001F

//计算两向量夹角
int SAUtility::ComputeDoublePrecisionAngle( double vector1[], double vector2[], double *rad_p)
{

	double		len1,len2;
	double		inn,con;
	double		wk1,wk2;
	int		status;
	register int	i;

	status = 0;
	*rad_p = 0.0;

	inn = 0.0;
	for (i=0; i<3; i++) {
		inn += (vector1[i] * vector2[i]);
	}

	wk1 = 0.0;
	wk2 = 0.0;
	for(i=0; i<3; i++) {
		wk1 += (vector1[i] * vector1[i]);
		wk2 += (vector2[i] * vector2[i]);
	}
	len1 = sqrt(wk1);
	len2 = sqrt(wk2);
	if (len1 <= EPSILON || len2 <= EPSILON || 
		len1 * len2 <= EPSILON) {
			status = -2;
			goto	rtn;
	}

	con = inn/(len1 * len2);
	if(con >= 1.0) {
		*rad_p = 0.0;
	}
	else if(con <= -1.0) {
		*rad_p = PI;
	}
	else {
		*rad_p = acos(con);
	}

rtn	:
	return(status);
}


void SAUtility::NormalizeDoublePrecisionVector(double* vector1)
{
	double length;
	length = sqrt(vector1[0]*vector1[0]+vector1[1]*vector1[1]+vector1[2]*vector1[2]);
	vector1[0]/=length;
	vector1[1]/=length;
	vector1[2]/=length;
}



double SAUtility::ComputeDoublePrecisionVectorLength(double * vector1)
{
	return sqrt(vector1[0]*vector1[0]+vector1[1]*vector1[1]+vector1[2]*vector1[2]);
}



void SAUtility::ComputeDoublePrecisionCrossProduct(double * vector1, double * vector2, double * up)
{
	up[0] = vector1[1] * vector2[2]  -  vector1[2] * vector2[1];
    up[1] = vector1[2] * vector2[0]  -  vector1[0] * vector2[2];
    up[2] = vector1[0] * vector2[1]  -  vector1[1] * vector2[0];
}



double SAUtility::ComputeDoublePrecisionDotProduct(double * vector1, double * vector2)
{
	return (vector1[0] * vector2[0]) + (vector1[1] * vector2[1]) + (vector1[2] * vector2[2]);
}


void SAUtility::NormalizeVector(const AniPoint& vector1,AniPoint& vector2)
{
	float length;
	length = sqrt(vector1.x*vector1.x+vector1.y*vector1.y+vector1.z*vector1.z);
	vector2 = vector1/length;
}

void SAUtility::Compute_Cross_Product (const AniPoint& p1,	const AniPoint& p2, AniPoint& pOut) {
		AniPoint tmp;
		tmp.x =  p1.y *  p2.z	 -	 p1.z * p2.y;
		tmp.y =  p1.z * p2.x	 -	 p1.x * p2.z;
		tmp.z =  p1.x * p2.y	 -	 p1.y * p2.x;

		pOut.x = tmp.x;
		pOut.y = tmp.y;
		pOut.z = tmp.z;
}

double SAUtility::Compute_Dot_Product (const AniPoint& p1, const AniPoint& p2) {

		return p1.x * p2.x	+  p1.y * p2.y	+  p1.z * p2.z;
}

double SAUtility::Compute_Vector_Length (const AniPoint&in) {
		float length;

		length = in.x * in.x	+  in.y * in.y  +	 in.z * in.z;
		if (length != 1.0f)
			length = sqrt(length);
		return length;
}

void SAUtility::MatrixMakeIdentity(float* fMtxAA) {

	for(int i = 0; i< 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(i == j)
			{
				fMtxAA[i*4+j] = 1.0;
			}
			else
			{
				fMtxAA[i*4+j] = 0.0;
			}
		}
	}
}


void SAUtility::AxisRadToMatrix (AniPoint& vector,double radians,float *out) {
	float quat[4];
	quat[3] = static_cast<float>(cos(radians/2));

	const float sineval = static_cast<float>(sin(radians/2));
	NormalizeVector(vector,vector);
	// we test for a null vector above
	quat[0] = vector.x * sineval;
	quat[1] = vector.y * sineval;
	quat[2] = vector.z * sineval;
	
	
	float x = quat[0];
	float y = quat[1];
	float z = quat[2];
	float w = quat[3];

	float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

	s = 2.0f/(x*x + y*y + z*z + w*w);

	xs = x*s;	ys = y*s;		zs = z*s;
	wx = w*xs;	wy = w*ys;		wz = w*zs;
	xx = x*xs;	xy = x*ys;		xz = x*zs;
	yy = y*ys;	yz = y*zs;		zz = z*zs;

	out[0] = 1.0f - (float)(yy+zz); out[1] = (float)xy + (float)wz; out[2] = (float)xz - (float)wy; out[3] = 0.0f;
	out[4] = (float)xy - (float)wz; out[5] = 1.0f - (float)(xx + zz); out[6] = (float)yz + (float)wx; out[7] = 0.0f;
	out[8] = (float)xz + (float)wy; out[9] = (float)yz - (float)wx; out[10] = 1.0f - (float)(xx + yy); out[11] = 0.0f;
	out[12] = 0.0f; out[13] = 0.0f; out[14] = 0.0f; out[15] = 1.0f;
}


void SAUtility::ComputeVectorToVectorRotationMatrix(float *matrix,AniPoint& p1,AniPoint& p2)
{
	AniPoint p3;

	NormalizeVector(p1,p1);
	NormalizeVector(p2,p2);
	Compute_Cross_Product(p1,p2,p3);

	float dprod = (float)Compute_Dot_Product(p1,p2);
	float l=(float)Compute_Vector_Length(p3);
	//float ang2 = (float)asin(l);
	float ang = (float)atan2(l,dprod);
	ang*=(180.0f/(float)PI);

	if (l< 0.000001)
	{
		//we need to handle the anti-parallel case
		MatrixMakeIdentity(matrix);
	}
	else
	{
		AxisRadToMatrix(p3,-ang, matrix);
	}
}

void SAUtility::GetMatrixFromCamera(AniPoint target, AniPoint position, AniPoint up_vector, AniPoint translation, float *matrix)
{
    AniPoint target_vector;
    target_vector.Set(target.x-position.x, target.y-position.y, target.z-position.z);

	double dTargetVector[3];
	dTargetVector[0] = -target_vector.x;
	dTargetVector[1] = -target_vector.y;
	dTargetVector[2] = -target_vector.z;
    NormalizeDoublePrecisionVector(dTargetVector);
	double dUpVector[3];
	dUpVector[0] = up_vector.x;
	dUpVector[1] = up_vector.y;
	dUpVector[2] = up_vector.z;
    NormalizeDoublePrecisionVector(dUpVector);

    double cross1[3];

    ComputeDoublePrecisionCrossProduct(dUpVector, dTargetVector, cross1);

    matrix[0] = cross1[0];
    matrix[1] = cross1[1];
    matrix[2] = cross1[2];
    matrix[3] = 0.0f;

    //double cross2[3];
    //ComputeDoublePrecisionCrossProduct(dTargetVector, cross1, cross2);

    matrix[4] = dUpVector[0];
    matrix[5] = dUpVector[1];
    matrix[6] = dUpVector[2];
    matrix[7] = 0.0f;

    matrix[8] = dTargetVector[0];
    matrix[9] = dTargetVector[1];
    matrix[10] = dTargetVector[2];
    matrix[11] = 0.0f;
    
    matrix[12] = translation.x;
    matrix[13] = translation.y;
    matrix[14] = translation.z;
    matrix[15] = 1.0f;
}


#define PA(t,m,a) (t*t * 0.5f * m / a)
#define PB(t,m,a)  (PA(a,m,a) + (t - a) * m)
#define PC(t,m,a,b,c)  (PB(a+b,m,a) + (t - (a+b)) * m * (1.0f - 0.5f * (t - (a+b)) / c))

float SAUtility::EaseInEaseOut(float t, float a, float b, float c)
{

    float res;
    float m = 2.0f / (a + 2.0f * b + c);

    if (t<=a)
	res = PA(t,m,a);
    else if (t>=a && t<=(a+b))
	res = PB(t,m,a);    
    else 
    	res = PC(t,m,a,b,c);
    if (res>1.0f)
	res = 1.0f;
    return res;
	
}


#define X 0
#define Y 1
#define Z 2
#define W 3

void SAUtility::TransitionQuaternion(float *quat_old, float *quat_new, float t, float *qt)
{
	double omega, cosom, sinom, sclp, sclq;

	cosom = quat_old[X]*quat_new[X] + quat_old[Y]*quat_new[Y] + quat_old[Z]*quat_new[Z] + quat_old[W]*quat_new[W];

	if ((1.0 + cosom) > EPSILON)
	{
		if ((1.0 - cosom) > EPSILON)
		{
			omega = acos(cosom);
			sinom = sin(omega);
			sclp = sin((1.0 - t)*omega)/sinom;
			sclq = sin(t*omega)/sinom;
		}
		else
		{
			sclp = 1.0 - t;
			sclq = t;
		}

		for (int i=0; i<4; i++)
			qt[i] = (float)sclp*quat_old[i] +(float)sclq*quat_new[i];
	}
	else
	{
		qt[X] = -quat_old[Y];	qt[Y] = quat_old[X];
		qt[Z] = -quat_old[W];	qt[W] = quat_old[Z];
		sclp = sin((1.0 -t) * PI/2);
		sclq = sin(t * PI/2);
		for (int i=0; i<3; i++)
			qt[i] = (float)sclp*quat_old[i] + (float)sclq*qt[i];
	}
}





