// Test structures and classes for 2
// Enjoy
// - Ed Keenan

namespace ReworkedDataStructs{

	struct Test_A
	{
		Test_A()
			:r(0), g(0), b(0)
		{

		}

		char r;		
		char g;		
		char b;		
	};


	struct Test_B
	{
		Test_B()
			:x(0), y(0)
		{

		}
		int   x;		
		int   y;		
	};


	class Test_C{
	public:
		Test_C()
			:x(0), b(0)
		{

		}
	private:
		int   x;	 
		char  b;
	};

	class Test_D
	{
	public:
		Test_D()
			:c(0), x(0), b(0)
		{
		}
	private:
		int   x;	 
		char  b;
		char  c;
	};

	struct Test_E
	{
		Test_E()
			:p(0), x(0), y(0), a(0), b(0), c(0)
		{

		}
		void  *p; //4 bytes   
		float x; //4 bytes    
		int   y; //4 bytes    
		char  a; //1 bytes   
		char  b; //1 bytes   
		char  c; //1 bytes
	};

	struct Test_F
	{
		Test_F()
			:t(0), s(0), a(0)
		{

		}
		double t; //8 bytes
		float  s; //4bytes
		char   a; //1bytes
	};

	struct Test_G
	{
		Test_G()
			:t(0), s(0), a(0)
		{

		}
		double t;    
		float  s;    
		char   a;
	};

	struct vect
	{
		vect()
			:vx(0), vy(0), vz(0), vw(0), d(0)
		{

		}
		float vx; //4
		float vy; //4
		float vz; //4
		float vw; //4
		char  d; //1
	};

	struct Test_H
	{
		Test_H()
			:v1(), a(0), b(0), c(0)
		{

		}

		vect  v1; //20 bytes
		int   a; //4
		int   b; //4
		char  c; // 1
	};

	class Test_I
	{
	public:
		Test_I()
			:x(0), p(0), z(0)
		{
			memset(name, 0, sizeof(name));
		}
	private:
		int   x; //4 bytes
		int   *p; //4 bytes
		char  name[63]; // 63 * 1 byte chars
		char	z; // one byte

	};

	struct Test_J
	{
		Test_J()
			:d(0), x(0), y(0), p(0), z(0), c(0)
		{
			memset(name, 0, sizeof(name));
		}

		double   d; //8
		int      x; //4
		float	   y; //4
		int		*p; //4
		char	   name[122]; // 122 * 1 byte 
		char	   z; //1	
		char	   c; //1
	};

}