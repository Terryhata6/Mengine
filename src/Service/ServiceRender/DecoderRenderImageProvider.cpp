#   include "DecoderRenderImageProvider.h"

#   include "DecoderRenderImageLoader.h"

#   include "Factory/FactorableUnique.h"

#   include "Logger/Logger.h"

namespace Menge
{
    //////////////////////////////////////////////////////////////////////////
    DecoderRenderImageProvider::DecoderRenderImageProvider()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    DecoderRenderImageProvider::~DecoderRenderImageProvider()
    {
    }
    //////////////////////////////////////////////////////////////////////////    
    void DecoderRenderImageProvider::initialize( const ConstString& _pakName, const FilePath & _fileName, const ConstString & _codecName )
    {
        m_pakName = _pakName;
        m_fileName = _fileName;
        m_codecName = _codecName;
    }
    //////////////////////////////////////////////////////////////////////////
    RenderImageLoaderInterfacePtr DecoderRenderImageProvider::getLoader() const
    {
        DecoderRenderImageLoaderPtr loader = new FactorableUnique<DecoderRenderImageLoader>();

        loader->setServiceProvider( m_serviceProvider );

        if( loader->initialize( m_pakName, m_fileName, m_codecName ) == false )
        {
            return nullptr;
        }

        return loader;
    }
}	// namespace Menge