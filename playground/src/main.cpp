#include "playground.hpp"

class CallbackContainer
{
public:
    using Container = std::list<std::function<void()>>;

private:
    Container _callbacks;

public:
    class Contract {
        friend class CallbackContainer;

    private:
        Container::iterator _iterator;
        CallbackContainer::Container* _container;
        bool _isOwner;

        Contract(Container::iterator p_iterator, CallbackContainer::Container* p_container) :
            _iterator(p_iterator),
            _container(p_container),
            _isOwner(true)
        {

        }

    public:
        Contract() :
            _iterator(),
            _container(nullptr),
            _isOwner(false)
        {

        }

        Contract(const Contract&) = delete;
        Contract& operator=(const Contract&) = delete;

        Contract(Contract&& other) noexcept :
            _iterator(other._iterator),
            _container(other._container),
            _isOwner(other._isOwner)
        {
            other._isOwner = false;
            other._container = nullptr;
        }

        Contract& operator=(Contract&& other) noexcept
        {
            if (this != &other) {
                resign();
                _iterator = other._iterator;
                _container = other._container;
                _isOwner = other._isOwner;

                other._isOwner = false;
                other._container = nullptr;
            }
            return *this;
        }

        ~Contract()
        {
            resign();
        }

        void amend(std::function<void()> p_newCallback)
        {
            if (_container) {
                *_iterator = p_newCallback;
            }
        }

        void resign()
        {
            if (_isOwner && _container) {
                _container->erase(_iterator);
                _isOwner = false;
            }
        }
    };

    Contract subscribe(std::function<void()> p_callback)
    {
        _callbacks.push_back(p_callback);

        return (std::move(Contract(--(_callbacks.end()), &_callbacks)));
    }

    void notify() const
    {
        for (auto& callback : _callbacks) {
            callback();
        }
    }
};

void preRun(CallbackContainer& p_callbackContainer)
{
    spk::cout << "--- --- --- Run Preparation" << std::endl;
    p_callbackContainer.notify();
}

void runA(CallbackContainer& p_callbackContainer)
{
    spk::cout << "--- --- --- Run A" << std::endl;
    CallbackContainer::Contract contractA = p_callbackContainer.subscribe([](){ spk::cout << "Callback RunA.1" << std::endl;});
    p_callbackContainer.notify();
}

void runB(CallbackContainer& p_callbackContainer)
{
    spk::cout << "--- --- --- Run B" << std::endl;
    CallbackContainer::Contract contractB1 = p_callbackContainer.subscribe([](){ spk::cout << "Callback RunB.1" << std::endl;});
    CallbackContainer::Contract contractB2 = p_callbackContainer.subscribe([](){ spk::cout << "Callback RunB.2" << std::endl;});
    p_callbackContainer.notify();
}

void runC(CallbackContainer& p_callbackContainer)
{
    spk::cout << "--- --- --- Run C" << std::endl;
    CallbackContainer runCContainer;
    CallbackContainer::Contract contractC1 = p_callbackContainer.subscribe([](){ spk::cout << "Callback RunC.1" << std::endl;});
    CallbackContainer::Contract contractCOther1 = p_callbackContainer.subscribe([](){ spk::cout << "Callback RunC Other 1" << std::endl;});

    p_callbackContainer.notify();
    runCContainer.notify();
}

void postRun(CallbackContainer& p_callbackContainer)
{
    spk::cout << "--- --- --- Run Closure" << std::endl;
    p_callbackContainer.notify();
}

int main()
{
    CallbackContainer callbackContainer;

    callbackContainer.subscribe([](){spk::cout << "Should never be printed" << std::endl;});
    auto contractAlways = callbackContainer.subscribe([](){spk::cout << "Should always be printed" << std::endl;});
    preRun(callbackContainer);
    spk::cout << "--- --- --- --- ---" << std::endl << std::endl;
    runA(callbackContainer);
    spk::cout << "--- --- --- --- ---" << std::endl << std::endl;
    runB(callbackContainer);
    spk::cout << "--- --- --- --- ---" << std::endl << std::endl;
    runC(callbackContainer);
    spk::cout << "--- --- --- --- ---" << std::endl << std::endl;
    postRun(callbackContainer);
    spk::cout << "--- --- --- --- ---" << std::endl << std::endl;

    return 0;
}