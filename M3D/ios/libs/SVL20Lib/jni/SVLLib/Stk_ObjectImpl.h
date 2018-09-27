// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_ObjectImpl.h
 *
 *	@brief	Stk_ObjectImplª˘¿‡,”√”⁄¥Ê¥¢ ˝æ›£®“˛≤ÿ ˝æ›£©
 *
 *	@par	¿˙ ∑:
 *		2016/11/20	¥¥Ω®°£MengYaohui@HOTEAMSOFT
****************************************************************************/
#ifndef _SVLLIB_OBJECTIMPL_H_
#define _SVLLIB_OBJECTIMPL_H_

#include "Stk_Define.h"
//#include "Stk_Confige.h"
#include "Stk_Declare_Ptr.h"
#include "Stk_File_struct.h"
#include "Stk_Tab.h"
#include <set>
#include <map>

namespace HoteamSoft {
	namespace SVLLib {
        
        class Stk_Object;
        
#define STK_CLASS_IMPL_DECLARE(class_name)									\
public:																		\
	static class_name##Impl* getImpl(const class_name *p);					\
	friend class class_name;

#define STK_CLASS_IMPL_IMPLEMENT(class_name)								\
	class_name##Impl* class_name##Impl::getImpl(const class_name *p){		\
		return (class_name##Impl*)Stk_SystemInternals::getImpl(p);}

#define STK_CLASS_IMPL(class_name, obj) class_name##Impl::getImpl(obj)
#define STK_CLASSPTR_IMPL(class_name, obj) class_name##Impl::getImpl(obj.get())
#define STK_IMPL_CAST(class_name) ((class_name##Impl *)m_pImpl)
#define STK_PTR_GET_IMPL(obj) Stk_SystemInternals::getImpl(obj.get())

#define DELETE_STK_CLASS_IMPL		\
	if (NULL != m_pImpl){			\
		delete m_pImpl;				\
		m_pImpl= NULL;}

typedef std::set<STK_ID>				Stk_ID_Set;
typedef std::set<Stk_string>			Stk_string_Set;
typedef std::set<Stk_wstring>			Stk_Wstring_Set;
typedef std::map<Stk_wstring, STK_ID>	Stk_Wstring_Map;

//class Stk_FileImpl;

class Stk_ObjectImpl
{
public:
	Stk_ObjectImpl(void);
	virtual ~Stk_ObjectImpl(void);

	friend class Stk_Object;
protected:
	STK_ID			m_ID;				//!< ID∫≈
	std::string		_uuid;				//!< uuid,æﬂ”–Œ®“ª–‘
	static STK_ID	_gsobjectId;			//!< ∏√objID≤ªƒ‹Ω¯––…Ë÷√£¨µ›‘ˆ,æﬂ”–Œ®“ª–‘
public:
	/*****************************************************************************
	*	@brief		Ω´ ˝æ›–¥»ÎSVLŒƒº˛
	*	@param[in]	i_head					º«¬ºÕ∑
	*	@param[in]	i_stkFileP				Œƒº˛∂‘œÛ÷∏’Î
	*	@param[out]	Œﬁ
	*	@retval		Œﬁ
	*	@note		Œﬁ
	*	@attention	ƒ⁄≤øΩ”ø⁄
	****************************************************************************/
	//virtual STK_STATUS WriteFile(StkSegmentHead* i_head, Stk_FilePtr i_stkFileP);

	/**************************************************************************
	 *	@brief		ªÒ»°Àﬁ÷˜µƒuuid
	 *	@param[in]	Œﬁ
	 *	@param[out]	Œﬁ
	 *	@retval		uuid◊÷∑˚¥Æ
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	**************************************************************************/
	Stk_string GetUuid(){return _uuid;}

	/**************************************************************************
	 *	@brief		Ω´ ˝æ›–¥»ÎSVLŒƒº˛
	 *	@param[out]	pFileImpl					Œƒº˛ µœ÷
	 *	@param[out]	i_head						∂ŒÕ∑
	 *	@retval		–¥»Î≥…π¶:STK_SUCCESS		–¥»Î ß∞‹:STK_ERROR_FILEWR
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	**************************************************************************/
	//virtual STK_STATUS WriteFile(Stk_FileImpl *pFileImpl, StkSegmentHead* i_head);

	/**************************************************************************
	 *	@brief		∂¡»°SVLŒƒº˛
	 *	@param[in]	pFileImpl					Œƒº˛ µœ÷
	 *	@param[in]	i_head						∂ŒÕ∑
	 *	@retval		∂¡»°≥…π¶:STK_SUCCESS		∂¡»° ß∞‹:STK_ERROR_FILERD
	 *	@note		Œﬁ
	 *	@attention	Œﬁ
	**************************************************************************/
	//virtual STK_STATUS ReadFile(Stk_FileImpl *pFileImpl, StkSegmentHead* i_head);
};

class Stk_SystemInternals
{
public:
	static Stk_ObjectImpl* getImpl(const Stk_Object* pObj);
};

	}//svllib
}//hoteamsoft
#endif
