

#ifndef M3DMANIPULATOR_TRACKBALLDRAGGER
#define M3DMANIPULATOR_TRACKBALLDRAGGER 1

#include "m3d/handler/RotateCylinderDragger.h"
#include "m3d/handler/RotateSphereDragger.h"

namespace M3D {

/**
 * Dragger for performing rotation in all axes.
 */
class M3D_API TrackballDragger : public CompositeDragger
{
    public:

        TrackballDragger(bool useAutoTransform=false);
 
        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

    protected:

        virtual ~TrackballDragger();

        RotateCylinderDragger* _xDragger;
        RotateCylinderDragger* _yDragger;
        RotateCylinderDragger* _zDragger;
        RotateSphereDragger*   _xyzDragger;
};


}

#endif
