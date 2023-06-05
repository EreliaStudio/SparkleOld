#pragma once

#include <thread>
#include <functional>
#include <string>
#include <future>

#include "iostream/spk_iostream.hpp"

namespace spk
{
    class Thread
    {
    public:
        enum class LaunchMethod
        {
            Delayed,
            Immediate
        };

    private:
        std::function<void()> _funct;
        std::thread _thread;
        std::promise<bool> _starterSignal;

    public:
        template <typename Funct, typename... Args>
        Thread(std::wstring p_threadName, Funct &&p_func, Args &&...p_args) :
            _funct(std::bind(std::forward<Funct>(p_func), std::forward<Args>(p_args)...)),
            _starterSignal()
        {
            auto wrapper = [this](std::wstring threadName)
            {
                spk::cout.setPrefix(threadName);
                spk::cerr.setPrefix(threadName);
                _starterSignal.get_future().wait();
                _funct();
            };
            _thread = std::thread(wrapper(p_threadName));
        }

        template <typename Funct, typename... Args>
        Thread(LaunchMethod p_launchMethod, std::wstring p_threadName, Funct &&p_func, Args &&...p_args) :
            Thread(p_threadName, p_func, p_args...)
        {
            if (p_launchMethod == LaunchMethod::Immediate)
            {
                start();
            }
        }

        ~Thread()
        {
            join();
        }

        std::thread::id getId() const
        {
            return (_thread.get_id());
        }

        void join()
        {
            if (_thread.joinable())
                _thread.join();
        }

        void start()
        {
            _starterSignal.set_value(true);
        }
    };
}
