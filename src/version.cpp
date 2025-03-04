//==============================================================================
//
//  version.cpp
//
//==============================================================================
//  automatically generated on 2025-03-03 21:42:31
//==============================================================================


#include <string.h>
#include "version.h"

#ifdef RELEASE_MODE
unsigned int version::major  = 1;
unsigned int version::minor  = 4;
unsigned int version::build  = 0;
std::string  version::sha    = "a225393";
std::string  version::branch = "master";
#else
unsigned int version::major  = 1;
unsigned int version::minor  = 2;
unsigned int version::build  = 39;
std::string  version::sha    = "a225393";
std::string  version::branch = "master";
#endif // RELEASE_MODE
