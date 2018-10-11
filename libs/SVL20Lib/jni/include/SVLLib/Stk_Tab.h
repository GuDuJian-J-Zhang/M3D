// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
 *	@file		Stk_Tab.h
 *  @brief		集合定义
 *	@par 历史记录
 *				2016/10/31 创建。MengYaohui(Hoteamsoft)
**************************************************************************/
#ifndef _SVLLIB_TAB_H_
#define _SVLLIB_TAB_H_
#include <vector>
#include <stdlib.h>
namespace HoteamSoft {
	namespace SVLLib {

template <typename T> 
class Stk_Tab
{
public:
	Stk_Tab(void);
	Stk_Tab(const int &);
	Stk_Tab(const std::vector<T> &);
	~Stk_Tab(void);
	//!< 迭代器定义
	class iterator
	{
	public:
		iterator(void);
		iterator(T *);
		iterator(const iterator &);
		~iterator(void);
	private:
		T *ptr_iter;
		int nSub;
	public:
		inline bool operator==(const iterator &);
		inline bool operator!=(const iterator &);
		inline const T & operator*() const;
		inline T * operator->() const;
		inline iterator & operator=(const iterator &);
		inline iterator & operator++();
		inline iterator operator++(int);
		inline iterator operator+(int);
		inline iterator & operator--();
		inline iterator operator--(int);
		inline iterator operator-(int);
		inline iterator & operator+=(int);
		inline iterator & operator-=(int);
	};
private:
	T *m_pTs;
	int m_nSize;
	int m_nCapacity;
private:
	inline void copy(T *, T *, int);
	inline void DestroyMemory(void*);
public:
	inline void resize(const int &);
	inline void push_back(const T &);
	inline T at(const int &);
	inline int capacity(){return m_nSize;}
	inline int size(){return m_nSize;}
	inline int find(const T &);
	inline int rfind(const T &);
	inline bool erase(const T &);
	inline bool erase_size(const size_t &);
	inline void swap(int, int);
	inline void clear();
	inline bool isEmpty();
	iterator  begin(){return iterator(m_pTs);}
	iterator end(){return iterator(m_pTs+m_nSize);}

	inline void set_vector(std::vector<T> &);
	inline T &operator[](int);
	inline Stk_Tab & operator=(const Stk_Tab &);
	inline Stk_Tab & operator=(const std::vector<T> &);

