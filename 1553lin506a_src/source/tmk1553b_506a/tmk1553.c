/*
 * tmk1553b.c -- the tmk1553b v5.06a char module. (c) ELCUS, 2002,2024.
 *
 * Part of this code comes from the book "Linux Device Drivers"
 * by Alessandro Rubini and Jonathan Corbet, published
 * by O'Reilly & Associates.
 */

#define TMK_VER_HI 5
#define TMK_VER_LO 06
#ifdef TMK1553B_THREADS
#define TMK_VERSION "5.06a-t1"
#else
#define TMK_VERSION "5.06a-t0"
#endif

#ifndef __KERNEL__
#  define __KERNEL__
#endif
#ifndef MODULE
#  define MODULE
#endif

#ifndef TMK1553B_NOCONFIGH
#include <linux/config.h>
#endif
#ifdef CONFIG_SMP
#define __SMP__
#endif
#ifdef CONFIG_64BIT
#define __64BIT__
#endif
//#if CONFIG_MODVERSIONS==1
//#define MODVERSIONS
//#include <linux/modversions.h>
//#endif
#include <linux/version.h>
#include <linux/module.h>

#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>     /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>    /* O_ACCMODE */

#include <linux/ioport.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/pci.h>
#include <linux/miscdevice.h>
#include <linux/poll.h>

#ifndef KERNEL_VERSION
#define KERNEL_VERSION(a,b,c) ((a)*65536+(b)*256+(c))
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
#include <linux/kcomp.h>
#define spin_lock_bh(lock) do {start_bh_atomic(); spin_lock(lock);} while(0)
#define spin_unlock_bh(lock) do {spin_unlock(lock); end_bh_atomic();} while(0)
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
#define LINUX26
#endif
#ifndef LINUX26
#define IRQ_NONE
#define IRQ_HANDLED
#define IRQRETURN_T void
#else
#define IRQRETURN_T irqreturn_t
#endif
#ifndef EXPORT_NO_SYMBOLS
#define EXPORT_NO_SYMBOLS
#endif
#ifndef SET_MODULE_OWNER
#define SET_MODULE_OWNER(some_struct) do { } while (0)
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0)
#include <linux/sched/signal.h>
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,12,0)
#include <linux/uaccess.h>
#else
#include <asm/uaccess.h>
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
#define wait_queue_t wait_queue_entry_t
#endif
#ifndef fallthrough
#define fallthrough
#endif

//#define DBG 1
//#define MY_DBG
//#define MY_DBG_DPC

//#ifdef CONFIG_DEVFS_FS
//#define MY_CONFIG_DEVFS_FS
//#endif

#ifdef MY_DBG
#define MY_KERN_DEBUG KERN_EMERG
#define MY_KERN_INFO KERN_EMERG
#define MY_KERN_WARNING KERN_EMERG
#else
#define MY_KERN_DEBUG KERN_DEBUG
#define MY_KERN_INFO KERN_INFO
#define MY_KERN_WARNING KERN_WARNING
#endif // def MY_DBG

// for kernels 4.x and above
#ifndef IRQF_DISABLED
#define IRQF_DISABLED 0x0
#endif

typedef void* PVOID;
typedef u32 UINT;
typedef u32* PUINT;
typedef u16 USHORT;
typedef u16* PUSHORT;
typedef u8 UCHAR;
typedef u8* PUCHAR;
typedef int HANDLE;
typedef int NTSTATUS;
typedef int PKEVENT;
//typedef int PEPROCESS; now defined in tmkisr.h

#define IN
#define OUT

#define TRUE 1
#define FALSE 0

#if defined(TMK1553B_THREADS) || defined(LINUX26)
#define CURRENT_PID ((mthread) ? current->group_leader->pid : current->pid)
#else
#define CURRENT_PID (current->pid)
#endif

char *pszVerChange;

#include "tmk1553b.h"          /* local definitions */

int fTMKInit = 0;

UINT tmkNumber;

#ifndef NTMK
#define NTMK 8
#endif

#define MAX_TMK_NUMBER ((NTMK)-1)

#ifndef NRT
//#define MAX_RT_NUMBER (30-MAX_TMK_NUMBER-1)
#define MAX_RT_NUMBER ((NTMK)*32-1)
#else
#define MAX_RT_NUMBER ((NRT)-1)
#endif

#define MAX_VTMK_NUMBER (MAX_TMK_NUMBER+1+MAX_RT_NUMBER)

#define U32_EVENTS_SIZE ((MAX_VTMK_NUMBER)/32+1)
//#define U32_EVENTS_SIZE ((MAX_VTMK_NUMBER+1)/32)

#ifndef NMBCID
#define MAX_MBC_ID ((NTMK)*2-1)
#else
#define MAX_MBC_ID ((NMBCID)-1)
#endif

#ifndef MAX_PCI_SLOTS
#define MAX_PCI_SLOTS (((MAX_TMK_NUMBER)+1)*2)
#endif

#include "tmklllin.h"
#include "tmkisr.h"

struct pci_dev *apciDevs[MAX_PCI_SLOTS];
int cpciDevs = 0;
int fpciDevs = 0;

int aManCfgDrv[MAX_TMK_NUMBER+1];
int aManCfgPci[MAX_PCI_SLOTS];

int nMaxTmkNumber = MAX_TMK_NUMBER;

typedef struct
{
   short nType;
   char szName[10];
   unsigned short wPorts1;
   unsigned short wPorts2;
   unsigned short wIrq1;
   unsigned short wIrq2;
   unsigned short wIODelay;
   unsigned short wHiddenPorts;
   int nTmk;
   int fLoaded;
   int nLoadType;
   int fIrqShared;
   int nDev;
   int nSubDev;
   int fLocalReadInt;
   int fMRT;
   int nPorts;
   unsigned DID;
   unsigned SubDID;
} TTmkConfig;

TTmkConfig aTmkConfig[MAX_TMK_NUMBER+1];

typedef struct
{
  struct list_head ProcListEntry;
  PEPROCESS hProc;
  int fTMK[MAX_TMK_NUMBER+1+MAX_RT_NUMBER+1];
  int nSelectedTMK;
  u32 maskTMK[U32_EVENTS_SIZE];
  u32 maskEvents[U32_EVENTS_SIZE];
  int fMBC[MAX_MBC_ID+1];
  wait_queue_head_t wq;
  int waitFlag;
} TListProc;

struct list_head hlProc;

//HPROC hTmkVM[MAX_TMK_NUMBER+1];
PEPROCESS hCurProc;
TListProc *hlnCurProc;

typedef struct
{
  PEPROCESS hProc;
  PKEVENT hEvent;
  TListProc *hlnProc;
} TTMK;

TTMK aTMK[MAX_TMK_NUMBER+1+MAX_RT_NUMBER+1];

//typedef struct
//{
//  int nInt;
//  USHORT wMode;
//  USHORT awEvData[3];
//  PEPROCESS hProc;
//} TListEvD, *pTListEvD; now defined in tmkisr.h

//pTListEvD ahlEvData[MAX_TMK_NUMBER+1];
//pTListEvD hlnEvData;
//VMMLIST ahlEvData[MAX_TMK_NUMBER+1];
//VMMLISTNODE hlnEvData;

spinlock_t tmkIrqSpinLock, tmkSpinLock;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
void tmkDpcRoutine(void *);
#else
void tmkDpcRoutine(unsigned long);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,9,0) || ( LINUX_VERSION_CODE < KERNEL_VERSION(5,9,0) && defined(DECLARE_TASKLET_OLD) )
DECLARE_TASKLET_OLD (tmkDpc, tmkDpcRoutine);
#else
DECLARE_TASKLET (tmkDpc, tmkDpcRoutine, 0);
#endif
#endif
IRQRETURN_T tmkInterruptServiceRoutine(int irq, void *dev_id
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
                                       , struct pt_regs *regs
#endif
                                      );

volatile int tmkEvents[U32_EVENTS_SIZE];

typedef struct
{
  int nIrq;
  int cTmks;
  int hTmk[MAX_TMK_NUMBER+1];
} TIrq;

TIrq ahIrq[MAX_TMK_NUMBER+1];

int cIrqs = 0;

int nmrt, nrtmax;

#ifdef LINUX_SWTIMER
U32 dwSecMul;
int nNsecDiv;
int fUseEvTime = 0;
int afUseEvTime[MAX_TMK_NUMBER+1+MAX_RT_NUMBER+1];
unsigned short wSwTimerCtrl = 0;
#endif


char *szTmk1553b = TMK1553B_NAME;

#include "tmkinlin.c"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,10)
MODULE_LICENSE("GPL");
#endif
MODULE_AUTHOR("ELCUS");
MODULE_DESCRIPTION("Driver for ELCUS (http://www.elcus.ru) MIL-STD-1553B boards v" TMK_VERSION);
#ifdef MODULE_VERSION
MODULE_VERSION(TMK_VERSION);
#endif
#ifdef TMK1553B_INTREE
MODULE_INFO(intree,"Y");
#endif

#ifdef TMK1553B_DEBUG
tmk1553b_Dev *tmk1553b_devices; /* allocated in tmk1553b_init_module */
#endif

static struct miscdevice tmk1553b_mdev;

UINT TMK_tmkgetmaxn(void);
UINT TMK_tmkconfig(void);
UINT TMK_tmkdone(void);
UINT TMK_tmkselect(void);
UINT TMK_tmkselected(void);
UINT TMK_tmkgetmode(void);
UINT TMK_tmksetcwbits(void);
UINT TMK_tmkclrcwbits(void);
UINT TMK_tmkgetcwbits(void);
UINT TMK_tmkwaitevents(void);
//UINT TMK_tmkdefevent(void);
UINT TMK_tmkgetevd(void);

//UINT TMK_bcdefintnorm(void);
//UINT TMK_bcdefintexc(void);
//UINT TMK_bcdefintx(void);
//UINT TMK_bcdefintsig(void);
UINT TMK_bcreset(void);
UINT TMK_bc_def_tldw(void);
UINT TMK_bc_enable_di(void);
UINT TMK_bc_disable_di(void);
UINT TMK_bcdefirqmode(void);
UINT TMK_bcgetirqmode(void);
UINT TMK_bcgetmaxbase(void);
UINT TMK_bcdefbase(void);
UINT TMK_bcgetbase(void);
UINT TMK_bcputw(void);
UINT TMK_bcgetw(void);
UINT TMK_bcgetansw(void);
UINT TMK_bcputblk(void);
UINT TMK_bcgetblk(void);
UINT TMK_bcdefbus(void);
UINT TMK_bcgetbus(void);
UINT TMK_bcstart(void);
UINT TMK_bcstartx(void);
UINT TMK_bcdeflink(void);
UINT TMK_bcgetlink(void);
UINT TMK_bcstop(void);
UINT TMK_bcgetstate(void);

//UINT TMK_rtdefintcmd(void);
//UINT TMK_rtdefinterr(void);
//UINT TMK_rtdefintdata(void);
UINT TMK_rtreset(void);
UINT TMK_rtdefirqmode(void);
UINT TMK_rtgetirqmode(void);
UINT TMK_rtdefmode(void);
UINT TMK_rtgetmode(void);
UINT TMK_rtgetmaxpage(void);
UINT TMK_rtdefpage(void);
UINT TMK_rtgetpage(void);
UINT TMK_rtdefpagepc(void);
UINT TMK_rtdefpagebus(void);
UINT TMK_rtgetpagepc(void);
UINT TMK_rtgetpagebus(void);
UINT TMK_rtdefaddress(void);
UINT TMK_rtgetaddress(void);
UINT TMK_rtdefsubaddr(void);
UINT TMK_rtgetsubaddr(void);
UINT TMK_rtputw(void);
UINT TMK_rtgetw(void);
UINT TMK_rtputblk(void);
UINT TMK_rtgetblk(void);
UINT TMK_rtsetanswbits(void);
UINT TMK_rtclranswbits(void);
UINT TMK_rtgetanswbits(void);
UINT TMK_rtgetflags(void);
UINT TMK_rtputflags(void);
UINT TMK_rtsetflag(void);
UINT TMK_rtclrflag(void);
UINT TMK_rtgetflag(void);
UINT TMK_rtgetstate(void);
UINT TMK_rtbusy(void);
UINT TMK_rtlock(void);
UINT TMK_rtunlock(void);
UINT TMK_rtgetcmddata(void);
UINT TMK_rtputcmddata(void);

//UINT TMK_mtdefintx(void);
//UINT TMK_mtdefintsig(void);
UINT TMK_mtreset(void);
#define TMK_mtdefirqmode TMK_bcdefirqmode
#define TMK_mtgetirqmode TMK_bcgetirqmode
#define TMK_mtgetmaxbase TMK_bcgetmaxbase
#define TMK_mtdefbase TMK_bcdefbase
#define TMK_mtgetbase TMK_bcgetbase
#define TMK_mtputw TMK_bcputw
#define TMK_mtgetw TMK_bcgetw
UINT TMK_mtgetsw(void);
#define TMK_mtputblk TMK_bcputblk
#define TMK_mtgetblk TMK_bcgetblk
#define TMK_mtstartx TMK_bcstartx
#define TMK_mtdeflink TMK_bcdeflink
#define TMK_mtgetlink TMK_bcgetlink
#define TMK_mtstop TMK_bcstop
#define TMK_mtgetstate TMK_bcgetstate

UINT TMK_tmkgetinfo(void);
UINT TMK_getversion(void);

UINT TMK_rtenable(void);

UINT TMK_mrtgetmaxn(void);
UINT TMK_mrtconfig(void);
UINT TMK_mrtselected(void);
UINT TMK_mrtgetstate(void);
UINT TMK_mrtdefbrcsubaddr0(void);
UINT TMK_mrtreset(void);

UINT TMK_tmktimer(void);
UINT TMK_tmkgettimer(void);
UINT TMK_tmkgettimerl(void);
UINT TMK_bcgetmsgtime(void);
#define TMK_mtgetmsgtime TMK_bcgetmsgtime
UINT TMK_rtgetmsgtime(void);

UINT TMK_tmkgethwver(void);

UINT TMK_tmkgetevtime(void);
UINT TMK_tmkswtimer(void);
UINT TMK_tmkgetswtimer(void);

UINT TMK_tmktimeout(void);

UINT TMK_mrtdefbrcpage(void);
UINT TMK_mrtgetbrcpage(void);

UINT TMK_mbcinit(void);
UINT TMK_mbcpreparex(void);
UINT TMK_mbcstartx(void);
UINT TMK_mbcalloc(void);
UINT TMK_mbcfree(void);

UINT TMK_tmkwaiteventsflag(void);

UINT TMK_mwreset(void);
UINT TMK_mwtreset(void);
UINT TMK_mwrtreset(void);
UINT TMK_mtrtreset(void);
UINT TMK_mwtrtreset(void);
UINT TMK_mwcdw(void);
UINT TMK_mtgetsw2(void);
UINT TMK_mtgetswa(void);
UINT TMK_mtgetmaxmaxbase(void);
UINT TMK_mtdefmaxbase(void);
UINT TMK_tmktxrxmode(void);

UINT TMK_tmkwaiteventsm(void);

UINT TMK_bcgetmsgsw(void);
#define TMK_mtgetmsgsw TMK_mtgetswa
UINT TMK_rtgetmsgsw(void);

UINT TMK_rtgap(void);

UINT TMK_rtallocsabuf(void);
UINT TMK_rtdefbuf(void);
UINT TMK_rtgetbuf(void);
UINT TMK_rtdeflink(void);
UINT TMK_rtgetlink(void);
UINT TMK_rtdeflinkbus(void);
UINT TMK_rtgetlinkbus(void);

UINT TMK_tmkdac(void);

UINT TMK_rtdefsubaddrm(void);
UINT TMK_rtgetbase(void);
UINT TMK_rtgetbasebus(void);

UINT TMK_mtdefmode(void);
UINT TMK_mtgetmode(void);

UINT TMK_bcdefmode(void);
UINT TMK_bcgetmode(void);
UINT TMK_bcstartctla(void);
UINT TMK_bcstarta(void);
UINT TMK_bcdefctla(void);
UINT TMK_bcgetctla(void);

UINT TMK_mbcpreparea(void);
UINT TMK_mbcstarta(void);

UINT TMK_tmkoptions(void);

UINT TMK_mtfilter(void);
UINT TMK_rtlegal(void);

UINT TMK_bcclrmsgtime(void);
UINT TMK_mtclrmsgtime(void);
UINT TMK_rtclrmsgtime(void);
UINT TMK_bcclrmsgsw(void);
UINT TMK_mtclrmsgsw(void);
UINT TMK_rtclrmsgsw(void);

UINT TMK_tmkbuserr(void);

UINT TMK_tmkinfo(void);

UINT TMK_tmkrawion(void);

UINT TMK_rtputblkifnb(void);
UINT TMK_rtgetblkifnb(void);

UINT TMK_tmknop(void);

//UINT TMK_bcputblk64(void);
//UINT TMK_bcgetblk64(void);

//UINT TMK_rtputblk64(void);
//UINT TMK_rtgetblk64(void);

//UINT TMK_rtgetflags64(void);
//UINT TMK_rtputflags64(void);

UINT (*TMK_Procs[])(void) = {
        TMK_tmkconfig,
        TMK_tmkdone,
        TMK_tmkgetmaxn,
        TMK_tmkselect,
        TMK_tmkselected,
        TMK_tmkgetmode,
        TMK_tmksetcwbits,
        TMK_tmkclrcwbits,
        TMK_tmkgetcwbits,
        TMK_tmkwaitevents,
//        TMK_tmkdefevent,
        TMK_tmkgetevd,

//        TMK_bcdefintnorm,
//        TMK_bcdefintexc,
//        TMK_bcdefintx,
//        TMK_bcdefintsig,
        TMK_bcreset,
        TMK_bc_def_tldw,
        TMK_bc_enable_di,
        TMK_bc_disable_di,
        TMK_bcdefirqmode,
        TMK_bcgetirqmode,
        TMK_bcgetmaxbase,
        TMK_bcdefbase,
        TMK_bcgetbase,
        TMK_bcputw,
        TMK_bcgetw,
        TMK_bcgetansw,
        TMK_bcputblk,
        TMK_bcgetblk,
        TMK_bcdefbus,
        TMK_bcgetbus,
        TMK_bcstart,
        TMK_bcstartx,
        TMK_bcdeflink,
        TMK_bcgetlink,
        TMK_bcstop,
        TMK_bcgetstate,

//        TMK_rtdefintcmd,
//        TMK_rtdefinterr,
//        TMK_rtdefintdata,
        TMK_rtreset,
        TMK_rtdefirqmode,
        TMK_rtgetirqmode,
        TMK_rtdefmode,
        TMK_rtgetmode,
        TMK_rtgetmaxpage,
        TMK_rtdefpage,
        TMK_rtgetpage,
        TMK_rtdefpagepc,
        TMK_rtdefpagebus,
        TMK_rtgetpagepc,
        TMK_rtgetpagebus,
        TMK_rtdefaddress,
        TMK_rtgetaddress,
        TMK_rtdefsubaddr,
        TMK_rtgetsubaddr,
        TMK_rtputw,
        TMK_rtgetw,
        TMK_rtputblk,
        TMK_rtgetblk,
        TMK_rtsetanswbits,
        TMK_rtclranswbits,
        TMK_rtgetanswbits,
        TMK_rtgetflags,
        TMK_rtputflags,
        TMK_rtsetflag,
        TMK_rtclrflag,
        TMK_rtgetflag,
        TMK_rtgetstate,
        TMK_rtbusy,
        TMK_rtlock,
        TMK_rtunlock,
        TMK_rtgetcmddata,
        TMK_rtputcmddata,

//        TMK_mtdefintx,
//        TMK_mtdefintsig,
        TMK_mtreset,
        TMK_mtdefirqmode,
        TMK_mtgetirqmode,
        TMK_mtgetmaxbase,
        TMK_mtdefbase,
        TMK_mtgetbase,
        TMK_mtputw,
        TMK_mtgetw,
        TMK_mtgetsw,
        TMK_mtputblk,
        TMK_mtgetblk,
        TMK_mtstartx,
        TMK_mtdeflink,
        TMK_mtgetlink,
        TMK_mtstop,
        TMK_mtgetstate,

        TMK_tmkgetinfo,
        TMK_getversion,

        TMK_rtenable,

        TMK_mrtgetmaxn,
        TMK_mrtconfig,
        TMK_mrtselected,
        TMK_mrtgetstate,
        TMK_mrtdefbrcsubaddr0,
        TMK_mrtreset,

        TMK_tmktimer,
        TMK_tmkgettimer,
        TMK_tmkgettimerl,
        TMK_bcgetmsgtime,
        TMK_mtgetmsgtime,
        TMK_rtgetmsgtime,

        TMK_tmkgethwver,

        TMK_tmkgetevtime,
        TMK_tmkswtimer,
        TMK_tmkgetswtimer,

        TMK_tmktimeout,

        TMK_mrtdefbrcpage,
        TMK_mrtgetbrcpage,

        TMK_mbcinit,
        TMK_mbcpreparex,
        TMK_mbcstartx,
        TMK_mbcalloc,
        TMK_mbcfree,

        TMK_tmknop,//112 MT_Start
        TMK_tmknop,//113 MT_GetMessage
        TMK_tmknop,//114 MT_Stop

        TMK_tmkwaiteventsflag,

        TMK_mwreset,
        TMK_mwtreset,
        TMK_mwrtreset,
        TMK_mtrtreset,
        TMK_mwtrtreset,

        TMK_mwcdw,
        TMK_mtgetsw2,
        TMK_mtgetswa,
        TMK_mtgetmaxmaxbase,
        TMK_mtdefmaxbase,

        TMK_tmktxrxmode,

        TMK_tmkwaiteventsm, //TMK_tmkreadsn_usb

        TMK_bcgetmsgsw,
        TMK_mtgetmsgsw,
        TMK_rtgetmsgsw,

        TMK_rtgap,

        TMK_rtallocsabuf,
        TMK_rtdefbuf,
        TMK_rtgetbuf,
        TMK_rtdeflink,
        TMK_rtgetlink,
        TMK_rtdeflinkbus,
        TMK_rtgetlinkbus,

        TMK_tmkdac,

        TMK_rtdefsubaddrm,
        TMK_rtgetbase,
        TMK_rtgetbasebus,

        TMK_mtdefmode,
        TMK_mtgetmode,

        TMK_bcdefmode,
        TMK_bcgetmode,
        TMK_bcstartctla,
        TMK_bcstarta,
        TMK_bcdefctla,
        TMK_bcgetctla,

        TMK_mbcpreparea,
        TMK_mbcstarta,

        TMK_tmkoptions,

        TMK_mtfilter,
        TMK_rtlegal,

        TMK_bcclrmsgtime,
        TMK_mtclrmsgtime,
        TMK_rtclrmsgtime,
        TMK_bcclrmsgsw,
        TMK_mtclrmsgsw,
        TMK_rtclrmsgsw,

        TMK_tmkbuserr,

        TMK_tmkinfo,

        TMK_tmkrawion,

        TMK_rtputblkifnb,
        TMK_rtgetblkifnb

        };

