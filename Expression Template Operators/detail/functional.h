#pragma once

#include <cmath>
#include <boost\tuple\tuple.hpp>

#include "config.h"

#ifdef ETREE_USING_THRUST
#include <thrust\complex.h>
#include <thrust\tuple.h>
#endif

namespace etree {

enum { Argument1 = 0, Argument2 = 1, };

// Binary operator functors 
template <typename Result>
struct sum
{
	using result_type = Result;
	template <class Tuple>
	ANY_SYSTEM
	Result operator () (const Tuple& args) const
	{ 
		IF_USING_THRUST (using thrust::get);
		NOT_USING_THRUST(using boost::get);
		return get<Argument1>(args) + get<Argument2>(args);
	}
};

template <typename Result>
struct difference
{
	using result_type = Result;
	template <class Tuple>
	ANY_SYSTEM
	Result operator () (const Tuple& args) const
	{ 
		IF_USING_THRUST (using thrust::get);
		NOT_USING_THRUST(using boost::get);
		return get<Argument1>(args) - get<Argument2>(args);
	}
};

template <typename Result>
struct product
{
	using result_type = Result;
	template <class Tuple>
	ANY_SYSTEM
	Result operator () (const Tuple& args) const
	{ 
		IF_USING_THRUST (using thrust::get);
		NOT_USING_THRUST(using boost::get);
		return get<Argument1>(args) * get<Argument2>(args);
	}
};

template <typename Result>
struct quotient
{
	using result_type = Result;
	template <class Tuple>
	ANY_SYSTEM
	Result operator () (const Tuple& args) const
	{ 
		IF_USING_THRUST (using thrust::get);
		NOT_USING_THRUST(using boost::get);
		return get<Argument1>(args) / get<Argument2>(args);
	}
};

template <typename Result>
struct power
{
	using result_type = Result;
	template <class Tuple>
	ANY_SYSTEM
	Result operator () (const Tuple& args) const
	{
		IF_USING_THRUST (using thrust::get);
		NOT_USING_THRUST(using boost::get);

		IF_USING_THRUST (using thrust::pow);
		NOT_USING_THRUST(using std::pow);
		return pow(get<Argument1>(args), get<Argument2>(args));
	}
};

// Unary operator functors for the thrust parallel execution policy
template <typename Result>
struct sin
{
	using result_type = Result;
	template <class Type>
	ANY_SYSTEM
	Result operator () (const Type& value) const
	{
		IF_USING_THRUST (using thrust::sin);
		NOT_USING_THRUST(using std::sin);
		return sin(value);
	}
};

template <typename Result>
struct cos
{
	using result_type = Result;
	template <class Type>
	ANY_SYSTEM
	Result operator () (const Type& value) const
	{
		IF_USING_THRUST (using thrust::cos);
		NOT_USING_THRUST(using std::cos);
		return cos(value);
	}
};

template <typename Result>
struct tan
{
	using result_type = Result;
	template <class Type>
	ANY_SYSTEM
	Result operator () (const Type& value) const
	{
		IF_USING_THRUST (using thrust::tan);
		NOT_USING_THRUST(using std::tan);
		return tan(value);
	}
};

template <typename Result>
struct log
{
	using result_type = Result;
	template <class Type>
	ANY_SYSTEM
	Result operator () (const Type& value) const
	{
		IF_USING_THRUST (using thrust::log);
		NOT_USING_THRUST(using std::log);
		return log(value);
	}
};

template <typename Result>
struct negate
{
	using result_type = Result;
	template <class Type>
	ANY_SYSTEM
	Result operator () (const Type& value) const
	{
		return -(value);
	}
};

} // end namespace etree