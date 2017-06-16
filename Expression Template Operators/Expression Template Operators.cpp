// Expression Template Operators.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include <Stopwatch.h>
#include <RuntimeTest.h>
#include <Algebra\Vector_Operators.h>

#include "vector.h"
#include "operators.h"
#include "expressions.h"

std::vector<double> etree_loop_result;
std::vector<double> etree_stl_result;
std::vector<double> std_result;
std::vector<double> loop_result;

using std::endl;

#define DURATION(timer) timer.nanoseconds()

template <typename V, typename Out>
long long test(const V* _x, const V* _y, Out* output)
{
	using namespace vector_operators::vector;
	using namespace etree::operators::binary;
	using namespace etree::operators::unary;

	Stopwatch timer;

	const std::size_t N = _x->size();
	const V& x = *_x;
	const V& y = *_y;

	timer.start();
	const V result = x + y;
	timer.stop();

	*output = result;
	return DURATION(timer);
}

template <typename V, typename Out>
long long test_Loop(const V* _x, const V* _y, Out* output)
{
	typedef V::value_type T;
	Stopwatch timer;

	const std::size_t N = _x->size();
	const V& x = *_x;
	const V& y = *_y;

	timer.start();
	T* result = new T[N];
	for (unsigned int j = 0; j < N; j++)
		result[j] = -cos(x[j] + y[j]) - log(y[j] * x[j] * y[j]);

	timer.stop();

	output->resize(N);
	std::copy(result, result + N, output->begin());

	delete[] result;
	return DURATION(timer);
}

template <typename V>
void setup(const std::size_t N, V* x, V* y)
{
	typedef V::value_type T;

	x->resize(N);
	y->resize(N);

	x->assign(N, 2.1);
	y->assign(N, 2);
}

void runTests()
{
	typedef double T;

	typedef std::vector<T> Vec;

	std::vector<std::size_t> sizes = {
		static_cast<std::size_t>(1e2),
		static_cast<std::size_t>(2e3),
		static_cast<std::size_t>(1e4),
		//static_cast<std::size_t>(1e5),
		//static_cast<std::size_t>(2e5),
		//static_cast<std::size_t>(3e5),
		//static_cast<std::size_t>(4e5),
		//static_cast<std::size_t>(5e5),
		//static_cast<std::size_t>(6e5),
		//static_cast<std::size_t>(7e5),
	};

	const std::size_t N = static_cast<std::size_t>(1e6);

	std::vector<T> x, y;
	etree::vector<T> c_x, c_y;
	etree::vector<T, etree::constructors::STL> stl_x, stl_y;

	RuntimeTest etree_loop(sizes), etree__stl(sizes), std(sizes);

	etree_loop.storeSetup(setup<etree::vector<T>>, &c_x, &c_y);
	etree_loop.storeTest("Operators_ETree_Loop_times.txt", test<etree::vector<T>, Vec>, &c_x, &c_y, &etree_loop_result);
	etree_loop.runAll(10);
	etree_loop.save();

	etree__stl.storeSetup(setup<etree::vector<T, etree::constructors::STL>>, &stl_x, &stl_y);
	etree__stl.storeTest("Operators_ETree_STL_times.txt", test<etree::vector<T, etree::constructors::STL>, Vec>, &stl_x, &stl_y, &etree_stl_result);
	etree__stl.runAll(10);
	etree__stl.save();

	std.storeSetup(setup<std::vector<T>>, &x, &y);
	std.storeTest("Operators_STD_times.txt", test<std::vector<T>, Vec>, &x, &y, &std_result);
	std.storeTest("Operators_Loop_times.txt", test_Loop<std::vector<T>, Vec>, &x, &y, &loop_result);
	std.runAll(10);
	std.save();

	std::cout << "Answers are equal: " << std::to_string(
		(std_result == etree_loop_result) &&
		(std_result == etree_stl_result) &&
		(std_result == loop_result))  
		<< endl;

	std::cout << "STD: "		<< std_result[0]		<< endl
			  << "Loop: "		<< loop_result[0]		<< endl
			  << "ETree STL: "	<< etree_stl_result[0]	<< endl
			  << "ETree Loop: " << etree_loop_result[0] << endl;
}

int main()
{
	using namespace etree::operators::binary;
	using namespace etree::operators::unary;

	typedef double T;
	typedef etree::vector<T> Vec;
	const std::size_t N = 3;


	std::vector<T> x, y;
	etree::vector<T> c_x(N), c_y(N);

	c_x.assign(N, 3.1);
	x = c_x;

	std::cout << "x = " << x[0] << endl;
	runTests();
	return 0;
}

