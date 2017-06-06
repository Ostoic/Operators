#pragma once

#include "operators.h"

#include <vector>

template <typename T>
class Vec : public agac::expressions::Expression<T, Vec<T>>
{
protected:
	std::vector<T> elements;

public:
	T  operator [] (const std::size_t i) const { return elements[i]; }
	T& operator [] (const std::size_t i)	   { return elements[i]; }
	std::size_t size()					 const { return elements.size(); }

	void assign(std::size_t count, const T& element)  { return elements.assign(count, element); }
	void resize(std::size_t size)					  { elements.resize(size); }

	// Preallocate template
	Vec() {}
	Vec(std::size_t n) : elements(n) {}

	// The actual evaluation is done here in the constructor for Vec
	// The [] operator is overloaded for the Expression e
	template <typename E>
	Vec(const agac::expressions::Expression<T, E>& e) :
		elements(e.size())
	{
		std::size_t sz = e.size();

		for (std::size_t i = 0; i != sz; ++i)
			elements[i] = e[i];
	}

	template <typename U>
	friend bool operator == (const Vec<U>& lhs, const std::vector<U>& rhs);

	template <typename U>
	friend bool operator == (const std::vector<U>& lhs, const Vec<U>& rhs);
};

template <typename T>
bool operator == (const Vec<T>& lhs, const std::vector<T>& rhs)
{
	return lhs.elements == rhs;
}

template <typename T>
bool operator == (const std::vector<T>& lhs, const Vec<T>& rhs)
{
	return rhs.elements == lhs;
}