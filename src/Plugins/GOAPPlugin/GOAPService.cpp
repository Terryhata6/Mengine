#include "GOAPService.h"

#include "Interface/AllocatorServiceInterface.h"

#include "Kernel/MemoryAllocator.h"

#include "GOAP/GOAP.h"
#include "GOAP/SourceProvider.h"

//////////////////////////////////////////////////////////////////////////
SERVICE_FACTORY( GOAPService, Mengine::GOAPService );
//////////////////////////////////////////////////////////////////////////
namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    class MengineAllocator
        : public GOAP::Allocator
    {
    protected:
        void * allocate( size_t _size ) override
        {
            void * p = ALLOCATOR_SERVICE()
                ->malloc( _size, "metabuf" );

            return p;
        }

        void deallocate( void * _ptr ) override
        {
            ALLOCATOR_SERVICE()
                ->free( _ptr, "metabuf" );
        }
    };
    //////////////////////////////////////////////////////////////////////////
    GOAPService::GOAPService()
        : m_allocator( nullptr )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    GOAPService::~GOAPService()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool GOAPService::_initializeService()
    {
        m_allocator = Helper::newT<MengineAllocator>();

        m_kernel = GOAP::Helper::makeKernel( m_allocator );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void GOAPService::_finalizeService()
    {
        m_kernel = nullptr;

        Helper::deleteT( m_allocator );
        m_allocator = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    GOAP::SourceInterfacePtr GOAPService::makeSource()
    {
        GOAP::SourceInterfacePtr source = m_kernel->makeSource();

        return source;
    }
    //////////////////////////////////////////////////////////////////////////
    GOAP::ChainInterfacePtr GOAPService::makeChain( const GOAP::SourceInterfacePtr & _source, const Char * _file, uint32_t _line )
    {
        GOAP::ChainInterfacePtr chain = m_kernel->makeChain( _source, _file, _line );

        return chain;
    }
    //////////////////////////////////////////////////////////////////////////
    GOAP::SemaphoreInterfacePtr GOAPService::makeSemaphore( int32_t _value )
    {
        GOAP::EventInterfacePtr event = m_kernel->makeEvent();

        GOAP::SemaphoreInterfacePtr semaphore = m_kernel->makeSemaphore( event, _value );

        return semaphore;
    }
}