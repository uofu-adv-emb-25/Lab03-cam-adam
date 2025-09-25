#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>
#include "functions.h"


int update_side(int* counter, SemaphoreHandle_t semaphore, TickType_t ticks) {
    int out = xSemaphoreTake(semaphore, ticks);
    {
        printf("hello world from %s! Count %d\n", "thread", (*counter)++);
    }
    xSemaphoreGive(semaphore);
    return out;
}

int update_main(int* counter, SemaphoreHandle_t semaphore, TickType_t ticks) {
    int out = xSemaphoreTake(semaphore, ticks);
    {
		printf("hello world from %s! Count %d\n", "main", (*counter)++);
    }
    xSemaphoreGive(semaphore);
    return out;
}

void deadLock(void* argsP) {
    deadlockArgs_t args = *((deadlockArgs_t*)argsP);
    *(args.state) = 0;
    xSemaphoreTake(args.sem1, portMAX_DELAY);
    {
        *(args.state) = 1;
        vTaskDelay(100);
        xSemaphoreTake(args.sem2, portMAX_DELAY);
        {
            *(args.state) = 2;
            vTaskDelay(100);
        }
        xSemaphoreGive(args.sem2);
    }
    xSemaphoreGive(args.sem1);
    *(args.state) = 3;
    while (1);
}