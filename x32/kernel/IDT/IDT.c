//
// Created by cocon on 20.11.2025.
//

#include <stdint.h>


__attribute__((noreturn))
void exceptionHandler(void);

struct InterruptDescriptor32 {
    uint16_t offset_1;        // offset bits 0..15
    uint16_t selector;        // a code segment selector in GDT or LDT
    uint8_t  zero;            // unused, set to 0
    uint8_t  type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;        // offset bits 16..31
}__attribute__((packed)) IDTEntry;

 void exceptionHandler (void) {
     __asm__ __volatile__("cli; halt");
 }