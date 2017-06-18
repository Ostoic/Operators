#pragma once

#include "vector.h"
#include "expressions.h"

namespace etree		  {

template <typename T>
struct is_expression								: std::false_type {};

template <class D>
struct is_expression <expressions::Expression<D>>	: std::true_type  {};

template <class L, class R, class O>
struct is_expression <expressions::Binary<L, R, O>>	: std::true_type{};

template <typename E, class O>
struct is_expression <expressions::Unary<E, O>>		: std::true_type{};

template <class T, class P, class C>
struct is_expression <vector<T, P, C>>				: std::true_type  {};

} // end namespace etree