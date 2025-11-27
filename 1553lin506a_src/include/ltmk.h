
/****************************************************************************/
/*      LTMK.H v5.06 for Linux. (c) ELCUS, 2002,2022.                       */
/*      Interface to driver tmk1553b v5.06 for Linux.                       */
/****************************************************************************/

//#ifndef _TMK1553B_
//#define _TMK1553B_

//#define USE_TMK_ERROR

#define TMK_DATA unsigned short
#define TMK_DATA_RET unsigned short
#define WORD unsigned short
#define HANDLE int
#define DWORD unsigned int
#define ULONG unsigned long
#define BOOL int
#define TRUE 1
#define FALSE 0

#ifdef LTMK_SEPARATE_THREADS
#define TMK_THREADLOCAL __thread
#else
#define TMK_THREADLOCAL
#endif

#ifndef _TMK1553B_MRT
#define _TMK1553B_MRT
#endif

#ifdef _TMK1553B_MRT
#define TMK_VERSION_MIN 0x0505 /* v5.05 */
#define TMK_VERSION 0x0506     /* v5.06 */
#else
#define TMK_VERSION_MIN 0x0505 /* v5.05 */
#define TMK_VERSION 0x0506     /* v5.06 */
#endif //def TMK1553B_MRT

#ifndef MAX_TMK_NUMBER
#ifdef _TMK1553B_MRT
#define MAX_TMK_NUMBER (8+8*32-1)
#else
#define MAX_TMK_NUMBER (8-1)
#endif //def TMK1553B_MRT
#endif

#ifndef NMBCID
#define NMBCID (8*2)
#endif

#define MIN_TMK_TYPE 2
#define MAX_TMK_TYPE 16

#define TMK400 2
#define TMKMPC 3
#define RTMK400 4
#define TMKX 5
#define TMKXI 6
#define MRTX 7
#define MRTXI 8
#define TA 9
#define TAI 10
#define MRTA 11
#define MRTAI 12
#define TSTX 13
#define TSTXI 14
#define TSTA 15
#define TSTAI 16

#define ALL_TMKS 0x7FFF

#define GET_TMK_OPTIONS 0x4L
#ifdef _TMK1553B_MRT
#define MRT_READ_BRC_DATA 0x1L
#define MRT_WRITE_BRC_DATA 0x2L
#endif //_TMK1553B_MRT
#define BC_STARTA_INTA 0x8L
#define MTRT_ALLOW_ANOMALY 0x10L
#define RT_USE_ADDR_PARITY 0x20L
#define RT_SHOW_BRC_STATE  0x40L

#define ADDR_PARITY_ON 0x0100

#define TMK_INT_DRV_OVF       0x0002
#define TMK_INT_DRV_BUF_SKIP  0x0004
#define TMK_INT_TIMER         0x4000
#define TMK_INT_BUSJAM        0x2000
#define TMK_INT_FIFO_OVF      0x0400
//#define TMK_INT_GEN1
//#define TMK_INT_GEN2

#ifdef NMBCID
#define MBC_ALLOC_FAIL 0xFFFF
#endif

#define MTF_OK           0x4000
#define MTF_FILTER_OFF   0x0000
#define MTF_FILTER_ON    0x2000
#define MTF_DISALLOW     0x0000
#define MTF_ALLOW        0x8000
#define MTF_DATA         0x0FFF
#define MTF_WRITE        0x4000
#define MTF_ALL          0x4001
#define MTF_ADDR         0x4002
#define MTF_GET_ADDR     0x0002

#define RTL_OK           0x4000
#define RTL_LEGAL_OFF    0x0000
#define RTL_LEGAL_ON     0x2000
#define RTL_DISALLOW     0x0000
#define RTL_ALLOW        0x8000
#define RTL_DATA         0x0FFF
#define RTL_WRITE        0x4000
#define RTL_ALL          0x4001
#define RTL_SA           0x4002
#define RTL_GET_SA       0x0002
#define RTL_CMD          0x4003
#define RTL_GET_CMD      0x0003
#define RTL_ALL_MRT      0x4004
#define RTL_ALL_RT       0x4005
#define RTL_ALL_BRCST    0x4006
#define RTL_SA_RT        0x4007
#define RTL_GET_SA_RT    0x0007
#define RTL_SA_BRCST     0x4008
#define RTL_GET_SA_BRCST 0x0008

#define CTLA_CTRLA 0x0001
#define CTLA_TIMEA 0x0002
#define CTLA_LINKA 0x0004
#define CTLA_ALL   0x0007

#define TMK_DAC_NODATA   0x1000
#define TMK_DAC_ERROR    0x1FFF
#define GET_DAC_MAX      0x2000
#define GET_DAC_STEP     0x3000
#define GET_DAC_TMK      0x4000

#define SA_R             0x0000
#define SA_T             0x0020
#define SABUF_GETMODE    0xF000
#define SABUF_OFF        0x0000
#define SABUF_OFF_R      (SABUF_OFF | SA_R)
#define SABUF_OFF_T      (SABUF_OFF | SA_T)
#define SABUF_STD        0x1000
#define SABUF_STD_R      (SABUF_STD | SA_R)
#define SABUF_STD_T      (SABUF_STD | SA_T)
#define SABUF_STD_R0     (SABUF_STD | SA_R)
#define SABUF_STD_T0     (SABUF_STD | SA_T)
#define SABUF_JOINT      0x2000
#define SABUF_HEAP       0x3000
#define SABUF_HEAP_R     (SABUF_HEAP | SA_R)
#define SABUF_HEAP_T     (SABUF_HEAP | SA_T)
#define SABUF_JHEAP      0x4000
#define SABUF_GLOBAL1_RT  0x5000
#define SABUF_GLOBAL1_MRT 0x6000

#define GET_MAX_ALLOC 0xFFFF
#define GET_ALLOC     0xFFFE

#define RT_BASE_ACTIVE 0x2000
#define MTRT_BASE_ACTIVE 0x2000

#define RT_LINK_ER 0x2000
#define RT_LINK_IP 0x4000
#define RT_LINK_BS 0x8000

#define GET_RT_GAP    0xFFFF
#define GET_RT_GAP_HW 0xFFFE
#define RT_GAP_DEFAULT     0
#define RT_GAP_OPT1        1

#define CODEC_READ      0xFFFF
#define CODEC_SYNC      0x0020
#define CODEC_SYNC_DATA 0x0000
#define CODEC_SYNC_CMD  0x0020
#define CODEC_BUS       0x0040
#define CODEC_BUS_A     0x0000
#define CODEC_BUS_B     0x0040

#define GET_TXRX_MODE    0xFFFF
#define GET_TXRX_MODE_HW 0xFFFE
#define TXRX_TXA_ENABLE  0x0200
#define TXRX_TXB_ENABLE  0x0100
#define TXRX_TXA_DISABLE 0x0000
#define TXRX_TXB_DISABLE 0x0000
#define TXRX_RXA_ENABLE  0x0080
#define TXRX_RXB_ENABLE  0x0040
#define TXRX_RXA_DISABLE 0x0000
#define TXRX_RXB_DISABLE 0x0000
#define TXRX_ALL_ENABLE  0x03C0
#define TXRX_ALL_DISABLE 0x0000

#define GET_TIMEOUT    0xFFFF
#define GET_TIMEOUT_HW 0xFFFE

#define SWTIMER_OFF   0x0000
#define SWTIMER_ON    0x2400
#define SWTIMER_EVENT 0x8000
#define SWTIMER_RESET 0xFBFF

#define GET_SWTIMER_CTRL 0xFFFF

#define TIMER_RESET 0xFBFF
#define TIMER_OFF   0x0000
#define TIMER_16BIT 0x3400
#define TIMER_32BIT 0x2400
#define TIMER_1US   0x0000
#define TIMER_2US   0x0080
#define TIMER_4US   0x0100
#define TIMER_8US   0x0180
#define TIMER_16US  0x0200
#define TIMER_32US  0x0280
#define TIMER_64US  0x0300
#define TIMER_STOP  0x0380
#define TIMER_SYN   0x0040
#define TIMER_SYND  0x0020
#define TIMER_SA    0x001F

#define TIMER_NOSTOP 0x2000

#define TIMER_MASK  0x37FF
#define TIMER_STEP  0x0380
#define TIMER_BITS  0x3400

