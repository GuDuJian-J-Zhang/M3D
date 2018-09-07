

#ifndef M3DMANIPULATOR_ROTATECYLINDERDRAGGER
#define M3DMANIPULATOR_ROTATECYLINDERDRAGGER 1

#include "m3d/handler/Dragger.h"
#include "m3d/handler/Projector.h"
#include "m3d/base/Matrix3x4.h"

namespace M3D {

/**
 * Dragger for performing 3D rotation on a cylinder.
 */
class M3D_API RotateCylinderDragger : public Dragger
{
    public:

        RotateCylinderDragger();
 
        /**
         * Handle pick events on dragger and generate TranslateInLine commands.
         */
        virtual bool handle(const PointerInfo& pi, const  TouchEvent& ea/*,  GUIActionAdapter& us*/);

        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();
 
    protected:

        virtual ~RotateCylinderDragger();

        CylinderPlaneProjector* _projector;

        M3D::Vector3  _prevWorldProjPt;
        bool        _prevPtOnCylinder;
        M3D::Matrix3x4 _startLocalToWorld, _startWorldToLocal;
        M3D::Quaternion   _prevRotation;
};


}

#endif
