#pragma once

template <typename T>
class Singleton {
public:
    static inline T* Inst() {
        static T inst;
        return &inst;
    };
protected:
    Singleton() {}
    virtual ~Singleton() {}
    Singleton(const Singleton& r) {}
    Singleton& operator=(const Singleton& r) {}
};