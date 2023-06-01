#include "playground.hpp"

void activeCallback() {
    std::cout << "The object is active." << std::endl;
}

void inactiveCallback() {
    std::cout << "The object is inactive." << std::endl;
}

int main() {
    // Création d'un ActivableObject
    spk::ActivableObject activable;

    // Ajout de callbacks pour les états actif et inactif
    auto activeContract = activable.addActivationCallback(activeCallback);
    auto inactiveContract = activable.addDeactivationCallback(inactiveCallback);

    // Activation de l'objet, déclenche le callback actif
    activable.activate();

    // Vérification de l'état de l'objet
    std::cout << "Is active: " << (activable.isActive() ? "true" : "false") << std::endl;

    // Désactivation de l'objet, déclenche le callback inactif
    activable.deactivate();

    // Vérification de l'état de l'objet
    std::cout << "Is active: " << (activable.isActive() ? "true" : "false") << std::endl;

    // Tentative de modification d'un contrat (doit échouer)
    try {
        activeContract.edit([]() { std::cout << "Modified callback." << std::endl; });
    } catch (const std::runtime_error &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Démission d'un contrat
    activeContract.resign();

    // Activation de l'objet, ne devrait pas déclencher le callback actif
    activable.activate();

    // Tentative de démission d'un contrat déjà démissionné (doit échouer)
    try {
        activeContract.resign();
    } catch (const std::runtime_error &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
