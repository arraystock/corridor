#ifndef PMM_H
#define PMM_H

#include <libk/types.h>

// Page Size
#define PAGE_SIZE 4096

// Physical address
typedef	uint32_t physical_addr;

// Initialize the physical memory manager
void pmm_init (size_t, physical_addr);

// Enables a physical memory region for use
void pmm_init_region (physical_addr, size_t);

// Disables a physical memory region as in use (unuseable)
void pmm_deinit_region (physical_addr base, size_t);

// Allocates a single memory block
void* pmm_alloc_block ();

// Releases a memory block
void pmm_free_block (void*);

// Allocates blocks of memory
void* pmm_alloc_blocks (size_t);

// Frees blocks of memory
void pmm_free_blocks (void*, size_t);

// Returns amount of physical memory the manager is set to use
size_t pmm_get_memory_size ();

// Returns number of blocks currently in use
uint32_t pmm_get_use_block_count ();

// Returns number of blocks not in use
uint32_t pmm_get_free_block_count ();

// Returns number of memory blocks
uint32_t pmm_get_block_count ();

// Returns default memory block size in bytes
uint32_t pmm_get_block_size ();

// Enable or disable paging
void pmm_paging_enable (int enabled);

// Loads the page directory base register (PDBR)
void pmm_load_PDBR (physical_addr);

// Get PDBR physical address
physical_addr pmm_get_PDBR ();

#endif
