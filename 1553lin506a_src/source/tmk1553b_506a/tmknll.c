//;***************************************************************************;
//;*      TMKNLL v8.06.1                                                     *;
//;*      - for TMKLL4.OBJ v8.06 for DOS.                                    *;
//;*      - for TMKLL4P.OBJ v8.06 for DPMI.                                  *;
//;*      - for TMK1553B.SYS v5.06 for Windows NT4.                          *;
//;*      - for 1553Bwdm.sys v5.06 for Windows 98/ME/2K/XP/Vista/7/8/10.     *;
//;*      - for TMK1553B v5.06 for Linux.                                    *;
//;*      - for TMK1553B v5.06 for QNX4.                                     *;
//;*      - for TMK1553B v5.06 for QNX6.                                     *;
//;*      ELCUS, 1995,2024.                                                  *;
//;***************************************************************************;
//6.00
// - ASM->C
// - TA minimal support
//6.01
// - added __ for internal vars
// - bits = 0 for TA in rtgetanswbits
//6.02
// - TA timer support (w/o user timer interrupts)
// - QNX4 support (w/o MRTX)
// - LINUX support
// - shared irq processing fixed (DOS/QNX4)
// - SMP support for TMKX, MRTX, TA serves:
//     TMKX writes in irq TMK_CtrlPort (RT mode)
//     MRTX writes in irq TMK_CtrlPort, TMK_AddrPort, TMK_DataPort
//     TA writes in irq TA_ADDR, TA_TIMCR, TA_MODE1
// - TA1-PCI4 support
//6.03
// - 80186/80188 support (DOS)
// - QNX4 support with MRTX (used tmkisr.c)
//6.04
// - QNX4 support for VME cards
// - commented __tmkDrvType[num] = 0xFFFF; (in tmkconfig on irq error)
// - TA timeout control support (tmktimeout function)
// - TA mt def/get mode functions
// - tmk def/get dac functions for TH6-PCI
// - rtsetanswbits bug for xTMK400 cards fixed
// - bcdeflink with CX_NOSIG fixed
// - GET_DIS_IRQ_SMP/REST_IRQ_SMP put out of ...PokeTA/...PeekTA
// - added Virtual RTs reset in bcreset (mrtreset)
//7.00
// - MRTA, MRTAI support
//7.01
// - QNX6 support (with separate tmkisr.c)
// - mbcinit, mbcpreparex, mbcstartx, mbcalloc, mbcfree functions
// - MRTA bcreset fixed (possible fake tester interrupt)
// - wrong type bug fixed (in __RT_DIS_MASK, __RT_BRC_MASK)
//7.02
// - updated tmkisr.c for MRTA and QNX6
// - bcstop for TA cards fixed
// - tmktimer single loop fixed
//7.03
// - mrtdefmaxnrt function
// - check of unitialized driver in tmkdone
// - TMKX/MRTX RT double interrupt reset
// - SMP support for TMKX,MRTX,TA,MRTA for Windows (__tmkIrqSpinLockSMP)
// - SMP IRQ for MRTX,MRTA fixed (DrvRtPoke/PeekIrqMRTX, tmkisr.c)
// - added state/timetag copy for brc cmds in MRTA
// - TMK1553B_NOCONFIGH option for LINUX added
//7.04
// - DOS TA timer (other) interrupt restore fixed
// - DrvFlagMode->DrvFlagModeTA reworked
// - TA/MRTA flags off bug fixed
//7.05
// - MRTX(I)/MRTA(I) support for DOS
// - small TMK_INT_OTHER fix (pEvD->awEvData[2] = 0;) in tmkisr.c
//7.06
// - DPMI (DOS 32 bit) support
// - PCI.C for DOS reworked into asm
// - DefIrqAL -> SetIrqAL/ResetIrqAL (DOS,DPMI)
// - pEvD->nInt = 0; added for brc MRT irq when not fWriteSA (here & tmkisr.c)
// - fixed (m)rtgetstate fake active bit for old cmd on TA/MRTA
// - fixed (m)rtgetstate/rtbusy fake active bit when lock|flag on TA/MRTA
// - fixed rtputcmddata/rtgetcmddata (supported by __rtRxDataCmd, DIRQLTmksInt1)
//7.07
// - DOS/DPMI shared interrupt restore on exit fixed (SetIrqCopy added)
//7.08
// - Windows GET_DIS_IRQ/REST_IRQ changed to support 64-bit (no asm)
//7.09
// - DOS/DPMI IntX base fixed
// - bc/mt/rtgetmsgsw added
// - rtgap added
// - MRTA broadcast cmd data extra write to next MRTA fixed in DIRQLTmksInt1
// - MRTX broadcast last cmd/cmd data extra write if one RT enabled fixed in DIRQLTmksInt1
//7.10
// - RT buffering for TA added:
// -- SABUF_STD mode only,
// -- functions rtallocsabuf, rtdef/getbuf, rtdef/getlink, rtdef/getlinkbus,
// -- rtgetbufirq for DOS and QNX4,
// -- send buf number through pevd[4] for further interrupt processing
// - DrvRtWMode for TA/MRTA fixed no previous unlock in subsequent locks
// - TA1-PE2 support
//7.11
// - TA1-3U-2R support (with led ctrl in DOS, see tmk...cwbits)
//7.12
// - tmkdac added instead of tmk[defdac/getdac]
// - fix flags on error messages on TA/MRTA (DIRQLTmksInt1)
// - TA1-PE4 support
// - TA1-PE-32RT support
//7.20
// - MW, MWT (MW+MT) modes
// - mwreset, mwtreset functions
// - DrvMtMwReset helper function
// - tmktxrxmode, mwcdw functions
// - mtgetsw2, mtgetswa functions
// - mtgetsw adjustment for TA
// - commented out SetIrqCopy DOS call in nonDOS (QNX4)
// - MRT_READ/WRITE_BRC_DATA moved to tmknll.h
// - fix MRTA BusJam and FIFO_Ovf bits check in DIRQLTmksInt1
// - fix MRTA broadcast data copy to virtual RT when BUSY is set for this RT
// - fix MRTA bug in (7.12) fix flags on error messages (DIRQLTmksInt1)
// - fix MRTA extra RT err int (Int=2) for transmitting RT in RT->RT broadcast
// - limit __mrtLastBrcTxRT / hTmkT for use numbers above 255
// - fix MRTA status bit set bug in rtdefaddress in flag mode (rtflgm added)
// - improved mtgetstate for TA (read MT base from AdrTab table instead of reg)
// - fixed TA1-3U-2R led ctrl
// - __mrtA2RT U08->U16 for use numbers above 255
// - fixed virtual RT close/open/reopen (__rtAddress[x], __rtControls1[x], etc.)
//8.00
// - move to 8.00 because of many changes and new MW modes and RT buffering
// - pciWriteConfigWord for QNX4 added
// - DrvReadOldMask added and DrvMaskTmk changed to not mask old used DOS irqs
// - SetupPCI reworked
// - mbcalloc, mbcfree, tmkdone improved
// - remove "statement/code has no effect" warnings on unused args with (void)
// - TA1-3U-2R led ctrl not only for DOS
// - TA1-MPE2 support
// - PCI enumeration reordered
// - drop nInt=2 when MRT receives broadcast data into busy subaddress (DOS)
// - fix MRTA bcreset, tmkdone for rtAddress == 0
// - fix MRTA rtreset to avoid cross RT interference
// - fix MRTA rtdefaddress to use rtdefmode settings
// - add RT address lines check in rtdefaddress for TA
// - improve rtenable to control subsequent rtdefaddress
// - fix MRTA table write in RTs with unassigned address
// - RT buffering extended for TA and added for MRTA:
// -- SABUF_JOINT mode with single buffer,
// -- SABUF_HEAP mode
// - improve rtgetmsgsw for MRTA with duplicate err bit 13 to bit 14 as in TA
// - extracted DpcPhysMRT, DpcVirtRTBrc from IncDpcData
// - DpcPhysMRT, DpcVirtRTBrc moved from tmkisr.c to tmknll.c (joined with DOS)
// - added tmkisrc.c for nonDOS C stuff related to tmknll.c
// - rtdefsubaddrm, rtgetbase, rtgetbasebus functions
// - nInt=5 TMK_INT_BUF_SKIP if MRTA brc data read delayed until new message
//   (only for new non-SABUF_OFF brc subaddr modes)
// - added reset before __tmkIrqBit/__tmkIrqPort determine in DOS/QNX4
// - fixed TA monitor result ('integrated bit' processing in TA/TX conversion)
// - fixed TA monitor result (bus/fmt at bus switching)
// - fixed TA monitor msgsw processing in case of MW msgsw
// - bcdefmode, bcgetmode functions
// - bcXStart -> bcXAStart
// - bcstartctla, bcstarta, bcdefctla, bcgetctla functions
// - bcdefinta, UserABC (nInt=6) functions
// - mbcpreparea, mbcstarta=mbcstartx functions
// - tmkdefintoth, UserOthTMK (nInt=5) functions
// - add RT_NOERR_MODE, RT_RSREQ_MODE in TA/MRTA rtdef/getmode
// - add RT_CBSNL_MODE, RT_CBSND_MODE, RT_CNOBS_MODE in TA/MRTA rtdef/getmode
// - tmkoptions function (BC_INTA_AS_INTX, MRT_READ/WRITE_BRC_DATA)
// - extract DrvInitNTMK, DrvInitNTMKNRT, DrvInitTmk, DrvInitDrv
// - rework tmkconfig, tmkdone with above
// - tmkreconfig function for non-DOS (i.e. for standalone drivers)
// - comment out unused __tmkRAMSize
// - mtfilter, rtlegal functions
// - init all __rtControls[] in DrvInitDrv
// - avoid rtdef/set/clr/enable... writes to mode/ctrl regs if not RT/MRT_MODE
// - change relative port calculations (inherited from asm) into direct port
//   references to be more straightforward; LOCAL_PORT, SET_LOCAL_PORT added
// - bc/rt/mtclrmsgsw functions
// - bc/rt/mtclrmsgtime functions
// - fixed bcgetstate for TA
// - initial TA mtgetstate preset in DrvMtMwReset
// - add inptrw, DrvBcPeekPtrTA, DrvRtPeekPtrTA to improve raw I/O chaining
// - rework top level funcs with I/O for single return point
// - add DrvIOPro/DrvIOProV/DrvIOEpi/DrvIOSetPar framing for TA I/O
// - replace BC_INTA_AS_INTX with opposite BC_STARTA_INTA to support old TA1-USB
// - move BC_STARTA_INTA checking from interrupt to start
// - fix rtdeflinkbus for SABUF_JOINT
// - add inptrwu and add/update DrvIO related stuff for TA1-USB I/O
// - update rtput/set/clr/getflag for TA
// - fix __bcBaseBus references in IncDpcData for DOS
// - MTRT (MT+RT), MWRT (MW+RT), MWTRT (MT+MW+RT) modes
// - mtrtreset, mwrtreset, mwtrtreset functions
// - DrvMtMwRtReset helper function
// - mtgetmaxmaxbase and mtdefmaxbase functions
// - CHECK_TMK_MODE_L/N->CHECK_TMK_MODE_BCMT/N change because of MW/MT+RT
// - BCMT_MODE_L remove
// - BC_MODE only check in bcdefbus
// - eliminate amrtNumber if MRT is not used
// - define register variables
// - use __rtControls1 instead of __bcControls1 for TA MT because of MW/MT+RT
// - add MTRT_ALLOW_ANOMALY option in tmkoptions
// - added earlier initial fWriteSA value write in DpcVirtRTBrc
// - 32RTV,32RTE,32RTF PCI id preliminary support
// - fix __tmkInIrqNumber and __tmkNumber for DOS consequtive irqs
// - exclude all 32RTs in DOS/QNX4 SetupPCI when MRTA support disabled
// - add RT Rx disable for 32RTE/32RTF (__mrtRxCtrl,tmktxrxmode,DpcPhysMRT,DIRQLTmksInt1)
// - add RT_LINK_ER, BUS_ERR_*, tmkbuserr(), __tmkBusErr[] for 32RTE/32RTF
// - add separate TA_BRCST_MODE processing for all RTs in MRTA
// - add CODEC_READ to mwcdw (API change, should move to v8.01 !)
// - add MW_DT00_MODE
//8.01
// - improve rtbusy, rtgetstate, mrtgetstate, rtgetbasebus for MRTA
// - replace BRCST_MASK with TA_BRCST_MODE for MRTA internal processing
// - tmkinfo function (MT_BASE_DRV_TO_HW, MT_BASE_HW_TO_DRV)
// - change rtbusy type in DOS to U16
// - rtget/putblkifnb functions
// - allow RT start (TA) in rtenable, mtstartx with properly set RT address only
// - rewrite PCI configuration/enumeration using CFG_DID
// - add extra PCI id (TA1P..,TA2P..) preliminary support
// - write TA_LCW in MRTA with unused bits always cleared
// - tmkrawion, tmkdefintraw, UserRawTMK (nInt=255) functions
// - faster RTRT check for TA in bcstart, bcstartx, bcdeflink, mbcpreparex
// - __tmkPortsMax1, __tmkPortsMax2 added for tmkrawion
// - changed register to REGPAR macro for Watcom C compatibility
// - fixed rm Ints alloc & copy for DPMI
// - updated DrvLockRam for actual _BSS segment values for DPMI
//8.02
// - fix (DpcPhysMRT) and extend (DIRQLTmksInt1) raw ints processing
// - fix 9030 (SetupPCI)
// - update MW_DT00_MODE (mtgetmode)
// - add timer interrupt forwarding in virtual RTs for MRTA (__dmrtaTimers)
// - fix DpcPhysMRT for MRTA with virtual RTs w disabled brc
// - improve DpcPhysMRT, DpcVirtRTBrc for DOS
// - improve processing of TAI/MRTAI with disabled irq output (__tmkIrqPort2/Bit2)
// - fix outpw addr port in SetupPCI
// - add DID and SubDID parameters for tmkconfig with external PCI setup
// - extract DrvInitDrvID from SetupPCI and tmkconfig
// - improve nInt=2 brc processing (both mode and data msgs) for MRTA and MRTX
// - update rtdef/getlink[bus] for compatibility with RT_LINK_ER
// - improve PCI setup compatibility in QNX4 (pciPciBiosPresent)
// - remove interrupt callbacks (.defint.) in QNX4 (move them under QNX4INT)
// - add optional extdpcdata call for DOS
// - make unified internal intd (wRtStatus) in nInt=4 for MRTX and MRTA
// - fix __tmkRAMInWork and __tmkRAMAddr to be volatile
// - add extra __tmkPortsAddr2 check in tmkrawion
// - add __tmkIrqOff and IOPAR_TMKIRQOFF
// - rework disabled irq output processing with __tmkIrqOff (remove __tmkIrqPort2/Bit2)
// - improve tmkconfig/mrtconfig processing in DOS for MRTs
// - remove TIMER_NOSTOP bit in TIMER_OFF operation in tmktimer
// - add TSTX, TSTXI, TSTA, TSTAI types for raw io
// - fix tmkdac
// - change nonspecific lists of multiple type cases in switches to defaults
// - add DrvIONone, DrvIOStop, DrvIOSetParTmk
// - fix all DrvIOPro/V instances for reentrancy
// - add modesav to DrvMtMwReset and DrvMtMwRtReset
// - fix tmkrawion (multiple w bigram/w ram/r bigram/r ram)
// - move DrvInitAll (with its DrvIO_DRVINITALL_1) earlier in tmkconfig
// - add DrvIO_TMKSELECT_1
// - improve rtgetcmddata, rtputcmddata for __tmkIrqOff and for DrvIO
//8.03
// - add GET_RT_GAP_HW, GET_TXRX_MODE_HW, GET_TIMEOUT_HW, GET_TIMER_CTRL_HW and
//   add these GET_*_HW to tmktxrxmode, rtgap, tmktimeout, tmktimer
// - add commands 19, 20 to tmkrawion and rework tmkrawion with GET_DIS_/REST_/EXIT_*_RAW
// - SMP support for (R)TMK400, TMKMPC serves writes in irq TMK(MPC)_Ctrl(H)Port (RT mode)
// - simplify bcstart for TMKX (remove unused MT mode branch)
// - remove unused MRTX, MRTA cases in bcdefbus
// - improve GET_(DIS)_*/REST_* operations through the whole driver
// - extra move of QNX4 direct irq stuff under QNX4INT
// - remove MRTX and MRTA сases in DrvBcPoke, DrvBcPeek, bcputblk, bcgetblk
// - improve SMP __tmkRAMInWork/__tmkRAMAddr processing for MRTX (see note)
// - improve rtreset with flag mode for MRTX(I)
// - add __tmkKeepFlgM, TMK_KEEP_FLGM (use in rtreset, DrvMtMwRtReset)
// - add tmkdrvkeepflgm for non-DOS internal use
// - improve tmkoptions and __tmkOptions for MRTX, MRTA
// - fix rtputflag for MRTA
// - add DRV_FLAG_MODE_HW, DRV_FLAG_MODE_SW, DRV_FLAG_UPDATE bit constants
// - change bool __FLAG_MODE_ON into multibit __FLAG_MODE
// - improve flag mode switching for TA and MRTA (DrvFlagModeTA, rtdefmode, rtgetmode)
// - improve rtsetflag, rtclrflag, rtputflag, rtgetflag for TA and MRTA
//8.04
// - fix cooperative operations of rtgap and rtlegal/mtfilter for TA
// - change rtlegal, mtfilter in ON/OFF only control
// - rework and change rtlegal, DrvMrtaUpdateRTA for MRTA
// - add explicit fall-through comments for newer gcc
// - fix DrvRtWMode for TMKMPC in SMP environment (after 8.03)
// - add optional TMK_REAL_MAX, __tmkRealMax, __tmkUseRealMax, tmkdrvrealmax
// - add TMK_INFO_MAX to tmkinfo
// - add LINUX_SWTIMER stuff
// - add __bcmtStateX and IOPAR_BCMTSTATEX as TMKX-like stopped state for TA
// - fix bcgetstate, mtgetstate for TA with __bcmtStateX in high word as in TMKX
// - fix mtgetstate for TA after IntX (writeback stopped msgBase in DIRQLTmksInt1)
// - improve TA monitor with MTA_STOP_BASE to avoid after-CX_STOP random base write
// - improve TA monitor: drop unused msgSW read in DIRQLTmksInt1 for CX_CONT|CX_NOINT
//8.05
// - add __tmkCBSND, TMK_CBSND
// - add tmkdrvcbsnd for non-DOS internal use
// - add __tmkWrapSA, __tmkWrapNoST5, __tmkWrapInt, TMK_WRAP_SA, TMK_WRAP_ST, TMK_WRAP_INT
// - add tmkdrvwrapsa for non-DOS internal use
// - extend tmkinfo with TMK_INFO_KEEP_FLGM, TMK_INFO_CBSND, TMK_INFO_WRAP_SA
// - add TMKDRV_FUNCS in DOS for tmkdrvkeepflgm, tmkdrvcbsnd, tmkdrvwrapsa
// - add minimal GENER1, GENER2 interrupt processing (DIRQLTmksInt1) for TMKX
// - improve rtget/putblkifnb and rtbusy functions
// - improve TA RT (TA_MODE2, __rtControls1) with addr=31 while addr not set (rtreset, DrvMtMwRtReset, rtdefaddress, rtenable)
// - change rtenable to work always (earlier only if new and old states differ)
// - add RT_USE_ADDR_PARITY option in tmkoptions, rtdefaddress, rtgetaddress
// - change rtdefaddress to disable RT if called with invalid address
// - add commands 21-24 to tmkrawion
// - remove rawio i/o delays in DIRQLTmksInt1
// - add abstract loop limits in hardware while loops (limit vars)
// - fix TMKXI/MRTXI PCI2 RT with double writes to TMKX_Ctrl
// - add brc sw buffer page access for MRTX and MRTA (mrtdefbrcpage, etc...)
// - extend tmkinfo with MRT_BRC_HW_PAGES, MRT_BRC_HW_PAGE_0, MRT_BRC_SW_PAGES, MRT_BRC_SW_PAGE_0
// - improve MRTX RTs switching in mrt/rtgetstate, rtbusy, rtget/putblkifnb
// - improve MRTX rtbusy, rtget/putblkifnb (add RT page check) as in rtgetstate
// - improve tmkrawion with undefined cmd check
// - add RT_SHOW_BRC_STATE option in tmkoptions, rtgetstate, DIRQLTmksInt1, DpcPhysMRT
//8.06
// - add tmkdrvinit for non-DOS internal use (fix init w/o tmkconfig)
// - fix QNX4VME compilation issues
// - add QNX4VME TE1-6UV/TSV and TA1-BV6 led ctrl in tmkset/clr/getcwbits
// - add QNX4VME TE1-6UV/TSV tmktimeout support
// - add __mtStopBaseTA to improve mtgetstate for TA
// - relocate MT part inside DrvMtMwRtReset
// - add TA_MSGA reg clear for TA in bcreset
//8.06.1
// - add fallthrough macros along with fall-through comments as gcc settings changed in LINUX (8.07)

#ifndef NOT_INCLUDE_DEFS
#include "tmkndefs.h"
#endif

#ifdef DDKWDM
#ifndef WINDOWS
#define WINDOWS
#endif
#include "pch.h"
#pragma warning(disable:4242)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
//        void outpw(USHORT port, USHORT data);
//        USHORT inpw(USHORT port);
//        void outp(USHORT port, UCHAR data);
//        UCHAR inp(USHORT port);
#endif //def DDKWDM

#ifdef DDKNT
#include <ntddk.h>
#define WINDOWS
#endif //def DDKNT

//#ifndef STATIC_TMKNUM
//#define DYNAMIC_TMKNUM
//#endif

#define VOID void

#define BOOL char

#ifdef __cplusplus
  #define __CPPARGS ...
  #define __INLINE inline
#else
  #define __CPPARGS void
  #define __INLINE
#endif

//#define INLINE inline

#ifdef __WATCOMC__
#define REGPAR
#else
#define REGPAR register
#endif

#ifdef DOS

/****************************************************************************/
/*    PCI.C v1.2 provides API to PCI BIOS. ELCUS, 2001, 2016.               */
/****************************************************************************/

#ifdef DOS32
#include <i86.h> /* regs, sregs, int86x() int386x() */
#else
#include <dos.h>
#endif //def DOS32

#define PCI_BIOS_PRESENT 0xB101
#define FIND_PCI_DEVICE 0xB102
#define FIND_PCI_CLASS_CODE 0xB103
#define GENERATE_SPECIAL_CYCLE 0xB106
#define READ_CONFIG_BYTE 0xB108
#define READ_CONFIG_WORD 0xB109
#define READ_CONFIG_DWORD 0xB10A
#define WRITE_CONFIG_BYTE 0xB10B
#define WRITE_CONFIG_WORD 0xB10C
#define WRITE_CONFIG_DWORD 0xB10D
#define GET_IRQ_ROUTING_OPTIONS 0xB10E
#define SET_PCI_IRQ 0xB10F

#define SUCCESSFUL 0

//unsigned int bcdVersion;
//unsigned int wMechanism;
//int nMaxPciBus;
//int __pciBiosPresent;

typedef unsigned short __TPciBusDevFun;
__TPciBusDevFun __pciBusDevFun;

int pciCmpBusDevFun(__TPciBusDevFun *ppciBusDevFun1, __TPciBusDevFun *ppciBusDevFun2)
{
  return (*ppciBusDevFun1 < *ppciBusDevFun2) ? -1 : 1;
}

#ifdef DOS32
#define regs_w regs.w
#define int86 int386
#else
#define regs_w regs.x
#endif

#ifdef DOS32
int pciPciBiosPresent(void)
{
  union REGS regs;

  regs_w.ax = PCI_BIOS_PRESENT;
  int86(0x1A, &regs, &regs);
  if (regs.x.cflag == 0 && regs.h.ah == 0 &&
      regs.h.dl == 'P' && regs.h.dh == 'C')
  {
//    bcdVersion = regs.x.bx;
//    wMechanism = (unsigned)regs.h.al;
//    nMaxPciBus = (int)regs.h.cl;
    return (//__pciBiosPresent =
            1);
  }
  else
    return (//__pciBiosPresent =
             0);
}

int pciFindPciDevice(unsigned short wDeviceID, unsigned short wVendorID, int short nIndex)
{
  union REGS regs;

  regs_w.ax = FIND_PCI_DEVICE;
  regs_w.cx = wDeviceID;
  regs_w.dx = wVendorID;
  regs_w.si = nIndex;
  int86(0x1A, &regs, &regs);
  if (regs.x.cflag == 0 && regs.h.ah == SUCCESSFUL)
  {
    __pciBusDevFun = regs_w.bx;
    return 1;
  }
  else
    return 0;
}

unsigned char pciReadConfigByte(unsigned short wBusDevFun, unsigned short wReg)
{
  union REGS regs;

  regs_w.ax = READ_CONFIG_BYTE;
  regs_w.bx = wBusDevFun;
  regs_w.di = wReg;
  int86(0x1A, &regs, &regs);
//  if (regs.x.cflag == 0 && regs.h.ah == SUCCESSFUL)
  return regs.h.cl;
}

unsigned short pciReadConfigWord(unsigned short wBusDevFun, unsigned short wReg)
{
  union REGS regs;

  regs_w.ax = READ_CONFIG_WORD;
  regs_w.bx = wBusDevFun;
  regs_w.di = wReg;
  int86(0x1A, &regs, &regs);
//  if (regs.x.cflag == 0 && regs.h.ah == SUCCESSFUL)
  return regs_w.cx;
}

unsigned long pciReadConfigDword(unsigned short wBusDevFun, unsigned short wReg)
{
  union REGS regs;

  regs_w.ax = READ_CONFIG_DWORD;
  regs_w.bx = wBusDevFun;
  regs_w.di = wReg;
  int86(0x1A, &regs, &regs);
//  if (regs.x.cflag == 0 && regs.h.ah == SUCCESSFUL)
  return regs.x.ecx;
}

void pciWriteConfigByte(unsigned short wBusDevFun, unsigned short wReg, unsigned char bData)
{
  union REGS regs;

  regs_w.ax = WRITE_CONFIG_BYTE;
  regs_w.bx = wBusDevFun;
  regs_w.di = wReg;
  regs.h.cl = bData;
  int86(0x1A, &regs, &regs);
}

void pciWriteConfigWord(unsigned short wBusDevFun, unsigned short wReg, unsigned short wData)
{
  union REGS regs;

  regs_w.ax = WRITE_CONFIG_WORD;
  regs_w.bx = wBusDevFun;
  regs_w.di = wReg;
  regs_w.cx = wData;
  int86(0x1A, &regs, &regs);
}

void pciWriteConfigDword(unsigned short wBusDevFun, unsigned short wReg, unsigned long dwData)
{
  union REGS regs;

  regs_w.ax = WRITE_CONFIG_DWORD;
  regs_w.bx = wBusDevFun;
  regs_w.di = wReg;
  regs.x.ecx = dwData;
  int86(0x1A, &regs, &regs);
}

#else //notDOS32

int pciPciBiosPresent(void)
{
  asm {
    mov  ax, PCI_BIOS_PRESENT;
    int  1Ah;
    jc   pPBP_0;
    or   ah, ah;
    jnz  pPBP_0;
    cmp  dx, 4350h; //"CP";
    jne  pPBP_0;
//    mov  bcdVersion, bx;
//    mov  wMechanism, ax; // because ah == 0
//    xor  ch, ch;
//    mov  nMaxPciBus, cx;
    mov  ax, 1;
    jmp  short pPBP_Exit;
  }
  pPBP_0:
  asm {
    xor  ax, ax;
  }
  pPBP_Exit:
//  asm {
//  mov  __pciBiosPresent, ax;
//  }
  return _AX;
}

int pciFindPciDevice(unsigned short wDeviceID, unsigned short wVendorID, int short nIndex)
{
  asm {
    mov  ax, FIND_PCI_DEVICE;
    mov  cx, wDeviceID;
    mov  dx, wVendorID;
    mov  si, nIndex;
    int  1Ah;
    jc   pFPD_0;
    or   ah, ah;
    jnz  pFPD_0;
    mov  __pciBusDevFun, bx;
    mov  ax, 1;
    jmp  short pFPD_Exit;
  }
  pFPD_0:
  asm {
    xor  ax, ax;
  }
  pFPD_Exit:
  return _AX;
}

unsigned char pciReadConfigByte(unsigned short wBusDevFun, unsigned short wReg)
{
  asm {
    mov  ax, READ_CONFIG_BYTE;
    mov  bx, wBusDevFun;
    mov  di, wReg;
    int  1Ah;
  }
//  if (regs.x.cflag == 0 && regs.h.ah == SUCCESSFUL)
  return _CL;
}

unsigned short pciReadConfigWord(unsigned short wBusDevFun, unsigned short wReg)
{
  asm {
    mov  ax, READ_CONFIG_WORD;
    mov  bx, wBusDevFun;
    mov  di, wReg;
    int  1Ah;
  }
//  if (regs.x.cflag == 0 && regs.h.ah == SUCCESSFUL)
  return _CX;
}

unsigned long pciReadConfigDword(unsigned short wBusDevFun, unsigned short wReg)
{
  asm {
    mov  ax, READ_CONFIG_DWORD;
    mov  bx, wBusDevFun;
    mov  di, wReg;
    int  1Ah;
//  if (regs.x.cflag == 0 && regs.h.ah == SUCCESSFUL)
    mov  dx, cx;
    db   066h, 0c1h, 0e9h, 010h; // shr ecx, 16
    xchg dx, cx;
  }
  return ((unsigned long)_DX << 16) | _CX;
}

void pciWriteConfigByte(unsigned short wBusDevFun, unsigned short wReg, unsigned char bData)
{
  asm {
    mov  ax, WRITE_CONFIG_BYTE;
    mov  bx, wBusDevFun;
    mov  di, wReg;
    mov  cl, bData;
    int  1Ah;
  }
}

void pciWriteConfigWord(unsigned short wBusDevFun, unsigned short wReg, unsigned short wData)
{
  asm {
    mov  ax, WRITE_CONFIG_WORD;
    mov  bx, wBusDevFun;
    mov  di, wReg;
    mov  cx, wData;
    int  1Ah;
  }
}

void pciWriteConfigDword(unsigned short wBusDevFun, unsigned short wReg, unsigned long dwData)
{
  asm {
    mov  ax, WRITE_CONFIG_DWORD;
    mov  bx, wBusDevFun;
    mov  di, wReg;
    mov  cx, word ptr dwData+2;
    db   066h, 0c1h, 0e1h, 010h; // shl ecx, 16
    mov  cx, word ptr dwData;
    int  1Ah;
  }
}
#endif //def DOS32
#endif //def DOS

#ifdef QNX4

#include <sys/kernel.h>
#include <sys/osinfo.h>
#include <sys/pci.h>

#ifdef QNX4VME

#include <sys/_vme.h>

extern int errno;

int __vmeDev;
volatile unsigned short *__vmeWin;

char __tmkClassName[24] = "Tmk1553b Driver Class";

#endif //def QNX4VME

pid_t far tmkInterruptServiceRoutine();

typedef struct {
  unsigned busnum;
  unsigned devfuncnum;
} __TPciBusDevFun;

// don't use pointer for unified processing with DOS
__TPciBusDevFun __pciBusDevFun;

int pciCmpBusDevFun(__TPciBusDevFun *ppciBusDevFun1, __TPciBusDevFun *ppciBusDevFun2)
{
  if (ppciBusDevFun1->busnum == ppciBusDevFun2->busnum)
    return (ppciBusDevFun1->devfuncnum < ppciBusDevFun2->devfuncnum) ? -1 : 1;
  else
    return (ppciBusDevFun1->busnum < ppciBusDevFun2->busnum) ? -1 : 1;
}

unsigned __tmkqnx4lastbus, __tmkqnx4version, __tmkqnx4hardware;
struct _osinfo __tmkqnx4info;

int pciPciBiosPresent(void)
{
  if (qnx_osinfo(0, &__tmkqnx4info) == -1 ||
      !(__tmkqnx4info.sflags & _PSF_PCI_BIOS) ||   // we have a PCI BIOS
      _CA_PCI_BIOS_Present(&__tmkqnx4lastbus, &__tmkqnx4version, &__tmkqnx4hardware) != PCI_SUCCESS)
  {
    return 0;
  }
//  if (__tmkqnx4lastbus > 0x40)  // KLUDGE FOR AMI BIOS it reports 66 buses
//    __tmkqnx4lastbus = 0;
  return 1;
}

int pciFindPciDevice(unsigned wDeviceID, unsigned wVendorID, int nIndex)
{
  return (_CA_PCI_Find_Device(wDeviceID, wVendorID, nIndex, &__pciBusDevFun.busnum, &__pciBusDevFun.devfuncnum) == PCI_SUCCESS);
}

unsigned char pciReadConfigByte(__TPciBusDevFun pciBusDevFun, unsigned wReg)
{
  unsigned char Byte;

  _CA_PCI_Read_Config_Byte(pciBusDevFun.busnum, pciBusDevFun.devfuncnum, wReg, 1, &Byte);
  return Byte;
}

unsigned short pciReadConfigWord(__TPciBusDevFun pciBusDevFun, unsigned wReg)
{
  unsigned short Word;

  _CA_PCI_Read_Config_Word(pciBusDevFun.busnum, pciBusDevFun.devfuncnum, wReg, 1, (char*)&Word);
  return Word;
}

unsigned long pciReadConfigDword(__TPciBusDevFun pciBusDevFun, unsigned wReg)
{
  unsigned long Dword;

  _CA_PCI_Read_Config_DWord(pciBusDevFun.busnum, pciBusDevFun.devfuncnum, wReg, 1, (char*)&Dword);
  return Dword;
}

void pciWriteConfigWord(__TPciBusDevFun pciBusDevFun, unsigned wReg, unsigned short Word)
{
  _CA_PCI_Write_Config_Word(pciBusDevFun.busnum, pciBusDevFun.devfuncnum, wReg, 1, (char*)&Word);
}
#endif //def QNX4

#ifdef LINUX

#ifndef TMK1553B_NOCONFIGH
#include <linux/config.h>
#endif
#ifdef CONFIG_SMP
#define __SMP__
#endif
#ifdef CONFIG_64BIT
#define __64BIT__
#endif
#include <asm/io.h>

#ifdef MY_DBG
#define MY_KERN_DEBUG KERN_EMERG
#define MY_KERN_INFO KERN_EMERG
#define MY_KERN_WARNING KERN_EMERG
#else
#define MY_KERN_DEBUG KERN_DEBUG
#define MY_KERN_INFO KERN_INFO
#define MY_KERN_WARNING KERN_WARNING
#endif // def MY_DBG

#endif //def LINUX

#ifdef QNX6

#ifdef CONFIG_SMP
#define __SMP__
#endif
#include <sys/neutrino.h>

#endif //def QNX6

#define CFG_DID         0x02
#define CFG_COMMAND     0x04
#define CFG_SUB_VID     0x2C
#define CFG_SUB_DID     0x2E
#define CFG_ADDR1       0x14
#define CFG_IRQ         0x3C

#define TMK_MAX_IDID    7
#define PLX_VID         0x10B5
#define PLX_DID0        0x9030
#define PLX_DID1        0x9056
#define ELCUS_VID       0xE1C5
#define ELCUS_DID0      0x9000
#define ELCUS_DID1      0x9001
#define ELCUS_DID2      0x9002
#define ELCUS_DID3      0x9003
#define TX1PCI_ID       0x0001
#define TX6PCI_ID       0x0002
#define TX1PCI2_ID      0x0003
#define TX6PCI2_ID      0x0004
//[5..7] can be used with 9030 only for compatibility with older drivers
#define TA1PCI_ID       0x0005
#define TA1PCI4_ID      0x0006
#define TA1PCI32RT_ID   0x0007
//[401] can be used with 9030 only for compatibility with older drivers
#define TA13U2R_ID      0x0401
//[8..A] can be used with 9056 only for compatibility with older drivers
#define TA1PE2_ID       0x0008
#define TA1PE4_ID       0x0009
#define TA1PE32RT_ID    0x000A
//[B..D] can be used with 9000/9001 only for compatibility with older drivers
#define TA1MPE2_ID      0x000B
#define TA1NPCI_ID      0x000C
#define TA1NPE_ID       0x000D
//#define TA1MPCIN_ID     0x000C
//#define TA1MPEN_ID      0x000D
//
//all below can be used with any 90xx with separate bridge type processing
//[F] use 1-2 std pci windows for 1-4 devs (9056 only)
#define TA2PE4_ID       0x000F
//[10..15] use 1-4 pci windows for 1-4 devs (1-2 for 9056; busmast if 9002/9003)
#define TA1P_ID         0x0010
#define TA1P32RT_ID     0x0011
#define TA1P32RTE_ID    0x0012
#define TA1P32RTF_ID    0x0013
#define TA1P32RTV_ID    0x0014
#define TA1PA708_ID     0x0015
//[16..18] use 1-2 std pci windows for 1-4 devs (9056 only)
#define TA1PE4A708_ID   0x0016
#define TA1PE432RT_ID   0x0017
#define TA1PE432RTE_ID  0x0018
//[19] use 1-2 large pci windows for 1-4 devs (9056 only)
#define TA1PE432RTF_ID  0x0019
//[1A..1F] use 1-2 large pci windows for 1-4 devs (bus master, 9056 only)
#define TA1BPE_ID       0x001A
#define TA1BPE32RT_ID   0x001B
#define TA1BPE32RTE_ID  0x001C
#define TA1BPE32RTF_ID  0x001D
#define TA1BPE32RTV_ID  0x001E
#define TA1BPEA708_ID   0x001F
//[20..25] use 1-4 pci windows for 1-4 devs (1-2 for 9056; busmast if 9002/9003)
#define TA2P_ID         0x0020
#define TA2P32RT_ID     0x0021
#define TA2P32RTE_ID    0x0022
#define TA2P32RTF_ID    0x0023
#define TA2P32RTV_ID    0x0024
#define TA2PA708_ID     0x0025
//[26..28] use 1-2 std pci windows for 1-4 devs (9056 only)
#define TA2PE4A708_ID   0x0026
#define TA2PE432RT_ID   0x0027
#define TA2PE432RTE_ID  0x0028
//[29] use 1-2 large pci windows for 1-4 devs (9056 only)
#define TA2PE432RTF_ID  0x0029
//[2A..2F] use 1-2 large pci windows for 1-4 devs (bus master, 9056 only)
#define TA2BPE_ID       0x002A
#define TA2BPE32RT_ID   0x002B
#define TA2BPE32RTE_ID  0x002C
#define TA2BPE32RTF_ID  0x002D
#define TA2BPE32RTV_ID  0x002E
#define TA2BPEA708_ID   0x002F

#ifndef MAX_PCI_SLOTS
#define MAX_PCI_SLOTS   16
#endif

#define IN_DELAY        1
#define OUT_DELAY       1

#define TMK_MIN_TYPE    2
#define TMK_MAX_TYPE    16

#define TMK400          2
#define TMKMPC          3
#define RTMK400         4
#define TMKX            5
#define TMKXI           6
#define MRTX            7
#define MRTXI           8
#define TA              9
#define TAI             10
#define MRTA            11
#define MRTAI           12
#define TSTX            13
#define TSTXI           14
#define TSTA            15
#define TSTAI           16

#ifndef NTMK
#define NTMK            8
#endif
#ifndef NRT
#define NRT             32
#endif

#ifndef DRV_MAX_BASE
#define DRV_MAX_BASE    1023
#endif

/*
#define TMKXV_VectPort  0x2
#define TAV_VectPort    0x22

#define TMKH_DacPort    0x2
#define TMKX_StopPort   0x4

#define TMK_BasePort    0x2
#define TMK_StartPort   0x4
#define TMK_ResetPort   0x6
#define TMK_ModePort    0x8
*/
#define TMK_AddrPort    0xA
/*
#define TMK_CtrlPort    0xC
#define TMK_StatePort   0xC
#define TMK_DataPort    0xE

#define TMKMPC_BasePort         0xC
#define TMKMPC_StartPort        0xA
#define TMKMPC_ResetPort        0x8
#define TMKMPC_ModePort         0x6
#define TMKMPC_AddrHPort        0x4
#define TMKMPC_AddrLPort        0xE
#define TMKMPC_CtrlHPort        0x3
#define TMKMPC_CtrlLPort        0x2
#define TMKMPC_StateHPort       0x3
#define TMKMPC_StateLPort       0x2
#define TMKMPC_DataHPort        0x1
#define TMKMPC_DataLPort        0x0
*/

#ifndef NO_LOCAL_PORT

#define LOCAL_PORT unsigned localport;
#define SET_LOCAL_PORT(num) localport = __tmkPortsAddr1[num]

#define TMKXV_Vect(i)      (localport+0x2)
#define TAV_Vect(i)        (localport+0x22)

#define TMKH_Dac(i)        (localport+0x2)
#define TMKX_Stop(i)       (localport+0x4)

#define TMK_Base(i)        (localport+0x2)
#define TMK_Start(i)       (localport+0x4)
#define TMK_Reset(i)       (localport+0x6)
#define TMK_Mode(i)        (localport+0x8)
#define TMK_Addr(i)        (localport+0xA)
#define TMK_Ctrl(i)        (localport+0xC)
#define TMK_State(i)       (localport+0xC)
#define TMK_Data(i)        (localport+0xE)

#define T400_ATR(i)        (__tmkPortsAddr2[i])
#define R400_AT(i)         (localport)

#define TMKMPC_Base(i)     (localport+0xC)
#define TMKMPC_Start(i)    (localport+0xA)
#define TMKMPC_Reset(i)    (localport+0x8)
#define TMKMPC_Mode(i)     (localport+0x6)
#define TMKMPC_AddrH(i)    (localport+0x4)
#define TMKMPC_AddrL(i)    (localport+0xE)
#define TMKMPC_CtrlH(i)    (localport+0x3)
#define TMKMPC_CtrlL(i)    (localport+0x2)
#define TMKMPC_StateH(i)   (localport+0x3)
#define TMKMPC_StateL(i)   (localport+0x2)
#define TMKMPC_DataH(i)    (localport+0x1)
#define TMKMPC_DataL(i)    (localport)

#define TA_BASE(i)     (localport)
#define TA_IRQ(i)      (localport+0x2)
#define TA_CODEC(i)    (localport+0x4)
#define TA_RESET(i)    (localport+0x6)
#define TA_MODE1(i)    (localport+0x8)
#define TA_ADDR(i)     (localport+0xA)
#define TA_MODE2(i)    (localport+0xC)
#define TA_DATA(i)     (localport+0xE)
#define TA_TIMER1(i)   (localport+0x10)
#define TA_TIMER2(i)   (localport+0x12)
#define TA_RTA(i)      (localport+0x14)
#define TA_TIMCR(i)    (localport+0x18)
#define TA_LCW(i)      (localport+0x1A)
#define TA_MSGA(i)     (localport+0x1C)

#define MRTA_ADDR2(i)  (localport+0x4)
#define MRTA_SW(i)     (localport+0x16)
#define MRTA_BASE2(i)  (localport+0x1E)

#else // NO_LOCAL_PORT defined

#define LOCAL_PORT
#define SET_LOCAL_PORT(num)

#define TMKXV_Vect(i)      (__tmkPortsAddr1[i]+0x2)
#define TAV_Vect(i)        (__tmkPortsAddr1[i]+0x22)

#define TMKH_Dac(i)        (__tmkPortsAddr1[i]+0x2)
#define TMKX_Stop(i)       (__tmkPortsAddr1[i]+0x4)

#define TMK_Base(i)        (__tmkPortsAddr1[i]+0x2)
#define TMK_Start(i)       (__tmkPortsAddr1[i]+0x4)
#define TMK_Reset(i)       (__tmkPortsAddr1[i]+0x6)
#define TMK_Mode(i)        (__tmkPortsAddr1[i]+0x8)
#define TMK_Addr(i)        (__tmkPortsAddr1[i]+0xA)
#define TMK_Ctrl(i)        (__tmkPortsAddr1[i]+0xC)
#define TMK_State(i)       (__tmkPortsAddr1[i]+0xC)
#define TMK_Data(i)        (__tmkPortsAddr1[i]+0xE)

#define T400_ATR(i)        (__tmkPortsAddr2[i])
#define R400_AT(i)         (__tmkPortsAddr1[i])

#define TMKMPC_Base(i)     (__tmkPortsAddr1[i]+0xC)
#define TMKMPC_Start(i)    (__tmkPortsAddr1[i]+0xA)
#define TMKMPC_Reset(i)    (__tmkPortsAddr1[i]+0x8)
#define TMKMPC_Mode(i)     (__tmkPortsAddr1[i]+0x6)
#define TMKMPC_AddrH(i)    (__tmkPortsAddr1[i]+0x4)
#define TMKMPC_AddrL(i)    (__tmkPortsAddr1[i]+0xE)
#define TMKMPC_CtrlH(i)    (__tmkPortsAddr1[i]+0x3)
#define TMKMPC_CtrlL(i)    (__tmkPortsAddr1[i]+0x2)
#define TMKMPC_StateH(i)   (__tmkPortsAddr1[i]+0x3)
#define TMKMPC_StateL(i)   (__tmkPortsAddr1[i]+0x2)
#define TMKMPC_DataH(i)    (__tmkPortsAddr1[i]+0x1)
#define TMKMPC_DataL(i)    (__tmkPortsAddr1[i])

#define TA_BASE(i)     (__tmkPortsAddr1[i])
#define TA_IRQ(i)      (__tmkPortsAddr1[i]+0x2)
#define TA_CODEC(i)    (__tmkPortsAddr1[i]+0x4)
#define TA_RESET(i)    (__tmkPortsAddr1[i]+0x6)
#define TA_MODE1(i)    (__tmkPortsAddr1[i]+0x8)
#define TA_ADDR(i)     (__tmkPortsAddr1[i]+0xA)
#define TA_MODE2(i)    (__tmkPortsAddr1[i]+0xC)
#define TA_DATA(i)     (__tmkPortsAddr1[i]+0xE)
#define TA_TIMER1(i)   (__tmkPortsAddr1[i]+0x10)
#define TA_TIMER2(i)   (__tmkPortsAddr1[i]+0x12)
#define TA_RTA(i)      (__tmkPortsAddr1[i]+0x14)
#define TA_TIMCR(i)    (__tmkPortsAddr1[i]+0x18)
#define TA_LCW(i)      (__tmkPortsAddr1[i]+0x1A)
#define TA_MSGA(i)     (__tmkPortsAddr1[i]+0x1C)

#define MRTA_ADDR2(i)  (__tmkPortsAddr1[i]+0x4)
#define MRTA_SW(i)     (__tmkPortsAddr1[i]+0x16)
#define MRTA_BASE2(i)  (__tmkPortsAddr1[i]+0x1E)

#endif //def NO_LOCAL_PORT

#define MRTA_RT_ON 0x0020
#define MRTA_BRC_PAGE 0x003F

#define TA_RT_DATA_INT_BLK 0x0010
#define TA_IRQ_EN 0x0400
#define TA_FIFO_RESET 0x0004
#define TA_RTMT_START 0x0008
#define TA_MW_MODE 0xC800
#define TA_MT_MODE 0x8000
#define TA_MWMT_MODE 0xC000

#define TA_BC_START 0x8000
#define TA_BC_STOP 0x2000
//#define TA_STOP_ON_EXC 0x000C

#define __TA_14US 0x0000
#define __TA_18US 0x1000
#define __TA_26US 0x2000
#define __TA_63US 0x3000

#define __TX_14US 0x0000
#define __TX_26US 0x8000

//Mask RRG1
#define RRG1_WORK_Reset     0xC000
#define RRG1_WORK_RT        0x4000
#define RRG1_WORK_MCO       0x8000
#define RRG1_WORK_MCL       0xC000
#define RRG1_PAUSE_Reset    0x3000
#define RRG1_PAUSE_18       0x1000
#define RRG1_PAUSE_26       0x2000
#define RRG1_PAUSE_63       0x3000
#define RRG1_Dis_MCO        0x0800
#define RRG1_En_IRQ         0x0400
#define RRG1_Dis_BUS_A_td   0x0200
#define RRG1_Dis_BUS_B_td   0x0100
#define RRG1_Dis_BUS_A_rd   0x0080
#define RRG1_Dis_BUS_B_rd   0x0040
#define RRG1_Dis_BS         0x0020
#define RRG1_Dis_IRQ_CW     0x0010
#define RRG1_Start_RT       0x0008
#define RRG1_Reset_FIFO     0x0004
#define RRG1_Dis_tr_BS      0x0002
#define RRG1_Dis_tr_LCW_BS  0x0001

//Mask RRG2
//BC
#define RRG2_BC_Start            0x8000
#define RRG2_BC_En_IRQ_er        0x4000
#define RRG2_BC_END_Stop_IRQ     0x2000
#define RRG2_BC_En_Restart       0x1000
#define RRG2_BC_En_2_Restart     0x0800
#define RRG2_BC_1Res_Activ_Bus   0x0400
#define RRG2_BC_2Res_Activ_Bus   0x0200
#define RRG2_BC_Dis_Flag_Gen     0x0100
#define RRG2_BC_Goto_Rezerv      0x0040
#define RRG2_BC_Dis_Answ_Bit     0x0020
#define RRG2_BC_Mask_Rez_Bit     0x0010
#define RRG2_BC_Stop_IRQ_AnswBit 0x8
#define RRG2_BC_Stop_IRQ_Error   0x4
#define RRG2_BC_Mode_Mask_BC     0x0001
//RT MCO MCL
#define RRG2_RT_ADR                         0xF800
#define RRG2_RT_EN_GRUP                     0x0010
#define RRG2_RT_DIS_IRQ_ERR                 0x0400
#define RRG2_RT_SET_BIT_SR                  0x0100
#define RRG2_RT_RESET_BIT_SR_TR_VECTOR_WORD 0x0080
#define RRG2_RT_SET_BIT_BS                  0x8
#define RRG2_RT_SET_BIT_SF                  0x4
#define RRG2_RT_SET_BIT_TF                  0x1
#define RRG2_RT_EN_BUS_CTRL                 0x2
#define RRG2_RT_SET_INSTR_BIT               0x0200
#define RRG2_MTW_TEST_BUS                   0x40
#define RRG2_MTW_TEST_SYNC                  0x20

#define TMK400_INT1_MASK        0x40
#define TMK400_INT2_MASK        0x80
#define TMKMPC_INT1_MASK        0x8000
#define RTMK400_INT1_MASK       0x02
#define RTMK400_INT2_MASK       0x01

#define TMK400CWBitsMask        0x0060

#define RT_CLEAR_INT    0x03E0
#define RTAnswBitsMask  0x001F

#define DRV_HBIT_MODE           0x0001
#define DRV_FLAG_MODE           0x0002
#define DRV_BRCST_MODE          0x0004
#define DRV_RSREQ_MODE          0x0080
#define DRV_NOERR_MODE          0x0400
#define DRV_CBSNL_MODE          0x0010
#define DRV_CBSND_MODE          0x0020
#define DRV_CNOBS_MODE          0x0200
#define RTMK400_HBIT_MODE       0x0020
#define RTMK400_FLAG_MODE       0x0040
#define RTMK400_BRCST_MODE      0x0080
//;TMKX_HBIT_MODE  0000001000000000b
#define TMKX_HBIT_MODE  0x0000
#define TMKX_FLAG_MODE  0x0400
#define TMKX_BRCST_MODE 0x0100
//;MRTX_HBIT_MODE  0000001000000000b
#define MRTX_HBIT_MODE  0x0000
#define MRTX_FLAG_MODE  0x0400
#define MRTX_BRCST_MODE 0x0100
#define TA_HBIT_MODE    0x0200
#define TA_BRCST_MODE   0x0010
#define TA_RSREQ_MODE   0x0080
#define TA_NOERR_MODE   0x0400
#define TA_CBSNL_MODE   0x0001
#define TA_CBSD_MODE    0x0002
#define TA_CNOBS_MODE   0x0020

#define TX_RT_DATA_INT_BLK 0x2000
#define GENER1_BLK      0x0004
#define GENER2_BLK      0x4000

#if NRT == 0
#ifdef MRTX
#undef MRTX
#endif
#ifdef MRTXI
#undef MRTXI
#endif
#ifdef MRTA
#undef MRTA
#endif
#ifdef MRTAI
#undef MRTAI
#endif
#endif

#ifndef USE_TMKRAWIO
#ifdef TSTX
#undef TSTX
#endif
#ifdef TSTXI
#undef TSTXI
#endif
#ifdef TSTA
#undef TSTA
#endif
#ifdef TSTA
#undef TSTAI
#endif
#endif //ndef USE_TMKRAWIO

#ifdef DOS32
//#pragma pack(4);
int __DataBegin = 0;
int DrvCodeBegin()
{
  return 0x1234;
}

int __fTmkDeep = 0;
U16 __ftmkInt1[NTMK];
#endif //def DOS32

unsigned __tmkaVID[TMK_MAX_IDID+1] = {PLX_VID,  PLX_VID,  ELCUS_VID, ELCUS_VID, ELCUS_VID,  ELCUS_VID,  ELCUS_VID,  ELCUS_VID};
unsigned __tmkaDID[TMK_MAX_IDID+1] = {PLX_DID0, PLX_DID1, PLX_DID0,  PLX_DID1,  ELCUS_DID0, ELCUS_DID1, ELCUS_DID2, ELCUS_DID3};

BOOL __tmkIsUserType[TMK_MAX_TYPE+1+1] =
{
  0, 0,
#ifdef TMK400
  1, 1, 1, //TMK400, TMKMPC, RTMK000
#else
  0, 0, 0,
#endif
#ifdef TMKX
  1, 1, //TMKX, TMKXI
#else
  0, 0,
#endif
#ifdef MRTX
  1, 1, //MRTX, MRTXI
#else
  0, 0,
#endif
#ifdef TA
  1, 1, //TA, TAI
#else
  0, 0,
#endif
#ifdef MRTA
  1, 1, //MRTA, MRTAI
#else
  0, 0,
#endif
#ifdef TSTX
  1, 1, //TSTX, TSTXI
#else
  0, 0,
#endif
#ifdef TSTA
  1, 1, //TSTA, TSTAI
#else
  0, 0,
#endif
  0
};

#define __TMK400 0
#define __TMKMPC 1
#define __RTMK400 2
#define __TMKX 3
#define __TA 4

#define DRV_MAX_TYPE0 4

#ifdef MRTX
#define __MRTX (DRV_MAX_TYPE0 + 1)
#define DRV_MAX_TYPE_INC1 1
#else
#define DRV_MAX_TYPE_INC1 0
#endif

#ifdef MRTA
#define __MRTA (DRV_MAX_TYPE0 + DRV_MAX_TYPE_INC1 + 1)
#define DRV_MAX_TYPE_INC2 1
#else
#define DRV_MAX_TYPE_INC2 0
#endif

#ifdef TSTX
#define __TSTX (DRV_MAX_TYPE0 + DRV_MAX_TYPE_INC1 + DRV_MAX_TYPE_INC2 + 1)
#define DRV_MAX_TYPE_INC3 1
#else
#define DRV_MAX_TYPE_INC3 0
#endif

#ifdef TSTA
#define __TSTA (DRV_MAX_TYPE0 + DRV_MAX_TYPE_INC1 + DRV_MAX_TYPE_INC2 + DRV_MAX_TYPE_INC3 + 1)
#define DRV_MAX_TYPE_INC4 1
#else
#define DRV_MAX_TYPE_INC4 0
#endif

#define DRV_MAX_TYPE (DRV_MAX_TYPE0 + DRV_MAX_TYPE_INC1 + DRV_MAX_TYPE_INC2 + DRV_MAX_TYPE_INC3 + DRV_MAX_TYPE_INC4)

U16 __tmkUser2DrvType[TMK_MAX_TYPE+1] =
{
  0xFFFF,
  0xFFFF,
  __TMK400,  //TMK400
  __TMKMPC,  //TMKMPC
  __RTMK400, //RTMK400
  __TMKX,    //TMKX
  __TMKX,    //TMKXI
#ifdef MRTX
  __MRTX,    //MRTX
  __MRTX,    //MRTXI
#else
  0xFFFF,
  0xFFFF,
#endif
  __TA,      //TA
  __TA,      //TAI
#ifdef MRTA
  __MRTA,    //MRTA
  __MRTA,    //MRTAI
#else
  0xFFFF,
  0xFFFF,
#endif
#ifdef TSTX
  __TSTX,    //TSTX
  __TSTX,    //TSTXI
#else
  0xFFFF,
  0xFFFF,
#endif
#ifdef TSTA
  __TSTA,    //TSTA
  __TSTA,    //TSTAI
#else
  0xFFFF,
  0xFFFF,
#endif
};

//        PUBLIC  tmkError
//        PUBLIC  __rtDisableMask

//;        PUBLIC  _tmkListErr

//#define tmkError tmkError_

BOOL __tmkFirstTime = 1;

U16 __tmkTimerCtrl[NTMK]; // DUP(0)
#ifdef MRTA
U32 __dmrtaTimers[NTMK];
#endif
U16 __tmkTimeOut[NTMK]; //DUP(0)

U16 __tmkHWVer[NTMK]; // DUP(0)

#define TFLGM U08

#define DRV_FLAG_MODE_HW 1
#define DRV_FLAG_MODE_SW 2
#define DRV_FLAG_UPDATE  4
//set DRV_FLAG_MODE_SW = DRV_FLAG_MODE
TFLGM  __FLAG_MODE[NTMK+NRT]; // DUP(0)

U16 __BC_RT_FLAG[NTMK+NRT][32];
U16 __RT_BC_FLAG[NTMK+NRT][32];


//int fTmkEventSet[NTMK]; // DUP(0)
U16 __wInDelay[NTMK]; // DUP(1)
U16 __wOutDelay[NTMK]; // DUP(1)
int __tmkMaxNumber = NTMK-1;

#ifdef STATIC_TMKNUM
int FARVR tmkError = 0;
#else
int FARVR tmkError[NTMK+NRT]; // DUP(0)
#endif

#ifdef STATIC_TMKNUM
#define TMKNUM(num)
#define TMKNUM__(num)
#else
#define TMKNUM(num) num
#define TMKNUM__(num) num,
#endif


#ifdef STATIC_TMKNUM
int volatile __tmkNumber = 0;
#endif
//;tmkRealNumber2  DD      0
#if NRT > 0
int __amrtNumber[NTMK+NRT]; // DUP(0)
#endif

#include "tmkisr.h"

#ifndef DOS
#include "tmkisrc.c"
#endif //nDOS

#define UOPT U08
UOPT __tmkOptions[NTMK+NRT];

#if NRT > 0
//U16 __awBrcRxState[NTMK][32*4];
//U16 __awBrcRxBuf[NTMK][32*32];
// use 31 sa to conserve ram, use sa 0 (from mt) instead of sa 1f for cmds !!!
U16 __awBrcRxState[NTMK][31*4];
U16 __awBrcRxBuf[NTMK][31*32];
#endif //NRT

#if defined(DOS) || defined(QNX4INT)
int volatile __tmkSaveNumber;
int volatile __tmkInIrqNumber;

TListEvD __aEvData[NTMK+NRT];
#endif //def DOS || QNX4INT
#ifdef WIN95
//;__tmkSaveNumber   DD      ?
//;__tmkInIrqNumber  DD      ?
//;_tmkListErr     U32[NTMK]; //!!! DUP(0)
#endif

U16 __tmkPortsAddr1[NTMK];
U16 __tmkPortsAddr2[NTMK];
#ifdef USE_TMKRAWIO
U16 __tmkPortsMax1[NTMK];
U16 __tmkPortsMax2[NTMK];
#endif
U16 __tmkDrvType[NTMK+NRT]; // DUP(0xFFFF)
U16 __tmkUserType[NTMK+NRT];
U16 __tmkMode[NTMK+NRT]; // DUP(0xFFFF)
BOOL __tmkModeMtRt[NTMK]; //DUP(0)
//U16 __tmkRAMSize[NTMK];
#if defined(DOS) || defined(QNX4)
BOOL __tmkPci[NTMK];// DUP(0)
#endif

#ifndef TMK_KEEP_FLGM
#define TMK_KEEP_FLGM 0
#endif
#if defined(DOS) && !defined(TMKDRV_FUNCS)
#define __tmkKeepFlgM TMK_KEEP_FLGM
//const int __tmkKeepFlgM = TMK_KEEP_FLGM;
#if TMK_KEEP_FLGM != 0
#define USE_TMK_KEEP_FLGM
#endif
#else
int __tmkKeepFlgM = TMK_KEEP_FLGM;
#define USE_TMK_KEEP_FLGM
#endif

#ifndef TMK_CBSND
#define TMK_CBSND 0
#endif
#if defined(DOS) && !defined(TMKDRV_FUNCS)
#define __tmkCBSND TMK_CBSND
//const int __tmkCBSND = TMK_CBSND;
#if TMK_CBSND != 0
#define USE_TMK_CBSND
#endif
#else
int __tmkCBSND = TMK_CBSND;
#define USE_TMK_CBSND
#endif

#ifndef TMK_WRAP_SA
#define TMK_WRAP_SA 0
#endif
#if (TMK_WRAP_SA < 0) || (TMK_WRAP_SA > 30)
#undef TMK_WRAP_SA
#define TMK_WRAP_SA 0
#endif
#ifndef TMK_WRAP_ST
#define TMK_WRAP_ST 0
#endif
#if TMK_WRAP_ST == 0
#define TMK_WRAP_NOST5 (((TMK_WRAP_SA) << 5) | RT_DIR_MASK)
#else
#define TMK_WRAP_NOST5 0
#endif
#ifndef TMK_WRAP_INT
#define TMK_WRAP_INT 0
#endif
#if TMK_WRAP_INT != 0
#undef TMK_WRAP_INT
#define TMK_WRAP_INT 0x4000
#endif
#if defined(DOS) && !defined(TMKDRV_FUNCS)
#define __tmkWrapSA TMK_WRAP_SA
#define __tmkWrapNoST5 TMK_WRAP_NOST5
#define __tmkWrapInt TMK_WRAP_INT
//const unsigned __tmkWrapSA = TMK_WRAP_SA;
//const unsigned __tmkWrapNoST5 = TMK_WRAP_NOST5;
//const unsigned __tmkWrapInt = TMK_WRAP_INT;
#if TMK_WRAP_SA != 0
#define USE_TMK_WRAP_SA
#endif
#else
unsigned __tmkWrapSA = TMK_WRAP_SA;
unsigned __tmkWrapNoST5 = TMK_WRAP_NOST5;
unsigned __tmkWrapInt = TMK_WRAP_INT;
#define USE_TMK_WRAP_SA
#endif

#ifdef TMK_REAL_MAX
int __tmkUseRealMax = 0;
int __tmkRealMax = -1;
#endif

#if NRT > 0
int __mrtMaxNRT = 0;
int __mrtMinRT[NTMK]; // DUP(0)
int __mrtNRT[NTMK]; // DUP(1)
U16 __mrtCtrl0[NTMK];
U16 __mrtCtrl1[NTMK];
U16 __mrtMask0[NTMK];
U16 __mrtMask1[NTMK];
U32 __dmrtRT[NTMK]; //DUP(0L)
U32 __dmrtBrc[NTMK]; //DUP(0L)
U32 __dmrtLgl[NTMK]; //DUP(0L)
#if (NTMK+NRT) < 256
U08 __mrtA2RT[NTMK][32];
#else
U16 __mrtA2RT[NTMK][32];
#endif //<256
U16 __mrtLastBrcTxRT[NTMK]; //DUP(0xFF)
U08 __mrtRxCtrl[NTMK+NRT]; // allow for NTMK and state for NRT
BOOL __tmkBusErr[NTMK];
#endif //NRT

BOOL volatile __tmkRAMInWork[NTMK]; // DUP(0)
unsigned volatile __tmkRAMAddr[NTMK]; // DUP(0)
#if defined(DOS) || defined(QNX4INT)
#ifdef RAMwoCLI
!!! not ready yet
unsigned __tmkRAMRestored[NTMK]; //!!! DUP(0)
#endif //def RAMwoCLI
BOOL volatile __tmkSaveRAMInWork = 0;
unsigned volatile __tmkSaveRAMAddr = 0;
#endif //def DOS || QNX4INT

//;                IRPC    N, 0123
//;VTMK&N&_Int1_Desc DD &N& //;VPICD_IRQ_Descriptor <255, VPICD_Opt_Read_Hw_IRR OR VPICD_OPT_REF_DATA, OFFSET32 DrvTmksInt1>
//;VTMK&N&_Int2_Desc DD &N& //;VPICD_IRQ_Descriptor <255, VPICD_Opt_Read_Hw_IRR OR VPICD_OPT_REF_DATA, OFFSET32 DrvTmksInt2>
//;                ENDM
//;
//;tmkDrvInt1Desc  LABEL   DWORD
//;                IRPC    N, 0123
//;                DD      VTMK&N&_Int1_Desc
//;                ENDM
//;tmkDrvInt2Desc  LABEL   DWORD
//;                IRPC    N, 0123
//;                DD      VTMK&N&_Int2_Desc
//;                ENDM
//;
//;tmkIRQ1Handle   U32 [NTMK]; //!!! DUP(0)
//;tmkIRQ2Handle   U32 [NTMK]; //!!! DUP(0)

#if defined(DOS)

#if (NTMK > 0)
RETIR TYPIR DrvTmk0Int1(__CPPARGS);
#endif
#if NTMK > 1
RETIR TYPIR DrvTmk1Int1(__CPPARGS);
#endif
#if NTMK > 2
RETIR TYPIR DrvTmk2Int1(__CPPARGS);
#endif
#if NTMK > 3
RETIR TYPIR DrvTmk3Int1(__CPPARGS);
#endif
#if NTMK > 4
RETIR TYPIR DrvTmk4Int1(__CPPARGS);
#endif
#if NTMK > 5
RETIR TYPIR DrvTmk5Int1(__CPPARGS);
#endif
#if NTMK > 6
RETIR TYPIR DrvTmk6Int1(__CPPARGS);
#endif
#if NTMK > 7
RETIR TYPIR DrvTmk7Int1(__CPPARGS);
#endif

RETIR (TYPIR *tmkDrvInt1[NTMK])(__CPPARGS) = {
#if NTMK > 0
  DrvTmk0Int1
#endif
#if NTMK > 1
  , DrvTmk1Int1
#endif
#if NTMK > 2
  , DrvTmk2Int1
#endif
#if NTMK > 3
  , DrvTmk3Int1
#endif
#if NTMK > 4
  , DrvTmk4Int1
#endif
#if NTMK > 5
  , DrvTmk5Int1
#endif
#if NTMK > 6
  , DrvTmk6Int1
#endif
#if NTMK > 7
  , DrvTmk7Int1
#endif
#if NTMK > 8
Not Ready Yet
#endif
};

#ifdef DOS
void (TYPIR *tmkOldInt1[NTMK])(__CPPARGS);
#endif

#endif //def DOS

#ifdef DOS32

#if (NTMK > 0)
RETIR TYPIR rmDrvTmk0Int1(__CPPARGS);
#endif
#if NTMK > 1
RETIR TYPIR rmDrvTmk1Int1(__CPPARGS);
#endif
#if NTMK > 2
RETIR TYPIR rmDrvTmk2Int1(__CPPARGS);
#endif
#if NTMK > 3
RETIR TYPIR rmDrvTmk3Int1(__CPPARGS);
#endif
#if NTMK > 4
RETIR TYPIR rmDrvTmk4Int1(__CPPARGS);
#endif
#if NTMK > 5
RETIR TYPIR rmDrvTmk5Int1(__CPPARGS);
#endif
#if NTMK > 6
RETIR TYPIR rmDrvTmk6Int1(__CPPARGS);
#endif
#if NTMK > 7
RETIR TYPIR rmDrvTmk7Int1(__CPPARGS);
#endif

RETIR (TYPIR *tmkDrvInt1RM[NTMK])(__CPPARGS) = {
#if NTMK > 0
  rmDrvTmk0Int1
#endif
#if NTMK > 1
  , rmDrvTmk1Int1
#endif
#if NTMK > 2
  , rmDrvTmk2Int1
#endif
#if NTMK > 3
  , rmDrvTmk3Int1
#endif
#if NTMK > 4
  , rmDrvTmk4Int1
#endif
#if NTMK > 5
  , rmDrvTmk5Int1
#endif
#if NTMK > 6
  , rmDrvTmk6Int1
#endif
#if NTMK > 7
  , rmDrvTmk7Int1
#endif
#if NTMK > 8
Not Ready Yet
#endif
};

U16 rmtmkOldInt1[NTMK][2]; //seg:off
U16 rmtmkCallbackInt1[NTMK][2]; //seg:off
//#pragma pack(1);
struct rmcs {
  U32 rm_EDI;
  U32 rm_ESI;
  U32 rm_EBP;
  U32 rm_Reserved;
// rm_BX           LABEL   WORD
  U32 rm_EBX;
  U32 rm_EDX;
  U32 rm_ECX;
// rm_AX           LABEL   WORD
  U32 rm_EAX;
  U16 rm_Flags;
  U16 rm_ES;
  U16 rm_DS;
  U16 rm_FS;
  U16 rm_GS;
  U16 rm_IP;
  U16 rm_CS;
  U16 rm_SP;
  U16 rm_SS;
} __rmCallStruc;
//#pragma pack(4);
U16 volatile __SaveES;
#endif //def DOS32

#ifdef QNX4
int __tmkiid[NTMK];
#endif

BOOL __tmkStarted[NTMK]; // DUP(0)

U16 __bcControls1[NTMK];
U16 __bcCtrlWA[NTMK];
U16 __bcControls[NTMK];
U16 __bcBus[NTMK]; // DUP(0)
#if defined(DOS) || defined(QNX4)
U16 volatile __bcSaveBase;
#endif //def DOS
U16 __bcMaxBase[NTMK]; // DUP(0)
#if DRV_MAX_BASE > 255
U16 __mtMaxBase[NTMK]; // DUP(0)
U16 __mtMaxMaxBase[NTMK]; // DUP(0)
#endif
U16 __bcBaseBus[NTMK];
U16 __bcBasePC[NTMK];
U08 __bcAW1Pos[NTMK];
U08 __bcAW2Pos[NTMK];
//U16 bcAW1[NTMK];
//U16 bcAW2[NTMK];
U16 __mtStopBaseTA[NTMK];
BOOL __bcXAStart[NTMK];
U08 volatile __bcmtStateX[NTMK]; // DUP(0) (6 bits)

U08 __bcExt2StdResult[] =
{
  0x00, 0x02, 0x08, 0x08,
  0x80, 0x01, 0x02, 0x20,
  0x04, 0x06, 0x0C, 0x0C,
  0x84, 0x05, 0x06, 0x24
};

//;bcStd2ExtResult DB      00h, 05h, 01h, 05h
//;                DB      08h, 0Dh, 09h, 0Dh
//;                DB      03h, 05h, 01h, 05h
//;                DB      0Bh, 0Dh, 09h, 0Dh
//;                DB      07h, 05h, 01h, 05h
//;                DB      0Fh, 0Dh, 09h, 0Dh
//;                DB      03h, 05h, 01h, 05h
//;                DB      0Bh, 0Dh, 09h, 0Dh
//;                DB      07h, 05h, 01h, 05h
//;                DB      0Fh, 0Dh, 09h, 0Dh
//;                DB      03h, 05h, 01h, 05h
//;                DB      0Bh, 0Dh, 09h, 0Dh
//;                DB      07h, 05h, 01h, 05h
//;                DB      0Fh, 0Dh, 09h, 0Dh
//;                DB      03h, 05h, 01h, 05h
//;                DB      0Bh, 0Dh, 09h, 0Dh


#ifdef WIN95
U32 hTmkEvent[NTMK]; //!!! DUP(0)
//;hIrqEvent       U32 [NTMK]; //!!! DUP(0)
//;hCurVM          U32 [NTMK]; //!!! DUP(0)
//;hSysVM          DD      0
//;EXTERN _hSysVM: DWORD
//;EXTERN _ahlEvData: DWORD

//;EXTERN _lpwOut: DWORD
//;EXTERN _VTMK_BC_Event_Callback: DWORD
#endif

void FARIR retfLabel(void);

#if defined(DOS) || defined(QNX4INT)
RETIR FARIR retfLabel1(U16 arg1);
#ifdef USE_TMKRAWIO
RETIR FARIR retfLabel2(U16 arg1, U16 arg2);
#endif
RETIR FARIR retfRLabel1(U16 arg1);
RETIR FARIR retfRLabel2(U16 arg1, U16 arg2);
RETIR FARIR retfRLabel3(U16 arg1, U16 arg2, U16 arg3);
#endif //def DOS || QNX4INT

#if defined(DOS) || defined(QNX4INT)
RETIR (FARIR *tmkUserNormBC[NTMK])(U16, U16, U16); // DUP(retfRLabel)
RETIR (FARIR *tmkUserExcBC[NTMK])(U16, U16, U16); // DUP(retfRLabel)
RETIR (FARIR *tmkUserSigBC[NTMK])(U16); // DUP(retfLabel)
RETIR (FARIR *tmkUserXBC[NTMK])(U16, U16); // DUP(retfRLabel)
RETIR (FARIR *tmkUserABC[NTMK])(U16, U16); // DUP(retfRLabel)

RETIR (FARIR *tmkUserSigMT[NTMK])(U16); // DUP(retfLabel)
RETIR (FARIR *tmkUserXMT[NTMK])(U16, U16); // DUP(retfRLabel)

RETIR (FARIR *tmkUserOthTMK[NTMK])(U16); // DUP(retfLabel)
#ifdef USE_TMKRAWIO
RETIR (FARIR *tmkUserRawTMK[NTMK])(U16, U16); // DUP(retfLabel)
#endif
#endif// def DOS || QNX4INT

#ifdef USE_TMKRAWIO
#ifndef MAX_RAW_IRQ_IO_LEN
#define MAX_RAW_IRQ_IO_LEN 32
#endif
U16 __tmkRawIrqIoLen[NTMK];
U16 __tmkRawIrqIoVal[NTMK][MAX_RAW_IRQ_IO_LEN];
U08 __tmkRawIrqIoOp[NTMK][MAX_RAW_IRQ_IO_LEN];
#endif //def USE_TMKRAWIO

U16 __rtControls[NTMK+NRT]; // DUP(0)
//MRTA(realnum) - MODE1
U16 __rtControls1[NTMK+NRT]; // DUP(0)
//MRTA(realnum) - MODE2
//MRTA(num) - SW
U16 __rtGap[NTMK];
U16 __rtPagePC[NTMK+NRT];
U16 __rtPageBus[NTMK+NRT];
U16 __rtAddress[NTMK+NRT]; //DUP(00FFh)
//;rtMaskAddr      U16 [NTMK+NRT]; DUP(001Fh)
//;rtMaskBrc       U16 [NTMK+NRT]; //!!! DUP(0)
U16 __rtMaxPage[NTMK+NRT]; // DUP(0)
#if defined(DOS) || defined(QNX4)
U16 volatile __rtSaveMode;
U16 volatile __rtSaveSubAddr;
U16 volatile __rtSaveSABuf;
U16 volatile __rtSavehm400Page;
#ifdef MRTA
U16 volatile __rtSavehm400Page2;
#endif
U16 volatile __rtIrqBuf[NTMK+NRT];
#endif //def DOS
U16 __rtMode[NTMK+NRT]; // DUP(0)
U16 __rtSubAddr[NTMK+NRT]; // DUP(0)
U16 __hm400Page[NTMK+NRT]; // DUP(0)
U16 __rtSABuf[NTMK+NRT]; // DUP(0)
U16 __rtSABufMode[NTMK+NRT][64]; // DUP(SABUF_OFF)
U16 __rtSABufSize[NTMK+NRT][64]; // DUP(1)
U16 __rtSABufExt[NTMK+NRT][64]; // DUP(0)
U16 __rtHeapMin[NTMK]; // initial - min
U16 __rtHeapAlloc[NTMK]; //initial - 1 above max
U16 __rtHeapIni[NTMK]; //initial (1 above max); also min mt base
// heap (SABUF_HEAP) grows down from 'initial' to 'heap min'
// subaddrs (SABUF_STD) grow up to heap with increasing 'heap min'
// TA heap min - first base after subaddrs
// TA heap max - last base before table (before MTA_STOP_BASE if used)
//unsigned AdrTab = 1023;
#define AdrTab 1023
#define MTA_STOP_BASE 1022
#define TA_RT_HEAP_MIN 64
// leave base 1023 for AdrTab
#define TA_RT_HEAP_MAX_RT 1022
#if DRV_MAX_BASE > 255
// actually in MTRT base 0 is unused and can be used as MTA_STOP_BASE
// instead of 1022 but that would require more complex code switching
// MTA_STOP_BASE between MT and MTRT
// leave base 1023 for AdrTab and base 1022 for MTA_STOP_BASE
#define TA_RT_HEAP_MAX_MTRT (1021 - 512)
#else
#define TA_RT_HEAP_MAX_MTRT (255 - 128)
#endif
#if NRT > 0
#ifdef MRTA
//U16 __rtSubAddr2[NTMK+NRT]; // DUP(0)
U16 __hm400Page2[NTMK+NRT]; // DUP(0) current ADDR2
U16 __hm400Page0[NTMK+NRT]; // DUP(0) starting page
// these below could be [32] but that requires (sa & 0x1F) in related code
// also [32] would allow SABUF_STD with bufsize == 2 for broadcast RT
//U16 __mrtSABufB[NTMK][64]; // DUP(0)
//U16 __mrtSABufModeB[NTMK][64]; // DUP(SABUF_OFF)
//U16 __mrtSABufSizeB[NTMK][64]; // DUP(1)
//U16 __mrtSABufExtB[NTMK][64]; // DUP(0)
// MRTA heap min - first base after table
// MRTA heap max - last base before common unused page for RTs w/undefined addrs
#define MRTA_MRT_HEAP_MIN 32
#define MRTA_MRT_HEAP_MAX (2047 - 64)
#endif //def MRTA
#endif
U16 __rtRxDataCmd[NTMK+NRT][5];
U16 __rtDisableMask[NTMK+NRT];
U16 __rtBRCMask[NTMK+NRT];
BOOL __rtEnableOnAddr[NTMK+NRT]; // DUP(1)
U16 __RT_DIS_MASK0[TMK_MAX_TYPE+1] =
{
  0,       //; none
  0,       //; none
  0x001F,  //; TMK400
  0x001F,  //; RTMK400
  0x001F,  //; TMKMPC
  0xF800,  //; TMKX
  0xF800,  //; TMKXI
  0x001F,  //; MRTX
  0x001F,  //; MRTXI
  0xF800,  //; TA
  0xF800,  //; TAI
  0x001F,  //; MRTA
  0x001F,  //; MRTAI
  0,       //; TSTX
  0,       //; TSTXI
  0,       //; TSTA
  0        //; TSTAI
};
U16 __RT_BRC_MASK0[TMK_MAX_TYPE+1] =
{
  0,       //; none
  0,       //; none
  0xFFFF,  //; TMK400
  0xFF7F,  //; RTMK400
  0xFFFF,  //; TMKMPC
  0xFEFF,  //; TMKX
  0xFEFF,  //; TMKXI
  0xFEFF,  //; MRTX
  0xFEFF,  //; MRTXI
  0xFFEF,  //; TA
  0xFFEF,  //; TAI
  0x7FFF,  //; MRTA
  0x7FFF,  //; MRTAI
  0,       //; TSTX
  0,       //; TSTXI
  0,       //; TSTA
  0        //; TSTAI
};

U16 __RT_DIS_MASK[DRV_MAX_TYPE+1];
U16 __RT_BRC_MASK[DRV_MAX_TYPE+1];

#if defined(DOS) || defined(QNX4INT)
RETIR (FARIR *tmkUserCmdRT[NTMK+NRT])(U16); // DUP(retfRLabel)
RETIR (FARIR *tmkUserErrRT[NTMK+NRT])(U16); // DUP(retfRLabel)
RETIR (FARIR *tmkUserDataRT[NTMK+NRT])(U16); // DUP(retfRLabel)
void DrvReadOldMask(void);
void DrvMaskTmk(int REGPAR num);
void DrvUnmaskTmk(int REGPAR num);
RETIR DrvTmksInt1(int REGPAR num);
#endif //def DOS || QNX4INT

#ifdef DOS32
U08 __tmkPic20Base = 0x08;
U08 __tmkPicA0Base = 0x70;
#endif //def DOS32

#if defined(DOS) || defined(QNX4)
#ifdef MASKTMKS
#if NRT > 0
!!! NOT SUPPORTED
#endif //NRT
#ifdef DOS32
!!! NOT SUPPORTED
#endif //def DOS32
#ifndef CPU188
U08 tmkSaveMask21 = 0;
U08 tmkAllMask21 = 0;
U08 tmkSaveMaskA1 = 0;
U08 tmkAllMaskA1 = 0;
#else //def CPU188
U16 tmkSaveMask21 = 0;
U16 tmkAllMask21 = 0;
#endif //ndef CPU188
#endif //def MASKTMKS

#ifndef CPU188
U08 __tmkOldMask21 = 0xFF;
U08 __tmkOldMaskA1 = 0xFF;
U08 __tmkMask21[NTMK]; // DUP(0)
U08 __tmkMaskA1[NTMK]; // DUP(0)
#else //def CPU188
U16 __tmkOldMask21 = 0xFFFF;
U16 __tmkMask21[NTMK]; // DUP(0)
#endif //ndef CPU188
U08 __tmkIrq1[NTMK]; // DUP(0xFF)
BOOL __tmkIrqShared[NTMK]; // DUP(0)
U16 __tmkIrqPort[NTMK];
U16 __tmkIrqBit[NTMK];
#ifdef QNX4VME
U08 __tmkIrqLevel[NTMK];
#endif //def QNX4VME
#endif //def DOSQNX4

#ifdef WIN95
U08 __tmkIrq1[NTMK];
#endif

#ifndef DrvIO
#define TMKIRQOFF_T BOOL
#else
#define TMKIRQOFF_T U16
#endif
// tmkIrqOff may be set asynchronously after register disable
// tmkIrqOff should be cleared synchronously after register enable or
// may be cleared asynchronously if irqs were kept reset before it
TMKIRQOFF_T __tmkIrqOff[NTMK];

U16 __mtCW[NTMK];
#if DRV_MAX_BASE > 255
U16 __bcLinkBaseN[NTMK][DRV_MAX_BASE+1];
U16 __bcLinkCCN[NTMK][DRV_MAX_BASE+1];
#else
U16 __bcLinkWN[NTMK][DRV_MAX_BASE+1];
#endif
////U32 bcLinkWPtr[NTMK];
////                DD      bcLinkW0
////                IRPC    N, 1234567
////                DD      bcLinkW&N&
////                ENDM
U16 __bcCmdWN[NTMK][DRV_MAX_BASE+1];
////U32 bcCmdWPtr[NTMK];
////                DD      bcCmdW0
////                IRPC    N, 1234567
////                DD      bcCmdW&N&
////                ENDM

U16 __tmkTxRxMode[NTMK];

#ifdef NMBCID
int __mbcAlloc[NMBCID];
int __mbci[NMBCID];
U16 __mbcBase[NMBCID][NTMK];
U16 __mbcTmkN[NMBCID][NTMK];
U16 __mbcPort[NMBCID][NTMK];
U16 __mbcData[NMBCID][NTMK];
U16 __mbcPort0[NMBCID][NTMK];
U16 __mbcData0[NMBCID][NTMK];
#endif //NMBCID

char __TMKLL_Ver[] = "TMKNLL v8.06";
#ifdef CPU188
char __ch186[] = "-188";
#endif
#ifdef RAMwoCLI
char __chRAMwoCLI[] = ".a";
#else
char __chRAMwoCLI[] = ".b";
#endif
#ifdef MASKTMKS
char __chMASKTMKS[] = ".a";
#else
char __chMASKTMKS[] = ".b";
#endif

//;_DATA  ENDS
//;VXD_LOCKED_DATA_ENDS

// GET_*/REST_* explanations:
//
// GET_MUTEX/REST_MUTEX are only for DOS or any other OS with direct interrupt
// user callbacks able to call inside of such user callbacks any other related
// functions changing device modes (write or read/modify/write) during user
// interrupt processing
// Maximally they should frame any read/modify/write for any global driver
// variable which can be changed by user in interrupt callback and also any
// read/outport, write/outport, read/modify/write/outport for such variables
// Historically they only covered __bcControls, __bcControls1, __rtControls,
// __rtControls1 (this one is most critical for TMKX as it's used in driver
// interrupt handler) as such variables in connection with functions:
//  tmksetcwbits,tmkclrcwbits
//  rtreset (only outps for shorter RT reinit)
//  bcdefirqmode,rtdefirqmode
//  bcstart,bcdefbus (for RTMK400)
//  mtstop (for TMKX)
//  mtstartx (in bcstartx, for TMKX)
//  rtsetanswbits,rtclranswbits
//  rtdefpage,rtdefpagebus
//  rtdefaddress (only to cover rtControls1 irq volatility for TMKX)
//  rtdefmode (only to cover rtControls1 irq volatility for TMKX/MRTX)
//  DrvRtWMode (only to cover rtControls1 irq volatility for TMKX/MRTX)
// To limit driver complexity it looks resonable to support these only for
// DOS backward compatibility, leaving other cases not covered and to be a
// subject to user-managed interrupt disable/enable (also Tmknll_IRQ.doc).
//
// GET_DIS_IRQ/REST_IRQ should frame:
// 1.
// Any device RAM operations where actual RAM address cannot be read from
// register (TMK400, RTMK400, TMKMPC, TMKX(I), MRTX(I))
// 2.
// Any device operations that should be or preferred to be performed at once
//
// GET_DIS_IRQ_SMP/REST_IRQ_SMP are only for SMP OS and should frame:
// 1.
// Every write (if not already framed with GET_DIS_IRQ/REST_IRQ) to any
// register which can be written during normal interrupt processing inside
// internal driver interrupt handler (DIRQLTmksInt1, DIRQLTmkSave/Restore),
// namely:
//  TMK400, RTMK400 writes in irq to TMK_Ctrl (RT mode only)
//  TMKMPC writes in irq to TMKMPC_CtrlH, TMKMPC_CtrlHL (RT mode only)
//  TMKX(I) writes in irq to TMK_Ctrl (RT mode only)
//  MRTX(I) writes in irq to TMK_Ctrl, TMK_Addr, TMK_Data
//  TA(I) writes in irq to TA_TIMCR, TA_MODE1, TA_ADDR, TA_DATA
//  MRTA(I) writes in irq to TA_TIMCR, TA_MODE1, MRTA_ADDR2, TA_ADDR, TA_DATA
// These don't cover any possible extra user raw io writes in irq
// 2.
// Every write or read (if not already framed with GET_DIS_IRQ/REST_IRQ) to
// any register that could reset device interrupt request concurrently with
// driver interrupt handler:
//  TMK400, RTMK400:
//    write TMK_Reset
//    read  TMK_State=TMK_Ctrl (BC)
//    write TMK_Ctrl with special RT_CLEAR_INT (RT)
//  TMKMPC:
//    write TMKMPC_Reset
//    read  TMKMPC_StateL=TMKMPC_CtrlL (BC)
//    write TMKMPC_CtrlL with special RT_CLEAR_INT (RT)
//  TMKX(I):
//    write TMK_Reset
//    write TMK_Mode changing TMK_IRQ_OFF, GENER1_BL, GENER2_BL
//    read  TMK_Mode (BC/MT)
//    read  TMK_State=TMK_Ctrl (BC/MT)
//    write TMK_Ctrl with special RT_CLEAR_INT (RT)
//  MRTX(I):
//    write TMK_Reset
//  TA(I)
//    write TA_RESET
//    read  TA_IRQ
//    write TA_MODE1 changing TA_IRQ_EN, TA_FIFO_RESET
//    write TA_TIMCR changing TIMER_RESET
//  MRTA(I)
//    write TA_RESET
//    read  TA_IRQ
//    write TA_MODE1 changing TA_IRQ_EN, TA_FIFO_RESET
//    write TA_TIMCR changing TIMER_RESET
//  as for write or read that could set device interrupt request, that would
//  depend on circumstances
//
// Note on MRTX SMP RAM read/write processing:
// MRTX is the only type using __tmkRAMInWork and __tmkRAMAddr in irq handler
// (in DrvRtPokeIrqMRTX and DrvRtPeekIrqMRTX calls). Extra DPC MRTX calls are
// not counted because in SMP environment they are called exactly as DPC (i.e.
// not within irq handler directly). For correct SMP processing it is required
// either frame with GET_DIS_IRQ_SMP/REST_IRQ_SMP every single _tmkRAMInWork,
// __tmkRAMAddr, addr port, data port access or frame any sequence changing
// data address (i.e.: __tmkRAMAddr + addr port write; __tmkRAMAddr + data
// port access). The second approach is used without framing of __tmkRAMInWork,
// however in such a case __tmkRAMInWork shouldn't be set in both environments
// (user code and irq handler) otherwise it can be set at some moment forever,
// slowing down further RAM access with unneeded address restore on every such
// access. So DrvRtPokeIrqMRTX and DrvRtPeekIrqMRTX don't set __tmkRAMInWork,
// supposing they cannot be interrupted and reentered.

#ifndef DrvIO

#ifdef DOS
#define IRQ_FLAGS
#define IRQ_FLAGS_F
/*
void __INLINE GET_DIS_IRQ()
{
  asm pushf;
  asm cli;
}

void __INLINE REST_IRQ()
{
  asm popf;
}
*/
#ifdef DOS32
void GET_DIS_IRQ();
#pragma aux GET_DIS_IRQ = \
" push    eax " \
" mov     eax, 0900h " \
" int     31h " \
" xchg    eax, [esp] " \
__parm [] __modify [esp];
/*
" pushfd " \
" cli " \
*/
void REST_IRQ();
#pragma aux REST_IRQ = \
" xchg    eax, [esp] " \
" int     31h " \
" pop     eax " \
__parm [] __modify [esp];
/*
" popfd " \
*/
#else //notDOS32
#define GET_DIS_IRQ() { \
  asm pushf; \
  asm cli; \
}
#define REST_IRQ() { \
  asm popf; \
}
#endif //def DOS32
#define GET_MUTEX GET_DIS_IRQ()
#define REST_MUTEX REST_IRQ()
#define GET_DIS_IRQ_F() GET_DIS_IRQ()
#define REST_IRQ_F() REST_IRQ()
#define GET_DIS_IRQ_SMP()
#define REST_IRQ_SMP()

#else //notDOS
/*
_inline void GET_DIS_IRQ()
{
  __asm
  {
    pushfd
    cli
  }
}

_inline void REST_IRQ()
{
  __asm
  {
    popfd
  }
}
*/
#ifdef QNX4
#define IRQ_FLAGS
#define IRQ_FLAGS_F
#pragma aux GET_DIS_IRQ = "pushfd" "cli" __parm [] __modify [esp];
void GET_DIS_IRQ();
#pragma aux REST_IRQ = "popfd" __parm [] __modify [esp];
void REST_IRQ();
#define GET_MUTEX
#define REST_MUTEX
#define GET_DIS_IRQ_F() GET_DIS_IRQ()
#define REST_IRQ_F() REST_IRQ()
#define GET_DIS_IRQ_SMP()
#define REST_IRQ_SMP()

#else
#ifdef LINUX
#define IRQ_FLAGS
#define IRQ_FLAGS_F
#include <linux/spinlock.h>
/*
#define IRQ_FLAGS
#define GET_DIS_IRQ() { \
  asm volatile ( \
  "pushfl\n\t" \
  "cli\n\t" \
  : \
  : \
  : "esp"); \
}
#define REST_IRQ() { \
  asm volatile ( \
  "popfl\n\t" \
  : \
  : \
  : "esp"); \
}
*/
//#define IRQ_FLAGS unsigned irq_flags
//#define GET_DIS_IRQ() { save_flags(irq_flags); cli(); }
//#define REST_IRQ() { restore_flags(irq_flags); }
extern spinlock_t tmkIrqSpinLock;
#define GET_DIS_IRQ() { spin_lock_irq(&tmkIrqSpinLock); }
#define REST_IRQ() { spin_unlock_irq(&tmkIrqSpinLock); }
#define GET_MUTEX
#define REST_MUTEX
#define GET_DIS_IRQ_F() GET_DIS_IRQ()
#define REST_IRQ_F() REST_IRQ()
#ifdef __SMP__
#define GET_DIS_IRQ_SMP() { spin_lock_irq(&tmkIrqSpinLock); }
#define REST_IRQ_SMP() { spin_unlock_irq(&tmkIrqSpinLock); }
#else
#define GET_DIS_IRQ_SMP()
#define REST_IRQ_SMP()
#endif //def __SMP__

#else
#ifdef QNX6
#define IRQ_FLAGS
#define IRQ_FLAGS_F
/*#define GET_DIS_IRQ() { \
  asm ("pushfl"); \
  asm ("cli"); \
}
#define REST_IRQ() { \
  asm ("popfl"); \
}*/
extern intrspin_t  tmkIrqSpinLock;
#define GET_DIS_IRQ(){InterruptLock(&tmkIrqSpinLock);}
#define REST_IRQ(){InterruptUnlock(&tmkIrqSpinLock);}
#define GET_MUTEX
#define REST_MUTEX
#define GET_DIS_IRQ_F() GET_DIS_IRQ()
#define REST_IRQ_F() REST_IRQ()
#ifdef __SMP__
#define GET_DIS_IRQ_SMP(){InterruptLock(&tmkIrqSpinLock);}
#define REST_IRQ_SMP(){InterruptUnlock(&tmkIrqSpinLock);}
#else
#define GET_DIS_IRQ_SMP()
#define REST_IRQ_SMP()
#endif

#else // WINDOWS, DDKNT, etc.
#define IRQ_FLAGS
#define IRQ_FLAGS_F KIRQL irqflagsf
volatile unsigned long __tmkIrqSpinLockSMP = 1;
/*
//#define GET_DIS_IRQ() { \
//  __asm {pushfd}; \
//  __asm {cli}; \
//}
//#define REST_IRQ() { \
//  __asm {popfd}; \
//}
*/
#define GET_MUTEX
#define REST_MUTEX
//#define GET_DIS_IRQ_SMP()
//#define REST_IRQ_SMP()

/*
#define GET_DIS_IRQ() \
{ \
  unsigned long _spintmp, _irqflags; \
  { \
    __asm {pushfd}; \
    __asm {cli}; \
    __asm {pop eax}; \
    __asm {mov _irqflags, eax}; \
    while (1) \
    { \
      __asm {lock dec __tmkIrqSpinLockSMP}; \
      __asm {pushfd}; \
      __asm {pop eax}; \
      __asm {mov _spintmp, eax}; \
      if ((_spintmp & 0x80) == 0) \
        break; \
      while (__tmkIrqSpinLockSMP <= 0) \
      { \
        __asm {rep nop}; \
      } \
    } \
  }

#define REST_IRQ() \
  { \
    __tmkIrqSpinLockSMP = 1; \
    __asm {mov eax, _irqflags}; \
    __asm {push eax}; \
    __asm {popfd}; \
  } \
}
*/

#ifndef KeMemoryBarrierWithoutFence
#pragma warning( push )
#pragma warning( disable : 4793 )

#ifndef FORCEINLINE
#define FORCEINLINE __inline
#endif

FORCEINLINE
VOID
KeMemoryBarrier (
    VOID
    )
{
    LONG Barrier;
    __asm {
        xchg Barrier, eax
    }
}

#pragma warning( pop )
#endif

#define GET_DIS_IRQ() \
{ \
  LONG _spintmp; \
  KIRQL _irqflags; \
  { \
    KeRaiseIrql(HIGH_LEVEL, &_irqflags); \
    while (1) \
    { \
      _spintmp = InterlockedDecrement((PLONG)(&__tmkIrqSpinLockSMP)); \
      if (_spintmp == 0) \
        break; \
      while (__tmkIrqSpinLockSMP <= 0) \
      { \
        KeMemoryBarrier(); \
      } \
    } \
  }

#define REST_IRQ() \
  { \
    __tmkIrqSpinLockSMP = 1; \
    KeLowerIrql(_irqflags); \
  } \
}

#define GET_DIS_IRQ_F() \
{ \
  LONG _spintmp; \
  KeRaiseIrql(HIGH_LEVEL, &irqflagsf); \
  while (1) \
  { \
    _spintmp = InterlockedDecrement((PLONG)(&__tmkIrqSpinLockSMP)); \
    if (_spintmp == 0) \
      break; \
    while (__tmkIrqSpinLockSMP <= 0) \
    { \
      KeMemoryBarrier(); \
    } \
  } \
}

#define REST_IRQ_F() \
{ \
  __tmkIrqSpinLockSMP = 1; \
  KeLowerIrql(irqflagsf); \
}

#define GET_DIS_IRQ_SMP() GET_DIS_IRQ()
#define REST_IRQ_SMP() REST_IRQ()

#endif //def QNX6
#endif //def LINUX
#endif //def QNX4
#endif //def DOS

#ifdef USE_TMKRAWIO

#if defined(DOS) || defined(QNX4) || defined(LINUX) || defined(QNX6)

#define IRQ_FLAGS_RAW \
unsigned irqflagscnt = 0

#define GET_DIS_IRQ_RAW() { \
  if (irqflagscnt++ == 0) \
    GET_DIS_IRQ(); \
}
#define REST_IRQ_RAW() { \
  if (irqflagscnt != 0) \
  { \
    if (--irqflagscnt == 0) \
      REST_IRQ(); \
  } \
}
#define EXIT_IRQ_ALL_RAW() { \
  if (irqflagscnt != 0) \
  { \
    irqflagscnt = 0; \
    REST_IRQ(); \
  } \
}

#ifdef __SMP__
#define GET_DIS_IRQ_SMP_RAW() GET_DIS_IRQ_RAW()
#define REST_IRQ_SMP_RAW() REST_IRQ_RAW()
#else
#define GET_DIS_IRQ_SMP_RAW()
#define REST_IRQ_SMP_RAW()
#endif

#else // WINDOWS, DDKNT, etc.

#define IRQ_FLAGS_RAW \
KIRQL irqflags; \
unsigned irqflagscnt = 0

#define GET_DIS_IRQ_RAW() { \
  if (irqflagscnt++ == 0) \
    GET_DIS_IRQ(); /* contains opening '{' !!! */ \
    irqflags = _irqflags; \
  } \
}
#define REST_IRQ_RAW() { \
  if (irqflagscnt != 0) \
  { \
    if (--irqflagscnt == 0) \
    { \
      KIRQL _irqflags; \
      _irqflags = irqflags; \
    REST_IRQ(); /* contains closing '}' !!! */ \
  } \
}
#define EXIT_IRQ_ALL_RAW() { \
  if (irqflagscnt != 0) \
  { \
    KIRQL _irqflags; \
    _irqflags = irqflags; \
    irqflagscnt = 0; \
  REST_IRQ(); /* contains closing '}' !!! */ \
}

#define GET_DIS_IRQ_SMP_RAW() GET_DIS_IRQ_RAW()
#define REST_IRQ_SMP_RAW() REST_IRQ_RAW()

#endif //def DOS/QNX4/LINUX/QNX6

#endif //def USE_TMKRAWIO

#endif //ndef DrvIO

/*
#define GET_DIS_IRQ { \
  __asm \
  { \
     pushfd \
     cli \
  } \
}
#define REST_IRQ { \
  __asm \
  { \
    popfd \
  } \
}
*/

#ifdef DOS

#ifdef DOS32
#define outpb outp
#define inpb inp
#else
#define outpw outport
#define inpw inport
#define outpb outportb
#define inpb inportb
#endif

#else
#ifdef LINUX

#define outpw(port, data) outw(data, port)
#define inpw(port) inw(port)
#define outpb(port, data) outb(data, port)
#define inpb(port) inb(port)

#else
#ifdef QNX4VME

#define outpw myoutpw
#define inpw myinpw
#define inpb inp
#define outpb outp

//#pragma off (check_stack);

void myoutpw(U16 addr, U16 data)
{
//!!! works with single window only!!!
  *(__vmeWin + ((unsigned)addr>>1)) = (U16)(((data>>8) & 0x00FF) | ((data<<8) & 0xFF00));
}
//MYOUT           MACRO
//                xchg    al, ah
//                mov     [edx], ax
//                xchg    al, ah
//                ENDM

U16 myinpw(U16 addr)
{
  unsigned register data;
//!!! works with single window only!!!
  data = *(__vmeWin + ((unsigned)addr>>1));
  return (U16)(((data>>8) & 0x00FF) | ((data<<8) & 0xFF00));
}
//MYIN            MACRO
//                movzx   eax, word ptr [edx]
//                xchg    al, ah
//                ENDM

//#pragma on (check_stack);

#else
/*
#pragma warning(disable:4035)

unsigned __inline inpb(unsigned port)
{
  __asm
  {
    xor eax, eax
    mov edx, port
    in al, dx
  }
}

unsigned __inline inpw(unsigned port)
{
  __asm
  {
    xor eax, eax
    mov edx, port
    in ax, dx
  }
}

#pragma warning(default:4035)

void __inline outpb(unsigned port, unsigned data)
{
  __asm
  {
    mov eax, data
    mov edx, port
    out dx, al
  }
}

void __inline outpw(unsigned port, unsigned data)
{
  __asm
  {
    mov eax, data
    mov edx, port
    out dx, ax
  }
}
*/

#define inpb inp
#define outpb outp

//void ___outpw(unsigned port, unsigned data) { if (port < 0x8000 || port > 0xF000) { __asm { int 3 }; } outpw(port, data); }
//unsigned ___inpw(unsigned port) { if (port < 0x8000 || port > 0xF000) { __asm { int 3 }; } return inpw(port); }

#endif //def QNX4VME
#endif //def LINUX
#endif //def DOS

#ifndef DrvIO

#define inpandoroutpw(port, and, or) outpw((port), (inpw(port) & (and)) | (or))

#define inptrw(port, ptr) *(ptr) = inpw(port)
#define inptrwu(port, ptr) *(ptr) = inpw(port)

#endif //#ndef DrvIO

#ifndef fallthrough
#define fallthrough
#endif

#define DrvBcDefBaseTA(realnum, base) { \
  __bcBasePC[realnum] = base; \
}

__INLINE
void DrvBcPokeTA(int REGPAR realnum, unsigned pos, unsigned data)
{
  LOCAL_PORT

  SET_LOCAL_PORT(realnum);
  outpw(TA_ADDR(realnum), (__bcBasePC[realnum] << 6) + pos);
  outpw(TA_DATA(realnum), data);
////  DrvBcPoke(realnum, TA, pos, data);
}

//PokeTA(PeekTA()) does not work in macro
/*
//#define DrvBcPokeTA(realnum, pos, data) { \
//  outpw(TA_ADDR(realnum), (__bcBasePC[realnum] << 6) + pos); \
//  outpw(TA_DATA(realnum), data); \
//}
*/

//__INLINE
void DrvBcDefBase(int REGPAR realnum, unsigned type, unsigned base);
//__INLINE
//void DrvBcDefBaseTA(int REGPAR realnum, unsigned base);

void DrvBcPoke(int REGPAR realnum, unsigned type, unsigned pos, unsigned data);
//__INLINE
//void DrvBcPokeTA(int REGPAR realnum, unsigned pos, unsigned data);

unsigned DrvBcPeek(int REGPAR realnum, unsigned type, unsigned pos);
//__INLINE
//unsigned DrvBcPeekTA(int REGPAR realnum, unsigned pos);
//__inline
__INLINE
unsigned DrvBcPeekTA(int REGPAR realnum, unsigned pos)
{
  unsigned data;
  LOCAL_PORT

  SET_LOCAL_PORT(realnum);
  outpw(TA_ADDR(realnum), (__bcBasePC[realnum] << 6) + pos);
  data = inpw(TA_DATA(realnum));
  return data;
//  return DrvBcPeek(realnum, TA, pos);
}

/*
//#define DrvBcPeekTA(realnum, pos) \
//  outpw(TA_ADDR(realnum), (__bcBasePC[realnum] << 6) + pos), \
//  inpw(TA_DATA(realnum));
*/

#define DrvBcPeekPtrTA(num, pos, ptr) *(ptr) = DrvBcPeekTA(num, pos)

void DrvRtPoke(int __tmkNumber, unsigned type, unsigned pos, unsigned data);
//void DrvRtPokeTA(int __tmkNumber, unsigned base, unsigned pos, unsigned data);

unsigned DrvRtPeek(int __tmkNumber, unsigned type, unsigned pos);
//unsigned DrvRtPeekTA(int __tmkNumber, unsigned base, unsigned pos);

__INLINE
void DrvRtPokeTA(int REGPAR realnum, unsigned base, unsigned pos, unsigned data)
{
  LOCAL_PORT

  SET_LOCAL_PORT(realnum);
  outpw(TA_ADDR(realnum), (base << 6) + pos);
  outpw(TA_DATA(realnum), data);
//  DrvBcPoke(realnum, TA, pos, data);
}

/*
//#define DrvRtPokeTA(realnum, base, pos, data) { \
//  outpw(TA_ADDR(realnum), (base << 6) + pos); \
//  outpw(TA_DATA(realnum), data); \
//}
*/

__INLINE
unsigned DrvRtPeekTA(int REGPAR realnum, unsigned base, unsigned pos)
{
  unsigned data;
  LOCAL_PORT

  SET_LOCAL_PORT(realnum);
  outpw(TA_ADDR(realnum), (base << 6) + pos);
  data = inpw(TA_DATA(realnum));
  return data;
//  return DrvBcPeek(realnum, TA, pos);
}

/*
//#define DrvRtPeekTA(realnum, base, pos) \
//  outpw(TA_ADDR(realnum), (base << 6) + pos), \
//  inpw(TA_DATA(realnum));
*/

#ifndef DrvIO

#define DrvRtPeekPtrTA(num, base, pos, ptr) *(ptr) = DrvRtPeekTA(num, base, pos)

__INLINE
void DrvPeekAndOrPokeTA(int REGPAR realnum, unsigned base, unsigned pos, unsigned and, unsigned or)
{
  unsigned addr;
  unsigned data;
  LOCAL_PORT

  SET_LOCAL_PORT(realnum);
  addr = (base << 6) + pos;
  outpw(TA_ADDR(realnum), addr);
  data = (inpw(TA_DATA(realnum)) & and) | or;
  outpw(TA_ADDR(realnum), addr);
  outpw(TA_DATA(realnum), data);
  return;
}

#endif //ndef DrvIO

#ifdef MRTA
void DrvRtPokeMRTA(int REGPAR realnum, unsigned base, unsigned pos, unsigned data, unsigned basep)
{
  unsigned pg;
  LOCAL_PORT

  SET_LOCAL_PORT(realnum);
  if ((pg = (base >> 10)) != (basep >> 10))
    outpw(MRTA_ADDR2(realnum), pg);
  outpw(TA_ADDR(realnum), (base << 6) + pos);
  outpw(TA_DATA(realnum), data);
}

unsigned DrvRtPeekMRTA(int REGPAR realnum, unsigned base, unsigned pos, unsigned basep)
{
  unsigned data, pg;
  LOCAL_PORT

  SET_LOCAL_PORT(realnum);
  if ((pg = (base >> 10)) != (basep >> 10))
    outpw(MRTA_ADDR2(realnum), pg);
  outpw(TA_ADDR(realnum), (base << 6) + pos);
  data = inpw(TA_DATA(realnum));
  return data;
}
#endif //def MRTA

// DrvIO_TMKCONFIG_0 - optional tmkconfig stuff for I/O
// DrvIO_TMKCONFIG_1 - optional tmkconfig stuff for I/O
// DrvIO_DRVINITALL_1 - optional DrvInitAll stuff for I/O
// DrvIO_TMKDONE_1 - optional tmkdone stuff for I/O
// DrvIO_TMKDONE_0 - optional tmkdone stuff for I/O
// DrvIO_TMKSELECT_1 - optional tmkselect stuff for I/O
// DrvIONone - Disabler for I/O in function (non-reentrant by now! w/o Epi/Stop)
// DrvIOPro  - Prologue for small known size I/O based either on local buffer or on large synchronized external buffer
// DrvIOProV - Prologue for large unknown size I/O based on large synchronized external buffer
// DrvIOEpi  - Epilogue for I/O
// DrvIOStop - Cancel I/O in function
// all DrvIO supposed to use __tmkNumber except DrvIOSetParTmk
// check DrvIO instances for reentrancy (Pro should be ended with Epi/Stop)!!!
// also any inpw includes DrvIOEpi and new DrvIOPro
#ifndef DrvIO
#define DrvIONone
#define DrvIOPro(sizeportout, sizeportinout, sizeramout, sizeraminout, sizeportin, sizeramin)
#define DrvIOProV
#define DrvIOEpi
#define DrvIOStop
#define DrvIOSetPar(parnum, parvalue)
#define DrvIOSetParTmk(parnum, parvalue, tmk)
#define DrvIOSetParN1(parnum, parindex, parvalue)
//#define DrvIOSetParNN(parnum, parindex, parvalue, N)
//#define DrvIOSetParNi(parnum, parindex, parvalue, N, i)
#define DrvIOGetPar(parnum, parvalue)
#define DrvIOGetParPtr(parnum, parvalueptr)
#define DrvIOGetParPtrU(parnum, parvalueptr)
#define DrvIOGetParN1(parnum, parindex, parvalue)
#endif //def DrvIO
#ifdef DrvIOPro
// substitute DrvIOPro with DrvIOProV, otherwise all sizes should be rechecked
#undef DrvIOPro
#endif
#define DrvIOPro(sizeportout, sizeportinout, sizeramout, sizeraminout, sizeportin, sizeramin) \
DrvIOProV
#define IOPAR_TMKMODE 1
#define IOPAR_BCBASEBUS 2
//define IOPAR_3 3
#define IOPAR_BCXASTART 4
#define IOPAR_MTCW 5
#define IOPAR_BCMTSTATEX 6
#define IOPAR_TMKIRQOFF 7
#define IOPAR_BCAW1POS 8
#define IOPAR_BCAW2POS 9
#if DRV_MAX_BASE < 256
//#define IOPARN_BCLINKWN 12
#else
#define IOPARN_BCLINKBASEN 10
#define IOPARN_BCLINKCCN 11
#endif

void DrvRtWMode(int REGPAR num, unsigned type, unsigned mode);
void DrvFlagModeTA(int REGPAR num, unsigned m);

void (FARIR *tmkUserErrors)(void) = retfLabel;

#define DEF_VAR(vartype, var) vartype var

#define GET_VAR(varloc, varpar) varloc = varpar

#if NRT > 0
#define GET_RealNum (__amrtNumber[__tmkNumber])
#define GET_RealNumOf(num) (__amrtNumber[num])
#else
#define GET_RealNum (__tmkNumber)
#define GET_RealNumOf(num) (num)
#endif

#define PUT_RealNum(num) {}

#define PUSH_RealNum {}
#define POP_RealNum {}

/*
GET_RealNum2    MACRO   reg
                mov     reg, __tmkNumber2
                movzx   reg, __amrtNumber2[reg]
                ENDM
GET_RealNum2R   MACRO   reg, reg2
                movzx   reg, __amrtNumber2[reg2]
                ENDM

PUT_RealNum2    MACRO   reg
                ENDM
PUSH_RealNum2   MACRO
                ENDM

POP_RealNum2    MACRO
                ENDM
*/



#define SYN_LPT { \
  outpb(0x378, 1); \
  outpb(0x378, 0); \
}
// ?
//#define INT3 { asm db 0cch; }
// w32
//#define INT3 { __asm {int 3}; };

#ifdef STATIC_TMKNUM

#define CLRtmkError { \
  tmkError = 0; \
}

#else

#ifdef NTMKLL_CLR_TMK_ERROR
#define CLRtmkError { \
  tmkError[__tmkNumber] = 0; \
}
#else
#define CLRtmkError {}
#endif

#endif //def DOS

/*
CLRtmkError    MACRO
#ifdef   NTMKLL_CLR_TMK_ERROR
                mov     eax, __tmkNumber2
                mov     tmkError[eax*2], 0
#endif
                ENDM
*/

/*
SYN_LPT         MACRO
                push    eax
                push    edx
                mov     edx, 378h
                mov     eax, 1
                out     dx, ax
                mov     eax, 0
                out     dx, ax
                pop     edx
                pop     eax
                ENDM
*/

#define CONVERT_TMKX_SW_BITS(bits, bitst) { \
    bits = bitst & 0x04; \
    if (bitst & 0x01) \
      bits |= 0x02; \
    if (bitst & 0x02) \
      bits |= 0x01; \
    if (bitst & 0x08) \
      bits |= 0x10; \
    if (bitst & 0x10) \
      bits |= 0x08; \
}

#define CONVERT_TA_SW_BITS(bits, bitst) { \
    bits = 0; \
    if (bitst & SREQ) \
      bits |= SREQ_MASK; \
    if (bitst & BUSY) \
      bits |= BUSY_MASK; \
    if (bitst & SSFL) \
      bits |= SSFL_MASK; \
    if (bitst & RTFL) \
      bits |= RTFL_MASK; \
    if (bitst & DNBA) \
      bits |= DNBA_MASK; \
}

#ifdef STATIC_TMKNUM
#define USER_ERROR(err) (tmkError = err)
#define USER_ERROR_R USER_ERROR
#else
#define USER_ERROR(err) (tmkError[__tmkNumber] = err)
#define USER_ERROR_R(err) (err)
#endif

/*
USER_ERROR      MACRO   err
                mov     eax, __tmkNumber2
                mov     tmkError[eax*2], err
                mov     eax, err
//;                call    dword ptr tmkUserErrors
                ENDM

USER_ERROR_R    MACRO   err
                mov     eax, err
//;                call    dword ptr tmkUserErrors
                ENDM
*/

#ifndef NOCHECK
#define CHECK_TMK_REAL_NUMBER(num) { \
  if (num >= NTMK) \
    return USER_ERROR_R(TMK_BAD_NUMBER); \
}
#else
#define CHECK_TMK_REAL_NUMBER(num)
#endif

#ifndef NOCHECK
#define CHECK_TMK_NUMBER(num) { \
  if ((unsigned)num > (unsigned)__tmkMaxNumber) \
    return USER_ERROR_R(TMK_BAD_NUMBER); \
}
#else
#define CHECK_TMK_NUMBER(num)
#endif

#ifndef NOCHECK
#define CHECK_TMK_MODE(num, ReqMode) { \
  if (__tmkMode[realnum] != ReqMode) \
    return USER_ERROR(TMK_BAD_FUNC); \
}
#else
#define CHECK_TMK_MODE(num, ReqMode)
#endif

#ifndef NOCHECK
#define CHECK_TMK_MODE_BCMT(num) { \
  if ((__tmkMode[num] != BC_MODE) && !(__tmkMode[num] & MWT_MODE)) \
  { \
    DrvIOStop; \
    return USER_ERROR(TMK_BAD_FUNC); \
  } \
}
#else
#define CHECK_TMK_MODE_BCMT(num)
#endif

#ifndef NOCHECK
#define CHECK_TMK_MODE_BCMTN(num) { \
  if ((__tmkMode[num] != BC_MODE) && !(__tmkMode[num] & MWT_MODE)) \
  { \
    USER_ERROR(TMK_BAD_FUNC); \
    DrvIOStop; \
    return; \
  } \
}
#else
#define CHECK_TMK_MODE_BCMTN(num)
#endif

#ifndef NOCHECK
#ifdef DOS
#ifndef CPU188
#define CHECK_IRQ(irq) { \
  if (irq > 0xF) \
    return USER_ERROR(TMK_BAD_IRQ); \
}
#else //def CPU188
#define CHECK_IRQ(irq) { \
  if (irq > 0x6) \
    return USER_ERROR(TMK_BAD_IRQ); \
}
#endif //ndef CPU188
#else
#define CHECK_IRQ(irq)
#endif //def DOS
#else
#define CHECK_IRQ(irq)
#endif

#ifdef QNX4VME
#ifdef USE_TMKRAWIO
#define CHECK_TMK_TYPE_1(type) { \
  if ((unsigned)(type) != TMKX && (unsigned)(type) != MRTX && (unsigned)(type) != TA && (unsigned)(type) != MRTA && (unsigned)(type) != TSTX && (unsigned)(type) != TSTA) \
    return USER_ERROR_R(TMK_BAD_TYPE); \
}
#else
#define CHECK_TMK_TYPE_1(type) { \
  if ((unsigned)(type) != TMKX && (unsigned)(type) != MRTX && (unsigned)(type) != TA && (unsigned)(type) != MRTA) \
    return USER_ERROR_R(TMK_BAD_TYPE); \
}
#endif
#else
#ifndef NOCHECK
#define CHECK_TMK_TYPE_1(type) { \
  if ((unsigned)(type) < TMK_MIN_TYPE || (unsigned)(type) > TMK_MAX_TYPE) \
  { \
    DrvIOEpi; \
    return USER_ERROR_R(TMK_BAD_TYPE); \
  } \
}
#else
#define CHECK_TMK_TYPE_1(type)
#endif
#endif //def QNX4VME

#ifndef NOCHECK
#define CHECK_TMK_TYPE_2(type) { \
  if ((unsigned)(type) > DRV_MAX_TYPE) \
    return USER_ERROR_R(TMK_BAD_TYPE); \
}
#else
#define CHECK_TMK_TYPE_2(type)
#endif

#define CHECK_TMK_TYPE(type)
//;#ifndef NOCHECK
//;                cmp     reg, TMK_MIN_TYPE
//;                jb      &lbl&_TmkBadType
//;                cmp     reg, TMK_MAX_TYPE
//;                jbe     &lbl&_TmkTypeOk
//;&lbl&_TmkBadType:
//;                USER_ERROR(TMK_BAD_TYPE
//;                jmp     &lbl&_Exit
//;#endif

#define CHECK_BC_BASE_BX(num, base) { \
  if (base > __bcMaxBase[num]) \
  { \
    DrvIOStop; \
    return USER_ERROR(BC_BAD_BASE); \
  } \
}

#if DRV_MAX_BASE > 255
#define CHECK_BCMT_BASE_BX(num, base) { \
  if ((__tmkMode[num] == BC_MODE && base > __bcMaxBase[num]) || \
      (__tmkMode[num] != BC_MODE && base > __mtMaxBase[num])) \
    return USER_ERROR(BC_BAD_BASE); \
}
#else
#define CHECK_BCMT_BASE_BX CHECK_BC_BASE_BX
#endif

#define CHECK_BC_ADDR(addr) { addr &= 0x3F; }

#define CHECK_BC_LEN(len) { \
  if (len > 64) \
  { \
    USER_ERROR(BC_BAD_LEN); \
    DrvIOStop; \
    return; \
  } \
}

#define CHECK_BC_BUS(bus) { bus &= 1; }

#define CHECK_BC_CTRL(ctrl) { ctrl &= 0xF; }

#define CHECK_BC_CTRLX(ctrlx) { ctrlx &= 0x803F; }

#define CHECK_MT_CTRLX(ctrlx) { ctrlx &= 0x8030; }

#define CHECK_RT_DIR(dir) { dir &= 0x0400; }

#define CHECK_RT_SUBADDR(sa) { sa &= 0x1F; }

#define CHECK_RT_SUBPOS(pos) { pos &= 0x1F; }

#define CHECK_RT_LEN(len) { \
  if (len > 32) \
  { \
    USER_ERROR(RT_BAD_LEN); \
    DrvIOStop; \
    return; \
  } \
}

#define CHECK_RT_LEN_R(len) { \
  if (len > 32) \
  { \
    DrvIOStop; \
    return USER_ERROR(RT_BAD_LEN); \
  } \
}

#define CHECK_RT_PAGE_BX(num, page) { \
  if (page > __rtMaxPage[num]) \
    return USER_ERROR(RT_BAD_PAGE); \
}

/*
#define CHECK_RT_ADDRESS(addr) { \
  if (addr > 0x1E) \
  { \
    DrvIOStop; \
    return USER_ERROR(RT_BAD_ADDRESS); \
  } \
}
*/

#define CHECK_RT_CMD(cmd) { cmd &= 0x041F; }

#ifdef DOS

#ifdef DOS32
#define IRQ2INT(irq) (((irq) >= 8) ? (irq) - 0x08 + __tmkPicA0Base : (irq) + __tmkPic20Base)
#else
#ifndef CPU188
#define IRQ2INT(irq) (((irq) >= 8) ? (irq) + 0x68 : (irq) + 0x08)
#else //def CPU188
// INT0-INT4 -> 0Ch-10h; INT5-INT6 -> 0Ah-0Bh
#define IRQ2INT(irq) (((irq) >= 5) ? (irq) + 0x05 : (irq) + 0x0C)
#endif //ndef CPU188
#endif //def DOS32

#ifdef MASKTMKS
#ifndef CPU188
#define DrvMaskTmks { \
  tmkSaveMask21 = inpb(0x21); \
  outpb(0x21, tmkSaveMask21 | tmkAllMask21); \
  tmkSaveMaskA1 = inpb(0xA1); \
  outpb(0xA1, tmkSaveMaskA1 | tmkAllMaskA1); \
}
#define DrvUnmaskTmks { \
  outpb(0x21, tmkSaveMask21); \
  outpb(0xA1, tmkSaveMaskA1); \
}
#else //def CPU188
#define DrvMaskTmks { \
  tmkSaveMask21 = inpw(0xFF28); \
  _AX = tmkSaveMask21 | tmkAllMask21; \
  outpb(0xFF28, _AL); \
}
#define DrvUnmaskTmks { \
  _AX = tmkSaveMask21; \
  outpb(0xFF28, _AL); \
}
#endif //ndef CPU188
#endif //def MASKTMKS

#ifndef CPU188
#define DrvEndInt(num) { \
  if (__tmkMaskA1[num]) \
    outpb(0xA0, 0x20); \
  outpb(0x20, 0x20); \
}
#else //def CPU188
#define DrvEndInt(num) { \
  _AX = 0x8000; \
  outpb(0xFF22, _AL); \
}
#endif //ndef CPU188

#endif //def DOS

//;_TEXT  SEGMENT PARA USE32 PUBLIC 'CODE'
//;VXD_LOCKED_CODE_SEG
//                .CODE

void MyUserErrors(unsigned err)
{
#ifdef STATIC_TMKNUM
  tmkError = err;
  tmkUserErrors();
#endif
}

unsigned inpb_d(int REGPAR num, unsigned port)
{
  unsigned t;
  unsigned data;

  data = inpb(port);
  t = __wInDelay[num];
  while (--t);
  return data;
}

unsigned inpw_d(int REGPAR num, unsigned port)
{
  unsigned t;
  unsigned data;

  data = inpw(port);
  t = __wInDelay[num];
  while (--t);
  return data;
}

void outpb_d(int REGPAR num, unsigned port, unsigned data)
{
  unsigned t;

  outpb(port, data);
  t = __wOutDelay[num];
  while (--t);
  return;
}

void outpw_d(int REGPAR num, unsigned port, unsigned data)
{
  unsigned t;

  outpw(port, data);
  t = __wOutDelay[num];
  while (--t);
  return;
}

#define REP_OUTSWB_D(port) { \
  do \
  { \
    unsigned data; \
    data = *(buf++); \
    /*lodsw*/ \
    GET_DIS_IRQ(); \
    outpb((port)+1, data >> 8);\
    outpb_d(realnum, port, data); \
    ++__tmkRAMAddr[realnum]; \
    REST_IRQ(); \
  } \
  while (--len != 0); \
}

#define REP_OUTSWB_D_IRQ(port) { \
  do \
  { \
    unsigned data; \
    data = *(buf++); \
    /*lodsw*/ \
    outpb((port)+1, data >> 8);\
    outpb_d(realnum, port, data); \
  } \
  while (--len != 0); \
}

#define REP_INSW(port) { \
  do \
  { \
    GET_DIS_IRQ(); \
    inptrw(port, (buf++)); \
    /*insw*/ \
    ++__tmkRAMAddr[realnum]; \
    REST_IRQ(); \
  } \
  while (--len != 0); \
}

#define REP_INSW_IRQ(port) { \
  do \
  { \
    inptrw(port, (buf++)); \
    /*insw*/ \
  } \
  while (--len != 0); \
}

#define REP_OUTSW(port) { \
  do \
  { \
    GET_DIS_IRQ(); \
    outpw(port, *(buf++)); \
    /*outsw*/ \
    ++__tmkRAMAddr[realnum]; \
    REST_IRQ(); \
  } \
  while (--len != 0); \
}

#define REP_OUTSW_IRQ(port) { \
  do \
  { \
    outpw(port, *(buf++)); \
    /*outsw*/ \
  } \
  while (--len != 0); \
}

#define outpwb_d(num, port, data) { \
  GET_DIS_IRQ(); \
  outpb(port, (data) >> 8); \
  outpb_d(num, (port)-1, data); \
  REST_IRQ(); \
}

//#define inpwb_d(port)
//;    GET_DIS_IRQ();
//;  al = inpb_d(port);
//;                mov     ah, al
//;                inc     edx
//;                in      al, dx
//;//;  al = inpb_d(port);
//;  REST_IRQ();
//;                xchg    al, ah

//rep_inswb_d                //;case __TMKMPC1:    GET_DIS_IRQ
//;//;                insb
//;                INSB_D
//;                inc     edx
//;//;                insb
//;                INSB_D
//;  REST_IRQ();
//;                dec     edx
//;                inc     __tmkRAMAddr[ebx*2];
//;                loop    case __TMKMPC1
/*
INSB_D          MACRO
                call    insb_1d
                ENDM

insb_1d         PROC    NEAR
                push    ebx
                insb
                movzx   ebx, __wInDelay[ebx]
i_sb_d:         dec     ebx
                jnz     i_sb_d
                pop     ebx
                ret
insb_1d         ENDP
*/

//the function always works as ifndef RAMwoCLI
#define REP_INSW_D(port) { \
  do \
  { \
    int t; \
    GET_DIS_IRQ(); \
    *(buf++) = inpw(port); \
    /*insw*/ \
    ++__tmkRAMAddr[realnum]; \
    REST_IRQ(); \
    t = __wInDelay[realnum]; \
    while (--t); \
  } \
  while (--len != 0); \
}

//the function always works as ifndef RAMwoCLI
#define REP_OUTSW_D(port) { \
  do \
  { \
    int t; \
    GET_DIS_IRQ(); \
    outpw(port, *(buf++)); \
    /*outsw*/ \
    ++__tmkRAMAddr[realnum]; \
    REST_IRQ(); \
    t = __wOutDelay[realnum]; \
    while (--t); \
  } \
  while (--len != 0); \
}

#define REP_INSW_D_IRQ(port) { \
  do \
  { \
    int t; \
    *(buf++) = inpw(port); \
    /*insw*/ \
    t = __wInDelay[realnum]; \
    while (--t); \
  } \
  while (--len != 0); \
}

#define REP_OUTSW_D_IRQ(port) { \
  do \
  { \
    int t; \
    outpw(port, *(buf++)); \
    /*outsw*/ \
    t = __wOutDelay[realnum]; \
    while (--t); \
  } \
  while (--len != 0); \
}

#ifdef DOS
#ifndef CPU188

void ENABLE_IRQ_AL(int REGPAR num, U08 irq)
{
  U08 mask;
  if (irq < 8)
  {
    mask = 1 << irq;
    __tmkMask21[num] |= mask;
#ifdef MASKTMKS
    tmkAllMask21 |= mask;
#endif
  }
  else
  {
    mask = 1 << (irq - 8);
    __tmkMaskA1[num] |= mask;
#ifdef MASKTMKS
    tmkAllMaskA1 |= mask;
#endif
  }
}

void DISABLE_IRQ_AL(int REGPAR num, U08 irq)
{
  U08 mask;
  if (irq < 8)
  {
    mask = ~(1 << irq);
    __tmkMask21[num] &= mask;
#ifdef MASKTMKS
    tmkAllMask21 &= mask;
#endif
  }
  else
  {
    mask = ~(1 << (irq - 8));
    __tmkMaskA1[num] &= mask;
#ifdef MASKTMKS
    tmkAllMaskA1 &= mask;
#endif
  }
}

#else //def CPU188

void ENABLE_IRQ_AL(int REGPAR num, U08 irq)
{
  U16 mask;
  if (irq < 5)
    mask = 1 << (irq + 4);
  else
    mask = 1 << (irq - 3);
  __tmkMask21[num] |= mask;
#ifdef MASKTMKS
  tmkAllMask21 |= mask;
#endif
}

void DISABLE_IRQ_AL(int REGPAR num, U08 irq)
{
  U16 mask;
  if (irq < 5)
    mask = ~(1 << (irq + 4));
  else
    mask = ~(1 << (irq - 3));
  __tmkMask21[num] &= mask;
#ifdef MASKTMKS
  tmkAllMask21 &= mask;
#endif
}

#endif //ndef CPU188
#endif //def DOS

////////////////////////////////////////////////////////////////
//void L_SetMask(unsigned adr, unsigned mask, unsigned zero_mask)
//{
//  outpw(adr, (inpw(adr) & ~mask) & (~zero_mask));
//}

//void H_SetMask(unsigned adr, unsigned mask, unsigned zero_mask)
//{
//  outpw(adr, (inpw(adr) | mask) & (~zero_mask));
//}

void FARIR retfLabel()
{
}

#if defined(DOS) || defined(QNX4INT)
#ifdef QNX4
#define RETURN0 return 0
#else
#define RETURN0
#endif

RETIR FARIR retfRLabel3(U16 arg1, U16 arg2, U16 arg3)
{
  __tmkNumber = __tmkSaveNumber;
  RETURN0;
  (void)arg1;(void)arg2;(void)arg3;
}
RETIR FARIR retfRLabel2(U16 arg1, U16 arg2)
{
  __tmkNumber = __tmkSaveNumber;
  RETURN0;
  (void)arg1;(void)arg2;
}
RETIR FARIR retfRLabel1(U16 arg1)
{
  __tmkNumber = __tmkSaveNumber;
  RETURN0;
  (void)arg1;
}
RETIR FARIR retfLabel1(U16 arg1)
{
  RETURN0;
  (void)arg1;
}
#ifdef USE_TMKRAWIO
RETIR FARIR retfLabel2(U16 arg1, U16 arg2)
{
  RETURN0;
  (void)arg1;(void)arg2;
}
#endif
#endif //def DOS || QNX4INT

U32 FARFN tmkoptions(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U32 tmkOptions)
{
  int register num;
  int realnum;
  unsigned type;
  UOPT options, optionsall;
#if NRT > 0
  int irt, nrt;
#endif

  CLRtmkError;
  num = __tmkNumber;
  realnum = GET_RealNum;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  options = (UOPT)tmkOptions;
#ifdef TMKOPTIONS_MASK
  options &= __tmkOptionsMask[num] | (UOPT)GET_TMK_OPTIONS;
#if NRT > 0
  if (num != realnum)
    options &= __tmkOptionsMask[realnum] | (UOPT)GET_TMK_OPTIONS;
#endif
#endif
  if (!(options & (UOPT)GET_TMK_OPTIONS))
  {
    optionsall = options & (UOPT)RT_USE_ADDR_PARITY;
    switch (type)
    {
    case __TA:
      options &= ((UOPT)BC_STARTA_INTA | (UOPT)MTRT_ALLOW_ANOMALY | (UOPT)RT_SHOW_BRC_STATE);
      break;
#if NRT > 0
#ifdef MRTX
    case __MRTX:
#endif
#ifdef MRTA
    case __MRTA:
#endif
      irt = __mrtMinRT[realnum];
      nrt = __mrtNRT[realnum];
      if (num != realnum)
      {
        __tmkOptions[num] = (options & ((UOPT)MRT_WRITE_BRC_DATA | (UOPT)MRT_READ_BRC_DATA | (UOPT)RT_SHOW_BRC_STATE)) | optionsall;
        options = __tmkOptions[realnum] & ~(UOPT)MRT_READ_BRC_DATA;
        optionsall = (UOPT)0x0L;
        do
        {
          if (__tmkMode[irt] != UNDEFINED_MODE)
            options |= __tmkOptions[irt] & (UOPT)MRT_READ_BRC_DATA;
          ++irt;
        }
        while (--nrt != 0);
      }
      else
      {
        options &= ((UOPT)MRT_WRITE_BRC_DATA | (UOPT)MRT_READ_BRC_DATA | (UOPT)RT_SHOW_BRC_STATE);
        do
        {
          __tmkOptions[irt] = options | optionsall;
          ++irt;
        }
        while (--nrt != 0);
        options &= (UOPT)MRT_READ_BRC_DATA | (UOPT)RT_SHOW_BRC_STATE;
      }
      break;
#endif
    default:
      options = (UOPT)0x0L;
      break;
    }
    __tmkOptions[realnum] = options | optionsall;
  }
  options = __tmkOptions[num];
#if NRT > 0
  if (num != realnum)
    options |= __tmkOptions[realnum] & (UOPT)MRT_READ_BRC_DATA;
#endif
  return (U32)options;
}

U16 FARFN tmktxrxmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 tmkTxRxMode)
{
  int register realnum;
#ifdef MRTA
  int num;
  unsigned mrtrxctrl;
#endif
  unsigned type;
  LOCAL_PORT

  DrvIOPro(0, 1, 0, 0, 0, 0);
  CLRtmkError;
#ifdef MRTA
  num = __tmkNumber;
#endif
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  if (tmkTxRxMode < GET_TXRX_MODE_HW) // != GET and != GET_HW
  {
    switch (type)
    {
#ifdef MRTA
    case __MRTA:
      if (num != realnum && __mrtRxCtrl[realnum])
      {
        __mrtRxCtrl[num] = ~(U08)tmkTxRxMode & (U08)0xC0;
//        GET_MUTEX;
        __rtControls1[num] = (__rtControls1[num] & 0xFF3F) | __mrtRxCtrl[num];
        outpw(MRTA_SW(realnum), __rtControls1[num]);
//        REST_MUTEX;
        tmkTxRxMode = (tmkTxRxMode & 0x0300) | (__tmkTxRxMode[realnum] & 0x00C0);
      }
      // no break here
      // fall-through
#endif
      fallthrough;
      // fall-through
    case __TA:
      __tmkTxRxMode[realnum] = tmkTxRxMode &= 0x03C0;
      __bcControls[realnum] = (__bcControls[realnum] & 0xFC3F) | tmkTxRxMode;
      __rtControls[realnum] = (__rtControls[realnum] & 0xFC3F) | tmkTxRxMode;
      GET_DIS_IRQ();
      inpandoroutpw(TA_MODE1(realnum), 0xFC3F, tmkTxRxMode); // common for BC/RT
      REST_IRQ();
      break;
    }
  }
  switch (type)
  {
#ifdef MRTA
  case __MRTA:
    if (tmkTxRxMode == GET_TXRX_MODE_HW)
    {
      mrtrxctrl = 0;
      if (num != realnum && __mrtRxCtrl[realnum])
      {
        GET_DIS_IRQ();
        outpw(TA_LCW(realnum), (__rtControls1[num] & 0xF800));
        inptrwu(MRTA_SW(realnum), &mrtrxctrl);
        REST_IRQ();
      }
      __tmkTxRxMode[realnum] = tmkTxRxMode = inpw(TA_MODE1(realnum)) & 0x03C0; // DrvIOEpi, DrvIOPro
      __bcControls[realnum] = (__bcControls[realnum] & 0xFC3F) | tmkTxRxMode;
      __rtControls[realnum] = (__rtControls[realnum] & 0xFC3F) | tmkTxRxMode;
      if (num != realnum && __mrtRxCtrl[realnum])
        __mrtRxCtrl[num] = (U08)mrtrxctrl & (U08)0xC0;
    }
    tmkTxRxMode = __tmkTxRxMode[realnum];
    if (num != realnum && __mrtRxCtrl[realnum])
      tmkTxRxMode &= 0xFF3F | (__mrtRxCtrl[num] ^ (U08)0xC0);
    break;
#endif
  case __TA:
    if (tmkTxRxMode == GET_TXRX_MODE_HW)
    {
      __tmkTxRxMode[realnum] = tmkTxRxMode = inpw(TA_MODE1(realnum)) & 0x03C0;
      __bcControls[realnum] = (__bcControls[realnum] & 0xFC3F) | tmkTxRxMode;
      __rtControls[realnum] = (__rtControls[realnum] & 0xFC3F) | tmkTxRxMode;
    }
    tmkTxRxMode = __tmkTxRxMode[realnum];
    break;
  default:
    tmkTxRxMode = TXRX_ALL_ENABLE;
    break;
  }
  DrvIOEpi;
  return tmkTxRxMode;
}

U16 FARFN tmkdac(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Dac)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT

  DrvIOPro(1, 0, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    Dac = TMK_DAC_ERROR;
    break;
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
    if (Dac < TMK_DAC_NODATA)
    {
      Dac >>= 4; // convert 12 bit to 8 bit
      outpw(TMKH_Dac(realnum), Dac);
      Dac = TMK_DAC_NODATA;
    }
    else
    {
      switch (Dac)
      {
      case GET_DAC_TMK:
        Dac = TMK_DAC_NODATA;
        break;
      case GET_DAC_MAX:
        Dac = 0x0FF0;
        break;
      case GET_DAC_STEP:
        Dac = 0x0010;
        break;
      default:
        Dac = TMK_DAC_ERROR;
        break;
      }
    }
    break;
  }
  DrvIOEpi;
  return Dac;
}

U16 FARFN rtgap(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Gap)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT

  DrvIOPro(0, 1, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  if (Gap < GET_RT_GAP_HW) // != GET and != GET_HW
  {
    switch (type)
    {
    case __TA:
//#ifdef MRTA
//    case __MRTA:
//#endif
      if (__tmkHWVer[realnum] < 15)
        Gap = RT_GAP_DEFAULT;
      if (Gap > RT_GAP_OPT1)
        Gap = RT_GAP_OPT1;
      __rtGap[realnum] = Gap;
//      GET_DIS_IRQ();
      inpandoroutpw(TA_RTA(realnum), ~((1 << 14) | (1 << 11)), (Gap << 11));
//      REST_IRQ();
      break;
    }
  }
  switch (type)
  {
  case __TA:
//#ifdef MRTA
//  case __MRTA:
//#endif
    if (Gap == GET_RT_GAP_HW)
      __rtGap[realnum] = (inpw(TA_RTA(realnum)) >> 11) & 1;
    Gap = __rtGap[realnum];
    break;
  default:
    Gap = RT_GAP_DEFAULT;
    break;
  }
  DrvIOEpi;
  return Gap;
}

U16 FARFN tmktimeout(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 TimeOut)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT

  DrvIOPro(0, 1, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  if (TimeOut < GET_TIMEOUT_HW) // != GET and != GET_HW
  {
    switch (type)
    {
#if defined(QNX4VME) || defined(DOSVME)
    case __TMKX:
      if (TimeOut <= 14)
        __tmkTimeOut[realnum] = __TX_14US;
      else
        __tmkTimeOut[realnum] = __TX_26US;
      inpandoroutpw(TMKXV_Vect(realnum), 0x7FFF, __tmkTimeOut[realnum]);
      break;
#endif //def QNX4VME
    case __TA:
#ifdef MRTA
    case __MRTA:
#endif
      if (TimeOut <= 14)
        __tmkTimeOut[realnum] = __TA_14US;
      else if (TimeOut <= 18)
        __tmkTimeOut[realnum] = __TA_18US;
      else if (TimeOut <= 26)
        __tmkTimeOut[realnum] = __TA_26US;
      else
        __tmkTimeOut[realnum] = __TA_63US;
      __bcControls[realnum] = (__bcControls[realnum] & 0xCFFF) | __tmkTimeOut[realnum];
      __rtControls[realnum] = (__rtControls[realnum] & 0xCFFF) | __tmkTimeOut[realnum];
      GET_DIS_IRQ();
      inpandoroutpw(TA_MODE1(realnum), 0xCFFF, __tmkTimeOut[realnum]); // common for BC/RT
      REST_IRQ();
      break;
    }
  }
  switch (type)
  {
#if defined(QNX4VME) || defined(DOSVME)
  case __TMKX:
    if (TimeOut == GET_TIMEOUT_HW)
      __tmkTimeOut[realnum] = inpw(TMKXV_Vect(realnum)) & 0x8000;
    switch (__tmkTimeOut[realnum])
    {
    case __TX_26US:
      TimeOut = 26;
      break;
    case __TX_14US:
      TimeOut = 14;
      break;
    default:
      TimeOut = 0;
      break;
    }
    break;
#endif //def QNX4VME
  case __TA:
#ifdef MRTA
  case __MRTA:
#endif
    if (TimeOut == GET_TIMEOUT_HW)
      __tmkTimeOut[realnum] = inpw(TA_MODE1(realnum)) & 0x3000;
    switch (__tmkTimeOut[realnum])
    {
    case __TA_63US:
      TimeOut = 63;
      break;
    case __TA_26US:
      TimeOut = 26;
      break;
    case __TA_18US:
      TimeOut = 18;
      break;
    case __TA_14US:
      TimeOut = 14;
      break;
    default:
      TimeOut = 0;
      break;
    }
    break;
  default:
    TimeOut = 0;
    break;
  }
  DrvIOEpi;
  return TimeOut;
}

#ifdef DOS
U16 FARFN tmkswtimer(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 SwTimerCtrl)
{
  (void)SwTimerCtrl;
  return 0;
}

U32 FARFN tmkgetswtimer(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  return 0L;
}

U32 FARFN tmkgetevtime(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  return 0L;
}
#endif //def DOS

U16 FARFN tmktimer(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 TimerCtrl)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT

  DrvIOPro(2, 0, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TA:
#ifdef MRTA
  case __MRTA:
#endif
    if (TimerCtrl < GET_TIMER_CTRL_HW) // != GET and != GET_HW
    {
      if (TimerCtrl != TIMER_RESET)
      {
        if (TimerCtrl != TIMER_OFF)
          TimerCtrl |= TIMER_NOSTOP;
        __tmkTimerCtrl[realnum] = TimerCtrl & TIMER_MASK;
      }
      GET_DIS_IRQ_SMP();
      if (TimerCtrl == TIMER_RESET)
        outpw(TA_TIMCR(realnum), 0);
      outpw(TA_TIMCR(realnum), __tmkTimerCtrl[realnum]);
      REST_IRQ_SMP();
#ifdef MRTA
      if (type == __MRTA && __tmkNumber != realnum)
      {
        if ((__tmkTimerCtrl[realnum] & 0x0400) &&
           ((__tmkTimerCtrl[realnum] & TIMER_STEP) != TIMER_STOP))
          __dmrtaTimers[realnum] |= 1L << (__tmkNumber - __mrtMinRT[realnum]);
        else
          __dmrtaTimers[realnum] &= ~(1L << (__tmkNumber - __mrtMinRT[realnum]));
      }
#endif
    }
    if (TimerCtrl == GET_TIMER_CTRL_HW)
      __tmkTimerCtrl[realnum] = inpw(TA_TIMCR(realnum));
    TimerCtrl = __tmkTimerCtrl[realnum];
    break;
  default:
    TimerCtrl = 0;
    break;
  }
  DrvIOEpi;
  return TimerCtrl;
}

U16 FARFN tmkgettimerl(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  U16 timer;

  DrvIOPro(0, 0, 0, 0, 1, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TA:
#ifdef MRTA
  case __MRTA:
#endif
    timer = inpw(TA_TIMER2(realnum));
    break;
  default:
    timer = 0;
    break;
  }
  DrvIOEpi;
  return timer;
}

U32 FARFN tmkgettimer(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned timerh;
  unsigned timerl;

  DrvIOPro(2, 0, 0, 0, 2, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TA:
#ifdef MRTA
  case __MRTA:
#endif
    GET_DIS_IRQ();
    outpw(TA_TIMCR(realnum), __tmkTimerCtrl[realnum] | 0x0800);
    inptrwu(TA_TIMER1(realnum), &timerh);
    inptrwu(TA_TIMER2(realnum), &timerl);
    outpw(TA_TIMCR(realnum), __tmkTimerCtrl[realnum]);
    REST_IRQ();
    break;
  default:
    timerh = 0;
    timerl = 0;
    break;
  }
  DrvIOEpi;
  return (((U32)timerh) << 16) | (U32)timerl;
}

U32 FARFN bcgetmsgtime(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned type;
  LOCAL_PORT
  unsigned timeh;
  unsigned timel;

  DrvIOPro(0, 0, 0, 0, 1, 1);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TA:
    SET_LOCAL_PORT(num);
    GET_DIS_IRQ_SMP();
    DrvBcPeekPtrTA(num, 59, &timeh);
    inptrwu(TA_DATA(num), &timel);
    REST_IRQ_SMP();
    break;
  default:
    timeh = 0;
    timel = 0;
    break;
  }
  DrvIOEpi;
  return (((U32)timeh) << 16) | (U32)timel;
}

U32 FARFN rtgetmsgtime(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned timeh;
  unsigned timel;
#ifdef MRTA
  U16 *buf;
#endif

  DrvIOPro(1, 0, 0, 0, 2, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  switch (type)
  {
  case __TA:
    GET_DIS_IRQ_SMP();
    outpw(TA_ADDR(realnum), ((__hm400Page[num] ^ __rtSubAddr[num]) << 1) | 59);
    inptrwu(TA_DATA(realnum), &timeh);
    inptrwu(TA_DATA(realnum), &timel);
    REST_IRQ_SMP();
    break;
#ifdef MRTA
  case __MRTA:
    if (__rtPagePC[num] == (__rtMaxPage[num] + 2)) // brc sw page
    {
      buf = &(__awBrcRxState[realnum][((__rtSubAddr[num] & 0x03E0) >> 3) + 1]); // clear T/R
      GET_DIS_IRQ_SMP(); // just for uniformity with hw
      timeh = *buf;
      timel = *(buf + 1);
      REST_IRQ_SMP();
      break;
    }
    GET_DIS_IRQ_SMP();
    outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
    outpw(TA_ADDR(realnum), ((__hm400Page[num] ^ __rtSubAddr[num]) << 1) | 59);
    inptrwu(TA_DATA(realnum), &timeh);
    inptrwu(TA_DATA(realnum), &timel);
    REST_IRQ_SMP();
    break;
#endif
  default:
    timeh = 0;
    timel = 0;
    break;
  }
  DrvIOEpi;
  return (((U32)timeh) << 16) | (U32)timel;
}

void FARFN bcclrmsgtime(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned type;
  LOCAL_PORT

  DrvIOPro(1, 0, 1, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TA:
    SET_LOCAL_PORT(num);
    GET_DIS_IRQ_SMP();
    DrvBcPokeTA(num, 59, 0);
    outpw(TA_DATA(num), 0);
    REST_IRQ_SMP();
    break;
  }
  DrvIOEpi;
  return;
}

void FARFN rtclrmsgtime(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
#ifdef MRTA
  U16 *buf;
#endif

  DrvIOPro(3, 0, 0, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  switch (type)
  {
  case __TA:
    GET_DIS_IRQ_SMP();
    outpw(TA_ADDR(realnum), ((__hm400Page[num] ^ __rtSubAddr[num]) << 1) | 59);
    outpw(TA_DATA(realnum), 0);
    outpw(TA_DATA(realnum), 0);
    REST_IRQ_SMP();
    break;
#ifdef MRTA
  case __MRTA:
    if (__rtPagePC[num] == (__rtMaxPage[num] + 2)) // brc sw page
    {
      buf = &(__awBrcRxState[realnum][((__rtSubAddr[num] & 0x03E0) >> 3) + 1]); // clear T/R
      GET_DIS_IRQ_SMP(); // just for uniformity with hw
      *buf = 0;
      *(buf + 1) = 0;
      REST_IRQ_SMP();
      break;
    }
    GET_DIS_IRQ_SMP();
    outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
    outpw(TA_ADDR(realnum), ((__hm400Page[num] ^ __rtSubAddr[num]) << 1) | 59);
    outpw(TA_DATA(realnum), 0);
    outpw(TA_DATA(realnum), 0);
    REST_IRQ_SMP();
    break;
#endif
  }
  DrvIOEpi;
  return;
}

U16 FARFN bcgetmsgsw(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned type;
  U16 state;

  DrvIOPro(0, 0, 0, 0, 0, 1);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TA:
    GET_DIS_IRQ_SMP();
    state = DrvBcPeekTA(num, 58);
    REST_IRQ_SMP();
    break;
  default:
    state = 0xFFFF;
    break;
  }
  DrvIOEpi;
  return state;
}

U16 FARFN rtgetmsgsw(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  U16 state;
#ifdef MRTA
  U16 *buf;
#endif

  DrvIOPro(1, 0, 0, 0, 1, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  switch (type)
  {
  case __TA:
    GET_DIS_IRQ_SMP();
    outpw(TA_ADDR(realnum), ((__hm400Page[num] ^ __rtSubAddr[num]) << 1) | 58);
    state = inpw(TA_DATA(realnum));
    REST_IRQ_SMP();
    break;
#ifdef MRTA
  case __MRTA:
    if (__rtPagePC[num] == (__rtMaxPage[num] + 2)) // brc sw page
    {
      buf = &(__awBrcRxState[realnum][(__rtSubAddr[num] & 0x03E0) >> 3]); // clear T/R
      state = *buf;
      state = (state & 0x3FFF) | ((state & 0x2000) << 1); // add err bit as in TA
      break;
    }
    GET_DIS_IRQ_SMP();
    outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
    outpw(TA_ADDR(realnum), ((__hm400Page[num] ^ __rtSubAddr[num]) << 1) | 58);
    state = inpw(TA_DATA(realnum));
    REST_IRQ_SMP();
    state = (state & 0x3FFF) | ((state & 0x2000) << 1); // add err bit as in TA
    break;
#endif
  default:
    state = 0xFFFF;
    break;
  }
  DrvIOEpi;
  return state;
}

void FARFN bcclrmsgsw(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned type;

  DrvIOPro(0, 0, 1, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TA:
    GET_DIS_IRQ_SMP();
    DrvBcPokeTA(num, 58, 0);
    REST_IRQ_SMP();
    break;
  }
  DrvIOEpi;
  return;
}

void FARFN rtclrmsgsw(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
#ifdef MRTA
  U16 *buf;
#endif

  DrvIOPro(2, 0, 0, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  switch (type)
  {
  case __TA:
    GET_DIS_IRQ_SMP();
    outpw(TA_ADDR(realnum), ((__hm400Page[num] ^ __rtSubAddr[num]) << 1) | 58);
    outpw(TA_DATA(realnum), 0);
    REST_IRQ_SMP();
    break;
#ifdef MRTA
  case __MRTA:
    if (__rtPagePC[num] == (__rtMaxPage[num] + 2)) // brc sw page
    {
      buf = &(__awBrcRxState[realnum][(__rtSubAddr[num] & 0x03E0) >> 3]); // clear T/R
      *buf = 0;
      break;
    }
    GET_DIS_IRQ_SMP();
    outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
    outpw(TA_ADDR(realnum), ((__hm400Page[num] ^ __rtSubAddr[num]) << 1) | 58);
    outpw(TA_DATA(realnum), 0);
    REST_IRQ_SMP();
    break;
#endif
  }
  DrvIOEpi;
  return;
}

void DrvMtRtWriteRTA(int REGPAR realnum, unsigned ctrl, unsigned data1, unsigned data2, unsigned shift)
{
  unsigned data;
  LOCAL_PORT

  SET_LOCAL_PORT(realnum);
  for (data = data1; data <= data2; ++data)
  {
//    GET_DIS_IRQ_SMP();
    outpw(TA_RTA(realnum), ctrl | (data << shift));
//    REST_IRQ_SMP();
  }
}

U16 FARFN mtfilter(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 mtFilterCtrl, U16 mtFilter1, U16 mtFilter2)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned ctrl, ctrlw;
  unsigned filter=0;

  DrvIOProV;
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TA:
    ctrlw = (mtFilterCtrl & (MTF_FILTER_ON | MTF_ALLOW | MTF_WRITE)) | 0x1000 | (__rtGap[realnum] << 11);
    ctrl = mtFilterCtrl & ~(MTF_FILTER_ON | MTF_ALLOW);
    switch (ctrl)
    {
    case 0: // on/off with MTF_FILTER_ON/OFF only
      ctrlw &= MTF_FILTER_ON | 0x1000 | (1 << 11);
      DrvMtRtWriteRTA(realnum, ctrlw, 0, 0, 0);
      filter = MTF_OK | (ctrlw & 0xA000);
      break;
    case MTF_ALL:
      DrvMtRtWriteRTA(realnum, ctrlw, 0, 31, 5);
      filter = MTF_OK | (ctrlw & 0xA000);
      break;
    case MTF_ADDR:
      mtFilter1 &= 0x001F;
      mtFilter2 &= 0x001F;
      if (mtFilter1 <= mtFilter2)
      {
        DrvMtRtWriteRTA(realnum, ctrlw, mtFilter1, mtFilter2, 5);
        filter =  MTF_OK | (ctrlw & 0xA000) | mtFilter1;
      }
      break;
    case MTF_GET_ADDR:
//      GET_DIS_IRQ_SMP();
      // this should be called when MT/RT passive and with TMK_IRQ_OFF!!!
      outpw(TA_RTA(realnum), ctrlw | ((mtFilter1 & 0x001F) << 5));
      filter = inpw(TA_RTA(realnum));
//      REST_IRQ_SMP();
      filter = MTF_OK | (filter & 0xA000) | ((filter & 0x03E0) >> 5);
      break;
    }
    break;
  }
  DrvIOEpi;
  return (U16)filter;
}

#if NRT > 0
#ifdef MRTA
void DrvMrtaUpdateRTA(int REGPAR realnum, int updmrt, int updmrtb)
{
//Supposed to be called when RTL_LEGAL_ON only!!!
//Because uses RTL_LEGAL_ON in all writes
//Otherwise RTL_LEGAL_ON bit should be read back and checked
  unsigned addr, sa;
  unsigned legalbrc, legal, legalw, legalr;
  int limit;
  int irqoff;
  unsigned mode1=0;
  LOCAL_PORT

  SET_LOCAL_PORT(realnum);
//1. Make sure all legal on RTs with legalization off
  if (updmrt)
  {
    for (addr = 0; addr <= 30; ++addr)
      if ((__dmrtRT[realnum] & (1L << addr)) && !(__dmrtLgl[realnum] & (1L << addr)))
        DrvMtRtWriteRTA(realnum, RTL_LEGAL_ON | RTL_ALLOW | RTL_WRITE, addr << 6, (addr << 6) | 0x3F, 0);
  }

//2. Make Brc RT legalization as ORed of all used RTs if not explicitly defined
  if (updmrtb && !(__dmrtLgl[realnum] & (1L << 31)))
  {
    irqoff = __tmkIrqOff[realnum];
    if (!irqoff)
      mode1 = inpw(TA_MODE1(realnum));
    for (sa = 0; sa <= 0x1F; ++sa)
    {
      legalbrc = 0;
      for (addr = 0; addr <= 30; ++addr)
      {
        if (__dmrtRT[realnum] & (1L << addr))
        {
//          GET_DIS_IRQ_SMP();
          GET_DIS_IRQ();
          if (!irqoff)
          {
            outpw(TA_MODE1(realnum), mode1 & ~TA_IRQ_EN);
            __tmkIrqOff[realnum] = 1;
            DrvIOSetPar(IOPAR_TMKIRQOFF, 1);
          }
          limit = 1024; // abstract loop limit for broken hardware
          legalw = RTL_LEGAL_ON | (addr << 6) | sa; // no rtgap in MRTA
          do
          { // try to avoid influence of other possibly active virtual RTs
            // if called when MRT not fully passive
            outpw(TA_RTA(realnum), legalw);
            inptrwu(TA_RTA(realnum), &legalr);
            outpw(TA_RTA(realnum), legalw);
            legal = inpw(TA_RTA(realnum));
          }
          while ((legal != legalr || ((legal ^ legalw) & ~RTL_ALLOW) != 0) && --limit != 0);
          if (!irqoff)
          {
            __tmkIrqOff[realnum] = 0;
            DrvIOSetPar(IOPAR_TMKIRQOFF, 0);
            outpw(TA_MODE1(realnum), mode1);
          }
          REST_IRQ();
          legalbrc |= legal;
//          REST_IRQ_SMP();
        }
      }
//      GET_DIS_IRQ_SMP();
      outpw(TA_RTA(realnum), (legalbrc & RTL_ALLOW) | RTL_LEGAL_ON | RTL_WRITE | (31 << 6) | sa);
//      REST_IRQ_SMP();
    }
  }
}
#endif
#endif

U16 FARFN rtlegal(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtLegalCtrl, U16 rtLegal1, U16 rtLegal2)
{
  int register realnum;
#if NRT > 0
  int num;
#endif
  unsigned type;
  LOCAL_PORT
  unsigned ctrl, ctrlw;
#if NRT > 0
  unsigned addr;
  int limit;
#endif
  unsigned legal=0;

  DrvIOProV;
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TA:
    ctrlw = (rtLegalCtrl & (RTL_LEGAL_ON | RTL_ALLOW | RTL_WRITE)) | (__rtGap[realnum] << 11);
    ctrl = rtLegalCtrl & ~(RTL_LEGAL_ON | RTL_ALLOW);
    switch (ctrl)
    {
    case 0: // on/off with RTL_LEGAL_ON/OFF only
      ctrlw &= RTL_LEGAL_ON | (1 << 11);
      DrvMtRtWriteRTA(realnum, ctrlw, 0, 0, 0);
      legal = RTL_OK | (ctrlw & 0xA000);
      break;
    case RTL_ALL:
      DrvMtRtWriteRTA(realnum, ctrlw, 0x0000, 0x07FF, 0);
      legal = RTL_OK | (ctrlw & 0xA000);
      break;
    case RTL_SA:
      rtLegal1 &= 0x003F;
      rtLegal2 &= 0x003F;
      if (rtLegal1 <= rtLegal2)
      {
        DrvMtRtWriteRTA(realnum, ctrlw, rtLegal1 << 5, (rtLegal2 << 5) | 0x1F, 0);
        legal =  RTL_OK | (ctrlw & 0xA000) | rtLegal1;
      }
      break;
    case RTL_GET_SA:
//      GET_DIS_IRQ_SMP();
      // this should be called when MT/RT passive and with TMK_IRQ_OFF!!!
      outpw(TA_RTA(realnum), ctrlw | ((rtLegal1 & 0x003F) << 5));
      legal = inpw(TA_RTA(realnum));
//      REST_IRQ_SMP();
      legal = RTL_OK | (legal & 0xA000) | ((legal & 0x07E0) >> 5);
      break;
    case RTL_CMD:
      rtLegal1 &= 0x07FF;
      rtLegal2 &= 0x07FF;
      if (rtLegal1 <= rtLegal2)
      {
        DrvMtRtWriteRTA(realnum, ctrlw, rtLegal1, rtLegal2, 0);
        legal =  RTL_OK | (ctrlw & 0xA000) | rtLegal1;
      }
      break;
    case RTL_GET_CMD:
//      GET_DIS_IRQ_SMP();
      // this should be called when MT/RT passive and with TMK_IRQ_OFF!!!
      outpw(TA_RTA(realnum), ctrlw | (rtLegal1 & 0x07FF));
      legal = inpw(TA_RTA(realnum));
//      REST_IRQ_SMP();
      legal = RTL_OK | (legal & 0xA7FF);
      break;
    }
    break;
#ifdef MRTA
  case __MRTA:
    num = __tmkNumber;
    ctrlw = (rtLegalCtrl & (RTL_LEGAL_ON | RTL_ALLOW | RTL_WRITE));
    ctrl = rtLegalCtrl & ~(RTL_LEGAL_ON | RTL_ALLOW);
    if (ctrl == RTL_ALL_MRT)
    {
      __dmrtLgl[realnum] = (ctrlw & RTL_LEGAL_ON) ? 0xFFFFFFFFL : 0L;
      DrvMtRtWriteRTA(realnum, ctrlw, 0, 0x07FF, 0);
      legal = RTL_OK | (ctrlw & 0xA000);
    }
    else if (num == realnum && ctrl == 0)
    { // on/off with RTL_LEGAL_ON/OFF only
      ctrlw &= RTL_LEGAL_ON;
      __dmrtLgl[realnum] = (ctrlw & RTL_LEGAL_ON) ? 0xFFFFFFFFL : 0L;
      DrvMtRtWriteRTA(realnum, ctrlw, 0, 0, 0);
      legal = RTL_OK | ctrlw;
    }
    else
    {
      int fMrtCtrl = 0;
      int fWriteRT = 0, fUpdateMRT = 0, fUpdateMRTB = 0; // used if !fMrtCtrl
      unsigned legalw, legalr;

      switch (ctrl)
      {
        case RTL_ALL_BRCST:
        case RTL_SA_BRCST:
        case RTL_GET_SA_BRCST:
          fMrtCtrl = 1;
          addr = 31;
          break;
        case RTL_ALL_RT:
        case RTL_SA_RT:
        case RTL_GET_SA_RT:
          fMrtCtrl = 1;
          fallthrough;
          // fall-through
        case 0: // on/off with RTL_LEGAL_ON/OFF only
        case RTL_ALL:
        case RTL_SA:
        case RTL_GET_SA:
          addr = (num != realnum) ? __rtAddress[num] : 0x00FF;
          break;
        default:
          addr = 0x00FF;
          break;
      }
      if (addr <= 31)
      {
        // fully transparent TA behavior emulation (except RTL_CMD controls,
        // of course, not supported in MRTA hardware) for multiple RTs with
        // consecutive ON/OFF calls in MRTA can be done only with a full
        // rtlegal data array copy in driver to store rtlegal values during
        // RTL_LEGAL_OFF, RT off, RT undefined address or RT address change
        // and with special brc processing during interrupt

        if (ctrl == 0 || (ctrl & RTL_WRITE))
        {
          if  (ctrlw & RTL_LEGAL_ON)
          {
            fWriteRT = 1;
            fUpdateMRT = (__dmrtLgl[realnum] == 0L);
            fUpdateMRTB = 1;
            __dmrtLgl[realnum] |= (1L << addr);
          }
          else
          {
            if (__dmrtLgl[realnum] != 0L)
            {
              if (__dmrtLgl[realnum] == (1L << addr))
              { // only this single RT rtlon
                fWriteRT = 1;   // write but can be overwritten if other RT will rtlon
//                fUpdateMRT = 0; // no need to update because MRT becomes rtloff
//                fUpdateMRTB = 0;
                __dmrtLgl[realnum] = 0L;
              }
              else if ((__dmrtLgl[realnum] & (1L << addr)) == 0L)
              { // this RT already rtloff but there are other RTs rtlon
//                fWriteRT = 0;   // keep whole RT rtloff because MRT rtlon
//                fUpdateMRT = 0;
//                fUpdateMRTB = 0;
                ctrlw |= RTL_LEGAL_ON;
              }
              else
              { // this RT rtlon and there are other RTs rtlon
//                fWriteRT = 0;   // make whole RT rtloff because MRT rtlon
                fUpdateMRT = 1;
                fUpdateMRTB = 1;
                ctrlw |= RTL_LEGAL_ON;
                __dmrtLgl[realnum] &= ~(1L << addr);
              }
            }
            else
            {
              fWriteRT = 1;   // write but can be overwritten if other RT will rtlon
//              fUpdateMRT = 0; // no need to update because MRT rtloff
//              fUpdateMRTB = 0;
            }
          }
        }
        if (fMrtCtrl)
        {
          fWriteRT = 1;
          fUpdateMRT = 0;
          fUpdateMRTB = 0;
        }
        switch (ctrl)
        {
        case 0: // on/off with RTL_LEGAL_ON/OFF only
          ctrlw &= RTL_LEGAL_ON;
          if (fWriteRT)
            DrvMtRtWriteRTA(realnum, ctrlw, 0, 0, 0);
          legal = RTL_OK | ctrlw;
          break;
        case RTL_ALL_BRCST:
        case RTL_ALL_RT:
        case RTL_ALL:
          if (fWriteRT)
            DrvMtRtWriteRTA(realnum, ctrlw, addr << 6, (addr << 6) | 0x3F, 0);
          legal = RTL_OK | (ctrlw & 0xA000);
          break;
        case RTL_SA_BRCST:
        case RTL_SA_RT:
        case RTL_SA:
          rtLegal1 &= 0x003F;
          rtLegal2 &= 0x003F;
          if (rtLegal1 <= rtLegal2)
          {
            if (fWriteRT)
              DrvMtRtWriteRTA(realnum, ctrlw, (addr << 6) | rtLegal1, (addr << 6) | rtLegal2, 0);
            legal =  RTL_OK | (ctrlw & 0xA000) | rtLegal1;
          }
          break;
        case RTL_GET_SA_BRCST:
        case RTL_GET_SA_RT:
        case RTL_GET_SA:
//          GET_DIS_IRQ_SMP();
          // this should be called during MRT passive and with TMK_IRQ_OFF!!!
          limit = 1024; // abstract loop limit for broken hardware
          legalw = ctrlw | (addr << 6) | (rtLegal1 & 0x003F);
          do
          { // try to avoid influence of other possibly active virtual RTs
            // if called when MRT not fully passive
            outpw(TA_RTA(realnum), legalw);
            inptrwu(TA_RTA(realnum), &legalr);
            outpw(TA_RTA(realnum), legalw);
            legal = inpw(TA_RTA(realnum));
          }
          while ((legal != legalr || ((legal ^ legalw) & ~RTL_ALLOW) != 0) && --limit != 0);
//          REST_IRQ_SMP();
          legal = RTL_OK | (legal & 0xA03F);
          break;
        }
        if ((__dmrtLgl[realnum] & (1L << addr)) == 0L)
          legal &= ~RTL_LEGAL_ON;
        if ((legal & RTL_OK) && (fUpdateMRT | fUpdateMRTB))
          DrvMrtaUpdateRTA(realnum, fUpdateMRT, fUpdateMRTB);
      }
    }
    break;
#endif
  }
  DrvIOEpi;
  return (U16)legal;
}

#ifndef DrvIO

U16 FARFN tmkiodelay(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 IODelay)
{
  int register realnum;
  U16 iodelay;

  CLRtmkError;
  realnum = GET_RealNum;
  iodelay = __wInDelay[realnum];
  if (IODelay != GET_IO_DELAY)
  {
    if (IODelay == 0)
      IODelay = 1;
    __wInDelay[realnum] = IODelay;
    __wOutDelay[realnum] = IODelay;
  }
  return iodelay;
}

void FARFN tmkdeferrors(void (FARIR* UserErrors)(void))
{
#ifdef STATIC_TMKNUM
  CLRtmkError;
  tmkUserErrors = UserErrors;
#endif
}

#if defined(DOS) || defined(QNX4INT)

void FARFN bcdefintsig(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserSigBC)(U16))
{
  int register realnum;

  realnum = GET_RealNum;
  tmkUserSigBC[realnum] = UserSigBC;
}

void FARFN bcdefintx(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserXBC)(U16, U16))
{
  int register realnum;

  realnum = GET_RealNum;
  tmkUserXBC[realnum] = UserXBC;
}

void FARFN bcdefinta(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserABC)(U16, U16))
{
  int register realnum;

  realnum = GET_RealNum;
  tmkUserABC[realnum] = UserABC;
}

void FARFN bcdefintnorm(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserNormBC)(U16, U16, U16))
{
  int register realnum;

  realnum = GET_RealNum;
  tmkUserNormBC[realnum] = UserNormBC;
}

void FARFN bcdefintexc(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserExcBC)(U16, U16, U16))
{
  int register realnum;

  realnum = GET_RealNum;
  tmkUserExcBC[realnum] = UserExcBC;
}

void FARFN rtdefintdata(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserDataRT)(U16))
{
  tmkUserDataRT[__tmkNumber] = UserDataRT;
}

void FARFN rtdefintcmd(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserCmdRT)(U16))
{
  tmkUserCmdRT[__tmkNumber] = UserCmdRT;
}

void FARFN rtdefinterr(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserErrRT)(U16))
{
  tmkUserErrRT[__tmkNumber] = UserErrRT;
}

void FARFN mtdefintsig(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserSigMT)(U16))
{
  int register realnum;

  realnum = GET_RealNum;
  tmkUserSigMT[realnum] = UserSigMT;
}

void FARFN mtdefintx(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserXMT)(U16, U16))
{
  int register realnum;

  realnum = GET_RealNum;
  tmkUserXMT[realnum] = UserXMT;
}

void FARFN tmkdefintoth(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserOthTMK)(U16))
{
  int register realnum;

  realnum = GET_RealNum;
  tmkUserOthTMK[realnum] = UserOthTMK;
}

#ifdef USE_TMKRAWIO
void FARFN tmkdefintraw(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        RETIR (FARIR* UserRawTMK)(U16, U16))
{
  int register realnum;

  realnum = GET_RealNum;
  tmkUserRawTMK[realnum] = UserRawTMK;
}
#endif //def USE_TMKRAWIO

#endif //def DOS || QNX4INT

#endif //ndef DrvIO

//;        PUBLIC  tmkdefevent
//;                ALIGN   4
#ifdef STATIC_TMKNUM
//;tmkdefevent     (ebx, TmkEventU32, fEventSetU32
#else
//;tmkdefevent     (ebx, U32 __tmkNumber2, TmkEventU32, fEventSetU32
#endif
//;                mov     ebx, __tmkNumber2
//;                mov     eax, TmkEvent
//;                mov     hTmkEvent[ebx*2], eax
//;                mov     eax, fEventSet
//;                mov     fTmkEventSet[ebx*2], eax
//;                ret
//;tmkdefevent     ENDP

#if NRT > 0
int FARFN rt2mrt(int hTMK)
{
//;#ifdef   STATIC_TMKNUM
//;  CLRtmkError;
//;#endif
  if ((unsigned)hTMK > __tmkMaxNumber)
  {
//;#ifdef   STATIC_TMKNUM
//;                USER_ERROR(TMK_BAD_NUMBER
//;#endif
    return -1;
  }
  else
  {
    return __amrtNumber[hTMK];
  }
}
#endif //NRT

int FARFN tmkselect(int hTMK)
{
#ifndef STATIC_TMKNUM
  int __tmkNumber; // used in CLRtmkError
#endif
//;  CLRtmkError;
  CHECK_TMK_NUMBER(hTMK);
//  CHECK_TMK_TYPE_1(tmkUserType[hTMK]);
  CHECK_TMK_TYPE_2(__tmkDrvType[hTMK]);
#ifdef DrvIO_TMKSELECT_1
  DrvIO_TMKSELECT_1;
#endif
  __tmkNumber = hTMK;
  CLRtmkError;
  PUT_RealNum(__amrtNumber[hTMK]);
  return 0;
}

#ifdef STATIC_TMKNUM
#if NRT > 0
//__inline
int FARFN mrtselected()
{
//;  CLRtmkError;
  return GET_RealNum;
}
#endif //NRT

//__inline
int FARFN tmkselected()
{
//;  CLRtmkError;
  return __tmkNumber;
}
#endif

//__inline
int FARFN tmkgetmaxn()
{
//;  CLRtmkError;
//;                mov     eax, NTMK - 1
#ifdef TMK_REAL_MAX
  if (__tmkUseRealMax && __tmkMaxNumber < NTMK)
    return __tmkRealMax;
#endif
  return __tmkMaxNumber;
}

#if NRT > 0
//__inline
int FARFN mrtgetmaxn()
{
//;  CLRtmkError;
#ifdef TMK_REAL_MAX
  if (__tmkUseRealMax)
    return __tmkRealMax;
#endif
  return NTMK - 1;
}

int FARFN mrtgetrt0(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  CLRtmkError;
  return __mrtMinRT[GET_RealNum];
}

int FARFN mrtgetnrt(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  CLRtmkError;
  return __mrtNRT[GET_RealNum];
}
#endif //NRT

U16 FARFN tmkgetmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  CLRtmkError;
  return __tmkMode[__tmkNumber];
}

void FARFN tmksetcwbits(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 SetControl)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned port;
  U08 bits;

  DrvIONone; //because of byte I/O and hidden port
  CLRtmkError;
  realnum = GET_RealNum;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  SET_LOCAL_PORT(realnum);
  port = TMK_Mode(realnum);
  GET_MUTEX;
  switch (type)
  {
  case __TMKMPC:
    port = TMKMPC_Mode(realnum);
    fallthrough;
    // fall-through
  case __TMK400:
  case __RTMK400:
    bits = SetControl & TMK400CWBitsMask;
    if (__tmkMode[realnum] != RT_MODE)
    {
      bits |= __bcControls[realnum];
      __bcControls[realnum] = bits;
    }
    else
    {
      bits |= __rtControls[realnum];
      __rtControls[realnum] = bits;
    }
    outpb_d(realnum, port, bits);
    break;
#if defined(QNX4VME) || defined(DOSVME)
  case __TMKX:
    bits = (SetControl & 2) << 9; // red bit 10
    inpandoroutpw(TMKXV_Vect(realnum), 0xFFFF, bits);  // led ctrl (on)
    break;
#endif //def QNX4VME
  case __TA:
    port = __tmkPortsAddr2[realnum];
    if (port != 0xFFFF)
    {
      port += 0x54;
      bits = (SetControl & 1) << 2; // byte !!! common grn bit 2
      outpb(port, (inpb(port) | 0x2) | bits); // led ctrl (off)
    }
#if defined(QNX4VME) || defined(DOSVME)
    else
    {
      bits = ((SetControl & 6) << 9);// red bit 10, grn bit 11
      inpandoroutpw(TAV_Vect(realnum), 0xFFFF, bits); // led ctrl (on)
    }
#endif //def QNX4VME
    break;
  }
  REST_MUTEX;
}

void FARFN tmkclrcwbits(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 ClrControl)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned port;
  U08 bits;

  DrvIONone; //because of byte I/O and hidden port
  CLRtmkError;
  realnum = GET_RealNum;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  SET_LOCAL_PORT(realnum);
  port = TMK_Mode(realnum);
  GET_MUTEX;
  switch (type)
  {
  case __TMKMPC:
    port = TMKMPC_Mode(realnum);
    fallthrough;
    // fall-through
  case __TMK400:
  case __RTMK400:
    bits = ~(ClrControl & TMK400CWBitsMask);
    if (__tmkMode[realnum] != RT_MODE)
    {
      bits &= __bcControls[realnum];
      __bcControls[realnum] = bits;
    }
    else
    {
      bits &= __rtControls[realnum];
      __rtControls[realnum] = bits;
    }
    outpb_d(realnum, port, bits);
    break;
#if defined(QNX4VME) || defined(DOSVME)
  case __TMKX:
    bits = (ClrControl & 2) << 9; // red bit 10
    inpandoroutpw(TMKXV_Vect(realnum), ~bits, 0);  // led ctrl (off)
    break;
#endif //def QNX4VME
  case __TA:
    port = __tmkPortsAddr2[realnum];
    if (port != 0xFFFF)
    {
      port += 0x54;
      bits = (ClrControl & 1) << 2; // byte !!! common grn bit 2
      outpb(port, (inpb(port) | 0x2) & ~bits); // led ctrl (on)
    }
#if defined(QNX4VME) || defined(DOSVME)
    else
    {
      bits = ((ClrControl & 6) << 9);// red bit 10, grn bit 11
      inpandoroutpw(TAV_Vect(realnum), ~bits, 0); // led ctrl (off)
    }
#endif //def QNX4VME
    break;
  }
  REST_MUTEX;
}

U16 FARFN tmkgetcwbits(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
#if defined(QNX4VME) || defined(DOSVME)
  LOCAL_PORT
#endif //def QNX4VME
  U08 bits;

  DrvIONone; //because of byte I/O and hidden port
  CLRtmkError;
  realnum = GET_RealNum;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
#if defined(QNX4VME) || defined(DOSVME)
  SET_LOCAL_PORT(realnum);
#endif //def QNX4VME
  if (__tmkMode[realnum] != RT_MODE)
    bits = (U08)__bcControls[realnum];
  else
    bits = (U08)__rtControls[realnum];
  switch (type)
  {
//  case __TMKX:
//  case __MRTX:
//    bits = 0;
//    break;
  case __TMK400:
  case __RTMK400:
  case __TMKMPC:
    bits &= TMK400CWBitsMask;
    break;
#if defined(QNX4VME) || defined(DOSVME)
  case __TMKX:
    bits = (inpw(TMKXV_Vect(realnum)) >> 9) & 2; // led state
    break;
#endif //def QNX4VME
  case __TA:
    bits = 0;
    {
      unsigned port;
      port = __tmkPortsAddr2[realnum];
      if (port != 0xFFFF)
      {
        port += 0x54;
        bits = (inpb(port) >> 2) & 1; // byte !!! led state
      }
#if defined(QNX4VME) || defined(DOSVME)
      else
      {
        bits = (inpw(TAV_Vect(realnum)) >> 9) & 6; // led state
      }
#endif //def QNX4VME
    }
    break;
  default:
    bits = 0;
    break;
  }
  return (U16)bits;
}

#ifndef DrvIO

void FARFN bc_def_tldw(U16 TLDW)
{
  (void)TLDW;
}

void FARFN bc_enable_di()
{
}

void FARFN bc_disable_di()
{
}

#endif //ndef DrvIO

void rtcreatlink(int REGPAR num, unsigned sat, unsigned sa, unsigned len, int linkt)
{
  unsigned i;
  unsigned base, next;
  unsigned bufext;

#ifdef USE_TMK_WRAP_SA
#define SA_INT_LOC saint
  unsigned saint = 0x4000;
  if (__tmkWrapSA)
  {
    if ((sa & 0x1F) == __tmkWrapSA)
    {
      sa &= 0x1F;
      saint = __tmkWrapInt;
    }
  }
#else
#define SA_INT_LOC 0x4000
#endif

  bufext = __rtSABufExt[num][sa];
  GET_DIS_IRQ_SMP();
  DrvRtPokeTA(num, AdrTab, sat, sa|SA_INT_LOC);
//  DrvRtPokeTA(num, AdrTab, sa, sa);
  REST_IRQ_SMP();
  for (i = 0; i < len; ++i)
  {
    if (__rtSABufMode[num][sa] == SABUF_HEAP)
    {
      base = (i == 0) ? sa : bufext + i - 1;
      next = bufext + i;
    }
    else
    {
      base = sa + (i << 6);
      next = base + 64; //sa + ((i+1) << 6);
    }
    if (linkt)
      next = ((i+1)==len) ? sa : next;
    else
      next = base;
    GET_DIS_IRQ_SMP();
    DrvRtPokeTA(num, base, 63, next|SA_INT_LOC);
    REST_IRQ_SMP();
  }
#undef SA_INT_LOC
}

void rtcreattab(int REGPAR num, unsigned len)
{
  unsigned sa;
  LOCAL_PORT

  SET_LOCAL_PORT(num);
  outpw(TA_MSGA(num), AdrTab);

  //for MT
  GET_DIS_IRQ_SMP();
  DrvRtPokeTA(num, AdrTab, 0, 0x0000); // for mtgetstate
  DrvRtPokeTA(num, 0, 63, 0x0000); // unused base 0
  REST_IRQ_SMP();

  //for RT
  for (sa = 1; sa < 31; ++sa)
  {
    rtcreatlink(num, sa, sa, len, 0);      //rx sa
    rtcreatlink(num, sa|0x20, sa|0x20, len, 0); //tx sa
  }
  rtcreatlink(num, 0x1F, 0x1F, len, 0); //All Mode Cmds
  rtcreatlink(num, 0x20, 0x20, len, 0); //Cmd TX VECTOR
  rtcreatlink(num, 0x3F, 0x3F, len, 0); //Cmd TX BIT
}

#ifdef MRTA
void mrtcreatlink(int REGPAR realnum, unsigned addr, unsigned sat, unsigned sa, unsigned len)
{
  int linkt = 0;
  int num;
  unsigned i;
  unsigned base, next, base0;
  unsigned bufmode;
  unsigned bufext;
  LOCAL_PORT

#ifdef USE_TMK_WRAP_SA
#define SA_INT_LOC saint
  unsigned saint = 0x4000;
  if (__tmkWrapSA)
  {
    if ((sa & 0x1F) == __tmkWrapSA)
    {
      sa &= 0x1F;
      saint = (addr == 31) ? 0x4000 : __tmkWrapInt;
    }
  }
#else
#define SA_INT_LOC 0x4000
#endif

  SET_LOCAL_PORT(realnum);
  if (addr == 31)
  {
    bufmode = __rtSABufMode[realnum][sa];
    bufext = __rtSABufExt[realnum][sa];
  }
  else
  {
    num = __mrtA2RT[realnum][addr];
    bufmode = __rtSABufMode[num][sa];
    bufext = __rtSABufExt[num][sa];
  }
  base0 = 0x0800 | (addr << 6) | sa;
  GET_DIS_IRQ_SMP();
  outpw(MRTA_ADDR2(realnum), 0);
  DrvRtPokeTA(realnum, addr, sat, base0|SA_INT_LOC);
  REST_IRQ_SMP();
  for (i = 0; i < len; ++i)
  {
    if (bufmode == SABUF_HEAP)
    {
      base = (i == 0) ? base0 : bufext + i - 1;
      next = bufext + i;
    }
    else
    {
      base = base0;
      next = base0;
    }
    if (linkt)
      next = ((i+1)==len) ? base0 : next;
    else
      next = base;
    GET_DIS_IRQ_SMP();
    outpw(MRTA_ADDR2(realnum), base >> 10);
    DrvRtPokeTA(realnum, base, 63, next|SA_INT_LOC);
    REST_IRQ_SMP();
  }
#undef SA_INT_LOC
}

void mrtcreattab(int REGPAR realnum, unsigned addr)
{
  unsigned sa;

  //for RT
  for (sa = 1; sa < 31; ++sa)
  {
    mrtcreatlink(realnum, addr, sa, sa, 1);      //rx sa
    mrtcreatlink(realnum, addr, sa|0x20, sa|0x20, 1); //tx sa
  }
  mrtcreatlink(realnum, addr, 0x1F, 0x1F, 1); //All Mode Cmds
  mrtcreatlink(realnum, addr, 0x20, 0x20, 1); //Cmd TX VECTOR
  mrtcreatlink(realnum, addr, 0x3F, 0x3F, 1); //Cmd TX BIT
}

void DrvUpdateHeapAllocMRTA(int REGPAR realnum)
{
  unsigned bufextm;
  unsigned irt;
  unsigned nrt;
  unsigned sa;

  bufextm = MRTA_MRT_HEAP_MAX + 1;
//  GET_MUTEX;
  // find heap buf with lowest bufext addr
  nrt = __mrtNRT[realnum];
  for (irt = __mrtMinRT[realnum]; nrt != 0; ++irt,--nrt)
  {
    for (sa = 1; sa <= 0x3F; ++sa)
    {
      if (__rtSABufMode[irt][sa] == SABUF_HEAP)
      {
        if (__rtSABufExt[irt][sa] != 0 && __rtSABufExt[irt][sa] < bufextm)
          bufextm = __rtSABufExt[irt][sa];
      }
    }
  }
  for (sa = 1; sa <= 0x3F; ++sa)
  {
    if (__rtSABufMode[realnum][sa] == SABUF_HEAP)
    {
      if (__rtSABufExt[realnum][sa] != 0 && __rtSABufExt[realnum][sa] < bufextm)
        bufextm = __rtSABufExt[realnum][sa];
    }
  }
  __rtHeapAlloc[realnum] = bufextm;
//  REST_MUTEX;
}
#endif //def MRTA

void DrvUpdateHeapAllocTA(int REGPAR num)
{
  unsigned bufextm;
  unsigned bufsize;
  unsigned sa, sam;

  bufextm = __rtHeapIni[num]; //TA_RT_HEAP_MAX + 1;
  bufsize = 1;
  sam = 0x3F;
//  GET_MUTEX;
  // find heap buf with lowest bufext addr
  // find largest non-heap buf or latest sa of equal non-heap bufs
  for (sa = 0x3F; sa >= 1; --sa)
  {
    if (__rtSABufMode[num][sa] == SABUF_HEAP)
    {
      if (__rtSABufExt[num][sa] != 0 && __rtSABufExt[num][sa] < bufextm)
        bufextm = __rtSABufExt[num][sa];
    }
    else
    {
      if (__rtSABufSize[num][sa] > bufsize)
      {
        bufsize = __rtSABufSize[num][sa];
        sam = sa;
      }
    }
  }
  __rtHeapAlloc[num] = bufextm;
  __rtHeapMin[num] = sam + (bufsize - 1) * 64 + 1;
//  REST_MUTEX;
}

#if NRT > 0
#ifdef MRTA
unsigned DrvMrtaBrcRtOn(int REGPAR realnum)
{
  return (__dmrtBrc[realnum] != 0L/* || (__dmrtRT[realnum] & (1L << 31)) != 0L*/) ? MRTA_RT_ON : 0;
}
#endif
#endif

int FARFN bcreset(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned cntr=0;
#if NRT > 0
  unsigned irt;
  unsigned nrt;
  unsigned sa;
  int err; // because of tmkError can be switched off
#endif

  DrvIOPro(5, 0, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  __tmkMode[realnum] = BC_MODE;
  DrvIOSetPar(IOPAR_TMKMODE, BC_MODE);
  __tmkStarted[realnum] = 0;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
#if defined(TSTX) || defined(TSTA)
#ifdef TSTX
  case __TSTX:
#endif
#ifdef TSTA
  case __TSTA:
#endif
    __tmkMode[realnum] = UNDEFINED_MODE;
    DrvIOSetPar(IOPAR_TMKMODE, UNDEFINED_MODE);
    DrvIOEpi;
    return USER_ERROR(TMK_BAD_FUNC);
#endif
  case __TMK400:
  case __RTMK400:
    GET_DIS_IRQ_SMP();
    outpb_d(realnum, TMK_Reset(realnum), 0);
    REST_IRQ_SMP();
    cntr = BC_MODE;
    outpb_d(realnum, TMK_Mode(realnum), cntr);
    break;
  case __TMKMPC:
    GET_DIS_IRQ_SMP();
    outpb_d(realnum, TMKMPC_Reset(realnum), 0);
    REST_IRQ_SMP();
    cntr = BC_MODE;
    outpb_d(realnum, TMKMPC_Mode(realnum), cntr);
    break;
#ifdef MRTX
  case __MRTX:
    __tmkMode[realnum] = MRT_MODE;
    DrvIOSetParTmk(IOPAR_TMKMODE, MRT_MODE, realnum);
    err = 0;
    if (realnum != __tmkNumber)
    {
      __tmkMode[__tmkNumber] = RT_MODE;
      DrvIOSetPar(IOPAR_TMKMODE, RT_MODE);
      rtenable(TMKNUM__(__tmkNumber) RT_DISABLE);
      __tmkMode[__tmkNumber] = UNDEFINED_MODE;
      DrvIOSetPar(IOPAR_TMKMODE, UNDEFINED_MODE);
      USER_ERROR(TMK_BAD_FUNC);
#ifdef STATIC_TMKNUM
      err = tmkError;
#else
      err = tmkError[__tmkNumber];
#endif
      irt = __mrtMinRT[realnum];
      nrt = __mrtNRT[realnum];
      do
      {
        if (__tmkMode[irt++] != UNDEFINED_MODE)
        {
          __rtAddress[__tmkNumber] = 0x00FF;
          __rtDisableMask[__tmkNumber] = __RT_DIS_MASK[type];
          __rtBRCMask[__tmkNumber] = __RT_BRC_MASK[type];
          __rtEnableOnAddr[__tmkNumber] = 1;
          break;
        }
      }
      while (--nrt != 0);
      if (nrt != 0)
      {
        DrvIOEpi;
        return err;
      }
    }
    __hm400Page[realnum] = 0;
    __rtDisableMask[realnum] = __RT_DIS_MASK[type];
    __rtBRCMask[realnum] = __RT_BRC_MASK[type];
    __rtEnableOnAddr[realnum] = 1;
    __rtControls1[realnum] = MRTX_HBIT_MODE | MRTX_BRCST_MODE;
    __mrtCtrl0[realnum] = 0x1FF8;
    __mrtCtrl1[realnum] = 0x8000 | 0x1FF8;
    __mrtMask0[realnum] = 0x1FF8;
    __mrtMask1[realnum] = 0x1FF8;
    __rtControls[realnum] = TX_RT_DATA_INT_BLK;
    irt = __mrtMinRT[realnum];
    nrt = __mrtNRT[realnum];
    do
    {
      __tmkMode[irt] = UNDEFINED_MODE;
      DrvIOSetParTmk(IOPAR_TMKMODE, UNDEFINED_MODE, irt);
      __rtAddress[irt] = 0x00FF;
      __rtDisableMask[irt] = __RT_DIS_MASK[type];
      __rtBRCMask[irt] = __RT_BRC_MASK[type];
      __rtEnableOnAddr[irt] = 1;
      ++irt;
    }
    while (--nrt != 0);
    GET_DIS_IRQ();
    outpw(TMK_Reset(realnum), 0);
    outpw(TMK_Mode(realnum), __mrtCtrl0[realnum] | __rtControls[realnum]);
    // | __mrtMask0[realnum]; __rtControls->m__rtControls
    outpw(TMK_Mode(realnum), __mrtCtrl1[realnum] | __rtControls[realnum]);
    // | __mrtMask1[realnum]; __rtControls->m__rtControls
    outpw(TMK_Ctrl(realnum), __rtControls1[realnum] & __rtBRCMask[realnum]);
    irt = __mrtMinRT[realnum];
    nrt = __mrtNRT[realnum];
    do
    {                //; simple ram write because of get_dis_irq
      outpw(TMK_Addr(realnum), __hm400Page[irt]);
      outpw(TMK_Data(realnum), 0);            //; status bits (__rtControls) = 0
      ++irt;
    }
    while (--nrt != 0);
    REST_IRQ();
    DrvIOEpi;
    return err;
#endif
  case __TMKX:
    GET_DIS_IRQ(); // because of GENER*_BLK
    outpw(TMK_Reset(realnum), 0);
    cntr = (BC_MODE >> 7) | (GENER1_BLK + GENER2_BLK);
    outpw(TMK_Mode(realnum), cntr);
    __tmkIrqOff[realnum] = 0;
    DrvIOSetPar(IOPAR_TMKIRQOFF, 0);
    REST_IRQ();
    break;
  case __TA:
    GET_DIS_IRQ_SMP();
    outpw(TA_RESET(realnum), 0);
    outpw(TA_TIMCR(realnum), 0);
//    if (__tmkTimerCtrl[realnum] & TIMER_BITS)
//      outpw(TA_TIMCR(realnum), __tmkTimerCtrl[realnum]);
    cntr = (BC_MODE << 7) | (TXRX_ALL_ENABLE | TA_IRQ_EN);
    outpw(TA_MODE1(realnum), cntr | TA_FIFO_RESET);
    outpw(TA_MODE1(realnum), cntr);
    __tmkIrqOff[realnum] = 0;
    DrvIOSetPar(IOPAR_TMKIRQOFF, 0);
    REST_IRQ_SMP();
    __bcControls1[realnum] = BC_ERSTP_MODE | BC_BTSTP_MODE | BC_DNRVM_MODE;
    __bcCtrlWA[realnum] = 0x1D1F;
    outpw(TA_MODE2(realnum), __bcControls1[realnum]);
    outpw(TA_MSGA(realnum), 0); // for bcgetstate
    __FLAG_MODE[realnum] = 0; //this almost unneded
    __bcmtStateX[realnum] = 0;
    DrvIOSetPar(IOPAR_BCMTSTATEX, 0);
    break;
#ifdef MRTA
  case __MRTA:
    __tmkMode[realnum] = MRT_MODE;
    DrvIOSetParTmk(IOPAR_TMKMODE, MRT_MODE, realnum);
    err = 0;
    if (realnum != __tmkNumber)
    {
      __tmkMode[__tmkNumber] = RT_MODE;
      DrvIOSetPar(IOPAR_TMKMODE, RT_MODE);
      rtenable(TMKNUM__(__tmkNumber) RT_DISABLE);
      __tmkMode[__tmkNumber] = UNDEFINED_MODE;
      DrvIOSetPar(IOPAR_TMKMODE, UNDEFINED_MODE);
      __dmrtaTimers[realnum] &= ~(1L << __tmkNumber);
      USER_ERROR(TMK_BAD_FUNC);
#ifdef STATIC_TMKNUM
      err = tmkError;
#else
      err = tmkError[__tmkNumber];
#endif
      // now check if there are other active virtual RTs on this MRT:
      // if they exist - perform limited RT init and return
      // if no other RTs - perform full MRT reset
      irt = __mrtMinRT[realnum];
      nrt = __mrtNRT[realnum];
      do
      {
        if (__tmkMode[irt++] != UNDEFINED_MODE)
          break;
      }
      while (--nrt != 0);
      if (nrt != 0)
      {
        // common unused page
        __hm400Page0[__tmkNumber] = 0x00 | 0x1F; //0x20 | rtaddr;
        __hm400Page2[__tmkNumber] = __hm400Page0[__tmkNumber] >> 4;
        __hm400Page[__tmkNumber] = __hm400Page0[__tmkNumber] << 11;
//        __rtSubAddr2[__tmkNumber] = 0;
        __rtSABuf[__tmkNumber] = 0;
        if (__rtAddress[__tmkNumber] < 31)
        {
          __dmrtRT[realnum] &= ~(1L << __rtAddress[__tmkNumber]);
          __dmrtBrc[realnum] &= ~(1L << __rtAddress[__tmkNumber]);
          __dmrtLgl[realnum] &= ~(1L << __rtAddress[__tmkNumber]); //???
        }
        __rtAddress[__tmkNumber] = 0x00FF;
        __rtDisableMask[__tmkNumber] = __RT_DIS_MASK[type];
        __rtBRCMask[__tmkNumber] = __RT_BRC_MASK[type];
        __rtEnableOnAddr[__tmkNumber] = 1;
        __rtControls1[__tmkNumber] = 0xF800 | TA_BRCST_MODE; // | TA_HBIT_MODE;
        __FLAG_MODE[__tmkNumber] = 0;
        for (sa = 0; sa < 64; ++sa)
        {
          __rtSABufMode[__tmkNumber][sa] = SABUF_OFF;
          __rtSABufSize[__tmkNumber][sa] = 1;
//          __rtSABufExt[irt][sa] = 0; // not required for SABUF_OFF actually
        }
        DrvUpdateHeapAllocMRTA(realnum);
        DrvIOEpi;
        return err;
      }
    }
    __dmrtaTimers[realnum] = 0L;
    __dmrtRT[realnum] = 0L;
    __dmrtBrc[realnum] = 0L;
    __dmrtLgl[realnum] = 0L;
    __hm400Page0[realnum] = 0;
    __hm400Page2[realnum] = 0;
    __hm400Page[realnum] = 0;
//    __rtSubAddr2[realnum] = 0;
    __rtSABuf[realnum] = 0;
    __rtDisableMask[realnum] = __RT_DIS_MASK[type];
    __rtBRCMask[realnum] = __RT_BRC_MASK[type];
    __rtEnableOnAddr[realnum] = 1;
    __rtControls1[realnum] = TA_HBIT_MODE; // | 0xF800 | TA_BRCST_MODE;
    __FLAG_MODE[realnum] = 0;
    __rtControls[realnum] = TA_RT_DATA_INT_BLK | TXRX_ALL_ENABLE | (__tmkCBSND ? 0 : TA_CBSD_MODE) | TA_IRQ_EN;
    irt = __mrtMinRT[realnum];
    nrt = __mrtNRT[realnum];
    do
    {
      __tmkMode[irt] = UNDEFINED_MODE;
      DrvIOSetParTmk(IOPAR_TMKMODE, UNDEFINED_MODE, irt);
      // common unused page
      __hm400Page0[irt] = 0x00 | 0x1F; //0x20 | rtaddr;
      __hm400Page2[irt] = __hm400Page0[irt] >> 4;
      __hm400Page[irt] = __hm400Page0[irt] << 11;
//      __rtSubAddr2[irt] = 0;
      __rtSABuf[irt] = 0;
//      if (__rtAddress[irt] < 31)
//      {
//        __dmrtRT[realnum] &= ~(1L << __rtAddress[irt]);
//        __dmrtBrc[realnum] &= ~(1L << __rtAddress[irt]);
//        __dmrtLgl[realnum] &= ~(1L << __rtAddress[irt]);
//      }
      __rtAddress[irt] = 0x00FF;
      __rtDisableMask[irt] = __RT_DIS_MASK[type];
      __rtBRCMask[irt] = __RT_BRC_MASK[type];
      __rtEnableOnAddr[irt] = 1;
      __mrtRxCtrl[irt] = 0;
      __rtControls1[irt] = 0xF800 | TA_BRCST_MODE; // | TA_HBIT_MODE;
      __FLAG_MODE[irt] = 0;
      ++irt;
    }
    while (--nrt != 0);
    GET_DIS_IRQ_SMP();
    outpw(TA_RESET(realnum), 0);
    __mrtLastBrcTxRT[realnum] = 0;
    outpw(TA_TIMCR(realnum), 0);
//    if (__tmkTimerCtrl[num] & TIMER_BITS)
//      outpw(TA_TIMCR(realnum), __tmkTimerCtrl[num]);
    outpw(TA_MODE1(realnum), __rtControls[realnum] | TA_FIFO_RESET);
    __tmkIrqOff[realnum] = 0;
    DrvIOSetPar(IOPAR_TMKIRQOFF, 0);
    REST_IRQ_SMP();
    outpw(TA_MODE2(realnum), __rtControls1[realnum]); // & __rtBRCMask[num]) | __rtDisableMask[num]);
    __rtSABuf[realnum] = 0;
    for (sa = 0; sa < 64; ++sa)
    {
      __rtSABufMode[realnum][sa] = SABUF_OFF;
      __rtSABufSize[realnum][sa] = 1;
//      __rtSABufExt[realnum][sa] = 0; // not required for SABUF_OFF actually
    }
    for (irt = __mrtMinRT[realnum], nrt = __mrtNRT[realnum]; nrt != 0; ++irt,--nrt)
    {
      __rtSABuf[irt] = 0;
      for (sa = 0; sa < 64; ++sa)
      {
        __rtSABufMode[irt][sa] = SABUF_OFF;
        __rtSABufSize[irt][sa] = 1;
//        __rtSABufExt[irt][sa] = 0; // not required for SABUF_OFF actually
      }
    }
    __rtHeapMin[realnum] = MRTA_MRT_HEAP_MIN;
    __rtHeapAlloc[realnum] = MRTA_MRT_HEAP_MAX + 1;
    mrtcreattab(realnum, 31);
    for (irt = 0; irt <= 31; ++irt)
    {
      outpw(MRTA_SW(realnum), irt << 11);
      __mrtA2RT[realnum][irt] = 0;
    }
    DrvIOEpi;
    return err;
#endif
  }
  __tmkTxRxMode[realnum] = TXRX_ALL_ENABLE;
  __tmkTimeOut[realnum] = 0;
  __tmkTimerCtrl[realnum] = 0;
  __bcControls[realnum] = cntr;
  __bcBus[realnum] = 0;
  __rtHeapIni[realnum] = 0;
  __tmkModeMtRt[realnum] = 0;
  DrvIOEpi;
  return 0;
}

int FARFN rtreset(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
#if NRT > 0
  int realnum;
#endif
  unsigned type;
  LOCAL_PORT
  unsigned cntr=0;
#if NRT > 0
  unsigned mask;
  unsigned mask05, mask16;
  unsigned irt;
  unsigned nrt;
#endif
  unsigned sa;
  int first=0;
  IRQ_FLAGS_F;
  IRQ_FLAGS;

  DrvIOProV;
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  if (__tmkMode[num] != RT_MODE)
  {
    __rtDisableMask[num] = __RT_DIS_MASK[type];
    __rtBRCMask[num] = __RT_BRC_MASK[type];
//    __rtEnableOnAddr[num] = 1;
    first = 1;
  }
  __tmkMode[num] = RT_MODE;
  DrvIOSetPar(IOPAR_TMKMODE, RT_MODE);
#if NRT > 0
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#else
  SET_LOCAL_PORT(num);
#endif
  switch (type)
  {
#if defined(TSTX) || defined(TSTA)
#ifdef TSTX
  case __TSTX:
#endif
#ifdef TSTA
  case __TSTA:
#endif
    __tmkMode[num] = UNDEFINED_MODE;
    DrvIOSetPar(IOPAR_TMKMODE, UNDEFINED_MODE);
    DrvIOEpi;
    return USER_ERROR(TMK_BAD_FUNC);
#endif
  case __TMK400:
    __hm400Page[num] = 0;
    GET_DIS_IRQ(); // for minimum RT off time
    outpb_d(num, TMK_Reset(num), 0);
    outpw_d(num, TMK_Ctrl(num), __rtAddress[num] | __rtDisableMask[num]);
    outpw_d(num, TMK_Addr(num), 0);
    cntr = RT_MODE;
    outpb_d(num, TMK_Mode(num), cntr);
    REST_IRQ();
    __tmkTxRxMode[num] = TXRX_ALL_ENABLE;
    __tmkTimeOut[num] = 0;
    __tmkTimerCtrl[num] = 0;
    __rtGap[num] = 0;
    break;
  case __RTMK400:
    __hm400Page[num] = 0;
    GET_DIS_IRQ(); // for minimum RT off time
    outpb_d(num, TMK_Reset(num), 0);
    __rtControls1[num] &= RTMK400_HBIT_MODE | RTMK400_BRCST_MODE | (__tmkKeepFlgM ? RTMK400_FLAG_MODE : 0);
    outpb_d(num, R400_AT(num), (__rtControls1[num] & __rtBRCMask[num]) | __rtAddress[num] | __rtDisableMask[num]);
    outpw_d(num, TMK_Addr(num), 0);
    cntr = RT_MODE;
    outpb_d(num, TMK_Mode(num), cntr);
    REST_IRQ();
    __tmkTxRxMode[num] = TXRX_ALL_ENABLE;
    __tmkTimeOut[num] = 0;
    __tmkTimerCtrl[num] = 0;
    __rtGap[num] = 0;
    break;
  case __TMKMPC:
    __hm400Page[num] = 0;
    GET_DIS_IRQ(); // for minimum RT off time
    outpb_d(num, TMKMPC_Reset(num), 0);
    outpb_d(num, TMKMPC_AddrH(num), 0);
    cntr = RT_MODE;
    outpb_d(num, TMKMPC_Mode(num), cntr);
    REST_IRQ();
    __tmkTxRxMode[num] = TXRX_ALL_ENABLE;
    __tmkTimeOut[num] = 0;
    __tmkTimerCtrl[num] = 0;
    __rtGap[num] = 0;
    break;
#ifdef MRTX
  case __MRTX:
    __tmkMode[realnum] = MRT_MODE;
    DrvIOSetParTmk(IOPAR_TMKMODE, MRT_MODE, realnum);
    __tmkTxRxMode[realnum] = TXRX_ALL_ENABLE;
    __tmkTimeOut[realnum] = 0;
    __tmkTimerCtrl[realnum] = 0;
    __rtGap[realnum] = 0;
    cntr = 0;
    if (num == realnum)
      break;
    mask = __rtDisableMask[num] << 3;
    mask05 = 0x1F << 3;
    if (__hm400Page[num] & (1 << 11))
    {
      mask <<= 5;
      mask05 <<= 5;
    }
    mask16 = 0;
    if (__hm400Page[num] & (1 << 12))
    {
      mask16 = ~mask16;
    }
/*
    ecx <<= (31-11);      //; 11->31
    int ecx >>= 31;           //; fill ecx with sign
    cl &= 5;             //; even->0 / odd->5
    eax <<= cl;           //; eax  new mask in place
    edx <<= cl;           //; edx  mask in place

    ecx = __hm400Page[num];
    ecx <<= (30-11);      //; 12->31, 11->30
    int ecx >>= 31;           //; fill ecx with sign (mask0/mask1)
*/
    __mrtMask1[realnum] &= ~(mask05 & mask16);
    __mrtMask1[realnum] |= mask & mask16;
    mask16 = ~mask16;
    __mrtMask0[realnum] &= ~(mask05 & mask16);
    __mrtMask0[realnum] |= mask & mask16;
/*
    edx &= ecx;
    edx = ~edx;
    __mrtMask1[realnum] &= dx;
    edx = eax;
    edx &= ecx;
    __mrtMask1[realnum] |= dx;
    ecx = ~ecx;
                 pop     edx
    edx &= ecx;
    edx = ~edx;
    __mrtMask0[realnum] &= dx;
    edx = eax;
    edx &= ecx;
    __mrtMask0[realnum] |= dx;
*/
    irt = __mrtMinRT[realnum];
    nrt = __mrtNRT[realnum] + irt - 1;
    mask = __rtBRCMask[irt];
    do
    {
      mask |= __rtBRCMask[++irt];
    }
    while (irt != nrt);
    __rtBRCMask[realnum] = mask;

    irt = __mrtMinRT[realnum];
    nrt = __mrtNRT[realnum];
    do
    {
      if (irt != num)
        if (__tmkMode[irt] != UNDEFINED_MODE)
          break;
      ++irt;
    }
    while (--nrt != 0);
    GET_DIS_IRQ(); // for minimum RT off time
    if (nrt == 0)
    {
      outpw(TMK_Reset(realnum), 0);
      __rtControls[realnum] = TX_RT_DATA_INT_BLK;  //; m__rtControls
    }
    outpw(TMK_Mode(realnum), __mrtCtrl0[realnum] | __mrtMask0[realnum] | __rtControls[realnum]);
                                                    //; m__rtControls
    outpw(TMK_Mode(realnum), __mrtCtrl1[realnum] | __mrtMask1[realnum] | __rtControls[realnum]);
                                                    //; m__rtControls
    __rtControls1[realnum] &= MRTX_BRCST_MODE | ((__tmkKeepFlgM || nrt != 0) ? MRTX_FLAG_MODE : 0);
    outpw(TMK_Ctrl(realnum), __rtControls1[realnum] & __rtBRCMask[realnum]);
    __rtMode[realnum] = 0;
                //; simple ram write because of get_dis_irq
    outpw(TMK_Addr(realnum), __hm400Page[num]);
    outpw(TMK_Data(realnum), 0);            //; status bits (__rtControls) = 0
    REST_IRQ();
//    cntr = 0; // above
    break;
#endif
  case __TMKX:
    __hm400Page[num] = 0;
    GET_DIS_IRQ(); // for minimum RT off time
    outpw(TMK_Reset(num), 0);
    cntr = ((RT_MODE >> 7) + TX_RT_DATA_INT_BLK + GENER1_BLK + GENER2_BLK);
    outpw(TMK_Mode(num), cntr);
    __tmkIrqOff[num] = 0;
    DrvIOSetPar(IOPAR_TMKIRQOFF, 0);
    __rtControls1[num] &= 0xF800 | TMKX_BRCST_MODE | (__tmkKeepFlgM ? TMKX_FLAG_MODE : 0);
                     //; Восстановление адреса ОУ и режимов
    outpw(TMK_Ctrl(num), (__rtControls1[num] & __rtBRCMask[num]) | __rtDisableMask[num]);
    REST_IRQ();
    __tmkTxRxMode[num] = TXRX_ALL_ENABLE;
    __tmkTimeOut[num] = 0;
    __tmkTimerCtrl[num] = 0;
    __rtGap[num] = 0;
    break;
  case __TA:
    __hm400Page[num] = 0;
    __rtSABuf[num] = 0;
    GET_DIS_IRQ_F(); // for minimum RT off time
    outpw(TA_RESET(num), 0);
    outpw(TA_TIMCR(num), 0);
    outpw(TA_RTA(num), 0);
//    if (__tmkTimerCtrl[num] & TIMER_BITS)
//      outpw(TA_TIMCR(num), __tmkTimerCtrl[num]);
    cntr = (RT_MODE << 7) | TA_RT_DATA_INT_BLK | TA_IRQ_EN | TXRX_ALL_ENABLE;
    cntr |= __rtControls[num] & (TA_CBSNL_MODE | TA_CBSD_MODE | TA_CNOBS_MODE);
                     //; Восстановление режимов
    outpw(TA_MODE1(num), cntr | TA_FIFO_RESET);
    __tmkIrqOff[num] = 0;
    DrvIOSetPar(IOPAR_TMKIRQOFF, 0);
    __rtControls1[num] &= 0xF800 | TA_HBIT_MODE | TA_BRCST_MODE | TA_RSREQ_MODE | TA_NOERR_MODE;
                     //; Восстановление адреса ОУ и режимов
    if (__rtAddress[num] >= 31)
      __rtControls1[num] |= 0xF800; // compensate commented out __rtDisableMask
    outpw(TA_MODE2(num), (__rtControls1[num])); // & __rtBRCMask[num]) | __rtDisableMask[num]);

    if (first) // first rtreset from any other mode
    {
      REST_IRQ_F();
//      __rtSABuf[num] = 0;
      for (sa = 0; sa < 64; ++sa)
      {
        __rtSABufMode[num][sa] = SABUF_OFF;
        __rtSABufSize[num][sa] = 1;
//        __rtSABufExt[num][sa] = 0; // not required for SABUF_OFF actually
      }
      __rtHeapMin[num] = TA_RT_HEAP_MIN;
      __rtHeapIni[num] = TA_RT_HEAP_MAX_RT + 1;
      __rtHeapAlloc[num] = TA_RT_HEAP_MAX_RT + 1;
#ifdef USE_TMK_KEEP_FLGM
      if (__tmkKeepFlgM && (__FLAG_MODE[num] & DRV_FLAG_MODE_HW))
      { // switching between rt[+mwt] modes
        DrvFlagModeTA(num, 0);
        __FLAG_MODE[num] |= DRV_FLAG_MODE_SW; // tmp var
      }
#endif
      rtcreattab(num, 1);
#ifdef USE_TMK_KEEP_FLGM
      if (__tmkKeepFlgM)
      {
        if (__FLAG_MODE[num] & DRV_FLAG_MODE_SW)
        {
          DrvFlagModeTA(num, DRV_FLAG_MODE); //1);
        }
      }
      else
#endif
      {
        __FLAG_MODE[num] &= ~(DRV_FLAG_MODE_SW | DRV_FLAG_MODE_HW);
        // keep DRV_FLAG_UPDATE as is
      }
      GET_DIS_IRQ_F();
    }
    else
    {
#ifdef USE_TMK_KEEP_FLGM
      if (!__tmkKeepFlgM)
#endif
      {
        if (__FLAG_MODE[num] & DRV_FLAG_MODE_SW)
        {
          REST_IRQ_F();
          DrvFlagModeTA(num, 0);
          GET_DIS_IRQ_F();
        }
      }
      //DrvRtWMode(num, __TA, 0); it doesn't support nested DIS_IRQ!!!
      // so we do it manually
      if (__rtMode[num] & 0x0800) // there was a lock
      { // clear old lock
        //!!!GET_DIS_IRQ_SMP();
        DrvPeekAndOrPokeTA(num, AdrTab, (__rtMode[num] >> 5) & 0x3F, 0x7FFF, 0);
        //!!!REST_IRQ_SMP();
        __rtMode[num] = 0;
      }

      cntr |= TA_RTMT_START;
    }
    outpw(TA_MODE1(num), cntr);
    REST_IRQ_F();
    __tmkTxRxMode[num] = TXRX_ALL_ENABLE;
    __tmkTimeOut[num] = 0;
    __tmkTimerCtrl[num] = 0;
    __rtGap[num] = 0;
    __tmkModeMtRt[num] = 0;
    break;
#ifdef MRTA
  case __MRTA:
    __tmkMode[realnum] = MRT_MODE;
    DrvIOSetParTmk(IOPAR_TMKMODE, MRT_MODE, realnum);
    cntr = __rtControls[num];
    if (num == realnum)
      break;

    if (__dmrtRT[realnum] == 0L)
    {
      __tmkTxRxMode[realnum] = TXRX_ALL_ENABLE;
      __tmkTimeOut[realnum] = 0;
      __tmkTimerCtrl[realnum] = 0;
      __dmrtaTimers[realnum] = 0L;
      __rtGap[realnum] = 0;
      cntr = TA_RT_DATA_INT_BLK | TXRX_ALL_ENABLE | TA_IRQ_EN;
      cntr |= __rtControls[realnum] & (TA_CBSNL_MODE | TA_CBSD_MODE | TA_CNOBS_MODE);
                     //; Восстановление режимов
      __rtControls[realnum] = cntr;

      GET_DIS_IRQ_SMP();
      outpw(TA_RESET(realnum), 0);
      __mrtLastBrcTxRT[realnum] = 0;
      outpw(TA_TIMCR(realnum), 0);
//    if (__tmkTimerCtrl[num] & TIMER_BITS)
//      outpw(TA_TIMCR(realnum), __tmkTimerCtrl[num]);
      outpw(TA_MODE1(realnum), cntr | TA_FIFO_RESET);
      __tmkIrqOff[realnum] = 0;
      DrvIOSetPar(IOPAR_TMKIRQOFF, 0);
      REST_IRQ_SMP();
      __rtSABuf[realnum] = 0;
      for (sa = 0; sa < 64; ++sa)
      {
        __rtSABufMode[realnum][sa] = SABUF_OFF;
        __rtSABufSize[realnum][sa] = 1;
//        __rtSABufExt[realnum][sa] = 0; // not required for SABUF_OFF actually
      }
      for (irt = __mrtMinRT[realnum], nrt = __mrtNRT[realnum]; nrt != 0; ++irt,--nrt)
      {
        __rtSABuf[irt] = 0;
        for (sa = 0; sa < 64; ++sa)
        {
          __rtSABufMode[irt][sa] = SABUF_OFF;
          __rtSABufSize[irt][sa] = 1;
//          __rtSABufExt[irt][sa] = 0; // not required for SABUF_OFF actually
        }
#ifdef USE_TMK_KEEP_FLGM
        if (!__tmkKeepFlgM)
#endif
        {
          __FLAG_MODE[irt] &= ~(DRV_FLAG_MODE_SW | DRV_FLAG_MODE_HW);
          // keep DRV_FLAG_UPDATE as is
        }
      }
      __rtHeapMin[realnum] = MRTA_MRT_HEAP_MIN;
      __rtHeapAlloc[realnum] = MRTA_MRT_HEAP_MAX + 1;
      mrtcreattab(realnum, 31);
    }
    __dmrtaTimers[realnum] &= ~(1L << num);
    // don't write TA_MODE1 here, it disturbs operating 32RT somehow

    if (__rtAddress[num] == 0xFF)
    {
      // common unused page for all RTs with undefined address
      __hm400Page0[num] = 0x00 | 0x1F; //0x20 | rtaddr;
      __hm400Page2[num] = __hm400Page0[num] >> 4;
      __hm400Page[num] = __hm400Page0[num] << 11;
//      __rtSubAddr2[num] = 0;
      __rtSABuf[num] = 0;
    }

    __rtControls1[realnum] &= TA_HBIT_MODE | TA_RSREQ_MODE | TA_NOERR_MODE; // | TA_BRCST_MODE;
                     //; Восстановление режимов
    outpw(TA_MODE2(realnum), (__rtControls1[realnum])); // & __rtBRCMask[num]) | __rtDisableMask[num]);
//    outpw(MRTA_SW(realnum), 0xF800 | __rtControls1[realnum] | DrvMrtaBrcRtOn(realnum));
    outpw(MRTA_SW(realnum), 0xF800 | DrvMrtaBrcRtOn(realnum));
    __mrtRxCtrl[num] = 0;
    __rtControls1[num] &= 0xF800 | TA_BRCST_MODE | MRTA_RT_ON; // | TA_HBIT_MODE;
                     //; Восстановление адреса ОУ и режимов
    if (__rtControls1[num] & MRTA_RT_ON) //!__rtDisableMask[num]
    {
      __rtControls1[num] &= ~MRTA_RT_ON;
      outpw(MRTA_SW(realnum), __rtControls1[num]);
#ifdef USE_TMK_KEEP_FLGM
      if (!__tmkKeepFlgM)
#endif
        DrvFlagModeTA(num, 0);
      DrvRtWMode(num, __MRTA, 0);
      __rtControls1[num] |= MRTA_RT_ON;
      outpw(MRTA_SW(realnum), __rtControls1[num]);
    }
    break;
#endif
  }
  __rtControls[num] = cntr;
  __rtMode[num] = 0;
  __rtPagePC[num] = 0;
  __rtPageBus[num] = 0;
  DrvIOEpi;
  return 0;
}

int DrvMtMwReset(int __tmkNumber, unsigned mode)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned cntr=0;
  unsigned modesav;
  IRQ_FLAGS;

  DrvIOPro(6, 0, 1, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  modesav = __tmkMode[realnum];
  __tmkMode[realnum] = mode;
  DrvIOSetPar(IOPAR_TMKMODE, mode);
  __tmkStarted[realnum] = 0;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    __tmkMode[realnum] = modesav;
    DrvIOSetPar(IOPAR_TMKMODE, modesav);
    DrvIOStop;
    return USER_ERROR(TMK_BAD_FUNC);
#if defined(TSTX) || defined(TSTA)
#ifdef TSTX
  case __TSTX:
#endif
#ifdef TSTA
  case __TSTA:
#endif
    __tmkMode[realnum] = UNDEFINED_MODE;
    DrvIOSetPar(IOPAR_TMKMODE, UNDEFINED_MODE);
    DrvIOEpi;
    return USER_ERROR(TMK_BAD_FUNC);
#endif
  case __TMKX:
    if (mode != MT_MODE)
    {
      __tmkMode[realnum] = modesav;
      DrvIOSetPar(IOPAR_TMKMODE, modesav);
      DrvIOStop;
      return USER_ERROR(TMK_BAD_FUNC);
    }
    GET_DIS_IRQ(); // because of GENER*_BLK
    outpw(TMK_Reset(realnum), 0);
    cntr = ((BC_MODE >> 7) | (GENER1_BLK + GENER2_BLK));
           //MT_MODE SHR 7
    outpw(TMK_Mode(realnum), cntr);
    __tmkIrqOff[realnum] = 0;
    DrvIOSetPar(IOPAR_TMKIRQOFF, 0);
    REST_IRQ();
    break;
  case __TA:
    switch (mode)
    {
    case MT_MODE:
      cntr = TA_MT_MODE;
      break;
    case MW_MODE:
      cntr = TA_MW_MODE;
      break;
    case MWT_MODE:
      cntr = TA_MWMT_MODE;
      break;
    }
    GET_DIS_IRQ_SMP();
    outpw(TA_RESET(realnum), 0);
    outpw(TA_TIMCR(realnum), 0);
    outpw(TA_RTA(realnum), 0);
//    if (__tmkTimerCtrl[realnum] & TIMER_BITS)
//      outpw(TA_TIMCR(realnum), __tmkTimerCtrl[realnum]);
    cntr |= (TXRX_ALL_ENABLE | TA_IRQ_EN);
    outpw(TA_MODE1(realnum), cntr | TA_FIFO_RESET);
    outpw(TA_MODE1(realnum), cntr);
    __tmkIrqOff[realnum] = 0;
    DrvIOSetPar(IOPAR_TMKIRQOFF, 0);
//    REST_IRQ_SMP();
    __rtControls1[realnum] = 0xF800; // disable RT
    outpw(TA_MODE2(realnum), __rtControls1[realnum]);
//    GET_DIS_IRQ_SMP();
    // for MT
    DrvRtPokeTA(realnum, AdrTab, 0, 0x0000); // for mtgetstate
    DrvRtPokeTA(realnum, MTA_STOP_BASE, 63, MTA_STOP_BASE);
    REST_IRQ_SMP();
    __mtStopBaseTA[realnum] = 0;
    __bcmtStateX[realnum] = 0;
    DrvIOSetPar(IOPAR_BCMTSTATEX, 0);
    outpw(TA_MSGA(realnum), AdrTab);
    break;
  }
  __tmkTxRxMode[realnum] = TXRX_ALL_ENABLE;
  __tmkTimeOut[realnum] = 0;
  __tmkTimerCtrl[realnum] = 0;
  __bcControls[realnum] = cntr;
  __rtHeapIni[realnum] = 0;
  __tmkModeMtRt[realnum] = 0;
  DrvIOEpi;
  return 0;
}

int FARFN mtreset(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  return DrvMtMwReset(__tmkNumber, MT_MODE);
}

int FARFN mwreset(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  return DrvMtMwReset(__tmkNumber, MW_MODE);
}

int FARFN mwtreset(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  return DrvMtMwReset(__tmkNumber, MWT_MODE);
}

int DrvMtMwRtReset(int __tmkNumber, unsigned mode)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned cntr=0;
  unsigned sa;
  unsigned modesav;
  int first=0;
  IRQ_FLAGS;

  DrvIOProV;
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  __tmkStarted[realnum] = 0;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  if (__tmkMode[realnum] != mode)
  {
    __rtDisableMask[realnum] = __RT_DIS_MASK[type];
    __rtBRCMask[realnum] = __RT_BRC_MASK[type];
//    __rtEnableOnAddr[realnum] = 0;
    first = 1;
  }
  modesav = __tmkMode[realnum];
  __tmkMode[realnum] = mode;
  DrvIOSetPar(IOPAR_TMKMODE, mode);
  switch (type)
  {
  default:
    __tmkMode[realnum] = modesav;
    DrvIOSetPar(IOPAR_TMKMODE, modesav);
    DrvIOStop;
    return USER_ERROR(TMK_BAD_FUNC);
#if defined(TSTX) || defined(TSTA)
#ifdef TSTX
  case __TSTX:
#endif
#ifdef TSTA
  case __TSTA:
#endif
    __tmkMode[realnum] = UNDEFINED_MODE;
    DrvIOSetPar(IOPAR_TMKMODE, UNDEFINED_MODE);
    DrvIOEpi;
    return USER_ERROR(TMK_BAD_FUNC);
#endif
  case __TA:
    if (mode == MTRT_MODE || mode == MWTRT_MODE)
    {
      if ((__tmkHWVer[realnum] < 20) && !(__tmkOptions[realnum] & (UOPT)MTRT_ALLOW_ANOMALY))
      {
        __tmkMode[realnum] = modesav;
        DrvIOSetPar(IOPAR_TMKMODE, modesav);
        DrvIOStop;
        return USER_ERROR(TMK_BAD_FUNC);
      }
    }
    switch (mode)
    {
    case MTRT_MODE:
      cntr = TA_MT_MODE;
      break;
    case MWRT_MODE:
      cntr = TA_MW_MODE;
      break;
    case MWTRT_MODE:
      cntr = TA_MWMT_MODE;
      break;
    }
    __hm400Page[realnum] = 0;
    __rtSABuf[realnum] = 0;
    GET_DIS_IRQ_SMP();
    outpw(TA_RESET(realnum), 0);
    outpw(TA_TIMCR(realnum), 0);
    outpw(TA_RTA(realnum), 0);
//    if (__tmkTimerCtrl[realnum] & TIMER_BITS)
//      outpw(TA_TIMCR(realnum), __tmkTimerCtrl[realnum]);
    cntr |= (TXRX_ALL_ENABLE + TA_IRQ_EN + TA_RT_DATA_INT_BLK);
    cntr |= __rtControls[realnum] & (TA_CBSNL_MODE | TA_CBSD_MODE | TA_CNOBS_MODE);
                     //; Восстановление режимов
    outpw(TA_MODE1(realnum), cntr | TA_FIFO_RESET);
    outpw(TA_MODE1(realnum), cntr);
    __tmkIrqOff[realnum] = 0;
    DrvIOSetPar(IOPAR_TMKIRQOFF, 0);
    REST_IRQ_SMP();
    __rtControls1[realnum] &= 0xF800 | TA_HBIT_MODE | TA_BRCST_MODE | TA_RSREQ_MODE | TA_NOERR_MODE;
    __bcControls1[realnum] = __rtControls1[realnum];
                     //; Восстановление адреса ОУ и режимов
    if (__rtAddress[realnum] >= 31)
      __rtControls1[realnum] |= 0xF800; // compensate commented out __rtDisableMask
    outpw(TA_MODE2(realnum), (__rtControls1[realnum])); // & __rtBRCMask[realnum]) | __rtDisableMask[realnum]);

    if (first) // first rtreset from any other mode
    {
//      __rtSABuf[realnum] = 0;
      for (sa = 0; sa < 64; ++sa)
      {
        __rtSABufMode[realnum][sa] = SABUF_OFF;
        __rtSABufSize[realnum][sa] = 1;
//        __rtSABufExt[realnum][sa] = 0; // not required for SABUF_OFF actually
      }
      __rtHeapMin[realnum] = TA_RT_HEAP_MIN;
      __rtHeapIni[realnum] = TA_RT_HEAP_MAX_MTRT + 1;
      __rtHeapAlloc[realnum] = TA_RT_HEAP_MAX_MTRT + 1;
#ifdef USE_TMK_KEEP_FLGM
      if (__tmkKeepFlgM && (__FLAG_MODE[realnum] & DRV_FLAG_MODE_HW))
      { // switching between rt[+mwt] modes
        DrvFlagModeTA(realnum, 0);
        __FLAG_MODE[realnum] = DRV_FLAG_MODE_SW; // tmp var
      }
#endif
      rtcreattab(realnum, 1);
#ifdef USE_TMK_KEEP_FLGM
      if (__tmkKeepFlgM)
      {
        if (__FLAG_MODE[realnum] & DRV_FLAG_MODE_SW)
        {
          DrvFlagModeTA(realnum, DRV_FLAG_MODE); //1);
        }
      }
      else
#endif
      {
        __FLAG_MODE[realnum] &= ~(DRV_FLAG_MODE_SW | DRV_FLAG_MODE_HW);
        // keep DRV_FLAG_UPDATE as is
      }
    }
    else
    {
#ifdef USE_TMK_KEEP_FLGM
      if (!__tmkKeepFlgM)
#endif
        DrvFlagModeTA(realnum, 0);
      DrvRtWMode(realnum, __TA, 0);
//      cntr |= TA_RTMT_START; !!! can be set in mtstartx only
    }
    //for MT
    GET_DIS_IRQ_SMP();
    DrvRtPokeTA(realnum, AdrTab, 0, 0x0000); // for mtgetstate
    DrvRtPokeTA(realnum, 0, 63, 0x0000); // unused base 0
    DrvRtPokeTA(realnum, MTA_STOP_BASE, 63, MTA_STOP_BASE);
    REST_IRQ_SMP();
    __mtStopBaseTA[realnum] = 0;
    __bcmtStateX[realnum] = 0;
    DrvIOSetPar(IOPAR_BCMTSTATEX, 0);

//    GET_DIS_IRQ_SMP();
//    outpw(TA_MODE1(realnum), cntr);
//    REST_IRQ_SMP();
    break;
  }
  __tmkTxRxMode[realnum] = TXRX_ALL_ENABLE;
  __tmkTimeOut[realnum] = 0;
  __tmkTimerCtrl[realnum] = 0;
  __bcControls[realnum] = cntr;
  __rtControls[realnum] = cntr;
  __rtMode[realnum] = 0;
  __rtPagePC[realnum] = 0;
  __rtPageBus[realnum] = 0;
  __tmkModeMtRt[realnum] = 1;
  DrvIOEpi;
  return 0;
}

int FARFN mtrtreset(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  return DrvMtMwRtReset(__tmkNumber, MTRT_MODE);
}

int FARFN mwrtreset(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  return DrvMtMwRtReset(__tmkNumber, MWRT_MODE);
}

int FARFN mwtrtreset(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  return DrvMtMwRtReset(__tmkNumber, MWTRT_MODE);
}

U16 FARFN mwcdw(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 mwCdData, U16 mwCdCtrl)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned data = 0;

  DrvIOPro(2, 0, 0, 0, 1, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    USER_ERROR(TMK_BAD_FUNC);
    break;
  case __TA:
    if (mwCdCtrl == CODEC_READ)
      data = inpw(TA_CODEC(realnum));
    else
    {
      outpw(TA_MODE2(realnum), (__rtControls1[realnum] & 0xFF9F) | (mwCdCtrl & 0x0060));
      outpw(TA_CODEC(realnum), mwCdData);
    }
    break;
  }
  DrvIOEpi;
  return (U16)data;
}

/*
U16 FARFN mwdcw(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        )
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned data;

  DrvIOPro(0, 0, 0, 0, 1, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    USER_ERROR(TMK_BAD_FUNC);
    data = 0;
    break;
  case __TA:
    data = inpw(TA_CODEC(realnum));
    break;
  }
  DrvIOEpi;
  return (U16)data;
}
*/

int FARFN mtdefmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 mtModeBits)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned bits, bitst;

  DrvIOPro(1, 0, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  bitst = mtModeBits;
  bits = 0;
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(BC_BAD_FUNC);
  case __TA:
    if (bitst & DRV_HBIT_MODE)
      bits |= TA_HBIT_MODE;
    if (bitst & DRV_BRCST_MODE)
      bits |= TA_BRCST_MODE;
//    GET_MUTEX;
    bits |= __rtControls1[realnum] & 0xFDEF;
    __rtControls1[realnum] = bits;
//    REST_MUTEX;
//    bits &= __rtBRCMask[num];
//    bits |= __rtDisableMask[num];
    outpw(TA_MODE2(realnum), bits);
    break;
  }
  DrvIOEpi;
  return 0;
}

U16 FARFN mtgetmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  unsigned bits=0, bitst;

//  DrvIOPro(0, 0, 0, 0, 1, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    USER_ERROR(BC_BAD_FUNC);
//    bits = 0;
    break;
  case __TA:
    bitst = __rtControls1[realnum]; //inpw(TA_MODE2(num));
//    bits = 0;
    if (bitst & TA_HBIT_MODE)
      bits |= DRV_HBIT_MODE;
    if (bitst & TA_BRCST_MODE)
      bits |= DRV_BRCST_MODE;
    if (__tmkHWVer[realnum] >= 21)
      bits |= MW_DT00_MODE;
    break;
  }
//  DrvIOEpi;
  return bits;
}

int FARFN bcdefmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 bcModeBits)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned bits, bitsca;

  DrvIOPro(1, 0, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  bits = bcModeBits & 0x7F7F;
  bitsca = 0;
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(BC_BAD_FUNC);
  case __TA:
    if (bits & 0x1000)
      bitsca |= 0x0200;
    if (bits & 0x0600)
      bitsca |= 0x4000;
    if (bits & 0x0010)
      bitsca |= 0x051F;
    if (bits & 0x0008)
      bitsca |= 0x1000;
    if (bits & 0x0004)
      bitsca |= 0x0800;
//    GET_MUTEX; // use cli/sti in main DOS program if needed
    __bcControls1[realnum] = bits;
    __bcCtrlWA[realnum] = bitsca;
//    REST_MUTEX;
    outpw(TA_MODE2(realnum), __bcControls1[realnum]);
    break;
  }
  DrvIOEpi;
  return 0;
}

U16 FARFN bcgetmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  unsigned bits=0, bitsca;

//  DrvIOPro(0, 0, 0, 0, 1, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    USER_ERROR(BC_BAD_FUNC);
    break;
  case __TA:
//    GET_MUTEX; // use cli/sti in main DOS program if needed
    bits = __bcControls1[realnum]; //inpw(TA_MODE2(num));
    bitsca = __bcCtrlWA[realnum];
//    REST_MUTEX;
    if (bits == (BC_ERSTP_MODE | BC_BTSTP_MODE | BC_DNRVM_MODE) &&
        bitsca == 0x1D1F) // initial values after bcreset
      bits |= 0x000C;
    break;
  }
//  DrvIOEpi;
  return bits;
}

int FARFN bcdefirqmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 bcIrqModeBits)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned bits;

  DrvIOPro(1, 0, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(BC_BAD_FUNC);
  case __TMKX:
    bits = bcIrqModeBits & 0xC004;
    GET_DIS_IRQ(); // because IRQ may be enabled via reg before tmkIrqOff clear
    bits |= __bcControls[realnum] & 0x0003;
    __bcControls[realnum] = bits;
    outpw(TMK_Mode(realnum), bits);
    __tmkIrqOff[realnum] = (TMKIRQOFF_T)(bits >> 15) & 1;
    DrvIOSetPar(IOPAR_TMKIRQOFF, __tmkIrqOff[realnum]);
    REST_IRQ();
    break;
  case __TA:
    bits = (((bcIrqModeBits & TMK_IRQ_OFF) ^ TMK_IRQ_OFF) >> 5);
    GET_DIS_IRQ(); // because IRQ may be enabled via reg before tmkIrqOff clear
    bits |= __bcControls[realnum] & ~TA_IRQ_EN;
    __bcControls[realnum] = bits;
    if (__tmkModeMtRt[realnum])
      __rtControls[realnum] = bits;
    outpw(TA_MODE1(realnum), bits);
    __tmkIrqOff[realnum] = ((TMKIRQOFF_T)(bits >> 10) ^ 1) & 1;
    DrvIOSetPar(IOPAR_TMKIRQOFF, __tmkIrqOff[realnum]);
    REST_IRQ();
    break;
  }
  DrvIOEpi;
  return 0;
}

U16 FARFN bcgetirqmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned bits=0;

  DrvIOPro(0, 0, 0, 0, 1, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    USER_ERROR(BC_BAD_FUNC);
//    bits = 0;
    break;
  case __TMKX:
    bits = __bcControls[realnum] & 0xC004;
    break;
  case __TA:
    bits = GENER1_BLK | GENER2_BLK;
    if (!(inpw(TA_MODE1(realnum)) & TA_IRQ_EN)) // if __tmkIrqOff
      bits |= TMK_IRQ_OFF;
    break;
  }
  DrvIOEpi;
  return bits;
}

int FARFN bcstart(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Base, U16 bcCtrlCode)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned code;
  unsigned base;
  unsigned basepc;
  unsigned len;
  IRQ_FLAGS;

  DrvIOPro(9, 0, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE(realnum, BC_MODE);
//;                mov     edx, ebx
//;                //;VMMcall Get_Sys_VM_Handle
//;                xchg    edx, ebx
//;                mov     hSysVM, edx
//;                mov     hCurVM[ebx*2], edx
//;//;
//;                mov     eax, 5                         //; number of milliseconds
//;                mov     edx, ebx                       //; reference data
//;                mov     esi, OFFSET32 _VTMK_BC_Event_Callback //; callback procedure
//;                //;VMMcall Set_Global_Time_Out
//;
//;//;                mov     [TimeOut], esi                  //; time-out handle
//;                jmp     bs_Ok
//;//;
  code = bcCtrlCode;
  CHECK_BC_CTRL(code);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  base = Base;
  CHECK_BC_BASE_BX(realnum, base);
//  base += __rtHeapIni[realnum]; //bcstart not used in mt
  if (__tmkStarted[realnum])
  {
    switch (type)
    {
    case __TMK400:
    case __RTMK400:
      GET_DIS_IRQ_SMP();
      outpb_d(realnum, TMK_Reset(realnum), 0);
      REST_IRQ_SMP();
      outpb_d(realnum, TMK_Mode(realnum), __bcControls[realnum]);
      break;
    case __TMKMPC:
      GET_DIS_IRQ_SMP();
      outpb_d(realnum, TMKMPC_Reset(realnum), 0);
      REST_IRQ_SMP();
      outpb_d(realnum, TMKMPC_Mode(realnum), __bcControls[realnum]);
      break;
    case __TMKX:
      GET_DIS_IRQ_SMP();
      outpw(TMK_Reset(realnum), 0);
      outpw(TMK_Mode(realnum), __bcControls[realnum]);
      REST_IRQ_SMP();
      break;
    case __TA:
      GET_DIS_IRQ_SMP();
      outpw(TA_RESET(realnum), 0);
      outpw(TA_TIMCR(realnum), __tmkTimerCtrl[realnum]);
      outpw(TA_MODE1(realnum), __bcControls[realnum]);
      REST_IRQ_SMP();
      outpw(TA_MODE2(realnum), __bcControls1[realnum]);
      break;
    }
  }
  __tmkStarted[realnum] = 1;
  __bcBaseBus[realnum] = base;
  DrvIOSetPar(IOPAR_BCBASEBUS, base);
  basepc = __bcBasePC[realnum];
  if (base == basepc)
    basepc = -1;
  __bcXAStart[realnum] = 0;
  DrvIOSetPar(IOPAR_BCXASTART, 0);
  __bcmtStateX[realnum] = 0;
  DrvIOSetPar(IOPAR_BCMTSTATEX, 0);
  __bcAW1Pos[realnum] = 0;
  __bcAW2Pos[realnum] = 0;
  switch (code)
  {
  case DATA_RT_RT_BRCST:
    __bcAW1Pos[realnum] = 2;
    break;
  case DATA_BC_RT:
    len = __bcCmdWN[realnum][base] & 0x1F;
////                mov     ecx, bcCmdWPtr[ebx*2]
////                movzx   eax, word ptr [ecx+eax*2]
////                and     eax, 1Fh
    if (len == 0)
      len = 0x20;
    __bcAW2Pos[realnum] = (U08)len + 1;
    break;
  case DATA_RT_BC:
    __bcAW1Pos[realnum] = 1;
    break;
  case DATA_RT_RT:
    len = __bcCmdWN[realnum][base] & 0x1F;
////                mov     ecx, bcCmdWPtr[ebx*2]
////                movzx   eax, word ptr [ecx+eax*2]
////                and     eax, 1Fh
    if (len == 0)
      len = 0x20;
    __bcAW1Pos[realnum] = 2;
    __bcAW2Pos[realnum] = (U08)len + 3;
    break;
  case CTRL_C_A:
    __bcAW2Pos[realnum] = 1;
    break;
  case CTRL_CD_A:
    __bcAW2Pos[realnum] = 2;
    break;
  case CTRL_C_AD:
    __bcAW1Pos[realnum] = 1;
    break;
  }
  DrvIOSetPar(IOPAR_BCAW1POS, __bcAW1Pos[realnum]);
  DrvIOSetPar(IOPAR_BCAW2POS, __bcAW2Pos[realnum]);

  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(BC_BAD_FUNC);
  case __TMK400:
  case __RTMK400:
    outpb_d(realnum, TMK_Base(realnum), base);
    GET_MUTEX;
    code |= __bcControls[realnum] & 0x00F0;
    __bcControls[realnum] = code;
    outpb_d(realnum, TMK_Mode(realnum), code);
    REST_MUTEX;
    outpb_d(realnum, TMK_Start(realnum), 0);
    break;
  case __TMKMPC:
    outpb_d(realnum, TMKMPC_Base(realnum), base);
    GET_MUTEX;
    code |= __bcControls[realnum] & 0x00F0;
    __bcControls[realnum] = code;
    outpb_d(realnum, TMKMPC_Mode(realnum), code);
    REST_MUTEX;
    outpb_d(realnum, TMKMPC_Start(realnum), 0);
    break;
  case __TMKX:
    base <<= 1;
    base |= __bcBus[realnum];
    base <<= 5;
    outpw(TMK_Ctrl(realnum), base | code);
    break;
  case __TA:
    {
      unsigned ContrW;
      ContrW = __bcCtrlWA[realnum];

//      if ((code == DATA_RT_RT) || (code == DATA_RT_RT_BRCST))
      if ((code&0x7) == DATA_RT_RT)
        ContrW |= 0x0040;
      ContrW |= __bcBus[realnum] << 7;

      GET_DIS_IRQ_SMP();
      outpw(TA_ADDR(realnum), (base<<6) | 61);
      outpw(TA_DATA(realnum), ContrW);
      outpw(TA_DATA(realnum), 0);
      REST_IRQ_SMP();
      outpw(TA_MSGA(realnum), base & 0x03FF);

      outpw(TA_MODE2(realnum), __bcControls1[realnum] | TA_BC_START);
    }
    break;
  }
//;                mov     esi, OFFSET32 BCIntCallback
//;                mov     esi, OFFSET32 _VTMK_BC_Event_Callback
//;                mov     edx, ebx
//;                push    ebx
//;                //;VMMcall Schedule_Global_Event
//;                pop     ebx
//;                mov     hIrqEvent[ebx*2], esi
  DrvIOEpi;
  return 0;
}

int FARFN bcstop(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  LOCAL_PORT

  DrvIOPro(1, 0, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE_BCMT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(BC_BAD_FUNC);
  case __TMKX:
    outpw(TMKX_Stop(realnum), 0);
    break;
  case __TA:
    if (__tmkMode[realnum] == BC_MODE) //bcstop
    {
      outpw(TA_MODE2(realnum), __bcControls1[realnum] | TA_BC_STOP);
    }
    else //mtstop
    {
      GET_MUTEX;
      GET_DIS_IRQ_SMP();
      outpw(TA_MODE1(realnum), __bcControls[realnum] &= ~TA_RTMT_START);
//      inpandoroutpw(TA_MODE1(realnum), 0xFFF7, 0);
      if (__tmkModeMtRt[realnum])
        __rtControls[realnum] = __bcControls[realnum];
      REST_IRQ_SMP();
      REST_MUTEX;
    }
    break;
  }
  DrvIOEpi;
  return 0;
}

int FARFN bcstartx(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Base, U16 bcCtrlCode)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned code;
  unsigned base;
  IRQ_FLAGS;

  DrvIOPro(13, 0, 1, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE_BCMT(realnum);
  code = bcCtrlCode;
  CHECK_BC_CTRLX(code);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  base = Base;
  CHECK_BCMT_BASE_BX(realnum, base);
  base += __rtHeapIni[realnum];
  __bcBaseBus[realnum] = base;
  DrvIOSetPar(IOPAR_BCBASEBUS, base);
  __bcXAStart[realnum] = 1;
  DrvIOSetPar(IOPAR_BCXASTART, 1);
  __bcmtStateX[realnum] = 0;
  DrvIOSetPar(IOPAR_BCMTSTATEX, 0);
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(BC_BAD_FUNC);
  case __TMKX:
    if (__tmkStarted[realnum])
    {
      GET_DIS_IRQ_SMP();
      outpw(TMK_Reset(realnum), 0);
      outpw(TMK_Mode(realnum), __bcControls[realnum]);
      REST_IRQ_SMP();
    }
    __tmkStarted[realnum] = 1;
    base <<= 6;
    if (__tmkMode[realnum] != MT_MODE)
    {
      outpw(TMK_Ctrl(realnum), base | code);
    }
    else
    {
      if ((__bcControls[realnum] & (MT_MODE >> 7)) != 0)
        outpw(TMK_Ctrl(realnum), base | code);
      else
      {
        GET_DIS_IRQ();
        __bcControls[realnum] |= MT_MODE >> 7;
        outpw(TMK_Mode(realnum), __bcControls[realnum]);
        outpw(TMK_Ctrl(realnum), base | code);
        REST_IRQ();
      }
    }
    break;
  case __TA:
    if (__tmkMode[realnum] == BC_MODE) //bcstartx
    {
      unsigned ContrW = __bcCtrlWA[realnum];
      unsigned code1;

      if (__tmkStarted[realnum])
      {
        GET_DIS_IRQ_SMP();
        outpw(TA_RESET(realnum), 0);
        outpw(TA_TIMCR(realnum), __tmkTimerCtrl[realnum]);
        outpw(TA_MODE1(realnum), __bcControls[realnum]);
        REST_IRQ_SMP();
        outpw(TA_MODE2(realnum), __bcControls1[realnum]);
      }
      __tmkStarted[realnum] = 1;
//      if (((code&0xf) == DATA_RT_RT) || ((code&0xf) == DATA_RT_RT_BRCST))
      if ((code&0x7) == DATA_RT_RT)
        ContrW |= 0x0040;
      if (code & CX_BUS_B)
        ContrW |= 0x0080;
      if (code & CX_CONT)
        ContrW |= 0x2000;
#if DRV_MAX_BASE < 256
      code1 = __bcLinkWN[realnum][base];
#else
      code1 = __bcLinkCCN[realnum][base];
#endif
      if (code1 & CX_SIG)
        ContrW |= 0x8000;

      GET_DIS_IRQ_SMP();
      outpw(TA_ADDR(realnum), (base<<6) | 61);
      outpw(TA_DATA(realnum), ContrW);
      outpw(TA_DATA(realnum), 0);
      REST_IRQ_SMP();
#ifdef DRV_EMULATE_FIRST_CX_SIG
//emulation is through special base 0x3ff
//also it could be a good (or bad) idea to block irq output
//and further poll it until it occurs
      if (code & CX_SIG)
      {
        GET_DIS_IRQ_SMP();
        outpw(TA_ADDR(realnum), (0x03FF<<6) | 61);
        outpw(TA_DATA(realnum), 0xA020);
        outpw(TA_DATA(realnum), 0);
        outpw(TA_DATA(realnum), base);
        REST_IRQ_SMP();
        base = 0x03FF;
      }
#endif //def DRV_EMULATE_FIRST_CX_SIG
      outpw(TA_MSGA(realnum), base & 0x03FF);

      outpw(TA_MODE2(realnum), __bcControls1[realnum] | TA_BC_START);
    }
    else //mtstartx
    {
//      ??? what for CX_SIG? ! make nop msg with irq! but for mt?
      if (__tmkStarted[realnum])
      {
        GET_DIS_IRQ_SMP();
        outpw(TA_RESET(realnum), 0);
        outpw(TA_TIMCR(realnum), __tmkTimerCtrl[realnum]);
        GET_MUTEX;
        outpw(TA_MODE1(realnum), __bcControls[realnum] &= ~TA_RTMT_START);
        if (__tmkModeMtRt[realnum])
          __rtControls[realnum] = __bcControls[realnum];
        REST_MUTEX;
        REST_IRQ_SMP();
        outpw(TA_MODE2(realnum), __rtControls1[realnum]);
      }
      __tmkStarted[realnum] = 1;
      __mtCW[realnum] = code;
      DrvIOSetPar(IOPAR_MTCW, code);
      GET_DIS_IRQ_SMP();
      DrvRtPokeTA(realnum, AdrTab, 0, base|0x4000);
      // protect from writing to a random base while monitor is going
      // to stop (in DIRQLTmksInt1) after next base message completed
      if ((code & CX_CONT) == 0) // CX_STOP
      {
        DrvRtPokeTA(realnum, base, 63, MTA_STOP_BASE);
        __mtStopBaseTA[realnum] = base;
      }
      REST_IRQ_SMP();

//      GET_DIS_IRQ_SMP();
//      DrvRtPokeTA(realnum, base, ADR_SIG_WORD, code);
//      REST_IRQ_SMP();
      outpw(TA_MSGA(realnum), AdrTab);
      if (!__tmkModeMtRt[realnum] || !__rtDisableMask[realnum])
      {
        GET_MUTEX;
        GET_DIS_IRQ_SMP();
        outpw(TA_MODE1(realnum), __bcControls[realnum] |= TA_RTMT_START);
        if (__tmkModeMtRt[realnum])
          __rtControls[realnum] = __bcControls[realnum];
        REST_IRQ_SMP();
        REST_MUTEX;
      }
    }
    break;
  }
  DrvIOEpi;
  return 0;
}

int FARFN bcstartctla(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 bcBase, U16 bcCtrlA, U16 bcTimeA, U16 bcLinkA)
{
  int register realnum;
  int res;
  unsigned base;
  unsigned oldbase;

  DrvIOProV;
  CLRtmkError;
  realnum = GET_RealNum;
  CHECK_TMK_MODE(realnum, BC_MODE);
  base = bcBase;
  CHECK_BCMT_BASE_BX(realnum, base);
//  base += __rtHeapIni[realnum]; //bcstartctla not used in mt
  oldbase = __bcBasePC[realnum];
  DrvBcDefBaseTA(realnum, base);
  res = bcdefctla(TMKNUM__(__tmkNumber) CTLA_ALL, bcCtrlA, bcTimeA, bcLinkA);
  DrvBcDefBaseTA(realnum, oldbase);
//  base -= __rtHeapIni[realnum]; //bcstartctla not used in mt
  if (!res)
    res = bcstarta(TMKNUM__(__tmkNumber) (U16)base);
  DrvIOEpi;
  return res;
}

int FARFN bcstarta(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 bcBase)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned base;
  IRQ_FLAGS;

  DrvIOPro(6, 0, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE(realnum, BC_MODE);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  base = bcBase;
  CHECK_BCMT_BASE_BX(realnum, base);
//  base += __rtHeapIni[realnum]; //bcstarta not used in mt
  __bcBaseBus[realnum] = base;
  DrvIOSetPar(IOPAR_BCBASEBUS, base);
  __bcXAStart[realnum] = (__tmkOptions[realnum] & (UOPT)BC_STARTA_INTA) ? 2 : 1;
  DrvIOSetPar(IOPAR_BCXASTART, __bcXAStart[realnum]);
  __bcmtStateX[realnum] = 0;
  DrvIOSetPar(IOPAR_BCMTSTATEX, 0);
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(BC_BAD_FUNC);
  case __TA:
    if (__tmkStarted[realnum])
    {
      GET_DIS_IRQ_SMP();
      outpw(TA_RESET(realnum), 0);
      outpw(TA_TIMCR(realnum), __tmkTimerCtrl[realnum]);
      outpw(TA_MODE1(realnum), __bcControls[realnum]);
      REST_IRQ_SMP();
      outpw(TA_MODE2(realnum), __bcControls1[realnum]);
    }
    __tmkStarted[realnum] = 1;

    outpw(TA_MSGA(realnum), base & 0x03FF);

    outpw(TA_MODE2(realnum), __bcControls1[realnum] | TA_BC_START);
    break;
  }
  DrvIOEpi;
  return 0;
}

int FARFN bcdefctla(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 bcCTLA, U16 bcCtrlA, U16 bcTimeA, U16 bcLinkA)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned pos;

  DrvIOPro(6, 0, 0, 0, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE(realnum, BC_MODE);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(BC_BAD_FUNC);
  case __TA:
    pos = 0;
    GET_DIS_IRQ_SMP();

    if (bcCTLA & CTLA_CTRLA)
    {
      outpw(TA_ADDR(realnum), (__bcBasePC[realnum]<<6) | 61);
      outpw(TA_DATA(realnum), bcCtrlA);
      pos = 61 + 1;
    }
    if (bcCTLA & CTLA_TIMEA)
    {
      if (pos != 62)
        outpw(TA_ADDR(realnum), (__bcBasePC[realnum]<<6) | 62);
      outpw(TA_DATA(realnum), bcTimeA);
      pos = 62 + 1;
    }
    if (bcCTLA & CTLA_LINKA)
    {
      if (pos != 63)
        outpw(TA_ADDR(realnum), (__bcBasePC[realnum]<<6) | 63);
      outpw(TA_DATA(realnum), bcLinkA);
    }

    REST_IRQ_SMP();
    break;
  }
  DrvIOEpi;
  return 0;
}

U16 FARFN bcgetctla(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 bcCTLA)
{
  int register realnum;
  unsigned type;
  unsigned ctla=0;
  int read;

  DrvIOPro(0, 0, 0, 0, 0, 1);
  CLRtmkError;
  realnum = GET_RealNum;
  CHECK_TMK_MODE(realnum, BC_MODE);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    USER_ERROR(BC_BAD_FUNC);
    break;
  case __TA:
    read = 0;
    GET_DIS_IRQ_SMP();

    switch (bcCTLA)
    {
    case CTLA_CTRLA:
      ctla = DrvBcPeekTA(realnum, 61);
      read = 1;
      break;
    case CTLA_TIMEA:
      ctla = DrvBcPeekTA(realnum, 62);
      read = 1;
      break;
    case CTLA_LINKA:
      ctla = DrvBcPeekTA(realnum, 63);
      read = 1;
      break;
    }

    REST_IRQ_SMP();
    if (!read)
      USER_ERROR(BC_BAD_FUNC);
    break;
  }
  DrvIOEpi;
  return ctla;
}

int FARFN bcdeflink(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Base, U16 bcCtrlCode)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned code;
  unsigned base;
#if DRV_MAX_BASE < 256
  unsigned link;
#endif
  unsigned code1;

  DrvIOPro(3, 0, 1, 1, 0, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE_BCMT(realnum);
  code = bcCtrlCode;
  CHECK_BC_CTRLX(code);
  base = Base;
  CHECK_BCMT_BASE_BX(realnum, base);
  base += __rtHeapIni[realnum];
#if DRV_MAX_BASE < 256
  link = (base << 6) | code;
  code1 = __bcLinkWN[realnum][__bcBasePC[realnum]];
  __bcLinkWN[realnum][__bcBasePC[realnum]] = link;
////                movzx   edx, __bcBasePC[ebx]
////                mov     edi, bcLinkWPtr[ebx*2]
////                mov     [edi+edx*2], ax
  DrvIOSetParN1(IOPARN_BCLINKWN, __bcBasePC[realnum], link);
#else
  __bcLinkBaseN[realnum][__bcBasePC[realnum]] = base;
  DrvIOSetParN1(IOPARN_BCLINKBASEN, __bcBasePC[realnum], base);
  code1 = __bcLinkCCN[realnum][__bcBasePC[realnum]];
  __bcLinkCCN[realnum][__bcBasePC[realnum]] = code;
  DrvIOSetParN1(IOPARN_BCLINKCCN, __bcBasePC[realnum], code);
#endif
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(BC_BAD_FUNC);
  case __TMKX:
#if DRV_MAX_BASE < 256
    DrvBcPoke(realnum, type, 0x3F, link);
#else
    DrvBcPoke(realnum, type, 0x3F, (base << 6) | code);
#endif
    break;
  case __TA:
    if (__tmkMode[realnum] == BC_MODE) //bcdeflink
    {
      unsigned ContrW = __bcCtrlWA[realnum];

//      if (((code&0xf) == DATA_RT_RT) || ((code&0xf) == DATA_RT_RT_BRCST))
      if ((code&0x7) == DATA_RT_RT)
        ContrW |= 0x0040;
      if (code & CX_BUS_B)
        ContrW |= 0x0080;
      if (code & CX_CONT)
        ContrW |= 0x2000;

      GET_DIS_IRQ_SMP();

      if ((code1 ^ code) & CX_SIG) //code1CX_SIG != codeCX_SIG
        DrvPeekAndOrPokeTA(realnum, __bcBasePC[realnum], 61, 0x7FFF, code & 0x8000);
//      DrvBcPokeTA(realnum, 61, ((DrvBcPeekTA(realnum, 61) & 0x7FFF) | (code & 0x8000)));
      DrvBcPokeTA(realnum, 63, base);

#if DRV_MAX_BASE < 256
      code1 = __bcLinkWN[realnum][base];
#else
      code1 = __bcLinkCCN[realnum][base];
#endif
      if (code1 & CX_SIG)
        ContrW |= 0x8000;
      outpw(TA_ADDR(realnum), (base<<6) | 61);
      outpw(TA_DATA(realnum), ContrW);
//      DrvPeekAndOrPokeTA(realnum, base, 61, 0x8000, ContrW));
      outpw(TA_DATA(realnum), 0);

      REST_IRQ_SMP();
    }
    else //mtdeflink
    {
      GET_DIS_IRQ_SMP();
      DrvBcPokeTA(realnum, 63, base|0x4000);
      // protect from writing to a random base while monitor is going
      // to stop (in DIRQLTmksInt1) after next base message completed
      if ((code & CX_CONT) == 0 && base != __bcBasePC[realnum]) // CX_STOP
      {
        DrvRtPokeTA(realnum, base, 63, MTA_STOP_BASE);
        __mtStopBaseTA[realnum] = base;
      }
      REST_IRQ_SMP();
    }
    break;
  }
  DrvIOEpi;
  return 0;
}

U32 FARFN bcgetlink(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
#if DRV_MAX_BASE < 256
  unsigned link;
#endif
  unsigned base;


  CLRtmkError;
  realnum = GET_RealNum;
  CHECK_TMK_MODE_BCMT(realnum);
#if DRV_MAX_BASE < 256
  link = __bcLinkWN[realnum][__bcBasePC[realnum]];
////                movzx   eax, __bcBasePC[ebx]
////                mov     edi, bcLinkWPtr[ebx*2]
////                movzx   eax, word ptr [edi+eax*2]
  base = ((link >> 6) & __bcMaxBase[realnum]) - __rtHeapIni[realnum];
  link &= 0x803F;
  return ((U32)link << 16) | (U32)base;
#else
  base = __bcBasePC[realnum];
  return ((U32)__bcLinkCCN[realnum][base] << 16) |
          (U32)(__bcLinkBaseN[realnum][base] - __rtHeapIni[realnum]);
#endif
}

U32 FARFN bcgetstate(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned stat;
  unsigned base;

  DrvIOPro(0, 0, 0, 0, 1, 1);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE_BCMT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(BC_BAD_FUNC);
  case __TMKX:
    GET_DIS_IRQ_SMP();
    stat = inpw(TMK_Mode(realnum));
    REST_IRQ_SMP();
    base = (stat >> 6) & __bcMaxBase[realnum];
    break;
  case __TA:
    stat = __bcmtStateX[realnum]; // should be read before base
    DrvIOGetParPtrU(IOPAR_BCMTSTATEX, &stat);
    if (__tmkMode[realnum] == BC_MODE)
    { // bcgetstate
      base = (inpw(TA_MSGA(realnum))) & 0x3FF;
    }
    else
    { // mtgetstate
//      base = (inpw(TA_BASE(realnum))) & 0x3FF; // random at poweron and useless for mt+rt
      GET_DIS_IRQ_SMP();
      base = DrvRtPeekTA(realnum, AdrTab, 0) & 0x3FF;
      REST_IRQ_SMP();
      if (base == MTA_STOP_BASE)
        base = __mtStopBaseTA[realnum];
      if (base <= __rtHeapIni[realnum])
        base = 0;
      else
        base -= __rtHeapIni[realnum];
    }
    break;
  }
  stat &= 0x003F;
  DrvIOEpi;
  return ((U32)stat << 16) | (U32)base;
}

U16 FARFN bcgetmaxbase(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;

  CLRtmkError;
  realnum = GET_RealNum;
#if DRV_MAX_BASE > 255
  if (__tmkMode[realnum] == BC_MODE)
    return __bcMaxBase[realnum];
  else
    return __mtMaxBase[realnum];
#else
  return __bcMaxBase[realnum];
#endif
}

U16 FARFN mtgetmaxmaxbase(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned maxmaxbase;

  CLRtmkError;
  realnum = GET_RealNum;
  if (__rtHeapIni[realnum] != 0 && __rtHeapIni[realnum] == __rtHeapAlloc[realnum])
    maxmaxbase = __mtMaxMaxBase[realnum] - __rtHeapMin[realnum];
  else
    maxmaxbase = __mtMaxMaxBase[realnum] - __rtHeapIni[realnum];
  return (U16)maxmaxbase;
}

int FARFN mtdefmaxbase(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 MaxBasePC)
{
  int register realnum;
  unsigned maxbase;

  CLRtmkError;
  realnum = GET_RealNum;
//  CHECK_TMK_MODE_BCMT(realnum);
  maxbase = MaxBasePC;
  if (__rtHeapIni[realnum] != 0 && __rtHeapIni[realnum] == __rtHeapAlloc[realnum])
  {
    if (maxbase > (unsigned)(__mtMaxMaxBase[realnum] - __rtHeapMin[realnum]))
      return USER_ERROR(BC_BAD_BASE);
    __rtHeapAlloc[realnum] =
    __rtHeapIni[realnum] = __mtMaxMaxBase[realnum] - maxbase;
  }
  else
  {
    if (maxbase > (unsigned)(__mtMaxMaxBase[realnum] - __rtHeapIni[realnum]))
      return USER_ERROR(BC_BAD_BASE);
  }
#if DRV_MAX_BASE > 255
  __mtMaxBase[realnum] = maxbase;
#else
  __bcMaxBase[realnum] = maxbase;
#endif
  return 0;
}

U32 FARFN tmkinfo(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 tmkData, U16 tmkInfo)
{
  int register realnum;
  U32 res;

  CLRtmkError;
  realnum = GET_RealNum;
  res = TMK_INFO_ERROR;
//  CHECK_TMK_MODE_BCMT(realnum);
  switch (tmkInfo)
  {
  case TMK_INFO_MAX:
    res = (U32)MRT_BRC_SW_PAGE_0;
    break;
  case MT_BASE_DRV_TO_HW:
    if (tmkData <= __mtMaxBase[realnum])
      res = (U32)(tmkData + __rtHeapIni[realnum]);
    break;
  case MT_BASE_HW_TO_DRV:
    if (tmkData <= __mtMaxMaxBase[realnum] && tmkData >= __rtHeapIni[realnum])
      res = (U32)(tmkData - __rtHeapIni[realnum]);
    break;
  case TMK_INFO_KEEP_FLGM:
    res = (U32)__tmkKeepFlgM;
    break;
  case TMK_INFO_CBSND:
#ifdef USE_TMK_CBSND
    switch (__tmkDrvType[realnum])
    {
    case __TA:
#ifdef MRTA
    case __MRTA:
#endif
      res = (U32)__tmkCBSND;
      break;
    default:
      res = (U32)0;
      break;
    }
#else
    res = (U32)0;
#endif
    break;
  case TMK_INFO_WRAP_SA:
#ifdef USE_TMK_WRAP_SA
    switch (__tmkDrvType[realnum])
    {
    case __TA:
#ifdef MRTA
    case __MRTA:
#endif
      res = (U32)__tmkWrapSA
        | ((__tmkWrapSA && !__tmkWrapNoST5) ? (U32)RT_WRAP_SA_TX : (U32)0)
        | ((__tmkWrapSA && __tmkWrapInt) ? (U32)RT_WRAP_SA_INT : (U32)0);
      break;
    default:
      res = (U32)0;
      break;
    }
#else
    res = (U32)0;
#endif
    break;
#if NRT > 0
  case MRT_BRC_HW_PAGES:
    switch (__tmkDrvType[realnum])
    {
#ifdef MRTA
    case __MRTA:
      res = (U32)1;
      break;
#endif
    default:
      res = (U32)0;
      break;
    }
    break;
  case MRT_BRC_HW_PAGE_0:
    res = (U32)0;
    break;
  case MRT_BRC_SW_PAGES:
    switch (__tmkDrvType[realnum])
    {
#ifdef MRTX
    case __MRTX:
#endif
#ifdef MRTA
    case __MRTA:
#endif
      res = (U32)1;
      break;
    default:
      res = (U32)0;
      break;
    }
    break;
  case MRT_BRC_SW_PAGE_0:
    switch (__tmkDrvType[realnum])
    {
#ifdef MRTX
    case __MRTX:
#endif
#ifdef MRTA
    case __MRTA:
#endif
      res = (U32)1;
      break;
    default:
      res = (U32)0;
      break;
    }
    break;
#else //NRT > 0
  case MRT_BRC_HW_PAGES:
  case MRT_BRC_HW_PAGE_0:
  case MRT_BRC_SW_PAGES:
  case MRT_BRC_SW_PAGE_0:
    res = (U32)0;
    break;
#endif //NRT > 0
// change param TMK_INFO_MAX return value if add new params!!!
  }
  return res;
}

U16 FARFN bcgetbase(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;

  CLRtmkError;
  realnum = GET_RealNum;
  return __bcBasePC[realnum] - __rtHeapIni[realnum];
}

/*
//__inline
__INLINE
void DrvBcDefBaseTA(int REGPAR realnum, unsigned base)
{
  __bcBasePC[realnum] = base;
  return;
}
*/

//__inline
//__INLINE
void DrvBcDefBase(int REGPAR realnum, unsigned type, unsigned base)
{
  LOCAL_PORT

  __bcBasePC[realnum] = base;
  switch (type)
  {
#ifdef MRTX
  case __MRTX:
#endif
  case __TMKX:
  case __TA:
#ifdef MRTA
  case __MRTA:
#endif
    return;
  }
  SET_LOCAL_PORT(realnum);
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    outpw_d(realnum, TMK_Addr(realnum), base);
    return;
  case __TMKMPC:
    outpb_d(realnum, TMKMPC_AddrH(realnum), base);
    return;
  }
  return;
}

int FARFN bcdefbase(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 BasePC)
{
  int register realnum;
  unsigned type;
  unsigned base;

  CLRtmkError;
  realnum = GET_RealNum;
  CHECK_TMK_MODE_BCMT(realnum);
  base = BasePC;
  CHECK_BCMT_BASE_BX(realnum, base);
  base += __rtHeapIni[realnum];
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  DrvBcDefBase(realnum, type, base);
  return 0;
}

void DrvBcPoke(int REGPAR realnum, unsigned type, unsigned pos, unsigned data)
// write to pos in current base
// doesn't suppose subsequent use of addr, so doesn't increment __tmkRAMAddr
{
  LOCAL_PORT
  unsigned save_rama, save_ramiw;
  IRQ_FLAGS;

  DrvIOPro(3, 0, 0, 0, 0, 0);
  SET_LOCAL_PORT(realnum);
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    outpw_d(realnum, TMK_Data(realnum), data);
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    outpwb_d(realnum, TMKMPC_DataH(realnum), data);
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
//#ifdef MRTX
//  case __MRTX:
//#endif
  case __TMKX:
  case __TA:
//#ifdef MRTA
//  case __MRTA:
//#endif
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    pos |= __bcBasePC[realnum] << 6;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    outpw(TMK_Data(realnum), data);
    REST_IRQ_SMP();
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
//  case __TA:
//    pos |= __bcBasePC[realnum] << 6;
//    outpw(TA_ADDR(realnum), pos);
//    outpw(TA_DATA(realnum), data);
//    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  DrvIOEpi;
  return;
}

//__INLINE
//void DrvBcPokeTA(int REGPAR realnum, unsigned pos, unsigned data)
//{
//  DrvBcPoke(realnum, TA, pos, data);
//}

/*
__INLINE
void DrvBcPokeTA(int REGPAR realnum, unsigned pos, unsigned data)
{
  outpw(TA_ADDR(realnum), (__bcBasePC[realnum] << 6) + pos);
  outpw(TA_DATA(realnum), data);
}
*/

unsigned DrvBcPeek(int REGPAR realnum, unsigned type, unsigned pos)
// read from pos in current base
// doesn't suppose subsequent use of addr, so doesn't increment __tmkRAMAddr
{
  LOCAL_PORT
  unsigned save_rama, save_ramiw;
  unsigned data=0;

  DrvIOPro(2, 0, 0, 0, 1, 0);
  SET_LOCAL_PORT(realnum);
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    data = inpw_d(realnum, TMK_Data(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    data = inpw_d(realnum, TMKMPC_DataL(realnum));
//    data = inpwb_d(TMKMPC_DataL(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
//#ifdef MRTX
//  case __MRTX:
//#endif
  case __TMKX:
  case __TA:
//#ifdef MRTA
//  case __MRTA:
//#endif
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    pos |= __bcBasePC[realnum] << 6;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    inptrwu(TMK_Data(realnum), &data);
    REST_IRQ_SMP();
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
//  case __TA:
//    pos |= __bcBasePC[realnum] << 6;
//    outpw(TA_ADDR(realnum), pos);
//    data = inpw(TA_DATA(realnum));
//    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  DrvIOEpi;
  return data;
}

//__inline
//__INLINE
//unsigned DrvBcPeekTA(int REGPAR realnum, unsigned pos)
//{
//  return DrvBcPeek(realnum, TA, pos);
//}

/*
__INLINE
unsigned DrvBcPeekTA(int REGPAR realnum, unsigned pos)
{
  outpw(TA_ADDR(realnum), (__bcBasePC[realnum] << 6) + pos);
  return inpw(TA_DATA(realnum));
}
*/

U32 FARFN bcgetansw(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 bcCtrlCode)
{
  int register realnum;
  unsigned type;
  unsigned aw1pos, aw2pos;
  unsigned aw1, aw2;
  unsigned code;

  DrvIOPro(0, 0, 0, 0, 0, 2);
  CLRtmkError;
  realnum = GET_RealNum;
  CHECK_TMK_MODE_BCMT(realnum);
  code = bcCtrlCode;
  CHECK_BC_CTRL(code);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  aw2pos = 0;
  switch (code)
  {
  case DATA_BC_RT:
    aw1pos = __bcCmdWN[realnum][__bcBasePC[realnum]] & 0x1F;
////                movzx   eax, __bcBasePC[ebx]
////                mov     esi, bcCmdWPtr[ebx*2]
////                movzx   eax, word ptr [esi+eax*2]
////                and     eax, 1Fh
    if (aw1pos == 0)
      aw1pos = 0x20;
    ++aw1pos;
    break;
  case DATA_RT_BC:
  case CTRL_C_A:
  case CTRL_C_AD:
    aw1pos = 1;
    break;
  case DATA_RT_RT:
    aw2pos = __bcCmdWN[realnum][__bcBasePC[realnum]] & 0x1F;
////                movzx   eax, __bcBasePC[ebx]
////                mov     esi, bcCmdWPtr[ebx*2]
////                movzx   edx, word ptr [esi+eax*2]
////                and     edx, 1Fh
    if (aw2pos == 0)
      aw2pos = 0x20;
    aw2pos += 3;
    fallthrough;
    // fall-through
  case DATA_RT_RT_BRCST:
  case CTRL_CD_A:
    aw1pos = 2;
    break;
  default:
    aw1pos = 0;
    break;
  }
  aw2 = (aw2pos == 0) ? 0xFFFF : DrvBcPeek(realnum, type, aw2pos);
  aw1 = (aw1pos == 0) ? 0xFFFF : DrvBcPeek(realnum, type, aw1pos);
  DrvIOEpi;
  return ((U32)aw2 << 16) | (U32)aw1;
}

U16 FARFN mtgetsw(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  unsigned RetData;
  unsigned SW;

  DrvIOPro(0, 0, 0, 0, 0, 1);
  CLRtmkError;
  realnum = GET_RealNum;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TMKX:
    RetData = DrvBcPeek(realnum, type, 0x3E);
    break;
  case __TA:
    GET_DIS_IRQ_SMP();
    SW = DrvBcPeekTA(realnum, 58);
    REST_IRQ_SMP();
    RetData = (SW & 0x7) | ((SW & 0x3C0) << 4) | ((SW & 0x8) << 12);
    if (SW & 0x30)
      RetData |= 0x8;
    if (((SW & 0x3C0) != 0x3C0) && ((SW & 0x07) == 0x07) && (__tmkHWVer[realnum] < 19)) // bus switching
      RetData = ((RetData & ~0x3C00) | 0x1C00) ^ 0x8000;
// any error or format error or bit set (integrated bit)
    if ((((SW & 0x3C0) != 0x3C0) && (SW & 0x37)) || ((SW & 0x3C0) == 0x1C0))
      RetData |= 0x4000;
    break;
  default:
    RetData = 0xFFFF;
    break;
  }
  DrvIOEpi;
  return RetData;
}

U32 FARFN mtgetsw2(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  unsigned RetDataH, RetData;
  unsigned SW;

  DrvIOPro(0, 0, 0, 0, 0, 1);
  CLRtmkError;
  realnum = GET_RealNum;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TMKX:
    RetData = DrvBcPeek(realnum, type, 0x3E);
    RetDataH = 0;
    break;
  case __TA:
    GET_DIS_IRQ_SMP();
    SW = DrvBcPeekTA(realnum, 58);
    REST_IRQ_SMP();
    RetData = (SW & 0x7) | ((SW & 0x3C0) << 4) | ((SW & 0x8) << 12);
    if (SW & 0x30)
      RetData |= 0x8;
    if (((SW & 0x3C0) != 0x3C0) && ((SW & 0x07) == 0x07) && (__tmkHWVer[realnum] < 19)) // bus switching
      RetData = ((RetData & ~0x3C00) | 0x1C00) ^ 0x8000;
// any error or format error or bit set (integrated bit)
    if ((((SW & 0x3C0) != 0x3C0) && (SW & 0x37)) || ((SW & 0x3C0) == 0x1C0))
      RetData |= 0x4000;
    RetDataH = (SW & 0xFC00) >> 10;
    break;
  default:
    RetData = 0xFFFF;
    RetDataH = 0;
    break;
  }
  DrvIOEpi;
  return ((U32)RetDataH << 16) | (U32)RetData;
}

U16 FARFN mtgetswa(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  unsigned RetData;
  unsigned SW;

  DrvIOPro(0, 0, 0, 0, 0, 1);
  CLRtmkError;
  realnum = GET_RealNum;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TMKX:
    SW = DrvBcPeek(realnum, type, 0x3E);
    RetData = 0xFC00 | (SW & 0x7) | ((SW & 0x3C00) >> 4) | ((SW & 0x8000) >> 12);
    if (SW & 0x8)
      RetData |= 0x30;
    if ((SW & 0x400F) == 0x4000)
      RetData = (RetData & 0xFC3F) | 0x01C0; // format error (integrated bit)
    break;
  case __TA:
    GET_DIS_IRQ_SMP();
    RetData = DrvBcPeekTA(realnum, 58);
    REST_IRQ_SMP();
    if (((RetData & 0x3C0) != 0x3C0) && ((RetData & 0x07) == 0x07) && (__tmkHWVer[realnum] < 19)) // bus switching
      RetData = ((RetData & ~0x03C0) | 0x01C0) ^ 0x0008;
    break;
  default:
    RetData = 0;
    break;
  }
  DrvIOEpi;
  return RetData;
}

void FARFN bcputw(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 bcPos, U16 bcData)
{
  int register realnum;
  unsigned type;
  unsigned pos;
  unsigned data;

  CLRtmkError;
  realnum = GET_RealNum;
  CHECK_TMK_MODE_BCMTN(realnum);
  pos = bcPos;
  CHECK_BC_ADDR(pos);
  data = bcData;
  if (pos == 0)
  {
    __bcCmdWN[realnum][__bcBasePC[realnum]] = data;
////                movzx   eax, __bcBasePC[ebx]
////                mov     edi, bcCmdWPtr[ebx*2]
////                mov     [edi+eax*2], dx
  }
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  DrvBcPoke(realnum, type, pos, data);
}

U16 FARFN bcgetw(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 bcPos)
{
  int register realnum;
  unsigned type;
  unsigned pos;

  CLRtmkError;
  realnum = GET_RealNum;
  CHECK_TMK_MODE_BCMT(realnum);
  pos = bcPos;
  CHECK_BC_ADDR(pos);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  return (U16)DrvBcPeek(realnum, type, pos);
}

void FARFN bcputblk(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 bcPos, VOID FARDT *pcBufAddr, U16 bcLen)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  U16 FARDT *buf;
  unsigned save_rama, save_ramiw;
  IRQ_FLAGS;

  DrvIOProV;
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE_BCMTN(realnum);
  pos = bcPos;
  CHECK_BC_ADDR(pos);
  buf = (U16 FARDT*)pcBufAddr;
  len = bcLen;
  CHECK_BC_LEN(len);
  if (len == 0)
  {
    DrvIOStop;
    return;
  }
  if (pos == 0)
  {
    __bcCmdWN[realnum][__bcBasePC[realnum]] = *(U16 FARDT*)buf;
////                movzx   eax, __bcBasePC[ebx]
////                mov     edi, bcCmdWPtr[ebx*2]
////                movzx   edx, word ptr [esi]
////                mov     [edi+eax*2], dx
  }
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    REP_OUTSW_D(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    REP_OUTSWB_D(TMKMPC_DataL(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
//#ifdef MRTX
//  case __MRTX:
//#endif
  case __TMKX:
  case __TA:
//#ifdef MRTA
//  case __MRTA:
//#endif
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    pos |= __bcBasePC[realnum] << 6;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    REST_IRQ_SMP();
    REP_OUTSW(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  DrvIOEpi;
  return;
}

void FARFN bcgetblk(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 bcPos, VOID FARDT *pcBufAddr, U16 bcLen)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  U16 FARDT *buf;
  unsigned save_rama, save_ramiw;
  IRQ_FLAGS;

  DrvIOProV;
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE_BCMTN(realnum);
  pos = bcPos;
  CHECK_BC_ADDR(pos);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  buf = (U16 FARDT*)pcBufAddr;
  len = bcLen;
  CHECK_BC_LEN(len);
  if (len == 0)
  {
    DrvIOStop;
    return;
  }
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    REP_INSW_D(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    REP_INSW_D(TMKMPC_DataL(realnum));
//    REP_INSWB_D(TMKMPC_DataL(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
//#ifdef MRTX
//  case __MRTX:
//#endif
  case __TMKX:
  case __TA:
//#ifdef MRTA
//  case __MRTA:
//#endif
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    pos |= __bcBasePC[realnum] << 6;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    REST_IRQ_SMP();
    REP_INSW(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  DrvIOEpi;
  return;
}

int FARFN bcdefbus(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Bus)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned bus;

  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE(realnum, BC_MODE);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  bus = Bus;
  switch (type)
  {
  case __RTMK400:
    CHECK_BC_BUS(bus);
    __bcBus[realnum] = bus;
    bus <<= 4;
    GET_MUTEX;
    bus |= __bcControls[realnum] & 0x00EF;
    __bcControls[realnum] = bus;
    outpb_d(realnum, TMK_Mode(realnum), bus);
    REST_MUTEX;
    break;
  case __TMKX:
  case __TA:
    CHECK_BC_BUS(bus)
    __bcBus[realnum] = bus;
    break;
  case __TMK400:
  case __TMKMPC:
    if (bus == 0)
      break;
    return USER_ERROR(BC_BAD_BUS);
  }
  return 0;
}

U16 FARFN bcgetbus(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;

  CLRtmkError;
  realnum = GET_RealNum;
  return __bcBus[realnum];
}

void DrvRtDefSubAddr(int REGPAR num, unsigned type, unsigned sa_shl5)
{
  LOCAL_PORT

  // what if mrta brc page active ???
  if (__rtSABufMode[num][sa_shl5>>5] == SABUF_JOINT)
    sa_shl5 &= ~RT_DIR_MASK;
  __rtSubAddr[num] = sa_shl5;
  __rtSABuf[num] = 0;
  switch (type)
  {
#ifdef MRTA
  case __MRTA:
#ifdef USE_TMK_WRAP_SA
    if (__tmkWrapSA)
    {
      if (__tmkWrapNoST5 && (sa_shl5 == __tmkWrapNoST5))
      {
//        sa_shl5 &= ~RT_DIR_MASK;
        __rtSubAddr[num] = sa_shl5 & ~RT_DIR_MASK;
      }
    }
#endif
    if (__rtPagePC[num] <= __rtMaxPage[num])
    { //std page 0
      __hm400Page2[num] = __hm400Page0[num] >> 4;
      __hm400Page[num] = __hm400Page0[num] << 11;
    }
    else
    { //brc page 0
      __hm400Page2[num] = MRTA_BRC_PAGE >> 4;
      __hm400Page[num] = (U16)(MRTA_BRC_PAGE << 11);
    }
    break;
#endif
  case __TA:
#ifdef USE_TMK_WRAP_SA
    if (__tmkWrapSA)
    {
      if (__tmkWrapNoST5 && (sa_shl5 == __tmkWrapNoST5))
      {
//        sa_shl5 &= ~RT_DIR_MASK;
        __rtSubAddr[num] = sa_shl5 & ~RT_DIR_MASK;
      }
    }
#endif
    __hm400Page[num] = 0;
    fallthrough;
    // fall-through
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
    return;
  }
  SET_LOCAL_PORT(num);
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    sa_shl5 |= __hm400Page[num];
    sa_shl5 >>= 6;
    outpw_d(num, TMK_Addr(num), sa_shl5);
    return;
  case __TMKMPC:
//;                or      ax, __hm400Page[ebx];
    sa_shl5 >>= 6;
    outpb_d(num, TMKMPC_AddrH(num), sa_shl5);
    return;
  }
  return;
}

//__inline
__INLINE
unsigned DrvRtGetBaseTA(int REGPAR num)
{
  return (__hm400Page[num] ^ __rtSubAddr[num]) >> 5;
}

#if NRT > 0
//__inline
__INLINE
unsigned DrvRtGetBaseMRTA(int REGPAR num)
{
  return (__hm400Page2[num] << 10) | ((__hm400Page[num] ^ __rtSubAddr[num]) >> 5);
}

void FARFN mrtdefbrcsubaddr0(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  unsigned sa;

  CLRtmkError;
  realnum = GET_RealNum;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
#ifdef MRTX
  case __MRTX:
    sa = 0x1F << 5;
    break;
#endif
#ifdef MRTA
// check with DrvFlagModeTA!!!
//  case __MRTA:
//    sa = mrtBrcSubAddr0[realnum]; //& 0xFFE0
//    break;
#endif
  default:
    return;
  }
  DrvRtDefSubAddr(realnum, type, sa);
  if (realnum != __tmkNumber)
  {
    DrvRtDefSubAddr(__tmkNumber, type, sa);
                //; call    DrvRtWMode
  }
  return;
}
#endif //NRT

void FARFN rtdefsubaddrm(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Dir, U16 SubAddr)
{
  int register num;
  unsigned type;
  unsigned sa;

  CLRtmkError;
  num = __tmkNumber;
  sa = SubAddr;
  CHECK_RT_SUBADDR(sa);
  sa <<= 5;
  CHECK_RT_DIR(Dir);
  sa |= Dir;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  DrvRtDefSubAddr(num, type, sa);
  return;
}

void FARFN rtdefsubaddr(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Dir, U16 SubAddr)
{
  int register num;
  unsigned type;
  unsigned sa;

  DrvIOPro(0, 0, 0, 2, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  sa = SubAddr;
  CHECK_RT_SUBADDR(sa);
  sa <<= 5;
  CHECK_RT_DIR(Dir);
  sa |= Dir;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  DrvRtDefSubAddr(num, type, sa);
  DrvRtWMode(num, type, sa);
  DrvIOEpi;
  return;
}

U16 FARFN rtgetsubaddr(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned sa;

  CLRtmkError;
  num = __tmkNumber;
  sa = __rtSubAddr[num];
  return ((sa >> 5) & 0x001F) | (sa & 0x0400);
}

U16 FARFN rtgetbase(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
#ifdef MRTA
  unsigned type;
#endif
  unsigned base;

  CLRtmkError;
  num = __tmkNumber;
#ifdef MRTA
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  if (type == __MRTA)
    base = DrvRtGetBaseMRTA(num);
  else
#endif
    base = DrvRtGetBaseTA(num);
  return base;
}

U16 FARFN rtgetbasebus(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
#if NRT > 0
  int realnum;
#endif
  unsigned type;
  LOCAL_PORT
  unsigned base=0;
#if NRT > 0
  unsigned state, statex;
  int limit;
#endif

  DrvIOPro(0, 0, 0, 0, 1, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
#if NRT > 0
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#else
  SET_LOCAL_PORT(num);
#endif
  switch (type)
  {
  case __TMKMPC:
  case __TMK400:
  case __RTMK400:
  case __TMKX:
    base = (__rtPageBus[num] << 6) | ((rtgetstate(TMKNUM(num)) & 0x07E0) >> 5);
    if ((base & 0x1F) == 0)
      base |= 0x1F;
    break;
#ifdef MRTX
  case __MRTX:
//    num = __tmkNumber;
    limit = 16; // abstract loop limit for broken hardware
    state = inpw(TMK_State(realnum));
    do
    {
      statex = state;
      state = inpw(TMK_State(realnum));
    }
    while (statex != state && --limit != 0);
    if ((state & 0x8000) == 0)
    {
      base = (((state >> 1) & 0x1800) | (state & 0x07E0)) >> 5;
      if (((base & 0x1F) == 0) || ((base & 0x3F) == 0x1F))
        base |= 0x3F;
    }
    else
      base = 0x1F;
    break;
#endif
  case __TA:
    base = inpw(TA_BASE(num)); //will return real base number for mt in mtrt
                               //we cannot return virtual mt base number
                               //because it can mix with real rt base number
    break;
#ifdef MRTA
  case __MRTA:
    GET_DIS_IRQ();
    if (__rtPagePC[num] <=  __rtMaxPage[num])
    {
      if ((__rtControls1[num] & TA_BRCST_MODE) && (__tmkOptions[num] & (UOPT)MRT_WRITE_BRC_DATA))
      {
        outpw(TA_LCW(realnum), 0xF800);
        inptrwu(TA_BASE(realnum), &statex);
      }
      else
        statex = 0;
      outpw(TA_LCW(realnum), __rtControls1[num] & 0xF800); // no matter if TA_BRCST_MODE here
      base = inpw(TA_BASE(realnum)); // DrvIOEpi, DrvIOPro
      if (((statex ^ base) & 0x0FFF) == 0)
        base |= statex; // join RT active bits
    }
    else
    {
      outpw(TA_LCW(realnum), 0xF800);
      base = inpw(TA_BASE(realnum));
    }
    REST_IRQ();
    break;
#endif
  }
  DrvIOEpi;
  return base;
}

void FARFN rtgetblk(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtPos, VOID FARDT *pcBufAddr, U16 Len)
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  U16 FARDT *buf;
  unsigned save_rama, save_ramiw;
  IRQ_FLAGS;

  DrvIOProV;
  CLRtmkError;
  num = __tmkNumber;
  pos = rtPos;
  CHECK_RT_SUBPOS(pos);
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  buf = (U16 FARDT*)pcBufAddr;
  len = Len;
  CHECK_RT_LEN(len);
  if (len == 0)
  {
    DrvIOStop;
    return;
  }
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#if NRT > 0
  if (__rtPagePC[num] == (__rtMaxPage[num] + 2)) // brc sw page
  {
    if ((pos + len) > 32)
      len = 32 - pos;
    pos += __rtSubAddr[num] & 0x03E0; // clear T/R
    do
      *buf++ = __awBrcRxBuf[realnum][pos++];
    while (--len != 0);
    DrvIOStop;
    return;
  }
#endif
  pos += __rtSubAddr[num];
  pos ^= __hm400Page[num];
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    REP_INSW_D(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    REP_INSW_D(TMKMPC_DataL(realnum));
//    REP_INSWB_D(TMKMPC_DataL(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
#ifdef MRTA
  case __MRTA:
    GET_DIS_IRQ_SMP();
    outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
    REST_IRQ_SMP();
    // fall-through
#endif
    fallthrough;
    // fall-through
  case __TA:
    pos = ((pos & 0xFFE0) << 1) | (pos & 0x1F);
    fallthrough;
    // fall-through
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    REST_IRQ_SMP();
    REP_INSW(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  DrvIOEpi;
  return;
}

#ifdef USE_RTIFNB
U16 FARFN rtgetblkifnb(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtPos, VOID FARDT *pcBufAddr, U16 Len)
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  U16 FARDT *buf;
  unsigned save_rama, save_ramiw;
  unsigned state=0, statex, statea;
  int limit;
  unsigned port, dataport;
  IRQ_FLAGS;

  DrvIOProV;
  CLRtmkError;
  num = __tmkNumber;
  pos = rtPos;
  CHECK_RT_SUBPOS(pos);
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  buf = (U16 FARDT*)pcBufAddr;
  len = Len;
  CHECK_RT_LEN_R(len);
  if (len == 0)
  {
    DrvIOStop;
    return 0;
  }
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#if NRT > 0
  if (__rtPagePC[num] == (__rtMaxPage[num] + 2)) // brc sw page
  {
    if ((pos + len) > 32)
      len = 32 - pos;
    pos += __rtSubAddr[num] & 0x03E0; // clear T/R
    GET_DIS_IRQ();
    do
      *buf++ = __awBrcRxBuf[realnum][pos++];
    while (--len != 0);
    REST_IRQ();
    DrvIOStop;
    return 0;
  }
#ifdef MRTX
  if (type == __MRTX)
  { // keep DrvRtWMode out of GET_DIS_IRQ
    if (__rtMode[num] != __rtMode[realnum] && (__rtMode[realnum] & 0x0800) == 0)
      // if different (switched rt) and unlocked mode
      DrvRtWMode(num, __MRTX, __rtMode[num]);
  }
#endif
#endif
  pos += __rtSubAddr[num];
  pos ^= __hm400Page[num];
  GET_DIS_IRQ();
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
//  __tmkRAMInWork[realnum] = 1; no need because of GET_DIS_IRQ
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
//    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    break;
  case __TMKMPC:
//    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    break;
#ifdef MRTA
  case __MRTA:
    outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
    // fall-through
#endif
    fallthrough;
    // fall-through
  case __TA:
    pos = ((pos & 0xFFE0) << 1) | (pos & 0x1F);
    fallthrough;
    // fall-through
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
//      __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    break;
  }
  dataport = TMK_Data(realnum);
  port = TMK_State(realnum);
  switch (type)
  {
  case __TMKMPC:
    dataport = TMKMPC_DataL(realnum);
    port = TMKMPC_StateL(realnum);
    fallthrough;
    // fall-through
  case __TMK400:
  case __RTMK400:
#ifndef DrvIO
    state = inpw_d(realnum, port);
#else
    state = 0; // force getblk
    inptrwu_weq(port, (1<<11), (1<<11), 0, 10000, (void*)0, &state);
#endif //ndef DrvIO
    break;
#ifdef MRTX
  case __MRTX:
#ifndef DrvIO
    statea = (__hm400Page[num]<<1) | 0x0800; // active const
    limit = 16; // abstract loop limit for broken hardware
    state = inpw(port);
    do
    {
      statex = state;
      state = inpw(port);
    }
    while (statex != state && --limit != 0);
    if ((state & 0xB800) != statea)
      state = 0;
//    if ((state & 0x8000) == 0)
//    {
//      if (((state >> 1) & 0x1800) != __hm400Page[num])
//        state = 0;
//    }
#else
    state = 0; // force getblk
    inptrwu_weq(port, 0xB800, 0x0800 | (__hm400Page[num]<<1), 1, 10000, (void*)0, &state);
#endif //ndef DrvIO
    break;
#endif
  case __TMKX:
#ifndef DrvIO
    limit = 16; // abstract loop limit for broken hardware
    state = inpw(port);
    do
    {
      statex = state;
      state = inpw(port);
    }
    while (statex != state && --limit != 0);
#else
    state = 0; // force getblk
    inptrwu_weq(port, (1<<11), (1<<11), 1, 10000, (void*)0, &state);
#endif //ndef DrvIO
    break;
  case __TA:
#ifndef DrvIO
    statea = (DrvRtGetBaseTA(realnum) & 0x03FF) | 0x2000; // active const
    state = inpw(TA_BASE(realnum));
    state = ((state & 0xA3FF) != statea) ? 0 : 0x0800;
//    if (((state & 0x8000) == 0) && ((state & 0x03FF) == (DrvRtGetBaseTA(realnum) & 0x03FF)))
//    if ((state & 0x83FF) == (DrvRtGetBaseTA(realnum) & 0x03FF))
//      state >>= 2;
//    else
//      state = 0;
#else
    state = 0; // force getblk
    inptrwu_weq(port, 0xA3FF, 0x2000 | (DrvRtGetBaseTA(realnum) & 0x03FF), 0, 10000, (void*)0, &state);
#endif //ndef DrvIO
    break;
#ifdef MRTA
  case __MRTA:
#ifndef DrvIO
    statea = (DrvRtGetBaseMRTA(num) & 0x0FFF) | 0x2000; // active const
    state = (__rtPagePC[num] <=  __rtMaxPage[num]) ? 0 : 0xF800;
    outpw(TA_LCW(realnum), (state | __rtControls1[num]) & 0xF800); // no matter if TA_BRCST_MODE here
    state = inpw(TA_BASE(realnum));
    state = ((state & 0xAFFF) != statea) ? 0 : 0x0800;
//    if (((state & 0x8000) == 0) && ((state & 0x0FFF) == (DrvRtGetBaseMRTA(num) & 0x0FFF)))
//    if ((state & 0x8FFF) == (DrvRtGetBaseMRTA(num) & 0x0FFF))
//      state >>= 2;
//    else
//      state = 0;
#else
    state = 0; // force getblk
    inptrwu_weq(port, 0xAFFF, 0x2000 | (DrvRtGetBaseMRTA(num) & 0x0FFF), 0, 10000, (void*)0, &state);
#endif //ndef DrvIO
    break;
#endif
  }
  state = (state >> 11) & 1;
  if (!state)
  {
    switch (type)
    {
    case __TMK400:
    case __RTMK400:
      REP_INSW_D_IRQ(dataport);
      break;
    case __TMKMPC:
      REP_INSW_D_IRQ(dataport);
//      REP_INSWB_D_IRQ(dataport);
      break;
    case __TMKX:
#ifdef MRTX
    case __MRTX:
#endif
    case __TA:
#ifdef MRTA
    case __MRTA:
#endif
      REP_INSW_IRQ(dataport);
      break;
    }
  }
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    if (!save_ramiw)
      break;
//    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    if (!save_ramiw)
      break;
//    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
  case __TA:
#ifdef MRTA
  case __MRTA:
#endif
    if (!save_ramiw)
      break;
//      __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    break;
  }
//  if (!save_ramiw)
//    __tmkRAMInWork[realnum] = 0;
  REST_IRQ();
  DrvIOEpi;
#ifdef DrvIO
  // process state from inptrwu_weq after DrvIOEpi
  // normally state from inptrwu_weq should be always nonbusy here
  switch (type)
  {
  case __TMKMPC:
  case __TMK400:
  case __RTMK400:
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
    state = (state >> 11) & 1;
    break;
  case __TA:
//    if (((state & 0x8000) == 0) && ((state & 0x03FF) == (DrvRtGetBaseTA(realnum) & 0x03FF)))
    if ((state & 0x83FF) == (DrvRtGetBaseTA(realnum) & 0x03FF))
      state = (state >> 13) & 1;
    else
      state = 0;
    break;
#ifdef MRTA
  case __MRTA:
//    if (((state & 0x8000) == 0) && ((state & 0x0FFF) == (DrvRtGetBaseMRTA(num) & 0x0FFF)))
    if ((state & 0x8FFF) == (DrvRtGetBaseMRTA(num) & 0x0FFF))
      state = (state >> 13) & 1;
    else
      state = 0;
    break;
#endif
  }
#endif //def DrvIO
  return state;
}
#endif //def USE_RTIFNB

#ifdef MRTA
void FARFN __rtgetblkmrta(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtPos, VOID FARDT *pcBufAddr, U16 Len)
{
  int num;
  int register realnum;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  U16 FARDT *buf;
  unsigned save_rama, save_ramiw;
  IRQ_FLAGS;

  CLRtmkError;
  num = __tmkNumber;
  pos = rtPos;
  buf = (U16 FARDT*)pcBufAddr;
  len = Len;
  if (len == 0)
    return;
  pos += __rtSubAddr[num] << 1;
  pos ^= __hm400Page[num] << 1;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
//  case __MRTA:
  GET_DIS_IRQ_SMP();
  outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
//  REST_IRQ_SMP();
//  case __TA:
//  GET_DIS_IRQ_SMP();
  __tmkRAMAddr[realnum] = pos;
  outpw(TMK_Addr(realnum), pos);
  REST_IRQ_SMP();
  REP_INSW(TMK_Data(realnum));
  if (save_ramiw)
  {
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
  }
  else
    __tmkRAMInWork[realnum] = 0;
  return;
}

void FARFN __rtgetblkmrtairq(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtPos, VOID FARDT *pcBufAddr, U16 Len)
{
  int num;
  int register realnum;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  U16 FARDT *buf;

  num = __tmkNumber;
  pos = rtPos;
  buf = (U16 FARDT*)pcBufAddr;
  len = Len;
  if (len == 0)
    return;
  pos += __rtSubAddr[num] << 1;
  pos ^= __hm400Page[num] << 1;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
//  case __MRTA:
  outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
//  case __TA:
  outpw(TMK_Addr(realnum), pos);
  REP_INSW_IRQ(TMK_Data(realnum));
  return;
}
#endif //def MRTA

void FARFN rtputblk(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtPos, VOID FARDT *pcBufAddr, U16 Len)
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  U16 FARDT *buf;
  unsigned save_rama, save_ramiw;
  IRQ_FLAGS;

  DrvIOProV;
  CLRtmkError;
  num = __tmkNumber;
  pos = rtPos;
  CHECK_RT_SUBPOS(pos);
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  buf = (U16 FARDT*)pcBufAddr;
  len = Len;
  CHECK_RT_LEN(len);
  if (len == 0)
  {
    DrvIOStop;
    return;
  }
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#if NRT > 0
  if (__rtPagePC[num] == (__rtMaxPage[num] + 2)) // brc sw page
  {
    if ((pos + len) > 32)
      len = 32 - pos;
    pos += __rtSubAddr[num] & 0x03E0; // clear T/R
    do
      __awBrcRxBuf[realnum][pos++] = *buf++;
    while (--len != 0);
    DrvIOStop;
    return;
  }
#endif
  pos += __rtSubAddr[num];
  pos ^= __hm400Page[num];
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    REP_OUTSW_D(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    REP_OUTSWB_D(TMKMPC_DataL(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
#ifdef MRTA
  case __MRTA:
    GET_DIS_IRQ_SMP();
    outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
    REST_IRQ_SMP();
    // fall-through
#endif
    fallthrough;
    // fall-through
  case __TA:
    pos = ((pos & 0xFFE0) << 1) | (pos & 0x1F);
    fallthrough;
    // fall-through
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    REST_IRQ_SMP();
    REP_OUTSW(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  DrvIOEpi;
  return;
}

#ifdef USE_RTIFNB
U16 FARFN rtputblkifnb(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtPos, VOID FARDT *pcBufAddr, U16 Len)
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  U16 FARDT *buf;
  unsigned save_rama, save_ramiw;
  unsigned state=0, statex, statea;
  int limit;
  unsigned port, dataport;
  unsigned data;
  IRQ_FLAGS;

  DrvIOProV;
  CLRtmkError;
  num = __tmkNumber;
  pos = rtPos;
  CHECK_RT_SUBPOS(pos);
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  buf = (U16 FARDT*)pcBufAddr;
  len = Len;
  CHECK_RT_LEN_R(len);
  if (len == 0)
  {
    DrvIOStop;
    return 0;
  }
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#if NRT > 0
  if (__rtPagePC[num] == (__rtMaxPage[num] + 2)) // brc sw page
  {
    if ((pos + len) > 32)
      len = 32 - pos;
    pos += __rtSubAddr[num] & 0x03E0; // clear T/R
    GET_DIS_IRQ();
    do
      __awBrcRxBuf[realnum][pos++] = *buf++;
    while (--len != 0);
    REST_IRQ();
    DrvIOStop;
    return 0;
  }
#ifdef MRTX
  if (type == __MRTX)
  { // keep DrvRtWMode out of GET_DIS_IRQ
    if (__rtMode[num] != __rtMode[realnum] && (__rtMode[realnum] & 0x0800) == 0)
      // if different (switched rt) and unlocked mode
      DrvRtWMode(num, __MRTX, __rtMode[num]);
  }
#endif
#endif
  pos += __rtSubAddr[num];
  pos ^= __hm400Page[num];
  GET_DIS_IRQ();
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
//  __tmkRAMInWork[realnum] = 1; no need because of GET_DIS_IRQ
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
//    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    break;
  case __TMKMPC:
//    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    break;
#ifdef MRTA
  case __MRTA:
    outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
    // fall-through
#endif
    fallthrough;
    // fall-through
  case __TA:
    pos = ((pos & 0xFFE0) << 1) | (pos & 0x1F);
    fallthrough;
    // fall-through
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
//      __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    break;
  }
  dataport = TMK_Data(realnum);
  port = TMK_State(realnum);
  switch (type)
  {
  case __TMKMPC:
    dataport = TMKMPC_DataL(realnum);
    port = TMKMPC_StateL(realnum);
    fallthrough;
    // fall-through
  case __TMK400:
  case __RTMK400:
#ifndef DrvIO
    state = inpw_d(realnum, port);
#else
    state = 0; // force putblk
    inptrwu_weq(port, (1<<11), (1<<11), 0, 10000, (void*)0, &state);
#endif //ndef DrvIO
    break;
#ifdef MRTX
  case __MRTX:
#ifndef DrvIO
    data = *(buf++); // prefetch
    statea = (__hm400Page[num]<<1) | 0x0800; // active const
    limit = 16; // abstract loop limit for broken hardware
    state = inpw(port);
    do
    {
      statex = state;
      state = inpw(port);
    }
    while (statex != state && --limit != 0);
    if ((state & 0xB800) != statea)
    {
      outpw(dataport, data); // early 1st write
      --len;
      state = 0;
    }
//    if ((state & 0x8000) == 0)
//    {
//      if (((state >> 1) & 0x1800) != __hm400Page[num])
//        state = 0;
//    }
#else
    state = 0; // force getblk
    inptrwu_weq(port, 0xB800, 0x0800 | (__hm400Page[num]<<1), 1, 10000, (void*)0, &state);
#endif //ndef DrvIO
    break;
#endif
  case __TMKX:
#ifndef DrvIO
    data = *(buf++); // prefetch
    limit = 16; // abstract loop limit for broken hardware
    state = inpw(port);
    do
    {
      statex = state;
      state = inpw(port);
    }
    while (statex != state && --limit != 0);
    if ((state & 0x0800) == 0)
    {
      outpw(dataport, data); // early 1st write
      --len;
    }
#else
    state = 0; // force putblk
    inptrwu_weq(port, (1<<11), (1<<11), 1, 10000, (void*)0, &state);
#endif //ndef DrvIO
    break;
  case __TA:
#ifndef DrvIO
    data = *(buf++); // prefetch
    statea = (DrvRtGetBaseTA(realnum) & 0x03FF) | 0x2000; // active const
    state = inpw(TA_BASE(realnum));
    if ((state & 0xA3FF) != statea)
    {
      outpw(dataport, data); // early 1st write
      --len;
      state = 0;
    }
    else
      state = 0x0800;
//    if (((state & 0x8000) == 0) && ((state & 0x03FF) == (DrvRtGetBaseTA(realnum) & 0x03FF)))
//    if ((state & 0x83FF) == (DrvRtGetBaseTA(realnum) & 0x03FF))
//    if ((state & 0x83FF) == (statex & 0x03FF))
//      state >>= 2;
//    else
//      state = 0;
#else
    state = 0; // force putblk
    inptrwu_weq(port, 0xA3FF, 0x2000 | (DrvRtGetBaseTA(realnum) & 0x03FF), 0, 10000, (void*)0, &state);
#endif //ndef DrvIO
    break;
#ifdef MRTA
  case __MRTA:
#ifndef DrvIO
    data = *(buf++); // prefetch
    statea = (DrvRtGetBaseMRTA(num) & 0x0FFF) | 0x2000; // active const
    state = (__rtPagePC[num] <=  __rtMaxPage[num]) ? 0 : 0xF800;
    outpw(TA_LCW(realnum), (state | __rtControls1[num]) & 0xF800); // no matter if TA_BRCST_MODE here
    state = inpw(TA_BASE(realnum));
    if ((state & 0xAFFF) != statea)
    {
      outpw(dataport, data); // early 1st write
      --len;
      state = 0;
    }
    else
      state = 0x0800;
//    if (((state & 0x8000) == 0) && ((state & 0x0FFF) == (DrvRtGetBaseMRTA(num) & 0x0FFF)))
//    if ((state & 0x8FFF) == (DrvRtGetBaseMRTA(num) & 0x0FFF))
//    if ((state & 0x8FFF) == (statex & 0x0FFF))
//      state >>= 2;
//    else
//      state = 0;
#else
    state = 0; // force putblk
    inptrwu_weq(port, 0xAFFF, 0x2000 | (DrvRtGetBaseMRTA(num) & 0x0FFF), 0, 10000, (void*)0, &state);
#endif //ndef DrvIO
    break;
#endif
  }
  state = (state >> 11) & 1;
  if (!state && len)
  {
    switch (type)
    {
    case __TMK400:
    case __RTMK400:
      REP_OUTSW_D_IRQ(dataport);
      break;
    case __TMKMPC:
      REP_OUTSWB_D_IRQ(dataport);
      break;
    case __TMKX:
#ifdef MRTX
    case __MRTX:
#endif
    case __TA:
#ifdef MRTA
    case __MRTA:
#endif
      REP_OUTSW_IRQ(dataport);
      break;
    }
  }
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    if (!save_ramiw)
      break;
//    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    if (!save_ramiw)
      break;
//    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
  case __TA:
#ifdef MRTA
  case __MRTA:
#endif
    if (!save_ramiw)
      break;
//      __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    break;
  }
//  if (!save_ramiw)
//    __tmkRAMInWork[realnum] = 0;
  REST_IRQ();
  DrvIOEpi;
#ifdef DrvIO
  // process state from inptrwu_weq after DrvIOEpi
  // normally state from inptrwu_weq should be always nonbusy here
  switch (type)
  {
  case __TMKMPC:
  case __TMK400:
  case __RTMK400:
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
    state = (state >> 11) & 1;
    break;
  case __TA:
//    if (((state & 0x8000) == 0) && ((state & 0x03FF) == (DrvRtGetBaseTA(realnum) & 0x03FF)))
    if ((state & 0x83FF) == (DrvRtGetBaseTA(realnum) & 0x03FF))
      state = (state >> 13) & 1;
    else
      state = 0;
    break;
#ifdef MRTA
  case __MRTA:
//    if (((state & 0x8000) == 0) && ((state & 0x0FFF) == (DrvRtGetBaseMRTA(__tmkNumber) & 0x0FFF)))
    if ((state & 0x8FFF) == (DrvRtGetBaseMRTA(num) & 0x0FFF))
      state = (state >> 13) & 1;
    else
      state = 0;
    break;
#endif
  }
#endif //def DrvIO
  return state;
}
#endif //def USE_RTIFNB

#ifdef MRTA
void FARFN __rtputblkmrta(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtPos, VOID FARDT *pcBufAddr, U16 Len)
{
  int num;
  int register realnum;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  U16 FARDT *buf;
  unsigned save_rama, save_ramiw;
  IRQ_FLAGS;

  CLRtmkError;
  num = __tmkNumber;
  pos = rtPos;
  buf = (U16 FARDT*)pcBufAddr;
  len = Len;
  if (len == 0)
    return;
  pos += __rtSubAddr[num] << 1;
  pos ^= __hm400Page[num] << 1;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
//  case __MRTA:
  GET_DIS_IRQ_SMP();
  outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
//  REST_IRQ_SMP();
//  case __TA:
//  GET_DIS_IRQ_SMP();
  __tmkRAMAddr[realnum] = pos;
  outpw(TMK_Addr(realnum), pos);
  REST_IRQ_SMP();
  REP_OUTSW(TMK_Data(realnum));
  if (save_ramiw)
  {
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
  }
  else
    __tmkRAMInWork[realnum] = 0;
  return;
}

void FARFN __rtputblkmrtairq(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtPos, VOID FARDT *pcBufAddr, U16 Len)
{
  int num;
  int register realnum;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  U16 FARDT *buf;

  num = __tmkNumber;
  pos = rtPos;
  buf = (U16 FARDT*)pcBufAddr;
  len = Len;
  if (len == 0)
    return;
  pos += __rtSubAddr[num] << 1;
  pos ^= __hm400Page[num] << 1;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
//  case __MRTA:
  outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
//  case __TA:
  outpw(TMK_Addr(realnum), pos);
  REP_OUTSW_IRQ(TMK_Data(realnum));
  return;
}
#endif //def MRTA

U16 FARFN rtgetw(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtPos)
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned pos;
  unsigned data=0;
  unsigned save_rama, save_ramiw;

  DrvIOPro(2, 0, 0, 0, 1, 0);
  CLRtmkError;
  num = __tmkNumber;
  pos = rtPos;
  CHECK_RT_SUBPOS(pos);
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#if NRT > 0
  if (__rtPagePC[num] == (__rtMaxPage[num] + 2)) // brc sw page
  {
    pos += __rtSubAddr[num] & 0x03E0; // clear T/R
    data = __awBrcRxBuf[realnum][pos];
    DrvIOStop;
    return data;
  }
#endif
  pos += __rtSubAddr[num];
  pos ^= __hm400Page[num];
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    data = inpw_d(realnum, TMK_Data(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    data = inpw_d(realnum, TMKMPC_DataL(realnum));
//    data = inpwb_d(TMKMPC_DataL(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
#ifdef MRTA
  case __MRTA:
    GET_DIS_IRQ_SMP();
    outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
    REST_IRQ_SMP();
    // fall-through
#endif
    fallthrough;
    // fall-through
  case __TA:
    pos = ((pos & 0xFFE0) << 1) | (pos & 0x1F);
    fallthrough;
    // fall-through
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    inptrwu(TMK_Data(realnum), &data);
    REST_IRQ_SMP();
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  DrvIOEpi;
  return data;
}

void FARFN rtputw(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtPos, U16 rtData)
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned pos;
  unsigned data;
  unsigned save_rama, save_ramiw;
  IRQ_FLAGS;

  DrvIOPro(3, 0, 0, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  pos = rtPos;
  CHECK_RT_SUBPOS(pos);
  data = rtData;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#if NRT > 0
  if (__rtPagePC[num] == (__rtMaxPage[num] + 2)) // brc sw page
  {
    pos += __rtSubAddr[num] & 0x03E0; // clear T/R
    __awBrcRxBuf[realnum][pos] = data;
    DrvIOStop;
    return;
  }
#endif
  pos += __rtSubAddr[num];
  pos ^= __hm400Page[num];
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    outpw_d(realnum, TMK_Data(realnum), data);
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    outpwb_d(realnum, TMKMPC_DataH(realnum), data);
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
#ifdef MRTA
  case __MRTA:
    GET_DIS_IRQ_SMP();
    outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
    REST_IRQ_SMP();
    // fall-through
#endif
    fallthrough;
    // fall-through
  case __TA:
    pos = ((pos & 0xFFE0) << 1) | (pos & 0x1F);
    fallthrough;
    // fall-through
  case __TMKX:
#ifdef MRTX
  case __MRTX:
#endif
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    outpw(TMK_Data(realnum), data);
    REST_IRQ_SMP();
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  DrvIOEpi;
  return;
}

void FARFN rtsetanswbits(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 SetControl)
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned port;
  unsigned bits;
  IRQ_FLAGS;

  DrvIOPro(1, 0, 0, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  port = TMK_Mode(realnum);
  GET_MUTEX;
  switch (type)
  {
  case __TMKMPC:
    port = TMKMPC_Mode(realnum);
    fallthrough;
    // fall-through
  case __TMK400:
  case __RTMK400:
    bits = SetControl & RTAnswBitsMask;
    bits |= __rtControls[realnum];
    __rtControls[realnum] = bits;
    if (__tmkMode[num] == RT_MODE)
      outpb_d(realnum, port, bits);
    break;
#ifdef MRTX
  case __MRTX:
    CONVERT_TMKX_SW_BITS(bits, SetControl);
//;                and     ecx, RTAnswBitsMask
    bits <<= 3;
    bits |= __rtControls[num];    //; __rtControls
    __rtControls[num] = bits;
    GET_DIS_IRQ();
                //; simplified ram write because of get_dis_irq
    outpw(TMK_Addr(realnum), __hm400Page[num]);
    outpw(TMK_Data(realnum), bits);
    if (__tmkRAMInWork[realnum])
      outpw(TMK_Addr(realnum), __tmkRAMAddr[realnum]);
    REST_IRQ();
    break;
#endif
  case __TMKX:
    CONVERT_TMKX_SW_BITS(bits, SetControl);
//;                and     ecx, RTAnswBitsMask
    bits <<= 3;
    bits |= __rtControls[realnum];
    __rtControls[realnum] = bits;
    if (__tmkMode[num] == RT_MODE)
      outpw(port, bits);
    break;
  case __TA:
    CONVERT_TA_SW_BITS(bits, SetControl);
    bits |= __rtControls1[realnum];
    __rtControls1[realnum] = bits;
    if ((U08)__tmkMode[num] == (U08)RT_MODE)
      outpw(TA_MODE2(realnum), bits);
    break;
//    H_SetMask(TA_MODE2(realnum), bits, 0);
//    return;
#ifdef MRTA
  case __MRTA:
    CONVERT_TA_SW_BITS(bits, SetControl);
    bits |= __rtControls1[num];
    __rtControls1[num] = bits;
    outpw(MRTA_SW(realnum), bits);
    break;
#endif
  }
  REST_MUTEX;
  DrvIOEpi;
  return;
}

void FARFN rtclranswbits(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 ClrControl)
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned port;
  unsigned bits;
  IRQ_FLAGS;

  DrvIOPro(1, 0, 0, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  port = TMK_Mode(realnum);
  GET_MUTEX;
  switch (type)
  {
  case __TMKMPC:
    port = TMKMPC_Mode(realnum);
    fallthrough;
    // fall-through
  case __TMK400:
  case __RTMK400:
    bits = ~(ClrControl & RTAnswBitsMask);
    bits &= __rtControls[realnum];
    __rtControls[realnum] = bits;
    if (__tmkMode[num] == RT_MODE)
      outpb_d(realnum, port, bits);
    break;
#ifdef MRTX
  case __MRTX:
    CONVERT_TMKX_SW_BITS(bits, ClrControl);
//;                and     ecx, RTAnswBitsMask
    bits = ~(bits << 3);
    bits &= __rtControls[num];    //; __rtControls
    __rtControls[num] = bits;
    GET_DIS_IRQ();
                //; simplified ram write because of get_dis_irq
    outpw(TMK_Addr(realnum), __hm400Page[num]);
    outpw(TMK_Data(realnum), bits);
    if (__tmkRAMInWork[realnum])
      outpw(TMK_Addr(realnum), __tmkRAMAddr[realnum]);
    REST_IRQ();
    break;
#endif
  case __TMKX:
    CONVERT_TMKX_SW_BITS(bits, ClrControl);
//;                and     ecx, RTAnswBitsMask
    bits = ~(bits << 3);
    bits &= __rtControls[realnum];
    __rtControls[realnum] = bits;
    if (__tmkMode[num] == RT_MODE)
      outpw(port, bits);
    break;
  case __TA:
    CONVERT_TA_SW_BITS(bits, ClrControl);
    bits = ~bits;
    bits &= __rtControls1[realnum];
    __rtControls1[realnum] = bits;
    if ((U08)__tmkMode[num] == (U08)RT_MODE)
      outpw(TA_MODE2(realnum), bits);
    break;
//    L_SetMask(TA_MODE2(realnum), bits, 0);
//    return;
#ifdef MRTA
  case __MRTA:
    CONVERT_TA_SW_BITS(bits, ClrControl);
    bits = ~bits;
    bits &= __rtControls1[num];
    __rtControls1[num] = bits;
    outpw(MRTA_SW(realnum), bits);
    break;
#endif
  }
  REST_MUTEX;
  DrvIOEpi;
  return;
}

U16 FARFN rtgetanswbits(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
#if NRT > 0
  int realnum;
#endif
  unsigned type;
  LOCAL_PORT
  unsigned bits=0, bitst;

  DrvIOPro(0, 0, 0, 0, 1, 0);
  CLRtmkError;
  num = __tmkNumber; //???
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
#if NRT > 0
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#else
  SET_LOCAL_PORT(num);
#endif
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
  case __TMKMPC:
    bits = __rtControls[num];
    break;
#ifdef MRTX
  case __MRTX:
#endif
  case __TMKX:
    bitst = __rtControls[num] >> 3;
    CONVERT_TMKX_SW_BITS(bits, bitst);
    break;
  case __TA:
    bitst = inpw(TA_MODE2(num));
//    bits = 0;
    if (bitst & SREQ_MASK)
      bits |= SREQ;
    if (bitst & BUSY_MASK)
      bits |= BUSY;
    if (bitst & SSFL_MASK)
      bits |= SSFL;
    if (bitst & RTFL_MASK)
      bits |= RTFL;
    if (bitst & DNBA_MASK)
      bits |= DNBA;
    break;
#ifdef MRTA
  case __MRTA:
//!!! bitst = __rtControls1[num];
    GET_DIS_IRQ();
    outpw(TA_LCW(realnum), (__rtControls1[num] & 0xF800));
    bitst = inpw(MRTA_SW(realnum));
    REST_IRQ();
//    bits = 0;
    if (bitst & SREQ_MASK)
      bits |= SREQ;
    if (bitst & BUSY_MASK)
      bits |= BUSY;
    if (bitst & SSFL_MASK)
      bits |= SSFL;
    if (bitst & RTFL_MASK)
      bits |= RTFL;
    if (bitst & DNBA_MASK)
      bits |= DNBA;
    break;
#endif
  }
  bits &= RTAnswBitsMask;
  DrvIOEpi;
  return bits;
}

int FARFN rtdefirqmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtIrqModeBits)
{
  int register num;
#if NRT > 0
  int realnum;
#endif
  unsigned type;
  LOCAL_PORT
  unsigned bits;

  DrvIOPro(1, 0, 0, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
#if NRT > 0
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#else
  SET_LOCAL_PORT(num);
#endif
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(RT_BAD_FUNC);
#ifdef MRTX
  case __MRTX:
    bits = rtIrqModeBits & 0x2000;
    GET_MUTEX;
    bits |= __rtControls[realnum] & 0xDFFF; //; m__rtControls
    __rtControls[realnum] = bits;
    bits |= __mrtCtrl0[realnum] | __mrtMask0[realnum];
    if (__tmkMode[realnum] == MRT_MODE)
      outpw(TMK_Mode(realnum), bits);
    REST_MUTEX;
    break;
#endif
  case __TMKX:
    bits = rtIrqModeBits & 0xE004;
    GET_DIS_IRQ(); // because IRQ may be enabled via reg before tmkIrqOff clear
    bits |= __rtControls[num] & 0x1FFB;
    __rtControls[num] = bits;
    if (__tmkMode[num] == RT_MODE)
    {
      outpw(TMK_Mode(num), bits);
      __tmkIrqOff[num] = (TMKIRQOFF_T)(bits >> 15) & 1;
      DrvIOSetPar(IOPAR_TMKIRQOFF, __tmkIrqOff[num]);
    }
    REST_IRQ();
    break;
  case __TA:
    bits = 0;
    if (!(rtIrqModeBits & TMK_IRQ_OFF))
      bits |= TA_IRQ_EN;
    if (rtIrqModeBits & RT_DATA_BL)
      bits |= TA_RT_DATA_INT_BLK;
    GET_DIS_IRQ(); // because IRQ may be enabled via reg before tmkIrqOff clear
    bits |= __rtControls[num] & ~(TA_IRQ_EN | TA_RT_DATA_INT_BLK);
    __rtControls[num] = bits;
    if (__tmkModeMtRt[num])
      __bcControls[num] = bits;
    if ((U08)__tmkMode[num] == (U08)RT_MODE)
    {
      outpw(TA_MODE1(num), bits);
      __tmkIrqOff[num] = ((TMKIRQOFF_T)(bits >> 10) ^ 1) & 1;
      DrvIOSetPar(IOPAR_TMKIRQOFF, __tmkIrqOff[num]);
    }
    REST_IRQ();
    break;
#ifdef MRTA
  case __MRTA:
    bits = 0;
    if (!(rtIrqModeBits & TMK_IRQ_OFF))
      bits |= TA_IRQ_EN;
    if (rtIrqModeBits & RT_DATA_BL)
      bits |= TA_RT_DATA_INT_BLK;
    GET_DIS_IRQ(); // because IRQ may be enabled via reg before tmkIrqOff clear
    bits |= __rtControls[realnum] & ~(TA_IRQ_EN | TA_RT_DATA_INT_BLK);
    __rtControls[realnum] = bits;
    if (__tmkMode[realnum] == MRT_MODE)
    {
      outpw(TA_MODE1(realnum), bits);
      __tmkIrqOff[realnum] = ((TMKIRQOFF_T)(bits >> 10) ^ 1) & 1;
      DrvIOSetPar(IOPAR_TMKIRQOFF, __tmkIrqOff[realnum]);
    }
    REST_IRQ();
    break;
#endif
  }
  DrvIOEpi;
  return 0;
}

U16 FARFN rtgetirqmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
#if NRT > 0
  int realnum;
#endif
  unsigned type;
  LOCAL_PORT
  unsigned bits=0;
  unsigned bitst;

  DrvIOPro(0, 0, 0, 0, 1, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
#if NRT > 0
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#else
  SET_LOCAL_PORT(num);
#endif
  switch (type)
  {
  default:
    USER_ERROR(RT_BAD_FUNC);
    DrvIOStop;
    return 0;
#ifdef MRTX
  case __MRTX:
    bits = __rtControls[realnum] & 0x2000;    //; m__rtControls
    break;
#endif
  case __TMKX:
    bits = __rtControls[num] & 0xE004;
    break;
  case __TA:
    bitst = inpw(TA_MODE1(num));
    bits = RT_GENER1_BL|RT_GENER2_BL;
    if (!(bitst & TA_IRQ_EN))
      bits |= TMK_IRQ_OFF;
    if (bitst & TA_RT_DATA_INT_BLK)
      bits |= RT_DATA_BL;
    break;
#ifdef MRTA
  case __MRTA:
    bitst = inpw(TA_MODE1(realnum));
    bits = RT_GENER1_BL|RT_GENER2_BL;
    if (!(bitst & TA_IRQ_EN)) // if __tmkIrqOff
      bits |= TMK_IRQ_OFF;
    if (bitst & TA_RT_DATA_INT_BLK)
      bits |= RT_DATA_BL;
    break;
#endif
  }
  DrvIOEpi;
  return bits;
}

void DrvFlagModeTA(int REGPAR num, unsigned m)
// m should be either DRV_FLAG_MODE (aka DRV_FLAG_MODE_SW) or 0
{
#ifdef MRTA
  unsigned type;
  LOCAL_PORT
#endif
  int register realnum;
  unsigned tablea;
  unsigned sa;
  unsigned i;

#ifdef MRTA
  type = __tmkDrvType[num];
  if (type == __MRTA)
  {
    realnum = GET_RealNumOf(num);
    tablea = __rtAddress[num];
  }
  else
#endif
  {
    realnum = num;
    tablea = AdrTab;
  }
#ifdef MRTA
  SET_LOCAL_PORT(realnum);
#endif
  if (((__FLAG_MODE[num] & DRV_FLAG_MODE_HW) << 1) != m)
  {
    if (m)
    {
      if (__rtMode[num] & 0x0800)
      {
  // disable rtlock if switch flag mode on
        sa = (__rtMode[num] >> 5) & 0x3F;
        GET_DIS_IRQ_SMP();
#ifdef MRTA
        if (type == __MRTA)
          outpw(MRTA_ADDR2(realnum), 0);
#endif
        DrvPeekAndOrPokeTA(realnum, tablea, sa, 0x7FFF, 0);
        REST_IRQ_SMP();
      }
      if (__FLAG_MODE[num] & DRV_FLAG_UPDATE)
      {
        __FLAG_MODE[num] |= DRV_FLAG_MODE_HW; // required by rtputflag
        rtputflags(TMKNUM__(num) __BC_RT_FLAG[num], RT_RECEIVE, 1, 30);
        rtputflags(TMKNUM__(num) __RT_BC_FLAG[num], RT_TRANSMIT, 1, 30);
        __FLAG_MODE[num] &= ~DRV_FLAG_UPDATE;
      }
    }
    else
    {
      rtgetflags(TMKNUM__(num) __BC_RT_FLAG[num], RT_RECEIVE, 1, 30);
      rtgetflags(TMKNUM__(num) __RT_BC_FLAG[num], RT_TRANSMIT, 1, 30);
      __FLAG_MODE[num] |= DRV_FLAG_UPDATE;

#ifdef MRTA
      if (type == __MRTA)
      {
        GET_DIS_IRQ_SMP();
        outpw(MRTA_ADDR2(realnum), 0);
        REST_IRQ_SMP();
      }
#endif
      for (i = 1; i < 31; ++i)
      {
#ifdef USE_TMK_WRAP_SA
        if (__tmkWrapSA)
        {
          if (i == __tmkWrapSA)
            continue;
        }
#endif
        sa = i;
        GET_DIS_IRQ_SMP();
        DrvPeekAndOrPokeTA(realnum, tablea, sa, 0x7FFF, 0);
        sa |= 0x0020;
        DrvPeekAndOrPokeTA(realnum, tablea, sa, 0x7FFF, 0);
        REST_IRQ_SMP();
      }

      if (__rtMode[num] & 0x0800)
      {
  // enable rtlock if switch flag mode off
        sa = (__rtMode[num] >> 5) & 0x3F;
        GET_DIS_IRQ_SMP();
//#ifdef MRTA
//        if (type == __MRTA)
//          outpw(MRTA_ADDR2(realnum), 0);
//#endif
        DrvPeekAndOrPokeTA(realnum, tablea, sa, 0xFFFF, 0x8000);
        REST_IRQ_SMP();
      }
    }

#ifdef MRTA
    if (type == __MRTA)
    {
      GET_DIS_IRQ_SMP();
      outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
      REST_IRQ_SMP();
    }
#endif
    for (i = 1; i < 31; ++i)
    {
#ifdef USE_TMK_WRAP_SA
      if (__tmkWrapSA)
      {
        if (i == __tmkWrapSA)
          continue;
      }
#endif
#ifdef MRTA
      if (type == __MRTA)
        sa = (__hm400Page[num] >> 5) | i;
      else
#endif
        sa = i;
      if (m)
      {
        GET_DIS_IRQ_SMP();
        DrvPeekAndOrPokeTA(realnum, sa, 63, 0xFFFF, 0x8000);
        sa |= 0x0020;
        DrvPeekAndOrPokeTA(realnum, sa, 63, 0xFFFF, 0x8000);
        REST_IRQ_SMP();
      }
      else
      {
        GET_DIS_IRQ_SMP();
        DrvPeekAndOrPokeTA(realnum, sa, 63, 0x7FFF, 0);
        sa |= 0x0020;
        DrvPeekAndOrPokeTA(realnum, sa, 63, 0x7FFF, 0);
        REST_IRQ_SMP();
      }
    }

    __FLAG_MODE[num] = (__FLAG_MODE[num] & ~DRV_FLAG_MODE_HW) | (m >> 1);
  }
  __FLAG_MODE[num] = (__FLAG_MODE[num] & ~DRV_FLAG_MODE_SW) | m;
}

int FARFN rtdefmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 __rtModeBits)
{
  int register num;
#if NRT > 0
  int realnum;
#endif
  unsigned type;
  LOCAL_PORT
  unsigned bits, bitst;

  DrvIOProV;
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  bitst = __rtModeBits;
  bits = 0;
#if NRT > 0
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#else
  SET_LOCAL_PORT(num);
#endif
  switch (type)
  {
  case __TMK400:
  case __TMKMPC:
    DrvIOStop;
    return USER_ERROR(RT_BAD_FUNC);
  case __RTMK400:
    if (bitst & DRV_HBIT_MODE)
      bits |= RTMK400_HBIT_MODE;
    if (bitst & DRV_FLAG_MODE)
      bits |= RTMK400_FLAG_MODE;
    if (bitst & DRV_BRCST_MODE)
      bits |= RTMK400_BRCST_MODE;
//    GET_MUTEX; // use cli/sti in main DOS program if needed
    __rtControls1[num] = bits;
    bits &= __rtBRCMask[num];
    bits |= __rtAddress[num];
    bits |= __rtDisableMask[num];
    outpb_d(num, R400_AT(num), bits);
//    REST_MUTEX;
    break;
#ifdef MRTX
  case __MRTX:
    if (bitst & DRV_FLAG_MODE)
      bits |= MRTX_FLAG_MODE;
    if (bitst & DRV_BRCST_MODE)
      bits |= MRTX_BRCST_MODE;
    GET_MUTEX;
    bits |= __rtControls1[realnum] & 0x007F;
    __rtControls1[realnum] = bits;
    REST_MUTEX;
    bits &= __rtBRCMask[realnum];
//;   num = __tmkNumber;
    GET_DIS_IRQ_SMP();
    if (__tmkMode[realnum] == MRT_MODE)
    {
      outpw(TMK_Ctrl(realnum), bits);
      outpw(TMK_Ctrl(realnum), bits); // PCI2
    }
    REST_IRQ_SMP();
    break;
#endif
  case __TMKX:
    if (bitst & DRV_FLAG_MODE)
      bits |= TMKX_FLAG_MODE;
    if (bitst & DRV_BRCST_MODE)
      bits |= TMKX_BRCST_MODE;
    GET_MUTEX;
    bits |= __rtControls1[num] & 0xF87F;
    __rtControls1[num] = bits;
    REST_MUTEX;
    bits &= __rtBRCMask[num];
    bits |= __rtDisableMask[num];
    GET_DIS_IRQ_SMP();
    if (__tmkMode[num] == RT_MODE)
    {
      outpw(TMK_Ctrl(num), bits);
      outpw(TMK_Ctrl(num), bits); // PCI2
    }
    REST_IRQ_SMP();
    break;
  case __TA:
    bits = (bitst & (DRV_CBSNL_MODE | DRV_CBSND_MODE | DRV_CNOBS_MODE)) >> 4;
    bits ^= TA_CBSD_MODE;
#ifdef USE_TMK_CBSND
    if (__tmkCBSND)
      bits &= ~TA_CBSD_MODE;
#endif
//    GET_MUTEX; // use cli/sti in main DOS program if needed
    bits |= __rtControls[num] & ~(TA_CBSNL_MODE | TA_CBSD_MODE | TA_CNOBS_MODE);
    __rtControls[num] = bits;
    if (__tmkModeMtRt[num])
      __bcControls[num] = bits;
    GET_DIS_IRQ_SMP();
    if ((U08)__tmkMode[num] == (U08)RT_MODE)
      outpw(TA_MODE1(num), bits);
    REST_IRQ_SMP();
    bits = bitst & (DRV_NOERR_MODE | DRV_RSREQ_MODE);
    if (bitst & DRV_HBIT_MODE)
      bits |= TA_HBIT_MODE;
    if (bitst & DRV_BRCST_MODE)
      bits |= TA_BRCST_MODE;
    bits |= __rtControls1[num] & 0xF96F;
    __rtControls1[num] = bits;
//    bits &= __rtBRCMask[num];
//    bits |= __rtDisableMask[num];
    if ((U08)__tmkMode[num] == (U08)RT_MODE)
    {
      outpw(TA_MODE2(num), bits);

//      DrvFlagModeTA(num, (bitst & DRV_FLAG_MODE) ? 1 : 0);
      DrvFlagModeTA(num, bitst & DRV_FLAG_MODE);
    }
    else
    {
//      __FLAG_MODE_ONSW[num] = (bitst & DRV_FLAG_MODE) ? 1 : 0;
      __FLAG_MODE[num] = (__FLAG_MODE[num] & ~DRV_FLAG_MODE_SW) | (bitst & DRV_FLAG_MODE);
    }
//    REST_MUTEX;
    break;
#ifdef MRTA
  case __MRTA:
    bits = (bitst & (DRV_CBSNL_MODE | DRV_CBSND_MODE | DRV_CNOBS_MODE)) >> 4;
    bits ^= TA_CBSD_MODE;
#ifdef USE_TMK_CBSND
    if (__tmkCBSND)
      bits &= ~TA_CBSD_MODE;
#endif
//    GET_MUTEX; // use cli/sti in main DOS program if needed
    bits |= __rtControls[realnum] & ~(TA_CBSNL_MODE | TA_CBSD_MODE | TA_CNOBS_MODE);
    __rtControls[realnum] = bits;
    GET_DIS_IRQ_SMP();
    if (__tmkMode[realnum] == MRT_MODE)
      outpw(TA_MODE1(realnum), bits);
    REST_IRQ_SMP();
    bits = bitst & (DRV_NOERR_MODE | DRV_RSREQ_MODE);
    if (bitst & DRV_HBIT_MODE)
      bits |= TA_HBIT_MODE;
//    if (bitst & DRV_BRCST_MODE)
//      bits |= TA_BRCST_MODE;
//    bits |= __rtControls1[realnum] & 0xF96F;
    __rtControls1[realnum] = bits;
    if (__tmkMode[realnum] == MRT_MODE)
      outpw(TA_MODE2(realnum), bits);
    bits = (bitst & DRV_BRCST_MODE) ? TA_BRCST_MODE : 0;
    bits |= __rtControls1[num] & 0xF9EF;
    __rtControls1[num] = bits;
    if (__rtAddress[num] < 31)
    {
      if (__rtControls1[num] & TA_BRCST_MODE)
        __dmrtBrc[realnum] |= 1L << __rtAddress[num];
      else
        __dmrtBrc[realnum] &= ~(1L << __rtAddress[num]);
    }
    if (__tmkMode[num] == RT_MODE)
    {
//        outpw(MRTA_SW(realnum), 0xF800 | __rtControls1[realnum] | DrvMrtaBrcRtOn(realnum));
        outpw(MRTA_SW(realnum), 0xF800 | DrvMrtaBrcRtOn(realnum));

      if (__rtAddress[num] < 31)
      {
//        DrvFlagModeTA(num, (bitst & DRV_FLAG_MODE) ? 1 : 0);
        DrvFlagModeTA(num, bitst & DRV_FLAG_MODE);
      }
      else
      {
//        __FLAG_MODE_ONSW[num] = (bitst & DRV_FLAG_MODE) ? 1 : 0;
        __FLAG_MODE[num] = (__FLAG_MODE[num] & ~DRV_FLAG_MODE_SW) | (bitst & DRV_FLAG_MODE);
      }
    }
    else
    {
//      __FLAG_MODE_ONSW[num] = (bitst & DRV_FLAG_MODE) ? 1 : 0;
      __FLAG_MODE[num] = (__FLAG_MODE[num] & ~DRV_FLAG_MODE_SW) | (bitst & DRV_FLAG_MODE);
    }
//    REST_MUTEX;
    break;
#endif
  }
  DrvIOEpi;
  return 0;
}

U16 FARFN rtgetmode(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
#if NRT > 0
  int realnum;
#endif
  unsigned type;
  unsigned bits=0, bitst;

//  DrvIOPro(0, 0, 0, 0, 2, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TMK400:
  case __TMKMPC:
    USER_ERROR(RT_BAD_FUNC);
    return 0;
  case __RTMK400:
    bitst = __rtControls1[num];
//    bits = 0;
    if (bitst & RTMK400_HBIT_MODE)
      bits |= DRV_HBIT_MODE;
    if (bitst & RTMK400_FLAG_MODE)
      bits |= DRV_FLAG_MODE;
    if (bitst & RTMK400_BRCST_MODE)
      bits |= DRV_BRCST_MODE;
    break;
#ifdef MRTX
  case __MRTX:
    realnum = GET_RealNum;
    bitst = __rtControls1[realnum];
//    bits = 0;
    if (bitst & MRTX_FLAG_MODE)
      bits |= DRV_FLAG_MODE;
    if (bitst & MRTX_BRCST_MODE)
      bits |= DRV_BRCST_MODE;
    break;
#endif
  case __TMKX:
    bitst = __rtControls1[num];
//    bits = 0;
    if (bitst & TMKX_FLAG_MODE)
      bits |= DRV_FLAG_MODE;
    if (bitst & TMKX_BRCST_MODE)
      bits |= DRV_BRCST_MODE;
    break;
  case __TA:
//    GET_MUTEX; // use cli/sti in main DOS program if needed
    bitst = __rtControls1[num]; //inpw(TA_MODE2(num));
    bits = bitst & (TA_NOERR_MODE | TA_RSREQ_MODE);
    if (bitst & TA_HBIT_MODE)
      bits |= DRV_HBIT_MODE;
    if (bitst & TA_BRCST_MODE)
      bits |= DRV_BRCST_MODE;
//    if (__FLAG_MODE_ON[num])
//      bits |= DRV_FLAG_MODE;
    bits |= (unsigned)__FLAG_MODE[num] & DRV_FLAG_MODE;
    bitst = __rtControls[num]; //inpw(TA_MODE1(num));
//    REST_MUTEX;
    bitst ^= TA_CBSD_MODE;
    bits |= (bitst & (TA_CBSNL_MODE | TA_CBSD_MODE | TA_CNOBS_MODE)) << 4;
    break;
#ifdef MRTA
  case __MRTA:
    realnum = GET_RealNum;
//    GET_MUTEX; // use cli/sti in main DOS program if needed
    bitst = __rtControls1[realnum];
    bits = bitst & (TA_NOERR_MODE | TA_RSREQ_MODE);
    if (bitst & TA_HBIT_MODE)
      bits |= DRV_HBIT_MODE;
//    if (bitst & TA_BRCST_MODE)
    if (__rtControls1[num] & TA_BRCST_MODE)
      bits |= DRV_BRCST_MODE;
//    if (__FLAG_MODE_ON[num])
//      bits |= DRV_FLAG_MODE;
    bits |= (unsigned)__FLAG_MODE[num] & DRV_FLAG_MODE;
    bitst = __rtControls[realnum];
//    REST_MUTEX;
    bitst ^= TA_CBSD_MODE;
    bits |= (bitst & (TA_CBSNL_MODE | TA_CBSD_MODE | TA_CNOBS_MODE)) << 4;
    break;
#endif
  }
//  DrvIOEpi;
  return bits;
}

#if NRT > 0
int FARFN mrtdefbrcpage(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 BrcPage)
{
  int register num;
  unsigned type;
  unsigned page;

  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  __rtSABuf[num] = 0;
  switch (type)
  {
  default:
    return USER_ERROR(RT_BAD_FUNC);
#ifdef MRTX
  case __MRTX:
    page = BrcPage;
    if (page != 1) // 1 - sw
      return USER_ERROR(RT_BAD_PAGE);
    __rtPagePC[num] =  __rtMaxPage[num] + BrcPage + 1;
    break;
#endif
#ifdef MRTA
  case __MRTA:
    page = BrcPage;
    if (page > 1) // 0 - hw, 1 - sw
      return USER_ERROR(RT_BAD_PAGE);
    __rtPagePC[num] =  __rtMaxPage[num] + BrcPage + 1;
//    __rtPageBus[num] =   __rtMaxPage[num] + 1;
    __hm400Page2[num] = MRTA_BRC_PAGE >> 4;
    __hm400Page[num] = (U16)(MRTA_BRC_PAGE << 11);
    break;
#endif
  }
  return 0;
}

U16 FARFN mrtgetbrcpage(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;

  CLRtmkError;
  num = __tmkNumber;
  return (__rtPagePC[num] <= __rtMaxPage[num]) ? 0 : __rtPagePC[num] - __rtMaxPage[num] - 1;
}
#endif //NRT

int FARFN rtdefpage(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtPage)
{
  int register num;
#if NRT > 0
  int realnum;
#endif
  unsigned type;
  LOCAL_PORT
  unsigned page;

  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  page = rtPage;
#ifdef MRTX
  if (type == __MRTX)
  {
    if (page > (__rtMaxPage[num] + 2) || page == (__rtMaxPage[num] + 1))
      return USER_ERROR(RT_BAD_PAGE);
  }
  else
#endif
#ifdef MRTA
  if (type == __MRTA)
  {
    if (page > (__rtMaxPage[num] + 2))
      return USER_ERROR(RT_BAD_PAGE);
  }
  else
#endif
  {
    CHECK_RT_PAGE_BX(num, page);
  }
  __rtPagePC[num] = page;
  __rtPageBus[num] = page;
  __rtSABuf[num] = 0;
#if NRT > 0
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#else
  SET_LOCAL_PORT(num);
#endif
  switch (type)
  {
  case __TMKMPC:
#ifdef MRTX
  case __MRTX:
#endif
  case __TA:
    break;
  case __TMK400:
  case __RTMK400:
    page <<= 5;
    outpw_d(num, TMK_Addr(num), page);
    __hm400Page[num] = page << 6;
    break;
  case __TMKX:
    page <<= 11;
    __hm400Page[num] = page;
    page >>= 3;
    GET_MUTEX;
    page |= __rtControls[num] & 0xE0FF;
    __rtControls[num] = page;
    REST_MUTEX;
    outpw(TMK_Mode(num), page);
    break;
#ifdef MRTA
  case __MRTA:
    if (page <= __rtMaxPage[num])
    { //std page 0
      __hm400Page2[num] = __hm400Page0[num] >> 4;
      __hm400Page[num] = __hm400Page0[num] << 11;
    }
    else
    { //brc page 0
      __hm400Page2[num] = MRTA_BRC_PAGE >> 4;
      __hm400Page[num] = (U16)(MRTA_BRC_PAGE << 11);
    }
    break;
#endif
  }
  return 0;
}

U16 FARFN rtgetpage(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;

  CLRtmkError;
  num = __tmkNumber;
  return __rtPagePC[num];
}

U16 FARFN rtgetmaxpage(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;

  CLRtmkError;
  num = __tmkNumber;
  return __rtMaxPage[num];
}

int FARFN rtdefpagepc(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 PagePC)
{
  int register num;
  unsigned type;
  unsigned page;

  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  page = PagePC;
#ifdef MRTA
  if (type == __MRTA)
  {
    if (page > (__rtMaxPage[num] + 1))
      return USER_ERROR(RT_BAD_PAGE);
  }
  else
#endif
  {
    CHECK_RT_PAGE_BX(num, page);
  }
  __rtSABuf[num] = 0;
  switch (type)
  {
  default:
    return USER_ERROR(RT_BAD_FUNC);
  case __TMKX:
    __rtPagePC[num] = page;
    __hm400Page[num] = page << 11;
    break;
#ifdef MRTA
  case __MRTA:
    __rtPagePC[num] = page;
    if (page <= __rtMaxPage[num])
    { //std page 0
      __hm400Page2[num] = __hm400Page0[num] >> 4;
      __hm400Page[num] = __hm400Page0[num] << 11;
    }
    else
    { //brc page 0
      __hm400Page2[num] = MRTA_BRC_PAGE >> 4;
      __hm400Page[num] = (U16)(MRTA_BRC_PAGE << 11);
    }
    break;
#endif
  }
  return 0;
}

int FARFN rtdefpagebus(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 PageBus)
{
  int register num;
  unsigned type;
  LOCAL_PORT
  unsigned page;

  CLRtmkError;
  num = __tmkNumber;
  page = PageBus;
  CHECK_RT_PAGE_BX(num, page);
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    return USER_ERROR(RT_BAD_FUNC);
  case __TMKX:
    SET_LOCAL_PORT(num);
    __rtPageBus[num] = page;
    page <<= 8;
    GET_MUTEX;
    page |= __rtControls[num] & 0xE0FF;
    __rtControls[num] = page;
    REST_MUTEX;
    outpw(TMK_Mode(num), page);
    break;
  }
  return 0;
}

U16 FARFN rtgetpagepc(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;

  CLRtmkError;
  num = __tmkNumber;
  return __rtPagePC[num];
}

U16 FARFN rtgetpagebus(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;

  CLRtmkError;
  num = __tmkNumber;
  return __rtPageBus[num];
}

U16 FARFN rtallocsabuf(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtBufModeSA,
        U16 rtBufSize)
{
  int register num;
  unsigned type;
  unsigned bufmode;
  unsigned bufsize, bufsizem;
  unsigned sa;
#ifdef MRTA
  int realnum;
  int num1;
  unsigned rtaddr1;
#endif
  unsigned res = 0;

  DrvIOProV;
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  bufmode = rtBufModeSA & 0xF000;
  sa = rtBufModeSA & 0x003F;
  bufsize = rtBufSize;
  if (bufsize == 0)
    bufsize = 1;
#ifdef USE_TMK_WRAP_SA
  if (__tmkWrapSA)
  {
    if ((sa & 0x1F) == __tmkWrapSA)
      type = 0; // disable any change through "default:" branch
  }
#endif
  switch (type)
  {
  default:
    if (sa == 0)
      break;
    switch (bufmode)
    {
    case SABUF_GETMODE:
      res = SABUF_OFF;
      break;
    case SABUF_OFF:
      res = 1;
      break;
    }
    break;
#ifdef MRTA
  case __MRTA:
    if (sa == 0)
      break;
    if ((rtaddr1 = __rtAddress[num]) == 0x00FF)
    {
      USER_ERROR(RT_BAD_ADDRESS);
      break;
    }
    realnum = GET_RealNum;
    if (__rtPagePC[num] > __rtMaxPage[num]) // brc page
    {
      num1 = realnum;
      rtaddr1 = 31;
    }
    else
      num1 = num;
    switch (bufmode)
    {
    case SABUF_GETMODE:
      res = __rtSABufMode[num1][sa];
      break;
    case SABUF_OFF:
      switch (bufsize)
      {
      case GET_MAX_ALLOC:
        res = 1;
        break;
      case GET_ALLOC:
        if (__rtSABufMode[num1][sa] == SABUF_OFF)
          res = 1;
        break;
      default:
        __rtSABuf[num] = 0;
        __rtSABufMode[num1][sa] = SABUF_OFF;
        __rtSABufSize[num1][sa] = 1;
        mrtcreatlink(realnum, rtaddr1, sa, sa, 1);
        res = 1;
        break;
      }
      break;
    case SABUF_STD:
      switch (bufsize)
      {
      case GET_MAX_ALLOC:
        res = 1;
        break;
      case GET_ALLOC:
        if (__rtSABufMode[num1][sa] == SABUF_STD)
          res = __rtSABufSize[num1][sa];
        break;
      default:
        __rtSABuf[num] = 0;
        __rtSABufMode[num1][sa] = SABUF_STD;
        if (bufsize > 1)
          bufsize = 1;
        __rtSABufSize[num1][sa] = 1; //bufsize;
        mrtcreatlink(realnum, rtaddr1, sa, sa, 1); //bufsize);
        res = 1; //bufsize;
        break;
      }
      break;
    case SABUF_JOINT:
      sa &= 0x001F;
      switch (bufsize)
      {
      case GET_MAX_ALLOC:
        res = 1;
        break;
      case GET_ALLOC:
        if (__rtSABufMode[num1][sa] == SABUF_JOINT)
          res = __rtSABufSize[num1][sa];
        break;
      default:
        __rtSABuf[num] = 0;
        __rtSABufMode[num1][sa] = SABUF_JOINT;
        __rtSABufMode[num1][sa|0x20] = SABUF_JOINT;
        if (bufsize > 1)
          bufsize = 1;
        __rtSABufSize[num1][sa] = 1; //bufsize;
        __rtSABufSize[num1][sa|0x20] = 1; //bufsize;
        mrtcreatlink(realnum, rtaddr1, sa, sa, 1); //bufsize);
        mrtcreatlink(realnum, rtaddr1, sa|0x20, sa, 1); //bufsize);
        res = 1; //bufsize;
        break;
      }
      break;
    case SABUF_HEAP:
      switch (bufsize)
      {
      case GET_MAX_ALLOC:
        res = __rtHeapAlloc[realnum] - __rtHeapMin[realnum] + 1; // heap plus base 0
        break;
      case GET_ALLOC:
        if (__rtSABufMode[num1][sa] == SABUF_HEAP)
          res = __rtSABufSize[num1][sa];
        break;
      default:
        __rtSABuf[num] = 0;
        __rtSABufMode[num1][sa] = SABUF_HEAP;
//        GET_MUTEX;
        res = __rtHeapAlloc[realnum] - __rtHeapMin[realnum] + 1; // heap plus base 0
        if (bufsize > res)
          bufsize = res;
        __rtHeapAlloc[realnum] -= bufsize - 1;
        __rtSABufExt[num1][sa] = (bufsize > 1) ? __rtHeapAlloc[realnum] : 0;
        __rtSABufSize[num1][sa] = bufsize;
//        REST_MUTEX;
        mrtcreatlink(realnum, rtaddr1, sa, sa, bufsize);
        res = bufsize;
        break;
      }
      break;
    }
    DrvUpdateHeapAllocMRTA(realnum);
    break;
#endif
  case __TA:
    if (sa == 0)
      break;
    switch (bufmode)
    {
    case SABUF_GETMODE:
      res = __rtSABufMode[num][sa];
      break;
    case SABUF_OFF:
      switch (bufsize)
      {
      case GET_MAX_ALLOC:
        res = 1;
        break;
      case GET_ALLOC:
        if (__rtSABufMode[num][sa] == SABUF_OFF)
          res = 1;
        break;
      default:
        __rtSABuf[num] = 0;
        __rtSABufMode[num][sa] = SABUF_OFF;
        __rtSABufSize[num][sa] = 1;
        rtcreatlink(num, sa, sa, 1, 0);
        res = 1;
        break;
      }
      break;
    case SABUF_STD:
      switch (bufsize)
      {
      case GET_MAX_ALLOC:
/*
        if (sa != 0x3F)
          res = 16;
        else
          res = 15; // protect table (assume no pages)
*/
        res = (__rtHeapAlloc[num] - sa - 1) / 64 + 1; // 64 sa per one buf level
        break;
      case GET_ALLOC:
        if (__rtSABufMode[num][sa] == SABUF_STD)
          res = __rtSABufSize[num][sa];
        break;
      default:
        __rtSABuf[num] = 0;
        __rtSABufMode[num][sa] = SABUF_STD;
//        GET_MUTEX;
        res = (__rtHeapAlloc[num] - sa - 1) / 64 + 1; // 64 sa per one buf level
        if (bufsize > res)
          bufsize = res;
        bufsizem = sa + (bufsize - 1) * 64 + 1;
        if (bufsizem > __rtHeapMin[num])
          __rtHeapMin[num] = bufsizem;
        __rtSABufSize[num][sa] = bufsize;
//        REST_MUTEX;
        rtcreatlink(num, sa, sa, bufsize, 0);
        res = bufsize;
        break;
      }
      break;
    case SABUF_JOINT:
      sa &= 0x001F;
      switch (bufsize)
      {
      case GET_MAX_ALLOC:
        res = 1;
        break;
      case GET_ALLOC:
        if (__rtSABufMode[num][sa] == SABUF_JOINT)
          res = __rtSABufSize[num][sa];
        break;
      default:
        __rtSABuf[num] = 0;
        __rtSABufMode[num][sa] = SABUF_JOINT;
        __rtSABufMode[num][sa|0x20] = SABUF_JOINT;
        if (bufsize > 1)
          bufsize = 1;
        __rtSABufSize[num][sa] = 1; //bufsize;
        __rtSABufSize[num][sa|0x20] = 1; //bufsize;
        rtcreatlink(num, sa, sa, 1, 0); //bufsize, 0);
        rtcreatlink(num, sa|0x20, sa, 1, 0); //bufsize, 0);
        res = 1; //bufsize;
        break;
      }
      break;
    case SABUF_HEAP:
      switch (bufsize)
      {
      case GET_MAX_ALLOC:
        res = __rtHeapAlloc[num] - __rtHeapMin[num] + 1; // heap plus base 0
        break;
      case GET_ALLOC:
        if (__rtSABufMode[num][sa] == SABUF_HEAP)
          res = __rtSABufSize[num][sa];
        break;
      default:
        __rtSABuf[num] = 0;
        __rtSABufMode[num][sa] = SABUF_HEAP;
//        GET_MUTEX;
        res = __rtHeapAlloc[num] - __rtHeapMin[num] + 1; // heap plus base 0
        if (bufsize > res)
          bufsize = res;
        __rtHeapAlloc[num] -= bufsize - 1;
        __rtSABufExt[num][sa] = (bufsize > 1) ? __rtHeapAlloc[num] : 0;
        __rtSABufSize[num][sa] = bufsize;
//        REST_MUTEX;
        rtcreatlink(num, sa, sa, bufsize, 0);
        res = bufsize;
        break;
      }
      break;
    }
    DrvUpdateHeapAllocTA(num);
    break;
  }
  DrvIOEpi;
  return (U16)res;
}

int FARFN rtdefbuf(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtBuf)
{
  int register num;
  int num1;
  unsigned type;
  unsigned sa;
  unsigned buf;
#ifdef MRTA
  unsigned page0;
#endif

  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  buf = rtBuf;
  sa = (__rtSubAddr[num] >> 5) & 0x003F;
  num1 = num;
#ifdef MRTA
  if (__rtPagePC[num] > __rtMaxPage[num]) // MRTA brc page
  {
    page0 = MRTA_BRC_PAGE;
    num1 = GET_RealNum;
  }
  else
    page0 = __hm400Page0[num];
#endif
  if (buf >= __rtSABufSize[num1][sa])
    return USER_ERROR(RT_BAD_BUF);
  __rtSABuf[num] = buf;
  switch (type)
  {
  case __TA:
    if (__rtSABufMode[num][sa] == SABUF_HEAP && buf != 0)
      __hm400Page[num] = ((__rtSABufExt[num][sa] + buf - 1) << 5) ^ __rtSubAddr[num];
    else
      __hm400Page[num] = buf << 11;
    break;
#ifdef MRTA
  case __MRTA:
    if (__rtAddress[num] == 0x00FF)
      return USER_ERROR(RT_BAD_ADDRESS);
    if (__rtSABufMode[num1][sa] == SABUF_HEAP && buf != 0)
    {
      __hm400Page2[num] = (__rtSABufExt[num1][sa] + buf - 1) >> 10;
      __hm400Page[num] = ((__rtSABufExt[num1][sa] + buf - 1) << 5) ^ __rtSubAddr[num];
    }
    else
    {
      __hm400Page2[num] = page0 >> 4;
      __hm400Page[num] = page0 << 11;
    }
    break;
#endif
  }
  return 0;
}

U16 FARFN rtgetbuf(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned buf;

  CLRtmkError;
  num = __tmkNumber;
  buf = __rtSABuf[num];
  return (U16)buf;
}

#if defined(DOS) || defined(QNX4INT)
U16 FARFN rtgetbufirq(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned buf;

  CLRtmkError;
  num = __tmkNumber;
  buf = __rtIrqBuf[num];
  return (U16)buf;
}
#endif //def DOS || QNX4INT

/*void FARFN rtdefsabuf(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Dir, U16 SubAddr, U16 Buf)
{
  int register num;
  unsigned type;
  unsigned sa;
  unsigned buf;

  CLRtmkError;
  num = __tmkNumber;
  sa = SubAddr;
  CHECK_RT_SUBADDR(sa);
  sa <<= 5;
  CHECK_RT_DIR(Dir);
  sa |= Dir;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  DrvRtDefSubAddr(num, type, sa);
  DrvRtWMode(num, type, sa);
  return;
}

U16 FARFN rtgetsabuf(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned sa;

  CLRtmkError;
  num = __tmkNumber;
  sa = __rtSubAddr[num];
  return ((sa >> 5) & 0x001F) | (sa & 0x0400);
}
*/

int FARFN rtdeflink(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Link) // Link = Buf | Ctrl ???
{
  int register num;
  unsigned type;
  unsigned sa;
  unsigned buf;
  unsigned buflink;
  unsigned ctrlink;
  unsigned base;
  unsigned link;
#ifdef MRTA
  int realnum;
  int num1;
  LOCAL_PORT
  unsigned page0;
#endif

  DrvIOPro(0, 0, 1, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  buflink = Link & 0x0FFF;
  ctrlink = Link & 0xF000;
  buf = __rtSABuf[num];
  sa = (__rtSubAddr[num] >> 5) & 0x003F;
#ifdef USE_TMK_WRAP_SA
  if (__tmkWrapSA)
  {
    if ((sa & 0x1F) == __tmkWrapSA)
      type = 0; // disable any change through "default:" branch
  }
#endif
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(RT_BAD_FUNC);
#ifdef MRTA
  case __MRTA:
    if (__rtAddress[num] == 0x00FF)
    {
      DrvIOStop;
      return USER_ERROR(RT_BAD_ADDRESS);
    }
    realnum = GET_RealNum;
    SET_LOCAL_PORT(realnum);
    if (__rtPagePC[num] > __rtMaxPage[num]) // MRTA brc page
    {
      page0 = MRTA_BRC_PAGE;
      num1 = realnum;
    }
    else
    {
      page0 = __hm400Page0[num];
      num1 = num;
    }
    if (buflink >= __rtSABufSize[num1][sa])
    {
      DrvIOStop;
      return USER_ERROR(RT_BAD_BUF);
    }
    switch (__rtSABufMode[num1][sa])
    {
    case SABUF_STD:
    case SABUF_JOINT:
      // buf == 0, buflink == 0, page0 <= 0x3F
      base = sa + (page0 << 6);
      link = (sa + (page0 << 6)) | ctrlink;
      GET_DIS_IRQ_SMP();
      outpw(MRTA_ADDR2(realnum), __hm400Page2[num]); // base >> 10
      DrvRtPokeTA(realnum, base, 63, link);
      REST_IRQ_SMP();
      break;
    case SABUF_HEAP:
      if (buf == 0)
        base = sa + (page0 << 6);
      else
        base = __rtSABufExt[num1][sa] + buf - 1;
      if (buflink == 0)
        link = sa + (page0 << 6);
      else
        link = __rtSABufExt[num1][sa] + buflink - 1;
      link |= ctrlink;
      GET_DIS_IRQ_SMP();
      outpw(MRTA_ADDR2(realnum), base >> 10);
      DrvRtPokeTA(realnum, base, 63, link);
      REST_IRQ_SMP();
      break;
    }
    break;
#endif
  case __TA:
    if (buflink >= __rtSABufSize[num][sa])
    {
      DrvIOStop;
      return USER_ERROR(RT_BAD_BUF);
    }
    switch (__rtSABufMode[num][sa])
    {
    case SABUF_STD:
    case SABUF_JOINT:
      base = sa + (buf << 6);
      link = (sa + (buflink << 6)) | ctrlink;
      GET_DIS_IRQ_SMP();
      DrvRtPokeTA(num, base, 63, link);
      REST_IRQ_SMP();
      break;
    case SABUF_HEAP:
      if (buf == 0)
        base = sa;
      else
        base = __rtSABufExt[num][sa] + buf - 1;
      if (buflink == 0)
        link = sa;
      else
        link = __rtSABufExt[num][sa] + buflink - 1;
      link |= ctrlink;
      GET_DIS_IRQ_SMP();
      DrvRtPokeTA(num, base, 63, link);
      REST_IRQ_SMP();
      break;
    }
    break;
  }
  DrvIOEpi;
  return 0;
}

U16 FARFN rtgetlink(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned type;
  unsigned sa;
  unsigned buf;
  unsigned base;
  unsigned link;
#ifdef MRTA
  int realnum;
  int num1;
  LOCAL_PORT
  unsigned page0;
#endif

  DrvIOPro(0, 0, 0, 0, 0, 1);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  base = DrvRtGetBaseTA(num); //could be used for MRTA here as well!!!
  sa = (__rtSubAddr[num] >> 5) & 0x003F;
  buf = 0;
  link = 0;
  switch (type)
  {
  default:
    USER_ERROR(RT_BAD_FUNC);
    link = 0x4000;
    break;
#ifdef MRTA
  case __MRTA:
    if (__rtAddress[num] == 0x00FF)
    {
      USER_ERROR(RT_BAD_ADDRESS);
      break;
    }
    realnum = GET_RealNum;
    SET_LOCAL_PORT(realnum);
    if (__rtPagePC[num] > __rtMaxPage[num]) // MRTA brc page
    {
      page0 = MRTA_BRC_PAGE;
      num1 = realnum;
    }
    else
    {
      page0 = __hm400Page0[num];
      num1 = num;
    }
    GET_DIS_IRQ_SMP();
    outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
    link = DrvRtPeekTA(realnum, base, 63);
    REST_IRQ_SMP();
    base = link & 0x0FFF;
    link &= 0xF000;
    switch (__rtSABufMode[num1][sa])
    {
//    case SABUF_OFF:
//    case SABUF_STD:
//    case SABUF_JOINT:
//      buf = 0;
//      break;
    case SABUF_HEAP:
      if (base != (sa + (page0 << 6)))
        buf = base - __rtSABufExt[num1][sa] + 1;
//      else
//        buf = 0;
      break;
    }
    link |= buf;
    break;
#endif
  case __TA:
    GET_DIS_IRQ_SMP();
    link = DrvRtPeekTA(num, base, 63);
    REST_IRQ_SMP();
    base = link & 0x0FFF;
    link &= 0xF000;
    switch (__rtSABufMode[num][sa])
    {
    case SABUF_OFF:
    case SABUF_STD:
    case SABUF_JOINT:
      buf = base >> 6;
      break;
    case SABUF_HEAP:
      if (base != sa)
        buf = base - __rtSABufExt[num][sa] + 1;
//      else
//        buf = 0;
      break;
    }
    link |= buf;
    break;
  }
  DrvIOEpi;
  return link;
}

int FARFN rtdeflinkbus(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Link) // Link = Buf | Ctrl ???
{
  int register num;
  unsigned type;
  unsigned sa;
  unsigned buflink;
  unsigned ctrlink;
  unsigned link;
  int fJoint = 0;
#ifdef MRTA
  int realnum;
  int num1;
  LOCAL_PORT
  unsigned page0;
  unsigned tablea;
#endif

  DrvIOPro(0, 0, 1, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  buflink = Link & 0x0FFF;
  ctrlink = Link & 0xF000;
  sa = (__rtSubAddr[num] >> 5) & 0x003F;
#ifdef USE_TMK_WRAP_SA
  if (__tmkWrapSA)
  {
    if ((sa & 0x1F) == __tmkWrapSA)
      type = 0; // disable any change through "default:" branch
  }
#endif
  switch (type)
  {
  default:
    DrvIOStop;
    return USER_ERROR(RT_BAD_FUNC);
#ifdef MRTA
  case __MRTA:
    if (__rtAddress[num] == 0x00FF)
    {
      DrvIOStop;
      return USER_ERROR(RT_BAD_ADDRESS);
    }
    realnum = GET_RealNum;
    SET_LOCAL_PORT(realnum);
    if (__rtPagePC[num] > __rtMaxPage[num]) // MRTA brc page
    {
      tablea = 31;
      page0 = MRTA_BRC_PAGE;
      num1 = realnum;
    }
    else
    {
      tablea = __rtAddress[num];
      page0 = __hm400Page0[num];
      num1 = num;
    }
    if (buflink >= __rtSABufSize[num1][sa])
    {
      DrvIOStop;
      return USER_ERROR(RT_BAD_BUF);
    }
    switch (__rtSABufMode[num1][sa])
    {
    case SABUF_JOINT:
      fJoint = 1;
      fallthrough;
      // fall-through
    case SABUF_STD:
      // buflink == 0, page0 <= 0x3F
      link = (sa + (page0 << 6)) | ctrlink;
      GET_DIS_IRQ_SMP();
      outpw(MRTA_ADDR2(realnum), 0);
      DrvRtPokeTA(realnum, tablea, sa, link);
      if (fJoint)
        DrvRtPokeTA(realnum, tablea, sa|0x20, link);
      REST_IRQ_SMP();
      break;
    case SABUF_HEAP:
      if (buflink == 0)
        link = sa + (page0 << 6);
      else
        link = __rtSABufExt[num1][sa] + buflink - 1;
      link |= ctrlink;
      GET_DIS_IRQ_SMP();
      outpw(MRTA_ADDR2(realnum), 0);
      DrvRtPokeTA(realnum, tablea, sa, link);
      REST_IRQ_SMP();
      break;
    }
    break;
#endif
  case __TA:
    if (buflink >= __rtSABufSize[num][sa])
    {
      DrvIOStop;
      return USER_ERROR(RT_BAD_BUF);
    }
    switch (__rtSABufMode[num][sa])
    {
    case SABUF_JOINT:
      fJoint = 1;
      fallthrough;
      // fall-through
    case SABUF_STD:
      link = (sa + (buflink << 6)) | ctrlink;
      GET_DIS_IRQ_SMP();
      DrvRtPokeTA(num, AdrTab, sa, link);
      if (fJoint)
        DrvRtPokeTA(num, AdrTab, sa|0x20, link);
      REST_IRQ_SMP();
      break;
    case SABUF_HEAP:
      if (buflink == 0)
        link = sa;
      else
        link = __rtSABufExt[num][sa] + buflink - 1;
      link |= ctrlink;
      GET_DIS_IRQ_SMP();
      DrvRtPokeTA(num, AdrTab, sa, link);
      REST_IRQ_SMP();
      break;
    }
    break;
  }
  DrvIOEpi;
  return 0;
}

U16 FARFN rtgetlinkbus(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned type;
  unsigned sa;
  unsigned buf;
  unsigned base;
  unsigned link;
#ifdef MRTA
  int realnum;
  int num1;
  LOCAL_PORT
  unsigned page0;
  unsigned tablea;
#endif

  DrvIOPro(0, 0, 0, 0, 0, 1);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  sa = (__rtSubAddr[num] >> 5) & 0x003F;
  buf = 0;
  link = 0;
  switch (type)
  {
  default:
    USER_ERROR(RT_BAD_FUNC);
    link = 0x4000;
    break;
#ifdef MRTA
  case __MRTA:
    if (__rtAddress[num] == 0x00FF)
    {
      USER_ERROR(RT_BAD_ADDRESS);
      break;
    }
    realnum = GET_RealNum;
    SET_LOCAL_PORT(realnum);
    if (__rtPagePC[num] > __rtMaxPage[num]) // MRTA brc page
    {
      tablea = 31;
      page0 = MRTA_BRC_PAGE;
      num1 = realnum;
    }
    else
    {
      tablea = __rtAddress[num];
      page0 = __hm400Page0[num];
      num1 = num;
    }
    GET_DIS_IRQ_SMP();
    outpw(MRTA_ADDR2(realnum), 0);
    link = DrvRtPeekTA(realnum, tablea, sa);
    REST_IRQ_SMP();
    base = link & 0x0FFF;
    link &= 0xF000;
    switch (__rtSABufMode[num1][sa])
    {
//    case SABUF_OFF:
//    case SABUF_STD:
//    case SABUF_JOINT:
//      buf = 0;
//      break;
    case SABUF_HEAP:
      if (base != (sa + (page0 << 6)))
        buf = base - __rtSABufExt[num1][sa] + 1;
//      else
//        buf = 0;
      break;
    }
    link |= buf;
    break;
#endif
  case __TA:
    GET_DIS_IRQ_SMP();
    link = DrvRtPeekTA(num, AdrTab, sa);
    REST_IRQ_SMP();
    base = link & 0x0FFF;
    link &= 0xF000;
    switch (__rtSABufMode[num][sa])
    {
    case SABUF_OFF:
    case SABUF_STD:
    case SABUF_JOINT:
      buf = base >> 6;
      break;
    case SABUF_HEAP:
      if (base != sa)
        buf = base - __rtSABufExt[num][sa] + 1;
//      else
//        buf = 0;
      break;
    }
    link |= buf;
    break;
  }
  DrvIOEpi;
  return link;
}

U16 FARFN rtenable(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 rtEnDis)
{
  int register num;
#if NRT > 0
  int realnum;
#endif
  unsigned type;
  LOCAL_PORT
  unsigned mask, maskbrc;
  unsigned ctrl;
#if NRT > 0
  unsigned mask05, mask16;
  unsigned irt, nrt;
#endif

  DrvIOPro(3, 0, 0, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
#if NRT > 0
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#else
  SET_LOCAL_PORT(num);
#endif
  switch (rtEnDis)
  {
  case RT_GET_ENABLE:
    rtEnDis = (__rtDisableMask[num]) ? RT_DISABLE : RT_ENABLE;
    break;
  case RT_ENABLE:
  case RT_DISABLE:
    mask = rtEnDis;
    maskbrc = (unsigned)(-1);
    __rtEnableOnAddr[num] = (mask == RT_ENABLE);
    if ((U08)__tmkMode[num] != (U08)RT_MODE)
      break;
    if (mask || __rtAddress[num] >= 31)
    {
      mask = __RT_DIS_MASK[type];
      maskbrc = __RT_BRC_MASK[type];
    }
//    if (mask == __rtDisableMask[num])
//      break;
    __rtDisableMask[num] = mask;
    __rtBRCMask[num] = maskbrc;
    switch (type)
    {
    case __TMK400:
      mask |= __rtAddress[num] | __rtMode[num];
      GET_DIS_IRQ_SMP();
      outpw_d(num, TMK_Ctrl(num), mask);
      REST_IRQ_SMP();
      break;
    case __RTMK400:
      mask |= (__rtControls1[num] & maskbrc) | __rtAddress[num];
      outpb_d(num, R400_AT(num), mask);
      break;
#ifdef MRTX
    case __MRTX:
      if (num == realnum)
        break;
      mask <<= 3;
      mask05 = 0x1F << 3;
      if (__hm400Page[num] & (1 << 11))
      {
        mask <<= 5;
        mask05 <<= 5;
      }
      mask16 = 0;
      if (__hm400Page[num] & (1 << 12))
      {
        mask16 = ~mask16;
      }
      __mrtMask1[realnum] &= ~(mask05 & mask16);
      __mrtMask1[realnum] |= mask & mask16;
      mask16 = ~mask16;
      __mrtMask0[realnum] &= ~(mask05 & mask16);
      __mrtMask0[realnum] |= mask & mask16;

      irt = __mrtMinRT[realnum];
      nrt = __mrtNRT[realnum] + irt - 1;
      mask = __rtBRCMask[irt];
      do
      {
        mask |= __rtBRCMask[++irt];
      }
      while (irt != nrt);
      __rtBRCMask[realnum] = mask;

      outpw(TMK_Mode(realnum), __mrtCtrl0[realnum] | __mrtMask0[realnum] | __rtControls[realnum]);
                                                      //; m__rtControls
      outpw(TMK_Mode(realnum), __mrtCtrl1[realnum] | __mrtMask1[realnum] | __rtControls[realnum]);
                                                      //; m__rtControls
      GET_DIS_IRQ_SMP();
      ctrl = __rtControls1[num] & __rtBRCMask[num];
      outpw(TMK_Ctrl(realnum), ctrl);
      outpw(TMK_Ctrl(realnum), ctrl); // PCI2
      REST_IRQ_SMP();
      break;
#endif
    case __TMKX:
      GET_DIS_IRQ_SMP();
      ctrl = (__rtControls1[num] & maskbrc) | mask;
      outpw(TMK_Ctrl(num), ctrl);
      outpw(TMK_Ctrl(num), ctrl); // PCI2
      REST_IRQ_SMP();
                   //; Восстановление адреса ОУ
      break;
    case __TA:
//      GET_MUTEX; // use cli/sti in main DOS program if needed
      GET_DIS_IRQ_SMP();
      outpw(TA_MODE1(num), __rtControls[num] &= ~TA_RTMT_START);
      if (__rtAddress[num] >= 31)
        __rtControls1[num] |= 0xF800; // compensate commented out __rtDisableMask
      outpw(TA_MODE2(num), (__rtControls1[num])); // & maskbrc) | mask;
      __rtControls[num] |= TA_RTMT_START & (maskbrc>>1) & ~((unsigned)__tmkModeMtRt[num]<<3);
      if (__tmkModeMtRt[num])
        __bcControls[num] = __rtControls[num];
      outpw(TA_MODE1(num), __rtControls[num]);
      REST_IRQ_SMP();
//      REST_MUTEX;
      break;
#ifdef MRTA
    case __MRTA:
      if (num == realnum || __rtAddress[num] >= 31)
        break;
      if (mask)
      {
        __dmrtRT[realnum] &= ~(1L << __rtAddress[num]);
        __dmrtBrc[realnum] &= ~(1L << __rtAddress[num]);
//        GET_MUTEX; // use cli/sti in main DOS program if needed
        __rtControls1[num] &= ~MRTA_RT_ON;
        outpw(MRTA_SW(realnum), __rtControls1[num]);
//        REST_MUTEX;
        if (DrvMrtaBrcRtOn(realnum) == 0)
          outpw(MRTA_SW(realnum), 0xF800);
        if (__dmrtRT[realnum] == 0L && __dmrtBrc[realnum] == 0L)
        {
//          GET_MUTEX; // use cli/sti in main DOS program if needed
          GET_DIS_IRQ_SMP();
          outpw(TA_MODE1(realnum), __rtControls[realnum] &= ~TA_RTMT_START);
          REST_IRQ_SMP();
//          REST_MUTEX;
        }
      }
      else
      {
        __dmrtRT[realnum] |= 1L << __rtAddress[num];
        if (__rtControls1[num] & TA_BRCST_MODE)
          __dmrtBrc[realnum] |= 1L << __rtAddress[num];
//        GET_MUTEX; // use cli/sti in main DOS program if needed
        __rtControls1[num] |= MRTA_RT_ON;
        outpw(MRTA_SW(realnum), __rtControls1[num]);
//        REST_MUTEX;
//        outpw(MRTA_SW(realnum), 0xF800 | __rtControls1[realnum] | DrvMrtaBrcRtOn(realnum));
        outpw(MRTA_SW(realnum), 0xF800 | DrvMrtaBrcRtOn(realnum));
        outpw(TA_MODE2(realnum), __rtControls1[realnum]);
        if ((__rtControls[realnum] & TA_RTMT_START) == 0 && (__dmrtRT[realnum] != 0L || __dmrtBrc[realnum] != 0L))
        {
//          GET_MUTEX; // use cli/sti in main DOS program if needed
          GET_DIS_IRQ_SMP();
          outpw(TA_MODE1(realnum), __rtControls[realnum] |= TA_RTMT_START);
          REST_IRQ_SMP();
//          REST_MUTEX;
        }
        DrvRtWMode(num, __MRTA, 0);
      }
      break;
#endif
    case __TMKMPC:
      break;
    }
    rtEnDis = 0;
    break;
  default:
    DrvIOStop;
    return USER_ERROR(RT_BAD_FUNC);
  }
  DrvIOEpi;
  return rtEnDis;
}

int FARFN rtdefaddress(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Address)
{
  int register num;
#if NRT > 0
  int realnum;
#endif
  unsigned type;
  LOCAL_PORT
  unsigned rtaddr, rtaddr1, rtaddr2, rtpar;
  int rterr;
  BOOL rteoa;
#if NRT > 0
  unsigned mask;
  unsigned mask05, mask16;
  unsigned irt, nrt;
  unsigned shift;
  int fEnable = 0;
  int rtctrl;
  TFLGM rtflgm;
#endif

  DrvIOPro(4, 0, 0, 0, 2, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  rtaddr = Address;
//  CHECK_RT_ADDRESS(rtaddr);
  if (__tmkOptions[num] & (UOPT)RT_USE_ADDR_PARITY)
  {
    rtpar = (((rtaddr) ^ (rtaddr >> 1) ^ (rtaddr >> 2) ^ (rtaddr >> 3) ^ (rtaddr >> 4) ^ 1) & 1) << 5;
    rtaddr ^= ADDR_PARITY_ON | rtpar;
  }
  if (rtaddr >= 31)
  {
    DrvIOStop;
    rteoa = __rtEnableOnAddr[num] || !__rtDisableMask[num];
    __rtAddress[num] = 0x00FF;
    rtenable(TMKNUM__(__tmkNumber) RT_DISABLE);
    __rtEnableOnAddr[num] = rteoa;
    return USER_ERROR(RT_BAD_ADDRESS);
  }
  __rtAddress[num] = rtaddr;
//  __rtAddrP[num] = rtpar;
  rterr = 0;
  if ((U08)__tmkMode[num] == (U08)RT_MODE &&
      __rtDisableMask[num] &&
      __rtEnableOnAddr[num])
  {
#if NRT > 0
    fEnable = 1;
#endif
    __rtDisableMask[num] = 0;  //; RT_ENABLE
    __rtBRCMask[num] = 0xFFFF;
  }
#if NRT > 0
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#else
  SET_LOCAL_PORT(num);
#endif
  switch (type)
  {
  case __TMKMPC:
    DrvIOStop;
    return USER_ERROR(RT_BAD_FUNC);
  case __TMK400:
    rtaddr |= __rtDisableMask[num] | __rtMode[num];
    GET_DIS_IRQ_SMP();
    outpw_d(num, TMK_Ctrl(num), rtaddr);
    REST_IRQ_SMP();
    break;
  case __RTMK400:
    rtaddr |= __rtControls1[num];
    rtaddr &= __rtBRCMask[num];
    rtaddr |= __rtDisableMask[num];
    outpb_d(num, R400_AT(num), rtaddr);
    break;
#ifdef MRTX
  case __MRTX:
    if (num == realnum)
      break;
    if (fEnable) //; became enabled?
    {
      mask = __rtDisableMask[num] << 3;
      mask05 = 0x1F << 3;
      if (__hm400Page[num] & (1 << 11))
      {
        mask <<= 5;
        mask05 <<= 5;
      }
      mask16 = 0;
      if (__hm400Page[num] & (1 << 12))
      {
        mask16 = ~mask16;
      }
      __mrtMask1[realnum] &= ~(mask05 & mask16);
      __mrtMask1[realnum] |= mask & mask16;
      mask16 = ~mask16;
      __mrtMask0[realnum] &= ~(mask05 & mask16);
      __mrtMask0[realnum] |= mask & mask16;

      irt = __mrtMinRT[realnum];
      nrt = __mrtNRT[realnum] + irt - 1;
      mask = __rtBRCMask[irt];
      do
      {
        mask |= __rtBRCMask[++irt];
      }
      while (irt != nrt);
      if (mask != __rtBRCMask[realnum])
      {
        __rtBRCMask[realnum] = mask;
        mask &= __rtControls1[realnum];
        GET_DIS_IRQ_SMP();
        outpw(TMK_Ctrl(realnum), mask);
        outpw(TMK_Ctrl(realnum), mask); // PCI2
        REST_IRQ_SMP();
      }
    }

    irt = __hm400Page[num] >> 11;
//    shift = (irt & 1) ? 8 : 3;
    shift = irt & 1;
    shift = (shift << 2) + shift + 3;
    mask = ~(0x1F << shift);
    rtaddr <<= shift;
//    GET_MUTEX; // use cli/sti in main DOS program if needed
    if ((irt & 2) == 0)
    {
      mask &= __mrtCtrl0[realnum];
      rtaddr |= mask;
      __mrtCtrl0[realnum] = rtaddr;
      rtaddr |= __mrtMask0[realnum];
    }
    else
    {
      mask &= __mrtCtrl1[realnum];
      rtaddr |= mask;
      __mrtCtrl1[realnum] = rtaddr;
      rtaddr |= __mrtMask1[realnum];
    }
    rtaddr |= __rtControls[realnum];      //; m__rtControls
    if (__tmkMode[num] == RT_MODE)
      outpw(TMK_Mode(realnum), rtaddr);
//    REST_MUTEX;
    break;
#endif
  case __TMKX:
    rtaddr <<= 11;
    GET_MUTEX;
    rtaddr |= __rtControls1[num] & 0x057F;
    __rtControls1[num] = rtaddr;
    REST_MUTEX;
    rtaddr &= __rtBRCMask[num];
    rtaddr |= __rtDisableMask[num];
    GET_DIS_IRQ_SMP();
    if (__tmkMode[num] == RT_MODE)
    {
      outpw(TMK_Ctrl(num), rtaddr);
      outpw(TMK_Ctrl(num), rtaddr); // PCI2
    }
    REST_IRQ_SMP();
    break;
  case __TA:
    rtaddr <<= 11;
//    GET_MUTEX; // use cli/sti in main DOS program if needed
    GET_DIS_IRQ_SMP();
    outpw(TA_MODE1(num), __rtControls[num] &= ~TA_RTMT_START);
    rtaddr |= __rtControls1[num] & 0x07FF;
    __rtControls1[num] = rtaddr;
    __rtControls[num] |= TA_RTMT_START & (__rtBRCMask[num]>>1) & ~((unsigned)__tmkModeMtRt[num]<<3);
    if (__tmkModeMtRt[num])
      __bcControls[num] = __rtControls[num];
//    rtaddr &= __rtBRCMask[num];
//    rtaddr |= __rtDisableMask[num];
    // check RT address lines and disable RT if error
    outpw(TA_MODE2(num), rtaddr ^ (unsigned) 0xF800);
    inptrwu(TA_MODE2(num), &rtaddr1);
    outpw(TA_MODE2(num), rtaddr);
    rtaddr2 = inpw(TA_MODE2(num)); // DrvIOEpi, DrvIOPro
    if (rtaddr1 != (rtaddr ^ (unsigned) 0xF800) || rtaddr2 != rtaddr)
      rterr = 1;
    if (rterr)
    {
      __rtControls[num] &= ~TA_RTMT_START;
      if (__tmkModeMtRt[num])
        __bcControls[num] = __rtControls[num];
      __rtAddress[num] = 0x00FF;
      __rtDisableMask[num] = __RT_DIS_MASK[type];
      __rtBRCMask[num] = __RT_BRC_MASK[type];
//      __rtEnableOnAddr[num] = 0;
      outpw(TA_MODE2(num), __rtControls1[num] |= 0xF800); // compensate commented out __rtDisableMask
    }
    outpw(TA_MODE1(num), __rtControls[num]);
    REST_IRQ_SMP();
//    REST_MUTEX;
    if (rterr)
    {
      DrvIOEpi;
      return USER_ERROR(RT_BAD_ADDRESS);
    }
    break;
#ifdef MRTA
  case __MRTA:
    if (num == realnum)
      break;
//    if (__dmrtRT[realnum] & (1L << rtaddr))
//      USER_ERROR(RT_BAD_ADDRESS)
    rtctrl = __rtControls1[num]; // previous ctrl
    rtflgm = __FLAG_MODE[num] & DRV_FLAG_MODE_SW; // previous flag mode
    rtaddr = rtctrl >> 11;   // previous addr
    if (rtaddr != 31 && rtaddr != __rtAddress[num])
    {
      __dmrtRT[realnum] &= ~(1L << rtaddr);
      __dmrtBrc[realnum] &= ~(1L << rtaddr);
      __mrtA2RT[realnum][rtaddr] = 0;
//      GET_MUTEX; // use cli/sti in main DOS program if needed
      __rtControls1[num] &= ~MRTA_RT_ON;
      outpw(MRTA_SW(realnum), __rtControls1[num]);
//      REST_MUTEX;
      if (DrvMrtaBrcRtOn(realnum) == 0)
        outpw(MRTA_SW(realnum), 0xF800);
      DrvFlagModeTA(num, 0);
    }
    if (rtaddr != __rtAddress[num]) // aka rtreset (+ all settings after) for new RT !
    {
      rtaddr = __rtAddress[num];
      __mrtA2RT[realnum][rtaddr] = num;
      __hm400Page0[num] = 0x20 | rtaddr;
      __hm400Page2[num] = __hm400Page0[num] >> 4;
      __hm400Page[num] = __hm400Page0[num] << 11;
//      __rtSubAddr2[num] = 0;
      __rtSABuf[num] = 0;
      mrtcreattab(realnum, rtaddr);
      if (rtflgm) // flag mode?
        DrvFlagModeTA(num, DRV_FLAG_MODE); //1);
      DrvRtWMode(num, __MRTA, 0);
      if (__rtDisableMask[num])
      {
        __rtControls1[num] = ((rtctrl & 0x07FF) | (rtaddr << 11)) & ~MRTA_RT_ON;
        __dmrtRT[realnum] &= ~(1L << rtaddr);
        __dmrtBrc[realnum] &= ~(1L << rtaddr);
      }
      else
      {
        __rtControls1[num] = (rtctrl & 0x07FF) | (rtaddr << 11) | MRTA_RT_ON;
        __dmrtRT[realnum] |= 1L << rtaddr;
        if (__rtControls1[num] & TA_BRCST_MODE)
          __dmrtBrc[realnum] |= 1L << rtaddr;
      }
      outpw(MRTA_SW(realnum), __rtControls1[num]);
//      outpw(MRTA_SW(realnum), 0xF800 | __rtControls1[realnum] | DrvMrtaBrcRtOn(realnum));
      outpw(MRTA_SW(realnum), 0xF800 | DrvMrtaBrcRtOn(realnum));
      outpw(TA_MODE2(realnum), __rtControls1[realnum]);
      if ((__rtControls[realnum] & TA_RTMT_START) == 0 && (__dmrtRT[realnum] != 0L || __dmrtBrc[realnum] != 0L))
      {
//        GET_MUTEX; // use cli/sti in main DOS program if needed
        GET_DIS_IRQ_SMP();
        outpw(TA_MODE1(realnum), __rtControls[realnum] |= TA_RTMT_START);
        REST_IRQ_SMP();
//        REST_MUTEX;
      }
    }
    break;
#endif
  }
  DrvIOEpi;
  return 0;
}

U16 FARFN rtgetaddress(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
#if NRT > 0
  int realnum;
#endif
  unsigned type;
  LOCAL_PORT
  unsigned rtaddr, rtpar;

  DrvIOPro(0, 0, 0, 0, 1, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
#if NRT > 0
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
#else
  SET_LOCAL_PORT(num);
#endif
  rtaddr = __rtAddress[num];
  switch (type)
  {
  case __TMKMPC:
    USER_ERROR(RT_BAD_FUNC);
    rtaddr = 0xFFFF;
    break;
  case __TMK400:
    rtaddr = inpb_d(num, T400_ATR(num)) & 0x1F;
    break;
  case __RTMK400:
    rtaddr = (inpb_d(num, R400_AT(num)) >> 2) & 0x1F;
    break;
#ifdef MRTX
  case __MRTX:
#endif
  case __TMKX:
#ifdef MRTA
  case __MRTA:
#endif
    break;
  case __TA:
    rtaddr = inpw(TA_MODE2(num)) >> 11;
    break;
  }
  if (rtaddr <= 31 && (__tmkOptions[num] & (UOPT)RT_USE_ADDR_PARITY))
  {
    rtpar = (((rtaddr) ^ (rtaddr >> 1) ^ (rtaddr >> 2) ^ (rtaddr >> 3) ^ (rtaddr >> 4) ^ 1) & 1) << 5;
    rtaddr ^= ADDR_PARITY_ON | rtpar;
  }
  DrvIOEpi;
  return rtaddr;
}

void FARFN rtgetflags(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
         VOID FARDT *pcBufAddr, U16 Dir, U16 FlagMin, U16 FlagMax)
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  unsigned dir;
  U16 FARDT *buf;
  unsigned save_rama, save_ramiw;
  IRQ_FLAGS;

  DrvIOProV;
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  pos = FlagMin;
  CHECK_RT_SUBADDR(pos);
  len = FlagMax;
  CHECK_RT_SUBADDR(len);
  len = len + 1 - pos;
  if ((int)len <= 0)
  {
    DrvIOStop;
    return;
  }
  dir = Dir;
  CHECK_RT_DIR(dir);
  pos |= dir;
  __rtSubAddr[num] = dir;
  pos |= __hm400Page[num];
  buf = (U16 FARDT*)pcBufAddr;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    outpw_d(realnum, TMK_Addr(realnum), pos >> 6);
    REP_INSW_D(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    outpb_d(realnum, TMKMPC_AddrH(realnum), pos >> 6);
    REP_INSW_D(TMKMPC_DataL(realnum));
//    REP_INSWB_D(TMKMPC_DataL(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
#ifdef MRTX
  case __MRTX:
#endif
  case __TMKX:
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    REST_IRQ_SMP();
    REP_INSW(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
#ifdef MRTA
  case __MRTA:
#endif
  case __TA:
    pos = FlagMin;
    do
    {
      *(buf++) = rtgetflag(TMKNUM__(realnum) (U16)dir, (U16)pos);
      ++pos;
    }
    while (--len);
    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  DrvIOEpi;
  return;
}

//__EXTERN void __FAR rtgetflags(void __FAR *pcBuffer, U16 rtDir, U16 rtFlagMin, U16 rtFlagMax);
//__EXTERN void __FAR rtputflags(void __FAR *pcBuffer, U16 rtDir, U16 rtFlagMin, U16 rtFlagMax);
void FARFN rtputflags(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
         VOID FARDT *pcBufAddr, U16 Dir, U16 FlagMin, U16 FlagMax)
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned pos;
  unsigned len;
  unsigned dir;
  U16 FARDT *buf;
  unsigned save_rama, save_ramiw;
  IRQ_FLAGS;

  DrvIOProV;
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  pos = FlagMin;
  CHECK_RT_SUBADDR(pos);
  len = FlagMax;
  CHECK_RT_SUBADDR(len);
  len = len + 1 - pos;
  if ((int)len <= 0)
  {
    DrvIOStop;
    return;
  }
  dir = Dir;
  CHECK_RT_DIR(dir);
  pos |= dir;
  __rtSubAddr[num] = dir;
  pos |= __hm400Page[num];
  buf = (U16 FARDT*)pcBufAddr;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    outpw_d(realnum, TMK_Addr(realnum), pos >> 6);
    REP_OUTSW_D(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    outpb_d(realnum, TMKMPC_AddrH(realnum), pos >> 6);
    REP_OUTSWB_D(TMKMPC_DataL(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
#ifdef MRTX
  case __MRTX:
#endif
  case __TMKX:
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    REST_IRQ_SMP();
    REP_OUTSW(TMK_Data(realnum));
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
#ifdef MRTA
  case __MRTA:
#endif
  case __TA:
    pos = FlagMin;
    do
    {
      rtputflag(TMKNUM__(realnum) (U16)dir, (U16)pos, *(buf++));
      ++pos;
    }
    while (--len);
    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  DrvIOEpi;
  return;
}

void FARFN rtsetflag(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned type;
#ifdef MRTA
  LOCAL_PORT
#endif
  unsigned sa;
  unsigned pos;

  DrvIOPro(0, 0, 0, 1, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  sa = __rtSubAddr[num];
  switch (type)
  {
  default:
    pos = ((sa >> 5) | sa) & 0x041F;
    DrvRtPoke(num, type, pos, 0x8000);
    DrvRtDefSubAddr(num, type, sa);
    break;
  case __TA:
    sa = (sa >> 5) & 0x3F;
    if (!(__FLAG_MODE[num] & DRV_FLAG_MODE_HW))
    {
      if (sa & 0x20)
        __RT_BC_FLAG[num][sa&0x1f] = 0x8000;
      else
        __BC_RT_FLAG[num][sa] = 0x8000;
      __FLAG_MODE[num] |= DRV_FLAG_UPDATE;
    }
    else
    {
#ifdef USE_TMK_WRAP_SA
      if (__tmkWrapSA)
      {
        if ((sa & 0x1F) == __tmkWrapSA)
          break;
      }
#endif
      GET_DIS_IRQ_SMP();
//      DrvRtPokeTA(num, sa, 58, 0);
//      DrvPeekAndOrPokeTA(num, AdrTab, sa, 0x7FFF, (sa & 0x20) ? 0 : 0x8000);
      DrvPeekAndOrPokeTA(num, AdrTab, sa, 0x7FFF, ((sa & 0x20) ^ 0x20) << 10);
      REST_IRQ_SMP();
//      DrvRtDefSubAddr(num, type, sa);
    }
    break;
#ifdef MRTA
  case __MRTA:
    sa = (sa >> 5) & 0x3F;
    if (!(__FLAG_MODE[num] & DRV_FLAG_MODE_HW))
    {
      if (sa & 0x20)
        __RT_BC_FLAG[num][sa&0x1f] = 0x8000;
      else
        __BC_RT_FLAG[num][sa] = 0x8000;
      __FLAG_MODE[num] |= DRV_FLAG_UPDATE;
    }
    else
    {
      int register realnum;
      realnum = GET_RealNum;
      SET_LOCAL_PORT(realnum);
#ifdef USE_TMK_WRAP_SA
      if (__tmkWrapSA)
      {
        if ((sa & 0x1F) == __tmkWrapSA)
          break;
      }
#endif
      GET_DIS_IRQ_SMP();
//      outpw(MRTA_ADDR2(realnum), __hm400Page0[num] >> 4);
//      DrvRtPokeTA(realnum, (__hm400Page0[num] << 6) | sa, 58, 0);
      outpw(MRTA_ADDR2(realnum), 0);
//      DrvPeekAndOrPokeTA(realnum, __rtAddress[num], sa, 0x7FFF, (sa & 0x20) ? 0 : 0x8000);
      DrvPeekAndOrPokeTA(realnum, __rtAddress[num], sa, 0x7FFF, ((sa & 0x20) ^ 0x20) << 10);
      REST_IRQ_SMP();
//      DrvRtDefSubAddr(num, type, sa);
    }
    break;
#endif
  }
  DrvIOEpi;
  return;
}

void FARFN rtclrflag(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned type;
#ifdef MRTA
  LOCAL_PORT
#endif
  unsigned sa;
  unsigned pos;

  DrvIOPro(0, 0, 0, 1, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  sa = __rtSubAddr[num];
  switch (type)
  {
  default:
    pos = ((sa >> 5) | sa) & 0x041F;
    DrvRtPoke(num, type, pos, 0);
    DrvRtDefSubAddr(num, type, sa);
    break;
  case __TA:
    sa = (sa >> 5) & 0x3F;
    if (!(__FLAG_MODE[num] & DRV_FLAG_MODE_HW))
    {
      if (sa & 0x20)
        __RT_BC_FLAG[num][sa&0x1f] = 0x0000;
      else
        __BC_RT_FLAG[num][sa] = 0x0000;
      __FLAG_MODE[num] |= DRV_FLAG_UPDATE;
    }
    else
    {
#ifdef USE_TMK_WRAP_SA
      if (__tmkWrapSA)
      {
        if ((sa & 0x1F) == __tmkWrapSA)
          break;
      }
#endif
      GET_DIS_IRQ_SMP();
//      DrvRtPokeTA(num, sa, 58, 0);
//      DrvPeekAndOrPokeTA(num, AdrTab, sa, 0x7FFF, (sa & 0x20) ? 0x8000 : 0);
      DrvPeekAndOrPokeTA(num, AdrTab, sa, 0x7FFF, (sa & 0x20) << 10);
      REST_IRQ_SMP();
//      DrvRtDefSubAddr(num, type, sa);
    }
    break;
#ifdef MRTA
  case __MRTA:
    sa = (sa >> 5) & 0x3F;
    if (!(__FLAG_MODE[num] & DRV_FLAG_MODE_HW))
    {
      if (sa & 0x20)
        __RT_BC_FLAG[num][sa&0x1f] = 0x0000;
      else
        __BC_RT_FLAG[num][sa] = 0x0000;
      __FLAG_MODE[num] |= DRV_FLAG_UPDATE;
    }
    else
    {
      int register realnum;
      realnum = GET_RealNum;
      SET_LOCAL_PORT(realnum);
#ifdef USE_TMK_WRAP_SA
      if (__tmkWrapSA)
      {
        if ((sa & 0x1F) == __tmkWrapSA)
          break;
      }
#endif
      GET_DIS_IRQ_SMP();
//      outpw(MRTA_ADDR2(realnum), __hm400Page0[num] >> 4);
//      DrvRtPokeTA(realnum, (__hm400Page0[num] << 6) | sa, 58, 0);
      outpw(MRTA_ADDR2(realnum), 0);
//      DrvPeekAndOrPokeTA(realnum, __rtAddress[num], sa, 0x7FFF, (sa & 0x20) ? 0x8000 : 0);
      DrvPeekAndOrPokeTA(realnum, __rtAddress[num], sa, 0x7FFF, (sa & 0x20) << 10);
      REST_IRQ_SMP();
//      DrvRtDefSubAddr(num, type, sa);
    }
    break;
#endif
  }
  DrvIOEpi;
  return;
}

void FARFN rtputflag(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Dir, U16 SubAddr, U16 Flag)
{
  int register num;
  unsigned type;
#ifdef MRTA
  LOCAL_PORT
#endif
  unsigned sa;
  unsigned dir;
  unsigned pos;

  DrvIOPro(0, 0, 0, 1, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  sa = SubAddr;
  CHECK_RT_SUBADDR(sa);
  dir = Dir;
  CHECK_RT_DIR(dir);
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    pos = sa | dir;
    DrvRtPoke(num, type, pos, Flag);
    DrvRtDefSubAddr(num, type, sa);
    break;
  case __TA:
    if (!(__FLAG_MODE[num] & DRV_FLAG_MODE_HW))
    {
      if (dir)
        __RT_BC_FLAG[num][sa] = Flag;
      else
        __BC_RT_FLAG[num][sa] = Flag;
      __FLAG_MODE[num] |= DRV_FLAG_UPDATE;
    }
    else
    {
#ifdef USE_TMK_WRAP_SA
      if (__tmkWrapSA)
      {
        if ((sa & 0x1F) == __tmkWrapSA)
          break;
      }
#endif
      pos = sa | (dir >> 5);
      GET_DIS_IRQ_SMP();
//      DrvRtPokeTA(num, pos, 58, Flag & 0x7FFF); // & 0x47FF;
//      DrvPeekAndOrPokeTA(num, AdrTab, pos, 0x7FFF, (((Flag & 0x8000) != 0) != (dir != 0)) ? 0x8000 : 0);
      DrvPeekAndOrPokeTA(num, AdrTab, pos, 0x7FFF, (Flag & 0x8000) ^ (dir << 5));
      REST_IRQ_SMP();
//      DrvRtDefSubAddr(num, type, ?sa);
    }
    break;
#ifdef MRTA
  case __MRTA:
    if (!(__FLAG_MODE[num] & DRV_FLAG_MODE_HW))
    {
      if (dir)
        __RT_BC_FLAG[num][sa] = Flag;
      else
        __BC_RT_FLAG[num][sa] = Flag;
      __FLAG_MODE[num] |= DRV_FLAG_UPDATE;
    }
    else
    {
      int register realnum;
      realnum = GET_RealNum;
      SET_LOCAL_PORT(realnum);
#ifdef USE_TMK_WRAP_SA
      if (__tmkWrapSA)
      {
        if ((sa & 0x1F) == __tmkWrapSA)
          break;
      }
#endif
      pos = sa | (dir >> 5);
      GET_DIS_IRQ_SMP();
// why this poke originally was active for MRTA and wasn't for TA???
//      outpw(MRTA_ADDR2(realnum), __hm400Page0[num] >> 4);
//      DrvRtPokeTA(realnum, (__hm400Page0[num] << 6) | pos, 58, Flag & 0x7FFF); // & 0x27FF;
      outpw(MRTA_ADDR2(realnum), 0);
//      DrvPeekAndOrPokeTA(realnum, __rtAddress[num], pos, 0x7FFF, (((Flag & 0x8000) != 0) != (dir != 0)) ? 0x8000 : 0);
      DrvPeekAndOrPokeTA(realnum, __rtAddress[num], pos, 0x7FFF, (Flag & 0x8000) ^ (dir << 5));
      REST_IRQ_SMP();
//      DrvRtDefSubAddr(num, type, ?sa);
    }
    break;
#endif
  }
  DrvIOEpi;
  return;
}

U16 FARFN rtgetflag(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Dir, U16 SubAddr)
{
  int register num;
  unsigned type;
#ifdef MRTA
  LOCAL_PORT
#endif
  unsigned sa;
  unsigned dir;
  unsigned pos;
  unsigned flag;
  unsigned flagv;

  DrvIOPro(0, 0, 0, 0, 0, 2);
  CLRtmkError;
  num = __tmkNumber;
  sa = SubAddr;
  CHECK_RT_SUBADDR(sa);
  dir = Dir;
  CHECK_RT_DIR(dir);
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  default:
    pos = sa | dir;
    flag = DrvRtPeek(num, type, pos);
    break;
  case __TA:
    if (!(__FLAG_MODE[num] & DRV_FLAG_MODE_HW))
    {
      if (dir)
        flag = __RT_BC_FLAG[num][sa];
      else
        flag = __BC_RT_FLAG[num][sa];
      __FLAG_MODE[num] |= DRV_FLAG_UPDATE;
    }
    else
    {
      pos = sa | (dir >> 5);
      GET_DIS_IRQ_SMP();
/*
      flag = DrvRtPeekTA(num, AdrTab, pos);
      if (dir)
      {
        flag = (~flag) & 0x8000;
      }
      else
      {
        flag = flag & 0x8000;
      }
      flag |= DrvRtPeekTA(num, pos, 58) & 0x07FF;
*/
      DrvRtPeekPtrTA(num, AdrTab, pos, &flagv);
      flag = (DrvRtPeekTA(num, pos, 58) & 0x47FF) | (flagv & 0x8000);
/*
      if (flag & RT_ERROR_MASK)
      {
//This part would be good redundant addon to the processing in the irq handler
//However it could give a side effect in case of a specially set BUSY in table
//and random value (with ERROR) in the state word 58 and if then the set BUSY
//checked back by the program (strange but why not)
        flag = 0;
//        DrvRtPokeTA(num, pos, 58, 0);
        if (flagv & 0x8000)
          DrvRtPokeTA(num, AdrTab, pos, flagv & 0x7FFF);
      }
*/
      if (dir)
        flag ^= 0x8000;
      REST_IRQ_SMP();
    }
    break;
#ifdef MRTA
  case __MRTA:
    if (!(__FLAG_MODE[num] & DRV_FLAG_MODE_HW))
    {
      if (dir)
        flag = __RT_BC_FLAG[num][sa];
      else
        flag = __BC_RT_FLAG[num][sa];
      __FLAG_MODE[num] |= DRV_FLAG_UPDATE;
    }
    else
    {
      int register realnum;
      realnum = GET_RealNum;
      SET_LOCAL_PORT(realnum);
      pos = sa | (dir >> 5);
      GET_DIS_IRQ_SMP();
/*
      outpw(MRTA_ADDR2(realnum), 0);
      flag = DrvRtPeekTA(realnum, __rtAddress[num], pos);
      if (dir)
      {
        flag = (~flag) & 0x8000;
      }
      else
      {
        flag = flag & 0x8000;
      }
      outpw(MRTA_ADDR2(realnum), __hm400Page0[num] >> 4);
      flag |= DrvRtPeekTA(realnum, (__hm400Page0[num] << 6) | pos, 58) & 0x07FF;
*/
      outpw(MRTA_ADDR2(realnum), 0);
      DrvRtPeekPtrTA(realnum, __rtAddress[num], pos, &flagv);
      outpw(MRTA_ADDR2(realnum), __hm400Page0[num] >> 4);
      flag = (DrvRtPeekTA(realnum, (__hm400Page0[num] << 6) | pos, 58) & 0x27FF) | (flagv & 0x8000);
      flag = (flag & 0xBFFF) | ((flag & 0x2000) << 1); // add err bit as in TA
/*
      if (flag & RT_ERROR_MASK)
      {
//This part would be good redundant addon to the processing in the irq handler
//However it could give a side effect in case of a specially set BUSY in table
//and random value (with ERROR) in the state word 58 and if then the set BUSY
//checked back by the program (strange but why not)
        flag = 0;
//        DrvRtPokeTA(realnum, (__hm400Page0[num] << 6) | pos, 58, 0);
        if (flagv & 0x8000)
        {
          outpw(MRTA_ADDR2(realnum), 0);
          DrvRtPokeTA(realnum, __rtAddress[num], pos, flagv & 0x7FFF);
        }
      }
*/
      if (dir)
        flag ^= 0x8000;
      REST_IRQ_SMP();
    }
    break;
#endif
  }
  DrvRtDefSubAddr(num, type, (sa << 5) | dir);
  DrvIOEpi;
  return flag;
}

U16 FARFN rtbusy(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned port;
  unsigned state=0, statex;
  int limit;

  DrvIOPro(0, 0, 0, 0, 1, 0);
  CLRtmkError;
#if NRT > 0
  if (__rtPagePC[__tmkNumber] == (__rtMaxPage[__tmkNumber] + 2)) // brc sw page
  {
    DrvIOStop;
    return 0;
  }
#endif
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  port = TMK_State(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TMKMPC:
    port = TMKMPC_StateL(realnum);
    fallthrough;
    // fall-through
  case __TMK400:
  case __RTMK400:
    state = inpw_d(realnum, port);
    break;
#ifdef MRTX
  case __MRTX:
    if (__rtMode[__tmkNumber] != __rtMode[realnum] && (__rtMode[realnum] & 0x0800) == 0)
      // if different (switched rt) and unlocked mode
      DrvRtWMode(__tmkNumber, __MRTX, __rtMode[__tmkNumber]);
    limit = 16; // abstract loop limit for broken hardware
    state = inpw(port);
    do
    {
      statex = state;
      state = inpw(port);
    }
    while (statex != state && --limit != 0);
    if ((state & 0x8000) == 0)
    {
      if (((state >> 1) & 0x1800) != __hm400Page[__tmkNumber])
        state = 0;
    }
    break;
#endif
  case __TMKX:
    limit = 16; // abstract loop limit for broken hardware
    state = inpw(port);
    do
    {
      statex = state;
      state = inpw(port);
    }
    while (statex != state && --limit != 0);
    break;
  case __TA:
    state = inpw(TA_BASE(realnum));
//    if (((state & 0x8000) == 0) && ((state & 0x03FF) == (DrvRtGetBaseTA(realnum) & 0x03FF)))
    if ((state & 0x83FF) == (DrvRtGetBaseTA(realnum) & 0x03FF))
      state >>= 2;
    else
      state = 0;
    break;
#ifdef MRTA
  case __MRTA:
    GET_DIS_IRQ();
    state = (__rtPagePC[__tmkNumber] <=  __rtMaxPage[__tmkNumber]) ? 0 : 0xF800;
    outpw(TA_LCW(realnum), (state | __rtControls1[__tmkNumber]) & 0xF800); // no matter if TA_BRCST_MODE here
    state = inpw(TA_BASE(realnum));
    REST_IRQ();
//    if (((state & 0x8000) == 0) && ((state & 0x0FFF) == (DrvRtGetBaseMRTA(__tmkNumber) & 0x0FFF)))
    if ((state & 0x8FFF) == (DrvRtGetBaseMRTA(__tmkNumber) & 0x0FFF))
      state >>= 2;
    else
      state = 0;
    break;
#endif
  }
  DrvIOEpi;
  return (state >> 11) & 1;
}

#if NRT > 0
U16 FARFN mrtgetstate(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned port;
  unsigned state=0, statex;
  int limit;

  DrvIOPro(0, 0, 0, 0, 2, 0);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  port = TMK_State(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TMKMPC:
    port = TMKMPC_StateL(realnum);
    fallthrough;
    // fall-through
  case __TMK400:
  case __RTMK400:
    state = inpw_d(realnum, port) & 0x5FFF;
    break;
#ifdef MRTX
  case __MRTX:
    if (__rtMode[__tmkNumber] != __rtMode[realnum] && (__rtMode[realnum] & 0x0800) == 0)
      // if different (switched rt) and unlocked mode
      DrvRtWMode(__tmkNumber, __MRTX, __rtMode[__tmkNumber]);
#endif
    fallthrough;
    // fall-through
  case __TMKX:
    limit = 16; // abstract loop limit for broken hardware
    state = inpw(port);
    do
    {
      statex = state;
      state = inpw(port);
    }
    while (statex != state && --limit != 0);
    // always RT_SHOW_BRC_STATE here for backward compatibility
    break;
  case __TA:
    GET_DIS_IRQ();
//    state = inpw(TA_LCW(realnum)) & 0x7FF;
    inptrwu(TA_BASE(realnum), &statex);
//    if (statex & 0x2000)
      state = inpw(TA_LCW(realnum)); // DrvIOEpi, DrvIOPro
    REST_IRQ();
    state &= ((__tmkOptions[realnum] & (UOPT)RT_SHOW_BRC_STATE) && ((state & 0xF800) == 0xF800)) ? 0x87FF : 0x07FF;
    if (((statex & 0x8000) == 0) && ((statex & 0x03FF) == (DrvRtGetBaseTA(realnum) & 0x03FF)))
      state |= (statex & 0x2000) >> 2;
    break;
#ifdef MRTA
  case __MRTA:
    GET_DIS_IRQ();
// no ability to read mrtgetstate from a register on MRTA!!!
// mrtgetstate after message can be retrieved from last irq history
// mrtgetstate during message can only be done by polling of all 32 RT TA_BASEs
// to get 1 or 2 active base numbers and lcw codes and 32 active bits
// and anyway for 2 bases (rt-rt) the result cannot be returned as U16
    state = (__rtPagePC[__tmkNumber] <=  __rtMaxPage[__tmkNumber]) ? 0 : 0xF800;
    outpw(TA_LCW(realnum), (state | __rtControls1[__tmkNumber]) & 0xF800);// & ~TA_BRCST_MODE); // no TA_BRCST_MODE here
    inptrwu(TA_BASE(realnum), &statex);
    state = inpw(TA_LCW(realnum)); // DrvIOEpi, DrvIOPro
    state &= ((__tmkOptions[__tmkNumber] & (UOPT)RT_SHOW_BRC_STATE) && ((state & 0xF800) == 0xF800)) ? 0x87FF : 0x07FF;
    REST_IRQ();
    if (((statex & 0x8000) == 0) && ((statex & 0x0FFF) == (DrvRtGetBaseMRTA(__tmkNumber) & 0x0FFF)))
      state |= (statex & 0x2000) >> 2;
    break;
#endif
  }
  DrvIOEpi;
  return state;
}
#endif //NRT

U16 FARFN rtgetstate(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
#if NRT > 0
  int num;
#endif
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned port;
  unsigned state=0, statex;
  int limit;
#ifdef MRTX
  unsigned pos;
#endif
#ifdef MRTA
  unsigned stateb;
#endif
  IRQ_FLAGS;

  DrvIOPro(0, 0, 0, 0, 2, 0);
  CLRtmkError;
#if NRT > 0
  num = __tmkNumber;
#endif
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  port = TMK_State(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
  case __TMKMPC:
    port = TMKMPC_StateL(realnum);
    fallthrough;
    // fall-through
  case __TMK400:
  case __RTMK400:
    state = inpw_d(realnum, port) & 0x5FFF;
    break;
#ifdef MRTX
  case __MRTX:
    if (__rtMode[num] != __rtMode[realnum] && (__rtMode[realnum] & 0x0800) == 0)
      // if different (switched rt) and unlocked mode
      DrvRtWMode(num, __MRTX, __rtMode[num]);
    limit = 16; // abstract loop limit for broken hardware
    state = inpw(port);
    do
    {
      statex = state;
      state = inpw(port);
    }
    while (statex != state && --limit != 0);
    if ((state & 0x8000) == 0)
    {
      if (((state >> 1) & 0x1800) != __hm400Page[num])
      {
        pos = __hm400Page[num] | 0x07F2;
        GET_DIS_IRQ();
        outpw(TMK_Addr(realnum), pos);
        state = inpw(TMK_Data(realnum));
        REST_IRQ();
        state &= 0x07FF;
      }
    }
    state &= 0xCFFF;
    // always RT_SHOW_BRC_STATE here for backward compatibility
    break;
#endif
  case __TMKX:
    limit = 16; // abstract loop limit for broken hardware
    state = inpw(port);
    do
    {
      statex = state;
      state = inpw(port);
    }
    while (statex != state && --limit != 0);
    state &= 0x7FFF;
    break;
  case __TA:
    GET_DIS_IRQ();
//    state = inpw(TA_LCW(realnum)) & 0x7FF;
    inptrwu(TA_BASE(realnum), &statex);
//    if (statex & 0x2000)
      state = inpw(TA_LCW(realnum)); // DrvIOEpi, DrvIOPro
    REST_IRQ();
    state &= ((__tmkOptions[realnum] & (UOPT)RT_SHOW_BRC_STATE) && ((state & 0xF800) == 0xF800)) ? 0x87FF : 0x07FF;
    if (((statex & 0x8000) == 0) && ((statex & 0x03FF) == (DrvRtGetBaseTA(realnum) & 0x03FF)))
      state |= (statex & 0x2000) >> 2;
    break;
#ifdef MRTA
  case __MRTA:
    stateb = 0;
    GET_DIS_IRQ();
    if (__rtPagePC[num] >  __rtMaxPage[num])
    {
      outpw(TA_LCW(realnum), 0xF800);
      inptrwu(TA_BASE(realnum), &stateb);
    }
// otherwise no need to check brc part because it will not be == DrvRtGetBaseMRTA
    outpw(TA_LCW(realnum), __rtControls1[num] & 0xF810); // use TA_BRCST_MODE (if set) for __tmkHwVer >= 6
    inptrwu(TA_BASE(realnum), &statex);
    state = inpw(TA_LCW(realnum));// & 0x7FF; // DrvIOEpi, DrvIOPro
    REST_IRQ();
//rt-rt brc messages with selected RT as Tx may need special attention!!!
    if (stateb != 0 &&              // selected brc page and
        (state & 0xF800) == 0xF800) // this RT processed Brc msg as Rx
      statex = stateb;
//    if ((state & 0xF800) != (__rtControls1[num] & 0xF800))
//      statex &= ~0x2000; // active another RT
    state &= ((__tmkOptions[num] & (UOPT)RT_SHOW_BRC_STATE) && ((state & 0xF800) == 0xF800)) ? 0x87FF : 0x07FF;
    if (((statex & 0x8000) == 0) && ((statex & 0x0FFF) == (DrvRtGetBaseMRTA(num) & 0x0FFF)))
      state |= (statex & 0x2000) >> 2;
    break;
#endif
  }
  DrvIOEpi;
  return state;
}

void FARFN rtlock(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Dir, U16 SubAddr)
{
  int register num;
  unsigned type;
  unsigned sa;
  unsigned dir;

  DrvIOPro(0, 0, 0, 2, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  sa = SubAddr;
  CHECK_RT_SUBADDR(sa);
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  sa <<= 5;
  dir = Dir;
  CHECK_RT_DIR(dir);
  sa |= dir;
  DrvRtDefSubAddr(num, type, sa);
  DrvRtWMode(num, type, sa | 0x0800);
  DrvIOEpi;
  return;
}

void FARFN rtunlock(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned type;

  DrvIOPro(0, 0, 0, 2, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  DrvRtWMode(num, type, __rtSubAddr[num]);
  DrvIOEpi;
  return;
}

U16 FARFN rtgetlock(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned mode;

  CLRtmkError;
  num = __tmkNumber;   //; tmkRealNumber2 ???
  mode = __rtMode[num];
  return (mode & 0xFC00) | ((mode >> 5) & 0x001F);
}

U16 FARFN rtgetcmddata(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 BusCommand)
{
  int register num;
  unsigned type;
  DEF_VAR(unsigned, cmd);
  unsigned data;

  DrvIOPro(0, 0, 0, 0, 0, 1);
  CLRtmkError;
  num = __tmkNumber;
  GET_VAR(cmd, BusCommand);
  CHECK_RT_CMD(cmd);
  cmd |= 0x03E0;
  data = 0;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
#ifdef MRTX
  case __MRTX:
    cmd |= 0x0400; //; 7F0h
    // fall-through
#endif
    fallthrough;
    // fall-through
  case __TMK400:
  case __RTMK400:
  case __TMKMPC:
  case __TMKX:
    data = DrvRtPeek(num, type, cmd);
    break;
  case __TA:
// need to mantain an array of cmddatas for full compatibility
    switch (cmd)
    {
    case 0x7F0: //0x410
      GET_DIS_IRQ_SMP();
      data = DrvRtPeekTA(num, 0x20, 0);
      REST_IRQ_SMP();
      break;
    case 0x7F3: //0x413
      GET_DIS_IRQ_SMP();
      data = DrvRtPeekTA(num, 0x3F, 0);
      REST_IRQ_SMP();
      break;
    case 0x3F1: //0x011
    case 0x3F4: //0x014
    case 0x3F5: //0x015
#ifndef DrvIO
      if (!__tmkIrqOff[num])
        data = __rtRxDataCmd[num][cmd-0x3F1];
      else
      {
// simplified approach for very last cmd only or 'syn w data' only
        GET_DIS_IRQ_SMP();
        data = DrvRtPeekTA(num, 0x1F, 0);
        REST_IRQ_SMP();
      }
#else
// simplified approach for very last cmd only or 'syn w data' only
// full set requires extra DrvIO support !!!
      GET_DIS_IRQ_SMP();
      data = DrvRtPeekTA(num, 0x1F, 0);
      REST_IRQ_SMP();
#endif //ndef DrvIO
      break;
    }
    break;
#ifdef MRTA
  case __MRTA:
// need to mantain an array of cmddatas for full compatibility
    {
    int register realnum;
    realnum = GET_RealNum;
    switch (cmd)
    {
    case 0x7F0: //0x410
      GET_DIS_IRQ_SMP();
      data = DrvRtPeekMRTA(realnum, (__hm400Page0[num] << 6) | 0x20, 0, 0xFFFF);
      REST_IRQ_SMP();
      break;
    case 0x7F3: //0x413
      GET_DIS_IRQ_SMP();
      data = DrvRtPeekMRTA(realnum, (__hm400Page0[num] << 6) | 0x3F, 0, 0xFFFF);
      REST_IRQ_SMP();
      break;
    case 0x3F1: //0x011
    case 0x3F4: //0x014
    case 0x3F5: //0x015
#ifndef DrvIO
      if (!__tmkIrqOff[realnum])
        data = __rtRxDataCmd[num][cmd-0x3F1];
      else
      {
// simplified approach for very last cmd only or 'syn w data' only
        GET_DIS_IRQ_SMP();
        data = DrvRtPeekMRTA(realnum, (__hm400Page0[num] << 6) | 0x1F, 0, 0xFFFF);
        REST_IRQ_SMP();
      }
#else
// simplified approach for very last cmd only or 'syn w data' only
// full set requires extra DrvIO support !!!
      GET_DIS_IRQ_SMP();
      data = DrvRtPeekMRTA(realnum, (__hm400Page0[num] << 6) | 0x1F, 0, 0xFFFF);
      REST_IRQ_SMP();
#endif //ndef DrvIO
      break;
    }
    break;
    }
#endif
  }
  DrvIOEpi;
  return data;
}

void FARFN rtputcmddata(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 BusCommand, U16 rtData)
{
  int register num;
  unsigned type;
  DEF_VAR(unsigned, cmd);

  DrvIOPro(0, 0, 1, 0, 0, 0);
  CLRtmkError;
  num = __tmkNumber;
  GET_VAR(cmd, BusCommand);
  CHECK_RT_CMD(cmd);
  cmd |= 0x03E0;        //;or      ax, 7F0h
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
#ifdef MRTX
  case __MRTX:
    cmd |= 0x0400; //; 7F0h
    // fall-through
#endif
    fallthrough;
    // fall-through
  case __TMK400:
  case __RTMK400:
  case __TMKMPC:
  case __TMKX:
    DrvRtPoke(num, type, cmd, rtData);
    break;
  case __TA:
// need to mantain an array of cmddatas for full compatibility
    switch (cmd)
    {
    case 0x7F0: //0x410
      GET_DIS_IRQ_SMP();
      DrvRtPokeTA(num, 0x20, 0, rtData);
      REST_IRQ_SMP();
      break;
    case 0x7F3: //0x413
      GET_DIS_IRQ_SMP();
      DrvRtPokeTA(num, 0x3F, 0, rtData);
      REST_IRQ_SMP();
      break;
    case 0x3F1: //0x011
    case 0x3F4: //0x014
    case 0x3F5: //0x015
#ifndef DrvIO
      __rtRxDataCmd[num][cmd-0x3F1] = rtData;
      if (__tmkIrqOff[num])
      {
// simplified approach for very last cmd only or 'syn w data' only
        GET_DIS_IRQ_SMP();
        DrvRtPokeTA(num, 0x1F, 0, rtData);
        REST_IRQ_SMP();
      }
#else
// simplified approach for very last cmd only or 'syn w data' only
// full set requires extra DrvIO support !!!
      GET_DIS_IRQ_SMP();
      DrvRtPokeTA(num, 0x1F, 0, rtData);
      REST_IRQ_SMP();
#endif
      break;
    }
    break;
#ifdef MRTA
  case __MRTA:
// need to mantain an array of cmddatas for full compatibility
    {
    int register realnum;
    realnum = GET_RealNum;
    switch (cmd)
    {
    case 0x7F0: //0x410
      GET_DIS_IRQ_SMP();
      DrvRtPokeMRTA(realnum,(__hm400Page0[num] << 6) | 0x20, 0, rtData, 0xFFFF);
      REST_IRQ_SMP();
      break;
    case 0x7F3: //0x413
      GET_DIS_IRQ_SMP();
      DrvRtPokeMRTA(realnum,(__hm400Page0[num] << 6) | 0x3F, 0, rtData, 0xFFFF);
      REST_IRQ_SMP();
      break;
    case 0x3F1: //0x011
    case 0x3F4: //0x014
    case 0x3F5: //0x015
#ifndef DrvIO
      __rtRxDataCmd[num][cmd-0x3F1] = rtData;
      if (__tmkIrqOff[realnum])
      {
// simplified approach for very last cmd only or 'syn w data' only
        GET_DIS_IRQ_SMP();
        DrvRtPokeMRTA(realnum, (__hm400Page0[num] << 6) | 0x1F, 0, rtData, 0xFFFF);
        REST_IRQ_SMP();
      }
#else
// simplified approach for very last cmd only or 'syn w data' only
// full set requires extra DrvIO support !!!
      GET_DIS_IRQ_SMP();
      DrvRtPokeMRTA(realnum, (__hm400Page0[num] << 6) | 0x1F, 0, rtData, 0xFFFF);
      REST_IRQ_SMP();
#endif
      break;
    }
    break;
    }
#endif
  }
  DrvIOEpi;
  return;
}

void DrvRtWMode(int __tmkNumber, unsigned type, unsigned mode)
{
  int num;
  int register realnum;
  LOCAL_PORT
  unsigned sa;
  unsigned modep;
  unsigned sap;
  IRQ_FLAGS;

  num = __tmkNumber;
  realnum = GET_RealNum;
  if (mode == __rtMode[realnum])
  {
#if NRT > 0
    switch (type)
    {
#ifdef MRTA
    case __MRTA:
      break;
#endif
#ifdef MRTX
    case __MRTX:
      __rtMode[num] = mode;
#endif
    fallthrough;
    // fall-through
    default:
      return;
    }
#else
      return;
#endif
  }
  SET_LOCAL_PORT(realnum);
  modep = __rtMode[num];
  __rtMode[num] = mode;
  switch (type)
  {
  case __TMK400:
    GET_DIS_IRQ_SMP();
    outpw_d(num, TMK_Ctrl(num), mode | __rtAddress[num] | __rtDisableMask[num]);
    REST_IRQ_SMP();
    return;
  case __RTMK400:
    GET_DIS_IRQ_SMP();
    outpw_d(num, TMK_Ctrl(num), mode);
    REST_IRQ_SMP();
    return;
  case __TMKMPC:
//    GET_DIS_IRQ_SMP(); !!! GET_DIS_IRQ in outpwb_d
    outpwb_d(num, TMKMPC_CtrlH(num), mode);
//    REST_IRQ_SMP();
    return;
#ifdef MRTX
  case __MRTX:
    __rtMode[realnum] = mode;
    mode >>= 5;
    if ((mode & 0x1F) == 0 || (mode & 0x1F) == 0x1F)
      mode = 1;
    GET_MUTEX;
    mode |= __rtControls1[realnum] & 0x0500;
    __rtControls1[realnum] = mode;
    REST_MUTEX;
    mode &= __rtBRCMask[realnum];
    GET_DIS_IRQ_SMP();
    outpw(TMK_Ctrl(realnum), mode);
    outpw(TMK_Ctrl(realnum), mode); // PCI2
    REST_IRQ_SMP();
    return;
#endif
  case __TMKX:
    mode >>= 5;
    GET_MUTEX;
    mode |= __rtControls1[num] & 0xFD00;
    __rtControls1[num] = mode;
    REST_MUTEX;
    mode = (mode & __rtBRCMask[num]) | __rtDisableMask[num];
    GET_DIS_IRQ_SMP();
    outpw(TMK_Ctrl(num), mode);
    outpw(TMK_Ctrl(num), mode); // PCI2
    REST_IRQ_SMP();
    return;
  case __TA:
//  repeats the check at beginning as num == realnum
//    if (mode == modep)
//      return;
    if (__FLAG_MODE[num] & DRV_FLAG_MODE_SW)
      return;
    if (modep & 0x0800) // there was another lock (mode != oldmode)
    { // clear old lock
      sap = (modep >> 5) & 0x3F;
//      if (__rtSABufMode[num][sap] != SABUF_OFF)
//        return;
      GET_DIS_IRQ_SMP();
      DrvPeekAndOrPokeTA(num, AdrTab, sap, 0x7FFF, 0);
      REST_IRQ_SMP();
    }
    if (mode & 0x0800)
    { // set new lock if required
      sa = (mode >> 5) & 0x3F;
#ifdef USE_TMK_WRAP_SA
      if (__tmkWrapSA)
      {
        if ((sa & 0x1F) == __tmkWrapSA)
        {
          __rtMode[num] = mode & 0xF7FF;
          return;
        }
      }
#endif
//      if (__rtSABufMode[num][sa] != SABUF_OFF)
//        return;
      GET_DIS_IRQ_SMP();
      DrvPeekAndOrPokeTA(num, AdrTab, sa, 0xFFFF, 0x8000);
      REST_IRQ_SMP();
    }
    return;
#ifdef MRTA
  case __MRTA:
    __rtMode[realnum] = mode; // no single mode requirement for whole MRTA
//  otherwise need to unlock/lock based on realnum instead of num below
//  the assignment is only needed to pass: if (mode == __rtMode[realnum])
    if (mode == modep)
      return;
    if (__FLAG_MODE[num] & DRV_FLAG_MODE_SW)
      return;
    if (modep & 0x0800) // there was another lock (mode != oldmode)
    { // clear old lock
      sap = (modep >> 5) & 0x3F;
      GET_DIS_IRQ_SMP();
      outpw(MRTA_ADDR2(realnum), 0);
      DrvPeekAndOrPokeTA(realnum, __rtAddress[num], sap, 0x7FFF, 0);
      REST_IRQ_SMP();
    }
    if (mode & 0x0800)
    { // set new lock if required
      sa = (mode >> 5) & 0x3F;
#ifdef USE_TMK_WRAP_SA
      if (__tmkWrapSA)
      {
        if ((sa & 0x1F) == __tmkWrapSA)
        {
          __rtMode[realnum] = __rtMode[num] = mode & 0xF7FF;
          return;
        }
      }
#endif
      GET_DIS_IRQ_SMP();
      outpw(MRTA_ADDR2(realnum), 0);
      DrvPeekAndOrPokeTA(realnum, __rtAddress[num], sa, 0xFFFF, 0x8000);
//      outpw(MRTA_ADDR2(realnum), __hm400Page2[num]);
      REST_IRQ_SMP();
    }
    return;
#endif
  }
  return;
}

void DrvRtPoke(int __tmkNumber, unsigned type, unsigned pos, unsigned data)
// not for TA/MRTA boards!
// write to pos in current page, update current subaddr
// doesn't suppose subsequent use of addr, so doesn't increment __tmkRAMAddr
{
  int num;
  int register realnum;
  LOCAL_PORT
  unsigned save_rama, save_ramiw;
  IRQ_FLAGS;

  num = __tmkNumber;
  __rtSubAddr[num] = pos & 0x07E0;
  pos |= __hm400Page[num];
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    outpw_d(realnum, TMK_Addr(realnum), pos >> 6);
    outpw_d(realnum, TMK_Data(realnum), data);
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    outpb_d(realnum, TMKMPC_AddrH(realnum), pos >> 6);
    outpwb_d(realnum, TMKMPC_DataH(realnum), data);
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
#ifdef MRTX
  case __MRTX:
#endif
  case __TMKX:
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    outpw(TMK_Data(realnum), data);
    REST_IRQ_SMP();
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
//  case __TA:
//  ???
//    break;
//  case __MRTA:
//  ???
//    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  return;
}

unsigned DrvRtPeek(int __tmkNumber, unsigned type, unsigned pos)
// not for TA/MRTA boards!
// read from pos in current page, update current subaddr
// doesn't suppose subsequent use of addr, so doesn't increment __tmkRAMAddr
{
  int num;
  int register realnum;
  unsigned data=0;
  LOCAL_PORT
  unsigned save_rama, save_ramiw;

  num = __tmkNumber;
  __rtSubAddr[num] = pos & 0x07E0;
  pos |= __hm400Page[num];
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  save_rama = __tmkRAMAddr[realnum];
  save_ramiw = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 1;
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMK_Addr(realnum), pos);
    outpw_d(realnum, TMK_Addr(realnum), pos >> 6);
    data = inpw_d(realnum, TMK_Data(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMK_Addr(realnum), save_rama);
    break;
  case __TMKMPC:
    __tmkRAMAddr[realnum] = pos;
    outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
    outpb_d(realnum, TMKMPC_AddrH(realnum), pos >> 6);
    data = inpw_d(realnum, TMKMPC_DataL(realnum));
//    data = inpwb_d(TMKMPC_DataL(realnum));
    if (!save_ramiw)
      break;
    __tmkRAMAddr[realnum] = save_rama;
    outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
    break;
#ifdef MRTX
  case __MRTX:
#endif
  case __TMKX:
// TMKX wouldn't require GET_DIS_IRQ_SMP/REST_IRQ_SMP
// TA/MRTA wouldn't require __tmkRAMAddr
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = pos;
    outpw(TMK_Addr(realnum), pos);
    data = inpw(TMK_Data(realnum));
    REST_IRQ_SMP();
    if (!save_ramiw)
      break;
    GET_DIS_IRQ_SMP();
    __tmkRAMAddr[realnum] = save_rama;
    outpw(TMK_Addr(realnum), save_rama);
    REST_IRQ_SMP();
    break;
//  case __TA:
//  ???
//    break;
//  case __MRTA:
//  ???
//    break;
  }
  if (!save_ramiw)
    __tmkRAMInWork[realnum] = 0;
  return data;
}

#ifdef MRTX
void DrvRtPokeIrqMRTX(int __tmkNumber, unsigned pos, unsigned data)
{
// Function should be called in non-reentrant disabled interrupt environment
// See also: Note on MRTX SMP RAM read/write processing
  int num;
  int register realnum;
  LOCAL_PORT
//  unsigned save_rama, save_ramiw;

  num = __tmkNumber;
  pos |= __hm400Page[num];
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
//  save_rama = __tmkRAMAddr[realnum];
//  save_ramiw = __tmkRAMInWork[realnum];
//  __tmkRAMInWork[realnum] = 1;
//  __tmkRAMAddr[realnum] = pos;
  outpw(TMK_Addr(realnum), pos);
  outpw(TMK_Data(realnum), data);
//  if (!save_ramiw)
//    __tmkRAMInWork[realnum] = 0;
//  else
//  {
//    __tmkRAMAddr[realnum] = save_rama;
//    outpw(TMK_Addr(realnum), save_rama);
//  }
  if (__tmkRAMInWork[realnum])
    outpw(TMK_Addr(realnum), __tmkRAMAddr[realnum]);
  return;
}

unsigned DrvRtPeekIrqMRTX(int __tmkNumber, unsigned pos)
{
// Function should be called in non-reentrant disabled interrupt environment
// See also: Note on MRTX SMP RAM read/write processing
  int num;
  int register realnum;
  unsigned data;
  LOCAL_PORT
//  unsigned save_rama, save_ramiw;

  num = __tmkNumber;
  pos |= __hm400Page[num];
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
//  save_rama = __tmkRAMAddr[realnum];
//  save_ramiw = __tmkRAMInWork[realnum];
//  __tmkRAMInWork[realnum] = 1;
//  __tmkRAMAddr[realnum] = pos;
  outpw(TMK_Addr(realnum), pos);
  data = inpw(TMK_Data(realnum));
//  if (!save_ramiw)
//    __tmkRAMInWork[realnum] = 0;
//  else
//  {
//    __tmkRAMAddr[realnum] = save_rama;
//    outpw(TMK_Addr(realnum), save_rama);
//  }
  if (__tmkRAMInWork[realnum])
    outpw(TMK_Addr(realnum), __tmkRAMAddr[realnum]);
  return data;
}
#endif //def MRTX

#if defined(DOS) || defined(QNX4)
#if defined(MRTA) || defined(USE_TMKRAWIO)
int SetupPCI(int REGPAR num, unsigned *pdid, unsigned *psubdid)
#else
int SetupPCI(int REGPAR num)
#endif //MRTARAWIO
{
  unsigned subdev, subdev1;
  int idev, idid;
  LOCAL_PORT
  unsigned port, portplx;
  unsigned data;
  unsigned did;
  unsigned subdid;
  int i, i0, i1, j;
  int cdevs;
// if this will be global we can run pcifind loop on the first call only
  __TPciBusDevFun apciBusDevFun[MAX_PCI_SLOTS];

  if (!pciPciBiosPresent())
    return USER_ERROR(TMK_PCI_ERROR);

  cdevs = 0;
  for (idid = 0; idid <= TMK_MAX_IDID; ++idid)
  {
    idev = 0;
    while (pciFindPciDevice(__tmkaDID[idid], __tmkaVID[idid], idev++) != 0)
    {
      if (pciReadConfigWord(__pciBusDevFun, CFG_SUB_VID) == ELCUS_VID)
      {
        subdid = pciReadConfigWord(__pciBusDevFun, CFG_SUB_DID);
        switch (subdid)
        {
        case TX1PCI_ID:
        case TX6PCI_ID:
        case TX1PCI2_ID:
        case TX6PCI2_ID:
        case TA1PCI_ID:
        case TA1PCI4_ID:
        case TA1PE2_ID:
        case TA1PE4_ID:
        case TA1MPE2_ID:
        case TA1NPCI_ID:
        case TA1NPE_ID:
        case TA13U2R_ID:
        case TA1P_ID:
        case TA1BPE_ID:
        case TA2PE4_ID:
        case TA2P_ID:
        case TA2BPE_ID:
#ifdef MRTA
        case TA1PCI32RT_ID:
        case TA1PE32RT_ID:
        case TA1P32RT_ID:
        case TA1P32RTE_ID:
        case TA1P32RTF_ID:
        case TA1P32RTV_ID:
        case TA1PE432RT_ID:
        case TA1PE432RTE_ID:
        case TA1PE432RTF_ID:
        case TA1BPE32RT_ID:
        case TA1BPE32RTE_ID:
        case TA1BPE32RTF_ID:
        case TA1BPE32RTV_ID:
        case TA2P32RT_ID:
        case TA2P32RTE_ID:
        case TA2P32RTF_ID:
        case TA2P32RTV_ID:
        case TA2PE432RT_ID:
        case TA2PE432RTE_ID:
        case TA2PE432RTF_ID:
        case TA2BPE32RT_ID:
        case TA2BPE32RTE_ID:
        case TA2BPE32RTF_ID:
        case TA2BPE32RTV_ID:
#endif
          break;
        default:
          continue;
        }
        i0 = 0;
        i1 = cdevs;
        for (i = i0; i < i1; ++i)
        {
          if (pciCmpBusDevFun(&__pciBusDevFun, &(apciBusDevFun[i])) < 0)
            break;
        }
        if (cdevs < MAX_PCI_SLOTS)
        {
          ++cdevs;
        }
        for (j = cdevs - 1; j > i; --j)
        {
          apciBusDevFun[j] = apciBusDevFun[j-1];
        }
        if (i < cdevs)
          apciBusDevFun[i] = __pciBusDevFun;
      } // if elcus
    } // while pcifind
  } // for idid

  if (__tmkPortsAddr1[num] == 0 || __tmkPortsAddr1[num] > cdevs)
    return USER_ERROR(TMK_BAD_NUMBER);
  __pciBusDevFun = apciBusDevFun[__tmkPortsAddr1[num] - 1];
  did = pciReadConfigWord(__pciBusDevFun, CFG_DID);
  subdid = pciReadConfigWord(__pciBusDevFun, CFG_SUB_DID);
#if defined(MRTA) || defined(USE_TMKRAWIO)
  *pdid = did;
  *psubdid = subdid;
#endif //MRTARAWIO

  subdev = __tmkPortsAddr2[num];
  if (subdev == 0xFFFF)
    subdev = 1;
  if (subdev < 1 || subdev > 4)
    return USER_ERROR(TMK_BAD_NUMBER);
  if (subdid == TA13U2R_ID && subdev > 2)
    return USER_ERROR(TMK_BAD_NUMBER);
  subdev1 = subdev;
  if (did == PLX_DID1)
  {
    switch (subdid)
    {
    case TA1PE4_ID:
    case TA1BPE_ID:
    case TA2PE4_ID:
    case TA2BPE_ID:
#ifdef MRTA
    case TA1PE432RT_ID:
    case TA1PE432RTE_ID:
    case TA1PE432RTF_ID:
    case TA1BPE32RT_ID:
    case TA1BPE32RTE_ID:
    case TA1BPE32RTF_ID:
    case TA1BPE32RTV_ID:
    case TA2PE432RT_ID:
    case TA2PE432RTE_ID:
    case TA2PE432RTF_ID:
    case TA2BPE32RT_ID:
    case TA2BPE32RTE_ID:
    case TA2BPE32RTF_ID:
    case TA2BPE32RTV_ID:
#endif
      subdev1 = ((subdev1 - 1) >> 1) + 1;
      break;
    }
  }
  port = pciReadConfigWord(__pciBusDevFun, CFG_ADDR1 + (subdev1<<2)) & 0xFFFE;
  if (port == 0)
    return USER_ERROR(TMK_BAD_NUMBER);
  switch (did)
  {
  case PLX_DID0:
  case PLX_DID1:
    portplx = pciReadConfigWord(__pciBusDevFun, CFG_ADDR1) & 0xFFFE;
    break;
  default:
    portplx = 0xFFFF;
    break;
  }
  if (did == PLX_DID0)
  {
    inpandoroutpw(portplx + 0x50, 0xFFFF, 0x4000); //Fix 9030
  }
  if (did == PLX_DID1)
  {
    pciWriteConfigWord(__pciBusDevFun, CFG_COMMAND, pciReadConfigWord(__pciBusDevFun, CFG_COMMAND) | 0x0003);
    switch (subdid)
    {
    case TA1PE4_ID:
    case TA2PE4_ID:
#ifdef MRTA
    case TA1PE432RT_ID:
    case TA1PE432RTE_ID:
    case TA2PE432RT_ID:
    case TA2PE432RTE_ID:
#endif
      if ((subdev == 2 && ((inpw(portplx) & 0xFF) != 0xC1)) ||
          (subdev == 4 && ((inpw(portplx + 0xF0) & 0xFF) != 0xC1)))
        return USER_ERROR(TMK_BAD_NUMBER);
      port += 32 * ((subdev - 1) & 0x1);
      break;
    case TA1BPE_ID:
    case TA2BPE_ID:
#ifdef MRTA
    case TA1PE432RTF_ID:
    case TA1BPE32RT_ID:
    case TA1BPE32RTE_ID:
    case TA1BPE32RTF_ID:
    case TA1BPE32RTV_ID:
    case TA2PE432RTF_ID:
    case TA2BPE32RT_ID:
    case TA2BPE32RTE_ID:
    case TA2BPE32RTF_ID:
    case TA2BPE32RTV_ID:
#endif
      if ((subdev == 2 && ((inpw(portplx) & 0xFF) != 0x81)) ||
          (subdev == 4 && ((inpw(portplx + 0xF0) & 0xFF) != 0x81)))
        return USER_ERROR(TMK_BAD_NUMBER);
      port += 64 * ((subdev - 1) & 0x1);
      break;
    }
    inpandoroutpw(portplx + 0x68, 0xFFFF, 0x0800); //Enable INTi#
    inpandoroutpw(portplx + 0x6A, 0xFFFE, 0);      //Disable INTo#
  }
  __tmkPortsAddr1[num] = port;
  __tmkPortsAddr2[num] = portplx;
  __tmkIrq1[num] = pciReadConfigByte(__pciBusDevFun, CFG_IRQ);
  SET_LOCAL_PORT(num);
  GET_DIS_IRQ();
  outpw(TMK_Reset(num), 0);
  data = inpw(TMK_Addr(num));
  outpw(TMK_Addr(num), ~data);
  data ^= inpw(TMK_Addr(num));
  REST_IRQ();
  outpw(TMK_Addr(num), 0);
  if (data == 0xFFFF)
  {
  // IrqTmk for TA1 (PCI,PCIe)
    __tmkIrqPort[num] = TA_RESET(num);
    __tmkIrqBit[num] = 0xE000;
  }
  else if (data == 0x3FFF)
  {
  // IrqTmk for TE1-PCI2
    __tmkIrqPort[num] = TMK_Addr(num);
    __tmkIrqBit[num] = 0x8000;
  }
  else //if (data == 0 || data == 0x7FFF)
  {
  // IrqPlx for TE1-PCI
    __tmkIrqPort[num] = __tmkPortsAddr2[num] + 0x4C;
    __tmkIrqBit[num] = 0x0004;
  }

  return 0;
}
#endif //def DOS | QNX4

int ExamTmkRAM(int REGPAR num, unsigned type)
{
  unsigned pos, size;
  LOCAL_PORT
  unsigned save_idelay, save_odelay;

#if DRV_MAX_BASE > 255
  __mtMaxBase[num] = 0;
#endif
  __mtMaxMaxBase[num] = 0;
  SET_LOCAL_PORT(num);
  switch (type)
  {
  case __TMK400:
//    __tmkRAMSize[num] = 8;
    __bcMaxBase[num] = 127;
    __rtMaxPage[num] = 3;
    break;
  case __RTMK400:
    save_idelay = __wInDelay[num];
    save_odelay = __wOutDelay[num];
    __wInDelay[num] = 32;
    __wOutDelay[num] = 32;
    outpw_d(num, TMK_Addr(num), 60 >> 6);
    outpb_d(num, TMK_Addr(num), 60);
    outpw_d(num, TMK_Data(num), 0);
    size = 0x0800;
    do
    {
      pos = size + 60;
      outpw_d(num, TMK_Addr(num), pos >> 6);
      outpb_d(num, TMK_Addr(num), pos);
      outpw_d(num, TMK_Data(num), pos);
      pos -= size;
      outpw_d(num, TMK_Addr(num), pos >> 6);
      outpb_d(num, TMK_Addr(num), pos);
      if (inpw_d(num, TMK_Data(num)) != 0)
        break;
      size <<= 1;
    }
    while ((size & 0x8000) == 0);
    size >>= 10;
//    __tmkRAMSize[num] = size;
    size <<= 4;
    --size;
    __bcMaxBase[num] = (size <= DRV_MAX_BASE) ? size : DRV_MAX_BASE;
    size >>= 5;
    __rtMaxPage[num] = size;
    __wOutDelay[num] = save_odelay;
    __wInDelay[num] = save_idelay;
    break;
  case __TMKMPC:
//    __tmkRAMSize[num] = 2;
    __bcMaxBase[num] = 31;
    __rtMaxPage[num] = 0;
    break;
#ifdef MRTX
  case __MRTX:
//    __tmkRAMSize[num] = 8;
    __bcMaxBase[num] = 0;
    __rtMaxPage[num] = 0;
    __mrtNRT[num] = 4;
    if (__mrtMaxNRT > 0 && __mrtMaxNRT < __mrtNRT[num])
      __mrtNRT[num] = __mrtMaxNRT;
    break;
#endif
  case __TMKX:
    outpw(TMK_Addr(num), 60);
    outpw(TMK_Data(num), 0);
    size = 0x0800;
    do
    {
      pos = size + 60;
      outpw(TMK_Addr(num), pos);
      outpw(TMK_Data(num), pos);
      pos -= size;
      outpw(TMK_Addr(num), pos);
      if (inpw(TMK_Data(num)) != 0)
        break;
      size <<= 1;
    }
    while ((size & 0x8000) == 0);
    size >>= 10;
//    __tmkRAMSize[num] = size;
    size <<= 4;
    --size;
#if DRV_MAX_BASE > 255
    __mtMaxBase[num] =
#endif
    __mtMaxMaxBase[num] =
    __bcMaxBase[num] = (size <= DRV_MAX_BASE) ? size : DRV_MAX_BASE;
    size >>= 5;
    __rtMaxPage[num] = size;
    break;
  case __TA:
//    __tmkRAMSize[num] = 64;
    __bcMaxBase[num] = (1023 <= DRV_MAX_BASE) ? 1023 : DRV_MAX_BASE;
#if DRV_MAX_BASE > 255
    __mtMaxBase[num] = (511 <= DRV_MAX_BASE) ? 511 : DRV_MAX_BASE;
#endif
    // leave base 1023 for AdrTab and base 1022 for MTA_STOP_BASE
    __mtMaxMaxBase[num] = (1021 <= DRV_MAX_BASE) ? 1021 : DRV_MAX_BASE;
    __rtMaxPage[num] = 0;
    break;
#ifdef MRTA
  case __MRTA:
//    __tmkRAMSize[num] = 256;
    __bcMaxBase[num] = 0;
    __rtMaxPage[num] = 0;
    __mrtNRT[num] = 31;
    if (__mrtMaxNRT > 0 && __mrtMaxNRT < __mrtNRT[num])
      __mrtNRT[num] = __mrtMaxNRT;
    break;
#endif
  }
  return 0;
}

#if defined(DOS) || defined(QNX4)

#ifdef DOS
#ifdef DOS32

#define LOCK_RAM 0x600
#define UNLOCK_RAM 0x601

#define GET_OLD_PM_VECTOR 1
#define SET_NEW_PM_VECTOR 2
#define GET_OLD_RM_VECTOR 4
#define SET_NEW_RM_VECTOR 8
#define ALLOCATE_RM_CALLBACK 16

int getvect_pm(unsigned intr, void (TYPIR __far **ProcA)(__CPPARGS))
{
  union REGS r;

  r.x.eax = 0x0204;
  r.h.bl = intr;
  int386(0x31, &r, &r);
  if (r.x.cflag)
    return TMK_DPMI_ERROR;
  *ProcA = MK_FP(r.x.ecx, r.x.edx);
  return 0;
}

int setvect_pm(unsigned intr, void (TYPIR __far *Proc)(__CPPARGS))
{
  union REGS r;

  r.x.eax = 0x0205;
  r.h.bl = intr;
  r.x.ecx = FP_SEG(*Proc);
  r.x.edx = FP_OFF(*Proc);
  int386(0x31, &r, &r);
  if (r.x.cflag)
    return TMK_DPMI_ERROR;
  return 0;
}

int getvect_rm(unsigned intr, U16 *rmProcA)
{
  union REGS r;

  r.x.eax = 0x0200;
  r.h.bl = intr;
  int386(0x31, &r, &r);
  if (r.x.cflag)
    return TMK_DPMI_ERROR;
  rmProcA[0] = r.w.dx;
  rmProcA[1] = r.w.cx;
  return 0;
}

int setvect_rm(unsigned intr, U16 *rmProcA)
{
  union REGS r;

  r.x.eax = 0x0201;
  r.h.bl = intr;
  r.w.dx = rmProcA[0];
  r.w.cx = rmProcA[1];
  int386(0x31, &r, &r);
  if (r.x.cflag)
    return TMK_DPMI_ERROR;
  return 0;
}

int alloc_rm_callback(void (TYPIR __far *ProcRM)(__CPPARGS), U16 *rmProcA)
{
  union REGS r;
  struct SREGS s;

  r.x.eax = 0x0303;
  s.ds = FP_SEG(*ProcRM);
  r.x.esi = FP_OFF(*ProcRM);
  s.es = FP_SEG(&__rmCallStruc);
  r.x.edi = FP_OFF(&__rmCallStruc);
  int386x(0x31, &r, &r, &s);
  if (r.x.cflag)
    return TMK_DPMI_ERROR;
  rmProcA[0] = r.w.dx;
  rmProcA[1] = r.w.cx;
  return 0;
}

int unalloc_rm_callback(U16 *rmProcA)
{
  union REGS r;

  r.x.eax = 0x0304;
  r.w.dx = rmProcA[0];
  r.w.cx = rmProcA[1];
  int386(0x31, &r, &r);
  if (r.x.cflag)
    return TMK_DPMI_ERROR;
  return 0;
}

#define DPMI_IRQ_OK (GET_OLD_PM_VECTOR | SET_NEW_PM_VECTOR | GET_OLD_RM_VECTOR | ALLOCATE_RM_CALLBACK | SET_NEW_RM_VECTOR)

int SetIrqAL(int REGPAR num, unsigned irq)
{
  irq = IRQ2INT(irq);
  __ftmkInt1[num]  = 0;
  if (getvect_pm(irq, &tmkOldInt1[num]) == 0)
    __ftmkInt1[num] |= GET_OLD_PM_VECTOR;
  if (setvect_pm(irq, tmkDrvInt1[num]) == 0)
    __ftmkInt1[num] |= SET_NEW_PM_VECTOR;
  if (getvect_rm(irq, rmtmkOldInt1[num]) == 0)
    __ftmkInt1[num] |= GET_OLD_RM_VECTOR;
  if (alloc_rm_callback(tmkDrvInt1RM[num], rmtmkCallbackInt1[num]) == 0)
    __ftmkInt1[num] |= ALLOCATE_RM_CALLBACK;
  if (setvect_rm(irq, rmtmkCallbackInt1[num]) == 0)
    __ftmkInt1[num] |= SET_NEW_RM_VECTOR;
  return (__ftmkInt1[num] == DPMI_IRQ_OK) ? 0 : TMK_DPMI_ERROR;
}

void SetIrqCopy(int REGPAR num, int nfrom)
{
  __ftmkInt1[num] = __ftmkInt1[nfrom];
  tmkOldInt1[num] = tmkOldInt1[nfrom];
  rmtmkOldInt1[num][0] = rmtmkOldInt1[nfrom][0];
  rmtmkOldInt1[num][1] = rmtmkOldInt1[nfrom][1];
}

void ResetIrqAL(int REGPAR num, unsigned irq)
{
  irq = IRQ2INT(irq);
  if (__ftmkInt1[num] & SET_NEW_PM_VECTOR)
  {
    setvect_pm(irq, tmkOldInt1[num]);
    __ftmkInt1[num] &= ~(GET_OLD_PM_VECTOR | SET_NEW_PM_VECTOR);
  }
  if (__ftmkInt1[num] & SET_NEW_RM_VECTOR)
  {
    setvect_rm(irq, rmtmkOldInt1[num]);
    __ftmkInt1[num] &= ~(GET_OLD_RM_VECTOR | SET_NEW_RM_VECTOR);
  }
  if (__ftmkInt1[num] & ALLOCATE_RM_CALLBACK)
  {
    unalloc_rm_callback(rmtmkCallbackInt1[num]);
    __ftmkInt1[num] &= ~ALLOCATE_RM_CALLBACK;
  }
}

#else //notDOS32

int SetIrqAL(int REGPAR num, unsigned irq)
{
  irq = IRQ2INT(irq);
  tmkOldInt1[num] = getvect(irq);
  setvect(irq, tmkDrvInt1[num]);
  return 0;
}

void SetIrqCopy(int REGPAR num, int nfrom)
{
  tmkOldInt1[num] = tmkOldInt1[nfrom];
}

void ResetIrqAL(int REGPAR num, unsigned irq)
{
  irq = IRQ2INT(irq);
  setvect(irq, tmkOldInt1[num]);
}

#endif //def DOS32
#endif //def DOS

int SetTmkIrqs(int REGPAR num)
{
  U08 irq;
  int i;
  int res = 0;

  irq = __tmkIrq1[num];
  CHECK_IRQ(irq);
  for (i = 0; i < NTMK; ++i)
  {
    if (i == num)
      continue;
//    if (__tmkDrvType[i] == 0xFFFF)
//      continue;
    if (irq == __tmkIrq1[i])
    {
#ifdef DOS
      SetIrqCopy(num, i);
#endif //def DOS
      for (i = 0; i < NTMK; ++i)
      {
//        if (__tmkDrvType[i] == 0xFFFF)
//          continue;
        if (irq != __tmkIrq1[i])
          continue;
        __tmkIrqShared[i] = 1;
      }
      return 0;
    }
  }
#ifdef DOS
  res = SetIrqAL(num, irq);
  if (!res)
    ENABLE_IRQ_AL(num, irq);
#endif //def DOS
#ifdef QNX4
#ifdef QNX4VME
  if (__vmeDev != -1)
    __tmkiid[num] = _VME_hint_attach(__vmeDev, __tmkIrqLevel[num], irq, tmkInterruptServiceRoutine, FP_SEG(&tmkError));
  else
    __tmkiid[num] = -1;
#else
  __tmkiid[num] = qnx_hint_attach(irq, tmkInterruptServiceRoutine, FP_SEG(&tmkError));
//  __tmkiid[num] = qnx_hint_attach(irq, tmkDrvInt1[num], FP_SEG(&tmkError));
#endif //def QNX4VME
  if (__tmkiid[num] == -1)
    res = TMK_BAD_IRQ;
#endif //def QNX4
  return res;
}

int ResetTmkIrqs(int REGPAR num)
{
  U08 irq;
  int i;

  irq = __tmkIrq1[num];
  CHECK_IRQ(irq);
  for (i = 0; i < NTMK; ++i)
  {
    if (i == num)
      continue;
//    if (__tmkDrvType[i] == 0xFFFF)
//      continue;
    if (irq == __tmkIrq1[i])
      break;
  }
  if (i == NTMK)
  {
#ifdef DOS
    DISABLE_IRQ_AL(num, irq);
    ResetIrqAL(num, irq);
#endif// def DOS
#ifdef QNX4
    if (__tmkiid[num] != -1)
#ifdef QNX4VME
      if (__vmeDev != -1)
        _VME_hint_detach(__vmeDev, irq);
#else
      qnx_hint_detach(__tmkiid[num]);
#endif //def QNX4VME
    __tmkiid[num] = -1;
#endif //def QNX4
  }
  __tmkIrq1[num] = 0xFF;
  __tmkIrqShared[num] = 0;
  return 0;
}

#endif //def DOS

#ifdef WIN95
//;                ALIGN   4
//;SetTmkIrqs      PROC    NEAR USES edi
//;                cli
//;                jmp     STI_Ok
//;                movzx   eax, byte ptr __tmkIrq1[ebx];
//;                CHECK_IRQ STI1
//;STI1_IrqOk:     mov     edi, tmkDrvInt1Desc[ebx*2];
//;                //;mov     [edi.VID_IRQ_Number], ax
//;                //;mov     [edi.VID_Hw_Int_Ref], ebx
//;                //;VxDcall VPICD_Virtualize_IRQ
//;                jc      STI_VPICD_Err
//;                mov     tmkIRQ1Handle[ebx*2], eax
//;//;                call    ENABLE_IRQ_AL
//;                //;VxDcall VPICD_Physically_Unmask
//;  type = __tmkDrvType[ebx];
//;  switch (type)    STI, edi
//;STI_RTMK:
//;                movzx   eax, byte ptr tmkIrq2[ebx];
//;                CHECK_IRQ STI2
//;STI2_IrqOk:     mov     edi, tmkDrvInt2Desc[ebx*2];
//;                //;mov     [edi.VID_IRQ_Number], ax
//;                //;mov     [edi.VID_Hw_Int_Ref], ebx
//;                //;VxDcall VPICD_Virtualize_IRQ
//;                jc      STI_VPICD_Err
//;                mov     tmkIRQ2Handle[ebx*2], eax
//;//;                call    ENABLE_IRQ_AL
//;                //;VxDcall VPICD_Physically_Unmask
//;                jmp     STI_Ok
//;
//;STI_VPICD_Err:  USER_ERROR(TMK_BAD_IRQ
//;                jmp     STI_Exit
//;
//;STI_RTMK1:
//;STI_TMK400:
//;STI_RTMK400:
//;STI_TMKMPC:
//;STI_TMKX:
//;
//;STI_Ok:         xor     eax, eax
//;STI1_Exit:
//;STI2_Exit:
//;STI_Exit:       sti
//;    return;
//;SetTmkIrqs      ENDP
//;
//;                ALIGN   4
//;ResetTmkIrqs    PROC    NEAR USES edi
//;                jmp     RTI_Ok
//;                movzx   eax, byte ptr __tmkIrq1[ebx];
//;                CHECK_IRQ RTI1
//;RTI1_IrqOk:     mov     eax, tmkIRQ1Handle[ebx*2];
//;                or      eax, eax
//;                jz      RTI1_Ok
//;//;                call    DISABLE_IRQ_AL
//;                //;VxDcall VPICD_Physically_Mask
//;                //;VxDcall VPICD_Force_Default_Behavior
//;                mov     tmkIRQ1Handle[ebx*2], 0
//;RTI1_Ok:
//;  type = __tmkDrvType[ebx];
//;  switch (type)    RTI, edi
//;RTI_RTMK:
//;                movzx   eax, byte ptr tmkIrq2[ebx];
//;                CHECK_IRQ RTI2
//;RTI2_IrqOk:     mov     eax, tmkIRQ2Handle[ebx*2];
//;                or      eax, eax
//;                jz      RTI2_Ok
//;//;                call    DISABLE_IRQ_AL
//;                //;VxDcall VPICD_Physically_Mask
//;                //;VxDcall VPICD_Force_Default_Behavior
//;                mov     tmkIRQ2Handle[ebx*2], 0
//;RTI2_Ok:
//;
//;RTI_RTMK1:
//;RTI_TMK400:
//;RTI_RTMK400:
//;RTI_TMKMPC:
//;RTI_TMKX:
//;
//;RTI_Ok:         xor     eax, eax
//;RTI1_Exit:
//;RTI2_Exit:
//;RTI_Exit:
//;    return;
//;ResetTmkIrqs    ENDP
#endif //def WIN95

void DrvInitNTMK(int i)
{
  __tmkTxRxMode[i] = TXRX_ALL_ENABLE;
  __tmkTimeOut[i] = 0;
  __tmkTimerCtrl[i] = 0;
#ifdef MRTA
  __dmrtaTimers[i] = 0L;
#endif
  __rtGap[i] = 0;
#if NRT > 0
  __dmrtRT[i] = 0L;
  __dmrtBrc[i] = 0L;
  __dmrtLgl[i] = 0L;
#endif //NRT
  __tmkRAMInWork[i] = 0;
  __tmkRAMAddr[i] = 0;
  __tmkStarted[i] = 0;
  __bcBus[i] = 0;
  __rtHeapMin[i] = 0;
  __rtHeapIni[i] = 0;
  __rtHeapAlloc[i] = 0; // or = 1; ?
  __tmkModeMtRt[i] = 0;
  __bcmtStateX[i] = 0;
}

void DrvInitNTMKNRT(int i)
{
  int sa;

  __tmkMode[i] = UNDEFINED_MODE;
//  DrvIOSetParTmk(IOPAR_TMKMODE, UNDEFINED_MODE, i); !!! DrvIO not initialised in DrvInitAll
  __rtControls[i] = 0;
  __rtControls1[i] = 0;
#ifdef MRTA
    if (i >= NTMK)
      __mrtRxCtrl[i] = 0;
#endif
  __rtAddress[i] = 0x00FF;
  __rtPagePC[i] = 0;
  __rtPageBus[i] = 0;
  __rtMode[i] = 0;
  __rtSubAddr[i] = 0;
  __hm400Page[i] = 0;
#ifdef MRTA
  __hm400Page0[i] = 0;
  __hm400Page2[i] = 0;
#endif
  __rtEnableOnAddr[i] = 1;
  __FLAG_MODE[i] = 0;
  __tmkOptions[i] = (UOPT)0x0L;
  __rtSABuf[i] = 0;
  for (sa = 0; sa < 64; ++sa)
  {
    __rtSABufMode[i][sa] = SABUF_OFF;
    __rtSABufSize[i][sa] = 1;
//    __rtSABufExt[i][sa] = 0; // not required for SABUF_OFF actually
  }
}

void DrvInitAll(void)
{
  int i;

  for (i = 0; i < NTMK; ++i)
  {
    DrvInitNTMK(i);
#ifdef DrvIO_DRVINITALL_1
    DrvIO_DRVINITALL_1; // should it be moved to DrvInitNTMK ?
#endif
#ifdef DOS32
    __ftmkInt1[i] = 0;
#endif //def DOS32
    __tmkHWVer[i] = 0;
#ifdef MRTA
    __mrtRxCtrl[i] = 0;
    __tmkBusErr[i] = 0;
#endif
//      fTmkEventSet[i] = 0;
    __wInDelay[i] = 1;
    __wOutDelay[i] = 1;
    __bcMaxBase[i] = 0;
#if DRV_MAX_BASE > 255
    __mtMaxBase[i] = 0;
#endif
    __mtMaxMaxBase[i] = 0;
#if NRT > 0
    __mrtMinRT[i] = 0;
    __mrtNRT[i] = 1;
#endif //NRT
#if defined(DOS) || defined(QNX4)
    __tmkPci[i] = 0;
    __tmkMask21[i] = 0;
#ifndef CPU188
    __tmkMaskA1[i] = 0;
#endif //ndef CPU188
    __tmkIrq1[i] = 0xFF;
    __tmkIrqShared[i] = 0;
    __tmkIrqPort[i] = 0;
    __tmkIrqBit[i] = 0;
    __tmkIrqOff[i] = 0;
#endif// def DOS
#if defined(DOS) || defined(QNX4INT)
    tmkUserNormBC[i] = retfRLabel3;
    tmkUserExcBC[i] = retfRLabel3;
    tmkUserSigBC[i] = retfLabel1;
    tmkUserXBC[i] = retfRLabel2;
    tmkUserABC[i] = retfRLabel2;
    tmkUserSigMT[i] = retfLabel1;
    tmkUserXMT[i] = retfRLabel2;
    tmkUserOthTMK[i] = retfLabel1;
#ifdef USE_TMKRAWIO
    tmkUserRawTMK[i] = retfLabel2;
    __tmkRawIrqIoLen[i] = 0;
#endif
#endif// def DOS || QNX4INT
#ifdef QNX4
    __tmkiid[i] = -1;
#endif //def QNX4
#ifdef QNX4VME
    __vmeDev = -1;
    __vmeWin = (void*) -1;
    __tmkIrqLevel[i] = 0;
#endif //def QNX4VME
  }
  for (i = 0; i < (NTMK + NRT); ++i)
  {
#ifdef STATIC_TMKNUM
    tmkError = 0;
#else
    tmkError[i] = 0;
#endif
    DrvInitNTMKNRT(i);
#if NRT > 0
    __amrtNumber[i] = 0;
#endif
    __tmkDrvType[i] = 0xFFFF;
    __tmkUserType[i] = 0xFFFF;
    __rtMaxPage[i] = 0;
#if defined(DOS) || defined(QNX4INT)
    tmkUserCmdRT[i] = retfRLabel1;
    tmkUserErrRT[i] = retfRLabel1;
    tmkUserDataRT[i] = retfRLabel1;
#endif //def DOS || QNX4INT
  }
#ifdef NMBCID
  for (i = 0; i < NMBCID; ++i)
  {
    __mbcAlloc[i] = 0;
  }
#endif //def NMBCID
#ifdef DOS
  DrvReadOldMask();
#endif
}

void DrvInitTmk(int REGPAR num, unsigned type)
{
  unsigned i;
  LOCAL_PORT
#ifdef DrvIO
  U16 aver[16];
#endif //def DrvIO
#ifdef QNX4VME
  unsigned led;
#endif //def QNX4VME

  SET_LOCAL_PORT(num);
  switch (type)
  {
  case __TMK400:
  case __RTMK400:
    GET_DIS_IRQ_SMP();
    outpb_d(num, TMK_Reset(num), 0);
    REST_IRQ_SMP();
    break;
  case __TMKMPC:
    GET_DIS_IRQ_SMP();
    outpb_d(num, TMKMPC_Reset(num), 0);
    REST_IRQ_SMP();
    break;
#ifdef MRTX
  case __MRTX:
#endif
#ifdef TSTX
  case __TSTX:
#endif
  case __TMKX:
    GET_DIS_IRQ_SMP();
    outpw(TMK_Reset(num), 0);
    REST_IRQ_SMP();
#ifdef QNX4VME
    outpw(TMKXV_Vect(num), (U16)__tmkIrq1[num]);
#endif //def QNX4VME
    break;
#ifdef MRTA
  case __MRTA:
    for (i = 0; i <= 31; ++i)
      outpw(MRTA_SW(num), i << 11);
    // fall-through
#endif
    fallthrough;
    // fall-through
  case __TA:
#ifdef TSTA
  case __TSTA:
#endif
    GET_DIS_IRQ_SMP();
    outpw(TA_RESET(num), 0);
#if NRT > 0
    __mrtLastBrcTxRT[num] = 0;
#endif
    outpw(TA_MODE1(num), TA_FIFO_RESET);
    outpw(TA_MODE1(num), 0);
#ifndef DrvIO
    for (i = 0; i < 16; ++i)
      __tmkHWVer[num] |= ((inpw(TA_IRQ(num)) & 0x1000) >> 12) << i;
#else
    for (i = 0; i < 16; ++i)
      inptrw(TA_IRQ(num), &aver[i]);
#endif //ndef DrvIO
    REST_IRQ_SMP();
    DrvIOEpi;
    DrvIOProV;
#ifdef DrvIO
    for (i = 0; i < 16; ++i)
      __tmkHWVer[num] |= ((aver[i] & 0x1000) >> 12) << i;
#endif //def DrvIO
#ifdef QNX4VME
    led = ((__tmkIrqLevel[num] & 7) << 12);
//    if (__tmkIrq1[num] & 1)
//      led |= 0x0400;
//    else
//      led |= 0x0800;
    outpw(TAV_Vect(num), (U16)(__tmkIrq1[num] | led));
#endif //def QNX4VME
    break;
  }
}

#if defined(MRTA) || defined(USE_TMKRAWIO)
void DrvInitDrvID(int REGPAR num, unsigned type, unsigned did, unsigned subdid)
{
#ifdef USE_TMKRAWIO
  unsigned port, port2;
#endif
#ifdef MRTA
  switch (subdid)
  {
  case TA1P32RTE_ID:
  case TA1P32RTF_ID:
  case TA1P32RTV_ID:
  case TA1PE432RTE_ID:
  case TA1PE432RTF_ID:
  case TA1BPE32RTE_ID:
  case TA1BPE32RTF_ID:
  case TA1BPE32RTV_ID:
  case TA2P32RTE_ID:
  case TA2P32RTF_ID:
  case TA2P32RTV_ID:
  case TA2PE432RTE_ID:
  case TA2PE432RTF_ID:
  case TA2BPE32RTE_ID:
  case TA2BPE32RTF_ID:
  case TA2BPE32RTV_ID:
    __mrtRxCtrl[num] = 1;
    __tmkBusErr[num] = 1;
    break;
  }
#endif
#ifdef USE_TMKRAWIO
  port = __tmkPortsAddr1[num];
  port2 = __tmkPortsAddr2[num];
  __tmkPortsMax1[num] = 0;
  __tmkPortsMax2[num] = 0;
  switch (subdid)
  {
    case TA1BPE_ID:
    case TA2BPE_ID:
#ifdef MRTA
    case TA1P32RTF_ID:
    case TA1P32RTV_ID:
    case TA1PE432RTF_ID:
    case TA1BPE32RT_ID:
    case TA1BPE32RTE_ID:
    case TA1BPE32RTF_ID:
    case TA1BPE32RTV_ID:
    case TA2P32RTF_ID:
    case TA2P32RTV_ID:
    case TA2PE432RTF_ID:
    case TA2BPE32RT_ID:
    case TA2BPE32RTE_ID:
    case TA2BPE32RTF_ID:
    case TA2BPE32RTV_ID:
#endif
      __tmkPortsMax1[num] = port + 0x3E;
      break;
    default: // PCI or 0 for nonPCI
      switch (type)
      {
        case __TMK400:
          __tmkPortsMax2[num] = port2 + 0x0E;
          // no break here
          fallthrough;
          // fall-through
        case __RTMK400:
        case __TMKMPC:
#ifdef MRTX
        case __MRTX:
#endif
#ifdef TSTX
        case __TSTX:
#endif
        case __TMKX:
          __tmkPortsMax1[num] = port + 0x0E;
          break;
#ifdef MRTA
        case __MRTA:
#endif
#ifdef TSTA
        case __TSTA:
#endif
        case __TA:
          __tmkPortsMax1[num] = port + 0x1E;
          break;
      }
      break;
  }
  switch (did)
  {
    case PLX_DID0:
      __tmkPortsMax2[num] = port2 + 0x7E; //portplx
      break;
    case PLX_DID1:
      __tmkPortsMax2[num] = port2 + 0xFE; //portplx
      break;
  }
#else
  (void)type;
  (void)did;
#endif
}
#endif //MRTARAWIO

void DrvInitDrv(int REGPAR num, int realnum, unsigned type)
{
#if NRT > 0
  unsigned irt, nrt;
#endif

  if (num == realnum)
  {
    __tmkMode[num] = UNDEFINED_MODE;
    DrvIOSetPar(IOPAR_TMKMODE, UNDEFINED_MODE);
    __tmkModeMtRt[num] = 0;
    __rtAddress[num] = 0x00FF;
    __rtDisableMask[num] = __RT_DIS_MASK[type];
    __rtBRCMask[num] = __RT_BRC_MASK[type];
    __rtEnableOnAddr[num] = 1;
    __tmkOptions[num] = (UOPT)0x0L;
    switch (type)
    {
    case __TMK400:
    case __TMKMPC:
      __rtControls[num] = RT_MODE;
      break;
    case __RTMK400:
      __rtControls[num] = RT_MODE;
      __rtControls1[num] = RTMK400_HBIT_MODE | RTMK400_BRCST_MODE;
      break;
    case __TMKX:
      __rtControls[num] = ((RT_MODE >> 7) + TX_RT_DATA_INT_BLK + GENER1_BLK + GENER2_BLK);
      __rtControls1[num] = TMKX_HBIT_MODE | TMKX_BRCST_MODE;
      break;
#ifdef MRTX
    case __MRTX:
      __tmkOptions[num] = (UOPT)MRT_READ_BRC_DATA;
      __rtControls[num] = TX_RT_DATA_INT_BLK;
      __rtControls1[num] = MRTX_HBIT_MODE | MRTX_BRCST_MODE;
      __mrtCtrl0[num] = 0x1FF8;
      __mrtCtrl1[num] = 0x8000 | 0x1FF8;
      __mrtMask0[num] = 0x1FF8;
      __mrtMask1[num] = 0x1FF8;
      break;
#endif
    case __TA:
      __rtControls[num] = (RT_MODE << 7) | TA_RT_DATA_INT_BLK | TXRX_ALL_ENABLE | (__tmkCBSND ? 0 : TA_CBSD_MODE) | TA_IRQ_EN;
      __rtControls1[num] = TA_HBIT_MODE | TA_BRCST_MODE;
      __FLAG_MODE[num] = 0;
//    ???
      break;
#ifdef MRTA
    case __MRTA:
      __tmkOptions[num] = (UOPT)MRT_READ_BRC_DATA;
      __rtControls[num] = TA_RT_DATA_INT_BLK | TXRX_ALL_ENABLE | (__tmkCBSND ? 0 : TA_CBSD_MODE) | TA_IRQ_EN;
      __rtControls1[num] = TA_HBIT_MODE; // | 0xF800  | TA_BRCST_MODE;
      __FLAG_MODE[num] = 0;
//    ???
      break;
#endif
    }
#if NRT > 0
    __amrtNumber[num] = num;
#endif
  }

#if NRT > 0
  switch (type)
  {
#ifdef MRTX
  case __MRTX:
#endif
#ifdef MRTA
  case __MRTA:
#endif
#if defined(MRTX) || defined(MRTA)
    if (num == realnum)
    {
      nrt = __mrtNRT[num];
      irt = __mrtMinRT[num];
    }
    else
    {
      nrt = 1;
      irt = num;
      num = realnum;
    }

//    __tmkNumber = irt;

    do
    {
      __amrtNumber[irt] = num;
      __tmkDrvType[irt] = type;
      __tmkMode[irt] = UNDEFINED_MODE;
      DrvIOSetParTmk(IOPAR_TMKMODE, UNDEFINED_MODE, irt);
      __rtAddress[irt] = 0x00FF;
      __rtDisableMask[irt] = __RT_DIS_MASK[type];
      __rtBRCMask[irt] = __RT_BRC_MASK[type];
      __rtEnableOnAddr[irt] = 1;
      __tmkOptions[irt] = (UOPT)MRT_WRITE_BRC_DATA | (UOPT)MRT_READ_BRC_DATA;
#ifdef MRTX
      if (type == __MRTX)
        __hm400Page[irt] = (irt - __mrtMinRT[num]) << 11;
#endif
#ifdef MRTA
      if (type == __MRTA)
      {
        __rtControls1[irt] = 0xF800 | TA_BRCST_MODE; // | TA_HBIT_MODE;
        __FLAG_MODE[irt] = 0;
      }
#endif
      ++irt;
    }
    while (--nrt != 0);
    break;
#endif
  }
#endif // NRT > 0

}

U16 FARFN tmkgethwver(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register realnum;

  CLRtmkError;
  realnum = GET_RealNum;
  return __tmkHWVer[realnum];
}

#if !defined(DOS) || defined(TMKDRV_FUNCS)
void tmkdrvkeepflgm(int tmkKeepFlgM)
{
  __tmkKeepFlgM = tmkKeepFlgM;
}

void tmkdrvcbsnd(int tmkCBSND)
{
  __tmkCBSND = tmkCBSND;
}

void tmkdrvwrapsa(unsigned tmkWrapSA, int tmkWrapST, int tmkWrapInt)
{
  __tmkWrapSA = (tmkWrapSA <= 30) ? tmkWrapSA : 0;
  __tmkWrapNoST5 = (tmkWrapST) ? 0 : (tmkWrapSA << 5) | RT_DIR_MASK;
  __tmkWrapInt = (tmkWrapInt) ? 0x4000 : 0;
}
#endif

#ifdef TMK_REAL_MAX
void tmkdrvrealmax(int tmkUseRealMax)
{
  __tmkUseRealMax = tmkUseRealMax;
}
#endif

#if NRT > 0
void mrtdefmaxnrt(int mrtMaxNrt)
{
  __mrtMaxNRT = mrtMaxNrt;
}
#endif //NRT

#ifdef DOS32
int DrvLockRam(unsigned lock_op);

void savees(void);
#pragma aux savees = \
"mov __SaveES, es" \
__parm [] __modify [];
#endif //def DOS32

#ifndef DOS
int FARFN tmkreconfig(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int num;
  int register realnum;
  unsigned type;
#if NRT > 0
  unsigned irt, nrt;
#endif

  num = __tmkNumber;
  CHECK_TMK_NUMBER(num);
  DrvIOProV;
  CLRtmkError;
  realnum = GET_RealNum;
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);

#if NRT > 0
  if (num != realnum)
  {
    irt = __mrtMinRT[realnum];
    nrt = __mrtNRT[realnum];
    do
    {
      if (irt != num)
       if (__tmkMode[irt] != UNDEFINED_MODE)
         break;
      ++irt;
    }
    while (--nrt != 0);
    if (nrt == 0) // if no other virtual RTs on this MRT then use full MRT
      num = realnum;
  }
#endif

  DrvInitNTMKNRT(num);
  if (num == realnum)
  {
    DrvInitNTMK(num);
    DrvInitTmk(num, type);
#if NRT > 0
    if (__mrtNRT[num] > 1)
    {
      irt = __mrtMinRT[realnum];
      nrt = __mrtNRT[realnum];
      do
      {
        DrvInitNTMKNRT(irt);
        ++irt;
      }
      while (--nrt != 0);
    }
#endif
  }

  DrvInitDrv(num, realnum, type);

  DrvIOEpi;
  return 0;
}
#endif //ndef DOS

#if !defined(DOS) || defined(TMKDRV_FUNCS)
void FARFN tmkdrvinit()
{
  if (__tmkFirstTime)
  {
    __tmkFirstTime = 0;
    DrvInitAll();
  }
}
#endif

#if defined(DOS) || defined(QNX4)
#ifdef QNX4VME
int FARFN tmkconfig(int hTMK, U16 wType, U16 PortsAddr1, U16 PortsAddr2, U08 Irq1, U08 Irq2, char *pTmkName)
//tmkconfig(hTMK, wType, PortsAddr, DeviceNumber, IrqVect, IrqLev, pTmkName);
#else
int FARFN tmkconfig(int hTMK, U16 wType, U16 PortsAddr1, U16 PortsAddr2, U08 Irq1, U08 Irq2)
#endif //def QNX4VME
#else
int FARFN tmkconfig(int hTMK, U16 wType, U16 PortsAddr1, U16 PortsAddr2, U08 Irq1, U08 Irq2, U16 DID, U16 SubDID)
#endif //def DOSQNX4
{
#ifndef STATIC_TMKNUM
  int __tmkNumber; // fake __tmkNumber
#endif
  int register num;
  unsigned type;
  int res;
#if NRT > 0
  unsigned irt, nrt;
#endif
#if defined(MRTA) || defined(USE_TMKRAWIO)
  unsigned did = 0, subdid = 0;
#endif //MRTARAWIO

#if !defined(DOS) || defined(TMKDRV_FUNCS)
  tmkdrvinit();
#else
  if (__tmkFirstTime)
  {
    __tmkFirstTime = 0;
    DrvInitAll();
  }
#endif
  num = hTMK;
  CHECK_TMK_REAL_NUMBER(num);
  __tmkNumber = num;
#ifdef DrvIO_TMKCONFIG_0
  DrvIO_TMKCONFIG_0;
#endif
  DrvIOProV;
#ifdef DrvIO_TMKCONFIG_1
  DrvIO_TMKCONFIG_1;
#endif
  CLRtmkError;
  PUT_RealNum(num);
  if (__tmkDrvType[num] != 0xFFFF)
  {
    DrvIOEpi;
    return USER_ERROR_R(TMK_BAD_TYPE);
  }
//;  CLRtmkError;
#ifdef TMK_REAL_MAX
  if (__tmkRealMax < num)
    __tmkRealMax = num;
#endif
#ifdef DOS32
  ++__fTmkDeep;
  if (__fTmkDeep == 1)
  {
/*
                ASSUME  ds:_TEXT
                mov     SaveDS, ds
                mov     SaveES, es
                ASSUME  ds:_DATA
*/
    savees();
    if (DrvLockRam(LOCK_RAM))
    {
      res = USER_ERROR_R(TMK_DPMI_ERROR);
      goto err;
    }
  }
#endif //def DOS32
//  __tmkMode[num] = UNDEFINED_MODE; see DrvInitDrv ???
//  DrvIOSetPar(IOPAR_TMKMODE, UNDEFINED_MODE);
  type = wType;
#if NRT > 0 && defined(DOS)
  switch (type & 0x7FFF)
  {
  default:
    break;
#ifdef MRTX
  case MRTX:
#endif
#ifdef MRTXI
  case MRTXI:
#endif
#ifdef MRTA
  case MRTA:
#endif
#ifdef MRTAI
  case MRTAI:
#endif
    type ^= 0x8000; // protection from calling plain tmkconfig for MRTs
    break;
  }
#endif
  CHECK_TMK_TYPE_1(type); // special if QNX4VME
#if defined(DOS) || defined(QNX4)
  __tmkPci[num] = 0;
  switch (type)
  {
  case TMKXI:
  case TAI:
#ifdef MRTXI
  case MRTXI:
#endif
#ifdef MRTAI
  case MRTAI:
#endif
#ifdef TSTXI
  case TSTXI:
#endif
#ifdef TSTAI
  case TSTAI:
#endif
    __tmkPci[num] = 1;
    break;
  }
#endif //def DOS
  __tmkUserType[num] = type;
  type = __tmkUser2DrvType[type];
  __tmkDrvType[num] = type;
  __RT_DIS_MASK[type] = __RT_DIS_MASK0[__tmkUserType[num]];
  __RT_BRC_MASK[type] = __RT_BRC_MASK0[__tmkUserType[num]];
  __tmkPortsAddr1[num] = PortsAddr1;
  __tmkPortsAddr2[num] = PortsAddr2;
#ifdef WIN95
  __tmkIrq1[num] = Irq1;
#endif
#if defined(DOS) || defined(QNX4)
  __tmkIrq1[num] = Irq1;
  if (__tmkPci[num])
  {
#if defined(MRTA) || defined(USE_TMKRAWIO)
    res = SetupPCI(num, &did, &subdid);
#else
    res = SetupPCI(num);
#endif //MRTARAWIO
    if (res)
      goto err;
  }
#else
#ifndef QNX4VME
#if defined(MRTA) || defined(USE_TMKRAWIO)
  did = (unsigned)DID;
  subdid = (unsigned)SubDID;
#else
  (void)DID;
  (void)SubDID;
#endif //MRTARAWIO
#endif
#endif //def DOSQNX4
#ifdef QNX4VME
  __tmkIrqLevel[num] = Irq2;
  __vmeDev = _VME_device_attach(pTmkName, __tmkClassName);
  if (__vmeDev == -1)
    return USER_ERROR_R(errno);
  __vmeWin = _VME_window_attach(__vmeDev, _ACC_A16, PortsAddr1, 0x200);
  if (__vmeWin == (void*) -1)
    return USER_ERROR_R(errno);
  if (PortsAddr2 > 3)
    return USER_ERROR_R(TMK_BAD_NUMBER);
  __tmkPortsAddr1[num] = PortsAddr2 << 7;
#endif //def QNX4VME
#if defined(MRTA) || defined(USE_TMKRAWIO)
  DrvInitDrvID(num, type, did, subdid);
#endif //MRTARAWIO

  DrvInitTmk(num, type);
  res = ExamTmkRAM(num, type);
  if (res)
    goto err;

#if NRT > 0
  switch (type)
  {
#ifdef MRTX
  case __MRTX:
#endif
#ifdef MRTA
  case __MRTA:
#endif
#if defined(MRTX) || defined(MRTA)
    nrt = __mrtNRT[num]; // should be after ExamTmkRAM !
    irt = __mrtMinRT[num];
    if (irt == 0)
    {
      irt = __tmkMaxNumber;
      __tmkMaxNumber += nrt;
      while (__tmkMaxNumber >= (NTMK + NRT))
      {
        --__tmkMaxNumber;
        --__mrtNRT[num];
        --nrt;
      }
      if (nrt <= 0)
      {
        res = USER_ERROR_R(TMK_BAD_NUMBER);
        goto err;
      }
      ++irt;
      __mrtMinRT[num] = irt;
    }
    __tmkNumber = irt; // tmkselected for MRT !!!
#endif
  }
#endif // NRT > 0

  DrvInitDrv(num, num, type);

#if defined(DOS) || defined(QNX4)
  res = SetTmkIrqs(num);
  if (res)
  {
    ResetTmkIrqs(num);
//    __tmkDrvType[num] = 0xFFFF;
    goto err;
  }
#endif //def DOS

#ifdef WIN95 //???
//;                call    SetTmkIrqs
//;                or      eax, eax
//;                jz        case Cont_1
//;                push    eax
//;                call    ResetTmkIrqs
//;                mov     __tmkDrvType[ebx], 0xFFFF
//;                pop     eax
//;                jmp       case Exit
#endif

#ifdef DOS
  DrvUnmaskTmk(num);
#endif //def DOS
  res = 0;
  goto tcret;
  err:
#ifdef DOS32
  if (__fTmkDeep != 0)
  {
    if (--__fTmkDeep == 0)
      DrvLockRam(UNLOCK_RAM);
  }
#endif //def DOS32
  tcret:
  DrvIOEpi;
  (void)Irq2;
  return res;
}

#if NRT > 0 && defined(DOS)
U32 FARFN mrtconfig(int hTMK, U16 wType, U16 PortsAddr1, U16 PortsAddr2, U08 Irq1, U08 Irq2)
{
  int iRt, iRt0, nRt;
  int i;
  U16 aw0[30];
  U32 res;

  switch (wType)
  {
#ifdef MRTX
  case MRTX:
  case MRTXI:
#endif
#ifdef MRTA
  case MRTA:
  case MRTAI:
#endif
    if (tmkconfig(hTMK, wType ^ 0x8000, PortsAddr1, PortsAddr2, Irq1, Irq2))
    {
      res = 0L;
      break;
    }
    // tmkconfig selects first RT
    tmkselect(hTMK);
    if (bcreset())
    {
      res = 0L;
      break;
    }
    for (i = 0; i < 30; ++i)
      aw0[i] = 0;
    rtputflags(aw0, RT_RECEIVE, 1, 30);
    iRt0 = mrtgetrt0();
    nRt = mrtgetnrt();
    for (iRt = iRt0+nRt-1; iRt >= iRt0; --iRt)
    {
      tmkselect(iRt);
      rtreset();
      rtputflags(aw0, RT_RECEIVE, 1, 30);
    }
    tmkselect(iRt0); // ??? select first RT
    res = (((U32)nRt<<16) | (U32)iRt0);
    break;
  default:
    res = 0L;
    break;
  }
  return res;
}
#endif //NRT DOS

int FARFN tmkdone(int hTMK)
{
  int register num;
  unsigned type;
  int ntmk;
#ifdef STATIC_TMKNUM
  int numsave;
#endif
#if NRT > 0
  int irt, nrt;
#endif
#ifdef NMBCID
  int imbcId, imbc;
#endif

#ifdef DOS
  CLRtmkError;
#endif
  if (__tmkFirstTime)
    return 0;
  num = hTMK;
  if (num != ALL_TMKS && num != ALL_TMKS_OLD)
  {
    CHECK_TMK_REAL_NUMBER(num);
    ntmk = num;
  }
  else
  {
    num = 0;
    ntmk = NTMK;
  }
#ifdef STATIC_TMKNUM
  numsave = tmkselected();
#endif
  do
  {
    type = __tmkDrvType[num];
    if ((unsigned)type == 0xFFFF)
      continue;
#ifdef STATIC_TMKNUM
    tmkselect(num);
#endif
    DrvIOProV;
//;    CHECK_TMK_TYPE(type);
#ifdef DOS
    DrvMaskTmk(num);
#endif
#if defined(DOS) || defined(QNX4)
    ResetTmkIrqs(num);
#endif //def DOSQNX4
#ifdef QNX4VME
    if (__vmeDev != -1 && __vmeWin != (void*) -1)
      _VME_window_detach(__vmeDev, _ACC_A16);
    __vmeWin = (void*) -1;
    if (__vmeDev != -1)
      _VME_device_detach(__vmeDev);
    __vmeDev = -1;
#endif //def QNX4VME

#ifdef DOS32
    if (__fTmkDeep != 0)
    {
      if (--__fTmkDeep == 0)
        DrvLockRam(UNLOCK_RAM);
    }
#endif //def DOS32

    DrvInitDrv(num, num, type);

//    DrvInitTmk(num, type);
    DrvInitNTMKNRT(num);
    DrvInitNTMK(num);
#if NRT > 0
    if (__mrtNRT[num] > 1)
    {
      irt = __mrtMinRT[num];
      nrt = __mrtNRT[num];
      do
      {
        DrvInitNTMKNRT(irt);
//;        __amrtNumber2[irt] = 0xFFFF;
        __tmkDrvType[irt] = 0xFFFF;
        ++irt;
      }
      while (--nrt != 0);
    }
#endif // NRT > 0

#ifdef DrvIO_TMKDONE_1
    DrvIO_TMKDONE_1;
#endif
    DrvIOEpi;
#ifdef DrvIO_TMKDONE_0
    DrvIO_TMKDONE_0;
#endif
    __tmkDrvType[num] = 0xFFFF;
//    tmkUserType[num] = 0xFFFF;

#ifdef NMBCID
// this will not free allocated MBC with no any assigned BC!
    for (imbcId = 0; imbcId < NMBCID; ++imbcId)
    {
      if (__mbcAlloc[imbcId] == 0)
        continue;
      for (imbc = 0; imbc < __mbci[imbcId]; ++imbc)
      {
        if (__mbcTmkN[imbcId][imbc] == num)
        {
          mbcfree((U16)imbcId);
          break;
        }
      }
    }
#endif
  }
  while (++num < ntmk);
#ifdef STATIC_TMKNUM
//  tmkselect(numsave);
  __tmkNumber = numsave;
  PUT_RealNum(__amrtNumber[numsave]);
#endif
  return 0;
}

#ifdef DOS

#ifndef CPU188

void DrvReadOldMask()
{
  __tmkOldMask21 = inpb(0x21);
  __tmkOldMaskA1 = inpb(0xA1);
}

void DrvMaskTmk(int REGPAR num)
{
  outpb(0x21, inpb(0x21) | (__tmkMask21[num] & __tmkOldMask21));
  outpb(0xA1, inpb(0xA1) | (__tmkMaskA1[num] & __tmkOldMaskA1));
}

void DrvUnmaskTmk(int REGPAR num)
{
  outpb(0x21, inpb(0x21) & ~__tmkMask21[num]);
  outpb(0xA1, inpb(0xA1) & ~__tmkMaskA1[num]);
}

#else //def CPU188

void DrvReadOldMask()
{
  __tmkOldMask21 = inpw(0xFF28);
}

void DrvMaskTmk(int REGPAR num)
{
  _AX = inpw(0xFF28) | (__tmkMask21[num] & __tmkOldMask21);
  outpb(0xFF28, _AL);
}

void DrvUnmaskTmk(int REGPAR num)
{
  _AX = inpw(0xFF28) & ~__tmkMask21[num];
  outpb(0xFF28, _AL);
}

#endif //ndef CPU188

void FARFN tmkdefirq(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Irq)
{
  CLRtmkError;
#ifdef MASKTMKS
  U08 irq;
  irq = Irq;
  CHECK_IRQ(irq);
#ifndef CPU188
  if (irq < 8)
    tmkAllMask21 |= 1 << irq;
  else
    tmkAllMaskA1 |= 1 << (irq - 8);
#else //def CPU188
  if (irq < 5)
    tmkAllMask21 |= 1 << (irq + 4);
  else
    tmkAllMask21 |= 1 << (irq - 3);
#endif //ndef CPU188
#else
  (void)Irq;
#endif //def MASKTMKS
  return;
}

void FARFN tmkundefirq(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 Irq)
{
  CLRtmkError;
#ifdef MASKTMKS
  U08 irq;
  irq = Irq;
  CHECK_IRQ(irq);
#ifndef CPU188
  if (irq < 8)
    tmkAllMask21 &= ~(1 << irq);
  else
    tmkAllMaskA1 &= ~(1 << (irq - 8));
#else //def CPU188
  if (irq < 5)
    tmkAllMask21 &= ~(1 << (irq + 4));
  else
    tmkAllMask21 &= ~(1 << (irq - 3));
#endif //ndef CPU188
#else
  (void)Irq;
#endif //def MASKTMKS
  return;
}
#endif //def DOS

#if defined(DOS) || defined(QNX4INT)
void FARFN bcrestore(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int register num;
  unsigned type;
  LOCAL_PORT

  num = __tmkInIrqNumber;
  type = __tmkDrvType[num];
  DrvBcDefBase(num, type, __bcSaveBase);
  __tmkRAMInWork[num] = __tmkSaveRAMInWork;
  if (__tmkSaveRAMInWork)
  {
    SET_LOCAL_PORT(num);
    __tmkRAMAddr[num] = __tmkSaveRAMAddr;
    switch (type)
    {
    case __TMKMPC:
      outpb_d(num, TMKMPC_AddrL(num), __tmkSaveRAMAddr);
      break;
    case __TMK400:
    case __RTMK400:
      outpb_d(num, TMK_Addr(num), __tmkSaveRAMAddr);
      break;
    case __TMKX:
//#ifdef MRTX
//    case __MRTX:
//#endif
      outpw(TMK_Addr(num), __tmkSaveRAMAddr);
      break;
//    case __TA:
//    ???
//      break;
//    case __MRTA:
//    ???
//      break;
    }
  }
  __tmkNumber = __tmkSaveNumber;
  return;
}

void FARFN rtrestore(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int num;
  int register realnum;
  unsigned type;
  LOCAL_PORT

  num = __tmkInIrqNumber;
  realnum = GET_RealNumOf(num);
  type = __tmkDrvType[num];
  DrvRtDefSubAddr(num, type, __rtSaveSubAddr);
  switch (type)
  {
  case __TA:
#ifdef MRTA
  case __MRTA:
#endif
    __rtSABuf[num] = __rtSaveSABuf;
    __hm400Page[num] = __rtSavehm400Page;
#ifdef MRTA
    __hm400Page2[num] = __rtSavehm400Page2;
#endif
  }
  DrvRtWMode(num, type, __rtSaveMode);
  __tmkRAMInWork[realnum] = __tmkSaveRAMInWork;
  if (__tmkSaveRAMInWork)
  {
    SET_LOCAL_PORT(realnum);
    __tmkRAMAddr[realnum] = __tmkSaveRAMAddr;
    switch (type)
    {
    case __TMKMPC:
      outpb_d(realnum, TMKMPC_AddrL(realnum), __tmkSaveRAMAddr);
      break;
    case __TMK400:
    case __RTMK400:
      outpb_d(realnum, TMK_Addr(realnum), __tmkSaveRAMAddr);
      break;
    case __TMKX:
#ifdef MRTX
    case __MRTX:
#endif
      outpw(TMK_Addr(realnum), __tmkSaveRAMAddr);
      break;
//    case __TA:
//    ???
//      break;
//    case __MRTA:
//    ???
//      break;
    }
  }
  __tmkNumber = __tmkSaveNumber;
  return;
}

void FARFN tmkrestore(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  __tmkInIrqNumber = __tmkNumber = __tmkSaveNumber;
  _disable();
#ifdef MASKTMKS
  DrvUnmaskTmks();
#endif //def MASKTMKS
  // no full support for MT+RT modes !!!
  if (__tmkMode[__tmkNumber] & RT_MODE) // RT, MRT
    rtrestore();
  else
    bcrestore();
  return;
}

void FARFN tmksave(
#ifndef STATIC_TMKNUM
        int __tmkNumber
#endif
        )
{
  int num;
  int register realnum;

  num = __tmkNumber;
  realnum = GET_RealNum;
  __tmkSaveNumber = num;
  __tmkSaveRAMAddr = __tmkRAMAddr[realnum];
  __tmkSaveRAMInWork = __tmkRAMInWork[realnum];
  __tmkRAMInWork[realnum] = 0;
  // no full support for MT+RT modes !!!
  if (__tmkMode[num] & RT_MODE) // RT, MRT
  {
    __rtSaveSubAddr = __rtSubAddr[num];
    __rtSaveSABuf = __rtSABuf[num];
    __rtSavehm400Page = __hm400Page[num];
#ifdef MRTA
    __rtSavehm400Page2 = __hm400Page2[num];
#endif
    __rtSaveMode = __rtMode[num];
  }
  else
  {
    __bcSaveBase = __bcBasePC[realnum];
  }
#ifdef MASKTMKS
  DrvMaskTmks();
#endif //def MASKTMKS
  return;
}

#ifdef QNX4
#define RETQNX4 return
#else
#define RETQNX4
#endif

#ifdef DOS32
void restes(void);
#pragma aux restes = \
"mov es, __SaveES" \
__parm [] __modify [];
#endif //def DOS32

#ifdef CALL_OLD_INTS
#define CallOldInt(num) tmkOldInt1[num]()
#else
#define CallOldInt(num)
#endif

#if NTMK > 0
RETIR TYPIR DrvTmk0Int1(__CPPARGS)
{
  RETQNX4 DrvTmksInt1(0);
  CallOldInt(0);
}
#endif
#if NTMK > 1
RETIR TYPIR DrvTmk1Int1(__CPPARGS)
{
  RETQNX4 DrvTmksInt1(1);
  CallOldInt(1);
}
#endif
#if NTMK > 2
RETIR TYPIR DrvTmk2Int1(__CPPARGS)
{
  RETQNX4 DrvTmksInt1(2);
  CallOldInt(2);
}
#endif
#if NTMK > 3
RETIR TYPIR DrvTmk3Int1(__CPPARGS)
{
  RETQNX4 DrvTmksInt1(3);
  CallOldInt(3);
}
#endif
#if NTMK > 4
RETIR TYPIR DrvTmk4Int1(__CPPARGS)
{
  RETQNX4 DrvTmksInt1(4);
  CallOldInt(4);
}
#endif
#if NTMK > 5
RETIR TYPIR DrvTmk5Int1(__CPPARGS)
{
  RETQNX4 DrvTmksInt1(5);
  CallOldInt(5);
}
#endif
#if NTMK > 6
RETIR TYPIR DrvTmk6Int1(__CPPARGS)
{
  RETQNX4 DrvTmksInt1(6);
  CallOldInt(6);
}
#endif
#if NTMK > 7
RETIR TYPIR DrvTmk7Int1(__CPPARGS)
{
  RETQNX4 DrvTmksInt1(7);
  CallOldInt(7);
}
#endif

#ifdef DOS32
//rmCallStruc.rm_IP == rmCallStruc+2aH
//rmCallStruc.rm_CS == rmCallStruc+2cH
//rmCallStruc.rm_Flags == rmCallStruc+20H
//rmCallStruc.rm_SP == rmCallStruc+2eH
void rmDrvTmksInt(void);
#pragma aux rmDrvTmksInt = \
" mov bx, ds" \
" mov ds, [esp+0xC]" /*original ds saved by Watcom*/ \
/*no need for cld here because Watcom places cld himself*/ \
/*" cld"*/ \
" lodsw" \
" mov es:[edi].0x2A, ax" \
" lodsw" \
" mov es:[edi].0x2C, ax" \
" lodsw" \
" mov es:[edi].0x20, ax" \
" add word ptr es:[edi].0x2E, 6" \
" mov ds, bx" \
__parm [] __modify [ebx esi];

#if NTMK > 0
RETIR TYPIR rmDrvTmk0Int1(__CPPARGS)
{
  rmDrvTmksInt();
  DrvTmksInt1(0);
}
#endif
#if NTMK > 1
RETIR TYPIR rmDrvTmk1Int1(__CPPARGS)
{
  rmDrvTmksInt();
  DrvTmksInt1(1);
}
#endif
#if NTMK > 2
RETIR TYPIR rmDrvTmk2Int1(__CPPARGS)
{
  rmDrvTmksInt();
  DrvTmksInt1(2);
}
#endif
#if NTMK > 3
RETIR TYPIR rmDrvTmk3Int1(__CPPARGS)
{
  rmDrvTmksInt();
  DrvTmksInt1(3);
}
#endif
#if NTMK > 4
RETIR TYPIR rmDrvTmk4Int1(__CPPARGS)
{
  rmDrvTmksInt();
  DrvTmksInt1(4);
}
#endif
#if NTMK > 5
RETIR TYPIR rmDrvTmk5Int1(__CPPARGS)
{
  rmDrvTmksInt();
  DrvTmksInt1(5);
}
#endif
#if NTMK > 6
RETIR TYPIR rmDrvTmk6Int1(__CPPARGS)
{
  rmDrvTmksInt();
  DrvTmksInt1(6);
}
#endif
#if NTMK > 7
RETIR TYPIR rmDrvTmk7Int1(__CPPARGS)
{
  rmDrvTmksInt();
  DrvTmksInt1(7);
}
#endif
#endif //def DOS32

#endif //def DOS || QNX4INT

#ifndef DYNAMIC_TMKNUM
#define HTMK
#define HTMK__
#else
#define HTMK hTmk
#define HTMK__ hTmk,
#endif

#ifdef DOS
RETIR IncDpcData(int REGPAR num);
#endif //DOS

#ifndef DrvIO

#if NRT > 0

void DpcPhysMRT(int hTmk, int cDpcDataTmk)
{
#ifndef DYNAMIC_TMKNUM
  int hTmkSave;
#endif
  pTListEvD pEvD, pEvD0;
  int iEvD, iEvDStop;
  int nInDpcType;
  int nInt, hTmk0, hTmk1, hTmkT;
  int fModeCmd;
  unsigned short wSA, nWords;
  unsigned short wMask;
  unsigned short wBuf;
  unsigned short wSavePage, wSaveSA, wSaveBuf;
  unsigned short state;
  LOCAL_PORT
  U08 rxbus;

  SET_LOCAL_PORT(hTmk);
  nInDpcType = __tmkDrvType[hTmk];
#ifndef DOS
  iEvD = (iEvDataBegin[hTmk] + cEvData[hTmk] - cDpcDataTmk) & (EVENTS_SIZE-1);
  iEvDStop = (iEvD + cDpcDataTmk) & (EVENTS_SIZE-1);
  // process all brc commands and datas
  // for MRTX would be better to find last brc data only
  // (because of single brc subaddr on MRTX)
  // !!! move cmds copy for MRTA from irq !!!
  do
  {
    pEvD = &(aEvData[hTmk][iEvD]);

#ifdef DDKNT
    #ifdef MY_DBG_DPC
    KdPrint(("MRT nInt %d\n", pEvD->nInt));
    #endif
#endif //DDKNT

#else
    (void)cDpcDataTmk;
    pEvD = &(__aEvData[hTmk]);
    pEvD0 = (void*)0;
#endif //nDOS

    if (pEvD->nInt == 4)
    {
      hTmk0 = (int)(pEvD->awEvData[1]);
      hTmkT = (hTmk0 >> 11) & 0x1F; // transmitting RT (if exist)
      hTmk0 &= 0x07FF;              // min receiving RT
      if (hTmkT != 0)
        hTmkT = hTmkT - 1 + hTmk0;
      hTmk1 = hTmk0 + (int)(pEvD->awEvData[2] & 0x001F) - 1; // max receiving RT
      wBuf = (pEvD->awEvData[2] >> 5) & 0x07FF;
      wMask = 0xFFFF;
      rxbus = ((U08)(pEvD->awEvData[0] >> 5) & 0x40) | ((U08)(~pEvD->awEvData[0] >> 4) & 0x80); //useless in MRTX

      fModeCmd = ((pEvD->awEvData[0] & 0x03E0) == 0x0000) || ((pEvD->awEvData[0] & 0x03E0) == 0x03E0);
      if (pEvD->awEvData[0] & 0x4000)
      {
        nInt = 2;
        wMask = 0x47FF;
      }
      else if (fModeCmd)
      {
        nInt = 1;
        wMask = 0x041F;
      }
      else
      {
        nInt = 3;
        wMask = 0x07FF;
      }
//      pEvD->awEvData[1] = (unsigned short)nInt;
//      was this for MRT tmkgetevd ???
//      even if so that's incorrect due to parameters difference

      if (!fModeCmd && (__tmkOptions[hTmk] & (UOPT)MRT_READ_BRC_DATA))
      {
#ifndef DYNAMIC_TMKNUM
        hTmkSave = tmkselected();
        tmkselect(hTmk);
#endif
        wSA = pEvD->awEvData[0] & 0x03E0;
        nWords = pEvD->awEvData[0] & 0x1F;
        if (nWords == 0)
          nWords = 32;
        wSaveSA = rtgetsubaddr(HTMK);
        wSaveSA = (wSaveSA & RT_DIR_MASK) | ((wSaveSA & 0x1F) << 5); //for DrvRtDefSubAddr
        if (nInDpcType == __MRTX)
        {
          if (nInt != 2)
          {
            mrtdefbrcsubaddr0(HTMK); // save ???
            rtgetblk(HTMK__ 0, &(__awBrcRxBuf[hTmk][wSA]), nWords);
            if (rtgetmode(HTMK) & RT_FLAG_MODE)
            {
              DrvRtDefSubAddr(hTmk, nInDpcType, wSA); //RT_RECEIVE, wSA>>5);
              rtclrflag(HTMK); // clrflag in mrt aka rt0
                // the flag may be cleared later if rt0 is used
                // (subj to optimize)
                // but if rt0 isn't used we have to have the flag cleared!
                // also the cleared flag is checked below before writing
                // saved __awBrcRxBuf data into rt0 (so if we do not clear
                // it here we need a workaround for rt0
            }
            DrvRtDefSubAddr(hTmk, nInDpcType, wSaveSA); //wSaveSA, wSaveSA);
          }
        }
        else // if (nInDpcType == __MRTA)
        {
          wSavePage = rtgetpage(HTMK);
          wSaveBuf = rtgetbuf(HTMK);
          mrtdefbrcpage(HTMK__ 0);
          DrvRtDefSubAddr(hTmk, nInDpcType, wSA); //RT_RECEIVE, wSA>>5);
          if (wBuf != 0) //&& __rtSABufMode[hTmk][wSA>>5] == SABUF_HEAP)
            rtdefbuf(HTMK__ wBuf);
          if (__rtSABufMode[hTmk][wSA>>5] == SABUF_OFF)
          {
            if (nInt != 2)
              __rtgetblkmrta(HTMK__ 0, &(__awBrcRxBuf[hTmk][wSA]), nWords);
            __rtgetblkmrta(HTMK__ 58, &(__awBrcRxState[hTmk][wSA>>3]), 3);
            // rtgetblk does not accept position 58
//            if (rtgetmode(HTMK) & RT_FLAG_MODE)
//            {
//              rtclrflag(HTMK);
                // We may need clrflag in the mrt brc page to be able to
                // receive new brc data if the brc page uses flags.
                // Actually it could be better (but more complex) to
                // clear the flag when saved __awBrcRxBuf data are written
                // into all RT pages.
                // But now we do not use flags in the brc page at all so
                // it always can receive data.
//            }
          }
          else
          {
            // check rtbusy
            GET_DIS_IRQ();
            outpw(TA_LCW(hTmk), 0xF800); // no matter if TA_BRCST_MODE here
            state = inpw(TA_BASE(hTmk));
            // data read (if fReadSA) should start within 20 us !
            if (((state & 0x2000) == 0) ||
                ((state & 0x0FFF) != (DrvRtGetBaseMRTA(hTmk) & 0x0FFF)))
            {
              if (nInt != 2)
                __rtgetblkmrtairq(HTMK__ 0, &(__awBrcRxBuf[hTmk][wSA]), nWords);
              __rtgetblkmrtairq(HTMK__ 58, &(__awBrcRxState[hTmk][wSA>>3]), 3);
            }
            else
            {
              nInt = 5;
              pEvD->nInt = 5;
              pEvD->wMode = UNDEFINED_MODE;
              pEvD->awEvData[0] = TMK_INT_DRV_BUF_SKIP;
              wMask = 0xFFFF;
            }
            REST_IRQ();
          }
          rtdefpage(HTMK__ wSavePage);
          DrvRtDefSubAddr(hTmk, nInDpcType, wSaveSA); //wSaveSA, wSaveSA);
          if (wSaveBuf != 0)
            rtdefbuf(HTMK__ wSaveBuf);
        }
//tr('r');
#ifndef DYNAMIC_TMKNUM
        tmkselect(hTmkSave);
#endif
      }

#ifdef DOS
      rtrestore(HTMK);
#endif //DOS

      while (hTmk0 <= hTmk1)
      {
        // prepare for DpcVirtRtBrc
        if (!__rtDisableMask[hTmk0] && (hTmk0 != hTmkT) && (rxbus & ~__mrtRxCtrl[hTmk0]) &&
            !(nInDpcType == __MRTA && !(__rtControls1[hTmk0] & TA_BRCST_MODE)))
        {
#ifdef DOS
          pEvD0 = &(__aEvData[hTmk0]);
#else
          pEvD0 = &(aEvData[hTmk0][iEvDataEnd[hTmk0]]);
#endif //DOS
          pEvD0->nInt = 4;
          pEvD0->wMode = MRT_MODE;
          pEvD0->awEvData[0] = (pEvD->awEvData[0] & wMask) | ((unsigned short)(__tmkOptions[hTmk0] & (UOPT)RT_SHOW_BRC_STATE) << 9);
          pEvD0->awEvData[1] = (unsigned short)nInt;
#ifdef DOS
          __tmkInIrqNumber = __tmkNumber = hTmk0;
#endif //DOS
#if defined(WINDOWS) || defined(LINUX_SWTIMER)
// no spinlock (synchronize) here because ISR doesn't touch virtual RTs ???
          aTimeCount[hTmk0][iEvDataEnd[hTmk0]] = aTimeCount[hTmk][iEvD];
#endif //WINDOWS LINUX_SWTIMER
          IncDpcData(hTmk0);
////          fTaskletSchedule = 1;
        }
        ++hTmk0;
      }
    } //if (pEvD->nInt == 4)
    else if (pEvD->nInt >= 1 && pEvD->nInt <= 3)
    {
      hTmk0 = (int)(pEvD->awEvData[1]);
#ifdef DOS
      pEvD0 = &(__aEvData[hTmk0]);
#else
      pEvD0 = &(aEvData[hTmk0][iEvDataEnd[hTmk0]]);
#endif //DOS
      pEvD0->nInt = pEvD->nInt;
      pEvD0->wMode = RT_MODE;
      pEvD0->awEvData[0] = pEvD->awEvData[0];
      pEvD0->awEvData[1] = (pEvD->awEvData[2] >> 5) & 0x07FF; // buf
#ifdef DOS
      __tmkInIrqNumber = __tmkNumber = hTmk0;
#endif //DOS
#if defined(WINDOWS) || defined(LINUX_SWTIMER)
// no spinlock (synchronize) here because ISR doesn't touch virtual RTs ???
      aTimeCount[hTmk0][iEvDataEnd[hTmk0]] = aTimeCount[hTmk][iEvD];
#endif //WINDOWS LINUX_SWTIMER
      IncDpcData(hTmk0);
////      fTaskletSchedule = 1;
    }
#ifdef MRTA
    else if (pEvD->nInt == 5)
    {
      if ((pEvD->awEvData[0] & TMK_INT_TIMER) && __dmrtaTimers[hTmk])
      {
        for (hTmkT = 0; hTmkT < 31; ++hTmkT)
        {
          if (__dmrtaTimers[hTmk] & (1L << hTmkT))
          {
            hTmk0 = __mrtMinRT[hTmk] + hTmkT;
#ifdef DOS
            pEvD0 = &(__aEvData[hTmk0]);
#else
            pEvD0 = &(aEvData[hTmk0][iEvDataEnd[hTmk0]]);
#endif //DOS
            pEvD0->nInt = 5;
            pEvD0->wMode = UNDEFINED_MODE;
            pEvD0->awEvData[0] = TMK_INT_TIMER;
            pEvD0->awEvData[1] = 0;
            pEvD0->awEvData[2] = 0;
#ifdef DOS
            __tmkInIrqNumber = __tmkNumber = hTmk0;
#endif //DOS
#if defined(WINDOWS) || defined(LINUX_SWTIMER)
// no spinlock (synchronize) here because ISR doesn't touch virtual RTs ???
            aTimeCount[hTmk0][iEvDataEnd[hTmk0]] = aTimeCount[hTmk][iEvD];
#endif //WINDOWS LINUX_SWTIMER
            IncDpcData(hTmk0);
////          fTaskletSchedule = 1;
          }
        }
      }
    } //if (pEvD->nInt == 5)
#endif //MRTA

#ifndef DOS
    iEvD = (iEvD + 1) & (EVENTS_SIZE-1);
  }
  while (iEvD != iEvDStop);
#else
  if (pEvD0 == (void*)0) // there were no IncDpcData calls
    __tmkNumber = __tmkSaveNumber;
#endif //nDOS

}

int DpcVirtRTBrc(int hTmk, int hTmkI, int cDpcDataTmk)
{
#ifndef DYNAMIC_TMKNUM
  int hTmkSave;
#endif
  pTListEvD pEvD;
  int iEvD, cEvD;
  int nInDpcType;
  int fWriteSA;
  int fDropInt;
  int fModeCmd;
  int fDataOk;
  unsigned short wSA, nWords;
  unsigned short wSavePage, wSaveSA, wSaveBuf;
  unsigned short wBuf;
  unsigned short base, link, basep, state;
  LOCAL_PORT

  SET_LOCAL_PORT(hTmkI);
  nInDpcType = __tmkDrvType[hTmk];
#ifndef DOS
  iEvD = (iEvDataBegin[hTmk] + cEvData[hTmk] - cDpcDataTmk) & (EVENTS_SIZE-1);
  cEvD = cDpcDataTmk;
  do
  {
    pEvD = &(aEvData[hTmk][iEvD]);

#ifdef DDKNT
    #ifdef MY_DBG_DPC
    KdPrint(("MRT1 nInt %d", pEvD->nInt));
    #endif
#endif //DDKNT
#if defined(LINUX) || defined(QNX6)
    #ifdef MY_DBG_DPC
    printk(MY_KERN_DEBUG "MRT1 nInt %d", pEvD->nInt);
    #endif
#endif //LINUX QNX6

#else
    pEvD = &(__aEvData[hTmk]);
#endif //nDOS

    if (pEvD->nInt == 4)
    {
//      if (__tmkOptions[hTmk] & (UOPT)RT_NO_BRC_IRQS)
      pEvD->nInt = (int)(pEvD->awEvData[1]);
      pEvD->wMode = RT_MODE;
      pEvD->awEvData[1] = 0; // buf 0
      base = 0xFFFF;
      basep = 0xFFFF;

#ifdef DDKNT
      #ifdef MY_DBG_DPC
      KdPrint((" -> %d", pEvD->nInt));
      #endif
#endif //DDKNT
#if defined(LINUX) || defined(QNX6)
      #ifdef MY_DBG_DPC
      printk(MY_KERN_DEBUG " -> %d\n", pEvD->nInt);
      #endif
#endif //LINUX QNX6

      fModeCmd = ((pEvD->awEvData[0] & 0x03E0) == 0x0000) || ((pEvD->awEvData[0] & 0x03E0) == 0x03E0);

      if (!fModeCmd && ((pEvD->nInt == 3) || (pEvD->nInt == 2)))
      {
#ifndef DYNAMIC_TMKNUM
        hTmkSave = tmkselected();
        tmkselect(hTmk);
#endif
        fDataOk = (pEvD->nInt == 3);
        fDropInt = fDataOk && (rtgetirqmode(HTMK) & RT_DATA_BL);

        if (__tmkOptions[hTmk] & (UOPT)MRT_WRITE_BRC_DATA)
        {
          fWriteSA = 0;
          if (rtgetanswbits(HTMK) & BUSY)
          {
            fDropInt = 1;
          }
          else
          {
            wSA = (pEvD->awEvData[0] >> 5) & 0x1F;
#ifdef USE_TMK_WRAP_SA
            if (__tmkWrapSA)
            {
              if (wSA == __tmkWrapSA && !__tmkWrapInt)
                fDropInt = 1;
            }
#endif
            wSaveSA = rtgetsubaddr(HTMK);  // how about saves of mrt data ???
            wSaveSA = (wSaveSA & RT_DIR_MASK) | ((wSaveSA & 0x1F) << 5); //for DrvRtDefSubAddr
            if (nInDpcType == __MRTA)
            {
              wSavePage = rtgetpage(HTMK);
              wSaveBuf = rtgetbuf(HTMK);
            }
            else
            {
              wSavePage = 0xFFFF;
              wSaveBuf = 0;
            }
            if (__rtSABufMode[hTmk][wSA] == SABUF_OFF)
            {
              if (rtgetmode(HTMK) & RT_FLAG_MODE)
              {
                fWriteSA = !(rtgetflag(HTMK__ RT_RECEIVE, wSA) & RT_FLAG_MASK);
                DrvRtDefSubAddr(hTmk, nInDpcType, wSaveSA); //wSaveSA, wSaveSA);
              }
              else
              {
                fWriteSA = !(rtgetlock(HTMK) == (wSA | 0x0800));
              }
              fDropInt |= !fWriteSA;
              fWriteSA &= fDataOk || (nInDpcType == __MRTA); //!!! process MRTA state for nint=2 too
              if (fWriteSA)
              {
//tr('w');
                nWords = pEvD->awEvData[0] & 0x1F;
                if (nWords == 0)
                  nWords = 32;
                // wBuf = 0;
                DrvRtDefSubAddr(hTmk, nInDpcType, wSA<<5); //RT_RECEIVE, wSA);
                if (nInDpcType == __MRTA)
                {
                  if (fDataOk)
                    __rtputblkmrta(HTMK__ 0, &(__awBrcRxBuf[hTmkI][wSA<<5]), nWords);
                  __rtputblkmrta(HTMK__ 58, &(__awBrcRxState[hTmkI][wSA<<2]), 3);
                  // rtputblk does not accept position 58
                }
                else // if (nInDpcType == __MRTX)
                  rtputblk(HTMK__ 0, &(__awBrcRxBuf[hTmkI][wSA<<5]), nWords);
                if (fDataOk)
                  if (rtgetmode(HTMK) & RT_FLAG_MODE)
                    rtputflag(HTMK__ RT_RECEIVE, wSA, (U16)(pEvD->awEvData[0] | RT_FLAG_MASK)); // rtsetflag(HTMK);
                //moved after all
                //DrvRtDefSubAddr(hTmk, nInDpcType, wSaveSA); //wSaveSA, wSaveSA);
                // no rtlock restore because of DrvRtDefSubAddr instead of rtdefsubaddr
                pEvD->awEvData[1] = 0; // wBuf;
/*
                // how it was before:
                if (rtgetmode(HTMK) & RT_FLAG_MODE)
                {
                  rtputflag(HTMK__ RT_RECEIVE, wSA, pEvD->awEvData[0] | RT_FLAG_MASK); // rtsetflag(HTMK);
                  rtdefsubaddr(HTMK__ wSaveSA, wSaveSA);
                }
                else
                {
                  if (wSaveLock & 0x0800)
                    rtlock(HTMK__ wSaveLock, wSaveLock);
                  else
                    rtdefsubaddr(HTMK__ wSaveSA, wSaveSA);
                }
*/
              }
            }
            else if (nInDpcType == __MRTA)
            {
              GET_DIS_IRQ();
//              GET_DIS_IRQ();
              link = DrvRtPeekMRTA(hTmkI, __rtAddress[hTmk], wSA, basep);
//              REST_IRQ();
              basep = __rtAddress[hTmk];
              base = link & 0x0FFF;
              fWriteSA = ((link & 0x8000) == 0);
              fDropInt |= !fWriteSA;
              fDropInt |= ((link & 0x4000) == 0);
//              fWriteSA &= fDataOk; !!! process state for nint=2 too
              if (fWriteSA)
              {
                // check rtbusy
//                GET_DIS_IRQ();
                outpw(TA_LCW(hTmkI), __rtControls1[hTmk] & 0xF800); // no matter if TA_BRCST_MODE here
                state = inpw(TA_BASE(hTmkI));
//                REST_IRQ();
                // data write (if fWriteSA) should start within 20 us !
                if (((state & 0x2000) != 0) && ((state & 0x0FFF) == base))
                { // base active?
/*
                  if (__rtSABufMode[hTmk][wSA] != SABUF_OFF) // __MRTA
                  {
                    // note that this brc msg will be written in the next
                    // buf num after current active buf however brc event
                    // will occur before event from current active buf !
                    //
                    // also that would create a strange situation of more
                    // than one active buf simultaneously, that conflicts
                    // with double buffering approach
                    //
//                    GET_DIS_IRQ();
                    link = DrvRtPeekMRTA(hTmkI, base, 63, basep);
//                    REST_IRQ();
                    basep = base;
                    fWriteSA = ((link & 0x8000) == 0) && ((link & 0x0FFF) != (base & 0x0FFF));
                    if (fWriteSA)
                      base = link & 0x0FFF;
                  }
                  else
*/
                  {
                    fWriteSA = 0;
                    fDropInt = 0; //1; // ???
                    pEvD->nInt = 5;
                    pEvD->wMode = UNDEFINED_MODE;
                    pEvD->awEvData[0] = TMK_INT_DRV_BUF_SKIP;
                  }
                }
              }
              if (fWriteSA)
              {
//tr('w');
                nWords = pEvD->awEvData[0] & 0x1F;
                if (nWords == 0)
                  nWords = 32;
                wBuf = 0;
                if ((base & 0x0800) == 0) // not buf 0 by default in MRTA
                  wBuf = base - __rtSABufExt[hTmk][wSA] + 1;
                rtdefpage(HTMK__ 0);
                DrvRtDefSubAddr(hTmk, nInDpcType, wSA<<5); //RT_RECEIVE, wSA);
                if (wBuf != 0)
                  rtdefbuf(HTMK__ wBuf);
                if (fDataOk)
                  __rtputblkmrtairq(HTMK__ 0, &(__awBrcRxBuf[hTmkI][wSA<<5]), nWords);
                __rtputblkmrtairq(HTMK__ 58, &(__awBrcRxState[hTmkI][wSA<<2]), 3);
                // rtputblk does not accept position 58
                //moved after all
                //rtdefpage(HTMK__ wSavePage);
                //DrvRtDefSubAddr(hTmk, nInDpcType, wSaveSA); //wSaveSA, wSaveSA);
                //if (wSaveBuf != 0)
                //  rtdefbuf(HTMK__ wSaveBuf);
                pEvD->awEvData[1] = wBuf;

                basep = 0xFFFF;
                // recheck rtbusy after data write
//                GET_DIS_IRQ();
//                outpw(TA_LCW(hTmkI), __rtControls1[hTmk] & 0xF800); // no matter if TA_BRCST_MODE here
                state = inpw(TA_BASE(hTmkI));
//                REST_IRQ();
                if (((state & 0x2000) != 0) && ((state & 0x0FFF) == base))
                { // if base became active then ignore written data
                  // as they should be overwritten by new message
                  //fWriteSA = 0; // !!! 1 needed for further SA restore
                  fDropInt = 0; //1; // ???
                  pEvD->nInt = 5;
                  pEvD->wMode = UNDEFINED_MODE;
                  pEvD->awEvData[0] = TMK_INT_DRV_BUF_SKIP;
                }
                else
                {
                  // update buf link in table
//                  GET_DIS_IRQ();
                  link = DrvRtPeekMRTA(hTmkI, base, 63, basep);
                  basep = base;
                  DrvRtPokeMRTA(hTmkI, __rtAddress[hTmk], wSA, link, basep);
                  basep = __rtAddress[hTmk];
//                  REST_IRQ();
                  if ((link & 0xFFFF) != base)
                  {
                    // if link points to a different base then
                    // recheck rtbusy after table update
//                    GET_DIS_IRQ();
//                    outpw(TA_LCW(hTmkI), __rtControls1[hTmk] & 0xF800); // no matter if TA_BRCST_MODE here
                    state = inpw(TA_BASE(hTmkI));
//                    REST_IRQ();
                    if (((state & 0x2000) != 0) && ((state & 0x0FFF) == base))
                    { // if base became active after table update then at first
                      // ignore written data as they should be overwritten by
                      // new message and then leave as is or adjust table link
                      //fWriteSA = 0; // !!! 1 needed for further SA restore
                      fDropInt = 0; //1; // ???
                      pEvD->nInt = 5;
                      pEvD->wMode = UNDEFINED_MODE;
                      pEvD->awEvData[0] = TMK_INT_DRV_BUF_SKIP;
                      // if new message in the same subaddr was started within
                      // that several microseconds between inpw(TA_BASE) and
                      // DrvRtPokeMRTA() then with MRT_WRITE_BRC_DATA option
                      // we are in a tight spot and without a sw driver buffer
                      // we should either to respin in the same hw buffer or
                      // to proceed with the written link in the table;
                      // if we respin then 2 consequent messages will get into
                      // the same buffer, this provides proper rtgetlinkbus
                      // results and should work smoothly with double buffering
                      // (when CPU looks for and uses non-busy buffers),
                      // however that approach will fail with circular buffers;
                      // if we proceed with the written table link then
                      // rtgetlinkbus result will point to inactive buffer for
                      // some time (during current message) however all data
                      // will get into different buffers, as intended in
                      // circular buffers;
                      // so we will change behavior depending on buffer size
                      // assuming bufsize=2 for double buffering and
                      // any higher value for circular buffering
                      if (__rtSABufSize[hTmk][wSA] <= 2)
                      {
//                        GET_DIS_IRQ();
                        DrvRtPokeMRTA(hTmkI, __rtAddress[hTmk], wSA, state & ~0x2000, basep);
                        basep = __rtAddress[hTmk];
//                        REST_IRQ();
                      }
                    }
                  }
                }
              }
              REST_IRQ();
            }
            if (fWriteSA)
            {
              if (wSavePage != 0xFFFF)
                rtdefpage(HTMK__ wSavePage);
              DrvRtDefSubAddr(hTmk, nInDpcType, wSaveSA); //wSaveSA, wSaveSA);
              if (wSaveBuf != 0)
                rtdefbuf(HTMK__ wSaveBuf);
            }
          }
        }
        if (fDropInt)
        {
          pEvD->nInt = 0;
#ifndef DOS
          if (iEvD == iEvDataBegin[hTmk])
          {
            iEvDataBegin[hTmk] = (iEvDataBegin[hTmk] + 1) & (EVENTS_SIZE-1);
            --cEvData[hTmk];
            --cDpcDataTmk;
          }
#endif //nDOS
        }
#ifndef DYNAMIC_TMKNUM
        tmkselect(hTmkSave);
#endif
      }
      else if (fModeCmd && ((pEvD->nInt == 1) || (pEvD->nInt == 2)))
      {
        wBuf = 0;
        if (nInDpcType == __MRTA)
        {
          if (__rtSABufMode[hTmk][0x1F] != SABUF_OFF) // __MRTA
          {
            // read base saved in irq handler
            base = __awBrcRxBuf[hTmkI][hTmk-__mrtMinRT[hTmkI]];
            if ((base & 0x0800) == 0) // not buf 0 by default in MRTA
              wBuf = base - __rtSABufExt[hTmk][0x1F] + 1;
          }
        }
        pEvD->awEvData[1] = wBuf;
      }
      else if (pEvD->nInt == 5)
      {
        pEvD->wMode = UNDEFINED_MODE;
      }
#ifdef DOS
      if (pEvD->nInt)
        IncDpcData(hTmk);
      else
        __tmkNumber = __tmkSaveNumber;
#endif
    }
#ifdef DOS
    else
      __tmkNumber = __tmkSaveNumber;
#endif

#ifndef DOS

#ifdef DDKNT
    #ifdef MY_DBG_DPC
    KdPrint(("\n"));
    #endif
#endif //DDKNT

    iEvD = (iEvD + 1) & (EVENTS_SIZE-1);
  }
  while (--cEvD);
#endif //nDOS

  return cDpcDataTmk;
}

#endif //NRT

#if defined(DOS) || defined(QNX4INT)

#ifdef MASKTMKS
#define DRVMASKTMKS(num) { \
  if (!__tmkIrqShared[num]) \
  { \
    DrvMaskTmks; \
    DrvEndInt(num); \
  } \
}
#define DRVUNMASKTMKS(num) { \
  disable(); \
  if (__tmkIrqShared[num]) \
    DrvEndInt(num); \
  DrvUnMaskTmks; \
}
#else
#define DRVMASKTMKS
#define DRVUNMASKTMKS
#endif //def MASKTMKS

#ifdef QNX4
#define GETRES proxy =
#define RETRES proxy
#else
#define GETRES
#define RETRES
#endif //def QNX4

RETIR IncDpcData(int REGPAR num)
{
  pTListEvD pEvD;
  unsigned bcAW1, bcAW2;
#ifdef QNX4
  int proxy = 0;
#endif //def QNX4
#if NRT > 0
#define MAX_TMK_NUMBER ((NTMK)-1)
  int hTmk, hTmkI;
  pTListEvD pEvD0;
  unsigned short wSaveSA, wSaveLock;
  int fWriteSA;
  int fDropInt;
  unsigned short wSA, nWords;
  int nInt, hTmk1, hTmkT;
  unsigned short wMask;
#endif //NRT

  pEvD = &(__aEvData[num]);
#ifdef EXT_DPCDATA
  extdpcdata(num);
#endif
  switch (pEvD->wMode)
  {
  case BC_MODE:
    __bcSaveBase = __bcBasePC[num];
    switch (pEvD->nInt)
    {
    case 1:
      DrvBcDefBase(num, __tmkDrvType[num], __bcBaseBus[num]);
      DRVMASKTMKS;
      GETRES tmkUserNormBC[num](0, 0xFFFF, 0xFFFF);
      break;
    case 2:
      DrvBcDefBase(num, __tmkDrvType[num], __bcBaseBus[num]);
      bcAW1 = 0xFFFF;
      bcAW2 = 0xFFFF;
      if (__bcAW1Pos[num])
        bcAW1 = DrvBcPeek(num, __tmkDrvType[num], (unsigned)__bcAW1Pos[num]);
      if (__bcAW2Pos[num])
      {
        if (__bcAW1Pos[num])
          bcAW2 = DrvBcPeek(num, __tmkDrvType[num], (unsigned)__bcAW2Pos[num]);
        else
          bcAW1 = DrvBcPeek(num, __tmkDrvType[num], (unsigned)__bcAW2Pos[num]);
      }
      DRVMASKTMKS;
      GETRES tmkUserExcBC[num](pEvD->awEvData[0], bcAW1, bcAW2);
      break;
    case 3:
      DrvBcDefBase(num, __tmkDrvType[num], pEvD->awEvData[1]); //__bcBaseBus not updated for int3 now
      DRVMASKTMKS;
      GETRES tmkUserXBC[num](pEvD->awEvData[1], pEvD->awEvData[0]);
      break;
    case 4:
      DRVMASKTMKS;
      GETRES tmkUserSigBC[num](pEvD->awEvData[0]);
      __tmkNumber = __tmkSaveNumber;
      break;
    case 6:
      DrvBcDefBase(num, __tmkDrvType[num], pEvD->awEvData[1]); //__bcBaseBus not updated for int6 now
      DRVMASKTMKS;
      GETRES tmkUserABC[num](pEvD->awEvData[1], pEvD->awEvData[0]);
      break;
    }
    break;
  case RT_MODE:
    __rtSaveSubAddr = __rtSubAddr[num];
    __rtSaveSABuf = __rtSABuf[num];
    __rtSavehm400Page = __hm400Page[num];
#ifdef MRTA
    __rtSavehm400Page2 = __hm400Page2[num];
#endif
    __rtSaveMode = __rtMode[num];
    __rtIrqBuf[num] = pEvD->awEvData[1]; // non reentrant buf for rtgetbufirq
    switch (pEvD->nInt)
    {
    case 1:
      DRVMASKTMKS;
      GETRES tmkUserCmdRT[num](pEvD->awEvData[0]);
      break;
    case 2:
      DRVMASKTMKS;
      GETRES tmkUserErrRT[num](pEvD->awEvData[0]);
      break;
    case 3:
      DRVMASKTMKS;
      GETRES tmkUserDataRT[num](pEvD->awEvData[0]);
      break;
    }
    break;
  case MT_MODE:
    __bcSaveBase = __bcBasePC[num];
    switch (pEvD->nInt)
    {
    case 3:
      DrvBcDefBase(num, __tmkDrvType[num], pEvD->awEvData[1] + __rtHeapIni[num]); //__bcBaseBus not updated for int3 now
      DRVMASKTMKS;
      GETRES tmkUserXMT[num](pEvD->awEvData[1], pEvD->awEvData[0]);
      break;
    case 4:
      DRVMASKTMKS;
      GETRES tmkUserSigMT[num](pEvD->awEvData[0]);
      __tmkNumber = __tmkSaveNumber;
      break;
    }
    break;
#if NRT > 0
  case MRT_MODE:
// No DRVMASKTMKS because MASKTMKS should be undefined!
    hTmk = num;
    if (hTmk <= MAX_TMK_NUMBER)
    {
      hTmkI = hTmk;
    }
    else
    {
      hTmkI = rt2mrt(hTmk);
    }

    if (hTmk <= MAX_TMK_NUMBER)// &&
//        (nInDpcType == MRTX || nInDpcType == MRTXI ||
//         nInDpcType == MRTA || nInDpcType == MRTAI))
    {
      DpcPhysMRT(hTmk, 0);
    }

    if (hTmk > MAX_TMK_NUMBER)
//        && ((__tmkOptions[hTmk] & (UOPT)MRT_WRITE_BRC_DATA) ||
//         (__TmkOptions[hTmk] & (UOPT)RT_NO_BRC_IRQS)))
    {
      DpcVirtRTBrc(hTmk, hTmkI, 0);
    }
    break;
#endif //NRT
  case UNDEFINED_MODE:
    switch (pEvD->nInt)
    {
    case 5:
      DRVMASKTMKS;
      GETRES tmkUserOthTMK[num](pEvD->awEvData[0]);
      __tmkNumber = __tmkSaveNumber;
      break;
#ifdef USE_TMKRAWIO
    case 255:
      DRVMASKTMKS;
      GETRES tmkUserRawTMK[num](pEvD->awEvData[0], pEvD->awEvData[1]);
      __tmkNumber = __tmkSaveNumber;
      break;
#endif
    }
    break;
  default:
    DRVMASKTMKS;
    __tmkNumber = __tmkSaveNumber;
    break;
  }
  __tmkSaveNumber = __tmkNumber; // track tmkselect in handlers
  DRVUNMASKTMKS;
  return RETRES;
}

RETIR DrvTmksInt1(int REGPAR num)
{
  int hTmk, hTmk0, hTmk1, nmin, nmax;
  int nInt;
  pTListEvD pEvD, pEvD0;
  unsigned intr;
#ifdef MRTA
  unsigned long saved;
#else
  unsigned saved;
#endif
  U08 irq;
#ifdef QNX4
  int proxy = 0;
  IRQ_FLAGS;
#endif

#ifdef DOS32
  restes();
#endif //def DOS32
  __tmkSaveNumber = __tmkNumber;
  if (__tmkIrqShared[num])
  {
    nmin = 0;
    nmax = NTMK - 1;
  }
  else
  {
    nmin = nmax = num;
  }
  irq = __tmkIrq1[num];
  for (hTmk = nmin; hTmk <= nmax; ++hTmk)
  {
    if (irq != __tmkIrq1[hTmk])
      continue;
    if (__tmkIrqOff[hTmk])
      continue;
    if (__tmkIrqBit[hTmk] != 0 && (inpw(__tmkIrqPort[hTmk]) & __tmkIrqBit[hTmk]) == 0)
      continue;
//    __tmkInIrqNumber = __tmkNumber = hTmk;
    __tmkInIrqNumber = hTmk;
    __tmkSaveRAMAddr = __tmkRAMAddr[hTmk];
    __tmkSaveRAMInWork = __tmkRAMInWork[hTmk];
//    __tmkRAMInWork[hTmk] = 0;
    saved = DIRQLTmkSave(hTmk);
#ifdef QNX4
    GET_DIS_IRQ();
#endif
    do
    {
      pEvD = &(__aEvData[hTmk]);

      intr = DIRQLTmksInt1(hTmk, pEvD);

      if (intr & TMK_INT_SAVED)
      {
        __tmkNumber = hTmk;
        __tmkRAMInWork[hTmk] = 0;
        GETRES IncDpcData(hTmk);
//        fTaskletSchedule = 1;
      }
      if (intr & TMK_INT_OTHER) //TMK_INT_TIMER | TMK_INT_BUSJAM | TMK_INT_FIFO_OVF | TMK_INT_GEN1 | TMK_INT_GEN2...
      {
        pEvD = &(__aEvData[hTmk]);
        pEvD->nInt = 5;
        pEvD->wMode = UNDEFINED_MODE;
        pEvD->awEvData[0] = (unsigned short) (intr & TMK_INT_OTHER);
//        *((ULONG*)(&(pEvD->awEvData[1]))) = tmkgettimer(hTmk);
        pEvD->awEvData[1] = 0;
        pEvD->awEvData[2] = 0;
        __tmkNumber = hTmk;
        __tmkRAMInWork[hTmk] = 0;
        GETRES IncDpcData(hTmk);
//        fTaskletSchedule = 1;
      }
    }
    while (intr & TMK_INT_MORE);
#ifdef QNX4
    REST_IRQ();
#endif
    DIRQLTmkRestore(hTmk, saved);
  }
#ifdef DOS
#ifndef MASKTMKS
  DrvEndInt(num);
#endif
#endif
  return RETRES;
}

#endif //def DOS || QNX4INT

#if defined(MRTA) || defined(TSTA)
unsigned long DIRQLTmkSave(int hTMK)
{
  LOCAL_PORT

  SET_LOCAL_PORT(hTMK);
  switch(__tmkDrvType[hTMK])
  {
  case __TA:
    return (unsigned long)inpw(TA_ADDR(hTMK));
#ifdef MRTA
  case __MRTA:
#endif
#ifdef TSTA
  case __TSTA:
#endif
    return (((unsigned long)inpw(MRTA_ADDR2(hTMK)) << 16) + (unsigned long)inpw(TA_ADDR(hTMK)));
  }
  return 0L;
}

void DIRQLTmkRestore(int hTMK, unsigned long Saved)
{
  LOCAL_PORT

  SET_LOCAL_PORT(hTMK);
  switch(__tmkDrvType[hTMK])
  {
  case __TA:
    outpw(TA_ADDR(hTMK), Saved);
    break;
#ifdef MRTA
  case __MRTA:
#endif
#ifdef TSTA
  case __TSTA:
#endif
    outpw(MRTA_ADDR2(hTMK), (unsigned)(Saved >> 16));
    outpw(TA_ADDR(hTMK), (unsigned)Saved);
    break;
  }
  return;
}
#else
unsigned DIRQLTmkSave(int hTMK)
{
  if (__tmkDrvType[hTMK] == __TA)
    return inpw(__tmkPortsAddr1[hTMK] + TMK_AddrPort);
  else
    return 0;
}

void DIRQLTmkRestore(int hTMK, unsigned Saved)
{
  if (__tmkDrvType[hTMK] == __TA)
    outpw(__tmkPortsAddr1[hTMK] + TMK_AddrPort, Saved);
  return;
}
#endif //def MRTA

unsigned DIRQLTmksInt1(int hTMK, void *pEvData)
{
  int register num;
  unsigned type;
  unsigned t;
  U16 FARDT *pevd;
  unsigned mode;
  LOCAL_PORT
  unsigned intd=0;
  unsigned nint=0;
  unsigned intm;
  int limit;
  unsigned irt, nrt;
  unsigned pos, data;
  unsigned res;
  unsigned vp;
  unsigned msgBase;
  unsigned msgSW;
  unsigned sa;
#ifdef MRTA
  unsigned base, link;
  unsigned state58, state59, state60;
  U08 rxbus;
#endif

  pevd = (U16 FARDT*)pEvData;
  num = hTMK;
  SET_LOCAL_PORT(num);
  type = __tmkDrvType[num];
  mode = __tmkMode[num];
  res = TMK_INT_SAVED;
  if (type == __TA)
  {
    vp = inpw(TA_IRQ(num)) & 0xE7FF;
    if (vp & 0x4000) // Timer Overflow == TMK_INT_TIMER
    {
//      if (!ts_TimerLoop[num])
//      if ((inpw(TA_TIMCR(num) & 0x2000) == 0)
      if (inpw(TA_RESET(num)) & 0x4000)
      {
        inpandoroutpw(TA_TIMCR(num), ~0x0400, 0);
//        outpw(TA_TIMCR(num), ts_TimerCtrl[num] & ~TS_TIMER_ENABLE);
//        outpw(TA_TIMCR(num), ts_TimerCtrl[num]);
      }
    }
    if (vp & 0x2000) // Bus Self Jum == TMK_INT_BUSJAM
    {
      if (inpw(TA_RESET(num)) & 0x2000)
      {
//      ts_IrqEn = 0;
        inpandoroutpw(TA_MODE1(num), ~0x0400, 0);
      }
    }
    if (vp & 0x8000) // FIFO Not Empty == TMK_INT_MORE
    {
      msgBase = vp & 0x03FF;
      switch (mode)
      {
        case MTRT_MODE:
        case MWRT_MODE:
        case MWTRT_MODE:
          if (msgBase < __rtHeapIni[num])
          {
            mode = RT_MODE;
            break;
          }
          // no break here
          fallthrough;
          // fall-through
        case MWT_MODE:
        case MW_MODE:
          mode = MT_MODE;
          break;
      }
      if (mode == MT_MODE && (__mtCW[num] & 0x30) == 0x30)
        msgSW = 0; // unused
      else
      {
        outpw(TA_ADDR(num), (msgBase << 6) + 58);
        msgSW = inpw(TA_DATA(num));
      }
      pevd[2] = mode; //pevd->4 = mode; //[esi+4] //; wMode
      pevd[5] = 0; //pevd->10 = 0; //[esi+10]  //; bc.wAW2, others.dummy
      switch (mode)
      {
      case BC_MODE:
        if ((msgSW & 0x0200) == 0)
        {
          intd = inpw(TA_MSGA(num)); // maybe __bcLinkBaseN?
          intd &= 0x03FF;          //; bcx.wBase
          nint = 4;                //; nInt = 4
        }
        else
        {
          __tmkStarted[num] = 0;
          intd = msgSW & 0x0007;   //; bcx.wResultX
          if (msgSW & 0x0030)
            intd |= 0x0008;
          __bcmtStateX[num] = intd;
//          if ((__bcXAStart[num] & 2) && (__tmkOptions[num] & (UOPT)BC_STARTA_INTA))
          if (__bcXAStart[num] & 2) // moved BC_STARTA_INTA checking to start
          {
            pevd[4] = msgBase; //; bca.wBase
            intd = msgSW;      //; bca.wResultA
            nint = 6;          //; nInt = 6
          }
          else
          {
            if (__bcXAStart[num] & 3) // 1 | 2
            {
              //; use intd as bcx.wResultX
              pevd[4] = msgBase; //pevd->8 = (intd >> 6) & __bcMaxBase[num]; //[esi+8] //; bcx.wBase
    //;          __bcBaseBus[num] = intd;
              nint = 3;                //; nInt = 3
            }
            else
            {
              intd = __bcExt2StdResult[intd]; //; bc.wResult
              nint = 1;                //; nInt = 1
              if (intd)
              {
                ++nint;                   //; nInt = 2
                pevd[5] = 1; //pevd->10 = 1; //[esi+10] //; bc.wAW2
              }
            }
          }
        }
        break;
      case RT_MODE:
        intd = msgSW;
        nint = 2;               //; nInt = 2

        if ((pevd[4] = (msgBase >> 6)) != 0) // RT buf number for SABUF_STD
        {
          sa = (msgSW >> 5) & 0x003F;
          if (__rtSABufMode[num][sa] == SABUF_HEAP)
            pevd[4] = msgBase - __rtSABufExt[num][sa] + 1;
        }
        if (__tmkHWVer[num] < 9)
        {
          if (intd & 0x2000)
            intd = (intd & 0x1FFF) | 0x4000;   //; rt.wStatus
        }
        if (intd & 0x4000)
        {
          intd &= 0x47FF; //0x5FFF;
          if (__FLAG_MODE[num] & DRV_FLAG_MODE_HW) // && RtIrqOnErrorEnabled
          {
            t = DrvRtPeekTA(num, AdrTab, msgBase);
            if (t & 0x8000)
              DrvRtPokeTA(num, AdrTab, msgBase, t & 0x7FFF);
//            DrvRtPokeTA(num, msgBase, 58, 0);
          }
        }
        else if ((intd & 0x03E0) == 0 || (intd & 0x03E0) == 0x03E0)
        {
          --nint;                   //; nInt = 1
          intd &= 0x041F;            //; rt.wCmd
          switch (intd)
          {
          case 0x011: //syn w data
          case 0x014: //sel tx shd
          case 0x015: //ovr sel tx shd
            if ((__rtControls1[num] & BUSY_MASK) &&
               !(__rtControls[num] & (TA_CBSD_MODE | TA_CNOBS_MODE)))
              break;
            outpw(TA_ADDR(num), (msgBase << 6));
            __rtRxDataCmd[num][intd-0x11] = inpw(TA_DATA(num));
            break;
          }
        }
        else
        {
          ++nint;                   //; nInt = 3
          intd &= 0x07FF;            //; rt.wCmd
        }
        intd |= ((msgSW & SMR_BRC_MASK) << 3) & ((unsigned)(__tmkOptions[num] & (UOPT)RT_SHOW_BRC_STATE) << 9);
        break;
      case MT_MODE:
//          asm db 0cch;
        if ((__mtCW[num] & 0x10) == 0 || //CX_STOP
           ((__mtCW[num] & 0x20) == 0 && ((((msgSW & 0x3C0) != 0x3C0) && (msgSW & 0x37)) || ((msgSW & 0x3C0) == 0x1C0)))) //CX_INT
        {
//          asm db 0cch;
          inpandoroutpw(TA_MODE1(num), 0xFFF7, 0); //mtstop
          __tmkStarted[num] = 0;
          DrvRtPokeTA(num, AdrTab, 0, msgBase|0x4000); // for mtgetstate
          pevd[4] = msgBase - __rtHeapIni[num]; //pevd->8 = intd; //[esi+8]//; mt.wBase
          intd = (msgSW & 0x0007) |
                ((msgSW & 0x03C0) << 4) |
                ((msgSW & 0x0008) << 12);   //; mt.wResultX
          if (msgSW & 0x0030)
            intd |= 0x0008;
          if (((msgSW & 0x3C0) != 0x3C0) && ((msgSW & 0x07) == 0x07) && (__tmkHWVer[num] < 19)) // bus switching
            intd = ((intd & ~0x3C00) | 0x1C00) ^ 0x8000;
// any error or format error or bit set (integrated bit)
          if ((((msgSW & 0x3C0) != 0x3C0) && (msgSW & 0x37)) || ((msgSW & 0x3C0) == 0x1C0))
            intd |= 0x4000;
          __bcmtStateX[num] = intd;
          nint = 3;                //; nInt = 3
        }
        else
        {
#if DRV_MAX_BASE > 255
          if (__bcLinkCCN[num][msgBase] & 0x8000) //CX_SIG
          {
            intd = __bcLinkBaseN[num][msgBase] - __rtHeapIni[num];
//            intd &= __bcMaxBase[num];  //; mt.wBase
            nint = 4;                //; nInt = 4
          }
#else
          if (__bcLinkWN[num][msgBase] & 0x8000) //CX_SIG
          {
            intd = ((__bcLinkWN[num][msgBase] >> 6) & __bcMaxBase[num]) - __rtHeapIni[num];
//            intd &= __bcMaxBase[num];  //; mt.wBase
            nint = 4;                //; nInt = 4
          }
#endif
          else
          {
            nint = 0;
            intd = 0;
            res = 0; // no TMK_INT_SAVED
          }
#if DRV_MAX_BASE > 255
          __mtCW[num] = __bcLinkCCN[num][msgBase];
#else
          __mtCW[num] = __bcLinkWN[num][msgBase];
#endif
        }
        break;
#ifdef USE_TMKRAWIO
      case UNDEFINED_MODE:
        nint = 255;
        intd = vp;
        pevd[4] = msgSW;
        break;
#endif
      }
    }
    else
    {
      nint = 0;
      intd = 0;
      res = 0; // no TMK_INT_SAVED
    }
//    res |= (vp & 0x0640);
    res |= (vp & 0xE400);
  }
#ifdef MRTA
  else if (type == __MRTA)
  {
    vp = inpw(TA_IRQ(num)) & 0xEFFF;
    if (vp & 0x4000) // Timer Overflow == TMK_INT_TIMER
    {
//      if (!ts_TimerLoop[num])
//      if ((inpw(TA_TIMCR(num) & 0x2000) == 0)
      if (inpw(TA_RESET(num)) & 0x4000)
      {
        inpandoroutpw(TA_TIMCR(num), ~0x0400, 0);
//        outpw(TA_TIMCR(num), ts_TimerCtrl[num] & ~TS_TIMER_ENABLE);
//        outpw(TA_TIMCR(num), ts_TimerCtrl[num]);
      }
    }
    if (vp & 0x8000) // FIFO Not Empty == TMK_INT_MORE
    {
      msgBase = vp & 0x0FFF;
      outpw(MRTA_ADDR2(num), msgBase >> 10);
      outpw(TA_ADDR(num), (msgBase << 6) + 58);
      msgSW = inpw(TA_DATA(num));
      pevd[2] = mode; //pevd->4 = mode; //[esi+4] //; wMode
      pevd[4] = 0;
      pevd[5] = 0; //pevd->10 = 0; //[esi+10]  //; bc.wAW2, others.dummy

#ifdef USE_TMKRAWIO
      switch (mode)
      {
      case MRT_MODE:
#endif
      intd = msgSW;

//      if (__tmkHWVer[num] < 9)
//      {
//        if (intd & 0x2000)
//          intd = (intd & 0x1FFF) | 0x4000;   //; rt.wStatus
//      }
      intd = (intd & 0x1FFF) | ((intd << 1) & 0x4000); // make unified MRT intd

      if ((intd & 0x1000) == 0 || ((intd & 0x1400) == 0x1400 && !((intd & 0x03E0) == 0 || (intd & 0x03E0) == 0x03E0)))       //; !brc ?
                      // mrtBrcTxRT ^^^^^^^^^^^^^^^^^^^^^^^^ //
      {
        __mrtLastBrcTxRT[num] = 0;
        if (msgBase & 0x0800) // high half memory for RTs, buf 0
        {
          pevd[4] = __mrtA2RT[num][(msgBase & 0x7FF) >> 6]; //pevd->8 = (intd >> 12) + __mrtMinRT[num];//[esi+8] //; rt number
          pevd[5] = 0; // buf 0
        }
        else // low half memory, not buf 0, SABUF_HEAP by default
        {
          switch (msgSW & 0x03E0)
          {
          case 0x0000:
          case 0x03E0:
            sa = 0x001F;
            switch (msgSW & 0x041F)
            {
            case 0x0410: // tvec
              sa = 0x0020;
              break;
            case 0x0413: // tvsk
              sa = 0x003F;
              break;
            }
            break;
          default:
            sa = (msgSW >> 5) & 0x003F;
            break;
          }
          irt = __mrtMinRT[num];
          nrt = __mrtNRT[num];
          while (nrt > 0)
          {
            if (__rtAddress[irt] != 0x00FF)
            {
              if ((unsigned)(msgBase - __rtSABufExt[irt][sa] + 1) <  (unsigned)(__rtSABufSize[irt][sa]))
              {
                pevd[4] = irt;
                pevd[5] = (msgBase - __rtSABufExt[irt][sa] + 1) << 5;
                break;
              }
            }
            ++irt;
            --nrt;
          }
          if (nrt == 0)
          {
            pevd[4] = num;
            intd |= 0x4000; // never should get here, indicate fake error
          }
        }
        nint = 2;                //; nInt = 2
        if (intd & 0x4000)
        {
          intd &= 0x47FF; //0x5FFF;
          if (__FLAG_MODE[pevd[4]] & DRV_FLAG_MODE_HW) // && RtIrqOnErrorEnabled
          {
            outpw(MRTA_ADDR2(num), 0);
            t = DrvRtPeekTA(num, 0, msgBase & 0x7FF);
            if (t & 0x8000)
              DrvRtPokeTA(num, 0, msgBase & 0x7FF, t & 0x7FFF);
//            outpw(MRTA_ADDR2(num), msgBase >> 10);
//            DrvRtPokeTA(num, msgBase, 58, 0);
          }
        }
        else if ((intd & 0x03E0) == 0 || (intd & 0x03E0) == 0x03E0)
        {
          --nint;                   //; nInt = 1
          intd &= 0x041F;            //; rt.wCmd
          switch (intd)
          {
          case 0x011: //syn w data
          case 0x014: //sel tx shd
          case 0x015: //ovr sel tx shd
            if ((__rtControls1[pevd[4]] & BUSY_MASK) &&
               !(__rtControls[num] & (TA_CBSD_MODE | TA_CNOBS_MODE)))
              break;
            outpw(TA_ADDR(num), (msgBase << 6));
            __rtRxDataCmd[pevd[4]][intd-0x11] = inpw(TA_DATA(num));
            break;
          }
        }
        else
        {
          if ((intd & 0x1400) == 0x1400)
            __mrtLastBrcTxRT[num] = pevd[4];
          ++nint;                   //; nInt = 3
          intd &= 0x07FF;            //; rt.wCmd
        }
      }
      else // brc
      {
        pevd[4] = __mrtMinRT[num]; //pevd->8 = __mrtMinRT[num];//[esi+8]      //; rt number
        pevd[5] = __mrtNRT[num]; //pevd->10 = __mrtNRT[num];//[esi+10]
        rxbus = ((U08)(msgSW >> 5) & 0x40) | ((U08)(~msgSW >> 4) & 0x80);
        if ((msgBase & 0x0800) == 0) // low half memory, not buf 0, SABUF_HEAP by default
        {
          switch (msgSW & 0x03E0)
          {
          case 0x0000:
          case 0x03E0:
            sa = 0x001F;
            switch (msgSW & 0x041F)
            {
            case 0x0410: // tvec
              sa = 0x0020;
              break;
            case 0x0413: // tvsk
              sa = 0x003F;
              break;
            }
            break;
          default:
            sa = (msgSW >> 5) & 0x003F;
            break;
          }
          pevd[5] |= (msgBase - __rtSABufExt[num][sa] + 1) << 5; // buf
        }
        intm = intd;
        if ((intm & 0x03E0) == 0 || (intm & 0x03E0) == 0x03E0) // cmd ?
        {
          intm &= 0x041F;
          pos = msgBase >> 10;
//          outpw(MRTA_ADDR2(num), msgBase >> 10); //already written
//!!!       outpw(TA_ADDR(num), (msgBase << 6) + 58); //already written and msgSW read!
          state58 = msgSW; //!!!inpw(TA_DATA(num));
          state59 = inpw(TA_DATA(num));
          state60 = inpw(TA_DATA(num));
          data = 0; // suppress untitialized data warning
          if ((intd & 0x4000) == 0)  //; !error ?
          {
            switch (intm)
            {
            case 0x011: //syn w data
            case 0x014: //sel tx shd
            case 0x015: //ovr sel tx shd
              outpw(TA_ADDR(num), (msgBase << 6));
              data = inpw(TA_DATA(num));
              break;
            }
          }
//!!! move to DPC? !!!
          irt = __mrtMinRT[num];
          nrt = __mrtNRT[num];
          while (nrt > 0)
          {
            if (__rtAddress[irt] < 31 && (__rtControls1[irt] & TA_BRCST_MODE) && (rxbus & ~__mrtRxCtrl[irt]))
            {
              if (__rtSABufMode[irt][0x1F] != SABUF_OFF)
              {
                if (pos != 0)
                {
                  pos = 0;
                  outpw(MRTA_ADDR2(num), 0);
                }
                outpw(TA_ADDR(num), (__rtAddress[irt] << 6) | 0x1F);
                base = inpw(TA_DATA(num));
                // ??? check here for BUSY in base?
                //...
                // ??? check here for rtbusy of base?
                //...
                if (pos != ((base & 0x0FFF) >> 10))
                {
                  pos = (base & 0x0FFF) >> 10;
                  outpw(MRTA_ADDR2(num), pos);
                }
                outpw(TA_ADDR(num), ((base & 0x3FF) << 6) | 63);
                link = inpw(TA_DATA(num));
                if (pos != 0)
                {
                  pos = 0;
                  outpw(MRTA_ADDR2(num), 0);
                }
                outpw(TA_ADDR(num), (__rtAddress[irt] << 6) | 0x1F);
                outpw(TA_DATA(num), link);
              }
              else
                base = (__hm400Page0[irt] << 6) | 0x1F;
              // ??? check here for BUSY in base?
              // ...
              // ??? check here for rtbusy of base?
              //...
              base &= 0x0FFF; // will we need BUSY and IRQ bits further?
              if (pos != (base >> 10))
              {
                pos = base >> 10;
                outpw(MRTA_ADDR2(num), pos);
              }
              if ((intd & 0x4000) == 0)  //; !error ?
              {
                switch (intm)
                {
                case 0x011: //syn w data
                case 0x014: //sel tx shd
                case 0x015: //ovr sel tx shd
                  if ((__rtControls1[irt] & BUSY_MASK) &&
                     !(__rtControls[num] & (TA_CBSD_MODE | TA_CNOBS_MODE)))
                    break;
                  outpw(TA_ADDR(num), base << 6); // actually, this write is not used further
                  outpw(TA_DATA(num), data);      // except if you read subaddr directly
                  __rtRxDataCmd[irt][intm-0x11] = data;
                  break;
                }
              }
              outpw(TA_ADDR(num), (base << 6) | 58);
              outpw(TA_DATA(num), state58);
              outpw(TA_DATA(num), state59);
              outpw(TA_DATA(num), state60);
              __awBrcRxBuf[num][irt-__mrtMinRT[num]] = base; // save base for DPC in sa 0 (from mt) space
            }
            ++irt;
            --nrt;
          }
        }
        else // !cmd
        {
          if (__tmkHWVer[num] >= 6)
          {
            __mrtLastBrcTxRT[num] = inpw(MRTA_BASE2(num)) >> 11; // read from FIFO after TA_IRQ(num)
            if (__mrtLastBrcTxRT[num] != 31)
              __mrtLastBrcTxRT[num] = __mrtA2RT[num][__mrtLastBrcTxRT[num]];
            else
              __mrtLastBrcTxRT[num] = 0;
          }
          if (__mrtLastBrcTxRT[num]) // limit the number to fit into 5 bits
             __mrtLastBrcTxRT[num] = __mrtLastBrcTxRT[num] - __mrtMinRT[num] + 1;
          pevd[4] |= __mrtLastBrcTxRT[num] << 11; // this rt on mrt is brc transmitter
        }
        nint = 4;                //; nInt = 4
        __mrtLastBrcTxRT[num] = 0;
      }
#ifdef USE_TMKRAWIO
        break;
      case UNDEFINED_MODE:
        nint = 255;
        intd = vp;
        pevd[4] = msgSW;
        break;
      }
#endif
    }
    else
    {
      nint = 0;
      intd = 0;
      res = 0; // no TMK_INT_SAVED
    }
    res |= (vp & 0xC000);
    if (vp & 0x2000) // MRTA FIFO Overflow
      res |= 0x0400; // TMK_INT_FIFO_OVF
  }
#endif //def MRTA
#ifdef TSTA
  else if (type == __TSTA)
  {
    vp = inpw(TA_IRQ(num)) & 0xEFFF;
    if (vp & 0x4000) // Timer Overflow == TMK_INT_TIMER
    {
//      if (!ts_TimerLoop[num])
//      if ((inpw(TA_TIMCR(num) & 0x2000) == 0)
      if (inpw(TA_RESET(num)) & 0x4000)
      {
        inpandoroutpw(TA_TIMCR(num), ~0x0400, 0);
//        outpw(TA_TIMCR(num), ts_TimerCtrl[num] & ~TS_TIMER_ENABLE);
//        outpw(TA_TIMCR(num), ts_TimerCtrl[num]);
      }
    }
    if (vp & 0x8000) // FIFO Not Empty == TMK_INT_MORE
    {
      msgBase = vp & 0x0FFF;
      outpw(MRTA_ADDR2(num), msgBase >> 10);
      outpw(TA_ADDR(num), (msgBase << 6) + 58);
      msgSW = inpw(TA_DATA(num));
      pevd[2] = UNDEFINED_MODE; //mode; //pevd->4 = mode; //[esi+4] //; wMode
      pevd[4] = 0;
      pevd[5] = 0; //pevd->10 = 0; //[esi+10]  //; bc.wAW2, others.dummy
      nint = 255;
      intd = vp;
      pevd[4] = msgSW;
    }
    else
    {
      nint = 0;
      intd = 0;
      res = 0; // no TMK_INT_SAVED
    }
    res |= (vp & 0xC000);
    if (vp & 0x2000) // MRTA FIFO Overflow
      res |= 0x0400; // TMK_INT_FIFO_OVF
  }
#endif //def TSTA
  else
  {
    pevd[2] = mode; //pevd->4 = mode; //[esi+4] //; wMode
    pevd[5] = 0; //pevd->10 = 0; //[esi+10]  //; bc.wAW2, others.dummy
    switch (mode)
    {
    case MT_MODE:
//      switch (type)
//      {
//      case __TMKX:
        intd = inpw(TMK_Mode(num));
        if (intd & 0x0030)
        {
          __bcControls[num] |= GENER1_BLK | GENER2_BLK;
          outpw(TMK_Mode(num), __bcControls[num]);
        }
        if ((intd & 0x8000) == 0)
        {
          intd >>= 6;
          intd &= __bcMaxBase[num];  //; mt.wBase
          nint = 4;                //; nInt = 4
        }
        else
        {
          __tmkStarted[num] = 0;
          intd >>= 6;
          intd &= __bcMaxBase[num];  //;
  //;        __bcBaseBus[num] = intd //;
          pevd[4] = intd; //pevd->8 = intd; //[esi+8]//; mt.wBase
          intd = inpw(TMK_State(num));       //; mt.wResultX
          nint = 3;                //; nInt = 3
        }
//        break;
//      }
      break;
    case BC_MODE:
      switch (type)
      {
      case __TMK400:
        t = __wInDelay[num];
        while (--t != 0);
        __tmkStarted[num] = 0;
        intm = inpb_d(num, T400_ATR(num));
        intd = inpw_d(num, TMK_State(num));
        intd >>= 10;              //; bc.wResult
        nint = 1;                 //; nInt = 1
        if (intm & TMK400_INT1_MASK)
        {
          ++nint;                 //; nInt = 2
          pevd[5] = 1; //pevd->10 = 1; //[esi+10]//; bc.wAW2
        }
        break;
      case __RTMK400:
        t = __wInDelay[num];
        while (--t != 0);
        __tmkStarted[num] = 0;
        intm = inpb_d(num, R400_AT(num));
        intd = inpw_d(num, TMK_State(num));
        intd >>= 10;              //; bc.wResult
        nint = 1;                 //; nInt = 1
        if (intm & RTMK400_INT1_MASK)
        {
          ++nint;                 //; nInt = 2
          pevd[5] = 1; //pevd->10 = 1; //[esi+10]//; bc.wAW2
        }
        break;
      case __TMKMPC:
        t = __wInDelay[num];
        while (--t != 0);
        __tmkStarted[num] = 0;
        intd = inpb_d(num, TMKMPC_StateL(num));
        intd &= 0x003F;           //; bc.wResult
        nint = 1;                 //; nInt = 1
        if (intd)
        {
          ++nint;                 //; nInt = 2
          pevd[5] = 1; //pevd->10 = 1; //[esi+10]//; bc.wAW2
        }
        break;
      case __TMKX:
        intd = inpw(TMK_Mode(num));
        if (intd & 0x0030)
        {
          __bcControls[num] |= GENER1_BLK | GENER2_BLK;
          outpw(TMK_Mode(num), __bcControls[num]);
        }
        if ((intd & 0x8000) == 0)
        {
          intd >>= 6;
          intd &= __bcMaxBase[num];    //; bcx.wBase
          nint = 4;                //; nInt = 4
        }
        else
        {
          __tmkStarted[num] = 0;
          intd = inpw(TMK_State(num));
          if (__bcXAStart[num] & 1)
          {
            pevd[4] = (intd >> 6) & __bcMaxBase[num]; //pevd->8 = (intd >> 6) & __bcMaxBase[num]; //[esi+8] //; bcx.wBase
  //;          __bcBaseBus[num] = intd;
            intd &= 0x003F;            //; bcx.wResultX
            nint = 3;                //; nInt = 3
          }
          else
          {
            intd = __bcExt2StdResult[intd & 0xF] | ((intd << 10) & 0xC000);
                                     //; bc.wResult
            nint = 1;                //; nInt = 1
            if (intd)
            {
              ++nint;                   //; nInt = 2
              pevd[5] = 1; //pevd->10 = 1; //[esi+10] //; bc.wAW2
            }
          }
        }
        break;
      }
      break;
    case RT_MODE:
      switch (type)
      {
      case __TMK400:
        t = __wInDelay[num];
        while (--t != 0);
        intm = inpb_d(num, T400_ATR(num));
        intd = inpw_d(num, TMK_State(num)); //; TMK_Ctrl(num);
        outpw_d(num, TMK_Ctrl(num), __rtAddress[num] | RT_CLEAR_INT);
        outpw_d(num, TMK_Ctrl(num), __rtAddress[num] | __rtMode[num]);//; don't mask because irq
        nint = 2;                //; nInt = 2
        if (intm & TMK400_INT1_MASK)
          intd &= 0x57FF;            //; rt.wStatus
        else
        {
          --nint;                   //; nInt = 1
          intd &= 0x041F;            //; rt.wCmd
        }
        break;
      case __RTMK400:
        t = __wInDelay[num];
        while (--t != 0);
        intm = inpb_d(num, R400_AT(num));
        intd = inpw_d(num, TMK_State(num)); //; TMK_Ctrl(num);
        outpw_d(num, TMK_Ctrl(num), RT_CLEAR_INT);
        outpw_d(num, TMK_Ctrl(num), __rtMode[num]);
        nint = 2;                //; nInt = 2
        if (intm & RTMK400_INT1_MASK)
          intd &= 0x57FF;            //; rt.wStatus
        else
        {
          --nint;                   //; nInt = 1
          intd &= 0x041F;            //; rt.wCmd
        }
        break;
      case __TMKMPC:
        t = __wInDelay[num];
        while (--t != 0);
        intd = inpw_d(num, TMKMPC_StateL(num)); //; TMKMPC_CtrlL();
        outpb(TMKMPC_CtrlH(num), RT_CLEAR_INT >> 8);
        outpb_d(num, TMKMPC_CtrlL(num), RT_CLEAR_INT);
        mode = __rtMode[num];
        outpb(TMKMPC_CtrlH(num), mode >> 8);
        outpb_d(num, TMKMPC_CtrlL(num), mode);
        nint = 2;                //; nInt = 2
        if (intd & TMKMPC_INT1_MASK)
          intd &= 0x57FF;            //; rt.wStatus
        else
        {
          --nint;                   //; nInt = 1
          intd &= 0x041F;            //; rt.wCmd
        }
        break;
      case __TMKX:
        limit = 16; // abstract loop limit for broken hardware
        intd = inpw(TMK_State(num)); //; TMK_Ctrl(num);
        do
        {
          intm = intd;
          intd = inpw(TMK_State(num));
        }
        while (intm != intd && --limit != 0);
        if (intd & 0x3000)
        {
          __rtControls[num] |= GENER1_BLK | GENER2_BLK;
          outpw(TMK_Mode(num), __rtControls[num]);
        }
        t = (__rtControls1[num] & __rtBRCMask[num]) | __rtDisableMask[num];
        outpw(TMK_Ctrl(num), t | (RT_CLEAR_INT >> 5));
        outpw(TMK_Ctrl(num), t | (RT_CLEAR_INT >> 5));
        outpw(TMK_Ctrl(num), t);
        outpw(TMK_Ctrl(num), t); // PCI2
        nint = 2;               //; nInt = 2
        if (intd & 0x7000)
          intd &= 0x77FF;           //; rt.wStatus
        else if ((intd & 0x03E0) == 0 || (intd & 0x03E0) == 0x03E0)
        {
          --nint;                   //; nInt = 1
          intd &= 0x041F;            //; rt.wCmd
        }
        else
        {
          ++nint;                   //; nInt = 3
          intd &= 0x07FF;            //; rt.wCmd
        }
        break;
      }
      break;
#if NRT > 0
    case MRT_MODE:
  //;    switch (type)
  //;    {
#ifdef MRTX
  //;    case __MRTX:
        limit = 16; // abstract loop limit for broken hardware
        intd = inpw(TMK_State(num)); //; MRT_Mode(num);
        do
        {
          intm = intd;
          intd = inpw(TMK_State(num));
        }
        while (intm != intd && --limit != 0);
        outpw(TMK_Ctrl(num), __rtControls1[num] | (RT_CLEAR_INT >> 5));
        outpw(TMK_Ctrl(num), __rtControls1[num] | (RT_CLEAR_INT >> 5));
        outpw(TMK_Ctrl(num), __rtControls1[num]);//; don't mask because irq
        outpw(TMK_Ctrl(num), __rtControls1[num]); // PCI2
        if ((intd & 0x8000) == 0)       //; !brc ?
        {
          pevd[4] = ((intd >> 12) & 0x3) + __mrtMinRT[num]; //pevd->8 = (intd >> 12) + __mrtMinRT[num];//[esi+8] //; rt number
          nint = 2;                //; nInt = 2
          if ((intd & 0x4000) == 0)
          {
            if ((intd & 0x03E0) == 0 || (intd & 0x03E0) == 0x03E0)
            {
              --nint;                   //; nInt = 1
              intd &= 0x041F;            //; rt.wCmd
            }
            else
            {
              ++nint;                   //; nInt = 3
              intd &= 0x07FF;            //; rt.wCmd
            }
          }
          else
            intd &= 0x47FF;
        }
        else
        {
          intd = (intd & 0x47FF) | 0x1000; // make unified MRT intd
          pevd[4] = __mrtMinRT[num]; //pevd->8 = __mrtMinRT[num];//[esi+8]      //; rt number
          pevd[5] = __mrtNRT[num]; //pevd->10 = __mrtNRT[num];//[esi+10]
          intm = intd;
  //;                  push    __tmkNumber
          PUSH_RealNum;
  //;                  push    tmkRealNumber
          PUT_RealNum(num);
  //;                  mov     tmkRealNumber, num
          irt = __mrtMinRT[num];
          nrt = __mrtNRT[num];
          while (nrt > 1) // skip first
          {
            if ((__rtDisableMask[++irt] & 0xFFFF) == 0)
            {
  //;                mov     __tmkNumber, irt
              DrvRtPokeIrqMRTX(irt, 0x07F2, intm | 0xF800); //; last cmd register
            }
            --nrt;
          }
          if ((intm & 0x4000) == 0)  //; !error ?
          {
            if ((intm & 0x03E0) == 0 || (intm & 0x03E0) == 0x03E0) // cmd ?
            {
              intm &= 0x041F;
              if (intm == 0x11 || intm == 0x14 || intm == 0x15)
              {
                irt = __mrtMinRT[num];
                nrt = __mrtNRT[num];
                pos = intm | 0x07E0;
  //;                mov     __tmkNumber2, irt
                data = DrvRtPeekIrqMRTX(irt, pos);
                while (nrt > 1) // skip first
                {
                  if ((__rtDisableMask[++irt] & 0xFFFF) == 0)
                  {
  //;                mov     __tmkNumber2, irt
                    DrvRtPokeIrqMRTX(irt, pos, data);
                  }
                  --nrt;
                }
              }
            }
          }
          POP_RealNum;
      //;                pop     tmkRealNumber2
      //;                pop     __tmkNumber2
          nint = 4;                //; nInt = 4
        }
  //;      break;
#endif
  //;    }
      break;
#endif
#ifdef USE_TMKRAWIO
    case UNDEFINED_MODE:
      nint = 255;
      intd = 0;
      pevd[4] = 0;
      break;
#endif
    default:
  //    case BC_MRTX:
  //    case RT_MRTX:
  //;    case MT_TMK400:
  //;    case MT_RTMK400:
  //;    case MT_TMKMPC:
  //;    case MT_MRTX:
  //;    case MR_TMK400:
  //;    case MR_RTMK400:
  //;    case MR_TMKMPC:
  //;    case MR_TMKX:
      nint = 0;
      intd = 0;
      res = 0; // no TMK_INT_SAVED
      switch (type)
      {
      case __TMKMPC:
        outpb(TMKMPC_Reset(num), 0);
        break;
      default:
        outpw(TMK_Reset(num), 0);
        break;
      }
      break;
    }
  }
#ifdef USE_TMKRAWIO
  if (mode == UNDEFINED_MODE)
  {
    data = 0; // suppress untitialized data warning
    for (pos = 0; pos < __tmkRawIrqIoLen[num]; ++pos)
    {
      sa = __tmkRawIrqIoVal[num][pos];
      if (__tmkRawIrqIoOp[num][pos] <= 5) // port address checked in tmkrawion
        sa += (sa <= 0xFF) ? localport : __tmkPortsAddr2[num] - 0x0100;
      switch (__tmkRawIrqIoOp[num][pos])
      {
      case 0:
//        t = __wInDelay[num];
//        while (--t != 0);
        data = inpw(sa);
        break;
      case 1:
//        t = __wInDelay[num];
//        while (--t != 0);
        data = (unsigned)inpb(sa);
        break;
      case 2:
        limit = 16; // abstract loop limit for broken hardware
        do
        {
//          t = __wInDelay[num];
//          while (--t != 0);
          vp = data;
          data = inpw(sa);
        }
        while (data != vp && --limit != 0);
        break;
      case 3:
        limit = 16; // abstract loop limit for broken hardware
        do
        {
//          t = __wInDelay[num];
//          while (--t != 0);
          vp = data;
          data = (unsigned)inpb(sa);
        }
        while (data != vp && --limit != 0);
        break;
      case 4:
//        t = __wInDelay[num];
//        while (--t != 0);
        outpw(sa, data);
        break;
      case 5:
//        t = __wInDelay[num];
//        while (--t != 0);
        outpb(sa, (U08)data);
        break;
      case 6:
        data &= sa;
        break;
      case 7:
        data |= sa;
        break;
      case 8:
        data = sa;
        break;
      case 9:
        data <<= sa;
        break;
      case 10:
        data >>= sa;
        break;
      case 11:
        t = sa;
        while (t-- != 0);
        break;
      case 12:
        intd = data;
        break;
      case 13:
        pevd[4] = data;
        break;
      case 14:
        data = intd;
        break;
      case 15:
        data = pevd[4];
        break;
// change tmkrawion w irqio limit if add new commands!!!
      }
    }
  }
#endif //def USE_TMKRAWIO
  pevd[0] = nint; //pevd->0 = nint; //[esi]  //; nInt
  pevd[1] = 0;
  pevd[3] = intd; //pevd->6 = intd; //[esi+6]//; look at tmkgetevd OutBuf[3];
  return res;
}

void DpcIExcBC(int hTMK, void *pEvData)
{
  int register num;
  unsigned type;
  unsigned pos;
  unsigned base;
  unsigned basepc;
  U16 FARDT *pevd;
  unsigned bcAW1, bcAW2;

  num = hTMK;
  if (__tmkMode[num] != BC_MODE)
    return;
  pevd = (U16 FARDT*)pEvData;
  basepc = __bcBasePC[num];
  base = __bcBaseBus[num];
  type = __tmkDrvType[num];
  DrvBcDefBase(num, type, base);
  bcAW1 = 0xFFFF;
  bcAW2 = 0xFFFF;
  pos = (unsigned)__bcAW1Pos[num];
  if (pos == 0)
    pos = (unsigned)__bcAW2Pos[num];
  if (pos != 0)
  {
    bcAW1 = DrvBcPeek(num, type, pos);
    if (pos != (unsigned)__bcAW2Pos[num])
    {
      pos = (unsigned)__bcAW2Pos[num];
      if (pos != 0)
        bcAW2 = DrvBcPeek(num, type, pos);
    }
  }
//  pos = (unsigned)__bcAW1Pos[num];
//  if (pos != 0)
//    bcAW1[num] = DrvBcPeek(num, type, pos);
//  pos = (unsigned)__bcAW2Pos[num];
//  if (pos != 0)
//  {
//    if (__bcAW1Pos[num] == 0)
//      bcAW1[num] = DrvBcPeek(num, type, pos);
//    else
//      bcAW2[num] = DrvBcPeek(num, type, pos);
//  }
  if (basepc != base)
    DrvBcDefBase(num, type, basepc);
  pevd[4] = bcAW1; //pevd->8 = bcAW1[num];
  pevd[5] = bcAW2; //pevd->10 = bcAW2[num];
  return;
}

#endif //ndef DrvIO

#if NRT > 0
int FARFN tmkbuserr(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 tmkBusErr)
{
  int register num;
  int realnum;
  unsigned type;
#ifdef MRTA
  LOCAL_PORT
#endif
  unsigned sa;
  unsigned base, link;
  int limit;

  DrvIOProV;
  CLRtmkError;
  num = __tmkNumber;
  realnum = GET_RealNum;
  type = __tmkDrvType[num];
  CHECK_TMK_TYPE(type);
  switch (type)
  {
#ifdef MRTA
  case __MRTA:
    if (!__tmkBusErr[realnum])
    {
      DrvIOStop;
      return USER_ERROR(TMK_BAD_FUNC);
    }
    sa = __rtSubAddr[num];
    sa = (sa >> 5) & 0x3F;
    SET_LOCAL_PORT(realnum);
    GET_DIS_IRQ_SMP();
    if (!(tmkBusErr & BUS_ERR_ONCE))
    {
      outpw(MRTA_ADDR2(realnum), 0);
      limit = 4096; // abstract loop limit for broken hardware
      link = DrvRtPeekTA(realnum, __rtAddress[num], sa) & 0x0FFF;
      do
      {
        base = link;
        outpw(MRTA_ADDR2(realnum), base >> 10);
        link = DrvRtPeekTA(realnum, base, 63);
        if ((link & BUS_ERR_SET) == (tmkBusErr & BUS_ERR_SET))
          break;
        DrvRtPokeTA(realnum, base, 63, (link & 0xDFFF) | (tmkBusErr & BUS_ERR_SET));
        link &= 0x0FFF;
      }
      while (link != base && --limit != 0);
    }
    outpw(MRTA_ADDR2(realnum), 0);
    DrvPeekAndOrPokeTA(realnum, __rtAddress[num], sa, 0xDFFF, tmkBusErr & BUS_ERR_SET);
    REST_IRQ_SMP();
    break;
#endif
  default:
    DrvIOStop;
    return USER_ERROR(TMK_BAD_FUNC);
  }
  DrvIOEpi;
  return 0;
}
#endif

#ifdef NMBCID

U16 FARFN mbcalloc()
{
  unsigned i;
  for (i = 0; i < NMBCID; ++i)
  {
    if (__mbcAlloc[i] == 0)
    {
      __mbcAlloc[i] = 1;
      __mbci[i] = 0;
      return (U16)i;
    }
  }
  return MBC_ALLOC_FAIL;
}

int FARFN mbcfree(U16 mbcId)
{
  if (mbcId >= NMBCID || __mbcAlloc[mbcId] == 0)
    return USER_ERROR_R(TMK_BAD_NUMBER);
  __mbcAlloc[mbcId] = 0;
  __mbci[mbcId] = 0;
  return 0;
}

int FARFN mbcinit(U16 mbcId)
{
  if (mbcId >= NMBCID || __mbcAlloc[mbcId] == 0)
    return USER_ERROR_R(TMK_BAD_NUMBER);
  __mbci[mbcId] = 0;
  return 0;
}

int FARFN mbcpreparex(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 mbcId, U16 bcBase, U16 bcCtrlCode, U16 mbcDelay)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned code;
  unsigned base;
  int i;
  IRQ_FLAGS;

  DrvIONone; // because mbcstartx anyway is DrvIONone
  if (mbcId >= NMBCID || __mbcAlloc[mbcId] == 0)
    return USER_ERROR_R(TMK_BAD_NUMBER);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE(realnum, BC_MODE);
  code = bcCtrlCode;
  CHECK_BC_CTRLX(code);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  base = bcBase;
  CHECK_BCMT_BASE_BX(realnum, base);
//  base += __rtHeapIni[realnum]; //mbcpreparex not used in mt
  for (i = 0; i < __mbci[mbcId]; ++i)
  {
    if (__mbcTmkN[mbcId][i] == realnum)
      break;
  }
  __mbcTmkN[mbcId][i] = realnum;
  __mbcBase[mbcId][i] = base;
  switch (type)
  {
  default:
    return USER_ERROR(BC_BAD_FUNC);
  case __TMKX:
    base <<= 6;
    __mbcPort0[mbcId][i] = 0;
    __mbcData0[mbcId][i] = 0;
    __mbcPort[mbcId][i] = TMK_Ctrl(realnum);
    __mbcData[mbcId][i] = base | code;
    if (i == __mbci[mbcId])
      ++__mbci[mbcId];
    break;
  case __TA:
    {
      unsigned ContrW = __bcCtrlWA[realnum];
      unsigned code1;

//      if (((code&0xf) == DATA_RT_RT) || ((code&0xf) == DATA_RT_RT_BRCST))
      if ((code&0x7) == DATA_RT_RT)
        ContrW |= 0x0040;
      if (code & CX_BUS_B)
        ContrW |= 0x0080;
      if (code & CX_CONT)
        ContrW |= 0x2000;
#if DRV_MAX_BASE < 256
      code1 = __bcLinkWN[realnum][base];
#else
      code1 = __bcLinkCCN[realnum][base];
#endif
      if (code1 & CX_SIG)
        ContrW |= 0x8000;

      GET_DIS_IRQ_SMP();
      outpw(TA_ADDR(realnum), (base<<6) | 61);
      outpw(TA_DATA(realnum), ContrW);
      outpw(TA_DATA(realnum), mbcDelay);
      REST_IRQ_SMP();
#ifdef DRV_EMULATE_FIRST_CX_SIG
//emulation is through special base 0x3ff
//also it could be a good (or bad) idea to block irq output
//and further poll it until it occurs
      if (code & CX_SIG)
      {
        GET_DIS_IRQ_SMP();
        outpw(TA_ADDR(realnum), (0x03FF<<6) | 61);
        outpw(TA_DATA(realnum), 0xA020);
        outpw(TA_DATA(realnum), 0);
        outpw(TA_DATA(realnum), base);
        REST_IRQ_SMP();
        base = 0x03FF;
      }
#endif //def DRV_EMULATE_FIRST_CX_SIG
      __mbcPort0[mbcId][i] = TA_MSGA(realnum);
      __mbcData0[mbcId][i] = base & 0x03FF;
      __mbcPort[mbcId][i] = TA_MODE2(realnum);
      __mbcData[mbcId][i] = __bcControls1[realnum] | TA_BC_START;
      if (i == __mbci[mbcId])
        ++__mbci[mbcId];
    }
    break;
  }
  return 0;
}

int FARFN mbcpreparea(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        U16 mbcId, U16 bcBase)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned base;
  int i;

  if (mbcId >= NMBCID || __mbcAlloc[mbcId] == 0)
    return USER_ERROR_R(TMK_BAD_NUMBER);
  CLRtmkError;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  CHECK_TMK_MODE(realnum, BC_MODE);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  base = bcBase;
  CHECK_BCMT_BASE_BX(realnum, base);
//  base += __rtHeapIni[realnum]; //mbcpreparea not used in mt
  for (i = 0; i < __mbci[mbcId]; ++i)
  {
    if (__mbcTmkN[mbcId][i] == realnum)
      break;
  }
  __mbcTmkN[mbcId][i] = realnum;
  __mbcBase[mbcId][i] = base | 0x8000; // base for starta
  switch (type)
  {
  default:
    return USER_ERROR(BC_BAD_FUNC);
  case __TA:
    __mbcPort0[mbcId][i] = TA_MSGA(realnum);
    __mbcData0[mbcId][i] = base & 0x03FF;
    __mbcPort[mbcId][i] = TA_MODE2(realnum);
    __mbcData[mbcId][i] = __bcControls1[realnum] | TA_BC_START;
    if (i == __mbci[mbcId])
      ++__mbci[mbcId];
    break;
  }
  return 0;
}

int FARFN mbcstartx(U16 mbcId)
{
  int register realnum;
  unsigned type;
  LOCAL_PORT
  unsigned port;
  register int i, ni;
#ifndef STATIC_TMKNUM
  int __tmkNumber;
#endif
  IRQ_FLAGS;

  DrvIONone; //because of variable __tmkNumber and outports to multiple tmks
  if (mbcId >= NMBCID || __mbcAlloc[mbcId] == 0)
    return USER_ERROR_R(TMK_BAD_NUMBER);
  ni = (int)__mbci[mbcId];
  for (i = 0; i < ni; ++i)
  {
    realnum = __mbcTmkN[mbcId][i];
    SET_LOCAL_PORT(realnum);
    __tmkNumber = realnum;
    CLRtmkError;
    type = __tmkDrvType[realnum];
//    CHECK_TMK_TYPE(type);
    __bcBaseBus[realnum] = __mbcBase[mbcId][i] & 0x7FFF;
//    DrvIOSetPar(IOPAR_BCBASEBUS, __bcBaseBus[realnum]);
    __bcXAStart[realnum] = (__mbcBase[mbcId][i] & 0x8000) ? ((__tmkOptions[realnum] & (UOPT)BC_STARTA_INTA) ? 2 : 1) : 1;
//    DrvIOSetPar(IOPAR_BCXASTART, __bcXAStart[realnum]);
    __bcmtStateX[realnum] = 0;
//    DrvIOSetPar(IOPAR_BCMTSTATEX, 0);
    switch (type)
    {
    default:
      return USER_ERROR(BC_BAD_FUNC);
    case __TMKX:
      if (__tmkStarted[realnum])
      {
        GET_DIS_IRQ_SMP();
        outpw(TMK_Reset(realnum), 0);
        outpw(TMK_Mode(realnum), __bcControls[realnum]);
        REST_IRQ_SMP();
      }
      break;
    case __TA:
      if (__tmkStarted[realnum])
      {
        GET_DIS_IRQ_SMP();
        outpw(TA_RESET(realnum), 0);
        outpw(TA_TIMCR(realnum), __tmkTimerCtrl[realnum]);
        outpw(TA_MODE1(realnum), __bcControls[realnum]);
        REST_IRQ_SMP();
        outpw(TA_MODE2(realnum), __bcControls1[realnum]);
      }
      break;
    }
    __tmkStarted[realnum] = 1;
  }
  for (i = 0; i < ni; ++i)
  {
    if ((port = __mbcPort0[mbcId][i]) != 0)
      outpw(port, __mbcData0[mbcId][i]);
  }
  GET_DIS_IRQ();
  for (i = 0; i < ni; ++i)
  {
    outpw(__mbcPort[mbcId][i], __mbcData[mbcId][i]);
  }
  REST_IRQ();
  return 0;
}

#endif //def NBCID

#ifdef USE_TMKRAWIO
#ifndef NULL
#define NULL ((void HUGEDT *)0)
#endif
int FARFN tmkrawion(
#ifndef STATIC_TMKNUM
        int __tmkNumber,
#endif
        void HUGEDT *pcBufOut, U32 dwLenOut, void HUGEDT *pcBufIn, U32 dwLenIn)
{
  int register realnum;
  unsigned type;
  TRAWIO HUGEDT *bufo;
  TRAWIO HUGEDT *bufi;
  TRAWIO HUGEDT *buf;
  U32 oo, ii;
  LOCAL_PORT
  unsigned port;
  unsigned cmd;
  unsigned mask1, mask2, mask2and, mask2or;
  unsigned data, datax;
  int limit;
  unsigned pos;
#if defined(MRTA) || defined(TSTA)
  unsigned posh=0;
#endif
  unsigned useh;
  unsigned len;
  unsigned save_rama, save_ramiw;
  unsigned param;
  unsigned base;
  IRQ_FLAGS_RAW;

//  DrvIOProV;
  CLRtmkError;
  if ((dwLenOut != 0L && pcBufOut == NULL) || (dwLenIn != 0L && pcBufIn == NULL))
    return 1;
  realnum = GET_RealNum;
  SET_LOCAL_PORT(realnum);
  type = __tmkDrvType[realnum];
  CHECK_TMK_TYPE(type);
  bufo = (TRAWIO HUGEDT*)pcBufOut;
  bufi = (TRAWIO HUGEDT*)pcBufIn;
  ii = 0L;
// at the end of every iteration:
// ii should point to next available bufi cell
// oo should point to last cell of last bufo cmd, because of for (; ; ++oo)
  for (oo = 0L; oo < dwLenOut; ++oo)
  {
    cmd = (unsigned)bufo[oo];
    if (cmd == 0xFFFF)
      break;
    useh = 0;
    switch (cmd)
    {

    case 0: // w reg
      if ((oo + 2L) >= dwLenOut)
        goto return_4;
      port = (unsigned)bufo[++oo];
      if (port > 0x1FF)
        goto return_3;
      if (port <= 0xFF)
      {
        port += localport;
        if (port > __tmkPortsMax1[realnum])
          goto return_3;
      }
      else
      {
        if (__tmkPortsAddr2[realnum] == 0xFFFF)
          goto return_3;
        port += __tmkPortsAddr2[realnum] - 0x0100;
        if (port > __tmkPortsMax2[realnum])
          goto return_3;
      }
      GET_DIS_IRQ_SMP_RAW();
      outpw(port, (unsigned)bufo[++oo]);
      REST_IRQ_SMP_RAW();
      break;

    case 1: // r reg
      if ((oo + 1L) >= dwLenOut || (ii + 2L) >= dwLenIn)
        goto return_4;
      port = (unsigned)bufo[++oo];
      if (port > 0x1FF)
        goto return_3;
      bufi[ii] = (TRAWIO)cmd;
      bufi[++ii] = (TRAWIO)port;
      if (port <= 0xFF)
      {
        port += localport;
        if (port > __tmkPortsMax1[realnum])
          goto return_3;
      }
      else
      {
        if (__tmkPortsAddr2[realnum] == 0xFFFF)
          goto return_3;
        port += __tmkPortsAddr2[realnum] - 0x0100;
        if (port > __tmkPortsMax2[realnum])
          goto return_3;
      }
      GET_DIS_IRQ_SMP_RAW();
      bufi[++ii] = (TRAWIO)inpw(port);
      REST_IRQ_SMP_RAW();
      ++ii;
      break;

    case 12: // w bigram
      useh = 1;
      fallthrough;
      // fall-through
    case 2: // w ram
      if ((oo + (U32)useh + 1L) >= dwLenOut || (oo + (U32)useh + 2L + (U32)bufo[oo+1L] >= dwLenOut))
        goto return_4;
      len = (unsigned)bufo[++oo];
      if (useh)
      {
#if defined(MRTA) || defined(TSTA)
        posh = (unsigned)bufo[++oo];
#else
        ++oo;
#endif
      }
      pos = (unsigned)bufo[++oo];
      buf = (TRAWIO HUGEDT*)&bufo[++oo];
      if (!len)
        break;
      oo += len - 1;

      save_rama = __tmkRAMAddr[realnum];
      save_ramiw = __tmkRAMInWork[realnum];
      __tmkRAMInWork[realnum] = 1;
      switch (type)
      {
      case __TMK400:
      case __RTMK400:
        outpw_d(realnum, TMK_Addr(realnum), pos>>6);
        pos &= 0x1F;
        __tmkRAMAddr[realnum] = pos;
        outpb_d(realnum, TMK_Addr(realnum), pos);
//        REP_OUTSW_D(TMK_Data(realnum)); // U32 buf can be here !!!
        do
        {
          int t;
          GET_DIS_IRQ_RAW();
          outpw(TMK_Data(realnum), *(buf++));
          /*outsw*/
          ++__tmkRAMAddr[realnum];
          REST_IRQ_RAW();
          t = __wOutDelay[realnum];
          while (--t);
        }
        while (--len != 0);
        if (!save_ramiw)
          break;
        __tmkRAMAddr[realnum] = save_rama;
        outpb_d(realnum, TMK_Addr(realnum), save_rama);
        break;
      case __TMKMPC:
        outpb_d(realnum, TMKMPC_AddrH(realnum), pos>>6);
        pos &= 0x1F;
        __tmkRAMAddr[realnum] = pos;
        outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
//        REP_OUTSWB_D(TMKMPC_DataL(realnum)); // U32 buf can be here !!!
        do
        {
          unsigned data;
          data = *(buf++);
          /*lodsw*/
          GET_DIS_IRQ_RAW();
          outpb((TMKMPC_DataL(realnum))+1, data >> 8);
          outpb_d(realnum, TMKMPC_DataL(realnum), data);
          ++__tmkRAMAddr[realnum];
          REST_IRQ_RAW();
        }
        while (--len != 0);
        if (!save_ramiw)
          break;
        __tmkRAMAddr[realnum] = save_rama;
        outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
        break;
#if defined(MRTA) || defined(TSTA)
#ifdef MRTA
      case __MRTA:
#endif
#ifdef TSTA
      case __TSTA:
#endif
        if (useh)
        {
          GET_DIS_IRQ_SMP_RAW();
          outpw(MRTA_ADDR2(realnum), posh);
          REST_IRQ_SMP_RAW();
        }
        // fall-through
#endif
        fallthrough;
        // fall-through
      case __TMKX:
#ifdef MRTX
      case __MRTX:
#endif
#ifdef TSTX
      case __TSTX:
#endif
      case __TA:
        GET_DIS_IRQ_SMP_RAW();
        __tmkRAMAddr[realnum] = pos;
        outpw(TMK_Addr(realnum), pos);
        REST_IRQ_SMP_RAW();
//        REP_OUTSW(TMK_Data(realnum)); // U32 buf can be here !!!
        do
        {
          GET_DIS_IRQ_RAW();
          outpw(TMK_Data(realnum), *(buf++));
          /*outsw*/
          ++__tmkRAMAddr[realnum];
          REST_IRQ_RAW();
        }
        while (--len != 0);
        if (!save_ramiw)
          break;
        GET_DIS_IRQ_SMP_RAW();
        __tmkRAMAddr[realnum] = save_rama;
        outpw(TMK_Addr(realnum), save_rama);
        REST_IRQ_SMP_RAW();
        break;
      }
      if (!save_ramiw)
        __tmkRAMInWork[realnum] = 0;
      break;

    case 13: // r bigram
      useh = 1;
      fallthrough;
      // fall-through
    case 3: // r ram
      if ((oo + (U32)useh + 2L >= dwLenOut) || (ii + (U32)useh + 2L + (U32)bufo[oo+1L] >= dwLenIn))
        goto return_4;
      len = (unsigned)bufo[++oo];
      if (useh)
      {
#if defined(MRTA) || defined(TSTA)
        posh = (unsigned)bufo[++oo];
#else
        ++oo;
#endif
      }
      pos = (unsigned)bufo[++oo];
      bufi[ii] = (TRAWIO)cmd;
      bufi[++ii] = (TRAWIO)len;
      if (useh)
      {
#if defined(MRTA) || defined(TSTA)
        bufi[++ii] = (TRAWIO)posh;
#else
        bufi[++ii] = (TRAWIO)0;
#endif
      }
      bufi[++ii] = (TRAWIO)pos;
      buf = (TRAWIO HUGEDT*)&bufi[++ii];
      if (!len)
        break;
      ii += len;
      save_rama = __tmkRAMAddr[realnum];
      save_ramiw = __tmkRAMInWork[realnum];
      __tmkRAMInWork[realnum] = 1;
      switch (type)
      {
      case __TMK400:
      case __RTMK400:
        outpw_d(realnum, TMK_Addr(realnum), pos>>6);
        pos &= 0x1F;
        __tmkRAMAddr[realnum] = pos;
        outpb_d(realnum, TMK_Addr(realnum), pos);
//        REP_INSW_D(TMK_Data(realnum)); // U32 buf can be here !!!
        do
        {
          int t;
          GET_DIS_IRQ_RAW();
          *(buf++) = inpw(TMK_Data(realnum));
          /*insw*/
          ++__tmkRAMAddr[realnum];
          REST_IRQ_RAW();
          t = __wInDelay[realnum];
          while (--t);
        }
        while (--len != 0);
        if (!save_ramiw)
          break;
        __tmkRAMAddr[realnum] = save_rama;
        outpb_d(realnum, TMK_Addr(realnum), save_rama);
        break;
      case __TMKMPC:
        outpb_d(realnum, TMKMPC_AddrH(realnum), pos>>6);
        pos &= 0x1F;
        __tmkRAMAddr[realnum] = pos;
        outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
//        REP_INSWB32_D(TMKMPC_DataL(realnum));
//        REP_INSW_D(TMKMPC_DataL(realnum)); // U32 buf can be here !!!
        do
        {
          int t;
          GET_DIS_IRQ_RAW();
          *(buf++) = inpw(TMKMPC_DataL(realnum));
          /*insw*/
          ++__tmkRAMAddr[realnum];
          REST_IRQ_RAW();
          t = __wInDelay[realnum];
          while (--t);
        }
        while (--len != 0);
        if (!save_ramiw)
          break;
        __tmkRAMAddr[realnum] = save_rama;
        outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
        break;
#if defined(MRTA) || defined(TSTA)
#ifdef MRTA
      case __MRTA:
#endif
#ifdef TSTA
      case __TSTA:
#endif
        if (useh)
        {
          GET_DIS_IRQ_SMP_RAW();
          outpw(MRTA_ADDR2(realnum), posh);
          REST_IRQ_SMP_RAW();
        }
        // fall-through
#endif
        fallthrough;
        // fall-through
      case __TMKX:
#ifdef MRTX
      case __MRTX:
#endif
#ifdef TSTX
      case __TSTX:
#endif
      case __TA:
        GET_DIS_IRQ_SMP_RAW();
        __tmkRAMAddr[realnum] = pos;
        outpw(TMK_Addr(realnum), pos);
        REST_IRQ_SMP_RAW();
//        REP_INSW(TMK_Data(realnum)); // U32 buf can be here !!!
        do
        {
          GET_DIS_IRQ_RAW();
//          inptrw(TMK_Data(realnum), (buf++));
          *(buf++) = inpw(TMK_Data(realnum));
          /*insw*/
          ++__tmkRAMAddr[realnum];
          REST_IRQ_RAW();
        }
        while (--len != 0);
        if (!save_ramiw)
          break;
        GET_DIS_IRQ_SMP_RAW();
        __tmkRAMAddr[realnum] = save_rama;
        outpw(TMK_Addr(realnum), save_rama);
        REST_IRQ_SMP_RAW();
        break;
      }
      if (!save_ramiw)
        __tmkRAMInWork[realnum] = 0;
//      ++ii; counted in ii += len
      break;

    case 4: // r&|w reg
      if ((oo + 3L) >= dwLenOut)
        goto return_4;
      port = (unsigned)bufo[++oo];
      if (port > 0x1FF)
        goto return_3;
      if (port <= 0xFF)
      {
        port += localport;
        if (port > __tmkPortsMax1[realnum])
          goto return_3;
      }
      else
      {
        if (__tmkPortsAddr2[realnum] == 0xFFFF)
          goto return_3;
        port += __tmkPortsAddr2[realnum] - 0x0100;
        if (port > __tmkPortsMax2[realnum])
          goto return_3;
      }
      mask1 = (unsigned)bufo[++oo];
      mask2 = (unsigned)bufo[++oo];
      GET_DIS_IRQ_SMP_RAW();
      outpw(port, (inpw(port) & mask1) | mask2);
      REST_IRQ_SMP_RAW();
      break;

    case 5: // r&&w reg
      if ((oo + 3L) >= dwLenOut)
        goto return_4;
      port = (unsigned)bufo[++oo];
      if (port > 0x1FF)
        goto return_3;
      if (port <= 0xFF)
      {
        port += localport;
        if (port > __tmkPortsMax1[realnum])
          goto return_3;
      }
      else
      {
        if (__tmkPortsAddr2[realnum] == 0xFFFF)
          goto return_3;
        port += __tmkPortsAddr2[realnum] - 0x0100;
        if (port > __tmkPortsMax2[realnum])
          goto return_3;
      }
      mask1 = (unsigned)bufo[++oo];
      mask2 = (unsigned)bufo[++oo];
      GET_DIS_IRQ_SMP_RAW();
      outpw(port, (inpw(port) & mask1) & mask2);
      REST_IRQ_SMP_RAW();
      break;

    case 14: // r&|w bigram
    case 15: // r&&w bigram
      useh = 1;
      fallthrough;
      // fall-through
    case 6: // r&|w ram
    case 7: // r&&w ram
      if ((oo + (U32)useh + 4L) >= dwLenOut)
        goto return_4;
      len = (unsigned)bufo[++oo];
      if (useh)
      {
#if defined(MRTA) || defined(TSTA)
        posh = (unsigned)bufo[++oo];
#else
        ++oo;
#endif
      }
      pos = (unsigned)bufo[++oo];
      mask1 = (unsigned)bufo[++oo];
      mask2 = (unsigned)bufo[++oo];
      if (!len)
        break;
      if (cmd & 1)
      {
//      case 7: // r&&w ram
//      case 15: // r&&w bigram
        mask2and = mask2;
        mask2or = 0;
      }
      else
      {
//      case 6: // r&|w ram
//      case 14: // r&|w bigram
        mask2and = 0xFFFF;
        mask2or = mask2;
      }
      save_rama = __tmkRAMAddr[realnum];
      save_ramiw = __tmkRAMInWork[realnum];
      __tmkRAMInWork[realnum] = 1;
      switch (type)
      {
      case __TMK400:
      case __RTMK400:
        outpw_d(realnum, TMK_Addr(realnum), pos>>6);
        pos &= 0x1F;
        __tmkRAMAddr[realnum] = pos;
        outpb_d(realnum, TMK_Addr(realnum), pos);
        do
        {
          int t;
          GET_DIS_IRQ_RAW();
          data = ((inpw(TMK_Data(realnum)) & mask1) & mask2and) | mask2or;
          t = __wInDelay[realnum];
          while (--t);
          outpb_d(realnum, TMK_Addr(realnum), pos);
          outpw(TMK_Data(realnum), data);
          ++__tmkRAMAddr[realnum];
          REST_IRQ_RAW();
          t = __wInDelay[realnum];
          while (--t);
          ++pos;
        }
        while (--len != 0);
        if (!save_ramiw)
          break;
        __tmkRAMAddr[realnum] = save_rama;
        outpb_d(realnum, TMK_Addr(realnum), save_rama);
        break;
      case __TMKMPC:
        outpb_d(realnum, TMKMPC_AddrH(realnum), pos>>6);
        pos &= 0x1F;
        __tmkRAMAddr[realnum] = pos;
        outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
        do
        {
          int t;
          GET_DIS_IRQ_RAW();
          data = ((inpw(TMK_Data(realnum)) & mask1) & mask2and) | mask2or;
          t = __wInDelay[realnum];
          while (--t);
          outpb_d(realnum, TMKMPC_AddrL(realnum), pos);
          outpb((TMKMPC_DataL(realnum))+1, data >> 8);
          outpb_d(realnum, TMKMPC_DataL(realnum), data);
          ++__tmkRAMAddr[realnum];
          REST_IRQ_RAW();
          ++pos;
        }
        while (--len != 0);
        if (!save_ramiw)
          break;
        __tmkRAMAddr[realnum] = save_rama;
        outpb_d(realnum, TMKMPC_AddrL(realnum), save_rama);
        break;
#if defined(MRTA) || defined(TSTA)
#ifdef MRTA
      case __MRTA:
#endif
#ifdef TSTA
      case __TSTA:
#endif
        if (useh)
        {
          GET_DIS_IRQ_SMP_RAW();
          outpw(MRTA_ADDR2(realnum), posh);
          REST_IRQ_SMP_RAW();
        }
        // fall-through
#endif
        fallthrough;
        // fall-through
      case __TMKX:
#ifdef MRTX
      case __MRTX:
#endif
#ifdef TSTX
      case __TSTX:
#endif
      case __TA:
        GET_DIS_IRQ_SMP_RAW();
        __tmkRAMAddr[realnum] = pos;
        outpw(TMK_Addr(realnum), pos);
        REST_IRQ_SMP_RAW();
        do
        {
          GET_DIS_IRQ_RAW();
          data = ((inpw(TMK_Data(realnum)) & mask1) & mask2and) | mask2or;
          outpw(TMK_Addr(realnum), pos);
          outpw(TMK_Data(realnum), data);
          ++__tmkRAMAddr[realnum];
          REST_IRQ_RAW();
          ++pos;
        }
        while (--len != 0);
        if (!save_ramiw)
          break;
        GET_DIS_IRQ_SMP_RAW();
        __tmkRAMAddr[realnum] = save_rama;
        outpw(TMK_Addr(realnum), save_rama);
        REST_IRQ_SMP_RAW();
        break;
      }
      if (!save_ramiw)
        __tmkRAMInWork[realnum] = 0;
      break;

    case 10: // w param
      if ((oo + 2L) >= dwLenOut)
        goto return_4;
      param = (unsigned)bufo[++oo];
      if (param >= 1 && param <= 9)
      {
        data = (unsigned)bufo[++oo];
        switch (param)
        {
        case 1: //IOPAR_TMKMODE 1
          __tmkMode[realnum] = data;
          DrvIOSetPar(IOPAR_TMKMODE, data);
          break;
        case 2: //IOPAR_BCBASEBUS 2
          __bcBaseBus[realnum] = data;
          DrvIOSetPar(IOPAR_BCBASEBUS, data);
          break;
//        case 3:
//          break;
        case 4: //IOPAR_BCXASTART 4
          __bcXAStart[realnum] = data;
          DrvIOSetPar(IOPAR_BCXASTART, data);
          break;
        case 5: //IOPAR_MTCW 5
          __mtCW[realnum] = data;
          DrvIOSetPar(IOPAR_MTCW, data);
          break;
        case 6: //IOPAR_BCMTSTATEX 6
          __bcmtStateX[realnum] = data;
          DrvIOSetPar(IOPAR_BCMTSTATEX, data);
          break;
        case 7: //IOPAR_TMKIRQOFF 7
          __tmkIrqOff[realnum] = (TMKIRQOFF_T)data;
          DrvIOSetPar(IOPAR_TMKIRQOFF, data);
          break;
        case 8: //IOPAR_BCAW1POS 8
          __bcAW1Pos[realnum] = data;
          DrvIOSetPar(IOPAR_BCAW1POS, data);
          break;
        case 9: //IOPAR_BCAW2POS 9
          __bcAW2Pos[realnum] = data;
          DrvIOSetPar(IOPAR_BCAW2POS, data);
          break;
        }
      }
      else if (param >= 10 && param <= 13)
      {
        len = (unsigned)bufo[++oo];
        if ((oo + 1L + len) >= dwLenOut) // and + 2L already incremented
          goto return_4;
        base = (unsigned)bufo[++oo];
        while (len--)
        {
          data = (unsigned)bufo[++oo];
          switch (param)
          {
#if DRV_MAX_BASE > 255
          case 10: //IOPARN_BCLINKBASEN 10
            __bcLinkBaseN[realnum][base] = data;
            DrvIOSetParN1(IOPARN_BCLINKBASEN, base, data);
            break;
          case 11: //IOPARN_BCLINKCCN 11
            __bcLinkCCN[realnum][base] = data;
            DrvIOSetParN1(IOPARN_BCLINKCCN, base, data);
            break;
#else
          case 12: //IOPARN_BCLINKWN 12
            __bcLinkWN[realnum][base] = data;
            DrvIOSetParN1(IOPARN_BCLINKWN, base, data);
            break;
#endif
//          case 13:
//            break;
          }
          ++base;
        }
      }
      else
        goto return_3;
      break;

    case 11: // r param
      if ((oo + 1L) >= dwLenOut)
        goto return_4;
      param = (unsigned)bufo[++oo];
      if (param >= 1 && param <= 9)
      {
        if ((ii + 2L) >= dwLenIn)
          goto return_4;
        bufi[ii] = (TRAWIO)cmd;
        bufi[++ii] = (TRAWIO)param;
        data = 0;
        switch (param)
        {
        case 1: //IOPAR_TMKMODE 1
          DrvIOGetPar(IOPAR_TMKMODE, __tmkMode[realnum]);
          data = __tmkMode[realnum];
          break;
        case 2: //IOPAR_BCBASEBUS 2
          DrvIOGetPar(IOPAR_BCBASEBUS, __bcBaseBus[realnum]);
          data = __bcBaseBus[realnum];
          break;
//        case 3:
//          break;
        case 4: //IOPAR_BCXASTART 4
          DrvIOGetPar(IOPAR_BCXASTART, __bcXAStart[realnum]);
          data = __bcXAStart[realnum];
          break;
        case 5: //IOPAR_MTCW 5
          DrvIOGetPar(IOPAR_MTCW, __mtCW[realnum]);
          data = __mtCW[realnum];
          break;
        case 6: //IOPAR_BCMTSTATEX 6
          DrvIOGetPar(IOPAR_BCMTSTATEX, __bcmtStateX[realnum]);
          data = __bcmtStateX[realnum];
          break;
        case 7: //IOPAR_TMKIRQOFF 7
          DrvIOGetPar(IOPAR_TMKIRQOFF, __tmkIrqOff[realnum]);
          data = (unsigned)__tmkIrqOff[realnum];
          break;
        case 8: //IOPAR_BCAW1POS 8
          DrvIOGetPar(IOPAR_BCAW1POS, __bcAW1Pos[realnum]);
          data = __bcAW1Pos[realnum];
          break;
        case 9: //IOPAR_BCAW2POS 9
          DrvIOGetPar(IOPAR_BCAW2POS, __bcAW2Pos[realnum]);
          data = __bcAW2Pos[realnum];
          break;
        }
        bufi[++ii] = (TRAWIO)data;
        ++ii;
      }
      else if (param >= 10 && param <= 13)
      {
        if ((oo + 3L) >= dwLenOut)
          goto return_4;
        len = (unsigned)bufo[++oo];
        if ((ii + 3L + len) >= dwLenIn)
          goto return_4;
        base = (unsigned)bufo[++oo];
        bufi[ii] = (TRAWIO)cmd;
        bufi[++ii] = (TRAWIO)len;
        bufi[++ii] = (TRAWIO)param;
        bufi[++ii] = (TRAWIO)base;
        while (len--)
        {
          data = 0;
          switch (param)
          {
#if DRV_MAX_BASE > 255
          case 10: //IOPARN_BCLINKBASEN 10
            DrvIOGetParN1(IOPARN_BCLINKBASEN, base, __bcLinkBaseN[realnum][base]);
            data = __bcLinkBaseN[realnum][base];
            break;
          case 11: //IOPARN_BCLINKCCN 11
            DrvIOGetParN1(IOPARN_BCLINKCCN, base, __bcLinkCCN[realnum][base]);
            data = __bcLinkCCN[realnum][base];
            break;
#else
          case 12: //IOPARN_BCLINKWN 12
            DrvIOGetParN1(IOPARN_BCLINKWN, base, __bcLinkWN[realnum][base]);
            data = __bcLinkWN[realnum][base];
            break;
#endif
//          case 13:
//            break;
          }
          bufi[++ii] = (TRAWIO)data;
          ++base;
        }
        ++ii;
      }
      break;

    case 16: // r maxcmd
      if ((oo) >= dwLenOut) // never
        goto return_4;
      if ((ii + 1L) >= dwLenIn)
        goto return_4;
      bufi[ii] = (TRAWIO)cmd;
      bufi[++ii] = (TRAWIO)24;
      ++ii;
      break;

    case 17: // w irqio
      if ((oo + 1L) >= dwLenOut || (oo + 1L + (U32)bufo[oo+1L] * 2 >= dwLenOut))
        goto return_4;
      len = (unsigned)bufo[++oo];
      if (len > MAX_RAW_IRQ_IO_LEN)
      {
        __tmkRawIrqIoLen[realnum] = 0;
        goto return_3;
      }
      __tmkRawIrqIoLen[realnum] = len;
      pos = 0;
      while (len--)
      {
        data = (unsigned)bufo[++oo];
        port = (unsigned)bufo[++oo];
        if (data > 15)
        {
          __tmkRawIrqIoLen[realnum] = 0;
          goto return_3;
        }
        if (data <= 5)
        {
          if ((port > 0x1FF) ||
             ((port <= 0xFF) &&
               ((port + localport) > __tmkPortsMax1[realnum])) ||
             ((port > 0xFF) &&
               ((__tmkPortsAddr2[realnum] == 0xFFFF) ||
               ((__tmkPortsAddr2[realnum] != 0xFFFF) &&
                 ((port + __tmkPortsAddr2[realnum] - 0x0100) > __tmkPortsMax2[realnum])))))
          {
            __tmkRawIrqIoLen[realnum] = 0;
            goto return_3;
          }
        }
        __tmkRawIrqIoOp[realnum][pos] = (U08)data;
        __tmkRawIrqIoVal[realnum][pos] = (U16)port;
        ++pos;
      }
      break;

    case 18: // r irqio
      if ((oo + 1L) >= dwLenOut || (ii + 2L + (U32)bufo[oo+1L] * 2 >= dwLenIn))
        goto return_4;
      len = (unsigned)bufo[++oo];
      bufi[ii] = (TRAWIO)cmd;
      bufi[++ii] = (TRAWIO)len;
      bufi[++ii] = (TRAWIO)__tmkRawIrqIoLen[realnum];
      pos = 0;
      while (len--)
      {
        if (pos < __tmkRawIrqIoLen[realnum])
        {
          bufi[++ii] = (TRAWIO)__tmkRawIrqIoOp[realnum][pos];
          bufi[++ii] = (TRAWIO)__tmkRawIrqIoVal[realnum][pos];
        }
        else
        {
          bufi[++ii] = (TRAWIO)0;
          bufi[++ii] = (TRAWIO)0;
        }
        ++pos;
      }
      ++ii;
      break;

    case 19: // disable irq
      if ((oo) >= dwLenOut) // never
        goto return_4;
      GET_DIS_IRQ_RAW();
      break;

    case 20: // restore irq
      if ((oo) >= dwLenOut) // never
        goto return_4;
      REST_IRQ_RAW();
      break;

    case 21: // delay loop
      if ((oo + 1L) >= dwLenOut)
        goto return_4;
      len = (unsigned)bufo[++oo];
      while (len-- != 0);
      break;

    case 22: // r reg double check
      if ((oo + 1L) >= dwLenOut || (ii + 2L) >= dwLenIn)
        goto return_4;
      port = (unsigned)bufo[++oo];
      if (port > 0x1FF)
        goto return_3;
      bufi[ii] = (TRAWIO)cmd;
      bufi[++ii] = (TRAWIO)port;
      if (port <= 0xFF)
      {
        port += localport;
        if (port > __tmkPortsMax1[realnum])
          goto return_3;
      }
      else
      {
        if (__tmkPortsAddr2[realnum] == 0xFFFF)
          goto return_3;
        port += __tmkPortsAddr2[realnum] - 0x0100;
        if (port > __tmkPortsMax2[realnum])
          goto return_3;
      }
      GET_DIS_IRQ_SMP_RAW();
      limit = 16; // abstract loop limit for broken hardware
      data = inpw(port);
      do
      {
        datax = data;
        data = inpw(port);
      }
      while (data != datax && --limit != 0);
      bufi[++ii] = (TRAWIO)data;
      REST_IRQ_SMP_RAW();
      ++ii;
      break;

    case 23: // r reg while eq
      if ((oo + 5L) >= dwLenOut || (ii + 6L) >= dwLenIn)
        goto return_4;
      port = (unsigned)bufo[++oo];
      if (port > 0x1FF)
        goto return_3;
      bufi[ii] = (TRAWIO)cmd;
      bufi[++ii] = (TRAWIO)port;
      if (port <= 0xFF)
      {
        port += localport;
        if (port > __tmkPortsMax1[realnum])
          goto return_3;
      }
      else
      {
        if (__tmkPortsAddr2[realnum] == 0xFFFF)
          goto return_3;
        port += __tmkPortsAddr2[realnum] - 0x0100;
        if (port > __tmkPortsMax2[realnum])
          goto return_3;
      }
      mask1 = (unsigned)bufo[++oo];
      mask2 = (unsigned)bufo[++oo];
      param = (unsigned)bufo[++oo]; // double check flag
      limit = (unsigned)bufo[++oo];
      if (!limit)
        ++limit;
      bufi[++ii] = (TRAWIO)mask1;
      bufi[++ii] = (TRAWIO)mask2;
      bufi[++ii] = (TRAWIO)param;
      GET_DIS_IRQ_SMP_RAW();
      data = inpw(port);
      REST_IRQ_SMP_RAW();
      datax = (param) ? ~data : data;
      while (--limit != 0 && data != datax && ((data & mask1) == mask2))
      {
        datax = data;
        GET_DIS_IRQ_SMP_RAW();
        data = inpw(port);
        REST_IRQ_SMP_RAW();
        if (!param)
          datax = data;
      }
      bufi[++ii] = (TRAWIO)limit;
      bufi[++ii] = (TRAWIO)data;
      ++ii;
      break;

    case 24: // r reg while neq
      if ((oo + 5L) >= dwLenOut || (ii + 6L) >= dwLenIn)
        goto return_4;
      port = (unsigned)bufo[++oo];
      if (port > 0x1FF)
        goto return_3;
      bufi[ii] = (TRAWIO)cmd;
      bufi[++ii] = (TRAWIO)port;
      if (port <= 0xFF)
      {
        port += localport;
        if (port > __tmkPortsMax1[realnum])
          goto return_3;
      }
      else
      {
        if (__tmkPortsAddr2[realnum] == 0xFFFF)
          goto return_3;
        port += __tmkPortsAddr2[realnum] - 0x0100;
        if (port > __tmkPortsMax2[realnum])
          goto return_3;
      }
      mask1 = (unsigned)bufo[++oo];
      mask2 = (unsigned)bufo[++oo];
      param = (unsigned)bufo[++oo]; // double check flag
      limit = (unsigned)bufo[++oo];
      if (!limit)
        ++limit;
      bufi[++ii] = (TRAWIO)mask1;
      bufi[++ii] = (TRAWIO)mask2;
      bufi[++ii] = (TRAWIO)param;
      GET_DIS_IRQ_SMP_RAW();
      data = inpw(port);
      REST_IRQ_SMP_RAW();
      datax = (param) ? ~data : data;
      while (--limit != 0 && data != datax && ((data & mask1) != mask2))
      {
        datax = data;
        GET_DIS_IRQ_SMP_RAW();
        data = inpw(port);
        REST_IRQ_SMP_RAW();
        if (!param)
          datax = data;
      }
      bufi[++ii] = (TRAWIO)limit;
      bufi[++ii] = (TRAWIO)data;
      ++ii;
      break;

    default:
      goto return_3;

// change cmd 16 return value if add new commands!!!
    }
  }
//  DrvIOEpi; many returns here!
  EXIT_IRQ_ALL_RAW();
  return 0;

  return_3:
  EXIT_IRQ_ALL_RAW();
  return 3;

  return_4:
  EXIT_IRQ_ALL_RAW();
  return 4;
}
#endif //def USE_TMKRAWIO

#ifdef DOS32
void tmkOpenDPMI()
{
  union REGS r;

  r.x.eax = 0x0400;
  int386(0x31, &r, &r);
  __tmkPic20Base = r.h.dh;
  __tmkPicA0Base = r.h.dl;
}

int __DataEnd = 0;
int DrvCodeEnd();

int DrvLockRam1(int lock_op, unsigned long addr, unsigned long len)
{
  union REGS r;

  r.x.eax = lock_op;
  r.x.ecx = addr & 0xFFFF;
  r.x.ebx = (addr >> 16) & 0xFFFF;
  r.x.edi = len & 0xFFFF;
  r.x.esi = (len >> 16) & 0xFFFF;
  int386(0x31, &r, &r);
  return (r.x.cflag) ? TMK_DPMI_ERROR : 0;
}

int DrvLockRam(int lock_op)
{
  unsigned long data1_addr, data1_len, data2_addr, data2_len, code_addr, code_len;
  int res = 0;

// unitialized data _BSS segment
// Real order should be checked in .OBJ and .MAP!!!
#if NRT > 0
  data1_addr = FP_OFF(&__mbcData0);
  data1_len = FP_OFF(&__pciBusDevFun) + sizeof(__pciBusDevFun) - data1_addr;
#else
  data1_addr = FP_OFF(&__bcCmdWN);
  data1_len = FP_OFF(&__pciBusDevFun) + sizeof(__pciBusDevFun) - data1_addr;
#endif
// initialized data _DATA segment
  data2_addr = FP_OFF(&__DataBegin);
  data2_len = FP_OFF(&__DataEnd) + sizeof(__DataEnd) - data2_addr;
// code _TEXT segment
  code_addr = FP_OFF(&DrvCodeBegin);
  code_len = FP_OFF(&DrvCodeEnd) - code_addr;
  res |= DrvLockRam1(lock_op, data1_addr, data1_len);
  res |= DrvLockRam1(lock_op, data2_addr, data2_len);
  res |= DrvLockRam1(lock_op, code_addr, code_len);
  return res;
}

int DrvCodeEnd()
{
  return 0x5678;
}
#endif //def DOS32
