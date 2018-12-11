

#include "m3d/handler/TranslateMinusAxisDragger.h"
#include "../scene/ShapeNode.h"
#include "m3d/base/Vector3.h"
#include "m3d/SceneManager.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/scene/SectionNode.h"
#include "m3d/graphics/Section.h"
#include "m3d/graphics/SectionPlane.h"
#include "m3d/action/RayPickAction.h"

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

bool M3D::TranslateMinusAxisDragger::handle(const TouchEvent& ea)
{
	if (ea.getHandled()) return false;
	SVIEW::View* view = ea.GetView();
	if (!view) return false;

	bool handled = false;
	//if (_draggerActive)
	{
		switch (ea.getEventType())
		{
		case TouchEvent::PUSH:
		{
			_pointer.reset();
			Section* pSection = _scene->GetSectionNode()->GetSection();
			IShape* selectedShape = _scene->GetPickShape(ea.getX(), ea.getY(), SHAPE_MODEL, 0);
			SectionPlane* section = dynamic_cast<SectionPlane*>(selectedShape);
			if (section && section->GetID() == GetID() && section->IsSelected())
			{
				CameraNode *rootCamera = view->GetSceneManager()->GetCamera();
				_pointer.setCamera(rootCamera);
				_pointer.setMousePosition(ea.getX(), ea.getY());

				_xDragger->handle(_pointer, ea);
				_xDragger->setDraggerActive(true);
				handled = true;
			}
		}
		case TouchEvent::DRAG:
		case TouchEvent::RELEASE:
		{
			if (_xDragger->getDraggerActive())//(_draggerActive)
			{
				//_pointer._hitIter = _pointer._hitList.begin();
				_pointer.setMousePosition(ea.getX(), ea.getY());

				_xDragger->handle(_pointer, ea);

				handled = true;
			}
			break;
		}
		default:
			break;
		}

		if ((_xDragger->getDraggerActive()) && ea.getEventType() == TouchEvent::RELEASE)
		{
			_xDragger->setDraggerActive(false);
			_pointer.reset();
		}
	}
	if (handled)
	{
		ea.setHandled(handled);
	}
	return handled;
}
