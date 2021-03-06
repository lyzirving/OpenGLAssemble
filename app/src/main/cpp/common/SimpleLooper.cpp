//
// Created by liuyuzhou on 2022/5/18.
//
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <ctime>
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

SimpleLooper::SimpleLooper(const char *name) :  mName(name), mWakeEventFd(-1), mEpollFd(-1),
                                                mNextMsgTimeNano(LLONG_MAX),
                                                mSendingMessage(false),
                                                mRunning(true),
                                                mPolling(false),
                                                mHead(),
                                                mMutex(){
    mWakeEventFd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if (mWakeEventFd < 0)
        LogE("looper(%s) fail to create wake event fd", mName.c_str());
    else
        createEpoll();
}

SimpleLooper::~SimpleLooper() {
    release();
}

void SimpleLooper::awoken() {
    uint64_t count;
    read(mWakeEventFd, &count, sizeof(uint64_t));
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
    evtItem.data.fd = mWakeEventFd;
    ret = epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mWakeEventFd, &evtItem);
    if (ret != 0) {
        LogE("looper(%s) fail to add wake event, reason = %s",mName.c_str(), strerror(errno));
        goto fail;
    }
    LogI("looper(%s) succeed to create epoll", mName.c_str());
    return;
    fail:
    release();
}

int64_t SimpleLooper::currentTimeNano() {
    timeval time{};
    time.tv_sec = time.tv_usec = 0;
    gettimeofday(&time, nullptr);
    return time.tv_sec * 1000000 + time.tv_usec;
}

const std::string& SimpleLooper::getName() {
    return mName;
}

bool SimpleLooper::isValid() {
    return mWakeEventFd >= 0 && mEpollFd >= 0;
}

void SimpleLooper::loop() {
    while(mRunning.load()) {
        int ret = pollOnce(-1);
        switch (ret) {
            case POLL_WAKE:
            case POLL_CALLBACK: {
                continue;
            }
            case POLL_ERROR: {
                LogE("looper(%s) poll error", mName.c_str());
                continue;
            }
            case POLL_TIMEOUT: {
                LogE("looper(%s) poll timeout", mName.c_str());
                continue;
            }
            default: {
                LogW("looper(%s) unknown poll state %d", mName.c_str(), ret);
                continue;
            }
        }
    }
    LogI("looper(%s) quit", mName.c_str());
    release();
}

int SimpleLooper::pollOnce(int64_t timeoutMill) {
    if(timeoutMill != 0 && mNextMsgTimeNano != LLONG_MAX) {
        int64_t now = currentTimeNano();
        int64_t msgTimeoutMill = 0;
        if(mNextMsgTimeNano > now) {
            uint64_t diff = mNextMsgTimeNano - now;
            if (diff > uint64_t((INT_MAX - 1) * 1000000L)) {
                msgTimeoutMill = -1;
            } else {
                msgTimeoutMill = (diff + 999999LL) / 1000000LL;
            }
        }
        if(msgTimeoutMill >= 0 && (timeoutMill < 0 || msgTimeoutMill < timeoutMill)) {
            timeoutMill = msgTimeoutMill;
        }
    }

    int result = POLL_WAKE;
    mPolling.store(true);
    epoll_event eventItems[EPOLL_MAX_EVENTS];
    int eventCnt = epoll_wait(mEpollFd, eventItems, EPOLL_MAX_EVENTS, timeoutMill);
    mPolling.store(false);

    mMutex.lock();
    //check for epoll error
    if (eventCnt < 0) {
        result = POLL_ERROR;
        LogE("looper(%s) poll fail with an unexpected error %s", mName.c_str(), strerror(errno));
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
        if (fd == mWakeEventFd) {
            if (eventType & EPOLLIN)
                awoken();
            else
                LogE("unexpected epoll events(0x%x) on wake event fd", eventType);
        } else
            LogE("unexpected events for fd(%d), we only need events on wake event fd(%d)", fd,
                 mWakeEventFd);
    }

    done:

    mNextMsgTimeNano = LLONG_MAX;
    std::shared_ptr<Message> current = mHead.mNext;
    std::shared_ptr<Message> last(nullptr);
    int64_t now = currentTimeNano();
    while(current != nullptr) {
        if(current->mUptimeNano <= now) {
            std::shared_ptr<MessageHandler> handler = current->mHandler;

            mSendingMessage = true;
            mMutex.unlock();
            handler->handleMessage(current->mWhat);
            mMutex.lock();
            mSendingMessage = false;

            result = POLL_CALLBACK;
            //only poll from head
            mHead.mNext = current->mNext;
            current = mHead.mNext;
        } else {
            mNextMsgTimeNano = current->mUptimeNano;
            break;
        }
    }

    mMutex.unlock();
    return result;
}

