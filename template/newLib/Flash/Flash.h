/*
 * Flash.h
 *
 *  Created on: 2014年7月4日
 *      Author: Administrator
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "sys.h"
#include "stm32f10x_flash.h"
#define STM32_FLASH_SIZE 512 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
#define STM32_FLASH_BASE 				0x08000000 	//STM32 FLASH的起始地址
#define DEFAULT_FLASH_SAVE_ADDR 		0X08070000
/*
 * 注意：
 *地址只能是偶数
 *数据的字节数最好(拜托)是“偶数”：
 *：如果是奇数：
 *：		写/读：默认把字节数+1（危险：读的时候可能会改变未定义地址的数据，写的时候可能会读取未定义地址的数据）
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

