/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : LCDConf_1375_C8_C320x240.h
Purpose     : Sample configuration file
----------------------------------------------------------------------
*/

#ifndef LCDCONF_H
#define LCDCONF_H

/*********************************************************************
*
*                   General configuration of LCD
*
**********************************************************************
*/

#define LCD_XSIZE          (128)		/* 320 -> 400 */
#define LCD_YSIZE          (64)		/* 240 */
#define LCD_CONTROLLER     (-1)		/* 9320 -> 5420 */
#define LCD_BITSPERPIXEL   (1)
#define LCD_FIXEDPALETTE   (1)
#define LCD_SWAP_RB        (1)		   //ºìÀ¶·´É«½»»»
#define LCD_REVERSE				 (1)
//#define LCD_SWAP_XY        (1)
//LCD_MIRROR_X
#define LCD_INIT_CONTROLLER()  LCD_hard_Init();
#endif /* LCDCONF_H */

 
