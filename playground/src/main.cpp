#include "playground.hpp"

void generateInput(spk::Observer<int>& observer, std::string p_message)
{
    std::cout << " --- " << p_message << " ---" << std::endl;
    observer.notify(1);
    observer.notify(2);
    observer.notify(3);  
    std::cout << std::endl;
}

void generateScopedContract(spk::Observer<int>& observer)
{
    std::shared_ptr<spk::Observer<int>::Contract> contract10 = observer.subscribe(1, []() { std::cout << "Callback for event 1.2" << std::endl; });
    std::shared_ptr<spk::Observer<int>::Contract> contract30 = observer.subscribe(3, []() { std::cout << "Callback for event 3.0" << std::endl; });
    generateInput(observer, "Inside scope");
}

int main()
{
    spk::Observer<int> observer;

	DEBUG_LINE();
    // Subscribe to events
    std::shared_ptr<spk::Observer<int>::Contract> contract10 = observer.subscribe(1, []() { std::cout << "Callback for event 1.0" << std::endl; });
	DEBUG_LINE();
    std::shared_ptr<spk::Observer<int>::Contract> contract11 = observer.subscribe(1, []() { std::cout << "Callback for event 1.1" << std::endl; });
	DEBUG_LINE();
    std::shared_ptr<spk::Observer<int>::Contract> contract20 = observer.subscribe(2, []() { std::cout << "Callback for event 2.0" << std::endl; });

    // Notify events
    generateInput(observer, "At start");
        
    // Resign from events
    contract10->resign();

    generateInput(observer, "After resigning 1.0");

    contract11->edit([]() { std::cout << "Callback for event 1.1 - edited" << std::endl; });

    generateInput(observer, "After editing 1.1");

    generateScopedContract(observer);

    generateInput(observer, "Outside scope");

    return 0;
}
