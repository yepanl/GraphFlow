#ifndef __LINK_H__
#define __LINK_H__

#include <list>
#include <mutex>
#include <condition_variable>

#include "message.h"

namespace GraphFlow {

#define UNLIMITED (0)    // limit must greater than 0
class Module;
class Graph;

class Link {
public:
    typedef enum {
        LINK_ENDPOINT_FROM = 0,
        LINK_ENDPOINT_TO,
        LINK_ENDPOINT_MAX
    } LINK_ENDPOINT;

    Link();
    virtual ~Link();

    int setEndpoint(LINK_ENDPOINT endpoint, Module *module);
    Module *getEndpoint(LINK_ENDPOINT endpoint);

    int setLimit(int limit);
    int getLimit();

    void setExited();
    bool getExited();
    void setExitedLocked();
    bool getExitedLocked();

    std::string &getName();
    void setName(std::string &name);
    void setName(std::string &&name);

    // message input/output
    int push(spMessage &message);
    spMessage pop();

private:
    Module *mEndPoints[LINK_ENDPOINT_MAX];

    int mLimit;
    bool mExited;
    std::string mName;
    std::list<spMessage> mMsgQueue;
    std::mutex mMutex;
    std::condition_variable mCondVar;
};

};

#endif
