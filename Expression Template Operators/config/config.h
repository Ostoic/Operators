#pragma once

#ifdef __CUDA_ARCH__ 
#	ifdef THRUST_DEVICE_SYSTEM
#		ifdef __host__
#			ifdef __device__
#				ifdef THRUST_DEVICE_SYSTEM
#					define ETREE_USING_THRUST
#				endif
#			endif
#		endif
#	endif
#endif
	
// Provide __host__ __device__ preprocessor directives when available
#ifdef ETREE_USING_THRUST
#	define HOST_DEVICE(code) __host__ __device__ code
#else
#	define HOST_DEVICE(code) code
#endif

#ifdef ETREE_USING_THRUST
#	define DEVICE(code) __device__ code
#else
#	define THRUST_DEVICE(code) code
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
