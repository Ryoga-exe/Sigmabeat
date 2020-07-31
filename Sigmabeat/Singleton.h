#pragma once

template <typename _T>
class Singleton {
public:
    static inline _T* Instance() {
        static _T inst;
        return &inst;
    };
protected:
    Singleton() {}
    virtual ~Singleton() {}
    Singleton(const Singleton& r) {}
    Singleton& operator=(const Singleton& r) {}
};