//ADD THIS TO /linux-4.13.1/kernel/sys.c

SYSCALL_DEFINE0(phil_call){    
    //The code below to turn the current process's
    //state number to a string was mainly taken from the
    //get_task_state() method in fs/proc/array.c
    unsigned int state_num = (current->state | current->exit_state) & TASK_REPORT;
    
    if(current->state == TASK_PARKED){
        state_num = TASK_INTERRUPTIBLE;
    }
    char *state;
    static const char * const task_state_array[] = {                                                  
        "R (running)",      /*   0 */
        "S (sleeping)",     /*   1 */
        "D (disk sleep)",   /*   2 */
        "T (stopped)",      /*   4 */
        "t (tracing stop)", /*   8 */
        "X (dead)",     /*  16 */
        "Z (zombie)",       /*  32 */
    };
    state = task_state_array[fls(state_num)];

    int prio = current->prio;
    int static_prio = current->static_prio;
    int currCPU = current->cpu;

    printk(KERN_INFO "STATE: %s\n", state);
    printk(KERN_INFO "PRIORITY: %d\n", prio);
    printk(KERN_INFO "STATIC_PRIO: %d\n", static_prio);
    printk(KERN_INFO "CURR_CPU: %d\n", currCPU);
}
