//
// Created by liuyuzhou on 2022/5/18.
//
#ifndef OPENGLASSEMBLE_SIMPLELOOPER_H
#define OPENGLASSEMBLE_SIMPLELOOPER_H

#include <string>
#include <mutex>
#include <memory>

struct Message {
    Message() : what(0) { }
    Message(int w) : what(w) {}
    Message(const Message &msg) : what(msg.what) {}

    int what;
};

class MessageHandler {
public:
    MessageHandler();
    virtual ~MessageHandler();

    virtual void handleMessage(const Message &message) = 0;
};

class MessageEnvelope {
public:
    MessageEnvelope();
    MessageEnvelope(int64_t u, const std::shared_ptr<MessageHandler> &h, const Message &m);
    MessageEnvelope(MessageEnvelope &&envelope) noexcept;
    MessageEnvelope& operator =(MessageEnvelope &&envelope) noexcept;

    int64_t mUpTimeUs;
    std::shared_ptr<MessageHandler> mHandler;
    Message mMessage;

private:
    MessageEnvelope(const MessageEnvelope &msgHandler);
};

class SimpleLooper {
public:
    enum {
        POLL_WAKE     = -1,
        POLL_CALLBACK = -2,
        POLL_TIMEOUT  = -3,
        POLL_ERROR    = -4,
    };
    SimpleLooper(const char* name);
    ~SimpleLooper();

    bool isValid();
    void loop();
    void sendMessage(const std::shared_ptr<MessageHandler> &handler, const Message &msg);
    void sendMessageDelay(int64_t delayMilSec, const std::shared_ptr<MessageHandler> &handler, const Message &msg);
private:
    std::string mName;
    int mWeakEventFd;
    int mEpollFd;
    int64_t mNextMsgTime;

    std::mutex mMutex;

    void awoken();
    int64_t currentTimeU();
    void createEpoll();
    int pollOnce();
    void release();
    void sendMessageAtTime(int64_t execTimeU, const std::shared_ptr<MessageHandler> &handler, const Message &msg);
};

#endif //OPENGLASSEMBLE_SIMPLELOOPER_H
