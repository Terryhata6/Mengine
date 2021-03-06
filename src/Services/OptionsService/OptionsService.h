#pragma once

#include "Interface/OptionsServiceInterface.h"

#include "Kernel/ServiceBase.h"
#include "Kernel/Vector.h"

#ifndef MENGINE_OPTIONS_KEY_SIZE
#define MENGINE_OPTIONS_KEY_SIZE 32
#endif

#ifndef MENGINE_OPTIONS_VALUE_SIZE
#define MENGINE_OPTIONS_VALUE_SIZE 64
#endif

#ifndef MENGINE_OPTIONS_VALUE_COUNT
#define MENGINE_OPTIONS_VALUE_COUNT 16
#endif

namespace Mengine
{
    class OptionsService
        : public ServiceBase<OptionsServiceInterface>
    {
    public:
        OptionsService();
        ~OptionsService() override;

    public:
        bool _initializeService() override;
        void _finalizeService() override;

    public:
        bool setArguments( const ArgumentsInterfacePtr & _arguments ) override;

    public:
        bool hasOption( const Char * _key ) const override;
        const Char * getOptionValue( const Char * _key, const Char * _default ) const override;
        bool getOptionValues( const Char * _key, const Char ** _values, uint32_t * _count ) const override;
        uint32_t getOptionUInt32( const Char * _key, uint32_t _default ) const override;
        bool testOptionValue( const Char * _key, const Char * _value ) const override;

    protected:
        struct Option
        {
            Char key[MENGINE_OPTIONS_KEY_SIZE] = {'\0'};
            Char value[MENGINE_OPTIONS_VALUE_SIZE][MENGINE_OPTIONS_VALUE_COUNT] = {'\0'};
            uint32_t value_count;
        };

        typedef Vector<Option> VectorOptions;
        VectorOptions m_options;
    };
}


