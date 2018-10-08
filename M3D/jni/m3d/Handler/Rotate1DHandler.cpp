#include "m3d/Handler/Rotate1DHandler.h"
#include "m3d/action/RayPickAction.h"
namespace M3D
{
Rotate1DHandler::Rotate1DHandler() :
		Handler()
{
	this->SetType(SHAPE_ROTATE_HANDLE);
}

Rotate1DHandler::~Rotate1DHandler()
{

}


void Rotate1DHandler::ComputeBox()
{

}

void Rotate1DHandler::RayPick(RayPickAction* action)
{
	if (!this->IsVisible() || !action->CanPickShape(this->GetType()))
	{
		return;
	}

	if (action->Intersect(GetBoundingBox()))
	{
		Mesh* meshData = this->GetMesh();
		meshData->RayPick(action);
		action->AddShape(this);
	}
}

void Rotate1DHandler::FindVisiableObject(RenderAction* renderAction)
{

}

void Rotate1DHandler::OnTouchBegin(const TouchEvent& event)
{

}

void Rotate1DHandler::OnTouchMove(const TouchEvent& event)
{

}

void Rotate1DHandler::OnTouchEnd(const TouchEvent& event)
{

}

const  Color& Rotate1DHandler::GetRenderColor()
{
	return Handler::GetRenderColor();
}


}
