#include "ToolUtils/ToolUtils.h"

#include <vector>
#include <string>
#include <sstream>

#include "movie/movie.hpp"
#include "pugixml.hpp"

//////////////////////////////////////////////////////////////////////////
static void unicode_to_utf8( char * _utf8, size_t _capacity, const wchar_t * _unicode, size_t _size )
{
    DWORD dwConversionFlags = WC_ERR_INVALID_CHARS;

    int utf8_size = ::WideCharToMultiByte(
        CP_UTF8
        , dwConversionFlags
        , _unicode
        , _size
        , _utf8
        , _capacity
        , NULL
        , NULL
    );

    _utf8[utf8_size] = 0;
}
//////////////////////////////////////////////////////////////////////////
static void * stdlib_movie_alloc( void * _data, size_t _size )
{
    (void)_data;

    return malloc( _size );
}
//////////////////////////////////////////////////////////////////////////
static void * stdlib_movie_alloc_n( void * _data, size_t _size, size_t _count )
{
    (void)_data;

    size_t total = _size * _count;

    return malloc( total );
}
//////////////////////////////////////////////////////////////////////////
static void stdlib_movie_free( void * _data, const void * _ptr )
{
    (void)_data;

    free( (void *)_ptr );
}
//////////////////////////////////////////////////////////////////////////
static void stdlib_movie_free_n( void * _data, const void * _ptr )
{
    (void)_data;

    free( (void *)_ptr );
}
//////////////////////////////////////////////////////////////////////////
static void stdlib_movie_logerror( void * _data, aeMovieErrorCode _code, const char * _format, ... )
{
    (void)_data;

    switch( _code )
    {
    case AE_ERROR_STREAM:
        {
            return;
        }break;
    default:
        {
        }break;
    }

    va_list argList;

    va_start( argList, _format );
    char msg[4096];
    vsprintf( msg, _format, argList );
    va_end( argList );

    printf( "# %s code %d"
        , msg
        , _code
    );
}
//////////////////////////////////////////////////////////////////////////
struct resource_provider_t
{
    pugi::xml_node * xmlDataBlock;
    CHAR movie_name[128];
};
//////////////////////////////////////////////////////////////////////////
static ae_bool_t my_resource_provider( const aeMovieResource * _resource, ae_voidptrptr_t _rd, ae_voidptr_t _data )
{
    resource_provider_t * provider = (resource_provider_t *)_data;

    pugi::xml_node * xmlDataBlock = provider->xmlDataBlock;

    aeMovieResourceTypeEnum resource_type = _resource->type;

    switch( resource_type )
    {
    case AE_MOVIE_RESOURCE_IMAGE:
        {
            const aeMovieResourceImage * resource_image = (const aeMovieResourceImage *)_resource;

            pugi::xml_node xmlResource = xmlDataBlock->append_child( "Resource" );
            xmlResource.append_attribute( "Name" ).set_value( resource_image->name );
            xmlResource.append_attribute( "Type" ).set_value( "ResourceImageDefault" );
            xmlResource.append_attribute( "Unique" ).set_value( 0U );

            //fprintf( f, "   <Resource Name = \"%s\" Type = \"ResourceImageDefault\" Unique = \"0\">\n"
                //, resource_image->name
            //);

            pugi::xml_node xmlResourceFile = xmlResource.append_child( "File" );

            char xmlFilePath[MAX_PATH];
            sprintf( xmlFilePath, "Movies2/%s/%s"
                , provider->movie_name
                , resource_image->path
            );

            xmlResourceFile.append_attribute( "Path" ).set_value( xmlFilePath );

            char xmlMaxSize[256];
            sprintf( xmlMaxSize, "%u;%u"
                , (uint32_t)resource_image->trim_width
                , (uint32_t)resource_image->trim_height
            );

            xmlResourceFile.append_attribute( "MaxSize" ).set_value( xmlMaxSize );

            if( strstr( resource_image->name, ".ptc_" ) != nullptr )
            {
                xmlResourceFile.append_attribute( "NoConvert" ).set_value( 1U );
                xmlResourceFile.append_attribute( "NoAtlas" ).set_value( 1U );
            }

            //fprintf( f, "       <File Path = \"Movies2/%s/%s\" NoConvert = \"1\" NoAtlas = \"1\" MaxSize = \"%u;%u\"/>\n"
            //    , provider->movie_name
            //    , resource_image->path
            //    , (uint32_t)resource_image->trim_width
            //    , (uint32_t)resource_image->trim_height
            //);

            //fprintf( f, "   </Resource>\n" );
        }break;
    case AE_MOVIE_RESOURCE_VIDEO:
        {
            const aeMovieResourceVideo * resource_video = (const aeMovieResourceVideo *)_resource;

            pugi::xml_node xmlResource = xmlDataBlock->append_child( "Resource" );
            xmlResource.append_attribute( "Name" ).set_value( resource_video->name );
            xmlResource.append_attribute( "Type" ).set_value( "ResourceVideo" );
            xmlResource.append_attribute( "Unique" ).set_value( 0U );

//#if AE_MOVIE_SDK_MAJOR_VERSION >= 17
//            fprintf( f, "   <Resource Name = \"%s\" Type = \"ResourceVideo\" Unique = \"0\">\n"
//                , resource_video->name
//            );
//#endif
            
            pugi::xml_node xmlResourceFile = xmlResource.append_child( "File" );

            char xmlFilePath[MAX_PATH];
            sprintf( xmlFilePath, "Movies2/%s/%s"
                , provider->movie_name
                , resource_video->path
            );

            xmlResourceFile.append_attribute( "Path" ).set_value( xmlFilePath );

            char xmlSize[256];
            sprintf( xmlSize, "%u;%u"
                , (uint32_t)resource_video->width
                , (uint32_t)resource_video->height
            );

            xmlResourceFile.append_attribute( "Size" ).set_value( xmlSize );

            xmlResourceFile.append_attribute( "Alpha" ).set_value( resource_video->alpha == AE_TRUE ? 1U : 0U );
            xmlResourceFile.append_attribute( "Codec" ).set_value( resource_video->alpha == AE_TRUE ? "ogvaVideo" : "ogvVideo" );
            xmlResourceFile.append_attribute( "FrameRate" ).set_value( resource_video->frameRate );
            xmlResourceFile.append_attribute( "Duration" ).set_value( resource_video->duration * 1000.0 );

            //fprintf( f, "       <File Path = \"Movies2/%s/%s\" Alpha = \"%u\" Codec = \"%s\" FrameRate = \"%f\" Duration = \"%f\"/>\n"
            //    , provider->movie_name
            //    , resource_video->path
            //    , resource_video->alpha
            //    , resource_video->alpha == AE_TRUE ? "ogvaVideo" : "ogvVideo"
            //    , resource_video->frameRate
            //    , resource_video->duration * 1000.0
            //);

            //fprintf( f, "   </Resource>\n" );
        }break;
    case AE_MOVIE_RESOURCE_SOUND:
        {
            const aeMovieResourceSound * resource_sound = (const aeMovieResourceSound *)_resource;
            
            pugi::xml_node xmlResource = xmlDataBlock->append_child( "Resource" );
            xmlResource.append_attribute( "Name" ).set_value( resource_sound->name );
            xmlResource.append_attribute( "Type" ).set_value( "ResourceSound" );
            xmlResource.append_attribute( "Unique" ).set_value( 0U );

//#if AE_MOVIE_SDK_MAJOR_VERSION >= 17
//            fprintf( f, "   <Resource Name = \"%s\" Type = \"ResourceSound\" Unique = \"0\">\n"
//                , resource_sound->name
//            );
//#endif

            pugi::xml_node xmlResourceFile = xmlResource.append_child( "File" );

            char xmlFilePath[MAX_PATH];
            sprintf( xmlFilePath, "Movies2/%s/%s"
                , provider->movie_name
                , resource_sound->path
            );

            xmlResourceFile.append_attribute( "Path" ).set_value( xmlFilePath );

            xmlResourceFile.append_attribute( "Codec" ).set_value( "oggSound" );

            //fprintf( f, "       <File Path = \"Movies2/%s/%s\"  Codec = \"oggSound\"/>\n"
            //    , provider->movie_name
            //    , resource_sound->path
            //);

            if( resource_sound->duration > 3.f )
            {
                xmlResource.append_child( "IsStreamable" ).append_attribute( "Value" ).set_value( 1U );
                //fprintf( f, "       <IsStreamable Value = \"1\"/>" );
            }

            //fprintf( f, "   </Resource>\n" );
        }break;
    case AE_MOVIE_RESOURCE_PARTICLE:
        {
            const aeMovieResourceParticle * resource_particle = (const aeMovieResourceParticle *)_resource;

            pugi::xml_node xmlResource = xmlDataBlock->append_child( "Resource" );
            xmlResource.append_attribute( "Name" ).set_value( resource_particle->name );
            xmlResource.append_attribute( "Type" ).set_value( "ResourceParticle" );
            xmlResource.append_attribute( "Unique" ).set_value( 0U );

//#if AE_MOVIE_SDK_MAJOR_VERSION >= 17
//            fprintf( f, "   <Resource Name = \"%s\" Type = \"ResourceParticle\" Unique = \"0\">\n"
//                , resource_particle->name
//            );
//#endif

            pugi::xml_node xmlResourceFile = xmlResource.append_child( "File" );

            char xmlFilePath[MAX_PATH];
            sprintf( xmlFilePath, "Movies2/%s/%s"
                , provider->movie_name
                , resource_particle->path
            );

            xmlResourceFile.append_attribute( "Path" ).set_value( xmlFilePath );

            //fprintf( f, "       <File Path = \"Movies2/%s/%s\"/>\n"
            //    , provider->movie_name
            //    , resource_particle->path
            //);

            xmlResource.append_child( "AtlasCount" ).append_attribute( "Value" ).set_value( resource_particle->image_count );

            //fprintf( f, "       <AtlasCount Value = \"%u\"/>\n"
            //    , resource_particle->image_count
            //);
            
            for( uint32_t i = 0; i != resource_particle->image_count; ++i )
            {
                const aeMovieResourceImage * image = resource_particle->images[i];

                pugi::xml_node xmlResourceAtlas = xmlResource.append_child( "Atlas" );
                
                xmlResourceAtlas.append_attribute( "Index" ).set_value( i );
                xmlResourceAtlas.append_attribute( "ResourceName" ).set_value( image->name );

//#if AE_MOVIE_SDK_MAJOR_VERSION >= 17
//                fprintf( f, "       <Atlas Index = \"%u\" ResourceName = \"%s\"/>\n"
//                    , i
//                    , image->name
//                );
//#endif
            }

            //fprintf( f, "   </Resource>\n" );
        }break;
    }

    *_rd = AE_NULL;
    return AE_TRUE;
}
//////////////////////////////////////////////////////////////////////////
struct node_provider_t
{
    pugi::xml_node * xmlComposition;
    CHAR movie_name[128];
};
//////////////////////////////////////////////////////////////////////////
static ae_bool_t __movie_composition_node_provider( const aeMovieNodeProviderCallbackData * _callbackData, ae_voidptrptr_t _nd, ae_voidptr_t _ud )
{
    node_provider_t * np = (node_provider_t *)_ud;

    const aeMovieLayerData * layer = _callbackData->layer;

    const char * layer_name = ae_get_movie_layer_data_name( layer );
    
    ae_bool_t is_track_matte = ae_is_movie_layer_data_track_mate( layer );

    if( is_track_matte == AE_TRUE )
    {
        *_nd = AE_NULL;

        return AE_TRUE;
    }

    aeMovieLayerTypeEnum type = ae_get_movie_layer_data_type( layer );

    switch( type )
    {
    case AE_MOVIE_LAYER_TYPE_TEXT:
        {
            pugi::xml_node xmlLayer = np->xmlComposition->append_child( "Layer" );

            xmlLayer.append_attribute( "Name" ).set_value( layer_name );
            xmlLayer.append_attribute( "Type" ).set_value( "TextField" );

            char xmlMatrix[256];
            sprintf( xmlMatrix, "%f;%f;%f;%f"";""%f;%f;%f;%f"";""%f;%f;%f;%f"";""%f;%f;%f;%f"
                , _callbackData->matrix[4 * 0 + 0]
                , _callbackData->matrix[4 * 0 + 1]
                , _callbackData->matrix[4 * 0 + 2]
                , _callbackData->matrix[4 * 0 + 3]
                , _callbackData->matrix[4 * 1 + 0]
                , _callbackData->matrix[4 * 1 + 1]
                , _callbackData->matrix[4 * 1 + 2]
                , _callbackData->matrix[4 * 1 + 3]
                , _callbackData->matrix[4 * 2 + 0]
                , _callbackData->matrix[4 * 2 + 1]
                , _callbackData->matrix[4 * 2 + 2]
                , _callbackData->matrix[4 * 2 + 3] 
                , _callbackData->matrix[4 * 3 + 0]
                , _callbackData->matrix[4 * 3 + 1]
                , _callbackData->matrix[4 * 3 + 2]
                , _callbackData->matrix[4 * 3 + 3] 
            );

            xmlLayer.append_attribute( "Matrix" ).set_value( xmlMatrix );

            char xmlColor[256];
            sprintf( xmlColor, "%f;%f;%f;%f"
                , _callbackData->color.r
                , _callbackData->color.g
                , _callbackData->color.b
                , _callbackData->opacity
            );
            
            xmlLayer.append_attribute( "Color" ).set_value( xmlColor );

            //if( ae_has_movie_layer_data_param( layer, AE_MOVIE_LAYER_PARAM_HORIZONTAL_CENTER ) == AE_TRUE )
            //{
            //    xmlNodeText.append_attribute( "AlignHorizontalCenter" ).set_value( 1U );
            //}

            //if( ae_has_movie_layer_data_param( layer, AE_MOVIE_LAYER_PARAM_VERTICAL_CENTER ) == AE_TRUE )
            //{
            //    xmlNodeText.append_attribute( "AlignVerticalCenter" ).set_value( 1U );
            //}

            *_nd = AE_NULL;

            return AE_TRUE;
        }break;
    case AE_MOVIE_LAYER_TYPE_SLOT:
        {
            pugi::xml_node xmlLayer = np->xmlComposition->append_child( "Layer" );

            xmlLayer.append_attribute( "Name" ).set_value( layer_name );
            xmlLayer.append_attribute( "Type" ).set_value( "Movie2Slot" );

            char xmlMatrix[256];
            sprintf( xmlMatrix, "%f;%f;%f;%f"";""%f;%f;%f;%f"";""%f;%f;%f;%f"";""%f;%f;%f;%f"
                , _callbackData->matrix[4 * 0 + 0]
                , _callbackData->matrix[4 * 0 + 1]
                , _callbackData->matrix[4 * 0 + 2]
                , _callbackData->matrix[4 * 0 + 3]
                , _callbackData->matrix[4 * 1 + 0]
                , _callbackData->matrix[4 * 1 + 1]
                , _callbackData->matrix[4 * 1 + 2]
                , _callbackData->matrix[4 * 1 + 3]
                , _callbackData->matrix[4 * 2 + 0]
                , _callbackData->matrix[4 * 2 + 1]
                , _callbackData->matrix[4 * 2 + 2]
                , _callbackData->matrix[4 * 2 + 3]
                , _callbackData->matrix[4 * 3 + 0]
                , _callbackData->matrix[4 * 3 + 1]
                , _callbackData->matrix[4 * 3 + 2]
                , _callbackData->matrix[4 * 3 + 3]
            );

            xmlLayer.append_attribute( "Matrix" ).set_value( xmlMatrix );

            char xmlColor[256];
            sprintf( xmlColor, "%f;%f;%f;%f"
                , _callbackData->color.r
                , _callbackData->color.g
                , _callbackData->color.b
                , _callbackData->opacity
            );

            xmlLayer.append_attribute( "Color" ).set_value( xmlColor );
            
            *_nd = AE_NULL;

            return AE_TRUE;
        }break;
    case AE_MOVIE_LAYER_TYPE_SOCKET:
        {
            pugi::xml_node xmlLayer = np->xmlComposition->append_child( "Layer" );

            xmlLayer.append_attribute( "Name" ).set_value( layer_name );
            xmlLayer.append_attribute( "Type" ).set_value( "HotSpotPolygon" );

            char xmlMatrix[256];
            sprintf( xmlMatrix, "%f;%f;%f;%f"";""%f;%f;%f;%f"";""%f;%f;%f;%f"";""%f;%f;%f;%f"
                , _callbackData->matrix[4 * 0 + 0]
                , _callbackData->matrix[4 * 0 + 1]
                , _callbackData->matrix[4 * 0 + 2]
                , _callbackData->matrix[4 * 0 + 3]
                , _callbackData->matrix[4 * 1 + 0]
                , _callbackData->matrix[4 * 1 + 1]
                , _callbackData->matrix[4 * 1 + 2]
                , _callbackData->matrix[4 * 1 + 3]
                , _callbackData->matrix[4 * 2 + 0]
                , _callbackData->matrix[4 * 2 + 1]
                , _callbackData->matrix[4 * 2 + 2]
                , _callbackData->matrix[4 * 2 + 3]
                , _callbackData->matrix[4 * 3 + 0]
                , _callbackData->matrix[4 * 3 + 1]
                , _callbackData->matrix[4 * 3 + 2]
                , _callbackData->matrix[4 * 3 + 3]
            );

            xmlLayer.append_attribute( "Matrix" ).set_value( xmlMatrix );

            char xmlColor[256];
            sprintf( xmlColor, "%f;%f;%f;%f"
                , _callbackData->color.r
                , _callbackData->color.g
                , _callbackData->color.b
                , _callbackData->opacity
            );

            xmlLayer.append_attribute( "Color" ).set_value( xmlColor );
          
            *_nd = AE_NULL;

            return AE_TRUE;
        }break;
    case AE_MOVIE_LAYER_TYPE_PARTICLE:
        {
            pugi::xml_node xmlLayer = np->xmlComposition->append_child( "Layer" );

            xmlLayer.append_attribute( "Name" ).set_value( layer_name );
            xmlLayer.append_attribute( "Type" ).set_value( "ParticleEmitter2" );

            char xmlMatrix[256];
            sprintf( xmlMatrix, "%f;%f;%f;%f"";""%f;%f;%f;%f"";""%f;%f;%f;%f"";""%f;%f;%f;%f"
                , _callbackData->matrix[4 * 0 + 0]
                , _callbackData->matrix[4 * 0 + 1]
                , _callbackData->matrix[4 * 0 + 2]
                , _callbackData->matrix[4 * 0 + 3]
                , _callbackData->matrix[4 * 1 + 0]
                , _callbackData->matrix[4 * 1 + 1]
                , _callbackData->matrix[4 * 1 + 2]
                , _callbackData->matrix[4 * 1 + 3]
                , _callbackData->matrix[4 * 2 + 0]
                , _callbackData->matrix[4 * 2 + 1]
                , _callbackData->matrix[4 * 2 + 2]
                , _callbackData->matrix[4 * 2 + 3]
                , _callbackData->matrix[4 * 3 + 0]
                , _callbackData->matrix[4 * 3 + 1]
                , _callbackData->matrix[4 * 3 + 2]
                , _callbackData->matrix[4 * 3 + 3]
            );

            xmlLayer.append_attribute( "Matrix" ).set_value( xmlMatrix );

            char xmlColor[256];
            sprintf( xmlColor, "%f;%f;%f;%f"
                , _callbackData->color.r
                , _callbackData->color.g
                , _callbackData->color.b
                , _callbackData->opacity
            );

            xmlLayer.append_attribute( "Color" ).set_value( xmlColor );

            *_nd = AE_NULL;

            return AE_TRUE;
        }break;
    case AE_MOVIE_LAYER_TYPE_SPRITE:
        {
            pugi::xml_node xmlLayer = np->xmlComposition->append_child( "Layer" );

            xmlLayer.append_attribute( "Name" ).set_value( layer_name );
            xmlLayer.append_attribute( "Type" ).set_value( "ShapeQuadFixed" );

            char xmlMatrix[256];
            sprintf( xmlMatrix, "%f;%f;%f;%f"";""%f;%f;%f;%f"";""%f;%f;%f;%f"";""%f;%f;%f;%f"
                , _callbackData->matrix[4 * 0 + 0]
                , _callbackData->matrix[4 * 0 + 1]
                , _callbackData->matrix[4 * 0 + 2]
                , _callbackData->matrix[4 * 0 + 3]
                , _callbackData->matrix[4 * 1 + 0]
                , _callbackData->matrix[4 * 1 + 1]
                , _callbackData->matrix[4 * 1 + 2]
                , _callbackData->matrix[4 * 1 + 3]
                , _callbackData->matrix[4 * 2 + 0]
                , _callbackData->matrix[4 * 2 + 1]
                , _callbackData->matrix[4 * 2 + 2]
                , _callbackData->matrix[4 * 2 + 3]
                , _callbackData->matrix[4 * 3 + 0]
                , _callbackData->matrix[4 * 3 + 1]
                , _callbackData->matrix[4 * 3 + 2]
                , _callbackData->matrix[4 * 3 + 3]
            );

            xmlLayer.append_attribute( "Matrix" ).set_value( xmlMatrix );

            char xmlColor[256];
            sprintf( xmlColor, "%f;%f;%f;%f"
                , _callbackData->color.r
                , _callbackData->color.g
                , _callbackData->color.b
                , _callbackData->opacity
            );

            xmlLayer.append_attribute( "Color" ).set_value( xmlColor );

            *_nd = AE_NULL;

            return AE_TRUE;
        }break;
    default:
        break;
    };
    
    *_nd = AE_NULL;

    return AE_TRUE;
}
//////////////////////////////////////////////////////////////////////////
static ae_bool_t __visit_sub_composition( const aeMovieComposition * _compositionData, const ae_char_t * _name, const aeMovieSubComposition * _subcomposition, ae_voidptr_t _ud )
{
    (void)_compositionData;
    (void)_subcomposition;

    node_provider_t * np = (node_provider_t *)_ud;

    pugi::xml_node xmlSubComposition = np->xmlComposition->append_child( "SubComposition" );

    xmlSubComposition.append_attribute( "Name" ).set_value( _name );

    return AE_TRUE;
}
//////////////////////////////////////////////////////////////////////////
static void my_resource_deleter( aeMovieResourceTypeEnum _type, ae_voidptr_t _data, ae_voidptr_t _ud )
{
    (void)_type;
    (void)_data;
    (void)_ud;
}
//////////////////////////////////////////////////////////////////////////
static size_t my_read_stream( void * _data, void * _buff, size_t _carriage, size_t _size )
{
    AE_UNUSED( _carriage );

    FILE * f = (FILE *)_data;

    ae_size_t s = fread( _buff, 1, _size, f );

    return s;
}
//////////////////////////////////////////////////////////////////////////
static void my_copy_stream( void * _data, const void * _src, void * _dst, size_t _size )
{
    AE_UNUSED( _data );

    memcpy( _dst, _src, _size );
}
//////////////////////////////////////////////////////////////////////////
int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nShowCmd )
{
	(void)hInstance;
	(void)hPrevInstance;
	(void)nShowCmd;
    
	std::wstring in_path = parse_kwds( lpCmdLine, L"--in_path", std::wstring() );
	std::wstring out_path = parse_kwds( lpCmdLine, L"--out_path", std::wstring() );
    std::wstring movie_name = parse_kwds( lpCmdLine, L"--movie_name", std::wstring() );
    std::wstring hash = parse_kwds( lpCmdLine, L"--hash", std::wstring() );
    
	if( in_path.empty() == true )
	{
		message_error("not found 'in' param\n"
			);

		return -1;
	}

    if( out_path.empty() == true )
    {
        message_error( "not found 'out' param\n"
        );

        return -1;
    }

    CHAR utf8_hash[64];
    unicode_to_utf8( utf8_hash, 64, hash.c_str(), hash.size() );

    CHAR utf8_movie_name[128];
    unicode_to_utf8( utf8_movie_name, 128, movie_name.c_str(), movie_name.size() );
    
    const aeMovieInstance *  movieInstance = ae_create_movie_instance( utf8_hash
        , &stdlib_movie_alloc
        , &stdlib_movie_alloc_n
        , &stdlib_movie_free
        , &stdlib_movie_free_n
        , 0
        , &stdlib_movie_logerror, AE_NULL );

    if( movieInstance == nullptr )
    {
        message_error( "invalid create movie instance\n"
        );

        return -1;
    }

    FILE * f_in = _wfopen( in_path.c_str(), L"rb" );

    if( f_in == NULL )
    {
        message_error( "invalid open in path '%ls'\n"
            , in_path.c_str()
        );

        return -1;
    }

    pugi::xml_document xmldoc;

    pugi::xml_node xmlDataBlock = xmldoc.append_child( "DataBlock" );
    pugi::xml_attribute xmlDataBlockName = xmlDataBlock.append_attribute( "Name" );
    xmlDataBlockName.set_value( utf8_movie_name );
    
    resource_provider_t rp;
    rp.xmlDataBlock = &xmlDataBlock;
    strcpy( rp.movie_name, utf8_movie_name );

    aeMovieData * movieData = ae_create_movie_data( movieInstance, &my_resource_provider, &my_resource_deleter, &rp );

    aeMovieStream * movieStream = ae_create_movie_stream( movieInstance, &my_read_stream, &my_copy_stream, f_in );

    ae_uint32_t major_version;
    ae_uint32_t minor_version;
    ae_result_t result_load_movie_data = ae_load_movie_data( movieData, movieStream, &major_version, &minor_version );

    pugi::xml_node xmlDataResource = xmlDataBlock.append_child( "Resource" );

    char xmlMovieName[256];
    sprintf( xmlMovieName, "Movie2_%s"
        , utf8_movie_name
    );

    xmlDataResource.append_attribute( "Name" ).set_value( xmlMovieName );
    xmlDataResource.append_attribute( "Type" ).set_value( "ResourceMovie2" );
        
    pugi::xml_node xmlDataResourceFile = xmlDataResource.append_child( "File" );

    char xmlMoviePath[MAX_PATH];
    sprintf( xmlMoviePath, "Movies2/%s/%s.aem"
        , utf8_movie_name
        , utf8_movie_name
    );

    xmlDataResourceFile.append_attribute( "Path" ).set_value( xmlMoviePath );

    ae_uint32_t movieDataCount = ae_get_movie_composition_data_count( movieData );

    for( ae_uint32_t index = 0; index != movieDataCount; ++index )
    {
        const aeMovieCompositionData * compositionData = ae_get_movie_composition_data_by_index( movieData, index );

        if( ae_is_movie_composition_data_master( compositionData ) == AE_FALSE )
        {
            continue;
        }

        const ae_char_t * compositionDataName = ae_get_movie_composition_data_name( compositionData );
        ae_float_t duration = ae_get_movie_composition_data_duration( compositionData );
        ae_float_t frame_duration = ae_get_movie_composition_data_frame_duration( compositionData );

        pugi::xml_node xmlDataResourceComposition = xmlDataResource.append_child( "Composition" );

        xmlDataResourceComposition.append_attribute( "Name" ).set_value( compositionDataName );
        xmlDataResourceComposition.append_attribute( "Duration" ).set_value( duration );
        xmlDataResourceComposition.append_attribute( "FrameDuration" ).set_value( frame_duration );       
        

        aeMovieCompositionProviders providers;
        ae_clear_movie_composition_providers( &providers );

        providers.node_provider = &__movie_composition_node_provider;

        node_provider_t np;
        np.xmlComposition = &xmlDataResourceComposition;
        strcpy( np.movie_name, utf8_movie_name );

        aeMovieComposition * composition = ae_create_movie_composition( movieData, compositionData, AE_TRUE, &providers, &np );
        
        ae_visit_movie_sub_composition( composition, __visit_sub_composition, &np );

        ae_delete_movie_composition( composition );
    }

    ae_delete_movie_stream( movieStream );

    fclose( f_in );

    ae_delete_movie_data( movieData );

    ae_delete_movie_instance( movieInstance );
    
    if( result_load_movie_data != AE_RESULT_SUCCESSFUL )
    {
        message_error( "invalid load movie data '%s'\n"
            , ae_get_result_string_info( result_load_movie_data )
        );

        return -1;
    }

    FILE * f_out = _wfopen( out_path.c_str(), L"wb" );

    if( f_out == NULL )
    {
        message_error( "invalid open out path '%ls'\n"
            , out_path.c_str()
        );

        return -1;
    }

    std::stringstream ss;
    xmldoc.save( ss );

    std::string s = ss.str();

    fprintf( f_out, s.c_str() );
    fclose( f_out );

	return 0;
}