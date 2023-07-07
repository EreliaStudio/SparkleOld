#include "playground.hpp"

int main() {
    srand(time(NULL));
    // Création d'une instance de la classe Perlin
    spk::Perlin perlin(rand());

    // Configuration de quelques paramètres
    perlin.configureRange(0.0f, 100.0f);
    perlin.configureFrequency(10.0f);
    perlin.configurePersistance(0.5f);
    perlin.configureOctave(3);

    // Création d'un échantillon de valeurs
    for (int x = 0; x < 20; ++x) {
        for (int y = 0; y < 49; ++y) {
            // Appel de la méthode sample
            float sample = perlin.sample(static_cast<float>(x), static_cast<float>(y));

            if (y != 0)
                spk::cout << " ";
            if (sample > 50)
                spk::cout << "X";
            else
                spk::cout << ".";
        }
        spk::cout << std::endl;
    }
    spk::cout << std::endl;

    return (0);
}
