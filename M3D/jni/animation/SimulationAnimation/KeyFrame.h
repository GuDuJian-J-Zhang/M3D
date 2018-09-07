// All Rights Reserved.Copyright(C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		KeyFrame.h
*
*	@brief	关键帧类型定义文件
*
*	@par	历史:
*
****************************************************************************/


#ifndef _KEYFRAME_H
#define _KEYFRAME_H

#ifdef USE_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "../SimulationCommon/SATools.h"
#include "SADefine.h"
#include "../SimulationCommon/vlist.h"
#include "STimeline.h"

#include "../SimulationCommon/Animation_def.h"

class CUtilityXMLTag;
class CUtilityTextParser;
class CSTimeline;
class CSAnimation;

SA_NAMESPACE_BEGIN
class CSBehaviorAction;
SA_NAMESPACE_END

/*! \enum ANIKeyframeType Keyframe type */
enum ANIKeyframeType {
	ANIChannel,			//!< Channel keyframe type. 
	ANIRotation,			//!< Rotational keyframe type.
 	ANIString,				//!< String keyframe type.
 	ANI3String				//!< String keyframe type.
};


/*! \enum ANIChannelType Subtype for channel keyframes */
enum ANIChannelType {
	ANILinear,				//!< Linear channel type.
	ANIHermiteSpline,		//!< Hermite spline channel type.
	ANIBezierSpline,		//!< Bezier spline channel type.
	ANIFollowPath,			//!< Follow path channel type.
	ANIDiscrete,			//!< Discrete channel type.
};


/*! \enum ANIRotationType Subtype for rotational keyframes */
enum ANIRotationType {		
	ANIAxisRotation,		//!< Axis rotation type.
	ANIQuatSlerpRotation,	//!< Linear quaternion rotational type.
	ANIQuatSquadRotation,	//!< Non-linear quaternion rotational type.
	ANIEulerRotation		//!< Euler rotation type.
};



					
//! The AniQuat class defines the data type of a Quaternion 
/*!
 The AniQuat class encapsulates the x,y,x,w values used to define a quaternion as well as various
 functions to perform algebrahic operations on quaternions
*/ 

class SA_API AniQuat
{
public:
	float x;		/*!< Quaternion x Coordinate*/
	float y;		/*!< Quaternion y Coordinate*/
	float z;		/*!< Quaternion z Coordinate*/
	float w;		/*!< Quaternion w Coordinate*/
	const AniQuat &Normalize();  /*!< normalizes an AniQuat */
 	/*! default constructor */
	AniQuat() { x=0.0f;y=0.0f;z=0.0f; w=0;};
	/*! constructor that accepts three floats */
	AniQuat(float X,float Y,float Z, float W) { x=X;y=Y;z=Z; w=W;};
	/*! constructor that accepts a pointer to an AniQuat object */
	AniQuat(AniQuat const *p) { x=p->x;y=p->y;z=p->z; w=p->w;};
	/*! constructor that accepts an AniQuat reference */
	AniQuat(const AniQuat &p) { x=p.x;y=p.y;z=p.z; w=p.w;};
	
	/*!  Set packs an AniQuat object with coordinate values.  */
	void Set(float X,float Y,float Z, float W) { x=X;y=Y;z=Z; w=W; };

	/*!
	Multiplies two quaternions
	\param q2 Quaternion to multiply with
	\return Result of multiplication
	*/
   	AniQuat operator *(const AniQuat &q2) const
	{
		
		return AniQuat(y*q2.z - z*q2.y + w*q2.x + x*q2.w,
						z*q2.x - x*q2.z + w*q2.y + y*q2.w,
						x*q2.y - y*q2.x + w*q2.z + z*q2.w,
						w*q2.w - x*q2.x - y*q2.y - z*q2.z);
	}

	/*!
	Multiplies a quaternion with a float value
	\param v Value to multiply Quaternion with
	\return Result of multiplication
	*/
 	AniQuat operator *(float v) const
	{
		return AniQuat(x*v, y*v, z*v, w*v);
	}

	/*!
	Multiplies a quaternion with a float value
	\param v Value to multiply Quaternion with
	\param q Quaternion to multiply with
	\return Result of multiplication
	*/	
	friend AniQuat operator* (float v, const AniQuat& q)
	{
	 	return AniQuat(v*q.x,v*q.y,v*q.z, v*q.w);
	}

