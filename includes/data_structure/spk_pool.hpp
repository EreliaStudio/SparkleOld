#pragma once
#include <deque>
#include <memory>
#include <functional>
#include <mutex>

namespace spk
{
    /**
     * @brief A template class for object pooling.
     * 
     * This class implements an object pool pattern. It is useful for managing a collection of reusable 
     * objects of type TType. Object pools can improve performance in situations where the cost of 
     * initializing an instance of the class is high, the rate of instantiation of a class is high, and 
     * the number of instantiations in use at any one time is low.
     * 
     * @tparam TType The type of object to pool.
     */
    template<typename TType>
    class Pool {
    public:
        /**
         * @brief Type definition for the destructor function.
         */
        using Destructor = typename std::function<void(TType* p_toReturn)>;
        
        /**
         * @brief Type definition for a unique pointer to the pooled object with a custom destructor.
         */
        using Object = typename std::unique_ptr<TType, Destructor>;

    private:
        std::recursive_mutex _mutex; ///< Mutex for thread-safe access to the pool.
        std::deque<std::unique_ptr<TType>> _preallocatedElements; ///< Queue of preallocated pooled objects.

        /**
         * @brief Lambda function that acts as a custom destructor for pooled objects.
         * 
         * This function is used to release the object back into the pool when the unique pointer
         * goes out of scope.
         */
        const Destructor _destructorLambda = [&](TType* p_toReturn){
            _release(p_toReturn);
        };

        /**
         * @brief Releases an object back into the pool.
         * 
         * @param p_toReturn The object to be returned to the pool.
         */
        void _release(TType* p_toReturn)
        {
            std::lock_guard<std::recursive_mutex> lock(_mutex);
            _preallocatedElements.push_back(std::unique_ptr<TType>(p_toReturn));
        }

    public:
        /**
         * @brief Constructor for the Pool class.
         * 
         * Initializes the pool with a specified number of preallocated elements.
         * 
         * @param p_poolSize Initial size of the pool.
         */
        Pool(const size_t& p_poolSize = 0)
        {
            resize(p_poolSize);
        }

        /**
         * @brief Resizes the pool to a new size.
         * 
         * @param p_newSize The new size of the pool.
         */
        void resize(size_t p_newSize) {
            std::lock_guard<std::recursive_mutex> lock(_mutex);
            while (_preallocatedElements.size() > p_newSize) {
                _preallocatedElements.pop_back();
            }
            while (_preallocatedElements.size() < p_newSize) {
                _preallocatedElements.push_front(std::make_unique<TType>());
            }
        }

        /**
         * @brief Obtains an object from the pool.
         * 
         * If the pool is empty, a new object is created and added to the pool.
         * 
         * @return Object A unique pointer to the pooled object.
         */
        Object obtain() {
            std::lock_guard<std::recursive_mutex> lock(_mutex);
            
            if (_preallocatedElements.empty())
            {
                _preallocatedElements.push_front(std::make_unique<TType>());
            }

            Object item(_preallocatedElements.front().release(), _destructorLambda);

            _preallocatedElements.pop_front();

            return std::move(item);
        }
    };
}
