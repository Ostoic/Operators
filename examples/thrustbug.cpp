#pragma once

#include <vector>
#include <iostream>

#include <thrust\copy.h>
#include <thrust\tuple.h>
#include <thrust\device_vector.h>
#include <thrust\iterator\zip_iterator.h>
#include <thrust\iterator\constant_iterator.h>
#include <thrust\iterator\transform_iterator.h>

// Apply tuple arguments to binary functor

template <class BinaryFunctor>
class Apply
{
private:
	BinaryFunctor f;

public:
	using result_type = typename BinaryFunctor::result_type;
	template <class Tuple>
	__host__ __device__
		result_type operator ()(const Tuple& t)
	{
			return f(thrust::get<0>(t), thrust::get<1>(t));
		}
};


void workingCode()
{
	using T = int;
	thrust::device_vector<T> data(4);
	data[0] = 3;
	data[1] = 7;
	data[2] = 2;
	data[3] = 5;

	Apply<thrust::plus<T>> add;
	auto constant_it = thrust::make_constant_iterator(10);

	// add 10 to all values in data
	thrust::copy(
		thrust::make_transform_iterator(
			thrust::make_zip_iterator(
				thrust::make_tuple(data.begin(), constant_it)), add),

		thrust::make_transform_iterator(
			thrust::make_zip_iterator(
				thrust::make_tuple(data.end(), constant_it)), add),

		std::ostream_iterator<T>(std::cout, "\n"));
}

void buggedCode()
{
	using T = int;
	thrust::device_vector<T> data(4);
	data[0] = 3;
	data[1] = 7;
	data[2] = 2;
	data[3] = 5;

	Apply<thrust::plus<T>> add;
	auto constant_it = thrust::make_constant_iterator(10);

	// add 10 to all values in data

	thrust::copy(
		thrust::make_transform_iterator(
			thrust::make_zip_iterator(
				thrust::make_tuple(constant_it, data.begin())), add),

		thrust::make_transform_iterator(
			thrust::make_zip_iterator(
				thrust::make_tuple(constant_it, data.end())), add),

		std::ostream_iterator<T>(std::cout, "\n"));
}

int main()
{
	std::cout << "This code should not print anything:" << std::endl;
	buggedCode();
	std::cout << "buggedCode() done" << std::endl << std::endl;

	std::cout << "This code should print the correct result:" << std::endl;
	workingCode();
	std::cout << "workingCode() done" << std::endl;
}