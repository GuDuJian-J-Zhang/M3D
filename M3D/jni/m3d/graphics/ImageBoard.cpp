#include "m3d/graphics/ImageBoard.h"
#include "m3d/action/RayPickAction.h"

namespace M3D
{

ImageBoard::ImageBoard():Shape()
{
    uv_ = Rect(Vector2(0,0),Vector2(1,1)) ;
    this->m_texture = NULL;

	points.clear();
	textCoords.clear();

	m_bindBillboard.AllowRotate(false);
	m_bindBillboard.AllowTran(true);
	m_bindBillboard.AllowScale(false);
}

ImageBoard::ImageBoard(const Vector3& pos, const Vector2& size):Shape()
{
    uv_ = Rect(Vector2(0,0),Vector2(1,1)) ;

	points.clear();
	textCoords.clear();

    this->m_texture = NULL;


	m_bindBillboard.AllowRotate(false);
	m_bindBillboard.AllowTran(true);
	m_bindBillboard.AllowScale(false);

	Set(pos,size);
}

ImageBoard::~ImageBoard()
{
	if(this->m_texture)
	{
		this->m_texture->Release();
		this->m_texture = NULL;
	}
}

Vector2* ImageBoard::GetTextureCoords()
{
	if (textCoords.size() == 0)
	{
		MutexLock lock(m_mutex);
		Vector2 min = uv_.m_min;
		Vector2 max = uv_.m_max;

		//左上角
		textCoords.push_back(Vector2(min.m_x, max.m_y)); //2
		//右上角
		textCoords.push_back(Vector2(max.m_x, max.m_y)); //3
		//左下角
		textCoords.push_back(Vector2(min.m_x, min.m_y)); //0

		//左下角
		textCoords.push_back(Vector2(min.m_x, min.m_y)); //0
		//右上角
		textCoords.push_back(Vector2(max.m_x, max.m_y)); //3
		//右下角
		textCoords.push_back(Vector2(max.m_x, min.m_y)); //1

	}
	return textCoords.data();
}

Vector3* ImageBoard::GetVertexs()
{
	if (points.size() == 0)
	{
		MutexLock lock(m_mutex);
		float x = size_.m_x/2;
		float y = size_.m_y/2;

		//顺时针方向构建三角形
		// 左下角
		points.push_back(
				Vector3(m_position.m_x - x, m_position.m_y - y, m_position.m_z));
		//右下角
		points.push_back(
				Vector3(m_position.m_x + x, m_position.m_y - y, m_position.m_z));

		//左上角
		points.push_back(
				Vector3(m_position.m_x - x, m_position.m_y + y, m_position.m_z));

		//左上角
		points.push_back(
				Vector3(m_position.m_x - x, m_position.m_y + y, m_position.m_z));
		//右下角
		points.push_back(
				Vector3(m_position.m_x + x, m_position.m_y - y, m_position.m_z));

		//右上角
		points.push_back(
				Vector3(m_position.m_x + x, m_position.m_y + y, m_position.m_z));
	}
	return points.data();
}

void ImageBoard::ComputeBox()
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

void ImageBoard::RayPick(RayPickAction* action)
{
	vector<Vector3> intersects = this->GetIntersects(action);

	if(intersects.size() > 0)
	{
		for(int i=0;i< intersects.size();i++)
		{
			action->AddIntersectPnt(intersects[i]);
		}
		action->AddShape(this);
	}
}

vector<Vector3> ImageBoard::GetIntersects(RayPickAction* action)
{
	vector<Vector3> intersects;
	MutexLock lock(m_mutex);

	Ray billboardRay = action->GetData()->GetModelRay();
	Matrix3x4 modelMatrixInverse = m_bindBillboard.GetWorldMatrix().Inverse();
	billboardRay = billboardRay.Transformed(modelMatrixInverse);
//	LOGE(" SolidPlane::RayPick");
	Vector3* triData = points.data();
	Vector3 intersectPos;
	for (int i = 0; i < points.size() / 3; i++)
	{
		if (RayPickAction::ISintersectRayAndTriangle(triData[i*3],triData[i*3+1],triData[i*3+2],billboardRay,intersectPos))
		{
			intersectPos = m_bindBillboard.GetWorldMatrix()*intersectPos;
			intersects.push_back(intersectPos);
		}
	}

	return intersects;
}

void ImageBoard::UpdateRenderData(RenderAction* renderAction)
{
	//如果使用billboard效果，则进行矩阵校正
	MutexLock lock(m_mutex);
	Matrix4& glworldMatrix = m_bindBillboard.GetGLWorldMatrix(renderAction);

	this->SetRenderWorldMatrix(&glworldMatrix);
}

const Color& ImageBoard::GetRenderColor()
{
	const Color& imageBackColor = Color::WHITE;
	if(this->IsSelected())
	{
		return *Color::SelectColor;
	}
	return imageBackColor;
}

void ImageBoard::SetTexture(Texture* texture)
{
	if(this->m_texture)
	{
		this->m_texture->Release();
		this->m_texture = NULL;
	}
	this->m_texture = texture;
	if(this->m_texture)
	{
		this->m_texture->AddRef();
	}
}

Texture*  ImageBoard::GetTexture()
{
	return this->m_texture;
}

void ImageBoard::SetPlan(float* p)
{

}

void ImageBoard::Set(const Vector3& pos, const Vector2& size)
{
	MutexLock lock(m_mutex);
	m_bindBillboard.SetCenter(pos);
    m_position = pos;
    size_ = size;
	points.clear();
	textCoords.clear();
}

bool ImageBoard::SetPosition(const Vector3& pos)
{
	MutexLock lock(m_mutex);

	m_bindBillboard.SetCenter(pos);
	m_position = pos;
	points.clear();

	return true;
}

Vector3& ImageBoard::GetPosition()
{
	return this->m_position;
}

void ImageBoard::AllowTran(bool allow)
{
	return this->m_bindBillboard.AllowTran(allow);
}

void ImageBoard::AllowRotate(bool allow)
{
	return this->m_bindBillboard.AllowRotate(allow);
}

void ImageBoard::AllowScale(bool allow)
{
	return this->m_bindBillboard.AllowScale(allow);
}

bool ImageBoard::IsAllowTran()
{
	return this->m_bindBillboard.IsAllowTran();
}

bool ImageBoard::IsAllowRotate()
{
	return this->m_bindBillboard.GetRotate();
}

bool ImageBoard::IsAllowScale()
{
	return this->m_bindBillboard.GetScale();
}

}