#define GET_TIMER_CTRL    0xFFFF
#define GET_TIMER_CTRL_HW 0xFFFE

#define DATA_BC_RT 0x00
#define DATA_BC_RT_BRCST 0x08
#define DATA_RT_BC 0x01
#define DATA_RT_RT 0x02
#define DATA_RT_RT_BRCST 0x0A
#define CTRL_C_A 0x03
#define CTRL_C_BRCST 0x0B
#define CTRL_CD_A 0x04
#define CTRL_CD_BRCST 0x0C
#define CTRL_C_AD 0x05

#define CC_FMT_1 0x00
#define CC_FMT_2 0x01
#define CC_FMT_3 0x02
#define CC_FMT_4 0x03
#define CC_FMT_5 0x05
#define CC_FMT_6 0x04
#define CC_FMT_7 0x08
#define CC_FMT_8 0x0A
#define CC_FMT_9 0x0B
#define CC_FMT_10 0x0C

#define MT_FMT_ERR 0x07
#define MT_FMT_MW 0x0F

#define BUS_A 0
#define BUS_B 1
#define BUS_1 0
#define BUS_2 1

#define S_ERAO_MASK 0x01
#define S_MEO_MASK 0x02
#define S_IB_MASK 0x04
#define S_TO_MASK 0x08
#define S_EM_MASK 0x10
#define S_EBC_MASK 0x20
#define S_DI_MASK 0x40
#define S_ELN_MASK 0x80

#define S_G1_MASK 0x1000
#define S_G2_MASK 0x2000

#define NWORDS_MASK 0x001F
#define CMD_MASK 0x041F
#define SUBADDR_MASK 0x03E0
#define CI_MASK 0x03E0
#define HBIT_MASK 0x0200
#define RT_DIR_MASK 0x0400
#define ADDRESS_MASK 0xF800
#define RTFL_MASK 0x0001
#define DNBA_MASK 0x0002
#define SSFL_MASK 0x0004
#define BUSY_MASK 0x0008
#define BRCST_MASK 0x0010
#define NULL_MASK 0x00E0
#define SREQ_MASK 0x0100
#define ERROR_MASK 0x0400

#define SREQ 0x01
#define BUSY 0x02
#define SSFL 0x04
#define RTFL 0x08
#define DNBA 0x10

#define CWB0 0x20
#define CWB1 0x40

#define BC_MODE 0x0000
#define RT_MODE 0x0080
#define MT_MODE 0x0100
#define MRT_MODE 0x0280
#define MW_MODE  0x0400
#define MWT_MODE (MW_MODE|MT_MODE)
#define MTRT_MODE (MT_MODE|RT_MODE)
#define MWRT_MODE (MW_MODE|RT_MODE)
#define MWTRT_MODE (MW_MODE|MT_MODE|RT_MODE)
#define UNDEFINED_MODE 0xFFFF

#define RT_ENABLE     0x0000
#define RT_DISABLE    0x001F
#define RT_GET_ENABLE 0xFFFF

#define BC_BRCMP_MODE 0x0001
#define BC_TMFUL_MODE 0x0002
#define BC_ERSTP_MODE 0x0004
#define BC_BTSTP_MODE 0x0008
#define BC_DNRVM_MODE 0x0010
#define BC_BTMSK_MODE 0x0020
#define BC_JMBUS_MODE 0x0040
#define BC_JMMSK_MODE 0x0100
#define BC_R2BUS_MODE 0x0200
#define BC_R1BUS_MODE 0x0400
#define BC_2RETR_MODE 0x0800
#define BC_RETRY_MODE 0x1000
#define BC_EXINT_MODE 0x4000

#define RT_TRANSMIT 0x0400
#define RT_RECEIVE 0x0000

#define RT_ERROR_MASK 0x4000
#define RT_ACTIVE_MASK 0x0800
#define RT_BRC_MASK 0x8000
#define RT_EM400_MASK 0x1000
#define RT_G1_MASK 0x1000
#define RT_G2_MASK 0x2000

#define RT_FLAG 0x8000
#define RT_FLAG_MASK 0x8000

#define RT_HBIT_MODE  0x0001
#define RT_INST_MODE  0x0001
#define RT_FLAG_MODE  0x0002
#define RT_BRCST_MODE 0x0004
#define RT_RSREQ_MODE 0x0080
#define RT_NOERR_MODE 0x0400
#define RT_CBSNL_MODE 0x0010
#define RT_CBSND_MODE 0x0020
#define RT_CNOBS_MODE 0x0200

#define MT_HBIT_MODE  0x0001
#define MT_INST_MODE  0x0001
#define MT_BRCST_MODE 0x0004
#define MW_DT00_MODE  0x0002

#define RT_DATA_BL 0x2000
#define RT_GENER1_BL 0x0004
#define RT_GENER2_BL 0x4000
#define BC_GENER1_BL 0x0004
#define BC_GENER2_BL 0x4000
#define MT_GENER1_BL 0x0004
#define MT_GENER2_BL 0x4000
#define TMK_IRQ_OFF 0x8000

#define CX_CC_MASK 0x000F
#define CX_CONT_MASK 0x0010
#define CX_BUS_MASK 0x0020
#define CX_SIG_MASK 0x8000
#define CX_INT_MASK 0x0020

#define CX_CONT 0x0010
#define CX_STOP 0x0000
#define CX_BUS_0 0x0000
#define CX_BUS_A 0x0000
#define CX_BUS_1 0x0020
#define CX_BUS_B 0x0020
#define CX_NOSIG 0x0000
#define CX_SIG 0x8000
#define CX_INT 0x0000
#define CX_NOINT 0x0020

#define CA_TF 0x0001
#define CA_BSDAT 0x0002
#define CA_NOBSDAT 0x0000
#define CA_SF 0x0004
#define CA_BS 0x0008
#define CA_BR 0x0010
#define CA_NOP 0x0020
#define CA_NOMSG 0x0020
#define CA_RTRT 0x0040
#define CA_BUS_0 0x0000
#define CA_BUS_A 0x0000
#define CA_BUS_1 0x0080
#define CA_BUS_B 0x0080
#define CA_SR 0x0100
#define CA_RETRY 0x0200
#define CA_NORETRY 0x0000
#define CA_ME 0x0400
#define CA_ERSTP 0x0800
#define CA_NOERSTP 0x0000
#define CA_BTSTP 0x1000
#define CA_NOBTSTP 0x0000
#define CA_CONT 0x2000
#define CA_STOP 0x0000
#define CA_RBUS 0x4000
#define CA_NORBUS 0x4000
#define CA_SIG 0x8000
#define CA_NOSIG 0x0000
#define CA_INT 0x0000
#define CA_NOINT 0x8000
#define CA_BITS (CA_TF | CA_SF | CA_BS | CA_BR | CA_SR | CA_ME)
#define CA_NOBITS 0x0000

//IntX constants

#define SX_NOERR 0
#define SX_MEO 1
#define SX_TOA 2
#define SX_TOD 3
#define SX_ELN 4
#define SX_ERAO 5
#define SX_ESYN 6
#define SX_EBC 7

#define SX_ERR_MASK 0x0007
#define SX_IB_MASK 0x0008
#define SX_G1_MASK 0x0010
#define SX_G2_MASK 0x0020
#define SX_K2_MASK 0x0100
#define SX_K1_MASK 0x0200
#define SX_MW_TO_MASK 0x0001
#define SX_SCC_MASK 0x3C00
#define SX_ME_MASK 0x4000
#define SX_BUS_MASK 0x8000
#define SX_SCC_SHR 10

#define SX_BUS_0 0x0000
#define SX_BUS_A 0x0000
#define SX_BUS_1 0x8000
#define SX_BUS_B 0x8000

// IntA / BC MsgSW / MT MsgSW constants

#define SA_NOERR 0
#define SA_MEO 1
#define SA_TOA 2
#define SA_TOD 3
#define SA_ELN 4
#define SA_ERAO 5
#define SA_ESYN 6
#define SA_EBC 7

#define SA_ERR_MASK 0x0007
#define SA_BUS_MASK 0x0008
#define SA_IB2_MASK 0x0010
#define SA_IB1_MASK 0x0020
#define SA_IB_MASK 0x0030
#define SA_MW_TO_MASK 0x0001
#define SA_RETRY_MASK 0x0040
#define SA_DONE_MASK 0x0080
#define SA_STOP_MASK 0x0200
#define SA_SCC_MASK 0x03C0
#define SA_NWORDS_MASK 0xFC00
#define SA_SCC_SHR 6
#define SA_NWORDS_SHR 10

