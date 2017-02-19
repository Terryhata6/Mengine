#	include "PluginBase.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	PluginBase::PluginBase()
		: m_serviceProvider( nullptr )
		, m_dynamicLoad( false )
		, m_initialize( false )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	PluginBase::~PluginBase()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void PluginBase::setServiceProvider( ServiceProviderInterface * _serviceProvider )
	{
		m_serviceProvider = _serviceProvider;
	}
	//////////////////////////////////////////////////////////////////////////
	ServiceProviderInterface * PluginBase::getServiceProvider() const
	{
		return m_serviceProvider;
	}
	//////////////////////////////////////////////////////////////////////////
	void PluginBase::setDynamicLoad( bool _dynamicLoad )
	{
		m_dynamicLoad = _dynamicLoad;
	}
	//////////////////////////////////////////////////////////////////////////
	bool PluginBase::isDynamicLoad() const
	{
		return m_dynamicLoad;
	}
	//////////////////////////////////////////////////////////////////////////
	bool PluginBase::initialize()
	{
		if( m_initialize == true )
		{
			return true;
		}

		bool successful = this->_initialize();

		m_initialize = successful;

		return m_initialize;
	}
	//////////////////////////////////////////////////////////////////////////
	void PluginBase::finalize()
	{
		if( m_initialize == false )
		{
			return;
		}

		m_initialize = false;

		this->_finalize();
	}
	//////////////////////////////////////////////////////////////////////////
	bool PluginBase::isInitialize() const
	{
		return m_initialize;
	}
	//////////////////////////////////////////////////////////////////////////
	void PluginBase::destroy()
	{
		this->_destroy();

        bool dynamicLoad = m_dynamicLoad;

        delete this;

		if( dynamicLoad == true )
		{
			stdex_allocator_finalize();
		}        
	}
	//////////////////////////////////////////////////////////////////////////
	bool PluginBase::_initialize()
	{
		///Empty

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void PluginBase::_finalize()
	{
		//Empty
	}
	//////////////////////////////////////////////////////////////////////////
	void PluginBase::_destroy()
	{
		//Empty
	}
}