// All Rights Reserved.Copyright(C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		KeyFrame.cpp
*
*	@brief	关键帧类型定义文件
*
*	@par	历史:
*
****************************************************************************/

#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../SimulationCommon/SATools.h"
#include "KeyFrame.h"
#include "SBehaviorAction.h"
#include "STimeline.h"
#include "SAnimation.h"
#include "SInterpolator.h"
#include "../SimulationCommon/SAUtility.h"
#include "../SimulationCommon/UtilityXMLParser.h"
#include "../SimulationCommon/UtilityTextParser.h"


#include "../SimulationCommon/vlist.h"
#include "../SimulationCommon/vhash.h"
#include "../SimulationCommon/MatrixOperation.h"
#include "../StringTable.h"

//SA_NAMESPACE_BEGIN
 
const AniQuat& AniQuat::Normalize()
{
	float mag = (float)(sqrt(x*x+y*y+z*z+w*w));
	if(mag>0)
	{
		x/=mag;
		y/=mag;
		z/=mag;
		w/=mag;
	}
	return *this;
} 


/*
e^AniQuat given as:
Qexp(v*a) = [cos(a),vsin(a)]
*/
AniQuat AniQuat::Qexp(const AniQuat &q)
{
	float a = float(sqrt(q.x*q.x + q.y*q.y + q.z*q.z));
	float sina = float(sin(a));
	float cosa = float(cos(a));
	AniQuat ret;

	ret.w = cosa;
	if(a > 0)
	{
		ret.x = sina * q.x / a;
		ret.y = sina * q.y / a;
		ret.z = sina * q.z / a;
	}
	else
	{
		ret.x = ret.y = ret.z = 0;
	}

	return ret;
}

/*
Linear interpolation between two HQuats
*/
AniQuat AniQuat::Qlerp(const AniQuat &q1,const AniQuat &q2,float t)
{
	AniQuat ret;
 	
 	ret = q1 +  t * (q2 - q1);
 	/*
		return value must be normalized
	*/
	return ret.Normalize();
}

/*
Spherical linear interpolation between two HQuats
*/
AniQuat AniQuat::Qslerp(const AniQuat &q1,const AniQuat &q2,float t)
{
	AniQuat q3;
	float dot;
	dot = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;

	/*
	dot = cos(theta)
	if (dot < 0), q1 and q2 are more than 90 degrees apart,
	so we can invert one to reduce spinning
	*/
	if (dot < 0)
	{
		dot = -dot;
		q3 = -1*q2;
	}
	else
	{
		q3 = q2;
	}

	
	if (dot < 0.95f)
	{
		float angle = float(acos(dot));
		float sina,sinat,sinaomt;
		sina = float(sin(angle));
		sinat = float(sin(angle*t));
		sinaomt = float(sin(angle*(1-t)));
		return (q1*sinaomt+q3*sinat)/sina;
	}
	/*
	if the angle is small, use linear interpolation
	*/
	else
	{
		return Qlerp(q1,q3,t);
	}
}

 


/*
This version of slerp, used by squad, does not check for theta > 90.
*/

AniQuat AniQuat::QslerpNoInvert(const AniQuat &q1,const AniQuat &q2,float t)
{
	float dot = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;

	if (dot > -0.95f && dot < 0.95f)
	{
		float angle = float(acos(dot));
		float sina,sinat,sinaomt;
		sina = float(sin(angle));
		sinat = float(sin(angle*t));
		sinaomt = float(sin(angle*(1-t)));
		return (q1*sinaomt+q2*sinat)/sina;
	}
	/*
	if the angle is small, use linear interpolation
	*/
	else
	{
		return Qlerp(q1,q2,t);
	}
}
AniQuat AniQuat::QslerpNoInvertExtraSpins(const AniQuat &rkP,const AniQuat &rkQ,float fT, int iExtraSpins)
{
	
	
	float fCos = rkP.x*rkQ.x + rkP.y*rkQ.y + rkP.z*rkQ.z + rkP.w*rkQ.w;
	float fAngle = float(acos(fCos));

	if (fabs(fAngle) < 0.005f)
		return rkP;
	
	float fSin = sin(fAngle);
	float fInvSin = 1.0f/fSin;
	float fPhase = 3.1415927f * iExtraSpins * fT;
	float fCoeff0 = sin((1.0f - fT) * fAngle - fPhase)*fInvSin;
	float fCoeff1 = sin(fT*fAngle + fPhase)*fInvSin;
	/*
        if (fCos < 0.0f)
        {
            fCoeff0 = -fCoeff0;
            // taking the complement requires renormalisation
            AniQuat t(fCoeff0*rkP + fCoeff1*rkQ);
            t.Normalize();
            return t;
        }
		*/
		

	return (fCoeff0 * rkP + fCoeff1 * rkQ);
}	




AniQuat AniQuat::MatrixToQuaternion(float *matrix)
{
	float quaternion[4];
	double	tr, s;
	int		i, j, k;

	tr = matrix[0] + matrix[5] + matrix[10];
	if (tr > 0.0)
	{
		s = sqrt(tr + 1.0);
		quaternion[3] = (float)s * 0.5f;
		s = 0.5f/(float)s;

		quaternion[0] = (matrix[6] - matrix[9]) * (float)s;
		quaternion[1] = (matrix[8] - matrix[2]) * (float)s;
		quaternion[2] = (matrix[1] - matrix[4]) * (float)s;
	}
	else
	{
		i = 0;
		if (matrix[(1*4)+1] > matrix[(0*4)+0])
			i = 1;
		if (matrix[(2*4)+2] > matrix[(i*4)+i])
			i = 2;

		int nxt[3] = {1,2,0};

		j = nxt[i];
		k = nxt[j];

		s = sqrt(matrix[(i*4)+i] - (matrix[(j*4)+j] + matrix[(k*4)+k]) + 1.0f);

		quaternion[i] = (float)s * 0.5f;
		s = 0.5f / s;

		quaternion[3] = (matrix[(j*4) + k] - matrix[(k*4) + j]) * (float)s;
		quaternion[j] = (matrix[(i*4) + j] + matrix[(j*4) + i]) * (float)s;
		quaternion[k] = (matrix[(i*4) + k] + matrix[(k*4) + i]) * (float)s;
	}
	return AniQuat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
}





/*
Spherical cubic interpolation
*/
AniQuat AniQuat::Qsquad(const AniQuat &q1,const AniQuat &q2,const AniQuat &a,const AniQuat &b,float t)
{
	AniQuat c,d;
	c = QslerpNoInvert(q1,q2,t);
	d = QslerpNoInvert(a,b,t);
	return QslerpNoInvert(c,d,2*t*(1-t));
}


/*
Given 3 HQuats, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
*/

AniQuat AniQuat::Qspline(const AniQuat &qnm1,const AniQuat &qn,const AniQuat &qnp1)
{
	AniQuat qni;
	
	qni.x = -qn.x;
	qni.y = -qn.y;
	qni.z = -qn.z;
	qni.w = qn.w;

	return qn*Qexp((Qlog(qni*qnm1)+Qlog(qni*qnp1))/-4);
}




void AniQuat::ToMatrix(float *matrix)
{	
	float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

	s = 2.0f/(x*x + y*y + z*z + w*w);

	xs = x*s;	ys = y*s;		zs = z*s;
	wx = w*xs;	wy = w*ys;		wz = w*zs;
	xx = x*xs;	xy = x*ys;		xz = x*zs;
	yy = y*ys;	yz = y*zs;		zz = z*zs;

	matrix[0] = 1.0f - (float)(yy+zz); matrix[1] = (float)xy + (float)wz; matrix[2] = (float)xz - (float)wy; matrix[3] = 0.0f;
	matrix[4] = (float)xy - (float)wz; matrix[5] = 1.0f - (float)(xx + zz); matrix[6] = (float)yz + (float)wx; matrix[7] = 0.0f;
	matrix[8] = (float)xz + (float)wy; matrix[9] = (float)yz - (float)wx; matrix[10] = 1.0f - (float)(xx + yy); matrix[11] = 0.0f;
	matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;
}

/*
Logarithm of a AniQuat, given as:
Qlog(q) = v*a where q = [cos(a),v*sin(a)]
*/

AniQuat AniQuat::Qlog(const AniQuat &q)
{
	float a = (float)(acos(q.w));
	float sina = (float)(sin(a));
	AniQuat ret;
	ret.w = 0;
	if (sina > 0)
	{
		ret.x = a*q.x/sina;
		ret.y = a*q.y/sina;
		ret.z = a*q.z/sina;
	}
	else
	{
		ret.x=ret.y=ret.z=0;
	}
	return ret;
}

