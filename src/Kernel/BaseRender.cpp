#include "Kernel/BaseRender.h"

#include "Interface/RenderServiceInterface.h"

#include "Kernel/Assertion.h"

#include <algorithm>

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    BaseRender::BaseRender()
        : m_relationRender( nullptr )
        , m_extraRelationRender( nullptr )
        , m_externalRender( false )
        , m_renderEnable( false )
        , m_hide( false )
        , m_localHide( false )
        , m_rendering( true )
        , m_invalidateRendering( false )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    BaseRender::~BaseRender()
    {
        MENGINE_ASSERTION_FATAL( m_extraRelationRender == nullptr );
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::setRelationRender( RenderInterface * _relationRender )
    {
        MENGINE_ASSERTION( _relationRender != nullptr, "set nullptr relation" );
        MENGINE_ASSERTION( _relationRender != this, "set this relation" );

        if( m_extraRelationRender != nullptr )
        {
            m_relationRender = static_cast<BaseRender *>(_relationRender);

            return;
        }

        if( m_relationRender != nullptr )
        {
            m_relationRender->removeRelationRenderChildren_( this );
        }

        m_relationRender = static_cast<BaseRender *>(_relationRender);
        m_relationRender->addRelationRenderChildrenBack_( this );

        this->invalidateColor();

        m_invalidateRendering = true;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::removeRelationRender()
    {
        if( m_extraRelationRender != nullptr )
        {
            m_extraRelationRender->removeRelationRenderChildren_( this );
            m_extraRelationRender = nullptr;

            m_relationRender = nullptr;

            return;
        }

        if( m_relationRender == nullptr )
        {
            return;
        }

        m_relationRender->removeRelationRenderChildren_( this );
        m_relationRender = nullptr;
    }
        //////////////////////////////////////////////////////////////////////////
    void BaseRender::setExtraRelationRender( RenderInterface * _relationRender )
    {
        MENGINE_ASSERTION( _relationRender != nullptr, "set nullptr relation" );
        MENGINE_ASSERTION( _relationRender != this, "set this relation" );

        if( m_extraRelationRender != nullptr )
        {
            m_extraRelationRender->removeRelationRenderChildren_( this );
        }
        else if( m_relationRender != nullptr )
        {
            m_relationRender->removeRelationRenderChildren_( this );
        }

        m_extraRelationRender = static_cast<BaseRender *>(_relationRender);
        m_extraRelationRender->addRelationRenderChildrenBack_( this );

        this->invalidateColor();

        m_invalidateRendering = true;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::removeExtraRelationRender()
    {
        if( m_extraRelationRender == nullptr )
        {
            return;
        }

        m_extraRelationRender->removeRelationRenderChildren_( this );
        m_extraRelationRender = nullptr;

        if( m_relationRender == nullptr )
        {
            return;
        }

        m_relationRender->addRelationRenderChildrenBack_( this );

        this->invalidateColor();

        m_invalidateRendering = true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool BaseRender::emptyRenderChildren() const
    {
        return m_renderChildren.empty();
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::clearRenderChildren()
    {
        for( BaseRender * child : m_renderChildren )
        {
            child->m_extraRelationRender = nullptr;
            child->m_relationRender = nullptr;
        }

        m_renderChildren.clear();
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::foreachRenderChildren( const LambdaRender & _lambda )
    {
        for( BaseRender * child : m_renderChildren )
        {
            _lambda( child );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::foreachRenderChildrenEnabled( const LambdaRender & _lambda )
    {
        if( m_renderEnable == false )
        {
            return;
        }

        for( BaseRender * child : m_renderChildren )
        {
            if( child->m_renderEnable == false )
            {
                continue;
            }

            _lambda( child );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::setRenderEnable( bool _renderEnable )
    {
        m_renderEnable = _renderEnable;

        m_invalidateRendering = true;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::setHide( bool _hide )
    {
        if( m_hide == _hide )
        {
            return;
        }

        m_hide = _hide;

        this->_setHide( _hide );

        m_invalidateRendering = true;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::_setHide( bool _hide )
    {
        MENGINE_UNUSED( _hide );

        //Empty
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::setLocalHide( bool _localHide )
    {
        if( m_localHide == _localHide )
        {
            return;
        }

        m_localHide = _localHide;

        this->_setLocalHide( _localHide );
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::_setLocalHide( bool _localHide )
    {
        MENGINE_UNUSED( _localHide );

        //Empty
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::addRelationRenderChildrenBack_( BaseRender * _childRender )
    {
        m_renderChildren.push_back( _childRender );
    }    
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::removeRelationRenderChildren_( BaseRender * _childRender )
    {
        VectorBaseRender::iterator it_erase = std::find( m_renderChildren.begin(), m_renderChildren.end(), _childRender );

        MENGINE_ASSERTION_FATAL( it_erase != m_renderChildren.end(), "remove relation child is not found" );

        m_renderChildren.erase( it_erase );
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::setRenderViewport( const RenderViewportInterfacePtr & _viewport )
    {
        m_renderViewport = _viewport;
    }
    //////////////////////////////////////////////////////////////////////////
    const RenderViewportInterfacePtr & BaseRender::getRenderViewport() const
    {
        return m_renderViewport;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::setRenderCamera( const RenderCameraInterfacePtr & _camera )
    {
        m_renderCamera = _camera;
    }
    //////////////////////////////////////////////////////////////////////////
    const RenderCameraInterfacePtr & BaseRender::getRenderCamera() const
    {
        return m_renderCamera;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::setRenderTransformation( const RenderTransformationInterfacePtr & _renderTransformation )
    {
        m_renderTransformation = _renderTransformation;
    }
    //////////////////////////////////////////////////////////////////////////
    const RenderTransformationInterfacePtr & BaseRender::getRenderTransformation() const
    {
        return m_renderTransformation;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::setRenderScissor( const RenderScissorInterfacePtr & _scissor )
    {
        m_renderScissor = _scissor;
    }
    //////////////////////////////////////////////////////////////////////////
    const RenderScissorInterfacePtr & BaseRender::getRenderScissor() const
    {
        return m_renderScissor;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::setRenderTarget( const RenderTargetInterfacePtr & _target )
    {
        m_renderTarget = _target;
    }
    //////////////////////////////////////////////////////////////////////////
    const RenderTargetInterfacePtr & BaseRender::getRenderTarget() const
    {
        return m_renderTarget;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::makeRenderContext( RenderContext * const _context ) const
    {
        _context->viewport = m_renderViewport.get();
        _context->camera = m_renderCamera.get();
        _context->transformation = m_renderTransformation.get();
        _context->scissor = m_renderScissor.get();
        _context->target = m_renderTarget.get();
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::renderWithChildren( const RenderPipelineInterfacePtr & _renderPipeline, const RenderContext * _context, bool _external ) const
    {
        if( this->isRendering() == false )
        {
            return;
        }

        if( m_externalRender == true && _external == false )
        {
            return;
        }

        RenderContext context;

        context.viewport = m_renderViewport != nullptr ? m_renderViewport.get() : _context->viewport;
        context.camera = m_renderCamera != nullptr ? m_renderCamera.get() : _context->camera;
        context.transformation = m_renderTransformation != nullptr ? m_renderTransformation.get() : _context->transformation;
        context.scissor = m_renderScissor != nullptr ? m_renderScissor.get() : _context->scissor;
        context.target = m_renderTarget != nullptr ? m_renderTarget.get() : _context->target;

        if( m_localHide == false && this->isPersonalTransparent() == false )
        {
            this->render( _renderPipeline, &context );
        }

        for( const BaseRender * child : m_renderChildren )
        {
            child->renderWithChildren( _renderPipeline, &context, false );
        }

        if( context.target != nullptr )
        {
            const RenderInterfacePtr & targetRender = this->makeTargetRender( &context );

            if( targetRender != nullptr )
            {
                targetRender->render( _renderPipeline, _context );
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////
    const RenderInterfacePtr & BaseRender::makeTargetRender( const RenderContext * _context ) const
    {
        MENGINE_UNUSED( _context );

        return RenderInterfacePtr::none();
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::setExternalRender( bool _externalRender )
    {
        if( m_externalRender == _externalRender )
        {
            return;
        }

        m_externalRender = _externalRender;

        this->_setExternalRender( _externalRender );
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::_setExternalRender( bool _externalRender )
    {
        MENGINE_UNUSED( _externalRender );

        //Empty
    }
    //////////////////////////////////////////////////////////////////////////
    const Color & BaseRender::getWorldColor() const
    {
        const BaseRender * relationRenderTotal = this->getTotalRelationRender();

        if( relationRenderTotal == nullptr )
        {
            const Color & localColor = Colorable::getLocalColor();

            return localColor;
        }

        if( Colorable::isInvalidateColor() == false )
        {
            const Color & relationColor = Colorable::getRelationColor();

            return relationColor;
        }

        const Color & parentColor = relationRenderTotal->getWorldColor();

        const Color & relationColor = Colorable::updateRelationColor( parentColor );

        return relationColor;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::calcTotalColor( Color * const _color ) const
    {
        const Color & worldColor = this->getWorldColor();
        *_color = worldColor;

        const Color & personalColor = this->getPersonalColor();
        *_color *= personalColor;
    }
    //////////////////////////////////////////////////////////////////////////
    bool BaseRender::isSolidColor() const
    {
        const Color & worldColour = this->getWorldColor();

        float worldAlpha = worldColour.getA();
        float personalAlpha = this->getPersonalAlpha();

        bool solid = mt::equal_f_f( worldAlpha * personalAlpha, 1.f );

        return solid;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::invalidateColor() const
    {
        Colorable::invalidateColor();

        for( const BaseRender * child : m_renderChildren )
        {
            child->invalidateColor();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::_setLocalTransparent( bool _transparent )
    {
        MENGINE_UNUSED( _transparent );

        m_invalidateRendering = true;
    }
    //////////////////////////////////////////////////////////////////////////
    void BaseRender::updateRendering_() const
    {
        m_invalidateRendering = true;

        m_rendering = false;

        if( this->isRenderEnable() == false )
        {
            return;
        }

        if( this->isHide() == true )
        {
            return;
        }

        if( this->isLocalTransparent() == true )
        {
            return;
        }

        m_rendering = true;
    }
    //////////////////////////////////////////////////////////////////////////
}
