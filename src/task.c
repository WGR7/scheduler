// task.c:

#include "task.h"

//
// The `ready_queue` will consist of tasks ready to run right now.
// In the initial implementation, all tasks will be ready to run since task
// states aren't implemented right now. Each slot on the ready queue will have
// an associated priority. The scheduler will then simply execute the task
// which has the highest priority followed by the lower ones until it reaches
// the end of the ready queue.
//
// If no task is added to the `ready_queue`,

const struct tcb_t tcb_initializer = {
    .priority = 0;
    .fn = &idler;
    .sp = 0x00;
    .sp_base = 0x00;
};

void init_queue(struct schedq_t* run_queue)
{
}


/*
 * @brief
 * Add a "ready" task to the run queue
 *
 * Checks and adds a task declaring itself as READY to the run queue
 * if there is space available on it. Otherwise returns an error.
 *
 * FIXME: Currently adds any task if there is space in the queue.
 *
 */
uint8_t add_to_queue(struct schedq_t* run_queue, struct tcb_t* task_block)
{
/*    static uint8_t id;
    int8_t err;
    if (id < MAX_TASKS) {
        ready_queue[id] = *task_block;
        last_task = &ready_queue[id];
        id++;
        err = 0;
    } else {
        err = -1;
    }
    return err;*/
    uint8_t errno;
    uint8_t queue_pos = task_block->priority;

    // If space left on queue
    if (run_queue->task_count < MAX_TASKS) {
        run_queue->list[queue_pos] = task_block;
        run_queue->task_count++;
        errno = OP_SUCCESS;
    } else {
        errno = OP_FAIL;
    }

    return errno;
}

/*
 * @brief
 * Register a task with the scheduler
 *
 * Creates the TCB for and registers the task with the scheduler with
 * a fixed priority. The `stack_base` would be dynamically computed
 * at runtime.
 *
 */
void create_task(task_fn fn, uint32_t* stack_base, uint8_t priority)
{
    struct tcb_t task_block = {
        .priority = (priority < MAX_TASKS) ? priority : -1 ;
        .fn = fn,
        .sp_base = stack_base,
        .sp = stack_init(uint32_t* stack_base)
    };

    uint8_t err_code = add_to_queue(&task_block);

    if (err_code) {
        // Handle error here
    }
}

/*
 * @brief
 * Initialize the stack for a new task
 *
 * Creates a stack representation for a new stack similar to that of an
 * interrupted task. The `stack_base` pointer is modified to point to the
 * point inside the stack where it looks like a task was interrupted and
 * is just ready to run.
 *
 */
uint32_t* stack_init(uint32_t* sp_base)
{
    // Start from the base stack pointer
    uint32_t* sp = sp_base;

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

    return sp;
}
