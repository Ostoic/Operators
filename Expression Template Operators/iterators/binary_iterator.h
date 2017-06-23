#pragma once

#include <iterator>

#include "..\execution_policy.h"
#include "..\config\config.h"

#include "iterator_traits.h"

namespace etree		{
namespace iterators {

template <class Left_Iterator,
		  class Right_Iterator,
		  class BinaryFunction>
class binary_iterator_base
{
public:
	binary_iterator_base(const Left_Iterator& lhs, const Right_Iterator& rhs, BinaryFunction f) : left(lhs), right(rhs), function(f) {}

	const BinaryFunction& functor() const { return function; }

	const Left_Iterator& left_base()   const { return left; }
	const Right_Iterator& right_base() const { return right; }

protected:
	Left_Iterator  left;
	Right_Iterator right;
	BinaryFunction function;
};

template <class... Ts>
class binary_iterator
{
	//static_assert(false, "Invalid execution policy specified for binary_iterator");
};

// Serial specialization of binary_iterator
template <class Left_Iterator,
		  class Right_Iterator,
		  class Value,
		  class BinaryFunction>
class binary_iterator <Left_Iterator,
					   Right_Iterator,
					   Value,
					   BinaryFunction,
					   serial_policy<Value>> // serial execution policy
					   :
	  // extends binary_iterator_base
	  public binary_iterator_base <Left_Iterator, 
								   Right_Iterator, 
								   BinaryFunction>,
	  // extends as a bidirectional iterator
	  public std::iterator <std::bidirectional_iterator_tag, Value>
{
public:
	using value_type = Value;

	binary_iterator(const Left_Iterator& lhs, const Right_Iterator& rhs, BinaryFunction f) : binary_iterator_base(lhs, rhs, f) {}

	// Policy-specific implementation
	value_type operator *() const { return function(*left, *right); }

	// Policy-specific implementation
	binary_iterator& operator++()
	{
		++left; ++right;
		return *this;
	}

	// Policy-specific implementation
	binary_iterator& operator--()
	{
		--left; --right;
		return *this;
	}

	template <class... Ts>
	friend inline bool operator== (const binary_iterator<Ts...>&, const binary_iterator<Ts...>&);

	template <class... Ts>
	friend inline bool operator!= (const binary_iterator<Ts...>&, const binary_iterator<Ts...>&);
};


// Parallel specialization of binary_iterator
template <class Left_Iterator,
		  class Right_Iterator,
		  class Value,
		  class BinaryFunction>
class binary_iterator <Left_Iterator,
					   Right_Iterator,
					   Value,
					   BinaryFunction,
					   thrust_policy<Value>> // parallel execution policy
					   :
	  // extends binary_iterator_base
	  public binary_iterator_base <Left_Iterator, 
								   Right_Iterator, 
								   BinaryFunction>,
	  // extends as a bidirectional iterator
	  public std::iterator <std::bidirectional_iterator_tag, Value>
{
public:
	using value_type = Value;

	binary_iterator(const Left_Iterator& lhs, const Right_Iterator& rhs, BinaryFunction f) : binary_iterator_base(lhs, rhs, f) {}

	// Policy-specific implementation
	THRUST_DEVICE(
	value_type operator * () const { return function(*left, *right); })

	// Policy-specific implementation
	THRUST_DEVICE(
	binary_iterator& operator++()
	{
		++left; ++right;
		return *this;
	})

	// Policy-specific implementation
	THRUST_DEVICE(
	binary_iterator& operator--()
	{
		--left; --right;
		return *this;
	})

	template <class... Ts>
	friend inline bool operator== (const binary_iterator<Ts...>&, const binary_iterator<Ts...>&);

	template <class... Ts>
	friend inline bool operator!= (const binary_iterator<Ts...>&, const binary_iterator<Ts...>&);
};

template <class... Ts>
inline bool operator== (const binary_iterator<Ts...>& lhs, const binary_iterator<Ts...>& rhs)
{
	return lhs.left_base()  == rhs.left_base() &&
		   lhs.right_base() == rhs.right_base(); 
}


template <class... Ts>
inline bool operator!= (const binary_iterator<Ts...>& lhs, const binary_iterator<Ts...>& rhs)
{
	return !operator== (lhs, rhs);
}

} // end namespace iterators
} // end namespace etree