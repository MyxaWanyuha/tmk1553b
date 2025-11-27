
/****************************************************************************/
/*      TMKLLLIN v8.05 for Linux. ELCUS, 1995,2021                          */
/*      Linux wrapper for the driver TMKNLL v8.05.                          */
/****************************************************************************/

#include "tmklllin.h"

//#define RAMwoCLI
//#define MASKTMKS

//#define DRV_MAX_BASE 255
//#define DRV_MAX_BASE 511
#define DRV_MAX_BASE 1023
//#define DRV_EMULATE_FIRST_CX_SIG

#define NOT_INCLUDE_DEFS

#include "tmknll.c"
