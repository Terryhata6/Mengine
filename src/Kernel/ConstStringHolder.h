#pragma once

#include "Config/Typedef.h"
#include "Config/Char.h"

#include "Kernel/HashType.h"
#include "Kernel/Factorable.h"
#include "Kernel/IntrusiveLinked.h"

namespace Mengine
{
    class ConstStringHolder
        : public IntrusiveLinked<ConstStringHolder>
    {
    protected:
        ConstStringHolder();
        virtual ~ConstStringHolder();

    public:
        ConstStringHolder( const ConstStringHolder & _holder );

    protected:
        ConstStringHolder & operator = ( const ConstStringHolder & _holder );

    public:
        typedef uint32_t size_type;
        typedef HashType hash_type;
        typedef Char value_type;

    public:
        MENGINE_INLINE const value_type * c_str() const noexcept
        {
            return m_data;
        }

        MENGINE_INLINE size_type size() const noexcept
        {
            return m_size;
        }

        MENGINE_INLINE bool empty() const noexcept
        {
            return m_size == 0;
        }

    public:
        MENGINE_INLINE hash_type hash() const noexcept
        {
            return m_hash;
        }

    protected:
        MENGINE_INLINE const value_type * data() const noexcept
        {
            return m_data;
        }

    public:
        void setup( const value_type * _data, size_type _size, hash_type _hash );

    protected:
        const value_type * m_data;
        size_type m_size;

        hash_type m_hash;
    };
}