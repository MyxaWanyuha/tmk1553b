
/****************************************************************************/
/*      TMKNLL.H v8.06.1. ELCUS, 1995,2024                                  */
/*      Interface to the driver TMKNLL v8.06.1.                             */
/****************************************************************************/

#ifndef _TMKNLLX_
#define _TMKNLLX_

typedef unsigned char U08;
#if defined(DOS) && !defined(DOS32)
typedef unsigned U16;
#else
typedef unsigned short U16;
#endif
typedef unsigned long U32;

#ifndef RETIR
#define RETIR void
#endif

#ifndef TYPIR
#define TYPIR interrupt
#endif

#ifndef FARFN
#define FARFN
#endif
#ifndef FARVR
#define FARVR
#endif
#ifndef FARIR
#define FARIR
#endif
#ifndef FARDT
#define FARDT
#endif

#define MIN_TMK_TYPE 2
#define MAX_TMK_TYPE 16

#define RTMK         0
#define RTMK1        1
#define TMK400       2
#define TMKMPC       3
#define RTMK400      4
#define TMKX         5
#define TMKXI        6
#define MRTX         7
#define MRTXI        8
#define TA           9
#define TAI          10
#define MRTA         11
#define MRTAI        12
#define TSTX         13
#define TSTXI        14
#define TSTA         15
#define TSTAI        16

#define ALL_TMKS 0x7FFF
#define ALL_TMKS_OLD 0x00FF

#define GET_TMK_OPTIONS     0x4L
#if NRT > 0
#define MRT_READ_BRC_DATA   0x1L
#define MRT_WRITE_BRC_DATA  0x2L
#endif //NRT
#define BC_STARTA_INTA      0x8L
#define MTRT_ALLOW_ANOMALY 0x10L
#define RT_USE_ADDR_PARITY 0x20L
#define RT_SHOW_BRC_STATE  0x40L

#define ADDR_PARITY_ON 0x0100

#define TMK_INT_SAVED        0x0001
#define TMK_INT_MORE         0x8000
#define TMK_INT_OTHER        0x7FF4

#define TMK_INT_DRV_OVF      0x0002
#define TMK_INT_DRV_BUF_SKIP 0x0004
#define TMK_INT_TIMER        0x4000
#define TMK_INT_BUSJAM       0x2000
#define TMK_INT_FIFO_OVF     0x0400
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

#define ERAO_MASK 0x0001
#define MEO_MASK 0x0002
#define IB_MASK 0x0004
#define TO_MASK 0x0008
#define EM_MASK 0x0010
#define EBC_MASK 0x0020
#define DI_MASK 0x0040
#define ELN_MASK 0x0080

#define G1_MASK 0x1000
#define G2_MASK 0x2000

#define S_ERAO_MASK 0x0001
#define S_MEO_MASK 0x0002
#define S_IB_MASK 0x0004
#define S_TO_MASK 0x0008
#define S_EM_MASK 0x0010
#define S_EBC_MASK 0x0020
#define S_DI_MASK 0x0040
#define S_ELN_MASK 0x0080

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

#ifdef DOS
#define CW(ADDR,DIR,SUBADDR,NWORDS) ((U16)(((ADDR)<<11)|(DIR)|((SUBADDR)<<5)|((NWORDS)&0x1F)))
#define CWM(ADDR,COMMAND) ((U16)(((ADDR)<<11)|(CI_MASK)|(COMMAND)))
#define CWMC(ADDR,CI,COMMAND) ((U16)(((ADDR)<<11)|((CI)&0x03E0)|(COMMAND)))
#endif

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
#define CMD_SELECTED_TRANSMITTER_SHUTDOWN 0x014
#define CMD_OVERRIDE_SELECTED_TRANSMITTER_SHUTDOWN 0x015

#define TA_SA_CMD      0x001F
#define TA_SA_CMD_VECW 0x0420
#define TA_SA_CMD_BITW 0x043F

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

#if NRT > 0
#define BUS_ERR_CLR  0x0000
#define BUS_ERR_SET  0x2000
#define BUS_ERR_ONCE 0x0001
#endif

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

#ifndef TMK_ERROR_0
#define TMK_ERROR_0 0
#endif

