#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <vector>
#include <map>
#include <thread>

namespace GraphFlow {

class Link;
class Module;

class Graph final {
public:
    Graph(std::string &&name = "");
    Graph(std::string &name);
    virtual ~Graph();

    virtual int ADD_MODULE(Module *module) final;
    virtual void DEL_MODULE(Module *module) final;
    virtual int RUN() final;
    virtual void STOP() final;

    virtual bool needStop() final;
    virtual void dumpLinks() final;

private:
    std::string mName;
    bool mNeedStop = false;
    std::vector<Link *> mLinks;
    std::vector<Module *> mModules;
    std::map<std::thread *, Module *> mMap;

    int mLogLevel;
};


};

#endif
