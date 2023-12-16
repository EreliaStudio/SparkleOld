#pragma once

namespace spk
{
    /**
     * @brief A wrapper for pointers that ensures safer pointer operations.
     * 
     * SafePointer is a template class that provides a safer way to handle 
     * pointers in C++. It encapsulates a raw pointer and provides pointer-like 
     * operations.
	 * Users of SafePointer should not manage the object's lifecycle (e.g.,
	 * deleting the pointed-to object is not the responsibility of
	 * SafePointer users).
     * 
     * @tparam TType The type of the object being pointed to.
     */
    template<typename TType>
    class SafePointer
    {
    private:
        TType* _pointer; /**< Pointer to the managed object. */

    public:
        /**
         * @brief Constructs a SafePointer that encapsulates a raw pointer.
         * 
         * @param p_pointer The raw pointer to be managed.
         */
        SafePointer(TType* p_pointer) :
            _pointer(p_pointer)
        {
        }

        /**
         * @brief Copy constructor.
         * 
         * Constructs a new SafePointer that points to the same object as the 
         * other SafePointer.
         * 
         * @param p_other The SafePointer to copy from.
         */
        SafePointer(const SafePointer& p_other) :
            _pointer(p_other._pointer)
        {
        }

        /**
         * @brief Copy assignment operator.
         * 
         * Replaces the managed object with the object managed by the other 
         * SafePointer.
         * 
         * @param p_other The SafePointer to assign from.
         * @return Reference to this SafePointer.
         */
        SafePointer& operator=(const SafePointer& p_other) {
            _pointer = p_other._pointer;
            return *this;
        }

        /**
         * @brief Raw pointer assignment operator.
         * 
         * Replaces the managed object with the object pointed to by the raw pointer.
         * 
         * @param p_pointer The raw pointer to assign from.
         * @return Reference to this SafePointer.
         */
        SafePointer& operator=(TType* p_pointer) {
            _pointer = p_pointer;
            return *this;
        }

        /**
         * @brief Destructor.
         * 
         * Note that the destructor does not manage the lifecycle of the pointed-to
         * object.
         */
        ~SafePointer()
        {
        }

        /**
         * @brief Equality operator overloading.
         * 
         * Compares two SafePointers for equality. Two SafePointers are considered
         * equal if they point to the same object.
         * 
         * @param p_other The SafePointer to compare with.
         * @return true if the SafePointers are equal, false otherwise.
         */
        bool operator==(const SafePointer& p_other) const
        {
            return _pointer == p_other._pointer;
        }

        /**
         * @brief Inequality operator overloading.
         * 
         * Compares two SafePointers for inequality. Two SafePointers are considered
         * unequal if they do not point to the same object.
         * 
         * @param p_other The SafePointer to compare with.
         * @return true if the SafePointers are unequal, false otherwise.
         */
        bool operator!=(const SafePointer& p_other) const
        {
            return !(*this == p_other);
        }

        /**
         * @brief Arrow operator overloading.
         * 
         * Provides direct access to the member functions of the pointed-to object.
         * 
         * @return Pointer to the managed object.
         */
        TType* operator->() const
        {
            return _pointer;
        }

        /**
         * @brief Dereference operator overloading.
         * 
         * Provides access to the pointed-to object itself.
         * 
         * @return Reference to the managed object.
         */
        TType& operator*() const
        {
            return *_pointer;
        }

        /**
         * @brief Checks if the SafePointer is null.
         * 
         * @return true if the SafePointer is null, false otherwise.
         */
        bool isNullptr() const
        {
            return _pointer == nullptr;
        }

        /**
         * @brief Gets the raw pointer being managed.
         * 
         * @return Raw pointer to the managed object.
         */
        TType* get() const
        {
            return _pointer;
        }
    };
}
