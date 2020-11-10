#include "Win32SocketPlugin.h"

#include "Interface/SocketSystemInterface.h"

#include "Kernel/Logger.h"

//////////////////////////////////////////////////////////////////////////
SERVICE_EXTERN( SocketSystem );
//////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( Win32Socket, Mengine::Win32SocketPlugin );
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    Win32SocketPlugin::Win32SocketPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Win32SocketPlugin::~Win32SocketPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool Win32SocketPlugin::_initializePlugin()
    {
        LOGGER_INFO( "socket", "Initializing Win32 Socket Plugin" );

        SERVICE_CREATE( SocketSystem, MENGINE_DOCUMENT_FACTORABLE );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Win32SocketPlugin::_finalizePlugin()
    {
        SERVICE_FINALIZE( SocketSystem );
    }
    //////////////////////////////////////////////////////////////////////////
    void Win32SocketPlugin::_destroyPlugin()
    {
        SERVICE_DESTROY( SocketSystem );
    }
}