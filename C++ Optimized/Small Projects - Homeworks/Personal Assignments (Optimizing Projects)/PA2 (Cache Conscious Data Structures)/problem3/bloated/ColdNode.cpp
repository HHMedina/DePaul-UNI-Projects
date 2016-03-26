#include "ColdNode.h"
#include <string>

ColdNode::ColdNode()
	:MA(), MB(), MC(), MD(), A(), B(), C(), x(0), y(0), z(0)
{
	//memset(name,0,sizeof(name));
}
/*
ColdNode::ColdNode(const OriginalNode& rhs)
	:MA(rhs.MA), MB(rhs.MB), MC(rhs.MC), MD(rhs.MD),
	A(rhs.A), B(rhs.B), C(rhs.C), x(rhs.x), y(rhs.y), z(rhs.z)
{
	strcpy(name,rhs.name);
}
*/

ColdNode& ColdNode::operator=(const ColdNode& rhs)
{
	key = rhs.key;
	MA = rhs.MA;
	MB = rhs.MB;
	MC = rhs.MC;
	MD = rhs.MD;
	A = rhs.A;
	B = rhs.B;
	C = rhs.C;
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	memcpy(name, rhs.name, sizeof(name));	
	return *this;

}


ColdNode::~ColdNode()
{
	//nothing to deallocate
}

