

#ifndef M3DMANIPULATOR_SCALE1DDRAGGER
#define M3DMANIPULATOR_SCALE1DDRAGGER 1

#include "m3d/handler/Dragger.h"
#include "m3d/handler/Projector.h"

namespace M3D {

/**
 * Dragger for performing 1D scaling.
 */
class M3D_API Scale1DDragger : public Dragger
{
    public:

        enum ScaleMode
        {
            SCALE_WITH_ORIGIN_AS_PIVOT = 0,
            SCALE_WITH_OPPOSITE_HANDLE_AS_PIVOT
        };

        Scale1DDragger(ScaleMode scaleMode=SCALE_WITH_ORIGIN_AS_PIVOT);
 
        /**
         * Handle pick events on dragger and generate TranslateInLine commands.
         */
        virtual bool handle(const PointerInfo& pi, const  TouchEvent& ea/*,  GUIActionAdapter& us*/);

        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

        /** Set/Get min scale for dragger. */
        inline void  setMinScale(double min) { _minScale = min; }
        inline double getMinScale() const    { return _minScale; }
 
        /** Set/Get left and right handle nodes for dragger. */
        //inline void setLeftHandleNode (M3D::Node& node) { _leftHandleNode = &node; }
        //inline void setRightHandleNode(M3D::Node& node) { _rightHandleNode = &node; }
        //inline M3D::Node* getLeftHandleNode()  { return _leftHandleNode; }
        //inline const M3D::Node* getLeftHandleNode() const  { return _leftHandleNode; }
        //inline M3D::Node* getRightHandleNode() { return _rightHandleNode; }
        //inline const M3D::Node* getRightHandleNode() const { return _rightHandleNode; }

        /** Set left/right handle position. */
        inline void  setLeftHandlePosition(double pos)  { _projector->getLineStart() = M3D::Vector3(pos,0.0,0.0); }
        inline double getLeftHandlePosition() const     { return _projector->getLineStart().Data()[0]; }
        inline void  setRightHandlePosition(double pos) { _projector->getLineEnd() = M3D::Vector3(pos,0.0,0.0); }
        inline double getRightHandlePosition() const    { return _projector->getLineEnd().Data()[0]; }

		double GetScale() const { return _scale; }
		void SetScale(double val) { _scale = val; }
protected:

        virtual ~Scale1DDragger();

         LineProjector*  _projector;
        M3D::Vector3                      _startProjectedPoint;
		M3D::Vector3                      _priProjectedPoint;
        double                          _scaleCenter;
        double                          _minScale;
		double    _scale;
     /*    M3D::Node*       _leftHandleNode;
         M3D::Node*       _rightHandleNode;*/
 
        ScaleMode                       _scaleMode;
};


}

#endif
