
/****************************************************************************/
/*      TMKISR.C v5.04.1. ELCUS, 1998,2024                                  */
/*      ISR C/CPP stuff for Windows, Linux, QNX4, QNX6                      */
/****************************************************************************/
//4.03
// - QNX4 nInt==3 event data swap fixed
// - moved MRT irq processing to DPC
// - improved MRT irq processing in DPC, using rtgetblkmrta/rtputblkmrta
// - fixed bug with device type in DPC (nInDpcType added)
//4.05
// - small TMK_INT_OTHER fix (pEvD->awEvData[2] = 0;)
//4.06
// - pEvD->nInt = 0; etc.; added for brc MRT irq when no fWriteSA
// - spin_lock_irqsave/spin_unlock_irqrestore regardless of __SMP__
//4.10
// - QNX6 some changes (now as in Linux)
//4.20
// - limit __mrtLastBrcTxRT / hTmkT for use numbers above 255 (with tmknll.c)
//5.00
// - extracted DpcPhysMRT, DpcVirtRTBrc, tmkDpcToWake from tmkDpcRoutine
//   to conserve stack usage for temp vars (in gcc)
// - made *hlnProcWake[] and cToWake global to conserve stack usage in case
//   of many virtual RTs, also shifted spin unlocks later to protect this
// - drop nInt=2 when MRT receives broadcast data into busy subaddress
// - joined DpcPhysMRT, DpcVirtRTBrc from tmkisr.c to tmknll.c
// - move all event related data into tmkisr.c with tmkisr.h
// - extracted C only stuff (related to tmknll.c) to tmkisrc.c
//5.02
// - improve processing of TAI/MRTAI with disabled irq output
// - rework disabled irq output processing with __tmkIrqOff for all types
// - add TSTX,TSTXI,TSTA,TSTAI types
//5.04
// - add LINUX_SWTIMER stuff
// - fix tmkDpcRoutine type for Linux kernel 2.2
//5.04.1 
// - update LINUX_SWTIMER stuff for Linux kernel >= 4.18 (5.07)
// - clarify Linux kernel < 2.6.19 for pt_regs use in irq handler (5.07)
// - avoid Linux warning on tmkDpc_task definition (5.07)

#include "tmkisr.h"

#ifdef LINUX
#define outpw(port, data) outw(data, port)
#define inpw(port) inw(port)
#define outpb(port, data) outb(data, port)
#define inpb(port) inb(port)
#endif


//
// This is the ISR
//

