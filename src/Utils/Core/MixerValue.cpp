#include "MixerValue.h"

namespace Mengine
{
	//////////////////////////////////////////////////////////////////////////
	MixerValue::MixerValue()
        : m_speed( 0.0005f )
	{
	}
    //////////////////////////////////////////////////////////////////////////
    void MixerValue::setSpeed( float _speed )
    {
        m_speed = _speed;
    }
    //////////////////////////////////////////////////////////////////////////
    float MixerValue::getSpeed() const
    {
        return m_speed;
    }
	//////////////////////////////////////////////////////////////////////////
	void MixerValue::setValue( const ConstString & _type, float _value, float _from, bool _force )
	{
		for( TMixerElement::iterator
			it = m_mixer.begin(),
			it_end = m_mixer.end();
		it != it_end;
		++it )
		{
			Element & mixer = *it;

			if( mixer.type != _type )
			{
				continue;
			}

            if( _force == true )
            {
                mixer.follower.setValue( _from );
            }

			mixer.follower.setFollow( _value );
            mixer.follower.setSpeed( m_speed );

			return;
		}

        if( _value == 1.f && _from == 1.f )
        {
            return;
        }

		Element el;
		el.type = _type;
        el.follower.setValue( _from );
		el.follower.setFollow( _value );
		el.follower.setSpeed( m_speed );

		m_mixer.emplace_back( el );
	}
	//////////////////////////////////////////////////////////////////////////
	float MixerValue::getValue( const ConstString & _type ) const
	{
		for( TMixerElement::const_iterator
			it = m_mixer.begin(),
			it_end = m_mixer.end();
		it != it_end;
		++it )
		{
			const Element & mixer = *it;

			if( mixer.type != _type )
			{
				continue;
			}

			float value = mixer.follower.getValue();

			return value;
		}

		return 1.f;
	}
	//////////////////////////////////////////////////////////////////////////
	float MixerValue::mixValue() const
	{
		float volume = 1.f;

		for( TMixerElement::const_iterator
			it = m_mixer.begin(),
			it_end = m_mixer.end();
		it != it_end;
		++it )
		{
			const Element & mixer = *it;

			float value = mixer.follower.getValue();

			volume *= value;
		}

		return volume;
	}
	//////////////////////////////////////////////////////////////////////////
	bool MixerValue::update( float _current, float _timing )
	{
		bool process = false;

		for( TMixerElement::iterator
			it = m_mixer.begin(),
			it_end = m_mixer.end();
		it != it_end;
		++it )
		{
			Element & m = *it;

			if( m.follower.update( _current, _timing ) == false )
			{
				process = true;
			}
		}

		return process;
	}
}