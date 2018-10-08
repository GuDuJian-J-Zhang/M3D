#include "m3d/Handler/Scale1DHandler.h"
#include "m3d/action/RayPickAction.h"
namespace M3D
{
Scale1DHandler::Scale1DHandler() :
		Handler()
{
	this->SetType(SHAPE_SCALE_HANDLE);
}

Scale1DHandler::~Scale1DHandler()
{

}

void Scale1DHandler::ComputeBox()
{

}

void Scale1DHandler::RayPick(RayPickAction* action)
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

void Scale1DHandler::FindVisiableObject(RenderAction* renderAction)
{

}

const  Color& Scale1DHandler::GetRenderColor()
{
	return Handler::GetRenderColor();
}

void Scale1DHandler::OnTouchBegin(const TouchEvent& event)
{

}

void Scale1DHandler::OnTouchMove(const TouchEvent& event)
{

}

void Scale1DHandler::OnTouchEnd(const TouchEvent& event)
{

}


}