bool AniQuat::Equal(const AniQuat& v, float tolerance) const
{

	if(!(tolerance >= 0.0f))
		return false;


	float xdist = this->x - v.x;
	float ydist = this->y - v.y;
	float zdist = this->z - v.z;
	float wdist = this->w - v.w;

	if(fabs(xdist) <= tolerance && 
		fabs(ydist) <= tolerance &&
		fabs(zdist) <= tolerance &&
		fabs(wdist) <= tolerance)
		return true;
	return false;
}
 
void CKeyframeChannelLinear::Interpolate(CKeyframeChannel **posarray, int keyframe, float fraction, int length, AniPoint &res)
{
	AniPoint delta;
		delta.Set(posarray[keyframe+1]->m_cp.x - posarray[keyframe]->m_cp.x, posarray[keyframe+1]->m_cp.y - posarray[keyframe]->m_cp.y,
			posarray[keyframe+1]->m_cp.z - posarray[keyframe]->m_cp.z);
		res.Set(posarray[keyframe]->m_cp.x + delta.x * fraction, posarray[keyframe]->m_cp.y + delta.y * fraction,
			posarray[keyframe]->m_cp.z + delta.z * fraction);
}

void CKeyframeChannelLinear::Serialize(CUtilityXMLTag *xmlgen)
{
	char text[SA_BUFFER_SIZE_BIG];
	if (m_bEaseInOut) {
		SACLOCALE(sprintf(text, "[[ea]%f %f %f]", m_cp.x, m_cp.y, m_cp.z));
	}
	else
	{
		SACLOCALE(sprintf(text, "[%f %f %f]", m_cp.x, m_cp.y, m_cp.z));
	}
	xmlgen->AddListItem(text);
}

void CKeyframeChannelCurve::Serialize(CUtilityXMLTag *xmlgen)
{
	char text[SA_BUFFER_SIZE_BIG];
	SACLOCALE(sprintf(text, "[b %f %f %f]", m_cp.x, m_cp.y, m_cp.z));
	xmlgen->AddListItem(text);
}

void CKeyframeChannelDiscrete::Interpolate(CKeyframeChannel **posarray, int keyframe, float fraction, int length, AniPoint &res)
{
	if (fraction < 1.0f)
		res.Set(posarray[keyframe]->m_cp);
	else
		res.Set(posarray[keyframe+1]->m_cp);

}


void CKeyframeChannelDiscrete::Serialize(CUtilityXMLTag *xmlgen)
{
	char text[SA_BUFFER_SIZE_BIG];
	SACLOCALE(sprintf(text, "[d %f %f %f]", m_cp.x, m_cp.y, m_cp.z));
	xmlgen->AddListItem(text);
}

void CKeyframeChannelCurve::Interpolate(CKeyframeChannel **posarray, int keyframe, float fraction, int length, AniPoint &res)
{	
 		AniPoint p0, p1, p2, p3;
		p1 = posarray[keyframe]->m_cp;
		p2 = posarray[keyframe+1]->m_cp;
		if (keyframe>0)
			p0 = posarray[keyframe - 1]->m_cp;
		else
			p0 = p1;
		if (keyframe<length-2)
			p3 = posarray[keyframe + 2]->m_cp;
		else
			p3 = p2;
		
//		CalculateHermiteTangents(p0, p2, p3);
		res = InterpolateHermiteSpline(fraction, p1, p2);
}

void CKeyframeChannelCurve::CalculateCurveFactor(CSTimeline *tl, int keyframe)
{
		int nm1, nm0, np1;

   		int *tlarray = tl->GetTimelineArray();
		int length = tl->GetTimelineArrayLength();
		if (keyframe>0 && keyframe<length)
			nm1 = tlarray[keyframe] - tlarray[keyframe-1];
		else
			nm1 = 0;

		if ( keyframe < length-1)
			nm0 = tlarray[keyframe+1] - tlarray[keyframe];
		else 
			nm0 = 0;

		if (keyframe<length-2)
			np1 = tlarray[keyframe+2] - tlarray[keyframe+1];
		else 
			np1 = 0;
		m_factor1 = (2.0f * nm0)/ (nm1 + nm0);	
		m_factor2 = (2.0f * nm0) / (nm0 + np1);

}
void CKeyframeChannelCurve::CalculateHermiteTangents(AniPoint p0, AniPoint p2, AniPoint p3, float a)
{

	AniPoint p1 = m_cp;
	a = 0.5f;
	m_tangent1.Set(a * (p2.x - p0.x),a * (p2.y - p0.y), a * (p2.z - p0.z));
	m_tangent2.Set(a * (p3.x - p1.x),a * (p3.y - p1.y), a * (p3.z - p1.z));

//	float factor = (2.0f * nm0)/ (nm1 + nm0);
 	m_tangent1.Set(m_factor1 * m_tangent1.x, m_factor1 * m_tangent1.y, m_factor1 * m_tangent1.z);
//	 factor = (2.0f * nm0) / (nm0 + np1);
 	m_tangent2.Set(m_factor2 * m_tangent2.x, m_factor2 * m_tangent2.y, m_factor2 * m_tangent2.z);
 

}



AniPoint CKeyframeChannelCurve::InterpolateHermiteSpline(float fraction, AniPoint p1, AniPoint p2)
{

	AniPoint res;
	float s = fraction;
	float s2 = fraction * fraction;
	float s3 = fraction * fraction * fraction;

	float h1 = 2 * s3 - 3 *s2 + 1;
	float h2 = -2 * s3 + 3 * s2;
	float h3 = s3 - 2 * s2 + s;
	float h4 = s3 - s2;
	res.Set(h1 * p1.x + h2 * p2.x + h3 * m_tangent1.x + h4 * m_tangent2.x,
		h1 * p1.y + h2 * p2.y + h3 * m_tangent1.y + h4 * m_tangent2.y,
		h1 * p1.z + h2 * p2.z + h3 * m_tangent1.z + h4 * m_tangent2.z);
	return res;
}
/*

AniPoint CKeyframeChannelCurve::InterpolateBezierSpline(float fraction, AniPoint p2)
{

	AniPoint p1 = m_cp, res;
	float s = fraction;
	float s2 = fraction * fraction;
	float s3 = fraction * fraction * fraction;
 
	float h1 = -1 * s3  + 3 *s2 -3 * s + 1;
	float h2 = 3 * s3   - 6 * s2 + 3 *s;
	float h3 = -3 * s3  + 3 * s2;
	float h4 = s3;

	res.Set(h1 * p1.x + h2 * m_tangent1.x + h3 * m_tangent2.x + h4 * p2.x,
		h1 * p1.y + h2 * m_tangent1.y + h3 * m_tangent2.y + h4 * p2.y,
		h1 * p1.z + h2 * m_tangent1.z + h3 * m_tangent2.z + h4 * p2.z);
	return res;
}
*/

void CKeyframeChannelCurve::CalculateCurveLength(AniPoint p2)
{
	float adder = 1.0f / 100.0f;
	float fraction = 0.0f;
	AniPoint oldpoint = m_cp;
	m_curvelength = 0.0f;
	AniPoint delta;
	for (int i=0;i<100;i++,fraction+=adder )
	{
		AniPoint res;
		res = InterpolateHermiteSpline(fraction,m_cp,  p2);
		delta.Set(res.x - oldpoint.x, res.y - oldpoint.y, res.z - oldpoint.z);
		oldpoint = res;
	}
}



void CKeyframeChannelFollowPath::Serialize(CUtilityXMLTag *xmlgen)
{
	char text[SA_BUFFER_SIZE_BIG];
	if (!m_bLinear)
		SACLOCALE(sprintf(text, "[f %f %f %f]", m_cp.x, m_cp.y, m_cp.z));
	else
		SACLOCALE(sprintf(text, "[fl %f %f %f]", m_cp.x, m_cp.y, m_cp.z));

	xmlgen->AddListItem(text);
}

