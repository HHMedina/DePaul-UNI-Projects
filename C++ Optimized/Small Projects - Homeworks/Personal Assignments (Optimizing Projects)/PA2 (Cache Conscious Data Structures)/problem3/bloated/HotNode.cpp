#include "HotNode.h"
#include "ColdNode.h"
#include <string>

HotNode::HotNode()
	:cNode(0), key(0)
{
}


HotNode& HotNode::operator=(const OriginalNode& rhs)
{
	this->key = rhs.key;
	cNode->key = rhs.key;
	cNode->MA = rhs.MA;
	cNode->MB = rhs.MB;
	cNode->MC = rhs.MC;
	cNode->MD = rhs.MD;
	cNode->A = rhs.A;
	cNode->B = rhs.B;
	cNode->C = rhs.C;
	cNode->x = rhs.x;
	cNode->y = rhs.y;
	cNode->z = rhs.z;
	memcpy(cNode->name, rhs.name, sizeof(cNode->name));//copy memory block

	return *this;

}


HotNode& HotNode::operator=(const HotNode& rhs)
{
	memcpy(this,&rhs,sizeof(HotNode));

	/*
	this->key = rhs.key;
	cNode->key = rhs.key;
	cNode->MA = rhs.cNode->MA;
	cNode->MB = rhs.cNode->MB;
	cNode->MC = rhs.cNode->MC;
	cNode->MD = rhs.cNode->MD;
	cNode->A = rhs.cNode->A;
	cNode->B = rhs.cNode->B;
	cNode->C = rhs.cNode->C;
	cNode->x = rhs.cNode->x;
	cNode->y = rhs.cNode->y;
	cNode->z = rhs.cNode->z;
	memcpy(cNode->name,rhs.cNode->name,sizeof(cNode->name)); //copy memory block
	*/
	return *this;

}


const bool HotNode::operator ==(const OriginalNode& rhs)
{
	
	return ( (key == rhs.key) && (cNode->A == rhs.A) && (cNode->B == rhs.B) && (cNode->C == rhs.C) 
		&& (cNode->MA == rhs.MA) && (cNode->MB == rhs.MB) && (cNode->MC == rhs.MC) && (cNode->MD == rhs.MD)
		&& (cNode->x == rhs.x) && (cNode->y == rhs.y) && (cNode->z == rhs.z) && (memcmp(cNode->name, rhs.name, sizeof(cNode->name)) == 0) );
}


HotNode::~HotNode(){
	delete cNode;
}