#pragma once

#include "detail\config.h"
#include "detail\iterators.h"
#include "execution_policy.h"

// This is more descriptive than the usual static_cast code
#define CRTP_DOWNCAST(_D) static_cast<_D>(*this)

namespace etree		  {
namespace expressions {

template <typename D>
struct traits;

// Represents any operator expression
template <class Derived>
class Expression
{
public:
	using iterator		 = typename traits<Derived>::iterator;
	using const_iterator = typename traits<Derived>::const_iterator;
	using value_type	 = typename traits<Derived>::value_type;

	// Forward the iterator interface downwards until we have enough 
	// class data to define a proper iterator type
	iterator begin() { return CRTP_DOWNCAST(Derived&).begin();  }
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
		  class	   Operator,
		  class	   Policy>
class Binary :
	public Expression <Binary <Left, Right, Operator, Policy>>
{
protected:
	const Left&  left;
	const Right& right;

	Operator apply;

public:
	using policy		 = typename traits<Binary>::policy;
	using iterator		 = typename traits<Binary>::iterator;
	using const_iterator = typename traits<Binary>::const_iterator;
	using value_type	 = typename traits<Binary>::value_type;

	Binary(const Left& lhs, const Right& rhs) : left(lhs), right(rhs) {}

	iterator begin() 
	{
		return make_transform_iterator(
			   make_zip_iterator	  (
			   make_tuple(left.begin(), right.begin())),
			   apply);
	}

	iterator end() 
	{
		return make_transform_iterator(
			   make_zip_iterator	  (
			   make_tuple(left.end(), right.end())),
			   apply);
	}

	const_iterator cbegin() const 
	{
		return make_transform_iterator(
			   make_zip_iterator	  (
			   make_tuple(left.cbegin(), right.cbegin())),
			   apply);
	}

	const_iterator cend() const
	{
		return make_transform_iterator(
			   make_zip_iterator(
			   make_tuple(left.cend(), right.cend())),
			   apply);
	}

	std::size_t size()					   const { return left.size(); }
	value_type operator [] (std::size_t i) const { return apply(boost::make_tuple(left[i], right[i])); }
};

// Holds the unary expression of a single object
// Example: element[i]
// Example: log(element[i])
template <typename Exp,
		  class	   Operator,
		  class	   Policy>
class Unary :
	public Expression <Unary <Exp, Operator, Policy>>
{
protected:
	const Exp& expression;

	Operator apply;

public:
	// STL iterator interface
	using policy		 = typename traits<Unary>::policy;
	using value_type	 = typename traits<Unary>::value_type;

	using iterator		 = typename traits<Unary>::iterator;
	using const_iterator = typename traits<Unary>::const_iterator;

	Unary(const Exp& exp) : expression(exp) {}

	iterator begin() 
	{
		return make_transform_iterator(expression.begin(), apply);
	}

	iterator end() 
	{
		return make_transform_iterator(expression.end(), apply);
	}

	const_iterator cbegin() const
	{
		return make_transform_iterator(expression.cbegin(), apply);
	}

	const_iterator cend() const
	{
		return make_transform_iterator(expression.cend(), apply);
	}

	std::size_t size()					   const { return expression.size(); }
	value_type operator [] (std::size_t i) const { return apply(expression[i]); }
};

// Defines traits for derived specialization Binary
template <typename L, typename R, class O, class P>
struct traits<Binary<L, R, O, P>>
{
	using policy = P;

	using value_type		   = typename L::value_type;

	using left_iterator		   = typename L::iterator;
	using left_const_iterator  = typename L::const_iterator;

	using right_iterator	   = typename R::iterator;
	using right_const_iterator = typename R::const_iterator;

	using iterator = typename iterators
		  ::binary_iterator<P>
		  ::template type<O, left_iterator, right_iterator>;
	
	using const_iterator = typename iterators
		  ::binary_iterator<P>
		  ::template type<O, left_const_iterator, right_const_iterator>;
};

// Defines traits for derived specialization Unary
template <typename T, class O, class P>
struct traits<Unary<T, O, P>>
{
	using policy = P;

	using value_type		  = typename T::value_type;

	using base_iterator		  = typename T::iterator;
	using const_base_iterator = typename T::const_iterator;
	
	using iterator = typename iterators
		  ::unary_iterator<P>
		  ::template type<O, base_iterator>;
	
	using const_iterator = typename iterators
		  ::unary_iterator<P>
		  ::template type<O, const_base_iterator>;
};
	
} // end namespace expressions
} // end namespace etree