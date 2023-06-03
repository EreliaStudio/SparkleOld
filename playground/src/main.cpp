#include "playground.hpp"
class TestActivity : public spk::Activity
{
private:
    int _counter;
    std::string _name;

public:
    TestActivity(const std::string& name, int counter) : _name(name), _counter(counter)
    {
    }

protected:
    void _onEnter() override
    {
        std::cout << "Entering " << _name << "..." << std::endl;
        _moveNextStep();
    }

    void _execute() override
    {
        std::cout << "Executing " << _name << "... counter[" << _counter << "]" << std::endl;

        if (_counter > 0)
        {
            --_counter;
        }
        else
        {
            _moveNextStep();
        }
    }

    void _onExit() override
    {
        std::cout << "Exiting " << _name << "..." << std::endl;
        _moveNextStep();
    }
};

int main()
{
    spk::ActivityScheduler activityScheduler;

    // Créer et ajouter plusieurs activités de test au gestionnaire d'activités
    TestActivity* testActivity1 = new TestActivity("TestActivity1", 3);
    activityScheduler.addActivity(testActivity1);

    TestActivity* testActivity2 = new TestActivity("TestActivity2", 5);
    activityScheduler.addActivity(testActivity2);

    TestActivity* testActivity3 = new TestActivity("TestActivity3", 2);
    activityScheduler.addActivity(testActivity3);

    // Mettre à jour le gestionnaire d'activités jusqu'à ce que toutes les activités soient complétées
    while (true)
    {
        std::cout << " --- UPDATE ---" << std::endl;
        activityScheduler.execute();

        if (testActivity1->isCompleted() && testActivity2->isCompleted() && testActivity3->isCompleted())
        {
            std::cout << "All TestActivities completed successfully!" << std::endl;
            break;
        }
    }

    // Libérer la mémoire
    delete testActivity1;
    delete testActivity2;
    delete testActivity3;

    return 0;
}