#pragma once

#include "Interface/FileMappedInterface.h"

#include "Kernel/Factorable.h"

#include "Environment/Windows/WindowsIncluder.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    class Win32FileMapped
        : public FileMappedInterface
        , public Factorable
    {
    public:
        Win32FileMapped();
        ~Win32FileMapped() override;

    public:
        bool open( const FilePath & _relationPath, const FilePath & _folderPath, const FilePath & _filePath, bool _share ) override;
        bool close() override;

    public:
        InputStreamInterfacePtr createInputStream( const DocumentPtr & _doc ) override;
        bool openInputStream( const InputStreamInterfacePtr & _stream, size_t _offset, size_t _size, void ** const _memory ) override;

    private:
        HANDLE m_hFile;
        HANDLE m_hMapping;
        LPVOID m_memory;

        LARGE_INTEGER m_liSize;
    };
    //////////////////////////////////////////////////////////////////////////
    typedef IntrusivePtr<Win32FileMapped> Win32FileMappedPtr;
    //////////////////////////////////////////////////////////////////////////
}