#define MAX_TMK_API (sizeof(TMK_Procs)/sizeof(void*)+1)

//int aCmds[MAX_TMK_API+1];

//#include "tmk1553b.h"

//#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
//#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((USHORT)(l))
#define HIWORD(l)           ((USHORT)(((UINT)(l) >> 16) & 0xFFFF))
//#define LOBYTE(w)           ((BYTE)(w))
//#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

//typedef char *PSTR;
//typedef USHORT USHORT;
//typedef USHORT *PUSHORT;

// Pointers to input and output buffers
PUSHORT lpwIn, lpwOut, lpwBuf;
UINT *pdwUserRawBufOO;
UINT *pdwUserRawBufII;
UINT dwUserRawLenO;
UINT dwUserRawLenI;
//PUINT lpcbReturned;

#define TMK_BAD_0      -1024
/*
#define TMK_BAD_TYPE   1
#define TMK_BAD_IRQ    2
#define TMK_BAD_NUMBER 3
#define BC_BAD_BUS     4
#define BC_BAD_BASE    5
#define BC_BAD_LEN     6
#define RT_BAD_PAGE    7
#define RT_BAD_LEN     8
#define RT_BAD_ADDRESS 9
#define RT_BAD_FUNC    10
#define BC_BAD_FUNC    11
#define TMK_BAD_FUNC   12

#define TMK_MAX_ERROR  12
*/

#include "tmktest.c"

#define MIN_LOAD_TYPE_TMKX 0
#define MAX_LOAD_TYPE_TMKX 8
#define MIN_LOAD_TYPE_MRTA 9
#define MAX_LOAD_TYPE_MRTA 10
#define MAX_LOAD_TYPE 10

#define LOADTX1 0      /* default for TMKX, always on */
//#define LOADRT1 1
#define LOADTX4 2
#define LOADTX5 3
#define LOADTX6 4
#define LOADMR6 5
//#define LOADTT6 6
//#define LOADTD6 7
//#define LOADAM6 8
#define LOADMRA 9      /* default for MRTA, always on */
#define LOADTRA 10

#include "load/txv11.c"      //fpgabuf0[]
#ifdef LOADRT1
#include "load/rt1v03.c"     //fpgabuf1[]
#endif //def LOADRT1
#ifdef LOADTX6
#define fpgabuf4 fpgabuf0
#endif //def LOADTX6
#ifdef LOADTX4
#define fpgabuf2 fpgabuf0
#endif //def LOADTX4
#ifdef LOADTX5
#define fpgabuf3 fpgabuf0
#endif //def LOADTX5

#ifdef CONFIG_PCI
#define fpgabuf5 fpgabuf0
#ifdef LOADTX6
#define fpgabuf6 fpgabuf0
#endif //def LOADTX6
#endif //def CONFIG_PCI

#ifdef LOADMR6
#include "load/mr6v02.c"     //fpgabuf7[]
#endif //def LOADMR6
#ifdef LOADTT6
#include "load/tt6v04.c"     //fpgabuf8[]
#endif //def LOADTT6
#ifdef LOADTD6
#include "load/td6v02.c"     //fpgabuf9[]
#endif //def LOADTD6
#ifdef LOADAM6
#include "load/am6v02.c"     //fpgabuf13[]
#endif //def LOADAM6

#ifdef CONFIG_PCI
#ifdef LOADMR6
#define fpgabuf10 fpgabuf7
#endif //def LOADMR6
#ifdef LOADTT6
#define fpgabuf11 fpgabuf8
#endif //def LOADTT6
#ifdef LOADTD6
#define fpgabuf12 fpgabuf9
#endif //def LOADTD6
#ifdef LOADAM6
#define fpgabuf14 fpgabuf13
#endif //def LOADTD6
#endif //def CONFIG_PCI


#ifdef CONFIG_PCI
#define TMK_MAX_IDID 7
u16 aPciVendorID[TMK_MAX_IDID+1] = {0x10B5, 0x10B5, 0xE1C5, 0xE1C5, 0xE1C5, 0xE1C5, 0xE1C5, 0xE1C5};
u16 aPciDeviceID[TMK_MAX_IDID+1] = {0x9030, 0x9056, 0x9030, 0x9056, 0x9000, 0x9001, 0x9002, 0x9003};
#define PLX_DID0 0x9030
#define PLX_DID1 0x9056
#define ID_TX1PCI       0x0001E1C5
#define ID_TX6PCI       0x0002E1C5
#define ID_TX1PCI2      0x0003E1C5
#define ID_TX6PCI2      0x0004E1C5
#define ID_TA1PCI       0x0005E1C5
#define ID_TA1PCI4      0x0006E1C5
#define ID_TA1PCI32RT   0x0007E1C5
#define ID_TA1PE2       0x0008E1C5
#define ID_TA13U2R      0x0401E1C5
#define ID_TA1PE4       0x0009E1C5
#define ID_TA1PE32RT    0x000AE1C5
#define ID_TA1MPE2      0x000BE1C5
#define ID_TA1NPCI4     0x000CE1C5
#define ID_TA1NPE4      0x000DE1C5
//#define ID_TA1NNPE4     0x000DE1C5
#define ID_TA2PE4       0x000FE1C5
#define ID_TA1P         0x0010E1C5
#define ID_TA1P32RT     0x0011E1C5
#define ID_TA1P32RTE    0x0012E1C5
#define ID_TA1P32RTF    0x0013E1C5
#define ID_TA1P32RTV    0x0014E1C5
//#define ID_TA1PA708     0x0015E1C5
//#define ID_TA1PE4A708   0x0016E1C5
#define ID_TA1PE432RT   0x0017E1C5
#define ID_TA1PE432RTE  0x0018E1C5
#define ID_TA1PE432RTF  0x0019E1C5
#define ID_TA1BPE       0x001AE1C5
#define ID_TA1BPE32RT   0x001BE1C5
#define ID_TA1BPE32RTE  0x001CE1C5
#define ID_TA1BPE32RTF  0x001DE1C5
#define ID_TA1BPE32RTV  0x001EE1C5
//#define ID_TA1BPEA708   0x001FE1C5
#define ID_TA2P         0x0020E1C5
#define ID_TA2P32RT     0x0021E1C5
#define ID_TA2P32RTE    0x0022E1C5
#define ID_TA2P32RTF    0x0023E1C5
#define ID_TA2P32RTV    0x0024E1C5
//#define ID_TA2PA708     0x0025E1C5
//#define ID_TA2PE4A708   0x0026E1C5
#define ID_TA2PE432RT   0x0027E1C5
#define ID_TA2PE432RTE  0x0028E1C5
#define ID_TA2PE432RTF  0x0029E1C5
#define ID_TA2BPE       0x002AE1C5
#define ID_TA2BPE32RT   0x002BE1C5
#define ID_TA2BPE32RTE  0x002CE1C5
#define ID_TA2BPE32RTF  0x002DE1C5
#define ID_TA2BPE32RTV  0x002EE1C5
//#define ID_TA2BPEA708   0x002FE1C5

#define CFG_SYSTEM_ID    0x00
#define CFG_DEVICE_ID    0x02
#define CFG_COMMAND      0x04
#define CFG_SUBSYSTEM_ID 0x2C
#define CFG_IRQ          0x3C
#define CFG_ADDR1        0x14
#define CFG_ADDR2        0x18
#define CFG_ADDR3        0x1C
#define CFG_ADDR4        0x20
#define CFG_ADDR5        0x24
#endif //def CONFIG_PCI

#define READ_PORT  (port+0xC)
#define LOAD_PORT  (port+0xA)
#define RESET_PORT (port+6)
#define ADDR_PORT  (port+0xA)
#define DATA_PORT  (port+0xE)
#define MODE_PORT  (port+8)

//#define BUSY_BIT      0x0080
#define BUSY_DEVICE   0x7E7F
#define EMPTY_DEVICE  0x7EFF
#define ERROR_DEVICE  0x7FFF
#define LOADED_DEVICE 0x0000

#define GENER1_BL 0x0004
#define GENER2_BL 0x4000

#define TX_OK         0
#define TX_BAD_PARAMS 1
#define TX_NOT_FOUND  2
#define TX_LOAD_ERROR 4
#define TX_INIT_ERROR 8
#define TX_TEST_ERROR 16

int tmkxload(unsigned port, unsigned char *fpgabuf, int len)
{
  int i;
  int wait;

  if ((inw(READ_PORT) & 0x7FFF) != EMPTY_DEVICE)
    return TX_NOT_FOUND;
  for (i = 0; i < len; ++i)
  {
    if ((inw(READ_PORT) & 0x7FFF) != EMPTY_DEVICE)
      return TX_LOAD_ERROR;
    outw((unsigned short)fpgabuf[i] | 0xFF00, LOAD_PORT);
    wait = 0;
    while ((inw(READ_PORT) & 0x7FFF) == BUSY_DEVICE)
      if (++wait >= 100)
        return TX_LOAD_ERROR;
  }
  for (wait = 0; wait < 100; ++wait) // wait >15 us
    inw(READ_PORT);
  outw(0, RESET_PORT);
  outw(GENER1_BL+GENER2_BL, MODE_PORT);
  if (inw(READ_PORT) != LOADED_DEVICE)
    return TX_INIT_ERROR;
  return TX_OK;
}

#ifdef CONFIG_PCI
int tmkxiload(unsigned portplx, int reset)
{
  unsigned char *fpgabuf;
  int len;
  int res = TX_OK;
  unsigned port;
  int i;
  int fLoad, fWait;
  int cTmk, iTmk;
  TTmkConfig *apTmk[MAX_TMK_NUMBER+1];
  TTmkConfig *pTmk;

  cTmk = 0;
  fLoad = 0;
  for (iTmk = 0; iTmk <= MAX_TMK_NUMBER; ++iTmk)
  {
    pTmk = &aTmkConfig[iTmk];
    if ((unsigned)(pTmk->wHiddenPorts) != portplx ||
        pTmk->fLoaded)
      continue;
    apTmk[cTmk++] = pTmk;
    if (pTmk->nLoadType == LOADTX1)
    {
      port = (unsigned)(pTmk->wPorts1);
      outw(0, RESET_PORT);
      outw(GENER1_BL+GENER2_BL, MODE_PORT);
      if (inw(READ_PORT) != LOADED_DEVICE)
      {
        fLoad = 1;
      }
    }
    else
    {
      fLoad = 1;
    }
  }

  if (fLoad || reset)
  {
    outw(inw(portplx + 0x56) | 0x0010, portplx + 0x56); // M02 = 1
    outw(inw(portplx + 0x52) | 0x4000, portplx + 0x52);  // Reset = 1
    outw(inw(portplx + 0x52) & ~0x4000, portplx + 0x52); // Reset = 0
    res = 0;
    do
    {
      if (++res > 2000)
      {
        outw(inw(portplx + 0x56) & ~0x0010, portplx + 0x56); // M02 = 0
        break;
      }
      fWait = 0;
      for (iTmk = 0; iTmk < cTmk; ++iTmk)
      {
        pTmk = apTmk[iTmk];
        port = (unsigned)(pTmk->wPorts1);
        if ((inw(READ_PORT)&0x7FFF) != EMPTY_DEVICE)
          fWait = 1;
      }
    }
    while (fWait);
    res = 0;
    for (iTmk = 0; iTmk < cTmk; ++iTmk)
    {
      pTmk = apTmk[iTmk];
      pTmk->fLoaded = 1;
      port = (unsigned)(pTmk->wPorts1);
      for (i = 0; i < 100; ++i) // wait 15 us
        inw(READ_PORT);
      switch (pTmk->nLoadType)
      {
      case LOADTX1:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltxi1v11\n");
        #endif
        fpgabuf = fpgabuf5;
        len = sizeof(fpgabuf5);
        break;
#ifdef LOADTX6
      case LOADTX6:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltxi6v11\n");
        #endif
        fpgabuf = fpgabuf6;
        len = sizeof(fpgabuf6);
        break;
#endif //def LOADTX6
#ifdef LOADMR6
      case LOADMR6:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: lmri6v02\n");
        #endif
        fpgabuf = fpgabuf10;
        len = sizeof(fpgabuf10);
        pTmk->nType = MRTXI;
        break;
#endif //def LOADMR6
#ifdef LOADTT6
      case LOADTT6:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltti6v04\n");
        #endif
        fpgabuf = fpgabuf11;
        len = sizeof(fpgabuf11);
        pTmk->nType = TSTXI;
        if (pTmk->SubDID == (ID_TX6PCI >> 16))
        {
          unsigned short wSavL, wSavH;
          wSavL = inw(portplx + 0x3C); //Set 16k increment mode in TE6-PCI
          wSavH = inw(portplx + 0x3E);
          outw( 0x0009, portplx + 0x3C);
          outw(0, portplx + 0x3E);
          outw(0x8000, ADDR_PORT);
          outw(wSavL, portplx + 0x3C);
          outw(wSavH, portplx + 0x3E);
        }
        break;
#endif //def LOADTT6
#ifdef LOADTD6
      case LOADTD6:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltdi6v02\n");
        #endif
        fpgabuf = fpgabuf12;
        len = sizeof(fpgabuf12);
        pTmk->nType = TSTXI;
        if (pTmk->SubDID == (ID_TX6PCI >> 16))
        {
          unsigned short wSavL, wSavH;
          wSavL = inw(portplx + 0x3C); //Set 16k increment mode in TE6-PCI
          wSavH = inw(portplx + 0x3E);
          outw( 0x0009, portplx + 0x3C);
          outw(0, portplx + 0x3E);
          outw(0x8000, ADDR_PORT);
          outw(wSavL, portplx + 0x3C);
          outw(wSavH, portplx + 0x3E);
        }
        break;
#endif //def LOADTD6
#ifdef LOADAM6
      case LOADAM6:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: lami6v02\n");
        #endif
        fpgabuf = fpgabuf14;
        len = sizeof(fpgabuf14);
        pTmk->nType = TSTXI;
        if (pTmk->SubDID == (ID_TX6PCI >> 16))
        {
          unsigned short wSavL, wSavH;
          wSavL = inw(portplx + 0x3C); //Set 16k increment mode in TE6-PCI
          wSavH = inw(portplx + 0x3E);
          outw( 0x0009, portplx + 0x3C);
          outw(0, portplx + 0x3E);
          outw(0x8000, ADDR_PORT);
          outw(wSavL, portplx + 0x3C);
          outw(wSavH, portplx + 0x3E);
        }
        break;
#endif //def LOADAM6
      default:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: default disabled\n");
        #endif
        pTmk->nType = -1;
        continue;
      }
      res |= tmkxload(port, fpgabuf, len);
    }
    outw(inw(portplx + 0x56) & ~0x0010, portplx + 0x56); // M02 = 0
  }
  return res;
}
#endif //def CONFIG_PCI

#ifdef LINUX_SWTIMER
void swtimerall(void)
{
  int fUseEvTimeT, i;

  fUseEvTimeT = 0;
  for (i = 0; i <= nMaxTmkNumber; ++i)
  {
    if (afUseEvTime[i])
    {
      fUseEvTimeT = 1;
      break;
    }
  }
  fUseEvTime = fUseEvTimeT;
  if (fUseEvTime == 0)
    wSwTimerCtrl = 0;
}
#endif //LINUX_SWTIMER

//int tmk1553b_trim(tmk1553b_Dev *dev)
//{
//    tmk1553b_Dev *next, *dptr;
//    int qset = dev->qset;   /* "dev" is not-null */
//    int i;
//
//    for (dptr = dev; dptr; dptr = next) { /* all the list items */
//        if (dptr->data) {
//            for (i = 0; i < qset; i++)
//                if (dptr->data[i])
//                    kfree(dptr->data[i]);
//            kfree(dptr->data);
//            dptr->data=NULL;
//        }
//        next=dptr->next;
//        if (dptr != dev) kfree(dptr); /* all of them but the first */
//    }
//    dev->size = 0;
//    dev->next = NULL;
//    return 0;
//}

#ifdef TMK1553B_DEBUG /* use proc only if debugging */
/*
 * The proc filesystem: function to read and entry
 */

int tmk1553b_read_procmem(char *buf, char **start, off_t offset,
                   int count, int *eof, void *data)
{
    int i, j, len = 0;
    int limit = count - 80; /* Don't print more than this */

    for (i = 0; i < tmk1553b_nr_devs && len <= limit; i++) {
        tmk1553b_Dev *d = &tmk1553b_devices[i];
        if (down_interruptible(&d->sem))
                return -ERESTARTSYS;
        len += sprintf(buf+len,"\nDevice %i: sz %li\n",
                       i, d->size);
        for (; d && len <= limit; d = d->next) { /* scan the list */
            len += sprintf(buf+len, "  item at %p, qset at %p\n", d, d->data);
            if (d->data && !d->next) /* dump only the last item - save space */
                for (j = 0; j < d->qset; j++) {
                    if (d->data[j])
                        len += sprintf(buf+len,"    % 4i: %8p\n",j,d->data[j]);
                }
        }
        up(&tmk1553b_devices[i].sem);
    }
    *eof = 1;
    return len;
}

#ifdef USE_PROC_REGISTER

static int tmk1553b_get_info(char *buf, char **start, off_t offset,
                int len, int unused)
{
    int eof = 0;
    return tmk1553b_read_procmem (buf, start, offset, len, &eof, NULL);
}

struct proc_dir_entry tmk1553b_proc_entry = {
    namelen:    8,
    name:       "tmk1553bmem",
    mode:       S_IFREG | S_IRUGO,
    nlink:      1,
    get_info:   tmk1553b_get_info
};

static void tmk1553b_create_proc()
{
    proc_register_dynamic(&proc_root, &tmk1553b_proc_entry);
}

static void tmk1553b_remove_proc()
{
    proc_unregister(&proc_root, tmk1553b_proc_entry.low_ino);
}

#else  /* no USE_PROC_REGISTER - modern world */

static void tmk1553b_create_proc()
{
    create_proc_read_entry("tmk1553bmem", 0 /* default mode */,
                           NULL /* parent dir */, tmk1553b_read_procmem,
                           NULL /* client data */);
}

static void tmk1553b_remove_proc()
{
    /* no problem if it was not registered */
    remove_proc_entry("tmk1553bmem", NULL /* parent dir */);
}

#endif /* def USE_PROC_REGISTER */

#endif /* def TMK1553B_DEBUG */


/*
 * Open and close
 */

