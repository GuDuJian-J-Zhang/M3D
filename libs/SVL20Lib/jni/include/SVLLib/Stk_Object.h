// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_Object
	*	@brief	Object��,����
	*			
**************************************************************************/
#ifndef _SVLLIB_OBJECT_H_
#define _SVLLIB_OBJECT_H_

 #ifdef  SVLLIB_EXPORT
 #define STK_TOOLKIT_EXPORT
 #else
 #ifdef  STK_DLL_EXPORT
 #define STK_TOOLKIT_EXPORT __declspec(dllexport)
 #else
 #define STK_TOOLKIT_EXPORT __declspec(dllimport)
 #endif
 #endif

#include "Stk_Define.h"
#include <stdlib.h>
#include "Stk_SharePtr.h"
#include "Stk_Declare_Ptr.h"
#include <string>

namespace HoteamSoft {
	namespace SVLLib {

typedef struct _Stk_Runtime
{
	std::string name;
	_Stk_Runtime *pBase;
}Stk_Runtime;

#define STK_DECLARE_CLASS(class_name)													\
private:																				\
	_HEAP_OPERATORS()																	\
	static const Stk_Runtime class##class_name;											\
protected:																				\
	class_name(Stk_ObjectImpl *);														\
public:																					\
	static Stk_Runtime* GetRuntime();													\
	static std::string desc();															\
	static class class_name * DynamicCast(Stk_Object *p);								\
	virtual bool isKindOf(const std::string & strClassName);							\
	virtual std::string GetClassNameA(){return #class_name;}							\
	virtual enum stk_type_detk get_type_detk() const{return (stk_type_detk)_typeID;};	\
	friend class Stk_SharePtr<class_name>;

#define STK_IMPLEMENT_CLASS(class_name, base_class_name)								\
	class_name::class_name(Stk_ObjectImpl *p):base_class_name(p){}						\
	const Stk_Runtime class_name::class##class_name = {									\
		#class_name, base_class_name::GetRuntime()};									\
	Stk_Runtime* class_name::GetRuntime()												\
	{																					\
		return (Stk_Runtime*)(&class_name::class##class_name);							\
	}																					\
	std::string class_name::desc()														\
	{																					\
		return class_name::GetRuntime()->name;											\
	}																					\
	bool class_name::isKindOf(const std::string & strClassName)							\
	{																					\
		if (class_name::desc() == strClassName)											\
		{																				\
			return true;																\
		}																				\
		const Stk_Runtime* pRuntime = base_class_name::GetRuntime();					\
		while(pRuntime != NULL)															\
		{																				\
			if (strClassName == pRuntime->name)											\
			{																			\
				return true;															\
			}																			\
			pRuntime = pRuntime->pBase;													\
		}																				\
			return false;																\
	}																					\
	class_name * class_name::DynamicCast(Stk_Object *p)									\
	{																					\
		if ( p->isKindOf( class_name::desc() ) )										\
		{																				\
			return static_cast<class_name *>(p);										\
		}																				\
		return NULL;																	\
	}

#define _HEAP_OPERATORS()																\
	void* operator new(size_t s)														\
	{																					\
		void* p = ::malloc(s);															\
		if ( !p ) throw std::bad_alloc();												\
		return p;																		\
	}																					\
	void operator delete(void* p) { ::free(p); }										\
	void* operator new[](size_t s)														\
	{																					\
		void* p = ::malloc(s);															\
		if ( !p ) throw std::bad_alloc();												\
		return p;																		\
	}																					\
	void operator delete[](void* p) { ::free(p); }										\
	void *operator new(size_t, void* p) { return p; }									\
	void operator delete( void*, void* ) {}												\
	void *operator new[](size_t, void* p) { return p; }									\
	void operator delete[]( void*, void* ) {}											

class Stk_ObjectImpl;
class Stk_SystemInternals;
class Mutex;

class STK_TOOLKIT_EXPORT Stk_Object
{
	STK_DECLARE_CLASS(Stk_Object)
public:
	Stk_Object(void);
	virtual ~Stk_Object(void);
protected:
	int				count_;				//!< ���ü���
	Mutex*			m_pMutexRefCount;
	enum {_typeID = _SVLLIB_TYPE_OBJECT};	//!< ���
	Stk_ObjectImpl *m_pImpl;			//!< ��������
public:
	std::string GetUuid();
	static STK_ID objectId();
	/****************************************************************************
	 *	@brief		��ȡID
	 *	@param[in]	��
	 *	@param[out]	��
	 *	@retval		m_ID			ID
	 *	@note		��
	 *	@attention	��
	 ****************************************************************************/
	STK_ID GetID();
	/****************************************************************************
	 *	@brief		��ȡID
	 *	@param[in]	i_ID			ID
	 *	@param[out]	��
	 *	@retval		���óɹ�:STK_SUCCESS		ʧ��:STK_ERROR
	 *	@note		��
	 *	@attention	��
	 ****************************************************************************/
	STK_STATUS SetID(STK_ID i_ID);

	/****************************************************************************
	*	@brief		���ü�����1
	*	@param[in]	��
	*	@param[out]	��
	*	@retval		��
	*	@note		��
	*	@attention	��
	****************************************************************************/
	void Ref();
	/****************************************************************************
	*	@brief		���ü�����1
	*	@param[in]	��
	*	@param[out]	��
	*	@retval		��
	*	@note		��
	*	@attention	��
	****************************************************************************/
	void UnRef();
	friend class Stk_ObjectImpl;
	friend class Stk_SystemInternals;
};

	}//svllib
}//hotamsoft

#endif