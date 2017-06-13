// Expression Template Operators.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include <Stopwatch.h>
#include <RuntimeTest.h>
#include <Algebra\Vector_Operators.h>

#include "vector.h"
#include "operators.h"
#include "expressions.h"

etree::vector<double> custom_result;
std::vector<double> std_result;

using std::endl;

template <typename V>
long long test(const V* _x, const V* _y)
{
	using namespace vector_operators::vector;
	using namespace etree::operators::binary;
	using namespace etree::operators::unary;

	Stopwatch timer;

	const std::size_t N = _x->size();
	const V& x = *_x;
	const V& y = *_y;

	timer.start();
	const V result = -x + y;
	timer.stop();

	custom_result = result;
	return timer.milliseconds();
}

template <typename V>
long long test_STD(const V* _x, const V* _y)
{
	using namespace vector_operators::vector;
	using namespace etree::operators::binary;
	using namespace etree::operators::unary;

	Stopwatch timer;

	const std::size_t N = _x->size();
	const V& x = *_x;
	const V& y = *_y;

	timer.start();
	const V result = -x + y;
	timer.stop();

	std_result = result;
	return timer.milliseconds();
}

template <typename V>
long long test_Loop(const V* _x, const V* _y)
{
	typedef V::value_type T;
	Stopwatch timer;

	const std::size_t N = _x->size();
	const V& x = *_x;
	const V& y = *_y;

	timer.start();
	T* result = new T[N];
	for (unsigned int j = 0; j < N; j++)
		result[j] = -x[j] + y[j];

	timer.stop();

	delete[] result;
	return timer.milliseconds();
}

template <typename V>
void setup(const std::size_t N, V* x, V* y)
{
	typedef V::value_type T;

	x->resize(N);
	y->resize(N);

	x->assign(N, 2);
	y->assign(N, 2);
}

void runTests()
{
	typedef double T;

	std::vector<std::size_t> sizes = {
		static_cast<std::size_t>(1e2),
		static_cast<std::size_t>(2e3),
	};

	const std::size_t N = static_cast<std::size_t>(1e6);

	std::vector<T> x, y;
	etree::vector<T> c_x, c_y;

	RuntimeTest custom(sizes), std(sizes);
	custom.storeSetup(setup<etree::vector<T>>, &c_x, &c_y);
	custom.storeTest("Operators_Custom_times.txt", test<etree::vector<T>>, &c_x, &c_y);
	custom.runAll(10);
	custom.save();

	std.storeSetup(setup<std::vector<T>>, &x, &y);
	std.storeTest("Operators_STD_times.txt", test_STD<std::vector<T>>, &x, &y);
	std.storeTest("Operators_Loop_times.txt", test_Loop<std::vector<T>>, &x, &y);
	std.runAll(10);
	std.save();

	std::cout << "Answers are equal: " << std::to_string(std_result == custom_result) << endl;
	std::cout << "Answers: " << std_result[0] << endl
		<< custom_result[0] << endl;
}

int main()
{
	using namespace etree::operators::binary;
	using namespace etree::operators::unary;

	typedef double T;
	const std::size_t N = 3;

	etree::vector<T> x(N), y(N), z(N);
	x.assign(N, 3);
	y.assign(N, 2);
	z.assign(N, -1);

	etree::vector<T> result = x + y + z;
	etree::expressions::Binary<T, std::vector<T>, std::vector<T>, std::plus<T>> e;

	result.begin();
	std::cout << "Result[0] = " << result[0] << endl;
	system("pause");
	return 0;
}

