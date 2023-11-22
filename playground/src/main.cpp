#include "playground.hpp"

int main()
{
    spk::Application app(L"Playground", 900);
    spk::Keyboard::instance()->setLayout(spk::Keyboard::Layout::Azerty);
    app.setMaxFPS(120);

    return (app.run());
}
