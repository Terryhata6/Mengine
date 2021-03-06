#pragma once

#include "Interface/UnknownInterface.h"
#include "Interface/DataInterface.h"

#include "Kernel/Magic.h"
#include "Kernel/Resource.h"
#include "Kernel/ResourceImage.h"

//////////////////////////////////////////////////////////////////////////
struct dz_effect_t;
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    DECLARE_MAGIC_NUMBER( MAGIC_DZZ, 'D', 'Z', 'Z', '1', 1 );
    //////////////////////////////////////////////////////////////////////////
    class UnknownDazzleEffectInterface
        : public UnknownInterface
    {
    public:
        virtual void setResourceDazzle( const ResourcePtr & _resource ) = 0;
        virtual const ResourcePtr & getResourceDazzle() const = 0;

        virtual void setResourceImage( const ResourceImagePtr & _resource ) = 0;
        virtual const ResourceImagePtr & getResourceImage() const = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    class UnknownResourceDazzleEffectInterface
        : public UnknownInterface
    {
    public:
        virtual const DataInterfacePtr & getData() const = 0;
    };
    //////////////////////////////////////////////////////////////////////////
    class UnknownResourceDazzleEffectCustomInterface
        : public UnknownResourceDazzleEffectInterface
    {
    public:
        virtual void setDazzleEffect( const dz_effect_t * _effect ) = 0;
        virtual const dz_effect_t * getDazzleEffect() const = 0;
    };
    //////////////////////////////////////////////////////////////////////////
}