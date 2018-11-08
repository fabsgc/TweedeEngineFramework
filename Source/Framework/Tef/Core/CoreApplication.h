#pragma once

#include "CorePrerequisites.h"
#include "Utility/Module.h"

namespace te
{
    class TE_CORE_EXPORT CoreApplication : public Module<CoreApplication>
    {
    public:
        CoreApplication();
        ~CoreApplication() = default;

        void RunMainLoop();

    protected:
        void OnStartUp() override;
        void OnShutDown() override;

    protected:
        volatile bool _runMainLoop;
    };

    /**	Provides easy access to CoreApplication. */
    TE_CORE_EXPORT CoreApplication& gCoreApplication();
}