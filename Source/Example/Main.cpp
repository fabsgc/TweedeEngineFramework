#include "CorePrerequisites.h"
#include "CoreApplication.h"
#include "Utility/Timer.h"

#include "Allocators/LinearAllocator.h"

using namespace te;

int CALLBACK WinMain(_In_  HINSTANCE hInstance, _In_  HINSTANCE hPrevInstance, _In_  LPSTR lpCmdLine, _In_  int nCmdShow)
{
    CoreApplication::StartUp();
    CoreApplication& app = gCoreApplication();

    app.RunMainLoop();
    app.ShutDown();

    return 0;
}