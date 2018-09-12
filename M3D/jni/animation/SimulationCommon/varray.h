
// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		varray.h
*
*	@brief	定义数组及操作接口
*
*
*	@par	历史:
*
****************************************************************************/

#ifndef VARRAY_H
#define VARRAY_H

#include <stdlib.h>
#include <string.h>
 
#ifdef __cplusplus
extern "C++" {

#define VARRAY_COMPILE_TIME_ASSERT(condition) /*\
	do { class message { char array[condition ? 1 : -1]; }; } while(0) */


template <typename T> class VArray
#ifdef SVIEW_DEFINED
: public Controlled_Memory_Object
#endif
{
public:
	
	VArray(int size=1):count(0),allocated(0),data(0) {
		EnsureSize(size);
		VARRAY_COMPILE_TIME_ASSERT(sizeof(T) <= sizeof(void *));	
	};

	VArray(VArray<T> &original):count(0),allocated(0),data(0) {
		EnsureSize(original.Count());
		count = original.Count();
		memcpy(data, original.data, count*sizeof(T));
		VARRAY_COMPILE_TIME_ASSERT(sizeof(T) <= sizeof(void *));	
	};

	VArray(VArray<T> *original):count(0),allocated(0),data(0) {
		EnsureSize(original->Count());
		count = original->Count();
		memcpy(data, original->data, count*sizeof(T));
		VARRAY_COMPILE_TIME_ASSERT(sizeof(T) <= sizeof(void *));	
	};
	
	virtual ~VArray() {
		TrimSize(0);
	}; 
	
 	inline int Count() const { return count; }
 	inline int Size() const { return allocated; }

	/* If T is a char, you can use this class like so...
	 *    VCharArray buf(BUFFER_SIZE);
	 */
	operator T * () {
		return this->data;
	};
#if 0
	operator T const * () {
		return this->data;
	};
#endif
	T& operator [] (int i){
		
		if(i >= count){
			EnsureSize(i+1);
			count=i+1;
		}
		return data[i];
	};

	T const& operator [] (int i) const {
		
		if(i >= count){
			EnsureSize(i+1);
			count=i+1;
		}
		return data[i];
	};

	bool Append(T const & item) {
		EnsureSize(count+1);
		data[count++] = item;
		return true;
	};

	void AppendArray(T const *item_array, int n_items) {
		if(!n_items) return;
		EnsureSize(n_items+count);
		memcpy(&data[count], item_array, n_items*sizeof(T));
		count+=n_items;
	};

 	T & GetData(int i) {
		return data[i];
	};

	void GetArrayCopy(T *new_item_array) {
		
		memcpy(new_item_array,data,count*sizeof(T));
	};


 	T & Pop() {
		return data[--count];
	};

	void InsertAt(T const & item, int i) {

		if(i >= count) {
			EnsureSize(i+1);
			count=i+1;
		}else{
			EnsureSize(count+1);
			memmove(&data[i+1], &data[i], (count-i)*sizeof(T)); 
			count++;
		}
		data[i] = item;
	};

	bool RemoveAt(int i, T * old_item=0) {

		if(i>=count) 
			return false;
		count--;
		if(old_item) 
			*old_item = data[i];
		if(i!=count) 
			memmove(&data[i], &data[i+1], (count-i)*sizeof(T)); 
		return true;
	};

	bool RemoveAt(int i, T & old_item) {
		return RemoveAt(i,&old_item);
	};

 	void SetCount(int in_count) { 
		EnsureSize(in_count);
		count=in_count;
	};

	void Reverse() {
		T	temp;
		T *	start;
		T *	end;

		start = data;
		end = data + count - 1;
		while (start < end) {
			temp = *start;
			*start = *end;
			*end = temp;
			start++;
			end--;
		}
	};

	bool ReplaceAt(T const & item, int i, T * old_item=0) {

		if(i>=count){
			EnsureSize(i+1);
			count=i+1;
			data[i]=item;
			return false;
		}
		if(old_item) *old_item = data[i];
		data[i]=item;
		return true;
	};

	bool ReplaceAt(T const & item, int i, T & old_item) {
		return ReplaceAt(item,i,&old_item);
	};

	void MapFunction(void(*function)(T, void*), void * user_data) const{
		int i;
		for(i=0;i<count;i++)
			(*function)(data[i],user_data);
	};

	void MapFunction(void(*function)(T&, void*), void * user_data) {
		int i;
		for(i=0;i<count;i++)
			(*function)(data[i],user_data);
	};

	void MapFunction(void(*function)(T const &, void*), void * user_data) const{
		int i;
		for(i=0;i<count;i++)
			(*function)(data[i],user_data);
	};

	void TrimSize(int size=-1) {
		if (size == -1)
			size = count;
		MakeSize(size);
	};

	void EnsureSize(int size) const {
		if(size<=allocated) 
			return;
		MakeSize((int)((float)size * 1.5 + 4.0));
	};
	
protected:

	void MakeSize(int size) const {

#ifdef SVIEW_DEFINED
		int old_allocated = allocated;
#endif
		allocated = size;
	
		T * new_data = 0;
		if (allocated>0)
#ifdef SVIEW_DEFINED
			new_data = (T *)operator new(allocated*sizeof(T));
#else
			new_data = new T[allocated];
#endif

		if (size < count)
			count = size;

		if (data != 0) {
			//update by likun 解决非标准数据类型数组重新设置数据大小数据丢失的问题
			//memcpy(new_data,data,count*sizeof(T));
			for(int i = 0; i < count; i++)
			{
				new_data[i] = data[i];
			}
			
#ifdef SVIEW_DEFINED
			operator delete(data, old_allocated*sizeof(T));
#else
			delete[] data;
#endif
		}
		data=new_data;
	};

	mutable int count;   
	mutable int allocated;
	mutable T *data;

}; 

typedef VArray<char> VCharArray;
typedef VArray<int> VIntArray;
typedef VArray<float> VFloatArray;

} /* extern "C++" */

#endif

#endif 



