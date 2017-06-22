#pragma once

#include <iterator>
#include <boost\iterator\iterator_adaptor.hpp>

#include "..\execution_policy.h"
#include "..\config\config.h"
#pragma once

#include <iterator>
#include <boost\iterator\iterator_adaptor.hpp>

#include "..\execution_policy.h"
#include "..\config\config.h"

#include "iterator_traits.h"

namespace etree		{
namespace iterators {	

template <class Iterator,
		  class UnaryFunction>
class unary_iterator_base
{
public:
	unary_iterator_base(const Iterator& x, UnaryFunction f) : it(x), function(f) {}

	UnaryFunction functor() const { return function; }

	const Iterator& base() const { return it; }

protected:
	Iterator it;
	UnaryFunction function;
};

template <class... Ts>
class unary_iterator
{
	//static_assert(false, "Invalid execution policy specified for binary_iterator");
};
	
template <class Iterator,
		  class Value,
		  class UnaryFunction>
class unary_iterator  <Iterator,
					   Value,
					   UnaryFunction,
					   serial_policy<Value>> // parallel execution policy
					   :
	  // extends binary_iterator_base
	  public unary_iterator_base <Iterator, 
								  UnaryFunction>,
	  // extends as a bidirectional iterator
	  public std::iterator <std::bidirectional_iterator_tag, Value>
{
public:
	using value_type = Value;

	unary_iterator(const Iterator& x, UnaryFunction f) : unary_iterator_base(x, f) {}

	// Policy-specific implementation
	value_type operator* () const { return function(*it); }

	// Policy-specific implementation
	unary_iterator& operator++()
	{
		++it;
		return *this;
	}

	// Policy-specific implementation
	unary_iterator& operator--()
	{
		--it;
		return *this;
	}

	template <class... Ts>
	friend inline bool operator== (const unary_iterator<Ts...>&, const unary_iterator<Ts...>&);

	template <class... Ts>
	friend inline bool operator!= (const unary_iterator<Ts...>&, const unary_iterator<Ts...>&);
};

	  
template <class Iterator,
		  class Value,
		  class UnaryFunction>
class unary_iterator  <Iterator,
					   Value,
					   UnaryFunction,
					   thrust_policy<Value>> // parallel execution policy
					   :
	  // extends binary_iterator_base
	  public unary_iterator_base <Iterator, 
								  UnaryFunction>,
	  // extends as a bidirectional iterator
	  public std::iterator <std::bidirectional_iterator_tag, Value>
{
public:
	using value_type = Value;

	unary_iterator(const Iterator& x, UnaryFunction f) : unary_iterator_base(x, f) {}

	// Policy-specific implementation
	THRUST_DEVICE(
	value_type operator* () { return function(*it); })

	// Policy-specific implementation
	THRUST_DEVICE(
	unary_iterator& operator++()
	{
		++it;
		return *this;
	})

	// Policy-specific implementation
	THRUST_DEVICE(
	unary_iterator& operator--()
	{
		--it;
		return *this;
	})

	template <class... Ts>
	friend inline bool operator== (const unary_iterator<Ts...>&, const unary_iterator<Ts...>&);

	template <class... Ts>
	friend inline bool operator!= (const unary_iterator<Ts...>&, const unary_iterator<Ts...>&);
};


template <class... Ts>
inline bool operator== (const unary_iterator<Ts...>& lhs, const unary_iterator<Ts...>& rhs)
{
	return lhs.base() == rhs.base();
}


template <class... Ts>
inline bool operator!= (const unary_iterator<Ts...>& lhs, const unary_iterator<Ts...>& rhs)
{
	return !operator== (lhs, rhs);
}

} // end namespace iterators
} // end namespace etree