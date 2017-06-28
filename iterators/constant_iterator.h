#pragma once

#include <vap\config.h>

#include <iterator>

namespace vap 		{
namespace iterators {

template <typename Type>
class constant_iterator : std::iterator<std::input_iterator_tag, Type>
{
public:
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

using iterators::make_constant_iterator;

} // end namespace vap