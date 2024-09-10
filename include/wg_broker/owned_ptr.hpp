#pragma once

#include <cassert>

namespace ussur {
namespace wg {


template<typename T>
class OwnedPtr {
public:
    // Constructor that takes a raw pointer and a freeing function
    OwnedPtr(T* ptr, void(*freeFunc)(T*)) : ptr_(ptr), freeFunc_(freeFunc) {
        assert(freeFunc_ && "Freeing function cannot be null");
    }

    // Move constructor
    OwnedPtr(OwnedPtr&& other) noexcept : ptr_(other.ptr_), freeFunc_(other.freeFunc_) {
        other.ptr_ = nullptr;
        other.freeFunc_ = nullptr;
    }

    // Move assignment operator
    OwnedPtr& operator=(OwnedPtr&& other) noexcept {
        if (this != &other) {
            // Clean up the existing resource
            reset();

            // Steal the resource and free function
            ptr_ = other.ptr_;
            freeFunc_ = other.freeFunc_;
            other.ptr_ = nullptr;
            other.freeFunc_ = nullptr;
        }
        return *this;
    }

    // Destructor calls the freeing function
    ~OwnedPtr() {
        reset();
    }

    // Dereference operator
    T& operator*() {
        assert(ptr_ && "Dereferencing a null pointer");
        return *ptr_;
    }

    // Arrow operator
    T* operator->() {
        assert(ptr_ && "Dereferencing a null pointer");
        return ptr_;
    }

    T* get() {
        return ptr_;
    }

    // Disable copy constructor and assignment
    OwnedPtr(const OwnedPtr&) = delete;
    OwnedPtr& operator=(const OwnedPtr&) = delete;

    // Manually release the pointer and free the resource
    void reset() {
        if (ptr_ && freeFunc_) {
            freeFunc_(ptr_);
            ptr_ = nullptr;
        }
    }

    // Get the raw pointer without releasing ownership
    T* get() const { return ptr_; }

private:
    T* ptr_;                    // The owned pointer
    void(*freeFunc_)(T*);        // The function to free the resource
};



} // namespace wg
} // namespace ussur