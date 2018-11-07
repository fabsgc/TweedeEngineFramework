#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Utility/Module.h"

namespace te
{
    class TE_UTILITY_EXPORT Console : public Module<Console>
    {
    public:
        Console() {}
        ~Console() {}

    protected:
        void OnStartUp() override;
        void OnShutDown() override;

    protected:
        int _hCrtIn;
        int _hCrtOut;
    };

    Console& gConsole();
}