#define SA_BUS_0 0x0000
#define SA_BUS_A 0x0000
#define SA_BUS_1 0x0008
#define SA_BUS_B 0x0008

//BC MsgSW constants (copy of SA_)

#define SMB_ERR_MASK 0x0007
#define SMB_BUS_MASK 0x0008
#define SMB_IB2_MASK 0x0010
#define SMB_IB1_MASK 0x0020
#define SMB_IB_MASK 0x0030
#define SMB_RETRY_MASK 0x0040
#define SMB_DONE_MASK 0x0080
#define SMB_STOP_MASK 0x0200
#define SMB_NWORDS_MASK 0xFC00
#define SMB_SCC_SHR 6
#define SMB_NWORDS_SHR 10

#define SMB_BUS_0 0x0000
#define SMB_BUS_A 0x0000
#define SMB_BUS_1 0x0008
#define SMB_BUS_B 0x0008

#define BC_MSW_ERR_MASK 0x0007
#define BC_MSW_BUS_MASK 0x0008
#define BC_MSW_IB2_MASK 0x0010
#define BC_MSW_IB1_MASK 0x0020
#define BC_MSW_IB_MASK 0x0030
#define BC_MSW_RETRY_MASK 0x0040
#define BC_MSW_DONE_MASK 0x0080
#define BC_MSW_STOP_MASK 0x0200
#define BC_MSW_NWORDS_MASK 0xFC00
#define BC_MSW_SCC_SHR 6
#define BC_MSW_NWORDS_SHR 10

//MT MsgSW constants (copy of SA_)

#define SMM_ERR_MASK 0x0007
#define SMM_BUS_MASK 0x0008
#define SMM_IB2_MASK 0x0010
#define SMM_IB1_MASK 0x0020
#define SMM_IB_MASK 0x0030
#define SMM_MW_TO_MASK 0x0001
#define SMM_SCC_MASK 0x03C0
#define SMM_NWORDS_MASK 0xFC00
#define SMM_SCC_SHR 6
#define SMM_NWORDS_SHR 10

#define SMM_BUS_0 0x0000
#define SMM_BUS_A 0x0000
#define SMM_BUS_1 0x0008
#define SMM_BUS_B 0x0008

#define MT_MSW_ERR_MASK 0x0007
#define MT_MSW_BUS_MASK 0x0008
#define MT_MSW_IB2_MASK 0x0010
#define MT_MSW_IB1_MASK 0x0020
#define MT_MSW_IB_MASK 0x0030
#define MT_MSW_MW_TO_MASK 0x0001
#define MT_MSW_SCC_MASK 0x03C0
#define MT_MSW_NWORDS_MASK 0xFC00
#define MT_MSW_SCC_SHR 6
#define MT_MSW_NWORDS_SHR 10

// RT MsgSW constants

#define SMR_CMDW_MASK 0x07FF
#define SMR_BUS_MASK 0x0800
#define SMR_BRC_MASK 0x1000
#define SMR_ERR_MASK 0x4000

#define SMR_BUS_0 0x0000
#define SMR_BUS_A 0x0000
#define SMR_BUS_1 0x0800
#define SMR_BUS_B 0x0800

#define RT_MSW_CMDW_MASK 0x07FF
#define RT_MSW_BUS_MASK 0x0800
#define RT_MSW_BRC_MASK 0x1000
#define RT_MSW_ERR_MASK 0x4000

#define GET_IO_DELAY 0xFFFF

#define CW(ADDR,DIR,SUBADDR,NWORDS) ((TMK_DATA)(((ADDR)<<11)|(DIR)|((SUBADDR)<<5)|((NWORDS)&0x1F)))
#define CWM(ADDR,COMMAND) ((TMK_DATA)(((ADDR)<<11)|(CI_MASK)|(COMMAND)))
#define CWMC(ADDR,CI,COMMAND) ((TMK_DATA)(((ADDR)<<11)|((CI)&0x03E0)|(COMMAND)))

/*#define CMD_ILLEGAL 0x000*/
#define CMD_DYNAMIC_BUS_CONTROL 0x400
#define CMD_SYNCHRONIZE 0x401
#define CMD_TRANSMIT_STATUS_WORD 0x402
#define CMD_INITIATE_SELF_TEST 0x403
#define CMD_TRANSMITTER_SHUTDOWN 0x404
#define CMD_OVERRIDE_TRANSMITTER_SHUTDOWN 0x405
#define CMD_INHIBIT_TERMINAL_FLAG_BIT 0x406
#define CMD_OVERRIDE_INHIBIT_TERMINAL_FLAG_BIT 0x407
#define CMD_RESET_REMOTE_TERMINAL 0x408
#define CMD_TRANSMIT_VECTOR_WORD 0x410
#define CMD_SYNCHRONIZE_WITH_DATA_WORD 0x011
#define CMD_TRANSMIT_LAST_COMMAND_WORD 0x412
#define CMD_TRANSMIT_BUILT_IN_TEST_WORD 0x413

#define TA_SA_CMD      0x001F
#define TA_SA_CMD_VECW 0x0400
#define TA_SA_CMD_BITW 0x041F

#define MWP_TIME      0xFF00
#define MWP_TIME_SHR  8
#define MWP_BUS       0x0080
#define MWP_BUS_A     0x0000
#define MWP_BUS_B     0x0080
#define MWP_BOTH      0x0040
#define MWP_SYNC      0x0020
#define MWP_SYNC_DATA 0x0000
#define MWP_SYNC_CMD  0x0020
#define MWP_ME        0x0010
#define MWP_CONT      0x0008
#define MWP_MASK      (MWP_BUS | MWP_BOTH | MWP_SYNC | MWP_ME | MWP_CONT)

#define BUS_ERR_CLR  0x0000
#define BUS_ERR_SET  0x2000
#define BUS_ERR_ONCE 0x0001

#define TMK_INFO_MAX       0x0000
#define MT_BASE_DRV_TO_HW  0x0001
#define MT_BASE_HW_TO_DRV  0x0002
#define TMK_INFO_KEEP_FLGM 0x0003
#define TMK_INFO_CBSND     0x0004
#define TMK_INFO_WRAP_SA   0x0005
#define MRT_BRC_HW_PAGES   0x0006
#define MRT_BRC_HW_PAGE_0  0x0007
#define MRT_BRC_SW_PAGES   0x0008
#define MRT_BRC_SW_PAGE_0  0x0009
#define TMK_INFO_ERROR     0xFFFFFFFFL

#define RT_WRAP_SA_SA  0x001F
#define RT_WRAP_SA_TX  0x0100
#define RT_WRAP_SA_INT 0x0200

#define TMK_BAD_0        -1024
#define TMK_BAD_TYPE     (TMK_BAD_0-1)
#define TMK_BAD_IRQ      (TMK_BAD_0-2)
#define TMK_BAD_NUMBER   (TMK_BAD_0-3)
#define BC_BAD_BUS       (TMK_BAD_0-4)
#define BC_BAD_BASE      (TMK_BAD_0-5)
#define BC_BAD_LEN       (TMK_BAD_0-6)
#define RT_BAD_PAGE      (TMK_BAD_0-7)
#define RT_BAD_LEN       (TMK_BAD_0-8)
#define RT_BAD_ADDRESS   (TMK_BAD_0-9)
#define RT_BAD_FUNC      (TMK_BAD_0-10)
#define BC_BAD_FUNC      (TMK_BAD_0-11)
#define TMK_BAD_FUNC     (TMK_BAD_0-12)
#define VTMK_BAD_VERSION (TMK_BAD_0-13)
#define RT_BAD_BUF       (TMK_BAD_0-14)

typedef struct
{
 short nType;
 char szName[10];
 unsigned short wPorts1;
 unsigned short wPorts2;
 unsigned short wIrq1;
 unsigned short wIrq2;
 unsigned short wIODelay;
} TTmkConfigData;