#define TMK_BAD_TYPE     (1 + TMK_ERROR_0)
#define TMK_BAD_IRQ      (2 + TMK_ERROR_0)
#define TMK_BAD_NUMBER   (3 + TMK_ERROR_0)
#define BC_BAD_BUS       (4 + TMK_ERROR_0)
#define BC_BAD_BASE      (5 + TMK_ERROR_0)
#define BC_BAD_LEN       (6 + TMK_ERROR_0)
#define RT_BAD_PAGE      (7 + TMK_ERROR_0)
#define RT_BAD_LEN       (8 + TMK_ERROR_0)
#define RT_BAD_ADDRESS   (9 + TMK_ERROR_0)
#define RT_BAD_FUNC      (10 + TMK_ERROR_0)
#define BC_BAD_FUNC      (11 + TMK_ERROR_0)
#define TMK_BAD_FUNC     (12 + TMK_ERROR_0)
#define TMK_PCI_ERROR    (13 + TMK_ERROR_0)
#define RT_BAD_BUF       (14 + TMK_ERROR_0)

#define TMK_MAX_ERROR    (14 + TMK_ERROR_0)

#ifdef DOS32
#define TMK_DPMI_ERROR (100 + TMK_ERROR_0)
#endif //DOS32

#ifndef __EXTERN
#ifdef __cplusplus
#define __EXTERN extern
#else
#define __EXTERN
#endif
#endif

#ifdef __cplusplus
//__EXTERN "C" {
#endif

#ifdef QNX4
__EXTERN U16 __tmkIrqPort[NTMK];
__EXTERN U16 __tmkIrqBit[NTMK];
#endif

#ifndef DYNAMIC_TMKNUM
extern FARVR int tmkError;
#define int_TMKNUM void
#define int_TMKNUM__
#else
extern FARVR int tmkError[NTMK+NRT];
#define int_TMKNUM int tmkNumber
#define int_TMKNUM__ int tmkNumber,
#endif

#ifdef DOS32
__EXTERN void FARFN tmkOpenDPMI(void);
#endif

__EXTERN U16 FARFN tmkiodelay(int_TMKNUM__ U16 IODelay);
__EXTERN int FARFN tmkgetmaxn(void);
#if defined(DOS) || defined(QNX4)
#ifdef QNX4VME
__EXTERN int FARFN tmkconfig(int tmkNumber, U16 tmkType, U16 tmkPorts1, U16 tmkPorts2, U08 tmkIrq1, U08 tmkIrq2, char *pTmkName);
#else
__EXTERN int FARFN tmkconfig(int tmkNumber, U16 tmkType, U16 tmkPorts1, U16 tmkPorts2, U08 tmkIrq1, U08 tmkIrq2);
#endif //def QNX4VME
#else
__EXTERN int FARFN tmkconfig(int tmkNumber, U16 tmkType, U16 tmkPorts1, U16 tmkPorts2, U08 tmkIrq1, U08 tmkIrq2, U16 tmkDID, U16 tmkSubDID);
#endif
__EXTERN int FARFN tmkdone(int tmkNumber);
#ifdef DOS
#if NRT > 0
__EXTERN U32 FARFN mrtconfig(int tmkNumber, U16 tmkType, U16 tmkPorts1, U16 tmkPorts2, U08 tmkIrq1, U08 tmkIrq2);
#endif //NRT
__EXTERN void FARFN tmkdefirq(int_TMKNUM__ U16 pcIrq);
__EXTERN void FARFN tmkundefirq(int_TMKNUM__ U16 pcIrq);
__EXTERN void FARFN tmkdeferrors(int_TMKNUM__ void (FARIR* UserErrors)());
#endif
__EXTERN int FARFN tmkselect(int tmkNumber);
#ifndef DYNAMIC_TMKNUM
__EXTERN int FARFN tmkselected(void);
#endif
__EXTERN U16 FARFN tmkgetmode(int_TMKNUM);
__EXTERN void FARFN tmksave(int_TMKNUM);
__EXTERN void FARFN tmkrestore(int_TMKNUM);
__EXTERN void FARFN tmksetcwbits(int_TMKNUM__ U16 tmkSetControl);
__EXTERN void FARFN tmkclrcwbits(int_TMKNUM__ U16 tmkClrControl);
__EXTERN U16 FARFN tmkgetcwbits(int_TMKNUM);

