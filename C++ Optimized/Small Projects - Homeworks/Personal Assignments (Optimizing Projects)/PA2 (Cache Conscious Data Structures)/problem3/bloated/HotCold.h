#ifndef HOTCOLD_H
#define HOTCOLD_H

// forward declaration
class HotNode;
class OriginalNode;
class ColdNode;

// needs to be this value, do not change in final submission
//#define NUM_NODES 1000000

class HotCold
{
public:
	HotCold(){}
	~HotCold();

	static void Convert();
	static void Verify();
	static bool findKey(int key, ColdNode&);//,  &foundNode);
	static HotNode *getListHead();
private:
	static HotNode *head;
};


#endif //bloated.h