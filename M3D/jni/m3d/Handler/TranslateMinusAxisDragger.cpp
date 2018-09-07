

#include "m3d/handler/TranslateMinusAxisDragger.h"
#include "../scene/ShapeNode.h"
#include "m3d/base/Vector3.h"

using namespace M3D;

TranslateMinusAxisDragger::TranslateMinusAxisDragger() :CompositeDragger()
{
	_xDragger = new Translate1DDragger(M3D::Vector3(0.0, 0.0, 0.0), M3D::Vector3(0.0, 0.0, 1.0));
	_xDragger->AddRef();
	_xDragger->SetDraggerName(L"MinusXAxis");
	AddChild(_xDragger);
	addDragger(_xDragger);

	_nxDragger = new Translate1DDragger(M3D::Vector3(0.0, 0.0, 0.0), M3D::Vector3(0.0, 0.0, 1.0));
	_nxDragger->AddRef();
	_nxDragger->SetDraggerName(L"NMinusXAxis");
	AddChild(_nxDragger);
	addDragger(_nxDragger);

	setParentDragger(getParentDragger());
}

TranslateMinusAxisDragger::~TranslateMinusAxisDragger()
{
	ReleaseMe(_xDragger);
	ReleaseMe(_nxDragger);
}

/************************************************************************/
/* 设置之后方向		                                                    */
/************************************************************************/
void M3D::TranslateMinusAxisDragger::SetOrientation(int orientation)
{
	M3D::Quaternion rotation;
	M3D::Quaternion m_rotation;
	if (orientation == 1)
	{
		rotation.FromRotationTo(Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f));
		m_rotation.FromRotationTo(Vector3(0.0f, 0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f));
	}
	else if (orientation == 2)
	{
		rotation.FromRotationTo(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
		m_rotation.FromRotationTo(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f));
	}
	else if (orientation == 3)
	{
		rotation.FromRotationTo(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f));
		m_rotation.FromRotationTo(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f));
	}
	_xDragger->SetRotation(rotation);
	_nxDragger->SetRotation(m_rotation);

}
void M3D::TranslateMinusAxisDragger::SetOrientation(Vector3 direct) {
	M3D::Quaternion rotation;
	M3D::Quaternion m_rotation;
	rotation.FromRotationTo(Vector3(0.0f, 0.0f, 1.0f), direct);
	m_rotation.FromRotationTo(Vector3(0.0f, 0.0f, 1.0f), -direct);
	_xDragger->SetRotation(rotation);
	_nxDragger->SetRotation(m_rotation);
}
void TranslateMinusAxisDragger::setupDefaultGeometry()
{
	const float AxisAlpha = 1.0f;

	_xDragger->setupDefaultGeometry();
	_nxDragger->setupDefaultGeometry();
	
	// Rotate X-axis dragger appropriately.
	{
		M3D::Quaternion rotation(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(1.0f, 0.0f, 0.0f));
		_xDragger->SetRotation(rotation);	
	}

	// Rotate Y-axis dragger appropriately.
	{
		M3D::Quaternion rotation(M3D::Vector3(0.0f, 0.0f, 1.0f), M3D::Vector3(-1.0f, 0.0f, 0.0f));
		_nxDragger->SetRotation(rotation);
	}

	// Send different colors for each dragger.
	_xDragger->setColor(M3D::Color(1.0f, 0.0f, 0.0f, AxisAlpha));
	_nxDragger->setColor(M3D::Color(0.0f, 1.0f, 0.0f, AxisAlpha));

	

	_xDragger->setPickColor(Color::YELLOW);
	_nxDragger->setPickColor(Color::YELLOW);

	_xDragger->SetPreSelectColor(Color::YELLOW);
	_nxDragger->SetPreSelectColor(Color::YELLOW);
}

void M3D::TranslateMinusAxisDragger::SetXAxisVisible(bool val)
{
	_xDragger->SetVisible(val);
}

void M3D::TranslateMinusAxisDragger::SetMinusXAxisVisible(bool val)
{
	_nxDragger->SetVisible(val);
}



void M3D::TranslateMinusAxisDragger::SetScene(M3D::SceneManager* val)
{
	CompositeDragger::SetScene(val);
	_xDragger->SetScene(val);
	_nxDragger->SetScene(val);
}

