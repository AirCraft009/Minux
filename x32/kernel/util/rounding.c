//
// Created by cocon on 16.11.2025.
//

#include <stdint.h>


/**
 * returns the ammount of digits in a number
 * @param x input number
 * @return number of digits in x
 *
 */
int digits32(uint32_t x) {
    /*
    * naive ladder implementation is probably fastest because number are more likely to be low vs high
    * second var binary search for randomly distributed numbers(but higher chance of branch misprediction
    */
    if (x < 10) return 1;
    if (x < 100) return 2;
    if (x < 1000) return 3;
    if (x < 10000) return 4;
    if (x < 100000) return 5;
    if (x < 1000000) return 6;
    if (x < 10000000) return 7;
    if (x < 100000000) return 8;
    if (x < 1000000000) return 9;
    return 10;
}
