#pragma once

/*TODO: make Resource::_headers use pool allocator (potentially)
 * make Resource::allocate check for duplicate GIDs
 * use templated ResourceHandles to make safer type casting?
*/

#include <forward_list>

#include "stackallocator.h"

#define KiB 1024
#define MiB 1024 * KiB

namespace Game
{
    class ResourceHandle;
    class ResourceBuffer;

    enum class ResourceType
    {
        INVALID,
        RAW_MEM,
        TEXTURE
    };

    struct ResourceHeader
    {
        size_t marker = 0;
        size_t gid = 0;
        unsigned int references = 0;

        ResourceType type = ResourceType::INVALID;
        void* data = nullptr;
        ResourceBuffer* owner = nullptr;
    };

    class ResourceHandle
    {
        public:
            ResourceHandle();
            ResourceHandle(ResourceHeader* resource, ResourceBuffer* owner);
            ResourceHandle(const ResourceHandle& handle);
            ~ResourceHandle();

            ResourceType getType() const;
            bool isValid() const;

            void* getData() const;
            size_t getGid() const;

        protected:
            ResourceHeader* _resource;
            ResourceBuffer* _owner;
    };

    class ResourceBuffer
    {
        friend class ResourceHandle;

        public:
            bool initialize(const size_t bytes); //returns false on bad allocation or if already allocated

            template <typename T>
            ResourceHandle allocateResource(const std::string& name, const ResourceType type, const T& resource);

            template <typename T>
            ResourceHandle allocateRawResources(const std::string& name, size_t number);
            //allocates an array of T as a single resource with a specific GID

            void deallocate(const size_t marker);
            void reset();

            size_t getNumberResources();

        protected:
            static size_t hashString(const std::string& name);
            void clearHandle(const ResourceHeader& resource);
            ResourceHandle pushHeader(const std::string& name, const ResourceType type, const size_t marker, void* data);

        private:
            StackAllocator _mainbuffer;
            std::forward_list<ResourceHeader> _headers;
    };


    template <typename T>
    ResourceHandle ResourceBuffer::allocateResource(const std::string& name, const ResourceType type, const T& resource)
    {
        void* data = _mainbuffer.allocate(resource);
        return pushHeader(name, type, _mainbuffer.getMarker(), data);
    }

    template <typename T>
    ResourceHandle ResourceBuffer::allocateRawResources(const std::string& name, size_t number)
    {
        void* data = _mainbuffer.allocate(sizeof(T) * number, alignof(T));
        return pushHeader(name, ResourceType::RAW_MEM, _mainbuffer.getMarker(), data);
    }
}