static int tmk1553b_open(struct inode *inode, struct file *filp)
{
//    tmk1553b_Dev *dev; /* device information */
    int num = MINOR(inode->i_rdev);

    TListProc *hlnProc;
    int iTMK, iMBC;

    #ifdef MY_DBG
    printk(MY_KERN_DEBUG "Tmk1553b: Opened!!\n");
    #endif

    /*
     * the num value is only valid if we are not using devfs.
     * However, since we use them to retrieve the device pointer, we
     * don't need them with devfs as filp->private_data is already
     * initialized
     */

    /*
     * If private data is not valid, we are not using devfs
     * so use the num (from minor nr.) to select
     */

    if ((!misc && (num > 0)) || (misc && (num != tmk1553b_mdev.minor)))
      return -ENODEV;

//    if (!filp->private_data || !(dev = (tmk1553b_Dev *)filp->private_data))
//    {
//      if (num >= tmk1553b_nr_devs) return -ENODEV;
//      dev = &tmk1553b_devices[num];
//      filp->private_data = dev; /* for other methods */
//    }

#ifdef LINUX26
    if (!try_module_get(THIS_MODULE))
        return -ENODEV;
#else
    MOD_INC_USE_COUNT;
#endif

    hlnProc = kmalloc(sizeof(TListProc), GFP_KERNEL);
    if (hlnProc == NULL)
      return -EFAULT;

    spin_lock_bh(&tmkSpinLock);

    for (iTMK = 0; iTMK <= nMaxTmkNumber; ++iTMK)
      hlnProc->fTMK[iTMK] = 0;
    hlnProc->hProc = CURRENT_PID;
    hlnProc->nSelectedTMK = -1;
    for (iTMK = 0; iTMK < U32_EVENTS_SIZE; ++iTMK) // iTMK is iEvPck !
    {
      hlnProc->maskTMK[iTMK] = 0;
      hlnProc->maskEvents[iTMK] = 0;
    }
    for (iMBC = 0; iMBC <= MAX_MBC_ID; ++iMBC)
      hlnProc->fMBC[iMBC] = 0;
    init_waitqueue_head(&hlnProc->wq);
    hlnProc->waitFlag = 0;
    list_add_tail(&hlnProc->ProcListEntry, &hlProc);
    hCurProc = hlnProc->hProc;
    hlnCurProc = hlnProc;
    tmkNumber = -1;

    spin_unlock_bh(&tmkSpinLock);

    return 0;          /* success */
}

static int tmk1553b_release(struct inode *inode, struct file *filp)
{
    TListProc *hlnProc;
    int iTMK, iMBC;
    PEPROCESS hProc;

    #ifdef MY_DBG
    printk(MY_KERN_DEBUG "Tmk1553b: Closed!!\n");
    #endif

    hProc = CURRENT_PID;

    spin_lock_bh(&tmkSpinLock);

    for (hlnProc = (TListProc*)(hlProc.next);
         hlnProc != (TListProc*)(&hlProc);
         hlnProc = (TListProc*)(hlnProc->ProcListEntry.next)
        )
    {
      if (hlnProc->hProc != hProc)
        continue;
      for (iTMK = 0; iTMK <= nMaxTmkNumber; ++iTMK)
      {
        if (hlnProc->fTMK[iTMK] == 0)
          continue;
        tmkselect(iTMK);
        tmkreconfig();
        bcreset();
        if (aTMK[iTMK].hEvent)
        {
        //tmkdefevent(0,0);
        //VWIN32_CloseVxDHandle(aTMK[iTMK].hEvent);
//          ObDereferenceObject(aTMK[iTMK].hEvent);
          aTMK[iTMK].hEvent = 0;
        }
        tmkEvents[iTMK>>5] &= ~(1<<(iTMK&0x1F));
        iEvDataBegin[iTMK] = iEvDataEnd[iTMK] = cEvData[iTMK] = cDpcData[iTMK] = 0;
#ifdef LINUX_SWTIMER
        afUseEvTime[iTMK] = 0;
#endif //LINUX_SWTIMER
        hlnProc->fTMK[iTMK] = 0;
        hlnProc->maskTMK[iTMK>>5] &= ~(1<<(iTMK&0x1F));
        aTMK[iTMK].hProc = 0;
        aTMK[iTMK].hlnProc = 0;
      }
#ifdef LINUX_SWTIMER
      swtimerall();
#endif //LINUX_SWTIMER
      for (iMBC = 0; iMBC <= MAX_MBC_ID; ++iMBC)
      {
        if (hlnProc->fMBC[iMBC] == 0)
          continue;
        mbcfree(iMBC);
        hlnProc->fMBC[iMBC] = 0;
      }
      list_del((struct list_head *)hlnProc);
      kfree(hlnProc);
      break;
    }
    hCurProc = 0;
    hlnCurProc = NULL;
    tmkNumber = -1;

    spin_unlock_bh(&tmkSpinLock);

#ifdef LINUX26
    module_put(THIS_MODULE);
#else
    MOD_DEC_USE_COUNT;
#endif
    return 0;
}

static unsigned int tmk1553b_poll(struct file *filp, poll_table *pollp)
{
    PEPROCESS hProc;
    TListProc *hlnProc;
    int iEvPck;
    unsigned int fEvents;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
    if ((poll_requested_events(pollp) & POLLPRI) == 0)
      return 0;
#endif

    hProc = CURRENT_PID;
    spin_lock_bh(&tmkSpinLock);
    if (hCurProc != hProc)
    {
      for (hlnProc = (TListProc*)(hlProc.next);
           hlnProc != (TListProc*)(&hlProc) && hlnProc->hProc != hProc;
           hlnProc = (TListProc*)(hlnProc->ProcListEntry.next)
          );
      if (hlnProc == (TListProc*)(&hlProc))
      {

        spin_unlock_bh(&tmkSpinLock);

        return 0;
      }
//      hCurProc = hProc;
//      hlnCurProc = hlnProc;
    }
    else
      hlnProc = hlnCurProc;
    fEvents = 0;
    for (iEvPck = 0; iEvPck < U32_EVENTS_SIZE; ++iEvPck)
    {
      if (tmkEvents[iEvPck] & hlnProc->maskTMK[iEvPck])
      {
        fEvents = POLLPRI;
        // if tmkEvents were set during active poll then maskEvents should
        // have already been cleared during interrupt processing
        break;
      }
    }

    if (!fEvents)
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
  #if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
  if (poll_requested_events(pollp) == 0)
  #else
  if (poll_does_not_wait(pollp))
  #endif
  {
    memset(hlnProc->maskEvents, 0, sizeof(hlnProc->maskEvents));
  }
  else
#endif
      {
        poll_wait(filp, &(hlnProc->wq), pollp);
        for (iEvPck = 0; iEvPck < U32_EVENTS_SIZE; ++iEvPck)
        {
          hlnProc->maskEvents[iEvPck] = hlnProc->maskTMK[iEvPck];
        }
        // as far as there could be no any finishing poll call in case of a
        // third party event or timeout so maskEvents may remain set causing
        // unneeded single wake up of already running process from interrupt;
        // we may add extra clearing of maskEvents in tmkwaitevents and
        // tmkwaiteventsm at the beginning and request they always to be
        // called after poll/select but the wake up can happen before such
        // a call, so that wouldn't give 100% solution, so useless
      }
    }
//    hlnCurProc = hlnProc;
//    hCurProc = hProc;
//    tmkselect(tmkNumber = hlnCurProc->nSelectedTMK);
    spin_unlock_bh(&tmkSpinLock);

    return fEvents;
}

/*
 * The ioctl() implementation
 *
 * This is done twice, once the 2.2 way, followed by the 2.0 way.  One
 * would not normally do things in this manner, but we wanted to illustrate
 * both ways...
 */

#ifndef LINUX_20

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
static long tmk1553b_uioctl(struct file *filp,
                 unsigned int cmd, unsigned long arg)
#else
static int tmk1553b_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
#endif
{
//#if (MAX_VTMK_NUMBER+1) > 64
//#define AWBUF_SIZE (MAX_VTMK_NUMBER+1)
//#else
//#define AWBUF_SIZE 64
//#endif
#define PWBUF_SIZE (MAX_VTMK_NUMBER+1)
#define AWBUF_SIZE 64

    int err = 0;
    UINT dwService;
    UINT dwRetVal = 0;
    PEPROCESS hProc;
    TListProc *hlnProc;
    USHORT awBuf[AWBUF_SIZE];
    USHORT awIn[5];
    USHORT awOut[6];
    USHORT *pwBuf = awBuf;
    USHORT *ptr=NULL;
    UINT   *pbuftodev=NULL;
    UINT   *pbuffromdev=NULL;
    UINT   *pusertodev=NULL;
    UINT   *puserfromdev=NULL;
    UINT   dwLenToDev = 0;
    UINT   dwLenFromDev = 0;
    UINT   dwLenToBuf = 0;
    int    fBuf = 0;
    int    fBufFrom = 0;
    int    fBufTo = 0;
    /*
     * extract the type and number bitfields, and don't decode
     * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
     */
    if (_IOC_TYPE(cmd) != TMK_IOC_MAGIC) return -ENOTTY;
    dwService = _IOC_NR(cmd);
    if (dwService > MAX_TMK_API /*||
        aCmds[dwService] != cmd*/) return -ENOTTY;
    {
      /*
       * the direction is a bitmask, and VERIFY_WRITE catches R/W
       * transfers. `Type' is user-oriented, while
       * access_ok is kernel-oriented, so the concept of "read" and
       * "write" is reversed
       */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0) || defined(TMK1553B_TWOARGACCOK)
      if (_IOC_DIR(cmd) & (_IOC_READ | _IOC_WRITE))
        err = !access_ok((void *)arg, _IOC_SIZE(cmd));
#else
      if (_IOC_DIR(cmd) & _IOC_READ) // VERIFY_WRITE covers VERIFY_READ as well
        err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
      else if (_IOC_DIR(cmd) & _IOC_WRITE)
        err = !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
#endif
      if (err) return -EFAULT;

//      printk(MY_KERN_INFO "Tmk1553b: cmd=%u srv=%u size=%d\n", cmd, dwService, _IOC_SIZE(cmd));
      if (_IOC_DIR(cmd) & _IOC_WRITE)
      {
        if (_IOC_SIZE(cmd) == sizeof(u32))
        {
          __get_user(*((u32*)awIn), (u32*)arg);
        }
        else if (_IOC_SIZE(cmd) == sizeof(u32[2]))
        {
          __get_user(*((u32*)awIn), (u32*)arg);
          __get_user(*((u32*)awIn+1), (u32*)arg+1);
          // use 32bit ptr on 32bit system or convert it to 64bit ptr on
          // 64bit system for backward compatibility with pre4.02 programs
          // can fail on 64bit system with addresses above 4G!!!
          ptr = (PUSHORT)((unsigned long)(*((u32*)awIn+1))); // not needed for:
          // tmkwaitevents, tmkwaiteventsflag, mbcpreparex,
          // bcstartctla, bcdefctla, mtfilter, rtlegal;
          // these are funcs using u[32]/i[32] direct parameters, not pointers
        }
#ifdef __64BIT__
        else if (_IOC_SIZE(cmd) == sizeof(u64[2]))
        {
          if (dwService == VTMK_tmkrawion) // size == sizeof(u32[4]) tmkrawio 32 bit
          {
            __get_user(*((u32*)awIn), (u32*)arg);
            __get_user(*((u32*)awIn+1), (u32*)arg+1);
            // convert 32bit ptr to 64bit ptr on 64bit system
            // can fail on 64bit system with addresses above 4G!!!
            __get_user(*((u32*)awOut), (u32*)arg+2);
            pusertodev = (PUINT)((unsigned long)(*((u32*)awOut)));
            __get_user(*((u32*)awOut), (u32*)arg+3);
            puserfromdev = (PUINT)((unsigned long)(*((u32*)awOut)));
          }
          else
          {
            __get_user(*((u32*)awIn), (u32*)arg);
            *((u32*)awIn+1) = 0; // not needed for put/get
            __get_user(ptr, (PUSHORT*)arg+1);
          }
        }
        else if (_IOC_SIZE(cmd) == sizeof(u64[4])) // tmkrawio 64 bit
        {
          __get_user(*((u32*)awIn), (u32*)arg);
          __get_user(*((u32*)awIn+1), (u32*)arg+2);
          __get_user(pusertodev, (PUINT*)arg+2);
          __get_user(puserfromdev, (PUINT*)arg+3);
        }
#else
        else if (_IOC_SIZE(cmd) == sizeof(u32[4])) // tmkrawio 32 bit
        {
          __get_user(*((u32*)awIn), (u32*)arg);
          __get_user(*((u32*)awIn+1), (u32*)arg+1);
          __get_user(pusertodev, (PUINT*)arg+2);
          __get_user(puserfromdev, (PUINT*)arg+3);
        }
#endif
        else
          return -EFAULT;
      }
      else // if ((IOC_DIR(cmd) & IOC_READ) == 0)
        *((UINT*)awIn) = (UINT)arg;

      hProc = CURRENT_PID;

      switch (dwService)
      {
#ifdef TMK1553B_DEBUG
        case tmk1553b_IOCHARDRESET:
           /*
            * reset the counter to 1, to allow unloading in case
            * of problems. Use 1, not 0, because the invoking
            * process has the device open.
            */
//           while (MOD_IN_USE)
//               MOD_DEC_USE_COUNT;
//           MOD_INC_USE_COUNT;
           /* don't break: fall through and reset things */
           break;
#endif /* TMK1553B_DEBUG */
      case VTMK_bcputblk:
      case VTMK_mtputblk:
      case VTMK_rtputblk:
      case VTMK_rtputblkifnb:
        if (awIn[1] > 64 || awIn[1] == 0)
          break;
        if (copy_from_user(
              awBuf,
              ptr,
              awIn[1]<<1
              ))
          return -EFAULT;
        break;
      case VTMK_rtputflags:
        awIn[4] = awIn[0] & RT_DIR_MASK;
        *((PUINT)(awIn)) &= ~(RT_DIR_MASK | (RT_DIR_MASK<<16));
        if (awIn[1] < awIn[0] || awIn[1] > 31)
          break;
        if (copy_from_user(
              awBuf,
              ptr,
              (awIn[1]-awIn[0]+1)<<1
              ))
          return -EFAULT;
        break;
      case VTMK_rtgetflags:
        awIn[4] = awIn[0] & RT_DIR_MASK;
        *((PUINT)(awIn)) &= ~(RT_DIR_MASK | (RT_DIR_MASK<<16));
        break;
      case VTMK_tmkwaiteventsflag:
        hProc = *((PUINT)awIn);
        break;
      case VTMK_tmkwaiteventsm:
        if (awIn[0] > (MAX_VTMK_NUMBER+1))
          return -EFAULT;
        if (awIn[0] == 0)
          break;
        if (awIn[0] > AWBUF_SIZE)
        {
          pwBuf = kmalloc(PWBUF_SIZE*2, GFP_KERNEL);
          if (pwBuf == NULL)
            return -EFAULT;
          fBuf = 1;
        }
        if (copy_from_user(
              pwBuf,
              ptr,
              awIn[0]<<1
              ))
        {
          if (fBuf)
            kfree(pwBuf);
          return -EFAULT;
        }
        break;
      case VTMK_tmkrawion:
        dwLenToDev = *((u32*)awIn);
        if (pusertodev == NULL)
          dwLenToDev = 0;
        dwLenFromDev = *((u32*)awIn+1);
        if (puserfromdev == NULL)
          dwLenFromDev = 0;
        dwLenToBuf = 0;
        if (dwLenFromDev)
        {
          // actually access_ok are redundant for copy_from/to_user
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0) || defined(TMK1553B_TWOARGACCOK)
          if (!access_ok((void *)puserfromdev, dwLenFromDev*sizeof(UINT)))
            return -EFAULT;
#else
          if (!access_ok(VERIFY_WRITE, (void *)puserfromdev, dwLenFromDev*sizeof(UINT)))
            return -EFAULT;
#endif
        }
        if (dwLenToDev)
        {
          // actually access_ok are redundant for copy_from/to_user
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,0,0) || defined(TMK1553B_TWOARGACCOK)
          if (!access_ok((void *)pusertodev, dwLenToDev*sizeof(UINT)))
            return -EFAULT;
#else
          if (!access_ok(VERIFY_READ, (void *)pusertodev, dwLenToDev*sizeof(UINT)))
            return -EFAULT;
#endif
          if (dwLenToDev <= ((AWBUF_SIZE*2)/sizeof(UINT)))
          {
            pbuftodev = (PUINT)awBuf;
            dwLenToBuf = dwLenToDev;
          }
          else
          {
            pbuftodev = kmalloc(dwLenToDev*sizeof(UINT), GFP_KERNEL);
            if (pbuftodev == NULL)
              return -EFAULT;
            fBufTo = 1;
          }
          if (copy_from_user(
                pbuftodev,
                pusertodev,
                dwLenToDev*sizeof(UINT)
                ))
          {
            if (fBufTo)
              kfree(pbuftodev);
            return -EFAULT;
          }
        }
        if (dwLenFromDev)
        {
          if ((dwLenFromDev + dwLenToBuf) <= ((AWBUF_SIZE*2)/sizeof(UINT)))
          {
            pbuffromdev = (PUINT)awBuf+dwLenToBuf;
          }
          else
          {
            pbuffromdev = kmalloc(dwLenFromDev*sizeof(UINT), GFP_KERNEL);
            if (pbuffromdev == NULL)
            {
              if (fBufTo)
                kfree(pbuftodev);
              return -EFAULT;
            }
            fBufFrom = 1;
          }
        }
        break;
      }

#ifdef LINUX26
//      #ifdef MY_DBG
//{
//      printk(MY_KERN_DEBUG "Tmk1553b: pid=%d parentpid=%d rparentpid=%d leaderpid=%d srv=%d.\n",
//        current->pid, current->parent->pid, current->real_parent->pid, current->group_leader->pid, dwService);
//      unsigned long jjj;
/*
      jjj = jiffies + HZ;
      while (jiffies < jjj)
        schedule();
*/
//}
//      #endif
#endif

      spin_lock_bh(&tmkSpinLock);

      lpwIn = (PUSHORT)&awIn;
      lpwOut = (PUSHORT)&awOut;
      lpwBuf = (PUSHORT)pwBuf;

      if (dwService == VTMK_tmkrawion)
      {
        dwUserRawLenO = dwLenToDev;
        dwUserRawLenI = dwLenFromDev;
        pdwUserRawBufOO = pbuftodev;
        pdwUserRawBufII = pbuffromdev;
      }

      if (hCurProc != hProc)
      {
        for (hlnProc = (TListProc*)(hlProc.next);
             hlnProc != (TListProc*)(&hlProc) && hlnProc->hProc != hProc;
             hlnProc = (TListProc*)(hlnProc->ProcListEntry.next)
            );
        if (hlnProc == (TListProc*)(&hlProc))
        {

          spin_unlock_bh(&tmkSpinLock);

          if (fBufFrom)
            kfree(pbuffromdev);
          if (fBufTo)
            kfree(pbuftodev);
          if (fBuf)
            kfree(pwBuf);
          return -EFAULT;
        }
        hCurProc = hProc;
        hlnCurProc = hlnProc;
        tmkselect(tmkNumber = hlnCurProc->nSelectedTMK);
      }

      tmkError = 0;
//      err = 0;
      dwRetVal = (TMK_Procs[dwService-2])();
      err = tmkError;

      spin_unlock_bh(&tmkSpinLock);