void CKeyframeChannelFollowPath::Interpolate(CKeyframeChannel **posarray, int keyframe, float fraction, int length, AniPoint &res)
{		
		AniPoint res2;
		AniPoint p0, p1, p2, p3;
		AniPoint reslinear;

		m_bNoRotate = false;
		float oldfraction = fraction;
		p1 = posarray[keyframe]->m_cp;
		p2 = posarray[keyframe+1]->m_cp;
		if (p1 == p2)
			m_bNoRotate = true;

		if (m_bLinear)
		{
			AniPoint delta;
			delta.Set(posarray[keyframe+1]->m_cp.x - posarray[keyframe]->m_cp.x, posarray[keyframe+1]->m_cp.y - posarray[keyframe]->m_cp.y,
				posarray[keyframe+1]->m_cp.z - posarray[keyframe]->m_cp.z);
			reslinear.Set(posarray[keyframe]->m_cp.x + delta.x * fraction, posarray[keyframe]->m_cp.y + delta.y * fraction,
				posarray[keyframe]->m_cp.z + delta.z * fraction);
		}


		if (keyframe>0)
			p0 = posarray[keyframe - 1]->m_cp;
		else
			p0 = p1;
		if (keyframe<length-2)
			p3 = posarray[keyframe + 2]->m_cp;
		else
			p3 = p2;

		AniPoint res_sav;
		m_factor1 = 1.0f;
		m_factor2 = 1.0f;
 		CalculateHermiteTangents(p0, p2, p3);
		res  = InterpolateHermiteSpline(fraction, p1, p2);

		

		fraction += 0.01f;
		if (fraction>1.0f)
		{
			fraction = fraction - 1.0f;
			keyframe+=1;
			if (keyframe>=length-1)
				keyframe = 0;
 		}

 		p1 = posarray[keyframe]->m_cp;
		p2 = posarray[keyframe+1]->m_cp;
		if (keyframe>0)
			p0 = posarray[keyframe - 1]->m_cp;
		else
			p0 = p1;
		if (keyframe<length-2)
			p3 = posarray[keyframe + 2]->m_cp;
		else
			p3 = p2;
		
		CalculateHermiteTangents(p0, p2, p3);
		res2 = InterpolateHermiteSpline(fraction, p1, p2);	

		AniPoint vec;
		vec.Set(res2.x - res.x, res2.y - res.y, res2.z - res.z);
		AniPoint px;
		px.Set(0,1,0);
		float result1[16];
		float result2[16];
		SAUtility::NormalizeVector(vec, vec);
 		SAUtility::ComputeVectorToVectorRotationMatrix(result1,vec,px);
		
 
		float ang;
		{
			AniPoint pnt1, pnt2, pnt3;
 			pnt1.Set(result1[8], result1[9], result1[10]);
			pnt1.Set(0,0,1);
	 
			CSACommonAPI::MultDirMatrix(result1, pnt1, pnt1);
			pnt1.y = 0;
			SAUtility::NormalizeVector(pnt1, pnt1);
			pnt2.Set(0,0,1);
	 
			SAUtility::NormalizeVector(pnt1,pnt1);
			SAUtility::NormalizeVector(pnt2,pnt2);
			SAUtility::Compute_Cross_Product(pnt1,pnt2,pnt3);

			float dprod = (float)SAUtility::Compute_Dot_Product(pnt1,pnt2);
			float l=(float)SAUtility::Compute_Vector_Length(pnt3);
			//float ang2 = (float)asin(l);
			ang = (float)atan2(l,dprod);
			ang*=(180.0f/(float)3.1415927f);
		}

		AniPoint rr;
		rr.Set(result1[4], result1[5] , result1[6]);
		SAUtility::NormalizeVector(rr, rr);
        SAUtility::AxisRadToMatrix(rr, -ang, result2);

//		SAUtility::ComputeVectorToVectorRotationMatrix(rmatrix2,vec2,px);
		MatrixOperation::Multiply(result1, result2, tmatrix);
		AniPoint ttx(1,0,0);
		AniPoint ttx2(1,0,0);
		CSACommonAPI::MultDirMatrix( tmatrix, ttx, ttx);
		ang = -ang;
        SAUtility::AxisRadToMatrix(rr, -ang, result2);
		MatrixOperation::Multiply(result1, result2, tmatrix);
		CSACommonAPI::MultDirMatrix(tmatrix, ttx2, ttx2);
		if (ttx.z < 0)
			ttx.z = -ttx.z;
		if (ttx2.z < 0)
			ttx2.z = -ttx2.z;
		if (ttx2.z >= ttx.z)
		{
			ang = -ang;
			AniPoint vec1;
			vec1.x = result1[4];
			vec1.y = result1[5];
			vec1.z = result1[6];
	        SAUtility::AxisRadToMatrix(vec1, -ang, result2);
			MatrixOperation::Multiply(result1, result2, tmatrix);
		}

 		fraction = oldfraction;
		if (m_bLinear)
		{
			res = reslinear;
		}

}

void CKeyframeQuatSquad::Serialize(CUtilityXMLTag *xmlgen)
{
	char text[SA_BUFFER_SIZE_BIG];

	char esptext[SA_BUFFER_SIZE_BIG];
	if (m_ExtraSpins)
		SACLOCALE(sprintf(esptext, "[es %d]", m_ExtraSpins));
	else
		SACLOCALE(sprintf(esptext, "%s", ""));


	if (m_bLinear) {
		SACLOCALE(sprintf(text, "[%s l %f %f %f %f]", esptext, m_quat.x, m_quat.y, m_quat.z, m_quat.w));
	}
	else
		SACLOCALE(sprintf(text, "[%s %f %f %f %f]", esptext, m_quat.x, m_quat.y, m_quat.z, m_quat.w));
	xmlgen->AddListItem(text);
}


CKeyframeString::CKeyframeString()
{
	strcpy(m_target, "");
	m_type =  ANIString;
	m_bDiscrete = false;
}

/*
CKeyframe& CKeyframeString::operator=(const CKeyframe &kfp) 
{
	if (this != &kfp)
	{
		if (kfp.m_type == ANIString)
			strcpy(m_target, ((CKeyframeString &)kfp).m_target);
	}
	return *this;
}
*/

void CKeyframeString::SetTarget(const char *target) 
{ 
	if(target && (int)strlen(target) < SA_BUFFER_SIZE_BIG)
		strcpy(m_target, target); 
}

void CKeyframeString::Serialize(CUtilityXMLTag *xmlgen)
{
	char result[SA_BUFFER_SIZE_BIG];
	strcpy(result, m_target);

	if (!m_bDiscrete)
 		xmlgen->AddListItem(result);
	else
	{
		char res2[4096];
		sprintf(res2, "[ d %s]",result);
 		xmlgen->AddListItem(res2);
	}
}







CKeyframe3String::CKeyframe3String()
{
	strcpy(m_t1, "");
	strcpy(m_t2, "");
	strcpy(m_t3, "");
	m_type =  ANI3String;
}

 

void CKeyframe3String::SetTarget(const char *t1,const char *t2,const char *t3) 
{ 
	if(t1 && (int)strlen(t1) < 255)
		strcpy(m_t1, t1); 
	if(t2 && (int)strlen(t2) < 255)
		strcpy(m_t2, t2); 
	if(t3 && (int)strlen(t3) < 255)
		strcpy(m_t3, t3); 
}

void CKeyframe3String::Serialize(CUtilityXMLTag *xmlgen)
{
	char result[SA_BUFFER_SIZE_BIG];
	//char temp[SA_BUFFER_SIZE_BIG];
	//char temp2[SA_BUFFER_SIZE_BIG];
	//NS_SimulationAnimation::STargetObject::CreateTargetString("PLCID", m_t1, temp, false);
	//NS_SimulationAnimation::STargetObject::CreateTargetString("PLCID", m_t2, temp2, false);
	sprintf(result, "\"%s$$%s$$%s\"", m_t1, m_t2, m_t3);
 	xmlgen->AddListItem(result);
}

bool CSACommonAPI::m_bUseTargetObjectTypeFolder = false;
void CSACommonAPI::GetQuatPoint(CUtilityTextParser *tp, AniQuat &quat)
{
	static delimiter_t number_td[] = {{"\x30-\x39|-|", false, " |]", true, false}};
 
	char result[SA_BUFFER_SIZE_BIG];
	int dummy;
	tp->ParseBuffer(result, dummy, number_td, 1);
	SACLOCALE(quat.x = (float)atof(result));
	tp->ParseBuffer(result, dummy, number_td, 1);
	SACLOCALE(quat.y = (float)atof(result));
	tp->ParseBuffer(result, dummy, number_td, 1);
	SACLOCALE(quat.z = (float)atof(result));
	tp->ParseBuffer(result, dummy, number_td, 1);
	SACLOCALE(quat.w = (float)atof(result));
}

void CSACommonAPI::GetMatrix(CUtilityTextParser *tp, float *matrix)
{
	static delimiter_t number_td[] = {{"\x30-\x39|-|", false, " |]", true, false}};
 
	char result[SA_BUFFER_SIZE_BIG];
	int dummy;
	for (int i=0;i<16;i++)
	{	
		tp->ParseBuffer(result, dummy, number_td, 1);
		SACLOCALE(matrix[i] = (float)atof(result));
	}
}

void CSACommonAPI::GetMatrix(const char *text, float *matrix)
{
	CUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
 		tp.SkipAhead(1); 
	GetMatrix(&tp, matrix);
}


void CSACommonAPI::GetQuatPoint(const char *text, AniQuat &quat)
{
	CUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
 		tp.SkipAhead(1); 
	GetQuatPoint(&tp, quat);
}

