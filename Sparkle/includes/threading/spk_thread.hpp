#pragma once

#include <string>
#include <functional>
#include <thread>
#include "iostream/spk_iostream.hpp"

namespace spk
{
    class Thread
    {
    public:
        static void configure(const std::wstring& p_name);

    private:
        std::thread _innerThread;
        bool _isRunning;

    public:
        Thread(const std::wstring& p_name, const std::function<void()>& p_funct);
        ~Thread();
        void join();
    };
}