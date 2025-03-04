//==============================================================================
//
//  version.cpp
//
//==============================================================================
//  automatically generated on 2025-03-03 21:20:38
//==============================================================================


#include <string.h>
#include "version.h"

#ifdef RELEASE_MODE
unsigned int version::major  = 1;
unsigned int version::minor  = 2;
unsigned int version::build  = 1;
std::string  version::sha    = "5e74096";
std::string  version::branch = "master";
#else
unsigned int version::major  = 1;
unsigned int version::minor  = 2;
unsigned int version::build  = 28;
std::string  version::sha    = "5e74096";
std::string  version::branch = "master";
#endif // RELEASE_MODE
