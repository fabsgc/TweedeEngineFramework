#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

namespace te
{
    class Exception : public std::exception
    {
    public:
        Exception(const char* type, const String& description, const String& source)
            :_typeName(type), _description(description), _source(source)
        { }

        Exception(const char* type, const String& description, const String& source, const char* file, long line)
            : _line(line), _typeName(type), _description(description), _source(source), _file(file)
        { }

        Exception(const Exception& rhs)
            : _line(rhs._line), _typeName(rhs._typeName), _description(rhs._description),
            _source(rhs._source), _file(rhs._file)
        { }

        ~Exception() noexcept = default;

        void operator = (const Exception& rhs)
        {
            _description = rhs._description;
            _source = rhs._source;
            _file = rhs._file;
            _line = rhs._line;
            _typeName = rhs._typeName;
        }

        /**
         * Returns a string with the full description of the exception.
         *
         * @note
         * The description contains the error number, the description supplied by the thrower, what routine threw the
         * exception, and will also supply extra platform-specific information where applicable.
         */
        virtual const String& getFullDescription() const
        {
            if (_fullDesc.empty())
            {
                StringStream desc;

                desc << "TWEEDE EXCEPTION(" << _typeName << "): "
                    << _description
                    << " in " << _source;

                if (_line > 0)
                {
                    desc << " at " << _file << " (line " << _line << ")";
                }

                _fullDesc = desc.str();
            }

            return _fullDesc;
        }

        /** Gets the source function that threw the exception. */
        virtual const String& getSource() const { return _source; }

        /** Gets the source file name in which the exception was thrown. */
        virtual const String& getFile() const { return _file; }

        /** Gets line number on which the exception was thrown. */
        virtual long getLine() const { return _line; }

        /** Gets a short description about the exception. */
        virtual const String& getDescription(void) const { return _description; }

        /** Overriden std::exception::what. Returns the same value as getFullDescription(). */
        const char* what() const noexcept override { return getFullDescription().c_str(); }

    protected:
        long _line = 0;
        String _typeName;
        String _description;
        String _source;
        String _file;
        mutable String _fullDesc;
    };

    /** Exception for signaling not implemented parts of the code. */
    class NotImplementedException : public Exception
    {
    public:
        NotImplementedException(const String& inDescription, const String& inSource, const char* inFile, long inLine)
            : Exception("NotImplementedException", inDescription, inSource, inFile, inLine) {}
    };

    /** Exception for signaling file system errors when file could not be found. */
    class FileNotFoundException : public Exception
    {
    public:
        FileNotFoundException(const String& inDescription, const String& inSource, const char* inFile, long inLine)
            : Exception("FileNotFoundException", inDescription, inSource, inFile, inLine) {}
    };

    /** Exception for signaling general IO errors.
     *
     * @note	An example being failed to open a file or a network connection.
     */
    class IOException : public Exception
    {
    public:
        IOException(const String& inDescription, const String& inSource, const char* inFile, long inLine)
            : Exception("IOException", inDescription, inSource, inFile, inLine) {}
    };

    /** Exception for signaling not currently executing code in not in a valid state. */
    class InvalidStateException : public Exception
    {
    public:
        InvalidStateException(const String& inDescription, const String& inSource, const char* inFile, long inLine)
            : Exception("InvalidStateException", inDescription, inSource, inFile, inLine) {}
    };

    /** Exception for signaling not some parameters you have provided are not valid. */
    class InvalidParametersException : public Exception
    {
    public:
        InvalidParametersException(const String& inDescription, const String& inSource, const char* inFile, long inLine)
            : Exception("InvalidParametersException", inDescription, inSource, inFile, inLine) {}
    };

    /**
     * Exception for signaling an internal error, normally something that shouldn't have happened or wasn't anticipated by
     * the programmers of that system.
     */
    class InternalErrorException : public Exception
    {
    public:
        InternalErrorException(const String& inDescription, const String& inSource, const char* inFile, long inLine)
            : Exception("InternalErrorException", inDescription, inSource, inFile, inLine) {}
    };

    /** Exception for signaling an error in a rendering API. */
    class RenderingAPIException : public Exception
    {
    public:
        RenderingAPIException(const String& inDescription, const String& inSource, const char* inFile, long inLine)
            : Exception("RenderingAPIException", inDescription, inSource, inFile, inLine) {}
    };

    /** Exception for signaling an error in an unit test. */
    class UnitTestException : public Exception
    {
    public:
        UnitTestException(const String& inDescription, const String& inSource, const char* inFile, long inLine)
            : Exception("UnitTestException", inDescription, inSource, inFile, inLine) {}
    };

    /**
     * Macro for throwing exceptions that will automatically fill out function name, file name and line number of the
     * exception.
     */
     // Banshee doesn't actually use exceptions, so we just emulate the unhandled exception handler by crashing the application.
#ifndef TE_EXCEPT
#define TE_EXCEPT(type, desc)																\
	{																						\
		static_assert((std::is_base_of<te::Exception, type>::value),						\
			"Invalid exception type (" #type ") for TE_EXCEPT macro."						\
			" It needs to derive from te::Exception.");										\
        TE_ASSERT_ERROR(false, desc);                                                       \
	}
#endif

    /** @} */
}

