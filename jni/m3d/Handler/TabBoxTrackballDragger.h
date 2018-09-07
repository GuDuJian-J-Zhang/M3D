

#ifndef M3DMANIPULATOR_TABBOXTRACKBALLDRAGGER
#define M3DMANIPULATOR_TABBOXTRACKBALLDRAGGER 1

#include "m3d/handler/TrackballDragger.h"
#include "m3d/handler/TabBoxDragger.h"

namespace M3D {

/**
 * Dragger for performing rotation in all axes.
 */
class M3D_API TabBoxTrackballDragger : public CompositeDragger
{
    public:

        TabBoxTrackballDragger();
 
        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

    protected:

        virtual ~TabBoxTrackballDragger();

        TrackballDragger*  _trackballDragger;
        TabBoxDragger*     _tabBoxDragger;
};


}

#endif
