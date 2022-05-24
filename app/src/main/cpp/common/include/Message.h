//
// Created by liuyuzhou on 2022/5/24.
//
#ifndef OPENGLASSEMBLE_MESSAGE_H
#define OPENGLASSEMBLE_MESSAGE_H

#include <cstdint>
#include <memory>

class MessageHandler {
public:
    MessageHandler();
    ~MessageHandler();

    virtual void handleMessage(uint8_t what) = 0;
};

class Message {
public:
    Message();
    Message(uint8_t what, const std::shared_ptr<MessageHandler> &handler);
    ~Message();

    int64_t mUptimeNano;
    uint8_t mWhat;
    std::shared_ptr<MessageHandler> mHandler;
    std::shared_ptr<Message> mNext;
};

#endif //OPENGLASSEMBLE_MESSAGE_H
