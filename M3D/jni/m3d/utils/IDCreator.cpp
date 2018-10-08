#include "m3d/utils/IDCreator.h"
#ifdef __IOS__
#include <uuid/uuid.h>
#elif WIN32
#include <objbase.h>
#include <stdio.h>
#define GUID_LEN 64
#endif
namespace M3D
{

IDCreator* IDCreator::m_pcreator = NULL;
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

int IDCreator::GetID(int type)
{
	//	LOGE("IDCreator::GetID 1");
	if (m_currentType != m_IDMap.end())
	{
		if (type == m_currentType->first)
		{
			m_currentType->second = m_currentType->second++;
			//					LOGE("IDCreator::GetID 3");
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
			//				LOGE("IDCreator::GetID 4");
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
		//		LOGE("IDCreator::GetID 5");
		return m_currentType->second;
	}
	//	LOGE("IDCreator::GetID 2");
}

string IDCreator::GetUUID()
{
    string uuid;
#ifdef __IOS__
    uuid_t uu;
    uuid_generate( uu );
    char str[37] ={0};
    uuid_unparse(uu,str);
    uuid = str;
#elif WIN32
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
	if (m_pcreator == NULL)
	{
		m_pcreator = new IDCreator;
	}
	return m_pcreator->GetID(IDCreator::DEFAULT);
}

}
