#pragma once

#include <vap\config.h>
#include <vap\execution_policy.h>
#include <vap\iterators\constant_iterator.h>

#include <boost\iterator\transform_iterator.hpp>
#include <boost\iterator\zip_iterator.hpp>
#include <boost\tuple\tuple.hpp>

namespace vap		{
namespace iterators {

template <typename T>
struct binary_iterator {};

template <typename T>
struct unary_iterator {};

template <typename T>
struct scalar_iterator {};

template <>
struct binary_iterator <serial_execution>
{
	template <class UnaryFunc, class LeftIterator, class Right_Iterator>
	using type = boost::transform_iterator<UnaryFunc,
										   boost::zip_iterator<
										   boost::tuple<LeftIterator, Right_Iterator>> , typename
										   UnaryFunc::result_type>;
};

template <>
struct unary_iterator <serial_execution>
{
	template <class UnaryFunc, class Iterator>
	using type = boost::transform_iterator<UnaryFunc,
										   Iterator, typename 
										   UnaryFunc::result_type>;
};

template <>
struct scalar_iterator <inherit_execution_policy>
{
	template <typename Type>
	using type = vap::iterators::constant_iterator<Type>;
};

#ifdef VAP_USING_THRUST

#include <thrust\iterator\transform_iterator.h>
#include <thrust\iterator\constant_iterator.h>
#include <thrust\iterator\zip_iterator.h>
#include <thrust\tuple.h>

template <>
struct binary_iterator <parallel_execution>
{
	template <class UnaryFunc, class LeftIterator, class RightIterator>
	using type = thrust::transform_iterator<UnaryFunc,
											thrust::zip_iterator<
											thrust::tuple<LeftIterator, RightIterator >> , typename
											UnaryFunc::result_type>;
};

template <>
struct unary_iterator <parallel_execution>
{
	template <class UnaryFunc, class Iterator>
	using type = thrust::transform_iterator<UnaryFunc,
											Iterator, typename 
											UnaryFunc::result_type>;
};

#endif

} // end namespace iterators
} // end namespace vap