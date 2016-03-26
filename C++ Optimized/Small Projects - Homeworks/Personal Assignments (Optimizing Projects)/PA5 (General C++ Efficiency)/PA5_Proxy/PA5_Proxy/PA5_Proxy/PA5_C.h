#ifndef PA3_C_H
#define PA3_C_H

#define PROXY_ENABLE 1  // use this toggle to flip back and forth with your proxies


class Vect2D
{
public:

	//***********************
	// Big Four
	//***********************

	// default constructor
	Vect2D()
		: x(0.0f), y(0.0f)
	{
	};

	//ctor
	Vect2D(const Vect2D& v2D)
		:x(v2D.getX()), y(v2D.getY())
	{}

	//assignment op
	Vect2D& operator =(const Vect2D& rhs)
	{
		this->x = rhs.getX();
		this->y = rhs.getY();
		return *this;
	}

	// destructor
	~Vect2D()
	{
	};
	//*******End Big Four*******


	//added overload operator
	Vect2D(float _x, float _y)
		:x(_x), y(_y)
	{}

	// set functions
	void setX(const float inX)
	{
		this->x = inX;
	};

	void setY(const float inY)
	{
		this->y = inY;
	};

	void set(const float inX, const float inY)
	{
		this->x = inX;
		this->y = inY;
	};

	float getX() const
	{
		return this->x;
	};

	float getY() const
	{
		return this->y;
	};

	//*********************************************************************
	// Convert these methods to proxys
	//*********************************************************************
#if !PROXY_ENABLE
	Vect2D operator + (const Vect2D &tmp) const;
#endif
private:
	float x;
	float y;


};


#if PROXY_ENABLE

//VAddV Vect + Vect
class VAddV
{
public:

	const Vect2D& V1;
	const Vect2D& V2;

	VAddV(const Vect2D& t1, const Vect2D& t2)
		:V1(t1), V2(t2)
	{
	}

	operator Vect2D()  //conversion to Vect2D (final step)
	{
		return Vect2D(V1.getX() + V2.getX(),
			V1.getY() + V2.getY());
	}


	~VAddV(){}

private:
	VAddV();
};

inline VAddV operator + (const Vect2D& t1, const Vect2D& t2)
{
	return VAddV(t1, t2);
}

//VAddVAddV VaddV + Vect
class VAddVAddV
{
public:

	const Vect2D& V1;
	const Vect2D& V2;
	const Vect2D& V3;

	VAddVAddV(const VAddV& _vav, const Vect2D& _v)
		:V1(_vav.V1), V2(_vav.V2), V3(_v)
	{
	}

	operator Vect2D() //conversion to Vect2D (final step)
	{
		return Vect2D(V1.getX() + V2.getX() + V3.getX(),
			V1.getY() + V2.getY() + V3.getY());
	}


	~VAddVAddV(){}

private:
	VAddVAddV();
};

inline VAddVAddV operator + (const VAddV& vav, const Vect2D& v)
{
	return VAddVAddV(vav, v);
}


//VAddVAddVAddV VaddVaddV + Vect
class VAddVAddVAddV
{
public:

	const Vect2D& V1;
	const Vect2D& V2;
	const Vect2D& V3;
	const Vect2D& V4;

	VAddVAddVAddV(const VAddVAddV& _vavav, const Vect2D& _v)
		:V1(_vavav.V1), V2(_vavav.V2), V3(_vavav.V3), V4(_v)
	{
	}

	operator Vect2D() //conversion to Vect2D (final step)
	{
		return Vect2D(V1.getX() + V2.getX() + V3.getX() + V4.getX(),
			V1.getY() + V2.getY() + V3.getY() + V4.getY());
	}


	~VAddVAddVAddV(){}

private:
	VAddVAddVAddV();
};

inline VAddVAddVAddV operator + (const VAddVAddV& vav, const Vect2D& v)
{
	return VAddVAddVAddV(vav, v);
}


//VAddVAddVAddVAddV VaddVaddVAddV + Vect
class VAddVAddVAddVAddV
{
public:

	const Vect2D& V1;
	const Vect2D& V2;
	const Vect2D& V3;
	const Vect2D& V4;
	const Vect2D& V5;

	VAddVAddVAddVAddV(const VAddVAddVAddV& _vavavav, const Vect2D& _v)
		:V1(_vavavav.V1), V2(_vavavav.V2), V3(_vavavav.V3), V4(_vavavav.V4), V5(_v)
	{
	}

	operator Vect2D()  //conversion to Vect2D (final step)
	{
		return Vect2D(V1.getX() + V2.getX() + V3.getX() + V4.getX() + V5.getX(),
			V1.getY() + V2.getY() + V3.getY() + V4.getY() + V5.getY());
	}


	~VAddVAddVAddVAddV(){}

private:
	VAddVAddVAddVAddV();
};

inline VAddVAddVAddVAddV operator + (const VAddVAddVAddV& vavavav, const Vect2D& v)
{
	return VAddVAddVAddVAddV(vavavav, v);
}

#endif //proxy_enable


#endif