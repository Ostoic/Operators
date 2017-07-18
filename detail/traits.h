#pragma once

#include <type_traits>

#include <vap\iterators\iterators.h>
#include <vap\execution_policy.h>

namespace vap		  {
namespace expressions {

// Forward declare all expression-derived classes.
template <class>
class Expression;

template <typename, typename, class, class, bool>
class Binary;

template <typename, class, class, bool>
class Unary;

template <typename>
class Scalar;

template <typename, typename, typename, typename>
class vector;

} // end namespace expressions

namespace detail {


// Specialize expression and expression-derived types to hold true
/*===============================*/
/* Direct Expression derivations */
/*===============================*/
//template <class D>
//struct is_expression <expressions::Expression<D>> : std::true_type{};

//template <typename Class>
//struct is_expression : std::false_type{};

// Convert the given type to a Scalar<T>, if it is not an arithmetic type.
template <typename T>
using vectorize = std::conditional<std::is_arithmetic<T>::value, 
								   expressions::Scalar<T>,		 // If T is an arithmetic type, wrap it in a Scalar<T>
								   T>;					         // Otherwise, T is a possible expression candidate
    
template <typename T>
using vectorize_t = typename vectorize<T>::type;

// General case: To is not a number type
// Behaviour: The wrapper acts as the identity function
template <typename To>
class wrap_number
{
public:
	template <typename Other>
	wrap_number(const To& lhs, const Other& rhs) : wrapper(lhs) {}

	operator const To& () const
	{ return this->wrapper; }

private:
	const To& wrapper;
};

// Scalar<T> case: To is a number type
// Behaviour: The number lhs is wrapped in Scalar<T> with the correct dimensions
template <typename T>
class wrap_number <expressions::Scalar<T>>
{
public:
	using To = expressions::Scalar<T>;

	template <typename Other>
	wrap_number(const T& lhs, const Other& rhs) : wrapper(lhs, rhs.size()) {}

	operator To () const
	{ return this->wrapper; }

private:
	To wrapper;
};

template <typename Check>
struct is_exec : std::is_base_of<execution_policy, Check>{};
    
template <typename Exec>
struct is_strong_exec 
{
    static const bool value = std::is_same<Exec, parallel_execution>::value ||
			                  std::is_same<Exec, serial_execution>::value;
};

template <typename Exec>
struct is_weak_exec
{
	static const bool value = std::is_same<Exec, execution_policy>::value ||
							  std::is_same<Exec, absorption_policy>::value;
};

// MSVC++ variadiac expansion bug work-around
template <typename T>
struct exec_extractor
{ 
	using type = typename T::exec;
};

// Choose the strongest execution policy from the template parameters
template <typename T1, typename... Ts>
struct get_strongest_exec
{
	using type = std::conditional_t<
					is_strong_exec<T1>::value,	                // Determine if T1 is strong or not
					T1,											// If T1 is strong, return it
					typename get_strongest_exec<Ts...>::type>;	// Otherwise, recurse down the list until we've found the strongest exec
};

// Edge case of get_strongest_exec
template <typename T1>
struct get_strongest_exec<T1>
{
	using type = std::conditional_t<
					is_strong_exec<T1>::value,	// Check if T1 is a strong execution policy
					T1,							// If T1 is strong, return T1
					absorption_policy>;			// Otherwise, absorb surrounding execution policies
};

template <typename... Ts>
struct get_exec;

template <typename T>
struct get_exec<T> : get_strongest_exec<typename T::exec>{};

// *** MSVC COMPILER BUG: https://connect.microsoft.com/VisualStudio/feedback/details/810018/variadic-template-parameter-pack-expansion-error
// ***					  https://connect.microsoft.com/VisualStudio/feedback/details/862959/c-parameter-pack-expansion-fails
// *** We can't expand parameter pack with a subtype specified. Although the issue has been fixed in a later MSVC, we must work around this bug.
// *** The current work-around uses exec_extractor

//template <typename T, typename ... Ts>
//struct get_exec<T, Ts...> : get_strongest_exec<typename T::exec,
											   //typename Ts::exec...> {};

template <typename T, typename ... Ts>
struct get_exec<T, Ts...> : get_strongest_exec<typename exec_extractor<T>::type,
											   typename exec_extractor<Ts>::type...>{};

// If either of the execution policies inherit, then return true.
// Otherwise compare execution policies to be equal.
template <typename Exec1, typename Exec2>
struct compatible_execs
{
	static const bool value = std::is_same<Exec1, absorption_policy>::value ||
							  std::is_same<Exec2, absorption_policy>::value ||
							  std::is_same<Exec1, Exec2>::value;
};

template <typename Type, Type x, Type y>
struct max
{
	const static Type value = x > y ? x : y;
};

template <typename Type, Type x, Type y>
struct min
{
	const static Type value = x < y ? x : y;
};

// Forward declare is_expression for use in expression_traits
template <class Check>
struct is_expression;

} // end namespace detail

namespace expressions {

template <typename T>
struct expression_traits
{
	static const bool is_operator = false;
};

// Defines expression_traits for derived specialization Binary
template <typename L, typename R, class Op, class Exec, bool Is_Operator>
class expression_traits<vap::expressions::Binary<L, R, Op, Exec, Is_Operator>>
{
public:
	static_assert(
		vap::detail::is_expression<L>::value && vap::detail::is_expression<R>::value,
		"Binary Traits: Left and Right types must be expressions");

