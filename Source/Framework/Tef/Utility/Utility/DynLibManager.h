#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Utility/Module.h"

namespace te
{
    class DynLib;

     /**
      * This manager keeps track of all the open dynamic-loading libraries, it manages opening them opens them and can be
      * used to lookup already already-open libraries.
      *
      * @note	Not thread safe.
      */
    class TE_UTILITY_EXPORT DynLibManager : public Module<DynLibManager>
    {
    public:
        DynLib* Load(String name);
        void Unload(DynLib* lib);

    protected:
        Set<UPtr<DynLib>, std::less<>> mLoadedLibraries;
    };

    /** Easy way of accessing DynLibManager. */
    TE_UTILITY_EXPORT DynLibManager& gDynLibManager();
}
