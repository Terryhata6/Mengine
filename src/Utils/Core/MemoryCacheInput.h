#	pragma once

#	include "Interface/ServiceInterface.h"
#	include "Interface/StreamInterface.h"

#	include "Core/ThreadGuard.h"

namespace Menge
{
	class MemoryCacheInput
		: public InputStreamInterface
	{
	public:
		MemoryCacheInput();
		~MemoryCacheInput();
		
	public:
		void setServiceProvider( ServiceProviderInterface * _serviceProvider );

    public:
        bool cacheMemory( size_t _size, const char * _doc );
		void * getMemory( size_t & _size ) const;

		template<class T>
		T getMemoryT( size_t & _size ) const
		{
			void * memory = this->getMemory( _size );

			return static_cast<T>(memory);
		}


	public:
		size_t read( void * _buf, size_t _size ) override;
		bool seek( size_t _pos ) override;
		bool skip( size_t _pos ) override;
		size_t tell() const override;
        size_t size() const override;
		bool eof() const override;

    public:
		bool time( uint64_t & _time ) const override;

	public:
		bool memory( void ** _memory, size_t * _size ) override;

	protected:
		void uncache_();

	protected:
		ServiceProviderInterface * m_serviceProvider;

		uint32_t m_bufferId;

		unsigned char * m_data;
		size_t m_size;

		unsigned char * m_pos;
		unsigned char * m_end;		

		THREAD_GUARD_INIT;
	};

	typedef stdex::intrusive_ptr<MemoryCacheInput> MemoryCacheInputPtr;
}	// namespace Menge


