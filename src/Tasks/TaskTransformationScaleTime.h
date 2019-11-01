#pragma once

#include "GOAP/Task.h"

#include "Kernel/Transformation.h"
#include "Kernel/Affectorable.h"

namespace Mengine
{
    enum ETaskTransformationScaleFlags
    {
        ETASK_FLAG_NONE = 0,
        ETASK_FLAG_NOREWIND = 1 << 0,
    };

    class TaskTransformationScaleTime
        : public GOAP::Task
    {
    public:
        TaskTransformationScaleTime( const TransformationPtr & _transformation, const AffectorablePtr & _affectorable, const EasingInterfacePtr & _easing, const mt::vec3f & _to, float _time, uint32_t _flags = ETASK_FLAG_NONE );
        ~TaskTransformationScaleTime() override;

    protected:
        bool _onRun( GOAP::NodeInterface * _node ) override;
        void _onSkip() override;
        void _onFinally() override;

    protected:
        TransformationPtr m_transformation;
        AffectorablePtr m_affectorable;
        EasingInterfacePtr m_easing;

        mt::vec3f m_to;
        float m_time;

        uint32_t m_id;
        uint32_t m_flags;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef GOAP::IntrusivePtr<TaskTransformationScaleTime> TaskTransformationScaleTimePtr;
    //////////////////////////////////////////////////////////////////////////
}