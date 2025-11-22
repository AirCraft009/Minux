//
// Created by cocon on 17.11.2025.
//

#include <stdint.h>
#include <stddef.h>
int memcpy(void *dest, const void *src, size_t n) {
    // at first we'll just use copy one byte at a time
    // later we could copy multiple bytes at a time
    if (n % 2 == 0) {
        for (size_t i = 0; i < n; i++) {
            ((uint16_t *)dest)[i] = ((uint16_t *)src)[i];
        }
        return 0;
    }
    for (size_t i = 0; i < n; i++) {
        ((char *)dest)[i] = ((char *)src)[i];
    }
    return 0;
}

void memset(void *dest, int ch, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((char *)dest)[i] = ch;
    }
}
