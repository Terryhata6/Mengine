#include "TexturepackerScriptEmbedding.h"

#include "Interface/VocabularyServiceInterface.h"
#include "Interface/StringizeServiceInterface.h"
#include "Interface/PrototypeServiceInterface.h"

#include "Environment/Python/PythonScriptWrapper.h"

#include "Frameworks/PythonFramework/DocumentTraceback.h"

#include "ResourceTexturepacker.h"

#include "Kernel/Logger.h"
#include "Kernel/ScriptWrapperInterface.h"
#include "Kernel/AssertionMemoryPanic.h"

#include "pybind/pybind.hpp"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    TexturepackerScriptEmbedding::TexturepackerScriptEmbedding()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    TexturepackerScriptEmbedding::~TexturepackerScriptEmbedding()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    bool TexturepackerScriptEmbedding::embedding( pybind::kernel_interface * _kernel )
    {
        pybind::interface_<ResourceTexturepacker, pybind::bases<Resource, Content> >( _kernel, "ResourceImageTexturepacker", false )
            .def( "getFrame", &ResourceTexturepacker::getFrame )
            ;

        VOCABULARY_SET( ScriptWrapperInterface, STRINGIZE_STRING_LOCAL( "ClassWrapping" ), STRINGIZE_STRING_LOCAL( "ResourceImageTexturepacker" ), Helper::makeFactorableUnique<PythonScriptWrapper<ResourceTexturepacker> >( _kernel ) );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void TexturepackerScriptEmbedding::ejecting( pybind::kernel_interface * _kernel )
    {
        _kernel->remove_scope<ResourceTexturepacker>();

        VOCABULARY_REMOVE( STRINGIZE_STRING_LOCAL( "ClassWrapping" ), STRINGIZE_STRING_LOCAL( "ResourceImageTexturepacker" ) );
    }
}
