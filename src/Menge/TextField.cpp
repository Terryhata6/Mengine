#	include "TextField.h" 

//#	include "Application.h"

#	include "Interface/RenderSystemInterface.h"
#	include "Interface/ResourceInterface.h"
#	include "Interface/TextInterface.h"

#	include "Consts.h"

#	include "Logger/Logger.h"

#	include "Core/String.h"
#	include "Core/UnicodeFormat.h"

#	include "math/box2.h"

#	include "pybind/system.hpp"

#	include <algorithm>

#	include <stdio.h>

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	TextField::TextField()
		: m_textSize(0.f, 0.f)
		, m_invalidateFont(true)
		, m_fontParams(EFP_NONE)
		, m_horizontAlign(ETFHA_NONE)
		, m_verticalAlign(ETFVA_NONE)
		, m_maxWidth(2048.f)
		, m_maxCharCount((size_t)-1)
		, m_charCount(0)
		, m_charOffset(0.f)
		, m_lineOffset(0.f)
		, m_outline(false)
		, m_pixelsnap(true)
		, m_materialFont(nullptr)
		, m_materialOutline(nullptr)
		, m_invalidateVertices(true)
        , m_invalidateVerticesWM(true)
		, m_invalidateTextLines(true)
	{
	}
	//////////////////////////////////////////////////////////////////////////
	TextField::~TextField()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	bool TextField::_activate()
	{
		if( Node::_activate() == false )
		{
			return false;
		}

        this->invalidateTextLines();

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::_deactivate()
	{
		Node::_deactivate();
	}
	//////////////////////////////////////////////////////////////////////////
	bool TextField::_compile()
	{
		this->invalidateFont();
		
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::_release()
	{
		TEXT_SERVICE(m_serviceProvider)
			->releaseFont( m_font );

        m_font = nullptr;

		m_materialFont = nullptr;
		m_materialOutline = nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	const TVectorTextLine & TextField::getTextLines() const
	{
		if( this->isInvalidateTextLines() == true )
		{
			this->updateTextLines_();
		}

		return m_lines;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::_invalidateWorldMatrix()
	{
		Node::_invalidateWorldMatrix();

		this->invalidateVerticesWM_();
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::updateVertexData_( const ColourValue & _color, TVectorRenderVertex2D& _vertexData )
	{
		_vertexData.clear();

		const TVectorTextLine & lines = this->getTextLines();
       
		const TextFontInterfacePtr & font = this->getFont();

		if( font == nullptr )
		{
			return;
		}

		float fontHeght = font->getFontHeight();
		const RenderTextureInterfacePtr & textureFont = font->getTextureFont();
		
		const mt::vec4f & uv = textureFont->getUV();

		float lineOffset = this->calcLineOffset();

		mt::vec2f offset(0.f, 0.f);
		switch( m_verticalAlign )
		{
		case ETFVA_NONE:
			{
				offset.y = 0.f;
			}break;
		case ETFVA_CENTER:
			{
				offset.y = -(fontHeght + lineOffset) * lines.size() * 0.5f;
			}break;
		}

		for( TVectorTextLine::const_iterator
			it_line = lines.begin(),
			it_line_end = lines.end();
		it_line != it_line_end;
		++it_line )
		{
			const TextLine & line = *it_line;

            float alignOffsetX = this->getHorizontAlignOffset_( line );
			offset.x = alignOffsetX;

			ARGB argb = _color.getAsARGB();

			line.prepareRenderObject( offset, uv, argb, m_pixelsnap, _vertexData );

            offset.y += fontHeght;
			offset.y += lineOffset;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::_render( const RenderViewportInterface * _viewport, const RenderCameraInterface * _camera )
	{	
		Node::_render( _viewport, _camera );

		this->renderOutline_( _viewport, _camera );

		const RenderMaterialInterfacePtr & material = this->getMaterialFont();

		if( material == nullptr )
		{
			return;
		}

		TVectorRenderVertex2D & textVertices = this->getTextVertices();
		
        if( textVertices.empty() == true )
        {
            return;
        }

		size_t countVertex;
		
		if( m_maxCharCount == (size_t)-1 )
		{
			countVertex = textVertices.size();
		}
		else
		{
			countVertex = m_maxCharCount * 4;
		}

		TVectorRenderVertex2D::value_type * vertices = &(textVertices[0]);

        RENDER_SERVICE(m_serviceProvider)
            ->addRenderQuad( _viewport, _camera, material, vertices, countVertex, nullptr );
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::renderOutline_( const RenderViewportInterface * _viewport, const RenderCameraInterface * _camera )
	{
		if( m_outline == false )
		{
			return;
		}

		const RenderMaterialInterfacePtr & material = this->getMaterialOutline();

		if( material == nullptr )
		{
			return;
		}

		TVectorRenderVertex2D & outlineVertices = this->getOutlineVertices();

		if( outlineVertices.empty() == true )
		{
			return;
		}

		size_t countVertex;

		if( m_maxCharCount == (size_t)-1 )
		{
			countVertex = outlineVertices.size();
		}
		else
		{
			countVertex = m_maxCharCount * 4;
		}

		TVectorRenderVertex2D::value_type * vertices = &(outlineVertices[0]);

        RENDER_SERVICE(m_serviceProvider)
            ->addRenderQuad( _viewport, _camera, material, vertices, countVertex, nullptr );
	}
	//////////////////////////////////////////////////////////////////////////
	size_t TextField::getCharCount() const
	{
		if (this->isCompile() == false)
		{
			LOGGER_ERROR(m_serviceProvider)("TextField::getCharCount '%s' not compile"
				, m_name.c_str()
				);

			return 0;
		}

		if( this->isInvalidateTextLines() == true )
		{
			this->updateTextLines_();
		}

		return m_charCount;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setMaxLen( float _len )
	{
		m_maxWidth = _len;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	float TextField::getMaxLen() const
	{
		return m_maxWidth;
	}
	//////////////////////////////////////////////////////////////////////////
	float TextField::getFontHeight() const
	{
		const TextFontInterfacePtr & font = this->getFont();

		if( font == nullptr )
		{
			return 0.f;
		}

		float fontHeight = font->getFontHeight();

		return fontHeight;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setFontName( const ConstString & _name )
	{
		m_fontName = _name;
		
		m_fontParams |= EFP_FONT;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	const ConstString & TextField::getFontName() const
	{
		return m_fontName;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setFontColor( const ColourValue & _color )
	{
		m_colorFont = _color;

		m_fontParams |= EFP_COLOR_FONT;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	const ColourValue& TextField::getFontColor() const
	{
		return m_colorFont;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::enableOutline( bool _value )
	{
		m_outline = _value;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	bool TextField::isOutline() const
	{
		return m_outline;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setOutlineColor( const ColourValue & _color )
	{
		m_colorOutline = _color;

		m_fontParams |= EFP_COLOR_OUTLINE;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	const ColourValue& TextField::getOutlineColor() const
	{
		return m_colorOutline;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setLineOffset( float _offset )
	{
		m_lineOffset = _offset;

		m_fontParams |= EFP_LINE_OFFSET;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	float TextField::getLineOffset() const
	{
		return m_lineOffset;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setCharOffset( float _offset )
	{
		m_charOffset = _offset;

		m_fontParams |= EFP_CHAR_OFFSET;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	float TextField::getCharOffset() const
	{
		return m_charOffset;
	}
	//////////////////////////////////////////////////////////////////////////
	const mt::vec2f& TextField::getTextSize() const
	{
		if( this->isCompile() == false )
		{
			LOGGER_ERROR(m_serviceProvider)("TextField::getTextLength '%s' not compile"
                , this->getName().c_str()
				);

			return m_textSize;
		}

		if( this->isInvalidateTextLines() == true )
		{
			this->updateTextLines_();
		}

		return m_textSize;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::updateTextLines_() const
	{
		m_invalidateTextLines = false;

		m_lines.clear();
		m_charCount = 0;

        String space_delim = " ";

		const TextFontInterfacePtr & font = this->getFont();

		if( font == nullptr )
		{
			return;
		}

		const String & text = this->getText();

		TVectorString lines;
		Utils::split( lines, text, false, "\n" );

		float charOffset = this->calcCharOffset();
		
		for(TVectorString::iterator 
			it = lines.begin(),
			it_end = lines.end(); 
		it != it_end; 
		++it)
		{
			TextLine textLine(m_serviceProvider, charOffset);
			            
			textLine.initialize( font, *it );

			float textLength = textLine.getLength();

			if( textLength > m_maxWidth )
			{
				TVectorString words;
				Utils::split( words, *it, false, " " );

				String newLine = words.front();
				words.erase( words.begin() );	
				while( words.empty() == false )
				{
					TextLine tl(m_serviceProvider,  charOffset);

					String tl_string( newLine + space_delim + words.front() );

					tl.initialize( font, tl_string );

					if( tl.getLength() > m_maxWidth )
					{
						TextLine line(m_serviceProvider, charOffset);

						line.initialize( font, newLine );

						m_lines.push_back( line );

						newLine.clear();
						newLine = words.front();
						words.erase( words.begin() );
					}
					else
					{
						newLine += space_delim + words.front();
						words.erase( words.begin() );
					}
				}

				TextLine line(m_serviceProvider, charOffset);				
				line.initialize( font, newLine );

				m_lines.push_back( line );
			}
			else
			{
				m_lines.push_back( textLine );
			}
		}

		float maxlen = 0.f;

		for(TVectorTextLine::iterator 
			it = m_lines.begin(),
			it_end = m_lines.end(); 
		it != it_end;
		++it )
		{
			maxlen = (std::max)( maxlen, it->getLength() );
			m_charCount += it->getCharsDataSize();
		}

		m_textSize.x = maxlen;

		float fontHeight = font->getFontHeight();
		float lineOffset = this->calcLineOffset();
		m_textSize.y = ( fontHeight + lineOffset ) * m_lines.size();

		this->invalidateVertices_();
		this->invalidateBoundingBox();
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::updateFont_() const
	{
		m_invalidateFont = false;

		ConstString fontName = this->calcFontName();

		if( fontName.empty() == true )
		{
			fontName = TEXT_SERVICE(m_serviceProvider)
				->getDefaultFontName();
		}

		if( m_font != nullptr )
		{
			const ConstString & currentFontName = m_font->getName();

			if( fontName == currentFontName )
			{
				return;
			}
			else
			{
				TEXT_SERVICE(m_serviceProvider)
					->releaseFont( m_font );
				m_font = nullptr;

				m_materialFont = nullptr;
				m_materialOutline = nullptr;
			}
		}

		if( fontName.empty() == true )
		{
			return;
		}

		m_font = TEXT_SERVICE(m_serviceProvider)
			->getFont( fontName );

		if( m_font == nullptr )
		{
			LOGGER_ERROR(m_serviceProvider)("TextField::updateFont_ '%s' can't found font '%s'"
				, this->getName().c_str()
				, fontName.c_str()
				);

			return;
		}
		
		const RenderTextureInterfacePtr & textureFont = m_font->getTextureFont();

		m_materialFont = RENDERMATERIAL_SERVICE(m_serviceProvider)
			->getMaterial( CONST_STRING(m_serviceProvider, BlendSprite), false, false, PT_TRIANGLELIST, 1, &textureFont );

		const RenderTextureInterfacePtr & textureOutline = m_font->getTextureOutline();

		if( textureOutline != nullptr )
		{
			m_materialOutline = RENDERMATERIAL_SERVICE(m_serviceProvider)
				->getMaterial( CONST_STRING(m_serviceProvider, BlendSprite), false, false, PT_TRIANGLELIST, 1, &textureOutline );
		}
		else
		{
			m_materialOutline = nullptr;
		}

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	const ConstString & TextField::calcFontName() const
	{
		if( m_textEntry != nullptr )
		{
			size_t params = m_textEntry->getFontParams();

			if( params & EFP_FONT )
			{
				const ConstString & fontName = m_textEntry->getFont();

				return fontName;
			}
		}

		if( m_fontName.empty() == true )
		{
			const ConstString & fontName = TEXT_SERVICE(m_serviceProvider)
				->getDefaultFontName();

			return fontName;
		}

		return m_fontName;
	}
	//////////////////////////////////////////////////////////////////////////
	float TextField::calcLineOffset() const
	{
		if( m_textEntry != nullptr )
		{
			size_t params = m_textEntry->getFontParams();

			if( params & EFP_LINE_OFFSET )
			{
				float value = m_textEntry->getLineOffset();

				return value;
			}
		}

		if( m_fontParams & EFP_LINE_OFFSET )
		{
			return m_lineOffset;
		}

		const TextFontInterfacePtr & font = this->getFont();

		if( font != nullptr )
		{
			size_t params = font->getFontParams();

			if( params & EFP_LINE_OFFSET )
			{
				float value = font->getLineOffset();

				return value;
			}
		}

		return m_lineOffset;
	}
	//////////////////////////////////////////////////////////////////////////
	float TextField::calcCharOffset() const
	{
		if( m_textEntry != nullptr )
		{
			size_t params = m_textEntry->getFontParams();

			if( params & EFP_CHAR_OFFSET )
			{
				float value = m_textEntry->getCharOffset();

				return value;
			}
		}

		if( m_fontParams & EFP_CHAR_OFFSET )
		{
			return m_charOffset;
		}

		const TextFontInterfacePtr & font = this->getFont();

		if( font != nullptr )
		{
			size_t params = font->getFontParams();

			if( params & EFP_CHAR_OFFSET )
			{
				float value = font->getCharOffset();

				return value;
			}
		}

		return m_charOffset;
	}
	//////////////////////////////////////////////////////////////////////////
	const ColourValue & TextField::calcColorFont() const
	{
		if( m_textEntry != nullptr )
		{
			size_t params = m_textEntry->getFontParams();

			if( params & EFP_COLOR_FONT )
			{
				const ColourValue & value = m_textEntry->getColorFont();

				return value;
			}
		}

		if( m_fontParams & EFP_COLOR_FONT )
		{
			return m_colorFont;
		}

		const TextFontInterfacePtr & font = this->getFont();

		if( font != nullptr )
		{
			size_t params = font->getFontParams();

			if( params & EFP_COLOR_FONT )
			{
				const ColourValue & value = font->getColorFont();

				return value;
			}
		}

		return m_colorFont;
	}
	//////////////////////////////////////////////////////////////////////////
	const ColourValue & TextField::calcColorOutline() const
	{
		if( m_textEntry != nullptr )
		{
			size_t params = m_textEntry->getFontParams();

			if( params & EFP_COLOR_OUTLINE )
			{
				const ColourValue & value = m_textEntry->getColorOutline();

				return value;
			}
		}

		if( m_fontParams & EFP_COLOR_OUTLINE )
		{
			return m_colorOutline;
		}

		const TextFontInterfacePtr & font = this->getFont();

		if( font != nullptr )
		{
			size_t params = font->getFontParams();

			if( params & EFP_COLOR_OUTLINE )
			{
				const ColourValue & value = font->getColorOutline();

				return value;
			}
		}

		return m_colorOutline;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::_updateBoundingBox( mt::box2f & _boundingBox )
	{
		Node::_updateBoundingBox( _boundingBox );

		//mt::vec2f offset = mt::zero_v2;

		//const mt::mat4f & wm = this->getWorldMatrix();

		//const TListTextLine & lines = this->getTextLines();

		//for( TListTextLine::const_iterator 
		//	it_line = lines.begin(),
		//	it_line_end = lines.end(); 
		//it_line != it_line_end; 
		//++it_line )
		//{
		//	mt::vec2f alignOffset;

		//	this->updateAlignOffset_( *it_line, alignOffset );

		//	offset.x = alignOffset.x;
		//	offset.y += alignOffset.y;

		//	it_line->updateBoundingBox( offset, wm, _boundingBox );

		//	offset.y += m_lineOffset;
		//}		
	}
	//////////////////////////////////////////////////////////////////////////
	float TextField::getHorizontAlignOffset_( const TextLine & _line )
	{
        float offset = 0.f;

		switch( m_horizontAlign )
		{
		case ETFHA_NONE:
			{
				offset = 0.f;
			}break;
		case ETFHA_CENTER:
			{
				offset = -_line.getLength() * 0.5f;
			}break;
		case ETFHA_RIGHT:
			{
				offset = -_line.getLength();
			}break;
		case ETFHA_LEFT:
			{
				offset = _line.getLength();
			}break;
		}

        return offset;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::_invalidateColor()
	{
		Node::_invalidateColor();

		this->invalidateVertices_();
	}
	//////////////////////////////////////////////////////////////////////////
	size_t TextField::getMaxCharCount() const
	{
		return m_maxCharCount;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setMaxCharCount( int  _maxCharCount ) 
	{
		m_maxCharCount = _maxCharCount;

        this->invalidateTextLines();	
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setTextID( const ConstString& _key )
	{	
		if( m_textEntry != nullptr )
		{
			const ConstString & textKey = m_textEntry->getKey();

			if( textKey == _key )
			{
				return;
			}
		}

		m_textEntry = TEXT_SERVICE(m_serviceProvider)
			->getTextEntry( _key );

		if( m_textEntry == nullptr )
		{
			LOGGER_ERROR(m_serviceProvider)("TextField::setTextID '%s' can't find text ID '%s'"
				, this->getName().c_str()
				, _key.c_str()
				);

			return;
		}

		this->invalidateFont();
		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::removeTextID()
	{
		m_textEntry = nullptr;
		m_textFormatArgs.clear();

		this->invalidateFont();
		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	const ConstString & TextField::getTextID() const
	{
		if( m_textEntry == nullptr )
		{
			return ConstString::none();
		}

		const ConstString & key = m_textEntry->getKey();

		return key;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setTextFormatArg( const String & _arg )
	{
		m_textFormatArgs.clear();
		m_textFormatArgs.push_back( _arg );

		this->invalidateFont();
		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setTextFormatArgs( const TVectorString & _args )
	{
		m_textFormatArgs = _args;

		this->invalidateFont();
		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::removeTextFormatArgs()
	{
		m_textFormatArgs.clear();

		this->invalidateFont();
		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	const TVectorString & TextField::getTextFormatArgs() const
	{
		return m_textFormatArgs;
	}
	//////////////////////////////////////////////////////////////////////////
	const String & TextField::getText() const
	{
		if( m_textEntry == nullptr )
		{
			LOGGER_ERROR(m_serviceProvider)("TextField::getText '%s' can't setup text ID"
				, this->getName().c_str()
				);
			
			return Utils::emptyString();
		}

		const ConstString & keyText = m_textEntry->getText();

		StringFormat format(keyText.c_str());

		for( TVectorString::const_iterator
			it = m_textFormatArgs.begin(),
			it_end = m_textFormatArgs.end();
		it != it_end;
		++it )
		{
			const String & arg = *it;

			format = format % arg;
		}

		m_cacheText = format.str();

		return m_cacheText;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setVerticalNoneAlign()
	{
		m_verticalAlign = ETFVA_NONE;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setPixelsnap( bool _pixelsnap )
	{
		m_pixelsnap = _pixelsnap;

        this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	bool TextField::getPixelsnap() const
	{
		return m_pixelsnap;
	}
	//////////////////////////////////////////////////////////////////////////
	bool TextField::isVerticalNoneAlign() const
	{
		return m_verticalAlign == ETFVA_NONE;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setVerticalCenterAlign()
	{
		m_verticalAlign = ETFVA_CENTER;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	bool TextField::isVerticalCenterAlign() const
	{
		return m_verticalAlign == ETFVA_CENTER;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setNoneAlign()
	{
		m_horizontAlign = ETFHA_NONE;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	bool TextField::isNoneAlign() const
	{
		return m_horizontAlign == ETFHA_NONE;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setCenterAlign()
	{
		m_horizontAlign = ETFHA_CENTER;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	bool TextField::isCenterAlign() const
	{
		return m_horizontAlign == ETFHA_CENTER;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setRightAlign()
	{
		m_horizontAlign = ETFHA_RIGHT;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	bool TextField::isRightAlign() const
	{
		return m_horizontAlign == ETFHA_RIGHT;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::setLeftAlign()
	{
		m_horizontAlign = ETFHA_LEFT;

		this->invalidateTextLines();
	}
	//////////////////////////////////////////////////////////////////////////
	bool TextField::isLeftAlign() const
	{
		return m_horizontAlign == ETFHA_LEFT;
	}
	//////////////////////////////////////////////////////////////////////////
	void TextField::invalidateVertices_() const
	{
		m_invalidateVertices = true;

        this->invalidateVerticesWM_();
	}
    //////////////////////////////////////////////////////////////////////////
    void TextField::invalidateVerticesWM_() const
    {
        m_invalidateVerticesWM = true;
    }
	//////////////////////////////////////////////////////////////////////////
	void TextField::invalidateTextLines() const
	{
		m_invalidateTextLines = true;

		this->invalidateVertices_();
	}
    //////////////////////////////////////////////////////////////////////////
    void TextField::updateVerticesWM_()
    {
        m_invalidateVerticesWM = false;

        if( m_invalidateVertices == true )
        {
            this->updateVertices_();
        }       

        this->updateVertexDataWM_( m_vertexDataTextWM, m_vertexDataText );
        this->updateVertexDataWM_( m_vertexDataOutlineWM, m_vertexDataOutline );
    }
    //////////////////////////////////////////////////////////////////////////
    void TextField::updateVertexDataWM_( TVectorRenderVertex2D & _outVertex, const TVectorRenderVertex2D & _fromVertex )
    {
        _outVertex.assign( _fromVertex.begin(), _fromVertex.end() );

        TVectorRenderVertex2D::const_iterator it = _fromVertex.begin();
        TVectorRenderVertex2D::const_iterator it_end = _fromVertex.end();

        TVectorRenderVertex2D::iterator it_w = _outVertex.begin();        

        mt::mat4f wm = this->getWorldMatrix();

		if( m_pixelsnap == true )
		{
			wm.v3.x = floorf( wm.v3.x + 0.5f );
			wm.v3.y = floorf( wm.v3.y + 0.5f );
		}

        for( ; it != it_end; ++it, ++it_w )
        {
            const RenderVertex2D & vertex = *it;
            
            RenderVertex2D & vertex_w = *it_w;
            
            mt::mul_v3_m4( vertex_w.pos, vertex.pos, wm );
        }
    }
	//////////////////////////////////////////////////////////////////////////
	void TextField::updateVertices_()
	{
		m_invalidateVertices = false;

		ColourValue colorNode;
		this->calcTotalColor( colorNode );
		
		ColourValue colorFont;

		const ColourValue & paramsColorFont = this->calcColorFont(); 
		colorFont = colorNode * paramsColorFont;
		
		this->updateVertexData_( colorFont, m_vertexDataText );

		const TextFontInterfacePtr & font = this->getFont();

		if( m_outline && font->getTextureOutline() != nullptr )
		{			
			ColourValue colorOutline;

			const ColourValue & paramsColorOutline = this->calcColorOutline();
			colorOutline = colorNode * paramsColorOutline;
			
			this->updateVertexData_( colorOutline, m_vertexDataOutline );
		}		
	}
}
