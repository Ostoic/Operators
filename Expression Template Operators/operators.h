#pragma once

#include <cmath>

namespace etree {
	namespace expressions {
		template <typename ReturnType, class E>
		class Expression 
		{
		public:
			// Save the return type of the expression
			typedef ReturnType value_type;

			// STL iterator interface
			typedef typename E::iterator iterator;
			typedef typename E::const_iterator const_iterator;

			iterator begin() { return static_cast<E&>(*this).begin(); }
			iterator end()	 { return static_cast<E&>(*this).end(); }

			const_iterator cbegin() { return static_cast<const E&>(*this).cbegin(); }
			const_iterator cend()	{ return static_cast<const E&>(*this).cend(); }

			// Casts *this to underlying expression type using CRTP, then calls [] operator in
			// Binary/Unary/N-ary derived class
			ReturnType operator [] (std::size_t i) const { return static_cast<const E&>(*this)[i]; }
			std::size_t size()					   const { return static_cast<const E&>(*this).size(); }

			// Provides implicit (or C-style) cast to expression type
			operator E& ()			    { return static_cast<E&>(*this); }
			operator const E& () const  { return static_cast<const E&>(*this); }
		};

		// Holds the binary expression of two objects. 
		template <
			typename ReturnType,
			typename LeftType, 
			typename RightType, 
			class	 Operator>
		class Binary : 
			public Expression <ReturnType, 
				   Binary     <ReturnType, LeftType, RightType, Operator>>
		{
		protected:
			const LeftType& _lhs;
			const RightType& _rhs;

		public:
			// STL iterator interface
			typedef typename LeftType::iterator iterator;
			typedef typename LeftType::const_iterator const_iterator;

			iterator begin() { return _lhs.begin(); }
			iterator end()	 { return _lhs.end(); }

			const_iterator cbegin() { return _lhs.cbegin(); }
			const_iterator cend()	{ return _lhs.cend(); }

			Binary(const LeftType& lhs, const RightType& rhs) : _lhs(lhs), _rhs(rhs) {}
			std::size_t size() const { return _lhs.size(); }

			// This is where the binary operation is actually performed
			// Cast to derived class via CRTP and call overloaded [] operator
			ReturnType operator [] (std::size_t i) const { return static_cast<const Operator&>(*this)[i]; }
		};

		// Holds the unary expression of a single object
		// Example: element[i]
		// Example: log(element[i])
		template <
			typename ReturnType,
			typename Type,
			class	 Operator>
		class Unary : 
			public Expression <ReturnType, 
				   Unary	  <ReturnType, Type, Operator>>
		{
		protected:
			const Type& _element;

		public:
			// STL iterator interface
			typedef typename Type::iterator iterator;
			typedef typename Type::const_iterator const_iterator;

			iterator begin() { return _element.begin(); }
			iterator end()	 { return _element.end(); }

			const_iterator cbegin() { return _element.cbegin(); }
			const_iterator cend()	{ return _element.cend(); }

			Unary(const Type& element) : _element(element) {}
			std::size_t size() const { return _element.size(); }

			// This is where the binary operation is actually performed
			// We require the accessor to be overridden by an Operator class.
			// Example: return op(_element[i])
			ReturnType operator [] (std::size_t i) const { return static_cast<const Operator&>(*this)[i]; }
		};

		// Note: When we define a unary expression, we use the using directive to 
		// ensure the default function to be used. Otherwise, ADL will (hopefully) take care of 
		// the proper lookup.
		namespace operators
		{
			namespace binary
			{
				/*************************************/
				/** Expression Type Specializations **/
				/*************************************/
				template <typename ReturnType, typename LeftType, typename RightType>
				class Sum : 
					public Binary <ReturnType, LeftType, RightType, 
						   Sum    <ReturnType, LeftType, RightType>>
				{
				public:
					Sum(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
					ReturnType operator [] (std::size_t i) const { return _lhs[i] + _rhs[i]; }
				};

				template <typename ReturnType, typename LeftType, typename RightType>
				class Difference :
					public Binary	  <ReturnType, LeftType, RightType,
						   Difference <ReturnType, LeftType, RightType>>
				{
				public:
					Difference(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
					ReturnType operator [] (std::size_t i) const { return _lhs[i] - _rhs[i]; }
				};

				template <typename ReturnType, typename LeftType, typename RightType>
				class Product : 
					public Binary  <ReturnType, LeftType, RightType,
						   Product <ReturnType, LeftType, RightType>>
				{
				public:
					Product(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
					ReturnType operator [] (std::size_t i) const { return _lhs[i] * _rhs[i]; }
				};

				template <typename ReturnType, typename LeftType, typename RightType>
				class Quotient :
					public Binary   <ReturnType, LeftType, RightType,
						   Quotient <ReturnType, LeftType, RightType >>
				{
				public:
					Quotient(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
					ReturnType operator [] (std::size_t i) const { return _lhs[i] / _rhs[i]; }
				};

				template <typename ReturnType, typename LeftType, typename RightType>
				class Power :
					public Binary <ReturnType, LeftType, RightType,
						   Power  <ReturnType, LeftType, RightType >>
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
				/** Expression Type Specializations **/
				/*************************************/
				template <typename ReturnType, typename Type>
				class Negate : 
					public Unary  <ReturnType, Type, 
						   Negate <ReturnType, Type>>
				{
				public:
					Negate(const Type& val) : Unary(val) {}
					ReturnType operator [] (std::size_t i) const { return -_element[i]; }
				};

				template <typename ReturnType, typename Type>
				class Log :
					public Unary <ReturnType, Type,
						   Log   <ReturnType, Type >>
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
					public Unary <ReturnType, Type,
						   Sin   <ReturnType, Type >>
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
					public Unary <ReturnType, Type,
						   Cos   <ReturnType, Type >>
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
					public Unary <ReturnType, Type,
						   Tan   <ReturnType, Type >>
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
}
