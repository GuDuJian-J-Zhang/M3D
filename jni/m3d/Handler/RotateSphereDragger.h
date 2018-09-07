

#ifndef M3DMANIPULATOR_ROTATESPHEREDRAGGER
#define M3DMANIPULATOR_ROTATESPHEREDRAGGER 1

#include "m3d/handler/Dragger.h"
#include "m3d/handler/Projector.h"

namespace M3D {

/**
 * Dragger for performing 3D rotation on a sphere.
 */
class M3D_API RotateSphereDragger : public Dragger
{
    public:

        RotateSphereDragger();
 
        /**
         * Handle pick events on dragger and generate TranslateInLine commands.
         */
        virtual bool handle(const PointerInfo&, const  TouchEvent& ea/*,  GUIActionAdapter& us*/);

        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

        /** Set/Get color for dragger. */
        inline void setColor(const M3D::Color& color) { _color = color; /*setMaterialColor(_color,*this);*/ }
        inline const M3D::Color& getColor() const { return _color; }

        /**
         * Set/Get pick color for dragger. Pick color is color of the dragger
         * when picked. It gives a visual feedback to show that the dragger has
         * been picked.
         */
        inline void setPickColor(const M3D::Color& color) { _pickColor = color; }
        inline const M3D::Color& getPickColor() const { return _pickColor; }

    protected:

        virtual ~RotateSphereDragger();

        SpherePlaneProjector* _projector;

        M3D::Vector3  _prevWorldProjPt;
        bool        _prevPtOnSphere;
        M3D::Matrix3x4 _startLocalToWorld, _startWorldToLocal;
        M3D::Quaternion   _prevRotation;

        M3D::Color _color;
        M3D::Color _pickColor;
};


}

#endif
