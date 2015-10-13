// task.h

#ifndef __TASK_H__
#define __TASK_H__

#include <stdint.h>

// Task function to be executed
typedef void (*task_fn)(void);

struct tcb_t {
    task_fn fn;
    uint32_t* sp;
} tcb_t;

void create_task(task_fn fn, uint32_t* sp);
void stack_init(struct tcb_t* task_block);

#endif
