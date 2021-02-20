
#include "App.hpp"

int main()
{
    cli::App app;
    app.AddCmd("display", []() { std::cout << "display command" << std::endl; });
    app.AddCmd("clock", []() { std::cout << "clock command" << std::endl; });
    app.Run();
    return 0;
}