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
			  typename Execution_Policy = serial_execution,
			  typename = std::enable_if_t<vap::is_expression<Left>::value &&
										  vap::is_expression<Right>::value>>
	struct Sum :
		public expressions::Binary <Left, Right, vap::sum <typename Left::value_type>, Execution_Policy>
	{ 
		Sum(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {} 
	};

	template <typename Left,
			  typename Right = Left,
			  typename Execution_Policy = serial_execution,
			  typename = std::enable_if_t<vap::is_expression<Left>::value &&
										  vap::is_expression<Right>::value>>
	struct Difference :
		public expressions::Binary <Left, Right, vap::difference <typename Left::value_type>, Execution_Policy>
	{
		Difference(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left,
			  typename Right = Left,
			  typename Execution_Policy = serial_execution,
			  typename = std::enable_if_t<vap::is_expression<Left>::value &&
										  vap::is_expression<Right>::value>>
	struct Product :
		public expressions::Binary <Left, Right, vap::product <typename Left::value_type>, Execution_Policy>
	{
		Product(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left,
			  typename Right = Left,
			  typename Execution_Policy = serial_execution,
			  typename = std::enable_if_t<vap::is_expression<Left>::value &&
										  vap::is_expression<Right>::value>>
	struct Quotient :
		public expressions::Binary <Left, Right, vap::quotient <typename Left::value_type>, Execution_Policy>
	{
		Quotient(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	template <typename Left,
			  typename Right = Left,
			  typename Execution_Policy = serial_execution,
			  typename = std::enable_if_t<vap::is_expression<Left>::value &&
										  vap::is_expression<Right>::value>>
	struct Power :
		public expressions::Binary <Left, Right, vap::power <typename Left::value_type>, Execution_Policy>
	{
		Power(const Left& lhs, const Right& rhs) : Binary(lhs, rhs) {}
	};

	/********************************/
	/** Unary Expression Operators **/
	/********************************/

	template <typename Type, 
			  typename Execution_Policy = serial_execution,
			  typename = std::enable_if_t<vap::is_expression<Type>::value >>
	struct Sin :
		public expressions::Unary <Type, vap::sin <typename Type::value_type>, Execution_Policy>
	{ Sin(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_execution,
			  typename = std::enable_if_t<vap::is_expression<Type>::value >>
	struct Cos :
		public expressions::Unary <Type, vap::cos <typename Type::value_type>, Execution_Policy>
	{ Cos(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_execution,
			  typename = std::enable_if_t<vap::is_expression<Type>::value >>
	struct Tan :
		public expressions::Unary <Type, vap::tan <typename Type::value_type>, Execution_Policy>
	{ Tan(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_execution,
			  typename = std::enable_if_t<vap::is_expression<Type>::value >>
	struct Log :
		public expressions::Unary <Type, vap::log <typename Type::value_type>, Execution_Policy>
	{ Log(const Type& value) : Unary(value) {} };
	
	template <typename Type, 
			  typename Execution_Policy = serial_execution,
			  typename = std::enable_if_t<vap::is_expression<Type>::value>>
	struct Negate :
		public expressions::Unary <Type, vap::negate <typename Type::value_type>, Execution_Policy>
	{ Negate(const Type& value) : Unary(value) {} };

	/*******************************/
	/** Binary Operator Overloads **/
	/*******************************/
	template <typename Left, typename Right, 
			  typename Return = 
			  const Sum<vap::scalarize_t<Left>,
						vap::scalarize_t<Right>,
						vap::get_exec_t<Left, Right>>>
	Return operator + (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Return = 
			  const Difference<vap::scalarize_t<Left>,
							   vap::scalarize_t<Right>,
							   vap::get_exec_t<Left, Right>>>
	Return operator - (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Return = 
			  const Product<vap::scalarize_t<Left>,
							vap::scalarize_t<Right>,
							vap::get_exec_t<Left, Right>>>
	Return operator * (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Return = 
			  const Quotient<vap::scalarize_t<Left>,
							 vap::scalarize_t<Right>,
							 vap::get_exec_t<Left, Right>>>
	Return operator / (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }
	
	template <typename Left, typename Right, 
			  typename Return = 
			  const Power<vap::scalarize_t<Left>,
						  vap::scalarize_t<Right>,
						  vap::get_exec_t<Left, Right>>>
	Return operator ^ (const Left& lhs, const Right& rhs)
	{ return Return(lhs, rhs); }

	/******************************/
	/** Unary Operator Overloads **/
	/******************************/
	template <typename Type,
			  typename Return = 
			  const Negate<vap::scalarize_t<Type>,
						   vap::scalarize_t<Type>::exec>>
	Return operator - (const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = 
			  const Sin<vap::scalarize_t<Type>,
						vap::scalarize_t<Type>::exec>>
	Return sin(const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = 
			  const Cos<vap::scalarize_t<Type>,
						vap::scalarize_t<Type>::exec>>
	Return cos(const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = 
			  const Tan<vap::scalarize_t<Type>,
						vap::scalarize_t<Type>::exec>>
	Return tan(const Type& t)
	{ return Return(t); }

	template <typename Type,
			  typename Return = 
			  const Log<vap::scalarize_t<Type>,
						vap::scalarize_t<Type>::exec>>
	Return log(const Type& t)
	{ return Return(t); }

} // end namespace operators
} // end namespace vap
