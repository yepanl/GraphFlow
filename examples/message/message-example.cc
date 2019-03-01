#include <iostream>
#include "graphflow.h"

using namespace GraphFlow;

#define MSG_TYPE_USER_TEST (MESSAGE_TYPE)(MESSAGE_TYPE_USER + 1)
class MyMessage: public Message {
public:
    MyMessage()
        : Message(MSG_TYPE_USER_TEST) {
    }
    virtual ~MyMessage() {}

    std::vector<std::string> mStringArray;
};
typedef std::shared_ptr<MyMessage> spMyMessage;

class ProcessModule: public Module {
public:
    ProcessModule(): Module("Process") {}
    ~ProcessModule() {}

    virtual void PROCESS() {
        spMessage spMsg = getInputMessage("Input");

        if (spMsg->getType() == MSG_TYPE_USER_TEST) { 
            std::vector<std::string> &stringsFromInput = (static_cast<MyMessage *>(spMsg.get()))->mStringArray;

            std::cout << "[Process] Strings From Input:" << std::endl;
            for (auto ele: stringsFromInput) {
                std::cout << "    " << ele << std::endl;
            }

            stringsFromInput.push_back("And");
            stringsFromInput.push_back("this");
            stringsFromInput.push_back("is");
            stringsFromInput.push_back("messages");
            stringsFromInput.push_back("from");
            stringsFromInput.push_back("Process");
      
            putOutputMessage(spMsg);
        }
    }
};

class InputModule: public Module {
public:
    InputModule(): Module("Input") {}
    ~InputModule() {}

    virtual void PROCESS() {
        spMyMessage inputMsg(new MyMessage());

        inputMsg->mStringArray.push_back("This");
        inputMsg->mStringArray.push_back("is");
        inputMsg->mStringArray.push_back("messages");
        inputMsg->mStringArray.push_back("from");
        inputMsg->mStringArray.push_back("input");
      
        putOutputMessage(inputMsg);
    }
};

class OutputModule: public Module {
public:
    OutputModule(): Module("Output") {}
    ~OutputModule() {}

    virtual void PROCESS() {
        spMessage spMsg = getInputMessage("Process");

        if (spMsg->getType() == MSG_TYPE_USER_TEST) { 
            std::vector<std::string> &stringsFromProcess = (static_cast<MyMessage *>(spMsg.get()))->mStringArray;

            std::cout << "[Process] Strings From Process:" << std::endl;
            for (auto ele: stringsFromProcess) {
                std::cout << "    " << ele << std::endl;
            }
        }

    }
};

int main() {
    Graph *g = new Graph();

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

    g->STOP();
    return 0;
}


