#pragma once

#include "operators.h"

#include <vector>

namespace etree
{
	// This is the default constructor for the vector class below
	class LoopConstructor
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

	// Default data container is std::vector<T> 
	// Default ctor is LoopConstructor
	// The construction policy can be overriden as in the case of thrust::device_vector.
	// Thus, thrust::transform, or a similar method can be used in place of the loop ctor
	// This allows for proper parellization of the etree expressions
	template <typename T, 
			  typename Container = std::vector<T>,
			  class    ConstructPolicy = LoopConstructor>
	class vector : 
		public  expressions::Expression<T, vector<T, Container>>,
		private ConstructPolicy
	{
	protected:
		Container elements;

	public:
		// Provide interface for STL iteration
		typedef typename Container::iterator iterator;
		typedef typename Container::const_iterator const_iterator;

		iterator begin() { return elements.begin(); }
		iterator end()	 { return elements.end(); }

		const_iterator cbegin() { return elements.cbegin(); }
		const_iterator cend()	{ return elements.cend(); }

		// Interface for interacting with the underlying data
		T  operator [] (const std::size_t i) const { return elements[i]; }
		T& operator [] (const std::size_t i)	   { return elements[i]; }
		std::size_t size()					 const { return elements.size(); }

		void assign(std::size_t count, const T& element)  { return elements.assign(count, element); }
		void resize(std::size_t size)					  { elements.resize(size); }

		// CTOR policy details
		using ConstructPolicy::ctor;
		using ConstructPolicy::assignment;

		vector() {}
		vector(std::size_t n) : elements(n) {}

		// The actual evaluation is done here in the constructor for vector
		// The [] operator is overloaded for the Expression e
		template <typename E>
		vector(const expressions::Expression<T, E>& e) :
			elements(e.size())
		{ ctor(elements, e); }

		/*template <typename E>
		vector<T, Container, ConstructPolicy>& operator = (const expressions::Expression<T, E>& e)
		{ assignment(elements, e); }*/

		template <typename U, typename C>
		friend bool operator == (const vector<U>& lhs, const C& rhs);

		template <typename U, typename C>
		friend bool operator == (const C& lhs, const vector<U>& rhs);
	};

	template <typename U, typename C>
	bool operator == (const vector<U>& lhs, const C& rhs)
	{
		return lhs.elements == rhs;
	}

	template <typename U, typename C>
	bool operator == (const C& lhs, const vector<U>& rhs)
	{
		return rhs.elements == lhs;
	}
}