#ifndef STC89C52RC_COMPILER_H
#define STC89C52RC_COMPILER_H

#if defined(SDCC) || defined(__SDCC)
#define STC_ISR(vector) __interrupt(vector)
#define STC_NAKED __naked
#define STC_REENTRANT __reentrant
#else
#define STC_ISR(vector)
#define STC_NAKED
#define STC_REENTRANT
#endif

#endif