//    if (err == 0 || dwService == VTMK_tmkwaiteventsm || dwService == VTMK_tmkrawion)
    {
      switch (dwService)
      {
      case VTMK_rtgetblkifnb:
        if (dwRetVal)
          break;
        fallthrough;
        // fall-through
      case VTMK_bcgetblk:
      case VTMK_mtgetblk:
      case VTMK_rtgetblk:
        if (awIn[1] > 64 || awIn[1] == 0)
          break;
        if (copy_to_user(
              ptr,
              awBuf,
              awIn[1]<<1
              ))
          return -EFAULT;
        break;
      case VTMK_rtgetflags:
        if (awIn[1] < awIn[0] || awIn[1] > 31)
          break;
        if (copy_to_user(
              ptr,
              awBuf,
              (awIn[1]-awIn[0]+1)<<1
              ))
          return -EFAULT;
        break;
      case VTMK_tmkgetinfo:
        if (copy_to_user(
              (TTmkConfigData*)arg,
              awBuf,
              sizeof(TTmkConfigData)
              ))
          return -EFAULT;
        break;
      case VTMK_tmkgetevd:
        if (copy_to_user(
              (PUSHORT)arg,
              awOut,
              12
              ))
          return -EFAULT;
        break;
      case VTMK_tmkwaiteventsm:
        if (awIn[0] > (MAX_VTMK_NUMBER+1))
          return -EFAULT;
        if (awIn[0] == 0)
          break;
        if (copy_to_user(
              ptr,
              pwBuf,
              awIn[0]<<1
              ))
        {
          if (fBuf)
            kfree(pwBuf);
          return -EFAULT;
        }
        if (fBuf)
        {
          kfree(pwBuf);
//          fBuf = 0;
        }
        break;
      case VTMK_tmkrawion:
        if (dwLenFromDev)
        {
          if (copy_to_user(
                puserfromdev,
                pbuffromdev,
                dwLenFromDev*sizeof(UINT)
                ))
          {
            if (fBufFrom)
              kfree(pbuffromdev);
            if (fBufTo)
              kfree(pbuftodev);
            return -EFAULT;
          }
          if (fBufFrom)
          {
            kfree(pbuffromdev);
//            fBufFrom = 0;
          }
        }
        if (fBufTo)
        {
          kfree(pbuftodev);
//          fBufTo = 0;
        }
        break;
      default:
        if (_IOC_DIR(cmd) & _IOC_READ)
        {
          __put_user(*((u32*)awOut), (u32*)arg);
/*
          if (dwService == VTMK_tmkgetevd) // _IOC_SIZE(cmd) == 12
          {
            __put_user(*((u32*)awOut+1), (u32*)arg+1);
            __put_user(*((u32*)awOut+2), (u32*)arg+2);
          }
*/
        }
//        else
//          dwRetVal = (UINT)awOut[0];
        break;
      }
    }

      if (err != 0)
        err = TMK_BAD_0 - err;
      else
        err = dwRetVal; // has to be > 0 for Ok and < 0 for error !!!
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
    return (long)err;
#else
    return err;
#endif
}

#else  /* LINUX_20 */

change !acces_ok -> verify_area
change __get_user -> get_user
change __put_user -> put_user
change copy_to_from_user -> memcpy_tofs_fromfs

#endif /* LINUX_20 */

/*
 * The following wrappers are meant to make things work with 2.0 kernels
 */
#ifdef LINUX_20
static void tmk1553b_release_20(struct inode *ino, struct file *f)
{
    tmk1553b_release(ino, f);
}

/* Redefine "real" names to the 2.0 ones */
#define tmk1553b_release tmk1553b_release_20
#endif  /* LINUX_20 */

static struct file_operations tmk1553b_fops = {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0)
    owner:      THIS_MODULE,
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
    unlocked_ioctl:      tmk1553b_uioctl,
#else
    ioctl:      tmk1553b_ioctl,
#endif
    poll:       tmk1553b_poll,
    open:       tmk1553b_open,
    release:    tmk1553b_release
};

/*
 * Finally, the module stuff
 */

#ifdef MY_CONFIG_DEVFS_FS
devfs_handle_t tmk1553b_devfs_dir;
static char devname[4];
#endif

/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
 */
static void tmk1553b_cleanup_module(void)
{
//    int i;
  int iTMK, jTMK;
  int fReleaseHiddenPorts;
  TIrq *pIrq;
  int hIrq;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
  int i;
#endif

  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "Tmk1553b: Unloading!!\n");
  #endif

  //
  // Disconnect the interrupt and region
  //

/*
  if (fTMKInit)
  {
    tmkdone(ALL_TMKS);
    List_Destroy(hlProc);
  }
*/

  fTMKInit = 0;
  tmkdone(ALL_TMKS);
//what if irq?

  for (hIrq = 0; hIrq < cIrqs; ++hIrq)
  {
    pIrq = ahIrq + hIrq;
    free_irq(pIrq->nIrq, tmkInterruptServiceRoutine + hIrq);
    pIrq->cTmks = 0;
  }
  cIrqs = 0;

  for (iTMK = 0; iTMK <= MAX_TMK_NUMBER; ++iTMK)
  {
    if (aTmkConfig[iTMK].nType == -1)
      continue;

    release_region(aTmkConfig[iTMK].wPorts1, aTmkConfig[iTMK].nPorts);

    if (aTmkConfig[iTMK].wPorts2 != 0xFFFF &&
        aTmkConfig[iTMK].wPorts2 != aTmkConfig[iTMK].wPorts1)
      release_region(aTmkConfig[iTMK].wPorts2, aTmkConfig[iTMK].nPorts);

    if ((aTmkConfig[iTMK].nType == TMKXI || aTmkConfig[iTMK].nType == TAI ||
         aTmkConfig[iTMK].nType == MRTXI || aTmkConfig[iTMK].nType == MRTAI ||
         aTmkConfig[iTMK].nType == TSTXI || aTmkConfig[iTMK].nType == TSTAI) &&
        (aTmkConfig[iTMK].wHiddenPorts != 0) &&
        (aTmkConfig[iTMK].wHiddenPorts != 0xFFFF))
    {
      fReleaseHiddenPorts = 1;
      for (jTMK = 0; jTMK < iTMK; ++jTMK)
      {
        if (aTmkConfig[jTMK].nType != TMKXI && aTmkConfig[jTMK].nType != TAI &&
            aTmkConfig[jTMK].nType != MRTXI && aTmkConfig[jTMK].nType != MRTAI &&
            aTmkConfig[jTMK].nType != TSTXI && aTmkConfig[jTMK].nType != TSTAI)
          continue;
        if (aTmkConfig[iTMK].wHiddenPorts == aTmkConfig[jTMK].wHiddenPorts)
        {
          fReleaseHiddenPorts = 0;
          break;
        }
      }
      if (fReleaseHiddenPorts)
        release_region(aTmkConfig[iTMK].wHiddenPorts, 128);
    }

//    aTmkConfig[iTMK].nType = -1;
  }

  for (iTMK = 0; iTMK <= MAX_TMK_NUMBER; ++iTMK)
  {
    aTmkConfig[iTMK].nType = -1;
  }

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
  if (fpciDevs)
    for (i = 0; i < cpciDevs; ++i)
      pci_dev_put(apciDevs[i]);
#endif
  cpciDevs = 0;
  fpciDevs = 0;

#ifndef MY_CONFIG_DEVFS_FS
    /* cleanup_module is never called if registering failed */
    if (!misc)
    {
      unregister_chrdev(major, name);
      printk(MY_KERN_INFO "tmk1553b: Unregistered device %s, major=%d\n", name, major);
    }
    else
    {
      misc_deregister(&tmk1553b_mdev);
      printk(MY_KERN_INFO "tmk1553b: Unregistered dynamic misc device %s, minor=%d\n", name, tmk1553b_mdev.minor);
    }
#endif

#ifdef TMK1553B_DEBUG /* use proc only if debugging */
    tmk1553b_remove_proc();
#endif

#ifdef MY_CONFIG_DEVFS_FS
    if (tmk1553b_devices) {
        for (i=0; i<tmk1553b_nr_devs; i++) {
// here was ...trim
            // the following line is only used for devfs
            devfs_unregister(tmk1553b_devices[i].handle);
        }
        kfree(tmk1553b_devices);
    }
    // once again, only for devfs
    devfs_unregister(tmk1553b_devfs_dir);
#endif

}

char *apszTmkTypeName[MAX_TMK_TYPE+1] =
                                       {"",
                                        "",
                                        "TMK400",
                                        "TMKMPC",
                                        "RTMK400",
                                        "TMKX",
                                        "TMKXI",
                                        "",
                                        "",
                                        "TA",
                                        "TAI",
                                        "MRTA",
                                        "MRTAI",
                                        "",
                                        "",
                                        "",
                                        ""};

char *apszTmkLoadName[MAX_LOAD_TYPE+1] = {
                            "TX1" // default for TMKX, always on
,
#ifdef LOADRT1
                            "RT1"
#else
""
#endif //def LOADRT1
,
#ifdef LOADTX4
                            "TX4"
#else
""
#endif //def LOADTX4
,
#ifdef LOADTX5
                            "TX5"
#else
""
#endif //def LOADTX5
,
#ifdef LOADTX6
                            "TX6"
#else
""
#endif //def LOADTX6
,
#ifdef LOADMR6
                            "MR6"
#else
""
#endif //def LOADMR6
,
#ifdef LOADTT6
                            "TT6"
#else
""
#endif //def LOADTT6
,
#ifdef LOADTD6
                            "TD6"
#else
""
#endif //def LOADTD6
,
#ifdef LOADAM6
                            "AM6"
#else
""
#endif //def LOADAM6
,
                            "MRA" // default for MRTA, always on
,
#ifdef LOADTRA
                            "TRA"
#else
""
#endif //def LOADTRA
                            };

#ifdef CONFIG_PCI
int pciCmpBusDevFun(struct pci_dev *pciDev1, struct pci_dev *pciDev2)
{
  if (pciDev1->bus->number == pciDev2->bus->number)
    return (pciDev1->devfn < pciDev2->devfn) ? -1 : 1;
  else
    return (pciDev1->bus->number < pciDev2->bus->number) ? -1 : 1;
}
#endif

int RegInit(int hTMK, UINT tmkPorts1, UINT tmkPorts2, UINT tmkIrq1, int tmkDev, int tmkDevExt, char *pszType, char *pszLoad)
{
  int tmkType, tmkLoad = 0;
  int iPciDeviceID;
  struct pci_dev *pciDev = NULL;
  u32 pciSubSystemID, tmkSubSystemID = 0;
  u32 tmkSystemID = 0;
  int tmkDevExt1;
  UINT tmkHiddenPorts = 0;
  UINT tmkLocalReadInt = 0;
  int i, j;
  int found;
  u16 tmkCfgCmd;
  u16 tmkDeviceID = 0;
  int nTmkPorts = 0;
  int tmkAutoPCI;

  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "tmk1553b: RegInit %d %s\n", hTMK, (pszType == 0) ? "0" : (pszType == (char*)1) ? "autopci" : pszType);
  #endif
  if ((sizeof(apszTmkTypeName)/sizeof(char*)) != (MAX_TMK_TYPE+1))
  {
    printk(MY_KERN_WARNING "tmk1553b: tmk type array error!\n");
    return -1;
  }
  if ((sizeof(apszTmkLoadName)/sizeof(char*)) != (MAX_LOAD_TYPE+1))
  {
    printk(MY_KERN_WARNING "tmk1553b: load type array error!\n");
    return -1;
  }

  if (pszType == 0)
    return 0;
  if (pszType == (char*)1)
  {
    tmkAutoPCI = 1;
    tmkType = -1;
    pszType = "autopci";
  }
  else
  {
    tmkAutoPCI = 0;
    aManCfgDrv[hTMK] = 1; // mark drv number as not used for autopci
    for (tmkType = MIN_TMK_TYPE; tmkType <= MAX_TMK_TYPE; ++tmkType)
    {
      if (strcmp(pszType, apszTmkTypeName[tmkType]) == 0)
        break;
    }
    if (tmkType > MAX_TMK_TYPE)
    {
      printk(MY_KERN_WARNING "tmk1553b: device type \"%s\" is incorrect!\n", pszType);
      return -1;
    }
  }
  switch (tmkType)
  {
  case TMK400:
    if (tmkPorts2 == 0xFFFF)
    {
      printk(MY_KERN_WARNING "tmk1553b: port address #2 isn't specified for TMK%d!\n", hTMK);
      return -1;
    }
    fallthrough;
    // fall-through
  case RTMK400:
  case TMKMPC:
  case TMKX:
  case TA:
  case MRTA:
    if (tmkPorts1 == 0xFFFF || tmkIrq1 == 0xFF)
    {
      printk(MY_KERN_WARNING "tmk1553b: port address or irq line aren't specified for TMK%d!\n", hTMK);
      return -1;
    }
    break;
#ifdef CONFIG_PCI
  case TMKXI:
  case TAI:
  case MRTAI:
  case -1: // autopci,autotype
    if (tmkDev == 0)
      tmkDev = 1;
    if (tmkDevExt == 0)
      tmkDevExt = 1;
    if (tmkDevExt > 4)
    {
      printk(MY_KERN_WARNING "tmk1553b: wrong PCI subdevice number %d.%d!\n", tmkDev, tmkDevExt);
      return -1;
    }
#ifndef LINUX26
    if (!pcibios_present())
    {
      printk(MY_KERN_WARNING "tmk1553b: can't find PCI BIOS!\n");
      return -1;
    }
#endif
    if (fpciDevs == 0)
    {
      fpciDevs = 1;
      cpciDevs = 0;
      for (iPciDeviceID = 0; iPciDeviceID <= TMK_MAX_IDID; ++iPciDeviceID)
      {
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "tmk1553b: iID %d %X %X\n", iPciDeviceID, aPciVendorID[iPciDeviceID], aPciDeviceID[iPciDeviceID]);
        #endif
        pciDev = NULL;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
        while ((pciDev = pci_get_device(aPciVendorID[iPciDeviceID], aPciDeviceID[iPciDeviceID], pciDev)) != NULL)
#else
        while ((pciDev = pci_find_device(aPciVendorID[iPciDeviceID], aPciDeviceID[iPciDeviceID], pciDev)) != NULL)
#endif
        {
          pci_read_config_dword(pciDev, CFG_SUBSYSTEM_ID, &pciSubSystemID);
          if (pciSubSystemID != ID_TX1PCI
              && pciSubSystemID != ID_TX1PCI2
#ifdef LOADTX6
              && pciSubSystemID != ID_TX6PCI
              && pciSubSystemID != ID_TX6PCI2
#endif //def LOADTX6
              && pciSubSystemID != ID_TA1PCI
              && pciSubSystemID != ID_TA1PCI4
              && pciSubSystemID != ID_TA1PCI32RT
              && pciSubSystemID != ID_TA1PE2
              && pciSubSystemID != ID_TA13U2R
              && pciSubSystemID != ID_TA1PE4
              && pciSubSystemID != ID_TA1PE32RT
              && pciSubSystemID != ID_TA1MPE2
              && pciSubSystemID != ID_TA1NPCI4
              && pciSubSystemID != ID_TA1NPE4
//              && pciSubSystemID != ID_TA1NNPE4
              && pciSubSystemID != ID_TA2PE4
              && pciSubSystemID != ID_TA1P
              && pciSubSystemID != ID_TA1P32RT
              && pciSubSystemID != ID_TA1P32RTE
              && pciSubSystemID != ID_TA1P32RTF
              && pciSubSystemID != ID_TA1P32RTV
//              && pciSubSystemID != ID_TA1PA708
//              && pciSubSystemID != ID_TA1PE4A708
              && pciSubSystemID != ID_TA1PE432RT
              && pciSubSystemID != ID_TA1PE432RTE
              && pciSubSystemID != ID_TA1PE432RTF
              && pciSubSystemID != ID_TA1BPE
              && pciSubSystemID != ID_TA1BPE32RT
              && pciSubSystemID != ID_TA1BPE32RTE
              && pciSubSystemID != ID_TA1BPE32RTF
              && pciSubSystemID != ID_TA1BPE32RTV
//              && pciSubSystemID != ID_TA1BPEA708
              && pciSubSystemID != ID_TA2P
              && pciSubSystemID != ID_TA2P32RT
              && pciSubSystemID != ID_TA2P32RTE
              && pciSubSystemID != ID_TA2P32RTF
              && pciSubSystemID != ID_TA2P32RTV
//              && pciSubSystemID != ID_TA2PA708
//              && pciSubSystemID != ID_TA2PE4A708
              && pciSubSystemID != ID_TA2PE432RT
              && pciSubSystemID != ID_TA2PE432RTE
              && pciSubSystemID != ID_TA2PE432RTF
              && pciSubSystemID != ID_TA2BPE
              && pciSubSystemID != ID_TA2BPE32RT
              && pciSubSystemID != ID_TA2BPE32RTE
              && pciSubSystemID != ID_TA2BPE32RTF
              && pciSubSystemID != ID_TA2BPE32RTV
//              && pciSubSystemID != ID_TA2BPEA708
             )
          {
            continue;
          }
          #ifdef MY_DBG
          printk(MY_KERN_DEBUG "tmk1553b: found iID %d\n", iPciDeviceID);
          #endif
          for (i = 0; i < cpciDevs; ++i)
          {
            if (pciCmpBusDevFun(pciDev, apciDevs[i]) < 0)
              break;
          }
          if (cpciDevs < MAX_PCI_SLOTS)
          {
            ++cpciDevs;
          }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
          else if (i < cpciDevs)
          {
            pci_dev_put(apciDevs[cpciDevs-1]); // drop the last excess device
          }
#endif
          #ifdef MY_DBG
          printk(MY_KERN_DEBUG "tmk1553b: cpciDevs= %d\n", cpciDevs);
          #endif
          for (j = cpciDevs - 1; j > i; --j)
          {
            apciDevs[j] = apciDevs[j-1];
          }
          if (i < cpciDevs)
          {
            apciDevs[i] = pciDev;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
            pci_dev_get(pciDev); //add the available device
#endif
          }
        } // while pci_get/find
      } // for iPciDeviceID
    } // if !fpciDevs

    pciDev = NULL;
    if (tmkDev <= cpciDevs)
      pciDev = apciDevs[tmkDev - 1];

