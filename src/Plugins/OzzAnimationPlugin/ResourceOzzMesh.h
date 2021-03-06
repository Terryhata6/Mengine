#pragma once

#include "OzzAnimationInterface.h"

#include "OzzDetail.h"

#include "Kernel/Resource.h"
#include "Kernel/BaseContent.h"

#include "Kernel/Vector.h"

#include "ozz/animation/runtime/skeleton.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    class ResourceOzzMesh
        : public Resource
        , public UnknownResourceOzzMeshInterface
        , protected BaseContent
        
    {
        DECLARE_VISITABLE( Resource );
        DECLARE_UNKNOWABLE();
        DECLARE_CONTENTABLE();

    public:
        ResourceOzzMesh();
        ~ResourceOzzMesh() override;

    public:
        ozz::span<const Detail::Mesh> getMeshes() const;

    protected:
        bool _compile() override;
        void _release() override;

    protected:
        ozz::vector<Detail::Mesh> m_meshes;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusiveResourcePtr<ResourceOzzMesh> ResourceOzzMeshPtr;
    //////////////////////////////////////////////////////////////////////////
}