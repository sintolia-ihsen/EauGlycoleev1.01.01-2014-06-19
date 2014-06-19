#if !defined(EXCEPTION_H)
#define EXCEPTION_H
// declared static in case exception condition would prevent
// auto variable being created

typedef enum {
    EXCEP_IRQ = 0, // interrupt
    EXCEP_AdEL = 4, // address error exception (load or ifetch)
    EXCEP_AdES, // address error exception (store)
    EXCEP_IBE, // bus error (ifetch)
    EXCEP_DBE, // bus error (load/store)
    EXCEP_Sys, // syscall
    EXCEP_Bp, // breakpoint
    EXCEP_RI, // reserved instruction
    EXCEP_CpU, // coprocessor unusable
    EXCEP_Overflow, // arithmetic overflow
    EXCEP_Trap, // trap (possible divide by zero)
    EXCEP_IS1 = 16, // implementation specfic 1
    EXCEP_CEU, // CorExtend Unuseable
    EXCEP_C2E // coprocessor 2
} _Excep_Code;

extern _Excep_Code _excep_code __attribute__((persistent));
extern unsigned int _excep_addr __attribute__((persistent));
extern unsigned int _cpt_reset __attribute__((persistent));

#endif //!defined(EXCEPTION_H)
