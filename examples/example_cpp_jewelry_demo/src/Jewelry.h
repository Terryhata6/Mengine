#pragma once

#include "Kernel/Factorable.h"

#include "Kernel/Node.h"
#include "Kernel/Pickerable.h"

namespace Mengine
{
    class Jewelry
        : public Factorable
    { 
    public:
        Jewelry();
        ~Jewelry() override;

    public:
        void setIterator( uint32_t _iterator );
        uint32_t getIterator() const;

    public:
        bool isDead() const;

    public:
        bool initialize( uint32_t _line, uint32_t _type );
        void finalize();

    public:
        void makeNodeActive_();
        void makeNodeBlock_();
        void makePickerable_();

    public:
        void block();
        bool isBlock() const;

    public:
        uint32_t getLine() const;
        uint32_t getType() const;
        const NodePtr & getNode() const;
        const PickerablePtr & getPickerable() const;

    public:
        float m_size;

        uint32_t m_line;
        uint32_t m_iterator;
        uint32_t m_type;

        NodePtr m_node;
        NodePtr m_nodeActive;
        NodePtr m_nodeBlock;
        PickerablePtr m_pickerable;

        bool m_dead;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Jewelry> JewelryPtr;
    //////////////////////////////////////////////////////////////////////////
}