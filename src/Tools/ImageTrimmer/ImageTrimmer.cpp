#	ifdef _WIN32_WINNT	
#       undef _WIN32_WINNT
#       define _WIN32_WINNT 0x0500
#   endif

#   ifdef _WIN32_WINDOWS
#       undef _WIN32_WINDOWS
#       define _WIN32_WINDOWS 0x0500
#   endif

#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>

#	include <shellapi.h>

#	include <stdio.h>

#	include <string>
#	include <vector>

#	include "Interface/PluginInterface.h"

#	include "Interface/StringizeInterface.h"
#	include "Interface/LogSystemInterface.h"
#	include "Interface/CodecInterface.h"
#	include "Interface/DataInterface.h"
#	include "Interface/MemoryInterface.h"
#   include "Interface/FileSystemInterface.h"
#   include "Interface/WindowsLayerInterface.h"
#   include "Interface/ImageCodecInterface.h"
#   include "Interface/UnicodeInterface.h"
#   include "Interface/ThreadSystemInterface.h"
#	include "Interface/ArchiveInterface.h"
#	include "Interface/ConfigInterface.h"

#   include "WindowsLayer/VistaWindowsLayer.h"

#	include "Logger/Logger.h"
	
//////////////////////////////////////////////////////////////////////////
extern "C" // only required if using g++
{
	extern bool initPluginMengeWin32FileGroup( Menge::PluginInterface ** _plugin );
	extern bool initPluginMengeImageCodec( Menge::PluginInterface ** _plugin );	
	extern bool initPluginMengeZip( Menge::PluginInterface ** _plugin );
	extern bool initPluginMengeLZ4( Menge::PluginInterface ** _plugin );
}
//////////////////////////////////////////////////////////////////////////
static void message_error( const char * _format, ... )
{
	va_list argList;

	va_start(argList, _format);

	char str[2048];

	vsnprintf( str, 2048 - 1, _format, argList );	

	va_end(argList);

	MessageBoxA( NULL, str, "ImageTrimmer", MB_OK );
}
//////////////////////////////////////////////////////////////////////////
SERVICE_EXTERN(ServiceProvider, Menge::ServiceProviderInterface);

SERVICE_EXTERN(StringizeService, Menge::StringizeServiceInterface);
SERVICE_EXTERN(LogService, Menge::LogServiceInterface);
SERVICE_EXTERN(CodecService, Menge::CodecServiceInterface);
SERVICE_EXTERN(DataService, Menge::DataServiceInterface);
SERVICE_EXTERN(MemoryService, Menge::MemoryServiceInterface);
SERVICE_EXTERN(ThreadSystem, Menge::ThreadSystemInterface);
SERVICE_EXTERN(ThreadService, Menge::ThreadServiceInterface);

SERVICE_EXTERN(UnicodeSystem, Menge::UnicodeSystemInterface);
SERVICE_EXTERN(UnicodeService, Menge::UnicodeServiceInterface);

SERVICE_EXTERN(ArchiveService, Menge::ArchiveServiceInterface);

SERVICE_EXTERN(FileService, Menge::FileServiceInterface);
SERVICE_EXTERN(ConfigService, Menge::ConfigServiceInterface);
//////////////////////////////////////////////////////////////////////////
namespace Menge
{
	class Win32Platform
		: public
	{ };

