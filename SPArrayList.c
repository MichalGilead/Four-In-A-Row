/*
 * SPArrayList.c
 *
 *  Created on: 10 ���� 2017
 *      Author: michal
 */

#include "SPArrayList.h"


SPArrayList* spArrayListCreate(int maxSize){
	if (maxSize <= 0 )
		return NULL;

	SPArrayList* history = (SPArrayList*)malloc(sizeof(SPArrayList));//initializes arrayList
	if (history == NULL)//malloc error
		return NULL;

	history->elements = (int*)malloc(maxSize*sizeof(int));//initializes arrayLists elements
	if (history->elements==NULL){//malloc error
		free(history);
		return NULL;
	}

	history->actualSize = 0;//initializes arrayLists actualSize
	history->maxSize = maxSize;//initializes arrayLists maxSize according to given argument
	return history;
}

SPArrayList* spArrayListCopy(SPArrayList* src){
	if (src == NULL)
		return NULL;

	SPArrayList* copy = spArrayListCreate(src->maxSize);//initializes arrayList with given arrayLists maxSize
	if (copy == NULL)//malloc error
		return NULL;

	for (int i=0;i<src->actualSize;i++)//copies given arrayList to "copy"
		spArrayListAddLast(copy, spArrayListGetAt(src, i));

	return copy;
}

void spArrayListDestroy(SPArrayList* src){
	if (src==NULL)
		return;
	if (src->elements!=NULL)
		free(src->elements);
	free(src);
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	if (src==NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	src->actualSize = 0;//clear's list by changing actualSize to 0
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index){
	if (src==NULL || index<0 || index > src->actualSize)//if index is out of bound or src==NULL returns an "invalid" message
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (spArrayListIsFull(src))//if arrayList is full returns "full" message
			return SP_ARRAY_LIST_FULL;
	if (index < src->actualSize)//if index is less then actualSize
		for (int i=src->actualSize-1; i>=index; i--)//moves all other elements accordingly
			src->elements[i+1] = src->elements[i];
	src->elements[index] = elem;//adds new element in given index
	src->actualSize++;//increase actual size by 1
	return SP_ARRAY_LIST_SUCCESS;//returns "success" message
}

 SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem){
		return spArrayListAddAt(src, elem, 0);//adds to first by spArrayListAddAt on index 0
	}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem){
	if (src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	return spArrayListAddAt(src, elem, src->actualSize);//adds to last by spArrayListAddAt on index actualSize
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	if (src==NULL)
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (spArrayListIsEmpty(src))//if arrayList is empty returns "empty" message
			return SP_ARRAY_LIST_EMPTY;
	if (index<0 || index >= src->actualSize)//if index is out of bound or src==NULL returns an "invalid" message
			return SP_ARRAY_LIST_INVALID_ARGUMENT;
	if (index == src->actualSize-1)//if index is of last element - removes it
		src->elements[index] = 0;
	else{ //index is less then actualSize
		for (int i=index; i<src->actualSize-1; i++)//moves other elements over to the given index and changes this index element to be the next
			src->elements[i] = src->elements[i+1];
		src->elements[src->actualSize-1] = 0;
	}
	src->actualSize--;//reduces actualSize by 1
	return SP_ARRAY_LIST_SUCCESS;// returns "success" message
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src, 0);//removes from first by spArrayListRemoveAt on index 0
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	if (src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	return spArrayListRemoveAt(src, src->actualSize-1);//removes from last by spArrayListRemoveAt on index actualSize-1
}

int spArrayListGetAt(SPArrayList* src, int index){
	if (src==NULL || index<0 || index >= src->actualSize)//if index is out of bound or src==NULL returns -1
		return -1;
	return src->elements[index];//returns the element in the given index
}

int spArrayListGetFirst(SPArrayList* src){
	return spArrayListGetAt(src, 0);//gets the first element by spArrayListGetAt on index 0
}

int spArrayListGetLast(SPArrayList* src){
	if (src == NULL) {
		return -1;
	}
	return spArrayListGetAt(src, src->actualSize-1);//gets the last element by spArrayListGetAt on index actualSize-1
}

int spArrayListMaxCapacity(SPArrayList* src){
	if( src==NULL)
		return -1;
	return src->maxSize; //returns maxSize
}

int spArrayListSize(SPArrayList* src){
	if( src==NULL)
			return -1;
	return src->actualSize;//returns actualSize
}

bool spArrayListIsFull(SPArrayList* src){
	if (src == NULL) {
		return false;
	}
	return (src->actualSize==src->maxSize);//returns true if full, else false
}

bool spArrayListIsEmpty(SPArrayList* src){
	if (src == NULL) {
		return false;
	}
	return (src->actualSize==0);//returns true if empty, else false
}