	/*!
	Divides a quaternion by a float value
	\param v Value to divide Quaternion by
 	\return Result of multiplication
	*/	
	AniQuat operator /(float v) const
	{
		return AniQuat(x/v, y/v, z/v, w/v);
	}

	/*!
	Subtracts one quaternion from another
	\param q2 Quaternion to subtract with
  	\return Result of subtraction
	*/		AniQuat operator -(const AniQuat &q2) const
	{
 		return AniQuat(x-q2.x, y-q2.y, z-q2.z, w-q2.w);
	}

	/*!
	Add one quaternion to another
	\param q2 Quaternion to add 
  	\return Result of addition
	*/
	AniQuat operator +(const AniQuat &q2) const
	{
		return AniQuat(x+q2.x, y+q2.y, z+q2.z, w+q2.w);
	}

	//判断两个四元数是否相等
	bool Equal(const AniQuat& v, float tolerance) const;
	
	/*!
	Logarithm of a AniQuat, given as:
	Qlog(q) = v*a where q = [cos(a),v*sin(a)]
	\param q Quaternion to perform log on
	\return Result of logarithm
	*/
	static AniQuat Qlog(const AniQuat &q);
 
	/*!
	Convert Quaternion to matrix form
 	\param matrix Returns result matrix
 	*/
	void ToMatrix(float matrix[16]);
	

	/*!
	Qexp(v*a) = [cos(a),vsin(a)]
	\param q Quaternion to perform exp on
	\return Result of Exp
 	*/
 	static AniQuat Qexp(const AniQuat &q);	
	/*!
	Linear interpolation between two HQuats
	\param q1 First Quaternion for Interpolation
	\param q2 Second Quaternion for Interpolation
	\param fraction Interpolation Distance (0 - 1)
	\return Result of interplation
	*/	
	static AniQuat Qlerp(const AniQuat &q1,const AniQuat &q2,float fraction);
	/*!
	Spherical interpolation between two HQuats
	\param q1 First Quaternion for Interpolation
	\param q2 Second Quaternion for Interpolation
	\param fraction Interpolation Distance (0 - 1)
	\return Result of interplation
	*/	
	static AniQuat Qslerp(const AniQuat &q1,const AniQuat &q2,float fraction);
	/*!
	Spherical interpolation between two HQuats
	This version of slerp, used by squad, does not check for theta > 90.
	\param q1 First Quaternion for Interpolation
	\param q2 Second Quaternion for Interpolation
	\param fraction Interpolation Distance (0 - 1)
	\return Result of interplation
	*/	
	static AniQuat QslerpNoInvert(const AniQuat &q1,const AniQuat &q2,float fraction);
	/*!
	Spherical interpolation between two HQuats
	This version of slerp, used by squad, does not check for theta > 90.
	\param q1 First Quaternion for Interpolation
	\param q2 Second Quaternion for Interpolation
	\param fraction Interpolation Distance (0 - 1)
	\return Result of interplation
	*/	
	static AniQuat QslerpNoInvertExtraSpins(const AniQuat &q1,const AniQuat &q2,float fraction, int ExtraSpins);
	/*!
	Spherical cubic interpolation between two HQuats
 	\param q1 First Quaternion for Interpolation
	\param q2 Second Quaternion for Interpolation
	\param a First Control Point
	\param b Second Control Point
	\param fraction Interpolation Distance (0 - 1)
	\return Result of interplation
	*/	
	static AniQuat Qsquad(const AniQuat &q1,const AniQuat &q2,const AniQuat &a,const AniQuat &b,float fraction);
	/*!
	Given 3 HQuats, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
 	\param qnm1 qn-1 control point
 	\param qn qn control point
 	\param qnp1 qn + 1 control point
 	\return Result of interplation
	*/	
	static AniQuat Qspline(const AniQuat &qnm1,const AniQuat &qn,const AniQuat &qnp1);
	/*!
	Convert matrix to quaternion
 	\return converted quaternion
 	*/
	static AniQuat MatrixToQuaternion(float *matrix);

};

 

