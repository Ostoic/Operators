#pragma once

#include "iterators.h"

// This is more descriptive than the usual static_cast code
#define CRTP_DOWNCAST(_D) static_cast<_D>(*this)

namespace etree {
	namespace expressions {

		// Forward declare traits type to allow the compiler to fully define Expression
		template <typename D>
		struct traits;

		// Represents any operator expression
		template <typename ReturnType, class Derived>
		class Expression
		{
		public:
			// Save the return type of the expression
			typedef ReturnType value_type;

			// STL iterator interface
			typedef typename traits<Derived>::iterator iterator;
			typedef typename traits<Derived>::const_iterator const_iterator;

			// Forward the iterator interface downwards until we have enough 
			// class data to define a proper iterator type
			iterator begin() { return CRTP_DOWNCAST(Derived&).begin(); }
			iterator end()	 { return CRTP_DOWNCAST(Derived&).end();   }

			const_iterator cbegin() const { return CRTP_DOWNCAST(const Derived&).cbegin(); }
			const_iterator cend()	const { return CRTP_DOWNCAST(const Derived&).cend(); }

			// Casts *this to underlying expression type using CRTP, then calls [] operator in
			// Binary/Unary/N-ary derived class
			ReturnType operator [] (std::size_t i) const { return CRTP_DOWNCAST(const Derived&)[i]; }
			std::size_t size()					   const { return CRTP_DOWNCAST(const Derived&).size(); }

			// Provides implicit (or C-style) cast to expression type
			operator Derived& ()			 { return CRTP_DOWNCAST(Derived&); }
			operator const Derived& () const { return CRTP_DOWNCAST(const Derived&); }
		};

		// Defines iterator traits for base class Expression
		template <typename R, typename D>
		struct traits<Expression<R, D>>
		{
			typedef typename D::iterator iterator;
			typedef typename D::const_iterator const_iterator;
			typedef typename R value_type;
		};

		// Holds the binary expression of two objects. 
		template <
			typename ReturnType,
			typename LeftType,
			typename RightType,
			class	 Operator>
		class Binary :
			public Expression <ReturnType,
				   Binary     <ReturnType, LeftType, RightType, Operator >>
		{
		protected:
			const LeftType& _lhs;
			const RightType& _rhs;

		public:
			// STL iterator interface
			typedef typename traits<Binary>::iterator iterator;
			typedef typename traits<Binary>::const_iterator const_iterator;

			// Return expression_iterator wrapper for derived *this.
			iterator begin() { return iterator(CRTP_DOWNCAST(const Operator&), 0); }
			iterator end()	 { return iterator(CRTP_DOWNCAST(const Operator&), size()); }

			const_iterator cbegin() const { return const_iterator(CRTP_DOWNCAST(const Operator&), 0); }
			const_iterator cend()	const { return const_iterator(CRTP_DOWNCAST(const Operator&), size()); }

			Binary(const LeftType& lhs, const RightType& rhs) : _lhs(lhs), _rhs(rhs) {}
			std::size_t size() const { return _lhs.size(); }

			// This is where the binary operation is actually performed
			// Cast to derived class via CRTP and call overloaded [] operator
			ReturnType operator [] (std::size_t i) const { return CRTP_DOWNCAST(const Operator&)[i]; }
		};

		// Defines iterator traits for derived specialization Binary
		template <typename Re, typename L, typename R, class O>
		struct traits<Binary<Re, L, R, O>>
		{
			typedef typename iterators::expression_iterator<O> iterator;
			typedef typename iterators::expression_iterator<O> const_iterator;
			typedef typename Re value_type;
		};

		// Holds the unary expression of a single object
		// Example: element[i]
		// Example: log(element[i])
		template <
			typename ReturnType,
			typename ContainerType,
			class	 Operator>
		class Unary :
			public Expression <ReturnType,
				   Unary	  <ReturnType, ContainerType, Operator >>
		{
		protected:
			const ContainerType& _element;

		public:
			// STL iterator interface
			typedef typename traits<Unary>::iterator iterator;
			typedef typename traits<Unary>::const_iterator const_iterator;

			iterator begin() { return _element.begin(); }
			iterator end()	 { return _element.end(); }

			const_iterator cbegin() const { return _element.cbegin(); }
			const_iterator cend()	const { return _element.cend(); }

			Unary(const ContainerType& element) : _element(element) {}
			std::size_t size() const { return _element.size(); }

			// This is where the binary operation is actually performed
			// We require the accessor to be overridden by an Operator class.
			// Example: return op(_element[i])
			ReturnType operator [] (std::size_t i) const { return CRTP_DOWNCAST(const Operator&)[i]; }
		};

		// Defines iterator traits for derived specialization Unary
		template <typename Re, typename T, class O>
		struct traits<Unary<Re, T, O>>
		{
			typedef typename iterators::expression_iterator<O> iterator;
			typedef typename iterators::expression_iterator<O> const_iterator;
			typedef typename Re value_type;
		};
	}
}