#pragma once

#include "Interface/RenderTextureServiceInterface.h"
#include "Interface/ImageCodecInterface.h"

#include "Kernel/ServiceBase.h"
#include "Kernel/ConstString.h"
#include "Kernel/Hashtable2.h"
#include "Kernel/Factory.h"
#include "Kernel/Pair.h"

namespace Mengine
{
    class RenderTextureService
        : public ServiceBase<RenderTextureServiceInterface>
    {
    public:
        RenderTextureService();
        ~RenderTextureService() override;

    public:
        bool _initializeService() override;
        void _finalizeService() override;

    public:
        RenderTextureInterfacePtr loadTexture( const FileGroupInterfacePtr & _fileGroup, const FilePath & _filePath, const ConstString & _codecType, uint32_t _codecFlags, const DocumentPtr & _doc ) override;
        RenderTextureInterfacePtr createRenderTexture( const RenderImageInterfacePtr & _image, uint32_t _width, uint32_t _height, const DocumentPtr & _doc ) override;

    public:
        RenderTextureInterfacePtr createTexture( uint32_t _mipmaps, uint32_t _width, uint32_t _height, uint32_t _channels, uint32_t _depth, EPixelFormat _format, const DocumentPtr & _doc ) override;

    public:
        RenderTextureInterfacePtr getTexture( const FileGroupInterfacePtr & _fileGroup, const FilePath & _filePath ) const override;

    public:
        bool hasTexture( const FileGroupInterfacePtr & _fileGroup, const FilePath & _filePath, RenderTextureInterfacePtr * const _texture ) const override;

    public:
        RenderImageLoaderInterfacePtr createDecoderRenderImageLoader( const FileGroupInterfacePtr & _fileGroup, const FilePath & _filePath, const ConstString & _codecType, uint32_t _codecFlags, const DocumentPtr & _doc ) override;

    public:
        void cacheFileTexture( const FileGroupInterfacePtr & _fileGroup, const FilePath & _filePath, const RenderTextureInterfacePtr & _texture ) override;

    public:
        bool saveImage( const RenderTextureInterfacePtr & _texture, const FileGroupInterfacePtr & _fileGroup, const ConstString & _codecType, const FilePath & _filePath ) override;

    public:
        void visitTexture( const LambdaRenderTexture & _lambda ) const override;
        size_t getImageMemoryUse( uint32_t _width, uint32_t _height, uint32_t _channels, uint32_t _depth, EPixelFormat _format ) const override;

    protected:
        bool onRenderTextureDestroy_( RenderTextureInterface * _texture );

    protected:
        void updateImageParams_( uint32_t * const _width, uint32_t * const _height, uint32_t * const _channels, uint32_t * const _depth, EPixelFormat * const _format ) const;

    protected:
        typedef Hashtable2<ConstString, FilePath, RenderTextureInterface *> MapRenderTextureEntry;
        MapRenderTextureEntry m_textures;

        FactoryPtr m_factoryRenderTexture;
        FactoryPtr m_factoryDecoderRenderImageProvider;
        FactoryPtr m_factoryDecoderRenderImageLoader;

        bool m_supportA8;
        bool m_supportL8;
        bool m_supportR8G8B8;
        bool m_supportNonPow2;
    };
}