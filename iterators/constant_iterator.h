#pragma once

#include <vap\config.h>

#include <iterator>

/*
typedef typename T::difference_type difference_type;
typedef typename T::value_type value_type;
typedef typename T::pointer pointer;
typedef typename T::reference reference;
typedef typename T::iterator_category iterator_category;
*/

namespace vap 		{
namespace iterators {

template <typename Type>
class constant_iterator : std::iterator<std::forward_iterator_tag, Type>
{
public:
	using difference_type	= std::size_t;
	using value_type		= Type;
	using pointer			= Type;
	using reference			= Type;
	using iterator_category = std::forward_iterator_tag;

	ANY_SYSTEM
	constant_iterator(const Type& val) : value(val) {}

	ANY_SYSTEM
	constant_iterator& operator ++() { return *this; }

	ANY_SYSTEM
	Type operator *() { return value; }
	
private:
	Type value;
};

template <typename Type>
constant_iterator<Type> make_constant_iterator(Type x)
{
	return constant_iterator<Type>(x);
}

} // end namespace iterators

using iterators::constant_iterator;
using iterators::make_constant_iterator;

} // end namespace vap