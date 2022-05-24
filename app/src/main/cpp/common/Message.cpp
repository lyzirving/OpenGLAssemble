//
// Created by liuyuzhou on 2022/5/24.
//
#include "Message.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Message"

MessageHandler::MessageHandler() = default;

MessageHandler::~MessageHandler() = default;

Message::Message() : mUptimeNano(0), mWhat(0), mHandler(nullptr), mNext(nullptr) {}

Message::Message(uint8_t what, const std::shared_ptr<MessageHandler> &handler)
        : mUptimeNano(0), mWhat(what), mHandler(handler), mNext(nullptr) {}

Message::~Message() {
    if (mHandler)
        mHandler.reset();
    if(mNext)
        mNext.reset();
}

