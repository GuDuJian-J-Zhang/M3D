#include "m3d/Handler/HandlerPoint.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/model/Point.h"
#include "m3d/action/RenderAction.h"
namespace M3D
{

HandlerPoint::HandlerPoint() :
		Handler()
{
	Vector3 pos;
	this->Init(pos,1.0f);
}

    HandlerPoint::HandlerPoint(const Vector3& vec,float size):Handler()
{
	this->Init(vec,size);
}

void HandlerPoint::Init(const Vector3& vec,float size)
{
	this->SetType(SHAPE_POINT_HANDLE);
    m_drawType = 3;
	this->m_point = new Point(vec);
	this->m_point->SetDrawType(m_drawType);
	this->m_point->SetSize(size);
}
    
void HandlerPoint::SetDrawType(int type)
{
    if(this->m_point){
        this->m_point->SetDrawType(type);
    }
}
    
const Vector3& HandlerPoint::GetPosition()
{
	return this->m_point->GetCoordinate();
}

HandlerPoint::~HandlerPoint()
{
	if(m_point)
	{
		delete m_point;
		m_point = NULL;
	}
}

Point* HandlerPoint::GetPoint()
{
	return this->m_point;
}

void HandlerPoint::ComputeBox()
{
	if(this->m_point)
	{
		this->SetBox(this->m_point->GetBoundingBox());
	}
}

void HandlerPoint::RayPick(RayPickAction* action)
{
	if(this->m_Visible)
	{
		if(this->m_point)
		{
			Vector3 intersectPos = this->m_point->GetCoordinate();

			if (action->Intersect(intersectPos))
			{
				action->AddIntersectPnt(intersectPos);
			}
		}
	}
}

void HandlerPoint::FindVisiableObject(RenderAction* renderAction)
{
	if(this->IsVisible())
	{
		this->SetRenderWorldMatrix(renderAction->GetGLWorldMatrix());
		if(this->m_point)
		{
			//更新显示数据
			this->m_point->UpdateRenderData(renderAction);
			//加入显示队列
			renderAction->PushRenderable(this, RenderableType::RGT_HANDLER);
		}
	}
}

const  Color& HandlerPoint::GetRenderColor()
{
	return Handler::GetRenderColor();
}

}
