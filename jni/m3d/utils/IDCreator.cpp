#include "m3d/utils/IDCreator.h"

#ifdef WIN32
#include <objbase.h>
#include <stdio.h>
#define GUID_LEN 64
#else
#include <uuid/uuid.h>
#endif
namespace M3D
{

IDCreator* IDCreator::g_pcreator = NULL;
const int IDCreator::DEFAULT = -1;
const int IDCreator::MODEL = 0;
const int IDCreator::NODE = 1;
const int IDCreator::SHAPE = 2;
const int IDCreator::HANDLE = 3;

IDCreator::IDCreator()
{
	m_IDMap.insert(map<int, int> ::value_type(IDCreator::DEFAULT, 100000));
	m_currentType = m_IDMap.find(IDCreator::DEFAULT);
}

IDCreator::~IDCreator()
{

}

IDCreator * IDCreator::Instance()
{
	if (g_pcreator == NULL)
	{
		g_pcreator = new IDCreator;
	}

	return g_pcreator;
}

int IDCreator::GetID(int type)
{
	if (m_currentType != m_IDMap.end())
	{
		if (type == m_currentType->first)
		{
			m_currentType->second = m_currentType->second++;
			return m_currentType->second;
		}
		else
		{
			m_currentType = m_IDMap.find(type);
			if (m_currentType == m_IDMap.end())
			{
				m_IDMap.insert(map<int, int>::value_type(type, -1));
				m_currentType = m_IDMap.find(type);
			}
			m_currentType->second = m_currentType->second++;
			return m_currentType->second;
		}
	}
	else
	{
		m_currentType = m_IDMap.find(type);
		if (m_currentType == m_IDMap.end())
		{
			m_IDMap.insert(map<int, int>::value_type(type, -1));
			m_currentType = m_IDMap.find(type);
		}
		m_currentType->second = m_currentType->second++;
		return m_currentType->second;
	}
}

void IDCreator::ResetSVLIDOffset()
{
	this->m_svlIDOffset = 0;
}

SVLGlobalID& IDCreator::GetSVLIDOffset()
{
	return this->m_svlIDOffset;
}

void IDCreator::UpdateSVLID(SVLGlobalID& maxSVLID)
{
	if (this->m_svlIDOffset.Code() < maxSVLID.Code())
	{
		this->m_svlIDOffset = maxSVLID;
	}
}

string IDCreator::GetUUID()
{

    string uuid;

#ifdef WIN32
	char buffer[GUID_LEN] = { 0 };
	GUID guid;
	if (CoCreateGuid(&guid))
	{
		fprintf(stderr, "create guid error\n");		
	}
	_snprintf(buffer, sizeof(buffer),
		"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	string tempID(buffer);
	uuid = tempID;
#else
    uuid_t uu;
    uuid_generate( uu );
    char str[37] ={0};
    uuid_unparse(uu,str);
    uuid = str;
#endif
    return uuid;
}

string IDCreator::GetIDFromTime()
{
	return "time";
}

void IDCreator::Initialize(int type, int id)
{
	if (m_currentType != m_IDMap.end())
	{
		if (type == m_currentType->first)
		{
			m_currentType->second = id;
		}
		else
		{
			m_currentType = m_IDMap.find(type);
			if (m_currentType == m_IDMap.end())
			{
				m_IDMap.insert(map<int, int>::value_type(type, id));
				m_currentType = m_IDMap.find(type);
			}
			m_currentType->second = id;
		}
	}
	else
	{
		m_currentType = m_IDMap.find(type);
		if (m_currentType == m_IDMap.end())
		{
			m_IDMap.insert(map<int, int>::value_type(type, id));
			m_currentType = m_IDMap.find(type);
		}
		m_currentType->second = id;
	}
}

int IDCreator::GetDefaultID()
{
	if (g_pcreator == NULL)
	{
		g_pcreator = new IDCreator;
	}
	return g_pcreator->GetID(IDCreator::DEFAULT);
}

}
