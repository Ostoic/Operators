#pragma once

#include <cmath>

namespace etree  {

// Base execution policy used for selecting different execution systems
template <class Derived>
struct execution_policy {};

template <typename Return>
struct serial_policy : execution_policy<serial_policy<Return>>
{
	using result_type = Return;

	// Binary operator functors for the standard serial policy
	struct sum			{ template <typename Left, typename Right> result_type operator () (const Left& left, const Right& right) const { return left + right; } };

	template <typename Left, typename Right>
	struct difference	{ result_type operator () (const Left& left, const Right& right)  { return left - right; } };

	template <typename Left, typename Right>
	struct product		{ result_type operator () (const Left& left, const Right& right)  { return left * right; } };

	template <typename Left, typename Right>
	struct quotient		{ result_type operator () (const Left& left, const Right& right)  { return left / right; } };

	template <typename Left, typename Right>
	struct power		{ result_type operator () (const Left& left, const Right& right)  { return pow(left, right); } };

	// Unary operator functors for the standard serial policy
	template <typename T>
	static inline result_type negate(const T& val) { return -val; }

	template <typename T>
	static inline result_type log(const T& val) { using std::log; return log(val); }

	template <typename T>
	static inline result_type sin(const T& val) { using std::sin; return sin(val); }

	template <typename T>
	static inline result_type cos(const T& val) { using std::cos; return cos(val); }

	template <typename T>
	static inline result_type tan(const T& val) { using std::tan; return tan(val); }
};

} // end namespace etree
