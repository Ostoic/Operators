// Expression Template Operators.cpp : Defines the entry point for the console application.

#include "cuda_runtime.h"
#include <iostream>

#include <Stopwatch.h>
#include <RuntimeTest.h>
#include <Algebra\Vector_Operators.h>

#include <thrust\device_vector.h>
#include "DeviceOperators.h"

#include "iterators.h"
#include "vector.h"
#include "operators.h"
#include "expressions.h"

thrust::device_vector<double> etree_thrust_result;
thrust::device_vector<double> thrust_std_result;
std::vector<double> etree_loop_result;
std::vector<double> etree_stl_result;
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
	using namespace thrust_operators::vector;
	using namespace etree::operators::binary;
	using namespace etree::operators::unary;

	Stopwatch timer;

	const std::size_t N = _x->size();
	const V& x = *_x;
	const V& y = *_y;

	timer.start();
	const V result = cos(y)*x + y - x;
	timer.stop();

	*output = result;
	return DURATION(timer);
}

template <typename V, typename Out>
double test_Loop(const V* _x, const V* _y, Out* output)
{
	using T = typename V::value_type;

	Stopwatch timer;

	const std::size_t N = _x->size();
	const V& x = *_x;
	const V& y = *_y;

	timer.start();
	T* result = new T[N];
	for (unsigned int j = 0; j < N; j++)
		result[j] = y[j] * x[j] + y[j] - x[j];

	timer.stop();

	output->resize(N);
	std::copy(result, result + N, output->begin());

	delete[] result;
	return DURATION(timer);
}

template <typename V>
void setup(const std::size_t N, V* x, V* y)
{
	using T = typename V::value_type;

	x->resize(N);
	y->resize(N);

	x->assign(N, -316376.1);
	y->assign(N, 362178);
}

void runTests()
{
	typedef double T;

	typedef std::vector<T> Vec;
	typedef thrust::device_vector<T> DVec;

	std::vector<std::size_t> sizes = {
		static_cast<std::size_t>(1e1),
		static_cast<std::size_t>(1e2),
		static_cast<std::size_t>(1e3),
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
		static_cast<std::size_t>(3e6),/*
		static_cast<std::size_t>(4e6),
		static_cast<std::size_t>(5e6),
		static_cast<std::size_t>(1e7),
		static_cast<std::size_t>(2e7),
		static_cast<std::size_t>(3e7),*/
	};

	Vec x, y;
	DVec d_x, d_y;
	etree::vector<T> c_x, c_y;
	etree::vector<T, etree::constructors::STL> stl_x, stl_y;
	etree::vector<T, etree::constructors::Thrust, DVec, etree::parallel_policy> ed_x, ed_y;

	RuntimeTest<double> etree_loop(sizes), etree_stl(sizes), etree_thrust(sizes), thrust_std(sizes), std(sizes);

	etree_loop.storeSetup(setup<decltype(c_x)>, &c_x, &c_y);
	etree_loop.storeTest("Operators_ETree_Loop_times.txt", test<decltype(c_x), Vec>, &c_x, &c_y, &etree_loop_result);
	etree_loop.runAll(10);
	etree_loop.save();

	//etree_stl.storeSetup(setup<decltype(stl_x)>, &stl_x, &stl_y);
	//etree_stl.storeTest("Operators_ETree_STL_times.txt", test<decltype(stl_x), Vec>, &stl_x, &stl_y, &etree_stl_result);
	//etree_stl.runAll(10);
	//etree_stl.save();

	thrust_std.storeSetup(setup<decltype(d_x)>, &d_x, &d_y);
	thrust_std.storeTest("Operators_Thrust_Std_times.txt", test<decltype(d_x), DVec>, &d_x, &d_y, &thrust_std_result);
	thrust_std.runAll(10);
	thrust_std.save();

	etree_thrust.storeSetup(setup<decltype(ed_x)>, &ed_x, &ed_y);
	etree_thrust.storeTest("Operators_ETree_Thrust_times.txt", test<decltype(ed_x), DVec>, &ed_x, &ed_y, &etree_thrust_result);
	etree_thrust.runAll(10);
	etree_thrust.save();

	std.storeSetup(setup<Vec>, &x, &y);
	std.storeTest("Operators_STD_times.txt", test<Vec, Vec>, &x, &y, &std_result);
	std.storeTest("Operators_Loop_times.txt", test_Loop<Vec, Vec>, &x, &y, &loop_result);
	std.runAll(10);
	std.save();

	Vec etree_thrust_host(etree_thrust_result.size());
	Vec thrust_std_host(etree_thrust_result.size());

	thrust::copy(etree_thrust_result.begin(), etree_thrust_result.end(), etree_thrust_host.begin());
	thrust::copy(thrust_std_result.begin(), thrust_std_result.end(), thrust_std_host.begin());

	std::cout << "Equality of Answers:"
		<< endl
		<< "loop_result == etree_loop_result: "
		<< std::to_string(loop_result == etree_loop_result)
		<< endl

		/*<< "loop_result == etree_stl_result: "
		<< std::to_string(loop_result == etree_stl_result)
		<< endl*/

		<< "loop_result == etree_thrust_result: "
		<< std::to_string(loop_result == thrust_std_host)
		<< endl

		<< "loop_result == thrust_std_result: "
		<< std::to_string(loop_result == etree_thrust_host)
		<< endl

		<< "loop_result == std_result: "
		<< std::to_string(loop_result == std_result)
		<< endl;

	std::cout << "STD: "		<< std_result[900]		  << endl
			  << "Loop: "		<< loop_result[900]		  << endl
			  << "Thrust STD: " << thrust_std_host[900] << endl
			  << "ETree Thrust: " << etree_thrust_host[900] << endl
			  //<< "ETree STL: "	<< etree_stl_result[900]  << endl
			  << "ETree Loop: " << etree_loop_result[900] << endl;

	std::cout << "Result vector sizes:"		<< endl;
	std::cout << "loop_result size: "		<< loop_result.size() << endl;
	std::cout << "std_result size: "		<< std_result.size() << endl;
	std::cout << "etree_loop_result size: " << etree_loop_result.size() << endl;
	std::cout << "thrust_std_host size: " << thrust_std_host.size() << endl;
	std::cout << "etree_thrust_result size: " << etree_thrust_host.size() << endl;
	//std::cout << "etree_stl_result size: "	<< etree_stl_result.size() << endl;
}

int main()
{
	using namespace etree::operators::binary;
	//using namespace etree::operators::unary;

	cudaFree(0);
	using T		= double;
	using Vec	= std::vector<T>;
	using DVec	= thrust::device_vector<T>;
	using EVec  = etree::vector < T, etree::constructors::STL, Vec, etree::serial_policy>;
	using EDVec = etree::vector<T, etree::constructors::Thrust, DVec, etree::parallel_policy>;

	const std::size_t N = 3;

	EDVec dx(N), dy(N);
	dx.assign(N, -1);
	dy.assign(N, 3);


	EDVec dsum = dx + dy;

	/*EVec x(N), y(N);
	x.assign(N, -1);
	y.assign(N, 3);

	EVec sum = x + y;*/

	//DVec& data = sum;

	//std::cout << "Sum = " << host[0] << std::endl;
	//std::cout << "Sum = " << sum[0] << std::endl;
	runTests();
	return 0;
}

