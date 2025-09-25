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
    xSemaphoreTake(semaphore, 100);
    TEST_ASSERT_TRUE_MESSAGE(update_main(&counter, semaphore, 100) == 0, "Semaphore did not time out");
}

void testSide() {
    TEST_ASSERT_TRUE_MESSAGE(update_side(&counter, semaphore, portMAX_DELAY) == 1, "Semaphore did not take");
    xSemaphoreTake(semaphore, 100);
    TEST_ASSERT_TRUE_MESSAGE(update_side(&counter, semaphore, 100) == 0, "Semaphore did not time out");
}

void testDeadlock() {
    // construct the arguments
    int state1, state2;
    SemaphoreHandle_t sem1 = xSemaphoreCreateCounting(1, 1);
    SemaphoreHandle_t sem2 = xSemaphoreCreateCounting(1, 1);
    deadlockArgs_t args1 = (deadlockArgs_t){&state1, sem1, sem2};
    deadlockArgs_t args2 = (deadlockArgs_t){&state2, sem2, sem1};

    // start the threads and wait for deadlock
    TaskHandle_t task1, task2;
    xTaskCreate(deadLock, "deadlock1", configMINIMAL_STACK_SIZE, &args1, tskIDLE_PRIORITY + 1UL, &task1);
    xTaskCreate(deadLock, "deadlock2", configMINIMAL_STACK_SIZE, &args2,  tskIDLE_PRIORITY + 1UL, &task2);
    vTaskDelay(500); // should be enough time to complete, should deadlock not occur

    // suspend the tasks to check the state
    vTaskSuspend(task1);
    vTaskSuspend(task2);

    // make sure that there is a deadlock
    TEST_ASSERT_TRUE_MESSAGE(state1 == 1, "Task 1 did not deadlock");
    TEST_ASSERT_TRUE_MESSAGE(state2 == 1, "Task 2 did not deadlock");

    // gotta get that memory back
    vTaskDelete(task1);
    vTaskDelete(task2);
    vSemaphoreDelete(sem1);
    vSemaphoreDelete(sem2);
}

void runTests (__unused void* args)
{
    semaphore = xSemaphoreCreateCounting(1, 1);
    sleep_ms(5000); // Give time for TTY to attach.
    while (1) {
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(testMain);
        RUN_TEST(testSide);
        RUN_TEST(testDeadlock);
        sleep_ms(5000);
        UNITY_END();
    }
}

int main () {
    // startup "borrowed" from threads.c
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    TaskHandle_t tester;
    xTaskCreate(runTests, "tester", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1UL, &tester);
    vTaskStartScheduler();
    return 0;
}