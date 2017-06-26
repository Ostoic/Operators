#pragma once

#include "config.h"
#include "execution_policy.h"

#include <boost\iterator\transform_iterator.hpp>
#include <boost\iterator\zip_iterator.hpp>
#include <boost\tuple\tuple.hpp>

namespace vap		{
namespace iterators {

template <typename T>
struct binary_iterator {};

template <typename T>
struct unary_iterator {};

template <>
struct binary_iterator <serial_policy>
{
	template <class AdaptableUnaryFunction, class Left_Iterator, class Right_Iterator>
	using type = boost::transform_iterator<AdaptableUnaryFunction,
										   boost::zip_iterator<
										   boost::tuple<Left_Iterator, Right_Iterator>>, typename 
										   AdaptableUnaryFunction::result_type>;
};

template <>
struct unary_iterator <serial_policy>
{
	template <class AdaptableUnaryFunction, class Iterator>
	using type = boost::transform_iterator<AdaptableUnaryFunction, 
										   Iterator, typename 
										   AdaptableUnaryFunction::result_type>;
};

#ifdef VAP_USING_THRUST

#include <thrust\iterator\transform_iterator.h>
#include <thrust\iterator\zip_iterator.h>
#include <thrust\tuple.h>

template <>
struct binary_iterator <parallel_policy>
{
	template <class AdaptableUnaryFunction, class Left_Iterator, class Right_Iterator>
	using type = thrust::transform_iterator<AdaptableUnaryFunction,
											thrust::zip_iterator<
											thrust::tuple<Left_Iterator, Right_Iterator >> , typename
											AdaptableUnaryFunction::result_type>;
};

template <>
struct unary_iterator <parallel_policy>
{
	template <class AdaptableUnaryFunction, class Iterator>
	using type = thrust::transform_iterator<AdaptableUnaryFunction,
											Iterator, typename 
											AdaptableUnaryFunction::result_type>;
};

#endif

} // end namespace iterators
} // end namespace vap