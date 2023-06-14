#pragma once

#include <vector>

namespace spk
{
    template <typename TType>
    class Pool
    {
    private:
        using Container = std::vector<TType *>;
        Container _allocatedObjects;

    public:
        class Object
        {
            friend class Pool;

        private:
            Container *_source;
            size_t *_referenceCount;
            TType *_content;

            Object(Container *p_source, TType *p_content) : _source(p_source),
                                                            _content(p_content),
                                                            _referenceCount(new size_t(1))
            {
                
            }

        public:
            ~Object()
            {
                (*_referenceCount)--;

                if (*_referenceCount == 0)
                    _source->push_back(_content);
            }

            Object(const Object &p_other) : _source(p_other._source),
                                            _content(p_other._content),
                                            _referenceCount(p_other._referenceCount)
            {
                (*_referenceCount)++;
            }

            TType *operator()()
            {
                return (_content);
            }
            const TType *operator()() const
            {
                return (_content);
            }

            TType *operator->()
            {
                return (_content);
            }
            const TType *operator->() const
            {
                return (_content);
            }

            TType &operator*()
            {
                return *_content;
            }

            const TType &operator*() const
            {
                return *_content;
            }
        };

    public:
        Pool()
        {
        }

        template <typename... Args>
        void reserve(size_t p_nbElement, Args &&...p_args)
        {
            size_t currentSize = _allocatedObjects.size();

            _allocatedObjects.resize(currentSize + p_nbElement);
            for (size_t i = currentSize; i < p_nbElement; i++)
            {
                _allocatedObjects[i] = new TType(std::forward<Args>(p_args)...);
            }
        }

        template <typename... Args>
        Object obtain(Args &&...p_args)
        {
            if (_allocatedObjects.size() == 0)
            {
                Object result = Object(&_allocatedObjects, new TType(std::forward<Args>(p_args)...));

                return (result);
            }
            else
            {
                TType *data = _allocatedObjects.back();
                _allocatedObjects.pop_back();

                *data = TType(std::forward<Args>(p_args)...);

                Object result = Object(&_allocatedObjects, data);
                return (result);
            }
        }

        size_t size() const
        {
            return (_allocatedObjects.size());
        }
    };
}