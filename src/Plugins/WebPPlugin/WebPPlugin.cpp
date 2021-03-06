#include "WebPPlugin.h"

#include "Interface/CodecInterface.h"
#include "Interface/CodecServiceInterface.h"
#include "Interface/ImageCodecInterface.h"

#include "ImageDecoderWebP.h"

#include "Kernel/DecoderFactory.h"
#include "Kernel/EncoderFactory.h"
#include "Kernel/ConstStringHelper.h"

#include "webp/decode.h"

//////////////////////////////////////////////////////////////////////////
PLUGIN_FACTORY( WebP, Mengine::WebPPlugin );
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    WebPPlugin::WebPPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    WebPPlugin::~WebPPlugin()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool WebPPlugin::_initializePlugin()
    {
        int32_t version = WebPGetDecoderVersion();

        LOGGER_MESSAGE_RELEASE( "WebP Version: %u.%u.%u"
            , (version >> 16) & 0xff
            , (version >> 8) & 0xff
            , (version >> 0) & 0xff
        );

        LOGGER_MESSAGE_RELEASE( "WebP ABI Version: %p"
            , (void *)WEBP_DECODER_ABI_VERSION
        );

        Helper::registerDecoder<ImageDecoderWEBP>( STRINGIZE_STRING_LOCAL( "webpImage" ), MENGINE_DOCUMENT_FACTORABLE );

        CODEC_REGISTER_EXT( STRINGIZE_STRING_LOCAL( "webp" ), STRINGIZE_STRING_LOCAL( "webpImage" ) );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void WebPPlugin::_finalizePlugin()
    {
        CODEC_UNREGISTER_EXT( STRINGIZE_STRING_LOCAL( "webp" ) );

        Helper::unregisterDecoder( STRINGIZE_STRING_LOCAL( "webpImage" ) );
    }
    //////////////////////////////////////////////////////////////////////////
}