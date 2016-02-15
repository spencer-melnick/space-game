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
    class ResourceManager;

    enum class ResourceType
    {
        INVALID,
        TEXTURE,
        DATA
    };

    struct Resource
    {
        size_t marker = 0;
        size_t gid = 0;
        unsigned int references = 0;

        ResourceType type = ResourceType::INVALID;
        void* data = nullptr;
        ResourceManager* owner = nullptr;
    };

    class ResourceHandle
    {
        public:
            ResourceHandle();
            ResourceHandle(Resource* resource, ResourceManager* owner);
            ResourceHandle(const ResourceHandle& handle);
            ~ResourceHandle();

            ResourceType getType() const;
            bool isValid() const;

            void* getData() const;
            size_t getGid() const;

        protected:
            Resource* _resource;
            ResourceManager* _owner;
    };

    class ResourceManager
    {
        friend class ResourceHandle;

        public:
            bool initialize(const size_t bytes); //returns false on bad allocation or if already allocated

            template <typename T>
            ResourceHandle allocateResource(const std::string& filename, const ResourceType type, const T& resource);

            void deallocate(const size_t marker);
            void reset();

        protected:
            static size_t hashString(const std::string& filename);
            void clearHandle(const Resource& resource);

        private:
            StackAllocator _mainbuffer;
            std::forward_list<Resource> _headers;
    };


    template <typename T>
    ResourceHandle ResourceManager::allocateResource(const std::string& filename, const ResourceType type, const T& resource)
    {
        Resource header;
        header.gid = hashString(filename);

        header.data = _mainbuffer.allocate(resource);

        if (header.data == nullptr)
            return ResourceHandle();

        header.marker = _mainbuffer.getMarker();
        header.type = type;

        _headers.push_front(header);
        Resource* bufferedHeader = &(_headers.front());

        return ResourceHandle(bufferedHeader, this);
    }
}
