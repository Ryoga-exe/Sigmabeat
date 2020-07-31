#pragma once

template <typename T>
class Singleton {
public:
    static inline T* Instance() {
        static T inst;
        return &inst;
    };
protected:
    Singleton() {}
    virtual ~Singleton() {}
    Singleton(const Singleton& r) {}
    Singleton& operator=(const Singleton& r) {}
};