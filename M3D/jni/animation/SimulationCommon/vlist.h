﻿// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		vlist.h
*
*	@brief	定义列表及操作接口
*
*
*	@par	历史:
*
****************************************************************************/

/*! 
    \file vlist.h
    \brief A documented header file containing the vlist 
*/


#ifndef VLIST_H
#define VLIST_H

#include <stdarg.h>
#include <stdlib.h>


#ifndef SVIEW_DEFINED
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif 


#ifdef _MSC_VER
#define VLIST_SYS_CDECL __cdecl
#else
#define VLIST_SYS_CDECL 
#endif

#ifdef WIN32
#define VLIST_API extern "C" __declspec(dllexport)
#else
#define VLIST_API extern "C"
#endif // WIN32


#define VLIST_CDECL

#ifdef _MSC_VER
#define VLIST_SYS_CDECL __cdecl
#else
#define VLIST_SYS_CDECL 
#endif

typedef void *(VLIST_SYS_CDECL *vmalloc_t) (size_t);
typedef void (VLIST_SYS_CDECL *vfree_t) (void*);

#ifdef V_PREFIX

#define VNAME_TMP_2(_x2_,_y2_) _x2_##_y2_
#define VNAME_TMP_1(_x1_,_y1_) VNAME_TMP_2(_x1_,_y1_)
#define VNAME(_x_) VNAME_TMP_1(V_PREFIX,_x_)

/* remap the names for special prefix */
#define new_vlist VNAME(new_vlist)
#define delete_vlist VNAME(delete_vlist)
#define vlist_add_first VNAME(vlist_add_first)
#define vlist_add_last VNAME(vlist_add_last)
#define vlist_add_before_cursor VNAME(vlist_add_before_cursor)
#define vlist_add_after_cursor VNAME(vlist_add_after_cursor)
#define vlist_remove VNAME(vlist_remove)
#define vlist_remove_first VNAME(vlist_remove_first)
#define vlist_remove_cursor_next VNAME(vlist_remove_cursor_next)
#define vlist_remove_at_cursor VNAME(vlist_remove_at_cursor)
#define vlist_reset_cursor VNAME(vlist_reset_cursor)
#define vlist_peek_cursor VNAME(vlist_peek_cursor)
#define vlist_peek_cursor_next VNAME(vlist_peek_cursor_next)
#define vlist_peek_cursor_next_next VNAME(vlist_peek_cursor_next_next)
#define vlist_advance_cursor VNAME(vlist_advance_cursor)
#define vlist_peek_first VNAME(vlist_peek_first)
#define vlist_peek_last VNAME(vlist_peek_last)
#define vlist_count VNAME(vlist_count)
#define vlist_item_exists VNAME(vlist_item_exists)
#define vlist_items_to_array VNAME(vlist_items_to_array)
#define vlist_map_function VNAME(vlist_map_function)
#define vlist_nth_item VNAME(vlist_nth_item)
#define vlist_reverse VNAME(vlist_reverse)
#define vlist_flush VNAME(vlist_flush)
#define vlist_sort VNAME(vlist_sort)
#define vlist_add_sorted VNAME(vlist_add_sorted)

#endif

typedef int (VLIST_SYS_CDECL *vcompare_t) (const void*, const void *, const void *);

/*!
	The vlist is single linked list that stores void pointers.
	It has a cursor to traverse the list. 
*/

typedef struct vlist_node_s{
	void * item;
	struct vlist_node_s * next;
} vlist_node_t;

typedef struct vlist_s{
	struct vlist_node_s * head;
	struct vlist_node_s * tail;
	struct vlist_node_s * cursor;
	struct vlist_node_s * cursor_backlink;
	unsigned int cursor_index;
	unsigned int count;
#ifdef SVIEW_DEFINED
	Memory_Pool * memory_pool;
#else
	vmalloc_t vmalloc;
	vfree_t vfree;
#endif
} vlist_t;

#ifdef SVIEW_DEFINED
VLIST_API vlist_t* VLIST_CDECL new_vlist(
	Memory_Pool * memory_pool);
#else
/*!
    \brief This function creates and initializes a vlist structure.
	\var vlist_malloc A malloc function for vlist to use	
	\var vlist_free A free function for vlist to use	
	\return The vlist structure 
*/
VLIST_API vlist_t* VLIST_CDECL new_vlist(
	vmalloc_t vlist_malloc,
	vfree_t vlist_free);
#endif

/*!
    \brief This function destroys and cleans up a vlist structure.
	\var vlist The list to delete
*/
VLIST_API void VLIST_CDECL delete_vlist( 
	vlist_t* vlist); 

