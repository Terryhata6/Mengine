#	include "Layer.h"

#	include "XmlEngine.h"
#	include "RenderEngine.h"

#	include "Scene.h"
#	include "HotSpot.h"
#	include "Arrow.h"

#	include "Player.h"
#	include "MousePickerSystem.h"

namespace Menge
{
	//////////////////////////////////////////////////////////////////////////
	Layer::Layer()
		: m_main( false )
		, m_size( 0.f, 0.f )
		, m_scene( 0 )
		, m_mousePickerSystem( 0 )
	{
		m_mousePickerSystem = new MousePickerSystem();
		
		m_layer = this;
	}
	//////////////////////////////////////////////////////////////////////////
	Layer::~Layer()
	{
		if( m_mousePickerSystem )
		{
			delete m_mousePickerSystem;
			m_mousePickerSystem = 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Layer::setMain( bool _main )
	{
		m_main = _main;

		m_scene->setMainLayer( this );
	}
	//////////////////////////////////////////////////////////////////////////
	bool Layer::isMain() const
	{
		return m_main;
	}
	//////////////////////////////////////////////////////////////////////////
	void Layer::setScene( Scene * _scene )
	{
		m_scene = _scene;
	}
	//////////////////////////////////////////////////////////////////////////
	Scene * Layer::getScene() const
	{
		return m_scene;
	}
	//////////////////////////////////////////////////////////////////////////
	void Layer::setSize( const mt::vec2f & _size )
	{
		m_size = _size;
	}
	//////////////////////////////////////////////////////////////////////////
	const mt::vec2f & Layer::getSize() const
	{
		return m_size;
	}
	//////////////////////////////////////////////////////////////////////////
	void Layer::setLayer( Layer * _layer )
	{
		Node::setLayer( this );
	}
	//////////////////////////////////////////////////////////////////////////
	void Layer::loader( XmlElement * _xml )
	{
		Node::loader( _xml );

		XML_SWITCH_NODE( _xml )
		{
			XML_CASE_ATTRIBUTE_NODE_METHOD( "Main", "Value", &Layer::setMain );
			XML_CASE_ATTRIBUTE_NODE( "Size", "Value", m_size );

		}
	}
	//////////////////////////////////////////////////////////////////////////
	void Layer::setRenderTarget( const std::string& _cameraName )
	{
		//m_renderTarget = _cameraName;
	}
	////////////////////////////////////////////////////////////////////////////
	//void Layer::setRenderViewport( const Viewport & _viewport )
	//{
	//	const mt::mat3f& wm = getWorldMatrix();
	//	mt::vec2f min, max;
	//	mt::mul_v2_m3( min, _viewport.begin, wm );
	//	mt::mul_v2_m3( max, _viewport.end, wm );
	//	m_renderViewport.begin = min;
	//	m_renderViewport.end = max;
	//}
	////////////////////////////////////////////////////////////////////////////
	//const Viewport & Layer::getRenderViewport() const
	//{
	//	return m_renderViewport;
	//}
	//////////////////////////////////////////////////////////////////////////
	void Layer::_deactivate()
	{
		m_mousePickerSystem->clear();
	}
	//////////////////////////////////////////////////////////////////////////
	void Layer::_update( float _timing )
	{
		Arrow * picker = 
			Player::hostage()->getArrow();

		m_mousePickerSystem->update( picker );
	}
	//////////////////////////////////////////////////////////////////////////
	MousePickerSystem * Layer::getMousePickerSystem()
	{
		return m_mousePickerSystem;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Layer::testBoundingBox( const Viewport & _viewport, const mt::box2f & _layerspaceBox, const mt::box2f & _screenspaceBox ) const
	{
		mt::box2f convertBox = _screenspaceBox;
		convertBox.minimum += _viewport.begin;
		convertBox.maximum += _viewport.begin;

		bool result = mt::is_intersect( _layerspaceBox, convertBox );
		
		return result;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Layer::testArrow( const Viewport & _viewport, HotSpot * _layerspaceHotspot, Arrow * _arrow ) const
	{
		/*const mt::vec2f & dirA = _layerspaceHotspot->getWorldDirection();
		const mt::vec2f & posA = _layerspaceHotspot->getScreenPosition();

		const mt::vec2f & dirB = _screenspaceHotspot->getWorldDirection();
		const mt::vec2f & posB = _screenspaceHotspot->getWorldPosition();

		const mt::polygon & layerspacePolygon = _layerspaceHotspot->getPolygon();
		const mt::polygon & screenspacePolygon = _screenspaceHotspot->getPolygon();

		bool is_intersect = mt::intersect_poly_poly( 
			layerspacePolygon, 
			screenspacePolygon,
			dirA, 
			posA, 
			dirB, 
			posB 
			);

		return is_intersect;*/
		const mt::polygon & screenPoly = _arrow->getPolygon();

		mt::mat3f lwm = _layerspaceHotspot->getWorldMatrix();
		lwm.v2 = mt::vec3f( _layerspaceHotspot->getScreenPosition(), 1.0f );

		const mt::mat3f & swm = _arrow->getWorldMatrix();
		
		return _layerspaceHotspot->testPolygon( lwm, screenPoly, swm );
	}
	//////////////////////////////////////////////////////////////////////////
	bool Layer::testPoint( const Viewport& _viewport, HotSpot* _layerspaceHotspot, const mt::vec2f& _point ) const
	{
		const mt::vec2f & dirA = _layerspaceHotspot->getWorldDirection();
		const mt::vec2f & posA = _layerspaceHotspot->getScreenPosition();

		const mt::polygon & layerspacePolygon = _layerspaceHotspot->getPolygon();

		bool is_intersect = mt::is_point_inside_polygon( 
			layerspacePolygon, 
			_point,
			posA, 
			dirA
			);

		return is_intersect;
	}
	//////////////////////////////////////////////////////////////////////////
	mt::vec2f Layer::calcScreenPosition( const Viewport& _viewport, Node* _node ) const
	{
		return _node->getWorldPosition() - _viewport.begin;
	}
	//////////////////////////////////////////////////////////////////////////
	void Layer::_updateBoundingBox( mt::box2f& _boundingBox )
	{
		// do nothing
	}
	//////////////////////////////////////////////////////////////////////////
	Camera* Layer::getCamera()
	{
		return NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Layer::handleKeyEvent( unsigned int _key, unsigned int _char, bool _isDown )
	{
		bool handle = false;

		if( handle == false )
		{
			Arrow * arrow = 
				Player::hostage()->getArrow();

			handle = m_mousePickerSystem->handleKeyEvent( arrow, _key, _char, _isDown );
		}

		return handle;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Layer::handleMouseButtonEvent( unsigned int _button, bool _isDown )
	{
		bool handle = false;

		if( handle == false )
		{
			Arrow * arrow = 
				Player::hostage()->getArrow();

			handle = m_mousePickerSystem->handleMouseButtonEvent( arrow, _button, _isDown );
		}

		return handle;
	}
	//////////////////////////////////////////////////////////////////////////
	bool Layer::handleMouseMove( float _x, float _y, int _whell )
	{
		bool handle = false;

		if( handle == false )
		{
			Arrow * arrow = 
				Player::hostage()->getArrow();

			handle = m_mousePickerSystem->handleMouseMove( arrow, _x, _y, _whell );
		}

		return handle;
	}

}
