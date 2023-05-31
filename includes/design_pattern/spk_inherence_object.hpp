#pragma once 

#include <memory>
#include <vector>
#include <functional>

namespace spk
{
    template <typename TType>
    class InherenceObject
    {
        public:
            using Parent = TType *;
            using Child = TType *;
            using Callback = std::function<void(Child)>;

        private:
            Parent _parent;
            std::vector<Child> _childrens;

            Callback _birthCallback;
            Callback _deathCallback;

            void _addChild(Child child)
            {
                _childrens.push_back(child);
                if (_birthCallback != nullptr)
                    _birthCallback(child);
            }

            void _removeChild(Child child)
            {
                if (_deathCallback != nullptr)
                    _deathCallback(child);
                for (auto it = _childrens.begin(); it != _childrens.end(); ++it)
                {
                    if (*it == child)
                    {
                        _childrens.erase(it);
                        break;
                    }
                }
            }

        public:
            InherenceObject() :
                _parent(nullptr),
                _childrens(),
                _birthCallback(nullptr),
                _deathCallback(nullptr)
            {
                
            }

            ~InherenceObject()
            {
                if (_parent != nullptr)
                    _parent->_removeChild(static_cast<Child>(this));
                for (auto child : _childrens)
                    child->_parent = nullptr;
            }
            
            void setParent(Parent parent)
            {
                if (_parent != nullptr)
                    _parent->_removeChild(static_cast<Child>(this));

                _parent = parent;
                
                if (_parent != nullptr)
                    _parent->_addChild(static_cast<Child>(this));
            }

            void addChild(Child child)
            {
                child->setParent(static_cast<Parent>(this));
            }

            void setBirthCallback(std::function<void(Child)> callback)
            {
                _birthCallback = callback;
            }

            void setDeathCallback(std::function<void(Child)> callback)
            {
                _deathCallback = callback;
            }

            constexpr const Parent &parent() const
            {
                return _parent;
            }

            constexpr const std::vector<Child> &childrens() const
            {
                return _childrens;
            }

            InherenceObject(const InherenceObject &) = delete;
            InherenceObject &operator=(const InherenceObject &) = delete;
    };
}