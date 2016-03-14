#pragma once

namespace Game
{
    template <typename T>
    class ObjectPool
    {
        public:
            struct Object
            {
                T data;
                Object* nextFree = nullptr;
                Object* nextUsed = nullptr;
                bool used = false;
            };

            class Handle
            {
                friend class ObjectPool;

                public:
                    Handle(Object* object) : _object(object)
                        {};
                    Handle() : _object(nullptr)
                        {};

                    bool isValid() const
                    {
                        if (_object == nullptr)
                            return false;
                        return _object->used;
                    };

                    T* getData() const
                    {
                        if (isValid())
                            return &(_object->data);
                        return nullptr;
                    }

                    Handle getNextObject() const
                    {
                        if (isValid())
                            if (_object->nextUsed != nullptr)
                                return Handle(_object->nextUsed);

                        return Handle();
                    };

                protected:
                    Object* _object;
            };

            ObjectPool(Object* data, size_t numObjects) : _objects(data), _numObjects(numObjects)
            {
                _firstFree = &(_objects[0]);

                for (size_t i = 0; i < _numObjects - 1; i ++)
                    _objects[i].nextFree = &(_objects[i + 1]);
            }

            Handle createObject()
            {
                if (_firstFree == nullptr)
                    return Handle();

                //pop the front of the free list
                Object* temp = _firstFree;
                _firstFree = temp->nextFree;

                //add to the front of the used list
                temp->nextUsed = _firstUsed;
                _firstUsed = temp;

                temp->used = true;

                return Handle(temp);
            }

            void freeObject(Handle& handle)
            {
                if (handle.isValid())
                {
                    Object* temp = handle._object;

                    //add to the free list
                    temp->nextFree = _firstFree;
                    _firstFree = temp;

                    //remove from used
                    if (temp->used)
                    {
                        if (_firstUsed == temp)
                        {
                            _firstUsed = temp->nextUsed;
                            temp->nextUsed = nullptr;
                        }
                        else
                        {
                            Object* i = _firstUsed;
                            Object* next;
                            while (i != nullptr)
                            {
                                next = i->nextUsed;
                                if (next == temp)
                                {
                                    i->nextUsed = temp->nextUsed;
                                    temp->nextUsed = nullptr;
                                    break;
                                }
                                i = i->nextUsed;
                            }
                        }

                        temp->used = false;
                    }
                }
            }

            Handle getUsedObjects() const
            {
                if (_firstUsed == nullptr)
                    return Handle();
                return Handle(_firstUsed);
            }

        private:
            Object* _objects;
            size_t _numObjects;
            Object* _firstFree = nullptr;
            Object* _firstUsed = nullptr;
    };
}