#if defined(DOS) || defined(QNX4INT)
__EXTERN void FARFN bcdefintnorm(int_TMKNUM__ RETIR (FARIR* UserNormBC)(U16, U16, U16));
__EXTERN void FARFN bcdefintexc(int_TMKNUM__ RETIR (FARIR* UserExcBC)(U16, U16, U16));
__EXTERN void FARFN bcdefintx(int_TMKNUM__ RETIR (FARIR* UserXBC)(U16, U16));
__EXTERN void FARFN bcdefintsig(int_TMKNUM__ RETIR (FARIR* UserSigBC)(U16));
#endif
__EXTERN int FARFN bcreset(int_TMKNUM);
__EXTERN void FARFN bcrestore(int_TMKNUM);
__EXTERN void FARFN bc_def_tldw(U16 wTLDW);
__EXTERN void FARFN bc_enable_di(void);
__EXTERN void FARFN bc_disable_di(void);
__EXTERN int FARFN bcdefirqmode(int_TMKNUM__ U16 bcIrqMode);
__EXTERN U16 FARFN bcgetirqmode(int_TMKNUM);
__EXTERN U16 FARFN bcgetmaxbase(int_TMKNUM);
__EXTERN int FARFN bcdefbase(int_TMKNUM__ U16 bcBasePC);
__EXTERN U16 FARFN bcgetbase(int_TMKNUM);
__EXTERN void FARFN bcputw(int_TMKNUM__ U16 bcAddr, U16 bcData);
__EXTERN U16 FARFN bcgetw(int_TMKNUM__ U16 bcAddr);
__EXTERN U32 FARFN bcgetansw(int_TMKNUM__ U16 bcCtrlCode);
__EXTERN void FARFN bcputblk(int_TMKNUM__ U16 bcAddr, void FARDT *pcBuffer, U16 cwLength);
__EXTERN void FARFN bcgetblk(int_TMKNUM__ U16 bcAddr, void FARDT *pcBuffer, U16 cwLength);
__EXTERN int FARFN bcdefbus(int_TMKNUM__ U16 bcBus);
__EXTERN U16 FARFN bcgetbus(int_TMKNUM);
__EXTERN int FARFN bcstart(int_TMKNUM__ U16 bcBase, U16 bcCtrlCode);
__EXTERN int FARFN bcstartx(int_TMKNUM__ U16 bcBase, U16 bcCtrlCode);
__EXTERN int FARFN bcdeflink(int_TMKNUM__ U16 bcBase, U16 bcCtrlCode);
__EXTERN U32 FARFN bcgetlink(int_TMKNUM);
__EXTERN int FARFN bcstop(int_TMKNUM);
__EXTERN U32 FARFN bcgetstate(int_TMKNUM);