typedef struct
{
  int nInt;
  unsigned short wMode;
  union
  {
    struct
    {
      unsigned short wResult;
      unsigned short wAW1;
      unsigned short wAW2;
    } bc;
    struct
    {
      unsigned short wBase;
      unsigned short wResultX;
    } bcx;
    struct
    {
      unsigned short wBase;
      unsigned short wResultA;
    } bca;
    struct
    {
      unsigned short wStatus;
      unsigned short wCmd;
      unsigned short wBuf;
    } rt;
    struct
    {
      unsigned short wBase;
      unsigned short wResultX;
    } mt;
    struct
    {
      unsigned short wStatus;
      unsigned short wMRT1;
      unsigned short wMRT2;
    } mrt;
    struct
    {
      unsigned short wRequest;
    } tmk;
  };
} TTmkEventData;

/*
 * Ioctl definitions
 */

/* Use 'k' as magic number */
#define TMK_IOC_MAGIC  'k'

#define TMK_IOC0 0

#define VTMK_tmkconfig 2
#define VTMK_tmkdone 3
#define VTMK_tmkgetmaxn 4
#define VTMK_tmkselect 5
#define VTMK_tmkselected 6
#define VTMK_tmkgetmode 7
#define VTMK_tmksetcwbits 8
#define VTMK_tmkclrcwbits 9
#define VTMK_tmkgetcwbits 10
#define VTMK_tmkwaitevents 11
//#define VTMK_tmkdefevent 11
#define VTMK_tmkgetevd 12

#define VTMK_bcreset 13
#define VTMK_bc_def_tldw 14
#define VTMK_bc_enable_di 15
#define VTMK_bc_disable_di 16
#define VTMK_bcdefirqmode 17
#define VTMK_bcgetirqmode 18
#define VTMK_bcgetmaxbase 19
#define VTMK_bcdefbase 20
#define VTMK_bcgetbase 21
#define VTMK_bcputw 22
#define VTMK_bcgetw 23
#define VTMK_bcgetansw 24
#define VTMK_bcputblk 25
#define VTMK_bcgetblk 26
#define VTMK_bcdefbus 27
#define VTMK_bcgetbus 28
#define VTMK_bcstart 29
#define VTMK_bcstartx 30
#define VTMK_bcdeflink 31
#define VTMK_bcgetlink 32
#define VTMK_bcstop 33
#define VTMK_bcgetstate 34

#define VTMK_rtreset 35
#define VTMK_rtdefirqmode 36
#define VTMK_rtgetirqmode 37
#define VTMK_rtdefmode 38
#define VTMK_rtgetmode 39
#define VTMK_rtgetmaxpage 40
#define VTMK_rtdefpage 41
#define VTMK_rtgetpage 42
#define VTMK_rtdefpagepc 43
#define VTMK_rtdefpagebus 44
#define VTMK_rtgetpagepc 45
#define VTMK_rtgetpagebus 46
#define VTMK_rtdefaddress 47
#define VTMK_rtgetaddress 48
#define VTMK_rtdefsubaddr 49
#define VTMK_rtgetsubaddr 50
#define VTMK_rtputw 51
#define VTMK_rtgetw 52
#define VTMK_rtputblk 53
#define VTMK_rtgetblk 54
#define VTMK_rtsetanswbits 55
#define VTMK_rtclranswbits 56
#define VTMK_rtgetanswbits 57
#define VTMK_rtgetflags 58
#define VTMK_rtputflags 59
#define VTMK_rtsetflag 60
#define VTMK_rtclrflag 61
#define VTMK_rtgetflag 62
#define VTMK_rtgetstate 63
#define VTMK_rtbusy 64
#define VTMK_rtlock 65
#define VTMK_rtunlock 66
#define VTMK_rtgetcmddata 67
#define VTMK_rtputcmddata 68

#define VTMK_mtreset 69
#define VTMK_mtdefirqmode 70
#define VTMK_mtgetirqmode 71
#define VTMK_mtgetmaxbase 72
#define VTMK_mtdefbase 73
#define VTMK_mtgetbase 74
#define VTMK_mtputw 75
#define VTMK_mtgetw 76
#define VTMK_mtgetsw 77
#define VTMK_mtputblk 78
#define VTMK_mtgetblk 79
#define VTMK_mtstartx 80
#define VTMK_mtdeflink 81
#define VTMK_mtgetlink 82
#define VTMK_mtstop 83
#define VTMK_mtgetstate 84

#define VTMK_tmkgetinfo 85
#define VTMK_GetVersion 86

#define VTMK_rtenable 87

#define VTMK_mrtgetmaxn 88
#define VTMK_mrtconfig 89
#define VTMK_mrtselected 90
#define VTMK_mrtgetstate 91
#define VTMK_mrtdefbrcsubaddr0 92
#define VTMK_mrtreset 93

#define VTMK_tmktimer 94
#define VTMK_tmkgettimer 95
#define VTMK_tmkgettimerl 96
#define VTMK_bcgetmsgtime 97
#define VTMK_mtgetmsgtime 98
#define VTMK_rtgetmsgtime 99

#define VTMK_tmkgethwver 100

#define VTMK_tmkgetevtime 101
#define VTMK_tmkswtimer 102
#define VTMK_tmkgetswtimer 103

#define VTMK_tmktimeout 104

#define VTMK_mrtdefbrcpage 105
#define VTMK_mrtgetbrcpage 106

#define VTMK_mbcinit 107
#define VTMK_mbcpreparex 108
#define VTMK_mbcstartx 109
#define VTMK_mbcalloc 110
#define VTMK_mbcfree 111

//#define VTMK_MT_Start 112
//#define VTMK_MT_GetMessage 113
//#define VTMK_MT_Stop 114

//#define VTMK_tmkwaiteventsflag 115

#define VTMK_mwreset 116
#define VTMK_mwtreset 117
#define VTMK_mwrtreset 118
#define VTMK_mtrtreset 119
#define VTMK_mwtrtreset 120
#define VTMK_mwcdw 121
#define VTMK_mtgetsw2 122
#define VTMK_mtgetswa 123
#define VTMK_mtgetmaxmaxbase 124
#define VTMK_mtdefmaxbase 125

#define VTMK_tmktxrxmode 126

#define VTMK_tmkwaiteventsm 127
//#define VTMK_tmkreadsn_usb 127

#define VTMK_bcgetmsgsw 128
#define VTMK_mtgetmsgsw 129
#define VTMK_rtgetmsgsw 130
#define VTMK_rtgap 131

#define VTMK_rtallocsabuf 132
#define VTMK_rtdefbuf 133
#define VTMK_rtgetbuf 134
#define VTMK_rtdeflink 135
#define VTMK_rtgetlink 136
#define VTMK_rtdeflinkbus 137
#define VTMK_rtgetlinkbus 138

#define VTMK_tmkdac 139

#define VTMK_rtdefsubaddrm 140
#define VTMK_rtgetbase 141
#define VTMK_rtgetbasebus 142

#define VTMK_mtdefmode 143
#define VTMK_mtgetmode 144

#define VTMK_bcdefmode 145
#define VTMK_bcgetmode 146
#define VTMK_bcstartctla 147
#define VTMK_bcstarta 148
#define VTMK_bcdefctla 149
#define VTMK_bcgetctla 150

#define VTMK_mbcpreparea 151
#define VTMK_mbcstarta 152

#define VTMK_tmkoptions 153

#define VTMK_mtfilter 154
#define VTMK_rtlegal 155

#define VTMK_bcclrmsgtime 156
#define VTMK_mtclrmsgtime 157
#define VTMK_rtclrmsgtime 158
#define VTMK_bcclrmsgsw 159
#define VTMK_mtclrmsgsw 160
#define VTMK_rtclrmsgsw 161

#define VTMK_tmkbuserr 162

#define VTMK_tmkinfo 163

#define TMK_IOC_MAXNR 163