//! The CKeyframe class is the base class for all keyframe types.
/*!	
	Every interpolator has an array of keyframe values. Based on the given time an interpolation is
	performed between those keyframes which contribute to the overall animation.
	Keyframes can not only hold positional information or rotations but also information like color or segment identifiers.
	For performance reasons, all data members of the various keyframe classes are defined as public.
*/ 
class SA_API CKeyframe
{
public:
 	/*!
	Constructs an CKeyframe object.
 	*/
	CKeyframe()
	{
		m_bConstant = false;
		m_bEaseInOut = false;
	}
	virtual ~CKeyframe() {};

 	/*!
	This method makes an exact copy of the current CKeyframe and returns a pointer to the newly
	created CKeyframe object.
	\return A pointer to the newly created CKeyframe object.
 	*/
	virtual CKeyframe *Clone() { 
		CKeyframe *p = new CKeyframe;
		*p =  *this;
		return p;
	}

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(CUtilityXMLTag *xmlgen) { }
 
	/*!
	Use this method to keep the keyframe value constant.  
	\param constant Pass true if you want to keep the same value as the previous keyframe or false if you
	want to use a different value.
  	*/
	virtual void SetConstant(bool constant) { m_bConstant = constant; }
	/*!
	Use this method to indicate that you want a smooth transition during the start
	and end of the animation.  
	\param easeinout Pass true to turn on easing in and out or false if you don't want this behavior.
  	*/
	virtual void SetEaseInOut(bool easeinout) { m_bEaseInOut = easeinout; }
	/*!This feature is not yet implemented.*/
	virtual void SetRelative(bool relative) { m_bRelative = relative; }
  
	ANIKeyframeType	m_type;			/*!< Type of Keyframe */
	bool				m_bConstant;	/*!< The value is true if this keyframe's value is the same value as the previous keyframe.*/
	bool				m_bRelative;	/*!< This feature is not yet implemented.*/
	bool				m_bEaseInOut;	/*!< The value is true if smooth transition for start and stop is turned on or false if otherwise.*/
};


//! The CKeyframeChannel class is the base class for all channel related keyframe types.
/*!	
	Channel keyframes are used for positional values as well as colour and scale.
*/ 
class  SA_API CKeyframeChannel : public CKeyframe
{
public:

	/*!
	Constructs an CKeyframeChannel object.
 	*/
 	CKeyframeChannel()
	{
		m_cp.Set(0.0f,0.0f,0.0f);
		m_type =  ANIChannel;
		m_channeltype = ANILinear;
	}
	virtual ~CKeyframeChannel() {};

 	/*!
	This method makes an exact copy of the current CKeyframeChannel and returns a pointer to the newly
	created CKeyframeChannel object.
	\return A pointer to the newly created CKeyframeChannel object.
 	*/
	virtual CKeyframe *Clone () 
	{
		CKeyframeChannel *p= new CKeyframeChannel;
		*p = *this;
		return p;
	}

	/*!
	Calculates the interpolation between this keyframe and the next keyframe.
	\param posarray The array of CKeyframeChannel objects.
	\param keyframe They current keyframe number.
	\param fraction The fraction from this keyframe to next.
	\param length The number of entries in the keyframe array.
	\param res Returns the interpolation result.
  	*/
	virtual void Interpolate(CKeyframeChannel **posarray, int keyframe, float fraction, int length, AniPoint &res){}
	
	AniPoint			m_cp;					/*!< The channnel coordinates. */
	ANIChannelType m_channeltype;			/*!< The type of channel. */
};



 
//! The CKeyframeChannelLinear class is used for linear interpolation of positional values.
class  SA_API CKeyframeChannelLinear : public CKeyframeChannel
{
public:
 
	/*!
	Constructs a CKeyframeChannelLinear object.
 	*/
	CKeyframeChannelLinear(float x=0, float y=0,  float z=0) : CKeyframeChannel()
	{
		m_channeltype = ANILinear;
		m_cp.Set(x,y,z);
	}
	virtual ~CKeyframeChannelLinear() {};

 	/*!
	This method makes an exact copy of the current CKeyframeChannelLinear and returns a pointer to the newly
	created CKeyframeChannelLinear object.
	\return A pointer to newly created CKeyframeChannelLinear object.
 	*/
	virtual CKeyframe *Clone () 
	{
		CKeyframeChannelLinear *p= new CKeyframeChannelLinear;
		*p = *this;
		return p;
	}
 
