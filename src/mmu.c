#include <stdint.h>

#include "mmu.h"
#include "memory.h"


page_table_t create_page_table()
{
    // XXX Do 2nd level page tables need to be 16KB aligned too?
    // TODO: Zero out the memory.
    return (page_table_t)kmalloc_aligned(0x4000); // 16KB
}


page_table_t create_identity_page_table()
{
    page_table_t _table = create_page_table();
    page_table_entry_l1_t* page_table = (page_table_entry_l1_t*)_table;
    for (uint32_t i = 0; i < 4096; i++) {
        page_table[i] = PTE_L1_SECTION_BASE_ADDR(i) |
                        PTE_L1_SECTION_AP(3) |
                        PTE_L1_TYPE_SECTION;
    }
    return _table;
}


// TODO: Allow setting of TTBR1
// TODO: Invalidate TLB/Caches
inline void set_level1_page_table(page_table_t page_table)
{
    asm volatile (
        "mcr p15, 0, %0, c2, c0, 0"
        : : "r" ((uint32_t)page_table) :
    );
}


inline void set_mmu_state(uint8_t enabled)
{
    uint32_t mmu_control = 0;
    asm volatile (
        "mrc p15, 0, %0, c1, c0, 0"
        : "=r" (mmu_control) : :
    );
    mmu_control = (mmu_control & 0xFFFE) | (enabled & 0x1);
    asm volatile (
        "mcr p15, 0, %0, c1, c0, 0"
        : : "r" (mmu_control) : "memory"
    );
}


page_table_t init_mmu()
{
    // Switch to using a single L1 PT with identity mapping.
    page_table_t page_table = create_identity_page_table();
    set_level1_page_table(page_table);

    // Set Domain 0 to "Manager"
    asm volatile (
        "mcr p15, 0, %0, c3, c0, 0"
        : : "r" (0x3) :
    );

    // Enable the MMU
    set_mmu_state(1);

    // Return out L1 PT, as we can modify it later.
    return page_table;
}

