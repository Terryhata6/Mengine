#pragma once

#include "Interface/PluginInterface.h"

#include "Kernel/Factorable.h"

namespace Mengine
{
    class SDLDynamicLibrary 
        : public DynamicLibraryInterface
		, public Factorable
    {
    public:
        SDLDynamicLibrary();
        ~SDLDynamicLibrary() override;

    public:
        void setName( const String & _name );
        const String & getName() const;

    public:
        bool load() override;

    public:
        TDynamicLibraryFunction getSymbol( const Char * _name ) const override;

    private:
        String m_name;
        void* m_instance;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef stdex::intrusive_ptr<SDLDynamicLibrary> SDLDynamicLibraryPtr;
    //////////////////////////////////////////////////////////////////////////
};