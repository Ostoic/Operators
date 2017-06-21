#pragma once

#include <iterator>
#include <boost\iterator\zip_iterator.hpp>

namespace etree		{
namespace iterators {

template <class Left_Iterator,
		  class Right_Iterator,
		  class BinaryFunction>
class binary_etree_iterator
{
public:

	using result_type = typename BinaryFunction::result_type;

	//binary_etree_iterator() {}
	binary_etree_iterator(const Left_Iterator& lhs, const Right_Iterator& rhs, BinaryFunction f) : left(lhs), right(rhs), function(f) {}

	result_type operator * () const { return function(*left, *right); }

	binary_etree_iterator& operator++()
	{
		++left; ++right;
		return *this;
	}

	binary_etree_iterator& operator--()
	{
		--left; --right;
		return *this;
	}

	BinaryFunction functor() const { return function; }

	const Left_Iterator& left_base() const { return iterator; }
	const Right_Iterator& right_base() const { return iterator; }

private:
	Left_Iterator  left;
	Right_Iterator right;
	BinaryFunction function;
};

template <class Iterator,
		  class UnaryFunction>
class unary_etree_iterator
{
public:

	using result_type = typename UnaryFunction::result_type;

	//binary_etree_iterator() {}
	unary_etree_iterator(const Iterator& it, UnaryFunction f) : iterator(it), function(f) {}

	result_type operator * () { return function(*right); }

	unary_etree_iterator& operator++()
	{
		++iterator;
		return *this;
	}

	unary_etree_iterator& operator--()
	{
		--iterator;
		return *this;
	}

	UnaryFunction functor() const { return function; }

	const Iterator& base() const { return iterator; }

private:
	Iterator iterator;
	UnaryFunction function;
};

} // end namespace iterators
} // end namespace etree