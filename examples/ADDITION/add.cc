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
        
        LOG("a + b = %d", a + b);
        std::cout << "a + b = " << a + b << std::endl;
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
    }
};

class ModuleB: public Module {
public:
    ModuleB(): Module("b") {}

    ~ModuleB() {}

    virtual void PROCESS() {
        spMessage message(new IntMessage(20));
        putOutputMessage(message);
    }
};

int main() {
    Graph *g = Graph::getInstance();

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


