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

ResourceHandle::ResourceHandle(Resource* resource, ResourceManager* owner) :
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

        if (_resource->references == 0 && _owner != nullptr)
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

bool ResourceManager::initialize(const size_t bytes)
{
    return _mainbuffer.initialize(bytes);
}

void ResourceManager::deallocate(const size_t marker)
{
    _mainbuffer.deallocate(marker);

    for (auto i = _headers.begin(); i != _headers.end();  i++)
    {
        if (i->marker > marker)
        {
            i->marker = 0;
            i->type = ResourceType::INVALID;
            i->data = nullptr;
        }
    }

    _headers.remove_if([](const Resource& resource){return (resource.references == 0 && resource.data == nullptr);});
}

void ResourceManager::reset()
{
    deallocate(0);
}

size_t ResourceManager::hashString(const std::string& filename)
{
    return getHash()(filename);
}

void ResourceManager::clearHandle(const Resource& resource)
{
    _headers.remove_if([&resource](const Resource& i){return &(resource) == &(i);});
}
