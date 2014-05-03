#include "easyFATFS.h"

/*********INTERFACE*************/
/*
Example:
	FATFS fs;
	FIL fp;
	UINT num;
	u32 total;
	u32 free;
	char buffer[10];
//����
	f_mount(0, &fs);
//�ռ��ѯ
exf_getfree("0:",&total,&free);
printf("TOTAL:%dKB\tFREE:%dKB\n",total,free);
//���ļ�
	f_open(&fp, "0:serial.txt", FA_OPEN_ALWAYS | FA_WRITE ); 
//д1���ַ�
	f_putc(buffer[0],&fp);
//д����ַ�
	f_write(&fp,buffer,sizeof(buffer),&num);
//�ر��ļ�
	f_close(&fp);
	
	
*/








//�õ�����ʣ������
//drv:���̱��("0:"/"1:")
//total:������	 ����λKB��
//free:ʣ������	 ����λKB��
//����ֵ:0,����.����,�������
u8 exf_getfree(u8 *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	u8 res;
    DWORD fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//�õ���������
	    fre_sect=fre_clust*fs1->csize;			//�õ�����������	    
		*total=tot_sect>>1;	//��λΪKB
		*free=fre_sect>>1;	//��λΪKB 
 	}
	return res;
}	
