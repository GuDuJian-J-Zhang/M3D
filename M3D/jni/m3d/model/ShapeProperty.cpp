#include "m3d/model/ShapeProperty.h"

namespace M3D
{
ShapeProperty::ShapeProperty() :
		Object()
{

}

ShapeProperty::~ShapeProperty()
{

}

vector<ShapePropertyItem>& ShapeProperty::GetProperties()
{
	return this->m_properties;
}

void ShapeProperty::SetProperties(vector<ShapePropertyItem>& properties)
{
	this->m_properties = properties;
}

}
