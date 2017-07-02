#pragma once

#include <type_traits>

#include <vap\config.h>
#include <vap\execution_policy.h>

#include <vap\iterators\iterators.h>

#include <vap\detail\functional.h>
#include <vap\expressions\expressions.h>

namespace vap		{
namespace operators {

	/*********************************/
	/** Binary Expression Operators **/
	/*********************************/
	// Sum: TEMPLATE PARAMETERS
	template <typename Left, 
			  typename Right	   = Left,
			  typename Functor     = vap::apply <vap::sum<typename Left::value_type>>,
			  typename Exec_Policy = absorption_policy>

	// Sum: CLASS DEFINITION
	struct Sum : public 
		expressions::Binary <Left, Right, Functor, Exec_Policy, true>
	{ Sum(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {} };

	// Difference: TEMPLATE PARAMETERS
	template <typename Left,
			  typename Right	   = Left,
			  typename Functor     = vap::apply <vap::difference <typename Left::value_type>>,
			  typename Exec_Policy = absorption_policy>

	// Difference: CLASS DEFINITION
	struct Difference : public 
		expressions::Binary <Left, Right, Functor, Exec_Policy, true>
	{ Difference(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {} };

	// Product: TEMPLATE PARAMETERS
	template <typename Left,
			  typename Right	   = Left,
			  typename Functor     = vap::apply <vap::product <typename Left::value_type>>,
			  typename Exec_Policy = absorption_policy>

	// Product: CLASS DEFINITION
	struct Product : public 
		expressions::Binary <Left, Right, Functor, Exec_Policy, true>
	{ Product(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {} };

	// Quotient: TEMPLATE PARAMETERS
	template <typename Left,
			  typename Right	   = Left,
			  typename Functor	   = vap::apply <vap::quotient <typename Left::value_type>>,
			  typename Exec_Policy = absorption_policy>

	// Quotient: CLASS DEFINITION
	struct Quotient : public 
		expressions::Binary <Left, Right, Functor, Exec_Policy, true>
	{ Quotient(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {} };

	// Power: TEMPLATE PARAMETERS
	template <typename Left,
			  typename Right	   = Left,
			  typename Functor	   =  vap::apply <vap::power<typename Left::value_type>>,
			  typename Exec_Policy = absorption_policy>

	// Power: CLASS DEFINITION
	struct Power : public 
		expressions::Binary <Left, Right, Functor, Exec_Policy, true>
	{ Power(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {} };

	/********************************/
	/** Unary Expression Operators **/
	/********************************/
	// Sin: TEMPLATE PARAMETERS
	template <typename Type,
			  typename Functor	   = vap::sin <typename Type::value_type>,
			  typename Exec_Policy = absorption_policy>

	// Sin: CLASS DEFINITION
	struct Sin : public 
		expressions::Unary <Type, Functor, Exec_Policy, true>
	{ Sin(const Type& value) : Unary(value) {} };
	
	// Cos TEMPLATE PARAMETERS
	template <typename Type,
			  typename Functor	   = vap::cos <typename Type::value_type>,
			  typename Exec_Policy = absorption_policy>

	// Cos: CLASS DEFINITION
	struct Cos : public 
		expressions::Unary <Type, Functor, Exec_Policy, true>
	{ Cos(const Type& value) : Unary(value) {} };
	
	// Tan: TEMPLATE PARAMETERS
	template <typename Type,
			  typename Functor	   = vap::tan <typename Type::value_type>,
			  typename Exec_Policy = absorption_policy>

	// Tan: CLASS DEFINITION
	struct Tan : public 
		expressions::Unary <Type, Functor, Exec_Policy, true>
	{ Tan(const Type& value) : Unary(value) {} };
	
	// Log: TEMPLATE PARAMETERS
	template <typename Type,
			  typename Functor	   = vap::log <typename Type::value_type>,
			  typename Exec_Policy = absorption_policy>

	// Log: CLASS DEFINITION
	struct Log : public 
		expressions::Unary <Type, Functor, Exec_Policy, true>
	{ Log(const Type& value) : Unary(value) {} };
	
	// Negate: TEMPLATE PARAMETERS
	template <typename Type,
			  typename Functor	   = vap::negate <typename Type::value_type>,
			  typename Exec_Policy = absorption_policy>

	// Negate: CLASS DEFINITION
	struct Negate : public 
		expressions::Unary <Type, Functor, Exec_Policy, true>
	{ Negate(const Type& value) : Unary(value) {} };

	/*******************************/
	/** Binary Operator Overloads **/
	/*******************************/
	template <typename Left, typename Right,
			  typename Return = Sum<vap::vectorize_t<Left>, 
									vap::vectorize_t<Right>>>
	const Return operator + (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Return = Difference<vap::vectorize_t<Left>, 
										   vap::vectorize_t<Right>>>
	const Return operator - (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Return = Product<vap::vectorize_t<Left>, 
										vap::vectorize_t<Right>>>
	const Return operator * (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Return = Quotient<vap::vectorize_t<Left>, 
										 vap::vectorize_t<Right>>>
	const Return operator / (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Return = Power<vap::vectorize_t<Left>, 
									  vap::vectorize_t<Right>>>
	const Return operator ^ (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }

	/******************************/
	/** Unary Operator Overloads **/
	/******************************/
	template <typename Type,
			  typename Return = Negate<vap::vectorize_t<Type>>>
	const Return operator - (const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = Sin<vap::vectorize_t<Type>>>
	const Return sin(const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = Cos<vap::vectorize_t<Type>>>
	const Return cos(const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = Tan<vap::vectorize_t<Type>>>
	const Return tan(const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = Log<vap::vectorize_t<Type>>>
	const Return log(const Type& t)
	{ return Return(t); }

} // end namespace operators
} // end namespace vap
