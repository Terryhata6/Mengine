#include "AndroidNativeUnityAdsPlugin.h"

#include "Interface/StringizeInterface.h"

#include "AndroidNativeUnityAdsModule.h"

#include "Core/ModuleFactory.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( AndroidNativeUnityAds, Mengine::AndroidNativeUnityAdsPlugin )
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Mengine
{    
    //////////////////////////////////////////////////////////////////////////
    AndroidNativeUnityAdsPlugin::AndroidNativeUnityAdsPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    AndroidNativeUnityAdsPlugin::~AndroidNativeUnityAdsPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeUnityAdsPlugin::_avaliable()
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool AndroidNativeUnityAdsPlugin::_initialize()
    {
        this->addModuleFactory( STRINGIZE_STRING_LOCAL( "ModuleAndroidUnityAds" )
            , new ModuleFactory<AndroidNativeUnityAdsModule>() );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void AndroidNativeUnityAdsPlugin::_finalize()
    {
    }    
}