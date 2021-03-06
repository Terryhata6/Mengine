#pragma once

#include "Interface/LoaderInterface.h"

#include "Kernel/Factorable.h"

namespace Mengine
{
    class LoaderResourceImageSubstractRGBAndAlpha
        : public LoaderInterface
        , public Factorable
    {
    public:
        LoaderResourceImageSubstractRGBAndAlpha();
        ~LoaderResourceImageSubstractRGBAndAlpha() override;

    protected:
        bool load( const LoadableInterfacePtr & _loadable, const Metabuf::Metadata * _meta ) override;
    };
}