//#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
//    for (i = 0; i < cpciDevs; ++i)
//    {
//      if (pciDev != apciDevs[i])
//        pci_dev_put(apciDevs[i]); moved to cleanup_module
//    }
//#endif

    if (pciDev != NULL)
    {
      pci_read_config_dword(pciDev, CFG_SYSTEM_ID, &tmkSystemID);
      tmkDeviceID = (u16)(tmkSystemID >> 16);
      pci_read_config_dword(pciDev, CFG_SUBSYSTEM_ID, &tmkSubSystemID);
      if (tmkType == -1)
      {
        switch (tmkSubSystemID)
        {
        case ID_TX1PCI:
        case ID_TX1PCI2:
  #ifdef LOADTX6
        case ID_TX6PCI:
        case ID_TX6PCI2:
  #endif //def LOADTX6
          tmkType = TMKXI;
          break;
        case ID_TA1PCI:
        case ID_TA1PCI4:
        case ID_TA1PE2:
        case ID_TA13U2R:
        case ID_TA1PE4:
        case ID_TA1MPE2:
        case ID_TA1NPCI4:
        case ID_TA1NPE4:
  //      case ID_TA1NNPE4:
        case ID_TA2PE4:
        case ID_TA1P:
        case ID_TA1BPE:
        case ID_TA2P:
        case ID_TA2BPE:
          tmkType = TAI;
          break;
        case ID_TA1PCI32RT:
        case ID_TA1PE32RT:
        case ID_TA1P32RT:
        case ID_TA1P32RTE:
        case ID_TA1P32RTF:
        case ID_TA1P32RTV:
        case ID_TA1PE432RT:
        case ID_TA1PE432RTE:
        case ID_TA1PE432RTF:
        case ID_TA1BPE32RT:
        case ID_TA1BPE32RTE:
        case ID_TA1BPE32RTF:
        case ID_TA1BPE32RTV:
        case ID_TA2P32RT:
        case ID_TA2P32RTE:
        case ID_TA2P32RTF:
        case ID_TA2P32RTV:
        case ID_TA2PE432RT:
        case ID_TA2PE432RTE:
        case ID_TA2PE432RTF:
        case ID_TA2BPE32RT:
        case ID_TA2BPE32RTE:
        case ID_TA2BPE32RTF:
        case ID_TA2BPE32RTV:
          tmkType = MRTAI;
          break;
  //      case ID_TA1PA708:
  //      case ID_TA1PE4A708:
  //      case ID_TA1BPEA708:
  //      case ID_TA2PA708:
  //      case ID_TA2PE4A708:
  //      case ID_TA2BPEA708:
  //        tmkType = TSTAI;
  //        break;
        }
      }
      if (aManCfgPci[tmkDev-1] == 0 && pci_enable_device(pciDev)) // enable only once
      {
        printk(MY_KERN_WARNING "tmk1553b: can't enable specified PCI device %d!\n", tmkDev);
//#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
//        pci_dev_put(pciDev); moved to cleanup_module
//#endif
        return -1;
      }
      aManCfgPci[tmkDev-1] = (tmkAutoPCI) ? -1 : 1; // mark pci dev number as used / not used by autopci
      tmkDevExt1 = tmkDevExt;
      if (tmkSubSystemID == ID_TA1PE4)
        tmkDevExt1 = ((tmkDevExt1 - 1) >> 1) + 1;
      pci_read_config_dword(pciDev, CFG_ADDR1+(tmkDevExt1<<2), &tmkPorts1);
      tmkPorts1 &= 0x0000FFFE;
      if (tmkPorts1 == 0)
      {
        if (!tmkAutoPCI)
          printk(MY_KERN_WARNING "tmk1553b: can't find specified PCI subdevice %d.%d!\n", tmkDev, tmkDevExt);
//#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
//        pci_dev_put(pciDev); moved to cleanup_module
//#endif
        return -1;
      }
      switch (tmkDeviceID)
      {
      case PLX_DID0: // PCI_9030
      case PLX_DID1: // PE_9056
        pci_read_config_dword(pciDev, CFG_ADDR1, &tmkHiddenPorts);
        tmkHiddenPorts &= 0x0000FFFE;
        break;
      default:
        tmkHiddenPorts = 0xFFFF;
        break;
      }
//          pci_read_config_byte(pciDev, CFG_IRQ, &tmkIrq1);
//          tmkIrq1 &= 0x000000FF;
      tmkIrq1 = pciDev->irq;
//          pciWriteConfigWord(wBusDevFun, CFG_ADDR2, wPort | 1);
      if (tmkSubSystemID == ID_TX1PCI2 || tmkSubSystemID == ID_TX6PCI2)
        tmkLocalReadInt = 1;
      else
        tmkLocalReadInt = 0;
      switch (tmkDeviceID)
      {
      case PLX_DID0: // PCI_9030
      case PLX_DID1: // PE_9056
        pci_read_config_word(pciDev, CFG_COMMAND, &tmkCfgCmd);
        pci_write_config_word(pciDev, CFG_COMMAND, tmkCfgCmd | 0x0001); // | 0x0003);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
        if (check_region(tmkHiddenPorts, 128))
#else
        if (!request_region(tmkHiddenPorts, 128, name))
#endif
        {
          printk(MY_KERN_WARNING "tmk1553b: can't get hidden I/O port address 0x%x for TMK%d!\n", tmkHiddenPorts, hTMK);
//#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
//          pci_dev_put(pciDev); moved to cleanup_module
//#endif
          return -1;
        }
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
        request_region(tmkHiddenPorts, 128, name);
#endif
        break;
      }
      switch (tmkDeviceID)
      {
      case PLX_DID0: // PCI_9030
        if (pcim >= 0)
        {
          if (pcim)
            outw(inw(tmkHiddenPorts + 0x50) | 0x4000, tmkHiddenPorts + 0x50); //Fix 9030
          else
            outw(inw(tmkHiddenPorts + 0x50) & ~0x4000, tmkHiddenPorts + 0x50);
        }
        release_region(tmkHiddenPorts, 128);
        break;
      case PLX_DID1: // PE_9056
        switch (tmkSubSystemID)
        {
        case ID_TA1PE4:
        case ID_TA1PE432RT:
        case ID_TA1PE432RTE:
        case ID_TA2PE4:
        case ID_TA2PE432RT:
        case ID_TA2PE432RTE:
//        case ID_TA1PE4A708:
//        case ID_TA2PE4A708:
          if ((tmkDevExt == 2 && ((inw(tmkHiddenPorts) & 0xFF) != 0xC1)) ||
              (tmkDevExt == 4 && ((inw(tmkHiddenPorts + 0xF0) & 0xFF) != 0xC1)))
          {
            if (!tmkAutoPCI)
              printk(MY_KERN_WARNING "tmk1553b: can't find specified PCI subdevice %d.%d!\n", tmkDev, tmkDevExt);
//#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
//            pci_dev_put(pciDev); moved to cleanup_module
//#endif
            release_region(tmkHiddenPorts, 128);
            return -1;
          }
          tmkPorts1 += 32 * ((tmkDevExt - 1) & 0x1);
          break;
        case ID_TA1PE432RTF:
        case ID_TA1BPE:
        case ID_TA1BPE32RT:
        case ID_TA1BPE32RTV:
        case ID_TA1BPE32RTE:
        case ID_TA1BPE32RTF:
        case ID_TA2PE432RTF:
        case ID_TA2BPE:
        case ID_TA2BPE32RT:
        case ID_TA2BPE32RTV:
        case ID_TA2BPE32RTE:
        case ID_TA2BPE32RTF:
//        case ID_TA1BPEA708:
//        case ID_TA2BPEA708:
          if ((tmkDevExt == 2 && ((inw(tmkHiddenPorts) & 0xFF) != 0x81)) ||
              (tmkDevExt == 4 && ((inw(tmkHiddenPorts + 0xF0) & 0xFF) != 0x81)))
          {
            if (!tmkAutoPCI)
              printk(MY_KERN_WARNING "tmk1553b: can't find specified PCI subdevice %d.%d!\n", tmkDev, tmkDevExt);
//#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
//            pci_dev_put(pciDev); moved to cleanup_module
//#endif
            release_region(tmkHiddenPorts, 128);
            return -1;
          }
          tmkPorts1 += 64 * ((tmkDevExt - 1) & 0x1);
          break;
        }
        outw(inw(tmkHiddenPorts + 0x68) | 0x0800, tmkHiddenPorts + 0x68); //Enable INTi#
        outw(inw(tmkHiddenPorts + 0x6A) & 0xFFFE, tmkHiddenPorts + 0x6A); //Disable INTo#
        release_region(tmkHiddenPorts, 128);
        break;
      }
//#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,16)
//      pci_dev_put(pciDev); moved to cleanup_module
//#endif
    }
    else // if pciDev
    {
      if (!tmkAutoPCI)
        printk(MY_KERN_WARNING "tmk1553b: can't find specified PCI device %d!\n", tmkDev);
      return -1;
    }
    break;
#endif //def CONFIG_PCI
  default:
    printk(MY_KERN_WARNING "tmk1553b: device type %d isn't supported!\n", tmkType);
    return -1;
  }
  switch(tmkSubSystemID)
  {
  case ID_TA1P32RTF:
  case ID_TA1P32RTV:
  case ID_TA1PE432RTF:
  case ID_TA1BPE:
  case ID_TA1BPE32RT:
  case ID_TA1BPE32RTE:
  case ID_TA1BPE32RTF:
  case ID_TA1BPE32RTV:
//  case ID_TA1BPEA708:
  case ID_TA2P32RTF:
  case ID_TA2P32RTV:
  case ID_TA2PE432RTF:
  case ID_TA2BPE:
  case ID_TA2BPE32RT:
  case ID_TA2BPE32RTE:
  case ID_TA2BPE32RTF:
  case ID_TA2BPE32RTV:
//  case ID_TA2BPEA708:
    nTmkPorts = 64;
    break;
  }
  if (tmkType == TMKX || tmkType == TMKXI)
  {
    found = 0;
    if (pszLoad != 0)
    {
      for (tmkLoad = MIN_LOAD_TYPE_TMKX; tmkLoad <= MAX_LOAD_TYPE_TMKX && tmkLoad <= MAX_LOAD_TYPE; ++tmkLoad)
      {
        if (strcmp(pszLoad, apszTmkLoadName[tmkLoad]) == 0)
        {
          found = 1;
          break;
        }
      }
    }
    else
    {
      found = 1;
      tmkLoad = LOADTX1;
#ifdef LOADTX6
      if (tmkSubSystemID == ID_TX6PCI || tmkSubSystemID == ID_TX6PCI2)
        tmkLoad = LOADTX6;
#endif //def LOADTX6
    }
    if (!found)
    {
      printk(MY_KERN_WARNING "tmk1553b: load type \"%s\" is incorrect!\n", pszLoad);
      return -1;
    }
  }
  if (tmkType == MRTA || tmkType == MRTAI)
  {
    found = 0;
    if (pszLoad != 0)
    {
      for (tmkLoad = MIN_LOAD_TYPE_MRTA; tmkLoad <= MAX_LOAD_TYPE_MRTA && tmkLoad <= MAX_LOAD_TYPE; ++tmkLoad)
      {
        if (strcmp(pszLoad, apszTmkLoadName[tmkLoad]) == 0)
        {
          found = 1;
          break;
        }
      }
    }
    else
    {
      found = 1;
      tmkLoad = LOADMRA;
    }
    if (!found)
    {
      printk(MY_KERN_WARNING "tmk1553b: load type \"%s\" is incorrect!\n", pszLoad);
      return -1;
    }
  }
  aTmkConfig[hTMK].nType = (unsigned short)tmkType;
  strcpy(aTmkConfig[hTMK].szName, apszTmkTypeName[tmkType]);
/*  pchS = apszTmkTypeName[tmkType];
  pchD = aTmkConfig[hTMK].szName;
  while (*pchS != '\0')
    *pchD++ = *pchS++;
*/
  aTmkConfig[hTMK].wPorts1 = (unsigned short)tmkPorts1;
  aTmkConfig[hTMK].wPorts2 = (unsigned short)tmkPorts2;
  aTmkConfig[hTMK].wIrq1 = (unsigned short)tmkIrq1;
  aTmkConfig[hTMK].wIrq2 = 0xFF;
  aTmkConfig[hTMK].nLoadType = tmkLoad;
  if (noplx && (tmkType == TAI || tmkType == MRTAI))
    tmkHiddenPorts = 0xFFFF; // set/clrcwbits, rawion may not work
  aTmkConfig[hTMK].wHiddenPorts = (unsigned short)tmkHiddenPorts;
  if (tmkType == TMKXI || tmkType == TAI || tmkType == MRTAI)
    aTmkConfig[hTMK].fIrqShared = 1;
  aTmkConfig[hTMK].fLocalReadInt = tmkLocalReadInt;
  aTmkConfig[hTMK].fMRT = 0;
  aTmkConfig[hTMK].fLoaded = 0;
  aTmkConfig[hTMK].DID = (unsigned)tmkDeviceID;
  aTmkConfig[hTMK].SubDID = (unsigned)(tmkSubSystemID >> 16);
  if (tmkType == TAI || tmkType == MRTAI)
  {
    if (nTmkPorts == 0)
      aTmkConfig[hTMK].nPorts = TA_NR_PORTS;
    else
      aTmkConfig[hTMK].nPorts = nTmkPorts;
  }
  else if (tmkType == TA || tmkType == MRTA)
    aTmkConfig[hTMK].nPorts = TA_NR_PORTS;
  else
    aTmkConfig[hTMK].nPorts = TMK_NR_PORTS;
  switch (tmkType)
  {
  case TMK400:
    printk(MY_KERN_INFO "tmk1553b: device %d: %s i/o addr=%X,%X irq=%u (ISA)\n", hTMK, apszTmkTypeName[tmkType], tmkPorts1, tmkPorts2, tmkIrq1);
    break;
  case RTMK400:
  case TMKMPC:
  case TMKX:
//  case MRTX:
  case TA:
  case MRTA:
    if ((tmkType == TMKX || tmkType == MRTA) && pszLoad != 0)
      printk(MY_KERN_INFO "tmk1553b: device %d: %s i/o addr=%X irq=%u load=%s (ISA)\n", hTMK, apszTmkTypeName[tmkType], tmkPorts1, tmkIrq1, pszLoad);
    else
      printk(MY_KERN_INFO "tmk1553b: device %d: %s i/o addr=%X irq=%u (ISA)\n", hTMK, apszTmkTypeName[tmkType], tmkPorts1, tmkIrq1);
    break;
  case TMKXI:
//  case MRTXI:
  case TAI:
  case MRTAI:
    if ((tmkType == TMKXI || tmkType == MRTAI) && pszLoad != 0)
      printk(MY_KERN_INFO "tmk1553b: device %d: %s i/o addr=%X irq=%u load=%s (PCI d:%d e:%d pci:%02x:%x.%x sys:%08X subsys:%08X addrh=%X)\n", hTMK, apszTmkTypeName[tmkType], tmkPorts1, tmkIrq1, pszLoad, tmkDev, tmkDevExt, pciDev->bus->number, pciDev->devfn >> 3, pciDev->devfn & 7, tmkSystemID, tmkSubSystemID, tmkHiddenPorts);
    else
      printk(MY_KERN_INFO "tmk1553b: device %d: %s i/o addr=%X irq=%u (PCI d:%d e:%d pci:%02x:%x.%x sys:%08X subsys:%08X addrh=%X)\n", hTMK, apszTmkTypeName[tmkType], tmkPorts1, tmkIrq1, tmkDev, tmkDevExt, pciDev->bus->number, pciDev->devfn >> 3, pciDev->devfn & 7, tmkSystemID, tmkSubSystemID, tmkHiddenPorts);
    break;
  }
  return 1;
}

