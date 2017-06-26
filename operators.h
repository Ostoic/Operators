#pragma once

#include <cmath>

#include "detail\config.h"
#include "detail\iterators.h"
#include "detail\functional.h"

#include "expressions.h"
#include "execution_policy.h"

namespace etree		{
namespace operators {

	/********************************/
	/** Binary Expression Operators**/
	/********************************/
	template <typename Left, 
			  typename Right = Left,
			  typename Execution_Policy = serial_policy>
	struct Sum :
		public expressions::Binary <Left, Right, etree::sum <typename Left::value_type>, Execution_Policy>
	{ Sum(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {} };

	template <typename Left,
		typename Right = Left,
		typename Execution_Policy = serial_policy>
	struct Difference :
		public expressions::Binary <Left, Right, etree::difference <typename Left::value_type>, Execution_Policy>
	{
		Difference(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left,
		typename Right = Left,
		typename Execution_Policy = serial_policy>
	struct Product :
		public expressions::Binary <Left, Right, etree::product <typename Left::value_type>, Execution_Policy>
	{
		Product(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left,
		typename Right = Left,
		typename Execution_Policy = serial_policy>
	struct Quotient :
		public expressions::Binary <Left, Right, etree::quotient <typename Left::value_type>, Execution_Policy>
	{
		Quotient(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left,
		typename Right = Left,
		typename Execution_Policy = serial_policy>
	struct Power :
		public expressions::Binary <Left, Right, etree::power <typename Left::value_type>, Execution_Policy>
	{
		Power(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	/********************************/
	/** Unary Expression Operators **/
	/********************************/

	template <typename Type, 
			  typename Execution_Policy = serial_policy>
	struct Sin :
		public expressions::Unary <Type, etree::sin <typename Type::value_type>, Execution_Policy>
	{ Sin(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_policy>
	struct Cos :
		public expressions::Unary <Type, etree::cos <typename Type::value_type>, Execution_Policy>
	{ Cos(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_policy>
	struct Tan :
		public expressions::Unary <Type, etree::tan <typename Type::value_type>, Execution_Policy>
	{ Tan(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_policy>
	struct Log :
		public expressions::Unary <Type, etree::log <typename Type::value_type>, Execution_Policy>
	{ Log(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_policy>
	struct Negate :
		public expressions::Unary <Type, etree::negate <typename Type::value_type>, Execution_Policy>
	{ Negate(const Type& value) : Unary(value) {} };

	/*******************************/
	/** Binary Operator Overloads **/
	/*******************************/
	template <typename Left, typename Right>
	const Sum<Left, Right, typename Left::policy> operator + (const Left& lhs, const Right& rhs)
	{
		using Policy = typename Left::policy;
		return Sum<Left, Right, Policy>(lhs, rhs);
	}

	template <typename Left, typename Right>
	const Difference<Left, Right, typename Left::policy> operator - (const Left& lhs, const Right& rhs)
	{
		using Policy = typename Left::policy;
		return Difference<Left, Right, Policy>(lhs, rhs);
	}

	template <typename Left, typename Right>
	const Product<Left, Right, typename Left::policy>	  operator * (const Left& lhs, const Right& rhs)
	{
		using Policy = typename Left::policy;
		return Product<Left, Right, Policy>(lhs, rhs);
	}

	template <typename Left, typename Right>
	const Quotient<Left, Right, typename Left::policy>   operator / (const Left& lhs, const Right& rhs)
	{
		using Policy = typename Left::policy;
		return Quotient<Left, Right, Policy>(lhs, rhs);
	}

	template <typename Left, typename Right>
	const Power<Left, Right, typename Left::policy>	  operator ^ (const Left& lhs, const Right& rhs)
	{
		using Policy = typename Left::policy;
		return Power<Left, Right, Policy>(lhs, rhs);
	}

	/******************************/
	/** Unary Operator Overloads **/
	/******************************/
	template <typename E>
	const Negate<E, typename E::policy> operator -(const E& vec)
	{
		using Policy = typename E::policy;
		return Negate<E, Policy>(vec);
	}

	template <typename E>
	const Cos<E, typename E::policy> cos(const E& vec)
	{
		using Policy = typename E::policy;
		return Cos<E, Policy>(vec);
	}

	template <typename E>
	const Sin<E, typename E::policy> sin(const E& vec)
	{
		using Policy = typename E::policy;
		return Sin<E, Policy>(vec);
	}

	template <typename E>
	const Tan<E, typename E::policy> tan(const E& vec)
	{
		using Policy = typename E::policy;
		return Tan<E, Policy>(vec);
	}

	template <typename E>
	const Log<E, typename E::policy> log(const E& vec)
	{
		using Policy = typename E::policy;
		return Log<E, Policy>(vec);
	}

} // end namespace operators
} // end namespace etree
