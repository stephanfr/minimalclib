// Copyright 2023 Stephan Friedl. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

#include <minimalstdclib_result_codes.h>

#ifdef __cplusplus
extern "C"
{
#endif

void __exit( int status_code );
#define exit __exit

void __assert (const char *msg, const char *file, int line);
#define assert( expression ) (void)((expression) || (__assert( #expression, __FILE__, __LINE__ ), 0))

char* itoa( int value, char *str, int base );
char* utoa( unsigned int value, char *str, int base );

//  On freestanding (bare-metal) targets there is no system stdlib.h, so
//  declare the custom 4-parameter variant.  On hosted targets the system
//  stdlib.h provides the standard 3-parameter strtol; omit the conflicting
//  declaration here to avoid linkage errors when system headers are pulled
//  in transitively (e.g. via CppUTest on x86-64 native test builds).
#if __STDC_HOSTED__ == 0
long strtol(const char *numeric_string, char **first_nonconverted_character, int base, int *result_code);
#endif

/*
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
*/

#ifdef __cplusplus
}
#endif

