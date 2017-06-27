Vector Arithmetic in Parallel
==================================

This library provides the correct operator overloads for use with **thrust** vectors (or any random-access container) numerically in scientific calculations, that allows for **serial** or **parallel** execution policies. Expression templates are used to reduce the number of temporary copies that would be necessary otherwise. 

```c++
#include <vap\vector.h>+
#include <thrust\device_vector.h>

#include <iostream>
#include <vector>

// This example shows how to use vap::vector in numerical calculations, and compares the
// output of the library to using thrust's fancy iterators to evaluate the given expression.
// We will evaluate the sum x + y + z, where x = (3, ...), y = (-1, ...), and z = (-1, ...).

// Addition functor for use with transform_iterator
template <typename Result>
struct add
{
	enum {Arg1 = 0, Arg2 = 1};
	
	template <class Tuple>
	Result operator () (const Tuple& t)
	{
		return thrust::get<Arg1>(t) + thrust::get<Arg2>(t);
	}
};

int main()
{
	using T = double;
	using Vec = std::vector<T>;
	using VVec = vap::vector<T, vap::constructors::Thrust, // Uses thrust::copy as the ctor
                                thrust::device_vector<T>,  // Underlying container is a thrus::device_vector
                                vap::parallel_policy>;     // Executes in parallel
	// Include operator overloads                          
	using vap::operators;
  
	const std::size_t N = 4;
  
	thrust::host_vector<T> host(N);
  
	// Define a vap::vector expression with the given properties defined in VVec.
	VVec x(N), y(N), z(N);
  
	// Set the contents of the above vectors
	x.assign(N, 3); y.assign(N, -1); z.assign(N, -1);
  
	// Compute the result in parallel.
	VVec result = x + y + z;
  
	// Print result of sum
	std::cout << "Result = " << host[0] << std::endl;
	
	// The above expression is expanded compile-time to be equivalent to the following code.
	// First make the transform iterators for evaluating x + y.
	auto xy_begin = thrust::make_transform_iterator(
						thrust::make_zip_iterator(thrust::make_tuple(x.begin(), y.begin())), add<T>);
													
	auto xy_end   = thrust::make_transform_iterator(
						thrust::make_zip_iterator(thrust::make_tuple(x.end(), y.end())), add<T>);
	
	// Then zip the previous iterators into a tuple ((x+y).begin(), z.begin()) 
	// (.end, respectively) to achieve an evaluation equivalent to (x + y) + z.
	auto zip_begin = thrust::make_zip_iterator(thrust::make_tuple(xy_begin, z.begin())), add<T>);
	auto zip_end   = thrust::make_zip_iterator(thrust::make_tuple(xy_end,   z.end())),   add<T>);
										 
	// Apply the operators when dereferenced in thrust::copy.
	thrust::copy(thrust::make_transform_iterator(zip_begin, add<T>),
				 thrust::make_transform_iterator(zip_end,   add<T>),
				 result.begin());
  
	// Transfer result to host
	thrust::copy(result.begin(), result.end(), host.begin());
  
	// Print result of sum
	std::cout << "Result = " << host[0] << std::endl;
	return 0;
}
```
