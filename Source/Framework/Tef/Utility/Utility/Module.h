#pragma once

#include "Prerequisites/PrerequisitesUtil.h"
#include "Error/Exception.h"

namespace te
{
     /**
      * Represents one engine module. Essentially it is a specialized type of singleton. Module must be manually started up
      * and shut down before and after use.
      */
    template <class T>
    class Module
    {
    public:
        /**
         * Returns a reference to the module instance. Module has to have been started up first otherwise an exception will
         * be thrown.
         */
        static T& Instance()
        {
            if (!IsStartedUp())
            {
                TE_EXCEPT(InternalErrorException,
                    "Trying to access a module but it hasn't been started up yet.");
            }

            if (IsDestroyed())
            {
                TE_EXCEPT(InternalErrorException,
                    "Trying to access a destroyed module.");
            }

            T* test = _instance();

            return *_instance();
        }

        /**
         * Returns a pointer to the module instance. Module has to have been started up first otherwise an exception will
         * be thrown.
         */
        static T* InstancePtr()
        {
            if (!IsStartedUp())
            {
                TE_EXCEPT(InternalErrorException, "Trying to access a module but it hasn't been started up yet.");
            }

            if (IsDestroyed())
            {
                TE_EXCEPT(InternalErrorException, "Trying to access a destroyed module.");
            }

            return _instance();
        }

        /** Constructs and starts the module using the specified parameters. */
        template<class ...Args>
        static void StartUp(Args &&...args)
        {
            if (IsStartedUp())
            {
                TE_EXCEPT(InternalErrorException, "Trying to start an already started module.");
            }

            _instance() = new T(std::forward<Args>(args)...);
            IsStartedUp() = true;

            ((Module*)_instance())->OnStartUp();
        }

        /**
         * Constructs and starts a specialized type of the module. Provided type must derive from type the Module is
         * initialized with.
         */
        template<class SubType, class ...Args>
        static void StartUp(Args &&...args)
        {
            static_assert(std::is_base_of<T, SubType>::value, "Provided type is not derived from type the Module is initialized with.");

            if (IsStartedUp())
            {
                TE_EXCEPT(InternalErrorException, "Trying to start an already started module.");
            }

            _instance() = new SubType(std::forward<Args>(args)...);
            IsStartedUp() = true;

            ((Module*)_instance())->OnStartUp();
        }

        /** Shuts down this module and frees any resources it is using. */
        static void ShutDown()
        {
            if (IsDestroyed())
            {
                TE_EXCEPT(InternalErrorException, "Trying to shut down an already shut down module.");
            }

            if (!IsStartedUp())
            {
                TE_EXCEPT(InternalErrorException, "Trying to shut down a module which was never started.");
            }

            ((Module*)_instance())->OnShutDown();

            te_delete(_instance());
            IsDestroyed() = true;
        }

        /** Query if the module has been started. */
        static bool IsStarted()
        {
            return IsStartedUp() && !IsDestroyed();
        }

    protected:
        Module() = default;

        virtual ~Module() = default;

        /*
         * The notion of copying or moving a singleton is rather nonsensical.
         */
        Module(Module&&) = delete;
        Module(const Module&) = delete;
        Module& operator=(Module&&) = delete;
        Module& operator=(const Module&) = delete;

        /**
         * Override if you want your module to be notified once it has been constructed and started.
         *
         * @note	Useful when your module is polymorphic and you cannot perform some implementation specific
         *			initialization in constructor itself.
         */
        virtual void OnStartUp() {}

        /**
         * Override if you want your module to be notified just before it is deleted.
         *
         * @note	Useful when your module is polymorphic and you might want to perform some kind of clean up perhaps
         *			overriding that of a base class.
         */
        virtual void OnShutDown() {}

        /** Returns a singleton instance of this module. */
        static T*& _instance()
        {
            static T* inst = nullptr;
            return inst;
        }

        /**
         * Checks if the Module has been shut down.
         *
         * @note	If module was never even started, this will return false.
         */
        static bool& IsDestroyed()
        {
            static bool inst = false;
            return inst;
        }

        /** Checks if the Module has been started up. */
        static bool& IsStartedUp()
        {
            static bool inst = false;
            return inst;
        }
    };
}
