#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include "common/logging.h"

namespace GraphFlow {

#define DIE(message) \
do { \
    LOG(LOG_LEVEL_FATAL, message); \
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
#define SET_THREAD_NAME(name) \
    std::cout << "SET_THREAD_NAME() IS NOT implemented under this platform" << std::endl;
#endif


class TimeUtils {
public:
    static int64_t getSystemTimeMs() {
        auto time_now = std::chrono::system_clock::now();
        auto duration_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());

        return duration_in_ms.count();
    }
};

/*
 * Used in module currently, and mLoglevel should be defined if any logic block use it.
 * Note:
 *     LOG_LEVEL_DEBUG is the default log level, and it'll be logged only when parameter
       logLevel >= LOG_LEVEL_DEBUG.
 */
class TimeElapsedMs {
public:
    TimeElapsedMs(std::string name, int logLevel=0)
        : mStartTimeMs(TimeUtils::getSystemTimeMs()), 
          mLogLevel(logLevel),
          mName(name) {}

    virtual ~TimeElapsedMs() {
        LOG(LOG_LEVEL_DEBUG, "[%s]: Time elapsed %ld ms", mName.c_str(),
            TimeUtils::getSystemTimeMs() - mStartTimeMs);
    }

private:
    int64_t mStartTimeMs;
    int mLogLevel;
    std::string mName;
};

};

#endif
