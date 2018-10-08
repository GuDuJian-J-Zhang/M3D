#include "m3d/base/Object.h"

namespace M3D
{
int Object::OBJID = 0;
int Object::TEST_OBJ_COUNT =0;
Object::Object(void)
{
	m_cRefrenceCount = 0;
//    LOGE("TEST_OBJ_COUNT create %d", TEST_OBJ_COUNT++);
}

Object::~Object()
{
//   LOGE("TEST_OBJ_COUNT release %d",TEST_OBJ_COUNT--);
}

void Object::AddRef(void)
{
//	LOGE("m_cRefrenceCount++ %d",m_cRefrenceCount);
	m_cRefrenceCount++;
}

void Object::Release(void)
{
	m_cRefrenceCount--;
// 	LOGE("m_cRefrenceCount-- %d",m_cRefrenceCount);
	if (m_cRefrenceCount <= 0)
	{
		FinalRelease(); //Release subClass source
	}
}

void Object::FinalRelease(void)
{
	delete this;
}

string Object::GetStates()
{
	string states = "";

	return states;
}

bool Object::SetStates(const string& states)
{
	bool ret = false;

	return ret;
}

}

