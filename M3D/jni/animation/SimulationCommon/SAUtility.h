// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		SAUtility.h
*
*	@brief	定义动画相关的几何算法
*
*
*	@par	历史:
*
****************************************************************************/

// All methods are public and static
#ifndef _SAUTILITY_H
#define _SAUTILITY_H

#ifdef USE_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif


#include "SATools.h"

class SAUtility
{
public:
#undef PI
	static const float PI;			//!<	default=3.1415926f

//
	static const float Epsilon;	//!<	default=0.01

//	/*! 
//	  ComputeDoublePrecisionCrossProduct computes the cross product of two double precision vectors.
//	  \param vector1 A pointer to a double precision vector.
//	  \param vector2 A pointer to a double precision vector.
//	  \param up Pass a pointer to a double.  This method will populate <i>up</i> with the result of the cross product.
//	*/
	static void		ComputeDoublePrecisionCrossProduct(double * vector1, double * vector2, double * up);
//
//	/*! 
//	  ComputeDoublePrecisionDotProduct computes a double precision dot product of two vectors.
//	  \param vector1 A pointer to a double precision vector.
//	  \param vector2 A pointer to a double precision vector.
//	  \return The double precision dot product of the two vectors.
//	*/
	static double	ComputeDoublePrecisionDotProduct(double * vector1, double * vector2);
//
//	/*! 
//	  ComputeDoublePrecisionVectorLength computes the length of a double precision vector.
//	  \param vector1 A pointer to a double precision vector.
//	  \return The double precision length of the vector.
//	*/
	static double	ComputeDoublePrecisionVectorLength(double * vector1);

	//计算两向量的夹角
	static int ComputeDoublePrecisionAngle( double vector1[], double vector2[], double *rad_p);
//	/*! 
//	  NormalizeDoublePrecisionVector normalizes a double-precision vector in place so that its magnitude is 1. 
//	  \param vector1 A pointer to a double precision vector.
//	*/
	static void		NormalizeDoublePrecisionVector(double* vector1);

	static void NormalizeVector(const AniPoint& vector1,AniPoint& vector2);
	static void Compute_Cross_Product (const AniPoint& p1,	const AniPoint& p2, AniPoint& pOut);
	static double Compute_Dot_Product (const AniPoint& p1, const AniPoint& p2);
	static double Compute_Vector_Length (const AniPoint& in);
	static void AxisRadToMatrix (AniPoint& vector,double radians,float *out);
	static void MatrixMakeIdentity(float* fMtxAA);

	/************************************************************************/
	/* 功能：计算两向量的偏移矩阵
	*参数：
		matrix：输出，偏移矩阵
		p1：向量1
		p2：向量2
	*/
	/************************************************************************/
	static void ComputeVectorToVectorRotationMatrix(float *matrix,AniPoint& p1,AniPoint& p2);

//	/*!
//	  TransitionQuaternion takes a start and end quaternion which represent two orientations in cartesian space.  
//	  Based on a value t between 0 and 1, it comes up with a third quaternion vector that represents an orientation 
//	  in cartesian space which is along the shortest, smooth path between the two initial orientations.
//	  \param quat_old An array of 4 by 1 floats representing the starting orientation in quaterion space.
//	  \param quat_new An array of 4 by 1 floats representing the final orientation in quaterion space.
//	  \param t A floating point value between 0 and 1 indicating the amount you want the returned quaternion to be along 
//	  the interpolated path.
//	  \param qt A pointer which contains the 4 by 1 vector of the floating point values representing the transition quaternion.  This value is returned to the user.
//	*/
	static void		TransitionQuaternion(float *quat_old, float *quat_new, float t, float *qt);


//	/*!
//	  This method composes a transformation martix from the given camera attributes.
//	  \param target A pointer to AniPoint object representing camera target.
//	  \param position A pointer to AniPoint object representing camera position.
//	  \param up_vector A pointer to AniPoint object representing view-up vector.
//	  \param translation A pointer to AniPoint object representing translation.
//	  \param *matrix Resultant transformation matrix.  Pass a pointer to an array that can accomodate a 4 x 4 matrix.
//	*/
	static void		GetMatrixFromCamera(AniPoint target, AniPoint position, AniPoint up_vector, AniPoint translation, float *matrix);

public:
	static float EaseInEaseOut(float t, float a, float b, float c);
	
};


#ifdef USE_PACK_8
#pragma pack(pop)
#endif

#endif



