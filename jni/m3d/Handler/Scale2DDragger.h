

#ifndef M3DMANIPULATOR_SCALE2DDRAGGER
#define M3DMANIPULATOR_SCALE2DDRAGGER 1

#include "m3d/handler/Dragger.h"
#include "m3d/handler/Projector.h"

namespace M3D {

/**
 * Dragger for performing 2D scaling.
 */
class M3D_API Scale2DDragger : public Dragger
{
    public:

        enum ScaleMode
        {
            SCALE_WITH_ORIGIN_AS_PIVOT = 0,
            SCALE_WITH_OPPOSITE_HANDLE_AS_PIVOT
        };

        Scale2DDragger(ScaleMode scaleMode=SCALE_WITH_ORIGIN_AS_PIVOT);
 
        /**
         * Handle pick events on dragger and generate TranslateInLine commands.
         */
        virtual bool handle(const PointerInfo& pi, const  TouchEvent& ea/*,  GUIActionAdapter& us*/);

        /** Setup default geometry for dragger. */
        void setupDefaultGeometry();

        /** Set/Get min scale for dragger. */
        inline void             setMinScale(const M3D::Vector2& min) { _minScale = min; }
        inline const M3D::Vector2& getMinScale() const               { return _minScale; }

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

        /** Set/Get the handle nodes for dragger. */
	/*    inline void setTopLeftHandleNode (M3D::Node& node) { _topLeftHandleNode = &node; }
		inline M3D::Node* getTopLeftHandleNode() { return _topLeftHandleNode.get(); }
		inline const M3D::Node* getTopLeftHandleNode() const { return _topLeftHandleNode.get(); }
		inline void setBottomLeftHandleNode (M3D::Node& node) { _bottomLeftHandleNode = &node; }
		inline M3D::Node* getBottomLeftHandleNode() { return _bottomLeftHandleNode.get(); }
		inline const M3D::Node* getBottomLeftHandleNode() const { return _bottomLeftHandleNode.get(); }
		inline void setTopRightHandleNode(M3D::Node& node) { _topRightHandleNode = &node; }
		inline M3D::Node* getTopRightHandleNode() { return _topRightHandleNode.get(); }
		inline const M3D::Node* getTopRightHandleNode() const { return _topRightHandleNode.get(); }
		inline void setBottomRightHandleNode(M3D::Node& node) { _bottomRightHandleNode = &node; }
		inline M3D::Node* getBottomRightHandleNode() { return _bottomRightHandleNode.get(); }
		inline const M3D::Node* getBottomRightHandleNode() const { return _bottomRightHandleNode.get(); }*/

        /** Set/Get the handle nodes postion for dragger. */
        inline void setTopLeftHandlePosition(const M3D::Vector2& pos) { _topLeftHandlePosition = pos; }
        const M3D::Vector2& getTopLeftHandlePosition() const { return _topLeftHandlePosition; }
        inline void setBottomLeftHandlePosition(const M3D::Vector2& pos) { _bottomLeftHandlePosition = pos; }
        const M3D::Vector2& getBottomLeftHandlePosition() const { return _bottomLeftHandlePosition; }
        inline void setTopRightHandlePosition(const M3D::Vector2& pos) { _topRightHandlePosition = pos; }
        const M3D::Vector2& getTopRightHandlePosition() const { return _topRightHandlePosition; }
        inline void setBottomRightHandlePosition(const M3D::Vector2& pos){ _bottomRightHandlePosition = pos; }
        const M3D::Vector2& getBottomRightHandlePosition() const { return _bottomRightHandlePosition; }

    protected:

        virtual ~Scale2DDragger();

         PlaneProjector*  _projector;
        M3D::Vector3                      _startProjectedPoint;
        M3D::Vector2                      _scaleCenter;
        M3D::Vector2                      _referencePoint;
        M3D::Vector2                      _minScale;

         //M3D::Node >       _topLeftHandleNode;
         //M3D::Node >       _bottomLeftHandleNode;
         //M3D::Node >       _topRightHandleNode;
         //M3D::Node >       _bottomRightHandleNode;

        M3D::Vector2                      _topLeftHandlePosition;
        M3D::Vector2                      _bottomLeftHandlePosition;
        M3D::Vector2                      _topRightHandlePosition;
        M3D::Vector2                      _bottomRightHandlePosition;

        M3D::Color                       _color;
        M3D::Color                       _pickColor;

        ScaleMode                       _scaleMode;
};


}

#endif
