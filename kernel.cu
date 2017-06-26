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

thrust::device_vector<double> vap_thrust_result;
thrust::device_vector<double> thrust_std_result;
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
	using namespace thrust_operators::vector;
	using namespace vap::operators::binary;
	using namespace vap::operators::unary;

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
	vap::vector<T> c_x, c_y;
	vap::vector<T, vap::constructors::STL> stl_x, stl_y;
	vap::vector<T, vap::constructors::Thrust, DVec, vap::parallel_policy> ed_x, ed_y;

	RuntimeTest<double> vap_loop(sizes), vap_stl(sizes), vap_thrust(sizes), thrust_std(sizes), std(sizes);

	vap_loop.storeSetup(setup<decltype(c_x)>, &c_x, &c_y);
	vap_loop.storeTest("Operators_vap_Loop_times.txt", test<decltype(c_x), Vec>, &c_x, &c_y, &vap_loop_result);
	vap_loop.runAll(10);
	vap_loop.save();

	//vap_stl.storeSetup(setup<decltype(stl_x)>, &stl_x, &stl_y);
	//vap_stl.storeTest("Operators_vap_STL_times.txt", test<decltype(stl_x), Vec>, &stl_x, &stl_y, &vap_stl_result);
	//vap_stl.runAll(10);
	//vap_stl.save();

	thrust_std.storeSetup(setup<decltype(d_x)>, &d_x, &d_y);
	thrust_std.storeTest("Operators_Thrust_Std_times.txt", test<decltype(d_x), DVec>, &d_x, &d_y, &thrust_std_result);
	thrust_std.runAll(10);
	thrust_std.save();

	vap_thrust.storeSetup(setup<decltype(ed_x)>, &ed_x, &ed_y);
	vap_thrust.storeTest("Operators_vap_Thrust_times.txt", test<decltype(ed_x), DVec>, &ed_x, &ed_y, &vap_thrust_result);
	vap_thrust.runAll(10);
	vap_thrust.save();

	std.storeSetup(setup<Vec>, &x, &y);
	std.storeTest("Operators_STD_times.txt", test<Vec, Vec>, &x, &y, &std_result);
	std.storeTest("Operators_Loop_times.txt", test_Loop<Vec, Vec>, &x, &y, &loop_result);
	std.runAll(10);
	std.save();

	Vec vap_thrust_host(vap_thrust_result.size());
	Vec thrust_std_host(vap_thrust_result.size());

	thrust::copy(vap_thrust_result.begin(), vap_thrust_result.end(), vap_thrust_host.begin());
	thrust::copy(thrust_std_result.begin(), thrust_std_result.end(), thrust_std_host.begin());

	std::cout << "Equality of Answers:"
		<< endl
		<< "loop_result == vap_loop_result: "
		<< std::to_string(loop_result == vap_loop_result)
		<< endl

		/*<< "loop_result == vap_stl_result: "
		<< std::to_string(loop_result == vap_stl_result)
		<< endl*/

		<< "loop_result == vap_thrust_result: "
		<< std::to_string(loop_result == thrust_std_host)
		<< endl

		<< "loop_result == thrust_std_result: "
		<< std::to_string(loop_result == vap_thrust_host)
		<< endl

		<< "loop_result == std_result: "
		<< std::to_string(loop_result == std_result)
		<< endl;

	std::cout << "STD: "		<< std_result[900]		  << endl
			  << "Loop: "		<< loop_result[900]		  << endl
			  << "Thrust STD: " << thrust_std_host[900] << endl
			  << "vap Thrust: " << vap_thrust_host[900] << endl
			  //<< "vap STL: "	<< vap_stl_result[900]  << endl
			  << "vap Loop: " << vap_loop_result[900] << endl;

	std::cout << "Result vector sizes:"		<< endl;
	std::cout << "loop_result size: "		<< loop_result.size() << endl;
	std::cout << "std_result size: "		<< std_result.size() << endl;
	std::cout << "vap_loop_result size: " << vap_loop_result.size() << endl;
	std::cout << "thrust_std_host size: " << thrust_std_host.size() << endl;
	std::cout << "vap_thrust_result size: " << vap_thrust_host.size() << endl;
	//std::cout << "vap_stl_result size: "	<< vap_stl_result.size() << endl;
}

int main()
{
	using namespace vap::operators::binary;
	//using namespace vap::operators::unary;

	cudaFree(0);
	using T		= double;
	using Vec	= std::vector<T>;
	using DVec	= thrust::device_vector<T>;
	using EVec  = vap::vector < T, vap::constructors::STL, Vec, vap::serial_policy>;
	using EDVec = vap::vector<T, vap::constructors::Thrust, DVec, vap::parallel_policy>;

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

