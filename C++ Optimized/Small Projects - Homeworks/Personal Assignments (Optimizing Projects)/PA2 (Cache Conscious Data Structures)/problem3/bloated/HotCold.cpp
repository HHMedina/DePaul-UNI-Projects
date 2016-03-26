#include "HotCold.h"
#include "bloated.h"
#include "HotNode.h"
#include "ColdNode.h"
#include "Node.h" // just added

#include <iostream>

HotNode* HotCold::head = 0;

HotCold::~HotCold()
{
	delete head;
}

void HotCold::Convert(){

	//Create HotNodes close together;
	head = new HotNode[NUM_NODES];
	ColdNode* cNodes = new ColdNode[NUM_NODES];
	
	HotNode* temp = head;
	ColdNode* cTemp = cNodes;
	for (int i = 0; i < NUM_NODES;i++){
		temp->next = temp + 1;
		temp->prev = temp - 1;
		if (i== NUM_NODES-1){
			int j = 0;
		}
		temp->cNode = cNodes+i;
		//cNodes++;
		temp++;

	}
	head->prev = 0;
	head[NUM_NODES - 1].next = 0;

	OriginalNode* bHead = Bloated::getListHead();
	HotNode * it = head;
	
	while (bHead != 0)//while there are nodes
	{
		*it = *bHead;
		bHead = bHead->next;
		it = it->next;
	}
	
	/*
	
	OriginalNode* bHead = Bloated::getListHead();
	int currentNodeIndex = 1;
	HotNode * it = head;

	//fix the first node.
	it->prev = 0;
	*it = *bHead; // copy original head info to Hot Node (and cold node)
	it->next = &head[currentNodeIndex];
	it = it->next;

	bHead = bHead->next; //go to second node

	//Create middle nodes
	while (bHead != 0)//while there are nodes
	{
		it->prev = &head[currentNodeIndex];
		*it = *bHead;

		if (currentNodeIndex<NUM_NODES-1){
			currentNodeIndex++;
			it->next = &head[currentNodeIndex];
			it = it->next;
		}
		bHead = bHead->next;
	}
	if (currentNodeIndex>=NUM_NODES - 1){
		int test = 0;
	}
	it->next = 0;

	//make last node point to 0
	
	*/
}

void HotCold::Verify()
{
	OriginalNode* bPtr = Bloated::getListHead();
	HotNode* hcPtr = head;

	while (bPtr != 0)
	{
		if ((*hcPtr) == (*bPtr))
		{
			
		}
		else{
			printf("Not the same!!!");
		}

		bPtr = bPtr->next;
		hcPtr = hcPtr->next;
	}
}

bool HotCold::findKey(int key,ColdNode& coldOut)
{
	HotNode* temp = head;

	while (temp != 0)
	{
		if (key==temp->key)
		{
			coldOut = *(temp->cNode);
			return true;
		}
		else{
			temp = temp->next;
		}
	}
	return false;
}