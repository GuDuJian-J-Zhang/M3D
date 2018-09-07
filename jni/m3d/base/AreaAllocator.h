/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_AREAALLOCATOR_H_
#define M3D_AREAALLOCATOR_H_

#include "m3d/base/Rect.h"

namespace M3D
{

/**
 * @brief Rectangular area allocator.
 */
class M3D_API AreaAllocator
{
public:
	/**
	 *@brief Default construct with empty size.
	 */
	AreaAllocator();
	/**
	 * @brief Construct with given width and height.
	 * @param width
	 * @param height
	 * @param fastMode
	 */
	AreaAllocator(int width, int height, bool fastMode = true);
	/**
	 * @brief Construct with given width and height, and set the maximum it allows to grow.
	 * @param width
	 * @param height
	 * @param maxWidth
	 * @param maxHeight
	 * @param fastMode
	 */
	AreaAllocator(int width, int height, int maxWidth, int maxHeight,
			bool fastMode = true);

	/**
	 * @brief Reset to given width and height and remove all previous allocations.
	 * @param width
	 * @param height
	 * @param maxWidth
	 * @param maxHeight
	 * @param fastMode
	 */
	void Reset(int width, int height, int maxWidth = 0, int maxHeight = 0,
			bool fastMode = true);
	/**
	 * @brief Try to allocate an area. Return true if successful, with x & y coordinates filled.
	 * @param width
	 * @param height
	 * @param x
	 * @param y
	 * @return
	 */
	bool Allocate(int width, int height, int& x, int& y);
	/**
	 * @brief Return the current width.
	 * @return
	 */
	int GetWidth() const
	{
		return size_.m_x;
	}
	/**
	 * @brief Return the current height.
	 * @return
	 */
	int GetHeight() const
	{
		return size_.m_y;
	}
	/**
	 * @brief Return whether uses fast mode. Fast mode uses a simpler allocation scheme which may waste free space, but is OK for eg. fonts.
	 * @return
	 */
	bool GetFastMode() const
	{
		return fastMode_;
	}

private:
	/**
	 * @brief Remove space from a free rectangle. Return true if the original rectangle should be erased from the free list. Not called in fast mode.
	 * @param original
	 * @param reserve
	 * @return
	 */
	bool SplitRect(IntRect original, const IntRect& reserve);
	/**
	 * @brief Clean up redundant free space. Not called in fast mode.
	 */
	void Cleanup();

	vector<IntRect> freeAreas_; //!< Free rectangles.

	IntVector2 size_; //!< Current size.

	IntVector2 maxSize_; //!< Maximum size it allows to grow. It is zero when it is not allowed to grow.

	bool doubleWidth_; //!< The dimension use for next growth. Used internally.

	bool fastMode_; //!< Fast mode flag.
};

}

#endif //M3D_AREAALLOCATOR_H_
