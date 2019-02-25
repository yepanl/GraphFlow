#include <iostream>
#include <thread>
#include "graphflow.h"

using namespace GraphFlow;

#define MSG_TYPE_USER_TEST (MESSAGE_TYPE)(MESSAGE_TYPE_USER + 1)
class MyMessage: public Message {
public:
    MyMessage(int count)
        : Message(MSG_TYPE_USER_TEST),
          mCount(count) {
        mMem1M.resize(0x1 << 20);
    }
    virtual ~MyMessage() {}

    std::vector<char> mMem1M;
    int mCount;
};
typedef std::shared_ptr<MyMessage> spMyMessage;

class InputModule: public Module {
public:
    InputModule()
        : Module("Input"),
          mCount(0) {}
    ~InputModule() {}

    virtual void PROCESS() {
        spMyMessage inputMsg(new MyMessage(mCount++));
        putOutputMessage(inputMsg);
    }

private:
    int mCount;
};

class ProcessModule: public Module {
public:
    ProcessModule(): Module("Process") {}
    ~ProcessModule() {}

    virtual void PROCESS() {
        spMessage spMsg = getInputMessage("Input");

        if (spMsg->getType() == MSG_TYPE_USER_TEST) { 
            putOutputMessage(spMsg);
        }
    }
};

class OutputModule: public Module {
public:
    OutputModule(): Module("Output") {}
    ~OutputModule() {}

    virtual void PROCESS() {
        spMessage spMsg = getInputMessage("Process");

        if (spMsg->getType() == MSG_TYPE_USER_TEST) { 
            int flag = (static_cast<MyMessage *>(spMsg.get()))->mCount;
            std::cout << "Message Flag:" << flag << std::endl;
        }

    }
};

int main() {
    Graph *g = Graph::getInstance();

    Module *inputModule = new InputModule();
    Module *outputModule = new OutputModule();
    Module *processModule = new ProcessModule();

    g->ADD_MODULE(inputModule);
    g->ADD_MODULE(outputModule);
    g->ADD_MODULE(processModule);

    inputModule->TO(processModule)->setLimit(1);
    processModule->TO(outputModule)->setLimit(1);

    if (g->RUN() < 0) {
       std::cout << "RUN failed!" << std::endl;
       return 0;
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
//    g->STOP();
    return 0;
}


