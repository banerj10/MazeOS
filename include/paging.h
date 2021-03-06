/*
 * paging.h - Declares functions for Paging initialzation
 */

#ifndef PAGING_H
#define PAGING_H

#include "types.h"
#include "lib.h"

#define SHIFT_4MB                 22
#define SHIFT_4KB                 12

#define PAGE_COUNT                1024
#define PAGE_ALIGN                4096

#define VIDEO_MEM_START           0xB8000  // start of video memory
#define VIDEO_MEM_PG_COUNT        1    // number of pages in video memory (32KB)
#define VIDEO_MEM_INDEX           (VIDEO_MEM_START / PAGE_ALIGN) // 0xB8

#define KERNEL_MEM_START          _4MB // start of 4MB Kernel in memory

#define VID_BKUP_MEM_START_VIRT   (_128MB + _8MB)
#define VID_BKUP_MEM_START_PHYS   _64MB

typedef struct __attribute__((packed)) pde_4M {
    uint32_t present : 1;
    uint32_t read_write : 1;
    uint32_t user_supervisor : 1;
    uint32_t writethrough : 1;
    uint32_t cache_disabled : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t page_size : 1;
    uint32_t global : 1;
    uint32_t available : 3;
    uint32_t attr_index : 1;
    uint32_t reserved : 9;
    uint32_t base_addr : 10;
} pde_4M_t;

typedef struct __attribute__((packed)) pde_4K {
    uint32_t present : 1;
    uint32_t read_write : 1;
    uint32_t user_supervisor : 1;
    uint32_t writethrough : 1;
    uint32_t cache_disabled : 1;
    uint32_t accessed : 1;
    uint32_t reserved : 1;
    uint32_t page_size : 1;
    uint32_t global : 1;
    uint32_t available : 3;
    uint32_t base_addr : 20;
} pde_4K_t;

typedef struct __attribute__((packed)) pte {
    uint32_t present : 1;
    uint32_t read_write : 1;
    uint32_t user_supervisor : 1;
    uint32_t writethrough : 1;
    uint32_t cache_disabled : 1;
    uint32_t accessed : 1;
    uint32_t dirty : 1;
    uint32_t attr_index : 1;
    uint32_t global : 1;
    uint32_t available : 3;
    uint32_t base_addr : 20;
} pte_t;

/* Externally visible functions */

void init_paging(void);

void map_page_4MB(uint32_t vir_addr, pde_4M_t pde);
void map_actual_vidmem(uint32_t phys_addr);
void map_user_video_mem(uint32_t vir_addr, pte_t pte);
void free_user_video_mem(uint32_t vir_addr);
void map_backup_vidmem(uint32_t vir_addr, uint32_t phys_addr);
void flush_tlb();

/* Functions defined in Assembly */
extern void load_page_directory(uint32_t pagedir_addr);
extern void enable_paging(void);

#endif
