/*
 * tmkinlin.c -- the tmk1553b parameters init v5.06a. (c) ELCUS, 2002,2024.
 */

#ifdef LINUX26
#include <linux/moduleparam.h>
#endif

int tmk1553b_nr_devs = TMK1553B_NR_DEVS;    /* number of bare tmk1553b devices */

static int major = TMK1553B_MAJOR;
static char *name = 0;
static int misc = -1;
static int autopci = -1;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
static int chmod = -1;
#endif
static int nrt = 0;
static int noplx = 0;
static int pcim = 1;
static int kfm = -1;
static int wraps = -1;
static int wrapt = 0;
static int wrapi = 0;
static int cbsnd = -1;
static int real = -1;
#ifdef TMK1553B_THREADS
static int mthread = 1;
#else
#ifdef LINUX26
static int mthread = 0;
#endif
#endif

#ifdef module_param
module_param(major, int, 0);
module_param(name, charp, 0);
module_param(misc, int, 0);
module_param(autopci, int, 0);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
module_param(chmod, int, 0);
#endif
module_param(nrt, int, 0);
module_param(noplx, int, 0);
module_param(pcim, int, 0);
module_param(kfm, int, 0);
module_param(wraps, int, 0);
module_param(wrapt, int, 0);
module_param(wrapi, int, 0);
module_param(cbsnd, int, 0);
module_param(real, int, 0);
#if defined(TMK1553B_THREADS) || defined(LINUX26)
module_param(mthread, int, 0);
#endif
#else
MODULE_PARM(major,"i");
MODULE_PARM(name,"s");
MODULE_PARM(misc,"i");
MODULE_PARM(autopci,"i");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
MODULE_PARM(chmod, "i");
#endif
MODULE_PARM(nrt,"i");
MODULE_PARM(noplx,"i");
MODULE_PARM(pcim,"i");
MODULE_PARM(kfm,"i");
MODULE_PARM(wraps,"i");
MODULE_PARM(wrapt,"i");
MODULE_PARM(wrapi,"i");
MODULE_PARM(cbsnd,"i");
MODULE_PARM(real,"i");
#if defined(TMK1553B_THREADS) || defined(LINUX26)
MODULE_PARM(mthread,"i");
#endif
#endif
//MODULE_PARM(tmk1553b_nr_devs,"i");

static int a0 = 0xFFFF;
static int b0 = 0xFFFF;
static int i0 = 0xFF;
static int d0 = 0;
static int e0 = 0;
static char *t0 = 0;
static char *l0 = 0;
static int a1 = 0xFFFF;
static int b1 = 0xFFFF;
static int i1 = 0xFF;
static int d1 = 0;
static int e1 = 0;
static char *t1 = 0;
static char *l1 = 0;
static int a2 = 0xFFFF;
static int b2 = 0xFFFF;
static int i2 = 0xFF;
static int d2 = 0;
static int e2 = 0;
static char *t2 = 0;
static char *l2 = 0;
static int a3 = 0xFFFF;
static int b3 = 0xFFFF;
static int i3 = 0xFF;
static int d3 = 0;
static int e3 = 0;
static char *t3 = 0;
static char *l3 = 0;
static int a4 = 0xFFFF;
static int b4 = 0xFFFF;
static int i4 = 0xFF;
static int d4 = 0;
static int e4 = 0;
static char *t4 = 0;
static char *l4 = 0;
static int a5 = 0xFFFF;
static int b5 = 0xFFFF;
static int i5 = 0xFF;
static int d5 = 0;
static int e5 = 0;
static char *t5 = 0;
static char *l5 = 0;
static int a6 = 0xFFFF;
static int b6 = 0xFFFF;
static int i6 = 0xFF;
static int d6 = 0;
static int e6 = 0;
static char *t6 = 0;
static char *l6 = 0;
static int a7 = 0xFFFF;
static int b7 = 0xFFFF;
static int i7 = 0xFF;
static int d7 = 0;
static int e7 = 0;
static char *t7 = 0;
static char *l7 = 0;
#if MAX_TMK_NUMBER > 7
static int a8 = 0xFFFF;
static int b8 = 0xFFFF;
static int i8 = 0xFF;
static int d8 = 0;
static int e8 = 0;
static char *t8 = 0;
static char *l8 = 0;
static int a9 = 0xFFFF;
static int b9 = 0xFFFF;
static int i9 = 0xFF;
static int d9 = 0;
static int e9 = 0;
static char *t9 = 0;
static char *l9 = 0;
static int a10 = 0xFFFF;
static int b10 = 0xFFFF;
static int i10 = 0xFF;
static int d10 = 0;
static int e10 = 0;
static char *t10 = 0;
static char *l10 = 0;
static int a11 = 0xFFFF;
static int b11 = 0xFFFF;
static int i11 = 0xFF;
static int d11 = 0;
static int e11 = 0;
static char *t11 = 0;
static char *l11 = 0;
static int a12 = 0xFFFF;
static int b12 = 0xFFFF;
static int i12 = 0xFF;
static int d12 = 0;
static int e12 = 0;
static char *t12 = 0;
static char *l12 = 0;
static int a13 = 0xFFFF;
static int b13 = 0xFFFF;
static int i13 = 0xFF;
static int d13 = 0;
static int e13 = 0;
static char *t13 = 0;
static char *l13 = 0;
static int a14 = 0xFFFF;
static int b14 = 0xFFFF;
static int i14 = 0xFF;
static int d14 = 0;
static int e14 = 0;
static char *t14 = 0;
static char *l14 = 0;
static int a15 = 0xFFFF;
static int b15 = 0xFFFF;
static int i15 = 0xFF;
static int d15 = 0;
static int e15 = 0;
static char *t15 = 0;
static char *l15 = 0;
#endif // MAX_TMK_NUMBER > 7
#if MAX_TMK_NUMBER > 15
static int a16 = 0xFFFF;
static int b16 = 0xFFFF;
static int i16 = 0xFF;
static int d16 = 0;
static int e16 = 0;
static char *t16 = 0;
static char *l16 = 0;
static int a17 = 0xFFFF;
static int b17 = 0xFFFF;
static int i17 = 0xFF;
static int d17 = 0;
static int e17 = 0;
static char *t17 = 0;
static char *l17 = 0;
static int a18 = 0xFFFF;
static int b18 = 0xFFFF;
static int i18 = 0xFF;
static int d18 = 0;
static int e18 = 0;
static char *t18 = 0;
static char *l18 = 0;
static int a19 = 0xFFFF;
static int b19 = 0xFFFF;
static int i19 = 0xFF;
static int d19 = 0;
static int e19 = 0;
static char *t19 = 0;
static char *l19 = 0;
static int a20 = 0xFFFF;
static int b20 = 0xFFFF;
static int i20 = 0xFF;
static int d20 = 0;
static int e20 = 0;
static char *t20 = 0;
static char *l20 = 0;
static int a21 = 0xFFFF;
static int b21 = 0xFFFF;
static int i21 = 0xFF;
static int d21 = 0;
static int e21 = 0;
static char *t21 = 0;
static char *l21 = 0;
static int a22 = 0xFFFF;
static int b22 = 0xFFFF;
static int i22 = 0xFF;
static int d22 = 0;
static int e22 = 0;
static char *t22 = 0;
static char *l22 = 0;
static int a23 = 0xFFFF;
static int b23 = 0xFFFF;
static int i23 = 0xFF;
static int d23 = 0;
static int e23 = 0;
static char *t23 = 0;
static char *l23 = 0;
static int a24 = 0xFFFF;
static int b24 = 0xFFFF;
static int i24 = 0xFF;
static int d24 = 0;
static int e24 = 0;
static char *t24 = 0;
static char *l24 = 0;
static int a25 = 0xFFFF;
static int b25 = 0xFFFF;
static int i25 = 0xFF;
static int d25 = 0;
static int e25 = 0;
static char *t25 = 0;
static char *l25 = 0;
static int a26 = 0xFFFF;
static int b26 = 0xFFFF;
static int i26 = 0xFF;
static int d26 = 0;
static int e26 = 0;
static char *t26 = 0;
static char *l26 = 0;
static int a27 = 0xFFFF;
static int b27 = 0xFFFF;
static int i27 = 0xFF;
static int d27 = 0;
static int e27 = 0;
static char *t27 = 0;
static char *l27 = 0;
static int a28 = 0xFFFF;
static int b28 = 0xFFFF;
static int i28 = 0xFF;
static int d28 = 0;
static int e28 = 0;
static char *t28 = 0;
static char *l28 = 0;
static int a29 = 0xFFFF;
static int b29 = 0xFFFF;
static int i29 = 0xFF;
static int d29 = 0;
static int e29 = 0;
static char *t29 = 0;
static char *l29 = 0;
static int a30 = 0xFFFF;
static int b30 = 0xFFFF;
static int i30 = 0xFF;
static int d30 = 0;
static int e30 = 0;
static char *t30 = 0;
static char *l30 = 0;
static int a31 = 0xFFFF;
static int b31 = 0xFFFF;
static int i31 = 0xFF;
static int d31 = 0;
static int e31 = 0;
static char *t31 = 0;
static char *l31 = 0;
#endif // MAX_TMK_NUMBER > 15

