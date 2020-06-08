#include "stm32f1xx_hal.h"
#include <string.h>

extern UART_HandleTypeDef huart3;

#define NEX_RET_CMD_FINISHED                 (0x01)
#define NEX_RET_EVENT_LAUNCHED               (0x88)
#define NEX_RET_EVENT_UPGRADED               (0x89)
#define NEX_RET_EVENT_TOUCH_HEAD             (0x65)     
#define NEX_RET_EVENT_POSITION_HEAD          (0x67)
#define NEX_RET_EVENT_SLEEP_POSITION_HEAD    (0x68)
#define NEX_RET_CURRENT_PAGE_ID_HEAD         (0x66)
#define NEX_RET_STRING_HEAD                  (0x70)
#define NEX_RET_NUMBER_HEAD                  (0x71)
#define NEX_RET_INVALID_CMD                  (0x00)
#define NEX_RET_INVALID_COMPONENT_ID         (0x02)
#define NEX_RET_INVALID_PAGE_ID              (0x03)
#define NEX_RET_INVALID_PICTURE_ID           (0x04)
#define NEX_RET_INVALID_FONT_ID              (0x05)
#define NEX_RET_INVALID_BAUD                 (0x11)
#define NEX_RET_INVALID_VARIABLE             (0x1A)
#define NEX_RET_INVALID_OPERATION            (0x1B)

char ENDTERMS[]={255,255,255};

void sendCommand(const char* cmd)
{
   
   while (__HAL_UART_GET_FLAG (&huart3, UART_FLAG_RXNE))
   {
      huart3.Instance->DR;
   }

   HAL_UART_Transmit (&huart3, (uint8_t*) cmd, strlen (cmd), 50);
   HAL_UART_Transmit (&huart3, (uint8_t*)&ENDTERMS, 3, 50);
}

int recvRetCommandFinished(void)
{
   
   int ret=0;
   uint8_t temp[4]={0};
   HAL_UART_Receive (&huart3, (uint8_t*)&temp, 4, 100);

   if (temp[0] == NEX_RET_CMD_FINISHED && temp[1]==0xFF && temp[2]==0xFF && temp[3]==0xFF)
   {
      ret=1;
   }

   return ret;
}

int recvRetNumber(void)
{
   int number=0;
   uint8_t temp[8]={0};
   HAL_UART_Receive (&huart3, (uint8_t*)&temp, 8, 50);

   if (temp[0] == NEX_RET_NUMBER_HEAD && temp[5]==0xFF && temp[6]==0xFF && temp[7]==0xFF )
   {
      number= ((uint32_t)temp[4]<<24)|((uint32_t)temp[3]<<16)|(temp[2]<<8)|(temp[1]);
   }

   return number;
}

int recvRetString(char* buffer,int len)
{
   uint16_t ret=0;
   int str_start_flag=0;
   uint8_t cnt_0xff=0;
   char temp[20]={0};
   char temp2[20]={0};
   int pointer=0, i=0;
   uint8_t c=0;
   HAL_UART_Receive (&huart3, (uint8_t*)&temp2, len+4, 50);
   for (i=0; i<len+4;  i++)
   {
      c=temp2[i];
      if (str_start_flag)
      {
         if (0xFF==c)
         {
            cnt_0xff++;
            if (cnt_0xff>=3)
            {
               break;
            }
         }
         else
         {
            temp[pointer]= (char) c;
            pointer++;
         }
      }
      else if (NEX_RET_STRING_HEAD==c)
      {
         str_start_flag=1;
      }
   }

   ret=ret>len?len : ret;
   strncpy (buffer, temp, pointer);
   return ret;
}

int nexInit(void)
{
   int ret1=0;
   int ret2=0;
   
   sendCommand ("");
   sendCommand ("bkcmd=1");
   ret1=recvRetCommandFinished  ()  ;
   sendCommand ("page 0");
   ret2=recvRetCommandFinished  ()  ;
   return ret1&&ret2;
}

int NexPageShow(int Page)
{
   char buffer[10]={0};
   sprintf (buffer, "page %d", Page);
   sendCommand (buffer);
   return recvRetCommandFinished ();
}

