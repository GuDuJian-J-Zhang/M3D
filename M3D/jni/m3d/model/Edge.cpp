#include "m3d/model/Edge.h"
#include "m3d/model/Curve.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/action/RayPickAction.h"
#include "m3d/ResourceManager.h"
#include "sview/views/Parameters.h"
#include "m3d/utils/M3DTools.h"

namespace M3D
{
Edge::Edge() :
		Shape()
{
	this->SetColor(*Color::EdgeDefaultColor);
	this->SetType(SHAPE_EDGE);
}

Edge::Edge(const Edge& orig)
{
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
		Shape::operator =(orig);
		this->SetColor(*Color::EdgeDefaultColor);

		this->m_lineSet = orig.m_lineSet;
		if(this->m_lineSet)
		this->m_lineSet->AddRef();
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

void Edge::ComputeBox()
{
	this->m_BoundingBox.Clear();
	if(m_lineSet)
	{
		this->SetBox(m_lineSet->GetBoundingBox());
	}
}

void Edge::RayPick(RayPickAction* action)
{
	if (!this->IsVisible() || !action->CanPickShape(this->GetType()))
	{
		return;
	}

	if(m_lineSet)
	{
		GeometryAttribute* geo = m_lineSet->GetGeoAttribute();
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

string Edge::GetGeoInfo()
{
	string geoInfo;
	GeometryAttribute* geo = m_lineSet->GetGeoAttribute();
	if (geo != NULL)
	{

	}
	return geoInfo;
}

const Color&  Edge::GetRenderColor()
{
	return this->GetDrawColor();
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
	AddProperty(nameStr, m_Name);
	AddProperty(ID, IntToString(m_Id));

	string colorStr = M3DTOOLS::floatToString(m_Color.m_r)+","+M3DTOOLS::floatToString(m_Color.m_g)+","
			+M3DTOOLS::floatToString(m_Color.m_b)+","+M3DTOOLS::floatToString(m_Color.m_a);
	AddProperty(Color,colorStr);

	//AddProperty(LOD0PointNumber, IntToString(GetEdgePntCount(0)));
	//AddProperty(LOD1PointNumber, IntToString(GetEdgePntCount(1)));
}


}
