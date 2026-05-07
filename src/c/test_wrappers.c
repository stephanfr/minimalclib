/* Wrapper to expose a non-conflicting symbol for the project's strtol implementation
   so hosted tests can call it without colliding with the system prototype. */

#include <stddef.h>

/* Declare the project's implementation signature (no system headers included here)
   and forward to it under a distinct symbol name. */
extern long strtol(const char *numeric_string, char **first_nonconverted_character, int base, int *result_code);

long project_strtol(const char *numeric_string, char **first_nonconverted_character, int base, int *result_code)
{
    return strtol(numeric_string, first_nonconverted_character, base, result_code);
}
