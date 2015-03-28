#ifndef __MMM_H__
#define __MMU_H__


// page_table_t -- Either a pointer to a pte_l1_t[4096] or pte_l2_t[256];
typedef void* page_table_t;


// page_table_entry_l1_t -- Level 1 Page Table Entry
enum {
    PTE_L1_TYPE_UNMAPPED = 0x0,
    PTE_L1_TYPE_SECTION = 0x2,
    PTE_L1_TYPE_69 = 0x1
};
typedef uint32_t page_table_entry_l1_t;
#define PTE_L1_DOMAIN(x) ((x & 0xF) << 5)
#define PTE_L1_SECTION_AP(x) ((x & 0x3) << 10)
#define PTE_L1_SECTION_TEX(x) ((x & 0x3) << 10)
#define PTE_L1_SECTION_BASE_ADDR(x) ((x & 0xFFF) << 20)
#define PTE_L1_COARSE_BASE_ADDR(x) ((x & 0xFFFFF) << 10)


// page_table_entry_l2_t -- Level 2 Page Table Entry
enum {
    PTE_L2_TYPE_EXTENDED_SMALL = 0x3,
};
typedef uint32_t page_table_entry_l2_t;


page_table_t create_page_table();
page_table_t create_identity_page_table();

void set_level1_page_table(page_table_t page_table);
void set_mmu_state(uint8_t enabled);

page_table_t init_mmu();


#endif // __MMU_H__