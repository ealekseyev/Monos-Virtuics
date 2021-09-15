#ifndef __TSS_C_
#define __TSS_C_
typedef struct {
    int x;
} tss_t;

extern void load_tss(tss_t*);
#endif