#define TMK_IOCtmkconfig _IO(TMK_IOC_MAGIC, VTMK_tmkconfig+TMK_IOC0)
#define TMK_IOCtmkdone _IO(TMK_IOC_MAGIC, VTMK_tmkdone+TMK_IOC0)
#define TMK_IOCtmkgetmaxn _IO(TMK_IOC_MAGIC, VTMK_tmkgetmaxn+TMK_IOC0)
#define TMK_IOCtmkselect _IO(TMK_IOC_MAGIC, VTMK_tmkselect+TMK_IOC0)
#define TMK_IOCtmkselected _IO(TMK_IOC_MAGIC, VTMK_tmkselected+TMK_IOC0)
#define TMK_IOCtmkgetmode _IO(TMK_IOC_MAGIC, VTMK_tmkgetmode+TMK_IOC0)
#define TMK_IOCtmksetcwbits _IO(TMK_IOC_MAGIC, VTMK_tmksetcwbits+TMK_IOC0)
#define TMK_IOCtmkclrcwbits _IO(TMK_IOC_MAGIC, VTMK_tmkclrcwbits+TMK_IOC0)
#define TMK_IOCtmkgetcwbits _IO(TMK_IOC_MAGIC, VTMK_tmkgetcwbits+TMK_IOC0)
#define TMK_IOCtmkwaitevents _IOW(TMK_IOC_MAGIC, VTMK_tmkwaitevents+TMK_IOC0, __u64)
//#define TMK_IOCtmkdefevent _IO(TMK_IOC_MAGIC, VTMK_tmkdefevent+TMK_IOC0)
#define TMK_IOCtmkgetevd _IOR(TMK_IOC_MAGIC, VTMK_tmkgetevd+TMK_IOC0, TTmkEventData)

#define TMK_IOCbcreset _IO(TMK_IOC_MAGIC, VTMK_bcreset+TMK_IOC0)
//#define TMK_IOCbc_def_tldw _IO(TMK_IOC_MAGIC, VTMK_bc_def_tldw+TMK_IOC0)
//#define TMK_IOCbc_enable_di _IO(TMK_IOC_MAGIC, VTMK_bc_enable_di+TMK_IOC0)
//#define TMK_IOCbc_disable_di _IO(TMK_IOC_MAGIC, VTMK_bc_disable_di+TMK_IOC0)
#define TMK_IOCbcdefirqmode _IO(TMK_IOC_MAGIC, VTMK_bcdefirqmode+TMK_IOC0)
#define TMK_IOCbcgetirqmode _IO(TMK_IOC_MAGIC, VTMK_bcgetirqmode+TMK_IOC0)
#define TMK_IOCbcgetmaxbase _IO(TMK_IOC_MAGIC, VTMK_bcgetmaxbase+TMK_IOC0)
#define TMK_IOCbcdefbase _IO(TMK_IOC_MAGIC, VTMK_bcdefbase+TMK_IOC0)
#define TMK_IOCbcgetbase _IO(TMK_IOC_MAGIC, VTMK_bcgetbase+TMK_IOC0)
#define TMK_IOCbcputw _IO(TMK_IOC_MAGIC, VTMK_bcputw+TMK_IOC0)
#define TMK_IOCbcgetw _IO(TMK_IOC_MAGIC, VTMK_bcgetw+TMK_IOC0)
#define TMK_IOCbcgetansw _IOWR(TMK_IOC_MAGIC, VTMK_bcgetansw+TMK_IOC0, __u32)
#define TMK_IOCbcputblk _IOW(TMK_IOC_MAGIC, VTMK_bcputblk+TMK_IOC0, ULONG[2])
#define TMK_IOCbcgetblk _IOW(TMK_IOC_MAGIC, VTMK_bcgetblk+TMK_IOC0, ULONG[2])
#define TMK_IOCbcdefbus _IO(TMK_IOC_MAGIC, VTMK_bcdefbus+TMK_IOC0)
#define TMK_IOCbcgetbus _IO(TMK_IOC_MAGIC, VTMK_bcgetbus+TMK_IOC0)
#define TMK_IOCbcstart _IO(TMK_IOC_MAGIC, VTMK_bcstart+TMK_IOC0)
#define TMK_IOCbcstartx _IO(TMK_IOC_MAGIC, VTMK_bcstartx+TMK_IOC0)
#define TMK_IOCbcdeflink _IO(TMK_IOC_MAGIC, VTMK_bcdeflink+TMK_IOC0)
#define TMK_IOCbcgetlink _IOR(TMK_IOC_MAGIC, VTMK_bcgetlink+TMK_IOC0, __u32)
#define TMK_IOCbcstop _IO(TMK_IOC_MAGIC, VTMK_bcstop+TMK_IOC0)
#define TMK_IOCbcgetstate _IOR(TMK_IOC_MAGIC, VTMK_bcgetstate+TMK_IOC0, __u32)

#define TMK_IOCrtreset _IO(TMK_IOC_MAGIC, VTMK_rtreset+TMK_IOC0)
#define TMK_IOCrtdefirqmode _IO(TMK_IOC_MAGIC, VTMK_rtdefirqmode+TMK_IOC0)
#define TMK_IOCrtgetirqmode _IO(TMK_IOC_MAGIC, VTMK_rtgetirqmode+TMK_IOC0)
#define TMK_IOCrtdefmode _IO(TMK_IOC_MAGIC, VTMK_rtdefmode+TMK_IOC0)
#define TMK_IOCrtgetmode _IO(TMK_IOC_MAGIC, VTMK_rtgetmode+TMK_IOC0)
#define TMK_IOCrtgetmaxpage _IO(TMK_IOC_MAGIC, VTMK_rtgetmaxpage+TMK_IOC0)
#define TMK_IOCrtdefpage _IO(TMK_IOC_MAGIC, VTMK_rtdefpage+TMK_IOC0)
#define TMK_IOCrtgetpage _IO(TMK_IOC_MAGIC, VTMK_rtgetpage+TMK_IOC0)
#define TMK_IOCrtdefpagepc _IO(TMK_IOC_MAGIC, VTMK_rtdefpagepc+TMK_IOC0)
#define TMK_IOCrtdefpagebus _IO(TMK_IOC_MAGIC, VTMK_rtdefpagebus+TMK_IOC0)
#define TMK_IOCrtgetpagepc _IO(TMK_IOC_MAGIC, VTMK_rtgetpagepc+TMK_IOC0)
#define TMK_IOCrtgetpagebus _IO(TMK_IOC_MAGIC, VTMK_rtgetpagebus+TMK_IOC0)
#define TMK_IOCrtdefaddress _IO(TMK_IOC_MAGIC, VTMK_rtdefaddress+TMK_IOC0)
#define TMK_IOCrtgetaddress _IO(TMK_IOC_MAGIC, VTMK_rtgetaddress+TMK_IOC0)
#define TMK_IOCrtdefsubaddr _IO(TMK_IOC_MAGIC, VTMK_rtdefsubaddr+TMK_IOC0)
#define TMK_IOCrtgetsubaddr _IO(TMK_IOC_MAGIC, VTMK_rtgetsubaddr+TMK_IOC0)
#define TMK_IOCrtputw _IO(TMK_IOC_MAGIC, VTMK_rtputw+TMK_IOC0)
#define TMK_IOCrtgetw _IO(TMK_IOC_MAGIC, VTMK_rtgetw+TMK_IOC0)
#define TMK_IOCrtputblk _IOW(TMK_IOC_MAGIC, VTMK_rtputblk+TMK_IOC0, ULONG[2])
#define TMK_IOCrtgetblk _IOW(TMK_IOC_MAGIC, VTMK_rtgetblk+TMK_IOC0, ULONG[2])
#define TMK_IOCrtsetanswbits _IO(TMK_IOC_MAGIC, VTMK_rtsetanswbits+TMK_IOC0)
#define TMK_IOCrtclranswbits _IO(TMK_IOC_MAGIC, VTMK_rtclranswbits+TMK_IOC0)
#define TMK_IOCrtgetanswbits _IO(TMK_IOC_MAGIC, VTMK_rtgetanswbits+TMK_IOC0)
#define TMK_IOCrtgetflags _IOW(TMK_IOC_MAGIC, VTMK_rtgetflags+TMK_IOC0, ULONG[2])
#define TMK_IOCrtputflags _IOW(TMK_IOC_MAGIC, VTMK_rtputflags+TMK_IOC0, ULONG[2])
#define TMK_IOCrtsetflag _IO(TMK_IOC_MAGIC, VTMK_rtsetflag+TMK_IOC0)
#define TMK_IOCrtclrflag _IO(TMK_IOC_MAGIC, VTMK_rtclrflag+TMK_IOC0)
#define TMK_IOCrtgetflag _IO(TMK_IOC_MAGIC, VTMK_rtgetflag+TMK_IOC0)
#define TMK_IOCrtgetstate _IO(TMK_IOC_MAGIC, VTMK_rtgetstate+TMK_IOC0)
#define TMK_IOCrtbusy _IO(TMK_IOC_MAGIC, VTMK_rtbusy+TMK_IOC0)
#define TMK_IOCrtlock _IO(TMK_IOC_MAGIC, VTMK_rtlock+TMK_IOC0)
#define TMK_IOCrtunlock _IO(TMK_IOC_MAGIC, VTMK_rtunlock+TMK_IOC0)
#define TMK_IOCrtgetcmddata _IO(TMK_IOC_MAGIC, VTMK_rtgetcmddata+TMK_IOC0)
#define TMK_IOCrtputcmddata _IO(TMK_IOC_MAGIC, VTMK_rtputcmddata+TMK_IOC0)

