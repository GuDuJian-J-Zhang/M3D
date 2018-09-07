

#ifndef M3DMANIPULATOR_SCALEAXISDRAGGER
#define M3DMANIPULATOR_SCALEAXISDRAGGER 1

#include "m3d/handler/Scale1DDragger.h"
#include "m3d/handler/Scale2DDragger.h"
namespace M3D {

/**
 * Dragger for performing scaling on all 3 axes.
 */
class M3D_API ScaleAxisDragger : public CompositeDragger
{
    public:

        ScaleAxisDragger();
 
        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

		virtual void addDraggerCallback(DraggerCallback* dc);
		virtual void removeDraggerCallback(DraggerCallback* dc);


		virtual void ClearDraggerCallbacks();
    protected:

        virtual ~ScaleAxisDragger();

         Scale1DDragger*        _xDragger;
         Scale1DDragger*        _yDragger;
         Scale1DDragger*        _zDragger;

		 Scale2DDragger* _xzDragger;
		 Scale2DDragger* _xyDragger;
		 Scale2DDragger*  _yzDragger;

		 CompositeDragger* comPositeDragger;
		 CompositeDragger* comPositeDragger1;
	     CompositeDragger* comPositeDragger2;
};


}

#endif
