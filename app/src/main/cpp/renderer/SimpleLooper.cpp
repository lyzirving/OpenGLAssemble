//
// Created by liuyuzhou on 2022/5/18.
//
#include <sys/epoll.h>
#include <sys/eventfd.h>
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

bool SimpleLooper::isValid() {
    return mWeakEventFd >= 0 && mEpollFd >= 0;
}

void SimpleLooper::loop() {
    if (!isValid()) {
        LogE("looper(%s) is not valid, return", mName.c_str());
        return;
    }
}

void SimpleLooper::pollOnce() {

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
