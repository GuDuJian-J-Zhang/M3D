// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		SInterpolator.h
*
*	@brief	插值器定义文件，包含插值器的基类、位置插值器、旋转插值器、旋转点插值器、缩放插值器、颜色插值器等。
*
*	@par	历史:
*
****************************************************************************/


#ifndef _SINTERPOLATOR_H
#define _SINTERPOLATOR_H

#ifdef USE_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "../SimulationCommon/SATools.h"
#include "KeyFrame.h"
#include "../SimulationCommon/varray.h"
#include "SADefine.h"
#include "AnimationSound.h"

//SA_NAMESPACE_BEGIN

class CUtilityXMLGenerator;
class CSAnimation;
  
#ifdef WINDOWS_SYSTEM
template class  VArray< CKeyframe *>;
template class  VArray< void *>;
#endif

//差值器类型宏定义
#define INTERPOLATOR_PIVOT				"Pivot"
#define INTERPOLATOR_POS				"Pos"
#define INTERPOLATOR_QUATROT			"QuatRot"
#define INTERPOLATOR_SCALE				"Scale"
#define INTERPOLATOR_VISIBLE			"Visible"
#define INTERPOLATOR_COLOR				"Color"
#define INTERPOLATOR_ATTSWITCH			"AttSwitch"
#define INTERPOLATOR_SOUND				"Sound"		//定义声音插值器类型 Add By YL 2014.11.6
#define INTERPOLATOR_IMAGE				"Image"		//定义图片插值器类型 Add By YL 2015.1.19
#define INTERPOLATOR_NORMAL				"Normal"	
#define INTERPOLATOR_INSTANCECREATE		"InstanceCreate"

/*! \enum TrailInterpolatorType
	Various Modes for Trail Interpolator
*/
enum TrailInterpolatorType
{
	Forward,		  //!< line is drawn from beginning of animation to current position
	Backward,			//!< line is drawn from end of animation to current position
	Full,				//!< line is drawn from beginning of animation to end of animation
};


#define REPLACE_VARRAY(c, pos) {	if (m_pArray.Count() > pos)     \
									{								\
									CKeyframe *temp = m_pArray[pos];\
									delete temp;\
									}								\
									m_pArray.ReplaceAt(c, pos);    }								

#define REMOVE_VARRAY(pos)			{  CKeyframe *temp = m_pArray[pos];\
										delete temp;\
										m_pArray.RemoveAt(pos);    }								

#define CURVE_ARRAY(pos)		((CKeyframeChannelCurve *)GetAt(pos))
							


//! The CSInterpolator class is the abstract base class for all interpolator types.
/*!	
	CSInterpolator keeps an array of keyframes as well as methods to access members of 
	that array. The various interpolator types then implement the required
	functions that actually perform the interpolation. 
*/ 
class  SA_API CSInterpolator
{
public:
	/*!
	Constructs an CSInterpolator object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of this interpolator.
 	*/
	CSInterpolator(CSAnimation *animation = 0, const char *name = 0);
	virtual ~CSInterpolator();

	/*! \return Type of this interpolator. */
 	virtual const char * GetType() = 0;

	/*!
	In special cases, if you want to reference another interpolator without changing it, use this method to indicate
	the interpolator that you want this interpolator to refer to.
	\param interpolator Interpolator to reference.
	*/	
	void SetInstancedInterpolator(CSInterpolator *interpolator);

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param animationinst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
  	*/
	virtual CSInterpolator *CreateInstance(CSAnimation *animationinst) = 0;

	/*!
	This method replaces an array item at a given position.
	\param c The replacement keyframe.
	\param pos The array position for the replacement keyframe. 
  	*/
 	void  Replace(CKeyframe * c, int pos) { REPLACE_VARRAY(c, pos); }

	/*!
	Inserts a keyframe at the given position.
	\param piece The keyframe to insert
	\param pos The array position to insert item after.
  	*/
	void  Insert(CKeyframe * piece, int pos = 0) { m_pArray.InsertAt(piece, pos); }

