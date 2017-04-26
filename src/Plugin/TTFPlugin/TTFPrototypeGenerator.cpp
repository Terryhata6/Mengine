#	include "TTFPrototypeGenerator.h"

#	include "Factory/FactoryPool.h"

#	include "TTFFont.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	TTFPrototypeGenerator::TTFPrototypeGenerator()
		: m_ftlibrary( nullptr )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TTFPrototypeGenerator::~TTFPrototypeGenerator()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void TTFPrototypeGenerator::setFTLibrary( FT_Library _ftlibrary )
	{
		m_ftlibrary = _ftlibrary;
	}
	//////////////////////////////////////////////////////////////////////////
	bool TTFPrototypeGenerator::initialize( const ConstString & _category, const ConstString & _prototype )
	{
		(void)_category;
		(void)_prototype;

		m_factoryFont = new FactoryPool<TTFFont, 8>( m_serviceProvider );

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	PointerFactorable TTFPrototypeGenerator::generate()
	{
		TTFFont * font = m_factoryFont->createObject();

		font->setServiceProvider( m_serviceProvider );

		font->setFTLibrary( m_ftlibrary );

		return font;
	}
	//////////////////////////////////////////////////////////////////////////
	uint32_t TTFPrototypeGenerator::count() const
	{
		uint32_t count = m_factoryFont->countObject();

		return count;
	}
}