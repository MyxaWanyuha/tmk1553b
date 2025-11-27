
/****************************************************************************/
/*      TMKLLLIN.H v8.05 for Linux. ELCUS, 1995,2021                        */
/*      Interface to the driver TMKNLL v8.05 for Linux.                     */
/****************************************************************************/

#ifndef _TMKLLX_
#define _TMKLLX_

#define FARFN
#define FARIR
#define FARDT

#ifndef NTMK
#define NTMK 8
#endif
#ifndef NRT
#define NRT ((NTMK)*32)
#endif
#ifndef NMBCID
#define NMBCID ((NTMK)*2)
#endif

#define LINUX

#include <linux/version.h>
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) ((a)*65536+(b)*256+(c))
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,28)
#ifndef NO_SWTIMER
#define LINUX_SWTIMER
#endif
#endif

//#define TMK_ERROR_0 0xE0040000

#define STATIC_TMKNUM
//#define DYNAMIC_TMKNUM

#define TMK_REAL_MAX

#define USE_RTIFNB
#define USE_TMKRAWIO

#define __EXTERN extern

#ifdef __cplusplus
extern "C" {
#endif

#include "tmknll.h"

#ifdef __cplusplus
}
#endif

#endif
