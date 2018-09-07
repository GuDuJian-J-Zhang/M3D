// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_SharePtr.h
 *
 *	@brief	智能指针模板类
 *
 *	@par	历史:
 *		2016/11/22	创建。MengYaohui@HOTEAMSOFT
****************************************************************************/
#ifndef _SVLLIB_SHAREPTR_H_
#define _SVLLIB_SHAREPTR_H_
namespace HoteamSoft {
	namespace SVLLib {

template <typename T> 
class Stk_SharePtr
{
public:
	Stk_SharePtr(void);
	Stk_SharePtr(T* p);
	Stk_SharePtr(const Stk_SharePtr<T> &p);
	template <typename T2>
	Stk_SharePtr(const Stk_SharePtr<T2> &p)
	{
		if ( p->isKindOf(T::desc()) )
		{
			if (p.get())
				p->Ref();//++p->count_;
			ptr_ = p.get();
		}
	}
	~Stk_SharePtr(void);

	typedef T*	stk_pointer;
	typedef T	stk_element_type;
private:
	stk_pointer ptr_;
private:
	inline void release();
public:
	template <typename T2>
	static Stk_SharePtr<T> DynamicCast(const Stk_SharePtr<T2>& p)
  	{
  		if (p.isNull())
  		{
  			return NULL;
  		}
  		else
  		{
  			return T::DynamicCast( p.get() );
  		}
  	}
	static Stk_SharePtr CreateObject(); 

	inline stk_pointer operator->() const {return ptr_;}
	inline stk_pointer get() const {return ptr_;}
	inline stk_element_type & operator*(){return *ptr_;};
	Stk_SharePtr & operator=(const Stk_SharePtr<T> &p);
	inline bool operator==(const Stk_SharePtr<T> &p)
	{
		if (ptr_ == p.ptr_) 
			return true;
		return false;
	}
	inline bool operator!=(const Stk_SharePtr<T> &p)
	{
		if (ptr_ != p.ptr_)
			return true;
		return false;
	}
	inline void reset();
	inline bool isNull() const
	{
		if (ptr_)
			return false;
		return true;
	}
	inline bool isNotNull() const
	{
		if (ptr_)
			return true;
		return false;
	}
};

template <typename T>
void Stk_SharePtr<T>::release()
{
	if(ptr_)
	{
		ptr_->UnRef();
		if ( 0 == ptr_->count_ )
		{
			delete ptr_; 
			ptr_ = 0;
		}
	}
}

template <typename T>
void Stk_SharePtr<T>::reset()
{
	while(ptr_)
	{
		release();
	}
}

template <typename T>
Stk_SharePtr<T>::~Stk_SharePtr()
{
	release();
}

template <typename T>
Stk_SharePtr<T>::Stk_SharePtr():ptr_(0)
{

} 

template <typename T>
Stk_SharePtr<T>::Stk_SharePtr(T* p):ptr_(p)
{
	if (p)
		p->Ref();//++p->count_;
} 

template <typename T>
Stk_SharePtr<T>::Stk_SharePtr(const Stk_SharePtr<T> &p):ptr_(p.ptr_)
{
	if (p.ptr_)
		ptr_->Ref();//++ptr_->count_;
} 

template <typename T>
Stk_SharePtr<T> Stk_SharePtr<T>::CreateObject()
{
	return Stk_SharePtr<T>(new T);
} 

template <typename T>
Stk_SharePtr<T> & Stk_SharePtr<T>::operator=(const Stk_SharePtr<T> &p)
{
	release();
	if (p.ptr_)
		p->Ref();//++p->count_;
	ptr_ = p.ptr_;

	return *this;
}

template <class T1, class T2>
inline bool operator==(const Stk_SharePtr<T1> &x, const Stk_SharePtr<T2> &y)
{  return x.get() == y.get(); }

template <class T1, class T2>
inline bool operator!=(const Stk_SharePtr<T1> &x, const Stk_SharePtr<T2> &y)
{  return !(x == y); }

template <class T1, class T2>
inline bool operator<(const Stk_SharePtr<T1> &x, const Stk_SharePtr<T2> &y)
{  return x.get() < y.get(); }

template <class T1, class T2>
inline bool operator<=(const Stk_SharePtr<T1> &x, const Stk_SharePtr<T2> &y)
{  return !(y < x); }

template <class T1, class T2>
inline bool operator>(const Stk_SharePtr<T1> &x, const Stk_SharePtr<T2> &y)
{  return y < x; }

template <class T1, class T2>
inline bool operator>=(const Stk_SharePtr<T1> &x, const Stk_SharePtr<T2> &y)
{  return !(x < y); }

	}//svllib
}//hoteamsoft

#endif