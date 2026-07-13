ENTRY(start)

SECTIONS {
    . = 1M;                  /* Addres - 1Mb, right after BIOS */

    .multiboot : {
        *(.multiboot)        /* Header at first */
    }

    .text : {
        *(.text)             /* Whole code */
    }

    .rodata : {
        *(.rodata)           /* Read-only data */
    }

    .data : {
        *(.data)             /* Global variables */
    }

    .bss : {
        *(.bss)
    }
}