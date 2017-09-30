#	include "Movie2.h"

#   include "Interface/TimelineInterface.h"
#   include "Interface/NodeInterface.h"
#   include "Interface/StringizeInterface.h"
#   include "Interface/PrototypeManagerInterface.h"

#	include "Menge/SurfaceVideo.h"
#	include "Menge/SurfaceSound.h"
#	include "Menge/SurfaceTrackMatte.h"

#	include "Kernel/Materialable.h"

#	include "Logger/Logger.h"

#	include "Math/quat.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	Movie2::Movie2()
		: m_composition(nullptr)
	{	
	}
	//////////////////////////////////////////////////////////////////////////
	Movie2::~Movie2()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::setResourceMovie2( const ResourceMovie2Ptr & _resourceMovie2 )
	{
		if( m_resourceMovie2 == _resourceMovie2 )
		{
			return;
		}

		m_resourceMovie2 = _resourceMovie2;

		this->recompile();
	}
	//////////////////////////////////////////////////////////////////////////
	const ResourceMovie2Ptr & Movie2::getResourceMovie2() const
	{
		return m_resourceMovie2;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::setCompositionName( const ConstString & _name )
	{
		m_compositionName = _name;
	}
	//////////////////////////////////////////////////////////////////////////
	const ConstString & Movie2::getCompositionName() const
	{
		return m_compositionName;
	}
	//////////////////////////////////////////////////////////////////////////
	float Movie2::getDuration() const
	{
		float duration = ae_get_movie_composition_duration( m_composition );

		return duration;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::setWorkAreaFromEvent( const ConstString & _eventName )
	{
		float a, b;
		ae_bool_t ok = ae_get_movie_composition_node_in_out_time( m_composition, _eventName.c_str(), AE_MOVIE_LAYER_TYPE_EVENT, &a, &b );

		if( ok == AE_FALSE )
		{
			return;
		}

		ae_set_movie_composition_work_area( m_composition, a, b );
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::removeWorkArea()
	{
		ae_remove_movie_composition_work_area( m_composition );
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::playSubComposition( const ConstString & _name )
	{
		const aeMovieSubComposition * subcomposition = ae_get_movie_sub_composition( m_composition, _name.c_str() );

		ae_play_movie_sub_composition( m_composition, subcomposition, 0.f );
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::stopSubComposition( const ConstString & _name )
	{
		const aeMovieSubComposition * subcomposition = ae_get_movie_sub_composition( m_composition, _name.c_str() );

		ae_stop_movie_sub_composition( m_composition, subcomposition );
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::_play( float _time )
	{
		(void)_time;

		ae_play_movie_composition( m_composition, _time );

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::_restart( uint32_t _enumerator, float _time )
	{
		(void)_time;
		(void)_enumerator;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_pause( uint32_t _enumerator )
	{
		(void)_enumerator;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_resume( uint32_t _enumerator, float _time )
	{
		(void)_time;
		(void)_enumerator;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_stop( uint32_t _enumerator )
	{
		(void)_enumerator;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_end( uint32_t _enumerator )
	{
		(void)_enumerator;
	}
	//////////////////////////////////////////////////////////////////////////
	static void * ae_movie_composition_camera_provider( const aeMovieCameraProviderCallbackData * _callbackData, void * _data )
	{
		Movie2 * movie2 = (Movie2 *)_data;

		ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();

		ConstString c_name = Helper::stringizeString( serviceProvider, _callbackData->name );

		Movie2::Camera * old_camera;
		if( movie2->getCamera( c_name, &old_camera ) == true )
		{
			return old_camera;
		}
		
		RenderCameraProjection * renderCameraProjection = NODE_SERVICE( serviceProvider )
			->createNodeT<RenderCameraProjection *>( STRINGIZE_STRING_LOCAL( serviceProvider, "RenderCameraProjection" ) );

		renderCameraProjection->setName( c_name );

        mt::vec3f cameraTarget;
        cameraTarget.from_f3( _callbackData->target );
		mt::vec3f cameraPosition;
		cameraPosition.from_f3( _callbackData->position );

		mt::vec3f cameraDirection;
		mt::norm_v3_v3( cameraDirection, cameraTarget - cameraPosition );

		float aspect = _callbackData->width / _callbackData->height;

		renderCameraProjection->setCameraPosition( cameraPosition );
		renderCameraProjection->setCameraDirection( cameraDirection );
		renderCameraProjection->setCameraFOV( _callbackData->fov );
		renderCameraProjection->setCameraAspect( aspect );
		
		RenderViewport * renderViewport = NODE_SERVICE( serviceProvider )
			->createNodeT<RenderViewport *>( STRINGIZE_STRING_LOCAL( serviceProvider, "RenderViewport" ) );

		renderViewport->setName( c_name );

		Viewport vp;
		vp.begin.x = 0.f;
		vp.begin.y = 0.f;

		vp.end.x = _callbackData->width;
		vp.end.y = _callbackData->height;

		renderViewport->setViewport( vp );
		
		Movie2::Camera * new_camera = movie2->addCamera( c_name, renderCameraProjection, renderViewport );

		return new_camera;
	}
    //////////////////////////////////////////////////////////////////////////
    static void ae_movie_composition_camera_deleter( const aeMovieCameraDestroyCallbackData * _callbackData, void * _data )
    {
        Movie2 * movie2 = (Movie2 *)_data;

        ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();
        
        ConstString c_name = Helper::stringizeString( serviceProvider, _callbackData->name );

        movie2->removeCamera( c_name );
    }
    //////////////////////////////////////////////////////////////////////////
    static void ae_movie_composition_camera_update( const aeMovieCameraUpdateCallbackData * _callbackData, void * _data )
    {
        (void)_data;

        Movie2::Camera * camera = (Movie2::Camera *)(_callbackData->element);
        
        //camera
        mt::vec3f cameraTarget;
        cameraTarget.from_f3( _callbackData->target );
        mt::vec3f cameraPosition;
        cameraPosition.from_f3( _callbackData->position );

        mt::vec3f cameraDirection;
        mt::norm_v3_v3( cameraDirection, cameraTarget - cameraPosition );

        camera->projection->setCameraPosition( cameraPosition );
        camera->projection->setCameraDirection( cameraDirection );
    }
	//////////////////////////////////////////////////////////////////////////
	static void * ae_movie_composition_node_provider( const aeMovieNodeProviderCallbackData * _callbackData, void * _data )
	{
		Movie2 * movie2 = (Movie2 *)_data;

		ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();
		
		const char * layer_name = ae_get_movie_layer_data_name( _callbackData->layer );

		ConstString c_name = Helper::stringizeString( serviceProvider, layer_name );
		
		ae_bool_t is_track_matte = ae_is_movie_layer_data_track_mate( _callbackData->layer );

		if( is_track_matte == AE_TRUE )
		{
			return nullptr;
		}

		aeMovieLayerTypeEnum type = ae_get_movie_layer_data_type( _callbackData->layer );

		if( _callbackData->trackmatteLayer != AE_NULL )
		{
			switch( type )
			{
			case AE_MOVIE_LAYER_TYPE_IMAGE:
				{
					SurfaceTrackMatte * surfaceTrackMatte = PROTOTYPE_SERVICE( serviceProvider )
						->generatePrototype( STRINGIZE_STRING_LOCAL( serviceProvider, "Surface" ), STRINGIZE_STRING_LOCAL( serviceProvider, "SurfaceTrackMatte" ) );

					surfaceTrackMatte->setName( c_name );

					ResourceImage * resourceImage = (ResourceImage *)ae_get_movie_layer_data_resource_data( _callbackData->layer );
					ResourceImage * resourceTrackMatteImage = (ResourceImage *)ae_get_movie_layer_data_resource_data( _callbackData->trackmatteLayer );

					surfaceTrackMatte->setResourceImage( resourceImage );
					surfaceTrackMatte->setResourceTrackMatteImage( resourceTrackMatteImage );

					EMaterialBlendMode blend_mode = EMB_NORMAL;
					
					aeMovieBlendMode layer_blend_mode = ae_get_movie_layer_data_blend_mode( _callbackData->layer );

					switch( layer_blend_mode )
					{
					case AE_MOVIE_BLEND_ADD:
						blend_mode = EMB_ADD;
						break;
					case AE_MOVIE_BLEND_SCREEN:
						blend_mode = EMB_SCREEN;
						break;
					};

					surfaceTrackMatte->setBlendMode( blend_mode );

					surfaceTrackMatte->compile();

					movie2->addSurface( surfaceTrackMatte );

					return surfaceTrackMatte;
				}break;
			default:
				{
				}break;
			}
		}
		else
		{
			switch( type )
			{
			case AE_MOVIE_LAYER_TYPE_VIDEO:
				{
					SurfaceVideo * surfaceVideo = PROTOTYPE_SERVICE( serviceProvider )
						->generatePrototype( STRINGIZE_STRING_LOCAL( serviceProvider, "Surface" ), STRINGIZE_STRING_LOCAL( serviceProvider, "SurfaceVideo" ) );

					surfaceVideo->setName( c_name );

					ResourceVideo * resourceVideo = (ResourceVideo *)ae_get_movie_layer_data_resource_data( _callbackData->layer );

					surfaceVideo->setResourceVideo( resourceVideo );

					EMaterialBlendMode blend_mode = EMB_NORMAL;

					aeMovieBlendMode layer_blend_mode = ae_get_movie_layer_data_blend_mode( _callbackData->layer );

					switch( layer_blend_mode )
					{
					case AE_MOVIE_BLEND_ADD:
						blend_mode = EMB_ADD;
						break;
					case AE_MOVIE_BLEND_SCREEN:
						blend_mode = EMB_SCREEN;
						break;
					};

					surfaceVideo->setBlendMode( blend_mode );

					surfaceVideo->compile();

					movie2->addSurface( surfaceVideo );

					return surfaceVideo;
				}break;
			case AE_MOVIE_LAYER_TYPE_SOUND:
				{
					SurfaceSound * surfaceSound = PROTOTYPE_SERVICE( serviceProvider )
						->generatePrototype( STRINGIZE_STRING_LOCAL( serviceProvider, "Surface" ), STRINGIZE_STRING_LOCAL( serviceProvider, "SurfaceSound" ) );

					surfaceSound->setName( c_name );

					ResourceSound * resourceSound = (ResourceSound *)ae_get_movie_layer_data_resource_data( _callbackData->layer );

					surfaceSound->setResourceSound( resourceSound );

					surfaceSound->compile();

					movie2->addSurface( surfaceSound );

					return surfaceSound;
				}break;
			}
		}
				
		return AE_NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	static void ae_movie_composition_node_deleter( const aeMovieNodeDestroyCallbackData * _callbackData, void * _data )
	{
		(void)_callbackData;
		(void)_data;
	}
	//////////////////////////////////////////////////////////////////////////
	static void ae_movie_composition_node_update( const aeMovieNodeUpdateCallbackData * _callbackData, void * _data )
	{
		Movie2 * movie2 = (Movie2 *)_data;

		switch( _callbackData->state )
		{
		case AE_MOVIE_NODE_UPDATE_UPDATE:
			{
				switch( _callbackData->type )
				{
				case AE_MOVIE_LAYER_TYPE_PARTICLE:
					{
						//printf( "AE_MOVIE_LAYER_TYPE_PARTICLE %f %f\n"
						//	, _matrix[12]
						//	, _matrix[13]
						//	);
					}break;
				case AE_MOVIE_LAYER_TYPE_SLOT:
					{
						//printf( "AE_MOVIE_LAYER_TYPE_SLOT %f %f\n"
						//	, _matrix[12]
						//	, _matrix[13]
						//	);
					}break;
				}
			}break;
		case AE_MOVIE_NODE_UPDATE_BEGIN:
			{
				switch( _callbackData->type )
				{
				case AE_MOVIE_LAYER_TYPE_VIDEO:
					{
						SurfaceVideo * surfaceVide = (SurfaceVideo *)_callbackData->element;

						ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();

						float time = TIMELINE_SERVICE( serviceProvider )
							->getTime();

						surfaceVide->setTiming( _callbackData->offset );

						if( surfaceVide->play( time ) == 0 )
						{
							return;
						}
					}break;
				case AE_MOVIE_LAYER_TYPE_SOUND:
					{
						SurfaceSound * surfaceSound = (SurfaceSound *)_callbackData->element;

						ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();

						float time = TIMELINE_SERVICE( serviceProvider )
							->getTime();

						surfaceSound->setTiming( _callbackData->offset );

						if( surfaceSound->play( time ) == 0 )
						{
							return;
						}
					}break;
				}
			}break;
		case AE_MOVIE_NODE_UPDATE_END:
			{
				switch( _callbackData->type )
				{
				case AE_MOVIE_LAYER_TYPE_VIDEO:
					{
						SurfaceVideo * surfaceVide = (SurfaceVideo *)_callbackData->element;

						surfaceVide->stop();
					}break;
				case AE_MOVIE_LAYER_TYPE_SOUND:
					{
						SurfaceSound * surfaceSound = (SurfaceSound *)_callbackData->element;

						surfaceSound->stop();
					}break;
				}
			}break;
		case AE_MOVIE_NODE_UPDATE_PAUSE:
			{
				switch( _callbackData->type )
				{
				case AE_MOVIE_LAYER_TYPE_VIDEO:
					{
						SurfaceVideo * surfaceVide = (SurfaceVideo *)_callbackData->element;

						surfaceVide->pause();						
					}break;
				case AE_MOVIE_LAYER_TYPE_SOUND:
					{
						SurfaceSound * surfaceSound = (SurfaceSound *)_callbackData->element;
												
						surfaceSound->pause();						
					}break;
				}
			}break;
		case AE_MOVIE_NODE_UPDATE_RESUME:
			{
				switch( _callbackData->type )
				{
				case AE_MOVIE_LAYER_TYPE_VIDEO:
					{
						SurfaceVideo * surfaceVide = (SurfaceVideo *)_callbackData->element;

						ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();

						float time = TIMELINE_SERVICE( serviceProvider )
							->getTime();

						surfaceVide->resume( time );
					}break;
				case AE_MOVIE_LAYER_TYPE_SOUND:
					{
						SurfaceSound * surfaceSound = (SurfaceSound *)_callbackData->element;

						ServiceProviderInterface * serviceProvider = movie2->getServiceProvider();

						float time = TIMELINE_SERVICE( serviceProvider )
							->getTime();

						surfaceSound->resume( time );
					}break;
				}
			}break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	struct TrackMatteDesc
	{
		mt::mat4f matrix;
		aeMovieRenderMesh mesh;
	};
    //////////////////////////////////////////////////////////////////////////
    static void * ae_movie_composition_track_matte_provider( const aeMovieTrackMatteProviderCallbackData * _callbackData, void * _data )
    {
        (void)_data;

        TrackMatteDesc * desc = new TrackMatteDesc;

        desc->matrix.from_f16( _callbackData->matrix );
        desc->mesh = *_callbackData->mesh;

        return desc;
    }
	//////////////////////////////////////////////////////////////////////////
	static void ae_movie_composition_track_matte_update( const aeMovieTrackMatteUpdateCallbackData * _callbackData, void * _data )
	{
		(void)_data;

		switch( _callbackData->state )
		{
		case AE_MOVIE_NODE_UPDATE_BEGIN:
			{
				TrackMatteDesc * desc = static_cast<TrackMatteDesc *>(_callbackData->track_matte_data);

				desc->matrix.from_f16( _callbackData->matrix );
				desc->mesh = *_callbackData->mesh;
			}break;
		case AE_MOVIE_NODE_UPDATE_UPDATE:
			{
				TrackMatteDesc * desc = static_cast<TrackMatteDesc *>(_callbackData->track_matte_data);

				desc->matrix.from_f16( _callbackData->matrix );
				desc->mesh = *_callbackData->mesh;
			}break;
		}
	}
    //////////////////////////////////////////////////////////////////////////
    static void * ae_movie_composition_shader_provider( const aeMovieShaderProviderCallbackData * _callbackData, ae_voidptr_t _data )
    {
        (void)_callbackData;
        (void)_data;

        return nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    static void ae_movie_composition_shader_property_update( const aeMovieShaderPropertyUpdateCallbackData * _callbackData, ae_voidptr_t _data )
    {
        (void)_callbackData;
        (void)_data;

    }
	//////////////////////////////////////////////////////////////////////////
	static void ae_movie_composition_event( const aeMovieCompositionEventCallbackData * _callbackData, void * _data )
	{
		(void)_callbackData;
		(void)_data;
	}
	//////////////////////////////////////////////////////////////////////////
	static void ae_movie_composition_state( const aeMovieCompositionStateCallbackData * _callbackData, void * _data )
	{
		if( _callbackData->subcomposition != AE_NULL )
		{
			return;
		}

		Movie2 * m2 = (Movie2 *)(_data);

		if( _callbackData->state == AE_MOVIE_COMPOSITION_END )
		{
			printf( "AE_MOVIE_COMPOSITION_END\n" );

			m2->stop();
			//ae_destroy_movie_composition( m2->m_composition );
			//m2->m_composition = nullptr;
			


		}
	}
	//////////////////////////////////////////////////////////////////////////
	Movie2::Camera * Movie2::addCamera( const ConstString & _name, RenderCameraProjection * _projection, RenderViewport * _viewport )
	{
		this->addChild( _projection );
		this->addChild( _viewport );

		Camera c;
		c.projection = _projection;
		c.viewport = _viewport;
		
		TMapCamera::iterator it_found = m_cameras.insert( std::make_pair(_name, c) ).first;

		Camera * new_camera = &it_found->second;

		return new_camera;
	}
    //////////////////////////////////////////////////////////////////////////
    bool Movie2::removeCamera( const ConstString & _name )
    {
        TMapCamera::iterator it_found = m_cameras.find( _name );

        if( it_found == m_cameras.end() )
        {
            return false;
        }

        Camera & c = it_found->second;

        c.projection->destroy();
        c.viewport->destroy();

        m_cameras.erase( it_found );

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::hasCamera( const ConstString & _name ) const
	{ 
		TMapCamera::const_iterator it_found = m_cameras.find( _name );

		if( it_found == m_cameras.end() )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::getCamera( const ConstString & _name, Camera ** _camera )
	{
		TMapCamera::iterator it_found = m_cameras.find( _name );

		if( it_found == m_cameras.end() )
		{
			return false;
		}

		Camera & camera = it_found->second;

		*_camera = &camera;

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::_compile()
	{
		if( m_resourceMovie2 == nullptr )
		{
			LOGGER_ERROR(m_serviceProvider)("Movie2::_compile: '%s' can't setup resource"
				, this->getName().c_str()
				);

			return false;
		}

		if( m_resourceMovie2.compile() == false )
		{
			LOGGER_ERROR(m_serviceProvider)("Movie2::_compile '%s' resource %s not compile"
				, m_name.c_str() 
				, m_resourceMovie2->getName().c_str()
				);

			return false;
		}

		const aeMovieData * movieData = m_resourceMovie2->getMovieData();

		const aeMovieCompositionData * compositionData = m_resourceMovie2->getCompositionData( m_compositionName );

		if( compositionData == nullptr )
		{
			LOGGER_ERROR( m_serviceProvider )("Movie2::_compile '%s' resource %s not found composition '%s'"
				, m_name.c_str()
				, m_resourceMovie2->getName().c_str()
				, m_compositionName.c_str()
				);

			return false;
		}

		aeMovieCompositionProviders providers;
		providers.camera_provider = &ae_movie_composition_camera_provider;
        providers.camera_deleter = &ae_movie_composition_camera_deleter;
        providers.camera_update = &ae_movie_composition_camera_update;

		providers.node_provider = &ae_movie_composition_node_provider;
		providers.node_deleter = &ae_movie_composition_node_deleter;
		providers.node_update = &ae_movie_composition_node_update;

        providers.track_matte_provider = &ae_movie_composition_track_matte_provider;
		providers.track_matte_update = &ae_movie_composition_track_matte_update;

        providers.shader_provider = &ae_movie_composition_shader_provider;
        providers.shader_property_update = &ae_movie_composition_shader_property_update;

		providers.composition_event = &ae_movie_composition_event;
		providers.composition_state = &ae_movie_composition_state;
		

		//while( true )
		{
			aeMovieComposition * composition = ae_create_movie_composition( movieData, compositionData, AE_TRUE, &providers, this );

			if( composition == nullptr )
			{
				return false;
			}

			ae_set_movie_composition_loop( composition, AE_TRUE );

			uint32_t max_render_node = ae_get_movie_composition_max_render_node( composition );

			m_meshes.reserve( max_render_node );

			//ae_destroy_movie_composition( composition );
			m_composition = composition;
		}		
				
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_release()
	{	
		Node::_release();

        ae_delete_movie_composition( m_composition );
		m_composition = nullptr;

		m_resourceMovie2.release();
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::_activate()
	{
		if( Node::_activate() == false )
		{
			return false;
		}

		ae_vector3_t anchorPoint;
		if( ae_get_movie_composition_anchor_point( m_composition, anchorPoint ) == AE_TRUE )
		{	
			mt::vec3f origin;

			origin.from_f3( anchorPoint );

			this->setOrigin( origin );
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_deactivate()
	{
		Node::_deactivate();
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_changeParent( Node * _oldParent, Node * _newParent )
	{
		(void)_oldParent;
		(void)_newParent;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_addChild( Node * _node )
	{
		(void)_node;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_removeChild( Node * _node )
	{
		(void)_node;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_afterActivate()
	{
		Node::_afterActivate();

		bool autoPlay = this->getAutoPlay();

		if( autoPlay == true )
		{
			float time = TIMELINE_SERVICE( m_serviceProvider )
				->getTime();

			if( this->play( time ) == 0 )
			{
				LOGGER_ERROR( m_serviceProvider )("Movie2::_afterActivate '%s' resource '%s' auto play return 0"
					, this->getName().c_str()
					, this->m_resourceMovie2->getName().c_str()
					);

				return;
			}
		}		
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_setLoop( bool _value )
	{
		if( m_composition == nullptr )
		{
			return;
		}

		ae_set_movie_composition_loop( m_composition, _value );
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_setTiming( float _timing )
	{
		if( m_composition == nullptr )
		{
			return;
		}

		ae_set_movie_composition_time( m_composition, _timing );
	}
	//////////////////////////////////////////////////////////////////////////
	float Movie2::_getTiming() const
	{
		float timing = ae_get_movie_composition_time( m_composition );

		return timing;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_update( float _current, float _timing )
	{
		(void)_current;
		(void)_timing;

		if( this->isPlay() == false )
		{
			return;
		}

		static float a = 0.f;

		if( a < 500.f )
		{
			a += _timing;
			return;
		}
				
		static int i = 0;
		
		//if( (i++) % 10 == 0 && _timing > 0.f )
		{
			ae_update_movie_composition( m_composition, _timing );
		}
						
		for( TVectorSurfaces::iterator
			it = m_surfaces.begin(),
			it_end = m_surfaces.end();
		it != it_end;
		++it )
		{
			const SurfacePtr & surface = *it;

			surface->update( _current, _timing );
		}
		

		printf( "time %f\n"
			, ae_get_movie_composition_time( m_composition )
			);

		a += _timing;

		if( a < 2500.f )
		{			
			return;
		}
		
		static bool aa = false;
		if( aa == false )
		{
			aa = true;

			//const aeMovieSubComposition * subcomposition = ae_get_movie_sub_composition( m_composition, "lock" );

			//if( subcomposition != nullptr )
			//{
			//	ae_stop_movie_sub_composition( m_composition, subcomposition );
			//	ae_set_movie_sub_composition_loop( subcomposition, AE_FALSE );
			//	ae_play_movie_sub_composition( m_composition, subcomposition, 0.f );
			//}

			//ae_pause_movie_composition( m_composition );
		}

		if( a < 5000.f )
		{
			return;
		}

		static bool aa2 = false;
		if( aa2 == false )
		{
			aa2 = true;

			//const aeMovieSubComposition * subcomposition = ae_get_movie_sub_composition( m_composition, "lock" );

			//if( subcomposition != nullptr )
			//{
			//	ae_stop_movie_sub_composition( m_composition, subcomposition );
			//	ae_set_movie_sub_composition_loop( subcomposition, AE_FALSE );
			//	ae_play_movie_sub_composition( m_composition, subcomposition, 0.f );
			//}

			//ae_resume_movie_composition( m_composition );
		}

		if( a < 6000.f )
		{
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::_render( Menge::RenderServiceInterface * _renderService, const RenderObjectState * _state )
	{
		m_meshes.clear();

		const mt::mat4f & wm = this->getWorldMatrix();

        ae_voidptr_t composition_camera_data = ae_get_movie_composition_camera_data( m_composition );
        (void)composition_camera_data;

		uint32_t mesh_iterator = 0;

		aeMovieRenderMesh mesh;
		while( ae_compute_movie_mesh( m_composition, &mesh_iterator, &mesh ) == AE_TRUE )
		{			
			ResourceReference * resource_reference = (ResourceReference *)mesh.resource_data;

			RenderObjectState state;

            if( mesh.camera_data != nullptr )
			{
				Movie2::Camera * camera = (Movie2::Camera *)mesh.camera_data;

				state.camera = camera->projection;
				state.viewport = camera->viewport;
				state.clipplane = _state->clipplane;
				state.target = _state->target;
			}
			else
			{
				state.camera = _state->camera;
				state.viewport = _state->viewport;
				state.clipplane = _state->clipplane;
				state.target = _state->target;
			}

			if( mesh.track_matte_data == nullptr )
			{
				switch( mesh.layer_type )
				{
				case AE_MOVIE_LAYER_TYPE_SHAPE:
					{
						m_meshes.push_back( Mesh() );
						Mesh & m = m_meshes.back();

						m.vertices.resize( mesh.vertexCount );

						ColourValue_ARGB color = Helper::makeARGB( mesh.r, mesh.g, mesh.b, mesh.a );

						for( uint32_t index = 0; index != mesh.vertexCount; ++index )
						{
							RenderVertex2D & v = m.vertices[index];

							mt::vec3f vp;
							vp.from_f3( &mesh.position[index][0] );

							mt::mul_v3_v3_m4( v.position, vp, wm );

							mt::vec2f uv;
							uv.from_f2( &mesh.uv[index][0] );

							v.uv[0] = uv;
							v.uv[1] = uv;

							v.color = color;
						}

						m.indices.assign( mesh.indices, mesh.indices + mesh.indexCount );

						EMaterialBlendMode blend_mode = EMB_NORMAL;

						switch( mesh.blend_mode )
						{
						case AE_MOVIE_BLEND_ADD:
							blend_mode = EMB_ADD;
							break;
						};

						m.material = Helper::makeTextureMaterial( m_serviceProvider, nullptr, 0, ConstString::none(), blend_mode, false, false, false );

						_renderService
							->addRenderObject( &state, m.material, &m.vertices[0], m.vertices.size(), &m.indices[0], m.indices.size(), nullptr, false );
					}break;
				case AE_MOVIE_LAYER_TYPE_SOLID:
					{
						m_meshes.push_back( Mesh() );
						Mesh & m = m_meshes.back();

						m.vertices.resize( mesh.vertexCount );

						ColourValue_ARGB color = Helper::makeARGB( mesh.r, mesh.g, mesh.b, mesh.a );

						for( uint32_t index = 0; index != mesh.vertexCount; ++index )
						{
							RenderVertex2D & v = m.vertices[index];

							mt::vec3f vp;
							vp.from_f3( &mesh.position[index][0] );

							mt::mul_v3_v3_m4( v.position, vp, wm );

							mt::vec2f uv;
							uv.from_f2( &mesh.uv[index][0] );

							v.uv[0] = uv;
							v.uv[1] = uv;

							v.color = color;
						}

						m.indices.assign( mesh.indices, mesh.indices + mesh.indexCount );

						EMaterialBlendMode blend_mode = EMB_NORMAL;

						switch( mesh.blend_mode )
						{
						case AE_MOVIE_BLEND_ADD:
							blend_mode = EMB_ADD;
							break;
						};

						m.material = Helper::makeTextureMaterial( m_serviceProvider, nullptr, 0, ConstString::none(), blend_mode, false, false, false );

						_renderService
							->addRenderObject( &state, m.material, &m.vertices[0], m.vertices.size(), &m.indices[0], m.indices.size(), nullptr, false );
					}break;
				case AE_MOVIE_LAYER_TYPE_SEQUENCE:
				case AE_MOVIE_LAYER_TYPE_IMAGE:
					{
						ResourceImage * resource_image = static_cast<ResourceImage *>(resource_reference);

						m_meshes.push_back( Mesh() );
						Mesh & m = m_meshes.back();

						m.vertices.resize( mesh.vertexCount );

						ColourValue_ARGB color = Helper::makeARGB( mesh.r, mesh.g, mesh.b, mesh.a );

						for( uint32_t index = 0; index != mesh.vertexCount; ++index )
						{
							RenderVertex2D & v = m.vertices[index];

							mt::vec3f vp;
							vp.from_f3( &mesh.position[index][0] );

							mt::mul_v3_v3_m4( v.position, vp, wm );

							mt::vec2f uv;
							uv.from_f2( &mesh.uv[index][0] );

							const mt::uv4f & uv_image = resource_image->getUVImage();

							mt::multiply_tetragon_uv4_v2( v.uv[0], uv_image, uv );

							const mt::uv4f & uv_alpha = resource_image->getUVAlpha();

							mt::multiply_tetragon_uv4_v2( v.uv[1], uv_alpha, uv );

							v.color = color;
						}

						m.indices.assign( mesh.indices, mesh.indices + mesh.indexCount );

						EMaterialBlendMode blend_mode = EMB_NORMAL;

						switch( mesh.blend_mode )
						{
						case AE_MOVIE_BLEND_ADD:
							blend_mode = EMB_ADD;
							break;
						};

						m.material = Helper::makeImageMaterial( m_serviceProvider, resource_image, ConstString::none(), blend_mode, false, false );

						//printf( "%f %f\n", ae_get_movie_composition_time( m_composition ), mesh.a );

						if( m.vertices.size() == 0 )
						{
							continue;
						}

						_renderService
							->addRenderObject( &state, m.material, &m.vertices[0], m.vertices.size(), &m.indices[0], m.indices.size(), nullptr, false );
					}break;
				case AE_MOVIE_LAYER_TYPE_VIDEO:
					{
						//ResourceVideo * resource_video = static_cast<ResourceVideo *>(resource_reference);
						SurfaceVideo * surfaceVideo = static_cast<SurfaceVideo *>(mesh.element_data);

						m_meshes.push_back( Mesh() );
						Mesh & m = m_meshes.back();

						m.vertices.resize( mesh.vertexCount );

						ColourValue_ARGB color = Helper::makeARGB( mesh.r, mesh.g, mesh.b, mesh.a );

						for( uint32_t index = 0; index != mesh.vertexCount; ++index )
						{
							RenderVertex2D & v = m.vertices[index];

							mt::vec3f vp;
							vp.from_f3( &mesh.position[index][0] );

							mt::mul_v3_v3_m4( v.position, vp, wm );

							mt::vec2f uv;
							uv.from_f2( &mesh.uv[index][0] );

                            const mt::uv4f & uv0 = surfaceVideo->getUV( 0 );
                            const mt::uv4f & uv1 = surfaceVideo->getUV( 1 );
                            
							mt::multiply_tetragon_uv4_v2( v.uv[0], uv0, uv );
							mt::multiply_tetragon_uv4_v2( v.uv[1], uv1, uv );

							v.color = color;
						}

						m.indices.assign( mesh.indices, mesh.indices + mesh.indexCount );

						m.material = surfaceVideo->getMaterial();

						_renderService
							->addRenderObject( &state, m.material, &m.vertices[0], m.vertices.size(), &m.indices[0], m.indices.size(), nullptr, false );
					}break;
				}
			}
			else
			{
				switch( mesh.layer_type )
				{
				case AE_MOVIE_LAYER_TYPE_IMAGE:
					{
						const SurfaceTrackMatte * surfaceTrackMatte = static_cast<const SurfaceTrackMatte *>(mesh.element_data);

						if( surfaceTrackMatte == nullptr )
						{
							return;
						}

						m_meshes.push_back( Mesh() );
						Mesh & m = m_meshes.back();

						m.vertices.resize( mesh.vertexCount );

						ColourValue_ARGB color = Helper::makeARGB( mesh.r, mesh.g, mesh.b, mesh.a );

						const ResourceImagePtr & resourceImage = surfaceTrackMatte->getResourceImage();
						const ResourceImagePtr & resourceTrackMatteImage = surfaceTrackMatte->getResourceTrackMatteImage();

						const TrackMatteDesc * track_matte_desc = static_cast<const TrackMatteDesc *>(mesh.track_matte_data);

						const aeMovieRenderMesh * track_matte_mesh = &track_matte_desc->mesh;

						for( uint32_t index = 0; index != mesh.vertexCount; ++index )
						{
							RenderVertex2D & v = m.vertices[index];

							mt::vec3f vp;
							vp.from_f3( mesh.position[index] );

							mt::mul_v3_v3_m4( v.position, vp, wm );

							mt::vec2f uv;
							uv.from_f2( &mesh.uv[index][0] );
							
							//const mt::uv4f & uv_image = resourceImage->getUVImage();

							const RenderTextureInterfacePtr & texture_image = resourceImage->getTexture();

							const mt::uv4f & texture_image_uv = texture_image->getUV();

							mt::multiply_tetragon_uv4_v2( v.uv[0], texture_image_uv, uv );
							//mt::multiply_tetragon_uv4_v2( v.uv[0], texture_image_uv, v.uv[0] );

							mt::vec2f uv_track_matte;
							uv_track_matte = calc_point_uv(
								mt::vec2f( track_matte_mesh->position[0] ), mt::vec2f( track_matte_mesh->position[1] ), mt::vec2f( track_matte_mesh->position[2] ),
								mt::vec2f( track_matte_mesh->uv[0] ), mt::vec2f( track_matte_mesh->uv[1] ), mt::vec2f( track_matte_mesh->uv[2] ),
								vp.to_vec2f()
								);

							//const mt::uv4f & uv_alpha = resourceTrackMatteImage->getUVImage();
							const RenderTextureInterfacePtr & texture_trackmatte = resourceTrackMatteImage->getTexture();

							const mt::uv4f & texture_trackmatte_uv = texture_trackmatte->getUV();

							//mt::multiply_tetragon_uv4_v2( v.uv[1], uv_alpha, uv_track_matte );
							mt::multiply_tetragon_uv4_v2( v.uv[1], texture_trackmatte_uv, uv_track_matte );
							//mt::multiply_tetragon_uv4_v2( v.uv[1], , v.uv[1] );

							v.color = color;
						}

						m.indices.assign( mesh.indices, mesh.indices + mesh.indexCount );

						EMaterialBlendMode blend_mode = EMB_NORMAL;

						switch( mesh.blend_mode )
						{
						case AE_MOVIE_BLEND_ADD:
							blend_mode = EMB_ADD;
							break;
						};

						m.material = surfaceTrackMatte->getMaterial();

						//printf( "%f %f\n", ae_get_movie_composition_time( m_composition ), mesh.a );

						if( m.vertices.size() == 0 )
						{
							continue;
						}

						_renderService
							->addRenderObject( &state, m.material, &m.vertices[0], m.vertices.size(), &m.indices[0], m.indices.size(), nullptr, false );
					}
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	bool Movie2::_interrupt( uint32_t _enumerator )
	{
		(void)_enumerator;

		ae_interrupt_movie_composition( m_composition, AE_FALSE );

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	void Movie2::addSurface( const SurfacePtr & _surface )
	{
		m_surfaces.push_back( _surface );
	}
}
