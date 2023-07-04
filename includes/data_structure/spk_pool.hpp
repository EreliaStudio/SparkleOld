#pragma once

#include <vector>

namespace spk
{
    /**
     * \class Pool
     * \brief A generic pool class that manages the creation and deletion of objects.
     *
     * This class is designed to handle memory in an efficient manner by reusing
     * previously allocated objects. This avoids frequent memory allocation and
     * deallocation, which can be costly for performance.
     *
     * The Pool class provides a mechanism for 'reserving' a number of objects of a
     * specific type, and then 'obtaining' those objects for use. When an object is
     * no longer needed, it is returned to the pool to be reused later.
     *
     * \tparam TType The type of objects that the pool will manage.
     */
    template <typename TType>
    class Pool
    {
    private:
        /**
         * \brief The container that holds the objects allocated by the pool.
         */
        using Container = std::vector<TType *>;
        Container _allocatedObjects;

    public:
        /**
         * \class Object
         * \brief An internal class to manage objects retrieved from the pool.
         *
         * This class is designed to keep track of the memory it manages and to
         * automatically return it to the pool when it is no longer needed.
         */
        class Object
        {
            friend class Pool;

        private:
            
            Container *_source;
            size_t *_referenceCount;
            TType *_content;

            /**
             * @brief Private constructor for the Object class.
             *
             * The private constructor is only accessible by the `Pool` class.
             * It is used to create an `Object` instance that manages a specific content object.
             *
             * @param p_source Pointer to the container of allocated objects.
             * @param p_content Pointer to the content object managed by the `Object` instance.
             */
            Object(Container *p_source, TType *p_content) : _source(p_source),
                                                            _content(p_content),
                                                            _referenceCount(new size_t(1))
            {
            }

        public:
            /**
             * @brief Destructor for the Object class.
             *
             * The destructor decrements the reference count and if it reaches zero,
             * the content of the object is returned to the pool.
             */
            ~Object()
            {
                (*_referenceCount)--;

                if (*_referenceCount == 0)
                    _source->push_back(_content);
            }

            /**
             * @brief Copy constructor for the Object class.
             *
             * The copy constructor increases the reference count.
             *
             * @param p_other The Object to copy.
             */
            Object(const Object &p_other) : _source(p_other._source),
                                            _content(p_other._content),
                                            _referenceCount(p_other._referenceCount)
            {
                (*_referenceCount)++;
            }

            /**
             * @brief Function call operator.
             *
             * Allows accessing the content of the object using the function call syntax.
             *
             * @return A pointer to the content.
             */
            TType *operator()()
            {
                return (_content);
            }

            /**
             * @brief Const version of the function call operator.
             *
             * Allows accessing the content of the object using the function call syntax,
             * preserving constness.
             *
             * @return A const pointer to the content.
             */
            const TType *operator()() const
            {
                return (_content);
            }

            /**
             * @brief Member access operator.
             *
             * Allows accessing members of the content using the arrow operator.
             *
             * @return A pointer to the content.
             */
            TType *operator->()
            {
                return (_content);
            }

            /**
             * @brief Const version of the member access operator.
             *
             * Allows accessing members of the content using the arrow operator,
             * preserving constness.
             *
             * @return A const pointer to the content.
             */
            const TType *operator->() const
            {
                return (_content);
            }

            /**
             * @brief Dereference operator.
             *
             * Allows dereferencing the content using the dereference operator.
             *
             * @return A reference to the content.
             */
            TType &operator*()
            {
                return *_content;
            }

            /**
             * @brief Const version of the dereference operator.
             *
             * Allows dereferencing the content using the dereference operator,
             * preserving constness.
             *
             * @return A const reference to the content.
             */
            const TType &operator*() const
            {
                return *_content;
            }
        };

    public:
        /**
         * @brief Default constructor for the Pool class.
         */
        Pool()
        {
        }

        /**
         * @brief Reserve a number of elements in the pool.
         *
         * This function reserves a number of elements in the pool by creating new instances
         * of the template type with the provided arguments.
         *
         * @tparam Args The types of the arguments.
         * @param p_nbElement The number of elements to reserve.
         * @param p_args The arguments to construct the elements.
         */
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

        /**
         * @brief Obtain an object from the pool.
         *
         * This function obtains an object from the pool. If there are no available objects,
         * a new object is created using the provided arguments.
         *
         * @tparam Args The types of the arguments.
         * @param p_args The arguments to construct the object if necessary.
         * @return An Object that manages the obtained object.
         */
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

        /**
         * @brief Get the size of the pool.
         *
         * @return The number of objects in the pool.
         */
        size_t size() const
        {
            return (_allocatedObjects.size());
        }
    };
}
