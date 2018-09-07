

#include "m3d/handler/TabPlaneDragger.h"

using namespace M3D;

namespace
{

M3D::Node* createHandleNode(Scale2DDragger* cornerScaleDragger, float handleScaleFactor, bool twosided)
{
    M3D::Vec3Array* vertices = new M3D::Vec3Array(4);
    (*vertices)[0] = M3D::Vector3(cornerScaleDragger->getTopLeftHandlePosition()[0],0.0,cornerScaleDragger->getTopLeftHandlePosition()[1]) * handleScaleFactor;
    (*vertices)[1] = M3D::Vector3(cornerScaleDragger->getBottomLeftHandlePosition()[0],0.0,cornerScaleDragger->getBottomLeftHandlePosition()[1]) * handleScaleFactor;
    (*vertices)[2] = M3D::Vector3(cornerScaleDragger->getBottomRightHandlePosition()[0],0.0,cornerScaleDragger->getBottomRightHandlePosition()[1]) * handleScaleFactor;
    (*vertices)[3] = M3D::Vector3(cornerScaleDragger->getTopRightHandlePosition()[0],0.0,cornerScaleDragger->getTopRightHandlePosition()[1]) * handleScaleFactor;

    M3D::Geometry* geometry = new M3D::Geometry();
    geometry->setVertexArray(vertices);
    geometry->addPrimitiveSet(new M3D::DrawArrays(M3D::PrimitiveSet::QUADS,0,vertices->size()));

    M3D::Vec3Array* normals = new M3D::Vec3Array;
    normals->push_back(M3D::Vector3(0.0,1.0,0.0));
    geometry->setNormalArray(normals);
    geometry->setNormalBinding(M3D::Geometry::BIND_OVERALL);

    M3D::Geode* geode = new M3D::Geode;
    geode->setName("Dragger Handle");
    geode->addDrawable(geometry);

    if (!twosided)
    {
        M3D::CullFace* cullface = new M3D::CullFace;
        cullface->setMode(M3D::CullFace::FRONT);
        geode->getOrCreateStateSet()->setAttribute(cullface, M3D::StateAttribute::ON | M3D::StateAttribute::OVERRIDE);
        geode->getOrCreateStateSet()->setMode(GL_CULL_FACE, M3D::StateAttribute::ON | M3D::StateAttribute::OVERRIDE);
    }

    geode->getOrCreateStateSet()->setMode(GL_LIGHTING,M3D::StateAttribute::OFF);

    return geode;
}

M3D::Node* createHandleScene(const M3D::Vector3& pos, M3D::Node* handleNode, float handleScaleFactor)
{
    M3D::AutoTransform *at = new M3D::AutoTransform;
    at->setPosition(pos);
    at->setPivotPoint(pos * handleScaleFactor);
    at->setAutoScaleToScreen(true);
    at->addChild(handleNode);

    AntiSquish* as = new AntiSquish;
    as->setPivot(pos);
    as->addChild(at);

    return as;
}

void createCornerScaleDraggerGeometry(Scale2DDragger* cornerScaleDragger, M3D::Node* handleNode, float handleScaleFactor)
{
    // Create a top left box.
    {
        M3D::Node* handleScene = createHandleScene(M3D::Vector3(cornerScaleDragger->getTopLeftHandlePosition()[0],
                                                             0.0,cornerScaleDragger->getTopLeftHandlePosition()[1]), 
                                                   handleNode, handleScaleFactor);
        cornerScaleDragger->addChild(handleScene);
        cornerScaleDragger->setTopLeftHandleNode(*handleScene);
    }
    
    // Create a bottom left box.
    {
        M3D::Node* handleScene = createHandleScene(M3D::Vector3(cornerScaleDragger->getBottomLeftHandlePosition()[0],
                                                             0.0,cornerScaleDragger->getBottomLeftHandlePosition()[1]),
                                                   handleNode, handleScaleFactor);
        cornerScaleDragger->addChild(handleScene);
        cornerScaleDragger->setBottomLeftHandleNode(*handleScene);
    }

    // Create a bottom right box.
    {
        M3D::Node* handleScene = createHandleScene(M3D::Vector3(cornerScaleDragger->getBottomRightHandlePosition()[0],
                                                             0.0,cornerScaleDragger->getBottomRightHandlePosition()[1]),
                                                   handleNode, handleScaleFactor);
        cornerScaleDragger->addChild(handleScene);
        cornerScaleDragger->setBottomRightHandleNode(*handleScene);
    }

    // Create a top right box.
    {
        M3D::Node* handleScene = createHandleScene(M3D::Vector3(cornerScaleDragger->getTopRightHandlePosition()[0],
                                                             0.0,cornerScaleDragger->getTopRightHandlePosition()[1]),
                                                    handleNode, handleScaleFactor);
        cornerScaleDragger->addChild(handleScene);
        cornerScaleDragger->setTopRightHandleNode(*handleScene);
    }
}

void createEdgeScaleDraggerGeometry(Scale1DDragger* horzEdgeScaleDragger, Scale1DDragger* vertEdgeScaleDragger,
                                    M3D::Node* handleNode, float handleScaleFactor)
{
    // Create a left box.
    {
        M3D::Node* handleScene = createHandleScene(M3D::Vector3(horzEdgeScaleDragger->getLeftHandlePosition(),0.0,0.0),
                                                   handleNode, handleScaleFactor);
        horzEdgeScaleDragger->addChild(handleScene);
        horzEdgeScaleDragger->setLeftHandleNode(*handleScene);
    }
    
    // Create a right box.
    {
        M3D::Node* handleScene = createHandleScene(M3D::Vector3(horzEdgeScaleDragger->getRightHandlePosition(),0.0,0.0),
                                                   handleNode, handleScaleFactor);
        horzEdgeScaleDragger->addChild(handleScene);
        horzEdgeScaleDragger->setRightHandleNode(*handleScene);
    }

    // Create a top box.
    {
        M3D::Node* handleScene = createHandleScene(M3D::Vector3(vertEdgeScaleDragger->getLeftHandlePosition(),0.0,0.0),
                                                   handleNode, handleScaleFactor);
        vertEdgeScaleDragger->addChild(handleScene);
        vertEdgeScaleDragger->setLeftHandleNode(*handleScene);
    }

    // Create a bottom box.
    {
        M3D::Node* handleScene = createHandleScene(M3D::Vector3(vertEdgeScaleDragger->getRightHandlePosition(),0.0,0.0),
                                                   handleNode, handleScaleFactor);
        vertEdgeScaleDragger->addChild(handleScene);
        vertEdgeScaleDragger->setRightHandleNode(*handleScene);
    }

    M3D::Quaternion rotation; rotation.makeRotate(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(1.0f, 0.0f, 0.0f));
    vertEdgeScaleDragger->setMatrix(M3D::Matrix3x4(rotation));
}

void createTranslateDraggerGeometry(Scale2DDragger* cornerScaleDragger, TranslatePlaneDragger* translateDragger)
{
    // Create a polygon.
    {
        M3D::Geode* geode = new M3D::Geode;
        M3D::Geometry* geometry = new M3D::Geometry();
        
        M3D::Vec3Array* vertices = new M3D::Vec3Array(4);
        (*vertices)[0] = M3D::Vector3(cornerScaleDragger->getTopLeftHandlePosition()[0],0.0,cornerScaleDragger->getTopLeftHandlePosition()[1]);
        (*vertices)[1] = M3D::Vector3(cornerScaleDragger->getBottomLeftHandlePosition()[0],0.0,cornerScaleDragger->getBottomLeftHandlePosition()[1]);
        (*vertices)[2] = M3D::Vector3(cornerScaleDragger->getBottomRightHandlePosition()[0],0.0,cornerScaleDragger->getBottomRightHandlePosition()[1]);
        (*vertices)[3] = M3D::Vector3(cornerScaleDragger->getTopRightHandlePosition()[0],0.0,cornerScaleDragger->getTopRightHandlePosition()[1]);

        geometry->setVertexArray(vertices);
        geometry->addPrimitiveSet(new M3D::DrawArrays(M3D::PrimitiveSet::QUADS,0,vertices->size()));

        M3D::Vec3Array* normals = new M3D::Vec3Array;
        normals->push_back(M3D::Vector3(0.0,1.0,0.0));
        geometry->setNormalArray(normals);
        geometry->setNormalBinding(M3D::Geometry::BIND_OVERALL);

        geode->addDrawable(geometry);

        M3D::PolygonMode* polymode = new M3D::PolygonMode;
        polymode->setMode(M3D::PolygonMode::FRONT_AND_BACK,M3D::PolygonMode::LINE);
        geode->getOrCreateStateSet()->setAttributeAndModes(polymode,M3D::StateAttribute::OVERRIDE|M3D::StateAttribute::ON);

        geode->getOrCreateStateSet()->setMode(GL_LIGHTING,M3D::StateAttribute::OFF);

        translateDragger->getTranslate2DDragger()->addChild(geode);
    }
 
}

}