#define TMK_IOCmtreset _IO(TMK_IOC_MAGIC, VTMK_mtreset+TMK_IOC0)
#define TMK_IOCmtdefirqmode _IO(TMK_IOC_MAGIC, VTMK_mtdefirqmode+TMK_IOC0)
#define TMK_IOCmtgetirqmode _IO(TMK_IOC_MAGIC, VTMK_mtgetirqmode+TMK_IOC0)
#define TMK_IOCmtgetmaxbase _IO(TMK_IOC_MAGIC, VTMK_mtgetmaxbase+TMK_IOC0)
#define TMK_IOCmtdefbase _IO(TMK_IOC_MAGIC, VTMK_mtdefbase+TMK_IOC0)
#define TMK_IOCmtgetbase _IO(TMK_IOC_MAGIC, VTMK_mtgetbase+TMK_IOC0)
#define TMK_IOCmtputw _IO(TMK_IOC_MAGIC, VTMK_mtputw+TMK_IOC0)
#define TMK_IOCmtgetw _IO(TMK_IOC_MAGIC, VTMK_mtgetw+TMK_IOC0)
#define TMK_IOCmtgetsw _IO(TMK_IOC_MAGIC, VTMK_mtgetsw+TMK_IOC0)
#define TMK_IOCmtputblk _IOW(TMK_IOC_MAGIC, VTMK_mtputblk+TMK_IOC0, ULONG[2])
#define TMK_IOCmtgetblk _IOW(TMK_IOC_MAGIC, VTMK_mtgetblk+TMK_IOC0, ULONG[2])
#define TMK_IOCmtstartx _IO(TMK_IOC_MAGIC, VTMK_mtstartx+TMK_IOC0)
#define TMK_IOCmtdeflink _IO(TMK_IOC_MAGIC, VTMK_mtdeflink+TMK_IOC0)
#define TMK_IOCmtgetlink _IOR(TMK_IOC_MAGIC, VTMK_mtgetlink+TMK_IOC0, __u32)
#define TMK_IOCmtstop _IO(TMK_IOC_MAGIC, VTMK_mtstop+TMK_IOC0)
#define TMK_IOCmtgetstate _IOR(TMK_IOC_MAGIC, VTMK_mtgetstate+TMK_IOC0, __u32)

#define TMK_IOCtmkgetinfo _IOR(TMK_IOC_MAGIC, VTMK_tmkgetinfo+TMK_IOC0, TTmkConfigData)
#define TMK_IOCGetVersion _IO(TMK_IOC_MAGIC, VTMK_GetVersion+TMK_IOC0)

#define TMK_IOCrtenable _IO(TMK_IOC_MAGIC, VTMK_rtenable+TMK_IOC0)

#ifdef _TMK1553B_MRT
#define TMK_IOCmrtgetmaxn _IO(TMK_IOC_MAGIC, VTMK_mrtgetmaxn+TMK_IOC0)
#define TMK_IOCmrtconfig _IO(TMK_IOC_MAGIC, VTMK_mrtconfig+TMK_IOC0)
#define TMK_IOCmrtselected _IO(TMK_IOC_MAGIC, VTMK_mrtselected+TMK_IOC0)
#define TMK_IOCmrtgetstate _IO(TMK_IOC_MAGIC, VTMK_mrtgetstate+TMK_IOC0)
#define TMK_IOCmrtdefbrcsubaddr0 _IO(TMK_IOC_MAGIC, VTMK_mrtdefbrcsubaddr0+TMK_IOC0)
#define TMK_IOCmrtreset _IO(TMK_IOC_MAGIC, VTMK_mrtreset+TMK_IOC0)
#endif //def _TMK1553B_MRT

#define TMK_IOCtmktimer _IO(TMK_IOC_MAGIC, VTMK_tmktimer+TMK_IOC0)
#define TMK_IOCtmkgettimer _IOR(TMK_IOC_MAGIC, VTMK_tmkgettimer+TMK_IOC0, __u32)
#define TMK_IOCtmkgettimerl _IO(TMK_IOC_MAGIC, VTMK_tmkgettimerl+TMK_IOC0)
#define TMK_IOCbcgetmsgtime _IOR(TMK_IOC_MAGIC, VTMK_bcgetmsgtime+TMK_IOC0, __u32)
#define TMK_IOCmtgetmsgtime _IOR(TMK_IOC_MAGIC, VTMK_mtgetmsgtime+TMK_IOC0, __u32)
#define TMK_IOCrtgetmsgtime _IOR(TMK_IOC_MAGIC, VTMK_rtgetmsgtime+TMK_IOC0, __u32)

#define TMK_IOCtmkgethwver _IO(TMK_IOC_MAGIC, VTMK_tmkgethwver+TMK_IOC0)

#define TMK_IOCtmkgetevtime _IOR(TMK_IOC_MAGIC, VTMK_tmkgetevtime+TMK_IOC0, __u32)
#define TMK_IOCtmkswtimer _IO(TMK_IOC_MAGIC, VTMK_tmkswtimer+TMK_IOC0)
#define TMK_IOCtmkgetswtimer _IOR(TMK_IOC_MAGIC, VTMK_tmkgetswtimer+TMK_IOC0, __u32)

#define TMK_IOCtmktimeout _IO(TMK_IOC_MAGIC, VTMK_tmktimeout+TMK_IOC0)

#ifdef _TMK1553B_MRT
#define TMK_IOCmrtdefbrcpage _IO(TMK_IOC_MAGIC, VTMK_mrtdefbrcpage+TMK_IOC0)
#define TMK_IOCmrtgetbrcpage _IO(TMK_IOC_MAGIC, VTMK_mrtgetbrcpage+TMK_IOC0)
#endif //def _TMK1553B_MRT

#define TMK_IOCmbcinit _IO(TMK_IOC_MAGIC, VTMK_mbcinit+TMK_IOC0)
#define TMK_IOCmbcpreparex _IOW(TMK_IOC_MAGIC, VTMK_mbcpreparex+TMK_IOC0, __u32[2])
#define TMK_IOCmbcstartx _IO(TMK_IOC_MAGIC, VTMK_mbcstartx+TMK_IOC0)
#define TMK_IOCmbcalloc _IO(TMK_IOC_MAGIC, VTMK_mbcalloc+TMK_IOC0)
#define TMK_IOCmbcfree _IO(TMK_IOC_MAGIC, VTMK_mbcfree+TMK_IOC0)

#define TMK_IOCmwreset _IO(TMK_IOC_MAGIC, VTMK_mwreset+TMK_IOC0)
#define TMK_IOCmwtreset _IO(TMK_IOC_MAGIC, VTMK_mwtreset+TMK_IOC0)
#define TMK_IOCmwrtreset _IO(TMK_IOC_MAGIC, VTMK_mwrtreset+TMK_IOC0)
#define TMK_IOCmtrtreset _IO(TMK_IOC_MAGIC, VTMK_mtrtreset+TMK_IOC0)
#define TMK_IOCmwtrtreset _IO(TMK_IOC_MAGIC, VTMK_mwtrtreset+TMK_IOC0)
#define TMK_IOCmwcdw _IO(TMK_IOC_MAGIC, VTMK_mwcdw+TMK_IOC0)
#define TMK_IOCmtgetsw2 _IOR(TMK_IOC_MAGIC, VTMK_mtgetsw2+TMK_IOC0, __u32)
#define TMK_IOCmtgetswa _IO(TMK_IOC_MAGIC, VTMK_mtgetswa+TMK_IOC0)
#define TMK_IOCmtgetmaxmaxbase _IO(TMK_IOC_MAGIC, VTMK_mtgetmaxmaxbase+TMK_IOC0)
#define TMK_IOCmtdefmaxbase _IO(TMK_IOC_MAGIC, VTMK_mtdefmaxbase+TMK_IOC0)

#define TMK_IOCtmktxrxmode _IO(TMK_IOC_MAGIC, VTMK_tmktxrxmode+TMK_IOC0)

