#ifndef PA3_B_H
#define PA3_B_H

#define RVO_Enable 1 //toggle to see performance difference. 0 = no optimization

class Vect2D
{
public:

	// default constructor
	Vect2D()
		: x(0.0f), y(0.0f)
	{
	};

	// Overloaded constructor - added in order to have RVO clean code.
	Vect2D(const float _x, const float _y)
		: x(_x), y(_y)
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



	// set functions
	void setX( const float inX )
	{
		this->x = inX;
	};

	void setY( const float inY )
	{
		this->y = inY;
	};

	void set( const float inX, const float inY )
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
	// Convert these methods to Return Value Optimization
	//*********************************************************************

	Vect2D operator + ( const Vect2D &tmp ) const;
	Vect2D operator - ( const Vect2D &tmp ) const;
	Vect2D operator * ( const float scale ) const;
	Vect2D operator / ( const float scale ) const;


private:
	float x;
	float y;

};

#endif