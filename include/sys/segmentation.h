/* Include guard */
#ifndef TX_SYS_SEGMENTATION_H
#define TX_SYS_SEGMENTATION_H

/* Includes */
#include <core/base.h>

/* Consts */
/* Segment selectors */
/* The kernel code segment */
#define SEGMENT_KERNEL_CODE 0x0010
/* The kernel data segment */
#define SEGMENT_KERNEL_DATA 0x0020
/* The user code segment */
#define SEGMENT_USER_CODE   0x0033
/* The user data segment */
#define SEGMENT_USER_DATA   0x0043
/* The task state segment */
#define SEGMENT_TSS         0x0053

/* Initialize segmentation */
extern bool segmentation_init(void);

#endif /* TX_SYS_SEGMENTATION_H */
