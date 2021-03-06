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


void map_section(page_table_t page_table, uint32_t section_addr, uint32_t physical_addr)
{
    uint32_t table_index = section_addr >> 20;
    page_table_entry_l1_t* _entries = (page_table_entry_l1_t*)page_table;
    _entries[table_index] = (physical_addr & 0xFFF00000) |
                            PTE_L1_SECTION_AP(2) | // always r/w for now
                            PTE_L1_TYPE_SECTION;
}


void unmap_section(page_table_t page_table, uint32_t section_addr)
{
    uint32_t table_index = section_addr >> 20;
    page_table_entry_l1_t* _entries = (page_table_entry_l1_t*)page_table;
    _entries[table_index] = 0;
}


page_table_t mmu_init()
{
    // boot.s creates a page table at 0xC000, which gets mirrored at 0xC000C000
    // it doesn't bother initializing the page table. It maps the section at
    // 0x0 and 0xC0000000 to point to the first 1MB of physical memory but
    // nothing else.
    page_table_t page_table = (page_table_t) 0xC000C000;

    // Unmap anything below 0xC0000000. We don't need the identity mapping on
    // the first 1MB of memory anymore (as we've branched to higher memory).
    // It also doesn't hurt to ensure that all other sections below 0xC0000000
    // are explicitly unmapped.
    const uint32_t section_size = 0x00100000;
    for (uint32_t addr = 0x0; addr < 0xC0000000; addr += section_size) {
        unmap_section(page_table, addr);
    }
    // Explicitly unmap anything but the first 1MB of memory above 0xC0000000
    // (which is where our kernel lives).
    for (uint32_t addr = 0xC1000000; addr < 0xFF000000; addr += section_size) {
        unmap_section(page_table, addr);
    }

    // The BCM2835 puts the I/O peripherals at 0x20000000. Map these to
    // 0x7Ennnnnn. Conveniently, these are the addresses that the BCM2835 doc
    // uses to refer to them.
    for (uint32_t addr = 0x7E000000; addr < 0x7F000000; addr += section_size) {
        map_section(page_table, addr, 0x20000000 | (addr & 0x00FFFFFF));
    }

    // Return out L1 PT, as we can modify it later.
    return page_table;
}