#if defined(DOS) || defined(QNX4INT)
__EXTERN void FARFN rtdefintcmd(int_TMKNUM__ RETIR (FARIR* UserCmdRT)(U16));
__EXTERN void FARFN rtdefinterr(int_TMKNUM__ RETIR (FARIR* UserErrRT)(U16));
__EXTERN void FARFN rtdefintdata(int_TMKNUM__ RETIR (FARIR* UserDataRT)(U16));
#endif
__EXTERN int FARFN rtreset(int_TMKNUM);
__EXTERN void FARFN rtrestore(int_TMKNUM);
__EXTERN int FARFN rtdefirqmode(int_TMKNUM__ U16 rtIrqMode);
__EXTERN U16 FARFN rtgetirqmode(int_TMKNUM);
__EXTERN int FARFN rtdefmode(int_TMKNUM__ U16 rtMode);
__EXTERN U16 FARFN rtgetmode(int_TMKNUM);
__EXTERN U16 FARFN rtgetmaxpage(int_TMKNUM);
__EXTERN int FARFN rtdefpage(int_TMKNUM__ U16 rtPage);
__EXTERN U16 FARFN rtgetpage(int_TMKNUM);
__EXTERN int FARFN rtdefpagepc(int_TMKNUM__ U16 rtPagePC);
__EXTERN int FARFN rtdefpagebus(int_TMKNUM__ U16 rtPageBus);
__EXTERN U16 FARFN rtgetpagepc(int_TMKNUM);
__EXTERN U16 FARFN rtgetpagebus(int_TMKNUM);
__EXTERN int FARFN rtdefaddress(int_TMKNUM__ U16 rtAddress);
__EXTERN U16 FARFN rtgetaddress(int_TMKNUM);
__EXTERN void FARFN rtdefsubaddr(int_TMKNUM__ U16 rtDir, U16 rtSubAddr);
__EXTERN U16 FARFN rtgetsubaddr(int_TMKNUM);
__EXTERN void FARFN rtputw(int_TMKNUM__ U16 rtAddr, U16 rtData);
__EXTERN U16 FARFN rtgetw(int_TMKNUM__ U16 rtAddr);
__EXTERN void FARFN rtputblk(int_TMKNUM__ U16 rtAddr, void FARDT *pcBuffer, U16 cwLength);
__EXTERN void FARFN rtgetblk(int_TMKNUM__ U16 rtAddr, void FARDT *pcBuffer, U16 cwLength);
__EXTERN void FARFN rtsetanswbits(int_TMKNUM__ U16 rtSetControl);
__EXTERN void FARFN rtclranswbits(int_TMKNUM__ U16 rtClrControl);
__EXTERN U16 FARFN rtgetanswbits(int_TMKNUM);
__EXTERN void FARFN rtgetflags(int_TMKNUM__ void FARDT *pcBuffer, U16 rtDir, U16 rtFlagMin, U16 rtFlagMax);
__EXTERN void FARFN rtputflags(int_TMKNUM__ void FARDT *pcBuffer, U16 rtDir, U16 rtFlagMin, U16 rtFlagMax);
__EXTERN void FARFN rtsetflag(int_TMKNUM);
__EXTERN void FARFN rtclrflag(int_TMKNUM);
__EXTERN U16 FARFN rtgetflag(int_TMKNUM__ U16 rtDir, U16 rtSubAddr);
__EXTERN U16 FARFN rtgetstate(int_TMKNUM);
__EXTERN U16 FARFN rtbusy(int_TMKNUM);
__EXTERN void FARFN rtlock(int_TMKNUM__ U16 rtDir, U16 rtSubAddr);
__EXTERN void FARFN rtunlock(int_TMKNUM);
__EXTERN U16 FARFN rtgetcmddata(int_TMKNUM__ U16 rtBusCommand);
__EXTERN void FARFN rtputcmddata(int_TMKNUM__ U16 rtBusCommand, U16 rtData);

#if defined(DOS) || defined(QNX4INT)
__EXTERN void FARFN mtdefintx(int_TMKNUM__ RETIR (FARIR* UserIntXMT)(U16, U16));
__EXTERN void FARFN mtdefintsig(int_TMKNUM__ RETIR (FARIR* UserSigMT)(U16));
#endif
__EXTERN int FARFN mtreset(int_TMKNUM);
#define mtrestore bcrestore
#define mtdefirqmode bcdefirqmode
#define mtgetirqmode bcgetirqmode
#define mtgetmaxbase bcgetmaxbase
#define mtdefbase bcdefbase
#define mtgetbase bcgetbase
#define mtputw bcputw
#define mtgetw bcgetw
__EXTERN U16 FARFN mtgetsw(int_TMKNUM);
__EXTERN U32 FARFN mtgetsw2(int_TMKNUM);
__EXTERN U16 FARFN mtgetswa(int_TMKNUM);
#define mtputblk bcputblk
#define mtgetblk bcgetblk
#define mtstartx bcstartx
#define mtdeflink bcdeflink
#define mtgetlink bcgetlink
#define mtstop bcstop
#define mtgetstate bcgetstate

__EXTERN int FARFN mwreset(int_TMKNUM);
__EXTERN int FARFN mwtreset(int_TMKNUM);

__EXTERN int FARFN mtrtreset(int_TMKNUM);
__EXTERN int FARFN mwrtreset(int_TMKNUM);
__EXTERN int FARFN mwtrtreset(int_TMKNUM);

__EXTERN U16 FARFN rtenable(int_TMKNUM__ U16 rtfEnable);

