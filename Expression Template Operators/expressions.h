#pragma once

#include "iterators.h"

// This is more descriptive than the usual static_cast code
#define CRTP_DOWNCAST(_D) static_cast<_D>(*this)

namespace etree {

namespace expressions {

template <typename D>
struct traits;


// Represents any operator expression
template <class Derived>
class Expression
{
public:
	// Save the return type of the expression
	typedef typename traits<Derived>::value_type value_type;
	typedef typename traits<Derived>::container_type container_type;

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
	value_type operator [] (std::size_t i) const { return CRTP_DOWNCAST(const Derived&)[i]; }
	std::size_t size()					   const { return CRTP_DOWNCAST(const Derived&).size(); }

	// Provides implicit (or C-style) cast to expression type
	operator Derived& ()			 { return CRTP_DOWNCAST(Derived&); }
	operator const Derived& () const { return CRTP_DOWNCAST(const Derived&); }
};

// Holds the binary expression of two objects. 
template <typename Left,
		  typename Right,
		  class	   Operator>
class Binary :
	public Expression <Binary <Left, Right, Operator>>
{
protected:
	const Left& left;
	const Right& right;

public:
	// STL iterator interface
	typedef typename traits<Binary>::value_type value_type;
	typedef typename traits<Binary>::container_type container_type;

	typedef typename traits<Binary>::iterator iterator;
	typedef typename traits<Binary>::const_iterator const_iterator;

	// Return expression_iterator wrapper for derived *this.
	iterator begin() { return iterator(CRTP_DOWNCAST(const Operator&), 0); }
	iterator end()	 { return iterator(CRTP_DOWNCAST(const Operator&), size()); }

	const_iterator cbegin() const { return const_iterator(CRTP_DOWNCAST(const Operator&), 0); }
	const_iterator cend()	const { return const_iterator(CRTP_DOWNCAST(const Operator&), size()); }

	Binary(const Left& lhs, const Right& rhs) : left(lhs), right(rhs) {}
	std::size_t size() const { return left.size(); }

	// This is where the binary operation is actually performed
	// Cast to derived class via CRTP and call overloaded [] operator
	value_type operator [] (std::size_t i) const { return Operator::apply(left[i], right[i]); }
};

// Holds the unary expression of a single object
// Example: element[i]
// Example: log(element[i])
template <typename Exp,
		  class	   Operator>
class Unary :
	public Expression <Unary <Exp, Operator >>
{
protected:
	const Exp& expression;

public:
	// STL iterator interface
	typedef typename traits<Unary>::value_type value_type;
	typedef typename traits<Unary>::container_type container_type;

	typedef typename traits<Unary>::iterator iterator;
	typedef typename traits<Unary>::const_iterator const_iterator;

	// Return expression_iterator wrapper for derived *this.
	iterator begin() { return iterator(CRTP_DOWNCAST(const Operator&), 0); }
	iterator end()	 { return iterator(CRTP_DOWNCAST(const Operator&), size()); }

	const_iterator cbegin() const { return const_iterator(CRTP_DOWNCAST(const Operator&), 0); }
	const_iterator cend()	const { return const_iterator(CRTP_DOWNCAST(const Operator&), size()); }

	using Operator::apply;

	Unary(const Exp& exp) : expression(exp) {}
	std::size_t size() const { return expression.size(); }

	// This is where the binary operation is actually performed
	// We require the accessor to be overridden by an Operator class.
	// Example: return op(_element[i])
	value_type operator [] (std::size_t i) const { return Operator::apply(expression[i]); }
	//value_type operator [] (std::size_t i) const { return CRTP_DOWNCAST(const Operator&)[i]; }
};

// Defines traits for derived specialization Binary
template <typename L, typename R, class O>
struct traits<Binary<L, R, O>>
{
	using value_type		   = typename L::value_type;
	using container_type	   = typename L::container_type;

	using left_iterator		   = typename L::iterator;
	using left_const_iterator  = typename L::iterator;

	using right_iterator	   = typename R::iterator;
	using right_const_iterator = typename R::iterator;

	using iterator		 = typename iterators
		::binary_etree_iterator<
		left_iterator,
		right_iterator, 
		typename O::apply>;

	using const_iterator = typename iterators
		::binary_etree_iterator<
		left_const_iterator,
		right_const_iterator,
		typename O::apply>;
};

// Defines traits for derived specialization Unary
template <typename T, class O>
struct traits<Unary<T, O>>
{
	using value_type		  = typename T::value_type;
	using container_type	  = typename T::container_type;

	using base_iterator		  = typename T::iterator;
	using const_base_iterator = typename T::const_iterator;

	using iterator			  = typename iterators::unary_etree_iterator<base_iterator, typename O::apply>;
	using const_iterator	  = typename iterators::unary_etree_iterator<const_base_iterator, typename O::apply>;
};
	
} // end namespace expressions
} // end namespace etree