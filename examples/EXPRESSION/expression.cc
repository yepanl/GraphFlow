#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "graphflow.h"

using namespace GraphFlow;

class AddModule: public Module {
public:
    AddModule(): Module("add") {}
    ~AddModule() {}

    virtual void PROCESS() {
        spMessage msgFromA = getInputMessage("a");
        spMessage msgFromB = getInputMessage("b");
        spMessage msgFromC = getInputMessage("c");

        if (msgFromA->getType() == MESSAGE_TYPE_INT &&
            msgFromB->getType() == MESSAGE_TYPE_INT &&
            msgFromC->getType() == MESSAGE_TYPE_INT) {
            int32_t a = (static_cast<IntMessage<int32_t> *>(msgFromA.get()))->getValue();
            int32_t b = (static_cast<IntMessage<int32_t> *>(msgFromB.get()))->getValue();
            int32_t c = (static_cast<IntMessage<int32_t> *>(msgFromC.get()))->getValue();

            spInt32Message intMsg(new IntMessage<int32_t>(a + b + c));
            putOutputMessage(intMsg);
//          std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
};

class ModuleA: public Module {
public:
    ModuleA(): Module("a") {}
    ~ModuleA() {}

    virtual void PROCESS() {
        spInt32Message intMsg(new IntMessage<int32_t>(10));
        putOutputMessage(intMsg);
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

class ModuleB: public Module {
public:
    ModuleB(): Module("b") {}
    ~ModuleB() {}

    virtual void PROCESS() {
        spInt32Message intMsg(new IntMessage<int32_t>(20));
        putOutputMessage(intMsg);
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

class ModuleC: public Module {
public:
    ModuleC(): Module("c") {}
    ~ModuleC() {}

    virtual void PROCESS() {
        spInt32Message intMsg(new IntMessage<int32_t>(30));
        putOutputMessage(intMsg);
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

class ModuleD: public Module {
public:
    ModuleD(): Module("d") {}
    ~ModuleD() {}

    virtual void PROCESS() {
        spMessage msgFromAdd = getInputMessage("add");
        if (msgFromAdd->getType() == MESSAGE_TYPE_INT) {
            int32_t value = (static_cast<IntMessage<int32_t> *>(msgFromAdd.get()))->getValue();
            spInt32Message intMsg(new IntMessage<int32_t>(value + 10));
            putOutputMessage(intMsg);
//          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

class ModuleE: public Module {
public:
    ModuleE(): Module("e") {}
    ~ModuleE() {}

    virtual void PROCESS() {
        spMessage msgFromAdd = getInputMessage("add");
        if (msgFromAdd->getType() == MESSAGE_TYPE_INT) {
            int32_t value = (static_cast<IntMessage<int32_t> *>(msgFromAdd.get()))->getValue();
            spInt32Message intMsg(new IntMessage<int32_t>(value + 20));
            putOutputMessage(intMsg);
//          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

class ModuleF: public Module {
public:
    ModuleF(): Module("f") {}
    ~ModuleF() {}

    virtual void PROCESS() {
        spMessage msgFromD = getInputMessage("d");
        spMessage msgFromE = getInputMessage("e");

        if (msgFromD->getType() == MESSAGE_TYPE_INT &&
            msgFromE->getType() == MESSAGE_TYPE_INT) {
            int32_t d = (static_cast<IntMessage<int32_t> *>(msgFromD.get()))->getValue();
            int32_t e = (static_cast<IntMessage<int32_t> *>(msgFromE.get()))->getValue();

            LOG("d * e = %d", d * e);
            std::cout << "d * e = " << d * e << std::endl;
        }
    }
};

int main() {
    Graph *g = Graph::getInstance();

loop:
    Module *a = new ModuleA();
    Module *b = new ModuleB();
    Module *c = new ModuleC();
    Module *add = new AddModule();
    Module *d = new ModuleD();
    Module *e = new ModuleE();
    Module *f = new ModuleF();

    g->ADD_MODULE(a);
    g->ADD_MODULE(b);
    g->ADD_MODULE(c);
    g->ADD_MODULE(add);
    g->ADD_MODULE(d);
    g->ADD_MODULE(e);
    g->ADD_MODULE(f);

    a->TO(add)->setLimit(1);
    b->TO(add)->setLimit(1);
    c->TO(add)->setLimit(1);
    add->TO(d)->setLimit(1);
    add->TO(e)->setLimit(1);
    d->TO(f)->setLimit(1);
    e->TO(f)->setLimit(1);

    if (g->RUN() < 0) {
       std::cout << "run failed!" << std::endl;
       return 0;
    }

    g->dumpLinks();

//   std::this_thread::sleep_for(std::chrono::seconds(2));

    g->STOP();
    goto loop;

    return 0;
}