#if NRT > 0
__EXTERN int FARFN mrtgetmaxn(void);
#ifndef DYNAMIC_TMKNUM
__EXTERN int FARFN mrtselected(void);
#endif
__EXTERN int FARFN mrtgetnrt(int_TMKNUM);
__EXTERN int FARFN mrtgetrt0(int_TMKNUM);
__EXTERN U16 FARFN mrtgetstate(int_TMKNUM);
__EXTERN void FARFN mrtdefbrcsubaddr0(int_TMKNUM);
__EXTERN int FARFN mrtdefbrcpage(int_TMKNUM__ U16 mrtBrcPage);
__EXTERN U16 FARFN mrtgetbrcpage(int_TMKNUM);
//__EXTERN int _cdecl mrtreset(void);
#define mrtreset bcreset
__EXTERN int FARFN rt2mrt(int rtNumber);
__EXTERN void FARFN mrtdefmaxnrt(int mrtMaxNrt);
#endif //NRT

__EXTERN U16 FARFN rtgetlock(int_TMKNUM);
__EXTERN void FARFN rtputflag(int_TMKNUM__ U16 rtDir, U16 rtSubAddr, U16 rtFlag);

__EXTERN U16 FARFN tmkgethwver(int_TMKNUM);

__EXTERN U16 FARFN tmktimer(int_TMKNUM__ U16 tmkTimerCtrl);
__EXTERN U16 FARFN tmkgettimerl(int_TMKNUM);
__EXTERN U32 FARFN tmkgettimer(int_TMKNUM);
__EXTERN U32 FARFN bcgetmsgtime(int_TMKNUM);
#define mtgetmsgtime bcgetmsgtime
__EXTERN U32 FARFN rtgetmsgtime(int_TMKNUM);

#ifdef DOS
__EXTERN U16 FARFN tmkswtimer(int_TMKNUM__  U16 tmkSwTimerCtrl);
__EXTERN U32 FARFN tmkgetswtimer(int_TMKNUM);
__EXTERN U32 FARFN tmkgetevtime(int_TMKNUM);
#endif //def DOS

__EXTERN U16 FARFN tmktimeout(int_TMKNUM__ U16 tmkTimeOut);

__EXTERN U16 FARFN tmkdac(int_TMKNUM__ U16 tmkDac);

__EXTERN U16 FARFN bcgetmsgsw(int_TMKNUM);
#define mtgetmsgsw mtgetswa
__EXTERN U16 FARFN rtgetmsgsw(int_TMKNUM);

__EXTERN U16 FARFN rtgap(int_TMKNUM__ U16 rtGap);

__EXTERN U16 FARFN rtallocsabuf(int_TMKNUM__ U16 rtBufModeSA, U16 rtBufSize);
__EXTERN int FARFN rtdefbuf(int_TMKNUM__ U16 rtBuf);
__EXTERN U16 FARFN rtgetbuf(int_TMKNUM);
__EXTERN int FARFN rtdeflink(int_TMKNUM__ U16 rtLink); // Link = Buf | Ctrl ???
__EXTERN U16 FARFN rtgetlink(int_TMKNUM);
__EXTERN int FARFN rtdeflinkbus(int_TMKNUM__ U16 rtLinkBus); // Link = Buf | Ctrl ???
__EXTERN U16 FARFN rtgetlinkbus(int_TMKNUM);
#if defined(DOS) || defined(QNX4INT)
__EXTERN U16 FARFN rtgetbufirq(int_TMKNUM);
#endif

__EXTERN U16 FARFN tmktxrxmode(int_TMKNUM__ U16 tmkTxRxMode);

__EXTERN U16 FARFN mwcdw(int_TMKNUM__ U16 mwCdData, U16 mwCdCtrl);

__EXTERN void FARFN rtdefsubaddrm(int_TMKNUM__ U16 rtDir, U16 rtSubAddr);
__EXTERN U16 FARFN rtgetbase(int_TMKNUM);
__EXTERN U16 FARFN rtgetbasebus(int_TMKNUM);

__EXTERN int FARFN mtdefmode(int_TMKNUM__ U16 mtMode);
__EXTERN U16 FARFN mtgetmode(int_TMKNUM);

__EXTERN int FARFN bcdefmode(int_TMKNUM__ U16 bcMode);
__EXTERN U16 FARFN bcgetmode(int_TMKNUM);
__EXTERN int FARFN bcstartctla(int_TMKNUM__ U16 bcBase, U16 bcCtrlA, U16 bcTimeA, U16 bcLinkA);
__EXTERN int FARFN bcstarta(int_TMKNUM__ U16 bcBase);
__EXTERN int FARFN bcdefctla(int_TMKNUM__ U16 bcCTLA, U16 bcCtrlA, U16 bcTimeA, U16 bcLinkA);
__EXTERN U16 FARFN bcgetctla(int_TMKNUM__ U16 bcCTLA);

