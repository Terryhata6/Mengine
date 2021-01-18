#include "Isometric.h"

#include "Interface/TransformationInterface.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    Isometric::Isometric()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Isometric::~Isometric()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    namespace
    {
        class FIsometricSortY
        {
        public:
            bool operator() ( const NodePtr & _left, const NodePtr & _right ) const
            {
                TransformationInterface * leftTransformation = _left->getTransformation();
                TransformationInterface * rigthTransformation = _right->getTransformation();

                const mt::vec3f & l = leftTransformation->getLocalPosition();
                const mt::vec3f & r = rigthTransformation->getLocalPosition();

                float less_y = l.y - r.y;
                if( less_y < 0.1f && less_y > -0.1f )
                {
                    return l.x < r.x;
                }

                return l.y < r.y;
            }
        };
    }
    ////////////////////////////////////////////////////////////////////////
    void Isometric::update( const UpdateContext * _context )
    {
        MENGINE_UNUSED( _context );

        stdex::helper::intrusive_sort_stable( m_children, FIsometricSortY() );
    }
    //////////////////////////////////////////////////////////////////////////
}
