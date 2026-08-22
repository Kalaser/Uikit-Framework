#include "UIKitResult.h"

static const char *result_strings[] = {
    "OK",
    "Invalid argument",
    "Out of memory",
    "Not initialized",
    "Already exists",
    "Not supported",
    "Not found",
    "Busy"
};

const char *UIKitResult_to_string(UIKitResult result)
{
    if (result < 0 || result >= (int)(sizeof(result_strings) / sizeof(result_strings[0]))) {
        return "Unknown error";
    }
    return result_strings[result];
}
