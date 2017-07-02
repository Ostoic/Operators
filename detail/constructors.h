#include <algorithm>

#include <vap\config.h>

#ifdef VAP_USING_THRUST
#include <thrust\copy.h>
#include <thrust\execution_policy.h>
#endif

namespace vap		   {
namespace constructors {
// This is the default constructor for the vector class below
class Loop
{
protected:
	template <class C, class E>
	void ctor(C& c, const E& e)
	{
		auto sz = e.size();
		for (std::size_t i = 0; i < sz; ++i)
			c[i] = e[i];
	}

	template <class C, class E>
	void assignment(C &c, const E& e)
	{
		ctor(c, e);
	}
};

// std::copy ctor using expression iterators serially
class STL
{
protected:
	template <class Container, class Exp>
	void ctor(Container& c, const Exp& expression)
	{
		std::copy(expression.cbegin(), expression.cend(), c.begin());
	}

	template <class Container, class Exp>
	void assignment(Container &c, const Exp& e)
	{
		ctor(c, e);
	}
};

// thrust::copy ctor using expression iterators in parallel
class Thrust
{
protected:
	template <class Container, class Exp>
	void ctor(Container& c, const Exp& expression)
	{
#		ifdef VAP_USING_THRUST
			thrust::copy(thrust::device, expression.cbegin(), expression.cend(), c.begin());
#		endif
	}

	template <class Container, class Exp>
	void assignment(Container &c, const Exp& e)
	{ ctor(c, e); }
};

} // end namespace constructors
} // end namespace vap