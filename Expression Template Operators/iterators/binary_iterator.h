#pragma once

#include <iterator>
#include <boost\iterator\iterator_adaptor.hpp>

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

	BinaryFunction functor() const { return function; }

	const Left_Iterator& left_base() const { return left; }
	const Right_Iterator& right_base() const { return right; }

protected:
	Left_Iterator  left;
	Right_Iterator right;
	BinaryFunction function;
};

//template <class... Ts>
//class binary_iterator
//{
//	//static_assert(false, "Invalid execution policy specified for binary_iterator");
//};

// Serial specialization of binary_iterator
template <class Left_Iterator,
		  class Right_Iterator,
		  class BinaryFunction>
class binary_iterator // serial execution policy
					  :													 // extends binary_iterator_base
	  public binary_iterator_base <Left_Iterator, 
								   Right_Iterator, 
								   BinaryFunction> 
{
public:
	using value_type = typename Left_Iterator::value_type;

	binary_iterator(const Left_Iterator& lhs, const Right_Iterator& rhs, BinaryFunction f) : binary_iterator_base(lhs, rhs, f) {}

	// Policy-specific implementation
	value_type operator * () const { return function(*left, *right); }

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

	template <class L, class R, class F>
	friend inline bool operator== (const binary_iterator<L, R, F>&, const binary_iterator<L, R, F>&);

	template <class L, class R, class F>
	friend inline bool operator!= (const binary_iterator<L, R, F>& lhs, const binary_iterator<L, R, F>& rhs);
};

template <class L, class R, class F>
inline bool operator== (const binary_iterator<L, R, F>& lhs, const binary_iterator<L, R, F>& rhs)
{
	return lhs.left_base()  == rhs.left_base() &&
		   lhs.right_base() == rhs.right_base();
}


template <class L, class R, class F>
inline bool operator!= (const binary_iterator<L, R, F>& lhs, const binary_iterator<L, R, F>& rhs)
{
	return !operator== (lhs, rhs);
}

//
//// Parellel specialization of binary_iterator
//template <class Left_Iterator,
//		  class Right_Iterator,
//		  class BinaryFunction>
//class binary_iterator<Left_Iterator,
//					  Right_Iterator, 
//					  BinaryFunction,
//					  thrust_policy<typename Left_Iterator::value_type>> // parllel execution policy
//					  :													 // extends binary_iterator_base
//	  public binary_iterator_base <Left_Iterator, 
//								   Right_Iterator, 
//								   BinaryFunction> 
//{
//	using value_type = typename Left_Iterator::value_type;
//
//	binary_iterator(const Left_Iterator& lhs, const Right_Iterator& rhs, BinaryFunction f) {}
//
//	// Policy-specific implementation
//	HOST_DEVICE(
//	value_type operator * () const { return function(*left, *right); })
//
//	// Policy-specific implementation
//	HOST_DEVICE(
//	binary_iterator& operator++()
//	{
//		++left; ++right;
//		return *this;
//	})
//
//	// Policy-specific implementation
//	HOST_DEVICE(
//	binary_iterator& operator--()
//	{
//		--left; --right;
//		return *this;
//	})
//};

} // end namespace iterators
} // end namespace etree