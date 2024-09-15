#pragma once

#ifdef _MSC_VER
	#include <io.h>
	#define isatty _isatty
#else
	#include <unistd.h>
#endif