	/*!
	Calculates the interpolation between this keyframe and the next keyframe.
	\param posarray The array of CKeyframeChannel objects.
	\param keyframe They current keyframe number.
	\param fraction The fraction from this keyframe to next.
	\param length The number of entries in the keyframe array.
	\param res Returns the interpolation result.
  	*/
	virtual void Interpolate(CKeyframeChannel **posarray, int keyframe, float fraction, int length, AniPoint &res);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(CUtilityXMLTag *xmlgen);


};

//! The CKeyframeChannelDiscrete class is used for discrete interpolation of positional values.
/*! 
	No interpolation is performed between this keyframe and the next keyframe.
*/
class SA_API CKeyframeChannelDiscrete : public CKeyframeChannel
{
public:
 
	/*!
	Constructs a CKeyframeChannelDiscrete Object.
 	*/
	CKeyframeChannelDiscrete(float x=0, float y=0, float z = 0) : CKeyframeChannel()
	{
		m_channeltype = ANIDiscrete;
		m_cp.Set(x,y,z);

	}
	virtual ~CKeyframeChannelDiscrete() {};

 	/*!
	This method makes an exact copy of the current CKeyframeChannelDiscrete and returns a pointer to the newly
	created CKeyframeChannelDiscrete object.
	\return A pointer to the newly created CKeyframeChannelDiscrete object.
 	*/
	virtual CKeyframe *Clone () 
	{
		CKeyframeChannelDiscrete *p= new CKeyframeChannelDiscrete;
		*p = *this;
		return p;
	}

	/*!
	Calculates the interpolation between this keyframe and the next keyframe.  In this case, there is no interpolation.
	The calculated position will be the same position value of the given keyframe if the fraction is less than one.  Otherwise, 
	the caluculated position will be the position value of the next keyframe in the array.
	\param posarray The array of CKeyframeChannel objects.
	\param keyframe They current keyframe number.
	\param fraction The fraction from this keyframe to next.
	\param length The number of entries in the keyframe array.
	\param res Returns the interpolation result.
  	*/
	virtual void Interpolate(CKeyframeChannel **posarray, int keyframe, float fraction, int length, AniPoint &res);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(CUtilityXMLTag *xmlgen);

	
 
};



//! The CKeyframeChannelCurve class is used for spline based interpolation of positional values.
/*! 
*/
class  SA_API CKeyframeChannelCurve : public CKeyframeChannel
{
public:

	/*!
	Constructs a CKeyframeChannelCurve Object.
 	*/
	CKeyframeChannelCurve(float x = 0, float y = 0, float z = 0) : CKeyframeChannel()
	{
		m_channeltype = ANIHermiteSpline;
		m_cp.Set(x,y,z);

	}
 	virtual ~CKeyframeChannelCurve() {};

 	/*!
	This method makes an exact copy of the current CKeyframeChannelCurve and returns a pointer to the newly
	created CKeyframeChannelCurve object.
	\return A pointer to the newly created CKeyframeChannelCurve object.
 	*/
	virtual CKeyframe *Clone () 
	{
		CKeyframeChannelCurve *p= new CKeyframeChannelCurve;
		*p = *this;
		return p;
	}

 	/*!
	This method is a helper function for CalculateHermiteTangents().  Call this method
	to precalculate curve factors before calling CalculateHermiteTangents().
	\param timeline Timeline array necessary to calculate correct curve factors
	\param keyframe Current Keyframe
  	*/
	void CalculateCurveFactor(CSTimeline *timeline, int keyframe);

 	/*!
	Use this method to precalculate tangents for the hermite spline.  This method can be usef as a helper function
	for interpolation.
	\param p0 The first point for tangent calculation.
	\param p2 The third point for tangent calculation.
	\param p3 The fourth Point for tangent calculation.
	\param a Constant for hermite spline
   	*/
	void CalculateHermiteTangents(AniPoint p0, AniPoint p2, AniPoint p3, float a = 0.5f); 

 	/*!
	This method uses the given frame and control points to interpolate a Hermite spline. 
	\param fraction Distance between the first and second control point.
	\param p1 The first control point.
	\param p2 The second control point.
	\return The interpolated result.
   	*/
	AniPoint InterpolateHermiteSpline(float fraction, AniPoint p1, AniPoint p2);

//	AniPoint InterpolateBezierSpline(float fraction, AniPoint p2);


 	/*!
	This method calculates the length of this curve between this keyframe and a given point.
	\param p2 The point to calculate the length with.
    */
	void CalculateCurveLength(AniPoint p2);