	static bool initializeEngine( Menge::ServiceProviderInterface ** _serviceProvider )
	{
		Menge::ServiceProviderInterface * serviceProvider;
		if( SERVICE_CREATE( ServiceProvider, &serviceProvider ) == false )
		{
			return false;
		}

		if( SERVICE_REGISTRY( m_serviceProvider, this ) == false )
		{
			return false;
		}

		UnicodeSystemInterface * unicodeSystem;
		if( SERVICE_CREATE( UnicodeSystem, &unicodeSystem ) == false )
		{
			return false;
		}

		if( SERVICE_REGISTRY(serviceProvider, unicodeSystem) == false )
		{
			return false;
		}

		UnicodeServiceInterface * unicodeService;
		if( SERVICE_CREATE( UnicodeService, &unicodeService ) == false )
		{
			return false;
		}

		if( SERVICE_REGISTRY(serviceProvider, unicodeService) == false )
		{
			return false;
		}

		StringizeServiceInterface * stringizeService;
		if( SERVICE_CREATE( StringizeService, &stringizeService ) == false )
		{
			return false;
		}

		if( SERVICE_REGISTRY(serviceProvider, stringizeService) == false )
		{
			return false;
		}

		ArchiveServiceInterface * archiveService;
		if( SERVICE_CREATE( ArchiveService, &archiveService ) == false )
		{
			return false;
		}

		if( SERVICE_REGISTRY(serviceProvider, archiveService) == false )
		{
			return false;
		}

		LogServiceInterface * logService;
		if( SERVICE_CREATE( LogService, &logService ) == false )
		{
			return false;
		}

		class MyLogger
			: public LoggerInterface
		{
		public:
			bool initialize() override 
			{ 
				return true; 
			}

			void finalize() override 
			{
			};

			void setServiceProvider(ServiceProviderInterface * _serviceProvider) override
			{
				m_serviceProvider = _serviceProvider;
			}

			ServiceProviderInterface * getServiceProvider() const override
			{
				return m_serviceProvider;
			}

			MyLogger()
				: m_verboseLevel(LM_WARNING)
				, m_verboseFlag(0xFFFFFFFF)
			{
			}

		public:
			void setVerboseLevel( EMessageLevel _level ) override 
			{
				m_verboseLevel = _level;
			};

			void setVerboseFlag( size_t _flag ) override 
			{
				m_verboseFlag = _flag;
			};

		public:
			bool validMessage( EMessageLevel _level, size_t _flag ) const override 
			{ 
				if( m_verboseLevel < _level )
				{
					return false;
				}

				if( _flag == 0 )
				{
					return true;
				}

				if( (m_verboseFlag & _flag) == 0 )
				{
					return false;
				}

				return true;
			};

		public:
			void log( EMessageLevel _level, size_t _flag, const char * _data, size_t _count ) override
			{
				(void)_level;
				(void)_flag;
				(void)_count;

				message_error("%s"
					, _data
					);
			}

			void flush() override 
			{
			}

		protected:
			EMessageLevel m_verboseLevel;
			uint32_t m_verboseFlag;

		private:
			ServiceProviderInterface * m_serviceProvider;
		};

		logService->setVerboseLevel( LM_WARNING );
		logService->registerLogger( new MyLogger );

		if( SERVICE_REGISTRY(serviceProvider, logService) == false )
		{
			return false;
		}

		CodecServiceInterface * codecService;
		if( SERVICE_CREATE( CodecService, &codecService ) == false )
		{
			return false;
		}

		if( SERVICE_REGISTRY(serviceProvider, codecService) == false )
		{
			return false;
		}

		DataServiceInterface * dataService;
		if( SERVICE_CREATE( DataService, &dataService ) == false )
		{
			return false;
		}

		dataService->initialize();

		if( SERVICE_REGISTRY(serviceProvider, dataService) == false )
		{
			return false;
		}

		ConfigServiceInterface * configService;
		if( SERVICE_CREATE( ConfigService, &configService ) == false )
		{
			return false;
		}

		SERVICE_REGISTRY(serviceProvider, configService);
		
		if( configService->initialize( Helper::stringizeString(serviceProvider, "WIN") ) == false )
		{
			return false;
		}
					
		ThreadSystemInterface * threadSystem;
		if( SERVICE_CREATE( ThreadSystem, &threadSystem ) == false )
		{
			return false;
		}

		if( SERVICE_REGISTRY(serviceProvider, threadSystem) == false )
		{
			return false;
		}

		if( threadSystem->initialize() == false )
		{
			return false;
		}

		ThreadServiceInterface * threadService;
		if( SERVICE_CREATE( ThreadService, &threadService ) == false )
		{
			return false;
		}

		if( SERVICE_REGISTRY( serviceProvider, threadService ) == false )
		{
			return false;
		}

		if( threadService->initialize( 16 ) == false )
		{
			return false;
		}

		MemoryServiceInterface * memoryService;
		if( SERVICE_CREATE( MemoryService, &memoryService ) == false )
		{
			return false;
		}

		if( SERVICE_REGISTRY( serviceProvider, memoryService ) == false )
		{
			return false;
		}

		if( memoryService->initialize() == false )
		{
			return false;
		}

		WindowsLayerInterface * windowsLayer = new VistaWindowsLayer();

		if( SERVICE_REGISTRY(serviceProvider, windowsLayer) == false )
		{
			return false;
		}

		FileServiceInterface * fileService;
		if( SERVICE_CREATE( FileService, &fileService ) == false )
		{
			return false;
		}

		if( SERVICE_REGISTRY(serviceProvider, fileService) == false )
		{
			return false;
		}

		if( fileService->initialize() == false )
		{
			return false;
		}

		PluginInterface * plugin_win32_file_group;
		initPluginMengeWin32FileGroup( &plugin_win32_file_group );

		if( plugin_win32_file_group == nullptr )
		{
			return false;
		}

		plugin_win32_file_group->initialize( serviceProvider );	

		PluginInterface * plugin_zip;
		initPluginMengeZip( &plugin_zip );
		plugin_zip->initialize( serviceProvider );

		PluginInterface * plugin_lz4;
		initPluginMengeLZ4( &plugin_lz4 );
		plugin_lz4->initialize( serviceProvider );
		
		PluginInterface * plugin_image_codec;
		initPluginMengeImageCodec( &plugin_image_codec );

		if( plugin_image_codec == nullptr )
		{
			return false;
		}

		plugin_image_codec->initialize( serviceProvider );

		if( FILE_SERVICE(serviceProvider)
			->mountFileGroup( ConstString::none(), ConstString::none(), Helper::stringizeString(serviceProvider, "dir") ) == false )
		{
			return false;
		}

		ConstString dev = Helper::stringizeString(serviceProvider, "dev");

		if( FILE_SERVICE(serviceProvider)
			->mountFileGroup( dev, ConstString::none(), Helper::stringizeString(serviceProvider, "dir") ) == false )
		{
			return false;
		}

		*_serviceProvider = serviceProvider;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool trimImage( Menge::ServiceProviderInterface * serviceProvider, const WString & in, const WString & out, const WString & info )
	{
		String utf8_in;
		Helper::unicodeToUtf8(serviceProvider, in, utf8_in);

		ConstString c_in = Helper::stringizeString(serviceProvider, utf8_in);

		InputStreamInterfacePtr input_stream = 
			FILE_SERVICE(serviceProvider)->openInputFile( ConstString::none(), c_in, false );

		if( input_stream == nullptr )
		{
			return false;
		}

		const ConstString & codecType = CODEC_SERVICE(serviceProvider)
			->findCodecType( c_in );

		if( codecType.empty() == true )
		{
			return false;
		}

		ImageDecoderInterfacePtr imageDecoder = CODEC_SERVICE(serviceProvider)
			->createDecoderT<ImageDecoderInterfacePtr>( codecType );

		if( imageDecoder == nullptr )
		{
			return false;
		}

		if( imageDecoder->prepareData( input_stream ) == false )
		{
			return false;
		}

		const ImageCodecDataInfo* decode_dataInfo = imageDecoder->getCodecDataInfo();

		if( decode_dataInfo->width == 0 ||
			decode_dataInfo->height == 0 )
		{
			return false;
		}

		ImageCodecOptions decode_options;

		decode_options.channels = decode_dataInfo->channels;
		decode_options.pitch = decode_dataInfo->width * decode_dataInfo->channels;

		imageDecoder->setOptions( &decode_options );

		size_t width = decode_dataInfo->width;
		size_t height = decode_dataInfo->height;
		
		size_t bufferSize = width * height * decode_dataInfo->channels;

		MemoryInterfacePtr memory_textureBuffer = MEMORY_SERVICE( serviceProvider )
			->createMemory();

		if( memory_textureBuffer == nullptr )
		{
			return false;
		}

		unsigned char * textureBuffer = memory_textureBuffer->newMemory( bufferSize );

		if( textureBuffer == nullptr )
		{
			return false;
		}

		if( imageDecoder->decode( textureBuffer, bufferSize ) == 0U )
		{
			return false; 
		}

		size_t new_width;
		size_t new_height;

		size_t min_i;
		size_t min_j;
		size_t max_i;
		size_t max_j;
		
		if( decode_dataInfo->channels == 4 )
		{
			min_i = width - 1;
			min_j = height - 1;
			max_i = 0;
			max_j = 0;

			for( size_t i = 0; i != width; ++i )
			{
				for( size_t j = 0; j != height; ++j )
				{
					size_t index =  i + j * width;
					unsigned char alpha = textureBuffer[index * 4 + 3];

					if( alpha == 0 )
					{
						continue;
					}

					if( min_i > i )
					{
						min_i = i;
					}

					if( min_j > j )
					{
						min_j = j;
					}

					if( max_i < i )
					{
						max_i = i;
					}

					if( max_j < j )
					{
						max_j = j;
					}
				}
			}

			if( min_i > max_i || min_j > max_j )
			{
				new_width = 1;
				new_height = 1;

				min_i = 0;
				min_j = 0;
				max_i = 1;
				max_j = 1;
			}
			else
			{
				new_width = max_i - min_i + 1;
				new_height = max_j - min_j + 1;
			}
		}
		else
		{
			new_width = width;
			new_height = height;

			min_i = 0;
			min_j = 0;
			max_i = width;
			max_j = height;
		}

		size_t new_bufferSize = new_width * new_height * decode_dataInfo->channels;

		MemoryInterfacePtr buffer = MEMORY_SERVICE( serviceProvider )
			->createMemory();

		if( buffer == nullptr )
		{
			return false;
		}

		unsigned char * new_textureBuffer = buffer->newMemory( new_bufferSize );

		if( new_textureBuffer == nullptr )
		{
			return false;
		}
		
		for( size_t i = 0; i != new_width; ++i )
		{
			for( size_t j = 0; j != new_height; ++j )
			{
				size_t old_index = (min_i + i) + (min_j + j) * width;
				size_t new_index = i + j * new_width;

				for( size_t k = 0; k != decode_dataInfo->channels; ++k )
				{
					new_textureBuffer[new_index * decode_dataInfo->channels + k] = textureBuffer[old_index * decode_dataInfo->channels + k];
				}
			}
		}

		String utf8_out;
		Helper::unicodeToUtf8(serviceProvider, out, utf8_out);

		ConstString c_out = Helper::stringizeString(serviceProvider, utf8_out);

		OutputStreamInterfacePtr output_stream = FILE_SERVICE(serviceProvider)
			->openOutputFile( ConstString::none(), c_out );

		if( output_stream == nullptr )
		{
			return false;
		}

		ImageEncoderInterfacePtr imageEncoder = CODEC_SERVICE(serviceProvider)
			->createEncoderT<ImageEncoderInterfacePtr>( codecType );

		if( imageEncoder == nullptr )
		{
			return false;
		}

		if( imageEncoder->initialize( output_stream ) == false )
		{
			return false;
		}

		ImageCodecOptions encode_options;

		encode_options.pitch = new_width * decode_dataInfo->channels;
		encode_options.channels = decode_dataInfo->channels;

		imageEncoder->setOptions( &encode_options );

		ImageCodecDataInfo encode_dataInfo;
		//dataInfo.format = _image->getHWPixelFormat();
		encode_dataInfo.width = new_width;
		encode_dataInfo.height = new_height;
		encode_dataInfo.channels = decode_dataInfo->channels;
		encode_dataInfo.depth = 1;
		encode_dataInfo.mipmaps = 1;

		if( imageEncoder->encode( new_textureBuffer, new_bufferSize, &encode_dataInfo ) == 0 )
		{
			return false;
		}

		String utf8_info;
		Helper::unicodeToUtf8(serviceProvider, info, utf8_info);

		ConstString c_info = Helper::stringizeString(serviceProvider, utf8_info);

		OutputStreamInterfacePtr info_stream = FILE_SERVICE(serviceProvider)
			->openOutputFile( ConstString::none(), c_info );

		if( info_stream == nullptr )
		{
			return false;
		}

		char info_buffer[1024];

		sprintf(info_buffer, "%u\n%u\n%u\n%u\n%u\n%u\n"
			, width
			, height
			, new_width
			, new_height
			, min_i
			, min_j
			);

		size_t info_buffer_size = strlen( info_buffer );

		if( info_stream->write( info_buffer, info_buffer_size + 1 ) == false )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool recanvasImage( Menge::ServiceProviderInterface * serviceProvider, const WString & in, const WString & out, const WString & params )
	{
		String utf8_in;
		Helper::unicodeToUtf8(serviceProvider, in, utf8_in);

		ConstString c_in = Helper::stringizeString(serviceProvider, utf8_in);

		InputStreamInterfacePtr input_stream = 
			FILE_SERVICE(serviceProvider)->openInputFile( ConstString::none(), c_in, false );

		if( input_stream == nullptr )
		{
			return false;
		}

		const ConstString & codecType = CODEC_SERVICE(serviceProvider)
			->findCodecType( c_in );

		if( codecType.empty() == true )
		{
			return false;
		}

		ImageDecoderInterfacePtr imageDecoder = CODEC_SERVICE(serviceProvider)
			->createDecoderT<ImageDecoderInterfacePtr>( codecType );

		if( imageDecoder == nullptr )
		{
			return false;
		}

		if( imageDecoder->prepareData( input_stream ) == false )
		{
			return false;
		}

		const ImageCodecDataInfo* decode_dataInfo = imageDecoder->getCodecDataInfo();

		if( decode_dataInfo->width == 0 ||
			decode_dataInfo->height == 0 )
		{
			return false;
		}

		ImageCodecOptions decode_options;

		decode_options.channels = decode_dataInfo->channels;
		decode_options.pitch = decode_dataInfo->width * decode_dataInfo->channels;

		imageDecoder->setOptions( &decode_options );

		size_t width = decode_dataInfo->width;
		size_t height = decode_dataInfo->height;

		size_t bufferSize = width * height * decode_dataInfo->channels;

		MemoryInterfacePtr memory_textureBuffer = MEMORY_SERVICE( serviceProvider )
			->createMemory();
		
		if( memory_textureBuffer == nullptr )
		{
			return false;
		}

		unsigned char * textureBuffer = memory_textureBuffer->newMemory( bufferSize );

		if( imageDecoder->decode( textureBuffer, bufferSize ) == 0U )
		{
			return false;
		}

		uint32_t new_width;
		uint32_t new_height;

		if( params == L"pow2" )
		{
			new_width = Helper::getTexturePOW2( width );
			new_height = Helper::getTexturePOW2( height );
		}
		else
		{
			if( swscanf( params.c_str(), L"%d;%d", &new_width, &new_height ) != 2 )
			{
				return false;
			}
		}

		if( new_width < width || new_height < height )
		{
			return false;
		}
		
		size_t new_bufferSize = new_width * new_height * decode_dataInfo->channels;

		MemoryInterfacePtr memory_new_textureBuffer = MEMORY_SERVICE( serviceProvider )
			->createMemory();

		if( memory_new_textureBuffer == nullptr )
		{
			return false;
		}

		unsigned char * new_textureBuffer = memory_new_textureBuffer->newMemory( new_bufferSize );

		if( new_textureBuffer == nullptr )
		{
			return false;
		}
		
		uint32_t channels = decode_dataInfo->channels;

		for( size_t i = 0; i != width; ++i )
		{
			for( size_t j = 0; j != height; ++j )
			{
				size_t old_index = i + j * width;
				size_t new_index = i + j * new_width;

				for( size_t k = 0; k != channels; ++k )
				{
					new_textureBuffer[new_index * channels + k] = textureBuffer[old_index * channels + k];
				}
			}
		}

		String utf8_out;
		Helper::unicodeToUtf8(serviceProvider, out, utf8_out);

		ConstString c_out = Helper::stringizeString(serviceProvider, utf8_out);

		OutputStreamInterfacePtr output_stream = FILE_SERVICE(serviceProvider)
			->openOutputFile( ConstString::none(), c_out );

		if( output_stream == nullptr )
		{
			return false;
		}

		ImageEncoderInterfacePtr imageEncoder = CODEC_SERVICE(serviceProvider)
			->createEncoderT<ImageEncoderInterfacePtr>( codecType );

		if( imageEncoder == nullptr )
		{
			return false;
		}

		if( imageEncoder->initialize( output_stream ) == false )
		{
			return false;
		}

		ImageCodecOptions encode_options;

		encode_options.pitch = new_width * decode_dataInfo->channels;
		encode_options.channels = decode_dataInfo->channels;

		imageEncoder->setOptions( &encode_options );

		ImageCodecDataInfo encode_dataInfo;
		//dataInfo.format = _image->getHWPixelFormat();
		encode_dataInfo.width = new_width;
		encode_dataInfo.height = new_height;
		encode_dataInfo.channels = decode_dataInfo->channels;
		encode_dataInfo.depth = 1;
		encode_dataInfo.mipmaps = 1;

		if( imageEncoder->encode( new_textureBuffer, new_bufferSize, &encode_dataInfo ) == 0 )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool lz4hcImage( Menge::ServiceProviderInterface * serviceProvider, const WString & in, const WString & out )
	{
		String utf8_in;
		Helper::unicodeToUtf8(serviceProvider, in, utf8_in);

		ConstString c_in = Helper::stringizeString(serviceProvider, utf8_in);

		InputStreamInterfacePtr input_stream = 
			FILE_SERVICE(serviceProvider)->openInputFile( ConstString::none(), c_in, false );

		if( input_stream == nullptr )
		{
			return false;
		}

		size_t bufferSize = input_stream->size();

		MemoryInterfacePtr memory_textureBuffer = MEMORY_SERVICE( serviceProvider )
			->createMemory();

		if( memory_textureBuffer == nullptr )
		{
			return false;
		}

		unsigned char * textureBuffer = memory_textureBuffer->newMemory( bufferSize );

		if( textureBuffer == nullptr )
		{
			return false;
		}

		if( input_stream->read( textureBuffer, bufferSize ) == 0U )
		{
			return false;
		}

		ArchivatorInterfacePtr archivator = ARCHIVE_SERVICE(serviceProvider)
			->getArchivator( STRINGIZE_STRING_LOCAL(serviceProvider, "lz4") );

		if( archivator == nullptr )
		{
			return false;
		}

		size_t compressBound = archivator->compressBound( bufferSize );

		MemoryInterfacePtr memory_compressBuffer = MEMORY_SERVICE( serviceProvider )
			->createMemory();

		if( memory_compressBuffer == nullptr )
		{
			return false;
		}

		unsigned char * compressBuffer = memory_compressBuffer->newMemory( compressBound );

		if( compressBuffer == nullptr )
		{
			return false;
		}

		size_t compressSize = 0;
		archivator->compress( compressBuffer, compressBound, textureBuffer, bufferSize, compressSize );
		
		String utf8_out;
		Helper::unicodeToUtf8(serviceProvider, out, utf8_out);

		ConstString c_out = Helper::stringizeString(serviceProvider, utf8_out);

		OutputStreamInterfacePtr output_stream = FILE_SERVICE(serviceProvider)
			->openOutputFile( ConstString::none(), c_out );

		if( output_stream == nullptr )
		{
			return false;
		}
		
		if( output_stream->write( compressBuffer, compressSize ) == 0 )
		{
			return false;
		}

		return true;
	}	
}

static Menge::WString s_correct_path( const Menge::WString & _path )
{
	Menge::WString true_path = _path;

	if( _path.size() <= 2 )
	{

	}
	else if( _path[0] == L'/' && _path[1] == L'/' )
	{

	}
	else if( _path[0] == L'/' )
	{
		true_path[0] = _path[1];
		true_path[1] = L':';
	}

	return true_path;
}

int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nShowCmd )
{
	(void)hInstance;
	(void)hPrevInstance;
	(void)nShowCmd;

	int cmd_num;
	LPWSTR * cmd_args = CommandLineToArgvW( lpCmdLine, &cmd_num );

	Menge::WString command;	
	Menge::WString in;
	Menge::WString out;
	Menge::WString params;
	Menge::WString info;

	for( int i = 0; i < cmd_num; i += 2 )
	{
		LPWSTR arg = cmd_args[i + 0];
		LPWSTR value = cmd_args[i + 1];

		if( wcscmp( arg, L"-command" ) == 0 )
		{
			command = value;
		}
		else if( wcscmp( arg, L"-in" ) == 0 )
		{
			in = value;
		}
		else if( wcscmp( arg, L"-out" ) == 0 )
		{
			out = value;
		}
		else if( wcscmp( arg, L"-params" ) == 0 )
		{
			params = value;
		}
		else if( wcscmp( arg, L"-info" ) == 0 )
		{
			info = value;
		}
	}

	if( command.empty() == true )
	{
		command = L"trim";
	}

	if( in.empty() == true )
	{
		message_error("not found 'in' param"
			);

		return 0;
	}

	if( out.empty() == true )
	{
		message_error("not found 'out' param"
			);

		return 0;
	}

	in = s_correct_path(in);
	out = s_correct_path(out);
	info = s_correct_path(info);

	Menge::ServiceProviderInterface * serviceProvider;

	try
	{
		if( Menge::initializeEngine( &serviceProvider ) == false )
		{
			message_error( "ImageTrimmer invalid initialize" );

			return 0;
		}
	}
	catch( const std::exception & se )
	{		
		MessageBoxA( NULL, se.what(), "Mengine exception", MB_OK );

		return 0;
	}

	if( command == L"trim" )
	{
		if( Menge::trimImage( serviceProvider, in, out, info ) == false )
		{
			message_error( "ImageTrimmer invalid trim %ls"
				, in.c_str()
				);

			return 0;
		}
	}
	else if( command == L"recanvas" )
	{
		if( Menge::recanvasImage( serviceProvider, in, out, params ) == false )
		{
			message_error( "ImageTrimmer invalid recanvas %ls"
				, in.c_str()
				);

			return 0;
		}		
	}
	else if( command == L"lz4hc" )
	{
		if( Menge::lz4hcImage( serviceProvider, in, out ) == false )
		{
			message_error( "ImageTrimmer invalid lz4hc %ls"
				, in.c_str()
				);

			return 0;
		}		
	}


	return 0;
}