#ifdef module_param
module_param(a0, int, 0);
module_param(b0, int, 0);
module_param(i0, int, 0);
module_param(d0, int, 0);
module_param(e0, int, 0);
module_param(t0, charp, 0);
module_param(l0, charp, 0);
module_param(a1, int, 0);
module_param(b1, int, 0);
module_param(i1, int, 0);
module_param(d1, int, 0);
module_param(e1, int, 0);
module_param(t1, charp, 0);
module_param(l1, charp, 0);
module_param(a2, int, 0);
module_param(b2, int, 0);
module_param(i2, int, 0);
module_param(d2, int, 0);
module_param(e2, int, 0);
module_param(t2, charp, 0);
module_param(l2, charp, 0);
module_param(a3, int, 0);
module_param(b3, int, 0);
module_param(i3, int, 0);
module_param(d3, int, 0);
module_param(e3, int, 0);
module_param(t3, charp, 0);
module_param(l3, charp, 0);
module_param(a4, int, 0);
module_param(b4, int, 0);
module_param(i4, int, 0);
module_param(d4, int, 0);
module_param(e4, int, 0);
module_param(t4, charp, 0);
module_param(l4, charp, 0);
module_param(a5, int, 0);
module_param(b5, int, 0);
module_param(i5, int, 0);
module_param(d5, int, 0);
module_param(e5, int, 0);
module_param(t5, charp, 0);
module_param(l5, charp, 0);
module_param(a6, int, 0);
module_param(b6, int, 0);
module_param(i6, int, 0);
module_param(d6, int, 0);
module_param(e6, int, 0);
module_param(t6, charp, 0);
module_param(l6, charp, 0);
module_param(a7, int, 0);
module_param(b7, int, 0);
module_param(i7, int, 0);
module_param(d7, int, 0);
module_param(e7, int, 0);
module_param(t7, charp, 0);
module_param(l7, charp, 0);
#if MAX_TMK_NUMBER > 7
module_param(a8, int, 0);
module_param(b8, int, 0);
module_param(i8, int, 0);
module_param(d8, int, 0);
module_param(e8, int, 0);
module_param(t8, charp, 0);
module_param(l8, charp, 0);
module_param(a9, int, 0);
module_param(b9, int, 0);
module_param(i9, int, 0);
module_param(d9, int, 0);
module_param(e9, int, 0);
module_param(t9, charp, 0);
module_param(l9, charp, 0);
module_param(a10, int, 0);
module_param(b10, int, 0);
module_param(i10, int, 0);
module_param(d10, int, 0);
module_param(e10, int, 0);
module_param(t10, charp, 0);
module_param(l10, charp, 0);
module_param(a11, int, 0);
module_param(b11, int, 0);
module_param(i11, int, 0);
module_param(d11, int, 0);
module_param(e11, int, 0);
module_param(t11, charp, 0);
module_param(l11, charp, 0);
module_param(a12, int, 0);
module_param(b12, int, 0);
module_param(i12, int, 0);
module_param(d12, int, 0);
module_param(e12, int, 0);
module_param(t12, charp, 0);
module_param(l12, charp, 0);
module_param(a13, int, 0);
module_param(b13, int, 0);
module_param(i13, int, 0);
module_param(d13, int, 0);
module_param(e13, int, 0);
module_param(t13, charp, 0);
module_param(l13, charp, 0);
module_param(a14, int, 0);
module_param(b14, int, 0);
module_param(i14, int, 0);
module_param(d14, int, 0);
module_param(e14, int, 0);
module_param(t14, charp, 0);
module_param(l14, charp, 0);
module_param(a15, int, 0);
module_param(b15, int, 0);
module_param(i15, int, 0);
module_param(d15, int, 0);
module_param(e15, int, 0);
module_param(t15, charp, 0);
module_param(l15, charp, 0);
#endif // MAX_TMK_NUMBER > 7
#if MAX_TMK_NUMBER > 15
module_param(a16, int, 0);
module_param(b16, int, 0);
module_param(i16, int, 0);
module_param(d16, int, 0);
module_param(e16, int, 0);
module_param(t16, charp, 0);
module_param(l16, charp, 0);
module_param(a17, int, 0);
module_param(b17, int, 0);
module_param(i17, int, 0);
module_param(d17, int, 0);
module_param(e17, int, 0);
module_param(t17, charp, 0);
module_param(l17, charp, 0);
module_param(a18, int, 0);
module_param(b18, int, 0);
module_param(i18, int, 0);
module_param(d18, int, 0);
module_param(e18, int, 0);
module_param(t18, charp, 0);
module_param(l18, charp, 0);
module_param(a19, int, 0);
module_param(b19, int, 0);
module_param(i19, int, 0);
module_param(d19, int, 0);
module_param(e19, int, 0);
module_param(t19, charp, 0);
module_param(l19, charp, 0);
module_param(a20, int, 0);
module_param(b20, int, 0);
module_param(i20, int, 0);
module_param(d20, int, 0);
module_param(e20, int, 0);
module_param(t20, charp, 0);
module_param(l20, charp, 0);
module_param(a21, int, 0);
module_param(b21, int, 0);
module_param(i21, int, 0);
module_param(d21, int, 0);
module_param(e21, int, 0);
module_param(t21, charp, 0);
module_param(l21, charp, 0);
module_param(a22, int, 0);
module_param(b22, int, 0);
module_param(i22, int, 0);
module_param(d22, int, 0);
module_param(e22, int, 0);
module_param(t22, charp, 0);
module_param(l22, charp, 0);
module_param(a23, int, 0);
module_param(b23, int, 0);
module_param(i23, int, 0);
module_param(d23, int, 0);
module_param(e23, int, 0);
module_param(t23, charp, 0);
module_param(l23, charp, 0);
module_param(a24, int, 0);
module_param(b24, int, 0);
module_param(i24, int, 0);
module_param(d24, int, 0);
module_param(e24, int, 0);
module_param(t24, charp, 0);
module_param(l24, charp, 0);
module_param(a25, int, 0);
module_param(b25, int, 0);
module_param(i25, int, 0);
module_param(d25, int, 0);
module_param(e25, int, 0);
module_param(t25, charp, 0);
module_param(l25, charp, 0);
module_param(a26, int, 0);
module_param(b26, int, 0);
module_param(i26, int, 0);
module_param(d26, int, 0);
module_param(e26, int, 0);
module_param(t26, charp, 0);
module_param(l26, charp, 0);
module_param(a27, int, 0);
module_param(b27, int, 0);
module_param(i27, int, 0);
module_param(d27, int, 0);
module_param(e27, int, 0);
module_param(t27, charp, 0);
module_param(l27, charp, 0);
module_param(a28, int, 0);
module_param(b28, int, 0);
module_param(i28, int, 0);
module_param(d28, int, 0);
module_param(e28, int, 0);
module_param(t28, charp, 0);
module_param(l28, charp, 0);
module_param(a29, int, 0);
module_param(b29, int, 0);
module_param(i29, int, 0);
module_param(d29, int, 0);
module_param(e29, int, 0);
module_param(t29, charp, 0);
module_param(l29, charp, 0);
module_param(a30, int, 0);
module_param(b30, int, 0);
module_param(i30, int, 0);
module_param(d30, int, 0);
module_param(e30, int, 0);
module_param(t30, charp, 0);
module_param(l30, charp, 0);
module_param(a31, int, 0);
module_param(b31, int, 0);
module_param(i31, int, 0);
module_param(d31, int, 0);
module_param(e31, int, 0);
module_param(t31, charp, 0);
module_param(l31, charp, 0);
#endif // MAX_TMK_NUMBER > 15
#else
MODULE_PARM(a0,"i");
MODULE_PARM(b0,"i");
MODULE_PARM(i0,"i");
MODULE_PARM(d0,"i");
MODULE_PARM(e0,"i");
MODULE_PARM(t0,"s");
MODULE_PARM(l0,"s");
MODULE_PARM(a1,"i");
MODULE_PARM(b1,"i");
MODULE_PARM(i1,"i");
MODULE_PARM(d1,"i");
MODULE_PARM(e1,"i");
MODULE_PARM(t1,"s");
MODULE_PARM(l1,"s");
MODULE_PARM(a2,"i");
MODULE_PARM(b2,"i");
MODULE_PARM(i2,"i");
MODULE_PARM(d2,"i");
MODULE_PARM(e2,"i");
MODULE_PARM(t2,"s");
MODULE_PARM(l2,"s");
MODULE_PARM(a3,"i");
MODULE_PARM(b3,"i");
MODULE_PARM(i3,"i");
MODULE_PARM(d3,"i");
MODULE_PARM(e3,"i");
MODULE_PARM(t3,"s");
MODULE_PARM(l3,"s");
MODULE_PARM(a4,"i");
MODULE_PARM(b4,"i");
MODULE_PARM(i4,"i");
MODULE_PARM(d4,"i");
MODULE_PARM(e4,"i");
MODULE_PARM(t4,"s");
MODULE_PARM(l4,"s");
MODULE_PARM(a5,"i");
MODULE_PARM(b5,"i");
MODULE_PARM(i5,"i");
MODULE_PARM(d5,"i");
MODULE_PARM(e5,"i");
MODULE_PARM(t5,"s");
MODULE_PARM(l5,"s");
MODULE_PARM(a6,"i");
MODULE_PARM(b6,"i");
MODULE_PARM(i6,"i");
MODULE_PARM(d6,"i");
MODULE_PARM(e6,"i");
MODULE_PARM(t6,"s");
MODULE_PARM(l6,"s");
MODULE_PARM(a7,"i");
MODULE_PARM(b7,"i");
MODULE_PARM(i7,"i");
MODULE_PARM(d7,"i");
MODULE_PARM(e7,"i");
MODULE_PARM(t7,"s");
MODULE_PARM(l7,"s");
#if MAX_TMK_NUMBER > 7
MODULE_PARM(a8,"i");
MODULE_PARM(b8,"i");
MODULE_PARM(i8,"i");
MODULE_PARM(d8,"i");
MODULE_PARM(e8,"i");
MODULE_PARM(t8,"s");
MODULE_PARM(l8,"s");
MODULE_PARM(a9,"i");
MODULE_PARM(b9,"i");
MODULE_PARM(i9,"i");
MODULE_PARM(d9,"i");
MODULE_PARM(e9,"i");
MODULE_PARM(t9,"s");
MODULE_PARM(l9,"s");
MODULE_PARM(a10,"i");
MODULE_PARM(b10,"i");
MODULE_PARM(i10,"i");
MODULE_PARM(d10,"i");
MODULE_PARM(e10,"i");
MODULE_PARM(t10,"s");
MODULE_PARM(l10,"s");
MODULE_PARM(a11,"i");
MODULE_PARM(b11,"i");
MODULE_PARM(i11,"i");
MODULE_PARM(d11,"i");
MODULE_PARM(e11,"i");
MODULE_PARM(t11,"s");
MODULE_PARM(l11,"s");
MODULE_PARM(a12,"i");
MODULE_PARM(b12,"i");
MODULE_PARM(i12,"i");
MODULE_PARM(d12,"i");
MODULE_PARM(e12,"i");
MODULE_PARM(t12,"s");
MODULE_PARM(l12,"s");
MODULE_PARM(a13,"i");
MODULE_PARM(b13,"i");
MODULE_PARM(i13,"i");
MODULE_PARM(d13,"i");
MODULE_PARM(e13,"i");
MODULE_PARM(t13,"s");
MODULE_PARM(l13,"s");
MODULE_PARM(a14,"i");
MODULE_PARM(b14,"i");
MODULE_PARM(i14,"i");
MODULE_PARM(d14,"i");
MODULE_PARM(e14,"i");
MODULE_PARM(t14,"s");
MODULE_PARM(l14,"s");
MODULE_PARM(a15,"i");
MODULE_PARM(b15,"i");
MODULE_PARM(i15,"i");
MODULE_PARM(d15,"i");
MODULE_PARM(e15,"i");
MODULE_PARM(t15,"s");
MODULE_PARM(l15,"s");
#endif // MAX_TMK_NUMBER > 7
#if MAX_TMK_NUMBER > 15
MODULE_PARM(a16,"i");
MODULE_PARM(b16,"i");
MODULE_PARM(i16,"i");
MODULE_PARM(d16,"i");
MODULE_PARM(e16,"i");
MODULE_PARM(t16,"s");
MODULE_PARM(l16,"s");
MODULE_PARM(a17,"i");
MODULE_PARM(b17,"i");
MODULE_PARM(i17,"i");
MODULE_PARM(d17,"i");
MODULE_PARM(e17,"i");
MODULE_PARM(t17,"s");
MODULE_PARM(l17,"s");
MODULE_PARM(a18,"i");
MODULE_PARM(b18,"i");
MODULE_PARM(i18,"i");
MODULE_PARM(d18,"i");
MODULE_PARM(e18,"i");
MODULE_PARM(t18,"s");
MODULE_PARM(l18,"s");
MODULE_PARM(a19,"i");
MODULE_PARM(b19,"i");
MODULE_PARM(i19,"i");
MODULE_PARM(d19,"i");
MODULE_PARM(e19,"i");
MODULE_PARM(t19,"s");
MODULE_PARM(l19,"s");
MODULE_PARM(a20,"i");
MODULE_PARM(b20,"i");
MODULE_PARM(i20,"i");
MODULE_PARM(d20,"i");
MODULE_PARM(e20,"i");
MODULE_PARM(t20,"s");
MODULE_PARM(l20,"s");
MODULE_PARM(a21,"i");
MODULE_PARM(b21,"i");
MODULE_PARM(i21,"i");
MODULE_PARM(d21,"i");
MODULE_PARM(e21,"i");
MODULE_PARM(t21,"s");
MODULE_PARM(l21,"s");
MODULE_PARM(a22,"i");
MODULE_PARM(b22,"i");
MODULE_PARM(i22,"i");
MODULE_PARM(d22,"i");
MODULE_PARM(e22,"i");
MODULE_PARM(t22,"s");
MODULE_PARM(l22,"s");
MODULE_PARM(a23,"i");
MODULE_PARM(b23,"i");
MODULE_PARM(i23,"i");
MODULE_PARM(d23,"i");
MODULE_PARM(e23,"i");
MODULE_PARM(t23,"s");
MODULE_PARM(l23,"s");
MODULE_PARM(a24,"i");
MODULE_PARM(b24,"i");
MODULE_PARM(i24,"i");
MODULE_PARM(d24,"i");
MODULE_PARM(e24,"i");
MODULE_PARM(t24,"s");
MODULE_PARM(l24,"s");
MODULE_PARM(a25,"i");
MODULE_PARM(b25,"i");
MODULE_PARM(i25,"i");
MODULE_PARM(d25,"i");
MODULE_PARM(e25,"i");
MODULE_PARM(t25,"s");
MODULE_PARM(l25,"s");
MODULE_PARM(a26,"i");
MODULE_PARM(b26,"i");
MODULE_PARM(i26,"i");
MODULE_PARM(d26,"i");
MODULE_PARM(e26,"i");
MODULE_PARM(t26,"s");
MODULE_PARM(l26,"s");
MODULE_PARM(a27,"i");
MODULE_PARM(b27,"i");
MODULE_PARM(i27,"i");
MODULE_PARM(d27,"i");
MODULE_PARM(e27,"i");
MODULE_PARM(t27,"s");
MODULE_PARM(l27,"s");
MODULE_PARM(a28,"i");
MODULE_PARM(b28,"i");
MODULE_PARM(i28,"i");
MODULE_PARM(d28,"i");
MODULE_PARM(e28,"i");
MODULE_PARM(t28,"s");
MODULE_PARM(l28,"s");
MODULE_PARM(a29,"i");
MODULE_PARM(b29,"i");
MODULE_PARM(i29,"i");
MODULE_PARM(d29,"i");
MODULE_PARM(e29,"i");
MODULE_PARM(t29,"s");
MODULE_PARM(l29,"s");
MODULE_PARM(a30,"i");
MODULE_PARM(b30,"i");
MODULE_PARM(i30,"i");
MODULE_PARM(d30,"i");
MODULE_PARM(e30,"i");
MODULE_PARM(t30,"s");
MODULE_PARM(l30,"s");
MODULE_PARM(a31,"i");
MODULE_PARM(b31,"i");
MODULE_PARM(i31,"i");
MODULE_PARM(d31,"i");
MODULE_PARM(e31,"i");
MODULE_PARM(t31,"s");
MODULE_PARM(l31,"s");
#endif // MAX_TMK_NUMBER > 15
#endif