void SimpleLooper::requestQuit() {
    if(mRunning.load()) {
        mRunning.store(false);
        if(mPolling.load())
            wake();
    } else {
        release();
    }
}

void SimpleLooper::release() {
    int ret{0};
    if(mWakeEventFd >= 0) {
        ret = close(mWakeEventFd);
        if (ret == 0)
            LogI("looper(%s) succeed to close wake event fd %d", mName.c_str(), mWakeEventFd);
        else
            LogE("looper(%s) fail to close wake evt fd, reason = %s", mName.c_str(), strerror(errno));
    }
    mWakeEventFd = -1;

    if(mEpollFd >= 0) {
        ret = close(mEpollFd);
        if (ret == 0)
            LogI("looper(%s) succeed to close epoll fd %d", mName.c_str(), mEpollFd);
        else
            LogE("looper(%s) fail to close wake epoll instance, reason = %s", mName.c_str(), strerror(errno));
    }
    mEpollFd = -1;
}

void SimpleLooper::sendMessage(const std::shared_ptr<Message> &message) {
    int64_t now = currentTimeNano();
    sendMessageAtTime(now, message);
}

void SimpleLooper::sendMessageDelay(int64_t upTimeMill, const std::shared_ptr<Message> &message) {
    int64_t now = currentTimeNano();
    sendMessageAtTime(upTimeMill * 1000000L + now, message);
}

void SimpleLooper::sendMessageAtTime(int64_t upTimeNano, const std::shared_ptr<Message> &message) {
    bool insertAtFirst{false};
    {
        std::lock_guard<std::mutex> lock(mMutex);
        message->mUptimeNano = upTimeNano;
        if (mHead.mNext == nullptr) {
            insertAtFirst = true;
            mHead.mNext = message;
            goto inserted;
        } else {
            std::shared_ptr<Message> current = mHead.mNext;
            std::shared_ptr<Message> last(nullptr);
            while (current != nullptr) {
                if (upTimeNano >= current->mUptimeNano) {
                    last = current;
                    current = current->mNext;
                } else {
                    if (last == nullptr) {
                        insertAtFirst = true;//insert at head
                        last = mHead.mNext;
                        mHead.mNext = message;
                        mHead.mNext->mNext = last;
                    } else {
                        last->mNext = message;
                        last->mNext->mNext = current;
                    }
                    goto inserted;
                }
            }
            last->mNext = message;//insert the message at tail
        }
        inserted:
        // if the looper is sending message to its handlers, we should not produce a wake event;
        if (mSendingMessage)
            return;
    }
    // Wake the poll loop only when we enqueue a new message at the head.
    if(insertAtFirst)
        wake();
}

void SimpleLooper::wake() {
    if(isValid()) {
        uint64_t inc = 1;
        int64_t bytesWrite = write(mWakeEventFd, &inc, sizeof(uint64_t));
        if(bytesWrite != sizeof(uint64_t)) {
            LogE("looper(%s) fail to write to wake event, reason = %s",mName.c_str(), strerror(errno));
        }
    } else {
        LogE("looper(%s) is not valid", mName.c_str());
    }
}

