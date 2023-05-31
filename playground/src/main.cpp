#include "playground.hpp"

enum class EventType
{
    Event1,
    Event2,
    Event3,
    Event4
};

class EventSubscriber
{
public:
    void onEvent1()
    {
        std::cout << "Event 1 received" << std::endl;
    }

    void onEvent2()
    {
        std::cout << "Event 2 received" << std::endl;
    }

    void onEvent3()
    {
        std::cout << "Event 3 received" << std::endl;
    }

    void onEvent4()
    {
        std::cout << "Event 4 received" << std::endl;
    }
};

spk::Observer<EventType> observer;

void funct()
{
    EventSubscriber subscriber;

    auto& contract4 = observer.subscribe(EventType::Event3, [&subscriber]() { subscriber.onEvent4(); });

    std::cout << " --- Launch notify ---" << std::endl << std::endl;
    observer.notify(EventType::Event1);
    observer.notify(EventType::Event2);
    observer.notify(EventType::Event3);
    observer.notify(EventType::Event4);
}

int main()
{
    EventSubscriber subscriber;

    // Subscribe to events
    auto& contract1 = observer.subscribe(EventType::Event1, [&subscriber]() { subscriber.onEvent1(); });
    auto& contract2 = observer.subscribe(EventType::Event2, [&subscriber]() { subscriber.onEvent2(); });
    auto& contract3 = observer.subscribe(EventType::Event3, [&subscriber]() { subscriber.onEvent3(); });

    // Notify events
    std::cout << " --- Launch notify ---" << std::endl << std::endl;
    observer.notify(EventType::Event1);
    observer.notify(EventType::Event2);
    observer.notify(EventType::Event3);
    observer.notify(EventType::Event4);

    funct();
    
    // Resign from events
    contract1.resign();
    contract2.resign();
    contract3.resign();


    // Notify events again (should not print anything)
    std::cout << " --- Launch notify ---" << std::endl << std::endl;
    observer.notify(EventType::Event1);
    observer.notify(EventType::Event2);
    observer.notify(EventType::Event3);
    observer.notify(EventType::Event4);

    return 0;
}