	/*!
	Calculates the interpolation between this keyframe and the next keyframe.
	\param posarray The array of CKeyframeChannel objects.
	\param keyframe They current keyframe number.
	\param fraction The fraction from this keyframe to next.
	\param length The number of entries in the keyframe array.
	\param res Returns the interpolation result.
  	*/	
	virtual void Interpolate(CKeyframeChannel **posarray, int keyframe, float fraction, int length, AniPoint &res);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(CUtilityXMLTag *xmlgen);
	
	AniPoint		m_tangent1;						/*!< The first tangent. */
	AniPoint		m_tangent2;						/*!< The second tangent. */
	float		m_factor1;					    /*!< For internal use only.*/
	float		m_factor2;						/*!< For internal use only.*/
	AniPoint*		control_polygon;				/*!< For internal use only.*/
	float		ItemType;						/*!< For internal use only.*/
	float		c;								/*!< For internal use only.*/
	float		b;								/*!< For internal use only.*/				
	float		m_curvelength;					/*!< For internal use only.*/

};

//! The CKeyframeChannelFollowPath class is used for interpolators that change orientation in relation to position
/*!
	By specifying a set of positional control points, this interpolator can adjust the orientation of the target 
	according to the path curvature like a car following the road.
*/
class SA_API CKeyframeChannelFollowPath : public CKeyframeChannelCurve
{

public:
 
	/*!
	Constructs a CKeyframeChannelFollowPath object.
 	*/
	CKeyframeChannelFollowPath() 
	{
		m_channeltype = ANIFollowPath;
		m_bLinear = false;
	}
	virtual ~CKeyframeChannelFollowPath() {};

 	/*!
	This method makes an exact copy of the current CKeyframeChannelFollowPath and returns a pointer to the newly
	created CKeyframeChannelFollowPath object.
	\return A pointer to newly created CKeyframeChannelFollowPath object.
 	*/
	virtual CKeyframe *Clone () 
	{
		CKeyframeChannelFollowPath *p= new CKeyframeChannelFollowPath;
		*p = *this;
		return p;
	}

	/*!
	Calculates the interpolation between this keyframe and the next keyframe.
	\param posarray The array of CKeyframeChannel objects.
	\param keyframe They current keyframe number.
	\param fraction The fraction from this keyframe to next.
	\param length The number of entries in the keyframe array.
	\param res Returns the interpolation result.
  	*/	
	virtual void Interpolate(CKeyframeChannel **posarray, int keyframe, float fraction, int length, AniPoint &res);
	virtual void Serialize(CUtilityXMLTag *xmlgen);

//	virtual void Serialize(CUtilityXMLTag *xmlgen);
	float tmatrix[16];				/*!< For internal use only.*/
 	float tmatrix2[16];				/*!< For internal use only.*/
	bool m_bLinear;
	bool m_bNoRotate;
};


 

//! The CKeyframeRotation class is the base class for all rotation related keyframes.
class SA_API CKeyframeRotation : public CKeyframe
{
public:
 
	/*!
	Constructs a CKeyframeRotation object.
 	*/
	CKeyframeRotation()
	{
		m_type = ANIRotation;
		m_bLinear = false;
	}
	virtual ~CKeyframeRotation() {};

 	/*!
	This method makes an exact copy of the current CKeyframeRotation and returns a pointer to the newly
	created CKeyframeRotation object.
	\return A pointer to the newly created CKeyframeRotation object.
 	*/
	virtual CKeyframe *Clone () 
	{
		CKeyframeRotation *p= new CKeyframeRotation;
		*p = *this;
		return p;
	} 

	ANIRotationType m_rotationtype;			/*!< The type of rotation.*/
	bool m_bLinear;								/*!< The value is true if the rotation is linear or false if the rotation is not non-linear.*/

};

////! The CKeyframeQuatSlerp class is used for linear quaternion based rotation.
class SA_API CKeyframeQuatSlerp : public CKeyframeRotation
{
public:

	/*!
	Constructs a CKeyframeQuatSlerp object.
 	*/
 	CKeyframeQuatSlerp(float x = 0, float y = 0, float z = 0, float w = 0)
	{
		m_rotationtype = ANIQuatSlerpRotation;
		m_quat.Set(x,y,z,w);
	}

	virtual ~CKeyframeQuatSlerp() {};
 
