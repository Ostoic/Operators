#pragma once

#include "iterators.h"

// This is more descriptive than the usual static_cast code
#define CRTP_DOWNCAST(_D) static_cast<_D>(*this)

namespace etree {

// Base execution policy used for selecting different execution systems
template <class Derived>
struct execution_policy {};

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

	// STL iterator interface
	//typedef typename traits<Derived>::expr_iterator expr_iterator;

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
	public Expression <Binary <Left, Right, Operator >>
{
protected:
	const Left& left;
	const Right& right;

public:
	// STL iterator interface
	typedef typename traits<Binary>::value_type value_type;
	typedef typename traits<Binary>::container_type container_type;

	typedef typename traits<Binary>::expr_iterator expr_iterator;

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
	value_type operator [] (std::size_t i) const { return CRTP_DOWNCAST(const Operator&)[i]; }
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

	typedef typename traits<Unary>::expr_iterator expr_iterator;

	typedef typename traits<Unary>::iterator iterator;
	typedef typename traits<Unary>::const_iterator const_iterator;

	// Return expression_iterator wrapper for derived *this.
	iterator begin() { return iterator(CRTP_DOWNCAST(const Operator&), 0); }
	iterator end()	 { return iterator(CRTP_DOWNCAST(const Operator&), size()); }

	const_iterator cbegin() const { return const_iterator(CRTP_DOWNCAST(const Operator&), 0); }
	const_iterator cend()	const { return const_iterator(CRTP_DOWNCAST(const Operator&), size()); }

	Unary(const Exp& exp) : expression(exp) {}
	std::size_t size() const { return expression.size(); }

	// This is where the binary operation is actually performed
	// We require the accessor to be overridden by an Operator class.
	// Example: return op(_element[i])
	value_type operator [] (std::size_t i) const { return CRTP_DOWNCAST(const Operator&)[i]; }
};

// Defines traits for derived specialization Binary
// Specialization: Left type is an expression
template <typename L, typename R, class O>
struct traits<Binary<L, R, O>>
{
	typedef typename L::value_type value_type;
	typedef typename L::container_type container_type;

	typedef typename container_type::iterator  iterator;
	typedef typename container_type::const_iterator const_iterator;
	typedef typename iterators::binary_iterator<O> expr_iterator;
};

// Defines traits for derived specialization Unary
template <typename E, class O>
struct traits<Unary<E, O>>
{
	// is_expression<E> then typedef E::container_type container_type
	// otherwise typedef E container_type
	typedef typename E::value_type value_type;
	typedef typename E::container_type container_type;

	typedef typename container_type::iterator  iterator;
	typedef typename container_type::const_iterator const_iterator;
	typedef typename iterators::unary_iterator<O> expr_iterator;
};
	
} // end namespace expressions
} // end namespace etree