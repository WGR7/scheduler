// main.c

#include <stdint.h>
#include <stdbool.h>

#include <lm3s811.h>
#include "task.h"

#define TICK_FREQ       100
#define TICK_PERIOD     (1/TICK_FREQ)

#define __DEBUG__

#define TASK_A_SP       (uint32_t*)(0x20000200)
#define TASK_B_SP       (uint32_t*)(0x20000100)

void hw_init();
void sched_start(void);
static inline void sched_int_enable(void);

/* Tasks */
void idler(void);
void LED_On(void);
void LED_Off(void);

void SysTick_Handler(void) __attribute__((naked));

int main(void)
{
    hw_init();
    init_queue();
    create_task(&LED_On, TASK_A_SP);
    create_task(&LED_Off, TASK_B_SP);
    sched_int_enable();
    while(1) {
        idler();
    }
}

static inline void sched_int_enable(void)
{
    // Enable all interrupt sources
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;
}

void hw_init(void)
{
    // GPIO C has to be clocked before using
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;

    // Setup SysTick
    // NVIC_ST_CTRL_CLK_SRC = Main clock
    // NVIC_ST_CTRL_INTEN = Interrupt enable
    // NVIC_ST_CTRL_ENABLE = Enable SysTick
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_ENABLE;
    NVIC_ST_RELOAD_R = 0xFFFFFFFF;

    // Set PC5 to be output
    GPIO_PORTC_DIR_R |= (1U << 5);
}

void SysTick_Handler(void)
{
    volatile uint32_t* tmp;

    if (current_task <= last_task) {
        ++current_task;
    } else {
        current_task = &ready_queue[0];
    }

    tmp = current_task->sp;

    // Store the registers not already stored by NVIC
    __asm__ volatile (
            "push {r4-r11} \n"
            "mov sp, %[stk_ptr] \n"
            "pop {r4-r11} \n"
            "bx lr \n" :: [stk_ptr] "r" (tmp));
}

void idler(void)
{
    while(1) {
        __asm__ volatile ("nop");
    }
}

void LED_On(void)
{
    while(1) {
        GPIO_PORTC_DATA_R |= (1U << 5);
    }
}

void LED_Off(void)
{
    while(1) {
        GPIO_PORTC_DATA_R &= ~(1U << 5);
    }
}
