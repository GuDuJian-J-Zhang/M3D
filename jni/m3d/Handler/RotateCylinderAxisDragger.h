

#ifndef M3DMANIPULATOR_ROTATECYLINDERAXISDRAGGER
#define M3DMANIPULATOR_ROTATECYLINDERAXISDRAGGER 1

#include "m3d/handler/RotateCylinderDragger.h"
#include "m3d/handler/dragger.h"

namespace M3D {

/**
 * Dragger for performing translation in all three axes.
 */
class M3D_API RotateCylinderAxisDragger :public CompositeDragger
{
    public:
		RotateCylinderAxisDragger();
        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

		void SetScene(M3D::SceneManager* val);
    protected:
        virtual ~RotateCylinderAxisDragger();
		RotateCylinderDragger* _xDragger;
		RotateCylinderDragger* _yDragger;
		RotateCylinderDragger*  _zDragger;
};


}

#endif
