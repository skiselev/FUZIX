/* Simple IDE interface */
#define CONFIG_IDE
#define IDE_REG_DATA		0xA3
#define IDE_REG_ERROR		0xA7
#define IDE_REG_FEATURES	0xA7
#define IDE_REG_SEC_COUNT	0xAB
#define IDE_REG_LBA_0		0xAF
#define IDE_REG_LBA_1		0xB3
#define IDE_REG_LBA_2		0xB7
#define IDE_REG_LBA_3		0xBB
#define IDE_REG_DEVHEAD		0xBB
#define IDE_REG_STATUS		0xBF
#define IDE_REG_COMMAND		0xBF

/* Enable to make ^Z dump the inode table for debug */
#undef CONFIG_IDUMP
/* Enable to make ^A drop back into the monitor */
#undef CONFIG_MONITOR
/* Profil syscall support (not yet complete) */
#define CONFIG_PROFIL
/* Multiple processes in memory at once */
#define CONFIG_MULTI
/* Single tasking */
#undef CONFIG_SINGLETASK
/* CP/M emulation */
#undef CONFIG_CPM_EMU

/* Video terminal, not a serial tty */
#define CONFIG_VT
/* Keyboard contains not ascii symbols */
#define CONFIG_UNIKEY
/* We don't need a font: We will use the ROM font */
#undef CONFIG_FONT8X8
#undef CONFIG_FONT8X8SMALL

/* Two fixed banks in use */
#define CONFIG_BANK_FIXED

/* We have two mappings from our 128K of memory */
#define MAX_MAPS	2
#define MAP_SIZE	0x8000U

/* Banks as reported to user space */
#define CONFIG_BANKS	1

/* Vt definitions */
#define VT_WIDTH	32
#define VT_HEIGHT	24
#define VT_RIGHT	31
#define VT_BOTTOM	23

#define TICKSPERSEC 50   /* Ticks per second */
#define PROGBASE    0x8000  /* also data base */
#define PROGLOAD    0x8000  /* also data base */
#define PROGTOP     0xFC00  /* Top of program, base of U_DATA copy */
#define PROC_SIZE   32	  /* Memory needed per process */

#define BOOT_TTY (513)  /* Set this to default device for stdio, stderr */
                          /* In this case, the default is the first TTY device */

/* We need a tidier way to do this from the loader */
#define CMDLINE	NULL	  /* Location of root dev name */

/* Device parameters */
#define NUM_DEV_TTY 1

#define TTYDEV   BOOT_TTY /* Device used by kernel for messages, panics */
#undef  SWAPDEV           /* Do not use swap */
#define NBUFS    9       /* Number of block buffers */
#define NMOUNTS	 4	  /* Number of mounts at a time */
#define MAX_BLKDEV 2	    /* 2 IDE drives, 1 SD drive */
