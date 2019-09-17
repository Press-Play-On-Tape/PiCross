#pragma once

#include <stddef.h>


// ----------------------------------------------------------------------------
//  A better absolute as it uses less memory than the standard one .. 
//
template<typename T> T absT(const T & v) {
  
  return (v < 0) ? -v : v;

}

// This gets the length of an array at compile time
template< typename T, size_t N > constexpr size_t ArrayLength(T (&)[N]) {
	return N;
}