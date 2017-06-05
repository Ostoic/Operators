#pragma once

#include "operators.h"

#include <vector>

template <typename T>
class Vec : public agac::expressions::Expression<T, Vec<T>>
{
private:
	std::vector<T> elements;

public:
	T  operator [] (const std::size_t i) const { return elements[i]; }
	T& operator [] (const std::size_t i)	   { return elements[i]; }
	std::size_t size()					 const { return elements.size(); }

	void assign(std::size_t count, T element)  { return elements.assign(count, element); }

	// Preallocate template
	Vec(std::size_t n) : elements(n) {}

	// The actual evaluation is done here in the constructor for Vec
	// The [] operator is overloaded for the Expression e
	template <typename E>
	Vec(const agac::expressions::Expression<T, E>& e) :
		elements(e.size())
	{
		std::size_t sz = e.size();

		for (std::size_t i = 0; i < sz; i++)
			elements[i] = e[i];
	}
};