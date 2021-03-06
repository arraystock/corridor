#include <mem/pmm.h>
#include <libk/stdlib.h>
#include <libk/types.h>
#include <task/spinlock.h>
#include <ds/bitmap.h>

// 8 blocks per byte
#define BLOCKS_PER_BYTE 8

// Block size (4k)
#define BLOCK_SIZE	4096

// Block alignment
#define BLOCK_ALIGN	BLOCK_SIZE

// Size of physical memory
uint32_t mem_size = 0;

// Number of blocks currently in use
uint32_t used_blocks = 0;

// Maximum number of available memory blocks
uint32_t max_blocks = 0;

// Memory bitmap. Each bit represents a memory block
bitmap_t* mem_map = 0;

// Frame allocator spinlock
spinlock_t pmm_lock = {.lock = 0};

// Enable paging!
extern void paging_enable();

// Disable paging!
extern void paging_disable();

extern physical_addr pmm_get_PDBR();

// Finds first free frame in the bit array and returns its index
static int pmm_first_free ()
{
	// Find the first free bit
	for (uint32_t i=0; i< pmm_get_block_count() / 32; i++)
		if (mem_map[i] != 0xffffffff)
			for (int j=0; j<32; j++)
			{	
				// Test each bit in the dword
				int bit = 1 << j;
				if (!(mem_map[i] & bit))
					return i*4*8+j;
			}

	return -1;
}

// Finds first free "size" number of frames and returns its index
static int pmm_first_free_s (size_t size)
{
	if (size == 0)
		return -1;

	if (size == 1)
		return pmm_first_free ();

	for (uint32_t i = 0; i < pmm_get_block_count() / 32; i++)
		if (mem_map[i] != 0xffffffff)
			for (int j = 0; j < 32; j++)
			{	
				// Test each bit in the dword
				int bit = 1<<j;
				if (! (mem_map[i] & bit) )
				{
					int startingBit = i*32;
					startingBit+=bit; // Get the free bit in the dword at index i

					uint32_t free=0; // Loop through each bit to see if its enough space
					for (uint32_t count=0; count<=size;count++)
					{
						if (!bitmap_test(mem_map, startingBit+count))
							free++;	// This bit is clear (free frame)

						if (free==size)
							return i*4*8+j; // free count==size needed; return index
					}
				}
			}

	return -1;
}

void pmm_init (size_t memSize, physical_addr bitmap)
{
	mem_size = memSize;
	mem_map	= (uint32_t*) bitmap;
	max_blocks	= (pmm_get_memory_size() * 1024) / BLOCK_SIZE;
	used_blocks	= max_blocks;

	// By default, all of memory is in use
	kmemset (mem_map, 0xf, pmm_get_block_count() / BLOCKS_PER_BYTE );
}

// NOTE: pmm_init_region and pmm_deinit_region are expected to only be called before tasking is initialized.
// Due to that, they are not implemented to be atomic!

void pmm_init_region (physical_addr base, size_t size)
{
	int align = base / BLOCK_SIZE;
	int blocks = size / BLOCK_SIZE;

	for (; blocks>=0; blocks--)
	{
		bitmap_unset(mem_map, align++);
		used_blocks--;
	}

	bitmap_set(mem_map, 0);	// First block is always set. This ensures allocs cant be 0.
}

void pmm_deinit_region (physical_addr base, size_t size)
{
	int align = base / BLOCK_SIZE;
	int blocks = size / BLOCK_SIZE;

	for (; blocks>=0; blocks--)
	{
		bitmap_set(mem_map, align++);
		used_blocks++;
	}
}

void* pmm_alloc_block ()
{
	acquire_spinlock(&pmm_lock);

	if (pmm_get_free_block_count() <= 0)
		return 0;	// Out of memory

	int frame = pmm_first_free ();

	if (frame == -1)
		return 0;	// Out of memory

	bitmap_set(mem_map, frame);

	physical_addr addr = frame * BLOCK_SIZE;
	used_blocks++;

	release_spinlock(&pmm_lock);

	return (void*)addr;
}

void pmm_free_block (void* p)
{
	acquire_spinlock(&pmm_lock);

	physical_addr addr = (physical_addr)p;
	int frame = addr / BLOCK_SIZE;

	bitmap_unset(mem_map, frame);

	used_blocks--;

	release_spinlock(&pmm_lock);
}

void* pmm_alloc_blocks (size_t size)
{
	acquire_spinlock(&pmm_lock);

	if (pmm_get_free_block_count() <= size)
		return 0;	// Not enough space

	int frame = pmm_first_free_s (size);

	if (frame == -1)
		return 0;	// Not enough space

	for (uint32_t i = 0; i < size; i++)
		bitmap_set(mem_map, frame+i);

	physical_addr addr = frame * BLOCK_SIZE;
	used_blocks+=size;

	release_spinlock(&pmm_lock);

	return (void*)addr;
}

void pmm_free_blocks (void* p, size_t size)
{
	acquire_spinlock(&pmm_lock);

	physical_addr addr = (physical_addr)p;
	int frame = addr / BLOCK_SIZE;

	for (uint32_t i=0; i<size; i++)
		bitmap_unset(mem_map, frame+i);

	used_blocks-=size;

	release_spinlock(&pmm_lock);
}

size_t pmm_get_memory_size ()
{
	return mem_size;
}

uint32_t pmm_get_block_count ()
{
	return max_blocks;
}

uint32_t pmm_get_use_block_count ()
{
	return used_blocks;
}

uint32_t pmm_get_free_block_count ()
{
	return max_blocks - used_blocks;
}

uint32_t pmm_get_block_size ()
{
	return BLOCK_SIZE;
}

void pmm_paging_enable (int b)
{
	if(b == 0)
		paging_disable();
	else
		paging_enable();
}

// Load cr3
void pmm_load_PDBR(physical_addr addr)
{
	__asm__ volatile("mov %0, %%cr3":: "r"(addr));
}
