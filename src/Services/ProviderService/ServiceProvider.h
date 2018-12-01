#pragma once

#include "Interface/PluginInterface.h"

#ifndef MENGINE_SERVICE_PROVIDER_NAME_SIZE
#define MENGINE_SERVICE_PROVIDER_NAME_SIZE 64
#endif

#ifndef MENGINE_SERVICE_PROVIDER_COUNT
#define MENGINE_SERVICE_PROVIDER_COUNT 64
#endif

#ifndef MENGINE_SERVICE_PROVIDER_MAX_WAIT
#define MENGINE_SERVICE_PROVIDER_MAX_WAIT 128
#endif

#ifndef MENGINE_SERVICE_PROVIDER_DEPENDENCY_COUNT
#define MENGINE_SERVICE_PROVIDER_DEPENDENCY_COUNT 512
#endif

namespace Mengine
{
    class ServiceProvider
        : public ServiceProviderInterface
    {
    public:
        ServiceProvider();
        ~ServiceProvider() override;

    public:
        bool existService( const Char * _name ) const override;

    protected:
        const ServiceInterfacePtr & getService( const Char * _name ) const override;

    protected:
        ServiceInterfacePtr generateService( TServiceProviderGenerator _generator ) override;
        bool initializeService( TServiceProviderGenerator _generator, const Char * _doc, const Char * _file, int _line ) override;
        bool finalizeService( const Char * _name ) override;

        bool destroyService( const Char * _name ) override;

    public:
        void dependencyService( const Char * _name, const Char * _dependency ) override;
        void waitService( const Char * _name, const LambdaWaitService & _lambda ) override;

    protected:
        void stopServices() override;

    protected:
        void destroy() override;

    protected:
        void removeDependency_( const Char * _name );
        void checkWaits_( const Char * _name, const ServiceInterfacePtr & _service );

    protected:
        struct ServiceDesc
        {
            Char name[MENGINE_SERVICE_PROVIDER_NAME_SIZE];
            ServiceInterfacePtr service;
        };

        ServiceDesc m_services[MENGINE_SERVICE_PROVIDER_COUNT];

        uint32_t m_servicesCount;

        struct DependencyDesc
        {
            Char name[MENGINE_SERVICE_PROVIDER_NAME_SIZE];
            Char dependency[MENGINE_SERVICE_PROVIDER_NAME_SIZE];
        };

        DependencyDesc m_dependencies[MENGINE_SERVICE_PROVIDER_DEPENDENCY_COUNT];

        uint32_t m_dependenciesCount;

        struct WaitDesc
        {
            Char name[MENGINE_SERVICE_PROVIDER_NAME_SIZE];
            LambdaWaitService lambda;
        };

        WaitDesc m_waits[MENGINE_SERVICE_PROVIDER_MAX_WAIT];

        uint32_t m_waitsCount;
    };
}