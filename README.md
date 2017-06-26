Vap: Vector Arithmetic in Parallel
==================================

This library provides the correct operator overloads to use **thrust** vectors (or any random-access container) numerically in scientific 
calculations, that allows for **serial** or **parallel** execution policies. Expression templates are used to reduce the number of temporary copies that would be necessary otherwise. 

```c++
#include <vap\vector.h>
#include <iostream>
#include <vector>

int main()
{
  using T = double;
  using Vec = std::vector<T>;
  using VVec = vap::vector<T>;
  
  const std::size_t N = 4;
  
  Vec x(N), y(N);
  x.assign(N, 3); y.assign(N, -1);
  
  VVec sum = x + y;
  
  std::cout << "Sum = " << sum[0] << std::endl;
  return 0;
}
```
