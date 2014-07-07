/*
 * Flash.h
 *
 *  Created on: 2014��7��4��
 *      Author: Administrator
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "sys.h"
#include "stm32f10x_flash.h"
#define STM32_FLASH_SIZE 512 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
#define STM32_FLASH_BASE 				0x08000000 	//STM32 FLASH����ʼ��ַ
#define DEFAULT_FLASH_SAVE_ADDR 		0X08070000
/*
 * ע�⣺
 *��ַֻ����ż��
 *���ݵ��ֽ������(����)�ǡ�ż������
 *�������������
 *��		д/����Ĭ�ϰ��ֽ���+1��Σ�գ�����ʱ����ܻ�ı�δ�����ַ�����ݣ�д��ʱ����ܻ��ȡδ�����ַ�����ݣ�
 */
#ifdef __cplusplus
extern "C" {
#endif
class Flash {
public:
	Flash();
	void write(u32 WriteAddr, void *_data, u16 _NumToWrite);
	void read(u32 ReadAddr, void *_data, u16 _NumToRead);
	virtual ~Flash();
}
;
#ifdef __cplusplus
}
#endif
#endif /* FLASH_H_ */

