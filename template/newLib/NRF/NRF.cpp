/*
 * NRF.cpp
 *
 *  Created on: 2014年7月5日
 *      Author: 2cats
 */
/*
24L01 C++封装
如果需要中断接收需额外配置外部中断为下降沿触发
Example:

#include "exampleA.h"
#include "exampleB.h"


struct AA
{
	char x[1024];
};
AA aa;
int count;
int interg;
Package package; 
void receive_proc(Package *package)
{

		switch(package->type)
		{
			case 0x99:
				interg=parse(int,package->data);
				break;
			case 0x76:
				aa=parse(AA,package->data);
				break;
		}
		count++;

}

	delay_init();//MAY USE IN SENDING
//中断配置
	EXTIManagerTypeDef EXTIManager;
	EXTIManager_Structure(&EXTIManager);
	EXTIManager.startListenling(GPIOG,GPIO_Pin_8,EXTI_Falling,NRF_RecvHandler);

	NRF nrf;	//default NRF_RX_MODE//创建 stream
	Postman postman(&nrf);//创建 postman
	nrf.setCallback(&postman,receive_proc);//通过中断及时调用  _NRF_defaultPostman_p.receive

////SEND
//	NRF nrf(NRF_TX_MODE);
//	Postman postman(&nrf);
//	postman.send(detail(interg),0x99);
//	postman.send(detail(aa),0x76);

只支持1个NRF对象和1个postman（不影响其它类型的postman）
测试速度：40kb/s  [flush无延时的情况下]
*/
#include "nrf/NRF.h"
#include "delay.h"
#include "postman/postman.h"
static u8 NRF_Txbuf[NRF_BUF_LEN];
static int NRF_tp;
static u8 NRF_Rxbuf[NRF_BUF_LEN];
static Buffer NRF_rxBuf(NRF_Rxbuf,NRF_BUF_LEN);//circle buf
static u8 _NRF_bufI[RX_PLOAD_WIDTH];//32byte buf
void(*NRF_callback)(Package *package);
Postman *_NRF_defaultPostman_p=0;
NRF::NRF(u8 mode){
	// TODO Auto-generated constructor stub
	NRF_tp  = 0;
	NRF24L01_Init();
	setMode(mode);
	NRF_callback=0;
}

NRF::~NRF() {
	// TODO Auto-generated destructor stub
}
bool NRF::write(uint8_t data) {
	if (NRF_tp < NRF_BUF_LEN) {
		NRF_Txbuf[NRF_tp++] = data;
		return true;
	} else
		return false;
}
bool NRF::write(uint8_t *data, int length) {
	if (NRF_tp + length - 1 < NRF_BUF_LEN) {
		int i;
		for (i = 0; i < length; i++) {
			write(data[i]);
		}
		return true;
	} else
		return false;
}
uint8_t NRF::read(void) {
	if(available())
	{
		return NRF_rxBuf.pop();
	}else
		return 0xff;
}
bool NRF::read(uint8_t *data, int length) {
	if(available()>=length)
	{
		NRF_rxBuf.pop(data,length);
		return true;
	}else
	{
		return false;
	}
}
int NRF::available() {
	return NRF_rxBuf.length();
}
void NRF::flush() {
	int i=0;
	int count=(NRF_tp/32)+((NRF_tp%32)!=0);
	for(i=0;i<count;i++)
	{
		NRF24L01_TxPacket(NRF_Txbuf+i*32);
		//delay_ms(1);
	}
	NRF_tp=0;
}
void NRF::setMode(u8 mode)
{
	if(mode==NRF_RX_MODE)
	{
		NRF24L01_RX_Mode();
	}else if(mode==NRF_TX_MODE)
	{
		NRF24L01_TX_Mode();
	}
}

void NRF::setCallback(Postman *postman_p,void(*cb)(Package *package))
{
	_NRF_defaultPostman_p=postman_p;
	NRF_callback=cb;
}
inline void NRF_Postman_Parse()
{
	Package package;
	while(_NRF_defaultPostman_p->receive(&package))
	{
		NRF_callback(&package);
	}
}
void NRF_RecvHandler(struct EXTILineType *triggeredLine)
{
	//may need some switch for triggeredLine
	if(NRF24L01_Read_Reg(STATUS)&RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,_NRF_bufI,RX_PLOAD_WIDTH);//读取数据

			NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除RX FIFO寄存器
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器
				NRF24L01_ClearFlag(MASK_RX_DR);
		NRF_rxBuf.pushback(_NRF_bufI,RX_PLOAD_WIDTH);
		if(NRF_callback!=0)
			NRF_Postman_Parse();
	
			NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除RX FIFO寄存器
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器
			NRF24L01_ClearFlag(MASK_RX_DR);
	}
}

