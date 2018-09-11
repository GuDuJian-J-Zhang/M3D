

#ifndef M3DMANIPULATOR_TRANSLATE1DDRAGGER
#define M3DMANIPULATOR_TRANSLATE1DDRAGGER 1

#include "m3d/handler/Dragger.h"
#include "m3d/handler/Projector.h"

namespace M3D {


/**
 * Dragger for performing 1D translation.
 */
class M3D_API Translate1DDragger : public Dragger
{
    public:

        Translate1DDragger();
 
        Translate1DDragger(const M3D::Vector3& s, const M3D::Vector3& e);

        /** Handle pick events on dragger and generate TranslateInLine commands. */
        virtual bool handle(const PointerInfo& pi, const  TouchEvent& ea);

        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

		void setupDefaultPointGeometry();
    protected:

        virtual ~Translate1DDragger();

        LineProjector*  _projector;
        M3D::Vector3                      _startProjectedPoint;
        bool                            _checkForNodeInNodePath;
};


}

#endif