#define TMK_IOCtmkwaiteventsm _IOW(TMK_IOC_MAGIC, VTMK_tmkwaiteventsm+TMK_IOC0, ULONG[2])

#define TMK_IOCbcgetmsgsw _IO(TMK_IOC_MAGIC, VTMK_bcgetmsgsw+TMK_IOC0)
#define TMK_IOCmtgetmsgsw _IO(TMK_IOC_MAGIC, VTMK_mtgetmsgsw+TMK_IOC0)
#define TMK_IOCrtgetmsgsw _IO(TMK_IOC_MAGIC, VTMK_rtgetmsgsw+TMK_IOC0)

#define TMK_IOCrtgap _IO(TMK_IOC_MAGIC, VTMK_rtgap+TMK_IOC0)

#define TMK_IOCrtallocsabuf _IO(TMK_IOC_MAGIC, VTMK_rtallocsabuf+TMK_IOC0)
#define TMK_IOCrtdefbuf _IO(TMK_IOC_MAGIC, VTMK_rtdefbuf+TMK_IOC0)
#define TMK_IOCrtgetbuf _IO(TMK_IOC_MAGIC, VTMK_rtgetbuf+TMK_IOC0)
#define TMK_IOCrtdeflink _IO(TMK_IOC_MAGIC, VTMK_rtdeflink+TMK_IOC0)
#define TMK_IOCrtgetlink _IO(TMK_IOC_MAGIC, VTMK_rtgetlink+TMK_IOC0)
#define TMK_IOCrtdeflinkbus _IO(TMK_IOC_MAGIC, VTMK_rtdeflinkbus+TMK_IOC0)
#define TMK_IOCrtgetlinkbus _IO(TMK_IOC_MAGIC, VTMK_rtgetlinkbus+TMK_IOC0)

#define TMK_IOCtmkdac _IO(TMK_IOC_MAGIC, VTMK_tmkdac+TMK_IOC0)

#define TMK_IOCrtdefsubaddrm _IO(TMK_IOC_MAGIC, VTMK_rtdefsubaddrm+TMK_IOC0)
#define TMK_IOCrtgetbase _IO(TMK_IOC_MAGIC, VTMK_rtgetbase+TMK_IOC0)
#define TMK_IOCrtgetbasebus _IO(TMK_IOC_MAGIC, VTMK_rtgetbasebus+TMK_IOC0)

#define TMK_IOCmtdefmode _IO(TMK_IOC_MAGIC, VTMK_mtdefmode+TMK_IOC0)
#define TMK_IOCmtgetmode _IO(TMK_IOC_MAGIC, VTMK_mtgetmode+TMK_IOC0)

#define TMK_IOCbcdefmode _IO(TMK_IOC_MAGIC, VTMK_bcdefmode+TMK_IOC0)
#define TMK_IOCbcgetmode _IO(TMK_IOC_MAGIC, VTMK_bcgetmode+TMK_IOC0)
#define TMK_IOCbcstartctla _IOW(TMK_IOC_MAGIC, VTMK_bcstartctla+TMK_IOC0, __u32[2])
#define TMK_IOCbcstarta _IO(TMK_IOC_MAGIC, VTMK_bcstarta+TMK_IOC0)
#define TMK_IOCbcdefctla _IOW(TMK_IOC_MAGIC, VTMK_bcdefctla+TMK_IOC0, __u32[2])
#define TMK_IOCbcgetctla _IO(TMK_IOC_MAGIC, VTMK_bcgetctla+TMK_IOC0)

#define TMK_IOCmbcpreparea _IO(TMK_IOC_MAGIC, VTMK_mbcpreparea+TMK_IOC0)
#define TMK_IOCmbcstarta _IO(TMK_IOC_MAGIC, VTMK_mbcstarta+TMK_IOC0)

#define TMK_IOCtmkoptions _IOWR(TMK_IOC_MAGIC, VTMK_tmkoptions+TMK_IOC0, __u32)

#define TMK_IOCmtfilter _IOW(TMK_IOC_MAGIC, VTMK_mtfilter+TMK_IOC0, __u32[2])
#define TMK_IOCrtlegal _IOW(TMK_IOC_MAGIC, VTMK_rtlegal+TMK_IOC0, __u32[2])

#define TMK_IOCbcclrmsgtime _IO(TMK_IOC_MAGIC, VTMK_bcclrmsgtime+TMK_IOC0)
#define TMK_IOCmtclrmsgtime _IO(TMK_IOC_MAGIC, VTMK_mtclrmsgtime+TMK_IOC0)
#define TMK_IOCrtclrmsgtime _IO(TMK_IOC_MAGIC, VTMK_rtclrmsgtime+TMK_IOC0)
#define TMK_IOCbcclrmsgsw _IO(TMK_IOC_MAGIC, VTMK_bcclrmsgsw+TMK_IOC0)
#define TMK_IOCmtclrmsgsw _IO(TMK_IOC_MAGIC, VTMK_mtclrmsgsw+TMK_IOC0)
#define TMK_IOCrtclrmsgsw _IO(TMK_IOC_MAGIC, VTMK_rtclrmsgsw+TMK_IOC0)

#define TMK_IOCtmkbuserr _IO(TMK_IOC_MAGIC, VTMK_tmkbuserr+TMK_IOC0)

#define TMK_IOCtmkinfo _IOWR(TMK_IOC_MAGIC, VTMK_tmkinfo+TMK_IOC0, __u32)

#ifndef _TMK1553B_
#ifdef USE_TMK_ERROR
extern int TMK_THREADLOCAL tmkError;
#endif
#endif

/*****************Definitions***************************/

int TmkOpenNameHandle(char *pszDrvName, HANDLE *_phVTMK4VxD);
int TmkOpenName(char *pszDrvName);
int TmkOpen(void);
void TmkCloseHandle(HANDLE *_phVTMK4VxD);
void TmkClose(void);
HANDLE TmkGetHandle(void);

int tmkgetmaxn(void);
int tmkconfig(int tmkNumber);
int tmkdone(int tmkNumber);
int tmkselect(int tmkNumber);
int tmkselected(void);
TMK_DATA_RET tmkgetmode(void);
void tmksetcwbits(TMK_DATA tmkSetControl);
void tmkclrcwbits(TMK_DATA tmkClrControl);
TMK_DATA_RET tmkgetcwbits(void);

/*
void bcdefintnorm(void (* UserNormBC)(TMK_DATA, TMK_DATA, TMK_DATA));
void bcdefintexc(void (* UserExcBC)(TMK_DATA, TMK_DATA, TMK_DATA));
void bcdefintx(void (* UserXBC)(TMK_DATA, TMK_DATA));
void bcdefintsig(void (* UserSigBC)(TMK_DATA));
*/
int bcreset(void);
//void bc_def_tldw(TMK_DATA wTLDW);
//void bc_enable_di(void);
//void bc_disable_di(void);
int bcdefirqmode(TMK_DATA bcIrqMode);
TMK_DATA_RET bcgetirqmode(void);
TMK_DATA_RET bcgetmaxbase(void);
int bcdefbase(TMK_DATA bcBasePC);
TMK_DATA_RET bcgetbase(void);
void bcputw(TMK_DATA bcAddr, TMK_DATA bcData);
TMK_DATA_RET bcgetw(TMK_DATA bcAddr);
DWORD bcgetansw(TMK_DATA bcCtrlCode);
void bcputblk(TMK_DATA bcAddr, void *pcBuffer, TMK_DATA cwLength);
void bcgetblk(TMK_DATA bcAddr, void *pcBuffer, TMK_DATA cwLength);
int bcdefbus(TMK_DATA bcBus);
TMK_DATA_RET bcgetbus(void);
int bcstart(TMK_DATA bcBase, TMK_DATA bcCtrlCode);
int bcstartx(TMK_DATA bcBase, TMK_DATA bcCtrlCode);
int bcdeflink(TMK_DATA bcBase, TMK_DATA bcCtrlCode);
DWORD bcgetlink(void);
TMK_DATA_RET bcstop(void);
DWORD bcgetstate(void);