AniPoint CSACommonAPI::UniTanslationToMtxTanslation(AniPoint pntPivot, AniQuat quat, AniPoint pntPos)
{
	AniPoint pntPosRet(0.0,0.0,0.0);
	float fMtxAA[4][4];
	quat.ToMatrix((float*)fMtxAA);
	float fPointA[3];
	fPointA[0] = pntPos.x;
	fPointA[1] = pntPos.y;
	fPointA[2] = pntPos.z;
	float fPivot[3];
	fPivot[0] = pntPivot.x;
	fPivot[1] = pntPivot.y;
	fPivot[2] = pntPivot.z;
	//将当前位置转换为沿旋转中心点旋转至单位矩阵后的坐标位置
	MatrixOperation::UniTanslationToMtxTanslation(fPivot,fMtxAA,fPointA);
	pntPosRet.Set(fPointA[0],fPointA[1],fPointA[2]);
	return pntPosRet;
}

AniPoint CSACommonAPI::MtxTanslationToUniTanslation(AniPoint pntPivot, AniQuat quat, AniPoint pntPos)
{
	AniPoint pntPosRet(0.0,0.0,0.0);
	float fMtxAA[4][4];
	quat.ToMatrix((float*)fMtxAA);
	float fPointA[3];
	fPointA[0] = pntPos.x;
	fPointA[1] = pntPos.y;
	fPointA[2] = pntPos.z;
	float fPivot[3];
	fPivot[0] = pntPivot.x;
	fPivot[1] = pntPivot.y;
	fPivot[2] = pntPivot.z;
	//将当前位置转换为沿旋转中心点旋转至单位矩阵后的坐标位置
	MatrixOperation::MtxTanslationToUniTanslation(fPivot,fMtxAA,fPointA);
	pntPosRet.Set(fPointA[0],fPointA[1],fPointA[2]);
	return pntPosRet;
}

void CSACommonAPI::MultDirMatrix(const float* MatrixA,AniPoint& srcDir,AniPoint& dstDir)
{
	float dSrcDir[3];
	float dDstDir[3];
	dSrcDir[0] = srcDir.x;
	dSrcDir[1] = srcDir.y;
	dSrcDir[2] = srcDir.z;

	MatrixOperation::MultDirMatrix(MatrixA,(float*)dSrcDir,(float*)dDstDir);
	dstDir.x = dDstDir[0];
	dstDir.y = dDstDir[1];
	dstDir.z = dDstDir[2];
}

CSAnimation * CSACommonAPI::AddAnimation(NS_SimulationAnimation::CSBehaviorAction  * pBehaviorAction,const char *name, const char *targetpath, AniPoint* pivot, CSAnimation *pParentAnimation)
{
	if(NULL == pBehaviorAction)
		return NULL;
	CSAnimation *ani = new CSAnimation(name, pBehaviorAction, pParentAnimation);
 	if (targetpath)
	{
		if(strstr(targetpath,TARGETOBJECTTYPE_FOLDER) )
		{
			ani->SetTargetByPath(targetpath);
		}
		else
		{
			ani->SetTargetByPath(name,targetpath);
		}
		if (pivot)
			ani->GetTarget()->SetPivot(pivot->x, pivot->y, pivot->z);
		
	}
	if(pParentAnimation)
	{
		pParentAnimation->AddChildAnimation(ani);
	}
	//else
	//{
		pBehaviorAction->AddAnimation(ani);
	//}
	
	return ani;
}

void CSACommonAPI::AddPosKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint& pos, bool linear)
{
	if(!pBehaviorAction || !pAnimation)
		return;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}

	CSInterpolatorPosition * interpol= NULL;
	vlist_s * pListInterpolatorList = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, pListInterpolatorList)
		if(strcmp(temp->GetType(), INTERPOLATOR_POS) == 0 )
		{
			interpol = (CSInterpolatorPosition *)temp;
			break;
		}
	END_LIST_ITERATION(pListInterpolatorList);
	if (!interpol)
	{
		interpol = new CSInterpolatorPosition();
		pAnimation->AddInterpolator(interpol);
	}
	bool replace = true;;
	int l = 0;
	if((l = pAnimation->GetTimeline()->CheckKeyframe(tick)) == -1)
	{
		l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
	}
	if (l < 0)
		l = 0;
	if(replace)
	{
		if (!linear)
			interpol->ReplaceCurve(pos, l);
		else
			interpol->ReplaceLinear(pos, l);
	}
	else
	{
		if (!linear)
			interpol->InsertCurve(pos, l);
		else
			interpol->InsertLinear(pos, l);
	}
	pAnimation->GetTimeline()->AddTLRange();
}

void CSACommonAPI::AddRotKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, AniQuat& quat, bool linear)
{
	if(NULL == pBehaviorAction)
		return;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}

	CSInterpolatorQuatSquad * interpol= NULL;
	vlist_s * pListInterpolatorList = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, pListInterpolatorList)
		if(strcmp(temp->GetType(), INTERPOLATOR_QUATROT) == 0 )
		{
			interpol = (CSInterpolatorQuatSquad*)temp;
			break;
		}
	END_LIST_ITERATION(pListInterpolatorList);
	if (!interpol)
	{
		interpol = new CSInterpolatorQuatSquad();
		pAnimation->AddInterpolator(interpol);
	}
	bool replace = true;
	int l = 0;
	if((l = pAnimation->GetTimeline()->CheckKeyframe(tick)) == -1)
	{
		l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
	}

	if (l < 0)
		l = 0;
	if(replace)
	{
		if (!linear)
			interpol->Replace(quat, l);
		else
			interpol->ReplaceLinear(quat, l);
	}
	else
	{
		if (!linear)
			interpol->Insert(quat, l);
		else
			interpol->InsertLinear(quat, l);
	}
	pAnimation->GetTimeline()->AddTLRange();
}

void CSACommonAPI::AddScaleKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint& scale)
{
	if(!pBehaviorAction || !pAnimation)
		return;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}

	CSInterpolatorScale * interpol= NULL;
	vlist_s * pListInterpolatorList = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, pListInterpolatorList)
		if(strcmp(temp->GetType(), INTERPOLATOR_SCALE) == 0 )
		{
			interpol = (CSInterpolatorScale *)temp;
			break;
		}
	END_LIST_ITERATION(pListInterpolatorList);
	if (!interpol)
	{
		interpol = new CSInterpolatorScale();
		pAnimation->AddInterpolator(interpol);
	}
	bool replace = true;;
	int l = 0;
	if((l = pAnimation->GetTimeline()->CheckKeyframe(tick)) == -1)
	{
		l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
	}
	if (l < 0)
		l = 0;
	if(replace)
	{
		interpol->Replace(scale, l);
	}
	else
	{
		interpol->Insert(scale, l);
	}
	pAnimation->GetTimeline()->AddTLRange();
}

void CSACommonAPI::AddVisibleKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, char* strVisible)
{
	if(!pBehaviorAction || !pAnimation)
		return;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}

	CSInterpolatorVisible * interpol= NULL;
	vlist_s * pListInterpolatorList = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, pListInterpolatorList)
		if(strcmp(temp->GetType(), INTERPOLATOR_VISIBLE) == 0 )
		{
			interpol = (CSInterpolatorVisible *)temp;
			break;
		}
	END_LIST_ITERATION(pListInterpolatorList);
	if (!interpol)
	{
		interpol = new CSInterpolatorVisible();
		pAnimation->AddInterpolator(interpol);
	}
	bool replace = true;;
	int l = 0;
	if((l = pAnimation->GetTimeline()->CheckKeyframe(tick)) == -1)
	{
		l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
	}
	if (l < 0)
		l = 0;
	if(replace)
	{
		interpol->Replace(strVisible, l);
	}
	else
	{
		interpol->Insert(strVisible, l);
	}
	pAnimation->GetTimeline()->AddTLRange();
}

void CSACommonAPI::AddColorKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint& color,bool bDiscrete)
{
	if(!pBehaviorAction || !pAnimation)
		return;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}

	CSInterpolatorColor * interpol= NULL;
	vlist_s * pListInterpolatorList = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, pListInterpolatorList)
		if(strcmp(temp->GetType(), INTERPOLATOR_COLOR) == 0 )
		{
			interpol = (CSInterpolatorColor *)temp;
			break;
		}
	END_LIST_ITERATION(pListInterpolatorList);
	if (!interpol)
	{
		interpol = new CSInterpolatorColor();
		pAnimation->AddInterpolator(interpol);
	}
	bool replace = true;;
	int l = 0;
	if((l = pAnimation->GetTimeline()->CheckKeyframe(tick)) == -1)
	{
		l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
	}
	if (l < 0)
		l = 0;
	if(replace)
	{
		if(bDiscrete)
		{
			interpol->ReplaceDiscrete(color, l);
		}
		else
		{
			interpol->Replace(color, l);
		}
		
	}
	else
	{
		if(bDiscrete)
		{
			interpol->InsertDiscrete(color, l);
		}
		else
		{
			interpol->Insert(color, l);
		}
	}
	pAnimation->GetTimeline()->AddTLRange();
}

