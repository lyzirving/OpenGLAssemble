//
// Created by liuyuzhou on 2022/5/18.
//
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/time.h>
#include <bits/epoll_event.h>
#include <linux/eventpoll.h>

#include <cerrno>
#include <unistd.h>
#include <cstring>

#include "SimpleLooper.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SimpleLooper"

// Hint for number of file descriptors to be associated with the epoll instance.
static const int EPOLL_SIZE_HINT = 8;
static const int EPOLL_MAX_EVENTS = 16;

MessageHandler::MessageHandler() = default;

MessageHandler::~MessageHandler() = default;

MessageEnvelope::MessageEnvelope() : mUpTimeUs(0), mHandler(nullptr), mMessage() {}

MessageEnvelope::MessageEnvelope(int64_t u, const std::shared_ptr<MessageHandler> &h,
                                 const Message &m) : mUpTimeUs(u), mHandler(h), mMessage(m) {}

MessageEnvelope::MessageEnvelope(MessageEnvelope &&envelope) noexcept : mUpTimeUs(envelope.mUpTimeUs),
                                                               mHandler(std::move(envelope.mHandler)),
                                                               mMessage(envelope.mMessage)  {}

MessageEnvelope &MessageEnvelope::operator=(MessageEnvelope &&envelope) noexcept {
    if(this != &envelope) {
        this->mUpTimeUs = envelope.mUpTimeUs;
        this->mHandler = envelope.mHandler;
        this->mMessage = envelope.mMessage;
        envelope.mHandler.reset();
    }
    return *this;
}

SimpleLooper::SimpleLooper(const char *name) : mName(name), mWeakEventFd(-1), mEpollFd(-1),
                                               mNextMsgTime(LLONG_MAX), mMutex() {
    mWeakEventFd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (mWeakEventFd < 0)
        LogE("looper(%s) fail to create weak event fd", mName.c_str());
    else
        createEpoll();
}

SimpleLooper::~SimpleLooper() {
    release();
}

void SimpleLooper::awoken() {
    uint64_t count;
    read(mWeakEventFd, &count, sizeof(uint64_t));
}

void SimpleLooper::createEpoll() {
    int ret{0};
    epoll_event evtItem{};
    std::lock_guard<std::mutex> lock(mMutex);
    mEpollFd = epoll_create(EPOLL_SIZE_HINT);
    if(mEpollFd < 0) {
        LogE("looper(%s) fail to create epoll instance, reason = %s", mName.c_str(), strerror(errno));
        goto fail;
    }
    memset(&evtItem, 0, sizeof(epoll_event));
    evtItem.events = EPOLLIN;
    evtItem.data.fd = mWeakEventFd;
    ret = epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mWeakEventFd, &evtItem);
    if (ret != 0) {
        LogE("looper(%s) fail to add weak event, reason = %s",mName.c_str(), strerror(errno));
        goto fail;
    }
    LogI("looper(%s) succeed to create epoll", mName.c_str());
    return;
    fail:
    release();
}

int64_t SimpleLooper::currentTimeU() {
    timeval time{};
    time.tv_sec = time.tv_usec = 0;
    gettimeofday(&time, nullptr);
    return time.tv_sec * 1000000 + time.tv_usec;
}

bool SimpleLooper::isValid() {
    return mWeakEventFd >= 0 && mEpollFd >= 0;
}

void SimpleLooper::loop() {
    if (!isValid()) {
        LogE("looper(%s) is not valid, return", mName.c_str());
        return;
    }
}

int SimpleLooper::pollOnce() {
    int result = POLL_WAKE;
    epoll_event eventItems[EPOLL_MAX_EVENTS];
    int eventCnt = epoll_wait(mEpollFd, eventItems, EPOLL_MAX_EVENTS, 0);

    mMutex.lock();
    //check for epoll error
    if (eventCnt < 0) {
        result = POLL_ERROR;
        goto done;
    }
    //check for epoll timeout
    if (eventCnt == 0) {
        result = POLL_TIMEOUT;
        goto done;
    }
    //process epoll event
    for (int i = 0; i < eventCnt; i++) {
        int fd = eventItems[i].data.fd;
        uint32_t eventType = eventItems[i].events;
        if (fd == mWeakEventFd) {
            if (eventType & EPOLLIN)
                awoken();
            else
                LogE("unexpected epoll events(0x%x) on wake event fd", eventType);
        } else
            LogE("unexpected events for fd(%d), we only need events on wake event fd(%d)", fd,
                 mWeakEventFd);
    }
    done:

    mNextMsgTime = LLONG_MAX;
    return result;
}

void SimpleLooper::release() {
    int ret{0};
    if(mWeakEventFd >= 0) {
        ret = close(mWeakEventFd);
        if(ret != 0)
            LogE("looper(%s) fail to close weak evt fd, reason = %s", mName.c_str(), strerror(errno));
    }
    mWeakEventFd = -1;

    if(mEpollFd >= 0) {
        ret = close(mEpollFd);
        if(ret != 0)
            LogE("looper(%s) fail to close weak epoll instance, reason = %s", mName.c_str(), strerror(errno));
    }
    mEpollFd = -1;
}

void SimpleLooper::sendMessage(const std::shared_ptr<MessageHandler> &handler,
                               const Message &msg) {
    int64_t now = currentTimeU();
    sendMessageAtTime(now, handler, msg);
}

void SimpleLooper::sendMessageDelay(int64_t delayMilSec,
                                    const std::shared_ptr<MessageHandler> &handler,
                                    const Message &msg) {
    int64_t now = currentTimeU();
    sendMessageAtTime(delayMilSec * 1000 + now, handler, msg);
}

void SimpleLooper::sendMessageAtTime(int64_t execTimeU,
                                     const std::shared_ptr<MessageHandler> &handler,
                                     const Message &msg) {
    MessageEnvelope envelope(execTimeU, handler, msg);
    int i = 0;
    {

    }
}

