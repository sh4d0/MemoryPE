#include <Windows.h>
#include <stdio.h>
#include <inttypes.h>

#include <mpelib.h>

typedef int(*SumFn)(int value1, int value2);

int main()
{
    HMODULE testdllmod = NULL;
    SumFn sum = NULL;
    mpe_handle_t handle = { 0 };
    uint8_t* buffer = NULL;
    uint32_t buffer_size = 0;
    PIMAGE_NT_HEADERS nt_header = NULL;

    printf("Test MPELib\n");
    //if (NULL != (testdllmod = LoadLibraryA("testdll.dll")))
    //{
    //    sum = (SumFn)GetProcAddress(testdllmod, "sum");
    //}

    //if (NULL != sum)
    //{
    //    printf("Result: %d\n", sum(5, 6));

    //}

    if (MPEReadFile("testdll.dll", &buffer, &buffer_size))
    {
        printf("File Size: %"PRIdPTR"\n", buffer_size);
        if (MPELoadMemoryModule(buffer, buffer_size, &handle))
        {
            printf("Yay go to here\n");
        }
        //nt_header = MPEImageNtHeader(buffer);
        //if (NULL != nt_header)
        //{
        //    printf("Image Size: %"PRIdPTR"\n", nt_header->OptionalHeader.SizeOfImage);
        //}
    }

    return EXIT_SUCCESS;
}