static int tmk1553b_init_module(void)
{
    int result; //, i;
    int tmk1553b_irq;
    int share;
//    int probe;
    int iTMK, jTMK;
    int fRequestHiddenPorts;
    TIrq *pIrq=NULL;
    int hIrq;
    int iRt, iRt1;

    #ifdef MY_DBG
    #ifdef __SMP__
    printk(MY_KERN_DEBUG "Tmk1553b: Entered the Tmk1553b SMP driver!\n");
    #else
    printk(MY_KERN_DEBUG "Tmk1553b: Entered the Tmk1553b UP driver!\n");
    #endif
    #endif

    tmkdrvinit(); // in case of no tmkconfig called further

    if (t0 == 0 && t1 == 0 && t2 == 0 && t3 == 0 && t4 == 0 && t5 == 0 && t6 == 0 && t7 == 0 &&
#if MAX_TMK_NUMBER > 7
        t8 == 0 && t9 == 0 && t10 == 0 && t11 == 0 && t12 == 0 && t13 == 0 && t14 == 0 && t15 == 0 &&
#endif // MAX_TMK_NUMBER > 7
#if MAX_TMK_NUMBER > 15
        t16 == 0 && t17 == 0 && t18 == 0 && t19 == 0 && t20 == 0 && t21 == 0 && t22 == 0 && t23 == 0 &&
        t24 == 0 && t25 == 0 && t26 == 0 && t27 == 0 && t28 == 0 && t29 == 0 && t30 == 0 && t31 == 0 &&
#endif // MAX_TMK_NUMBER > 15
        autopci < 0)
    { // special auto mode without parameters:
      // special autopci - on
      // special misc    - on
      // special chmod   - 0666
      autopci = 1;
      if (misc < 0 && major == 0)
        misc = 1;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
      if (chmod < 0 && misc)
        chmod = 0666;
#endif
    }
    // default autopci - off
    // default misc    - off
    // default chmod   - default
    if (autopci < 0)
      autopci = 0;
    if (misc < 0)
      misc = 0;

    if (name == 0)
      name = szTmk1553b;

    if (misc && major)
    {
      printk(MY_KERN_WARNING "tmk1553b: 'misc' and 'major' parameters cannot be set both\n");
      return -EINVAL;
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
    if (chmod >= 0 && major)
    {
      printk(MY_KERN_WARNING "tmk1553b: 'chmod' and 'major' parameters cannot be set both\n");
      return -EINVAL;
    }
#endif

    fTMKInit = 0;
/*
    if (RegTmkInit(RegistryPath))
    {
      #ifdef MY_DBG
      printk(MY_KERN_DEBUG "Tmk1553b: Configuration Loading from Registry Failed\n");
      #endif
      return STATUS_UNSUCCESSFUL;
    }
*/
  for (iTMK = 0; iTMK <= MAX_TMK_NUMBER; ++iTMK)
  {
    aTmkConfig[iTMK].nTmk = iTMK;
    aTmkConfig[iTMK].nType = -1;
    aTmkConfig[iTMK].szName[0] = '\0';
    aTmkConfig[iTMK].wPorts1 = aTmkConfig[iTMK].wPorts2 = 0;
    aTmkConfig[iTMK].wIrq1 = aTmkConfig[iTMK].wIrq2 = 0;
    aTmkConfig[iTMK].wHiddenPorts = 0;
    aTmkConfig[iTMK].fIrqShared = 0;
    aTmkConfig[iTMK].fMRT = 0;
    aTmkConfig[iTMK].fLoaded = 0;
    aTmkConfig[iTMK].nPorts = 0;
    aTmkConfig[iTMK].DID = 0;
    aTmkConfig[iTMK].SubDID = 0;
  }

  if (nrt < 0)
    nrt = 0;

  if (RegInitAll() < 0)
  {
    printk(MY_KERN_WARNING "tmk1553b: RegInit error!\n");
    return -1;
  }

    fTMKInit = 1;

//    for (iTMK = 0; iTMK <= (MAX_TMK_NUMBER+1+MAX_RT_NUMBER+1) - 1; ++iTMK)
//    {
//      __adwTmkOptions[iTMK] = MRT_READ_BRC_DATA | MRT_WRITE_BRC_DATA;
//    }


    pszVerChange = "";
#ifdef TMK1553B_THREADS
    if (!mthread)
      pszVerChange = "->t0";
#else
#ifdef LINUX26
    if (mthread)
      pszVerChange = "->t1";
#endif
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0)
    SET_MODULE_OWNER(&tmk1553b_fops);
#endif
#ifdef MY_CONFIG_DEVFS_FS
    /* If we have devfs, create /dev/tmk1553b to put files in there */
    tmk1553b_devfs_dir = devfs_mk_dir(NULL, name, NULL);
    if (!tmk1553b_devfs_dir) return -EBUSY; /* problem */
#else /* no devfs, do it the "classic" way */
    if (!misc)
    {
      /*
       * Register your major, and accept a dynamic number. This is the
       * first thing to do, in order to avoid releasing other module's
       * fops in tmk1553b_cleanup_module()
       */
      result = register_chrdev(major, name, &tmk1553b_fops);
      if (result < 0)
      {
        if (major == 0)
          printk(MY_KERN_WARNING "tmk1553b: can't register device %s with dynamic major!\n", name);
        else
          printk(MY_KERN_WARNING "tmk1553b: can't register device %s, major=%d!\n", name, major);
        return result;
      }
      if (major == 0) major = result; /* dynamic */

      printk(MY_KERN_INFO "tmk1553b: Registered device %s v%s%s, major=%d\n", name, TMK_VERSION, pszVerChange, major);
    }
    else
    {
      tmk1553b_mdev.minor = MISC_DYNAMIC_MINOR;
      tmk1553b_mdev.name = name;
      tmk1553b_mdev.fops = &tmk1553b_fops;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
      if (chmod >= 0)
        tmk1553b_mdev.mode = chmod;
#endif
      result = misc_register(&tmk1553b_mdev);
      if (result < 0)
      {
        printk(MY_KERN_WARNING "tmk1553b: can't register dynamic misc device %s!\n", name);
        return result;
      }

      printk(MY_KERN_INFO "tmk1553b: Registered dynamic misc device %s v%s%s, minor=%d\n", name, TMK_VERSION, pszVerChange, tmk1553b_mdev.minor);
    }
#endif /* ndef MY_CONFIG_DEVFS_FS */

    /*
     * allocate the devices -- we can't have them static, as the number
     * can be specified at load time
     */
/*
    tmk1553b_devices = kmalloc(tmk1553b_nr_devs * sizeof(tmk1553b_Dev), GFP_KERNEL);
    if (!tmk1553b_devices) {
        result = -ENOMEM;
        goto fail;
    }
    memset(tmk1553b_devices, 0, tmk1553b_nr_devs * sizeof(tmk1553b_Dev));
    for (i=0; i < tmk1553b_nr_devs; i++) {
        tmk1553b_devices[i].quantum = tmk1553b_quantum;
        tmk1553b_devices[i].qset = tmk1553b_qset;
        sema_init(&tmk1553b_devices[i].sem, 1);

#ifdef MY_CONFIG_DEVFS_FS
        sprintf(devname, "%i", i);
        devfs_register(tmk1553b_devfs_dir, devname,
                       DEVFS_FL_AUTO_DEVNUM,
                       0, 0, S_IFCHR | S_IRUGO | S_IWUGO,
                       &tmk1553b_fops,
                       tmk1553b_devices+i);
#endif
    }
*/

    /* At this point call the init function for any friend device */
//    if ( (result = tmk1553b_access_init()) )
//        goto fail;
    /* ... */

  spin_lock_init(&tmkSpinLock);
  spin_lock_init(&tmkIrqSpinLock);

  //
  // connect the device driver to the IRQs
  //

  for (iTMK = 0; iTMK <= MAX_TMK_NUMBER; ++iTMK)
  {
    if (aTmkConfig[iTMK].nType == -1)
      continue;

    //
    // check if resources (ports and interrupt) are available
    //

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
    result = check_region(aTmkConfig[iTMK].wPorts1, aTmkConfig[iTMK].nPorts);
#else
    if (!request_region(aTmkConfig[iTMK].wPorts1, aTmkConfig[iTMK].nPorts, name))
      result = -EINVAL;
    else
      result = 0;
#endif
    if (result)
    {
      printk(MY_KERN_WARNING "tmk1553b: can't get I/O port 1 address 0x%x for TMK%d!\n", aTmkConfig[iTMK].wPorts1, iTMK);
      goto fail;
    }
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
    request_region(aTmkConfig[iTMK].wPorts1, aTmkConfig[iTMK].nPorts, name);
#endif

    if (aTmkConfig[iTMK].wPorts2 != 0xFFFF &&
        aTmkConfig[iTMK].wPorts2 != aTmkConfig[iTMK].wPorts1)
    {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
      result = check_region(aTmkConfig[iTMK].wPorts2, aTmkConfig[iTMK].nPorts);
#else
      if (!request_region(aTmkConfig[iTMK].wPorts2, aTmkConfig[iTMK].nPorts, name))
        result = -EINVAL;
      else
        result = 0;
#endif
      if (result)
      {
        printk(MY_KERN_WARNING "tmk1553b: can't get I/O port 2 address 0x%x for TMK%d!\n", aTmkConfig[iTMK].wPorts2, iTMK);
        goto fail;
      }
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
      request_region(aTmkConfig[iTMK].wPorts2, aTmkConfig[iTMK].nPorts, name);
#endif
    }

    if ((aTmkConfig[iTMK].nType == TMKXI || aTmkConfig[iTMK].nType == TAI ||
         aTmkConfig[iTMK].nType == MRTAI) &&
        (aTmkConfig[iTMK].wHiddenPorts != 0) &&
        (aTmkConfig[iTMK].wHiddenPorts != 0xFFFF))
    {
      fRequestHiddenPorts = 1;
      for (jTMK = 0; jTMK < iTMK; ++jTMK)
      {
        if (aTmkConfig[jTMK].nType != TMKXI && aTmkConfig[jTMK].nType != TAI &&
            aTmkConfig[jTMK].nType != MRTAI)
          continue;
        if (aTmkConfig[iTMK].wHiddenPorts == aTmkConfig[jTMK].wHiddenPorts)
        {
          fRequestHiddenPorts = 0;
          break;
        }
      }
      if (fRequestHiddenPorts)
      {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
        result = check_region(aTmkConfig[iTMK].wHiddenPorts, 128);
#else
        if (!request_region(aTmkConfig[iTMK].wHiddenPorts, 128, name))
          result = -EINVAL;
        else
          result = 0;
#endif
        if (result)
        {
          printk(MY_KERN_WARNING "tmk1553b: can't get hidden I/O port address 0x%x for TMK%d!\n", aTmkConfig[iTMK].wHiddenPorts, iTMK);
          goto fail;
        }
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
        request_region(aTmkConfig[iTMK].wHiddenPorts, 128, name);
#endif
      }
    }
    /*
     * Now we deal with the interrupt: either kernel-based
     * autodetection, DIY detection or default number
     */

       tmk1553b_irq = aTmkConfig[iTMK].wIrq1;
       share = aTmkConfig[iTMK].fIrqShared;
//       probe = 0;

/*
    if (tmk1553b_irq < 0 && probe == 1)
        tmk1553b_kernelprobe();

    if (tmk1553b_irq < 0 && probe == 2)
        tmk1553b_selfprobe();
*/

    /*
     * If shared has been specified, installed the shared handler
     * instead of the normal one. Do it first, before a -EBUSY will
     * force tmk1553b_irq to -1.
     */

    for (hIrq = 0; hIrq < cIrqs; ++hIrq)
    {
      pIrq = ahIrq + hIrq;
      if (pIrq->nIrq == tmk1553b_irq)
        break;
    }
    if (hIrq == cIrqs)
    {
      if (share)
      {
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: request_irq shared irq%d, hIrq=%d: %X, %X\n", tmk1553b_irq, hIrq, (int)tmkInterruptServiceRoutine, (int)tmkInterruptServiceRoutine + hIrq);
        #endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
        result = request_irq(tmk1553b_irq, tmkInterruptServiceRoutine,
                             SA_SHIRQ | SA_INTERRUPT, name,
                             tmkInterruptServiceRoutine + (ptrdiff_t)hIrq);
#else
        result = request_irq(tmk1553b_irq, tmkInterruptServiceRoutine,
                             IRQF_DISABLED | IRQF_SHARED, name,
                             tmkInterruptServiceRoutine + (ptrdiff_t)hIrq);
#endif
        if (result)
        {
          printk(MY_KERN_WARNING "tmk1553b: can't get assigned shared irq %i for TMK%d!\n", tmk1553b_irq, iTMK);
          goto fail;
        }
      }
      else
      {
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553B: request_irq irq%d, hIrq=%d: %X, %X\n", tmk1553b_irq, hIrq, (int)tmkInterruptServiceRoutine, (int)tmkInterruptServiceRoutine + hIrq);
        #endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
        result = request_irq(tmk1553b_irq, tmkInterruptServiceRoutine,
                             SA_INTERRUPT, name,
                             tmkInterruptServiceRoutine + (ptrdiff_t)hIrq);
#else
        result = request_irq(tmk1553b_irq, tmkInterruptServiceRoutine,
                             IRQF_DISABLED, name,
                             tmkInterruptServiceRoutine + (ptrdiff_t)hIrq);
#endif
        if (result)
        {
          printk(MY_KERN_WARNING "tmk1553b: can't get assigned irq %i for TMK%d!\n", tmk1553b_irq, iTMK);
          goto fail;
        }
      }
      pIrq = ahIrq + hIrq;
      pIrq->nIrq = tmk1553b_irq;
      pIrq->cTmks = 0;
      ++cIrqs;
    }
    else
    {
      if (!share)
      {
        printk(MY_KERN_WARNING "tmk1553b: can't share assigned irq %i for TMK%d!\n", tmk1553b_irq, iTMK);
        goto fail;
      }
    }
    pIrq->hTmk[pIrq->cTmks++] = iTMK;

  } //end for

  //
  // Initialize the device
  //

//!!! here find max possible nrt from MAX_RT_NUMBER!
  nmrt = 0;
  for (iTMK = 0; iTMK <= MAX_TMK_NUMBER; ++iTMK)
  {
//    if (aTmkConfig[iTMK].nType == -1)
//      continue;
    if (
#ifdef LOADMR6
        ((aTmkConfig[iTMK].nType == TMKX || aTmkConfig[iTMK].nType == TMKXI) && aTmkConfig[iTMK].nLoadType == LOADMR6) ||
#endif //def LOADMR6
        ((aTmkConfig[iTMK].nType == MRTA || aTmkConfig[iTMK].nType == MRTAI) && aTmkConfig[iTMK].nLoadType == LOADMRA))
    {
      ++nmrt;
    }
  }
  if (nmrt == 0)
    nmrt = 1;
  nrtmax = (MAX_RT_NUMBER + 1) / nmrt;
  if (nrt == 0 || nrt > nrtmax)
    nrt = nrtmax;

  mrtdefmaxnrt(nrt);

  if (kfm >= 0)
    tmkdrvkeepflgm(kfm);
  if (wraps >= 0)
    tmkdrvwrapsa(wraps, wrapt, wrapi);
  if (cbsnd >= 0)
    tmkdrvcbsnd(cbsnd);
  if (real >= 0)
    tmkdrvrealmax(real);

#ifdef LINUX_SWTIMER
  for (iTMK = 0; iTMK <= (MAX_TMK_NUMBER + 1 + MAX_RT_NUMBER + 1 - 1); ++iTMK)
  {
    afUseEvTime[iTMK] = 0;
  }
#endif //LINUX_SWTIMER

  for (iTMK = 0; iTMK <= MAX_TMK_NUMBER; ++iTMK)
  {
    iEvDataBegin[iTMK] = iEvDataEnd[iTMK] = cEvData[iTMK] = cDpcData[iTMK] = 0;

    if (aTmkConfig[iTMK].nType == -1)
      continue;

    if (aTmkConfig[iTMK].nType == TMKX)
    {
      switch (aTmkConfig[iTMK].nLoadType)
      {
      case LOADTX1:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltx1v11\n");
        #endif
        tmkxload(aTmkConfig[iTMK].wPorts1, fpgabuf0, sizeof(fpgabuf0));
        break;
#ifdef LOADRT1
      case LOADRT1:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: lrt1v03\n");
        #endif
        tmkxload(aTmkConfig[iTMK].wPorts1, fpgabuf1, sizeof(fpgabuf1));
        break;
#endif //def LOADRT1
#ifdef LOADTX4
      case LOADTX4:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltx4v11\n");
        #endif
        tmkxload(aTmkConfig[iTMK].wPorts1, fpgabuf2, sizeof(fpgabuf2));
        break;
#endif //def LOADTX4
#ifdef LOADTX5
      case LOADTX5:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltx5v11\n");
        #endif
        tmkxload(aTmkConfig[iTMK].wPorts1, fpgabuf3, sizeof(fpgabuf3));
        break;
#endif //def LOADTX5
#ifdef LOADTX6
      case LOADTX6:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltx6v11\n");
        #endif
        tmkxload(aTmkConfig[iTMK].wPorts1, fpgabuf4, sizeof(fpgabuf4));
        break;
#endif //def LOADTX6
#ifdef LOADMR6
      case LOADMR6:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: lmr6v02\n");
        #endif
        tmkxload(aTmkConfig[iTMK].wPorts1, fpgabuf7, sizeof(fpgabuf7));
        aTmkConfig[iTMK].nType = MRTX;
        break;
#endif //def LOADMR6
#ifdef LOADTT6
      case LOADTT6:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltt6v04\n");
        #endif
        tmkxload(aTmkConfig[iTMK].wPorts1, fpgabuf8, sizeof(fpgabuf8));
        aTmkConfig[iTMK].nType = TSTX;
        break;
#endif //def LOADTT6
#ifdef LOADTD6
      case LOADTD6:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltd6v02\n");
        #endif
        tmkxload(aTmkConfig[iTMK].wPorts1, fpgabuf9, sizeof(fpgabuf9));
        aTmkConfig[iTMK].nType = TSTX;
        break;
#endif //def LOADTD6
#ifdef LOADAM6
      case LOADAM6:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: lam6v02\n");
        #endif
        tmkxload(aTmkConfig[iTMK].wPorts1, fpgabuf13, sizeof(fpgabuf13));
        aTmkConfig[iTMK].nType = TSTX;
        break;
#endif //def LOADAM6
      default:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: default disabled\n");
        #endif
        aTmkConfig[iTMK].nType = -1;
        continue;
//        printk(MY_KERN_WARNING "tmk1553b: unsupported load type %d requested!\n", aTmkConfig[iTMK].nLoadType);
//        goto fail;
//        break;
      }
    }
    if (aTmkConfig[iTMK].nType == MRTA)
    {
      switch (aTmkConfig[iTMK].nLoadType)
      {
      case LOADMRA:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: lmrta\n");
        #endif
        break;
#ifdef LOADTRA
      case LOADTRA:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltsta\n");
        #endif
        aTmkConfig[iTMK].nType = TSTA;
        break;
#endif //def LOADTRA
      default:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: default disabled\n");
        #endif
        aTmkConfig[iTMK].nType = -1;
        continue;
      }
    }
#ifdef CONFIG_PCI
    if (aTmkConfig[iTMK].nType == TMKXI && aTmkConfig[iTMK].wHiddenPorts != 0xFFFF)
      tmkxiload(aTmkConfig[iTMK].wHiddenPorts, 0);
    if (aTmkConfig[iTMK].nType == MRTAI)
    {
      switch (aTmkConfig[iTMK].nLoadType)
      {
      case LOADMRA:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: lmrtai\n");
        #endif
        break;
#ifdef LOADTRA
      case LOADTRA:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: ltstai\n");
        #endif
        aTmkConfig[iTMK].nType = TSTAI;
        break;
#endif //def LOADTRA
      default:
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: default disabled\n");
        #endif
        aTmkConfig[iTMK].nType = -1;
        continue;
      }
    }
#endif //def CONFIG_PCI

    switch (aTmkConfig[iTMK].nType)
    {
    case TMK400:
    case TMKMPC:
    case RTMK400:
    case TMKX:
    case TA:
    case MRTX:
    case MRTA:
    case TSTX:
    case TSTA:
      tmkconfig(
        iTMK,
        aTmkConfig[iTMK].nType,
        aTmkConfig[iTMK].wPorts1,
        aTmkConfig[iTMK].wPorts2,
        (unsigned char)aTmkConfig[iTMK].wIrq1,
        (unsigned char)aTmkConfig[iTMK].wIrq2,
        0,
        0
        );
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: tmkconfig(%d, %d, 0x%X, 0x%X, %d, %d, 0, 0)\n",
          iTMK,
          aTmkConfig[iTMK].nType,
          aTmkConfig[iTMK].wPorts1,
          aTmkConfig[iTMK].wPorts2,
          (unsigned char)aTmkConfig[iTMK].wIrq1,
          (unsigned char)aTmkConfig[iTMK].wIrq2
        );
        #endif
      break;
    case TMKXI:
    case TAI:
    case MRTXI:
    case MRTAI:
    case TSTXI:
    case TSTAI:
      tmkconfig(
        iTMK,
        aTmkConfig[iTMK].nType,
        aTmkConfig[iTMK].wPorts1,
        aTmkConfig[iTMK].wHiddenPorts,
        (unsigned char)aTmkConfig[iTMK].wIrq1,
        (unsigned char)aTmkConfig[iTMK].wIrq2,
        aTmkConfig[iTMK].DID,
        aTmkConfig[iTMK].SubDID
        );
        #ifdef MY_DBG
        printk(MY_KERN_DEBUG "Tmk1553b: tmkconfig(%d, %d, 0x%X, 0x%X, %d, %d, %04X, %04X)\n",
          iTMK,
          aTmkConfig[iTMK].nType,
          aTmkConfig[iTMK].wPorts1,
          aTmkConfig[iTMK].wHiddenPorts,
          (unsigned char)aTmkConfig[iTMK].wIrq1,
          (unsigned char)aTmkConfig[iTMK].wIrq2,
          aTmkConfig[iTMK].DID,
          aTmkConfig[iTMK].SubDID
        );
        #endif
      break;
    }

    switch (aTmkConfig[iTMK].nType)
    {
    case TMK400:
    case TMKMPC:
    case RTMK400:
      tmkiodelay(30);
      fallthrough;
      // fall-through
    case TMKX:
    case TMKXI:
    case TA:
    case TAI:
      bcreset();
//      #ifdef MY_DBG
      if (TMK_TuneIODelay(0))
        printk(MY_KERN_WARNING "tmk1553b: warning - device test fail for TMK%d!\n", iTMK);
//      #else
//      TMK_TuneIODelay(0);
//      #endif
      break;
    case MRTX:
    case MRTXI:
    case MRTA:
    case MRTAI:
      iRt = mrtgetrt0();
      aTmkConfig[iTMK].fMRT = 0;
      iRt1 = iRt + mrtgetnrt();
      #ifdef MY_DBG
      printk(MY_KERN_DEBUG "Tmk1553b: MRT:");
      #endif
      while (iRt < iRt1)
      {
        tmkError = 0;
        tmkselect(iRt);
        rtreset();
        #ifdef MY_DBG
        if (tmkError)
          printk(MY_KERN_DEBUG "-");
        else
          printk(MY_KERN_DEBUG "+");
        #endif
        bcreset();
        tmkError = 0;
        ++iRt;
      }
      #ifdef MY_DBG
      printk(MY_KERN_DEBUG "\n");
      #endif
      break;
    case TSTX:
    case TSTXI:
    case TSTA:
    case TSTAI:
      bcreset();
      break;
    }
  }


  nMaxTmkNumber = tmkgetmaxn();

  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "Tmk1553b: just about ready!\n");
  #endif

  for (iTMK = 0; iTMK < U32_EVENTS_SIZE; ++iTMK) // iTMK is iEvPck !
    tmkEvents[iTMK] = 0;
  hCurProc = 0;
  INIT_LIST_HEAD(&hlProc);
  for (iTMK = 0; iTMK <= nMaxTmkNumber; ++iTMK)
  {
    aTMK[iTMK].hProc = 0;
    aTMK[iTMK].hlnProc = 0;
    aTMK[iTMK].hEvent = 0;
  //if (tmkselect(iTMK))
  //  continue;
  //tmkdefevent(0,0);
  }
  tmkNumber = -1;

  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "Tmk1553b: All initialized!\n");
  #endif

  for (iTMK = nMaxTmkNumber; iTMK >= 0; --iTMK)
  {
    if (aTmkConfig[iTMK].nType >= 0)
      break;
  }
  if (iTMK < 0)
    printk(MY_KERN_WARNING "tmk1553b: warning - no devices!\n");

#ifndef TMK1553B_DEBUG
    EXPORT_NO_SYMBOLS; /* otherwise, leave global symbols visible */
#endif

#ifdef TMK1553B_DEBUG /* only when debugging */
    tmk1553b_create_proc();
#endif

    return 0; /* succeed */

  fail:
    tmk1553b_cleanup_module();
    return result;
}

module_init(tmk1553b_init_module);
module_exit(tmk1553b_cleanup_module);


UINT TMK_tmkgetmaxn()
{
//  lpwOut[0] = (USHORT)tmkgetmaxn();
//  return tmkError;
  return tmkgetmaxn();
}
UINT TMK_tmkconfig()
{
  int nRt0;
  USHORT aw0[30];

  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "Tmk1553b: tmkconfig\n");
  #endif

  if (tmkselect(tmkNumber = (int)lpwIn[0]) || tmkgetmode() == MRT_MODE || aTMK[tmkNumber].hProc)
  {
    tmkselect(tmkNumber = hlnCurProc->nSelectedTMK);
    tmkError = TMK_BAD_NUMBER;
  }
  else
  {
    tmkreconfig();
//    tmkError = 0;
    if (bcreset())
    {
      if (tmkNumber > MAX_TMK_NUMBER)
      {
        nRt0 = mrtgetrt0();
        if (aTMK[nRt0].hProc == 0)
        {
          switch (aTmkConfig[rt2mrt(tmkNumber)].nType)
          {
          case MRTX:
          case MRTXI:
            tmkselect(nRt0);
            memset(aw0, 0, sizeof(aw0)); // RtlZeroMemory(aw0, sizeof(aw0));
            rtputflags(aw0, RT_RECEIVE, 1, 30);
            tmkselect(tmkNumber);
            break;
          }
        }
      }
      tmkError = 0;
    }
    hlnCurProc->fTMK[tmkNumber] = 1;
    hlnCurProc->maskTMK[tmkNumber>>5] |= (1<<(tmkNumber&0x1F));
    hlnCurProc->nSelectedTMK = tmkNumber;
    aTMK[tmkNumber].hProc = hCurProc;
    aTMK[tmkNumber].hlnProc = hlnCurProc;
    tmkEvents[tmkNumber>>5] &= ~(1<<(tmkNumber&0x1F));
    iEvDataBegin[tmkNumber] = iEvDataEnd[tmkNumber] = cEvData[tmkNumber] = 0;
  }
  return tmkError;
}
UINT TMK_tmkdone()
{
  int nMinTMK, nMaxTMK;
  int nPrevTMK;
  int iTMK;
  int iRt, nMaxRt;
  USHORT aw0[30];

  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "Tmk1553b: tmkdone\n");
  #endif

  nPrevTMK = tmkNumber;
  iTMK = (int)lpwIn[0];
  if (iTMK == ALL_TMKS || (iTMK == ALL_TMKS_OLD && iTMK > nMaxTmkNumber))
  {
    nMinTMK = 0;
    nMaxTMK = nMaxTmkNumber;
  }
  else if (iTMK >= 0 && iTMK <= nMaxTmkNumber && aTMK[iTMK].hProc == hCurProc)
  {
    nMinTMK = nMaxTMK = iTMK;
  }
  else
  {
    tmkError = TMK_BAD_NUMBER;
    return tmkError;
  }
  for (iTMK = nMinTMK; iTMK <= nMaxTMK; ++iTMK)
  {
    if (aTMK[iTMK].hProc != hCurProc)
      continue;
    tmkselect(iTMK);

    tmkreconfig();
    bcreset();

    if (iTMK <= MAX_TMK_NUMBER && aTmkConfig[iTMK].fMRT)
    {
      iRt = mrtgetrt0();
      nMaxRt = mrtgetnrt() + iRt - 1;
      for (; iRt <= nMaxRt; ++iRt)
      {
        if (aTMK[iRt].hProc != hCurProc)
          continue;
        if (aTMK[iTMK].hEvent)
        {
        //tmkdefevent(0,0);
        //VWIN32_CloseVxDHandle(aTMK[iTMK].hEvent);
//          ObDereferenceObject(aTMK[iTMK].hEvent);
          aTMK[iTMK].hEvent = 0;
        }
        tmkEvents[iRt>>5] &= ~(1<<(iRt&0x1F));
        iEvDataBegin[iRt] = iEvDataEnd[iRt] = cEvData[iRt] = 0;
#ifdef LINUX_SWTIMER
        afUseEvTime[iRt] = 0;
#endif
        aTMK[iRt].hProc = 0;
        aTMK[iRt].hlnProc = 0;
        hlnCurProc->fTMK[iRt] = 0;
        hlnCurProc->maskTMK[iRt>>5] &= ~(1<<(iRt&0x1F));
      }
      switch (aTmkConfig[iTMK].nType)
      {
      case MRTX:
      case MRTXI:
        memset(aw0, 0, sizeof(aw0)); // RtlZeroMemory(aw0, sizeof(aw0));
        rtputflags(aw0, RT_RECEIVE, 1, 30);
        break;
      }
      aTmkConfig[iTMK].fMRT = 0;
    }

    if (iTMK > MAX_TMK_NUMBER && iTMK == mrtgetrt0())
    {
      switch (aTmkConfig[rt2mrt(iTMK)].nType)
      {
      case MRTX:
      case MRTXI:
        memset(aw0, 0, sizeof(aw0)); // RtlZeroMemory(aw0, sizeof(aw0));
        rtputflags(aw0, RT_RECEIVE, 1, 30);
        break;
      }
    }

    if (aTMK[iTMK].hEvent)
    {
    //tmkdefevent(0,0);
    //VWIN32_CloseVxDHandle(aTMK[iTMK].hEvent);
//      ObDereferenceObject(aTMK[iTMK].hEvent);
      aTMK[iTMK].hEvent = 0;
    }
    tmkEvents[iTMK>>5] &= ~(1<<(iTMK&0x1F));
    iEvDataBegin[iTMK] = iEvDataEnd[iTMK] = cEvData[iTMK] = 0;
#ifdef LINUX_SWTIME
    afUseEvTime[iTMK] = 0;
#endif
    aTMK[iTMK].hProc = 0;
    aTMK[iTMK].hlnProc = 0;
    hlnCurProc->fTMK[iTMK] = 0;
    hlnCurProc->maskTMK[iTMK>>5] &= ~(1<<(iTMK&0x1F));
  }
