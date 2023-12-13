#include "playground.hpp"

int main()
{
    spk::Application app(L"Playground", 900, false);
    app.setKeyboardLayout(spk::Keyboard::Layout::Qwerty);

    return (app.run());
}
