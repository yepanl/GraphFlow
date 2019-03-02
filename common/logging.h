#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

namespace GraphFlow {

#define LOG_LEVEL_FATAL   0
#define LOG_LEVEL_ERROR   1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_MESSAGE 3
#define LOG_LEVEL_DEBUG   4
#define LOG_LEVEL_DETAIL  5

/* Logging
 * Note: This macro should be used in user-defined modules that
 *       can't be private derived.
 */
#define LOG(level, format, ...) \
    { \
        if (mLogLevel >= level) { \
            printf("[%s]:" format "\n", __FUNCTION__, ##__VA_ARGS__); \
        } \
    }

};

#endif