 	/*!
	This method makes an exact copy of the current CKeyframeQuatSlerp and returns a pointer to the newly
	created CKeyframeQuatSlerp object.
	\return A pointer to the newly created CKeyframeQuatSlerp object.
 	*/
	virtual CKeyframe *Clone () 
	{
		CKeyframeQuatSlerp *p= new CKeyframeQuatSlerp;
		*p = *this;
		return p;
	} 	
	
	AniQuat m_quat;			/*!< The quaternion.*/
};
//
//
//
//
////! The CKeyframeQuatSquad class is used for non-linear quaternion based rotation
class SA_API CKeyframeQuatSquad : public CKeyframeRotation
{
public:

 
	/*!
	Constructs a CKeyframeQuatSquad object.
 	*/
 	CKeyframeQuatSquad(float x = 0, float y = 0, float z = 0, float w = 0)
	{
		m_rotationtype = ANIQuatSquadRotation;
		m_quat.Set(x,y,z,w);
		m_ExtraSpins = 0;

	}
	virtual ~CKeyframeQuatSquad() {};

 	/*!
	This method makes an exact copy of the current CKeyframeQuatSquad and returns a pointer to the newly
	created CKeyframeQuatSquad object.
	\return A pointer to the newly created CKeyframeQuatSquad object.
 	*/
	virtual CKeyframe *Clone () 
	{
		CKeyframeQuatSquad *p= new CKeyframeQuatSquad;
		*p = *this;
		return p;
	} 

	/*!
	Writes XML data to a buffer.
  	*/	
	virtual void Serialize(CUtilityXMLTag *xmlgen);

	AniQuat m_quat;			/*!< The quaternion.*/
	int m_ExtraSpins;	

};


//! The CKeyframeString class is used for any string related keyframes
/*!
	String keyframes can contain a segment path in the case of segment switch interpolation or some other
	text based identifier.
*/
class  SA_API CKeyframeString : public CKeyframe
{
public:
	
	/*!
	Constructs a CKeyframeString object.
 	*/
  	CKeyframeString();
	virtual ~CKeyframeString() {};

 	/*!
	This method makes an exact copy of the current CKeyframeString and returns a pointer to the newly
	created CKeyframeString object.
	\return A pointer to the newly created CKeyframeString object.
 	*/
 	virtual CKeyframe *Clone () 
	{
		CKeyframeString *p= new CKeyframeString;
		*p = *this;
		return p;
	}

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(CUtilityXMLTag *xmlgen);

	
	/*!
	Sets a new target string.
	\param target The new target for the interpolation.
  	*/
	void SetTarget(const char *target);

	/*! \return The target string.
   	*/
 	char * GetTarget() { return m_target; }

	char m_target[SA_BUFFER_SIZE_BIG];			/*!< Keyframe Target*/
	bool m_bDiscrete;

};


//! The CKeyframeString class is used for any string related keyframes
/*!
	String keyframes can contain a segment path in the case of segment switch interpolation or some other
	text based identifier.
*/
class SA_API CKeyframe3String : public CKeyframe
{
public:
	
	/*!
	Constructs a CKeyframeString object.
 	*/
  	CKeyframe3String();
	virtual ~CKeyframe3String() {};

 	/*!
	This method makes an exact copy of the current CKeyframeString and returns a pointer to the newly
	created CKeyframeString object.
	\return A pointer to the newly created CKeyframeString object.
 	*/
 	virtual CKeyframe *Clone () 
	{
		CKeyframe3String *p= new CKeyframe3String;
		*p = *this;
		return p;
	}

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(CUtilityXMLTag *xmlgen);

	
	/*!
	Sets a new target string.
	\param target The new target for the interpolation.
  	*/
	void SetTarget(const char *t1, const char *t2, const char *t3);

	/*! \return The target string.
   	*/
 	char * GetTarget1() { return m_t1; }
 	char * GetTarget2() { return m_t2; }
 	char * GetTarget3() { return m_t3; }

	char m_t1[255];			/*!< Keyframe Target*/
	char m_t2[255];			/*!< Keyframe Target*/
	char m_t3[255];			/*!< Keyframe Target*/

};

//! The CSACommonAPI class contains a set of utility functions and data structures used in the behaviour classes.
class SA_API CSACommonAPI
{
public:
	
