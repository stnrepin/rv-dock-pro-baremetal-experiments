#ifndef SYSDEF_H
#define SYSDEF_H

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

#endif // !SYSDEF_H
