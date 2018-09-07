
#include "m3d/base/AreaAllocator.h"

namespace M3D
{

AreaAllocator::AreaAllocator()
{
    Reset(0, 0);
}

AreaAllocator::AreaAllocator(int width, int height, bool fastMode)
{
    Reset(width, height, width, height, fastMode);
}

AreaAllocator::AreaAllocator(int width, int height, int maxWidth, int maxHeight, bool fastMode)
{
    Reset(width, height, maxWidth, maxHeight, fastMode);
}

void AreaAllocator::Reset(int width, int height, int maxWidth, int maxHeight, bool fastMode)
{
    doubleWidth_ = true;
    size_ = IntVector2(width, height);
    maxSize_ = IntVector2(maxWidth, maxHeight);
    fastMode_ = fastMode;
    
    freeAreas_.clear();
    IntRect initialArea(0, 0, width, height);
    freeAreas_.push_back(initialArea);
}

bool AreaAllocator::Allocate(int width, int height, int& x, int& y)
{
    if (width < 0)
        width = 0;
    if (height < 0)
        height = 0;
    
    vector<IntRect>::iterator best;
    int bestFreeArea;
    
    for(;;)
    {
        best = freeAreas_.end();
        bestFreeArea = M_MAX_INT;
        for (vector<IntRect>::iterator i = freeAreas_.begin(); i != freeAreas_.end(); ++i)
        {
            int freeWidth = i->Width();
            int freeHeight = i->Height();
            
            if (freeWidth >= width && freeHeight >= height)
            {
                // Calculate rank for free area. Lower is better
                int freeArea = freeWidth * freeHeight;
                
                if (freeArea < bestFreeArea)
                {
                    best = i;
                    bestFreeArea = freeArea;
                }
            }
        }
        
        if (best == freeAreas_.end())
        {
            if (doubleWidth_ && size_.m_x < maxSize_.m_x)
            {
                int oldWidth = size_.m_x;
                size_.m_x <<= 1;
                // If no allocations yet, simply expand the single free area
                IntRect& first = freeAreas_.front();
                if (freeAreas_.size() == 1 && first.m_left == 0 && first.m_top == 0 && first.m_right == oldWidth && first.m_bottom == size_.m_y)
                    first.m_right = size_.m_x;
                else
                {
                    IntRect newArea(oldWidth, 0, size_.m_x, size_.m_y);
                    freeAreas_.push_back(newArea);
                }
            }
            else if (!doubleWidth_ && size_.m_y < maxSize_.m_y)
            {
                int oldHeight = size_.m_y;
                size_.m_y <<= 1;
                // If no allocations yet, simply expand the single free area
                IntRect& first = freeAreas_.front();
                if (freeAreas_.size() == 1 && first.m_left == 0 && first.m_top == 0 && first.m_right == size_.m_x && first.m_bottom == oldHeight)
                    first.m_bottom = size_.m_y;
                else
                {
                    IntRect newArea(0, oldHeight, size_.m_x, size_.m_y);
                    freeAreas_.push_back(newArea);
                }
            }
            else
                return false;
            
            doubleWidth_ = !doubleWidth_;
        }
        else
            break;
    }
    
    IntRect reserved(best->m_left, best->m_top, best->m_left + width, best->m_top + height);
    x = best->m_left;
    y = best->m_top;
    
    if (fastMode_)
    {
        // Reserve the area by splitting up the remaining free area
        best->m_left = reserved.m_right;
        if (best->Height() > 2 * height || height >= size_.m_y / 2)
        {
            IntRect splitArea(reserved.m_left, reserved.m_bottom, best->m_right, best->m_bottom);
            best->m_bottom = reserved.m_bottom;
            freeAreas_.push_back(splitArea);
        }
    }
    else
    {
        // Remove the reserved area from all free areas
        for (unsigned i = 0; i < freeAreas_.size();)
        {
            if (SplitRect(freeAreas_[i], reserved))
            	///TODO
              //  freeAreas_.erase(i);
            	;
            else
                ++i;
        }
        
        Cleanup();
    }
    
    return true;
}

bool AreaAllocator::SplitRect(IntRect original, const IntRect& reserve)
{
    if (reserve.m_right > original.m_left && reserve.m_left < original.m_right && reserve.m_bottom > original.m_top &&
        reserve.m_top < original.m_bottom)
    {
        // Check for splitting from the right
        if (reserve.m_right < original.m_right) 
        {
            IntRect newRect = original;
            newRect.m_left = reserve.m_right;
            freeAreas_.push_back(newRect);
        }
        // Check for splitting from the left
        if (reserve.m_left > original.m_left)
        {
            IntRect newRect = original;
            newRect.m_right = reserve.m_left;
            freeAreas_.push_back(newRect);
        }
        // Check for splitting from the bottom
        if (reserve.m_bottom < original.m_bottom)
        {
            IntRect newRect = original;
            newRect.m_top = reserve.m_bottom;
            freeAreas_.push_back(newRect);
        }
        // Check for splitting from the top
        if (reserve.m_top > original.m_top)
        {
            IntRect newRect = original;
            newRect.m_bottom = reserve.m_top;
            freeAreas_.push_back(newRect);
        }
        
        return true;
    }
    
    return false;
}

void AreaAllocator::Cleanup()
{
    // Remove rects which are contained within another rect
    for (unsigned i = 0; i < freeAreas_.size(); )
    {
        bool erased = false;
        for (unsigned j = i + 1; j < freeAreas_.size(); )
        {
            if ((freeAreas_[i].m_left >= freeAreas_[j].m_left) &&
                (freeAreas_[i].m_top >= freeAreas_[j].m_top) &&
                (freeAreas_[i].m_right <= freeAreas_[j].m_right) &&
                (freeAreas_[i].m_bottom <= freeAreas_[j].m_bottom))
            {
            	///TODO
               // freeAreas_.erase(i);
                erased = true;
                break;
            }
            if ((freeAreas_[j].m_left >= freeAreas_[i].m_left) &&
                (freeAreas_[j].m_top >= freeAreas_[i].m_top) &&
                (freeAreas_[j].m_right <= freeAreas_[i].m_right) &&
                (freeAreas_[j].m_bottom <= freeAreas_[i].m_bottom))
            	///TODO
                //freeAreas_.erase(j);
            	;
            else
                ++j;
        }
        if (!erased)
            ++i;
    }
}

}
