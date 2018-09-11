/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef M3D_VIEWPORT_H_
#define M3D_VIEWPORT_H_

#include "m3d/base/Ray.h"
#include "m3d/base/Rect.h"
#include "m3d/base/Vector2.h"
#include "m3d/base/Vector3.h"

namespace M3D
{
class CameraNode;
/** 
 * @brief %Viewport definition either for a render surface or the backbuffer. 
 */
class M3D_API Viewport
{
public:
	/**
	 * @brief Construct with defaults.
	 */
	Viewport();
	/**
	 * @brief Construct with a specified rectangle.
	 */
	Viewport(CameraNode* camera, const IntRect& rect);
	/**
	 * @brief Destruct.
	 */
	~Viewport();

	/**
	 * @brief Set camera.
	 */
	void SetCamera(CameraNode* camera);
	/**
	 * @brief Set rectangle.
	 */
	void SetRect(const IntRect& rect);

	/**
	 * @brief Return camera.
	 */
	CameraNode* GetCamera() const;

	/**
	 * @brief Return rectangle.
	 */
	const IntRect& GetRect() const
	{
		return m_rect;
	}

	/**
	 * @brief Return ray corresponding to normalized screen coordinates.
	 */
	Ray GetScreenRay(int x, int y) const;
	// Convert a world space point to normalized screen coordinates.
	IntVector2 WorldToScreenPoint(const Vector3& worldPos) const;
	// Convert screen coordinates and depth to a world space point.
	Vector3 ScreenToWorldPoint(int x, int y, float depth) const;

	Vector3 ScreenToWorldPoint(const Vector2& scrPnt, float depth) const;
	M3D_ADDRESSTYPE Address()
	{
		//获取地址的真实方式
		return TAddress(*this);
	}

	void FillByAddress(M3D_ADDRESSTYPE memoryAddress)
	{
		*this = *TFromAddress<Viewport>(memoryAddress);
	}

private:
	/**
	 * @brief Camera pointer.
	 */
	CameraNode* m_camera;
	/**
	 * @brief Viewport rectangle.
	 */
	IntRect m_rect;

};

}

#endif //M3D_VIEWPORT_H_
