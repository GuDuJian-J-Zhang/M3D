

#ifndef M3DMANIPULATOR_TRANSLATE2DDRAGGER
#define M3DMANIPULATOR_TRANSLATE2DDRAGGER 1

#include "m3d/handler/Dragger.h"
#include "m3d/handler/Projector.h"
 
namespace M3D {

/**
 * Dragger for performing 2D translation.
 */
class M3D_API Translate2DDragger : public Dragger
{
    public:

        Translate2DDragger();

        Translate2DDragger(const M3D::Plane& plane);

       
        /** Handle pick events on dragger and generate TranslateInLine commands. */
        virtual bool handle(const PointerInfo& pi, const  TouchEvent& ea);

        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();
 
    protected:

        virtual ~Translate2DDragger();

         PlaneProjector*  _projector;
        M3D::Vector3                      _startProjectedPoint;
};


}

#endif
