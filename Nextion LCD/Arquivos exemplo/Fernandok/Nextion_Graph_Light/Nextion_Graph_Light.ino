#include "Nextion.h" //biblioteca para comunicação com o display NEXTION
#include <SimpleDHT.h> //biblioteca para comunicação com o DHT22


SimpleDHT22 dht22;
static int pinDHT22 = 7; //pino que ligamos o DATA do DHT22 no arduino

//variáveis que armazenam os dados do DHT22
float temperature = 0;
float humidity = 0;

uint8_t pinLED = 3; //pino que ligamos o LED no arduino

bool isPage0 = true; //variável de controle para saber se é a página ZERO que está em tela
bool isLedOn = false; //variável de controle para saber se o LED está aceso

/*
 * DECLARAÇÕES DOS OBJETOS DO NEXTION:  [pageID, componentID, componentName]
 */
NexProgressBar progTemp     = NexProgressBar(0, 1, "prog_temp"); //Progress Bar temperatura
NexProgressBar progHumid    = NexProgressBar(0, 16, "prog_humid"); //Progress Bar umidade
NexButton btnNext           = NexButton(0, 3, "btn_next"); //botão da página 0
NexText txtTemp             = NexText(0, 2, "txt_temp"); //texto com o valor da temperatura
NexText txtHumid            = NexText(0, 5, "txt_humid"); //texto com o valor da umidade
NexWaveform waveform        = NexWaveform(0, 6, "waveform"); //componente do gráfico de temperatura e umidade

NexDSButton btnLight        = NexDSButton(1, 1, "btn_light"); //botão de duplo estado para controlar o LED
NexSlider slider            = NexSlider(1, 2, "slider_pwm"); //slider que controlará o PWM do LED
NexText statusPWM           = NexText(1, 4, "status_pwm"); //texto com o status do pwm
NexVariable valueSlider     = NexVariable(1,5,"valueSlider");  //(objeto do nextion) variavel que guarda o valor do slider
NexButton btnBack           = NexButton(1, 3, "btn_back"); //botão da página 1

NexPage page0    = NexPage(0, 0, "page0");
NexPage page1    = NexPage(1, 0, "page1");

//Nesse array, declaramos os objetos Nextion que terão inter~ção de eventos touch
NexTouch *nex_listen_list[] = 
{
    &btnLight,
    &slider,
    &valueSlider,
    &btnNext,
    &btnBack,
    NULL
};

char buffer[100] = {0}; //buffer para armazenar dados string

//variáveis de controle de tempo, para funcionar como um cronômetro
long previousMillis = 0;
const int maxTime = 1000;

void setup() {
    Serial.begin(9600);

    pinMode(pinLED, OUTPUT);

    nexInit(); //inicializa a comunicação com o nextion

    btnNext.attachPop(btnNextPopCallback, &btnNext); //callback para o evento de release do botão btnNext
 
    btnBack.attachPop(btnBackPopCallback, &btnBack); //callback para o evento de release do botão btnBack

    slider.attachPop(sliderPopCallback); //callback para o evento de release do slider

    btnLight.attachPop(btnLightPopCallback, &btnLight); //callback para o evento de release do botão de dois estados (btnLight)

}

void loop() {

    //essa função trabalha como um listener para os eventos de press e release dos objetos utilizados no NEXTION
    nexLoop(nex_listen_list);

    //verifica se a página atual é a ZERO(inicial)
    //caso seja, verifica se o tempo passado entra a última passagem e a atual, foi de 1 segundo
    //e então chama a atualização do gráfico de temperatura e umidade
    if(isPage0)
    {
      if(millis() - previousMillis >= maxTime)
      {
        previousMillis = millis();
        updateDHTInfo(); //pega os valores do DHT22 e então manda para o NEXTION
      }      
    }    
}

//função que recupera os valores de temperatura e umidade do DHT22
//e através dos objetos do NEXTION seta as informações
void updateDHTInfo()
{
  int status = dht22.read2(pinDHT22, &temperature, &humidity, NULL);//faz a leitura dos dados do DHT22

  //se conseguiu ler os dados corretamente
  if (status == SimpleDHTErrSuccess) 
  {
      progTemp.setValue(temperature);  //progress bar da temperatura
      memset(buffer, 0, sizeof(buffer));
      itoa(temperature, buffer, 10);        
      txtTemp.setText(buffer); //texto que indica a temperatura

      progHumid.setValue(humidity); //progress bar da umidade
      memset(buffer, 0, sizeof(buffer));
      itoa(humidity, buffer, 10);      
      txtHumid.setText(buffer); //texto que indica a umidade

      waveform.addValue(0, temperature); //envia o valor da temperatura para o gráfico
      waveform.addValue(1, humidity); //envia o valor da umidade para o gráfico
  }
}


/*
  CALLBACKS
*/

//callback do SLIDER, quando o evento de release acontecer essa função é chamada
void sliderPopCallback(void *ptr)
{
    //se o LED está ligado, busca o valor da variável que guarda o valor do slider e então
    //seta o PWM do LED
    if(isLedOn)
    {
      uint32_t number = 0;
      valueSlider.getValue(&number);
      int value = map(number, 0,100,0,255);
      analogWrite(pinLED, value);
      memset(buffer, 0, sizeof(buffer));
      itoa(value, buffer, 10);        
      statusPWM.setText(buffer); 
    }

}

/*
 * Dual state button component pop callback function. 
 * In this example,the button's text value will plus one every time when it is released. 
 */
/* Callback do botão de dois estados */
void btnLightPopCallback(void *ptr)
{
    uint32_t dual_state;
    NexDSButton *btn = (NexDSButton *)ptr;
    dbSerialPrintln("b0PopCallback");
    dbSerialPrint("ptr=");
    dbSerialPrintln((uint32_t)ptr); 
    memset(buffer, 0, sizeof(buffer));

    /* Get the state value of dual state button component . */
    /* recupera o valor do estado de botão de dois estados */
    btnLight.getValue(&dual_state);
    if(dual_state) 
    {
        isLedOn = true;
        sliderPopCallback(ptr);
    }
    else
    {
         statusPWM.setText("DESLIGADO");
         digitalWrite(pinLED, LOW);
         isLedOn = false;
    }
}

//callback do botão btnNext
void btnNextPopCallback(void *ptr)
{
  page1.show();
  isPage0 = false;
  sliderPopCallback(ptr);  //chama o callback do slider para reconfigura-lo
}

//callback do botão btnBack 
void btnBackPopCallback(void *ptr)
{
  page0.show();
  isPage0 = true;
}



void page0PopCallback(void *ptr)
{
    dbSerialPrintln("page0PopCallback");
    page1.show();
    isPage0 = false;
}

void page1PopCallback(void *ptr)
{
    dbSerialPrintln("page1PopCallback");
    page0.show();
    isPage0 = true;
}

