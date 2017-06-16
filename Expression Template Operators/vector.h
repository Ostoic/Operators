#pragma once

#include "operators.h"
#include "iterators.h"

#include <vector>
#include <algorithm>

namespace etree		   {
namespace constructors {
// This is the default constructor for the vector class below
class Loop
{
protected:
	template <class C, class E>
	void ctor(C& c, const E& e)
	{
		auto sz = e.size();
		for (std::size_t i = 0; i < sz; ++i)
			c[i] = e[i];
	}

	template <class C, class E>
	void assignment(C &c, const E& e)
	{
		ctor(c, e);
	}
};

class STL
{
protected:
	template <class Container, class Exp>
	void ctor(Container& c, const Exp& expression)
	{
		std::copy(expression.cbegin(), expression.cend(), c.begin());
	}

	template <class Container, class Exp>
	void assignment(Container &c, const Exp& e)
	{
		ctor(c, e);
	}
};

} // end namespace construcors

// Default data container is std::vector<T> 
// Default ctor is LoopConstructor
// The construction policy can be overriden as in the case of thrust::device_vector.
// Thus, thrust::transform, or a similar method can be used in place of the loop ctor
// This allows for proper parallelization of the etree expressions
template <
	typename T, 
	class    ConstructPolicy = constructors::Loop,
	typename Container = std::vector<T>>
class vector : 
	public  expressions::Expression<T, etree::vector<T, ConstructPolicy, Container>>,
	private ConstructPolicy
{
protected:
	Container elements;

public:
	// Provide interface for STL iteration
	typedef typename expressions
		::traits<vector>
		::iterator iterator;

	typedef typename expressions
		::traits<vector>
		::const_iterator const_iterator;

	iterator begin() { return elements.begin(); }
	iterator end()	 { return elements.end(); }

	const_iterator cbegin() const { return elements.cbegin(); }
	const_iterator cend()	const { return elements.cend(); }

	// Interface for interacting with the underlying data
	T  operator [] (const std::size_t i) const { return elements[i]; }
	T& operator [] (const std::size_t i)	   { return elements[i]; }
	std::size_t size()					 const { return elements.size(); }

	void assign(std::size_t count, const T& element)  { elements.assign(count, element); }
	void resize(std::size_t size)					  { elements.resize(size); }

	// CTOR policy details
	using ConstructPolicy::ctor;
	using ConstructPolicy::assignment;

	vector() {}
	vector(std::size_t n) : elements(n) {}
	//vector(const Container& c) : elements(c) {}

	// The actual evaluation is done here in the constructor for vector
	// The [] operator is overloaded for the Expression e
	template <typename E>
	vector(const expressions::Expression<T, E>& e) :
		elements(e.size())
	{ ctor(elements, e); }

	/*template <typename E>
	vector<T, Container, ConstructPolicy>& operator = (const expressions::Expression<T, E>& e)
	{ assignment(elements, e); }*/

	operator Container&() { return this->elements; }
	operator const Container&() const { return this->elements; }

	template <typename U, typename C>
	friend bool operator == (const vector<U>& lhs, const C& rhs);

	template <typename U, typename C>
	friend bool operator == (const C& lhs, const vector<U>& rhs);
};

// Define traits of vector as an expression
// This is the vector traits template specialization.
// Since CRTP typedef vision is limited, we have to rely on the traits idiom.
template <typename T, class P, typename C>
struct expressions::traits<etree::vector<T, P, C>>
{
	typedef typename C::iterator iterator;
	typedef typename C::const_iterator const_iterator;
};

// Vector on the left, container on the right
template <typename U, typename C>
bool operator == (const etree::vector<U>& lhs, const C& rhs)
{
	return lhs.elements == rhs;
}

// Container on the left, vector on the right
template <typename U, typename C>
bool operator == (const C& lhs, const etree::vector<U>& rhs)
{
	return rhs.elements == lhs;
}

} // end namespace etree