int NexTextSetText(int Text,const char *buffer)
{
   char cmd[50]={0}, buff[50]={0};
   for (int i=0; i<20; i++) buff[i]=buffer[i];
   sprintf (cmd, "t%d.txt=\"%s\"",  Text,  buff);
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexTextGetText( int Number ,char* buffer, int len  )
{
   char cmd[10]={0};
   sprintf (cmd, "get t%d.txt", Number);
   sendCommand (cmd);
   return recvRetString (buffer, len);
}

int NexNumberSetValue(int Number, int value)
{
   char cmd[15]={0};
   sprintf (cmd, "n%d.val=%d", Number, value) ;
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexNumberGetValue(int Number)
{
   char cmd[15]={0};
   sprintf (cmd, "get n%d.val", Number);
   sendCommand (cmd);
   return recvRetNumber ();
}

int NexXfloatSetValue(int Number, int value)
{
   char cmd[15]={0};
   sprintf (cmd, "x%d.val=%d", Number, value) ;
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexPictureSetPic(int pic, int value)
{
   char cmd[15]={0};
   sprintf (cmd, "p%d.pic=%d", pic, value);
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexButtonGetText(int Button, char *buffer, int len)
{
   char cmd[15]={0};
   sprintf (cmd, "get b%d.txt", Button);
   sendCommand (cmd);
   return recvRetString (buffer, len);
}

int NexButtonSetText(int Button, const char *buffer)
{
   char cmd[50]={0}, buff[50]={0};
   for (int i=0; i<strlen (buffer); i++) buff[i]=buffer[i];
   sprintf (cmd, "b%d.txt=\"%s\"",  Button,  buff);
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexCheckboxGetValue(int Checkbox)
{
   char cmd[15]={0};
   sprintf (cmd, "get c%d.val", Checkbox);
   sendCommand (cmd);
   return recvRetNumber ();
}

int NexCheckboxSetValue(int Checkbox,int value)
{
   char cmd[15]={0};
   sprintf (cmd,"c%d.val=%d",Checkbox,value) ;
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexDSButtonGetValue(int DSButton)
{
   char cmd[15]={0};
   sprintf (cmd,"get bt%d.val",DSButton);
   sendCommand (cmd);
   return recvRetNumber ();
}

int NexDSButtonSetValue(int DSButton,int value)
{
   char cmd[15]={0};
   sprintf (cmd,"bt%d.val=%d",DSButton,value) ;
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexDSButtonGetText(int DSButton, char *buffer, int len)
{
   char cmd[15]={0};
   sprintf (cmd,"get bt%d.txt",DSButton);
   sendCommand (cmd);
   return recvRetString (buffer, len);
}

int NexDSButtonSetText(int DSButton,const char *buffer)
{
   char cmd[50]={0}, buff[50]={0};
   for (int i=0; i<strlen  (buffer); i++) buff[i]=buffer[i];
   sprintf (cmd,"bt%d.txt=\"%s\"",DSButton,buff);
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexGaugeGetValue(int Gauge)
{
   char cmd[15]={0};
   sprintf (cmd,"get z%d.val",Gauge);
   sendCommand (cmd);
   return recvRetNumber ();
}

int  NexGaugeSetValue(int Gauge,int value)
{
   char cmd[15]={0};
   sprintf (cmd,"z%d.val=%d",Gauge,value) ;
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexProgressBarGetValue(int ProgressBar)
{
   char cmd[15]={0};
   sprintf (cmd,"get j%d.val",ProgressBar);
   sendCommand (cmd);
   return recvRetNumber ();
}

int NexProgressBarSetValue(int ProgressBar,int value)
{
   char cmd[15]={0};
   sprintf (cmd,"j%d.val=%d",ProgressBar,value) ;
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexRadioGetValue(int Radio)
{
   char cmd[15]={0};
   sprintf (cmd,"get r%d.val",Radio);
   sendCommand (cmd);
   return recvRetNumber ();
}

int  NexRadioSetValue(int Radio,int value)
{
   char cmd[15]={0};
   sprintf (cmd,"r%d.val=%d",Radio,value) ;
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexVariableGetValue(int Variable)
{
   char cmd[15]={0};
   sprintf (cmd,"get va%d.val",Variable);
   sendCommand (cmd);
   return recvRetNumber ();
}

int NexVariableSetValue(int Variable,int value )
{
   char cmd[15]={0};
   sprintf (cmd,"va%d.val=%d",Variable,value) ;
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

int NexVariableGetText(int Variable, char *buffer, uint32_t len)
{
   char cmd[15]={0};
   sprintf (cmd,"get va%d.txt",Variable);
   sendCommand (cmd);
   return recvRetString (buffer,len);
}

int NexVariableSetText(int Variable,const char *buffer)
{
   char cmd[50]={0}, buff[50]={0};
   for (int i=0; i<strlen(buffer);i++) buff[i]=buffer[i];
   sprintf (cmd,"va%d.txt=\"%s\"",Variable,buff);
   sendCommand (cmd);
   return recvRetCommandFinished ();
}