/*!
    \brief This function adds an item to the head of a vlist structure.
	\var vlist The list operate on
	\var item A void pointer to store 
*/
VLIST_API void VLIST_CDECL vlist_add_first(
	vlist_t* vlist, 
	void* item);

/*!
    \brief This function adds an item to the tail of a vlist structure.
	\var vlist The list operate on
	\var item A void pointer to store 
*/
VLIST_API void VLIST_CDECL vlist_add_last(
	vlist_t* vlist, 
	void* item);


/*!
    \brief This function adds an item before the cursor of a vlist structure.
	\var vlist The list operate on
	\var item A void pointer to store 
*/
VLIST_API void VLIST_CDECL vlist_add_before_cursor(
	vlist_t* vlist, 
	void* item);

/*!
    \brief This function adds an item after the cursor to the vlist structure.
	\var vlist The list operate on
	\var item A void pointer to store 
*/
VLIST_API void VLIST_CDECL vlist_add_after_cursor(
	vlist_t* vlist,
	void *item);

/*!
    \brief This function removes an item from the vlist structure.
	\var vlist The list operate on
	\var item A void pointer to remove from the list 
	\return 0 if item not found ~0 if found
*/
VLIST_API int VLIST_CDECL vlist_remove(
	vlist_t* vlist, 
	void* item);

/*!
    \brief This function removes the first item from the vlist structure.
	\var vlist The list operate on
	\return 0 if vlist is empty or first item pointer
*/
VLIST_API void * VLIST_CDECL vlist_remove_first(
	vlist_t* vlist);

/*!
    \brief This function removes the next item after the cursor from the vlist structure.
	\var vlist The list operate on
	\return 0 if no such item exists else the item
*/
VLIST_API void * VLIST_CDECL vlist_remove_cursor_next(
	vlist_t* vlist);

/*!
    \brief This function removes the item at the cursor from the vlist structure.
	\var vlist The list operate on
	\return 0 if no such item exists else the item
*/
VLIST_API void * VLIST_CDECL vlist_remove_at_cursor(
	vlist_t* vlist);

/*!
    \brief This function moves the cursor to the front of the vlist structure.
	\var vlist The list operate on
*/
VLIST_API void VLIST_CDECL vlist_reset_cursor(
	vlist_t* vlist);

/*!
    \brief This function returns item at the cursor but doesn't remove it.
	\var vlist The list operate on
	\return 0 if no such item exists else the item
*/
VLIST_API void * VLIST_CDECL vlist_peek_cursor(
	vlist_t* vlist);

/*!
    \brief This function returns next item after the cursor but doesn't remove it.
	\var vlist The list operate on
	\return 0 if no such item exists else the item
*/
VLIST_API void * VLIST_CDECL vlist_peek_cursor_next(
	vlist_t* vlist);

/*!
    \brief This function returns next item's next after the cursor but doesn't remove it.
	\var vlist The list operate on
	\return 0 if no such item exists else the item
*/
VLIST_API void * VLIST_CDECL vlist_peek_cursor_next_next(
	vlist_t* vlist);

/*!
    \brief This function moves the cursor to the next postition in the vlist structure.
	\var vlist The list operate on
*/
VLIST_API void VLIST_CDECL vlist_advance_cursor(
	vlist_t* vlist);

/*!
    \brief This function returns first item on the vlist but doesn't remove it.
	\var vlist The list operate on
	\return 0 if no such item exists else the item
*/
VLIST_API void * VLIST_CDECL vlist_peek_first(
	vlist_t* vlist);

/*!
    \brief This function returns last item on the vlist but doesn't remove it.
	\var vlist The list operate on
	\return 0 if no such item exists else the item
*/
VLIST_API void * VLIST_CDECL vlist_peek_last(
	vlist_t* vlist);

/*!
    \brief This function returns number of items in the vlist.
	\var vlist The list operate on
	\return Number of items in the list.
*/
VLIST_API unsigned long VLIST_CDECL vlist_count(
	vlist_t* vlist);

/*!
    \brief This function checks the existence of a item in the vlist.
	\var vlist The list operate on
	\var item A void pointer look for
	\return 0 if item not found ~0 if found
*/
VLIST_API int VLIST_CDECL vlist_item_exists(
	vlist_t* vlist, 
	void* item);

/*!
    \brief This function writes the items in the vlist into a array doesn't change the list .
	\var vlist The list operate on
	\var items An array of void pointers the size of number of items in the list
*/
VLIST_API void VLIST_CDECL vlist_items_to_array(
	vlist_t* vlist, 
	void** items);

