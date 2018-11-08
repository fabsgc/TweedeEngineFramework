#include "CoreApplication.h"

namespace te
{
    CoreApplication::CoreApplication()
    {
    }

    void CoreApplication::RunMainLoop()
    {
        while (_runMainLoop)
        {

        }
    }

    void CoreApplication::OnStartUp()
    {
        std::cout << "Started" << std::endl;
    }

    void CoreApplication::OnShutDown()
    {
        std::cout << "Shut down" << std::endl;
    }

    CoreApplication& gCoreApplication()
    {
        return CoreApplication::Instance();
    }
}