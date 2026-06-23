#ifndef STC89C52RC_MCU_H
#define STC89C52RC_MCU_H

#if defined(SDCC) || defined(__SDCC)
#include <8052.h>
#else
#error "The STC89C52RC HAL must be compiled with SDCC for the mcs51 target."
#endif

#endif
