/****************************************************************************/
/*      TMKISR.H v5.04.1. ELCUS, 1998,2024                                  */
/*      ISR stuff header for DOS, Windows, Linux, QNX4, QNX6                */
/****************************************************************************/

#ifndef TMKISR_H
#define TMKISR_H

#ifndef NTMK
#define NTMK 8
#endif
#ifndef NRT
#define NRT ((NTMK)*32)
#endif

#ifdef DDKWDM
#ifndef WINDOWS
#define WINDOWS
#endif
#endif

#ifdef DDKNT
#define WINDOWS
#endif

#if defined(LINUX) || defined(QNX6)
typedef int PEPROCESS;
#endif

typedef struct
{
  int nInt;
#if defined(DOS) && !defined(DOS32)
  U16 empty;
#endif
  U16 wMode;
  U16 awEvData[3];
#if !defined(DOS) && !defined(QNX4)
  PEPROCESS hProc;
#endif
} TListEvD, *pTListEvD;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DOS

// 2^^n only !
#define EVENTS_SIZE 512

extern TListEvD aEvData[NTMK+NRT][EVENTS_SIZE];
extern int iEvDataBegin[NTMK+NRT];
extern int iEvDataEnd[NTMK+NRT];
extern int cEvData[NTMK+NRT];
extern int cDpcData[NTMK+NRT];

#if defined(WINDOWS) || defined(LINUX_SWTIMER)
extern unsigned aTimeCount[NTMK+NRT][EVENTS_SIZE];
extern unsigned dwTimeCount[NTMK+NRT];
#endif //WINDOWS LINUX_SWTIMER

__EXTERN void IncDpcData(int hTmk);

#ifndef DrvIO
#ifndef BOOL
#define BOOL char
#endif
#define TMKIRQOFF_T BOOL
#else
#define TMKIRQOFF_T U16
#endif
extern TMKIRQOFF_T __tmkIrqOff[NTMK];

#endif //nDOS

#if (NRT > 0 && defined(MRTA)) || defined(TSTA)
__EXTERN unsigned long DIRQLTmkSave(int hTMK);
__EXTERN void DIRQLTmkRestore(int hTMK, unsigned long Saved);
#else
__EXTERN unsigned DIRQLTmkSave(int hTMK);
__EXTERN void DIRQLTmkRestore(int hTMK, unsigned Saved);
#endif //NRT MRTA
#if NRT > 0
__EXTERN void DpcPhysMRT(int hTmk, int cDpcDataTmk);
__EXTERN int DpcVirtRTBrc(int hTmk, int hTmkI, int cDpcDataTmk);
#endif //NRT
__EXTERN unsigned DIRQLTmksInt1(int hTMK, void *pEvData);
__EXTERN void DpcIExcBC(int hTMK, void *pEvData);

#ifdef __cplusplus
}
#endif

#endif //TMKISR_H
