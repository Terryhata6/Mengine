#include "OpenGLRenderImageTarget.h"

#include "OpenGLRenderExtension.h"
#include "OpenGLRenderError.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    OpenGLRenderImageTarget::OpenGLRenderImageTarget()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    OpenGLRenderImageTarget::~OpenGLRenderImageTarget()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool OpenGLRenderImageTarget::initialize( const OpenGLRenderTargetTexturePtr & _renderTarget )
    {
        m_renderTarget = _renderTarget;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void OpenGLRenderImageTarget::finalize()
    {
        m_renderTarget = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    void OpenGLRenderImageTarget::bind( uint32_t _stage )
    {
#ifdef MENGINE_RENDER_OPENGL_ES
        GLCALL( glActiveTexture, (GL_TEXTURE0 + _stage) );
#else
        GLCALL( glActiveTexture_, (GL_TEXTURE0 + _stage) );
#endif

        GLuint uid = m_renderTarget->getUID();

        GLCALL( glBindTexture, (GL_TEXTURE_2D, uid) );
    }
    //////////////////////////////////////////////////////////////////////////
    void OpenGLRenderImageTarget::unbind( uint32_t _stage )
    {
#ifdef MENGINE_RENDER_OPENGL_ES
        GLCALL( glActiveTexture, (GL_TEXTURE0 + _stage) );
#else
        GLCALL( glActiveTexture_, (GL_TEXTURE0 + _stage) );
#endif

        GLCALL( glBindTexture, (GL_TEXTURE_2D, 0) );
    }
    //////////////////////////////////////////////////////////////////////////
    void OpenGLRenderImageTarget::setRenderImageProvider( const RenderImageProviderInterfacePtr & _renderImageProvider )
    {
        MENGINE_UNUSED( _renderImageProvider );
    }
    //////////////////////////////////////////////////////////////////////////
    const RenderImageProviderInterfacePtr & OpenGLRenderImageTarget::getRenderImageProvider() const
    {
        return RenderImageProviderInterfacePtr::none();
    }
    ///////////////////////////////////////////////////////////////////////////
    Pointer OpenGLRenderImageTarget::lock( size_t * const _pitch, uint32_t _level, const Rect & _rect, bool _readOnly )
    {
        MENGINE_UNUSED( _pitch );
        MENGINE_UNUSED( _level );
        MENGINE_UNUSED( _rect );
        MENGINE_UNUSED( _readOnly );

        return nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    bool OpenGLRenderImageTarget::unlock( uint32_t _level, bool _successful )
    {
        MENGINE_UNUSED( _level );
        MENGINE_UNUSED( _successful );

        return false;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t OpenGLRenderImageTarget::getHWWidth() const
    {
        uint32_t HWWidth = m_renderTarget->getHWWidth();

        return HWWidth;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t OpenGLRenderImageTarget::getHWHeight() const
    {
        uint32_t HWHeight = m_renderTarget->getHWHeight();

        return HWHeight;
    }
    //////////////////////////////////////////////////////////////////////////
    EPixelFormat OpenGLRenderImageTarget::getHWPixelFormat() const
    {
        EPixelFormat HWPixelFormat = m_renderTarget->getHWPixelFormat();

        return HWPixelFormat;
    }
    //////////////////////////////////////////////////////////////////////////
    float OpenGLRenderImageTarget::getHWWidthInv() const
    {
        float HWWidthInv = m_renderTarget->getHWWidthInv();

        return HWWidthInv;
    }
    //////////////////////////////////////////////////////////////////////////
    float OpenGLRenderImageTarget::getHWHeightInv() const
    {
        float HWHeightInv = m_renderTarget->getHWHeightInv();

        return HWHeightInv;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t OpenGLRenderImageTarget::getHWChannels() const
    {
        uint32_t HWChannels = m_renderTarget->getHWChannels();

        return HWChannels;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t OpenGLRenderImageTarget::getHWDepth() const
    {
        uint32_t HWDepth = m_renderTarget->getHWDepth();

        return HWDepth;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t OpenGLRenderImageTarget::getHWMipmaps() const
    {
        uint32_t HWMipmaps = m_renderTarget->getHWMipmaps();

        return HWMipmaps;
    }
    //////////////////////////////////////////////////////////////////////////
    UnknownPointer OpenGLRenderImageTarget::getRenderImageExtention()
    {
        return this;
    }
    //////////////////////////////////////////////////////////////////////////
    GLuint OpenGLRenderImageTarget::getUID() const
    {
        GLuint uid = m_renderTarget->getUID();

        return uid;
    }
    //////////////////////////////////////////////////////////////////////////
    void OpenGLRenderImageTarget::onRenderReset()
    {
        //Empty
    }
    //////////////////////////////////////////////////////////////////////////
    bool OpenGLRenderImageTarget::onRenderRestore()
    {
        //Empty

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
}
