//
// Created by liuyuzhou on 2022/5/18.
//
#ifndef OPENGLASSEMBLE_SIMPLELOOPER_H
#define OPENGLASSEMBLE_SIMPLELOOPER_H

#include <string>
#include <mutex>

class SimpleLooper {
public:
    SimpleLooper(const char* name);
    ~SimpleLooper();

    bool isValid();
    void loop();
private:
    std::string mName;
    int mWeakEventFd;
    int mEpollFd;
    int64_t mNextMsgTime;

    std::mutex mMutex;

    void createEpoll();
    void pollOnce();
    void release();
};

#endif //OPENGLASSEMBLE_SIMPLELOOPER_H
