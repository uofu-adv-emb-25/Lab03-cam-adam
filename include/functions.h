#ifndef functions_h
#define functions_h

#include <semphr.h>
#include <semphr.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>

typedef struct {
    int* state;
    SemaphoreHandle_t sem1;
    SemaphoreHandle_t sem2;
} deadlockArgs_t;

int update_side(int*, SemaphoreHandle_t, TickType_t);
int update_main(int*, SemaphoreHandle_t, TickType_t);

void deadLock(void*);


#endif