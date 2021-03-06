#include "AstralaxPlugin.h"

#include "AstralaxInterface.h"

#include "AstralaxModule.h"

#include "ParticleConverterPTCToPTZ.h"

#include "Kernel/Logger.h"
#include "Kernel/ModuleFactory.h"
#include "Kernel/ConstStringHelper.h"
#include "Kernel/ConverterFactory.h"

//////////////////////////////////////////////////////////////////////////
SERVICE_EXTERN( AstralaxService );
//////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( Astralax, Mengine::AstralaxPlugin );
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    AstralaxPlugin::AstralaxPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    AstralaxPlugin::~AstralaxPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool AstralaxPlugin::_initializePlugin()
    {
        LOGGER_INFO( "astralax", "Initializing Astralax System..." );

        SERVICE_CREATE( AstralaxService, MENGINE_DOCUMENT_FACTORABLE );

        this->addModuleFactory( STRINGIZE_STRING_LOCAL( "ModuleAstralax" )
            , Helper::makeModuleFactory<AstralaxModule>( MENGINE_DOCUMENT_FACTORABLE ), MENGINE_DOCUMENT_FACTORABLE );

        Helper::registerConverter<ParticleConverterPTCToPTZ>( "ptc2ptz", MENGINE_DOCUMENT_FACTORABLE );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void AstralaxPlugin::_finalizePlugin()
    {
        Helper::unregisterConverter( "ptc2ptz" );

        SERVICE_FINALIZE( AstralaxService );
    }
    //////////////////////////////////////////////////////////////////////////
    void AstralaxPlugin::_destroyPlugin()
    {
        SERVICE_DESTROY( AstralaxService );
    }
    //////////////////////////////////////////////////////////////////////////
}