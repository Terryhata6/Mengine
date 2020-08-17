#pragma once

#include "Interface/Interface.h"

#include "Kernel/LoggerLevel.h"

#include "Config/Typedef.h"
#include "Config/Char.h"

#ifndef MENGINE_LOGGER_MAX_MESSAGE
#define MENGINE_LOGGER_MAX_MESSAGE 16384
#endif

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    class LoggerInterface
        : public Interface
    {
    public:
        virtual bool initialize() = 0;
        virtual void finalize() = 0;

    public:
        virtual void setColor( uint32_t _color ) = 0;
        virtual uint32_t getColor() const = 0;

    public:
        virtual void setVerboseLevel( ELoggerLevel _level ) = 0;
        virtual ELoggerLevel getVerboseLevel() const = 0;

        virtual void setVerboseFlag( uint32_t _flag ) = 0;
        virtual uint32_t getVerboseFlag() const = 0;

    public:
        virtual bool validMessage( ELoggerLevel _level, uint32_t _flag ) const = 0;

    public:
        virtual void log( ELoggerLevel _level, uint32_t _flag, uint32_t _color, const Char * _data, size_t _size ) = 0;
        virtual void flush() = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<LoggerInterface> LoggerInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}

