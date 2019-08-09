#include "Stringalized.h"

#include "Kernel/ConstStringHelper.h"
#include "Kernel/FilePathHelper.h"

#include <string.h>

namespace Mengine
{
    namespace Helper
    {
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, bool * _value )
        {
            uint32_t tmp_value;
            if( ::sscanf( _string, "%u", &tmp_value ) != 1 )
            {
                return false;
            }

            *_value = tmp_value != 0;

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, int32_t * _value )
        {
            if( ::sscanf( _string, "%d", _value ) != 1 )
            {
                return false;
            }

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, uint32_t * _value )
        {
            if( ::sscanf( _string, "%u", _value ) != 1 )
            {
                return false;
            }

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, int64_t * _value )
        {
            if( ::sscanf( _string, "%lld", _value ) != 1 )
            {
                return false;
            }

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, uint64_t * _value )
        {
            if( ::sscanf( _string, "%llu", _value ) != 1 )
            {
                return false;
            }

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, float * _value )
        {
            if( ::sscanf( _string, "%f", _value ) != 1 )
            {
                return false;
            }

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, double * _value )
        {
            if( ::sscanf( _string, "%lf", _value ) != 1 )
            {
                return false;
            }

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, const Char ** _value )
        {
            *_value = _string;

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, String * _value )
        {
            _value->assign( _string );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, ConstString * _value )
        {
            *_value = Helper::stringizeString( _string );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, FilePath * _value )
        {
            *_value = Helper::stringizeFilePath( _string );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, Tags * _value )
        {
            const Char * it_tag = nullptr;

            for( const Char
                * it = _string,
                *it_end = _string + strlen( _string );
                it != it_end;
                ++it )
            {
                if( *it == ' ' )
                {
                    if( it_tag != nullptr )
                    {
                        ConstString cstr_value = Helper::stringizeStringSize( it_tag, it - it_tag );

                        _value->addTag( cstr_value );

                        it_tag = nullptr;
                    }
                }
                else
                {
                    if( it_tag == nullptr )
                    {
                        it_tag = it;
                    }
                }
            }

            if( it_tag != nullptr )
            {
                ConstString cstr_value = Helper::stringizeString( it_tag );

                _value->addTag( cstr_value );

                it_tag = nullptr;
            }

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, Resolution * _value )
        {
            uint32_t width;
            uint32_t height;
            if( sscanf( _string, "%u %u", &width, &height ) != 2 )
            {
                return false;
            }

            _value->setWidth( width );
            _value->setHeight( height );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, Color * _value )
        {
            float r;
            float g;
            float b;
            float a;
            if( sscanf( _string, "%f %f %f %f", &r, &g, &b, &a ) != 4 )
            {
                return false;
            }

            const float coef = 1.f / 255.f;

            r *= coef;
            g *= coef;
            b *= coef;
            a *= coef;

            _value->setRGBA( r, g, b, a );

            return true;
        }
        //////////////////////////////////////////////////////////////////////////
        bool stringalized( const Char * _string, AspectRatioViewport * _value )
        {
            float width;
            float height;
            float bx;
            float by;
            float ex;
            float ey;

            if( sscanf( _string, "%f %f %f %f %f %f", &width, &height, &bx, &by, &ex, &ey ) != 6 )
            {
                return false;
            }

            _value->width = width;
            _value->height = height;
            _value->viewport.begin.x = bx;
            _value->viewport.begin.y = by;
            _value->viewport.end.x = ex;
            _value->viewport.end.y = ey;

            return true;
        }
    }
}