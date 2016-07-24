#include <mpe_file.h>

#include <Windows.h>

bool
MPEReadFile
(
    char* filepath,
    uint8_t** buffer,
    uintptr_t* buffer_size
)
{
    bool ret = false;
    HANDLE file_handle = INVALID_HANDLE_VALUE;
    LARGE_INTEGER file_size = { 0 };
    uintptr_t bytes_read = 0;

    if (NULL == buffer ||
        NULL == buffer_size)
    {
        return false;
    }

    if (INVALID_HANDLE_VALUE != (file_handle = CreateFileA(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
    {
        if (GetFileSizeEx(file_handle, &file_size))
        {
            *buffer_size = (uintptr_t)file_size.QuadPart;
            *buffer = (uint8_t*)calloc(*buffer_size, 1);
            if (NULL != *buffer)
            {
                if (ReadFile(file_handle, *buffer, *buffer_size, &bytes_read, NULL))
                {
                    ret = true;
                }
                else
                {
                    free(*buffer);
                    *buffer = NULL;
                    *buffer_size = 0;
                }
                
            }
            else
            {
                *buffer_size = 0;
            }
        }
    }

    return ret;
}