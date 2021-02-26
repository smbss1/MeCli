
#include "App.hpp"

int main()
{
    mecli::App app;
    app.AddCmd("display", "Display something", [](const std::vector<std::string>&) { std::cout << "display command" << std::endl; });
    app.AddCmd("clock", "Clock Something", [](const std::vector<std::string>&) { std::cout << "clock command" << std::endl; });
    app.Run();
    return 0;
}