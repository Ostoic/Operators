#pragma once

namespace etree		{
namespace iterators {

template<typename T>
struct iterator_traits
{
	using value_type	 = typename T::value_type;
	/*using Left_Iterator  = typename T::Left_Iterator;
	using Right_Iterator = typename T::Right_Iterator;
	using BinaryFunction = typename T::BinaryFunction;*/

	//typedef typename T::difference_type difference_type;
	//typedef typename T::pointer pointer;
	//typedef typename T::reference reference;
	//typedef typename T::iterator_category iterator_category;
};

} // end namespace iterators
} // end namespace etree
