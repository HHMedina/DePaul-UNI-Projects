#include <vector>
#include <assert.h>

template <typename T>
class Stack
{

public:
	// Write the following functions
	void push(T const & data)
	{
		elems.push_back(data);

	}      // push element
	void pop()
	{
		elems.pop_back();
	}               // pop element
	T top() const
	{
		return elems.back();
	}// return top element
	bool empty() const
	{
		return elems.empty();
	}// return whether the stack is empty

private:
	std::vector<T> elems;     // elements
};

template <>
class Stack<float>
{

public:
	// Write the following functions
	void push(float const & data)
	{
		elems.push_back(data);
		elems.push_back(data);

	}      // push element
	void pop()
	{
		elems.pop_back();
	}               // pop element
	float top() const
	{
		return elems.back();
	}// return top element
	bool empty() const
	{
		return elems.empty();
	}// return whether the stack is empty

private:
	std::vector<float> elems;     // elements
};

// Write a full class specialization for float
// in this class when you push() a float it double pushes
// in this class when you pop() it pops a singe element off the stack
// top() and empty() behave the same.

void F()
{
	// Cut & Paste from E.cpp for the general class template

	// Write a full class specialization for float
	// in this class when you push() a float it double pushes
	// in this class when you pop() it pops a singe element off the stack
	// top() and empty() behave the same.

// Do not change the code below -----------------------

			int val;
			bool bval;
			Stack<int> Stack_int;

			bval = Stack_int.empty();
			assert( bval == true );

			Stack_int.push( 5 );
			val = Stack_int.top();
			assert( val == 5 );

			bval = Stack_int.empty();
			assert( bval == false );

			Stack_int.push( 6 );
			val = Stack_int.top();
			assert( val == 6);

			Stack_int.push( 7 );
			val = Stack_int.top();
			assert( val == 7 );

			Stack_int.push( 8 );
			val = Stack_int.top();
			assert( val == 8 );

			Stack_int.pop();
			val = Stack_int.top();
			assert( val == 7 );

			Stack_int.pop();
			val = Stack_int.top();
			assert( val == 6);

			Stack_int.pop();
			val = Stack_int.top();
			assert( val == 5 );

			bval = Stack_int.empty();
			assert( bval == false );

			Stack_int.pop();
			bval = Stack_int.empty();
			assert( bval == true);

			// ----- float ---------------

			float fval;
			Stack<float> Stack_float;

			bval = Stack_float.empty();
			assert( bval == true );

			Stack_float.push( 55.0f );
			fval = Stack_float.top();
			assert( fval == 55.0f );

			bval = Stack_float.empty();
			assert( bval == false );

			Stack_float.push( 88.0f );
			fval = Stack_float.top();
			assert( fval == 88.0f );

			// ---- start to pop -------------
			Stack_float.pop();
			fval = Stack_float.top();
			assert( fval == 88.0f );

			bval = Stack_float.empty();
			assert( bval == false );

			Stack_float.pop();
			fval = Stack_float.top();
			assert( fval == 55.0f );

			bval = Stack_float.empty();
			assert( bval == false );

			Stack_float.pop();
			fval = Stack_float.top();
			assert( fval == 55.0f );

			bval = Stack_float.empty();
			assert( bval == false );

			Stack_float.pop();
			bval = Stack_float.empty();
			assert( bval == true );



// Do not change the code above -----------------------

}