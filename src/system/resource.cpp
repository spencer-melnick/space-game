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

size_t ResourceHandle::getNumberElements() const
{
    if (isValid())
        return _resource->elements;

    return 0;
}

bool ResourceBuffer::initialize(const size_t bytes, const size_t maxElements)
{
    bool result = _mainbuffer.initialize(bytes);

    if (result == true)
    {
        void* data = _mainbuffer.allocate(sizeof(HeaderPool::Object) * maxElements, alignof(HeaderPool::Object));

        if (data == nullptr)
        {
            _mainbuffer.destroy();
            return false;
        }

        _headers = new HeaderPool(static_cast<HeaderPool::Object*>(data), maxElements);
    }

    return result;
}

void ResourceBuffer::deallocateTo(const size_t marker)
{
    _mainbuffer.deallocate(marker);

    for (HeaderPool::Handle i = _headers->getUsedObjects(); i.isValid(); i = i.getNextObject())
    {
        auto j = i.getData();
        if (j->marker > marker)
        {
            auto deallocator = _deallocators.find(j->type);

            if (deallocator != _deallocators.end())
                deallocator->second(j->data, j->elements);

            j->marker = 0;
            j->type = ResourceType::INVALID;
            j->data = nullptr;
        }
    }

    auto i = _headers->getUsedObjects();
    while (i.isValid())
    {
        auto j = i.getData();
        if (j->references == 0 && j->data == nullptr)
        {
            auto temp = i;
            i = i.getNextObject();
            _headers->freeObject(temp);
        }
        else
        {
            i = i.getNextObject();
        }
    }
    //_headers.remove_if([](const ResourceHeader& resource){return (resource.references == 0 && resource.data == nullptr);});
}

void ResourceBuffer::reset()
{
    deallocateTo(0);
}

void ResourceBuffer::destroy()
{
    reset();
    _mainbuffer.destroy();
}

bool ResourceBuffer::isInitialized() const
{
    return _initialized;
}

size_t ResourceBuffer::getNumberResources() const
{
    size_t numElements = 0;

    for (HeaderPool::Handle i = _headers->getUsedObjects(); i.isValid(); i = i.getNextObject())
        numElements ++;

    return numElements;
}

ResourceHandle ResourceBuffer::getResource(const size_t gid)
{
    for (HeaderPool::Handle i = _headers->getUsedObjects(); i.isValid(); i = i.getNextObject())
    {
        auto j = i.getData();
        if (j->gid == gid)
            return ResourceHandle(j, this);
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
    for (HeaderPool::Handle i = _headers->getUsedObjects(); i.isValid(); i = i.getNextObject())
    {
        auto j = i.getData();

        if (j == &resource)
            _headers->freeObject(i);
    }
    //_headers.remove_if([&resource](const ResourceHeader& i){return &(resource) == &(i);});
}

ResourceHandle ResourceBuffer::pushHeader(const std::string& name, const ResourceType type,
                                            const size_t marker, const size_t number, void* data)
{
    if (data == nullptr)
        return ResourceHandle();

    HeaderPool::Handle handle = _headers->createObject();
    if (!handle.isValid())
        return ResourceHandle();

    auto header = handle.getData();

    header->gid = hashString(name);
    header->data = data;
    header->elements = number;
    header->marker = marker;
    header->type = type;

    return ResourceHandle(header, this);
}
