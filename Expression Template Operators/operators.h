#pragma once

#include <cmath>

#include "expressions.h"
#include "iterators.h"
#include "execution_policy.h"
#include "config\config.h"

namespace etree		{
namespace operators {
namespace binary	{

	/********************************/
	/** Expression Specializations **/
	/********************************/
	template <typename Left, 
			  typename Right 
					   =	  std::enable_if
							  <std::is_convertible<Left::value_type, Right::value_type>::value, // Make sure left and right scalar types are convertible between eachother
					   Left>, // Default type is Right = Left
			  typename Execution_Policy = serial_policy <typename Left::value_type>>
	class Sum : 
		public expressions::Binary <Left, Right, typename
							Execution_Policy::sum,
							Execution_Policy>
	{
	public:
		Sum(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left, 
			  typename Right 
					   =	  std::enable_if
							  <std::is_convertible<Left::value_type, Right::value_type>::value, // Make sure left and right scalar types are convertible between eachother
					   Left>, // Default type is Right = Left
			  typename Execution_Policy = serial_policy <typename Left::value_type>>
	class Difference : 
		public expressions::Binary <Left, Right, typename
							Execution_Policy::difference,
							Execution_Policy>
	{
	public:
		Difference(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	

	template <typename Left, 
			  typename Right 
					   =	  std::enable_if
							  <std::is_convertible<Left::value_type, Right::value_type>::value, // Make sure left and right scalar types are convertible between eachother
					   Left>, // Default type is Right = Left
			  typename Execution_Policy = serial_policy <typename Left::value_type>>
	class Product :
		public expressions::Binary <Left, Right, typename
							Execution_Policy::product,
							Execution_Policy>
	{
	public:
		Product(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left, 
			  typename Right 
					   =	  std::enable_if
							  <std::is_convertible<Left::value_type, Right::value_type>::value, // Make sure left and right scalar types are convertible between eachother
					   Left>, // Default type is Right = Left
			  typename Execution_Policy = serial_policy <typename Left::value_type>>
	class Quotient :
		public expressions::Binary <Left, Right, typename
							Execution_Policy::quotient,
							Execution_Policy>
	{
	public:
		Quotient(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left, 
			  typename Right 
					   =	  std::enable_if
							  <std::is_convertible<Left::value_type, Right::value_type>::value, // Make sure left and right scalar types are convertible between eachother
					   Left>, // Default type is Right = Left
			  typename Execution_Policy = serial_policy <typename Left::value_type>>
	class Power :
		public expressions::Binary <Left, Right, typename
							Execution_Policy::power,
							Execution_Policy>
	{
	public:
		Power(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	/************************/
	/** Operator Overloads **/
	/************************/
	template <typename Left, typename Right>
	const Sum<Left, Right> operator + (const Left& lhs, const Right& rhs)
	{
		return Sum<Left, Right>(lhs, rhs);
	}

	template <typename Left, typename Right>
	const Difference<Left, Right> operator - (const Left& lhs, const Right& rhs)
	{
		return Difference<Left, Right>(lhs, rhs);
	}

	template <typename Left, typename Right>
	const Product<Left, Right> operator * (const Left& lhs, const Right& rhs)
	{
		return Product<Left, Right>(lhs, rhs);
	}

	template <typename Left, typename Right>
	const Quotient<Left, Right> operator / (const Left& lhs, const Right& rhs)
	{
		return Quotient<Left, Right>(lhs, rhs);
	}

	template <typename Left, typename Right>
	const Power<Left, Right> operator ^ (const Left& lhs, const Right& rhs)
	{
		return Power<Left, Right>(lhs, rhs);
	}

} // end namespace binary

// Note: When we define a unary expression, we use the using directive to 
// ensure the default function to be used. Otherwise, ADL will (hopefully) take care of 
// the proper lookup.
namespace unary
{
	/*************************************/
	/** Expression Specializations **/
	/*************************************/

	template <typename Type, 
			  typename Execution_Policy = serial_policy <typename Type::value_type>>
	struct Sin :
		public expressions::Unary <Type, typename
							Execution_Policy::sin,
							Execution_Policy>
	{ Sin(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_policy <typename Type::value_type>>
	struct Cos :
		public expressions::Unary <Type, typename
							Execution_Policy::cos,
							Execution_Policy>
	{ Cos(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_policy <typename Type::value_type>>
	struct Tan :
		public expressions::Unary <Type, typename
							Execution_Policy::tan,
							Execution_Policy>
	{ Tan(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_policy <typename Type::value_type>>
	struct Log :
		public expressions::Unary <Type, typename
							Execution_Policy::log,
							Execution_Policy>
	{ Log(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_policy <typename Type::value_type>>
	struct Negate :
		public expressions::Unary <Type, typename
							Execution_Policy::negate,
							Execution_Policy>
	{ Negate(const Type& value) : Unary(value) {} };

	/***********************/
	/** Operator Overloads **/
	/***********************/
	template <typename E>
	const Negate<E> operator - (const E& vec)
	{
		return Negate<E>(vec);
	}

	template <typename E>
	const Cos<E> cos(const E& vec)
	{
		return Cos<E>(vec);
	}

	template <typename E>
	const Sin<E> sin(const E& vec)
	{
		return Sin<E>(vec);
	}

	template <typename E>
	const Tan<E> tan(const E& vec)
	{
		return Tan<E>(vec);
	}

	template <typename E>
	const Log<E> log(const E& vec)
	{
		return Log<E>(vec);
	}
}

namespace scalar
{
			
} // end namespace scalar
} // end namespace operators
} // end namespace etree
