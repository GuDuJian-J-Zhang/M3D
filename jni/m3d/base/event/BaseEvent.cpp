#include "m3d/base/event/BaseEvent.h"

namespace M3D
{

const string BaseEvent::BASE_EVENT = "BASE";
const string BaseEvent::CAMERA_EVENT ="CAMERA";
const string BaseEvent::SHAPE_EVENT ="SHAPE";

BaseEvent::BaseEvent(void)
{

}

BaseEvent::~BaseEvent()
{

}

string& BaseEvent::GetProperty()
{
	if(this->IsDirty())
	{
		this->m_isDirty = false;
		this->CreateProperty();

	}
	return this->m_Content;
}


void BaseEvent::SetProperty(const string& property)
{
	this->m_Content = property;
	if(this->IsDirty())
	{
		this->m_isDirty = false;
		this->ApplyProperty();
	}
}


const string& BaseEvent::GetType()
{
	return BASE_EVENT;
}

void BaseEvent::CreateProperty()
{

}

void BaseEvent::ApplyProperty()
{

}

void BaseEvent::MarkDirty()
{
	this->m_isDirty = true;
}

bool BaseEvent::IsDirty()
{
	return this->m_isDirty;
}

}

