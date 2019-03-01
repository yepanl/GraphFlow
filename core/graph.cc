#include <iostream>
#include <functional>

#include "link.h"
#include "module.h"
#include "graph.h"
#include "common/utils.h"

namespace GraphFlow {

Graph::Graph(std::string &&name)
    : mName(name),
      mLogLevel(LOG_LEVEL_DETAIL) {}

Graph::Graph(std::string &name)
    : mName(name) {}
// nonlocked, must be destroyed after all threads quit
Graph::~Graph() {}

int Graph::ADD_MODULE(Module *module) {
    if (module) {
        mModules.push_back(module);
        module->setGraph(this);
        return 0;
    }

    return -1;
}

void Graph::DEL_MODULE(Module *module) {
    if (module) {
        for (auto iter = mModules.begin(); iter != mModules.end(); iter++) {
            if (module == *iter) {
                mModules.erase(iter);
            }
        }
    }
}

int Graph::RUN() {
    std::thread *pThread = nullptr;

    // initialize all modules
    for (auto module: mModules) {
        if (module->init() < 0) {
            LOG(LOG_LEVEL_FATAL, "Module(%s) init failed", module->getName().c_str());
            return -1;
        }
    }

    // collect all links
    for (auto module: mModules) {
        for (auto link: module->getOutputLinks()) {
            mLinks.push_back(link);
        }
    }

    for (auto module: mModules) {
        if (!(pThread = new std::thread(std::bind(&Module::RUN, module)))) {
            ASSERT_PTR(pThread, "new thread failed!");
        }

        mMap.insert(std::pair<std::thread *, Module *>(pThread, module));
    }
    return 0;
}

void Graph::dumpLinks() {
    LOG(LOG_LEVEL_DEBUG, "Grap Links [");
    for (auto link: mLinks) {
        LOG(LOG_LEVEL_DEBUG, "  %s", link->getName().c_str());
    }
    LOG(LOG_LEVEL_DEBUG, "]");
}

bool Graph::needStop() {
    return mNeedStop;
}

void Graph::STOP() {
    // stop all threads, must be in the master thread
    mNeedStop = true;

    for (auto pair: mMap) {
        pair.first->join();
        delete pair.first;
    }
    mMap.clear();

    for (auto link: mLinks) {
        delete link;
    }
    mLinks.clear();

    for (auto module: mModules) {
        module->unInit();
        delete module;
    }
    mModules.clear();
   
    LOG(LOG_LEVEL_DEBUG, "Free all graph resources, quit!");
}

};

