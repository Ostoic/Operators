#pragma once

#include <vap\config.h>

namespace vap  {

/*-------------------------*/
/* Weak Execution Policies */
/*-------------------------*/
// Base execution policy tag used for selecting different execution systems
struct execution_policy {};

// Indicates that the expression will absorb the surrounding execution policies
struct absorption_policy : execution_policy{};

/*==========================*/
/* Strong Execution Policies*/
/*==========================*/
// Indicates that operations will be performed serially
struct serial_execution : execution_policy {};

// Indicates that operations will be performed in parallel 
struct parallel_execution: execution_policy {};

} // end namespace vap
