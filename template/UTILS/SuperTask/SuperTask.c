#include "SuperTask.h"
//�˴�����ʩ��...

// ʹ��Cд�ɵ�SVC�������̣�����һ��ָ�������pwdSF������ջջ����ʼ��ַ�� 
// pwdSF[0] = R0 , pwdSF[1] = R1 
// pwdSF[2] = R2 , pwdSF[3] = R3 
// pwdSF[4] = R12, pwdSF[5] = LR 
// pwdSF[6] = ���ص�ַ����ջ��PC�� 
// pwdSF[7] = xPSR 
unsigned long SVC_CHandler(unsigned int* pwdSF)
{
	unsigned int svc_number; 
	unsigned int svc_r0; 
	unsigned int svc_r1; 
//	unsigned int svc_r2; 
//	unsigned int svc_r3; 
	int retVal; //���ڴ洢����ֵ 
	svc_number = ((char *) pwdSF[6])[-2]; 
	svc_r0 = ((unsigned long) pwdSF[0]);
	svc_r1 = ((unsigned long) pwdSF[1]); 
//	svc_r2 = ((unsigned long) pwdSF[2]); 
//	svc_r3 = ((unsigned long) pwdSF[3]);
	switch(svc_number)
	{
		case SVC_U8_U8_TYPE:
			retVal=((SVC_U8_U8_FuncDef)(svc_r0))(*((unsigned char*)svc_r1));
			break;
		case SVC_U8_VOID_TYPE:
			 retVal=((SVC_U8_VOID_FuncDef)svc_r0)();
			break;
		case SVC_VOID_U8_TYPE:
			((SVC_VOID_U8_FuncDef)(svc_r0))(*((unsigned char*)svc_r1));
			break;
		case SVC_VOID_VOID_TYPE:
			((SVC_VOID_VOID_FuncDef)(svc_r0))();
			break;
	}
	pwdSF[0]=retVal;
	return 0;
}
  __asm void SVC_Handler(void)   
{   
	IMPORT SVC_CHandler
	TST LR, #4   
	ITE EQ   
	MRSEQ R0, MSP   
	MRSNE R0, PSP   
	B SVC_CHandler  
}
