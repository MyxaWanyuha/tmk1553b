/****************************************************************************/
/*      TMKISRC.C v5.04. ELCUS, 1998,2021                                   */
/*      ISR C stuff for Windows, Linux, QNX4, QNX6                          */
/****************************************************************************/

#include "tmkisr.h"

TListEvD aEvData[NTMK+NRT][EVENTS_SIZE];
int iEvDataBegin[NTMK+NRT];
int iEvDataEnd[NTMK+NRT];
int cEvData[NTMK+NRT];
int cDpcData[NTMK+NRT];

#if defined(WINDOWS) || defined(LINUX_SWTIMER)
unsigned aTimeCount[NTMK+NRT][EVENTS_SIZE];
unsigned dwTimeCount[NTMK+NRT];

#ifdef WINDOWS
extern ULONG dwFreqDivider;
extern int nFreqDividerShift;
#endif
#ifdef LINUX_SWTIMER
extern U32 dwSecMul;
extern int nNsecDiv;
#endif
extern int fUseEvTime;
extern unsigned short wSwTimerCtrl;
#endif //WINDOWS LINUX_SWTIMER

void IncDpcData(int hTmk)
{
  pTListEvD pEvD;

  if (cEvData[hTmk] < EVENTS_SIZE)
  {
    ++cDpcData[hTmk];
//tr('Z');
//tr('0'+(char)(cDpcData[hTmk]));
    iEvDataEnd[hTmk] = (iEvDataEnd[hTmk] + 1) & (EVENTS_SIZE-1);
  }
  else
  {
    pEvD = &(aEvData[hTmk][iEvDataEnd[hTmk]]);
    pEvD->nInt = 5;
    pEvD->wMode = UNDEFINED_MODE;
    pEvD->awEvData[0] = TMK_INT_DRV_OVF;
    pEvD->awEvData[1] = 0;
    pEvD->awEvData[2] = 0;
  }
}
