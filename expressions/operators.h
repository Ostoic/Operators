#pragma once

#include <type_traits>

#include <vap\config.h>
#include <vap\execution_policy.h>

#include <vap\iterators\iterators.h>

#include <vap\detail\functional.h>
#include <vap\expressions\expressions.h>

namespace vap		{
namespace operators {

	/********************************/
	/** Binary Expression Operators**/
	/********************************/
	template <typename Left, 
			  typename Right = Left,
			  typename Exec_Policy = vap::get_exec<Left, Right >>
	struct Sum :
		public expressions::Binary <Left, Right, vap::sum<typename Left::value_type>, Exec_Policy>
	{ Sum(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {} };

	template <typename Left,
			  typename Right = Left,
			  typename Exec_Policy = vap::get_exec<Left, Right >>
	struct Difference :
		public expressions::Binary <Left, Right, vap::difference <typename Left::value_type>, Exec_Policy>
	{
		Difference(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left,
			  typename Right = Left,
			  typename Exec_Policy = vap::get_exec<Left, Right >>
	struct Product :
		public expressions::Binary <Left, Right, vap::product <typename Left::value_type>, Exec_Policy>
	{
		Product(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left,
			  typename Right = Left,
			  typename Exec_Policy = vap::get_exec<Left, Right >>
	struct Quotient :
		public expressions::Binary <Left, Right, vap::quotient <typename Left::value_type>, Exec_Policy>
	{
		Quotient(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left,
			  typename Right = Left,
			  typename Exec_Policy = vap::get_exec<Left, Right>>
	struct Power :
		public expressions::Binary <Left, Right, vap::power <typename Left::value_type>, Exec_Policy>
	{
		Power(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	/********************************/
	/** Unary Expression Operators **/
	/********************************/

	template <typename Type, 
			  typename Exec_Policy = vap::get_exec<Type>>
	struct Sin :
		public expressions::Unary <Type, vap::sin <typename Type::value_type>, Exec_Policy>
	{ Sin(const Type& value) : Unary(value) {} };
	
	template <typename Type,
			  typename Exec_Policy = vap::get_exec<Type>>
	struct Cos :
		public expressions::Unary <Type, vap::cos <typename Type::value_type>, Exec_Policy>
	{ Cos(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Exec_Policy = vap::get_exec<Type>>
	struct Tan :
		public expressions::Unary <Type, vap::tan <typename Type::value_type>, Exec_Policy>
	{ Tan(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Exec_Policy = vap::get_exec<Type>>
	struct Log :
		public expressions::Unary <Type, vap::log <typename Type::value_type>, Exec_Policy>
	{ Log(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Exec_Policy = vap::get_exec<Type>>
	struct Negate :
		public expressions::Unary <Type, vap::negate <typename Type::value_type>, Exec_Policy>
	{ Negate(const Type& value) : Unary(value) {} };

	/*******************************/
	/** Binary Operator Overloads **/
	/*******************************/
	template <typename Left, typename Right,
			  typename Left_Exp  = vap::scalarize<Left>,
			  typename Right_Exp = vap::scalarize<Right>,
			  typename Return    = Sum<Left_Exp, Right_Exp>>
	const Return operator + (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Left_Exp  = vap::scalarize<Left>,
			  typename Right_Exp = vap::scalarize<Right>,
			  typename Return    = Difference<Left_Exp, Right_Exp>>
	const Return operator - (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Left_Exp  = vap::scalarize<Left>,
			  typename Right_Exp = vap::scalarize<Right>,
			  typename Return    = Product<Left_Exp, Right_Exp>>
	const Return operator * (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Left_Exp  = vap::scalarize<Left>,
			  typename Right_Exp = vap::scalarize<Right>,
			  typename Return    = Quotient<Left_Exp, Right_Exp>>
	const Return operator / (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Left_Exp  = vap::scalarize<Left>,
			  typename Right_Exp = vap::scalarize<Right>,
			  typename Return    = Power<Left_Exp, Right_Exp>>
	const Return operator ^ (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }

	/******************************/
	/** Unary Operator Overloads **/
	/******************************/
	template <typename Type,
			  typename Return = Negate<vap::scalarize<Type>>>
	const Return operator - (const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = Sin<vap::scalarize<Type>>>
	const Return sin(const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = Cos<vap::scalarize<Type>>>
	const Return cos(const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = Tan<vap::scalarize<Type>>>
	const Return tan(const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = Log<vap::scalarize<Type>>>
	const Return log(const Type& t)
	{ return Return(t); }

} // end namespace operators
} // end namespace vap