/*!
    \brief This function calls a function once for each item in the vlist. 
	\var vlist The list operate on
	\var function A pointer to a function taking an item and user data.
	\var user_data A pointer passed with each function call. 
*/
VLIST_API void VLIST_CDECL vlist_map_function(
	vlist_t* vlist, 
	void(*function)(void*, void*),
	void * user_data);

/*!
    \brief This function peeks at the nth item in the vlist. 
	\var vlist The list operate on
	\var index A 0 based index into the vlist
	\return 0 if no such item exists else the item
*/
VLIST_API void* VLIST_CDECL vlist_nth_item(
	vlist_t* vlist,
	unsigned long index);

/*!
    \brief This function reverses the items in the vlist.
	\var vlist The list operate on
*/
VLIST_API void VLIST_CDECL vlist_reverse(
	vlist_t* vlist);

/*!
    \brief This function wipes out all items in the list
	\var vlist The list operate on
*/
VLIST_API void VLIST_CDECL vlist_flush(
	vlist_t* vlist);


/*!
    \brief This function sort items in the list 
	\var vlist The list operate on
	\var compare The a qsort style compare function
*/
VLIST_API void VLIST_CDECL vlist_sort(
	vlist_t* vlist,
	vcompare_t compare,
	void * data);

/*!
    \brief This function adds a item to a sorted list
	\var vlist The list operate on
	\var item A void pointer to store 
	\var compare The a qsort style compare function
*/
VLIST_API void VLIST_CDECL vlist_add_sorted(
	vlist_t* vlist,
	void* item,
	vcompare_t compare,
	void * data);

VLIST_API bool VLIST_CDECL vlist_move_item(
	vlist_t* vlist,
	const int oldIdx,
	const int newIdx);

#ifndef SVIEW_DEFINED
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
#endif 


#ifndef V_POINTER_SIZED_INT
#ifdef _WIN64
#define  V_POINTER_SIZED_INT    unsigned __int64
#else
#define  V_POINTER_SIZED_INT    unsigned long
#endif
#endif

/* these are used for the internal functions */
#define VLIST_RESET_CURSOR(XL) \
	(XL)->cursor = (XL)->head; \
	(XL)->cursor_index=0; \
	(XL)->cursor_backlink=0;

#define VLIST_PEEK_CURSOR(XL) \
    ((!(XL)->cursor)?0:(XL)->cursor->item)

#define	VLIST_ADVANCE_CURSOR(XL) \
    if((XL)->cursor) {\
		(XL)->cursor_backlink=(XL)->cursor; \
		(XL)->cursor = (XL)->cursor->next; \
		(XL)->cursor_index++; \
    }

#define LIST_REMOVE_AND_CONTINUE_ITERATION(XL) \
	vlist_remove_at_cursor((XL)); \
	continue;


