

#ifndef M3DMANIPULATOR_TRANSLATEPLANEDRAGGER
#define M3DMANIPULATOR_TRANSLATEPLANEDRAGGER 1

#include "m3d/handler/Translate2DDragger.h"
#include "m3d/handler/Translate1DDragger.h"

namespace M3D {

/**
 * Tab plane dragger consists of a plane with tabs on it's corners and edges
 * for scaling. And the plane is used as a 2D translate dragger.
 */
class M3D_API TranslatePlaneDragger : public CompositeDragger
{
    public:

        TranslatePlaneDragger();
 
        virtual bool handle(const PointerInfo& pi, const  TouchEvent& ea/*,  GUIActionAdapter& us*/);

        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

        inline void setColor(const M3D::Color& color) { if (_translate2DDragger) _translate2DDragger->setColor(color); }

        Translate1DDragger* getTranslate1DDragger() { return _translate1DDragger; }
        Translate2DDragger* getTranslate2DDragger() { return _translate2DDragger; }

    protected:

        virtual ~TranslatePlaneDragger();

         Translate2DDragger *      _translate2DDragger;
         Translate1DDragger *      _translate1DDragger;
        bool                                    _usingTranslate1DDragger;
};


}

#endif