#ifdef LINUX_SWTIMER
  swtimerall();
#endif
  tmkNumber = -1;
  for (iTMK = 0; iTMK <= nMaxTmkNumber; ++iTMK)
  {
    if (hlnCurProc->fTMK[iTMK])
    {
      tmkselect(tmkNumber = iTMK);
      break;
    }
  }
  hlnCurProc->nSelectedTMK = tmkNumber;
  tmkError = 0;
  return tmkError;
}
UINT TMK_tmkselect()
{
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: tmkselect\n");
//  #endif

  if (tmkselect(tmkNumber = (int)lpwIn[0]) || !hlnCurProc->fTMK[tmkNumber])
  {
    tmkselect(tmkNumber = hlnCurProc->nSelectedTMK);
    tmkError = TMK_BAD_NUMBER;
  }
  else
  {
    hlnCurProc->nSelectedTMK = tmkNumber;
  }
  return tmkError;
}
UINT TMK_tmkselected()
{
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: tmkselected\n");
//  #endif

////  lpwOut[0] = (USHORT)tmkselected();
//  lpwOut[0] = (USHORT)hlnCurProc->nSelectedTMK;
//  return tmkError;
  return hlnCurProc->nSelectedTMK;
}
UINT TMK_tmkgetmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = tmkgetmode();
//  return tmkError;
  return tmkgetmode();
}
UINT TMK_tmksetcwbits()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  tmksetcwbits(lpwIn[0]);
  return tmkError;
}
UINT TMK_tmkclrcwbits()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  tmkclrcwbits(lpwIn[0]);
  return tmkError;
}
UINT TMK_tmkgetcwbits()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = tmkgetcwbits();
//  return tmkError;
  return tmkgetcwbits();
}
UINT TMK_tmkwaitevents()
{
  HANDLE hUserEvents;
  int fWait;
  int tmkMyEvents;
  TListProc *hlnSaveCurProc;
  PEPROCESS hSaveCurProc;
//  int tmkSaveNumber;
  long timeout;
  wait_queue_t __wait;
  int fSignal = 0;

//  if (tmkNumber == -1)
//    return (tmkError = TMK_BAD_NUMBER);
  hUserEvents = (HANDLE)(*((PUINT)(lpwIn))) & hlnCurProc->maskTMK[0] & 0x7FFFFFFF;
  fWait = *((int*)(lpwIn+2));
  if (hUserEvents == 0)
    return (tmkError = TMK_BAD_NUMBER);
  tmkMyEvents = hUserEvents & tmkEvents[0];
//  tmkEvents[0] &= ~tmkMyEvents;
  if (fWait == 0 || tmkMyEvents)
  {
    return tmkMyEvents;
  }
  if (fWait > 0)
  {
//    return (tmkError = TMK_BAD_FUNC);
    timeout = fWait * HZ / 1000;
    if (timeout == 0)
      timeout = 1;
  }
  else
    timeout = 0;
  hlnSaveCurProc = hlnCurProc;
  hSaveCurProc = hCurProc;
//  tmkSaveNumber = tmkNumber;

//  printk(MY_KERN_DEBUG "Tmk1553b: waitev1 ");

  init_waitqueue_entry(&__wait, current);
  add_wait_queue(&(hlnSaveCurProc->wq), &__wait);
  hlnSaveCurProc->maskEvents[0] = hUserEvents;
  for (;;)
  {
    if (hlnSaveCurProc->waitFlag)
    {
      hlnSaveCurProc->waitFlag = 0;
      break;
    }
    if (hUserEvents & tmkEvents[0])
      break;
    set_current_state(TASK_INTERRUPTIBLE);
    spin_unlock_bh(&tmkSpinLock);
    if (fWait > 0)
      timeout = schedule_timeout(timeout);
    else
      schedule();
//  printk(MY_KERN_DEBUG "Tmk1553b: waitev w\n");
    spin_lock_bh(&tmkSpinLock);
    if ((fSignal = signal_pending(current)) != 0)
    {
      #ifdef MY_DBG
      printk(MY_KERN_DEBUG "Tmk1553b: break on signal %X, events=%X, myevents=%X\n", signal_pending(current), tmkEvents[0], (hUserEvents&tmkEvents[0]));
      #endif
      break;
    }
    if (hUserEvents & tmkEvents[0])
      break;
    if (fWait > 0 && timeout == 0)
    {
      break;
    }
  }
  hlnSaveCurProc->maskEvents[0] = 0; // for signal, timeout, etc.
  set_current_state(TASK_RUNNING);
  remove_wait_queue(&(hlnSaveCurProc->wq), &__wait);

  tmkError = 0;

//  printk(MY_KERN_DEBUG "Tmk1553b: waitev2 %X\n", tmkEvents[0]);

//  wait_event_interruptible(hlnSaveCurProc->wq,  hUserEvents & tmkEvents[0]);
//  interruptible_sleep_on_timeout(&hlnCurProc->wq, HZ*2);
  hlnCurProc = hlnSaveCurProc;
  hCurProc = hSaveCurProc;
  tmkselect(tmkNumber = hlnCurProc->nSelectedTMK);

  tmkMyEvents = hUserEvents & tmkEvents[0];
//  tmkEvents[0] &= ~tmkMyEvents;
  if (fSignal)
    return -ERESTARTSYS;
  return tmkMyEvents;
}
UINT TMK_tmkwaiteventsm()
{
  HANDLE hUserEvents[U32_EVENTS_SIZE];
  HANDLE hUserEventsAll;
  int cUserEvents;
  int fWait;
// save some kernel stack with tmkMyEvents macro
//  int tmkMyEvents[U32_EVENTS_SIZE];
#define tmkMyEvents(iev) (hUserEvents[(iev)] & tmkEvents[(iev)])
  int tmkMyEventsAll;
  TListProc *hlnSaveCurProc;
  PEPROCESS hSaveCurProc;
//  int tmkSaveNumber;
  long timeout;
  wait_queue_t __wait;
  int fSignal = 0;
  int iEvent, iEvPck;
  int nTmk;
  int fEvents;
  PUSHORT lpwSaveBuf;

//  if (tmkNumber == -1)
//    return (tmkError = TMK_BAD_NUMBER);
  cUserEvents = (int)lpwIn[0];
  fWait = (int)lpwIn[1];
//  lpwBuf
  for (iEvPck = 0; iEvPck < U32_EVENTS_SIZE; ++iEvPck)
    hUserEvents[iEvPck] = 0;
  hUserEventsAll = 0;
  for (iEvent = 0; iEvent < cUserEvents; ++iEvent)
  {
    nTmk = (int)((unsigned)lpwBuf[iEvent]); //avoid extending bit15 to sign
    if (nTmk > MAX_VTMK_NUMBER)
//      return (tmkError = TMK_BAD_NUMBER);
      continue;
    iEvPck = nTmk >> 5;
    hUserEvents[iEvPck] |= (1 << (nTmk & 0x1F)) & hlnCurProc->maskTMK[iEvPck];
    hUserEventsAll |= hUserEvents[iEvPck];
  }
  if (hUserEventsAll == 0)
  {
    return (tmkError = TMK_BAD_NUMBER);
  }
  tmkMyEventsAll = 0;
  for (iEvPck = 0; iEvPck < U32_EVENTS_SIZE; ++iEvPck)
  {
//    tmkMyEvents[iEvPck] = hUserEvents[iEvPck] & tmkEvents[iEvPck];
//    tmkMyEventsAll |= tmkMyEvents[iEvPck];
    tmkMyEventsAll |= tmkMyEvents(iEvPck);
//  tmkEvents[0] &= ~tmkMyEvents;
  }
  if (tmkMyEventsAll)
    goto twem_ExitUpdateEvents;
  if (fWait == 0)
  {
    return 0;
  }
  if (fWait > 0)
  {
//    return (tmkError = TMK_BAD_FUNC);
    timeout = fWait * HZ / 1000;
    if (timeout == 0)
      timeout = 1;
  }
  else
    timeout = 0;
  hlnSaveCurProc = hlnCurProc;
  hSaveCurProc = hCurProc;
  lpwSaveBuf = lpwBuf;
//  tmkSaveNumber = tmkNumber;

//  printk(MY_KERN_DEBUG "Tmk1553b: waitev1 ");

  init_waitqueue_entry(&__wait, current);
  add_wait_queue(&(hlnSaveCurProc->wq), &__wait);
  for (iEvPck = 0; iEvPck < U32_EVENTS_SIZE; ++iEvPck)
  {
    hlnSaveCurProc->maskEvents[iEvPck] = hUserEvents[iEvPck];
  }
  for (;;)
  {
    if (hlnSaveCurProc->waitFlag)
    {
      hlnSaveCurProc->waitFlag = 0;
      break;
    }
    fEvents = 0;
    for (iEvPck = 0; iEvPck < U32_EVENTS_SIZE; ++iEvPck)
    {
      if (hUserEvents[iEvPck] & tmkEvents[iEvPck])
      {
        fEvents = 1;
        break;
      }
    }
    if (fEvents)
      break;
    set_current_state(TASK_INTERRUPTIBLE);
    spin_unlock_bh(&tmkSpinLock);
    if (fWait > 0)
      timeout = schedule_timeout(timeout);
    else
      schedule();
//  printk(MY_KERN_DEBUG "Tmk1553b: waitev w\n");
    spin_lock_bh(&tmkSpinLock);
    if ((fSignal = signal_pending(current)) != 0)
    {
      #ifdef MY_DBG
      printk(MY_KERN_DEBUG "Tmk1553b: break on signal %X, events=%X, myevents=%X\n", signal_pending(current), tmkEvents[0], (hUserEvents[0]&tmkEvents[0]));
      #endif
      break;
    }
    fEvents = 0;
    for (iEvPck = 0; iEvPck < U32_EVENTS_SIZE; ++iEvPck)
    {
      if (hUserEvents[iEvPck] & tmkEvents[iEvPck])
      {
        fEvents = 1;
        break;
      }
    }
    if (fEvents)
      break;
    if (fWait > 0 && timeout == 0)
    {
      break;
    }
  }
  memset(hlnSaveCurProc->maskEvents, 0, sizeof(hlnSaveCurProc->maskEvents)); // for signal, timeout, etc.
  set_current_state(TASK_RUNNING);
  remove_wait_queue(&(hlnSaveCurProc->wq), &__wait);

  tmkError = 0;

//  printk(MY_KERN_DEBUG "Tmk1553b: waitev2 %X\n", tmkEvents[0]);

//  wait_event_interruptible(hlnSaveCurProc->wq,  hUserEvents & tmkEvents[0]);
//  interruptible_sleep_on_timeout(&hlnCurProc->wq, HZ*2);
  hlnCurProc = hlnSaveCurProc;
  hCurProc = hSaveCurProc;
  lpwBuf = lpwSaveBuf;
  tmkselect(tmkNumber = hlnCurProc->nSelectedTMK);

//  for (iEvPck = 0; iEvPck < U32_EVENTS_SIZE; ++iEvPck)
//  {
//    tmkMyEvents[iEvPck] = hUserEvents[iEvPck] & tmkEvents[iEvPck];
//  tmkEvents[0] &= ~tmkMyEvents;
//  }
  twem_ExitUpdateEvents:
  fEvents = 0;
  for (iEvent = 0; iEvent < cUserEvents; ++iEvent)
  {
    nTmk = (int)((unsigned)lpwBuf[iEvent]); //avoid extending bit15 to sign
    if (nTmk > MAX_VTMK_NUMBER)
//      return (tmkError = TMK_BAD_NUMBER);
      continue;
    iEvPck = nTmk >> 5;
//    if (tmkMyEvents[iEvPck] & (1 << (nTmk & 0x1F)))
    if (tmkMyEvents(iEvPck) & (1 << (nTmk & 0x1F)))
    {
      fEvents += 1;
      lpwBuf[iEvent] |= 0x8000;
    }
  }
  if (fSignal)
    return -ERESTARTSYS;
  return fEvents;
}
//UINT TMK_tmkdefevent()
//{
//  HANDLE hUserEvent;
////  PKEVENT hKeEvent;
//  int fEventSet;
////  HANDLE *phEvent;
////  int iTMK;
//
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: tmkdefevent\n");
//  #endif
//
//  if (tmkNumber == -1)
//    return (tmkError = TMK_BAD_NUMBER);
//  hUserEvent = (HANDLE)(*((PUINT)(lpwIn))); //(((UINT)lpwIn[0])|((UINT)lpwIn[1]<<16));
//  fEventSet = (*((PUINT)(lpwIn+2))); //((UINT)lpwIn[2])|((UINT)lpwIn[3]<<16);
/*
  if (hUserEvent && !NT_SUCCESS(tmkError = ObReferenceObjectByHandle(
                                               hUserEvent,
                                               EVENT_ALL_ACCESS,
                                               *ExEventObjectType,
                                               KernelMode,
                                               &hKeEvent,
                                               NULL
                                               )))
  {
    return tmkError;
  }
  else
  {
    tmkError = 0;
  }
  if (!hUserEvent)
    hKeEvent = NULL;
  phEvent = &(aTMK[tmkNumber].hEvent);
  if (*phEvent && *phEvent != hKeEvent)
  {
*/
/*
    for (iTMK = 0; iTMK <= MAX_TMK_NUMBER; ++iTMK)
    {
      if (iTMK == tmkNumber)
        continue;
      if (*phEvent == aTMK[iTMK].hEvent)
        break;
    }
    if (iTMK > MAX_TMK_NUMBER)
*/
/*
    {
//      ObDereferenceObject(*phEvent);
    //VWIN32_CloseVxDHandle(*hEvent);
    }
  }
//tmkdefevent(*phEvent=hUserEvent, fEventSet);
  *phEvent = hKeEvent;
*/
//  return tmkError;
//}
UINT TMK_tmkgetevd()
{
//  void *lpdOut;
//  void *lpdEvD;
  pTListEvD pEvD;

  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpdOut = lpwOut;
  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "Tmk1553b: tmkgetevd(%d).\n", tmkNumber);
  #endif
  while (cEvData[tmkNumber])
  {
    pEvD = &(aEvData[tmkNumber][iEvDataBegin[tmkNumber]]);

  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "Tmk1553b: pEvd->nInt = %d.\n", pEvD->nInt);
  #endif
    if (pEvD->nInt)
    {
      if (pEvD->awEvData[2])
        DpcIExcBC(tmkNumber, pEvD);
  //    lpdEvD = &(aEvData[tmkNumber][iEvDataBegin[tmkNumber]]);
  //    *((UINT*)lpdOut) = *((UINT*)lpdEvD);
  //    *(++(UINT*)lpdOut) = *(++(UINT*)lpdEvD);
  //    *(++(UINT*)lpdOut) = *(++(UINT*)lpdEvD);
      memcpy(
          lpwOut,
          pEvD,
          12
          );
#ifdef LINUX_SWTIMER
      dwTimeCount[tmkNumber] = aTimeCount[tmkNumber][iEvDataBegin[tmkNumber]];
#endif //LINUX_SWTIMER
      break;
    }

    iEvDataBegin[tmkNumber] = (iEvDataBegin[tmkNumber] + 1) & (EVENTS_SIZE-1);
    --cEvData[tmkNumber];
  }

  if (cEvData[tmkNumber])
  {
    iEvDataBegin[tmkNumber] = (iEvDataBegin[tmkNumber] + 1) & (EVENTS_SIZE-1);
    --cEvData[tmkNumber];
  }
  else
  {
//    *((UINT*)lpdOut) = 0;
//    *(++(UINT*)lpdOut) = 0;
//    *(++(UINT*)lpdOut) = 0;
    memset(
        lpwOut,
        0,
        12
        );
  }

//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: tmkgetevd");
//  #endif
  if (cEvData[tmkNumber] == 0)
  {
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG " %X ", tmkEvents[tmkNumber>>5]);
//  #endif
    tmkEvents[tmkNumber>>5] &= ~(1<<(tmkNumber&0x1F));
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG " %X ", tmkEvents[tmkNumber>>5]);
//  #endif
  }
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "\n");
//  #endif

  return tmkError;
}
UINT TMK_tmkgetinfo()
{
//  TTmkConfigData *pUserTmkConfig;
//  char *pchS, *pchD;

  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  pUserTmkConfig = (TTmkConfigData*)(*((PUINT)(lpwIn))); //((UINT)(lpwIn[0])+(((UINT)(lpwIn[1]))<<16));
  memcpy(
      lpwBuf,
      &aTmkConfig[mrtselected()],
      sizeof(TTmkConfigData)
      );
/*
  pTmkConfig->nType = aTmkConfig[tmkNumber].nType;
  pchD = pTmkConfig->szName;
  pchS = aTmkConfig[tmkNumber].szName;
  while (*pchD++ = *pchS++);
  pTmkConfig->wPorts1 = aTmkConfig[tmkNumber].wPorts1;
  pTmkConfig->wPorts2 = aTmkConfig[tmkNumber].wPorts2;
  pTmkConfig->wIrq1 = aTmkConfig[tmkNumber].wIrq1;
  pTmkConfig->wIrq2 = aTmkConfig[tmkNumber].wIrq2;
  pTmkConfig->wIODelay = tmkiodelay(GET_IO_DELAY);
*/
  return tmkError;
}
UINT TMK_getversion()
{
  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "Tmk1553b: TMK_GetVersion\n");
  #endif

//  lpwOut[0] = (USHORT)((TMK_VER_HI<<8)|TMK_VER_LO);
//  lpwOut[1] = 0;
//  return tmkError;
  return ((TMK_VER_HI<<8)|TMK_VER_LO);
}

/*
UINT TMK_bcdefintnorm()
{
  return tmkError;
}
UINT TMK_bcdefintexc()
{
  return tmkError;
}
UINT TMK_bcdefintx()
{
  return tmkError;
}
UINT TMK_bcdefintsig()
{
  return tmkError;
}
*/
UINT TMK_bcreset()
{
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: bcreset\n");
//  #endif

  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  if (tmkgetmode() == MRT_MODE)
    return (tmkError = TMK_BAD_FUNC);
  bcreset();
  return tmkError;
}
UINT TMK_bc_def_tldw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  return tmkError;
}
UINT TMK_bc_enable_di()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  return tmkError;
}
UINT TMK_bc_disable_di()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  return tmkError;
}
UINT TMK_bcdefirqmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcdefirqmode(lpwIn[0]);
  return tmkError;
}
UINT TMK_bcgetirqmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = bcgetirqmode();
//  return tmkError;
  return bcgetirqmode();
}
UINT TMK_bcgetmaxbase()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = bcgetmaxbase();
//  return tmkError;
  return bcgetmaxbase();
}
UINT TMK_bcdefbase()
{
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: bcdefbase\n");
//  #endif

  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcdefbase(lpwIn[0]);
  return tmkError;
}
UINT TMK_bcgetbase()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = bcgetbase();
//  return tmkError;
  return bcgetbase();
}
UINT TMK_bcputw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcputw(lpwIn[0],lpwIn[1]);
  return tmkError;
}
UINT TMK_bcgetw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = bcgetw(lpwIn[0]);
//  return tmkError;
  return bcgetw(lpwIn[0]);
}
UINT TMK_bcgetansw()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  res = bcgetansw(lpwIn[0]);
  lpwOut[0] = (USHORT)(LOWORD(res));
  lpwOut[1] = (USHORT)(HIWORD(res));
  return tmkError;
}
UINT TMK_bcputblk()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcputblk(lpwIn[0],lpwBuf,lpwIn[1]);
  return tmkError;
}
UINT TMK_bcgetblk()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcgetblk(lpwIn[0],lpwBuf,lpwIn[1]);
  return tmkError;
}
UINT TMK_bcdefbus()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcdefbus(lpwIn[0]);
  return tmkError;
}
UINT TMK_bcgetbus()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = bcgetbus();
//  return tmkError;
  return bcgetbus();
}
UINT TMK_bcstart()
{
  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "Tmk1553b: bcstart\n");
  #endif

  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcstart(lpwIn[0],lpwIn[1]);
  return tmkError;
}
UINT TMK_bcstartx()
{
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: bcstartx\n");
//  #endif
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcstartx(lpwIn[0],lpwIn[1]);
  return tmkError;
}
UINT TMK_bcdeflink()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcdeflink(lpwIn[0],lpwIn[1]);
  return tmkError;
}
UINT TMK_bcgetlink()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  res = bcgetlink();
  lpwOut[0] = (USHORT)(LOWORD(res));
  lpwOut[1] = (USHORT)(HIWORD(res));
  return tmkError;
}
UINT TMK_bcstop()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = bcstop();
//  return tmkError;
  return bcstop();
}
UINT TMK_bcgetstate()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  res = bcgetstate();
  lpwOut[0] = (USHORT)(LOWORD(res));
  lpwOut[1] = (USHORT)(HIWORD(res));
  return tmkError;
}

