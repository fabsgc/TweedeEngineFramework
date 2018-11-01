#include "Utility/DynLibManager.h"
#include "Utility/DynLib.h"

namespace te
{
    static void dynlib_delete(DynLib* lib)
    {
        lib->Unload();
        te_delete(lib);
    }

    static bool operator<(const UPtr<DynLib>& lhs, const String& rhs)
    {
        return lhs->GetName() < rhs;
    }

    static bool operator<(const String& lhs, const UPtr<DynLib>& rhs)
    {
        return lhs < rhs->GetName();
    }

    static bool operator<(const UPtr<DynLib>& lhs, const UPtr<DynLib>& rhs)
    {
        return lhs->GetName() < rhs->GetName();
    }

    DynLib* DynLibManager::Load(String filename)
    {
        // Add the extension (.dll, .so, ...) if necessary.

        // Note: The string comparison here could be slightly more efficent by using a templatized string_concat function
        // for the lower_bound call and/or a custom comparitor that does comparison by parts.
        const String::size_type length = filename.length();
        const String extension = String(".") + DynLib::EXTENSION;
        const String::size_type extLength = extension.length();
        if (length <= extLength || filename.substr(length - extLength) != extension)
            filename.append(extension);

        if (DynLib::PREFIX != nullptr)
            filename.insert(0, DynLib::PREFIX);

        const auto& iterFind = mLoadedLibraries.lower_bound(filename);
        if (iterFind != mLoadedLibraries.end() && (*iterFind)->GetName() == filename)
        {
            return iterFind->get();
        }
        else
        {
            DynLib* newLib = te_new<DynLib>(std::move(filename));
            mLoadedLibraries.emplace_hint(iterFind, newLib, &dynlib_delete);
            return newLib;
        }
    }

    void DynLibManager::Unload(DynLib* lib)
    {
        const auto& iterFind = mLoadedLibraries.find(lib->GetName());
        if (iterFind != mLoadedLibraries.end())
        {
            mLoadedLibraries.erase(iterFind);
        }
        else
        {
            // Somehow a DynLib not owned by the manager...?
            // Well, we should clean it up anyway...
            dynlib_delete(lib);
        }
    }

    DynLibManager& gDynLibManager()
    {
        return DynLibManager::Instance();
    }
}
