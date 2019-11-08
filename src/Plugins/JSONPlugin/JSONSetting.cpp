#include "JSONSetting.h"

#include "Kernel/AssertionMemoryPanic.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    JSONSetting::JSONSetting()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    JSONSetting::~JSONSetting()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool JSONSetting::loadSetting( const FileGroupInterfacePtr & _fileGroup, const FilePath & _filePath, const Char * _doc )
    {
        JSONStorageInterfacePtr storage = JSON_SERVICE()
            ->loadJSON( _fileGroup, _filePath, _doc );

        MENGINE_ASSERTION_MEMORY_PANIC( storage, false );

        m_storage = storage;

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool JSONSetting::getValue( const Char * _key, bool _default ) const
    {
        const jpp::object & j = m_storage->getJSON();

        bool value = j.get( _key, _default );

        return value;
    }
    //////////////////////////////////////////////////////////////////////////
    int32_t JSONSetting::getValue( const Char * _key, int32_t _default ) const
    {
        const jpp::object & j = m_storage->getJSON();

        int32_t value = j.get( _key, _default );

        return value;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t JSONSetting::getValue( const Char * _key, uint32_t _default ) const
    {
        const jpp::object & j = m_storage->getJSON();

        uint32_t value = j.get( _key, _default );

        return value;
    }
    //////////////////////////////////////////////////////////////////////////
    float JSONSetting::getValue( const Char * _key, float _default ) const
    {
        const jpp::object & j = m_storage->getJSON();

        float value = j.get( _key, _default );

        return value;
    }
    //////////////////////////////////////////////////////////////////////////
    const Char * JSONSetting::getValue( const Char * _key, const Char * _default ) const
    {
        const jpp::object & j = m_storage->getJSON();

        const Char * value = j.get( _key, _default );

        return value;
    }
    //////////////////////////////////////////////////////////////////////////
    mt::vec2f JSONSetting::getValue( const Char * _key, const mt::vec2f & _default ) const
    {
        const jpp::object & j = m_storage->getJSON();

        if( j.exist( _key ) == false )
        {
            return _default;
        }

        jpp::object jv2 = j[_key];

        mt::vec2f value( jv2["x"], jv2["y"] );

        return value;
    }
    //////////////////////////////////////////////////////////////////////////
    mt::vec3f JSONSetting::getValue( const Char * _key, const mt::vec3f & _default ) const
    {
        const jpp::object & j = m_storage->getJSON();

        if( j.exist( _key ) == false )
        {
            return _default;
        }

        jpp::object jv3 = j[_key];

        mt::vec3f value( jv3["x"], jv3["y"], jv3["z"] );

        return value;
    }
    //////////////////////////////////////////////////////////////////////////
    Color JSONSetting::getValue( const Char * _key, const Color & _default ) const
    {
        const jpp::object & j = m_storage->getJSON();

        if( j.exist( _key ) == false )
        {
            return _default;
        }

        jpp::object jc = j[_key];

        Color value( jc["r"], jc["g"], jc["b"], jc.get( "a", 1.f ) );

        return value;
    }
}