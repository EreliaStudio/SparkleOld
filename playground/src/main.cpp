#include "playground.hpp"

int main()
{
    // Création d'un pool de travailleurs avec 4 threads
    spk::WorkerPool<4> pool(L"Pool");

    // Ajoute des tâches à la queue du pool de travailleurs
    for (int i = 0; i < 1000; i++)
    {
        pool.addJob([i]()
        {
            spk::cout << L"Worker is handling job: " << i << std::endl;
        });
    }

    // Démarre le pool de travailleurs
    pool.start();

    // Ici on simule le reste du programme, pour être sûr que nos jobs ont le temps de s'executer
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Arrête le pool de travailleurs (et attend qu'ils finissent leurs tâches en cours)
    pool.stop();

    return 0;
}