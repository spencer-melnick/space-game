#include "stackallocator.h"

using namespace Game;

bool StackAllocator::initialize(const size_t bytes)
{
    if (_buffer != nullptr || _size != 0)
        return false;

    _buffer = new(std::nothrow) char[bytes];

    if (_buffer == nullptr)
        return false;

    _size = bytes;

    return true;
}

void* StackAllocator::allocate(const size_t bytes, const size_t alignment)
{
    void* buffer = static_cast<void*>(_buffer + _top);
    size_t space = _size - _top;

    void* data = std::align(alignment, bytes, buffer, space);
    if (data != nullptr)
    {
        size_t offset = (_size - _top) - space;
        _top += (offset + bytes);

        return data;
    }

    return nullptr;
}

size_t StackAllocator::getMarker() const
{
    return _top;
}

void StackAllocator::deallocate(const size_t marker)
{
    _top = marker;
}

void StackAllocator::reset()
{
    _top = 0;
}
