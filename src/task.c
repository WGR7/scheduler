// task.c

#include "task.h"

void create_task(task_fn fn, uint32_t* sp)
{
    struct tcb_t task_block = {
        .fn = fn,
        .sp = sp
    };
    stack_init(&task_block);
}

void stack_init(struct tcb_t* task_block)
{
    uint32_t* sp = task_block->sp;
    // Full descending stack
    sp--;
    *sp = 0x61000000;
    sp--;
    *sp = (uint32_t) task_block->fn;
    sp--;
    *sp = (uint32_t) task_block->fn;
    sp--;
    *sp = 0x00;             // r12
    sp--;
    *sp = 0x00;             // r3
    sp--;
    *sp = 0x00;             // r2
    sp--;
    *sp = 0x00;             // r1
    sp--;
    *sp = 0x00;             // r0
    sp -= 8;                // r4-r11
}
