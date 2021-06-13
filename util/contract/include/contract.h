#include <assert.h>

#ifndef CONTRACT
#define CONTRACT

#define INVARIANT(expression) assert(expression)
#define PRECONDITION(expression) assert(expression)
#define POSTCONDITION(expression) assert(expression)

#endif