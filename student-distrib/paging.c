/*
 * paging.c - Defines function for Paging initialzation
 */

#include "paging.h"

#define PAGE_COUNT         1024
#define PAGE_ALIGN         4096

#define VIDEO_MEM_START    0xB8     // index of start of video memory
#define VIDEO_MEM_PG_COUNT 8        // number of pages in video memory (32KB)

#define KERNEL_MEM_START   0x400000 // start of 4MB Kernel in memory

/* Paging fields */
#define PAGE_PRESENT       0x01     // bit 0
#define PAGE_READWRITE     0x02     // bit 1
#define PAGE_USER          0x04     // bit 2
#define PAGE_4MB           0x80     // bit 7


/* Arrays to hold the Page Directory and the table for the first 4MB section */
uint32_t page_directory[PAGE_COUNT] __attribute__((aligned(PAGE_ALIGN)));
uint32_t first_4MB_table[PAGE_COUNT] __attribute__((aligned(PAGE_ALIGN)));


/*
 * init_paging
 *   DESCRIPTION: Initializes the page directory array and the first 4MB page
 *                table, and calls functions to set up the Control Registers
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Enables paging
 */
void
init_paging(void)
{
    int i;

    /* Initialize all PDE's to be NOT present, Read/Write, Supervisor */
    for (i = 0; i < PAGE_COUNT; i++)
        page_directory[i] = PAGE_READWRITE;

    /* Initialize the 4KB PTE's for the first 4MB table in physical memory,
       to NOT present, Read/Write, Supervisor */
    for (i = 0; i < PAGE_COUNT; i++)
    {
        first_4MB_table[i] = (i * PAGE_ALIGN) | PAGE_READWRITE;
    }

    /* Initialize video memory pages (32KB) starting at 0xB8000,
       to present, Read/Write, Supervisor */
    for (i = 0; i < VIDEO_MEM_PG_COUNT; i++)
    {
        first_4MB_table[VIDEO_MEM_START + i] = ((VIDEO_MEM_START + i) *
            PAGE_ALIGN) | PAGE_PRESENT | PAGE_READWRITE;
    }

    /* First PDE should point to first_4MB_table, set to present, Read/Write,
       and User access */
    page_directory[0] = ((uint32_t) first_4MB_table) | PAGE_PRESENT |
        PAGE_READWRITE | PAGE_USER;

    /* 4MB for Kernel Space, set to present, Read/Write, Supervisor */
    page_directory[1] = KERNEL_MEM_START | PAGE_4MB | PAGE_PRESENT |
        PAGE_READWRITE;

    /* give the page_directory pointer to CR3 */
    load_page_directory((uint32_t) page_directory);
    /* call the enabler */
    enable_paging();
}