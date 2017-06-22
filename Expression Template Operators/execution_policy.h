#pragma once

#include <cmath>

#ifdef ETREE_USING_THRUST
#	include <thrust\complex.h>
#endif

#include "config\config.h"

namespace etree  {

// Base execution policy used for selecting different execution systems
template <class Return>
struct execution_policy
{};

template <typename Return>
struct serial_policy : execution_policy<Return>
{
	// Binary operator functors for the serial execution policy
	struct sum
	{
		using result_type = Return;
		template <typename Left, typename Right>
		Return operator () (const Left& left, const Right& right) const
		{ return left + right; }
	};

	struct difference
	{
		using result_type = Return;
		template <typename Left, typename Right>
		Return operator () (const Left& left, const Right& right) const
		{ return left - right; }
	};

	struct product
	{
		using result_type = Return;
		template <typename Left, typename Right>
		Return operator () (const Left& left, const Right& right) const
		{ return left * right; }
	};

	struct quotient
	{
		using result_type = Return;
		template <typename Left, typename Right>
		Return operator () (const Left& left, const Right& right) const
		{ return left / right; }
	};

	struct pow
	{
		using result_type = Return;
		template <typename Left, typename Right>
		Return operator () (const Left& left, const Right& right) const
		{ using std::pow; return pow(left, right); }
	};

	// Unary operator functors for the serial execution policy
	struct sin
	{
		using result_type = Return;
		template <typename Type>
		Return operator () (const Type& value) const
		{
			using std::sin;
			return sin(value);
		}
	};

	struct cos
	{
		using result_type = Return;
		template <typename Type>
		Return operator () (const Type& value) const
		{
			using std::cos;
			return cos(value);
		}
	};

	struct tan
	{
		using result_type = Return;
		template <typename Type>
		Return operator () (const Type& value) const
		{
			using std::tan;
			return tan(value);
		}
	};

	struct log
	{
		using result_type = Return;
		template <typename Type>
		Return operator () (const Type& value) const
		{
			using std::log;
			return log(value);
		}
	};

	struct negate
	{
		using result_type = Return;
		template <typename Type>
		Return operator () (const Type& value) const
		{
			return -(value);
		}
	};

};


// We can create HOST_DEVICE functions, but in order to force a parallel execution policy,
// we must explicity create a separate policy for these, with THRUST_DEVICE functions
template <typename Return>
struct thrust_policy : execution_policy<Return>
{
	// Binary operator functors for the thrust parallel execution policy
	struct sum
	{
		template <typename Left, typename Right>
		THRUST_DEVICE(
		Return operator () (const Left& left, const Right& right) const
		{ return left + right; })
	};

	struct difference
	{
		template <typename Left, typename Right>
		THRUST_DEVICE(
		Return operator () (const Left& left, const Right& right) const
		{ return left - right; })
	};

	struct product
	{
		template <typename Left, typename Right>
		THRUST_DEVICE(
		Return operator () (const Left& left, const Right& right) const
		{ return left * right; })
	};

	struct quotient
	{
		template <typename Left, typename Right>
		THRUST_DEVICE(
		Return operator () (const Left& left, const Right& right) const
		{ return left / right; })
	};

	struct pow
	{
		template <typename Left, typename Right>
		THRUST_DEVICE(
		Return operator () (const Left& left, const Right& right) const
		{ using IF_USING_THRUST(using thrust::pow); return pow(left, right); })
	};

	// Unary operator functors for the thrust parallel execution policy
	struct sin
	{
		using result_type = Return;
		template <typename Type>
		THRUST_DEVICE(
		Return operator () (const Type& value) const
		{
			IF_USING_THRUST(using thrust::sin);
			return sin(value);
		})
	};

	struct cos
	{
		using result_type = Return;
		template <typename Type>
		THRUST_DEVICE(
		Return operator () (const Type& value) const
		{
			IF_USING_THRUST(using thrust::cos);
			return cos(value);
		})
	};

	struct tan
	{
		using result_type = Return;
		template <typename Type>
		THRUST_DEVICE(
		Return operator () (const Type& value) const
		{
			IF_USING_THRUST(using thrust::tan);
			return tan(value);
		})
	};

	struct log
	{
		using result_type = Return;
		template <typename Type>
		THRUST_DEVICE(
		Return operator () (const Type& value) const
		{
			IF_USING_THRUST(using thrust::log);
			return log(value);
		})
	};

	struct negate
	{
		using result_type = Return;
		template <typename Type>
		THRUST_DEVICE(
		Return operator () (const Type& value) const
		{
			return -(value);
		})
	};

};

} // end namespace etree
