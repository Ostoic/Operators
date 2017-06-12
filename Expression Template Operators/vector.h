#pragma once

#include "operators.h"

#include <vector>

namespace etree {
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

	/*class ThrustConstructor
	{
	protected:
		template <class Container, class Expression>
		void ctor(Container& c, const E& expression)
		{
			thrust::copy(expression.begin(), expression.end(), thrust::back_inserter(c));
		}

		template <class Container, class Expression>
		void assignment(Container &c, const Expression& e)
		{
			ctor(c, e);
		}
	};*/

	class STLConstructor
	{
	protected:
		template <class Container, class Expression>
		void ctor(Container& c, const E& expression)
		{
			std::copy(expression.begin(), expression.end(), std::back_inserter(c));
		}

		template <class Container, class Expression>
		void assignment(Container &c, const Expression& e)
		{
			ctor(c, e);
		}
	};

	// Default data container is std::vector<T> 
	// Default ctor is LoopConstructor
	// The construction policy can be overriden as in the case of thrust::device_vector.
	// Thus, thrust::transform, or a similar method can be used in place of the loop ctor
	// This allows for proper parallelization of the etree expressions
	template <typename T, 
			  typename Container = std::vector<T>,
			  class    ConstructPolicy = LoopConstructor>
	class vector : 
		public  expressions::Expression<T, etree::vector<T, Container, ConstructPolicy>>,
		private ConstructPolicy
	{
	protected:
		Container elements;

	public:
		// Provide interface for STL iteration
		typedef typename expressions
			::traits<vector<T, Container, ConstructPolicy>>
			::iterator iterator;

		typedef typename expressions
			::traits<vector<T, Container, ConstructPolicy>>
			::const_iterator const_iterator;

		iterator begin() { return elements.begin(); }
		iterator end()	 { return elements.end(); }

		const_iterator cbegin() { return elements.cbegin(); }
		const_iterator cend()	{ return elements.cend(); }

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

	// Define traits of vector as an expression
	// This is the vector traits template specialization.
	// Since CRTP typedef vision is limited, we have to rely on the traits idiom.
	template <typename T, typename C, typename... Ts>
	struct expressions::traits<etree::vector<T, C, Ts...>>
	{
		typedef typename C::iterator iterator;
		typedef typename C::const_iterator const_iterator;
	};

	template <typename U, typename C>
	bool operator == (const etree::vector<U>& lhs, const C& rhs)
	{
		return lhs.elements == rhs;
	}

	template <typename U, typename C>
	bool operator == (const C& lhs, const etree::vector<U>& rhs)
	{
		return rhs.elements == lhs;
	}
}