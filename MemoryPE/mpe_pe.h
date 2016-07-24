#pragma once
#ifndef MPE_PE_H
#define MPE_PE_H

#include <Windows.h>
#include <stdint.h>
#include <stdbool.h>

typedef void* mpe_handle_t;

typedef struct
{
    uint8_t *image_base;
    LIST_ENTRY module_list;
} mpe_module_t;

PIMAGE_NT_HEADERS
MPEImageNtHeader
(
    mpe_handle_t handle
);

bool
MPELoadMemoryModule
(
    uint8_t* buffer,
    uintptr_t buffer_size,
    mpe_handle_t* handle
);

bool
MPEUnloadMemoryModule
(
    mpe_handle_t handle
);

uint8_t*
MPEGetEntryPoint
(
    mpe_handle_t handle
);

bool
MPEPerformRelocations
(
    mpe_handle_t handle
);
#endif