MODULE_PARM_DESC(major, "Major number");
MODULE_PARM_DESC(name, "Device name (default: tmk1553b)");
MODULE_PARM_DESC(misc, "Register as misc device if =1");
MODULE_PARM_DESC(autopci, "Auto config PCI if =1");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
MODULE_PARM_DESC(chmod, "Device file permissions for misc device");
#endif
MODULE_PARM_DESC(nrt,"Maximum number of RTs per MultiRT device");
MODULE_PARM_DESC(noplx,"Don't hold TA/MRTA PLX ports if =1");
MODULE_PARM_DESC(pcim,"PCI mode (experimental)");
MODULE_PARM_DESC(kfm,"Keep flag mode on RT reset if =1");
MODULE_PARM_DESC(wraps,"Subaddress for data wrap-around in TA/MRTA RT");
MODULE_PARM_DESC(wrapt,"Add a dummy Tx part of TA/MRTA RT wrap-around subaddress if =1");
MODULE_PARM_DESC(wrapi,"Allow interrupts in TA/MRTA RT wrap-around subaddress if =1");
MODULE_PARM_DESC(cbsnd,"No data with busy in mode commands in TA/MRTA RT if =1");
MODULE_PARM_DESC(real,"Report real maxn to programs if =1");
#ifdef TMK1553B_THREADS
MODULE_PARM_DESC(mthread,"Treat threads as processes if =0");
#else
#ifdef LINUX26
MODULE_PARM_DESC(mthread,"Treat threads as processes if =0 (default)");
#endif
#endif

