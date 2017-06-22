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
{
};

template <typename Return>
struct serial_policy : execution_policy<Return>
{
	// Binary operator functors for the serial execution policy
	struct sum
	{
		template <typename Left, typename Right>
		Return operator () (const Left& left, const Right& right) const
		{ return left + right; }
	};

	struct difference
	{
		template <typename Left, typename Right>
		Return operator () (const Left& left, const Right& right) const
		{ return left - right; }
	};

	struct product
	{
		template <typename Left, typename Right>
		Return operator () (const Left& left, const Right& right) const
		{ return left * right; }
	};

	struct quotient
	{
		template <typename Left, typename Right>
		Return operator () (const Left& left, const Right& right) const
		{ return left / right; }
	};

	struct pow
	{
		template <typename Left, typename Right>
		Return operator () (const Left& left, const Right& right) const
		{ using std::pow; return pow(left, right); }
	};

};

template <typename Return>
struct thrust_policy : execution_policy<Return>
{
	// Binary operator functors for the serial execution policy
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

};

} // end namespace etree
