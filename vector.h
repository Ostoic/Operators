#pragma once

#include <vap\config.h>
#include <vap\detail\traits.h>
#include <vap\detail\constructors.h>
#include <vap\expressions\operators.h>
#include <vap\execution_policy.h>

#include <vector>
#include <iostream>
#include <algorithm>

#ifdef VAP_USING_THRUST
#include <thrust\copy.h>
#include <thrust\execution_policy.h>
#endif

namespace vap		  {
namespace expressions {

// Default data container is std::vector<T> 
// Default ctor is LoopConstructor
// The construction policy can be overriden as in the case of thrust::device_vector.
// Thus, thrust::transform, or a similar method can be used in place of the loop ctor
// This allows for proper parallelization of the vap expressions
template <typename T, 
		  typename Constructor		= constructors::Loop,
		  typename Container		= std::vector<T>,
		  typename Execution_Policy = serial_execution>
class vector : 
	public  expressions::Expression<vap::expressions::vector<T, Constructor, Container, Execution_Policy>>,
	private Constructor
{
protected:
	Container elements;

public:
	iterator begin() { return elements.begin(); }
	iterator end()	 { return elements.end(); }

	const_iterator cbegin() const { return elements.cbegin(); }
	const_iterator cend()	const { return elements.cend(); }

	// Interface for interacting with the underlying data
	const T&  operator [] (const std::size_t i) const { return elements[i]; }
	T operator [] (const std::size_t i)				  { return elements[i]; }
	std::size_t size()							const { return elements.size(); }

	void assign(const std::size_t count, const T& element)  { elements.assign(count, element); }
	void resize(const std::size_t size)						{ elements.resize(size); }

	// CTOR policy details
	using Constructor::ctor;
	using Constructor::assignment;

	vector() {}
	vector(const std::size_t n) : elements(n) {}
	vector(const Container& c)  : elements(c) {}

	explicit vector(Container&& c) : elements(std::move(c)) {}

	// The actual evaluation is done here in the constructor for vector
	template <typename E,
			  typename = std::enable_if_t<vap::is_expression<E>::value>>
	vector(const expressions::Expression<E>& e) : elements(e.size())
	{ ctor(elements, e); }

	template <typename E,
			  typename = std::enable_if_t<vap::is_expression<E>::value>>
	vector& operator = (const expressions::Expression<E>& e)
	{ 
		if (elements.size() < e.size()) elements.resize(e.size());
		assignment(elements, e); 
	}

	operator	   Container&()		  { return this->elements; }
	operator const Container&() const { return this->elements; }

	template <typename U, typename C>
	friend bool operator == (const vector<U>& lhs, const C& rhs);

	template <typename U, typename C>
	friend bool operator == (const C& lhs, const vector<U>& rhs);
};

// Vector on the left, container on the right
template <typename U, typename C>
bool operator == (const vap::expressions::vector<U>& lhs, const C& rhs)
{
	return lhs.elements == rhs;
}

// Container on the left, vector on the right
template <typename U, typename C>
bool operator == (const C& lhs, const vap::expressions::vector<U>& rhs)
{
	return rhs.elements == lhs;
}

} // end namespace expressions

using expressions::vector;

} // end namespace vap