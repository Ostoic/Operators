// Expression Template Operators.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include <Stopwatch.h>
#include <RuntimeTest.h>
#include <Algebra\Vector_Operators.h>

#include <ETree\operators.h>
#include <ETree\vector.h>

etree::vector<double> custom_result;
std::vector<double> std_result;

using std::endl;

template <typename V>
long long test(const V* _x, const V* _y)
{
	using namespace vector_operators::vector;
	using namespace etree::expressions::operators::binary;
	using namespace etree::expressions::operators::unary;

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
	using namespace etree::expressions::operators::binary;
	using namespace etree::expressions::operators::unary;

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

int main()
{
	typedef double T;

	std::vector<std::size_t> sizes = {
		static_cast<std::size_t>(1e2),
		static_cast<std::size_t>(2e3),
		static_cast<std::size_t>(3e4),
		static_cast<std::size_t>(4e5),
		static_cast<std::size_t>(1e6),
		static_cast<std::size_t>(5e6),
		static_cast<std::size_t>(6e6),
		static_cast<std::size_t>(1e7),
	};

	const std::size_t N = 1e6;

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
	return 0;
}