	/*!
	This method appends the given keyframe to the end of array.
	\param piece The keyframe you want to append.
   	*/
	void  Append(CKeyframe * piece) { m_pArray.InsertAt(piece, m_pArray.Count()); }

	/*!
	This method removes a keyframe at the given position.
 	\param pos The array position indicating where to remove the keyframe.
  	*/
 	void  Remove(int pos) { REMOVE_VARRAY(pos); }

	/*! \return The name of the interpolator. */
	const char * GetName() { return m_Name; }

	/*!
	Returns the keyframe that exists at the given position.
 	\param pos The array position to retrieve.
	\return The keyframe at the given position.
  	*/
	virtual CKeyframe *GetAt(int pos) { return m_pArray[pos]; }

	/*! \return The keyframe array.  Note this is for read-only purposes.  If you want to modify the keyframe
	array, use the methods provided in this class to do so.*/
	CKeyframe **GetArray() { return &m_pArray[0]; }

	/*! \return The number of entries in the keyframe array. */
	virtual int   GetArrayLength() { return m_pArray.Count(); }
   
	/*!
	Duplicates a keyframe and inserts it at the position indicated the replace and next parameters.
	\param pos The position of the keyframe to duplicate.
	\param replace Pass true if you want to replace the item at pos with the duplicate and false to insert the duplicate 
	item after the pos item.
	\param next Pass true duplicate the item at given pos and false to duplicate the item before the given pos.
   	*/	
	void Duplicate(int pos, bool replace, bool next);
 	
	/*!
	This method copies a given keyframe and inserts the copy at a new location. 
	\param pos The position of the item to copy.
	\param adpos The target position.
	\param replace Pass true to replace item at the target position and false to insert after the target position.
   	*/	
	void Copy(int pos, int adpos, bool replace);
 
	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(CUtilityXMLGenerator *xmlgen) { }

	/*!
	Performs interpolation on the animation target given a keyframe and a fraction.
	\param keyframe Reference keyframe for this interpolation.
	\param fraction Fraction value from the specified keyframe to the next keyframe.  This value is from 0 to 1.
  	*/
	virtual void Interpolate(int keyframe, float fraction) { };
	virtual void Evaluate(int keyframe, float fraction, bool &hasPos, AniPoint &pos, bool &hasQuat, AniQuat &quat, bool &hasScale, AniPoint &scale) { };
 
	/*! \return A pointer to the animation associated with this interpolator. */
 	CSAnimation * GetAnimation() { return m_pAnimation; }

	/*!
	Associates an animation with this interpolater.
   	*/
	void  SetAnimation(CSAnimation *animation) { m_pAnimation = animation; }

	/*!
	Calculates the translation components from modelling matrix of the target segment.
 	\param translation Returns the x/y/z translation.
	*/
	void GetTranslationFromMatrix(AniPoint &translation);
	/*!
	Calculates the rotation components from modelling matrix of the target segment.
 	\param rotation Returns the x/y/z rotation.
	*/
	void GetRotationFromMatrix(AniQuat &rotation);

	/*!
	Resets the interpolator instance. 
	*/	
	virtual void Reset() { };
	//克隆
	virtual CSInterpolator* Clone();

protected:	

	/*!
	Sets the target as the one associated with this interpolator's animation instance.
 	*/	
	void SetTarget();
		
	char						m_Name[SA_BUFFER_SIZE_SMALL];		/*!< The name of the interpolator object.*/	 
	VArray< CKeyframe *>		m_pArray;							/*!< The keyframe array.*/	
	CSAnimation*				m_pAnimation;						/*!< A pointer to the animation that this interpolator belongs to.*/	
	CSInterpolator*			m_pInterpolatorInstance;			/*!< A pointer to instanced interpolator.*/	
	long						m_pTarget;  						/*!< The key of interpolator target.*/	
};


//! The CSInterpolatorPosition class controls positional interpolation
/*!	
	CSInterpolatorPosition allows for either linear or spline based movement. This class can be used for camera interpolation as well other targets.
*/ 

