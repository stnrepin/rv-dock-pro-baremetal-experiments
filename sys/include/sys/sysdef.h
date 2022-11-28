#ifndef SYS_SYSDEF_H
#define SYS_SYSDEF_H

typedef unsigned long long vaddr_t;
typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

#define NULL ((void*)0)

static inline void write32(vaddr_t addr, uint32_t value) {
    *((volatile uint32_t *)(addr)) = value;
}

static inline uint32_t read32(vaddr_t addr) {
    return *((volatile uint32_t *)(addr));
}

static inline uint64_t sys_ticks(void) {
    uint64_t cnt;
    __asm__ volatile("csrr %0, time\n" : "=r"(cnt) :: "memory");
    return cnt;
}

static inline void sys_delay(uint64_t us) {
    uint64_t t1 = sys_ticks();
    uint64_t t2 = t1 + us * 24;
    do {
        t1 = sys_ticks();
    } while(t2 >= t1);
}

#endif // !SYS_SYSDEF_H
