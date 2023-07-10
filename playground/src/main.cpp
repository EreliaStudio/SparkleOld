#include "playground.hpp"
#include <iostream>

enum class States
{
    STATE1,
    STATE2,
    STATE3,
    UNKNOWN
};

int main()
{
    StateMachine<States> machine(States::STATE1);

    machine.setOnUnknowStateTransition([]() {
        std::cout << "Unknown state transition occurred!\n";
    });

    machine.setOnUnknowStateExecution([]() {
        std::cout << "Unknown state execution occurred!\n";
    });

    machine.setStateTransitionCallback(States::STATE1, States::STATE2, []() {
        std::cout << "Transition from STATE1 to STATE2\n";
    });

    machine.setStateExecutionCallback(States::STATE2, []() {
        std::cout << "Execution of STATE2\n";
    });

    // Here, it's up to you to test the transitions and executions
    machine.setState(States::STATE2); // should print "Transition from STATE1 to STATE2"
    machine.update(); // should print "Execution of STATE2"
    machine.setState(States::UNKNOWN); // should print "Unknown state transition occurred!"

    return 0;
}
