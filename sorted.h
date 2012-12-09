
/*
 sorted - Powerful sorted list in C.
 BSD license.
 by Sven Nilsen, 2012
 http://www.cutoutpro.com
 
 Version: 0.000 in angular degrees version notation
 http://isprogrammingeasy.blogspot.no/2012/08/angular-degrees-versioning-notation.html
 */
/*
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 */

/*
 
 This is a list with a sorted index attached to it.
 In the list itself, the items are stored in the order they are added.
 The sorted indices maps order to the item index.
 
 Can be used as a stack with unique elements.
 
	sorted_type(double);	// Goes into C header.
 
	sorted_methods(double);	// Goes into the .c file.
 
	double_sorted *list = double_sorted 
 
 */

#ifndef SORTED_GUARD
#define SORTED_GUARD

#include "ref.h"
#include "slice.h"

#ifndef INT_SLICE
#define INT_SLICE
slice_type(int);
#endif
// Declares the information that goes into a header.
#define sorted_type(type)\
slice_type(type);\
/* Creates the structure of the sorted list. */\
typedef struct\
{\
	ref ref; /* Uses Ref garbage collection. */\
	type##_slice items; /* Contains the items. */\
	int_slice sortedindices; /* Maps sorted order to list. */\
	int (*compare)(const type *a, const type *b);\
} type##_sorted;\
/*	Creates a new sorted list. */\
type##_sorted *type##_sorted_New\
(int capacity, int (*compare)(const type *a, const type *b));\
int type##_sorted_SortedIndex(type##_sorted *a, const type *item);\
int type##_sorted_Push(type##_sorted *a, const type *item);\
int type##_sorted_PushValue(type##_sorted *a, const type item);\
type type##_sorted_Pop(type##_sorted *a);\
int type##_sorted_Contains(type##_sorted *a, const type *b);

// Declares the stuff that is put into the .c file.
#define sorted_methods(type)\
/*	Cleans up the memory allocated by the slices.
	This method is used by Ref garbage collector.
*/\
void type##_sorted_Delete(void *ptr)\
{\
	type##_sorted *a = ptr;\
	slice_free(a->items);\
	slice_free(a->sortedindices);\
}\
/* Method declaration for the sorted list. */\
type##_sorted *type##_sorted_New\
(int capacity, int (*compare)(const type *a, const type *b))\
{\
	gcInit(type##_sorted, a, .ref.destructor = type##_sorted_Delete);\
	a->items = slice_make(type, capacity);\
	a->sortedindices = slice_make(int, capacity);\
	a->compare = compare;\
	return a;\
}\
/*	Returns the sorted index of an item.
	This is positive if the item already exists in the list.
	If the item does not exists, the negative index minus one of
	the smallest item that is larger, is returned.
	The negative index is used to insert new items correctly.

	The sorted index can be used to measure how it is related to other
	items in the same list.
*/\
int type##_sorted_SortedIndex(type##_sorted *a, const type *item)\
{\
	return slice_binarysearch_sortedindex\
	(a->items, a->sortedindices, a->compare, *item);\
}\
/*	Method for pushing items to the sorted list, like a stack. 
	Returns 0 if the item was not inserted and 1 if it was inserted.
*/\
int type##_sorted_Push(type##_sorted *a, const type *item)\
{\
	int sortedindex = type##_sorted_SortedIndex(a, item);\
	if (sortedindex >= 0) return 0;\
	/* Double the capacity when needing more space. */\
	int block = a->items.cap == 0 ? 1 : (a->items.cap << 1) - a->items.len;\
	slice_check(type, a->items, 1, block);\
	slice_check(int, a->sortedindices, 1, block);\
	slice_push(a->items, *item);\
	sortedindex = -(sortedindex+1);\
	slice_insert(a->sortedindices, sortedindex, a->items.len-1);\
	return 1;\
}\
/*	Pushes a value to the list instead by reference. */\
int type##_sorted_PushValue(type##_sorted *a, const type item)\
{\
	return type##_sorted_Push(a, &item);\
}\
/*	Pops the last item from the list. */\
type type##_sorted_Pop(type##_sorted *a)\
{\
	type b = slice_pop(a->items);\
	int sortedindex = type##_sorted_SortedIndex(a, &b);\
	slice_cut(a->sortedindices, sortedindex, sortedindex+1);\
	return b;\
}\
/*	Returns 1 if the list contains and item and 0 if it doesn't contains it.
*/\
int type##_sorted_Contains(type##_sorted *a, const type *b)\
{\
	return type##_sorted_SortedIndex(a, b) >= 0;\
}

#endif
// Used to emulate namespace aliasing.
#ifndef FUNC_GUARD
#define FUNC_GUARD
#define func(ns, n) static __typeof__(ns##_##n) * const n = ns##_##n
#endif

