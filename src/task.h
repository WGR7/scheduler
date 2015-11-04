// task.h

#ifndef __TASK_H__
#define __TASK_H__

#include <stdint.h>
#include <stdbool.h>

#define MAX_TASKS       (8U)

// Error codes
#define OP_FAIL         (0xFF)
#define OP_SUCCESS      (0x02)


// Task function to be executed
typedef void (*task_fn)(void);

struct tcb_t {
    uint8_t priority;
    task_fn fn;
    uint32_t* sp;
    uint32_t* sp_base;
};

struct schedq_t {
    struct tcb_t* list[MAX_TASKS];
    uint8_t task_count;
};

void create_task(task_fn fn, uint32_t* stack_base);
uint32_t* stack_init(struct tcb_t* task_block);
void init_queue(struct schedq_t* run_queue);
int8_t add_to_queue(struct schedq_t* run_queue, struct tcb_t* task_block);

void idler(void);
volatile struct schedq_t run_queue;
volatile struct tcb_t* current_task;
volatile struct tcb_t* last_task;

#endif
