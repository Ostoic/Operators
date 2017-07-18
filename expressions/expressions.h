#pragma once

#include <vap\config.h>
#include <vap\iterators\iterators.h>
#include <vap\detail\traits.h>
#include <vap\execution_policy.h>

// This is more descriptive than the usual static_cast code
#define CRTP_DOWNCAST(_D) static_cast<_D>(*this)

namespace vap		  {
namespace expressions {

// Represents any operator expression
template <class Derived_Class>
class Expression
{
public:
	// Save the derived class type
	using Derived = Derived_Class;

	using exec			 = typename expressions::expression_traits<Derived>::exec;
	using iterator		 = typename expressions::expression_traits<Derived>::iterator;
	using const_iterator = typename expressions::expression_traits<Derived>::const_iterator;
	using value_type	 = typename expressions::expression_traits<Derived>::value_type;

	Derived& derived()
	{ return *static_cast<Derived*>(this); }

	Derived const& derived() const
	{ return *static_cast<Derived const*>(this); }

	// Forward the iterator interface downwards until we have enough 
	// class data to define a proper iterator type
	iterator begin() { return CRTP_DOWNCAST(Derived&).begin();  }
	iterator end()	 { return CRTP_DOWNCAST(Derived&).end();   }

	const_iterator cbegin() const { return CRTP_DOWNCAST(const Derived&).cbegin(); }
	const_iterator cend()	const { return CRTP_DOWNCAST(const Derived&).cend(); }

	// Casts *this to underlying expression type using CRTP, then calls [] operator in
	// Binary/Unary/N-ary derived class
	value_type operator [] (std::size_t i) const { return CRTP_DOWNCAST(const Derived&)[i]; }
	std::size_t size()					   const { return CRTP_DOWNCAST(const Derived&).size(); }

	// Provides implicit (or C-style) cast to expression type
	operator Derived& ()			 { return CRTP_DOWNCAST(Derived&); }
	operator const Derived& () const { return CRTP_DOWNCAST(const Derived&); }
};

// Represents a binary expression to be executed under the given execution policy and operator
template <typename Left,
		  typename Right,
		  class	   Operator,
		  class	   Exec_Policy = absorption_policy,
		  bool	   Is_Operator = false>
class Binary :
	public Expression <Binary <Left, Right, Operator, Exec_Policy, Is_Operator>>
{
protected:
	const Left&  left;
	const Right& right;

	Operator apply;

public:
	Binary(const Left& lhs, const Right& rhs) : left(lhs), right(rhs) 
	{ 
		// Assert that both Left and Right types must be derived from Expression
		static_assert (
			vap::is_expression<Left>::value && vap::is_expression<Right>::value,
			"Binary: Left and Right types must be expressions");

		// Execution policies must coincide
		// Note that putting the static_assert here ensures the class type
		// is fully defined, so we can properly compare execution policies.
		static_assert (
			vap::compatible_execs<typename Left::exec, typename Right::exec>::value,
			"Binary: Incompatible execution policies selected");

		//if (right.size() > left.size()) left.update(right.size());
		assert(left.size() == right.size()); 
	}

	iterator begin() 
	{
		return make_transform_iterator(
					make_zip_iterator (
					make_tuple(left.begin(), right.begin())),
					apply);
	}

	iterator end() 
	{
		return make_transform_iterator(
					make_zip_iterator (
					make_tuple(left.end(), right.end())),
					apply);
	}

	const_iterator cbegin() const 
	{
		return make_transform_iterator(
					make_zip_iterator (
					make_tuple(left.cbegin(), right.cbegin())),
					apply);
	}

	const_iterator cend() const
	{
		return make_transform_iterator(
					make_zip_iterator (
					make_tuple(left.cend(), right.cend())),
					apply);
	}

	std::size_t size() const { return std::max(left.size(), right.size()); }
	value_type operator [] (std::size_t i) const 
	{ 
		using boost::make_tuple; 
		return apply(make_tuple(left[i], right[i]));
	}
};

// Represents a unary expression to be executed under the given execution policy and operator
template <typename Base,
		  class	   Operator,
		  class	   Exec_Policy = absorption_policy,
		  bool     Is_Operator = false>
class Unary :
	public Expression <Unary <Base, Operator, Exec_Policy, Is_Operator>>
{
protected:
	const Base& expression;

	Operator apply;

public:
	Unary(const Base& exp) : expression(exp)
	{
		// Assert that the Base type must be derived from Expression
		static_assert (
			vap::is_expression<Base>::value,
			"Unary: Base type must be an expression");
	}

	// Empty functor
	void update(const std::size_t&) {}

	iterator begin()
	{ return make_transform_iterator(expression.begin(), apply); }

	iterator end() 
	{ return make_transform_iterator(expression.end(), apply); }

	const_iterator cbegin() const
	{ return make_transform_iterator(expression.cbegin(), apply); }

	const_iterator cend() const
	{ return make_transform_iterator(expression.cend(), apply); }

	std::size_t size()					   const { return expression.size(); }
	value_type operator [] (std::size_t i) const { return apply(expression[i]); }
};


template <typename Type>
class Scalar :
	public Expression <Scalar <Type>>
{
protected:
	// This MUST be a by-value variable. Otherwise rvalues
	// will be invalidated when this class goes out of scope.
	value_type value;
	std::size_t m_size;

	void requirements() 
	{
		// Assert that the scalar type must be an arithmetic type
		static_assert (
			std::is_arithmetic<Type>::value,
			"Scalar: Scalar type must be an arithmetic type");
	}

public:
	Scalar(const Type& val) : value(val), m_size(0)
	{ requirements(); }

	template <typename E>
	Scalar(const Type& val, const std::size_t size) : value(val), m_size(size) 
	{ requirements(); }

	iterator begin() 
	{ return thrust::constant_iterator<value_type, std::size_t>(value, 0); }

	iterator end() 
	{ return thrust::constant_iterator<value_type, std::size_t>(value, m_size); }

	const_iterator cbegin() const
	{ return thrust::constant_iterator<value_type, std::size_t>(value, 0); }

	const_iterator cend()   const
	{ return thrust::constant_iterator<value_type, std::size_t>(value, m_size); }

	std::size_t size()					   const { return m_size; }
	value_type operator [] (std::size_t i) const { return value; }
};
	
} // end namespace expressions
} // end namespace vap