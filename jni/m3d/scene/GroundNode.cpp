#include "m3d/M3D.h"
#include "m3d/model/Shape.h"
#include "m3d/scene/GroundNode.h"

#include "m3d/ResourceManager.h"

#include "m3d/model/Image.h"
#include "m3d/graphics/Texture.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/action/RenderAction.h"
namespace M3D
{

	GroundNode::GroundNode()
	{
		m_isDirty = true;
		m_size = 10;
		m_divisions = 10;
		m_center = Vector3::ZERO;
	}

	GroundNode::~GroundNode()
	{

	}

	int GroundNode::GetType(void)
	{
		return SCENEGROUND_NODE;
	}

	void GroundNode::FindVisiableObject(RenderAction* renderAction)
	{
		if (!this->IsVisible())
		{
			renderAction->SetGroundNode(NULL);
		}
		else
		{
			renderAction->SetGroundNode(this);
		}
	}

	void GroundNode::SetGroundSize(int size, int divisions,Vector3 center)
	{
		if (m_size != size)
		{
			m_size = size;
			m_isDirty = true;
		}

		if (m_divisions != divisions)
		{
			m_divisions = divisions;
			m_isDirty = true;
		}

		m_center = center;
	}

	void GroundNode::SetColor(const Color& color1, const Color& color2)
	{

	}

	Color* GroundNode::GetColors()
	{
		return &colors[0];
	}

	Vector3* GroundNode::GetVertexs()
	{
		if (m_isDirty)
		{
			Initial();
			m_isDirty = false;
		}

		return &points[0];
	}

	void GroundNode::SetResourceManager(ResourceManager * resMgr)
	{
		this->m_resMgr = resMgr;
	}

	int GroundNode::GetPointsNumber()
	{
		return points.size();
	}

	void GroundNode::Initial()
	{
		int step = ceil(m_size / (float)m_divisions);
		int tempSize = step*m_divisions;
		int center = m_divisions / 2;
		
		int halfSize = tempSize / 2;
		Color color1(0xffffff);
		Color color2(0x555555);
		points.clear();
		colors.clear();
		for (int i = 0,j = 0,k = -halfSize;i<=m_divisions;i++,k+=step)
		{
			points.push_back(Vector3(-halfSize, 0, k) + m_center);
			points.push_back(Vector3(halfSize, 0, k) + m_center);

			points.push_back(Vector3(k, 0, -halfSize) + m_center);
			points.push_back(Vector3(k, 0, halfSize) + m_center);

			Color color = i == center ? color1 : color2;
			colors.push_back(color);
			colors.push_back(color);
			colors.push_back(color);
			colors.push_back(color);
		}
	}

}