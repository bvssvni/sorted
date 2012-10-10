
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

 This is a generic sorted list for storing pointers or data in long format.
 I want to use 2^x expansion to have few memory allocations.
 I will keep the data separate allocated from the struct to be able
 to pass around the pointer without worrying about the expansion.
 
 Sorting will be explicit but it will still work without calling 'Sort'.
 The part that is not sorted will be linearly searched,
 while the part that is sorted will be binary searched.
 
 When looking up index, it will always check the reference value of
 the pointer first to see if it is inside the data range.
 If so, it can calculate the index directly using pointer arithmetic.
 Else it does normal search.
 
 Instead of storing key/value pairs, the objects has to be key/value
 pairs themselves. This is handled by the compare function.
 
 When the positions are used up, it will expand to twice the size and
 copy the data to new memory location. It will shrink only when using
 less than half the size and 'Shrink' is called.
 
 For simple reading and saving, the sorted list will have
 dependices to other sorted lists which it will save to the same
 file before it saves itself. Basically it will dump the data with
 dependices first, so don't use this on structures with pointers.
 
 It is neither safe to keep and use either index or pointer to item.
 Instead, sort items by a unique identifier and then use this
 to refer between sorted lists. This will also make it possible to
 link dependices between lists and just save the whole thing with
 one line of code.
 
 It will also be possible to use custom allocations to speed up
 a program. By setting compare function to NULL, the blocks
 will just be put together when calling 'free'.
 
*/

typedef struct sorted sorted;
typedef struct
{
    int (*compare)(void *a, void *b); // function to compare items.
    int itemSize; // the size of an item in bytes.
    int sortedIndex; // index of last sorted item.
    unsigned long cap; // capacity.
    unsigned long len; // length.
    void *data; // contains the data.
    sorted *dependices; // array of dependices ending with NULL.
} sorted;

sorted *sorted_New
(int cap, int itemSize, sorted *dependices, int (*compare)(void *a, void *b));
int sorted_IndexOf(sorted *list, void *a); // returns index of item.
void sorted_Sort(sorted *list); // sorts the list, does searching faster.
sorted *sorted_Copy(sorted *list); // creates a copy.
sorted *sorted_CopyCascade(sorted *list); // creates a copy with dependices.
void *sorted_ByIndex(sorted *list, int index); // finds item by index.
void *sorted_Shrink(void); // shrinks to fit the capacity to items.
void sorted_Save(const char *filename); // saves data to file (with dependices).
sorted *sorted_Read(const char *filename); // reads data from file.
void sorted_Add(sorted *list, void *a); // copies item and adds it to list.
void *sorted_Blank(sorted *list); // adds empty item at end.
void sorted_Set(sorted *list, int index, void *a); // copies item.
void sorted_Insert(sorted *list, int index, void *a); // inserts a copy.
void sorted_Remove(sorted *list, int index); // removes by index.
unsigned long sorted_Length(sorted *list); // returns number of items.

void *sorted_malloc(sorted *list); // custom allocations.
void sorted_free(sorted *list, void *a); // free memory.