/*
UINT TMK_rtdefintcmd()
{
  return tmkError;
}
UINT TMK_rtdefinterr()
{
  return tmkError;
}
UINT TMK_rtdefintdata()
{
  return tmkError;
}
*/
UINT TMK_rtreset()
{
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: rtreset\n");
//  #endif
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtreset();
  return tmkError;
}
UINT TMK_rtdefirqmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtdefirqmode(lpwIn[0]);
  return tmkError;
}
UINT TMK_rtgetirqmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetirqmode();
//  return tmkError;
  return rtgetirqmode();
}
UINT TMK_rtdefmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtdefmode(lpwIn[0]);
  return tmkError;
}
UINT TMK_rtgetmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetmode();
//  return tmkError;
  return rtgetmode();
}
UINT TMK_rtgetmaxpage()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetmaxpage();
//  return tmkError;
  return rtgetmaxpage();
}
UINT TMK_rtdefpage()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtdefpage(lpwIn[0]);
  return tmkError;
}
UINT TMK_rtgetpage()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetpage();
//  return tmkError;
  return rtgetpage();
}
UINT TMK_rtdefpagepc()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtdefpagepc(lpwIn[0]);
  return tmkError;
}
UINT TMK_rtdefpagebus()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtdefpagebus(lpwIn[0]);
  return tmkError;
}
UINT TMK_rtgetpagepc()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetpagepc();
//  return tmkError;
  return rtgetpagepc();
}
UINT TMK_rtgetpagebus()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetpagebus();
//  return tmkError;
  return rtgetpagebus();
}
UINT TMK_rtdefaddress()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtdefaddress(lpwIn[0]);
  return tmkError;
}
UINT TMK_rtgetaddress()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetaddress();
//  return tmkError;
  return rtgetaddress();
}
UINT TMK_rtdefsubaddr()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtdefsubaddr(lpwIn[0],lpwIn[1]);
  return tmkError;
}
UINT TMK_rtgetsubaddr()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetsubaddr();
//  return tmkError;
  return rtgetsubaddr();
}
UINT TMK_rtputw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtputw(lpwIn[0],lpwIn[1]);
  return tmkError;
}
UINT TMK_rtgetw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetw(lpwIn[0]);
//  return tmkError;
  return rtgetw(lpwIn[0]);
}
UINT TMK_rtputblk()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtputblk(lpwIn[0],lpwBuf,lpwIn[1]);
  return tmkError;
}
UINT TMK_rtgetblk()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtgetblk(lpwIn[0],lpwBuf,lpwIn[1]);
  return tmkError;
}
UINT TMK_rtsetanswbits()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtsetanswbits(lpwIn[0]);
  return tmkError;
}
UINT TMK_rtclranswbits()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtclranswbits(lpwIn[0]);
  return tmkError;
}
UINT TMK_rtgetanswbits()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetanswbits();
//  return tmkError;
  return rtgetanswbits();
}
UINT TMK_rtgetflags()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtgetflags(lpwBuf,lpwIn[0],lpwIn[0],lpwIn[1]);
  return tmkError;
}
UINT TMK_rtputflags()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtputflags(lpwBuf,lpwIn[0],lpwIn[0],lpwIn[1]);
  return tmkError;
}
UINT TMK_rtsetflag()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtsetflag();
  return tmkError;
}
UINT TMK_rtclrflag()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtclrflag();
  return tmkError;
}
UINT TMK_rtgetflag()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetflag(lpwIn[0],lpwIn[1]);
//  return tmkError;
  return rtgetflag(lpwIn[0],lpwIn[1]);
}
UINT TMK_rtgetstate()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetstate();
//  return tmkError;
  return rtgetstate();
}
UINT TMK_rtbusy()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtbusy();
//  return tmkError;
  return rtbusy();
}
UINT TMK_rtlock()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtlock(lpwIn[0],lpwIn[1]);
  return tmkError;
}
UINT TMK_rtunlock()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtunlock();
  return tmkError;
}
UINT TMK_rtgetcmddata()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetcmddata(lpwIn[0]);
//  return tmkError;
  return rtgetcmddata(lpwIn[0]);
}
UINT TMK_rtputcmddata()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtputcmddata(lpwIn[0],lpwIn[1]);
  return tmkError;
}
/*
UINT TMK_mtdefintx()
{
  return tmkError;
}
UINT TMK_mtdefintsig()
{
  return tmkError;
}
*/
UINT TMK_mtreset()
{
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: mtreset\n");
//  #endif

  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mtreset();
  return tmkError;
}
UINT TMK_mtgetsw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = mtgetsw();
//  return tmkError;
  return mtgetsw();
}

UINT TMK_rtenable()
{
  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "Tmk1553b: rtenable\n");
  #endif

  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtenable(lpwIn[0]);
//  return tmkError;
  return rtenable(lpwIn[0]);
}
UINT TMK_mrtgetmaxn()
{
//  lpwOut[0] = (USHORT)mrtgetmaxn();
//  return tmkError;
  return mrtgetmaxn();
}
UINT TMK_mrtconfig()
{
  int iRt, iRt0, nRt;
  USHORT aw0[30];

  #ifdef MY_DBG
  printk(MY_KERN_DEBUG "Tmk1553b: mrtconfig\n");
  #endif

  if (tmkselect(tmkNumber = (int)lpwIn[0]) || tmkgetmode() != MRT_MODE || aTMK[tmkNumber].hProc)
  {
    tmkselect(tmkNumber = hlnCurProc->nSelectedTMK);
    tmkError = TMK_BAD_NUMBER;
//    lpwOut[0] = lpwOut[1] = 0;
    nRt = iRt0 = 0;
  }
  else
  {
    tmkreconfig();
//    tmkError = 0;
    bcreset();
    switch (aTmkConfig[tmkNumber].nType)
    {
    case MRTX:
    case MRTXI:
      memset(aw0, 0, sizeof(aw0)); // RtlZeroMemory(aw0, sizeof(aw0));
      rtputflags(aw0, RT_RECEIVE, 1, 30);
      break;
    }
    hlnCurProc->fTMK[tmkNumber] = 1;
    hlnCurProc->maskTMK[tmkNumber>>5] |= (1<<(tmkNumber&0x1F));
    hlnCurProc->nSelectedTMK = tmkNumber;
    aTMK[tmkNumber].hProc = hCurProc;
    aTMK[tmkNumber].hlnProc = hlnCurProc;
    tmkEvents[tmkNumber>>5] &= ~(1<<(tmkNumber&0x1F));
    iEvDataBegin[tmkNumber] = iEvDataEnd[tmkNumber] = cEvData[tmkNumber] = 0;
    aTmkConfig[tmkNumber].fMRT = 1;
    iRt0 = mrtgetrt0();
    nRt = mrtgetnrt();
    for (iRt = iRt0+nRt-1; iRt >= iRt0; --iRt)
    {
      if (aTMK[iRt].hProc == 0)
      {
        hlnCurProc->fTMK[iRt] = 1;
        hlnCurProc->maskTMK[iRt>>5] |= (1<<(iRt&0x1F));

        aTMK[iRt].hProc = hCurProc;
        aTMK[iRt].hlnProc = hlnCurProc;

        tmkEvents[iRt>>5] &= ~(1<<(iRt&0x1F));
        iEvDataBegin[iRt] = iEvDataEnd[iRt] = cEvData[iRt] = 0;
      }
    }
//    lpwOut[0] = (USHORT)iRt0;
//    lpwOut[1] = (USHORT)nRt;
  }
  return ((nRt<<16) | iRt0);
}
UINT TMK_mrtselected()
{
//  lpwOut[0] = (USHORT)((tmkNumber != -1) ? mrtselected() : -1);
//  return tmkError;
  return ((tmkNumber != -1) ? mrtselected() : -1);
}
UINT TMK_mrtgetstate()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = (USHORT)mrtgetstate();
//  return tmkError;
  return mrtgetstate();
}
UINT TMK_mrtdefbrcsubaddr0()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mrtdefbrcsubaddr0();
  return tmkError;
}
UINT TMK_mrtreset()
{
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: mrtreset\n");
//  #endif

  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  if (tmkgetmode() != MRT_MODE)
    return (tmkError = TMK_BAD_FUNC);
  bcreset();
  return tmkError;
}

UINT TMK_tmktimer()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = tmktimer(lpwIn[0]);
//  return tmkError;
  return tmktimer(lpwIn[0]);
}

UINT TMK_tmkgettimer()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  res = tmkgettimer();
  lpwOut[0] = (USHORT)(LOWORD(res));
  lpwOut[1] = (USHORT)(HIWORD(res));
  return tmkError;
}

UINT TMK_tmkgettimerl()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = tmkgettimerl();
//  return tmkError;
  return tmkgettimerl();
}

UINT TMK_bcgetmsgtime()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  res = bcgetmsgtime();
  lpwOut[0] = (USHORT)(LOWORD(res));
  lpwOut[1] = (USHORT)(HIWORD(res));
  return tmkError;
}

UINT TMK_rtgetmsgtime()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  res = rtgetmsgtime();
  lpwOut[0] = (USHORT)(LOWORD(res));
  lpwOut[1] = (USHORT)(HIWORD(res));
  return tmkError;
}

UINT TMK_tmkgethwver()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = tmkgethwver();
//  return tmkError;
  return tmkgethwver();
}

UINT TMK_tmkgetevtime()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
#ifdef LINUX_SWTIMER
  res = dwTimeCount[tmkNumber];
#else
  res = 0;
#endif //LINUX_SWTIMER
  lpwOut[0] = (USHORT)(LOWORD(res));
  lpwOut[1] = (USHORT)(HIWORD(res));
  return tmkError;
}

UINT TMK_tmkswtimer()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
#ifdef LINUX_SWTIMER
  if (lpwIn[0] != GET_TIMER_CTRL)
  {
    afUseEvTime[tmkNumber] = lpwIn[0] & 0x0400;
    if (afUseEvTime[tmkNumber])
    {
      wSwTimerCtrl = (lpwIn[0] & 0x0780) | 0x2000;

#define DividerShift0 ((int)((wSwTimerCtrl & 0x0380) >> 7))
#define Divider0 (1 << DividerShift0)
      dwSecMul = 1000000 >> DividerShift0; // int result for 0..6 bits shift
      nNsecDiv = 1000 << DividerShift0;

      fUseEvTime = 1;
    }
    else
    {
      swtimerall();
    }
  }
  return wSwTimerCtrl;
#else
  return 0;
#endif //LINUX_SWTIMER
}

UINT TMK_tmkgetswtimer()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
#ifdef LINUX_SWTIMER
  res = 0;
  if (fUseEvTime)
  {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,18,0)
    struct timespec64 tsTime;
    ktime_get_raw_ts64(&tsTime);
#else
    struct timespec tsTime;
    getrawmonotonic(&tsTime);
#endif
    res = (u32)tsTime.tv_sec * dwSecMul + (u32)(tsTime.tv_nsec / nNsecDiv);
  }
#else
  res = 0;
#endif //LINUX_SWTIMER
  lpwOut[0] = (USHORT)(LOWORD(res));
  lpwOut[1] = (USHORT)(HIWORD(res));
  return tmkError;
}

UINT TMK_tmktimeout()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = tmktimeout(lpwIn[0]);
//  return tmkError;
  return tmktimeout(lpwIn[0]);
}

UINT TMK_mrtdefbrcpage()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mrtdefbrcpage(lpwIn[0]);
  return tmkError;
}

UINT TMK_mrtgetbrcpage()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = mrtgetbrcpage();
//  return tmkError;
  return mrtgetbrcpage();
}

UINT TMK_mbcinit()
{
  mbcinit(lpwIn[0]);
  return tmkError;
}

UINT TMK_mbcpreparex()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mbcpreparex(lpwIn[0],lpwIn[1],lpwIn[2],lpwIn[3]);
  return tmkError;
}

UINT TMK_mbcstartx()
{
  mbcstartx(lpwIn[0]);
  return tmkError;
}

UINT TMK_mbcalloc()
{
  UINT mbcId;
//  lpwOut[0] = mbcalloc();
//  return tmkError;
  mbcId = (UINT)mbcalloc();
  if (mbcId != MBC_ALLOC_FAIL)
    hlnCurProc->fMBC[mbcId] = 1;
  return mbcId;
}

UINT TMK_mbcfree()
{
  mbcfree(lpwIn[0]);
  hlnCurProc->fMBC[lpwIn[0]] = 0;
  return tmkError;
}

UINT TMK_tmkwaiteventsflag()
{
  int flag = *((PUINT)lpwIn + 1);

  hlnCurProc->waitFlag = flag;
  if (flag)
    wake_up_interruptible(&(hlnCurProc->wq));
  return 0;
}

UINT TMK_mwreset()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mwreset();
  return tmkError;
}

UINT TMK_mwtreset()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mwtreset();
  return tmkError;
}

UINT TMK_mwcdw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mwcdw(lpwIn[0],lpwIn[1]);
  return tmkError;
}

UINT TMK_mtgetsw2()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  res = mtgetsw2();
  lpwOut[0] = (USHORT)(LOWORD(res));
  lpwOut[1] = (USHORT)(HIWORD(res));
  return tmkError;
}

UINT TMK_mtgetswa()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = mtgetswa();
//  return tmkError;
  return mtgetswa();
}

UINT TMK_tmktxrxmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = tmktxrxmode(lpwIn[0]);
//  return tmkError;
  return tmktxrxmode(lpwIn[0]);
}

UINT TMK_bcgetmsgsw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = mtgetsw();
//  return tmkError;
  return bcgetmsgsw();
}

UINT TMK_rtgetmsgsw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = mtgetsw();
//  return tmkError;
  return rtgetmsgsw();
}

UINT TMK_rtgap()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgap(lpwIn[0]);
//  return tmkError;
  return rtgap(lpwIn[0]);
}

UINT TMK_rtallocsabuf()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetflag(lpwIn[0],lpwIn[1]);
//  return tmkError;
  return rtallocsabuf(lpwIn[0],lpwIn[1]);
}
UINT TMK_rtdefbuf()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtdefbuf(lpwIn[0]);
  return tmkError;
}
UINT TMK_rtgetbuf()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetbuf();
//  return tmkError;
  return rtgetbuf();
}
UINT TMK_rtdeflink()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtdeflink(lpwIn[0]);
  return tmkError;
}
UINT TMK_rtgetlink()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetlink();
//  return tmkError;
  return rtgetlink();
}
UINT TMK_rtdeflinkbus()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtdeflinkbus(lpwIn[0]);
  return tmkError;
}
UINT TMK_rtgetlinkbus()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetlinkbus();
//  return tmkError;
  return rtgetlinkbus();
}

UINT TMK_tmkdac()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = tmkdac(lpwIn[0]);
//  return tmkError;
  return tmkdac(lpwIn[0]);
}

UINT TMK_rtdefsubaddrm()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtdefsubaddrm(lpwIn[0],lpwIn[1]);
  return tmkError;
}
UINT TMK_rtgetbase()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetbase();
//  return tmkError;
  return rtgetbase();
}
UINT TMK_rtgetbasebus()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtgetbasebus();
//  return tmkError;
  return rtgetbasebus();
}

UINT TMK_mtdefmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mtdefmode(lpwIn[0]);
  return tmkError;
}
UINT TMK_mtgetmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = mtgetmode();
//  return tmkError;
  return mtgetmode();
}

UINT TMK_bcdefmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcdefmode(lpwIn[0]);
  return tmkError;
}
UINT TMK_bcgetmode()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = bcgetmode();
//  return tmkError;
  return bcgetmode();
}

UINT TMK_bcstartctla()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcstartctla(lpwIn[0],lpwIn[1],lpwIn[2],lpwIn[3]);
  return tmkError;
}
UINT TMK_bcstarta()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcstarta(lpwIn[0]);
  return tmkError;
}
UINT TMK_bcdefctla()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcdefctla(lpwIn[0],lpwIn[1],lpwIn[2],lpwIn[3]);
  return tmkError;
}
UINT TMK_bcgetctla()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = bcgetctla(lpwIn[0]);
//  return tmkError;
  return bcgetctla(lpwIn[0]);
}

UINT TMK_mbcpreparea()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mbcpreparea(lpwIn[0],lpwIn[1]);
  return tmkError;
}
UINT TMK_mbcstarta()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mbcstarta(lpwIn[0]);
  return tmkError;
}

UINT TMK_tmkoptions()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  res = tmkoptions((U32)lpwIn[0] | ((U32)lpwIn[1] << 16));
  lpwOut[0] = (USHORT)(LOWORD(res));
  lpwOut[1] = (USHORT)(HIWORD(res));
  return tmkError;
}

UINT TMK_mtfilter()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = mtfilter(lpwIn[0],lpwIn[2],lpwIn[3]);
//  return tmkError;
  return mtfilter(lpwIn[0],lpwIn[2],lpwIn[3]);
}
UINT TMK_rtlegal()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = rtlegal(lpwIn[0],lpwIn[2],lpwIn[3]);
//  return tmkError;
  return rtlegal(lpwIn[0],lpwIn[2],lpwIn[3]);
}

UINT TMK_bcclrmsgtime()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcclrmsgtime();
  return tmkError;
}
UINT TMK_mtclrmsgtime()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mtclrmsgtime();
  return tmkError;
}
UINT TMK_rtclrmsgtime()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtclrmsgtime();
  return tmkError;
}

UINT TMK_bcclrmsgsw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  bcclrmsgsw();
  return tmkError;
}
UINT TMK_mtclrmsgsw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mtclrmsgsw();
  return tmkError;
}
UINT TMK_rtclrmsgsw()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  rtclrmsgsw();
  return tmkError;
}

UINT TMK_mwrtreset()
{
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: mwrtreset\n");
//  #endif

  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mwrtreset();
  return tmkError;
}
UINT TMK_mtrtreset()
{
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: mtrtreset\n");
//  #endif

  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mtrtreset();
  return tmkError;
}
UINT TMK_mwtrtreset()
{
//  #ifdef MY_DBG
//  printk(MY_KERN_DEBUG "Tmk1553b: mwtrtreset\n");
//  #endif

  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mwtrtreset();
  return tmkError;
}
UINT TMK_mtgetmaxmaxbase()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
//  lpwOut[0] = mtgetmaxmaxbase();
//  return tmkError;
  return mtgetmaxmaxbase();
}
UINT TMK_mtdefmaxbase()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  mtdefmaxbase(lpwIn[0]);
  return tmkError;
}

UINT TMK_tmkbuserr()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  tmkbuserr(lpwIn[0]);
  return tmkError;
}

UINT TMK_tmkinfo()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  res = tmkinfo(lpwIn[0], lpwIn[1]);
  lpwOut[0] = (USHORT)(LOWORD(res));
  lpwOut[1] = (USHORT)(HIWORD(res));
  return tmkError;
}

UINT TMK_tmkrawion()
{
  U32 res;
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  res = tmkrawion(pdwUserRawBufOO, dwUserRawLenO, pdwUserRawBufII, dwUserRawLenI);
  return res;
}

UINT TMK_rtputblkifnb()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  return rtputblkifnb(lpwIn[0],lpwBuf,lpwIn[1]);
}
UINT TMK_rtgetblkifnb()
{
  if (tmkNumber == -1)
    return (tmkError = TMK_BAD_NUMBER);
  return rtgetblkifnb(lpwIn[0],lpwBuf,lpwIn[1]);
}

UINT TMK_tmknop()
{
  return -ENOTTY;
}

#include "tmkisr.c"
