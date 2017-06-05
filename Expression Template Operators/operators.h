#pragma once

namespace agac {
	namespace expressions
	{
		template <typename ReturnType, class E>
		class Expression
		{
		public:
			// Save the return type of the expression
			typedef ReturnType value_type;

			// Casts to const T&, calling  the overload below.
			ReturnType operator [] (std::size_t i) const { return static_cast<const E&>(*this)[i]; }
			std::size_t size()					   const { return static_cast<const E&>(*this).size(); }

			// These type conversion operators should implicitly call the constructor for type T with parameter *this, 
			// of type Expression.
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
			// Example: return _lhs[i] op _rhs[i];
			virtual ReturnType operator [] (std::size_t) const = 0;
		};

		//template <typename ReturnType, typename C>
		//class Unary : public Expression<ReturnType, Unary, C>
		//{
		//private:
		//	const C& _val;

		//public:
		//	Unary(const C& val) : _val(rhs) {}
		//	std::size_t size() { return _val.size(); }

		//	// This member function is where the binary operation is actually performed
		//	virtual ReturnType operator [] (std::size_t) const = 0;
		//};

		template <typename ReturnType, typename LeftType, typename RightType>
		class Sum : public Binary<ReturnType, LeftType, RightType>
		{
		public:
			Sum(const LeftType& lhs, const RightType& rhs) : Binary(lhs, rhs) {}
			virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] + _rhs[i]; }
		};

		template <typename E1, typename E2>
		const Sum<E1::value_type, E1, E2> operator + (const E1& lhs, const E2& rhs)
		{
			return Sum<E1::value_type, E1, E2>(lhs, rhs);
		}

		//template <typename ReturnType, typename LeftType, typename RightType>
		//class Subtractor : Binary<ReturnType, LeftType, RightType>
		//{
		//public:
		//	virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] - _rhs[i]; }
		//};

		//template <typename ReturnType, typename LeftType, typename RightType>
		//class Multiplier : Binary<ReturnType, LeftType, RightType>
		//{
		//public:
		//	virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] * _rhs[i]; }
		//};

		//template <typename ReturnType, typename LeftType, typename RightType>
		//class Divider : Binary<ReturnType, LeftType, RightType>
		//{
		//public:
		//	virtual ReturnType operator [] (std::size_t i) const override { return _lhs[i] / _rhs[i]; }
		//};
	}
}