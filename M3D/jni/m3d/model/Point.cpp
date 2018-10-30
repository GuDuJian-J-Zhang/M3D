#include "m3d/model/Point.h"
#include "m3d/action/RayPickAction.h"
#include "sview/views/Parameters.h"
#include "m3d/graphics/ImageBoard.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/MeshBoard.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/ResourceManager.h"
#include "m3d/action/RenderAction.h"
using namespace SVIEW;

namespace M3D
{

float Point::BOX_LENGTH = 0.1;

//const string Point::defaultPath = "/data/pic/pointx.png";
//
//const string Point::defaultPathPointO = "/data/pic/pointo.png";
//
//const string Point::defaultPathPointr2 = "/data/pic/pointr2.png";
//
//Texture* Point::defaultTexture = NULL;
//
//Texture* Point::defaultTexturePointO = NULL;
//
//Texture* Point::defaultTexturePointr2 = NULL;
//
//Texture* Point::GetDefaultPointTexture(int type)
//{
//	Texture* texture = NULL;
//	if (type == 1)
//	{
//		if (!defaultTexturePointO)
//		{
//			defaultTexturePointO = new Texture2D;
//
//			if (defaultTexturePointO
//					&& defaultTexturePointO->GetType() == Texture::TEXTURE_2D)
//			{
//				Texture2D* texture2d = (Texture2D*) defaultTexturePointO;
//				texture2d->SetImagePath(
//						Parameters::Instance()->m_appWorkPath
//								+ defaultPathPointO);
//			}
//		}
//		texture = defaultTexturePointO;
//	}
//	else if (type == 2)
//	{
//		if (!defaultTexture)
//		{
//			defaultTexture = new Texture2D;
//
//			if (defaultTexture
//					&& defaultTexture->GetType() == Texture::TEXTURE_2D)
//			{
//				Texture2D* texture2d = (Texture2D*) defaultTexture;
//				texture2d->SetImagePath(
//						Parameters::Instance()->m_appWorkPath + defaultPath);
//			}
//		}
//		texture = defaultTexture;
//	}
//	else if (type == 3)
//	{
//		if(!defaultTexturePointr2)
//		{
//			defaultTexturePointr2 = new Texture2D;
//
//			if(defaultTexturePointr2 && defaultTexturePointr2->GetType() == Texture::TEXTURE_2D)
//			{
//				Texture2D* texture2d = (Texture2D*)defaultTexturePointr2;
//				texture2d->SetImagePath(Parameters::Instance()->m_appWorkPath + defaultPathPointr2);
//			}
//		}
//		texture = defaultTexturePointr2;
//	}
//
//	return texture;
//}

Point::Point() :Shape()
{
	Vector3 coord;
	this->Init(coord);
}

Point::Point(const Point& orig) :
		Shape(orig)
{
	*this = orig;
}

Point& Point::operator=(const Point& orig)
{
	if (this != &orig)
	{
		Shape::operator =(orig);
	}
	return *this;
}

Point::Point(const Vector3& coord) :Shape()
{
	this->Init(coord);
}

Point::~Point()
{
	if(m_pointImage)
	{
		delete m_pointImage;
		this->m_pointImage = NULL;
	}

	if(m_meshboard)
	{
		delete m_meshboard;
		this->m_meshboard = NULL;
	}
}

void Point::Init(const Vector3& coord)
{
	this->SetType(SHAPE_POINT);
	this->SetCoordinate(coord);
	m_BoundingBox.Clear();
	this->m_pointImage = NULL;
	this->m_meshboard = NULL;
	m_drawType = 0;
	this->SetSize(0.8f);
}

void Point::SetCoordinate(const Vector3& coordinate)
{
	this->m_coordinate = coordinate;
}

Vector3& Point::GetCoordinate()
{
	return this->m_coordinate;
}

void Point::ComputeBox()
{
	m_BoundingBox.Clear();

	if (!m_BoundingBox.Defined())
	{
		//根据当前坐标值和指定的偏移量 确定最大值和最小值
		Vector3 _max(this->m_coordinate.m_x + BOX_LENGTH,
				this->m_coordinate.m_y + BOX_LENGTH,
				this->m_coordinate.m_z + BOX_LENGTH);
		Vector3 _min(this->m_coordinate.m_x - BOX_LENGTH,
				this->m_coordinate.m_y - BOX_LENGTH,
				this->m_coordinate.m_z - BOX_LENGTH);

		m_BoundingBox = BoundingBox(_min,_max);
	}
}

void Point::RayPick(RayPickAction* action)
{
	if (!this->IsVisible() || !action->CanPickShape(this->GetType()))
	{
		return;
	}
	//判断是否有交点
	if (action->Intersect(this->GetCoordinate()))
	{
		//如果有交点，则将交点加入到拾取动作中。
		action->AddIntersectPnt(this->GetCoordinate());
//		LOGE("point picked");
		action->AddShape(this);
	}
}

void Point::FindVisiableObject(RenderAction* renderAction)
{
	if(this->IsVisible())
	{
		this->UpdateRenderData(renderAction);
		renderAction->PreapareRenderPoint(this);
	}
}

void Point::UpdateRenderData(RenderAction* renderAction)
{
	this->SetRenderWorldMatrix(renderAction->GetGLWorldMatrix());
	//如果使用billboard效果，则进行矩阵校正
	int drawType = this->m_drawType ;
	if ( drawType == 1 || drawType == 2 || drawType == 3)
	{
		if (!m_pointImage)
		{
			Vector2 pointSize(this->m_size, this->m_size);
			//pointSize = ShapeHelper::GetCommonSize(renderAction->GetScene(), pointSize);
			m_pointImage = new ImageBoard(this->GetCoordinate(), pointSize);
			m_pointImage->SetTexture(renderAction->GetScene()->GetResourceManager()->GetDefaultPointTexture(this->m_drawType));
		}

		m_pointImage->UpdateRenderData(renderAction);
	}
	else if (drawType == 0)
	{
		if (!m_meshboard)
		{
			Vector2 pointSize(this->m_size, this->m_size);
			//ShapeHelper::GetCommonSize(renderAction->GetScene(), pointSize);
			m_meshboard = new MeshBoard(this->GetCoordinate(), pointSize);
		}

		m_meshboard->UpdateRenderData(renderAction);
	}

	this->SetFrontShow(this->IsSelected());
}

void Point::SetBody(Body* body)
{
	this->m_body = body;
}

Body* Point::GetBody()
{
	return this->m_body;
}

const Color&  Point::GetRenderColor()
{
	return *this->GetDrawColor();
}

const Color & Point::GetShapeColor()
{
	return *this->GetColor();
}

ImageBoard* Point::GetImageboard()
{
	return m_pointImage;
}

MeshBoard* Point::GetMeshBoard()
{
	 return m_meshboard;
}

void Point::SetDrawType(int type)
{
	this->m_drawType = type;
}

void Point::SetSize(float size)
{
	this->m_size = size/4;
}

float Point::GetSize()
{
	return this->m_size*4;
}

int Point::GetDrawType()
{
	return this->m_drawType;
}
    
void Point::SetFrontShow(bool frontShow)
{
    Renderable::SetFrontShow(frontShow);
    
    if(this->m_pointImage){
        this->m_pointImage->SetFrontShow(frontShow);
    }
}
    
}
