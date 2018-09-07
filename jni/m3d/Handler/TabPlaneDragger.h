

#ifndef M3DMANIPULATOR_TABPLANEDRAGGER
#define M3DMANIPULATOR_TABPLANEDRAGGER 1

#include "m3d/handler/TranslatePlaneDragger.h"
#include "m3d/handler/Scale2DDragger.h"
#include "m3d/handler/Scale1DDragger.h"

namespace M3D {

/**
 * Tab plane dragger consists of a plane with tabs on it's corners and edges
 * for scaling. And the plane is used as a 2D translate dragger.
 */
class M3D_API TabPlaneDragger : public CompositeDragger
{
    public:

        TabPlaneDragger();
 
        virtual bool handle(const PointerInfo& pi, const  GUIEventAdapter& ea/*,  GUIActionAdapter& us*/);

        /** Setup default geometry for dragger. */
        void setupDefaultGeometry(bool twoSidedHandle = true);

        void setPlaneColor(const M3D::Color& color) { _translateDragger->setColor(color); }

    protected:

        virtual ~TabPlaneDragger();

         TranslatePlaneDragger*   _translateDragger;
         Scale2DDragger*          _cornerScaleDragger;
         Scale1DDragger*         _horzEdgeScaleDragger;
         Scale1DDragger*          _vertEdgeScaleDragger;

        float                                   _handleScaleFactor;
};


}

#endif
