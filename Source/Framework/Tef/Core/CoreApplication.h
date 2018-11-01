#pragma once

#include "CorePrerequisites.h"
#include "Utility/Module.h"

namespace te
{
    class CoreApplication : Module<CoreApplication>
    {
        CoreApplication() {};
        ~CoreApplication() = default;
    };
}