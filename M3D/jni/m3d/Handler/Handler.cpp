#include "m3d/Handler/Handler.h"
#include "m3d/base/Matrix3x4.h"
#include "m3d/scene/SceneNode.h"

namespace M3D
{
Handler::Handler() :
		Shape()
{
	this->SetAlloewExculding(false);
	this->SetType(SHAPE_HANDLE);
	this->SetMesh(NULL);
}

Handler::~Handler()
{
	this->SetMesh(NULL);
}

bool Handler::OnTouch(const TouchEvent& event)
{
    return true;
}


void Handler::ComputeBox()
{
	m_BoundingBox.Clear();

	if (!m_BoundingBox.Defined())
	{
		BoundingBox tempBox;
		m_BoundingBox = tempBox;

		IVertexSet* meshData = this->m_Mesh;

		if (meshData != NULL)
		{
			if (meshData->GetBoundingBox().Defined())
			{
				tempBox = meshData->GetBoundingBox();
				m_BoundingBox = tempBox;
			}
		}
	}
}

void Handler::RayPick(RayPickAction* action)
{

}

Color& Handler::GetDrawColor()
{
	if (IsSelected())
	{
		//LOGE("shape is selected id:: %d",(int)this);
		Color selectedColor = Color::YELLOW;
		return selectedColor;
	}
	else
		return m_Color;
}

const  Color& Handler::GetRenderColor()
{
	return this->GetDrawColor();
}

void Handler::FindVisiableObject(RenderAction* renderAction)
{

}

void Handler::OnTouchBegin(const TouchEvent& event)
{

}

void Handler::OnTouchMove(const TouchEvent& event)
{

}

void Handler::OnTouchEnd(const TouchEvent& event)
{

}

void Handler::SetMesh(Mesh* mesh)
{
	this->m_Mesh = mesh;
}

Mesh* Handler::GetMesh()
{
	return this->m_Mesh;
}

Matrix3x4 Handler::GetWorldTransform()
{
	SceneNode* parentNode = this->GetSceneNode();
	Matrix3x4 worldMatrix = this->ComputeTransform();

	if(parentNode)
	{
		Matrix3x4& parentWorldMatrix = this->GetSceneNode()->GetWorldTransform();
		worldMatrix = parentWorldMatrix*worldMatrix;
	}

	return worldMatrix;
}

Matrix3x4 Handler::ComputeTransform()
{
	return Matrix3x4::IDENTITY;
}

}