	static_assert(
		vap::detail::is_exec<Exec>::value,
		"Binary Traits: Invalid execution policy");

	static const bool is_operator = Is_Operator;

private:
	using left_iterator		   = typename L::iterator;
	using left_const_iterator  = typename L::const_iterator;

	using right_iterator	   = typename R::iterator;
	using right_const_iterator = typename R::const_iterator;

public:
	using exec		 = typename detail::get_strongest_exec<Exec, typename L::exec, typename R::exec>::type;
	using value_type = typename L::value_type;

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
template <typename T, class Op, class Exec, bool IsOp>
class expression_traits<vap::expressions::Unary<T, Op, Exec, IsOp>>
{
public:
	static_assert(
		vap::detail::is_expression<T>::value,
		"Unary Traits: Base type must be an expression");

	static_assert(
		vap::detail::is_exec<Exec>::value,
		"Unary Traits: Invalid execution policy");

	static const bool is_operator = IsOp;

private:
	using base_iterator		  = typename T::iterator;
	using const_base_iterator = typename T::const_iterator;

public:
	using exec				  = typename detail::get_strongest_exec<Exec, typename T::exec>::type;
	using value_type		  = typename T::value_type;

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
	static_assert(
		std::is_arithmetic<T>::value,
		"Scalar Traits: Scalar type must be an arithmetic type");

	static const bool is_operator = false;

	using exec = vap::absorption_policy;
	using value_type = T;

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
	static_assert(
		std::is_arithmetic<T>::value,
		"Vector Traits: Scalar type must be an arithmetic type");

	static_assert(
		vap::detail::is_exec<Exec>::value,
		"Vector Traits: Invalid execution policy");

	static const bool is_operator = false;

	using value_type	 = T;
	using exec			 = Exec;
	using iterator		 = typename C::iterator;
	using const_iterator = typename C::const_iterator;
};

} // end namespace expressions

namespace detail
{
	template <class Check>
	struct is_expression
	{
		static const bool value = std::is_base_of<expressions::Expression<Check::Derived>, Check>::value ||
												  expressions::expression_traits<Check>::is_operator;
	};

	template <typename T, template <typename> class Check>
	using is_scalar_expression = std::is_same<vap::expressions::Scalar<T>, Check<T>>;
}

using detail::is_expression;
using detail::is_scalar_expression;

using detail::vectorize;
using detail::vectorize_t;

using detail::wrap_number;
    
using detail::get_exec;
using detail::get_strongest_exec;

using detail::compatible_execs;

} // end namespace vap
