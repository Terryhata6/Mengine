#pragma once

#include "Kernel/Node.h"

#include "GOAP/TaskInterface.h"

namespace Mengine
{
    class TaskNodeDisable
        : public GOAP::TaskInterface
    {
    public:
        TaskNodeDisable( GOAP::Allocator * _allocator, const NodePtr & _node );
        ~TaskNodeDisable() override;

    protected:
        bool _onRun( GOAP::NodeInterface * _node ) override;
        void _onFinally() override;

    protected:
        NodePtr m_node;
    };
}