// main.c

#include <stdint.h>
#include <stdbool.h>

#include <lm3s811.h>
// #include "task.h"

#define TICK_PERIOD
#define TICK_FREQ

#define __DEBUG__   (1)

void hw_init();

void LED_On(void);
void LED_Off(void);

volatile uint8_t flag;

void SysTick_Handler(void) __attribute__((interrupt));

int main(void)
{
    hw_init();
    // sched_init();
    //    create_task(&LED_On, 0);
     //   create_task(&LED_Off, 1);
    // sched_start();
    while(1){
        LED_On();
        LED_Off();
    }
}

// Insert small delays
void delay(uint8_t dly_tm)
{
    while(dly_tm > 0) {
        __asm__ volatile ("nop");
        dly_tm--;
    }
}

void hw_init(void)
{
    // GPIO C has to be clocked before using
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;

    // Setup SysTick
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN | NVIC_ST_CTRL_ENABLE;
    NVIC_ST_RELOAD_R = 0xEA5F;  // 5999 = 6000-1 = 100 Hz

    // Check to see if SysTick is actually active

    // Set PC5 to be output
    GPIO_PORTC_DIR_R |= (1U << 5);
}

void SysTick_Handler(void)
{
    flag ^= 1;
}

void LED_On(void)
{
    if (flag) {
        GPIO_PORTC_DATA_R |= (1U << 5);
    }
}

void LED_Off(void)
{
    if (!flag){
       GPIO_PORTC_DATA_R &= ~(1U << 5);
    }
}
