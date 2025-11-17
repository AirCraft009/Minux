//
// Created by Mxsxll on 17.11.2025.
//

static inline unsigned char inb(int port){
    unsigned char ret;
    asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
    return ret;
}


static inline void outb(unsigned short port, unsigned char val) {
    __asm__ __volatile__("outb dx, al" : : "d"(port), "a"(val));
}


