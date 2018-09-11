

#include "m3d/handler/TranslateAxisDragger.h"
#include "../scene/ShapeNode.h"

using namespace M3D;

TranslateAxisDragger::TranslateAxisDragger():CompositeDragger()
{
	_xDragger = new Translate1DDragger(M3D::Vector3(0.0, 0.0, 0.0), M3D::Vector3(0.0, 0.0, 1.0)); 
	_xDragger->AddRef();
	_xDragger->SetDraggerName(L"XAxis");
    AddChild(_xDragger);
    addDragger(_xDragger);
    
    _yDragger = new Translate1DDragger(M3D::Vector3(0.0,0.0,0.0), M3D::Vector3(0.0,0.0,1.0));
	_yDragger->AddRef();
	_yDragger->SetDraggerName(L"YAxis");
	AddChild(_yDragger);
    addDragger(_yDragger);
    
    _zDragger = new Translate1DDragger(M3D::Vector3(0.0,0.0,0.0), M3D::Vector3(0.0,0.0,1.0));
	_zDragger->AddRef();
	_zDragger->SetDraggerName(L"ZAxis");
	AddChild(_zDragger);
    addDragger(_zDragger);

	 _xzDragger = new Translate2DDragger();
	 _xzDragger->AddRef();
	 _xzDragger->SetDraggerName(L"XZAxis");
	 AddChild(_xzDragger);
	 addDragger(_xzDragger);
	 _xyDragger = new Translate2DDragger();
	 _xyDragger->AddRef();
	 _xyDragger->SetDraggerName(L"XYAxis");
	 AddChild(_xyDragger);
	 addDragger(_xyDragger);
	 _yzDragger = new Translate2DDragger();
	 _yzDragger->AddRef();
	 _yzDragger->SetDraggerName(L"YZAxis");
	 AddChild(_yzDragger);
	  addDragger(_yzDragger);

	 _xyzDragger = new Translate3DDragger();
	 _xyzDragger->AddRef();
	 _xyzDragger->SetDraggerName(L"XYZAxis");
	 AddChild(_xyzDragger);
	 addDragger(_xyzDragger);
 
    setParentDragger(getParentDragger());
}
       
TranslateAxisDragger::~TranslateAxisDragger()
{
	ReleaseMe(_xDragger);
	ReleaseMe(_yDragger);
	ReleaseMe(_zDragger);
	ReleaseMe(_xzDragger);
	ReleaseMe(_xyDragger);
	ReleaseMe(_yzDragger);
	ReleaseMe(_xyzDragger);
}



void TranslateAxisDragger::setupDefaultGeometry()
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

	const float AxisAlpha = 1.0f;
 
	_xDragger->setupDefaultGeometry();
	_yDragger->setupDefaultGeometry();
	_zDragger->setupDefaultGeometry();

	_xzDragger->setupDefaultGeometry();
	_xyDragger->setupDefaultGeometry();
	_yzDragger->setupDefaultGeometry();

	_xyzDragger->setupDefaultGeometry();
    // Rotate X-axis dragger appropriately.
    {
        M3D::Quaternion rotation(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(1.0f, 0.0f, 0.0f));
        _xDragger->SetRotation(rotation);
		M3D::Quaternion rotation2(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(.0f, 1.0f, 0.0f));
		_xyDragger->SetRotation(rotation2);
    }

    // Rotate Y-axis dragger appropriately.
    {
        M3D::Quaternion rotation(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(0.0f, 1.0f, .0f));
        _yDragger->SetRotation(rotation);
		M3D::Quaternion rotation2(M3D::Vector3(1.0f, 0.0f, .0f), M3D::Vector3(.0f, 1.0f, .0f));
		_yzDragger->SetRotation(rotation2);
    }

    // Send different colors for each dragger.
	_xDragger->setColor(M3D::Color(1.0f, 0.0f, 0.0f, AxisAlpha));
	_yDragger->setColor(M3D::Color(0.0f, 1.0f, 0.0f, AxisAlpha));
	_zDragger->setColor(M3D::Color(0.0f, 0.0f, 1.0f, AxisAlpha));

	_xzDragger->setColor(M3D::Color(1.0f, 0.0f, 0.0f, AxisAlpha));
	_xyDragger->setColor(M3D::Color(0.0f, 1.0f, 0.0f, AxisAlpha));
	_yzDragger->setColor(M3D::Color(0.0f, 0.0f, 1.0f, AxisAlpha));

	_xyzDragger->setColor(M3D::Color(0.0f, 0.0f, 1.0f, AxisAlpha));

	_xDragger->setPickColor(Color::YELLOW);
	_yDragger->setPickColor(Color::YELLOW);
	_zDragger->setPickColor(Color::YELLOW);

	_xDragger->SetPreSelectColor(Color::YELLOW);
	_yDragger->SetPreSelectColor(Color::YELLOW);
	_zDragger->SetPreSelectColor(Color::YELLOW);

	_xzDragger->setPickColor(Color::YELLOW);
	_xyDragger->setPickColor(Color::YELLOW);
	_yzDragger->setPickColor(Color::YELLOW);

	_xyzDragger->setPickColor(Color::YELLOW);

	_xzDragger->SetPreSelectColor(Color::YELLOW);
	_xyDragger->SetPreSelectColor(Color::YELLOW);
	_yzDragger->SetPreSelectColor(Color::YELLOW);

	_xyzDragger->SetPreSelectColor(Color::YELLOW);
}

void M3D::TranslateAxisDragger::SetXAxisVisible(bool val)
{
	_xDragger->SetVisible(val);
}

void M3D::TranslateAxisDragger::SetYAxisVisible(bool val)
{
	_yDragger->SetVisible(val);
}

void M3D::TranslateAxisDragger::SetZAxisVisible(bool val)
{
	_zDragger->SetVisible(val);
}

void M3D::TranslateAxisDragger::SetXYPlaneVisible(bool val)
{
	_xyDragger->SetVisible(val);
}

void M3D::TranslateAxisDragger::SetYZPlaneVisible(bool val)
{
	_yzDragger->SetVisible(val);
}

void M3D::TranslateAxisDragger::SetXZPlaneVisible(bool val)
{
	_xzDragger->SetVisible(val);
}

void M3D::TranslateAxisDragger::SetScene(M3D::SceneManager* val)
{
	CompositeDragger::SetScene(val);
	_xDragger->SetScene(val);
	_yDragger->SetScene(val);
	_zDragger->SetScene(val);

	_xzDragger->SetScene(val);
	_xyDragger->SetScene(val);
	_yzDragger->SetScene(val);

	_xyzDragger->SetScene(val);
}
