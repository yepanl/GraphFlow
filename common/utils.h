#ifndef __UTILS_H__
#define __UTILS_H__

#include "common/log.h"

#define DIE(message) \
do { \
    LOG(message); \
    exit(0); \
} while (0)

#define ASSERT_PTR(ptr, message) \
do { \
    if (!ptr) \
        DIE(message); \
} while (0)

#ifdef __LINUX__

/* Set thread Name on Linux system
 * Note: 1, name must be std::string type, and its length is
 *          best less than 16 bytes, and will be truncated if more.
 *       2, you can goto /proc/[pid]/task/ directory, and see
 *          all threads tid under this process.
 *       3, Got all threads information via command `ps -T -p [pid]`,
 *          also via command `top -H`.
 */
#include <sys/prctl.h>
#define SET_THREAD_NAME(name) \
    prctl(PR_SET_NAME, name.c_str(), 0, 0, 0)

#else

#define SET_THREAD_NAME(name)

#endif

#endif
