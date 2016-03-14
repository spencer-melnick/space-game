#pragma once

/*TODO: make Resource::_headers use pool allocator (potentially)
 * make Resource::allocate check for duplicate GIDs
 */

/*IMPORTANT:
 * A resource struct must have a static ResourceType "resourceType" member variable
 * and a void "deallocate(void* data, size_t elements)" function
 */

#include <forward_list>
#include <map>
#include <functional>

#include "stackallocator.h"
#include "pool.h"

#define KiB 1024
#define MiB 1024 * KiB

namespace Game
{
    class ResourceHandle;
    class ResourceBuffer;

    enum class ResourceType
    {
        INVALID,
        TEXTURE
    };

    struct ResourceHeader
    {
        size_t marker = 0;
        size_t gid = 0;
        size_t elements = 1;
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
            size_t getMarker() const; //returns marker to after this resource
            size_t getNumberElements() const;

            template <typename T>
            T* getData();

        protected:
            ResourceHeader* _resource;
            ResourceBuffer* _owner;
    };

    class ResourceBuffer
    {
        friend class ResourceHandle;

        public:
            bool initialize(const size_t bytes, const size_t maxElements); //returns false on bad allocation or if already allocated

            template <typename t>
            ResourceHandle allocateResource(const std::string& name);

            template <typename T>
            ResourceHandle allocateResource(const std::string& name, const T& resource);

            template <typename T>
            ResourceHandle allocateResourceArray(const std::string& name, size_t number);
            //allocates an array of T as a single resource with a specific GID

            void deallocateTo(const size_t marker);
            void reset();
            void destroy();

            bool isInitialized() const;
            size_t getNumberResources() const;
            ResourceHandle getResource(const size_t gid);
            ResourceHandle getResource(const std::string& name);

        protected:
            using HeaderPool = ObjectPool<ResourceHeader>;

            static size_t hashString(const std::string& name);
            void clearHandle(const ResourceHeader& resource);

            ResourceHandle pushHeader(const std::string& name, const ResourceType type, const size_t marker,
                                        const size_t elements, void* data);

            template <typename T>
            void bindDeallocator();

        private:
            StackAllocator _mainbuffer;
            HeaderPool* _headers;
            bool _initialized;
            std::map<ResourceType, std::function<void(void*, size_t)> > _deallocators;
    };


    template <typename T>
    T* ResourceHandle::getData()
    {
        if (isValid())
            if (T::resourceType == _resource->type)
                return static_cast<T*>(_resource->data);

        return nullptr;
    }

    template <typename T>
    ResourceHandle ResourceBuffer::allocateResource(const std::string& name)
    {
        bindDeallocator<T>();

        if (!getResource(name).isValid())
        {
            void* data = _mainbuffer.allocate(sizeof(T), alignof(T));
            return pushHeader(name, T::resourceType, _mainbuffer.getMarker(), 1, data);
        }

        return ResourceHandle();
    }

    template <typename T>
    ResourceHandle ResourceBuffer::allocateResource(const std::string& name, const T& resource)
    {
        bindDeallocator<T>();

        if (!getResource(name).isValid())
        {
            void* data = _mainbuffer.allocate(resource);
            return pushHeader(name, T::resourceType, _mainbuffer.getMarker(), 1, data);
        }

        return ResourceHandle();
    }

    template <typename T>
    ResourceHandle ResourceBuffer::allocateResourceArray(const std::string& name, size_t number)
    {
        bindDeallocator<T>();

        if (!getResource(name).isValid())
        {
            void* data = _mainbuffer.allocate(sizeof(T) * number, alignof(T));
            return pushHeader(name, T::resourceType, _mainbuffer.getMarker(), number, data);
        }

        return ResourceHandle();
    }

    template <typename T>
    void ResourceBuffer::bindDeallocator()
    {
        if (_deallocators.find(T::resourceType) == _deallocators.end())
            _deallocators[T::resourceType] = &T::deallocate;
    }
}
