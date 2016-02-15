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

    return ResourceType::INVALID
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

    return nullptr;
}

bool ResourceManager::initialize(const size_t bytes)
{
    return _mainbuffer.initialize(bytes);
}

void ResourceManager::deallocate(const size_t marker)
{

}