TabPlaneDragger::TabPlaneDragger() : _handleScaleFactor(20.0)
{
    _cornerScaleDragger = new Scale2DDragger(Scale2DDragger::SCALE_WITH_OPPOSITE_HANDLE_AS_PIVOT);
    addChild(_cornerScaleDragger.get());
    addDragger(_cornerScaleDragger.get());
    
    _horzEdgeScaleDragger = new Scale1DDragger(Scale1DDragger::SCALE_WITH_OPPOSITE_HANDLE_AS_PIVOT);
    addChild(_horzEdgeScaleDragger.get());
    addDragger(_horzEdgeScaleDragger.get());

    _vertEdgeScaleDragger = new Scale1DDragger(Scale1DDragger::SCALE_WITH_OPPOSITE_HANDLE_AS_PIVOT);
    addChild(_vertEdgeScaleDragger.get());
    addDragger(_vertEdgeScaleDragger.get());

    _translateDragger = new TranslatePlaneDragger();
    _translateDragger->setColor(M3D::Color(0.7f, 0.7f, 0.7f, 1.0f));
    addChild(_translateDragger.get());
    addDragger(_translateDragger.get());
 
    setParentDragger(getParentDragger());
}
       
TabPlaneDragger::~TabPlaneDragger()
{
}

bool TabPlaneDragger::handle(const PointerInfo& pointer, const  GUIEventAdapter& ea/*,  GUIActionAdapter& aa*/)
{
    if (ea.getButtonMask() &  GUIEventAdapter::RIGHT_MOUSE_BUTTON) return false;

    // Check if the dragger node is in the nodepath.
    if (!pointer.contains(this)) return false;

    // Since the translate plane and the handleNode lie on the same plane the hit could've been on either one. But we
    // need to handle the scaling draggers before the translation. Check if the node path has the scaling nodes else 
    // check for the scaling nodes in next hit.
    if (_cornerScaleDragger->handle(pointer, ea, aa))
        return true;
    if (_horzEdgeScaleDragger->handle(pointer, ea, aa))
        return true;
    if (_vertEdgeScaleDragger->handle(pointer, ea, aa))
        return true;
    
    PointerInfo nextPointer(pointer);
    nextPointer.next();
    
    while (!nextPointer.completed())
    {
        if (_cornerScaleDragger->handle(nextPointer, ea, aa))
            return true;
        if (_horzEdgeScaleDragger->handle(nextPointer, ea, aa))
            return true;
        if (_vertEdgeScaleDragger->handle(nextPointer, ea, aa))
            return true;

        nextPointer.next();
    }

    if (_translateDragger->handle(pointer, ea, aa))
        return true;

    return false;
}

void TabPlaneDragger::setupDefaultGeometry(bool twoSidedHandle)
{
    M3D::Node* handleNode = createHandleNode(_cornerScaleDragger.get(), _handleScaleFactor, twoSidedHandle);

    createCornerScaleDraggerGeometry(_cornerScaleDragger.get(), handleNode.get(), _handleScaleFactor);
    createEdgeScaleDraggerGeometry(_horzEdgeScaleDragger.get(),_vertEdgeScaleDragger.get(),handleNode.get(),_handleScaleFactor);
    createTranslateDraggerGeometry(_cornerScaleDragger.get(), _translateDragger.get());
}


