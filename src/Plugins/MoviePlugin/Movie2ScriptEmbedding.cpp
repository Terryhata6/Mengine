#include "Movie2ScriptEmbedding.h"

#include "Interface/ScriptServiceInterface.h"
#include "Interface/VocabularyServiceInterface.h"

#include "Environment/Python/PythonAnimatableEventReceiver.h"
#include "Environment/Python/PythonScriptWrapper.h"
#include "Environment/Python/PythonDocumentTraceback.h"

#include "Movie2.h"

#include "Kernel/Logger.h"
#include "Kernel/AssertionMemoryPanic.h"
#include "Kernel/ConstStringHelper.h"

#include "pybind/pybind.hpp"
#include "pybind/stl/stl_type_cast.hpp"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    namespace Detail
    {
        //////////////////////////////////////////////////////////////////////////
        class PythonMovie2EventReceiver
            : public PythonAnimatableEventReceiver<Movie2EventReceiverInterface>
        {
        public:
            PythonMovie2EventReceiver()
            {
            }

            ~PythonMovie2EventReceiver() override
            {
            }
        };
        //////////////////////////////////////////////////////////////////////////
        static PyObject * s_Movie2_setEventListener( pybind::kernel_interface * _kernel, Movie2 * _node, PyObject * _args, PyObject * _kwds )
        {
            MENGINE_UNUSED( _args );

            MENGINE_ASSERTION_MEMORY_PANIC( _kwds );

            pybind::dict py_kwds( _kernel, _kwds );
            Helper::registerAnimatableEventReceiver<PythonMovie2EventReceiver>( _kernel, py_kwds, _node, MENGINE_DOCUMENT_PYBIND );

            MENGINE_ASSERTION_PYTHON_EVENT_RECEIVER( _node, py_kwds );

            return _kernel->ret_none();
        }
        //////////////////////////////////////////////////////////////////////////
        static mt::box2f s_Movie2_getCompositionBoundsWM( Movie2 * _movie )
        {
            const mt::box2f & bounds = _movie->getCompositionBounds();

            const TransformationInterface * transformation = _movie->getTransformation();

            const mt::mat4f & wm = transformation->getWorldMatrix();

            mt::box2f boundsWM;
            mt::mul_box2_m4( boundsWM, bounds, wm );

            return boundsWM;
        }
        //////////////////////////////////////////////////////////////////////////    
        static pybind::list s_Movie2_getSockets( pybind::kernel_interface * _kernel, Movie2 * _movie )
        {
            pybind::list py_list( _kernel );

            _movie->foreachSockets( [_kernel, &py_list]( Movie2 * _movie, uint32_t _index, const NodePtr & _node )
            {
                MENGINE_UNUSED( _index );

                HotSpotPolygonPtr hotspot = stdex::intrusive_static_cast<HotSpotPolygonPtr>(_node);

                if( hotspot == nullptr )
                {
                    return;
                }

                const ConstString & name = _node->getName();

                py_list.append( pybind::make_tuple_t( _kernel, _movie, name, hotspot ) );
            } );

            return py_list;
        }
        //////////////////////////////////////////////////////////////////////////    
        static pybind::list s_Movie2_getSlots( pybind::kernel_interface * _kernel, Movie2 * _movie )
        {
            pybind::list py_list( _kernel );

            _movie->foreachSlots( [_kernel, &py_list]( Movie2 * _movie, uint32_t _index, const NodePtr & _node )
            {
                MENGINE_UNUSED( _index );

                Movie2SlotPtr hotspot = stdex::intrusive_static_cast<Movie2SlotPtr>(_node);

                if( hotspot == nullptr )
                {
                    return;
                }

                const ConstString & name = _node->getName();

                py_list.append( pybind::make_tuple_t( _kernel, _movie, name, hotspot ) );
            } );

            return py_list;
        }
        //////////////////////////////////////////////////////////////////////////
        class PythonMovie2SubCompositionEventReceiver
            : public PythonAnimatableEventReceiver<Movie2SubCompositionEventReceiver>
        {
        public:
            PythonMovie2SubCompositionEventReceiver()
            {
            }

            ~PythonMovie2SubCompositionEventReceiver() override
            {
            }
        };
        //////////////////////////////////////////////////////////////////////////
        static PyObject * s_Movie2SubComposition_setEventListener( pybind::kernel_interface * _kernel, Movie2SubComposition * _node, PyObject * _args, PyObject * _kwds )
        {
            MENGINE_UNUSED( _args );

            MENGINE_ASSERTION_MEMORY_PANIC( _kwds );

            pybind::dict py_kwds( _kernel, _kwds );
            Helper::registerAnimatableEventReceiver<PythonMovie2SubCompositionEventReceiver>( _kernel, py_kwds, _node, MENGINE_DOCUMENT_PYBIND );

            MENGINE_ASSERTION_PYTHON_EVENT_RECEIVER( _node, py_kwds );

            return _kernel->ret_none();
        }
        //////////////////////////////////////////////////////////////////////////
        static pybind::list s_ResourceMovie2_getCompositionResources( pybind::kernel_interface * _kernel, ResourceMovie2 * _resourceMovie2, const ConstString & _compositionName )
        {
            const Movie2DataInterfacePtr & data = _resourceMovie2->getData();

            const aeMovieCompositionData * compositionData = _resourceMovie2->getCompositionData( _compositionName );

            pybind::list l = pybind::make_list_t( _kernel );

            data->visitCompositionDataResources( compositionData, [&l]( const ResourcePtr & _resource )
            {
                l.append( _resource );
            } );

            return l;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    Movie2ScriptEmbedding::Movie2ScriptEmbedding()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    Movie2ScriptEmbedding::~Movie2ScriptEmbedding()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool Movie2ScriptEmbedding::embedding( pybind::kernel_interface * _kernel )
    {
        pybind::interface_<Movie2, pybind::bases<Node>>( _kernel, "Movie2", false )
            .def( "setResourceMovie2", &Movie2::setResourceMovie2 )
            .def( "getResourceMovie2", &Movie2::getResourceMovie2 )
            .def( "setCompositionName", &Movie2::setCompositionName )
            .def( "getCompositionName", &Movie2::getCompositionName )
            .def( "setTextAliasEnvironment", &Movie2::setTextAliasEnvironment )
            .def( "getTextAliasEnvironment", &Movie2::getTextAliasEnvironment )
            .def( "setWorkAreaFromEvent", &Movie2::setWorkAreaFromEvent )
            .def( "removeWorkArea", &Movie2::removeWorkArea )
            .def( "hasSubComposition", &Movie2::hasSubComposition )
            .def( "getSubComposition", &Movie2::getSubComposition )
            .def( "hasCompositionBounds", &Movie2::hasCompositionBounds )
            .def( "getCompositionBounds", &Movie2::getCompositionBounds )
            .def_static( "getCompositionBoundsWM", &Detail::s_Movie2_getCompositionBoundsWM )
            .def_static_native_kernel( "setEventListener", &Detail::s_Movie2_setEventListener )
            .def_static_kernel( "getSockets", &Detail::s_Movie2_getSockets )
            .def_static_kernel( "getSlots", &Detail::s_Movie2_getSlots )
            .def( "findSprite", &Movie2::findSprite )
            .def( "hasSprite", &Movie2::hasSprite )
            .def( "findParticle", &Movie2::findParticle )
            .def( "hasParticle", &Movie2::hasParticle )
            .def( "findSlot", &Movie2::findSlot )
            .def( "hasSlot", &Movie2::hasSlot )
            .def( "findSocket", &Movie2::findSocket )
            .def( "hasSocket", &Movie2::hasSocket )
            .def( "findText", &Movie2::findText )
            .def( "hasText", &Movie2::hasText )
            .def( "hasMovieLayers", &Movie2::hasMovieLayers )
            .def( "setEnableMovieLayers", &Movie2::setEnableMovieLayers )
            ;

        pybind::interface_<Movie2Slot, pybind::bases<Node>>( _kernel, "Movie2Slot", false )
            ;

        pybind::interface_<Movie2SubComposition, pybind::bases<Eventable, Animatable, Scriptable, Identity>>( _kernel, "Movie2SubComposition", false )
            .def( "setEnable", &Movie2SubComposition::setSubCompositionEnable )
            .def( "getEnable", &Movie2SubComposition::getSubCompositionEnable )
            .def_static_native_kernel( "setEventListener", &Detail::s_Movie2SubComposition_setEventListener )
            ;

        pybind::interface_<ResourceMovie2, pybind::bases<Resource>>( _kernel, "ResourceMovie2", false )
            .def( "hasComposition", &ResourceMovie2::hasComposition )
            .def( "hasCompositionLayer", &ResourceMovie2::hasCompositionLayer )
            .def( "getCompositionDuration", &ResourceMovie2::getCompositionDuration )
            .def( "getCompositionFrameDuration", &ResourceMovie2::getCompositionFrameDuration )
            .def_static_kernel( "getCompositionResources", &Detail::s_ResourceMovie2_getCompositionResources )
            ;

        Helper::registerScriptWrapping<Movie2>( _kernel, STRINGIZE_STRING_LOCAL( "Movie2" ), MENGINE_DOCUMENT_FACTORABLE );
        Helper::registerScriptWrapping<Movie2Slot>( _kernel, STRINGIZE_STRING_LOCAL( "Movie2Slot" ), MENGINE_DOCUMENT_FACTORABLE );
        Helper::registerScriptWrapping<Movie2SubComposition>( _kernel, STRINGIZE_STRING_LOCAL( "Movie2SubComposition" ), MENGINE_DOCUMENT_FACTORABLE );
        Helper::registerScriptWrapping<ResourceMovie2>( _kernel, STRINGIZE_STRING_LOCAL( "ResourceMovie2" ), MENGINE_DOCUMENT_FACTORABLE );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void Movie2ScriptEmbedding::ejecting( pybind::kernel_interface * _kernel )
    {
        _kernel->remove_scope<Movie2>();
        _kernel->remove_scope<Movie2Slot>();
        _kernel->remove_scope<Movie2SubComposition>();
        _kernel->remove_scope<ResourceMovie2>();

        Helper::unregisterScriptWrapping( STRINGIZE_STRING_LOCAL( "Movie2" ) );
        Helper::unregisterScriptWrapping( STRINGIZE_STRING_LOCAL( "Movie2Slot" ) );
        Helper::unregisterScriptWrapping( STRINGIZE_STRING_LOCAL( "Movie2SubComposition" ) );
        Helper::unregisterScriptWrapping( STRINGIZE_STRING_LOCAL( "ResourceMovie2" ) );
    }
    //////////////////////////////////////////////////////////////////////////
}

