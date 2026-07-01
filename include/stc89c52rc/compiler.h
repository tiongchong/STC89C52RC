#ifndef STC89C52RC_COMPILER_H
#define STC89C52RC_COMPILER_H

#if defined(SDCC) || defined(__SDCC)
#define STC_ISR(vector) __interrupt(vector)
#define STC_NAKED __naked
#define STC_REENTRANT __reentrant
#define STC_IDATA __idata
#define STC_XDATA __xdata
#define STC_CODE __code
#else
#define STC_ISR(vector)
#define STC_NAKED
#define STC_REENTRANT
#define STC_IDATA
#define STC_XDATA
#define STC_CODE
#endif

#endif
