// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		SAGlobals.h
*
*	@brief	定义动画全局数据
*			
*
*	@par	历史:
*
****************************************************************************/

/*! \file SAGlobals.h*/

#ifndef _SAGLOBALS_H
#define _SAGLOBALS_H

#ifdef USE_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

/*! \enum CFileInputResult
* Indicates the result of an attempt to read a file
*/
enum CFileInputResult
{
	InputOK					= 1,    //!< 成功
	InputVersionMismatch	= 2,    //!< 版本不匹配
	InputFail				= 3,    //!< 失败
	InputNotHandled			= 4,    //!< 文件句柄不正确
	InputBadFileName		= 5,    //!< 文件名错误
	InputBadOptions			= 6     //!< 属性错误
};


/*! \enum CFileOutputResult
* Indicates the result of an attempt to write a file
*/
enum CFileOutputResult
{
	OutputOK				= 1,    //!< 成功
	OutputVersionMismatch	= 2,    //!< 版本不匹配
	OutputFail				= 3,    //!< 失败
	OutputNotHandled		= 4,    //!< 文件句柄不正确
	OutputBadFileName		= 5,    //!< 文件名错误
	OutputBadOptions		= 6     //!< 属性错误
};

/*! \def LAST_FileOutputResult
    \brief Specifies last file output type
*/
#define LAST_FileOutputResult OutputBadOptions	   

/*! \enum SACameraType Camera Type of target object */
enum SACameraType
{
	NoCamera,		 //!< There is no camera animation.
	CameraTarget,	 //!< Animation is target driven.  The position location is derived from the target location.
	CameraPosition,	 //!< Animation is position driven.  The target location is derived from the  position location.
	CameraTargetFree,	 //!< The camera target is independently defined but works in conjunction with CameraPositionFree.
	CameraPositionFree,	 //!< The camera position is independently defined but works in conjunction with CameraTargetFree.
	CameraPositionTarget	 //!< Animation keyframe defines the camera target and position.
};

//! The AniPoint class is the data type of a three-dimensional point.
/*! 
  The AniPoint class encapsulates the x, y, and z coordinates of a 3-dimensional point.
*/
class SA_API AniPoint 
{ 
public:
	float		x;	//!< The x-coordinate of a 3-dimensional point.
	float		y;	//!< The y-coordinate of a 3-dimensional point.
	float		z;	//!< The z-coordinate of a 3-dimensional point.
	
	/*! The default constructor. */
	AniPoint() : x(0), y(0), z(0) {};

	/*! This constructor accepts floats for the x, y and z coordinates.*/
	AniPoint(float X, float Y, float Z=0.0f) : x(X), y(Y), z(Z) {};
	
	/*! This constructor accepts a pointer to an AniPoint object. */
	AniPoint(AniPoint const * p) : x(p->x), y(p->y), z(p->z) {};
	
	/*! This constructor accepts an AniPoint reference. */
	AniPoint(AniPoint const & p) : x(p.x), y(p.y), z(p.z) {};
	
	/*!  This method packs an AniPoint object with coordinate values.  */
	void Set(float X, float Y, float Z=0.0f) { x=X; y=Y; z=Z; };
	
	/*!  This method packs an AniPoint object with coordinate values from a given AniPoint.  */
	void Set(AniPoint *p) { x=p->x; y=p->y; z=p->z; };
	
	/*!  This method packs an AniPoint object with coordinate values from a given AniPoint.  */
	void Set(AniPoint const *p) { x=p->x; y=p->y; z=p->z; };
	
	/*!  This method packs an AniPoint object with coordinate values from a given AniPoint that is passed by reference.  */
	void Set(AniPoint &p) { x=p.x; y=p.y; z=p.z; };
	
	/*!  This method packs an AniPoint object with coordinate values from a given AniPoint that is passed by reference.  */
	void Set(const AniPoint &p) { x=p.x; y=p.y; z=p.z; };

	/*!  This method increases/decreases the coordinate values of an existing AniPoint object.  
	\param X The amount in which you want to increment or decrement the x coordinate.
	\param Y The amount in which you want to increment or decrement the y coordinate.
	\param Z The amount in which you want to increment or decrement the z coordinate.
	*/
	void Add(float X, float Y, float Z=0.0){ x+=X; y+=Y; z+=Z; };

	AniPoint const operator -(const AniPoint &p2) const
	{
 		return AniPoint(x-p2.x, y-p2.y, z-p2.z);
	}

	AniPoint const operator +(const AniPoint &p2) const
	{
 		return AniPoint(x+p2.x, y+p2.y, z+p2.z);
	}

	AniPoint const operator *(float const rhs) const
	{
 		return AniPoint(x*rhs, y*rhs, z*rhs);
	}

	AniPoint const operator /(float const rhs) const
	{
 		return AniPoint(x/rhs, y/rhs, z/rhs);
	}

	AniPoint const & operator += (AniPoint const & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	
	AniPoint const & operator -= (AniPoint const & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	AniPoint const & operator *= (float const rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	AniPoint const & operator /= (float const rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	bool operator ==(AniPoint const & rhs) const
	{
		return Equal(&rhs);
	}
	
	bool operator !=(AniPoint const & rhs) const
	{
		return !Equal(&rhs);
	}


/*!\def POINT_EPSILON */
#define POINT_EPSILON (1e-5f)  //!<  POINT_EPSILON


	/*! This method checks if the xyz components of the point are equal.  */
	bool Equal(AniPoint const *p, float epsi = POINT_EPSILON) const { 
		return (bool)(
			(x - p->x < epsi) && (x - p->x > -epsi) &&
			(y - p->y < epsi) && (y - p->y > -epsi) &&
			(z - p->z < epsi) && (z - p->z > -epsi));
	};

	/*!  This method checks if the xyz components of the point are equal.  */
	bool Equal(AniPoint const &p, float epsi = POINT_EPSILON) const { 
		return Equal(&p, epsi);
	};
};
 
//! The SAVector class is the data type of a three-dimensional vector.
/*! 
  The SAVector class encapsulates the x, y, and z coordinates of a 3-dimensional vector.  
*/
typedef class AniPoint SAVector;

#ifdef USE_PACK_8
#pragma pack(pop)
#endif

#endif







