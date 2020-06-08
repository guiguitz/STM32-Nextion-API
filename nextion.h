#include <string.h>

void sendCommand(const char* cmd);

int recvRetCommandFinished(void);

int recvRetNumber(void);

int recvRetString(char* buffer,int len);

int nexInit(void);

int NexPageShow(int Page);

int NexTextSetText(int Text,const char *buffer);

int NexTextGetText( int Number ,char* buffer, int len  );

int NexNumberSetValue(int Number,int value);

int NexNumberGetValue(int Number);

int NexXfloatSetValue(int Number, int value);

int NexPictureSetPic(int pic, int value);

int NexButtonGetText(int Button, char *buffer, int len);

int NexButtonSetText(int Button, const char *buffer);

int NexCheckboxGetValue(int Checkbox);

int NexCheckboxSetValue(int Checkbox,int value);

int NexDSButtonGetValue(int DSButton);

int NexDSButtonSetValue(int DSButton,int value);

int NexDSButtonGetText(int DSButton, char *buffer, int len);

int NexDSButtonSetText(int DSButton,const char *buffer);

int NexGaugeGetValue(int Gauge);

int NexGaugeSetValue(int Gauge,int value);

int NexProgressBarGetValue(int ProgressBar);

int NexProgressBarSetValue(int ProgressBar,int value);

int NexRadioGetValue(int Radio);

int NexRadioSetValue(int Radio,int value);

int NexVariableGetValue(int Variable);

int NexVariableSetValue(int Variable,int value );

int NexVariableGetText(int Variable, char *buffer, uint32_t len);

int NexVariableSetText(int Variable,const char *buffer);


