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
template <class Derived>
class Expression
{
public:
	using iterator		 = typename expressions::expression_traits<Derived>::iterator;
	using const_iterator = typename expressions::expression_traits<Derived>::const_iterator;
	using value_type	 = typename expressions::expression_traits<Derived>::value_type;

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
		  class	   Exec_Policy>
class Binary :
	public Expression <Binary <Left, Right, Operator, Exec_Policy>>
{
protected:
	const Left&  left;
	const Right& right;

	Operator apply;

public:
	// Retrieve traits 
	using exec			 = typename expressions::expression_traits<Binary>::exec;
	using value_type	 = typename expressions::expression_traits<Binary>::value_type;

	using iterator		 = typename expressions::expression_traits<Binary>::iterator;
	using const_iterator = typename expressions::expression_traits<Binary>::const_iterator;

	Binary(const Left& lhs, const Right& rhs) : left(lhs), right(rhs) 
	{ 
		// Execution policies must coincide
		// Note that putting the static_assert here ensures the class type
		// is fully defined, so we can properly compare execution policies.
		static_assert(
			std::is_same<typename Left::exec, typename Right::exec>::value,
			"Difference: Incompatible execution policies selected");

		//assert(left.size() == right.size()); 
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

	std::size_t size()					   const { return left.size(); }
	value_type operator [] (std::size_t i) const 
	{ 
		using boost::make_tuple; 
		return apply(make_tuple(left[i], right[i]));
	}
};

// Represents a unary expression to be executed under the given execution policy and operator
template <typename Exp,
		  class	   Operator,
		  class	   Exec_Policy>
class Unary :
	public Expression <Unary <Exp, Operator, Exec_Policy>>
{
protected:
	const Exp& expression;

	Operator apply;

public:
	// Retrieve traits 
	using exec			 = typename expressions::expression_traits<Unary>::exec;
	using value_type	 = typename expressions::expression_traits<Unary>::value_type;

	using iterator		 = typename expressions::expression_traits<Unary>::iterator;
	using const_iterator = typename expressions::expression_traits<Unary>::const_iterator;

	Unary(const Exp& exp) : expression(exp) {}

	iterator begin() 
	{
		return make_transform_iterator(expression.begin(), apply);
	}

	iterator end() 
	{
		return make_transform_iterator(expression.end(), apply);
	}

	const_iterator cbegin() const
	{
		return make_transform_iterator(expression.cbegin(), apply);
	}

	const_iterator cend() const
	{
		return make_transform_iterator(expression.cend(), apply);
	}

	std::size_t size()					   const { return expression.size(); }
	value_type operator [] (std::size_t i) const { return apply(expression[i]); }
};


template <typename Type>
class Scalar :
	public Expression <Scalar <Type>>
{
protected:
	const Type& value;

public:
	// Retrieve traits 
	using exec			 = typename expressions::expression_traits<Scalar>::exec;
	using value_type	 = typename expressions::expression_traits<Scalar>::value_type;

	using iterator		 = typename expressions::expression_traits<Scalar>::iterator;
	using const_iterator = typename expressions::expression_traits<Scalar>::const_iterator;

	Scalar(const Type& val) : value(val) {}

	iterator begin() 
	{
		return vap::make_constant_iterator(value);
	}

	iterator end() 
	{
		return vap::make_constant_iterator(value);
	}

	const_iterator cbegin() const
	{
		return vap::make_constant_iterator(value);
	}

	const_iterator cend() const
	{
		return vap::make_constant_iterator(value);
	}

	//std::size_t size()					   const { return expression.size(); }
	value_type operator [] (std::size_t i) const { return value; }
};
	
} // end namespace expressions
} // end namespace vap