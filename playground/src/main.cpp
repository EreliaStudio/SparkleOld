#include "playground.hpp"

void generateSignal(spk::Observer<int>& observer, std::string p_message)
{
    std::cout << " --- " << p_message << " ---" << std::endl;

    observer.notify(1);
    observer.notify(2);

    std::cout << std::endl << std::endl;
}

void test(spk::Observer<int>::Contract tmp)
{

}

void scopedSubscription(spk::Observer<int>& observer)
{
    spk::Observer<int>::Contract contractD = observer.subscribe(2, [&](){std::cout << "Event 2.1 notified ! - scoped" << std::endl;});

    generateSignal(observer, "Inside scoped subscription");
}

int main()
{
    spk::Observer<int> observer;

    spk::Observer<int>::Contract contractA = observer.subscribe(1, [&](){std::cout << "Event 1.0 notified !" << std::endl;});
    spk::Observer<int>::Contract contractB = observer.subscribe(1, [&](){std::cout << "Event 1.1 notified !" << std::endl;});
    spk::Observer<int>::Contract contractC = observer.subscribe(2, [&](){std::cout << "Event 2.0 notified !" << std::endl;});

    generateSignal(observer, "At start");

    contractB.resign();
    
    generateSignal(observer, "After resign");

    contractA.edit([&](){std::cout << "Event 1.0 notified ! - edited" << std::endl;});
    
    generateSignal(observer, "After edit");

    scopedSubscription(observer);
    
    generateSignal(observer, "After scope");

    return 0;
}
