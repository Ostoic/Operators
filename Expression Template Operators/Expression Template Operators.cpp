// Expression Template Operators.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "operators.h"
#include "Vec.h"

using std::endl;

int main()
{
	using namespace agac::expressions::operators::binary;
	using namespace agac::expressions::operators::unary;

	typedef double T;
	Vec<T> a(4), b(4), c(4);

	a.assign(3, -1);
	b.assign(3, 0);
	c.assign(3, 1);

	//Vec<T> x = a + b + c;
	Vec<T> y = 2 * a;

	//std::cout << x[0] << endl;
	std::cout << y[0] << endl;
	return 0;
}

