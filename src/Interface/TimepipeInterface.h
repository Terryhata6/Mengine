#pragma once

#include "Kernel/Mixin.h"
#include "Kernel/UpdateContext.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    class TimepipeInterface
        : public Mixin
    {
    public:
         virtual void onTimepipe( const UpdateContext * _contet ) = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TimepipeInterface> TimepipeInterfacePtr;
    //////////////////////////////////////////////////////////////////////////
}