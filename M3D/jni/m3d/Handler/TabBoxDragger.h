

#ifndef M3DMANIPULATOR_TABBOXDRAGGER
#define M3DMANIPULATOR_TABBOXDRAGGER 1

#include "m3d/handler/TabPlaneDragger.h"

namespace M3D {

/**
 * TabBoxDragger consists of 6 TabPlaneDraggers to form a box dragger that
 * performs translation and scaling.
 */
class M3D_API TabBoxDragger : public CompositeDragger
{
    public:

        TabBoxDragger();
 
        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

        void setPlaneColor(const M3D::Color& color);

    protected:

        virtual ~TabBoxDragger();

        std::vector<  TabPlaneDragger* > _planeDraggers;
};


}

#endif
