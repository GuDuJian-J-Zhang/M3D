

#ifndef M3DMANIPULATOR_TRANSLATEAXISDRAGGER
#define M3DMANIPULATOR_TRANSLATEAXISDRAGGER 1

#include "m3d/handler/Translate1DDragger.h"
#include "m3d/handler/Translate2DDragger.h"
#include "m3d/handler/dragger.h"

namespace M3D {

/**
 * Dragger for performing translation in all three axes.
 */
class M3D_API TranslateAxisDragger:public CompositeDragger
{
    public:
        TranslateAxisDragger();
        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();
		void SetXAxisVisible(bool val);

		void SetYAxisVisible(bool val);

		void SetZAxisVisible(bool val);

		void SetXYPlaneVisible(bool val);

		void SetYZPlaneVisible(bool val);

		void SetXZPlaneVisible(bool val);

		void SetScene(M3D::SceneManager* val);

    protected:
        virtual ~TranslateAxisDragger();
        Translate1DDragger* _xDragger;
        Translate1DDragger* _yDragger;
        Translate1DDragger*  _zDragger;

		Translate2DDragger* _xzDragger;
		Translate2DDragger* _xyDragger;
		Translate2DDragger*  _yzDragger;
};


}

#endif
