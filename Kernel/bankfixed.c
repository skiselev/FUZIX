/*
 *	This module manages a system with fixed sized banking where all but one
 *	area of memory is pinned and the other area can be set to multiple
 *	different banks.
 *
 *	FFFF		+-----------------------------------+
 *              	|          Common Memory            |
 *    MAPBASE+MAP_SIZE	+-----------------------------------+
 *	        	|          Banked Memory            |
 *      MAPBASE		+-----------------------------------+
 *			|          Common Memory	    |
 *	0000		+-----------------------------------+
 *
 *
 *	Other requirements:
 *	- 16bit address space (under 64K is fine, over is not) [Wants fixing]
 *
 *	Set:
 *	CONFIG_BANK_FIXED
 *	MAX_MAPS	to the number of banks available for user programs
 *	SWAPDEV		if using swap
 *	MAP_SIZE	the size of the bankable area
 *
 *	Most platforms using this module can use the standard lib/ tricks.s for
 *	their processor.
 *
 *	Bank numbers must not include 0 (0 is taken as swapped)
 */

#include <kernel.h>
#include <kdata.h>
#include <printf.h>

#ifdef CONFIG_BANK_FIXED

#ifndef CONFIG_COMMON_COPY
#define flush_cache(p)	do {} while(0)
#endif

/* Kernel is 0, apps 1,2,3 etc */
static unsigned char pfree[MAX_MAPS];
static unsigned char pfptr = 0;
static unsigned char pfmax;

void pagemap_add(uint8_t page)
{
	pfree[pfptr++] = page;
	pfmax = pfptr;
}

void pagemap_free(ptptr p)
{
	if (p->p_page == 0)
		panic("free0");
	pfree[pfptr++] = p->p_page;
}

int pagemap_alloc(ptptr p)
{
#ifdef SWAPDEV
	if (pfptr == 0) {
		swapneeded(p, 1);
	}
#endif
	if (pfptr == 0)
		return ENOMEM;
	p->p_page = pfree[--pfptr];
	return 0;
}

/* Realloc is trivial - we can't do anything useful */
int pagemap_realloc(uint16_t size)
{
	if (size > MAP_SIZE)
		return ENOMEM;
	return 0;
}

uint16_t pagemap_mem_used(void)
{
	return (pfmax - pfptr) * (MAP_SIZE >> 10);
}

#ifdef SWAPDEV
/*
 *	Swap out the memory of a process to make room
 *	for something else
 */
int swapout(ptptr p)
{
	uint16_t page = p->p_page;
	uint16_t blk;
	uint16_t map;

	swapproc = p;

	if (!page)
		panic("process already swapped!\n");
#ifdef DEBUG
	kprintf("Swapping out %x (%d)\n", p, p->p_page);
#endif
	/* Are we out of swap ? */
	map = swapmap_alloc();
	if (map == 0)
		return ENOMEM;
	blk = map * SWAP_SIZE;
	/* Write the app (and possibly the uarea etc..) to disk */
	swapwrite(SWAPDEV, blk, SWAPTOP - SWAPBASE,
		  SWAPBASE);
	pagemap_free(p);
	p->p_page = 0;
	p->p_page2 = map;
#ifdef DEBUG
	kprintf("%x: swapout done %d\n", p, p->p_page);
#endif
	return 0;
}

/*
 * Swap ourself in: must be on the swap stack when we do this
 */
void swapin(ptptr p)
{
	uint16_t blk = p->p_page2 * SWAP_SIZE;

#ifdef DEBUG
	kprintf("Swapin %x, %d\n", p, p->p_page);
#endif
	if (!p->p_page) {
		kprintf("%x: nopage!\n", p);
		return;
	}

	/* Return our swap */
	swapmap_add(p->p_page2);

	swapproc = p;		/* always ourself */
	swapread(SWAPDEV, blk, SWAPTOP - SWAPBASE,
		 SWAPBASE);
#ifdef DEBUG
	kprintf("%x: swapin done %d\n", p, p->p_page);
#endif
}

#endif

#endif
