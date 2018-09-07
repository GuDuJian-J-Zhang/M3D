

#include "m3d/handler/TranslatePlaneDragger.h"

using namespace M3D;

TranslatePlaneDragger::TranslatePlaneDragger() : CompositeDragger(),_usingTranslate1DDragger(false)
{
    _translate2DDragger = new Translate2DDragger();
	_translate2DDragger->AddRef();
    _translate2DDragger->setColor(M3D::Color(0.7f, 0.7f, 0.7f, 1.0f));
    //addChild(_translate2DDragger.get());
    addDragger(_translate2DDragger);

    _translate1DDragger = new Translate1DDragger(M3D::Vector3(0.0f,0.0f,0.0f),M3D::Vector3(0.0f,1.0f,0.0f));
	_translate1DDragger->AddRef();
    //_translate1DDragger->setCheckForNodeInNodePath(false);
    //addChild(_translate1DDragger.get());
    addDragger(_translate1DDragger);

    setParentDragger(getParentDragger());
}
       
TranslatePlaneDragger::~TranslatePlaneDragger()
{
	ReleaseMe(_translate1DDragger);
	ReleaseMe(_translate2DDragger);
}

bool TranslatePlaneDragger::handle(const PointerInfo& pointer, const  TouchEvent& ea/*,  GUIActionAdapter& aa*/)
{
    // Check if the dragger node is in the nodepath.
    if (!pointer.contains(this)) return false;

   /* if ((ea.getButtonMask() &  GUIEventAdapter::MIDDLE_MOUSE_BUTTON) &&
        ea.getEventType() ==  GUIEventAdapter::PUSH)
        _usingTranslate1DDragger = true;*/

    bool handled = false;
    if (_usingTranslate1DDragger)
    {
        if (_translate1DDragger->handle(pointer, ea/*, aa*/))
            handled = true;
    }
    else
    {
        if (_translate2DDragger->handle(pointer, ea/*, aa*/))
            handled = true;
    }

    if (ea.getEventType() ==  TouchEvent::RELEASE)
        _usingTranslate1DDragger = false;

    return handled;
}

void TranslatePlaneDragger::setupDefaultGeometry()
{
    // Create a polygon.
    {
       /* M3D::Geode* geode = new M3D::Geode;
        M3D::Geometry* geometry = new M3D::Geometry();
        
        M3D::Vec3Array* vertices = new M3D::Vec3Array(4);
        (*vertices)[0] = M3D::Vector3(-0.5,0.0,0.5);
        (*vertices)[1] = M3D::Vector3(-0.5,0.0,-0.5);
        (*vertices)[2] = M3D::Vector3(0.5,0.0,-0.5);
        (*vertices)[3] = M3D::Vector3(0.5,0.0,0.5);

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

        _translate2DDragger->addChild(geode);*/
    }
}