	friend int operator-(const iterator &t1, const iterator &t2);
};

template <typename T> 
Stk_Tab<T>::iterator::iterator(void):nSub(0),ptr_iter(NULL)
{

}

template <typename T> 
Stk_Tab<T>::iterator::iterator(T *p):nSub(0),ptr_iter(p)
{

}

template <typename T> 
Stk_Tab<T>::iterator::iterator(const iterator &p)
{
	nSub	 = p.nSub;
	ptr_iter = p.ptr_iter;
}

template <typename T> 
Stk_Tab<T>::iterator::~iterator(void)
{

}

template <typename T> 
bool Stk_Tab<T>::iterator::operator==(const iterator &p)
{
	return (ptr_iter + nSub) == (p.ptr_iter + p.nSub);
}

template <typename T> 
bool Stk_Tab<T>::iterator::operator!=(const iterator &p)
{
	return (ptr_iter + nSub) != (p.ptr_iter + p.nSub);
}

template <typename T>
typename Stk_Tab<T>::iterator & Stk_Tab<T>::iterator::operator=(const iterator &p)
{
	ptr_iter = p.ptr_iter;
	nSub	 = p.nSub;

	return *this;
}

template <typename T> 
const T & Stk_Tab<T>::iterator::operator*() const
{
	if (ptr_iter)
		return *(ptr_iter + nSub);
//	else
//		throw std::bad_exception("迭代器包含的指针为空");
}

template <typename T> 
T * Stk_Tab<T>::iterator::operator->() const
{
	if (ptr_iter)
		return (ptr_iter + nSub);
//	else
//		throw std::bad_exception("迭代器包含的指针为空");
}

template <typename T> 
typename Stk_Tab<T>::iterator & Stk_Tab<T>::iterator::operator++()
{
	++nSub;
	return *this;
}

template <typename T> 
typename Stk_Tab<T>::iterator Stk_Tab<T>::iterator::operator++(int)
{
	iterator ret = *this;
	++nSub;
	return ret;
}

template <typename T> 
typename Stk_Tab<T>::iterator Stk_Tab<T>::iterator::operator+(int n)
{
	iterator ret = *this;
	nSub += n;
	return ret;
}

template <typename T> 
typename Stk_Tab<T>::iterator & Stk_Tab<T>::iterator::operator--()
{
	--nSub;
	return *this;
}

template <typename T> 
typename Stk_Tab<T>::iterator Stk_Tab<T>::iterator::operator--(int)
{
	iterator ret = *this;
	--nSub;
	return ret;
}

template <typename T> 
typename Stk_Tab<T>::iterator Stk_Tab<T>::iterator::operator-(int n)
{
	iterator ret = *this;
	nSub -= n;
	return ret;
}

template <typename T> 
typename Stk_Tab<T>::iterator & Stk_Tab<T>::iterator::operator+=(int n)
{
	nSub += n;
	return *this;
}

template <typename T> 
typename Stk_Tab<T>::iterator & Stk_Tab<T>::iterator::operator-=(int n)
{
	nSub -= n;
	return *this;
}

template <typename T>
Stk_Tab<T>::Stk_Tab(void):m_nSize(0),m_nCapacity(1),m_pTs(NULL)
{
	//m_pTs = new T[m_nCapacity];
}

template <typename T>
Stk_Tab<T>::Stk_Tab(const int &nCapacity):m_nSize(0),m_nCapacity(nCapacity),m_pTs(NULL)
{
	//m_pTs = new T[m_nCapacity];
}

template <typename T>
Stk_Tab<T>::Stk_Tab(const std::vector<T> &v)
{
	if (v.empty())
		return;
	int vsize	= v.size();
	m_nSize		= vsize;
	m_nCapacity = vsize;
	m_pTs		= new T[m_nSize];
	for (int i = 0; i < vsize; i++)
		m_pTs[i] = v[i];
}

template <typename T>
Stk_Tab<T>::~Stk_Tab(void)
{
	clear();
}

template <typename T>
void Stk_Tab<T>::copy(T *pSource, T *pDest, int nSize)
{
	if (NULL == pSource || NULL == pDest)
		return;
	for (int i = 0; i < nSize; i++)
		pDest[i] = pSource[i];
}

template <typename T>
void Stk_Tab<T>::DestroyMemory(void* pMemory)
{
	if (NULL != m_pTs)
	{
		delete [] m_pTs;;
		pMemory = NULL;
	}
}

template <typename T>
void Stk_Tab<T>::resize(const int &nCapacity)
{
	if (m_nCapacity == nCapacity || m_nSize > nCapacity)
		return;
	m_nCapacity = nCapacity;
	T *pTs = new T[m_nCapacity];
	copy(m_pTs, pTs, m_nSize);
	DestroyMemory(m_pTs);
	m_pTs = pTs;
}

template <typename T>
void Stk_Tab<T>::push_back(const T &p)
{
	if (NULL == m_pTs)
		m_pTs = new T[m_nCapacity];
	if (m_nSize == m_nCapacity)
	{
		m_nCapacity += (m_nCapacity+1)/2;
		resize(m_nCapacity);
	}
	if (NULL != m_pTs)
		m_pTs[m_nSize++] = p;
}

template <typename T>
T Stk_Tab<T>::at(const int &index)
{
	if ( 0 > index || index > (m_nSize - 1) )
//		throw std::bad_alloc("访问越界!");
	return m_pTs[index];
}

template <typename T>
int Stk_Tab<T>::find(const T &p)
{
	int nFind = -1;
	for (int i = 0; i < m_nSize; i++)
	{
		if (m_pTs[i] == p)
		{
			nFind = i;
			break;
		}
	}
	return nFind;
}

template <typename T>
int Stk_Tab<T>::rfind(const T &p)
{
	int nFind = -1;
	for (int i = m_nSize - 1; i > -1; i--)
	{
		if (m_pTs[i] == p)
		{
			nFind = i;
			break;
		}
	}
	return nFind;
}

template <typename T>
bool Stk_Tab<T>::erase(const T &p)
{
	int nFind = find(p);
	if ( -1 == nFind )
		return false;
	for (int i = nFind; i < m_nSize - 1; i++)
		m_pTs[i] = m_pTs[i+1];
	m_nSize--;

	return true;
}

template <typename T>
bool Stk_Tab<T>::erase_size(const size_t &index)
{
	if ( 0 > index || index > (m_nSize - 1) )
//		throw std::bad_alloc("访问越界!");
	for (int i = index; i < m_nSize - 1; i++)
		m_pTs[i] = m_pTs[i+1];
	m_nSize--;

	return true;
}	

template <typename T>
void Stk_Tab<T>::swap(int i, int j)
{
	if ( 0 > i || m_nSize >= i || 0 > j || m_nSize >= j)
		throw std::bad_alloc();
	T tmp	 = m_pTs[i];
	m_pTs[i] = m_pTs[j];
	m_pTs[j] = tmp;
}

template <typename T>
void Stk_Tab<T>::clear()
{
	DestroyMemory(m_pTs);
	m_nSize = 0;
	m_nCapacity = 0;
}	

template <typename T>
bool Stk_Tab<T>::isEmpty()
{
	if (m_pTs)
		return false;
	return true;
}

template <typename T>
T & Stk_Tab<T>::operator[](int n)
{
	if (n >= m_nSize || 0 > n) {
		//throw std::bad_alloc("访问越界!");
		throw std::bad_alloc();
	}

	return m_pTs[n];
}

template <typename T>
Stk_Tab<T> & Stk_Tab<T>::operator=(const Stk_Tab &t)
{
	m_nSize		= t.m_nSize;
	m_nCapacity = t.m_nCapacity;
	m_pTs		= new T[m_nSize];
	for (int i = 0; i < m_nSize; i++)
		m_pTs[i] = t[i];

	return *this;
}

template <typename T>
Stk_Tab<T> & Stk_Tab<T>::operator=(const std::vector<T> &v)
{
	m_nSize		= (int)v.size();
	m_nCapacity = (int)v.size();
	m_pTs		= new T(m_nSize);
	for (int i = 0; i < m_nSize; i++)
		m_pTs[i] = v[i];

	return *this;
}

template <typename T>
void Stk_Tab<T>::set_vector(std::vector<T> &v)
{
	for (int i = 0; i < m_nSize; i++)
		v.push_back(m_pTs[i]);
}
	}//svllib
}//hoteamsoft
#endif