/*******************************************************************
*	@functin	插入一个声音关键帧
*	@param[in]	pBehaviorAction-当前动画管理器 pAnimation-当前动画
*				tick-帧值 strSound-关键帧说明(play/stop)
*	@retval		无
*	@author		Add By YL 2014.11.6
*   @modify		
********************************************************************/
void CSACommonAPI::AddSoundKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, char* strSound)
{
	if(!pBehaviorAction || !pAnimation)
		return;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}

	CSInterpolatorSound * interpol= NULL;
	vlist_s * pListInterpolatorList = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, pListInterpolatorList)
	if(strcmp(temp->GetType(), INTERPOLATOR_SOUND) == 0 )
	{
		interpol = (CSInterpolatorSound *)temp;
		break;
	}
	END_LIST_ITERATION(pListInterpolatorList);
	if (!interpol)
	{
		interpol = new CSInterpolatorSound();
		pAnimation->AddInterpolator(interpol);
	}
	bool replace = true;;
	int l = 0;
	if((l = pAnimation->GetTimeline()->CheckKeyframe(tick)) == -1)
	{
		l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
	}
	if (l < 0)
		l = 0;
	if(replace)
	{
		interpol->Replace(strSound, l);
	}
	else
	{
		interpol->Insert(strSound, l);
	}
	pAnimation->GetTimeline()->AddTLRange();
}

/*******************************************************************
*	@functin	插入一个图片关键帧
*	@param[in]	NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, 动画管理器
				CSAnimation *pAnimation, 当前动画
				int tick, 当前帧
				AniPoint& pos, 图片位置信息
				AniPoint& scale, 缩放信息
				char* strImage, 图片是否显示
				bool linear, 线性
*	@retval		无
*	@author		Add By YL 2015.1.19
*   @modify		Edit By Zhangcc 2015/03/03
********************************************************************/
void CSACommonAPI::AddImageKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint& pos, AniPoint& scale, char* strVisible, bool linear)
{
	CSInterpolatorPosition * pInterpolatorPos = NULL;
	CSInterpolatorScale * pInterpolatorScale = NULL;
	CSInterpolatorVisible * pInterpolatorVisible = NULL;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}

	vlist_s *pInterpolatorList = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, pInterpolatorList);
		if (strcmp(temp->GetType(), INTERPOLATOR_POS) == 0)
		{
			pInterpolatorPos = (CSInterpolatorPosition *)temp;
		}
		if (strcmp(temp->GetType(), INTERPOLATOR_SCALE) == 0)
		{
			pInterpolatorScale = (CSInterpolatorScale *)temp;
		}
		if (strcmp(temp->GetType(), INTERPOLATOR_VISIBLE) == 0)
		{
			pInterpolatorVisible = (CSInterpolatorVisible *)temp;
		}
	END_LIST_ITERATION(pInterpolatorList);

	if (!pInterpolatorPos)
	{
		pInterpolatorPos = new CSInterpolatorPosition();
		pAnimation->AddInterpolator(pInterpolatorPos);
	}
	if (!pInterpolatorScale)
	{	 
		pInterpolatorScale = new CSInterpolatorScale();
		pAnimation->AddInterpolator(pInterpolatorScale);
	}
	if (!pInterpolatorVisible)
	{
		pInterpolatorVisible = new CSInterpolatorVisible();
		pAnimation->AddInterpolator(pInterpolatorVisible);
	}

	bool replace;
	int l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
	if (l<=0)
		l=0;

	if (replace)
	{
		if (!linear)
			pInterpolatorPos->ReplaceCurve(pos, l);
		else
			pInterpolatorPos->ReplaceLinear(pos, l);
		pInterpolatorScale->Replace(scale, l);
		pInterpolatorVisible->Replace(strVisible, l);
	}
	else
	{
		if (!linear)
			pInterpolatorPos->InsertCurve(pos, l);
		else
			pInterpolatorPos->InsertLinear(pos, l);
		pInterpolatorScale->Insert(scale, l);
		pInterpolatorVisible->Insert(strVisible, l);
	}
	pAnimation->GetTimeline()->AddTLRange();
}
/************************************************************************/
/* 功能：录制剖面插入关键帧的方法
/* 参数：NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction, 动画管理器
		 CSAnimation* pAnimation, 当前动画
		 int tick, 当前帧
		 AniPoint pos, 位置
		 AniPoint normal, 法向
		 char* strVisible, 是否显示剖面
		 bool linear, 直线还是曲线
	返回值：空
	创建：zhangcc 2015/03/13
/************************************************************************/
void CSACommonAPI::AddClipKeyframe(NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction, CSAnimation* pAnimation, int tick, AniPoint pos, AniPoint normal, char* strVisible, bool linear)
{
	CSInterpolatorPosition * pInterpolatorPos = NULL;
	CSInterpolatorNormal * pInterpolatorNormal = NULL;
	CSInterpolatorVisible * pInterpolatorVisible = NULL;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}

	vlist_s *pInterpolatorList = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, pInterpolatorList);
		if (strcmp(temp->GetType(), INTERPOLATOR_POS) == 0)
		{
			pInterpolatorPos = (CSInterpolatorPosition *)temp;
		}
		if (strcmp(temp->GetType(), INTERPOLATOR_NORMAL) == 0)
		{
			pInterpolatorNormal = (CSInterpolatorNormal *)temp;
		}
		if (strcmp(temp->GetType(), INTERPOLATOR_VISIBLE) == 0)
		{
			pInterpolatorVisible = (CSInterpolatorVisible *)temp;
		}
	END_LIST_ITERATION(pInterpolatorList);

	if (!pInterpolatorPos)
	{
		pInterpolatorPos = new CSInterpolatorPosition();
		pAnimation->AddInterpolator(pInterpolatorPos);
	}
	if (!pInterpolatorNormal)
	{	 
		pInterpolatorNormal = new CSInterpolatorNormal();
		pAnimation->AddInterpolator(pInterpolatorNormal);
	}
	if (!pInterpolatorVisible)
	{
		pInterpolatorVisible = new CSInterpolatorVisible();
		pAnimation->AddInterpolator(pInterpolatorVisible);
	}

	bool replace;
	int l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
	if (l<=0)
		l=0;

	if (replace)
	{
		if (!linear)
			pInterpolatorPos->ReplaceCurve(pos, l);
		else
			pInterpolatorPos->ReplaceLinear(pos, l);
		pInterpolatorNormal->Replace(normal, l);
		pInterpolatorVisible->Replace(strVisible, l);
	}
	else
	{
		if (!linear)
			pInterpolatorPos->InsertCurve(pos, l);
		else
			pInterpolatorPos->InsertLinear(pos, l);
		pInterpolatorNormal->Insert(normal, l);
		pInterpolatorVisible->Insert(strVisible, l);
	}
	pAnimation->GetTimeline()->AddTLRange();
}

void CSACommonAPI::AddPosRotKeyframe(CSBehaviorAction *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint pivot, AniPoint pos, bool linear, AniQuat quat, bool linear2,bool bFollowPath)
{
	CSInterpolatorPivot * interpolatorPivot = 0;
	CSInterpolatorPosition * interpolatorPos = 0;
	CSInterpolatorQuatSquad * interpolatorQuatSquad = 0;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}
 
	vlist_s *il = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, il);
		if (strcmp(temp->GetType(), INTERPOLATOR_PIVOT) == 0)
		{
			interpolatorPivot = (CSInterpolatorPivot *)temp;
		}
		if (strcmp(temp->GetType(), INTERPOLATOR_POS) == 0)
		{
			interpolatorPos = (CSInterpolatorPosition *)temp;
		}
		if (strcmp(temp->GetType(), INTERPOLATOR_QUATROT) == 0)
		{
			interpolatorQuatSquad = (CSInterpolatorQuatSquad *)temp;
		}
	END_LIST_ITERATION(il);

	if (!interpolatorPivot)
	{
		interpolatorPivot = new CSInterpolatorPivot();
		pAnimation->AddInterpolator(interpolatorPivot);
	}
	if (!interpolatorPos)
	{
		interpolatorPos = new CSInterpolatorPosition();
 		pAnimation->AddInterpolator(interpolatorPos);

		for(int i = 0; i< pAnimation->GetTimeline()->GetTimelineArrayLength(); i++)
		{
			interpolatorPos->InsertLinear(pos,i);
		}
	}
	if (!interpolatorQuatSquad)
	{	 
		interpolatorQuatSquad = new CSInterpolatorQuatSquad();
		pAnimation->AddInterpolator(interpolatorQuatSquad);

		for(int i = 0; i< pAnimation->GetTimeline()->GetTimelineArrayLength(); i++)
		{
			interpolatorQuatSquad->InsertLinear(quat,i);
		}
	}
	
 	bool replace;
	int l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
	if (l<=0)
		l=0;
	if(replace)
	{
		((CSInterpolatorPivot *)(interpolatorPivot))->Replace(pivot, l);
		if(!bFollowPath)
		{
			if (!linear)
				((CSInterpolatorPosition *)(interpolatorPos))->ReplaceCurve(pos, l);
			else
				((CSInterpolatorPosition *)(interpolatorPos))->ReplaceLinear(pos, l);
		}
		else
		{
			((CSInterpolatorPosition *)(interpolatorPos))->ReplaceFollowPath(pos, l,linear);
		}
		if (!linear2)
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->Replace(quat, l);
		else
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->ReplaceLinear(quat, l);

	}
	else
	{
		((CSInterpolatorPivot *)(interpolatorPivot))->Insert(pivot, l);
		if(!bFollowPath)
		{
			if (!linear)
				((CSInterpolatorPosition *)(interpolatorPos))->InsertCurve(pos, l);
			else
				((CSInterpolatorPosition *)(interpolatorPos))->InsertLinear(pos, l);
		}
		else
		{
			((CSInterpolatorPosition *)(interpolatorPos))->InsertFollowPath(pos, l,linear);
		}
		if (!linear2)
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->Insert(quat, l);
		else
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->InsertLinear(quat, l);
	}
 }

