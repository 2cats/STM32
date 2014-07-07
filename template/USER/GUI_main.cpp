#include <stdlib.h>
#include <string.h>
#include "DIALOG.h"
#include "GRAPH.h"
#include "MENU.h"
#include "math.h"
#include "messagebox.h"
#define  GRAPH_ID0    1 //��Ϊ0���ܻ��mainWidget��id��ͻ
GUI_WIDGET_CREATE_INFO mainWidget[]={
    {FRAMEWIN_CreateIndirect,"mainWidget",0,0,0,320,320},//FRAMEWIN�����һ��
    {GRAPH_CreateIndirect,0,GRAPH_ID0,5,5,200,150},//��FRAMEWIN�е��������
};
    
void addItem(MENU_Handle menu,MENU_Handle sub,const char *text,int Id)
{
    MENU_ITEM_DATA item;
    item.pText=text;
    item.hSubmenu=sub;
    item.Id      = Id;//menu_data=(MENU_MSG_DATA *)pMsg->Data.p;menu_data->ItemId
    MENU_AddItem(menu,&item);//�ֲ�item���������󣬾�û�����ˣ������޸��������ٴ������item
}
void addMenu(WM_HWIN mainH)
{
    MENU_Handle rootMenuH,FileMenuH,EditMenuH,HelpMenuH;
//MENU_CreateEx����������һ��menu��Ȼ�����menu���item
//һ��menu�൱��һ������
//�����Ҫ�༶menu����ôMENU_AddItem()���MENU_ITEM_DATA����ͱ���ҪΪhSubmenu��ֵ��emWIN�ǲ����Զ�����Ӳ˵��ģ���
//size�Զ�����
    rootMenuH=MENU_CreateEx(0,0,0,0,0,0,MENU_CF_HORIZONTAL,0);
    FileMenuH=MENU_CreateEx(0,0,0,0,0,0,MENU_CF_VERTICAL,0);
    EditMenuH=MENU_CreateEx(0,0,0,0,0,0,MENU_CF_VERTICAL,0);
    HelpMenuH=MENU_CreateEx(0,0,0,0,0,0,MENU_CF_VERTICAL,0);


    addItem(FileMenuH,0,"Open",0);
    addItem(FileMenuH,0,"Save",1);
    addItem(FileMenuH,0,"Close",2);

    addItem(EditMenuH,0,"revert",0);

    addItem(HelpMenuH,0,"About",0);
    addItem(HelpMenuH,0,"Exit",1);

    addItem(rootMenuH,FileMenuH,"File",0);
    addItem(rootMenuH,EditMenuH,"Edit",0);
    addItem(rootMenuH,HelpMenuH,"Help",0);
//!
    FRAMEWIN_AddMenu(mainH,rootMenuH);
}
void _MessageBox(const char * pText, const char * pCaption) {
  WM_HWIN hBox;
  hBox = MESSAGEBOX_Create(pText, pCaption, GUI_MESSAGEBOX_CF_MODAL | GUI_MESSAGEBOX_CF_MOVEABLE);
  GUI_ExecCreatedDialog(hBox);
}
static GRAPH_DATA_Handle  _ahData[3]; /* Array of handles for the GRAPH_DATA objects */
GUI_COLOR _aColor[] = {GUI_RED, GUI_GREEN, GUI_LIGHTBLUE};
void addGraph(WM_HWIN hGraph)
{
    int i;
    for (i = 0; i < GUI_COUNTOF(_aColor); i++) {//GUI_COUNTOF��sizeof/sizeof
        _ahData[i] = GRAPH_DATA_YT_Create(_aColor[i], 400, 0, 0);//400Ϊ����(��)����������������������ݻᱻ�������ݸ���
        GRAPH_AttachData(hGraph, _ahData[i]);
        //GRAPH_AttachData,�൱����������ߣ��Ժ�����GRAPH_DATA_Handle���͵�_ahData[i]�����ص��������������
    }
}
int debug_id;
WM_HWIN CreateInputDialog(void);
void main_cb(WM_MESSAGE * pMsg) {
    WM_HWIN mainH=pMsg->hWin;//dialog�ľ��Ҳ��WM_HWIN���ͣ�pMsg->hWin���ص����������ˣ�GUI_CreateDialogBox��
    WM_HWIN hGraph;
    MENU_MSG_DATA *menu_data;
    switch(pMsg->MsgId)
    {
        case WM_INIT_DIALOG://dialog�ĳ�ʼ��
            addMenu(mainH);
            hGraph=WM_GetDialogItem(mainH,GRAPH_ID0);//ʵ�ú���WM_GetDialogItem
            addGraph(hGraph);
        break;
        case WM_MENU://�����menu��dialog����ִ���Ϣ
            menu_data=(MENU_MSG_DATA *)pMsg->Data.p;//
            switch(menu_data->MsgType)
            {
            case MENU_ON_ITEMSELECT://ֻ��enable�˵�ѡ��Ż��д���Ϣ
                 debug_id=menu_data->ItemId;//ItemIdΪ��ȫ�֡��ģ���    item.Id       = Id  ��ָ����id�ţ���ͬ�˵�������ͬ����id���������ã�
                _MessageBox("ITEMSELECT","title");
                break;
            }
        default:
        WM_DefaultProc(pMsg);

    }
}
void _AddValues(WM_HWIN hGraph) {
  int i;
  static int wt=21;
  int value;
  for (i = 0; i < GUI_COUNTOF(_aColor); i++) {
    value=20*sin(0.01*(i+1)*wt)+80;
    wt++;
    GRAPH_DATA_YT_AddValue(_ahData[i],value);//����GRAPH_DATA_Handle�������(��y��һ����)��ͼ����Զ�����
  }
}

