//
// Created by liuyuzhou on 2022/5/18.
//
#ifndef OPENGLASSEMBLE_SIMPLELOOPER_H
#define OPENGLASSEMBLE_SIMPLELOOPER_H

#include <string>
#include <mutex>
#include <atomic>

#include "Message.h"

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
    void sendMessage(const std::shared_ptr<Message> &message);
    void sendMessageDelay(int64_t upTimeMill, const std::shared_ptr<Message> &message);
private:
    std::string mName;
    int mWakeEventFd;
    int mEpollFd;
    int64_t mNextMsgTimeNano;
    // this filed is guarded by mMutex
    bool mSendingMessage;
    std::atomic<bool> mRunning;
    std::atomic<bool> mPolling;
    // mHead is only used as place a holder
    Message mHead;

    std::mutex mMutex;

    void awoken();
    int64_t currentTimeNano();
    void createEpoll();
    int pollOnce(int64_t timeoutMill);
    void release();
    void sendMessageAtTime(int64_t upTimeNano, const std::shared_ptr<Message> &message);
    /**
     * Wakes the poll asynchronously.
     *
     * This method can be called on any thread and returns immediately.
     */
    void wake();
};

#endif //OPENGLASSEMBLE_SIMPLELOOPER_H
