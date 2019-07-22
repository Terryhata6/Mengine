#include "TaskAnimatablePlay.h"

#include "Kernel/Logger.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    TaskAnimatablePlay::TaskAnimatablePlay( const AnimatablePtr & _animatable )
        : m_animatable( _animatable )
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TaskAnimatablePlay::~TaskAnimatablePlay()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TaskAnimatablePlay::_onRun()
    {
        AnimationInterface * animation = m_animatable->getAnimation();

        if( animation == nullptr )
        {
            LOGGER_ERROR( "animatable not have animation" );

            return true;
        }

        animation->play( 0.f );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TaskAnimatablePlay::_onFinally()
    {
        m_animatable = nullptr;
    }
}