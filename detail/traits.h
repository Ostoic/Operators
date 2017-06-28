#pragma once

#include <type_traits>

#include <vap\iterators\iterators.h>
#include <vap\execution_policy.h>

namespace vap		  {
namespace expressions {

// Forward declare all expression-derived classes.
template <class Derived>
class Expression;

template <typename Left, typename Right, class Operator, class Exec>
class Binary;

template <typename Exp, class Operator, class Exec>
class Unary;

template <typename Type>
class Scalar;

template <typename T, typename ConstructPolicy, typename Container, typename Exec>
class vector;

} // end namespace expressions

namespace detail {

// General type is not an expression
template <typename T>
struct is_expression : std::false_type{};

// Specialize expression and expression-derived types to hold true
template <class D>
struct is_expression <expressions::Expression<D>>			: std::true_type{};

template <class L, class R, class O, class P>
struct is_expression <expressions::Binary<L, R, O, P>>		: std::true_type{};

template <typename E, class O, class P>
struct is_expression <expressions::Unary<E, O, P>>			: std::true_type{};

template <class T, class Ctor, class C, class P>
struct is_expression <expressions::vector<T, Ctor, C, P>>	: std::true_type{};

// For ensuring the type will have class-like properties
template <typename T>
using scalarize = std::conditional<std::is_arithmetic<T>::value, expressions::Scalar<T>, T>;

template <typename T>
using scalarize_t = scalarize<T>::type;

template <typename T1, typename T2>
using get_exec = std::conditional<is_expression<T1>::value, typename T1::exec,		// If T1 is an expression, use its execution policy
					std::conditional_t<is_expression<T2>::value, typename T2::exec, // Else if T2 is an expression, use its execution policy
									   inherit_execution_policy>>;					// Otherwise inherit the surrounding execution policy

template <typename T1, typename T2>
using get_exec_t = get_exec<T1, T2>::type;

} // end namespace detail

namespace expressions {

template <typename T>
struct expression_traits;

// Defines expression_traits for derived specialization Binary
template <typename L, typename R, class Op, class Exec>
struct expression_traits<vap::expressions::Binary<L, R, Op, Exec>>
{
	using exec				   = Exec;
	using value_type		   = typename L::value_type;

	using left_iterator		   = typename L::iterator;
	using left_const_iterator  = typename L::const_iterator;

	using right_iterator	   = typename R::iterator;
	using right_const_iterator = typename R::const_iterator;

	// Retrieve binary_iterator specialization type for the given execution policy
	using iterator	= typename iterators
		  ::binary_iterator<exec>
		  ::template type<Op, left_iterator, right_iterator>; 
	
	// Retrieve const binary_iterator specialization type for the given execution policy
	using const_iterator = typename iterators
		  ::binary_iterator<exec>
		  ::template type<Op, left_const_iterator, right_const_iterator>; 
};

// Defines expression_traits for derived specialization Unary
template <typename T, class Op, class Exec>
struct expression_traits<vap::expressions::Unary<T, Op, Exec>>
{
	using exec				  = Exec;
	using value_type		  = typename T::value_type;
	using base_iterator		  = typename T::iterator;
	using const_base_iterator = typename T::const_iterator;
	
	// Retrieve unary_iterator specialization type for the given execution policy
	using iterator = typename iterators
		  ::unary_iterator<exec>
		  ::template type<Op, base_iterator>;
	
	// Retrieve const unary_iterator specialization type for the given execution policy
	using const_iterator = typename iterators
		  ::unary_iterator<exec>
		  ::template type<Op, const_base_iterator>;
};

 //Defines expression_traits for derived specialization Unary
template <typename T>
struct expression_traits<vap::expressions::Scalar<T>>
{
	using exec				  = vap::inherit_execution_policy;
	using value_type		  = T;
	
	// Retrieve unary_iterator specialization type for the given execution policy
	using iterator = typename iterators
		  ::scalar_iterator<exec>
		  ::template type<T>;
	
	// Retrieve const unary_iterator specialization type for the given execution policy
	using const_iterator = typename iterators
		  ::scalar_iterator<exec>
		  ::template type<T>;
};

template <typename T, class Ctor, typename C, class Exec>
struct expression_traits<vap::expressions::vector<T, Ctor, C, Exec>>
{
	using value_type	 = T;
	using exec			 = Exec;
	using iterator		 = typename C::iterator;
	using const_iterator = typename C::const_iterator;
};

} // end namespace expressions

using detail::is_expression;

using detail::scalarize;
using detail::scalarize_t;

using detail::get_exec;
using detail::get_exec_t;

} // end namespace vap