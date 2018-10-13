// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_ComponentFeature.cpp
 *
 *		2016/11/22	÷ÿππ°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#include "Stk_Accessor.h"
using namespace std;

namespace HoteamSoft {
	namespace SVLLib {

STK_IMPLEMENT_CLASS(Stk_Accessor, Stk_Object)
Stk_Accessor::Stk_Accessor(void) :Stk_Object()
{
	this->SetType(ACCESSOR_TYPE_UNKNOWN);
}

Stk_Accessor::~Stk_Accessor(void)
        {
            //DELETE_STK_CLASS_IMPL
}

STK_BOOL Stk_Accessor::Encode(rapidjson::Value& accessor)
{
	return false;
}

STK_BOOL Stk_Accessor::Uncode(rapidjson::Value& accessor)
{
	return false;
}

}//svllib
}//hoteamsoft
