

#include "m3d/handler/RotateCylinderAxisDragger.h"
#include "m3d/scene/ShapeNode.h"

using namespace M3D;

RotateCylinderAxisDragger::RotateCylinderAxisDragger():CompositeDragger()
{
    _xDragger = new RotateCylinderDragger();
	_xDragger->AddRef();
	_xDragger->SetDraggerName(L"XRotate");
    AddChild(_xDragger);
    addDragger(_xDragger);
    
    _yDragger = new RotateCylinderDragger();
	_yDragger->SetDraggerName(L"YRotate");
	AddChild(_yDragger);
    addDragger(_yDragger);
	_yDragger->AddRef();

    _zDragger = new RotateCylinderDragger();
	_zDragger->SetDraggerName(L"ZRotate");
	AddChild(_zDragger);
    addDragger(_zDragger);
	_zDragger->AddRef();

    setParentDragger(getParentDragger());
}
       
RotateCylinderAxisDragger::~RotateCylinderAxisDragger()
{
	ReleaseMe(_xDragger);
	ReleaseMe(_yDragger);
	ReleaseMe(_zDragger);
}

void RotateCylinderAxisDragger::setupDefaultGeometry()
{
    // Create a line.
    //M3D::Geode* lineGeode = new M3D::Geode;
    //{
    //    M3D::Geometry* geometry = new M3D::Geometry();
    //    
    //    M3D::Vec3Array* vertices = new M3D::Vec3Array(2);
    //    (*vertices)[0] = M3D::Vector3(0.0f,0.0f,0.0f);
    //    (*vertices)[1] = M3D::Vector3(0.0f,0.0f,1.0f);

    //    geometry->setVertexArray(vertices);
    //    geometry->addPrimitiveSet(new M3D::DrawArrays(M3D::PrimitiveSet::LINES,0,2));

    //    lineGeode->addDrawable(geometry);
    //}

    //// Turn of lighting for line and set line width.
    //{
    //    M3D::LineWidth* linewidth = new M3D::LineWidth();
    //    linewidth->setWidth(2.0f);
    //    lineGeode->getOrCreateStateSet()->setAttributeAndModes(linewidth, M3D::StateAttribute::ON);
    //    lineGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,M3D::StateAttribute::OFF);
    //}

    //// Add line to all the individual 1D draggers.
    //_xDragger->addChild(lineGeode);
    //_yDragger->addChild(lineGeode);
    //_zDragger->addChild(lineGeode);

    //M3D::Geode* geode = new M3D::Geode;
    //
    //// Create a cone.
    //{
    //    M3D::Cone* cone = new M3D::Cone (M3D::Vector3(0.0f, 0.0f, 1.0f), 0.025f, 0.10f);
    //    geode->addDrawable(new M3D::ShapeDrawable(cone));
    //}

    //// Create an invisible cylinder for picking the line.
    //{
    //    M3D::Cylinder* cylinder = new M3D::Cylinder (M3D::Vector3(0.0f,0.0f,0.5f), 0.015f, 1.0f);
    //    M3D::Drawable* geometry = new M3D::ShapeDrawable(cylinder);
    //    setDrawableToAlwaysCull(*geometry);
    //    geode->addDrawable(geometry);
    //}

    //// Add geode to all 1D draggers.
    //_xDragger->addChild(geode);
    //_yDragger->addChild(geode);
    //_zDragger->addChild(geode);
 
	_xDragger->setupDefaultGeometry();
	_yDragger->setupDefaultGeometry();
	_zDragger->setupDefaultGeometry();

    // Rotate X-axis dragger appropriately.
    {
        M3D::Quaternion rotation(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(1.0f, 0.0f, 0.0f));
        _xDragger->SetRotation(rotation);
    }

    // Rotate Y-axis dragger appropriately.
    {
        M3D::Quaternion rotation(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(0.0f, 1.0f, 0.0f));
        _yDragger->SetRotation(rotation);
    }

    // Send different colors for each dragger.
    _xDragger->setColor(M3D::Color(1.0f,0.0f,0.0f,1.0f));
    _yDragger->setColor(M3D::Color(0.0f,1.0f,0.0f,1.0f));
    _zDragger->setColor(M3D::Color(0.0f,0.0f,1.0f,1.0f));

	_xDragger->setPickColor(Color::YELLOW);
	_yDragger->setPickColor(Color::YELLOW);
	_zDragger->setPickColor(Color::YELLOW);

	_xDragger->SetPreSelectColor(Color::YELLOW);
	_yDragger->SetPreSelectColor(Color::YELLOW);
	_zDragger->SetPreSelectColor(Color::YELLOW);
}

void M3D::RotateCylinderAxisDragger::SetScene(M3D::SceneManager * val)
{
	_xDragger->SetScene(val);
	_yDragger->SetScene(val);
	_zDragger->SetScene(val);
}
