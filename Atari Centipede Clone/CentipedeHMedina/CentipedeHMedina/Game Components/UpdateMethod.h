#ifndef UPDATEMETHOD_H
#define UPDATEMETHOD_H

class UpdateMethod{

	UpdateMethod(){}
	UpdateMethod(const UpdateMethod&){}
	UpdateMethod& operator =(const UpdateMethod&){}

	void Update()=0;
};




#endif UPDATEMETHOD_H