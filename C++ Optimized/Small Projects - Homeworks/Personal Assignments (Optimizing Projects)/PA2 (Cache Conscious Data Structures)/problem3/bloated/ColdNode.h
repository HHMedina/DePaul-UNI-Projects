#ifndef COLDNODE_H
#define COLDNODE_H

#include "Node.h"//for struct information.

class ColdNode
{
public:
	ColdNode();
	~ColdNode();
	
	//ColdNode(const OriginalNode&);

	ColdNode& operator=(const ColdNode& );

	Matrix_t		   MA;
	Matrix_t		   MB;
	Matrix_t		   MC;
	Matrix_t		   MD;
	Vect_t			A;
	Vect_t			B;
	Vect_t			C;
	int key;
	float			   x;
	float			   y;
	float			   z;
	char			   name[NAME_SIZE];

};

#endif //COLDNODE_H