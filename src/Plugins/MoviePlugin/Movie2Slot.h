#pragma once

#include "Kernel/Node.h"
#include "Kernel/BaseRender.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<class Movie2> Movie2Ptr;
    //////////////////////////////////////////////////////////////////////////
    class Movie2Slot
        : public Node
        , public BaseRender
    {
        DECLARE_VISITABLE( Node );
        DECLARE_RENDERABLE();

    public:
        Movie2Slot();
        ~Movie2Slot() override;

    public:
        void setMovie( const Movie2Ptr & _movie );
        const Movie2Ptr & getMovie() const;

    protected:
        void _destroy() override;

    //protected:
    //    void _setLocalHide( bool _value ) override;

    protected:
        void _setPersonalColor( const Color& _color ) override;
        void _setPersonalAlpha( float _alpha ) override;

    protected:
        void _changeParent( Node * _oldParent, Node * _newParent ) override;

    protected:
        void _render( const RenderContext * _state ) override;

    protected:
        Movie2Ptr m_movie;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Movie2Slot> Movie2SlotPtr;
    //////////////////////////////////////////////////////////////////////////
}