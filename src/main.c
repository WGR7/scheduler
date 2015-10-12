// main.c

#include <stdint.h>
#include <stdbool.h>

#include <lm3s811.h>
// #include "task.h"

#define TICK_FREQ       100
#define TICK_PERIOD     (1/TICK_FREQ)

#define __DEBUG__

void hw_init();

void LED_On(void);
void LED_Off(void);

volatile uint8_t flag;

void (*volatile LED_fn)(void);

void SysTick_Handler(void) __attribute__((naked));


int main(void)
{
    hw_init();
#ifndef __DEBUG__
    sched_init();
    create_task(&LED_On, 0);
    create_task(&LED_Off, 1);
    sched_start();
#endif
    flag = 0;
    LED_fn = &LED_On;
    while(1) {
        LED_fn();
    }
}

void hw_init(void)
{
    // GPIO C has to be clocked before using
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;

    // Setup SysTick
    // NVIC_ST_CTRL_CLK_SRC = Main clock
    // NVIC_ST_CTRL_INTEN = Interrupt enable
    // NVIC_ST_CTRL_ENABLE = Enable SysTick
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN | NVIC_ST_CTRL_ENABLE;
    NVIC_ST_RELOAD_R = 0x000092BF;

    // Set PC5 to be output
    GPIO_PORTC_DIR_R |= (1U << 5);
}

void SysTick_Handler(void)
{
    if (flag) {
        // When we do enter here, we'll have a functioning stack
        // So just get the stack pointer
        __asm__ volatile("push {r4-r7}\r\n");
    }

    if (!flag) {
        flag ^= 0x01;
        LED_fn = &LED_Off;
    }
    // Store the registers not already stored by NVIC
    // r4-r7
    // Since Thumb code can access only the lower registers
    // Decrement `sp` before storing the values
    // Implement a simpler version where we begin with already
    // executing tasks - so the stacks are already setup
}

void LED_On(void)
{
    GPIO_PORTC_DATA_R |= (1U << 5);
}

void LED_Off(void)
{
   GPIO_PORTC_DATA_R &= ~(1U << 5);
}