class SA_API CSInterpolatorPosition : public CSInterpolator
{
public:
	/*!
	Constructs an CSInterpolatorPosition object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
 	*/
	CSInterpolatorPosition(CSAnimation *animation = 0, const char *name = 0);


	/*! \return Type of the interpolator which is 'Pos'. */
 	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
  	*/
	virtual CSInterpolator * CreateInstance(CSAnimation *ainst);

	/*!
	This method creates a new linear positional keyframe with the passed <i>pos</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param pos The position value for the keyframe to be inserted.
	\param l The position in the array to insert the item after.
  	*/
 	void  InsertLinear(AniPoint pos, int l = 0 ) { CKeyframeChannelLinear * linear = new CKeyframeChannelLinear; 
				linear->m_cp = pos; m_pArray.InsertAt(linear, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new linear positional keyframe that
	was created with the passed <i>pos</i> value.
	\param pos The position value for the new replacement keyframe.
	\param l The array position to replace.
  	*/
	void  ReplaceLinear(AniPoint pos, int l) { CKeyframeChannelLinear * c = new CKeyframeChannelLinear; 
				c->m_cp = pos; REPLACE_VARRAY(c, l) }
	/*!
	This method creates a new non-linear positional keyframe with the passed <i>pos</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param pos The position value for the keyframe to be insert.
	\param l The array position to insert item after.
  	*/
 	void  InsertCurve(AniPoint pos, int l = 0) { CKeyframeChannelCurve * linear = new CKeyframeChannelCurve; 
				linear->m_cp = pos; m_pArray.InsertAt(linear, l); }
	/*!
	//Replaces the item at the given position in the keyframe array with a new non-linear positional keyframe that
	//was created with the passed <i>pos</i> value.
	//\param pos The position value for the new replacement keyframe.
	//\param l The array position to replace.
 	*/
	void  ReplaceCurve(AniPoint pos, int l) { CKeyframeChannelCurve * c = new CKeyframeChannelCurve; 
			c->m_cp = pos; REPLACE_VARRAY(c, l) }

	/*!
	This method creates a new discrete positional keyframe with the passed <i>pos</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param pos The position value for the new keyframe to be inserted.
	\param l The array position to insert item after.
  	*/
 	void  InsertDiscrete(AniPoint pos, int l = 0) { CKeyframeChannelDiscrete * d = new CKeyframeChannelDiscrete; 
				d->m_cp = pos; m_pArray.InsertAt(d, l); }

	void  InsertFollowPath(AniPoint pos, int l = 0, bool bLiner = false) { CKeyframeChannelFollowPath * followPath = new CKeyframeChannelFollowPath; 
	followPath->m_cp = pos; followPath->m_bLinear = bLiner; m_pArray.InsertAt(followPath, l); }

	void  ReplaceFollowPath(AniPoint pos, int l, bool bLiner = false) { CKeyframeChannelFollowPath * followPath = new CKeyframeChannelFollowPath; 
	followPath->m_cp = pos; followPath->m_bLinear = bLiner; REPLACE_VARRAY(followPath, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
  	*/
	virtual void Serialize(CUtilityXMLGenerator *xmlgen);
	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static	void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);
	static	void *XMLCallback_V1(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on this interpolator's animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
  	*/
	virtual void Interpolate(int keyframe, float fraction);

	virtual void Evaluate(int keyframe, float fraction, bool &hasPos, AniPoint &pos, bool &hasQuat, AniQuat &quat, bool &hasScale, AniPoint &scale);

	/*!
	Resets the interpolator instance. 
	*/	
  	virtual void Reset();
 	
  	/*!
	Calculates the interpolated positional value given a keyframe and fraction to the next keyframe.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fractional value from the specified keyframe to the next keyframe.  This value is from 0 to 1.
	\param res The result of the interpolation.
  	*/
	virtual void CalculatePos(int keyframe, float fraction, AniPoint &res);
	void CalculatePosCamera(int keyframe, float fraction,  AniPoint &res);
	//克隆
	virtual CSInterpolator* Clone();

protected:
	/*!
	Calculates the interpolated camera orientation and position for camera types that are position driven.
	\param pos New Camera Position Reference  
  	*/
	virtual void InterpolateCamera(AniPoint &pos, bool simulate = false);

	/*!
	Calculates the interpolated camera orientation for camera types that are target driven.
	\param pos New Camera Position Reference  
  	*/
	virtual void InterpolateCamera2(AniPoint &pos, bool simulate = false);

	/*!
	This is a helper method that calculates the tangents for all the entries in the keyframe array. You can call
	this method before doing an interpolation.
  	*/
  	virtual void CalculateAllTangents(); 
};

//! The CSInterpolatorColor class controls color attribute changes.
	
class SA_API CSInterpolatorColor : public CSInterpolator
{
public:
	/*!
	Constructs an CSInterpolatorColor object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
 	*/

	CSInterpolatorColor(CSAnimation *animation = 0, const char *name = 0);
	/*! \return The type of the interpolator which is 'Color'.*/
 	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
  	*/
	virtual CSInterpolator * CreateInstance(CSAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed color value and then inserts it after the given
	position <i>l</i> in the keyframe array.
	\param pos The color value you want to set for the new keyframe.  Pass r,g, and b
	values in the x,y and z parameters respectively in AniPoint.
	\param l array position to insert item after
  	*/
	void  Insert(AniPoint pos, int l = 0) { CKeyframeChannelLinear* color = new CKeyframeChannelLinear; 
				color->m_cp = pos; m_pArray.InsertAt(color, l); }

	void  InsertDiscrete(AniPoint pos, int l = 0) { CKeyframeChannelDiscrete * d = new CKeyframeChannelDiscrete; 
	d->m_cp = pos; m_pArray.InsertAt(d, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed color value.
	\param pos The color value for the replacement keyframe. Pass r,g, and b
	values in the x,y and z parameters respectively in AniPoint.  
	\param l array position to replace
  	*/
 	void  Replace(AniPoint pos, int l) { CKeyframeChannelLinear * c = new CKeyframeChannelLinear; 
				c->m_cp = pos; REPLACE_VARRAY(c, l); }

	void  ReplaceDiscrete(AniPoint pos, int l) { CKeyframeChannelDiscrete * d = new CKeyframeChannelDiscrete; 
	d->m_cp = pos; REPLACE_VARRAY(d, l); }

	/*!
	Writes XML data to a buffer using the passed XML generator.
  	*/
	virtual void Serialize(CUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);
	
	/*!
	Sets the geometry type that color changes applies to like faces, lines, etc...
  	*/
	void SetGeomType (const char *geomtype);

	/*! \return The geometry type that color changes apply to like faces, lines, etc... */
	char * GetGeomType () { return m_GeomType; }

	/*!
	Sets color components attributes like diffuse, specular and gloss that color changes will be applied to.
  	*/
	void SetColorComponent (const char *ColorComponent);

	/*! \return color components attributes. */
	char * GetColorComponent () { return m_ColorComponent; }

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
  	*/
	virtual void Interpolate(int keyframe, float fraction);
	virtual void CalculatePos(int keyframe, float fraction, AniPoint &res);
	//克隆
	virtual CSInterpolator* Clone();

protected:
	char				m_ColorComponent[SA_BUFFER_SIZE_SMALL];  /*!< For internal use only.*/
	char				m_GeomType[SA_BUFFER_SIZE_SMALL];		/*!< For internal use only.*/
 
};



//! The CSInterpolatorScale class controls scaling.  This class can be used for camera interpolation as well other targets.

class SA_API CSInterpolatorScale : public CSInterpolator
{
public:

 	/*!
	Calculates the interpolated positional value given a keyframe and fraction to the next keyframe.
	\param keyframe The reference keyframe for this interpolation/
	\param fraction The fractional value from the specified keyframe to the next keyframe.  This value is from 0 to 1.
	\param res The result of the interpolation.
  	*/
    virtual void CalculatePos(int keyframe, float fraction,  AniPoint &res);


	/*!
	This is a helper method that calculates the tangents for all the entries in the keyframe array. You can call
	this method before doing an interpolation.
  	*/
  	virtual void CalculateAllTangents(); 

	/*!
	Resets this interpolator instance. 
	*/	
	virtual void Reset();

	/*!
	Constructs an CSInterpolatorScale object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
 	*/
 	CSInterpolatorScale(CSAnimation *animation = 0, const char *name = 0);

	/*! \return The type of this interpolator which is 'Scale'. */
 	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
  	*/
	virtual CSInterpolator * CreateInstance(CSAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed scaling factor and then inserts it after the given
	position <i>l</i> in the keyframe array.
	\param pos The scale factors for the new keyframe.
	\param l The array position to insert item after.
  	*/
	void  Insert(AniPoint pos, int l = 0) { CKeyframeChannelLinear * scale = new CKeyframeChannelLinear; 
				scale->m_cp = pos; m_pArray.InsertAt(scale, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed scaling factors.
	\param pos The scale factors for the replacement keyframe.
	\param l The array position to replace.
  	*/
 	void  Replace(AniPoint pos, int l) { CKeyframeChannelLinear * c = new CKeyframeChannelLinear; 
				c->m_cp = pos; REPLACE_VARRAY(c, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
  	*/
 	virtual void Serialize(CUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
  	*/
	virtual void Interpolate(int keyframe, float fraction);
	void Evaluate(int keyframe, float fraction, bool &hasPos, AniPoint &pos, bool &hasQuat, AniQuat &quat, bool &hasScale, AniPoint &scale);

	//克隆
	virtual CSInterpolator* Clone();

protected:
	/*!
	Calculates the interpolated camera orientation and position for camera types that are position driven.
	\param pos New Camera Position Reference  
  	*/
	virtual void InterpolateCamera(AniPoint &Scale, bool simulate = false);
	/*!
	Calculates the interpolated camera orientation for camera types that are target driven.
	\param pos New Camera Position Reference  
  	*/	
	virtual void InterpolateCamera2(AniPoint &Scale, bool simulate = false);

};

 

//! The CSInterpolatorQuatSquad class controls quaternion based rotation. This class can be used for camera interpolation as well other targets.
class SA_API CSInterpolatorQuatSquad : public CSInterpolator
{
public:

	/*!
	Constructs an CSInterpolatorQuatSquad object.
	\param animation Animation that this interpolator belongs to
	\param name Name of Interpolator
 	*/
 	CSInterpolatorQuatSquad(CSAnimation *animation = 0, const char *name = 0);

	/*! \return The type of the interpolator which is 'QuatRot'. */
 	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
  	*/
	virtual CSInterpolator * CreateInstance(CSAnimation *ainst);

	/*!
	This method creates a new quaternion rotation keyframe with the passed <i>q</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param q The quaternion to use for new keyframe.
	\param l The array position to insert item after.
  	*/
	void  Insert(AniQuat q, int l = 0) { CKeyframeQuatSquad * rot = new CKeyframeQuatSquad; 
				rot->m_quat = q; m_pArray.InsertAt(rot, l); }
	
	/*!
	This method creates a new linear quaternion rotation keyframe with the passed <i>q</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param q The quaternion to use for new keyframe.
	\param l The array position to insert item after.
  	*/
	void  InsertLinear(AniQuat q, int l = 0) { CKeyframeQuatSquad * rot = new CKeyframeQuatSquad; 
				rot->m_quat = q; rot->m_bLinear = true; m_pArray.InsertAt(rot, l); }

	/*!
	Replaces the item at the given position in the keyframe array with a new quaternion rotation keyframe that
	was created with the passed <i>q</i> value.
	\param q The quaternion to use for the replace keyframe
	\param l The array position to replace.
  	*/
 	void  Replace(AniQuat q, int l) { CKeyframeQuatSquad * c = new CKeyframeQuatSquad; 
				c->m_quat = q; REPLACE_VARRAY(c, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new linear quaternion rotation keyframe that
	was created with the passed <i>q</i> value.
	\param q The quaternion to use for the replace keyframe
	\param l The array position to replace.
  	*/
 	void  ReplaceLinear(AniQuat q, int l) { CKeyframeQuatSquad * c = new CKeyframeQuatSquad; 
				c->m_quat = q; c->m_bLinear = true; REPLACE_VARRAY(c, l); }

	/*!
	Recalculates the quaternion array to ensure shortest rotation path.
   	*/ 	
	void AdjustQuaternions();

	/*!
	Writes XML data to a buffer using the passed XML generator.
  	*/
	virtual void Serialize(CUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
  	*/
 	virtual void Interpolate(int keyframe, float fraction);
	virtual void Evaluate(int keyframe, float fraction, bool &hasPos, AniPoint &pos, bool &hasQuat, AniQuat &quat, bool &hasScale, AniPoint &scale);

	/*!
	Calculates the quaternion interpolation given a reference keyframe and a fraction.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	\param res Returns the result of the calculation.  	
	*/
	virtual void CalculateQuat(int keyframe, float fraction,  AniQuat &res);
	//克隆
	virtual CSInterpolator* Clone();

protected:
	/*!
	Calculates the interpolated camera orientation and position for camera types that are position driven.
	\param pos New Camera Position Reference  
  	*/
	virtual void InterpolateCamera(AniQuat &quat, bool simulate = false);

	/*!
	Calculates the interpolated camera orientation for camera types that are target driven.
	\param pos New Camera Position Reference  
  	*/	
	virtual void InterpolateCamera2(AniQuat &quat, bool simulate = false);

};


//! The CSInterpolatorVisible class switches visible state

class SA_API CSInterpolatorVisible : public CSInterpolator
{
public:
	/*!
	Constructs an CSInterpolatorAttSwitch object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/
	CSInterpolatorVisible(CSAnimation *animation = 0, const char *name = 0);

	/*! \return The type of the interpolator which is 'AttSwitch'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual CSInterpolator * CreateInstance(CSAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed style segment path <i>t</i>  and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param t The style segment path for the new keyframe.
	\param l The array position to insert item after.
	*/
	void  Insert(const char * t, int l = 0) { CKeyframeString* AttSwitch = new CKeyframeString; 
	AttSwitch->SetTarget(t); m_pArray.InsertAt(AttSwitch, l); }

	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed style segment path <i>spath</i>.
	\param spath The style segment path for the new replacement keyframe.
	\param l The array position to replace.
	*/
	void  Replace(const char *spath, int l) { CKeyframeString * c = new CKeyframeString; 
	c->SetTarget(spath); REPLACE_VARRAY(c, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(CUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);
	//克隆
	virtual CSInterpolator* Clone();
};

//定义声音插值器类 Add By YL 2014.11.6
class SA_API CSInterpolatorSound : public CSInterpolator
{
public:
	/*!
	Constructs an CSInterpolatorAttSwitch object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/

	CAnimationSound * m_pSound;

	CSInterpolatorSound(CSAnimation *animation = 0, const char *name = 0);
	~CSInterpolatorSound(void);

	/*! \return The type of the interpolator which is 'AttSwitch'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual CSInterpolator * CreateInstance(CSAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed style segment path <i>t</i>  and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param t The style segment path for the new keyframe.
	\param l The array position to insert item after.
	*/
	void  Insert(const char * t, int l = 0) { CKeyframeString* AttSwitch = new CKeyframeString; 
	AttSwitch->SetTarget(t); m_pArray.InsertAt(AttSwitch, l); }

	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed style segment path <i>spath</i>.
	\param spath The style segment path for the new replacement keyframe.
	\param l The array position to replace.
	*/
	void  Replace(const char *spath, int l) { CKeyframeString * c = new CKeyframeString; 
	c->SetTarget(spath); REPLACE_VARRAY(c, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(CUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);
	void GetFilePathName(char* cSoundFilePath);
	//克隆
	virtual CSInterpolator* Clone();
};

/************************************************************************/
/*定义剖面差值器类 added by yhp 2014-09-24 Start                            */
/************************************************************************/
#if 1
class SA_API CSInterpolatorClip : public CSInterpolator
{
public:
	/*!
	Constructs an CSInterpolatorPosition object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
 	*/
	CSInterpolatorClip(CSAnimation *animation = 0, const char *name = 0);


	/*! \return Type of the interpolator which is 'Pos'. */
 	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
  	*/
	virtual CSInterpolator * CreateInstance(CSAnimation *ainst);

	/*!
	This method creates a new linear positional keyframe with the passed <i>pos</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param pos The position value for the keyframe to be inserted.
	\param l The position in the array to insert the item after.
  	*/
 	void  InsertLinear(AniPoint pos, AniPoint normal, bool bIsVisible, int l = 0) { 
				CKeyframeChannelClip * clipKFC = new CKeyframeChannelClip; 
				clipKFC->m_cp = pos; 
				clipKFC->m_clipNormal = normal;
				clipKFC->m_bVisible = bIsVisible;
				 m_pArray.InsertAt(clipKFC, l); 
	}
	/*!
	Replaces the item at the given position in the keyframe array with a new linear positional keyframe that
	was created with the passed <i>pos</i> value.
	\param pos The position value for the new replacement keyframe.
	\param l The array position to replace.
  	*/
	void  ReplaceLinear(AniPoint pos, int l, AniPoint normal, bool bIsVisible) { 
				CKeyframeChannelClip * clipKFC = new CKeyframeChannelClip; 
				clipKFC->m_cp = pos; 
				clipKFC->m_clipNormal;
				clipKFC->m_bVisible; 
				REPLACE_VARRAY(clipKFC, l) }
	/*!
	Writes XML data to a buffer using the passed XML generator.
  	*/
	virtual void Serialize(CUtilityXMLGenerator *xmlgen);
	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static	void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on this interpolator's animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
  	*/
	virtual void Interpolate(int keyframe, float fraction);

	virtual void Evaluate(int keyframe, float fraction, bool &hasPos, AniPoint &pos, bool &hasQuat, AniQuat &quat, bool &hasScale, AniPoint &scale);

	/*!
	Resets the interpolator instance. 
	*/	
  	virtual void Reset();
 	
  	/*!
	Calculates the interpolated positional value given a keyframe and fraction to the next keyframe.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fractional value from the specified keyframe to the next keyframe.  This value is from 0 to 1.
	\param res The result of the interpolation.
  	*/
	virtual void CalculatePos(int keyframe, float fraction, AniPoint &res);
	//克隆
	virtual CSInterpolator* Clone();

protected:

	/*!
	This is a helper method that calculates the tangents for all the entries in the keyframe array. You can call
	this method before doing an interpolation.
  	*/
  	virtual void CalculateAllTangents(); 
};
#endif
/************************************************************************/
/*定义剖面差值器类 added by yhp 2014-09-24 End                            */
/************************************************************************/

/************************************************************************/
/*定义剖面差值器类 added by yhp 2014-10-25 Start                            */
/************************************************************************/
class SA_API CSInterpolatorNormal : public CSInterpolator
{
public:
	CSInterpolatorNormal(CSAnimation *animation = 0, const char *name = 0);
	
 	const char * GetType();

	virtual CSInterpolator * CreateInstance(CSAnimation *ainst);

	void  Insert(AniPoint pos, int l = 0) { CKeyframeChannelLinear* Section = new CKeyframeChannelLinear; 
				Section->m_cp = pos; m_pArray.InsertAt(Section, l); }

 	void  Replace(AniPoint pos, int l) { CKeyframeChannelLinear * c = new CKeyframeChannelLinear; 
				c->m_cp = pos; REPLACE_VARRAY(c, l); }

	virtual void Serialize(CUtilityXMLGenerator *xmlgen);

	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);
	
	virtual void Interpolate(int keyframe, float fraction);
	virtual void CalculatePos(int keyframe, float fraction, AniPoint &res);
	//克隆
	virtual CSInterpolator* Clone();
};
/************************************************************************/
/*定义剖面差值器类 added by yhp 2014-10-25 End                            */
/************************************************************************/


/************************************************************************/
/*定义旋转点差值器类，实现可沿多沿旋转功能 added by likun 2014-11-03 Start  */
/************************************************************************/
class SA_API CSInterpolatorPivot : public CSInterpolator
{
public:
	/*!
	Constructs an CSInterpolatorPosition object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/
	CSInterpolatorPivot(CSAnimation *animation = 0, const char *name = 0);


	/*! \return Type of the interpolator which is 'Pivot'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual CSInterpolator * CreateInstance(CSAnimation *ainst);

	/*!
	This method creates a new discrete positional keyframe with the passed <i>pos</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param pos The position value for the new keyframe to be inserted.
	\param l The array position to insert item after.
	*/
	void  Insert(AniPoint pos, int l) { CKeyframeChannelDiscrete * discrete = new CKeyframeChannelDiscrete; 
	discrete->m_cp = pos; m_pArray.InsertAt(discrete, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new linear positional keyframe that
	was created with the passed <i>pos</i> value.
	\param pos The position value for the new replacement keyframe.
	\param l The array position to replace.
	*/
	void  Replace(AniPoint pos, int l) { CKeyframeChannelDiscrete * discrete = new CKeyframeChannelDiscrete; 
	discrete->m_cp = pos; REPLACE_VARRAY(discrete, l) }	

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(CUtilityXMLGenerator *xmlgen);
	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static	void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on this interpolator's animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);

	/*!
	Calculates the interpolated positional value given a keyframe and fraction to the next keyframe.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fractional value from the specified keyframe to the next keyframe.  This value is from 0 to 1.
	\param res The result of the interpolation.
	*/
	virtual void CalculatePos(int keyframe, float fraction, AniPoint &res);
	//克隆
	virtual CSInterpolator* Clone();
};

/************************************************************************/
/*定义旋转点差值器类，实现可沿多沿旋转功能 added by likun 2014-11-03 Start  */
/************************************************************************/
class SA_API CSInterpolatorInstanceCreate : public CSInterpolator
{
public:

	/*!
	Constructs an HBhvInterpolatorInstanceCreate object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
 	*/
 	CSInterpolatorInstanceCreate(CSAnimation *animation = 0, const char *name = 0);
 	~CSInterpolatorInstanceCreate();

	/*! \return The type of the interpolator which is 'InstanceCreate'. */
 	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
  	*/
	virtual CSInterpolator * CreateInstance(CSAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed segment path <i>t</i> and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param t Segment path to insert
	\param l array position to insert item after
  	*/
	void  Insert(char * t, char *t2, char *t3, int l = 0) { CKeyframe3String* InstanceCreate = new CKeyframe3String;
				InstanceCreate->SetTarget(t,t2,t3); m_pArray.InsertAt(InstanceCreate, l); }

	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed segment path <i>t</i>.
	\param spath Segment path to replace with
	\param l array position to replace
  	*/
 	void  Replace(char *t, char *t2, char *t3, int l) { CKeyframe3String * c = new CKeyframe3String;
				c->SetTarget(t,t2,t3); REPLACE_VARRAY(c, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
  	*/
 	virtual void Serialize(CUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(CUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1/
  	*/
	virtual void Interpolate(int keyframe, float fraction);

	/*!
	Resets the interpolator instance by turning off the visibility of all segments in the keyframe list.
	*/
	virtual void Reset();
	static void DeciperString(CSInterpolatorInstanceCreate *itp, char *text, int &counter);	
	//克隆
	virtual CSInterpolator* Clone();
};
//SA_NAMESPACE_END

#ifdef USE_PACK_8
#pragma pack(pop)
#endif

#endif



