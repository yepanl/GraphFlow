/*
    Link is actually a FIFO, push into tail and pop from head
*/

#include <iostream>

#include "link.h"
#include "module.h"
#include "graph.h"
#include "common/utils.h"

namespace GraphFlow {

Link::Link(): mLimit(UNLIMITED), mExited(false) {}

// no hold lock, must be destroyed after all threads quit
Link::~Link() {
    for (auto iter = mMsgQueue.begin(); iter != mMsgQueue.end();) {
        mMsgQueue.erase(iter++);
    }
}

int Link::setEndpoint(LINK_ENDPOINT endpoint, Module *module) {
    if (endpoint != LINK_ENDPOINT_FROM || endpoint != LINK_ENDPOINT_TO) {
        return -1;
    }

    mEndPoints[endpoint] = module;
    return 0;
}


void Link::setExited() {
    mExited = true;
}

bool Link::getExited() {
    return mExited;
}

void Link::setExitedLocked() {
    std::unique_lock<std::mutex> lock(mMutex);

    mExited = true;
    if (!mMsgQueue.empty()) {
        mMsgQueue.pop_front();
        mCondVar.notify_all();
    }
}

bool Link::getExitedLocked() {
    std::unique_lock<std::mutex> lock(mMutex);
    return mExited;
}

std::string &Link::getName() {
    return mName;
}

void Link::setName(std::string &name) {
    mName = name;
}

void Link::setName(std::string &&name) {
    mName = name;
}

Module *Link::getEndpoint(LINK_ENDPOINT endpoint) {
    if (endpoint != LINK_ENDPOINT_FROM || endpoint != LINK_ENDPOINT_TO) {
        return nullptr;
    }

    return mEndPoints[endpoint];
}

int Link::setLimit(int limit) {
    if (limit <= 0) {
        LOG("Set limit(%d) failed, invalid limit!", limit);
        return -1;
    }

    mLimit = limit;
    return 0;
}

int Link::getLimit() {
    return mLimit;
}

int Link::push(spMessage &message) {
    ASSERT_PTR(message, "Input message is nullptr");

    {
        std::unique_lock<std::mutex> lock(mMutex);
   
        if (mExited) {
            return 0;
        }

        if (mLimit != UNLIMITED) {
            mCondVar.wait(lock, [this] {return this->mMsgQueue.size() < (unsigned int)mLimit;});
        }

        mMsgQueue.push_back(message);
        mCondVar.notify_all();
    }

    return 0;
}

spMessage Link::pop() {
    spMessage msg(nullptr);

    {
        std::unique_lock<std::mutex> lock(mMutex);
    
        mCondVar.wait(lock, [this] {return !this->mMsgQueue.empty();});
        msg = mMsgQueue.front();
        mMsgQueue.pop_front();
        mCondVar.notify_all();
    }

    return msg;
}

}



