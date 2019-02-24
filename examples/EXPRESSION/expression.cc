#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "graphflow.h"

using namespace GraphFlow;

class IntMessage: public Message {
public:
    IntMessage(int value): mValue(value) {}
    virtual ~IntMessage() {}

    int getValue() {
        return mValue;
    }

private:
    int mValue;
};

class AddModule: public Module {
public:
    AddModule(): Module(std::string("add")) {
    }

    ~AddModule() {}

    virtual void PROCESS() {
        int a = getInputMessage("a")->getValue();
        int b = getInputMessage("b")->getValue();
        int c = getInputMessage("c")->getValue();
        
        spMessage message(new IntMessage(a + b + c));
        putOutputMessage(message);
//        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

private:
};

class ModuleA: public Module {
public:
    ModuleA(): Module("a") {}

    ~ModuleA() {}

    virtual void PROCESS() {
        spMessage message(new IntMessage(10));
        putOutputMessage(message);
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

class ModuleB: public Module {
public:
    ModuleB(): Module("b") {}

    ~ModuleB() {}

    virtual void PROCESS() {
        spMessage message(new IntMessage(20));
        putOutputMessage(message);
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

class ModuleC: public Module {
public:
    ModuleC(): Module("c") {}

    ~ModuleC() {}

    virtual void PROCESS() {
        spMessage message(new IntMessage(30));
        putOutputMessage(message);
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

class ModuleD: public Module {
public:
    ModuleD(): Module("d") {}

    ~ModuleD() {}

    virtual void PROCESS() {
        spMessage message(new IntMessage(getInputMessage("add")->getValue() + 10));
        putOutputMessage(message);
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

class ModuleE: public Module {
public:
    ModuleE(): Module("e") {}

    ~ModuleE() {}

    virtual void PROCESS() {
        spMessage message(new IntMessage(getInputMessage("add")->getValue() + 20));
        putOutputMessage(message);
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

class ModuleF: public Module {
public:
    ModuleF(): Module("f") {}

    ~ModuleF() {}

    virtual void PROCESS() {
        int d = getInputMessage("d")->getValue();
        int e = getInputMessage("e")->getValue();
        std::cout << "d * e = " << d * e << std::endl;
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


