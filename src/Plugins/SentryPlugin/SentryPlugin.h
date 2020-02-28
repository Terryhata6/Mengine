#pragma once

#include "Kernel/PluginBase.h"
#include "Kernel/Observable.h"

#include "sentry.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    class SentryPlugin
        : public PluginBase
        , public Observable
    {
        PLUGIN_DECLARE( "Sentry" )

    public:
        SentryPlugin();
        ~SentryPlugin() override;

    protected:
        bool _initializePlugin() override;
        void _finalizePlugin() override;
        void _destroyPlugin() override;

    protected:
        void notifyCreateApplication_();

    protected:
        sentry_options_t * m_options;
    };
}