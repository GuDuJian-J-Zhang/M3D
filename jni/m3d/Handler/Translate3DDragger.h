

#ifndef M3DMANIPULATOR_TRANSLATE3DDRAGGER
#define M3DMANIPULATOR_TRANSLATE3DDRAGGER 1

#include "m3d/handler/Dragger.h"
#include "m3d/handler/Projector.h"
 
namespace M3D {

/**
 * Dragger for performing 2D translation.
 */
class M3D_API Translate3DDragger : public Dragger
{
    public:

		Translate3DDragger();

		Translate3DDragger(const M3D::Plane& plane);

       
        /** Handle pick events on dragger and generate TranslateInLine commands. */
        virtual bool handle(const PointerInfo& pi, const  TouchEvent& ea);

        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();
 
		virtual void FindVisiableObject(RenderAction* renderAction);

    protected:

        virtual ~Translate3DDragger();

         PlaneProjector*  _projector;
        M3D::Vector3                      _startProjectedPoint;
};


}

#endif
