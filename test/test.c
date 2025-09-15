#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include <string.h>
#include "unity_config.h"

// reset the test cases
void setUp(void) {}

// nothing to do here
void tearDown(void) {}

int main (void)
{
        stdio_init_all();
        sleep_ms(5000); // Give time for TTY to attach.
        while (1) {
            printf("Start tests\n");
            UNITY_BEGIN();
            sleep_ms(5000);
            UNITY_END();
        }
}
