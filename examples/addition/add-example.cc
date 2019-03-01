#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "graphflow.h"

using namespace GraphFlow;

class AddModule: public Module {
public:
    AddModule(): Module("add", LOG_LEVEL_MESSAGE) {}
    ~AddModule() {}

    virtual void PROCESS() {
        spMessage msgFromA = getInputMessage("a");
        spMessage msgFromB = getInputMessage("b");

        if (msgFromA->getType() == MESSAGE_TYPE_INT &&
            msgFromB->getType() == MESSAGE_TYPE_INT) {
            int32_t a = (static_cast<IntMessage<int32_t> *>(msgFromA.get()))->getValue();
            int32_t b = (static_cast<IntMessage<int32_t> *>(msgFromB.get()))->getValue();

            LOG(LOG_LEVEL_MESSAGE, "a + b = %d", a + b);
        }
    }
};

class ModuleA: public Module {
public:
    ModuleA(): Module("a", LOG_LEVEL_MESSAGE) {}
    ~ModuleA() {}

    virtual void PROCESS() {
        spInt32Message intMsg(new IntMessage<int32_t>(10));
        putOutputMessage(intMsg);
    }
};

class ModuleB: public Module {
public:
    ModuleB(): Module("b", LOG_LEVEL_MESSAGE) {}

    ~ModuleB() {}

    virtual void PROCESS() {
        spInt32Message intMsg(new IntMessage<int32_t>(20));
        putOutputMessage(intMsg);
    }
};

int main() {
    Graph *g = new Graph();

    Module *a = new ModuleA();
    Module *b = new ModuleB();
    Module *add = new AddModule();

    g->ADD_MODULE(a);
    g->ADD_MODULE(b);
    g->ADD_MODULE(add);

    a->TO(add)->setLimit(1);
    b->TO(add)->setLimit(1);

    if (g->RUN() < 0) {
       std::cout << "RUN failed!" << std::endl;
       return 0;
    }

    g->STOP();
    return 0;
}