void CSACommonAPI::AddCameraKeyframe(CSBehaviorAction *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint pivot, AniPoint pos, AniQuat quat, AniPoint scale, bool linear)
{
	CSInterpolatorPivot * interpolatorPivot = 0;
	CSInterpolatorPosition * interpolatorPos = 0;
	CSInterpolatorQuatSquad * interpolatorQuatSquad = 0;
	CSInterpolatorScale * interpolatorScale = 0;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}

	vlist_s *il = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, il);
	if (strcmp(temp->GetType(), INTERPOLATOR_PIVOT) == 0)
	{
		interpolatorPivot = (CSInterpolatorPivot *)temp;
	}
	if (strcmp(temp->GetType(), INTERPOLATOR_POS) == 0)
	{
		interpolatorPos = (CSInterpolatorPosition *)temp;
	}
	if (strcmp(temp->GetType(), INTERPOLATOR_QUATROT) == 0)
	{
		interpolatorQuatSquad = (CSInterpolatorQuatSquad *)temp;
	}
	if (strcmp(temp->GetType(), INTERPOLATOR_SCALE) == 0)
	{
		interpolatorScale = (CSInterpolatorScale *)temp;
	}
	END_LIST_ITERATION(il);

	if (!interpolatorPivot)
	{
		interpolatorPivot = new CSInterpolatorPivot();
		pAnimation->AddInterpolator(interpolatorPivot);
	}
	if (!interpolatorPos)
	{
		interpolatorPos = new CSInterpolatorPosition();
		pAnimation->AddInterpolator(interpolatorPos);
	}
	if (!interpolatorQuatSquad)
	{	 
		interpolatorQuatSquad = new CSInterpolatorQuatSquad();
		pAnimation->AddInterpolator(interpolatorQuatSquad);
	}
	if (!interpolatorScale)
	{
		interpolatorScale = new CSInterpolatorScale();
		pAnimation->AddInterpolator(interpolatorScale);
	}

	bool replace;
	int l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
	if (l<=0)
		l=0;
	if(replace)
	{
		((CSInterpolatorPivot *)(interpolatorPivot))->Replace(pivot, l);
		if (!linear)
		{
			((CSInterpolatorPosition *)(interpolatorPos))->ReplaceCurve(pos, l);
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->Replace(quat, l);
		}
		else
		{
			((CSInterpolatorPosition *)(interpolatorPos))->ReplaceLinear(pos, l);
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->ReplaceLinear(quat, l);
		}
		((CSInterpolatorScale *)(interpolatorScale))->Replace(scale, l);
	}
	else
	{
		((CSInterpolatorPivot *)(interpolatorPivot))->Insert(pivot, l);
		if (!linear)
		{
			((CSInterpolatorPosition *)(interpolatorPos))->InsertCurve(pos, l);
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->Insert(quat, l);
		}
		else
		{
			((CSInterpolatorPosition *)(interpolatorPos))->InsertLinear(pos, l);
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->InsertLinear(quat, l);
		}
		((CSInterpolatorScale *)(interpolatorScale))->Insert(scale, l);
	}
	
	pAnimation->GetTimeline()->AddTLRange();
}

void CSACommonAPI::AddZoomKeyframe(CSBehaviorAction *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint pos, AniQuat quat, AniPoint scale, bool linear, const char* strVisible,bool bRelaceVisible)
{
	CSInterpolatorPosition * interpolatorPos = 0;
	CSInterpolatorQuatSquad * interpolatorQuatSquad = 0;
	CSInterpolatorScale * interpolatorScale = 0;
	CSInterpolatorVisible * interpolatorVisible = 0;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}

	vlist_s *il = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, il);
	if (strcmp(temp->GetType(), INTERPOLATOR_POS) == 0)
	{
		interpolatorPos = (CSInterpolatorPosition *)temp;
	}
	if (strcmp(temp->GetType(), INTERPOLATOR_QUATROT) == 0)
	{
		interpolatorQuatSquad = (CSInterpolatorQuatSquad *)temp;
	}
	if (strcmp(temp->GetType(), INTERPOLATOR_SCALE) == 0)
	{
		interpolatorScale = (CSInterpolatorScale *)temp;
	}
	if (strcmp(temp->GetType(), INTERPOLATOR_VISIBLE) == 0)
	{
		interpolatorVisible = (CSInterpolatorVisible *)temp;
	}
	END_LIST_ITERATION(il);

	if (!interpolatorVisible)
	{
		interpolatorVisible = new CSInterpolatorVisible();
		pAnimation->AddInterpolator(interpolatorVisible);
	}
	if (!interpolatorPos)
	{
		interpolatorPos = new CSInterpolatorPosition();
		pAnimation->AddInterpolator(interpolatorPos);
	}
	if (!interpolatorQuatSquad)
	{	 
		interpolatorQuatSquad = new CSInterpolatorQuatSquad();
		pAnimation->AddInterpolator(interpolatorQuatSquad);
	}
	if (!interpolatorScale)
	{
		interpolatorScale = new CSInterpolatorScale();
		pAnimation->AddInterpolator(interpolatorScale);
	}
	

	bool replace;
	int l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
	if (l<=0)
		l=0;

	if(replace)
	{
		if (!linear)
		{
			((CSInterpolatorPosition *)(interpolatorPos))->ReplaceCurve(pos, l);
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->Replace(quat, l);
		}
		else
		{
			((CSInterpolatorPosition *)(interpolatorPos))->ReplaceLinear(pos, l);
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->ReplaceLinear(quat, l);
		}
		((CSInterpolatorScale *)(interpolatorScale))->Replace(scale, l);
		if(bRelaceVisible)
		{
			((CSInterpolatorVisible *)(interpolatorVisible))->Replace(strVisible, l);
		}
	}
	else
	{
		if (!linear)
		{
			((CSInterpolatorPosition *)(interpolatorPos))->InsertCurve(pos, l);
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->Insert(quat, l);
		}
		else
		{
			((CSInterpolatorPosition *)(interpolatorPos))->InsertLinear(pos, l);
			((CSInterpolatorQuatSquad *)(interpolatorQuatSquad))->InsertLinear(quat, l);
		}
		((CSInterpolatorScale *)(interpolatorScale))->Insert(scale, l);
		((CSInterpolatorVisible *)(interpolatorVisible))->Insert(strVisible, l);
	}

	pAnimation->GetTimeline()->AddTLRange();
}


void CSACommonAPI::AddToolKeyframe(NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction, CSAnimation* pAnimation, int tick, char* strPlcPath, char* toolPath, char* parentPath)
{
	if(!pBehaviorAction || !pAnimation)
		return;

	if(!pAnimation->GetTimeline())
	{
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
	}

	CSInterpolatorInstanceCreate * interpol= NULL;
	vlist_s * pListInterpolatorList = pAnimation->GetInterpolatorList();
	START_LIST_ITERATION(CSInterpolator, pListInterpolatorList)
		if(strcmp(temp->GetType(), INTERPOLATOR_INSTANCECREATE) == 0 )
		{
			interpol = (CSInterpolatorInstanceCreate *)temp;
			break;
		}
		END_LIST_ITERATION(pListInterpolatorList);
		if (!interpol)
		{
			interpol = new CSInterpolatorInstanceCreate();
			pAnimation->AddInterpolator(interpol);
		}
		bool replace = true;;
		int l = 0;
		if((l = pAnimation->GetTimeline()->CheckKeyframe(tick)) == -1)
		{
			l = pAnimation->GetTimeline()->AddKeyframe(tick, replace);
		}
		if (l < 0)
			l = 0;
		if(replace)
		{
			interpol->Replace(strPlcPath, toolPath, parentPath, l);
		}
		else
		{
			interpol->Insert(strPlcPath, toolPath, parentPath, l);
		}
		pAnimation->GetTimeline()->AddTLRange();
}

