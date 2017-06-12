#pragma once

#include <cmath>

#include "expressions.h"

namespace etree {

	// Note: When we define a unary expression, we use the using directive to 
	// ensure the default function to be used. Otherwise, ADL will (hopefully) take care of 
	// the proper lookup.
	namespace operators {

		namespace binary {
			/*************************************/
			/** Expression Specializations **/
			/*************************************/
			template <typename ReturnType, typename LeftType, typename RightType>
			class Sum : 
				public expressions::Binary <ReturnType, LeftType, RightType, 
									Sum	   <ReturnType, LeftType, RightType>>
			{
			public:
				Sum(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
				ReturnType operator [] (std::size_t i) const { return _lhs[i] + _rhs[i]; }
			};

			template <typename ReturnType, typename LeftType, typename RightType>
			class Difference :
				public expressions::Binary	   <ReturnType, LeftType, RightType,	// Arity
									Difference <ReturnType, LeftType, RightType>>	// Operation
			{
			public:
				Difference(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
				ReturnType operator [] (std::size_t i) const { return _lhs[i] - _rhs[i]; }
			};

			template <typename ReturnType, typename LeftType, typename RightType>
			class Product : 
				public expressions::Binary  <ReturnType, LeftType, RightType,	// Arity
									Product <ReturnType, LeftType, RightType>>	// Operation
			{
			public:
				Product(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
				ReturnType operator [] (std::size_t i) const { return _lhs[i] * _rhs[i]; }
			};

			template <typename ReturnType, typename LeftType, typename RightType>
			class Quotient :
				public expressions::Binary   <ReturnType, LeftType, RightType,		// Arity
								    Quotient <ReturnType, LeftType, RightType >>	// Operation
			{
			public:
				Quotient(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
				ReturnType operator [] (std::size_t i) const { return _lhs[i] / _rhs[i]; }
			};

			template <typename ReturnType, typename LeftType, typename RightType>
			class Power :
				public expressions::Binary <ReturnType, LeftType, RightType,	// Arity
									Power  <ReturnType, LeftType, RightType >>	// Operation
			{
			public:
				Power(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
				ReturnType operator [] (std::size_t i) const 
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
		}

		namespace unary
		{
			/*************************************/
			/** Expression Specializations **/
			/*************************************/
			template <typename ReturnType, typename Type>
			class Negate : 
				public expressions::Unary  <ReturnType, Type,	// Arity
									Negate <ReturnType, Type>>	// Operation
			{
			public:
				Negate(const Type& val) : Unary(val) {}
				ReturnType operator [] (std::size_t i) const { return -_element[i]; }
			};

			template <typename ReturnType, typename Type>
			class Log :
				public expressions::Unary <ReturnType, Type,	// Arity
									Log   <ReturnType, Type >>	// Operation
			{
			public:
				Log(const Type& val) : Unary(val) {}
				ReturnType operator [] (std::size_t i) const
				{
					using std::log;
					return log(_element[i]);
				}
			};

			template <typename ReturnType, typename Type>
			class Sin : 
				public expressions::Unary <ReturnType, Type,	// Arity
									Sin   <ReturnType, Type >>	// Operation
			{
			public:
				Sin(const Type& val) : Unary(val) {}
				ReturnType operator [] (std::size_t i) const
				{
					using std::sin;
					return sin(_element[i]);
				}
			};

			template <typename ReturnType, typename Type>
			class Cos :
				public expressions::Unary <ReturnType, Type,	// Arity
									Cos   <ReturnType, Type >>	// Operation
			{
			public:
				Cos(const Type& val) : Unary(val) {}
				ReturnType operator [] (std::size_t i) const
				{
					using std::cos;
					return cos(_element[i]);
				}
			};

			template <typename ReturnType, typename Type>
			class Tan : 
				public expressions::Unary <ReturnType, Type,	// Arity
									Tan   <ReturnType, Type >>	// Operation
			{
			public:
				Tan(const Type& val) : Unary(val) {}
				ReturnType operator [] (std::size_t i) const
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
			
		}
	}
}
