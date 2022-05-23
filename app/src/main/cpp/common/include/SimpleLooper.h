//
// Created by liuyuzhou on 2022/5/18.
//
#ifndef OPENGLASSEMBLE_SIMPLELOOPER_H
#define OPENGLASSEMBLE_SIMPLELOOPER_H

#include <string>
#include <deque>
#include <mutex>
#include <memory>
#include <atomic>

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
    MessageEnvelope(const MessageEnvelope &msgHandler) noexcept;
    MessageEnvelope& operator =(MessageEnvelope &&envelope) noexcept;
    MessageEnvelope& operator =(const MessageEnvelope &envelope) noexcept;
    ~MessageEnvelope();

    int64_t mUpTimeNano;
    std::shared_ptr<MessageHandler> mHandler;
    Message mMessage;
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

    const std::string& getName();
    bool isValid();
    void loop();
    void requestQuit();
    void sendMessage(const std::shared_ptr<MessageHandler> &handler, const Message &msg);
    void sendMessageDelay(int64_t upTimeMill, const std::shared_ptr<MessageHandler> &handler, const Message &msg);
private:
    std::string mName;
    int mWakeEventFd;
    int mEpollFd;
    int64_t mNextMsgTimeNano;
    //todo use link to implement message queue;
    std::deque<MessageEnvelope> mMessageEnvelopes;
    // this filed is guarded by mMutex
    bool mSendingMessage;
    std::atomic<bool> mRunning;
    std::atomic<bool> mPolling;

    std::mutex mMutex;

    void awoken();
    int64_t currentTimeNano();
    void createEpoll();
    int pollOnce(int64_t timeoutMill);
    void release();
    void sendMessageAtTime(int64_t upTimeNano, const std::shared_ptr<MessageHandler> &handler, const Message &msg);
    /**
     * Wakes the poll asynchronously.
     *
     * This method can be called on any thread and returns immediately.
     */
    void wake();
};

#endif //OPENGLASSEMBLE_SIMPLELOOPER_H
