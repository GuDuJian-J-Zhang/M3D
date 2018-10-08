#include "m3d/Handler/Translate1DHandler.h"
#include "m3d/utils/MeshHelper.h"
#include "m3d/action/RayPickAction.h"
namespace M3D
{
Translate1DHandler::Translate1DHandler() :
		Handler()
{
	this->SetType(SHAPE_AXIS_HANDLE);
	this->Init();
}

Translate1DHandler::~Translate1DHandler()
{

}

void Translate1DHandler::ComputeBox()
{
	m_BoundingBox.Clear();

	if (!m_BoundingBox.Defined())
	{
		BoundingBox tempBox;
		m_BoundingBox = tempBox;
		Mesh* meshData = this->GetMesh();
//		IVertexSet* meshData = this->GetData();
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

void Translate1DHandler::RayPick(RayPickAction* action)
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

void Translate1DHandler::FindVisiableObject(RenderAction* renderAction)
{
	this->SetRenderWorldMatrix(renderAction->GetGLWorldMatrix());
//	Color& drawColor = this->GetDrawColor();
//	this->setre
	this->SetRenderMesh(this->GetMesh());

	renderAction->PushRenderable(this, RenderableType::RGT_HANDLER);
}

void Translate1DHandler::OnTouchBegin(const TouchEvent& event)
{

}

void Translate1DHandler::OnTouchMove(const TouchEvent& event)
{

}

void Translate1DHandler::OnTouchEnd(const TouchEvent& event)
{

}

void Translate1DHandler::Init()
{
	Mesh* axisMesh = MeshHelper::GetArrowMesh();
	this->SetMesh(axisMesh);
}

}
