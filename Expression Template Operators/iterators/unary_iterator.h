#pragma once

#include <iterator>
#include <boost\iterator\iterator_adaptor.hpp>

#include "..\execution_policy.h"
#include "..\config\config.h"


namespace etree		{
namespace iterators {

template <class Iterator,
		  class UnaryFunction>
class unary_iterator
{
public:

	using result_type = typename UnaryFunction::result_type;

	unary_iterator(const Iterator& it, UnaryFunction f) : iterator(it), function(f) {}

	// Host, Device code
	result_type operator * () { return function(*right); }

	// Host, Device code
	unary_iterator& operator++()
	{
		++iterator;
		return *this;
	}

	// Host, Device code
	unary_iterator& operator--()
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