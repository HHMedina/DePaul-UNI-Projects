#ifndef HOTNODE_H
#define HOTNODE_H
#include "Node.h" // for struct info and original node type

//forward declarations
class ColdNode;

class HotNode
{
public:
	HotNode();
	~HotNode();

	HotNode& operator =(const HotNode&);

	HotNode& operator= (const OriginalNode& rhs);

	const bool operator ==(const OriginalNode& rhs);

	HotNode* next;
	HotNode* prev;
	int key;
	ColdNode* cNode;

};

#endif //HOTNODE_H