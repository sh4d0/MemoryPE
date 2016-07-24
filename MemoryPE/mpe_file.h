#pragma once
#ifndef MPE_FILE_H
#define MPE_FILE_H

#include <stdint.h>
#include <stdbool.h>

bool
MPEReadFile
(
    char* filepath,
    uint8_t** buffer,
    uint32_t* buffer_size
);

#endif