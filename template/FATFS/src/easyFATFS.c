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
//挂载
	f_mount(0, &fs);
//空间查询
exf_getfree("0:",&total,&free);
printf("TOTAL:%dKB\tFREE:%dKB\n",total,free);
//打开文件
	f_open(&fp, "0:serial.txt", FA_OPEN_ALWAYS | FA_WRITE ); 
//写1个字符
	f_putc(buffer[0],&fp);
//写多个字符
	f_write(&fp,buffer,sizeof(buffer),&num);
//关闭文件
	f_close(&fp);
	
	
*/








//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量	 （单位KB）
//free:剩余容量	 （单位KB）
//返回值:0,正常.其他,错误代码
u8 exf_getfree(u8 *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	u8 res;
    DWORD fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	    
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB 
 	}
	return res;
}	
