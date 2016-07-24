#include <mpe_pe.h>

#include <Windows.h>
#include <stdint.h>

// Private Functions

bool
copy_sections
(
    mpe_module_t* module,
    uint8_t* buffer,
    uintptr_t buffer_size
)
{
    bool ret = false;
    PIMAGE_NT_HEADERS nt_headers = NULL;
    PIMAGE_SECTION_HEADER section = NULL;
    uint32_t i = 0;

    if (NULL == module ||
        NULL == buffer ||
        0 == buffer_size)
    {
        return false;
    }

    nt_headers = MPEImageNtHeader((mpe_module_t*)&buffer);
    RtlCopyMemory(module->image_base, buffer, nt_headers->OptionalHeader.SizeOfHeaders);

    section = IMAGE_FIRST_SECTION(nt_headers);
    for (i = 0; i < nt_headers->FileHeader.NumberOfSections; ++i)
    {
        RtlCopyMemory((module->image_base + section->VirtualAddress), (buffer + section->PointerToRawData), section->SizeOfRawData);
    }

    return true;
}

// Public API Functions

PIMAGE_NT_HEADERS
MPEImageNtHeader
(
    mpe_handle_t handle
)
{
    PIMAGE_NT_HEADERS result = NULL;
    mpe_module_t* module = (mpe_module_t*)handle;
    uint32_t e_lfanew = 0;
    uint8_t* image_base;
    
    if (NULL != module)
    {
        image_base = module->image_base;
        if (NULL != image_base)
        {
            if (INVALID_HANDLE_VALUE != image_base && IMAGE_DOS_SIGNATURE == *(uint16_t*)image_base)
            {
                e_lfanew = ((PIMAGE_DOS_HEADER)image_base)->e_lfanew;
                if (e_lfanew < 0x10000000)
                {
                    result = (PIMAGE_NT_HEADERS)((uintptr_t)image_base + e_lfanew);
                    if (IMAGE_NT_SIGNATURE != result->Signature)
                    {
                        result = NULL;
                    }
                }
            }
        }
    }

    return result;
}

bool 
MPELoadMemoryModule
(
    uint8_t* buffer,
    uintptr_t buffer_size,
    mpe_handle_t * handle
)
{
    bool ret = false;
    mpe_module_t* module = NULL;
    PIMAGE_NT_HEADERS nt_headers = NULL;
    
    if (NULL == handle)
    {
        return false;
    }

    if (NULL != (nt_headers = MPEImageNtHeader((mpe_module_t*)&buffer))) // This is a little bit of hackery magic going on
    {
        if (!(nt_headers->FileHeader.Characteristics & IMAGE_FILE_DLL))
        {
            return false; // We can only load DLL's
        }

        if (NULL != (module = (mpe_module_t*)calloc(1, sizeof(*module))))
        {
            ZeroMemory(module, sizeof(*module));
            if (NULL != (module->image_base = (uint8_t*)VirtualAlloc(NULL, nt_headers->OptionalHeader.SizeOfImage, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)))
            {
                ZeroMemory(module->image_base, nt_headers->OptionalHeader.SizeOfImage);
                ret = copy_sections(module, buffer, buffer_size);
            }
        }
    }

    if (ret &&
        (ret = MPEPerformRelocations((mpe_handle_t)module)))
    {

        *handle = (mpe_handle_t*)module;
    }
    else
    {
        VirtualFree(module->image_base, 0, MEM_RELEASE);
        free(module);
    }

    return ret;
}

bool 
MPEUnloadMemoryModule
(
    mpe_handle_t handle
)
{
    bool ret = false;

    return ret;
}

uint8_t*
MPEGetEntryPoint
(
    mpe_handle_t handle
)
{
    void* ret = NULL;
    mpe_module_t* module = (mpe_module_t*)handle;
    PIMAGE_NT_HEADERS nt_headers = NULL;
    uintptr_t entry_point = 0;

    if (NULL != module)
    {
        if (NULL != (nt_headers = MPEImageNtHeader(module)))
        {
            if (0 != (entry_point = nt_headers->OptionalHeader.AddressOfEntryPoint))
            {
                ret = (void*)(module->image_base + entry_point);
            }
        }
    }

    return ret;
}

bool
MPEPerformRelocations
(
    mpe_handle_t handle
)
{
    bool ret = false;
    mpe_module_t* module = (mpe_module_t*)handle;
    PIMAGE_NT_HEADERS nt_headers = NULL;
    PIMAGE_DATA_DIRECTORY relocation_ddir = NULL;
    intptr_t delta = 0;

    if (NULL == module)
    {
        return false;
    }

    if (NULL == (nt_headers = MPEImageNtHeader(handle)))
    {
        return false;
    }

    if (nt_headers->FileHeader.Characteristics & IMAGE_FILE_RELOCS_STRIPPED)
    {
        //Unable to relocate
        return false;
    }

    if (module->image_base == nt_headers->OptionalHeader.ImageBase)
    {
        //Nothing to relocate
        return true;
    }

    relocation_ddir = &nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
    if (0 == relocation_ddir->VirtualAddress || 0 == relocation_ddir->Size)
    {
        //Nothing to relocate
        return true;
    }

    delta = (uintptr_t)module->image_base


    return ret;
}