#pragma once

namespace spk
{
    template<typename TType>
    class SafePointer
    {
    private:
        TType* _pointer;

    public:
        SafePointer(TType* p_pointer) :
            _pointer(p_pointer)
        {
        }

        SafePointer(const SafePointer& p_other) :
            _pointer(p_other._pointer)
        {
        }

        SafePointer& operator=(const SafePointer& p_other) {
            _pointer = p_other._pointer;
            return *this;
        }

        SafePointer& operator=(TType* p_pointer) {
            _pointer = p_pointer;
            return *this;
        }

        ~SafePointer()
        {
        }

        bool operator==(const SafePointer& p_other) const
        {
            return _pointer == p_other._pointer;
        }

        bool operator!=(const SafePointer& p_other) const
        {
            return !(*this == p_other);
        }

        TType* operator->() const
        {
            return _pointer;
        }

        TType& operator*() const
        {
            return *_pointer;
        }

        bool isNullptr() const
        {
            return _pointer == nullptr;
        }

        TType* get() const
        {
            return _pointer;
        }
    };
}