void MainTask(void) {
    WM_HWIN mainH;
    WM_HWIN hGraph;
   // GUI_Init();
    WM_SetCreateFlags(WM_CF_MEMDEV);//����˸
    mainH=GUI_CreateDialogBox(mainWidget,GUI_COUNTOF(mainWidget),main_cb,0,0,0);
    hGraph = WM_GetDialogItem(mainH, GUI_ID_GRAPH0);
  while (1) {
    GUI_Exec();
   // GUI_Delay(10);
    _AddValues(hGraph);
  }
}

/*************************** End of file ****************************/



////#include "MKeyScan/MKeyScan.h"
//#include "delay.h"
//#include "timer/Timer.h"
//#include "InputCatcher.h"
//#include "millis.h"
//#include "util/util.h"
//#include "gui.h"
//#include "touch.h" 
//#include "WM.h"
//#include "MENU.h"
//#include "FRAMEWIN.h"
//#include "mygui.h"
//#include "math.h"
//#include "flash/flash.h"
//#include "24l01.h"
//#include "EXTIManager.h"
//#include "spi.h"
//#define PIN_READ	(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_8))
//u8 tmp_buf[33];
//int x;
//u8 pin_sta;
//void doWhenTrigger( struct EXTILineType *triggeredLine)
//{
//	if(NRF24L01_Read_Reg(STATUS)&RX_OK)//���յ�����
//	{
//		NRF24L01_Read_Buf(RD_RX_PLOAD,tmp_buf,RX_PLOAD_WIDTH);//��ȡ����
//		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���RX FIFO�Ĵ��� 
//		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
//		NRF24L01_ClearFlag(MASK_RX_DR); 
//	}
//}

//int main()
//{	
////	Timer timer4(TIM4_Conf);
//	delay_init();
////	GUI_AllInit(&timer4,GUI_RED);
//	tmp_buf[0]='A';

//	EXTIManagerTypeDef EXTIManager;
//	delay_init();
//	EXTIManager_Structure(&EXTIManager);
//	//
//		EXTIManager.startListenling(GPIOG,GPIO_Pin_8,EXTI_Falling,doWhenTrigger);
//	NRF24L01_Init(); 	
//		NRF24L01_TX_Mode(); 
//	NRF24L01_TxPacket(tmp_buf);  
//	tmp_buf[0]='a';
//	NRF24L01_TxPacket(tmp_buf);  
//	NRF24L01_RX_Mode(); 
//	while(1)
//	{
//		pin_sta=NRF24L01_Read_Reg(STATUS);
//		pin_sta=NRF24L01_Read_Reg(CONFIG);
//		pin_sta=NRF24L01_Read_Reg(NRF_FIFO_STATUS);
//		delay_ms(500);
//	}
//}