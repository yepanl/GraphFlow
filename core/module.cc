#include <iostream>

#include "link.h"
#include "module.h"
#include "graph.h"
#include "common/utils.h"

namespace GraphFlow {

Module::Module(std::string &name, int logLevel) {
    mName = name;
    mLogLevel = logLevel;
}

Module::Module(std::string &&name, int logLevel) {
    mName = name;
    mLogLevel = logLevel;
}
Module::~Module() {}

int Module::setGraph(Graph *graph) {
    mGraph = graph;
    return 0;
}

Graph *Module::getGraph() {
    return mGraph;
}

int Module::init() {
    return 0;
}

void Module::unInit() {
    return;
}

std::string &Module::getName() {
    return mName;
}

Link *Module::TO(Module *destModule, std::string &inputName) {
    ASSERT_PTR(destModule, "destination module is nullptr");

    Link *link = new Link();
    ASSERT_PTR(link, "OOM, new Link() failed");

    link->setEndpoint(Link::LINK_ENDPOINT_FROM, this);
    link->setEndpoint(Link::LINK_ENDPOINT_TO, destModule);
    link->setName(mName + "_" + destModule->getName());

    mOutputLinks.push_back(link);

    // @TODO, what when it has the same input name ?
    if (inputName.empty()) {
        destModule->mInputLinks.insert(std::pair<std::string, Link *>(getName(), link));
    } else {
        destModule->mInputLinks.insert(std::pair<std::string, Link *>(inputName, link));
    }

    return link;
}

Link *Module::TO(Module *destModule, std::string &&inputName) {
    ASSERT_PTR(destModule, "destination module is nullptr");

    Link *link = new Link();
    ASSERT_PTR(link, "OOM, new Link() failed");

    link->setEndpoint(Link::LINK_ENDPOINT_FROM, this);
    link->setEndpoint(Link::LINK_ENDPOINT_TO, destModule);
    link->setName(mName + "_" + destModule->getName());

    mOutputLinks.push_back(link);

    // @TODO, what when it has the same input name ?
    if (inputName.empty()) {
        destModule->mInputLinks.insert(std::pair<std::string, Link *>(getName(), link));
    } else {
        destModule->mInputLinks.insert(std::pair<std::string, Link *>(inputName, link));
    }

    return link;
}

std::vector<Link *> &Module::getOutputLinks()  {
    return mOutputLinks;
}

// get message from input link by name
spMessage Module::getInputMessage(std::string &name) {
    if (mInputMessages.find(name) != mInputMessages.end()) {
        return mInputMessages[name];
    }

    return nullptr;
}

spMessage Module::getInputMessage(std::string &&name) {
    if (mInputMessages.find(name) != mInputMessages.end()) {
        return mInputMessages[name];
    }

    return nullptr;
}

// put result into output links
void Module::putOutputMessage(spMessage &message) {
    if (message) {
        mOutputMessage = message;
    }
}

void Module::putOutputMessage(spMessage &&message) {
    if (message) {
        mOutputMessage = message;
    }
}

int Module::preProcess() {
    // drop messages from input links into mInputMessages
    for (auto pair: mInputLinks) {
        mInputMessages.insert(std::pair<std::string, spMessage>(pair.first, pair.second->pop()));
    }

    return 0;
}

int Module::postProcess() {
    mInputMessages.clear();

    // drop result message from mOutputMessage into output links
    if (mOutputMessage) {
        for (auto link: mOutputLinks) {
            link->push(mOutputMessage);
        }
    }

    // check whether flow is stopped
    if (mGraph->needStop()) {
        // self is last module
        if (mOutputLinks.empty()) {
            for (auto pair: mInputLinks) {
                pair.second->setExitedLocked();
            }

            return -1; //self exited
        } else {
            for (auto link: mOutputLinks) {
                if (!link->getExitedLocked()) {
                    return 0; //self normal
                }
            }

            for (auto pair: mInputLinks) {
                pair.second->setExitedLocked();
            }

            return -1; //self exited
        }
    }

    return 0;
}

void Module::RUN() {
    while (true) {
        preProcess();
        PROCESS();

        if (postProcess() < 0) {
            break;
        }
    }
}

};