MODULE_PARM_DESC(a0,"ISA addr1 device 0");
MODULE_PARM_DESC(b0,"ISA addr2 device 0");
MODULE_PARM_DESC(i0,"ISA interrupt device 0");
MODULE_PARM_DESC(d0,"PCI card number device 0");
MODULE_PARM_DESC(e0,"PCI device on PCI card number device 0");
MODULE_PARM_DESC(t0,"Device type device 0");
MODULE_PARM_DESC(l0,"Device load device 0");
MODULE_PARM_DESC(a1,"ISA addr1 device 1");
MODULE_PARM_DESC(b1,"ISA addr2 device 1");
MODULE_PARM_DESC(i1,"ISA interrupt device 1");
MODULE_PARM_DESC(d1,"PCI card number device 1");
MODULE_PARM_DESC(e1,"PCI device on PCI card number device 1");
MODULE_PARM_DESC(t1,"Device type device 1");
MODULE_PARM_DESC(l1,"Device load device 1");
MODULE_PARM_DESC(a2,"ISA addr1 device 2");
MODULE_PARM_DESC(b2,"ISA addr2 device 2");
MODULE_PARM_DESC(i2,"ISA interrupt device 2");
MODULE_PARM_DESC(d2,"PCI card number device 2");
MODULE_PARM_DESC(e2,"PCI device on PCI card number device 2");
MODULE_PARM_DESC(t2,"Device type device 2");
MODULE_PARM_DESC(l2,"Device load device 2");
MODULE_PARM_DESC(a3,"ISA addr1 device 3");
MODULE_PARM_DESC(b3,"ISA addr2 device 3");
MODULE_PARM_DESC(i3,"ISA interrupt device 3");
MODULE_PARM_DESC(d3,"PCI card number device 3");
MODULE_PARM_DESC(e3,"PCI device on PCI card number device 3");
MODULE_PARM_DESC(t3,"Device type device 3");
MODULE_PARM_DESC(l3,"Device load device 3");
MODULE_PARM_DESC(a4,"ISA addr1 device 4");
MODULE_PARM_DESC(b4,"ISA addr2 device 4");
MODULE_PARM_DESC(i4,"ISA interrupt device 4");
MODULE_PARM_DESC(d4,"PCI card number device 4");
MODULE_PARM_DESC(e4,"PCI device on PCI card number device 4");
MODULE_PARM_DESC(t4,"Device type device 4");
MODULE_PARM_DESC(l4,"Device load device 4");
MODULE_PARM_DESC(a5,"ISA addr1 device 5");
MODULE_PARM_DESC(b5,"ISA addr2 device 5");
MODULE_PARM_DESC(i5,"ISA interrupt device 5");
MODULE_PARM_DESC(d5,"PCI card number device 5");
MODULE_PARM_DESC(e5,"PCI device on PCI card number device 5");
MODULE_PARM_DESC(t5,"Device type device 5");
MODULE_PARM_DESC(l5,"Device load device 5");
MODULE_PARM_DESC(a6,"ISA addr1 device 6");
MODULE_PARM_DESC(b6,"ISA addr2 device 6");
MODULE_PARM_DESC(i6,"ISA interrupt device 6");
MODULE_PARM_DESC(d6,"PCI card number device 6");
MODULE_PARM_DESC(e6,"PCI device on PCI card number device 6");
MODULE_PARM_DESC(t6,"Device type device 6");
MODULE_PARM_DESC(l6,"Device load device 6");
MODULE_PARM_DESC(a7,"ISA addr1 device 7");
MODULE_PARM_DESC(b7,"ISA addr2 device 7");
MODULE_PARM_DESC(i7,"ISA interrupt device 7");
MODULE_PARM_DESC(d7,"PCI card number device 7");
MODULE_PARM_DESC(e7,"PCI device on PCI card number device 7");
MODULE_PARM_DESC(t7,"Device type device 7");
MODULE_PARM_DESC(l7,"Device load device 7");
#if MAX_TMK_NUMBER > 7
MODULE_PARM_DESC(a8,"ISA addr1 device 8");
MODULE_PARM_DESC(b8,"ISA addr2 device 8");
MODULE_PARM_DESC(i8,"ISA interrupt device 8");
MODULE_PARM_DESC(d8,"PCI card number device 8");
MODULE_PARM_DESC(e8,"PCI device on PCI card number device 8");
MODULE_PARM_DESC(t8,"Device type device 8");
MODULE_PARM_DESC(l8,"Device load device 8");
MODULE_PARM_DESC(a9,"ISA addr1 device 9");
MODULE_PARM_DESC(b9,"ISA addr2 device 9");
MODULE_PARM_DESC(i9,"ISA interrupt device 9");
MODULE_PARM_DESC(d9,"PCI card number device 9");
MODULE_PARM_DESC(e9,"PCI device on PCI card number device 9");
MODULE_PARM_DESC(t9,"Device type device 9");
MODULE_PARM_DESC(l9,"Device load device 9");
MODULE_PARM_DESC(a10,"ISA addr1 device 10");
MODULE_PARM_DESC(b10,"ISA addr2 device 10");
MODULE_PARM_DESC(i10,"ISA interrupt device 10");
MODULE_PARM_DESC(d10,"PCI card number device 10");
MODULE_PARM_DESC(e10,"PCI device on PCI card number device 10");
MODULE_PARM_DESC(t10,"Device type device 10");
MODULE_PARM_DESC(l10,"Device load device 10");
MODULE_PARM_DESC(a11,"ISA addr1 device 11");
MODULE_PARM_DESC(b11,"ISA addr2 device 11");
MODULE_PARM_DESC(i11,"ISA interrupt device 11");
MODULE_PARM_DESC(d11,"PCI card number device 11");
MODULE_PARM_DESC(e11,"PCI device on PCI card number device 11");
MODULE_PARM_DESC(t11,"Device type device 11");
MODULE_PARM_DESC(l11,"Device load device 11");
MODULE_PARM_DESC(a12,"ISA addr1 device 12");
MODULE_PARM_DESC(b12,"ISA addr2 device 12");
MODULE_PARM_DESC(i12,"ISA interrupt device 12");
MODULE_PARM_DESC(d12,"PCI card number device 12");
MODULE_PARM_DESC(e12,"PCI device on PCI card number device 12");
MODULE_PARM_DESC(t12,"Device type device 12");
MODULE_PARM_DESC(l12,"Device load device 12");
MODULE_PARM_DESC(a13,"ISA addr1 device 13");
MODULE_PARM_DESC(b13,"ISA addr2 device 13");
MODULE_PARM_DESC(i13,"ISA interrupt device 13");
MODULE_PARM_DESC(d13,"PCI card number device 13");
MODULE_PARM_DESC(e13,"PCI device on PCI card number device 13");
MODULE_PARM_DESC(t13,"Device type device 13");
MODULE_PARM_DESC(l13,"Device load device 13");
MODULE_PARM_DESC(a14,"ISA addr1 device 14");
MODULE_PARM_DESC(b14,"ISA addr2 device 14");
MODULE_PARM_DESC(i14,"ISA interrupt device 14");
MODULE_PARM_DESC(d14,"PCI card number device 14");
MODULE_PARM_DESC(e14,"PCI device on PCI card number device 14");
MODULE_PARM_DESC(t14,"Device type device 14");
MODULE_PARM_DESC(l14,"Device load device 14");
MODULE_PARM_DESC(a15,"ISA addr1 device 15");
MODULE_PARM_DESC(b15,"ISA addr2 device 15");
MODULE_PARM_DESC(i15,"ISA interrupt device 15");
MODULE_PARM_DESC(d15,"PCI card number device 15");
MODULE_PARM_DESC(e15,"PCI device on PCI card number device 15");
MODULE_PARM_DESC(t15,"Device type device 15");
MODULE_PARM_DESC(l15,"Device load device 15");
#endif // MAX_TMK_NUMBER > 7
#if MAX_TMK_NUMBER > 15
MODULE_PARM_DESC(a16,"ISA addr1 device 16");
MODULE_PARM_DESC(b16,"ISA addr2 device 16");
MODULE_PARM_DESC(i16,"ISA interrupt device 16");
MODULE_PARM_DESC(d16,"PCI card number device 16");
MODULE_PARM_DESC(e16,"PCI device on PCI card number device 16");
MODULE_PARM_DESC(t16,"Device type device 16");
MODULE_PARM_DESC(l16,"Device load device 16");
MODULE_PARM_DESC(a17,"ISA addr1 device 17");
MODULE_PARM_DESC(b17,"ISA addr2 device 17");
MODULE_PARM_DESC(i17,"ISA interrupt device 17");
MODULE_PARM_DESC(d17,"PCI card number device 17");
MODULE_PARM_DESC(e17,"PCI device on PCI card number device 17");
MODULE_PARM_DESC(t17,"Device type device 17");
MODULE_PARM_DESC(l17,"Device load device 17");
MODULE_PARM_DESC(a18,"ISA addr1 device 18");
MODULE_PARM_DESC(b18,"ISA addr2 device 18");
MODULE_PARM_DESC(i18,"ISA interrupt device 18");
MODULE_PARM_DESC(d18,"PCI card number device 18");
MODULE_PARM_DESC(e18,"PCI device on PCI card number device 18");
MODULE_PARM_DESC(t18,"Device type device 18");
MODULE_PARM_DESC(l18,"Device load device 18");
MODULE_PARM_DESC(a19,"ISA addr1 device 19");
MODULE_PARM_DESC(b19,"ISA addr2 device 19");
MODULE_PARM_DESC(i19,"ISA interrupt device 19");
MODULE_PARM_DESC(d19,"PCI card number device 19");
MODULE_PARM_DESC(e19,"PCI device on PCI card number device 19");
MODULE_PARM_DESC(t19,"Device type device 19");
MODULE_PARM_DESC(l19,"Device load device 19");
MODULE_PARM_DESC(a20,"ISA addr1 device 20");
MODULE_PARM_DESC(b20,"ISA addr2 device 20");
MODULE_PARM_DESC(i20,"ISA interrupt device 20");
MODULE_PARM_DESC(d20,"PCI card number device 20");
MODULE_PARM_DESC(e20,"PCI device on PCI card number device 20");
MODULE_PARM_DESC(t20,"Device type device 20");
MODULE_PARM_DESC(l20,"Device load device 20");
MODULE_PARM_DESC(a21,"ISA addr1 device 21");
MODULE_PARM_DESC(b21,"ISA addr2 device 21");
MODULE_PARM_DESC(i21,"ISA interrupt device 21");
MODULE_PARM_DESC(d21,"PCI card number device 21");
MODULE_PARM_DESC(e21,"PCI device on PCI card number device 21");
MODULE_PARM_DESC(t21,"Device type device 21");
MODULE_PARM_DESC(l21,"Device load device 21");
MODULE_PARM_DESC(a22,"ISA addr1 device 22");
MODULE_PARM_DESC(b22,"ISA addr2 device 22");
MODULE_PARM_DESC(i22,"ISA interrupt device 22");
MODULE_PARM_DESC(d22,"PCI card number device 22");
MODULE_PARM_DESC(e22,"PCI device on PCI card number device 22");
MODULE_PARM_DESC(t22,"Device type device 22");
MODULE_PARM_DESC(l22,"Device load device 22");
MODULE_PARM_DESC(a23,"ISA addr1 device 23");
MODULE_PARM_DESC(b23,"ISA addr2 device 23");
MODULE_PARM_DESC(i23,"ISA interrupt device 23");
MODULE_PARM_DESC(d23,"PCI card number device 23");
MODULE_PARM_DESC(e23,"PCI device on PCI card number device 23");
MODULE_PARM_DESC(t23,"Device type device 23");
MODULE_PARM_DESC(l23,"Device load device 23");
MODULE_PARM_DESC(a24,"ISA addr1 device 24");
MODULE_PARM_DESC(b24,"ISA addr2 device 24");
MODULE_PARM_DESC(i24,"ISA interrupt device 24");
MODULE_PARM_DESC(d24,"PCI card number device 24");
MODULE_PARM_DESC(e24,"PCI device on PCI card number device 24");
MODULE_PARM_DESC(t24,"Device type device 24");
MODULE_PARM_DESC(l24,"Device load device 24");
MODULE_PARM_DESC(a25,"ISA addr1 device 25");
MODULE_PARM_DESC(b25,"ISA addr2 device 25");
MODULE_PARM_DESC(i25,"ISA interrupt device 25");
MODULE_PARM_DESC(d25,"PCI card number device 25");
MODULE_PARM_DESC(e25,"PCI device on PCI card number device 25");
MODULE_PARM_DESC(t25,"Device type device 25");
MODULE_PARM_DESC(l25,"Device load device 25");
MODULE_PARM_DESC(a26,"ISA addr1 device 26");
MODULE_PARM_DESC(b26,"ISA addr2 device 26");
MODULE_PARM_DESC(i26,"ISA interrupt device 26");
MODULE_PARM_DESC(d26,"PCI card number device 26");
MODULE_PARM_DESC(e26,"PCI device on PCI card number device 26");
MODULE_PARM_DESC(t26,"Device type device 26");
MODULE_PARM_DESC(l26,"Device load device 26");
MODULE_PARM_DESC(a27,"ISA addr1 device 27");
MODULE_PARM_DESC(b27,"ISA addr2 device 27");
MODULE_PARM_DESC(i27,"ISA interrupt device 27");
MODULE_PARM_DESC(d27,"PCI card number device 27");
MODULE_PARM_DESC(e27,"PCI device on PCI card number device 27");
MODULE_PARM_DESC(t27,"Device type device 27");
MODULE_PARM_DESC(l27,"Device load device 27");
MODULE_PARM_DESC(a28,"ISA addr1 device 28");
MODULE_PARM_DESC(b28,"ISA addr2 device 28");
MODULE_PARM_DESC(i28,"ISA interrupt device 28");
MODULE_PARM_DESC(d28,"PCI card number device 28");
MODULE_PARM_DESC(e28,"PCI device on PCI card number device 28");
MODULE_PARM_DESC(t28,"Device type device 28");
MODULE_PARM_DESC(l28,"Device load device 28");
MODULE_PARM_DESC(a29,"ISA addr1 device 29");
MODULE_PARM_DESC(b29,"ISA addr2 device 29");
MODULE_PARM_DESC(i29,"ISA interrupt device 29");
MODULE_PARM_DESC(d29,"PCI card number device 29");
MODULE_PARM_DESC(e29,"PCI device on PCI card number device 29");
MODULE_PARM_DESC(t29,"Device type device 29");
MODULE_PARM_DESC(l29,"Device load device 29");
MODULE_PARM_DESC(a30,"ISA addr1 device 30");
MODULE_PARM_DESC(b30,"ISA addr2 device 30");
MODULE_PARM_DESC(i30,"ISA interrupt device 30");
MODULE_PARM_DESC(d30,"PCI card number device 30");
MODULE_PARM_DESC(e30,"PCI device on PCI card number device 30");
MODULE_PARM_DESC(t30,"Device type device 30");
MODULE_PARM_DESC(l30,"Device load device 30");
MODULE_PARM_DESC(a31,"ISA addr1 device 31");
MODULE_PARM_DESC(b31,"ISA addr2 device 31");
MODULE_PARM_DESC(i31,"ISA interrupt device 31");
MODULE_PARM_DESC(d31,"PCI card number device 31");
MODULE_PARM_DESC(e31,"PCI device on PCI card number device 31");
MODULE_PARM_DESC(t31,"Device type device 31");
MODULE_PARM_DESC(l31,"Device load device 31");
#endif // MAX_TMK_NUMBER > 15