CSAnimation* CSACommonAPI::FindAmimation(const char *target, const char *interpolatortype,CSBehaviorAction* pBehaviorAction)
{
	CSAnimation* animation = NULL;
	if(!pBehaviorAction)
	{
		return animation;
	}
	animation = pBehaviorAction->FindAnimation(target,interpolatortype);
	return animation;
}

/************************************************************************/
/* 创建动画文件夹（TargetObject类型为"Folder"的动画），只包含时间线，无插值器                                                                   */
/************************************************************************/
CSAnimation* CSACommonAPI::CreateAmimationFolder(CSAnimation* pParentFolder,const char* strFolderName,CSBehaviorAction* pBehaviorAction)
{
	CSAnimation* pAnimation = NULL;
	if(!pBehaviorAction)
	{
		return pAnimation;
	}

	char cPreFolderName[SA_BUFFER_SIZE_SMALL] = {0};
	char cFolderName[SA_BUFFER_SIZE_SMALL] = {0};


	if(!strFolderName ||
		strlen(strFolderName) <=0)
	{
		strcpy(cPreFolderName, IDS_STRING_NEW_GROUP);
	}
	else
	{
		strcpy(cPreFolderName,strFolderName);
	}
	strcpy(cFolderName,cPreFolderName);
	int i = 1;
	while( pBehaviorAction->FindAnimationByName(cFolderName))
	{
		strcpy(cFolderName,cPreFolderName);
		char cFolderID[20];
		sprintf(cFolderID,"%d",i);
		strcat(cFolderName,cFolderID);
		i++;
	}
	if(!(pAnimation = pBehaviorAction->FindAnimationByName(cFolderName)))
	{
		char  cTargetPath[256];
		//strcpy_s(cTargetPath, MAX_PATH,TARGETOBJECTTYPE_FOLDER);
		////strcat_s(cTargetPath,":0");
		long iTargetKey = pBehaviorAction->RegisterTargetObjectKey();
		sprintf(cTargetPath, "%s:%d",TARGETOBJECTTYPE_FOLDER, iTargetKey);
		pAnimation = CSACommonAPI::AddAnimation(pBehaviorAction,cFolderName,cTargetPath,NULL,pParentFolder);
		CSTimeline *t = new CSTimeline();
		//t->AddKeyframe(pBehaviorAction->GetCurrentTick());
		//t->AddKeyframe(pBehaviorAction->GetCurrentTick()+ m_numAutoKeyFrame);
		//t->GetTimelineArray()[0] = t->GetTimelineArray()[1] = 0; 
		pAnimation->SetTimeline(t);
		t->AddTLRange();
	}
	return pAnimation;
}

void CSACommonAPI::GetAllAmimationFolder(vlist_s* pAmimationFolderList,CSBehaviorAction* pBehaviorAction)
{
	vlist_flush(pAmimationFolderList);

	if(!pBehaviorAction)
	{
		return;
	}

	vlist_s* pAnimationList =pBehaviorAction->GetAnimationList();
	START_LIST_ITERATION(CSAnimation, pAnimationList)
		if (strcmp(temp->GetTarget()->GetType(),TARGETOBJECTTYPE_FOLDER) == 0)
		{
			vlist_add_last(pAmimationFolderList,temp);
		}
		END_LIST_ITERATION(pAnimationList)
}

void CSACommonAPI::GetChildAmimationFolder(CSAnimation* pParentAnimation,vlist_s* pAmimationFolderList,CSBehaviorAction* pBehaviorAction)
{
	//vlist_flush(pAmimationFolderList);
	if(!pBehaviorAction)
	{
		return;
	}

	if(!pParentAnimation)
	{
		GetAllAmimationFolder(pAmimationFolderList,pBehaviorAction);
		return;
	}

	vlist_s* pAnimationList =pParentAnimation->GetChildAnimationList();
	START_LIST_ITERATION(CSAnimation, pAnimationList)
		if (strcmp(temp->GetTarget()->GetType(),TARGETOBJECTTYPE_FOLDER) == 0)
		{
			vlist_add_last(pAmimationFolderList,temp);
			GetChildAmimationFolder(temp,pAmimationFolderList,pBehaviorAction);
		}
		END_LIST_ITERATION(pAnimationList)
}

CSAnimation* CSACommonAPI::GetTopAmimationFolder(CSAnimation* pChildAnimation)
{
	CSAnimation* pAnimation = NULL;
	if(!pChildAnimation)
	{
		return pAnimation;
	}
	pAnimation = pChildAnimation;
	while(pAnimation->GetParentAnimation())
	{
		pAnimation = pAnimation->GetParentAnimation();
	}
	//如果顶级动画不是动画分组，返回空
	if (strcmp(pAnimation->GetTarget()->GetType(),TARGETOBJECTTYPE_FOLDER) != 0)
	{
		pAnimation = NULL;
	}
	return pAnimation;
}

CSAnimation* CSACommonAPI::GetAmimationFolderByName(const char* strName,CSBehaviorAction* pBehaviorAction)
{
	CSAnimation* pAnimation = NULL;
	if(!pBehaviorAction)
	{
		return pAnimation;
	}

	vlist_s* pAnimationList = pBehaviorAction->GetAnimationList();
	START_LIST_ITERATION(CSAnimation, pAnimationList)
		if (strcmp(temp->GetTarget()->GetType(),TARGETOBJECTTYPE_FOLDER) == 0 &&
			strcmp(temp->GetName(),strName) == 0)
		{
			pAnimation = temp;
			break;
		}
		END_LIST_ITERATION(pAnimationList)
			return pAnimation;
}

CSAnimation* CSACommonAPI::GetTargetObjectTypeFolder(const char* strPlcID,CSBehaviorAction* pBehaviorAction)
{
	CSAnimation* pAnimation = NULL;
	if(!pBehaviorAction)
	{
		return pAnimation;
	}
	if(!CSACommonAPI::IsUseTargetObjectTypeFolder())
	{
		return pAnimation;
	}

	char cTargetObjectType[256] = {0};
	if(strstr(strPlcID,TARGETOBJECTTYPE_INS))
	{
		strcpy(cTargetObjectType,TARGETOBJECTTYPE_NAME_INS);
	}
	else if(strstr(strPlcID,TARGETOBJECTTYPE_PMI))
	{
		strcpy(cTargetObjectType,TARGETOBJECTTYPE_NAME_PMI);
	}
	else if(strstr(strPlcID,TARGETOBJECTTYPE_SOUND))
	{
		strcpy(cTargetObjectType,TARGETOBJECTTYPE_NAME_SOUND);
	}
	else if(strstr(strPlcID,TARGETOBJECTTYPE_IMAGE))
	{
		strcpy(cTargetObjectType,TARGETOBJECTTYPE_NAME_IMAGE);
	}

	if(strlen(cTargetObjectType) > 0)
	{
		pAnimation = GetAmimationFolderByName(cTargetObjectType,pBehaviorAction);
		if(!pAnimation)
		{
			pAnimation = CreateAmimationFolder(NULL,cTargetObjectType,pBehaviorAction);
		}
	}
	return pAnimation;
}

/************************************************************************/
/* 创建路径创建对象的主空白动画,如果没有，创建该动画                                                                    */
/************************************************************************/
CSAnimation* CSACommonAPI::GetTargetMainAmimation(const char* strPlcID, const char* strObjName,CSBehaviorAction* pBehaviorAction,CSAnimation* pFolderAnimation)
{
	CSAnimation* pAnimation = NULL;
	if(!pBehaviorAction)
	{
		return pAnimation;
	}
	if(!(pAnimation = pBehaviorAction->FindAnimation(strPlcID,NULL)))
	{
		//获取目标对象类型动画分组
		if(pFolderAnimation)
		{
			pAnimation = CSACommonAPI::AddAnimation(pBehaviorAction,strObjName,strPlcID,NULL,pFolderAnimation);
		}
		else
		{
			CSAnimation* pParentAnimation = GetTargetObjectTypeFolder(strPlcID,pBehaviorAction);
			pAnimation = CSACommonAPI::AddAnimation(pBehaviorAction,strObjName,strPlcID,NULL,pParentAnimation);
		}
		
		CSTimeline *t = new CSTimeline();
		pAnimation->SetTimeline(t);
		//t->AddKeyframe(pBehaviorAction->GetCurrentTick());
		//t->AddKeyframe(pBehaviorAction->GetCurrentTick()+ m_numAutoKeyFrame);
		t->AddTLRange();
	}
	return pAnimation;
}
/************************************************************************/
/* 获取所有的子动画（一直到最后一层）
/* 参数：CSAnimation* pAnimation, (I)动画指针
		 vlist_s* pChildList, (O)一直到最后一层的所有子动画
		 bool bIncludeNoInterpolator, (I)是否包含没有插值器的这一层
/************************************************************************/
void CSACommonAPI::GetTailChildAnimationList(CSAnimation* pAnimation, vlist_s* pChildList, bool bIncludeNoInterpolator)
{
	if (pAnimation->GetInterpolator() == NULL)
	{
		vlist_s* pTmpChildList = pAnimation->GetChildAnimationList();
		START_LIST_ITERATION(CSAnimation, pTmpChildList)
			if (bIncludeNoInterpolator && temp && temp->GetInterpolator() == NULL)
				vlist_add_last(pChildList, temp);
			GetTailChildAnimationList(temp, pChildList, bIncludeNoInterpolator);
		END_LIST_ITERATION(pTmpChildList)
	}
	else
	{
		if (pAnimation)
			vlist_add_last(pChildList, (void*)pAnimation);
	}
}

