#include "testdll.h"

// This is an example of an exported function.
TESTDLL_API int sum(int value1, int value2)
{
    return value1 + value2;
}