#define START_LIST_ITERATION(TYPE, LIST) \
				{unsigned int iTempCursor_index=iTempCursor_index = LIST->cursor_index; \
				VLIST_RESET_CURSOR(LIST);\
				while (1) {\
				TYPE *temp;\
				if (!(temp = (TYPE *)VLIST_PEEK_CURSOR(LIST)))\
					break;

#define END_LIST_ITERATION(LIST)  VLIST_ADVANCE_CURSOR(LIST); }\
	vlist_nth_item(LIST, iTempCursor_index);}

#define START_LIST_LOOP(TYPE, LIST) \
				{unsigned int iTempCursor_index=iTempCursor_index = LIST->cursor_index; \
				int iItemCount= vlist_count(LIST); \
				for(int i = 0; i < iItemCount; i++) {\
				TYPE *temp;\
				if (!(temp = (TYPE *)vlist_nth_item(LIST, i)))\
				break;

#define END_LIST_LOOP(LIST)   }\
	vlist_nth_item(LIST, iTempCursor_index);}

#ifdef _MSC_VER
#	pragma warning (disable: 4127)	/* conditional expression is constant */
#endif

/* template wrapper */
#ifdef __cplusplus
extern "C++" {

/*the following macro generates a compile-time errors
  if anyone tries to instantiate a VList with incompatible types */
#define VLIST_COMPILE_TIME_ASSERT(condition) \
	do { class message { char array[condition ? 1 : -1]; }; } while(0)	

template <class T> class VList 
#ifdef SVIEW_DEFINED
: public Controlled_Memory_Object
#endif
{
public:

	VList(struct vlist_s * vl) { m_vlist = vl;};

#ifdef SVIEW_DEFINED
	VList() { m_vlist = new_vlist(OUR(memory_pool));};

	VList(Memory_Pool * memory_pool) 
		{m_vlist = new_vlist(memory_pool);};
#else
	VList() { m_vlist = new_vlist(malloc,free);};
	
	VList(void * (VLIST_CDECL *  vlist_malloc) (size_t), void (VLIST_CDECL *vlist_free) (void *)) 
		{m_vlist = new_vlist( vlist_malloc,vlist_free);};
#endif

	virtual ~VList() {
		delete_vlist(m_vlist);
		VLIST_COMPILE_TIME_ASSERT(sizeof(T) <= sizeof(void *)); 
	};

	void AddFirst(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		vlist_add_first(m_vlist, x); 
	};

	void AddLast(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		vlist_add_last(m_vlist, x);
	};

	void AddBeforeCursor(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		vlist_add_before_cursor(m_vlist, x); 
	};

	void AddAfterCursor(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		vlist_add_after_cursor(m_vlist, x); 
	};

	int Remove(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		return (vlist_remove(m_vlist, x)); 
	};

	T RemoveFirst() { 
		void *x = vlist_remove_first(m_vlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

 	void ResetCursor() { vlist_reset_cursor(m_vlist); };

 	T PeekCursor() { 
		void *x = vlist_peek_cursor(m_vlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

 	T PeekCursorNext() { 
		void *x = vlist_peek_cursor_next(m_vlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

	T RemoveAtCursor() { 
		void *x = vlist_remove_at_cursor(m_vlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

	T RemoveCursorNext() { 
		void *x = vlist_remove_cursor_next(m_vlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

	T PeekCursorNextNext() { 
		void *x = vlist_peek_cursor_next_next(m_vlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

	void AdvanceCursor() { vlist_advance_cursor(m_vlist); };

	T PeekFirst() { 
		void *x = vlist_peek_first(m_vlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y; 
	};

 	T PeekLast() { 
		void *x = vlist_peek_last(m_vlist);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y;  
	};

 	unsigned long Count() { return (vlist_count(m_vlist)); };

	int ItemExists(T item) { 
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		return (vlist_item_exists(m_vlist, x)); 
	};

	void ItemsToArray(T* items) {
		if (sizeof(T) != sizeof(V_POINTER_SIZED_INT)) {
			V_POINTER_SIZED_INT *tmp=new V_POINTER_SIZED_INT[m_vlist->count];
			vlist_items_to_array(m_vlist, (void **)tmp);
			for(unsigned int i=0; i<m_vlist->count; i++)
				memcpy(&items[i], &tmp[i], sizeof(T));
			delete[] tmp;
		}
		else
			vlist_items_to_array(m_vlist, (void **)items);
	};

	void MapFunction(void(*function)(T, void*), void const * const user_data=0) {
		unsigned long i = Count();
		ResetCursor();
		while(i){
			(*function)(PeekCursor(),(void*)user_data);
			AdvanceCursor();
			i--;
		}
	};

	void DeleteItems() {
		unsigned long i = Count();
		ResetCursor();
		while(i){
			delete PeekCursor();
			AdvanceCursor();
			i--;
		}
	};

	T NthItem(unsigned long index) { 
		void *x = vlist_nth_item(m_vlist, index);
		T y=0;
		memcpy(&y, &x, sizeof(T));
		return y;  
	};

	
	typedef  int(*SortFunctionType)(T, T, void const * const data);
	
	void Sort(SortFunctionType compare,  void const * const data=0) { 
		SortFunctionData sfd(compare, data);
		vlist_sort(m_vlist, sort_helper, (void*)&sfd);
	};

	void AddSorted(T item, SortFunctionType compare,  void const * const data=0) { 
		SortFunctionData sfd(compare, data);
		void *x=0;
		memcpy(&x, &item, sizeof(T));
		vlist_add_sorted(m_vlist, x, sort_helper, (void*)&sfd);
	};

	void Reverse() { vlist_reverse(m_vlist); };
	
	void Flush() { vlist_flush(m_vlist); };

private:
	struct vlist_s *m_vlist;

	class SortFunctionData {
		public:	
		SortFunctionData(SortFunctionType function=0, void const * const data=0) {
			m_function=function;
			m_data=(void*)data;
		};
		SortFunctionType m_function;
		void * m_data;
	};

	static int sort_helper(const void* in_a, const void* in_b, const void * in_data) {
		SortFunctionData *data = (SortFunctionData *)in_data;
		T a=0;
		T b=0;
		memcpy(&a, &in_a, sizeof(T));
		memcpy(&b, &in_b, sizeof(T));
		return data->m_function(a,b,data->m_data);
	};

};
} /* extern "C++" */

#endif



#endif /*VLIST_H*/



