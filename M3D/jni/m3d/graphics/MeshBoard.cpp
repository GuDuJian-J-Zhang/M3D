#include "m3d/graphics/MeshBoard.h"
#include "m3d/action/RayPickAction.h"

namespace M3D
{

MeshBoard::MeshBoard():Shape()
{
//    uv_ = Rect(Vector2(0,0),Vector2(1,1)) ;

	points.clear();

	m_bindBillboard.AllowRotate(false);
	m_bindBillboard.AllowTran(true);
	m_bindBillboard.AllowScale(false);
}

MeshBoard::MeshBoard(const Vector3& pos, const Vector2& size):Shape()
{
//    uv_ = Rect(Vector2(0,0),Vector2(1,1)) ;
	points.clear();

	m_bindBillboard.AllowRotate(false);
	m_bindBillboard.AllowTran(true);
	m_bindBillboard.AllowScale(false);

	Set(pos,size);
}

MeshBoard::~MeshBoard()
{

}

Vector3* MeshBoard::GetLinesVertexs()
{
	if (points.size() == 0)
	{
		MutexLock lock(m_mutex);
		float x = size_.m_x/2;
		float y = size_.m_y/2;

		//构建一个十字架
		points.push_back(
						Vector3(m_position.m_x, m_position.m_y - y, m_position.m_z));
		points.push_back(
						Vector3(m_position.m_x, m_position.m_y + y, m_position.m_z));

		points.push_back(
						Vector3(m_position.m_x - x, m_position.m_y, m_position.m_z));
		points.push_back(
						Vector3(m_position.m_x + x, m_position.m_y, m_position.m_z));
	}
	return points.data();
}

void MeshBoard::ComputeBox()
{
	m_BoundingBox.Clear();
	if(points.size() > 0)
	{
		//第一个点
		this->m_BoundingBox.Merge(points[0]);
		//最后一个点
		this->m_BoundingBox.Merge(points[points.size() -1]);
	}
}

void MeshBoard::RayPick(RayPickAction* action)
{
//	MutexLock lock(m_mutex);
//
//	Ray billboardRay = action->GetData()->GetModelRay();
//	Matrix3x4 modelMatrixInverse = m_bindBillboard.GetWorldMatrix().Inverse();
//	billboardRay = billboardRay.Transformed(modelMatrixInverse);
	//LOGE(" SolidPlane::RayPick");
//	Vector3* triData = points.data();
//	Vector3 intersectPos;
//	for (int i = 0; i < points.size() / 3; i++)
//	{
//		if (RayPickAction::ISintersectRayAndTriangle(triData[i*3],triData[i*3+1],triData[i*3+2],billboardRay,intersectPos))
//		{
//			intersectPos = (*this->getGLWorldMatrix())*intersectPos;
//
//			action->AddIntersectPnt(intersectPos);
//		}
//	}
}

void MeshBoard::UpdateRenderData(RenderAction* renderAction)
{
	//如果使用billboard效果，则进行矩阵校正
	MutexLock lock(m_mutex);
	Matrix4& glworldMatrix = m_bindBillboard.GetGLWorldMatrix(renderAction);

	this->SetRenderWorldMatrix(&glworldMatrix);
}

void MeshBoard::Set(const Vector3& pos, const Vector2& size)
{
	MutexLock lock(m_mutex);
	m_bindBillboard.SetCenter(pos);
    m_position = pos;
    size_ = size;
	points.clear();
}

}
