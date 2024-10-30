/* Include guard */
#ifndef TX_MEM_PFA_H
#define TX_MEM_PFA_H

/* Includes */
#include <core/base.h>
#include <core/panic.h>
#include <io/tty.h>
#include <ext/limine.h>

/* Initialize page frame allocator */
extern bool pfa_init(struct limine_memmap_entry **mmap, size_t mmap_size);

/* Get a free page from the page frame allocator, and set it as used */
extern void *pfa_get_page(void);
/* Mark a page as free */
extern void pfa_free_page(void *page);

#endif /* TX_MEM_PFA_H */
