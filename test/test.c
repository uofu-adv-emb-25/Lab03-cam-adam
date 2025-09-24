#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include <string.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>
#include "unity_config.h"
#include "functions.h"

SemaphoreHandle_t semaphore;

int counter;
int on;

// reset the test cases
void setUp(void) {counter = 0; on = 0;}

// nothing to do here
void tearDown(void) {}

void testMain() {
    TEST_ASSERT_TRUE_MESSAGE(update_main(&counter, semaphore, portMAX_DELAY) == 1, "Semaphore did not take");
    xSemaphoreTake(semaphore, portMAX_DELAY);
    TEST_ASSERT_TRUE_MESSAGE(update_main(&counter, semaphore, 100) == 0, "Semaphore did not time out");
}

void testSide() {
    TEST_ASSERT_TRUE_MESSAGE(update_side(&counter, semaphore, portMAX_DELAY) == 1, "Semaphore did not take");
    xSemaphoreTake(semaphore, portMAX_DELAY);
    TEST_ASSERT_TRUE_MESSAGE(update_side(&counter, semaphore, 100) == 0, "Semaphore did not time out");
}

void testDeadlock() {
    
}

int main (void)
{
    semaphore = xSemaphoreCreateCounting(1, 1);
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    while (1) {
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(testMain);
        RUN_TEST(testSide);
        sleep_ms(5000);
        UNITY_END();
    }
}