	/*! 
	Extracts Quaternion from string
	\param tp Pointer to text parser object
	\param quat Returns extracted quaternion
 	*/
 	static void	GetQuatPoint(CUtilityTextParser *tp, AniQuat &quat);
	static void GetMatrix(CUtilityTextParser *tp, float *matrix);


	/*! 
	Extracts Quaternion from string
	\param text Text to extract quaternion from
	\param quat Returns extracted quaternion
 	*/
	static void GetQuatPoint(const char *text, AniQuat &quat);

	static void GetMatrix(const char *text, float *matrix);

	//计算相对单位矩阵位置沿旋转点旋转quat后的位置
	static AniPoint UniTanslationToMtxTanslation(AniPoint pntPivot, AniQuat quat, AniPoint pntPos);
	//计算位置沿旋转点旋转至单位矩阵后的位置
	static AniPoint MtxTanslationToUniTanslation(AniPoint pntPivot, AniQuat quat, AniPoint pntPos);

	static void MultDirMatrix(const float* MatrixA,AniPoint& srcDir,AniPoint& dstDir);

	static CSAnimation* AddAnimation(NS_SimulationAnimation::CSBehaviorAction  * pBehaviorAction, const char *name, const char *targetpath, AniPoint* pivot, CSAnimation *pParentAnimation);
	static void AddPosKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint& pos, bool linear);
	static void AddRotKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, AniQuat& quat, bool linear);
	static void AddScaleKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint& scale);
	static void AddVisibleKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, char* strVisible);
	static void AddColorKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint& color,bool bDiscrete );
	static void AddSoundKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, char* strSound);
	static void AddImageKeyframe(NS_SimulationAnimation::CSBehaviorAction  *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint& pos, AniPoint& scale, char* strVisible, bool linear);	

	static void AddPosRotKeyframe(NS_SimulationAnimation::CSBehaviorAction *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint pivot, AniPoint pos, bool linear, AniQuat quat, bool linear2,bool bFollowPath);
	static void AddClipKeyframe(NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction, CSAnimation* pAnimation, int tick, AniPoint pos, AniPoint normal, char* strVisible, bool linear);
	static void AddCameraKeyframe(NS_SimulationAnimation::CSBehaviorAction *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint pivot, AniPoint pos, AniQuat quat, AniPoint scale, bool linear);
	static void AddZoomKeyframe(NS_SimulationAnimation::CSBehaviorAction *pBehaviorAction, CSAnimation *pAnimation, int tick, AniPoint pos, AniQuat quat, AniPoint scale, bool linear, const char* strVisible,bool bRelaceVisible);
	//添加工具关键帧
	static void AddToolKeyframe(NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction, CSAnimation* pAnimation, int tick, char* strPlcPath, char* toolPath, char* parentPath);

	//查找动画
	static CSAnimation* FindAmimation(const char *target, const char *interpolatortype,NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction);
	//创建动画文件夹
	static CSAnimation* CreateAmimationFolder(CSAnimation* pParentFolder,const char* strFolderName,NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction);
	//获取所有动画文件夹
	static void GetAllAmimationFolder(vlist_s* pAmimationFolderList,NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction);
	//获取指定分组下的所有子分组
	static void GetChildAmimationFolder(CSAnimation* pParentAnimation,vlist_s* pAmimationFolderList,NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction);
	//获取指定动画所属的顶级动画分组
	static CSAnimation* GetTopAmimationFolder(CSAnimation* pChildAnimation);
	//根据名称获取动画分组
	static CSAnimation* GetAmimationFolderByName(const char* strName,NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction);
	//根据对象路径获取该对象类型的动画分组
	static CSAnimation* GetTargetObjectTypeFolder(const char* strPlcID,NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction);
	//获取指定路径对象的主动画，如果不存在，创建该动画
	static CSAnimation* GetTargetMainAmimation(const char* strPlcID, const char* strObjName,NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction,CSAnimation* pFolderAnimation = NULL);
	//获取所有的子动画（一直到最后一层）
	static void GetTailChildAnimationList(CSAnimation* pAnimation, vlist_s* pChildList, bool bIncludeNoInterpolator = false);
	//判断是否是对象的主动画
	static bool IsTargetMainAmimation(CSAnimation* pAnimation,NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction);
	static bool IsAmimationFolder(CSAnimation* pAnimation,NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction);
	static bool IsTargetObjectTypeFolder(CSAnimation* pAnimation);

	//获取指定类型的动画
	static void GetAnimationByTargetType(NS_SimulationAnimation::CSBehaviorAction* pBehaviorAction, const char* strTargetType, vlist_s* pAnimationList);
	static bool IsUseTargetObjectTypeFolder(){ return m_bUseTargetObjectTypeFolder;};
	static void SetUseTargetObjectTypeFolder(bool bUseTargetObjectTypeFolder){ m_bUseTargetObjectTypeFolder = bUseTargetObjectTypeFolder;};
