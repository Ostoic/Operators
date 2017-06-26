// Expression Template Operators.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include <Stopwatch.h>
#include <RuntimeTest.h>
#include <Algebra\Vector_Operators.h>

#include "iterators.h"
#include "vector.h"
#include "operators.h"
#include "expressions.h"

std::vector<double> vap_loop_result;
std::vector<double> vap_stl_result;
std::vector<double> std_result;
std::vector<double> loop_result;

const double pi = 3.14159265358979323846;
const double pi2 = pi*pi;

using std::endl;

#define DURATION(timer) timer.nanoseconds().to_seconds()

template <typename V, typename Out>
double test(const V* _x, const V* _y, Out* output)
{
	using namespace vector_operators::vector;
	using namespace vap::operators::binary;
	using namespace vap::operators::unary;

	Stopwatch timer;

	const std::size_t N = _x->size();
	const V& x = *_x;
	const V& y = *_y;

	V v_1(N), v_2(N), v_pi(N), v_pi2(N);
	v_1.assign(N, 1);
	v_2.assign(N, 2);
	v_pi.assign(N, pi);
	v_pi2.assign(N, pi2);

	timer.start();
	const V result = - (v_2 + v_pi2 * x * (v_1 - x)) * cos (v_pi * y);
	timer.stop();

	*output = result;
	return DURATION(timer);
}

template <typename V, typename Out>
double test_Loop(const V* _x, const V* _y, Out* output)
{
	typedef V::value_type T;
	Stopwatch timer;

	const std::size_t N = _x->size();
	const V& x = *_x;
	const V& y = *_y;

	timer.start();
	T* result = new T[N];
	for (unsigned int j = 0; j < N; j++)
		result[j] = - (2 + pi2 * x[j] * (1 - x[j])) * cos (pi * y[j]);

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
		static_cast<std::size_t>(1e4),
		static_cast<std::size_t>(1e5),
		static_cast<std::size_t>(2e5),
		static_cast<std::size_t>(3e5),
		static_cast<std::size_t>(4e5),
		static_cast<std::size_t>(5e5),
		static_cast<std::size_t>(6e5),
		static_cast<std::size_t>(7e5),
		static_cast<std::size_t>(1e6),
		static_cast<std::size_t>(2e6),
		static_cast<std::size_t>(3e6),
		static_cast<std::size_t>(4e6),
		static_cast<std::size_t>(5e6),
	};

	const std::size_t N = static_cast<std::size_t>(1e6);

	std::vector<T> x, y;
	vap::vector<T> c_x, c_y;
	vap::vector<T, vap::constructors::STL> stl_x, stl_y;

	RuntimeTest<double> vap_loop(sizes), vap_stl(sizes), vap_thrust(sizes), std(sizes);

	vap_loop.storeSetup(setup<vap::vector<T>>, &c_x, &c_y);
	vap_loop.storeTest("Operators_vap_Loop_times.txt", test<vap::vector<T>, Vec>, &c_x, &c_y, &vap_loop_result);
	vap_loop.runAll(10);
	vap_loop.save();

	vap_stl.storeSetup(setup<vap::vector<T, vap::constructors::STL>>, &stl_x, &stl_y);
	vap_stl.storeTest("Operators_vap_STL_times.txt", test<vap::vector<T, vap::constructors::STL>, Vec>, &stl_x, &stl_y, &vap_stl_result);
	vap_stl.runAll(10);
	vap_stl.save();
			
	/*vap_thrust.storeSetup(setup<vap::vector<T, vap::constructors::STL>>, &stl_x, &stl_y);
	vap_thrust.storeTest("Operators_vap_STL_times.txt", test<vap::vector<T, vap::constructors::STL>, Vec>, &stl_x, &stl_y, &vap_stl_result);
	vap_thrust.runAll(10);
	vap_thrust.save();*/

	std.storeSetup(setup<std::vector<T>>, &x, &y);
	std.storeTest("Operators_STD_times.txt", test<std::vector<T>, Vec>, &x, &y, &std_result);
	std.storeTest("Operators_Loop_times.txt", test_Loop<std::vector<T>, Vec>, &x, &y, &loop_result);
	std.runAll(10);
	std.save();

	std::cout << "Equality of Answers:"
			  << endl
			  << "loop_result == vap_loop_result: " 
			  <<  std::to_string(loop_result == vap_loop_result) 
			  << endl

			  << "loop_result == vap_stl_result: " 
			  <<  std::to_string(loop_result == vap_stl_result) 
			  << endl

			  << "loop_result == std_result: " 
			  <<  std::to_string(loop_result == std_result) 
			  << endl;

	std::cout << "STD: "		<< std_result[900]			<< endl
			  << "Loop: "		<< loop_result[900]			<< endl
			  << "vap STL: "	<< vap_stl_result[900]	<< endl
			  << "vap Loop: " << vap_loop_result[900]	<< endl;

	std::cout << "Result vector sizes:"		<< endl;
	std::cout << "loop_result size: "		<< loop_result.size()		<< endl;
	std::cout << "std_result size: "		<< std_result.size()		<< endl;
	std::cout << "vap_loop_result size: " << vap_loop_result.size() << endl;
	std::cout << "vap_stl_result size: "	<< vap_stl_result.size()	<< endl;
}

int main()
{
	using namespace vap::operators::binary;
	//using namespace vap::operators::unary;

	typedef double T;
	typedef vap::vector<T> Vec;
	const std::size_t N = 3;

	std::vector<T> x, y;

	vap::vector<T, vap::constructors::STL> c_x(N), c_y(N), c_z(N);
	c_x.assign(N, 3);
	c_y.assign(N, -1);
	c_z.assign(N, 4);

	runTests();
	system("pause");
	return 0;
}

