#pragma once

// This header file contains the correct arithmetic operators and string conversion function
// for std::vector arithmetic (in particular the Algebra class).

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <Algebra\vector_operators.h>

#include "Neighbor.h"

// Note: ADL only searches namespaces associated with the given type, so to use these operators,
// a using directive (using namespace vector_operators) must be declared.
namespace neighbor_operators
{
	template <typename T>
	using _Vec = std::vector<T>;
	using namespace vector_operators;

	//--------------------------------
	// Order: vector _ neighbor
	//--------------------------------
	template <typename T, typename ElemOp>
	inline _Vec<T> hetero_operator(const _Vec<T> &v, Neighbor<T> const &nbhr, ElemOp c)
	{
		if (v.empty()) return v;

		std::size_t const size = v.size();
		_Vec<T> out(size);

		for (int x = 0; x < size; x++)
			out[x] = c(v[x], nbhr[x]);

		return out;
	}

	//--------------------------------
	// Order: neighbor _ vector
	//--------------------------------
	template <typename T, typename ElemOp>
	inline _Vec<T> hetero_operator(Neighbor<T> const &nbhr, const _Vec<T> &v, ElemOp c)
	{
		if (v.empty()) return v;

		std::size_t const size = v.size();
		_Vec<T> out(size);

		for (int x = 0; x < size; x++)
			out[x] = c(nbhr[x], v[x]);

		return out;
	}

	//-----------------------------------------------------
	// Homogenous Neighbor-Neighbor arithmetic operators
	//-----------------------------------------------------
	namespace homogenous
	{
		/********** Vector Binary operators **********/
		template <typename T>
		inline _Vec<T> operator + (const Neighbor<T> &v, const Neighbor<T> &u)
		{
			typedef std::plus<T> _Func;
			return binary_operator<T, Neighbor<T>, _Func>(v, u, _Func());
		}

		template <typename T>
		inline _Vec<T> operator - (const Neighbor<T> &v, const Neighbor<T> &u)
		{
			typedef std::minus<T> _Func;
			return binary_operator<T, Neighbor<T>, _Func>(v, u, _Func());
		}

		template <typename T>
		inline _Vec<T> operator * (const Neighbor<T> &v, const Neighbor<T> &u)
		{
			typedef std::multiplies<T> _Func;
			return binary_operator<T, Neighbor<T>, _Func>(v, u, Neighbor());
		}

		template <typename T>
		inline _Vec<T> operator / (const Neighbor<T> &v, const Neighbor<T> &u)
		{
			typedef std::divides<T> _Func;
			return binary_operator<T, Neighbor<T>, _Func>(v, u, _Func());
		}

		template <typename T>
		inline _Vec<T> pow(const Neighbor<T> &v, const Neighbor<T> &u)
		{
			typedef transform_functors::pow<T> _Func;
			return binary_operator<T, Neighbor<T>, _Func>(v, u, _Func());
		}

	}

	//-----------------------------------------------------
	// Heterogenous Neighbor-Vector arithmetic operators
	//-----------------------------------------------------
	namespace heterogeneous
	{
		// Binary operators
		template <typename T>
		inline _Vec<T> operator + (const _Vec<T> &v, const Neighbor<T> &nhbr)
		{
			typedef std::plus<T> _Func;
			return hetero_operator(v, nhbr, _Func());
		}

		template <typename T>
		inline _Vec<T> operator + (const Neighbor<T> &nhbr, const _Vec<T> &v)
		{
			typedef std::plus<T> _Func;
			return hetero_operator(v, nhbr, _Func());
		}

		template <typename T>
		inline _Vec<T> operator - (const _Vec<T> &left, const Neighbor<T> &right)
		{
			typedef std::minus<T> _Func;
			return hetero_operator(left, right, _Func());
		}

		template <typename T>
		inline _Vec<T> operator - (const Neighbor<T> &left, const _Vec<T> &right)
		{
			typedef std::minus<T> _Func;
			return hetero_operator(left, right, _Func());
		}

		template <typename T>
		inline _Vec<T> operator * (const _Vec<T> &v, const Neighbor<T> &u)
		{
			typedef std::multiplies<T> _Func;
			return hetero_operator(left, right, _Func());
		}

		template <typename T>
		inline _Vec<T> operator * (const Neighbor<T> &u, const _Vec<T> &v)
		{
			typedef std::multiplies<T> _Func;
			return hetero_operator(left, right, _Func());
		}

		template <typename T>
		inline _Vec<T> operator / (const _Vec<T> &v, const Neighbor<T> &u)
		{
			typedef std::divides<T> _Func;
			return hetero_operator(left, right, _Func());
		}

		template <typename T>
		inline _Vec<T> operator / (const Neighbor<T> &u, const _Vec<T> &v)
		{
			typedef std::divides<T> _Func;
			return hetero_operator(left, right, _Func());
		}
	}

	//--------------------------------
	// Scalar arithmetic operators
	//--------------------------------
	namespace scalar
	{
		// Binary operators
		template <typename T>
		inline Neighbor<T> operator + (const T &v, const Neighbor<T> &u)
		{
			return scalar_operator(v, u, std::plus<T>());
		}

		template <typename T>
		inline Neighbor<T> operator + (const Neighbor<T> &u, const T &v)
		{
			return scalar_operator(v, u, std::plus<T>());
		}

		template <typename T>
		inline Neighbor<T> operator - (const T &left, const Neighbor<T> &right)
		{
			return scalar_operator(left, right, std::minus<T>());
		}

		template <typename T>
		inline Neighbor<T> operator - (const Neighbor<T> &left, const T &right)
		{
			return scalar_operator(left, right, std::minus<T>());
		}

		template <typename T>
		inline Neighbor<T> operator * (const T &v, const Neighbor<T> &u)
		{
			return scalar_operator(v, u, std::multiplies<T>());
		}

		template <typename T>
		inline Neighbor<T> operator * (const Neighbor<T> &u, const T &v)
		{
			return scalar_operator(v, u, std::multiplies<T>());
		}

		template <typename T>
		inline Neighbor<T> operator / (const T &v, const Neighbor<T> &u)
		{
			return scalar_operator(v, u, std::divides<T>());
		}

		template <typename T>
		inline Neighbor<T> operator / (const Neighbor<T> &u, const T &v)
		{
			return scalar_operator(v, u, std::divides<T>());
		}
	}

	// Stream operator
	template <typename T>
	inline std::ostream & operator << (std::ostream &out, const Neighbor<T> &v)
	{
		std::string format = "(";

		for (auto &each : v)
			format += (std::to_string(*each) + ", ");

		// Erase last ", " from the string
		if (format.size() > 3)
			format.erase(format.end() - 2, format.end());

		format += ")";
		return out << format;
	}
}

/********** 0-nary operators **********/
//template <typename T>
//static Neighbor<T> pi()
//{
//	const T _pi = (T)3.141592653589793238462643383279502884;
//
//	Neighbor<T> out(v.size());
//
//	std::fill(out.begin(), out.end(), _pi);
//}
