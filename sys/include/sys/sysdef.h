#ifndef SYS_SYSDEF_H
#define SYS_SYSDEF_H

typedef unsigned long long virtual_addr_t;
typedef unsigned long long u64_t;
typedef unsigned int u32_t;
typedef unsigned char u8_t;

static inline void write32(virtual_addr_t addr, u32_t value) {
    *((volatile u32_t *)(addr)) = value;
}

static inline u32_t read32(virtual_addr_t addr) {
    return *((volatile u32_t *)(addr));
}

static inline u64_t sys_ticks(void) {
    u64_t cnt;
    __asm__ __volatile__("csrr %0, time\n" : "=r"(cnt) :: "memory");
    return cnt;
}

static inline void sys_delay(u64_t us) {
    u64_t t1 = sys_ticks();
    u64_t t2 = t1 + us * 24;
    do {
        t1 = sys_ticks();
    } while(t2 >= t1);
}

#endif // !SYS_SYSDEF_H
