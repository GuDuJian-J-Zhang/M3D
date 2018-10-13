#include "Stk_ObjectImpl.h"
#include "Stk_Object.h"
#include "Stk_Uuid.h"
//#include "Stk_File.h"
//#include "Stk_FileImpl.h"

namespace HoteamSoft {
	namespace SVLLib {

	
/**********************************************************************//**
 *	@brief		≤˙…˙uuid
 *	@param[in]	Œﬁ
 *	@param[out]	Œﬁ
 *	@retval		◊÷∑˚¥Æ
 *	@note		”√MD5À„∑®≤˙…˙uuid
 *	@attention	Œﬁ
**************************************************************************/
std::string generate_uuid()
        {
            stk::uuid::uuid_t u;
	stk::uuid::uuid_create(&u);

	return std::string(stk::uuid::uuid_to_string(&u));
}

STK_ID Stk_ObjectImpl::_gsobjectId = 0x3EA;

Stk_ObjectImpl::Stk_ObjectImpl(void):
m_ID(-1)
{
	//_uuid = generate_uuid();
	_uuid = "";
	m_ID = _gsobjectId++;
}

Stk_ObjectImpl::~Stk_ObjectImpl(void)
{
}


Stk_ObjectImpl* Stk_SystemInternals::getImpl(const Stk_Object* pObj)
{
	return const_cast<Stk_ObjectImpl*>(pObj->m_pImpl);
}

	}//svllib
}//hoteamsoft