int RegInit(int hTMK, UINT tmkPorts1, UINT tmkPorts2, UINT tmkIrq1, int tmkDev, int tmkDevExt, char *pszType, char *pszLoad);

int RegInitAll(void)
{
  int icfg, id, ie;

  for (icfg = 0; icfg <= MAX_TMK_NUMBER; ++icfg)
    aManCfgDrv[icfg] = 0;
  for (id = 0; id < MAX_PCI_SLOTS; ++id)
    aManCfgPci[id] = 0;

  if (RegInit(0, a0, b0, i0, d0, e0, t0, l0) < 0 ||
      RegInit(1, a1, b1, i1, d1, e1, t1, l1) < 0 ||
      RegInit(2, a2, b2, i2, d2, e2, t2, l2) < 0 ||
      RegInit(3, a3, b3, i3, d3, e3, t3, l3) < 0 ||
      RegInit(4, a4, b4, i4, d4, e4, t4, l4) < 0 ||
      RegInit(5, a5, b5, i5, d5, e5, t5, l5) < 0 ||
      RegInit(6, a6, b6, i6, d6, e6, t6, l6) < 0 ||
      RegInit(7, a7, b7, i7, d7, e7, t7, l7) < 0
#if MAX_TMK_NUMBER > 7
      ||
      RegInit(8, a8, b8, i8, d8, e8, t8, l8) < 0 ||
      RegInit(9, a9, b9, i9, d9, e9, t9, l9) < 0 ||
      RegInit(10, a10, b10, i10, d10, e10, t10, l10) < 0 ||
      RegInit(11, a11, b11, i11, d11, e11, t11, l11) < 0 ||
      RegInit(12, a12, b12, i12, d12, e12, t12, l12) < 0 ||
      RegInit(13, a13, b13, i13, d13, e13, t13, l13) < 0 ||
      RegInit(14, a14, b14, i14, d14, e14, t14, l14) < 0 ||
      RegInit(15, a15, b15, i15, d15, e15, t15, l15) < 0
#endif // MAX_TMK_NUMBER > 7
#if MAX_TMK_NUMBER > 15
      ||
      RegInit(16, a16, b16, i16, d16, e16, t16, l16) < 0 ||
      RegInit(17, a17, b17, i17, d17, e17, t17, l17) < 0 ||
      RegInit(18, a18, b18, i18, d18, e18, t18, l18) < 0 ||
      RegInit(19, a19, b19, i19, d19, e19, t19, l19) < 0 ||
      RegInit(20, a20, b20, i20, d20, e20, t20, l20) < 0 ||
      RegInit(21, a21, b21, i21, d21, e21, t21, l21) < 0 ||
      RegInit(22, a22, b22, i22, d22, e22, t22, l22) < 0 ||
      RegInit(23, a23, b23, i23, d23, e23, t23, l23) < 0 ||
      RegInit(24, a24, b24, i24, d24, e24, t24, l24) < 0 ||
      RegInit(25, a25, b25, i25, d25, e25, t25, l25) < 0 ||
      RegInit(26, a26, b26, i26, d26, e26, t26, l26) < 0 ||
      RegInit(27, a27, b27, i27, d27, e27, t27, l27) < 0 ||
      RegInit(28, a28, b28, i28, d28, e28, t28, l28) < 0 ||
      RegInit(29, a29, b29, i29, d29, e29, t29, l29) < 0 ||
      RegInit(30, a30, b30, i30, d30, e30, t30, l30) < 0 ||
      RegInit(31, a31, b31, i31, d31, e31, t31, l31) < 0
#endif // MAX_TMK_NUMBER > 15
      )
    return -1;
  if (autopci)
  {
    id = 0;
    ie = 0;
    for (icfg = 0; icfg <= MAX_TMK_NUMBER; ++icfg)
    {
      if (aManCfgDrv[icfg])
        continue;
      while (ie == 3 || (id < MAX_PCI_SLOTS && aManCfgPci[id] > 0))
      {
        ++id;
        ie = 0;
      }
      if (id >= MAX_PCI_SLOTS)
        break;
      if (RegInit(icfg, 0xFFFF, 0xFFFF, 0xFF, id+1, ie+1, (char*)1, 0) < 0)
      {
        if (ie == 0)
          break; // no more PCI cards
        ie = 3;
        --icfg;
      }
      else
        ++ie;
    }
  }
  return 1;
}
