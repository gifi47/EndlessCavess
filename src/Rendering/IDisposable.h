#ifndef IDISPOSABLE_H
#define IDISPOSABLE_H

#ifdef DEBUG
#include <iostream>
#include <map>
#include <string>

extern std::map<long long, std::string> names;
extern int _____k;
#define THIS_NAME names[reinterpret_cast<long long>(this)]
#define NEW_NAME std::to_string(_____k++)
#endif //DEBUG

class IDisposable {
protected:
    int* ref_count;

    void release() {

        #ifdef DEBUG
        std::cout << "Release obj: " << THIS_NAME << " ref_count->"<< (ref_count ? (*ref_count) : 0) << "\n";
        #endif

        if (ref_count && --(*ref_count) == 0) {
            delete ref_count;
            OnDispose();
        }
    }

    virtual void OnDispose() = 0;

public:
    IDisposable() : ref_count(new int(1)) {
        #ifdef DEBUG
            std::cout << "Def Constructor\n";
        #endif
    }

    IDisposable(const IDisposable& other) : ref_count(other.ref_count) {
        ++(*ref_count);
        #ifdef DEBUG
            std::cout << "Copy Constructor " << (*ref_count) << "\n";
        #endif
    }

    IDisposable(IDisposable&& other) noexcept 
        : ref_count(other.ref_count) {
        other.ref_count = nullptr;
        #ifdef DEBUG
            std::cout << "Move Constructor " << (*ref_count) << "\n";
        #endif
    }

    IDisposable& operator=(const IDisposable& other) {
        if (this != &other) {
            release();
            ref_count = other.ref_count;
            ++(*ref_count);
        }
        #ifdef DEBUG
            std::cout << "Copy Operator= " << (*ref_count) << "\n";
        #endif
        return *this;
    }

    IDisposable& operator=(IDisposable&& other) noexcept {
        if (this != &other) {
            release();
            ref_count = other.ref_count;
            other.ref_count = nullptr;
        }
        #ifdef DEBUG
            std::cout << "Move Operator= " << (*ref_count) << "\n";
        #endif
        return *this;
    }

    ~IDisposable() {
        release();
    }
};

#endif