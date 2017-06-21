#pragma once

#include <cmath>

#include "expressions.h"
#include "iterators.h"
#include "execution_policy.h"

namespace etree		{
namespace operators {
namespace binary	{

	/********************************/
	/** Expression Specializations **/
	/********************************/
	template <typename Left, 
			  typename Right = Left,
			  typename Execution_Policy = serial_policy<typename Left::value_type>>
	class Sum : 
		public  expressions::Binary <Left, Right, 
							 Sum	<Left, Right, Execution_Policy>>,
		private Execution_Policy
	{
	public:
		//using Execution_Policy::sum;

		using apply = typename Execution_Policy::sum;

		Sum(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
		//value_type apply(const Left& l, const Right& r) { return sum(l, r); }

		value_type operator [] (std::size_t i) const { return apply(_lhs[i], _rhs[i]); }
	};

	template <typename Left, 
			  typename Right = Left,
			  typename Execution_Policy = serial_policy<typename Left::value_type >>
	class Difference :
		public expressions::Binary	   <Left, Right,	// Arity
							Difference <Left, Right>>	// Operation
	{
	public:  
		using apply = typename Execution_Policy::difference;

		Difference(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
		//value_type operator [] (std::size_t i) const { return apply(_lhs[i], _rhs[i]); }
	};

	template <typename Left, 
			  typename Right = Left,
			  typename Execution_Policy = serial_policy<typename Left::value_type >>
	class Product : 
		public expressions::Binary  <Left, Right,	// Arity
							Product <Left, Right>>	// Operation
	{
	public:  
		using apply = typename Execution_Policy::product;

		Product(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
		//value_type operator [] (std::size_t i) const { return apply(_lhs[i], _rhs[i]); }
	};

	template <typename Left, 
			  typename Right = Left,
			  typename Execution_Policy = serial_policy<typename Left::value_type >>
	class Quotient :
		public expressions::Binary   <Left, Right,	// Arity
							Quotient <Left, Right >>	// Operation
	{
	public: 
		using apply = typename Execution_Policy::quotient;

		Quotient(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
		//value_type operator [] (std::size_t i) const { return apply(_lhs[i], _rhs[i]); }
	};

	template <typename Left, 
			  typename Right = Left,
			  typename Execution_Policy = serial_policy<typename Left::value_type >>
	class Power :
		public expressions::Binary <Left, Right,		// Arity
							Power  <Left, Right >>	// Operation
	{
	public:  
		using apply = typename Execution_Policy::power;

		Power(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
		//value_type operator [] (std::size_t i) const { return apply(_lhs[i], _rhs[i]); }
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
//namespace unary
//{
//	/*************************************/
//	/** Expression Specializations **/
//	/*************************************/
//	template <typename Exp>
//	class Negate : 
//		public expressions::Unary  <Exp,	// Arity
//							Negate <Exp>>	// Operation
//	{
//	public:  
//		Negate(const Exp& val) : Unary(val) {}
//		value_type operator [] (std::size_t i) const { return -expression[i]; }
//	};
//
//	template <typename Exp>
//	class Log :
//		public expressions::Unary <Exp,	// Arity
//							Log   <Exp >>	// Operation
//	{
//	public:  
//		Log(const Exp& exp) : Unary(exp) {}
//		value_type operator [] (std::size_t i) const
//		{
//			using std::log;
//			return log(expression[i]);
//		}
//	};
//
//	template <typename Exp>
//	class Sin : 
//		public expressions::Unary <Exp,	// Arity
//							Sin   <Exp >>	// Operation
//	{
//	public:  
//		Sin(const Exp& exp) : Unary(exp) {}
//		value_type operator [] (std::size_t i) const
//		{
//			using std::sin;
//			return sin(expression[i]);
//		}
//	};
//
//	template <typename Exp>
//	class Cos :
//		public expressions::Unary <Exp,	// Arity
//							Cos   <Exp >>	// Operation
//	{
//	public: 
//		Cos(const Exp& exp) : Unary(exp) {}
//		value_type operator [] (std::size_t i) const
//		{
//			using std::cos;
//			return cos(expression[i]);
//		}
//	};
//
//	template <typename Exp>
//	class Tan : 
//		public expressions::Unary <Exp,	// Arity
//							Tan   <Exp>>	// Operation
//	{
//	public:  
//		Tan(const Exp& exp) : Unary(exp) {}
//		value_type operator [] (std::size_t i) const
//		{
//			using std::tan;
//			return tan(expression[i]);
//		}
//	};
//
//	/***********************/
//	/** Operator Overloads **/
//	/***********************/
//	template <typename E>
//	const Negate<E> operator - (const E& vec)
//	{
//		return Negate<E>(vec);
//	}
//
//	template <typename E>
//	const Cos<E> cos(const E& vec)
//	{
//		return Cos<E>(vec);
//	}
//
//	template <typename E>
//	const Sin<E> sin(const E& vec)
//	{
//		return Sin<E>(vec);
//	}
//
//	template <typename E>
//	const Tan<E> tan(const E& vec)
//	{
//		return Tan<E>(vec);
//	}
//
//	template <typename E>
//	const Log<E> log(const E& vec)
//	{
//		return Log<E>(vec);
//	}
//}

namespace scalar
{
			
} // end namespace scalar
} // end namespace operators
} // end namespace etree
