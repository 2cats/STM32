#ifndef __SUPERTASK_H
#define __SUPERTASK_H
#define SVC_U8_U8_TYPE 			  0x03
#define SVC_U8_VOID_TYPE		  0x04
#define SVC_VOID_U8_TYPE 			0x05
#define SVC_VOID_VOID_TYPE 		0x06
typedef	 unsigned char(*SVC_U8_U8_FuncDef)(unsigned char);
typedef	 unsigned char(*SVC_U8_VOID_FuncDef)(void);
typedef	 void(*SVC_VOID_U8_FuncDef)(unsigned char);
typedef	 void(*SVC_VOID_VOID_FuncDef)(void);

typedef  unsigned long(*SVCDefaultFuncTypeDef)(unsigned long svc_r0, unsigned long svc_r1, unsigned long svc_r2, unsigned long svc_r3);

unsigned long __svc(SVC_U8_U8_TYPE) doTaskInSuperMode_88(void*funcP, void* arg);
unsigned long __svc(SVC_U8_VOID_TYPE) doTaskInSuperMode_8v(void*funcP, void*arg);
unsigned long __svc(SVC_VOID_U8_TYPE) doTaskInSuperMode_v8(void*funcP, void*arg);
unsigned long __svc(SVC_VOID_VOID_TYPE) doTaskInSuperMode_vv(void*funcP,void*arg);
#endif