/*
void rtdefintcmd(void (* UserCmdRT)(TMK_DATA));
void rtdefinterr(void (* UserErrRT)(TMK_DATA));
void rtdefintdata(void (* UserDataRT)(TMK_DATA));
*/
int rtreset(void);
int rtdefirqmode(TMK_DATA rtIrqMode);
TMK_DATA_RET rtgetirqmode(void);
int rtdefmode(TMK_DATA rtMode);
TMK_DATA_RET rtgetmode(void);
TMK_DATA_RET rtgetmaxpage(void);
int rtdefpage(TMK_DATA rtPage);
TMK_DATA_RET rtgetpage(void);
int rtdefpagepc(TMK_DATA rtPagePC);
int rtdefpagebus(TMK_DATA rtPageBus);
TMK_DATA_RET rtgetpagepc(void);
TMK_DATA_RET rtgetpagebus(void);
int rtdefaddress(TMK_DATA rtAddress);
TMK_DATA_RET rtgetaddress(void);
void rtdefsubaddr(TMK_DATA rtDir, TMK_DATA rtSubAddr);
TMK_DATA_RET rtgetsubaddr(void);
void rtputw(TMK_DATA rtAddr, TMK_DATA rtData);
TMK_DATA_RET rtgetw(TMK_DATA rtAddr);
void rtputblk(TMK_DATA rtAddr, void *pcBuffer, TMK_DATA cwLength);
void rtgetblk(TMK_DATA rtAddr, void *pcBuffer, TMK_DATA cwLength);
void rtsetanswbits(TMK_DATA rtSetControl);
void rtclranswbits(TMK_DATA rtClrControl);
TMK_DATA_RET rtgetanswbits(void);
void rtgetflags(void *pcBuffer, TMK_DATA rtDir, TMK_DATA rtFlagMin, TMK_DATA rtFlagMax);
void rtputflags(void *pcBuffer, TMK_DATA rtDir, TMK_DATA rtFlagMin, TMK_DATA rtFlagMax);
void rtsetflag(void);
void rtclrflag(void);
TMK_DATA_RET rtgetflag(TMK_DATA rtDir, TMK_DATA rtSubAddr);
TMK_DATA_RET rtgetstate(void);
TMK_DATA_RET rtbusy(void);
void rtlock(TMK_DATA rtDir, TMK_DATA rtSubAddr);
void rtunlock(void);
TMK_DATA_RET rtgetcmddata(TMK_DATA rtBusCommand);
void rtputcmddata(TMK_DATA rtBusCommand, TMK_DATA rtData);

/*
void mtdefintx(void (* UserIntXMT)(TMK_DATA, TMK_DATA));
void mtdefintsig(void (* UserSigMT)(TMK_DATA));
*/
int mtreset(void);
int mtdefirqmode(TMK_DATA mtIrqMode);
TMK_DATA_RET mtgetirqmode(void);
TMK_DATA_RET mtgetmaxbase(void);
int mtdefbase(TMK_DATA mtBasePC);
TMK_DATA_RET mtgetbase(void);
void mtputw(TMK_DATA mtAddr, TMK_DATA mtData);
TMK_DATA_RET mtgetw(TMK_DATA mtAddr);
TMK_DATA_RET mtgetsw(void);
void mtputblk(TMK_DATA mtAddr, void *pcBuffer, TMK_DATA cwLength);
void mtgetblk(TMK_DATA mtAddr, void *pcBuffer, TMK_DATA cwLength);
int mtstartx(TMK_DATA mtBase, TMK_DATA mtCtrlCode);
int mtdeflink(TMK_DATA mtBase, TMK_DATA mtCtrlCode);
DWORD mtgetlink(void);
TMK_DATA_RET mtstop(void);
DWORD mtgetstate(void);
TMK_DATA_RET rtenable(TMK_DATA rtEnable);
#ifdef _TMK1553B_MRT
int mrtgetmaxn(void);
DWORD mrtconfig(int tmkNumber);
int mrtselected(void);
TMK_DATA_RET mrtgetstate(void);
void mrtdefbrcsubaddr0(void);
int mrtreset(void);
#endif //def_TMK1553B_MRT

TMK_DATA_RET tmktimer(TMK_DATA tmkTimerCtrl);
DWORD tmkgettimer(void);
TMK_DATA_RET tmkgettimerl(void);
DWORD bcgetmsgtime(void);
DWORD mtgetmsgtime(void);
DWORD rtgetmsgtime(void);

TMK_DATA_RET tmkgethwver(void);

DWORD tmkgetevtime(void);
TMK_DATA_RET tmkswtimer(TMK_DATA tmkSwTimerCtrl);
DWORD tmkgetswtimer(void);

TMK_DATA_RET tmktimeout(TMK_DATA tmkTimeOut);

#ifdef _TMK1553B_MRT
int mrtdefbrcpage(TMK_DATA mrtBrcPage);
TMK_DATA_RET mrtgetbrcpage(void);
#endif //def_TMK1553B_MRT

int mbcinit(TMK_DATA mbcId);
int mbcpreparex(TMK_DATA mbcId, TMK_DATA bcBase, TMK_DATA bcCtrlCode, TMK_DATA mbcDelay);
int mbcstartx(TMK_DATA mbcId);
TMK_DATA_RET mbcalloc(void);
int mbcfree(TMK_DATA mbcId);

void tmkgetevd(TTmkEventData *pEvD);
int tmkwaitevents(int maskEvents, int fWait);
int tmkwaiteventsm(int cEvents, void *pEvents, int fWait);

TMK_DATA_RET bcgetmsgsw(void);
TMK_DATA_RET mtgetmsgsw(void);
TMK_DATA_RET rtgetmsgsw(void);

TMK_DATA_RET rtgap(TMK_DATA rtGap);

TMK_DATA_RET rtallocsabuf(TMK_DATA rtBufModeSA, TMK_DATA rtBufSize);
int rtdefbuf(TMK_DATA rtBuf);
TMK_DATA_RET rtgetbuf(void);
int rtdeflink(TMK_DATA rtLink);
TMK_DATA_RET rtgetlink(void);
int rtdeflinkbus(TMK_DATA rtLinkBus);
TMK_DATA_RET rtgetlinkbus(void);

int mwreset(void);
int mwtreset(void);
TMK_DATA_RET mwcdw(TMK_DATA mwCdData, TMK_DATA mwCdCtrl);
DWORD mtgetsw2(void);
TMK_DATA_RET mtgetswa(void);

TMK_DATA_RET tmktxrxmode(TMK_DATA tmkTxRxMode);

TMK_DATA_RET tmkdac(TMK_DATA tmkDac);

void rtdefsubaddrm(TMK_DATA rtDir, TMK_DATA rtSubAddr);
TMK_DATA_RET rtgetbase(void);
TMK_DATA_RET rtgetbasebus(void);

int mtdefmode(TMK_DATA mtMode);
TMK_DATA_RET mtgetmode(void);

int bcdefmode(TMK_DATA bcMode);
TMK_DATA_RET bcgetmode(void);
int bcstartctla(TMK_DATA bcBase, TMK_DATA bcCtrlA, TMK_DATA bcTimeA, TMK_DATA bcLinkA);
int bcstarta(TMK_DATA bcBase);
int bcdefctla(TMK_DATA bcCTLA, TMK_DATA bcCtrlA, TMK_DATA bcTimeA, TMK_DATA bcLinkA);
TMK_DATA_RET bcgetctla(TMK_DATA bcCTLA);

int mbcpreparea(TMK_DATA mbcId, TMK_DATA bcBase);
int mbcstarta(TMK_DATA mbcId);

DWORD tmkoptions(DWORD tmkOptions);

TMK_DATA_RET mtfilter(TMK_DATA mtFilterCtrl, TMK_DATA mtFilter1, TMK_DATA mtFilter2);
TMK_DATA_RET rtlegal(TMK_DATA rtLegalCtrl, TMK_DATA rtLegal1, TMK_DATA rtLegal2);

void bcclrmsgtime(void);
void mtclrmsgtime(void);
void rtclrmsgtime(void);
void bcclrmsgsw(void);
void mtclrmsgsw(void);
void rtclrmsgsw(void);

int mwrtreset(void);
int mtrtreset(void);
int mwtrtreset(void);
TMK_DATA_RET mtgetmaxmaxbase(void);
int mtdefmaxbase(TMK_DATA mtMaxBase);

//#ifdef _TMK1553B_MRT
int tmkbuserr(TMK_DATA tmkBusErr);
//#endif //def_TMK1553B_MRT

DWORD tmkinfo(TMK_DATA tmkData, TMK_DATA tmkInfo);

//#endif