#if defined(DOS) || defined(QNX4INT)
__EXTERN void FARFN bcdefinta(int_TMKNUM__ RETIR (FARIR* UserABC)(U16, U16));
__EXTERN void FARFN tmkdefintoth(int_TMKNUM__ RETIR (FARIR* UserOthTMK)(U16));
#endif

__EXTERN U32 FARFN tmkoptions(int_TMKNUM__ U32 tmkOptions);

#ifndef DOS
__EXTERN int FARFN tmkreconfig(int_TMKNUM);
#endif

__EXTERN U16 FARFN mtfilter(int_TMKNUM__ U16 mtFilterCtrl, U16 mtFilter1, U16 mtFilter2);
__EXTERN U16 FARFN rtlegal(int_TMKNUM__ U16 rtLegalCtrl, U16 rtLegal1, U16 rtLegal2);

__EXTERN void FARFN bcclrmsgtime(int_TMKNUM);
#define mtclrmsgtime bcclrmsgtime
__EXTERN void FARFN rtclrmsgtime(int_TMKNUM);
__EXTERN void FARFN bcclrmsgsw(int_TMKNUM);
#define mtclrmsgsw bcclrmsgsw
__EXTERN void FARFN rtclrmsgsw(int_TMKNUM);

__EXTERN U16 FARFN mtgetmaxmaxbase(int_TMKNUM);
__EXTERN int FARFN mtdefmaxbase(int_TMKNUM__ U16 mtMaxBase);

#if NRT > 0
__EXTERN int FARFN tmkbuserr(int_TMKNUM__ U16 tmkBusErr);
#endif

__EXTERN U32 FARFN tmkinfo(int_TMKNUM__ U16 tmkData, U16 tmkInfo);

#ifdef USE_RTIFNB
__EXTERN U16 FARFN rtputblkifnb(int_TMKNUM__ U16 rtAddr, void FARDT *pcBuffer, U16 cwLength);
__EXTERN U16 FARFN rtgetblkifnb(int_TMKNUM__ U16 rtAddr, void FARDT *pcBuffer, U16 cwLength);
#endif

#ifdef USE_TMKRAWIO
#ifndef HUGEDT
#define HUGEDT
#endif
#ifndef TRAWIO
#define TRAWIO unsigned
#endif
__EXTERN int FARFN tmkrawion(int_TMKNUM__ void HUGEDT *pcBufOut, U32 dwLenOut, void HUGEDT *pcBufIn, U32 dwLenIn);
#if defined(DOS) || defined(QNX4INT)
__EXTERN void FARFN tmkdefintraw(int_TMKNUM__ RETIR (FARIR* UserRawTMK)(U16,U16));
#endif
#endif

#if !defined(DOS) || defined(TMKDRV_FUNCS)
__EXTERN void FARFN tmkdrvkeepflgm(int tmkKeepFlgM);
__EXTERN void FARFN tmkdrvcbsnd(int tmkCBSND);
__EXTERN void FARFN tmkdrvwrapsa(unsigned tmkWrapSA, int tmkWrapST, int tmkWrapInt);
__EXTERN void FARFN tmkdrvinit(void);
#endif

#ifdef TMK_REAL_MAX
__EXTERN void FARFN tmkdrvrealmax(int tmkUseRealMax);
#endif

#ifdef NMBCID
__EXTERN int FARFN mbcinit(U16 mbcId);
__EXTERN int FARFN mbcpreparex(int_TMKNUM__ U16 mbcId, U16 bcBase, U16 bcCtrlCode, U16 mbcDelay);
__EXTERN int FARFN mbcstartx(U16 mbcId);
__EXTERN U16 FARFN mbcalloc(void);
__EXTERN int FARFN mbcfree(U16 mbcId);
__EXTERN int FARFN mbcpreparea(int_TMKNUM__ U16 mbcId, U16 bcBase);
#define mbcstarta mbcstartx
#endif

#ifdef __cplusplus
//}
#endif

#endif
