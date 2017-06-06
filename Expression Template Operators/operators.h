#pragma once

#include <cmath>

namespace agac {
	namespace expressions
	{
		template <typename ReturnType, class E>
		class Expression
		{
		public:
			// Save the return type of the expression
			typedef ReturnType value_type;

			// Casts *this to underlying expression type, then calls [] operator in
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
			typename RightType>
		class Binary : public Expression<ReturnType, Binary<ReturnType, LeftType, RightType>>
		{
		protected:
			const LeftType& _lhs;
			const RightType& _rhs;

		public:
			Binary(const LeftType& lhs, const RightType& rhs) : _lhs(lhs), _rhs(rhs) {}
			std::size_t size() const { return _lhs.size(); }

			// This is where the binary operation is actually performed
			// We require the accessor to be overridden by an Operator class.
			// Example: _lhs[i] + rhs[i]
			virtual ReturnType operator [] (std::size_t) const = 0;
		};

		// Holds the unary expression of a single object
		// Example: element[i]
		// Example: log(element[i])
		template <
			typename ReturnType,
			typename Type>
		class Unary : public Expression<ReturnType, Unary<ReturnType, Type>>
		{
		protected:
			const Type& _element;

		public:
			Unary(const Type& element) : _element(element) {}
			std::size_t size() const { return _element.size(); }

			// This is where the binary operation is actually performed
			// We require the accessor to be overridden by an Operator class.
			// Example: return op(_element[i])
			virtual ReturnType operator [] (std::size_t) const = 0;
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
				class Sum : public Binary<ReturnType, LeftType, RightType>
				{
				public:
					Sum(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
					virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] + _rhs[i]; }
				};

				template <typename ReturnType, typename LeftType, typename RightType>
				class Difference : public Binary<ReturnType, LeftType, RightType>
				{
				public:
					Difference(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
					virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] - _rhs[i]; }
				};

				template <typename ReturnType, typename LeftType, typename RightType>
				class Product : public Binary<ReturnType, LeftType, RightType>
				{
				public:
					Product(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
					virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] * _rhs[i]; }
				};

				template <typename ReturnType, typename LeftType, typename RightType>
				class Quotient : public Binary<ReturnType, LeftType, RightType>
				{
				public:
					Quotient(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
					virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] / _rhs[i]; }
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
			}

			namespace unary
			{
				/*************************************/
				/** Expression Type Specializations **/
				/*************************************/
				template <typename ReturnType, typename Type>
				class Negate : public Unary<ReturnType, Type>
				{
				public:
					Negate(const Type& val) : Unary(val) {}
					virtual ReturnType operator [] (std::size_t i) const override { return -_element[i]; }
				};

				template <typename ReturnType, typename Type>
				class Log : public Unary<ReturnType, Type>
				{
				public:
					Log(const Type& val) : Unary(val) {}
					virtual ReturnType operator [] (std::size_t i) const override
					{
						using std::log;
						return log(_element[i]);
					}
				};

				template <typename ReturnType, typename Type>
				class Sin : public Unary<ReturnType, Type>
				{
				public:
					Sin(const Type& val) : Unary(val) {}
					virtual ReturnType operator [] (std::size_t i) const override
					{
						using std::sin;
						return sin(_element[i]);
					}
				};

				template <typename ReturnType, typename Type>
				class Cos : public Unary<ReturnType, Type>
				{
				public:
					Cos(const Type& val) : Unary(val) {}
					virtual ReturnType operator [] (std::size_t i) const override
					{
						using std::cos;
						return cos(_element[i]);
					}
				};

				template <typename ReturnType, typename Type>
				class Tan : public Unary<ReturnType, Type>
				{
				public:
					Tan(const Type& val) : Unary(val) {}
					virtual ReturnType operator [] (std::size_t i) const override
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
			//	/*************************************/
			//	/** Expression Type Specializations **/
			//	/*************************************/
			//	template <typename ReturnType, typename LeftType, typename RightType>
			//	class Sum : public Binary<ReturnType, LeftType, RightType>
			//	{
			//	public:
			//		Sum(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
			//		virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] + _rhs[i]; }
			//	};

			//	template <typename ReturnType, typename LeftType, typename RightType>
			//	class Difference : public Binary<ReturnType, LeftType, RightType>
			//	{
			//	public:
			//		Difference(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
			//		virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] - _rhs[i]; }
			//	};

			//	template <typename ReturnType, typename LeftType, typename RightType>
			//	class Product : public Binary<ReturnType, LeftType, RightType>
			//	{
			//	public:
			//		Product(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
			//		virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] * _rhs[i]; }
			//	};

			//	template <typename ReturnType, typename LeftType, typename RightType>
			//	class Quotient : public Binary<ReturnType, LeftType, RightType>
			//	{
			//	public:
			//		Quotient(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
			//		virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] / _rhs[i]; }
			//	};

			//	/************************/
			//	/** Operator Overloads **/
			//	/************************/
			//	template <typename LeftType, typename RightType>
			//	const Sum<typename LeftType::value_type, LeftType, RightType> operator + (const LeftType& lhs, const RightType& rhs)
			//	{
			//		return Sum<typename LeftType::value_type, LeftType, RightType>(lhs, rhs);
			//	}

			//	template <typename LeftType, typename RightType>
			//	const Difference<typename LeftType::value_type, LeftType, RightType> operator - (const LeftType& lhs, const RightType& rhs)
			//	{
			//		return Difference<typename LeftType::value_type, LeftType, RightType>(lhs, rhs);
			//	}

			//	template <typename LeftType, typename RightType>
			//	const Product<typename LeftType::value_type, LeftType, RightType> operator * (const LeftType& lhs, const RightType& rhs)
			//	{
			//		return Product<typename LeftType::value_type, LeftType, RightType>(lhs, rhs);
			//	}

			//	template <typename LeftType, typename RightType>
			//	const Quotient<typename LeftType::value_type, LeftType, RightType> operator / (const LeftType& lhs, const RightType& rhs)
			//	{
			//		return Quotient<typename LeftType::value_type, LeftType, RightType>(lhs, rhs);
			//	}
			}
		}
	}
}
