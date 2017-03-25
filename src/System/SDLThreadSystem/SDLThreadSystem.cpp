#	include "SDLThreadSystem.h"

#	include "SDLThreadIdentity.h"
#   include "SDLThreadMutex.h"

#   include "Factory/FactoryPool.h"

#	include "Logger/Logger.h"

//////////////////////////////////////////////////////////////////////////
SERVICE_FACTORY( ThreadSystem, Menge::SDLThreadSystem );
//////////////////////////////////////////////////////////////////////////
namespace Menge
{
    //////////////////////////////////////////////////////////////////////////
    SDLThreadSystem::SDLThreadSystem()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    SDLThreadSystem::~SDLThreadSystem()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool SDLThreadSystem::_initialize()
    {	
        m_poolThreadIdentity = new FactoryPool<SDLThreadIdentity, 16>( m_serviceProvider );
        m_poolThreadMutex = new FactoryPool<SDLThreadMutex, 16>( m_serviceProvider );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void SDLThreadSystem::_finalize()
    {
        m_poolThreadIdentity = nullptr;
        m_poolThreadMutex = nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    bool SDLThreadSystem::avaliable() const
    {
        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    ThreadIdentityInterfacePtr SDLThreadSystem::createThread( int _priority, const char * _doc)
    {
        SDLThreadIdentityPtr identity = m_poolThreadIdentity->createObject();

        if( identity == nullptr )
        {
            LOGGER_ERROR( m_serviceProvider )("SDLThreadSystem::createThread invalid initialize (doc: '%s')"
                , _doc
                );

            return nullptr;
        }

        ThreadMutexInterfacePtr mutex = this->createMutex(_doc);

        if( identity->initialize( mutex, _priority, _doc ) == false )
        {
            LOGGER_ERROR(m_serviceProvider)("SDLThreadSystem::createThread invalid initialize (doc: '%s')"
                , _doc
                );

            return nullptr;
        }

        return identity;
    }
    //////////////////////////////////////////////////////////////////////////
    void SDLThreadSystem::sleep( uint32_t _ms )
    {
        SDL_Delay( _ms );
    }
    //////////////////////////////////////////////////////////////////////////
    ThreadMutexInterfacePtr SDLThreadSystem::createMutex(const char * _doc)
    {
        SDLThreadMutexPtr mutex = m_poolThreadMutex->createObject();
        
        if( mutex == nullptr )
        {
            LOGGER_ERROR( m_serviceProvider )("MarmaladeThreadSystem::createMutex invalid create (doc: '%s')"
                , _doc
                );

            return nullptr;
        }

        mutex->setServiceProvider( m_serviceProvider );

        if( mutex->initialize( _doc ) == false )
        {
            LOGGER_ERROR(m_serviceProvider)("MarmaladeThreadSystem::createMutex invalid initialize (doc: '%s')"
                , _doc
                );

            return nullptr;
        }

        return mutex;
    }
    //////////////////////////////////////////////////////////////////////////
    ptrdiff_t SDLThreadSystem::getCurrentThreadId() const
    {
        const SDL_threadID threadId = SDL_ThreadID();

        return static_cast<ptrdiff_t>(threadId);
    }
}
