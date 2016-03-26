#include <assert.h>
typedef struct LinkList
{
	struct LinkList *next;
	struct LinkList *prev;
	int* *data;
} LinkList_t;
LinkList_t *headList;

/*
target = node to remove.

Assumptions: I assumed this function would be within a linked list class, allowing
the use of the headList member;
*/

void Remove(LinkList_t* target){

	assert(target != 0 && "Null was passed in via target parameter of Remove(LinkList_t* target).");

	if (target->prev == 0){//check if this is the first node
		delete target->data;

		if (target->next != 0)
			target->next->prev = 0;

		headList = target->next;//next node will be the new head
	}
	else{
		delete target->data;
		target->prev->next = target->next;//the previous node's next will be the node after the target

		if (target->next != 0){//if there is a node following target
			target->next->prev = target->prev;//set the following node's prev to the preceding node before target
		}
	}

	delete target;//clean up the removed node
}