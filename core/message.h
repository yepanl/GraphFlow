#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <memory>

namespace GraphFlow {

typedef enum {
    MESSAGE_TYPE_INT = 0,
    MESSAGE_TYPE_FLOAT,
    MESSAGE_TYPE_DOUBLE,
    MESSAGE_TYPE_USER
} MESSAGE_TYPE;

class Message {
public:
    Message(MESSAGE_TYPE type=MESSAGE_TYPE_INT): mType(type) {}
    virtual ~Message() {}

    virtual void setType(MESSAGE_TYPE type) final;
    virtual MESSAGE_TYPE getType() final;

    virtual bool checkType(MESSAGE_TYPE type) final {
        return type == mType;
    }

private:
    MESSAGE_TYPE mType;
};
typedef std::shared_ptr<Message> spMessage;

template <typename T>
class IntMessage: public Message {
public:
    IntMessage(T value)
        : Message(),
          mValue(value) {}
    virtual ~IntMessage() {}

    virtual void setValue(T value) final {
        mValue = value;
    }
    virtual T getValue() final {
        return mValue;
    }

private:
    T mValue;
};
typedef std::shared_ptr<IntMessage<int8_t>> spInt8Message;
typedef std::shared_ptr<IntMessage<uint8_t>> spUInt8Message;
typedef std::shared_ptr<IntMessage<int16_t>> spInt16Message;
typedef std::shared_ptr<IntMessage<uint16_t>> spUInt16Message;
typedef std::shared_ptr<IntMessage<int32_t>> spInt32Message;
typedef std::shared_ptr<IntMessage<uint32_t>> spUInt32Message;
typedef std::shared_ptr<IntMessage<int64_t>> spInt64Message;
typedef std::shared_ptr<IntMessage<uint64_t>> spUInt64Message;

class FloatMessage: public Message {
public:
    FloatMessage(float value)
        : Message(MESSAGE_TYPE_FLOAT),
          mValue(value) {}
    virtual ~FloatMessage() {}

    virtual void setValue(float value) final {
        mValue = value;
    }
    virtual float getValue() final {
        return mValue;
    }

private:
    float mValue;
};
typedef std::shared_ptr<FloatMessage> spFloatMessage;

class DoubleMessage: public Message {
public:
    DoubleMessage(double value)
        : Message(MESSAGE_TYPE_DOUBLE),
          mValue(value) {}
    virtual ~DoubleMessage() {}

    virtual void setValue(double value) final {
        mValue = value;
    }
    virtual double getValue() final {
        return mValue;
    }

private:
    double mValue;
};
typedef std::shared_ptr<DoubleMessage> spDoubleMessage;

};


#endif