private:		

	static bool m_bUseTargetObjectTypeFolder;
};

/************************************************************************/
/*定义剖面关键帧类 added by yhp 2014-09-24 Start                            */
/************************************************************************/
class  SA_API CKeyframeChannelClip : public CKeyframeChannel
{
public:

	/*!
	Constructs a CKeyframeChannelClip Object.
 	*/
	CKeyframeChannelClip():CKeyframeChannel()
	{

	}
	CKeyframeChannelClip(AniPoint clipPos, AniPoint clipNormal, bool bIsVisible) : CKeyframeChannel()
	{
		m_channeltype = ANILinear;
		//m_cp代表剖面位置
		m_cp = clipPos;
		m_clipNormal = clipNormal;
		m_bVisible = bIsVisible;
	}
 	virtual ~CKeyframeChannelClip() {};

 	/*!
	This method makes an exact copy of the current CKeyframeChannelCurve and returns a pointer to the newly
	created CKeyframeChannelCurve object.
	\return A pointer to the newly created CKeyframeChannelCurve object.
 	*/
	virtual CKeyframe *Clone () 
	{
		CKeyframeChannelClip *p= new CKeyframeChannelClip;
		*p = *this;
		return p;
	}

 	/*!
	This method is a helper function for CalculateHermiteTangents().  Call this method
	to precalculate curve factors before calling CalculateHermiteTangents().
	\param timeline Timeline array necessary to calculate correct curve factors
	\param keyframe Current Keyframe
  	*/
	void CalculateCurveFactor(CSTimeline *timeline, int keyframe);

 	/*!
	Use this method to precalculate tangents for the hermite spline.  This method can be usef as a helper function
	for interpolation.
	\param p0 The first point for tangent calculation.
	\param p2 The third point for tangent calculation.
	\param p3 The fourth Point for tangent calculation.
	\param a Constant for hermite spline
   	*/
	void CalculateHermiteTangents(AniPoint p0, AniPoint p2, AniPoint p3, float a = 0.5f); 

 	/*!
	This method uses the given frame and control points to interpolate a Hermite spline. 
	\param fraction Distance between the first and second control point.
	\param p1 The first control point.
	\param p2 The second control point.
	\return The interpolated result.
   	*/
	AniPoint InterpolateHermiteSpline(float fraction, AniPoint p1, AniPoint p2);

//	AniPoint InterpolateBezierSpline(float fraction, AniPoint p2);


 	/*!
	This method calculates the length of this curve between this keyframe and a given point.
	\param p2 The point to calculate the length with.
    */
	void CalculateCurveLength(AniPoint p2);

 	
	/*!
	Calculates the interpolation between this keyframe and the next keyframe.
	\param posarray The array of CKeyframeChannel objects.
	\param keyframe They current keyframe number.
	\param fraction The fraction from this keyframe to next.
	\param length The number of entries in the keyframe array.
	\param res Returns the interpolation result.
  	*/	
	virtual void Interpolate(CKeyframeChannel **posarray, int keyframe, float fraction, int length, AniPoint &res, AniPoint &normal, bool &bIsvisible);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(CUtilityXMLTag *xmlgen);
	
	AniPoint		m_clipNormal;						/*!< The first tangent. */
	bool			m_bVisible;						/*!< The second tangent. */
	float		m_factor1;					    /*!< For internal use only.*/
	float		m_factor2;						/*!< For internal use only.*/
	AniPoint		m_tangent1;						/*!< The first tangent. */
	AniPoint		m_tangent2;						/*!< The second tangent. */
	float		m_curvelength;					/*!< For internal use only.*/
};
/************************************************************************/
/*定义剖面关键帧类 added by yhp 2014-09-24 End								*/
/************************************************************************/
	
//SA_NAMESPACE_END

#ifdef USE_PACK_8
#pragma pack(pop)
#endif

#endif



