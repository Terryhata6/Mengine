#	include "DX9RenderTargetOffscreen.h"

#	include "DX9RenderEnum.h"
#	include "DX9ErrorHelper.h"

namespace Menge
{
	DX9RenderTargetOffscreen::DX9RenderTargetOffscreen()
        : m_surfacePlain( nullptr )
	{
	}
	//////////////////////////////////////////////////////////////////////////
	DX9RenderTargetOffscreen::~DX9RenderTargetOffscreen()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool DX9RenderTargetOffscreen::_initialize()
	{
		LPDIRECT3DSURFACE9 surfacePlain;
		IF_DXCALL( m_serviceProvider, m_device, CreateOffscreenPlainSurface, (m_hwWidth, m_hwHeight, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surfacePlain, NULL) )
		{
			return false;
		}

		m_surfacePlain = surfacePlain;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void DX9RenderTargetOffscreen::_finalize()
	{
		m_surfacePlain->Release();
		m_surfacePlain = nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	bool DX9RenderTargetOffscreen::getData( unsigned char * _buffer, size_t _pitch )
	{
		if( m_surfacePlain == nullptr )
		{
			return false;
		}

		if( m_surface == nullptr )
		{
			return false;
		}

		DXCALL( m_serviceProvider, m_device, GetRenderTargetData, (m_surface, m_surfacePlain) );

		D3DLOCKED_RECT LockedRect;
		DXCALL( m_serviceProvider, m_surfacePlain, LockRect, ( &LockedRect, NULL, 0 ) );

		if( LockedRect.pBits == NULL || LockedRect.Pitch == 0 )
		{
			return false;
		}

		unsigned char * dstData = _buffer;
		unsigned char * srcData = (unsigned char *)LockedRect.pBits;

		for( DWORD i = 0; i != m_height; ++i )
		{
			stdex::memorycopy( dstData, 0, srcData, m_width * 4 );

			dstData += _pitch;
			srcData += LockedRect.Pitch;
		}

		DXCALL( m_serviceProvider, m_surfacePlain, UnlockRect, () );

		return true;
	}
}
