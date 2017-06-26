#pragma once

#include "detail\config.h"

namespace vap  {

// Base execution policy used for selecting different execution systems
struct execution_policy {};

struct serial_policy : execution_policy {};

struct parallel_policy : execution_policy {};

} // end namespace vap
