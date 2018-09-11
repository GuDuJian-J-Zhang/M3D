

#include "m3d/handler/ScaleAxisDragger.h"

namespace M3D
{

ScaleAxisDragger::ScaleAxisDragger() :CompositeDragger()
{
	_xzDragger = NULL;
	_xyDragger = NULL;
	 _yzDragger = NULL;
    _xDragger = new Scale1DDragger();
	_xDragger->AddRef();
	_xDragger->SetDraggerName(L"XScale");
	comPositeDragger = new CompositeDragger();
	comPositeDragger->AddRef();

	comPositeDragger->AddChild(_xDragger);
	comPositeDragger->addDragger(_xDragger);

	this->AddChild(comPositeDragger);

	comPositeDragger->setParentDragger(comPositeDragger->getParentDragger());

	comPositeDragger1 = new CompositeDragger();
	comPositeDragger1->AddRef();
    _yDragger = new Scale1DDragger();
	_yDragger->AddRef();
	_yDragger->SetDraggerName(L"YScale");
	comPositeDragger1->AddChild(_yDragger);
	comPositeDragger1->addDragger(_yDragger);

	this->AddChild(comPositeDragger1);

	comPositeDragger1->setParentDragger(comPositeDragger1->getParentDragger());

	comPositeDragger2 = new CompositeDragger();
	comPositeDragger2->AddRef();
    _zDragger = new Scale1DDragger();
	_zDragger->AddRef();
	_zDragger->SetDraggerName(L"ZScale");
	comPositeDragger2->AddChild(_zDragger);
	comPositeDragger2->addDragger(_zDragger);

	this->AddChild(comPositeDragger2);

	comPositeDragger2->setParentDragger(comPositeDragger2->getParentDragger());

	/*CompositeDragger* comPositeDragger3 = new CompositeDragger();
	_xzDragger = new Scale2DDragger();
	comPositeDragger3->AddChild(_xzDragger);
	comPositeDragger3->addDragger(_xzDragger);

	this->AddChild(comPositeDragger3);

	comPositeDragger3->setParentDragger(comPositeDragger3->getParentDragger());

	CompositeDragger* comPositeDragger = new CompositeDragger();
	_xyDragger = new Scale2DDragger();
	comPositeDragger->AddChild(_xyDragger);
	comPositeDragger->addDragger(_xyDragger);

	this->AddChild(comPositeDragger);

	comPositeDragger->setParentDragger(comPositeDragger->getParentDragger());

	CompositeDragger* comPositeDragger = new CompositeDragger();
	_yzDragger = new Scale2DDragger();
	comPositeDragger->AddChild(_yzDragger);
	comPositeDragger->addDragger(_yzDragger);

	this->AddChild(comPositeDragger);

	comPositeDragger->setParentDragger(comPositeDragger->getParentDragger());*/
    //setParentDragger(getParentDragger());
}
       
ScaleAxisDragger::~ScaleAxisDragger()
{
	ReleaseMe(_xyDragger);
	ReleaseMe(_yDragger);
	ReleaseMe(_zDragger);

	ReleaseMe(_xzDragger);
	ReleaseMe(_xyDragger);
	ReleaseMe(_yzDragger);

	ReleaseMe(comPositeDragger);
	ReleaseMe(comPositeDragger1);
	ReleaseMe(comPositeDragger2);
}

void ScaleAxisDragger::addDraggerCallback(DraggerCallback* dc)
{
	comPositeDragger->addDraggerCallback(dc);
	comPositeDragger1->addDraggerCallback(dc);
	comPositeDragger2->addDraggerCallback(dc);
}

void ScaleAxisDragger::removeDraggerCallback(DraggerCallback* dc)
{
	comPositeDragger->removeDraggerCallback(dc);
	comPositeDragger1->removeDraggerCallback(dc);
	comPositeDragger2->removeDraggerCallback(dc);
}

void ScaleAxisDragger::ClearDraggerCallbacks()
{
	Dragger::ClearDraggerCallbacks();

	if (comPositeDragger)
	{
		comPositeDragger->ClearDraggerCallbacks();
	}

	if (comPositeDragger1)
	{
		comPositeDragger1->ClearDraggerCallbacks();
	}

	if (comPositeDragger2)
	{
		comPositeDragger2->ClearDraggerCallbacks();
	}
}

void ScaleAxisDragger::SetScene(M3D::SceneManager * val)
{
	_xDragger->SetScene(val);
	_yDragger->SetScene(val);
	_zDragger->SetScene(val);
}

void ScaleAxisDragger::setupDefaultGeometry()
{
    // Create a line.
    //M3D::Geode* lineGeode = new M3D::Geode;
    //{
    //    M3D::Geometry* geometry = new M3D::Geometry();
    //    
    //    M3D::Vec3Array* vertices = new M3D::Vec3Array(2);
    //    (*vertices)[0] = M3D::Vector3(0.0f,0.0f,0.0f);
    //    (*vertices)[1] = M3D::Vector3(1.0f,0.0f,0.0f);

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
    //// Create a box.
    //geode->addDrawable(new M3D::ShapeDrawable(new M3D::Box(M3D::Vector3(1.0f,0.0f,0.0f), 0.05)));

	_xDragger->setupDefaultGeometry();
	_yDragger->setupDefaultGeometry();
	_zDragger->setupDefaultGeometry();

	/*_xzDragger->setupDefaultGeometry();
	_xyDragger->setupDefaultGeometry();
	_yzDragger->setupDefaultGeometry();*/
	{
		M3D::Quaternion rotation(M3D::Vector3(1.0f, 0.0f, 0.0f), M3D::Vector3(0.0f, 1.0f, 0.0f));
		_yDragger->SetRotation(rotation);
		/*M3D::Quaternion rotation2(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(.0f, 1.0f, 0.0f));
		_xyDragger->SetRotation(rotation2);*/
	}

	{
		M3D::Quaternion rotation(M3D::Vector3(1.0f, 0.0f, 0.0f), M3D::Vector3(0.0f, 0.0f, 1.0f));
		_zDragger->SetRotation(rotation);
		//M3D::Quaternion rotation2(M3D::Vector3(1.0f, 0.0f, .0f), M3D::Vector3(.0f, 1.0f, .0f));
		//_yzDragger->SetRotation(rotation2);
	}

	// Send different colors for each dragger.
	_xDragger->setColor(M3D::Color(1.0f, 0.0f, 0.0f, 1.0f));
	_yDragger->setColor(M3D::Color(0.0f, 1.0f, 0.0f, 1.0f));
	_zDragger->setColor(M3D::Color(0.0f, 0.0f, 1.0f, 1.0f));

	_xDragger->setPickColor(Color::YELLOW);
	_yDragger->setPickColor(Color::YELLOW);
	_zDragger->setPickColor(Color::YELLOW);
}

}
