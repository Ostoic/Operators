#pragma once

#include <cmath>
#include <type_traits>

#include "expressions.h"
#include "iterators.h"

namespace etree		{
namespace operators {
namespace binary	{
	/********************************/
	/** Expression Specializations **/
	/********************************/
	template <typename Container, 
			  typename LeftType, 
			  typename RightType = LeftType>
	class Sum : 
		public expressions::Binary <Container, LeftType, RightType, 
							Sum	   <Container, LeftType, RightType>>
	{
	public:
		typedef typename value_type value_type;

		Sum(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
		value_type operator [] (std::size_t i) const { return _lhs[i] + _rhs[i]; }
	};

	template <typename Container, 
			  typename LeftType, 
			  typename RightType = LeftType>
	class Difference :
		public expressions::Binary	   <Container, LeftType, RightType,	// Arity
							Difference <Container, LeftType, RightType>>	// Operation
	{
	public:  
		typedef typename value_type value_type;

		Difference(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
		value_type operator [] (std::size_t i) const { return _lhs[i] - _rhs[i]; }
	};

	template <typename Container, 
			  typename LeftType, 
			  typename RightType = LeftType>
	class Product : 
		public expressions::Binary  <Container, LeftType, RightType,	// Arity
							Product <Container, LeftType, RightType>>	// Operation
	{
	public:  
		typedef typename value_type value_type;  typedef typename value_type value_type;
		
		Product(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
		value_type operator [] (std::size_t i) const { return _lhs[i] * _rhs[i]; }
	};

	template <typename Container, 
			  typename LeftType, 
			  typename RightType = LeftType>
	class Quotient :
		public expressions::Binary   <Container, LeftType, RightType,		// Arity
							Quotient <Container, LeftType, RightType >>	// Operation
	{
	public: 
		typedef typename value_type value_type;
		
		Quotient(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
		value_type operator [] (std::size_t i) const { return _lhs[i] / _rhs[i]; }
	};

	template <typename Container,
			  typename LeftType, 
			  typename RightType = LeftType>
	class Power :
		public expressions::Binary <Container, LeftType, RightType,	// Arity
							Power  <Container, LeftType, RightType >>	// Operation
	{
	public:  
		typedef typename value_type value_type;
		
		Power(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
		value_type operator [] (std::size_t i) const 
		{ 
			using std::pow;
			return pow(_lhs[i], _rhs[i]); 
		}
	};

	/************************/
	/** Operator Overloads **/
	/************************/
	template <typename LeftType, typename RightType>
	const Sum<typename LeftType::value_type, LeftType, RightType> operator + (const LeftType& lhs, const RightType& rhs)
	{
		return Sum<typename LeftType::value_type, LeftType, RightType>(lhs, rhs);
	}

	template <typename LeftType, typename RightType>
	const Difference<typename LeftType::value_type, LeftType, RightType> operator - (const LeftType& lhs, const RightType& rhs)
	{
		return Difference<typename LeftType::value_type, LeftType, RightType>(lhs, rhs);
	}

	template <typename LeftType, typename RightType>
	const Product<typename LeftType::value_type, LeftType, RightType> operator * (const LeftType& lhs, const RightType& rhs)
	{
		return Product<typename LeftType::value_type, LeftType, RightType>(lhs, rhs);
	}

	template <typename LeftType, typename RightType>
	const Quotient<typename LeftType::value_type, LeftType, RightType> operator / (const LeftType& lhs, const RightType& rhs)
	{
		return Quotient<typename LeftType::value_type, LeftType, RightType>(lhs, rhs);
	}

	template <typename LeftType, typename RightType>
	const Power<typename LeftType::value_type, LeftType, RightType> operator ^ (const LeftType& lhs, const RightType& rhs)
	{
		return Power<typename LeftType::value_type, LeftType, RightType>(lhs, rhs);
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
	template <typename Container, typename Type>
	class Negate : 
		public expressions::Unary  <Container, Type,	// Arity
							Negate <Container, Type>>	// Operation
	{
	public:  
		typedef typename value_type value_type;
		
		Negate(const Type& val) : Unary(val) {}
		value_type operator [] (std::size_t i) const { return -_element[i]; }
	};

	template <typename Container, typename Type>
	class Log :
		public expressions::Unary <Container, Type,	// Arity
							Log   <Container, Type >>	// Operation
	{
	public:  
		typedef typename value_type value_type;
		
		Log(const Type& val) : Unary(val) {}
		value_type operator [] (std::size_t i) const
		{
			using std::log;
			return log(_element[i]);
		}
	};

	template <typename Container, typename Type>
	class Sin : 
		public expressions::Unary <Container, Type,	// Arity
							Sin   <Container, Type >>	// Operation
	{
	public:  
		typedef typename value_type value_type;
		
		Sin(const Type& val) : Unary(val) {}
		value_type operator [] (std::size_t i) const
		{
			using std::sin;
			return sin(_element[i]);
		}
	};

	template <typename Container, typename Type>
	class Cos :
		public expressions::Unary <Container, Type,	// Arity
							Cos   <Container, Type >>	// Operation
	{
	public: 
		typedef typename value_type value_type;
		
		Cos(const Type& val) : Unary(val) {}
		value_type operator [] (std::size_t i) const
		{
			using std::cos;
			return cos(_element[i]);
		}
	};

	template <typename Container, typename Type>
	class Tan : 
		public expressions::Unary <Container, Type,	// Arity
							Tan   <Container, Type >>	// Operation
	{
	public:  
		typedef typename value_type value_type;
		
		Tan(const Type& val) : Unary(val) {}
		value_type operator [] (std::size_t i) const
		{
			using std::tan;
			return tan(_element[i]);
		}
	};

	/***********************/
	/** Operator Overloads **/
	/***********************/
	template <typename E>
	const Negate<typename E::value_type, E> operator - (const E& vec)
	{
		return Negate<typename E::value_type, E>(vec);
	}

	template <typename E>
	const Cos<typename E::value_type, E> cos(const E& vec)
	{
		return Cos<typename E::value_type, E>(vec);
	}

	template <typename E>
	const Sin<typename E::value_type, E> sin(const E& vec)
	{
		return Sin<typename E::value_type, E>(vec);
	}

	template <typename E>
	const Tan<typename E::value_type, E> tan(const E& vec)
	{
		return Tan<typename E::value_type, E>(vec);
	}

	template <typename E>
	const Log<typename E::value_type, E> log(const E& vec)
	{
		return Log<typename E::value_type, E>(vec);
	}
}

namespace scalar
{
			
} // end namespace scalar
} // end namespace operators
} // end namespace etree
