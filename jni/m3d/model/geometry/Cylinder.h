/**@file
 *@brief	Cylinder类头文件
 *@author	xuli
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *Handler类头文件
 *
 */

#ifndef  M3D_CYLINDER_H
#define M3D_CYLINDER_H
#include "m3d/base/Object.h"
#include "m3d/M3DMacros.h"
#include "m3d/base/Quaternion.h"
#include "m3d/base/Vector3.h"

namespace M3D
{
/**@class Cylinder
 * @brief Cylinder类
 *
 * 提供Cylinder类的相关方法
 */
	class M3D_API Cylinder:public Object
	{
	public:

		Cylinder() :Object(),
			_center(0.0f, 0.0f, 0.0f),
			_radius(1.0f),
			_height(1.0f) {}

		Cylinder(const Vector3& center, float radius, float height) :Object(),
			_center(center),
			_radius(radius),
			_height(height) {}

		~Cylinder()
		{

		}

		inline bool Valid() const { return _radius >= 0.0f; }

		inline void Set(const Vector3& center, float radius, float height)
		{
			_center = center;
			_radius = radius;
			_height = height;
		}

		inline void SetCenter(const Vector3& center) { _center = center; }
		inline const Vector3& GetCenter() const { return _center; }

		inline void SetRadius(float radius) { _radius = radius; }
		inline float GetRadius() const { return _radius; }

		inline void SetHeight(float height) { _height = height; }
		inline float GetHeight() const { return _height; }

		inline void SetRotation(const Quaternion& quat) { _rotation = quat; }
		inline const Quaternion& GetRotation() const { return _rotation; }
		inline Matrix3 ComputeRotationMatrix() const { return _rotation.RotationMatrix(); }
		bool ZeroRotation() const { return _rotation.IsZero(); }

	protected:
 
		Vector3    _center;
		float   _radius;
		float   _height;
		Quaternion    _rotation;
	};

}
#endif /*CYLINDER_H*/