/************************************************************************/
/* 判断是否是录制对象的主动画                                                                   */
/************************************************************************/
bool CSACommonAPI::IsTargetMainAmimation(CSAnimation* pAnimation,NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction)
{
	bool bRet = false;
	if(!pAnimation || !pBehaviorAction)
	{
		return bRet;
	}
	if(!pAnimation->GetInterpolator() && 
		strcmp(pAnimation->GetTarget()->GetType(),TARGETOBJECTTYPE_FOLDER)!= 0)
	{
		bRet = true;
	}
	return bRet;
}

/************************************************************************/
/* 判断是否是动画分组                                                                   */
/************************************************************************/
bool CSACommonAPI::IsAmimationFolder(CSAnimation* pAnimation,NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction)
{
	bool bRet = false;
	if(!pAnimation || !pBehaviorAction)
	{
		return bRet;
	}
	if(strcmp(pAnimation->GetTarget()->GetType(),TARGETOBJECTTYPE_FOLDER)== 0)
	{
		bRet = true;
	}
	return bRet;
}

bool CSACommonAPI::IsTargetObjectTypeFolder(CSAnimation* pAnimation)
{
	bool bRet = false;
	if(!pAnimation)
	{
		return bRet;
	}
	const char* strName = pAnimation->GetName();
	if(strcmp(strName,TARGETOBJECTTYPE_INS) == 0 ||
		strcmp(strName,TARGETOBJECTTYPE_PMI) == 0 ||
		strcmp(strName,TARGETOBJECTTYPE_SOUND) == 0 ||
		strcmp(strName,TARGETOBJECTTYPE_IMAGE) == 0)
	{
		bRet = true;
	}
	return bRet;
}

/************************************************************************/
/* 获取指定类型的动画
/* 参数：CSBehaviorAction* pBehaviorAction,(I)动画管理器
		 const char* strTargetType, (I)动画类型
		 vlist_s* pAnimationList, (O)动画列表
		 bool bIncludeNoInterpolator, (I)是否包含没有插值器的这一层
/************************************************************************/
void CSACommonAPI::GetAnimationByTargetType(NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction, const char* strTargetType, vlist_s* pAnimationList)
{
	if (strTargetType &&strlen(strTargetType) >0)
	{
		vlist_s* pAllAnimationList = pBehaviorAction->GetAnimationList();
		START_LIST_ITERATION(CSAnimation, pAllAnimationList)
			if(strcmp(temp->GetTarget()->GetType(),strTargetType) == 0)
			{
				vlist_add_last(pAnimationList, (void*)temp);
			}
		END_LIST_ITERATION(pAllAnimationList)
	}
}

  //SA_NAMESPACE_END
/************************************************************************/
/*实现剖面关键帧类的成员方法 added by yhp 2014-09-25 Start            */
/************************************************************************/
void CKeyframeChannelClip::CalculateCurveFactor( CSTimeline *timeline, int keyframe )
{
	int nm1, nm0, np1;

	int *tlarray = timeline->GetTimelineArray();
	int length = timeline->GetTimelineArrayLength();
	if (keyframe>0 && keyframe<length)
		nm1 = tlarray[keyframe] - tlarray[keyframe-1];
	else
		nm1 = 0;

	if ( keyframe < length-1)
		nm0 = tlarray[keyframe+1] - tlarray[keyframe];
	else 
		nm0 = 0;

	if (keyframe<length-2)
		np1 = tlarray[keyframe+2] - tlarray[keyframe+1];
	else 
		np1 = 0;
	m_factor1 = (2.0f * nm0)/ (nm1 + nm0);	
	m_factor2 = (2.0f * nm0) / (nm0 + np1);
}

void CKeyframeChannelClip::CalculateHermiteTangents( AniPoint p0, AniPoint p2, AniPoint p3, float a /*= 0.5f*/ )
{
	AniPoint p1 = m_cp;
	a = 0.5f;
	m_tangent1.Set(a * (p2.x - p0.x),a * (p2.y - p0.y), a * (p2.z - p0.z));
	m_tangent2.Set(a * (p3.x - p1.x),a * (p3.y - p1.y), a * (p3.z - p1.z));

	//	float factor = (2.0f * nm0)/ (nm1 + nm0);
	m_tangent1.Set(m_factor1 * m_tangent1.x, m_factor1 * m_tangent1.y, m_factor1 * m_tangent1.z);
	//	 factor = (2.0f * nm0) / (nm0 + np1);
	m_tangent2.Set(m_factor2 * m_tangent2.x, m_factor2 * m_tangent2.y, m_factor2 * m_tangent2.z);
}

AniPoint CKeyframeChannelClip::InterpolateHermiteSpline( float fraction, AniPoint p1, AniPoint p2 )
{
	AniPoint res;
	float s = fraction;
	float s2 = fraction * fraction;
	float s3 = fraction * fraction * fraction;

	float h1 = 2 * s3 - 3 *s2 + 1;
	float h2 = -2 * s3 + 3 * s2;
	float h3 = s3 - 2 * s2 + s;
	float h4 = s3 - s2;
	res.Set(h1 * p1.x + h2 * p2.x + h3 * m_tangent1.x + h4 * m_tangent2.x,
		h1 * p1.y + h2 * p2.y + h3 * m_tangent1.y + h4 * m_tangent2.y,
		h1 * p1.z + h2 * p2.z + h3 * m_tangent1.z + h4 * m_tangent2.z);
	return res;
}

void CKeyframeChannelClip::Interpolate( CKeyframeChannel **posarray, int keyframe, float fraction, int length, AniPoint &res, AniPoint &normal, bool &bIsvisible )
{
	AniPoint p0, p1, p2, p3;
   //计算位置差值
	p1 = posarray[keyframe]->m_cp;
	p2 = posarray[keyframe+1]->m_cp;
	if (keyframe>0)
		p0 = posarray[keyframe - 1]->m_cp;
	else
		p0 = p1;
	if (keyframe<length-2)
		p3 = posarray[keyframe + 2]->m_cp;
	else
		p3 = p2;
	//		CalculateHermiteTangents(p0, p2, p3);
	res = InterpolateHermiteSpline(fraction, p1, p2);
	//计算法向差值
	p1 = ((CKeyframeChannelClip*)posarray[keyframe])->m_clipNormal;
	p2 = ((CKeyframeChannelClip*)posarray[keyframe + 1])->m_clipNormal;
	if (keyframe>0)
		p0 = ((CKeyframeChannelClip*)posarray[keyframe - 1])->m_clipNormal;
	else
		p0 = p1;
	if (keyframe<length-2)
		p3 = ((CKeyframeChannelClip*)posarray[keyframe + 2])->m_clipNormal;
	else
		p3 = p2;
	//		CalculateHermiteTangents(p0, p2, p3);
	normal = InterpolateHermiteSpline(fraction, p1, p2);
	//计算控制显隐状态差值
	bIsvisible = ((CKeyframeChannelClip*)posarray[keyframe])->m_bVisible;
}

void CKeyframeChannelClip::CalculateCurveLength( AniPoint p2 )
{
	float adder = 1.0f / 100.0f;
	float fraction = 0.0f;
	AniPoint oldpoint = m_cp;
	m_curvelength = 0.0f;
	AniPoint delta;
	for (int i=0;i<100;i++,fraction+=adder )
	{
		AniPoint res;
		res = InterpolateHermiteSpline(fraction,m_cp,  p2);
		delta.Set(res.x - oldpoint.x, res.y - oldpoint.y, res.z - oldpoint.z);
				m_curvelength += (float)SAUtility::Compute_Vector_Length(&delta);
		oldpoint = res;
	}
}

void CKeyframeChannelClip::Serialize( CUtilityXMLTag *xmlgen )
{
	char text[SA_BUFFER_SIZE_BIG];
	SACLOCALE(sprintf(text, "[l %f %f %f|%f %f %f|%d]", m_cp.x, m_cp.y, m_cp.z, m_clipNormal.x, m_clipNormal.y, m_clipNormal.z, m_bVisible));
	xmlgen->AddListItem(text);
}
