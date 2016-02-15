#pragma once

/*TODO: Make resource handle list using pool allocator:
Each resource handle is held in a pool allocator, and keeps number of
references to it - akin to smart pointers - when it's resource is
removed, then it is no longer valid, and when there are no more references
to the handle, it is freed */

#include <list>

#include "stackallocator.h"

namespace Game
{
    enum class ResourceType
    {
        INVALID,
        TEXTURE
    };

    struct Resource
    {
        size_t marker = 0;
        size_t gid = 0;
        unsigned int references = 0;

        ResourceType type = INVALID;
        void* data = nullptr;
        ResourceManager = nullptr;
    }

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
    ResourceHandle allocateResource(const std::string& filename, const ResourceType type, const T& resource)
    {
        Resource header;
        header.gid = hashString(filename);

        header.data = _mainbuffer.allocate(resource);

        if (header.data == nullptr)
            return ResourceHandle();

        header.marker = _mainbuffer.getMarker();
        header.type = type;

        _headers.push_front(header);

        return ResourceHandle(header, this);
    }
}
