#include "m3d/graphics/Resource.h"

#include "m3d/ResourceManager.h"

namespace M3D
{

Resource::Resource():Object()
{
	Init();
}

Resource::~Resource()
{
	SetResourceManager(NULL);
}

void Resource::Init()
{
	SetResourceManager(NULL);
}

void Resource::SetResourceManager(ResourceManager* resourceMgr)
{
	this->m_resourceMgr = resourceMgr;
}

}

