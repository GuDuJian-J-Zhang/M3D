
#include "m3d/base/Viewport.h"
#include "m3d/graphics/CameraNode.h"

namespace M3D
{
/// Construct with defaults.
Viewport::Viewport():m_camera(NULL),
		m_rect(IntRect::ZERO)
{

}

Viewport::Viewport(CameraNode* camera,const IntRect& rect) :
    m_camera(camera),
    m_rect(rect)
{

}

Viewport::~Viewport()
{
}

void Viewport::SetCamera(CameraNode* camera)
{
    m_camera = camera;
}

void Viewport::SetRect(const IntRect& rect)
{
    m_rect = rect;
}

CameraNode* Viewport::GetCamera() const
{
    return m_camera;
}

Ray Viewport::GetScreenRay(int x, int y) const
{
    if (!m_camera)
        return Ray();

    float screenX= 0;
    float screenY= 0;

    if (m_rect == IntRect::ZERO)
    {
//        Graphics* graphics = GetSubsystem<Graphics>();
//        screenX = (float)x / (float)graphics->GetWidth();
//        screenY = (float)y / (float)graphics->GetHeight();
    }
    else
    {
        screenX = float(x - m_rect.m_left) / (float)m_rect.Width();
        screenY = float(y - m_rect.m_top) / (float)m_rect.Height();
    }

    return m_camera->GetScreenRay(screenX, screenY);
}

IntVector2 Viewport::WorldToScreenPoint(const Vector3& worldPos) const
{
    if (!m_camera)
    {
        return IntVector2::ZERO;
    }


    Vector2 screenPoint = m_camera->WorldToScreenPoint(worldPos);
    int x= 0;
    int y= 0;
    if (m_rect == IntRect::ZERO)
    {
//        Graphics* graphics = GetSubsystem<Graphics>();
//        x = (int)(screenPoint.x_ * graphics->GetWidth());
//        y = (int)(screenPoint.y_ * graphics->GetHeight());
    }
    else
    {
        x = (int)(m_rect.m_left + screenPoint.m_x * m_rect.Width());
        y = (int)(m_rect.m_top + screenPoint.m_y * m_rect.Height());
    }
    
    return IntVector2(x, y);
}

Vector3 Viewport::ScreenToWorldPoint(int x, int y, float depth) const
{
    if (!m_camera)
        return Vector3::ZERO;

    float screenX = 0;
    float screenY = 0;

    if (m_rect == IntRect::ZERO)
    {
//        Graphics* graphics = GetSubsystem<Graphics>();
//        screenX = (float)x / (float)graphics->GetWidth();
//        screenY = (float)y / (float)graphics->GetHeight();
    }
    else
    {
        screenX = float(x - m_rect.m_left) / (float)m_rect.Width();
        screenY = float(y - m_rect.m_top) / (float)m_rect.Height();
    }

    return m_camera->ScreenToWorldPoint(Vector3(screenX, screenY, depth));
}

Vector3 Viewport::ScreenToWorldPoint(const Vector2& scrPnt, float depth) const
{
	if (!m_camera)
		return Vector3::ZERO;

	float screenX = 0;
	float screenY = 0;

	if (m_rect == IntRect::ZERO)
	{
		//        Graphics* graphics = GetSubsystem<Graphics>();
		//        screenX = (float)x / (float)graphics->GetWidth();
		//        screenY = (float)y / (float)graphics->GetHeight();
	}
	else
	{
		screenX = float(scrPnt.m_x - m_rect.m_left) / (float)m_rect.Width();
		screenY = float(scrPnt.m_y - m_rect.m_top) / (float)m_rect.Height();
	}

	return m_camera->ScreenToWorldPoint(Vector3(screenX, screenY, depth));
}

}
