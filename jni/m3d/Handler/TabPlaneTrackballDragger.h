

#ifndef M3DMANIPULATOR_TABPLANETRACKBALLDRAGGER
#define M3DMANIPULATOR_TABPLANETRACKBALLDRAGGER 1

#include "m3d/handler/TrackballDragger.h"
#include "m3d/handler/TabPlaneDragger.h"

namespace M3D {

/**
 * Dragger for performing rotation in all axes.
 */
class M3D_API TabPlaneTrackballDragger : public CompositeDragger
{
    public:

        TabPlaneTrackballDragger();
 
        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

        void setPlaneColor(const M3D::Color& color) { _tabPlaneDragger->setPlaneColor(color); }

    protected:

        virtual ~TabPlaneTrackballDragger();

        TrackballDragger* _trackballDragger;
        TabPlaneDragger*  _tabPlaneDragger;
};


}

#endif
