#include "threading/spk_thread.hpp"
#include "application/spk_application.hpp"

namespace spk
{
    void Thread::configure(const std::wstring& p_name)
    {
        spk::cout.setPrefix(p_name);
        spk::cerr.setPrefix(p_name);
    }

    Thread::Thread(const std::wstring& p_name, const std::function<void()>& p_funct) :
        _isRunning(true),
        _innerThread([&, p_funct](std::wstring p_name, spk::Application* p_application){
			if (p_application != nullptr)
                p_application->setAsInstance();
            configure(p_name);
            p_funct();
        }, p_name, spk::Application::instance())
    {
    }
    
    Thread::~Thread()
    {
        join();
    }

    void Thread::join()
    {
        if (_isRunning == false)
            return ;

        while (_innerThread.joinable() == false);

        if (_innerThread.joinable())
        {
            _innerThread.join();
        }
        
        _isRunning = false;
    }
}