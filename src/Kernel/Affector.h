#	pragma once

#	include "Kernel/AffectorType.h"

#	include "Kernel/Scriptable.h"

#   include "Factory/Factorable.h"
#   include "Factory/FactoryStore.h"

#	include "Core/ValueInterpolator.h"

#   include "stdex/intrusive_slug_linked.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	typedef uint32_t AFFECTOR_ID;
	//////////////////////////////////////////////////////////////////////////
	static const AFFECTOR_ID INVALID_AFFECTOR_ID = 0;
	//////////////////////////////////////////////////////////////////////////
    class ServiceProviderInterface;
	//////////////////////////////////////////////////////////////////////////
	class AffectorCallback
		: public FactorablePtr
	{
	public:
		virtual void onAffectorEnd( AFFECTOR_ID _id, bool _isEnd ) = 0;
	};
	//////////////////////////////////////////////////////////////////////////
	typedef stdex::intrusive_ptr<AffectorCallback> AffectorCallbackPtr;
	//////////////////////////////////////////////////////////////////////////
	class Affector
        : public stdex::intrusive_slug_linked<Affector>
        , public Factorable
	{
	public:
		Affector();
		virtual ~Affector();

    public:
        void setServiceProvider( ServiceProviderInterface * _serviceProvider );
		void setAffectorType( EAffectorType _type );

	public:
		void setId( AFFECTOR_ID _id );
		AFFECTOR_ID getId() const;

	public:
		EAffectorType getType() const;

	public:
		void setFreeze( bool _value );
		bool getFreeze() const;

	public:
		void setSpeedFactor( float _speed );
		float getSpeedFactor() const;

	public:
		bool prepare();

	protected:
		virtual bool _prepare();

	public:
		bool affect( float _timing );

	protected:
		virtual bool _affect( float _timing ) = 0;

	public:
        virtual void complete() = 0;
		virtual void stop() = 0;

    protected:
        ServiceProviderInterface * m_serviceProvider;

		EAffectorType m_type;
		AFFECTOR_ID m_id;

		float m_speedFactor;
		
		bool m_freeze;
	};
	//////////////////////////////////////////////////////////////////////////
	class CallbackAffector
		: public Affector
	{
	public:
		CallbackAffector();
		~CallbackAffector();

	public:
		void setCallback( const AffectorCallbackPtr & _cb );
		const AffectorCallbackPtr & getCallback() const;

	protected:
		void complete() override;

	protected:
		void end_( bool _isEnd );

	protected:
		AffectorCallbackPtr m_cb;
	};
    //////////////////////////////////////////////////////////////////////////
	template<class C, class M>
	class MemeberAffector
		: public CallbackAffector
	{
	public:
        MemeberAffector()
            : m_self(nullptr)
            , m_method(nullptr)
		{
		}

        void initialize( C * _self, M _method )
        {			
            m_self = _self;
            m_method = _method;
        }

		template<class T>
		void update( T _value )
		{
			(m_self->*m_method)( _value );
		}

	protected:
		C * m_self;
		M m_method;
	};
    //////////////////////////////////////////////////////////////////////////
	template<class C, class M, class T, template<class> class Accumulator>
	class MemberAffectorAccumulate
		: public MemeberAffector<C,M>
	{
	protected:
		bool _affect( float _timing ) override
		{
			T value;
			bool finish = m_accumulator.update( _timing, &value );

			this->update( value );

			if( finish == false )
			{
				return false;
			}

			return true;
		}

		void stop() override
		{
			m_accumulator.stop();

			this->end_( false );
		}

	protected:
		Accumulator<T> m_accumulator;
	};
    //////////////////////////////////////////////////////////////////////////
	template<class C, class M, class T, class Interpolator>
	class MemberAffectorInterpolate
		: public MemeberAffector<C,M>
	{
	protected:
		bool _affect( float _timing ) override
		{
			T value;
			bool finish = m_interpolator.update( _timing, &value );

			this->update( value );

			if( finish == false )
			{
				return false;
			}

			return true;
		}

		void stop() override
		{
			m_interpolator.stop();

			this->end_( false );
		}

	protected:
		Interpolator m_interpolator;
	};
    //////////////////////////////////////////////////////////////////////////
	template<class C, class M, class T>
	class MemberAffectorAccumulateLinear
		: public MemberAffectorAccumulate<C,M,T,ValueAccumulateLinear>
	{
	public:
		template<class ABS>
		void initialize( C * _self, M _method, const T & _start, const T & _dir, float _speed, ABS _abs)
		{
            MemberAffectorAccumulate<C,M,T,ValueAccumulateLinear>::initialize( _self, _method );
			MemberAffectorAccumulate<C,M,T,ValueAccumulateLinear>::m_accumulator.start( _start, _dir, _speed, _abs );
		}        
	};

	template<class C, class M, class T>
	class MemberAffectorInterpolateLinear
		: public MemberAffectorInterpolate<C,M,T, ValueInterpolatorLinear<T> >
	{
	public:
		template<class ABS>
		void initialize( C * _self, M _method, const T & _start, const T & _end, float _time, ABS _abs)
		{
			MemberAffectorInterpolate<C, M, T, ValueInterpolatorLinear<T> >::initialize( _self, _method );
			MemberAffectorInterpolate<C, M, T, ValueInterpolatorLinear<T> >::m_interpolator.start( _start, _end, _time, _abs );
		}
	};
    //////////////////////////////////////////////////////////////////////////
	template<class C, class M, class T>
	class MemberAffectorInterpolateQuadratic
		: public MemberAffectorInterpolate<C,M,T, ValueInterpolatorQuadratic<T> >
	{
	public:
		template< typename ABS >
		void initialize( C * _self, M _method, const T & _start, const T & _end, const T & _v0, float _time, ABS _abs )
		{
			MemberAffectorInterpolate<C, M, T, ValueInterpolatorQuadratic<T> >::initialize( _self, _method );
			MemberAffectorInterpolate<C, M, T, ValueInterpolatorQuadratic<T> >::m_interpolator.start( _start, _end, _v0, _time, _abs );
		}
	};
    //////////////////////////////////////////////////////////////////////////
	template<class C, class M, class T, size_t N>
	class MemberAffectorInterpolateBezier
		: public MemberAffectorInterpolate<C,M,T, ValueInterpolatorBezier<T, N> >
	{
	public:
		void initialize( C * _self, M _method, const T & _start, const T & _end, const T * _v, float _time)
		{
			MemberAffectorInterpolate<C, M, T, ValueInterpolatorBezier<T, N> >::initialize( _self, _method );
			MemberAffectorInterpolate<C, M, T, ValueInterpolatorBezier<T, N> >::m_interpolator.start( _start, _end, _v, _time );
		}
	};
    //////////////////////////////////////////////////////////////////////////
	namespace NodeAffectorCreator
	{
        template<class C, class M, class T>
        class NodeAffectorCreatorAccumulateLinear
        {
        public:
            typedef MemberAffectorAccumulateLinear<C, M, T> AffectorType;

        public:
            template<class ABS>
			Affector * create( ServiceProviderInterface * _serviceProvider, EAffectorType _type, const AffectorCallbackPtr & _cb
                , C * _self, M _method
                , const T & _pos, const T & _dir, float _speed, ABS _abs )
            {
                AffectorType * affector = m_factory.createObject();

				affector->setServiceProvider( _serviceProvider );
				affector->setAffectorType( _type );

				affector->setCallback( _cb );

                affector->initialize( _self, _method, _pos, _dir, _speed, _abs );
				affector->update( _pos );

                return affector;
            }

            typedef FactoryPoolStore<AffectorType, 4> TFactoryAffector;
            TFactoryAffector m_factory;
        };
        //////////////////////////////////////////////////////////////////////////
		template<class C, class M, class T>
        class NodeAffectorCreatorInterpolateLinear
        {
        public:
            typedef MemberAffectorInterpolateLinear<C, M, T> AffectorType;

        public:
            template<class ABS>
			Affector * create( ServiceProviderInterface * _serviceProvider, EAffectorType _type, const AffectorCallbackPtr & _cb
                , C * _self, M _method
                , const T & _start, const T & _end, float _time, ABS _abs )
            {
                AffectorType * affector = m_factory.createObject();

				affector->setServiceProvider( _serviceProvider );
				affector->setAffectorType( _type );

				affector->setCallback( _cb );

                affector->initialize( _self, _method, _start, _end, _time, _abs );
				affector->update( _start );

                return affector;
            }

        protected:
            typedef FactoryPoolStore<AffectorType, 4> TFactoryAffector;
            TFactoryAffector m_factory;
        };
        //////////////////////////////////////////////////////////////////////////
		template<class C, class M, class T>
        class NodeAffectorCreatorInterpolateQuadratic
        {
        public:
            typedef MemberAffectorInterpolateQuadratic<C, M, T> AffectorType;

        public:
            template<class ABS>
			Affector * create( ServiceProviderInterface * _serviceProvider, EAffectorType _type, const AffectorCallbackPtr & _cb
			    , C * _self, M _method
			    , const T & _start, const T & _end, const T & _v0, float _time, ABS _abs )
		    {
                AffectorType * affector = m_factory.createObject();

				affector->setServiceProvider( _serviceProvider );
				affector->setAffectorType( _type );

				affector->setCallback( _cb );

                affector->initialize( _self, _method, _start, _end, _v0, _time, _abs );
				affector->update( _start );

                return affector;
		    }

        protected:
            typedef FactoryPoolStore<AffectorType, 4> TFactoryAffector;
            TFactoryAffector m_factory;
        };
        //////////////////////////////////////////////////////////////////////////
		template<class C, class M, class T, size_t N>
        class NodeAffectorCreatorInterpolateBezier
        {
        public:
            typedef MemberAffectorInterpolateBezier<C, M, T, N> AffectorType;

        public:
			Affector * create( ServiceProviderInterface * _serviceProvider, EAffectorType _type, const AffectorCallbackPtr & _cb
			    , C * _self, M _method
			    , const T & _start, const T & _end, const T * _v, float _time )
		    {
                AffectorType * affector = m_factory.createObject();

				affector->setServiceProvider( _serviceProvider );
				affector->setAffectorType( _type );

				affector->setCallback( _cb );

                affector->initialize( _self, _method, _start, _end, _v, _time );
				affector->update( _start );

                return affector;
		    }

        protected:
            typedef FactoryPoolStore<AffectorType, 4> TFactoryAffector;
            TFactoryAffector m_factory;
        };
		//////////////////////////////////////////////////////////////////////////
	}
}	// namespace Menge
