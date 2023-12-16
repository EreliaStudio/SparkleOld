#pragma once

#include <string>
#include <functional>
#include <thread>
#include "iostream/spk_iostream.hpp"

namespace spk
{
    /**
     * @class Thread
     * @brief A wrapper class around std::thread to manage application threads.
     *
     * This class provides a simplified interface for working with threads in the application.
     * It encapsulates a std::thread object and offers methods to configure, start, and join threads.
     * The Thread class is designed to be extended for specific threading tasks in the application.
     */
    class Thread
    {
    public:
        /**
         * @brief Static method to configure thread properties.
         * @param p_name The name to be assigned to the thread.
         */
        static void configure(const std::wstring& p_name);

    private:
        std::thread _innerThread; ///< Internal std::thread object.
        bool _isRunning; ///< Indicates whether the thread is currently running.

    public:
        /**
         * @brief Constructs a Thread with a specified function to execute.
         * @param p_name The name of the thread.
         * @param p_funct The function to be executed in this thread.
         */
        Thread(const std::wstring& p_name, const std::function<void()>& p_funct);

        /**
         * @brief Destructor for the Thread class.
         *
         * Ensures that the thread is properly joined or detached before destruction.
         */
        ~Thread();

        /**
         * @brief Joins the thread to the current context.
         *
         * Blocks the calling thread until the thread represented by this object completes its execution.
         */
        void join();
    };  
}