#ifdef QNX6
const struct sigevent* tmkInterruptServiceRoutine(void* dev_id, int id)
{
  int hIrq;
#endif //QNX6

#ifdef QNX4
pid_t far tmkInterruptServiceRoutine()
{
#endif //QNX4

#ifdef LINUX
IRQRETURN_T tmkInterruptServiceRoutine(int irq, void *dev_id
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
                                       , struct pt_regs *regs
#endif
                                      )
{
  int hIrq;
#endif //LINUX

#ifdef WINDOWS
BOOLEAN
tmkInterruptServiceRoutine(
    IN PKINTERRUPT Interrupt,
    IN OUT PVOID Context
    )
{
//  PDEVICE_OBJECT DeviceObject;
//  PTMK_DEVICE_EXTENSION extension;
  LARGE_INTEGER ddwTime;
  LARGE_INTEGER ddwFreq;
//  unsigned short wTimer;
  int fReadTime = 1;
#endif //WINDOWS

  int ihTmk, hTmk;
//  int fTaskletSchedule = 0;
  int fMyISR = FALSE;
  pTListEvD pEvD;
  TIrq *pIrq;
  TTmkConfig *pTmkCfg;
  unsigned intr;
  unsigned saved;
#ifdef LINUX
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
  static struct tq_struct tmkDpc_task = {NULL, 0, (void *)tmkDpcRoutine, 0};
#endif
  unsigned long flags;
#ifdef LINUX_SWTIMER
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,18,0)
  struct timespec64 tsTime;
#else
  struct timespec tsTime;
#endif
  U32 dwTime = 0; // =0 to suppress uninitialized data warning in old gcc
  int fReadTime = 1;
#endif
#endif //LINUX

#ifdef QNX6
  hIrq = (int)((long int)dev_id - (long int)tmkInterruptServiceRoutine);
  pIrq = &ahIrq[hIrq];

  InterruptLock(&tmkIrqSpinLock);
#endif //QNX6

#ifdef QNX4
  pIrq = &tmkIrq;
#endif //QNX4

#ifdef LINUX
  hIrq = (int)((ptrdiff_t)dev_id - (ptrdiff_t)tmkInterruptServiceRoutine);
  pIrq = &ahIrq[hIrq];

  #ifdef MY_DBG_DPC
  printk(MY_KERN_DEBUG "Tmk1553b: irq %d\n", hIrq);
  #endif

  spin_lock_irqsave(&tmkIrqSpinLock, flags);
#endif //LINUX

#ifdef WINDOWS
//  DeviceObject = Context;
//  extension = DeviceObject->DeviceExtension;

  pIrq = (TIrq*)Context;
#endif //WINDOWS

  for (ihTmk = 0; ihTmk < pIrq->cTmks; ++ihTmk)
  {
    hTmk = pIrq->hTmk[ihTmk];
    if (__tmkIrqOff[hTmk])
      continue;
    pTmkCfg = &aTmkConfig[hTmk];
    if (pTmkCfg->nType == TAI || pTmkCfg->nType == MRTAI || pTmkCfg->nType == TSTAI)
    {
      if ((inpw(pTmkCfg->wPorts1 + 0x6) & 0xE000) == 0)
        continue;
    }
    else if (pTmkCfg->nType == TMKXI || pTmkCfg->nType == MRTXI || pTmkCfg->nType == TSTXI)
    {
      if (pTmkCfg->fLocalReadInt)
      {
        if ((inpw(pTmkCfg->wPorts1 + 0xA) & 0x8000) == 0)
          continue;
      }
      else
      {
        if ((inpw(pTmkCfg->wHiddenPorts + 0x4C) & 0x0004) == 0)
          continue;
      }
    }
    else if (pTmkCfg->nType < MIN_TMK_TYPE || pTmkCfg->nType > MAX_TMK_TYPE)
    {
      #ifdef MY_DBG_DPC
      printk(MY_KERN_DEBUG "Tmk1553b: check type=%d error!!!\n", pTmkCfg->nType);
      #endif
      continue;
    }
    fMyISR = TRUE;
//    fTaskletSchedule = 1;

#ifdef WINDOWS
    if (fReadTime)
    {
      if (fUseEvTime)
      {
        ddwTime = KeQueryPerformanceCounter((PLARGE_INTEGER)&ddwFreq);
        ddwTime.QuadPart = (ddwTime.QuadPart << nFreqDividerShift) / dwFreqDivider;
      }
      else
      {
        ddwTime.LowPart = 0;
      }
      fReadTime = 0;
    }
#endif //WINDOWS
#ifdef LINUX_SWTIMER
    if (fReadTime)
    {
      if (fUseEvTime)
      {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,18,0)
        ktime_get_raw_ts64(&tsTime);
#else
        getrawmonotonic(&tsTime);
#endif
        dwTime = (U32)tsTime.tv_sec * dwSecMul + (U32)(tsTime.tv_nsec / nNsecDiv);
      }
      else
      {
        dwTime = 0;
      }
      fReadTime = 0;
    }
#endif //LINUX_SWTIMER

    saved = DIRQLTmkSave(hTmk);
    do
    {
      pEvD = &(aEvData[hTmk][iEvDataEnd[hTmk]]);

      intr = DIRQLTmksInt1(hTmk, pEvD);

      if (intr & TMK_INT_SAVED)
      {
#ifdef WINDOWS
        aTimeCount[hTmk][iEvDataEnd[hTmk]] = ddwTime.LowPart;
#endif //WINDOWS
#ifdef LINUX_SWTIMER
        aTimeCount[hTmk][iEvDataEnd[hTmk]] = dwTime;
#endif //LINUX_SWTIMER
        IncDpcData(hTmk);
////        fTaskletSchedule = 1;
      }//if (intr & TMK_INT_SAVED)
      if (intr & TMK_INT_OTHER) //TMK_INT_TIMER | TMK_INT_BUSJAM | TMK_INT_FIFO_OVF | TMK_INT_GEN1 | TMK_INT_GEN2...
      {
        pEvD = &(aEvData[hTmk][iEvDataEnd[hTmk]]);
        pEvD->nInt = 5;
        pEvD->wMode = UNDEFINED_MODE;
        pEvD->awEvData[0] = (unsigned short) (intr & TMK_INT_OTHER);
// this requires SMP support in tmkgettimer or move this to DPC
//        *((ULONG*)(&(pEvD->awEvData[1]))) = tmkgettimer(m_Unit+hTmk);
        pEvD->awEvData[1] = 0;
        pEvD->awEvData[2] = 0;
#ifdef WINDOWS
        aTimeCount[hTmk][iEvDataEnd[hTmk]] = ddwTime.LowPart;
#endif //WINDOWS
#ifdef LINUX_SWTIMER
        aTimeCount[hTmk][iEvDataEnd[hTmk]] = dwTime;
#endif //LINUX_SWTIMER
        IncDpcData(hTmk);
////        fTaskletSchedule = 1;
      }
    }
    while (intr & TMK_INT_MORE);
    DIRQLTmkRestore(hTmk, saved);
  }

#ifdef QNX6
  InterruptUnlock(&tmkIrqSpinLock);
  if (fMyISR)
    return(&intr_event);
  else
    return(NULL);
#endif //QNX6

#ifdef QNX4
  return (fMyISR) ? tmk_proxy : 0;
#endif //QNX4

#ifdef LINUX
  spin_unlock_irqrestore(&tmkIrqSpinLock, flags);

//  if (!fTaskletSchedule)
  if (!fMyISR)
    return IRQ_NONE;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
  queue_task(&tmkDpc_task, &tq_immediate);
  mark_bh(IMMEDIATE_BH);
#else
  tasklet_schedule(&tmkDpc);
#endif
  return IRQ_HANDLED;
#endif //LINUX

#ifdef WINDOWS
//  if (fTaskletSchedule)
  if (fMyISR)
  {
    KeInsertQueueDpc(
        &tmkDpc,
        0,
        0);
  }
  return fMyISR;
#endif //WINDOWS

}

#ifndef DYNAMIC_TMKNUM
#define HTMK
#define HTMK__
#else
#define HTMK hTmk
#define HTMK__ hTmk,
#endif

#if defined(LINUX) || defined(QNX6)
TListProc *hlnProcWake[MAX_TMK_NUMBER+1+MAX_RT_NUMBER+1];
int cToWake;
#endif

void tmkDpcToWake(int hTmk, int cDpcDataTmk)
{
// Assume cDpcDataTmk > 0 on call here !!!
#if defined(LINUX) || defined(QNX6)
  TListProc *hlnProcTmk;
#endif //LINUX QNX6
#ifdef QNX4
  pTListEvD pEvD;
  int iEvD, cEvD;
  unsigned short tmpw;
#endif //QNX4

#ifdef QNX4
  iEvD = (iEvDataBegin[hTmk] + cEvData[hTmk] - cDpcDataTmk) & (EVENTS_SIZE-1);
  cEvD = cDpcDataTmk;
  do
  {
    pEvD = &(aEvData[hTmk][iEvD]);
    pEvD->nInt |= (pEvD->nInt << 16);
    switch (pEvD->wMode)
    {
    case BC_MODE:
    case MT_MODE:
// check for tmkselect/tmkselected !!!
      if (pEvD->nInt)
      {
        if (pEvD->nInt == 0x30003)
        {
          tmpw = pEvD->awEvData[1];
          pEvD->awEvData[1] = pEvD->awEvData[0];
          pEvD->awEvData[0] = tmpw;
        }
        else if (pEvD->awEvData[2])
          DpcIExcBC(hTmk, pEvD);
        if (pTmkStartOCB->tmkProxy)
        {
          if (pTmkStartOCB->wProxyMode)
            pTmkStartOCB->aiPxD[(pTmkStartOCB->iiPxD + pTmkStartOCB->ciPxD++) & 511] = iEvD;
          Trigger(pTmkStartOCB->tmkProxy);
        }
        iPxD[hTmk] = iEvD;
      }
      break;
    case RT_MODE:
    case MRT_MODE:
      if (pEvD->nInt)
      {
        if (pEvD->nInt == 0x10001)
          pEvD->awEvData[1] = pEvD->awEvData[0];
        for (pOCB = FirstOCB[hTmk].pNext; pOCB != NULL; pOCB = pOCB->pNext)
        {
          if (pOCB->tmkProxy)
          {
            if (pOCB->wModeDeep && pOCB->wProxyMode)
              pOCB->aiPxD[(pOCB->iiPxD + pOCB->ciPxD++) & 511] = iEvD;
            Trigger(pOCB->tmkProxy);
          }
        }
        iPxD[hTmk] = iEvD;
      }
      break;
    }
    iEvD = (iEvD + 1) & (EVENTS_SIZE-1);
  }
  while (--cEvD);
  iEvDataBegin[hTmk] = (iEvDataBegin[hTmk] + cEvData[hTmk]) & (EVENTS_SIZE-1);
  cEvData[hTmk] = 0;
#endif //QNX4

#if defined(LINUX) || defined(QNX6)
  hlnProcTmk = aTMK[hTmk].hlnProc;
  if (hlnProcTmk)
  {
    tmkEvents[hTmk>>5] |= (1 << (hTmk & 0x1F));
    #ifdef MY_DBG_DPC
    printk(MY_KERN_DEBUG "Dpc tmkEvents %Xh, Tmk %d\n", tmkEvents[hTmk>>5], hTmk);
    #endif
    if (hlnProcTmk->maskEvents[hTmk>>5] & (1 << (hTmk & 0x1F)))
    {
      hlnProcWake[cToWake++] = hlnProcTmk;
//          hlnProcTmk->maskEvents = 0;
      memset(hlnProcTmk->maskEvents, 0, sizeof(hlnProcTmk->maskEvents));
    }
    else
    {
      if (cEvData[hTmk] == EVENTS_SIZE)
      {
        iEvDataBegin[hTmk] = (iEvDataBegin[hTmk] + EVENTS_SIZE/2) & (EVENTS_SIZE-1);
        cEvData[hTmk] = EVENTS_SIZE/2;
      }
    }
  }
  else
  {
    iEvDataBegin[hTmk] = (iEvDataBegin[hTmk] + cEvData[hTmk]) & (EVENTS_SIZE-1);
    cEvData[hTmk] = 0;
  }
#endif //LINUX QNX6

#ifdef WINDOWS
  if (aTMK[hTmk].hEvent)
  {
//      for (; cDpcDataTmk != 0; --cDpcDataTmk)
    #ifdef MY_DBG_DPC
    KdPrint(("DpcEvent %d for Tmk %d\n", cDpcDataTmk, hTmk));
    #endif
    KeSetEvent(aTMK[hTmk].hEvent, (KPRIORITY)0, FALSE);
  }
  else
  {
//!!! in pre 5.00 we got here always if !hEvent, now if cDpcDataTmk > 0 externally
//!!! logically it seems OK anyway, just the difference to note
    iEvDataBegin[hTmk] = (iEvDataBegin[hTmk] + cEvData[hTmk]) & (EVENTS_SIZE-1);
    cEvData[hTmk] = 0;
  }
#endif //WINDOWS

  return;
}

//
// This is the deferred procedure call that gets data queued by the ISR to
// finish any interrupt relate processing (AKA tasklet)
// DpcIExcBC called in tmkgetevd() for LINUX and WINDOWS
// DpcIExcBC called in tmkDpcRoutine() for QNX4
//

#ifdef QNX6
void
tmkDpcRoutine()
{
#endif //QNX6

#ifdef QNX4
void
tmkDpcRoutine()
{
#endif //QNX4

#ifdef LINUX
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,0)
void
tmkDpcRoutine(
    void *SystemArgument
    )
{
#else
void
tmkDpcRoutine(
    unsigned long SystemArgument
    )
{
#endif
#endif //LINUX

#ifdef WINDOWS
VOID
tmkDpcRoutine(
    IN PKDPC Dpc,
    IN PVOID DeferredContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
    )
{
//  PDEVICE_OBJECT DeviceObject;
//  PTMK_DEVICE_EXTENSION extension;
#endif //WINDOWS

  int hTmk, cDpcDataTmk, hTmkI;
  int nInDpcType;
  char afMRT[MAX_TMK_NUMBER+1];

#ifdef QNX6
  pthread_mutex_lock(&tmkMutexLock);

  #ifdef MY_DBG_DPC
  printk(MY_KERN_DEBUG "Tmk1553b: DPC\n");
  #endif
#endif //QNX6

#ifdef LINUX
  spin_lock(&tmkSpinLock);

  #ifdef MY_DBG_DPC
  printk(MY_KERN_DEBUG "Tmk1553b: DPC ");
  #endif
#endif //LINUX

#ifdef WINDOWS
//  DeviceObject = DeferredContext;
//  extension = DeviceObject->DeviceExtension;

  KeAcquireSpinLockAtDpcLevel(&tmkSpinLock);

  #ifdef MY_DBG_DPC
  KdPrint(("DPC\n"));
  #endif
#endif //WINDOWS

#if defined(LINUX) || defined(QNX6)
  cToWake = 0;
#endif //LINUX QNX6

  for (hTmk = 0; hTmk <= nMaxTmkNumber; ++hTmk)
  {
    if (hTmk <= MAX_TMK_NUMBER)
    {
      if (aTmkConfig[hTmk].nType == -1)
        continue;
      afMRT[hTmk] = 0;
      hTmkI = hTmk;
      nInDpcType = aTmkConfig[hTmk].nType;
    }
    else
    {
      hTmkI = rt2mrt(hTmk);
      if (afMRT[hTmkI] == 0)
        continue;
      nInDpcType = aTmkConfig[hTmkI].nType;
    }

#ifdef QNX6
    InterruptLock(&tmkIrqSpinLock);
    cDpcDataTmk = cDpcData[hTmk];  // dwSyncArg
    cDpcData[hTmk] = 0;
    InterruptUnlock(&tmkIrqSpinLock);
#endif //QNX6

#ifdef QNX4
    _disable();
    cDpcDataTmk = cDpcData[hTmk];
    cDpcData[hTmk] = 0;
    _enable();
#endif //QNX4

#ifdef LINUX
    spin_lock_irq(&tmkIrqSpinLock);
    cDpcDataTmk = cDpcData[hTmk];  // dwSyncArg
    cDpcData[hTmk] = 0;
    spin_unlock_irq(&tmkIrqSpinLock);
#endif //LINUX

#ifdef WINDOWS
    cDpcDataTmk = hTmk;
    KeSynchronizeExecution(ahIrq[aTmkConfig[hTmkI].hIrq].InterruptObject1,
                           tmkGetClrIrqs,
                           &cDpcDataTmk);
#endif //WINDOWS

    if (cDpcDataTmk == 0)
      continue;

#ifdef QNX6
    #ifdef MY_DBG_DPC
    printk(MY_KERN_DEBUG "Tmk1553b: dpc %d, %d\n", hTmk, cDpcDataTmk);
    #endif
#endif //QNX6

#ifdef LINUX
    #ifdef MY_DBG_DPC
    printk(MY_KERN_DEBUG "Tmk1553b: dpc %d, %d\n", hTmk, cDpcDataTmk);
    #endif
#endif //LINUX

#ifdef DDKNT
    #ifdef MY_DBG_DPC
    KdPrint(("Dpc %d (%d)\n", hTmk, hTmkI));
    #endif
#endif //DDKNT

// Drop last
//    if (cDpcDataTmk + cEvData[hTmk] > EVENTS_SIZE)
//      cDpcDataTmk = EVENTS_SIZE - cEvData[hTmk];
// Drop last

    cEvData[hTmk] += cDpcDataTmk;

// Drop first
    if (cEvData[hTmk] > EVENTS_SIZE)
    {
      iEvDataBegin[hTmk] = (iEvDataBegin[hTmk] + cEvData[hTmk] - EVENTS_SIZE) & (EVENTS_SIZE-1);
      cEvData[hTmk] = EVENTS_SIZE;
    }
// Drop first

    if (hTmk <= MAX_TMK_NUMBER &&
        (nInDpcType == MRTX || nInDpcType == MRTXI ||
         nInDpcType == MRTA || nInDpcType == MRTAI))
    {
      afMRT[hTmk] = 1;
      DpcPhysMRT(hTmk, cDpcDataTmk);
    }

    if (hTmk > MAX_TMK_NUMBER)
//        && ((__adwTmkOptions[hTmk] & MRT_WRITE_BRC_DATA) ||
//         (__adwTmkOptions[hTmk] & RT_NO_BRC_IRQS)))
    {
      cDpcDataTmk = DpcVirtRTBrc(hTmk, hTmkI, cDpcDataTmk);
    }

    if (cDpcDataTmk > 0)
    {
      tmkDpcToWake(hTmk, cDpcDataTmk);
    }
  }

#ifdef QNX6
  while (cToWake > 0)
  {
//#ifdef MY_DBG
//    printk(MY_KERN_DEBUG "tmk1553b: pthread_cond_broadcast\n");
//#endif
    pthread_cond_broadcast(&hlnProcWake[--cToWake]->cond);
  }

  pthread_mutex_unlock(&tmkMutexLock);
#endif //QNX6

#ifdef LINUX
  while (cToWake > 0)
  {
    #ifdef MY_DBG_DPC
    printk(MY_KERN_DEBUG "w");
    #endif
    wake_up_interruptible(&hlnProcWake[--cToWake]->wq);
  }
  #ifdef MY_DBG_DPC
  printk(MY_KERN_DEBUG "z\n");
  #endif

  spin_unlock(&tmkSpinLock);
#endif //LINUX

#ifdef WINDOWS
  KeReleaseSpinLockFromDpcLevel(&tmkSpinLock);
#endif //WINDOWS

}

#ifdef WINDOWS
BOOLEAN
tmkGetClrIrqs(
    IN OUT PVOID Context
    )
{
  int hTmk = *(PULONG)Context;
  *(PULONG)Context = cDpcData[hTmk];
  cDpcData[hTmk] = 0;

  return TRUE;
}
#endif //WINDOWS
