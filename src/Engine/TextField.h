# pragma once

#include "Interface/TextInterface.h"

#include "TextChar.h"

#include "Kernel/Node.h"
#include "Kernel/BaseRender.h"
#include "Kernel/Observable.h"
#include "Kernel/Materialable.h"

#include "Kernel/ColourValue.h"
#include "Kernel/ValueInterpolator.h"
#include "Kernel/ConstString.h"
#include "Kernel/RenderVertex2D.h"

#include "Config/Vector.h"

#include "math/vec4.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    class TextLine;
    //////////////////////////////////////////////////////////////////////////
    class TextField
        : public Node
        , public BaseRender
        , public Materialable
        , public Observable
    {
        DECLARE_VISITABLE( Node );
        DECLARE_RENDERABLE();

    public:
        TextField();
        ~TextField() override;

    public:
        void setMaxLength( float _maxLength );
        float getMaxLength() const;

    public:
        void setWrap( bool _wrap );
        bool getWrap() const;

    public:
        void setTextID( const ConstString& _key );
        void removeTextID();

        const ConstString & getTextID() const;

        void setTextAliasEnvironment( const ConstString & _aliasEnvironment );
        const ConstString & getTextAliasEnvironment() const;

        void setTextFormatArgs( const VectorString & _args );
        void removeTextFormatArgs();
        const VectorString & getTextFormatArgs() const;

    public:
        uint32_t getTextExpectedArgument() const;

    protected:
        bool updateTextCache_( U32String & _cacheText ) const;

    public:
        void setFontName( const ConstString & _fontName );
        const ConstString & getFontName() const;

    public:
        float getFontHeight() const;

    public:
        void setFontColor( const ColourValue & _color );
        const ColourValue & getFontColor() const;

        void setLineOffset( float _offset );
        float getLineOffset() const;

        void setCharOffset( float _offset );
        float getCharOffset() const;

        void setCharScale( float _value );
        float getCharScale() const;

    public:
        void setHorizontalCenterAlign();
        bool isHorizontalCenterAlign() const;

        void setHorizontalRightAlign();
        bool isHorizontalRightAlign() const;

        void setHorizontalLeftAlign();
        bool isHorizontalLeftAlign() const;

        void setVerticalBottomAlign();
        bool isVerticalBottomAlign() const;

        void setVerticalCenterAlign();
        bool isVerticalCenterAlign() const;

    public:
        void setMaxCharCount( uint32_t _maxCharCount );
        uint32_t getMaxCharCount() const;

        void setPixelsnap( bool _pixelsnap );
        bool getPixelsnap() const;

    public:
        const mt::vec2f & getTextSize() const;

    public:
        uint32_t getCharCount() const;

    protected:
        void _render( const RenderContext * _state ) override;

    protected:
        bool _activate() override;
        void _deactivate() override;
        bool _compile() override;
        void _release() override;

        void _invalidateWorldMatrix() override;
        void _updateBoundingBox( mt::box2f & _boundingBox ) const override;
        void _invalidateColor() override;

    protected:
        void notifyChangeLocale( const ConstString & _prevLocale, const ConstString & _currentlocale );
        void notifyDebugMode( bool _debugMode );
        void notifyChangeTextAliasArguments( const ConstString & _environment, const ConstString & _alias );

    protected:
        void updateVertices_( const TextFontInterfacePtr & _font );
        void invalidateVertices_() const;

        void updateVerticesWM_( const TextFontInterfacePtr & _font );
        void updateVertexDataWM_( VectorRenderVertex2D & _outVertex, const VectorRenderVertex2D & _fromVertex );

        void invalidateVerticesWM_() const;

        inline const VectorRenderVertex2D & getTextVertices( const TextFontInterfacePtr & _font );

        void updateVertexData_( const TextFontInterfacePtr & _font, const ColourValue & _color, VectorRenderVertex2D& _vertexData );

    protected:
        void invalidateTextLines() const;
        void invalidateTextEntry() const;
        inline bool isInvalidateTextLines() const;

        void updateTextLines_() const;
        void updateTextLinesMaxCount_( VectorTextLines & _textLines ) const;
        void updateTextLinesDimension_( const TextFontInterfacePtr & _font, const VectorTextLines & _textLines, mt::vec2f & _size, uint32_t & _charCount, uint32_t & _layoutCount ) const;

    protected:
        inline const TextEntryInterfacePtr & getTextEntry() const;
        inline const TextFontInterfacePtr & getFont() const;
        inline void invalidateFont() const;
        void updateFont_() const;
        void updateTextEntry_() const;

    protected:
        const ConstString & calcFontName() const;
        float calcLineOffset() const;
        float calcCharOffset() const;
        float calcMaxLength() const;

        const ColourValue & calcFontColor() const;

        ETextHorizontAlign calcHorizontalAlign() const;
        ETextVerticalAlign calcVerticalAlign() const;

        float calcCharScale() const;

    protected:
        RenderMaterialInterfacePtr _updateMaterial() const override;

    protected:
        ConstString m_key;
        ConstString m_aliasEnvironment;

        mutable TextEntryInterfacePtr m_textEntry;
        mutable VectorString m_textFormatArgs;

        ETextHorizontAlign m_horizontAlign;
        ETextVerticalAlign m_verticalAlign;

        mutable TextFontInterfacePtr m_font;
        mutable bool m_invalidateFont;

        float m_charScale;

        float m_maxLength;

        ConstString m_fontName;

        float m_lineOffset;
        float m_charOffset;

        ColourValue m_colorFont;

        uint32_t m_fontParams;

        uint32_t m_maxCharCount;

        mutable uint32_t m_charCount;
        mutable uint32_t m_layoutCount;
        mutable mt::vec2f m_textSize;

        bool m_wrap;
        bool m_pixelsnap;

        bool m_debugMode;

        typedef Vector<TextLine> VectorTextLine;
        typedef Vector<VectorTextLine> VectorTextLine2;
        typedef Vector<VectorTextLine2> VectorTextLineLayout;
        mutable VectorTextLineLayout m_layouts;

        typedef Vector<float> VectorTextLineAlignOffset;
        mutable VectorTextLineAlignOffset m_textLineAlignOffsets;

        struct Chunk
        {
            uint32_t vertex_begin;
            uint32_t vertex_count;

            RenderMaterialInterfacePtr material;
        };

        typedef Vector<Chunk> VectorChunks;
        VectorChunks m_chunks;

        mutable VectorCacheFonts m_cacheFonts;

        VectorRenderVertex2D m_vertexDataText;
        VectorRenderVertex2D m_vertexDataTextWM;

        mutable bool m_invalidateVertices;
        mutable bool m_invalidateVerticesWM;

        mutable bool m_invalidateTextLines;
        mutable bool m_invalidateTextEntry;

    protected:
        const VectorTextLineLayout & getTextLayots() const;
        float getHorizontAlignOffset_( const VectorTextLine2 & _lines ) const;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<TextField> TextFieldPtr;
    //////////////////////////////////////////////////////////////////////////
    inline const VectorRenderVertex2D & TextField::getTextVertices( const TextFontInterfacePtr & _font )
    {
        if( m_invalidateVerticesWM == true )
        {
            this->updateVerticesWM_( _font );
        }

        return m_vertexDataTextWM;
    }
    //////////////////////////////////////////////////////////////////////////
    inline bool TextField::isInvalidateTextLines() const
    {
        return m_invalidateTextLines;
    }
    //////////////////////////////////////////////////////////////////////////
    inline const TextFontInterfacePtr & TextField::getFont() const
    {
        if( m_invalidateFont == true )
        {
            this->updateFont_();
        }

        return m_font;
    }
    //////////////////////////////////////////////////////////////////////////
    inline const TextEntryInterfacePtr & TextField::getTextEntry() const
    {
        if( m_invalidateTextEntry == true )
        {
            this->updateTextEntry_();
        }

        return m_textEntry;
    }
    //////////////////////////////////////////////////////////////////////////
    inline void TextField::invalidateFont() const
    {
        m_invalidateFont = true;
    }
}
