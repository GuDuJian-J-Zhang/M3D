#include "m3d/model/Edge.h"
#include "m3d/model/Curve.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/ResourceManager.h"
#include "sview/views/Parameters.h"
#include "m3d/utils/M3DTools.h"
#include "m3d/action/RenderAction.h"
#include "m3d/model/ExtendInfoManager.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/model/body.h"
#include "m3d/model/Model.h"

namespace M3D
{
Edge::Edge() :Object()
{
	m_visible = true;
	m_IsHighlight = false;
	m_lineSet = NULL;
	m_IsSelect= false; ///是否选中
	m_IsDotted = false; 

	face = NULL; //!< 上级面
	body = NULL; //!<上级Body

	this->m_Id = OBJID++;
	this->SetInitColor(*Color::EdgeDefaultColor);
	this->SetType(SHAPE_EDGE);
	m_svlId = OBJID++;
	m_bNeedClip = true;
	m_IsFirstGetProperties = true;
	this->m_selectable = true;
}

Edge::Edge(const Edge& orig)
{
	this->m_selectable = true;
	m_bNeedClip = true;
	*this = orig;
}

Edge::~Edge()
{
	if(this->m_lineSet)
	this->m_lineSet->Release();
	this->m_lineSet = NULL;
}

void Edge::AddData(RefPolyLine * m_lineSet, int Level)
{
	switch (Level)
	{
	case 0:
		this->m_lineSet = m_lineSet;
		this->m_lineSet->AddRef();
		break;
	default:
		break;
	}
}

Edge& Edge::operator=(const Edge& orig)
{
	if (this != &orig)
	{
		//Shape::operator =(orig);
		this->SetColor(*Color::EdgeDefaultColor);
		this->m_Id = OBJID++;
		this->m_lineSet = orig.m_lineSet;
		if(this->m_lineSet)
		this->m_lineSet->AddRef();
		this->SetSVLId(orig.m_svlId); 
		this->m_IsSelect = orig.m_IsSelect;
		this->m_visible = orig.m_visible;
		this->m_IsHighlight = orig.m_IsHighlight;
		this->m_selectable = orig.m_selectable;
		this->m_bNeedClip = orig.m_bNeedClip;
		this->m_IsDotted = orig.m_IsDotted;
	}
	return *this;
}

RefPolyLine* Edge::GetLineData()
{
	return this->m_lineSet;
}

void Edge::SetFace(Face* face)
{
	this->face = face;
}

Face* Edge::GetFace()
{
	return this->face;
}

void Edge::SetBody(Body* body)
{
	this->body = body;
}

Body* Edge::GetBody()
{
	return this->body;
}


void Edge::Restore()
{
	ResetColor();
	SetVisible(this->IsVisible());
	SetSelected(false);
}

void Edge::Restore(bool resub)
{
	Restore();
}

void Edge::SetAlpha(float a)
{
	m_Color.m_a = a;
}

void Edge::SetAlpha(float a, bool relSub)
{
	SetAlpha(a);
}

/**
* 设置颜色为初始颜色
*/
void Edge::ResetColor()
{
	this->SetColor(*Color::EdgeDefaultColor);
	//m_Color = m_InitColor;
	//if (m_FaceExtInfo)
	//{
		//m_Color = m_FaceExtInfo->m_InitColor;
	//}
}

M3D::SHAPETYPE Edge::GetType(void)
{
	return SHAPETYPE::SHAPE_EDGE;
}

void Edge::SetInitAlpha(float alpha)
{
	m_Color.m_a = alpha;
	//	ResetColor();
}

void Edge::SetInitHightlight(bool isHighlight)
{

}

bool Edge::IsHightlight()
{
	return this->m_IsSelect;
}

void Edge::SetInitColor(const Color &color)
{
	//m_InitColor = color;
	this->SetColor(color);
}

void Edge::ResetAlpha()
{
	this->m_Color.m_a = 1.0f;
}

void Edge::Selectable(bool selectable)
{
	m_selectable = selectable;
}


bool Edge::Selectable()
{
	return this->m_selectable;
}


void Edge::SetVisible(bool visible, bool relSub)
{
	SetVisible(visible);
}

void Edge::ComputeBox()
{

}

void Edge::SetColor(const Color& color, bool reSub)
{
	SetColor(color);
}


string Edge::GetGeometryInfo()
{
	return "";
}

void Edge::FindVisiableObject(RenderAction* renderAction)
{

}

void Edge::Traverse(Action* action)
{

}

void Edge::SetColor(const Color &color)
{
	this->m_Color = color;
}

void Edge::SetVisible(bool visible)
{
	this->m_visible = visible;
}

void Edge::SetType(SHAPETYPE type)
{
	//this->m_Type = type;
}

void Edge::SetSelected(bool select)
{
	this->m_IsSelect = select;

}

Color* Edge::GetDrawColor()
{
	if (IsSelected())
	{
		//LOGE("shape is selected id:: %d",(int)this);
		return Color::SelectColor;
	}
	else
		return &m_Color;
}

Color* Edge::GetColor()
{
	return &this->m_Color;
}


float Edge::GetAlpha()
{
	return m_Color.m_a;
}
bool Edge::IsSelected() const
{
	return m_IsSelect;
}

void Edge::SetBox(const BoundingBox& box)
{
	//m_BoundingBox = box;
}

bool Edge::IsVisible()
{
	return this->m_visible;
}


bool Edge::IsOrigVisible()
{
	return true;
}


void Edge::SetOrigVisible(bool visible)
{
	//m_origVisible = visible;

	this->SetVisible(visible);
}

void Edge::SetOrigVisible(bool visible, bool relSub)
{
	this->SetVisible(visible);
}

BoundingBox& Edge::GetBoundingBox()
{
	if (m_lineSet)
	{
		return m_lineSet->GetBoundingBox();
	}
	return BoundingBox::NO_BOX;
}

string Edge::GetProperties()
{
	if (m_IsFirstGetProperties)
	{
		InitProperties();
		m_IsFirstGetProperties = false;
	}
	return m_Properties;
}

void Edge::ClearProperties()
{
	m_Properties.clear();
}

/**
* 获取id
* @return
*/
IDTYPE Edge::GetID()
{
	return this->m_Id;
}

/**
* 设置Id
* @param value
*/
void Edge::SetID(IDTYPE  value)
{
	this->m_Id = value;
}

std::string Edge::GetName()
{
	return "";
}

void Edge::SetName(const std::string& value)
{
	//this->m_Name = value;
}

/**
* 获取初始颜色
* @return
*/
Color* Edge::GetInitColor()
{
	return NULL;
}


void Edge::SetSceneNode(SceneNode* node)
{
	//this->node = node;
}

SceneNode* Edge::GetSceneNode()
{
	return NULL;
}

void Edge::SetMaterial(BaseMaterial* material)
{

}

BaseMaterial* Edge::GetMaterial()
{
	return NULL;
}

bool Edge::AllowExculding()
{
	return true;
}

void Edge::SetAlloewExculding(bool allow)
{
	//this->m_allowExcluding = allow;
}

bool Edge::RendreVisible()
{
	return true;
}

void Edge::SetRenderVisible(bool visible)
{
	//this->m_renderVisible = visible;
}

float Edge::GetVolumeAndArea(float& volume, float& area)
{
	return 0.0f;
}

void Edge::AddProperty(string key, string value)
{
	if (m_Properties.find(key + "::") != string::npos)
	{
	LOGI("find properties");
	return;
	}
	if (!m_Properties.empty())
	m_Properties.append(";;");
	m_Properties.append(key + "::" + value);
}

void Edge::AddRef(void)
{
	Object::AddRef();
}

void Edge::Release(void)
{
	Object::Release();
}

int Edge::GetSVLId()
{
	return this->m_svlId;
}

void Edge::SetSVLId(int Id)
{
	this->m_svlId = Id;
}

GeometryAttribute* Edge::GetGeoAttribute()
{
	if (m_lineSet)
	{
		GeometryAttribute* geo = m_lineSet->GetGeoAttribute();
		if (geo == NULL && this->GetSVLId())
		{
			if (this->GetBody())
			{
				if (this->GetBody()->GetModel())
				{
					geo = this->GetBody()->GetModel()->GetExtendInfoManager()->GetEdgeGeoAttribute(
							this->GetBody()->GetModel()->GetProtoTypeId(), this->GetSVLId());
				}
			}
			m_lineSet->SetGeoAttribute(geo);
		}
		return geo;
	}
	return NULL;
}

void Edge::RayPick(RayPickAction* action)
{
	if (!this->IsVisible() || !this->m_selectable || !action->CanPickShape(this->GetType()))
	{
		return;
	}

	if(m_lineSet)
	{
		GeometryAttribute* geo = this->GetGeoAttribute();
		if (geo != NULL)
		{
			if (!action->CanPickGeo(geo->GetGeoAttrType()))
			{
				//LOGE("GEO TYPE IS %d",geo->GetGeoAttrType()+SHAPE__GEOATTR_TYPE_OFFSET);
				return;
			}
		}
		else
		{
			//几何属性为空，则赋值默认几何属性为 M3D_GEOATTR_TYPE_UNKNOWN
			if (!action->CanPickGeo(M3D_GEOATTR_TYPE_UNKNOWN))
			{
				//LOGE("GEO TYPE IS %d",geo->GetGeoAttrType()+SHAPE__GEOATTR_TYPE_OFFSET);
				return;
			}

		}
		m_lineSet->RayPick(action);

		action->AddShape(this);
	}
}

void Edge::FramePick(RayPickAction* action)
{

}

string Edge::GetGeoInfo()
{
	string geoInfo;
	GeometryAttribute* geo = this->GetGeoAttribute();
	if (geo != NULL)
	{

	}
	return geoInfo;
}

const Color&  Edge::GetRenderColor()
{
	return *this->GetDrawColor();
}

void Edge::InitProperties()
{
	InternationalManager language;
	language.SetLanguage(SVIEW::Parameters::Instance()->m_internationalLanguage);
	string nameStr =language.GetCurrentTypeValue("EdgePropertiesName");
	string ID = language.GetCurrentTypeValue("EdgePropertiesID");
	string Color =language.GetCurrentTypeValue("EdgePropertiesColor");
	string LOD0PointNumber = language.GetCurrentTypeValue("EdgePropertiesLOD0PointNumber");
	string LOD1PointNumber =language.GetCurrentTypeValue("EdgePropertiesLOD1PointNumber");
	//AddProperty(nameStr, m_Name);
	AddProperty(ID, IntToString(m_Id));

	string colorStr = M3DTOOLS::floatToString(m_Color.m_r)+","+M3DTOOLS::floatToString(m_Color.m_g)+","
			+M3DTOOLS::floatToString(m_Color.m_b)+","+M3DTOOLS::floatToString(m_Color.m_a);
	AddProperty(Color,colorStr);

	//AddProperty(LOD0PointNumber, IntToString(GetEdgePntCount(0)));
	//AddProperty(LOD1PointNumber, IntToString(GetEdgePntCount(1)));
}

void Edge::SetNeedClip(bool val)
{
	m_bNeedClip = val;
}

bool Edge::GetNeedClip()
{
	return m_bNeedClip;
}

void Edge::SetDotted(bool val)
{
	m_IsDotted = val;
}
bool Edge::GetDotted()
{
	return m_IsDotted;
}
}
