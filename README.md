Vector Arithmetic in Parallel
==================================

This library provides the correct operator overloads for use with **thrust** vectors (or any random-access container) numerically in scientific calculations, that allows for **serial** or **parallel** execution policies. Expression templates are used to reduce the number of temporary copies that would be necessary otherwise. 

```c++
#include <vap\vector.h>+
#include <thrust\device_vector.h>

#include <iostream>
#include <vector>

int main()
{
  using T = double;
  using Vec = std::vector<T>;
  using VVec = vap::vector<T, vap::constructors::Thrust, // Uses thrust::copy as the ctor
                              thrust::device_vector<T>,  // Underlying container is a thrus::device_vector
                              vap::parallel_policy>;     // Executes in parallel
  const std::size_t N = 4;
  
  thrust::host_vector<T> host(N);
  
  // Define a vap::vector expression with the given properties defined in VVec.
  VVec x(N), y(N);
  
  // Set the contents of the above vectors
  x.assign(N, 3); y.assign(N, -1);
  
  // Compute the result in parallel.
  VVec result = x*y - x + y*x*x*y*x*y;
  
  // Transfer result to host
  thrust::copy(result.begin(), result.end(), host.begin());
  
  std::cout << "Result = " << host[0] << std::endl;
  return 0;
}
```
