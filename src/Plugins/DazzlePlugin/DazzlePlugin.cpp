#include "DazzlePlugin.h"

#include "Interface/PrototypeServiceInterface.h"
#include "Interface/DataServiceInterface.h"
#include "Interface/VocabularyServiceInterface.h"
#include "Interface/CodecServiceInterface.h"

#include "DazzleInterface.h"

#include "DataflowDZZ.h"
#include "DazzleEffect.h"
#include "ResourceDazzleEffect.h"
#include "DazzleEffectConverterDZBToDZZ.h"
#include "DazzleEffectPrototypeGenerator.h"

#include "Kernel/Logger.h"
#include "Kernel/ModuleFactory.h"
#include "Kernel/ConstStringHelper.h"
#include "Kernel/NodePrototypeGenerator.h"
#include "Kernel/ResourcePrototypeGenerator.h"
#include "Kernel/AssertionAllocator.h"
#include "Kernel/ConverterFactory.h"

//////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( Dazzle, Mengine::DazzlePlugin );
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    static void * s_dz_malloc( dz_size_t _size, dz_userdata_t _ud )
    {
        DZ_UNUSED( _ud );

        void * p = Helper::allocateMemory( _size, "dazzle" );

        return p;
    }
    //////////////////////////////////////////////////////////////////////////
    static void * s_dz_realloc( void * _ptr, dz_size_t _size, dz_userdata_t _ud )
    {
        DZ_UNUSED( _ud );

        void * p = Helper::reallocateMemory( _ptr, _size, "dazzle" );

        return p;
    }
    //////////////////////////////////////////////////////////////////////////
    static void s_dz_free( const void * _ptr, dz_userdata_t _ud )
    {
        DZ_UNUSED( _ud );

        Helper::deallocateMemory( (void *)_ptr, "dazzle" );
    }
    //////////////////////////////////////////////////////////////////////////
    static float s_dz_sqrtf( float _a, dz_userdata_t _ud )
    {
        DZ_UNUSED( _ud );

        float value = MT_sqrtf( _a );

        return value;
    }
    //////////////////////////////////////////////////////////////////////////
    static float s_dz_cosf( float _a, dz_userdata_t _ud )
    {
        DZ_UNUSED( _ud );

        float value = MT_cosf( _a );

        return value;
    }
    //////////////////////////////////////////////////////////////////////////
    static float s_dz_sinf( float _a, dz_userdata_t _ud )
    {
        DZ_UNUSED( _ud );

        float value = MT_sinf( _a );

        return value;
    }
    //////////////////////////////////////////////////////////////////////////
    DazzlePlugin::DazzlePlugin()
        : m_service( nullptr )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    DazzlePlugin::~DazzlePlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool DazzlePlugin::_initializePlugin()
    {
        LOGGER_INFO( "dazzle", "Initializing Dazzle..." );

        dz_service_providers_t providers;
        providers.f_malloc = &s_dz_malloc;
        providers.f_realloc = &s_dz_realloc;
        providers.f_free = &s_dz_free;
        providers.f_sqrtf = &s_dz_sqrtf;
        providers.f_cosf = &s_dz_cosf;
        providers.f_sinf = &s_dz_sinf;

        dz_service_t * service;
        if( dz_service_create( &service, &providers, DZ_NULLPTR ) == DZ_FAILURE )
        {
            return false;
        }

        m_service = service;

        DazzleEffectPrototypeGeneratorPtr prototypeGenerator = Helper::makeFactorableUnique<DazzleEffectPrototypeGenerator>( MENGINE_DOCUMENT_FACTORABLE );

        prototypeGenerator->setDazzleService( m_service );

        if( PROTOTYPE_SERVICE()
            ->addPrototype( STRINGIZE_STRING_LOCAL( "Node" ), STRINGIZE_STRING_LOCAL( "DazzleEffect" ), prototypeGenerator ) == false )
        {
            return false;
        }

        if( PROTOTYPE_SERVICE()
            ->addPrototype( STRINGIZE_STRING_LOCAL( "Resource" ), STRINGIZE_STRING_LOCAL( "ResourceDazzleEffect" ), Helper::makeFactorableUnique<ResourcePrototypeGenerator<ResourceDazzleEffect, 128>>( MENGINE_DOCUMENT_FACTORABLE ) ) == false )
        {
            return false;
        }

        CODEC_REGISTER_EXT( STRINGIZE_STRING_LOCAL( "dzz" ), STRINGIZE_STRING_LOCAL( "dazzle" ) );

        Helper::registerConverter<DazzleEffectConverterDZBToDZZ>( "dzb2dzz", MENGINE_DOCUMENT_FACTORABLE );

        PLUGIN_SERVICE_WAIT( DataServiceInterface, [this]()
        {
            DataflowDZZPtr dataflowDazzle = Helper::makeFactorableUnique<DataflowDZZ>( MENGINE_DOCUMENT_FACTORABLE );

            ArchivatorInterfacePtr archivator = VOCABULARY_GET( STRINGIZE_STRING_LOCAL( "Archivator" ), STRINGIZE_STRING_LOCAL( "lz4" ) );

            MENGINE_ASSERTION_MEMORY_PANIC( archivator );

            dataflowDazzle->setArchivator( archivator );
            dataflowDazzle->setDazzleService( m_service );

            if( dataflowDazzle->initialize() == false )
            {
                return false;
            }

            VOCABULARY_SET( DataflowInterface, STRINGIZE_STRING_LOCAL( "Dataflow" ), STRINGIZE_STRING_LOCAL( "dazzle" ), dataflowDazzle, MENGINE_DOCUMENT_FACTORABLE );

            return true;
        } );

        PLUGIN_SERVICE_LEAVE( DataServiceInterface, []()
        {
            DataflowInterfacePtr dataflowFE = VOCABULARY_REMOVE( STRINGIZE_STRING_LOCAL( "Dataflow" ), STRINGIZE_STRING_LOCAL( "dazzle" ) );
            dataflowFE->finalize();
        } );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void DazzlePlugin::_finalizePlugin()
    {
        Helper::unregisterConverter( "dzb2dzz" );

        CODEC_UNREGISTER_EXT( STRINGIZE_STRING_LOCAL( "dzz" ) );

        PROTOTYPE_SERVICE()
            ->removePrototype( STRINGIZE_STRING_LOCAL( "Node" ), STRINGIZE_STRING_LOCAL( "DazzleEffect" ) );

        PROTOTYPE_SERVICE()
            ->removePrototype( STRINGIZE_STRING_LOCAL( "Resource" ), STRINGIZE_STRING_LOCAL( "ResourceDazzleEffect" ) );
    }
    //////////////////////////////////////////////////////////////////////////
    void DazzlePlugin::_destroyPlugin()
    {
        dz_service_destroy( m_service );
        m_service = nullptr;

        MENGINE_ASSERTION_ALLOCATOR( "dazzle" );
    }
    //////////////////////////////////////////////////////////////////////////
}