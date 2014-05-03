;********************************************************************************************************
;                                                uC/CPU
;                                    CPU CONFIGURATION & PORT LAYER
;
;                          (c) Copyright 2004-2011; Micrium, Inc.; Weston, FL
;
;               All rights reserved.  Protected by international copyright laws.
;
;               uC/CPU is provided in source form to registered licensees ONLY.  It is 
;               illegal to distribute this source code to any third party unless you receive 
;               written permission by an authorized Micrium representative.  Knowledge of 
;               the source code may NOT be used to develop a similar product.
;
;               Please help us continue to provide the Embedded community with the finest 
;               software available.  Your honesty is greatly appreciated.
;
;               You can contact us at www.micrium.com.
;********************************************************************************************************

;********************************************************************************************************
;
;                                            CPU PORT FILE
;
;                                            ARM-Cortex-M3
;                                            IAR C Compiler
;
; Filename      : cpu_a.asm
; Version       : V1.28.01.00
; Programmer(s) : JJL
;********************************************************************************************************


;********************************************************************************************************
;                                           EXPORT FUNCTIONS
;********************************************************************************************************

        EXPORT  CPU_IntDis
        EXPORT  CPU_IntEn

        EXPORT  CPU_SR_Save
        EXPORT  CPU_SR_Restore

        EXPORT  CPU_CntLeadZeros
        EXPORT  CPU_RevBits

        EXPORT  CPU_WaitForInt
        EXPORT  CPU_WaitForExcept


;********************************************************************************************************
;                                      CODE GENERATION DIRECTIVES
;********************************************************************************************************

        AREA |.text|, CODE, READONLY, ALIGN=2
		THUMB
		REQUIRE8
		PRESERVE8

;$PAGE
;********************************************************************************************************
;                                    DISABLE and ENABLE INTERRUPTS
;
; Description: Disable/Enable interrupts.
;
; Prototypes : void  CPU_IntDis(void);
;              void  CPU_IntEn (void);
;********************************************************************************************************

CPU_IntDis
        CPSID   I
        BX      LR


CPU_IntEn
        CPSIE   I
        BX      LR


;********************************************************************************************************
;                                      CRITICAL SECTION FUNCTIONS
;
; Description : Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking, the
;               state of the interrupt disable flag is stored in the local variable 'cpu_sr' & interrupts
;               are then disabled ('cpu_sr' is allocated in all functions that need to disable interrupts).
;               The previous interrupt state is restored by copying 'cpu_sr' into the CPU's status register.
;
; Prototypes  : CPU_SR  CPU_SR_Save   (void);
;               void    CPU_SR_Restore(CPU_SR  cpu_sr);
;
; Note(s)     : (1) These functions are used in general like this :
;
;                       void  Task (void  *p_arg)
;                       {
;                           CPU_SR_ALLOC();                     /* Allocate storage for CPU status register */
;                               :
;                               :
;                           CPU_CRITICAL_ENTER();               /* cpu_sr = CPU_SR_Save();                  */
;                               :
;                               :
;                           CPU_CRITICAL_EXIT();                /* CPU_SR_Restore(cpu_sr);                  */
;                               :
;                       }
;********************************************************************************************************

CPU_SR_Save
        MRS     R0, PRIMASK                     ; Set prio int mask to mask all (except faults)
        CPSID   I
        BX      LR


CPU_SR_Restore                                  ; See Note #2.
        MSR     PRIMASK, R0
        BX      LR


;$PAGE
;********************************************************************************************************
;                                         CPU_CntLeadZeros()
;                                        COUNT LEADING ZEROS
;
; Description : Counts the number of contiguous, most-significant, leading zero bits before the first 
;                   binary one bit in a data value.
;
; Prototype   : CPU_DATA  CPU_CntLeadZeros(CPU_DATA  val);
;
; Argument(s) : val         Data value to count leading zero bits.
;
; Return(s)   : Number of contiguous, most-significant, leading zero bits in 'val'.
;
; Caller(s)   : Application.
;
;               This function is an INTERNAL CPU module function but MAY be called by application function(s).
;
; Note(s)     : (1) If the argument is zero, the value 32 is returned.
;
;               (2) MUST be implemented in cpu_a.asm if and only if CPU_CFG_LEAD_ZEROS_ASM_PRESENT is 
;                   #define'd in 'cpu_cfg.h' or 'cpu.h'.
;********************************************************************************************************

CPU_CntLeadZeros
        CLZ     R0, R0                          ; Count leading zeros
        BX      LR


;********************************************************************************************************
;                                             REVERSE BITS
;
; Description : Reverses the bits in the argument.
;
; Prototypes  : CPU_DATA  CPU_RevBits (CPU_DATA  val)
;
; Argument(s) : val     variable to reverse
;********************************************************************************************************

CPU_RevBits
        RBIT    R0, R0                          ; Reverse bits
        BX      LR


;$PAGE
;********************************************************************************************************
;                                         WAIT FOR INTERRUPT
;
; Description : Enters sleep state, which will be exited when an interrupt is received.
;
; Prototypes  : void  CPU_WaitForInt (void)
;
; Argument(s) : none.
;********************************************************************************************************

CPU_WaitForInt
        WFI                                     ; Wait for interrupt
        BX      LR


;********************************************************************************************************
;                                         WAIT FOR EXCEPTION
;
; Description : Enters sleep state, which will be exited when an exception is received.
;
; Prototypes  : void  CPU_WaitForExcept (void)
;
; Argument(s) : none.
;********************************************************************************************************

CPU_WaitForExcept
        WFE                                     ; Wait for exception
        BX      LR


;$PAGE
;********************************************************************************************************
;                                     CPU ASSEMBLY PORT FILE END
;********************************************************************************************************

        END

