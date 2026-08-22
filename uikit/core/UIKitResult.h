#ifndef UIKIT_RESULT_H
#define UIKIT_RESULT_H

/**
 * @file UIKitResult.h
 * @brief Error codes for the UIKit C API.
 *
 * The C API avoids exceptions. Functions return UIKitResult to indicate
 * success or failure. The C++ wrapper may optionally use exceptions,
 * but exceptions should not be required.
 */

typedef enum {
    UIKIT_OK              = 0,
    UIKIT_ERROR_INVALID_ARG,
    UIKIT_ERROR_NO_MEMORY,
    UIKIT_ERROR_NOT_INITIALIZED,
    UIKIT_ERROR_ALREADY_EXISTS,
    UIKIT_ERROR_NOT_SUPPORTED,
    UIKIT_ERROR_NOT_FOUND,
    UIKIT_ERROR_BUSY
} UIKitResult;

/**
 * Convert a UIKitResult to a human-readable string.
 */
const char *UIKitResult_to_string(UIKitResult result);

#endif /* UIKIT_RESULT_H */
