
#include "Application.h"

#include <iostream>
#include <exception>

int main()
{
    try
    {
        Application app;
        app.run();
    }
    catch (const std::exception& error)
    {
        std::cerr << "Application error: "
                  << error.what() << '\n';

        return 1;
    }

    return 0;
}
