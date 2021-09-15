#ifndef _PIT_TIMER_C_
#define _PIT_TIMER_C_
#include <io/ports.c>
#include <intel/idt.c>

/*
    Timed processes subsystem
*/

typedef struct {
    void* handler;
    uint32_t freq;   // execute every <freq> ticks
    uint32_t _start; // internal
} timer_proc_t;

static timer_proc_t timer_processes[255];
static size_t tp_len;

// prototypes
uint32_t timer_get_ticks(void);

void register_timed_process(timer_proc_t* proc) {
    timer_processes[tp_len].handler = proc->handler;
    timer_processes[tp_len].freq = proc->freq;
    timer_processes[tp_len]._start = timer_get_ticks(); // time of launch
    // initiate first launch
    void (*proc_launch)();
    proc_launch = timer_processes[tp_len].handler;
    proc_launch();
    tp_len++;
}

// this is the handler that runs every time the timer fires.
// 1000 ticks in a second
// prolly dont need volatile (no multicore processing in kernel yet)
// TODO: all these operations seriously slow down the timer.
static volatile uint32_t ticks; // millis since timer init
void _timer_handler(irs_t* registers) {
    ticks++;
    master_pic_irq_done();
    /*for(int i = 0; i < tp_len; i++) {
        if((ticks - timer_processes[i]._start) % timer_processes[i].freq == 0) {
            void (*proc_launch)();
            proc_launch = timer_processes[i].handler;
            proc_launch(); 
        }
    }*/
}

static void set_timer_freq(int hz) {
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    port_writeb(0x43, 0x34);             /* Set our command byte 0x36 note: did not work with 0x36 */
    port_writeb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    port_writeb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

#define kern_millis timer_get_ticks
uint32_t timer_get_ticks() {
    return ticks;
}

// sleeps for <time> milliseconds
void kern_sleep(uint32_t time) {
    uint32_t start = timer_get_ticks();
    while(timer_get_ticks() - start < time);
}

void wait_for_timer_init() {
    while(ticks == 0);
}

void init_pit() {
    set_timer_freq(1000); // 1000 hz
    isr_register_handler(32, _timer_handler);
    irq_enable(0);
}

void disable_timer() {
    irq_disable(0);
}

#endif