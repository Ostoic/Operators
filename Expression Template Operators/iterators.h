#pragma once

#include <iterator>
#include <boost\iterator\zip_iterator.hpp>

namespace etree		{
namespace iterators {

// expression_iterator is an STL-style input iterator (with bidirectional properties).
// It is very similar to a standard random-access iterator used with arrays,
// though it expects that the expression accessor operator [] is overloaded.
// Expectations:
//		- Exp_Type has the scalar type "value_type"

template <class Exp_Type>
	class expression_iterator : public std::iterator<std::input_iterator_tag, Exp_Type>
{
	const Exp_Type& expression;
	std::size_t index = 0;

public:
	typedef typename Exp_Type::value_type value_type;
	typedef typename Exp_Type::iterator base_iterator;

	expression_iterator(const Exp_Type& exp, const std::size_t i) : expression(exp), index(i) {}

	// Arithmetic operators
	// Increment prefix and postfix
	expression_iterator& operator ++() { ++index; return *this; }
	expression_iterator operator ++(int)
	{
		expression_iterator temp(*this);
		temp.operator++();
		return temp;
	}

	// Decrement prefix and postfix
	expression_iterator& operator --() { --index; return *this; }
	expression_iterator operator --(int)
	{
		expression_iterator temp(*this);
		temp.operator--();
		return temp;
	}

	template <typename E>
	inline friend bool operator ==(const expression_iterator<E>& lhs, const expression_iterator<E>& rhs);
				
	template <typename E>
	inline friend bool operator !=(const expression_iterator<E>& lhs, const expression_iterator<E>& rhs);

	//// Returns expression overload at current index
	//value_type operator *() 
	//{ 
	//	return *(expression.begin() + this->index);
	//};
};

// Relational operators
template <typename Exp_Type>
inline bool operator ==(const expression_iterator<Exp_Type>& lhs, const expression_iterator<Exp_Type>& rhs)
{
	return lhs.index == rhs.index;
}

template <typename Exp_Type>
inline bool operator !=(const expression_iterator<Exp_Type>& lhs, const expression_iterator<Exp_Type>& rhs)
{
	return !operator ==(lhs, rhs);
}

template <class Exp_Type>
class binary_iterator : public expression_iterator<Exp_Type>
{
public:
	typedef typename boost::tuple<base_iterator, base_iterator> tuple;
	typedef typename boost::zip_iterator<tuple> zip_iterator;

	

private:

public:

	/*value_type operator *()
	{
	return boost::make_zip_iterator(
	boost::tuple(expression.left.begin() + index, expression.right.begin() + index))
	}*/
};

template <class Exp_Type>
class unary_iterator : public expression_iterator<Exp_Type>
{
private:


public:

	/*value_type operator *()
	{
	return boost::make_zip_iterator(
	boost::tuple(expression.left.begin() + index, expression.right.begin() + index))
	}*/
};

} // end namespace iterators
} // end namespace etree