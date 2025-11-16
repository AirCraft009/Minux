//
// Created by cocon on 16.11.2025.
//

#include <stddef.h>

size_t strlen(const char *str) {
    size_t len = 0;
    while (*str) {
        len++;
    }
    return len;
}


