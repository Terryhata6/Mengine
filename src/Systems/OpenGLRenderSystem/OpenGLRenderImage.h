#pragma once

#include "Interface/RenderImageInterface.h"
#include "Interface/OpenGLRenderImageExtensionInterface.h"
#include "Interface/MemoryInterface.h"

#include "Environment/OpenGL/OpenGLRenderIncluder.h"

#include "OpenGLRenderResourceHandler.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    class OpenGLRenderImage
        : public RenderImageInterface
        , public OpenGLRenderImageExtensionInterface
        , public OpenGLRenderResourceHandler
        , public Factorable
    {
    public:
        OpenGLRenderImage();
        ~OpenGLRenderImage() override;

    public:
        bool initialize( uint32_t _mipmaps, uint32_t _width, uint32_t _height, uint32_t _channels, EPixelFormat _pixelFormat, GLint _internalFormat, GLenum _format, GLenum _type );
        void finalize();

    public:
        void bind( uint32_t _stage ) override;
        void unbind( uint32_t _stage ) override;

    public:
        void setRenderImageProvider( const RenderImageProviderInterfacePtr & _renderImageProvider ) override;
        const RenderImageProviderInterfacePtr & getRenderImageProvider() const override;

    public:
        bool create();
        void release();

    public:
        Pointer lock( size_t * const _pitch, uint32_t _level, const Rect & _rect, bool _readOnly ) override;
        bool unlock( uint32_t _level, bool _successful ) override;

    public:
        uint32_t getHWMipmaps() const override;
        uint32_t getHWWidth() const override;
        uint32_t getHWHeight() const override;
        uint32_t getHWChannels() const override;
        uint32_t getHWDepth() const override;

        EPixelFormat getHWPixelFormat() const override;

    public:
        float getHWWidthInv() const override;
        float getHWHeightInv() const override;

    public:
        UnknownPointer getRenderImageExtention() override;

    public:
        GLuint getUID() const override;

    public:
        void setMinFilter( GLenum _minFilter );
        GLenum getMinFilter() const;

        void setMagFilter( GLenum _magFilter );
        GLenum getMagFilter() const;

        void setWrapS( GLenum _wrapS );
        GLenum getWrapS() const;

        void setWrapT( GLenum _wrapT );
        GLenum getWrapT() const;

    protected:
        void onRenderReset() override;
        bool onRenderRestore() override;

    protected:
        RenderImageProviderInterfacePtr m_renderImageProvider;

        GLuint m_uid;

        EPixelFormat m_hwPixelFormat;

        uint32_t m_width;
        uint32_t m_height;

        uint32_t m_hwMipmaps;
        uint32_t m_hwWidth;
        uint32_t m_hwHeight;
        uint32_t m_hwChannels;

        float m_hwWidthInv;
        float m_hwHeightInv;

        GLenum m_minFilter;
        GLenum m_magFilter;
        GLenum m_wrapS;
        GLenum m_wrapT;

        GLint m_internalFormat;

        GLenum m_format;
        GLenum m_type;

        MemoryInterfacePtr m_lockMemory;
        Rect m_lockRect;
        uint32_t m_lockLevel;
        bool m_lockFirst;

        bool m_pow2;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<OpenGLRenderImage> OpenGLRenderImagePtr;
    //////////////////////////////////////////////////////////////////////////
}
