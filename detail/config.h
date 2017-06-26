#pragma once

#ifdef THRUST_DEVICE_SYSTEM
#	ifdef __host__
#		ifdef __device__
#			define ETREE_USING_THRUST
#		endif
#	endif
#endif

// Provide __host__ __device__ preprocessor directives when available
#ifdef ETREE_USING_THRUST
#	define ANY_SYSTEM __host__ __device__ 
#else
#	define ANY_SYSTEM 
#endif

#ifdef ETREE_USING_THRUST
#	define IF_USING_THRUST(code) code
#else
#	define IF_USING_THRUST(code)  
#endif

#ifdef ETREE_USING_THRUST
#	define NOT_USING_THRUST(code)
#else
#	define NOT_USING_THRUST(code) code
#endif

//#ifdef ETREE_USING_THRUST
//#	define THRUST_DEVICE __device__
//#else
//#	define THRUST_DEVICE() 
//#endif
