#include "resource.h"

using namespace Game;

std::hash<std::string>& getHash()
{
    static std::hash<std::string> _hash;
    return _hash;
}

ResourceHandle::ResourceHandle() :
    _resource(nullptr), _owner(nullptr)
{}

ResourceHandle::ResourceHandle(ResourceHeader* resource, ResourceBuffer* owner) :
    _owner(owner)
{
    resource->references ++;
    _resource = resource;
}

ResourceHandle::ResourceHandle(const ResourceHandle& handle) :
    ResourceHandle(handle._resource, handle._owner)
{}

ResourceHandle::~ResourceHandle()
{
    if (_resource != nullptr)
    {
        _resource->references --;

        if (_resource->references == 0 && _resource->data == nullptr && _owner != nullptr)
            _owner->clearHandle(*_resource);
    }
}

ResourceType ResourceHandle::getType() const
{
    if (_resource != nullptr)
        return _resource->type;

    return ResourceType::INVALID;
}

bool ResourceHandle::isValid() const
{
    return (getType() != ResourceType::INVALID);
}

void* ResourceHandle::getData() const
{
    if (isValid())
        return _resource->data;

    return nullptr;
}

size_t ResourceHandle::getGid() const
{
    if (isValid())
        return _resource->gid;

    return 0;
}

size_t ResourceHandle::getMarker() const
{
    if (isValid())
        return _resource->marker;

    return 0;
}

bool ResourceBuffer::initialize(const size_t bytes)
{
    return _mainbuffer.initialize(bytes);
}

void ResourceBuffer::deallocateTo(const size_t marker)
{
    _mainbuffer.deallocate(marker);

    for (auto& i : _headers)
    {
        if (i.marker > marker)
        {
            i.marker = 0;
            i.type = ResourceType::INVALID;
            i.data = nullptr;
        }
    }

    _headers.remove_if([](const ResourceHeader& resource){return (resource.references == 0 && resource.data == nullptr);});
}

void ResourceBuffer::reset()
{
    deallocateTo(0);
}

size_t ResourceBuffer::getNumberResources()
{
    size_t numElements = 0;

    for (auto i = _headers.begin(); i != _headers.end(); i++)
        numElements ++;

    return numElements;
}

ResourceHandle ResourceBuffer::getResource(const size_t gid)
{
    for (auto& i : _headers)
    {
        if (i.gid == gid)
            return ResourceHandle(&i, this);
    }

    return ResourceHandle();
}

ResourceHandle ResourceBuffer::getResource(const std::string& name)
{
    return getResource(hashString(name));
}

size_t ResourceBuffer::hashString(const std::string& name)
{
    return getHash()(name);
}

void ResourceBuffer::clearHandle(const ResourceHeader& resource)
{
    _headers.remove_if([&resource](const ResourceHeader& i){return &(resource) == &(i);});
}

ResourceHandle ResourceBuffer::pushHeader(const std::string& name, const ResourceType type, const size_t marker, void* data)
{
    if (data == nullptr)
        return ResourceHandle();

    ResourceHeader header;

    header.gid = hashString(name);
    header.data = data;
    header.marker = marker;
    header.type = type;

    _headers.push_front(header);
    ResourceHeader* bufferedHeader = &(_headers.front());

    return ResourceHandle(bufferedHeader, this);
}
