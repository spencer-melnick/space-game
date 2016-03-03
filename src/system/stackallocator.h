#pragma once

#include <memory>
#include <new>

namespace Game
{
    class StackAllocator
    {
        public:
            bool initialize(const size_t bytes); //returns false on bad allocation or if already allocated
            void* allocate(const size_t bytes, const size_t alignment);
            size_t getMarker() const; //returns the top of the buffer stack
            void deallocate(const size_t marker); //does not invalidate pointers to elements!
            void reset();
            void destroy();

            template <typename T>
            T* allocate();

            template <typename T>
            T* allocate(const T& val);

        protected:
            size_t alignedAddress(const void* address, const size_t alignment);

        private:
            char* _buffer = nullptr;
            size_t _size = 0;
            size_t _top = 0;
    };




    template <typename T>
    T* StackAllocator::allocate()
    {
        void* address = allocate(sizeof(T), alignof(T));

        if (address != nullptr)
        {
            T* data = new(address) T();

            if (data != nullptr)
                return data;
        }

        return nullptr;
    }

    template <typename T>
    T* StackAllocator::allocate(const T& val)
    {
        void* address = allocate(sizeof(T), alignof(T));

        if (address != nullptr)
        {
            T* data = new(address) T(val);

            if (data != nullptr)
                return data;
        }

        return nullptr;
    }
}
