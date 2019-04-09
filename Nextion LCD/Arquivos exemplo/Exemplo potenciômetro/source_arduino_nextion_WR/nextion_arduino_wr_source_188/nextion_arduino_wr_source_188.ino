/* ===============================================================================

     Vídeo Aula de Engenharia Eletrônica 188

     Utilizando Display Nextion TFT com Arduino


     Autor: Eng. Wagner Rambo

     Data:  Setembro de 2017


     www.wrkits.com.br | facebook.com/wrkits | youtube.com/canalwrkits

  
=============================================================================== */


// ===============================================================================
// --- Biblioteca Auxiliar ---
#include  "Nextion.h"   //biblioteca Nextion


// ===============================================================================
// --- Mapeamento de Hardware ---
#define    pot    A0   //potenciômetro na analógica 0
#define    led    LED_BUILTIN   //led onboard pino digital 13


// ===============================================================================
// --- Declaração de Objetos ---
                     //page id:0, id componente:1, nome do componente: "bt0"
NexProgressBar j1 = NexProgressBar(0, 4, "j1");
NexText pot_val   = NexText(0, 3, "t1");
NexDSButton bt0   = NexDSButton(0, 5, "bt0");


// ===============================================================================
// --- Variáveis Globais ---
int     pot_value = 0,    //valor ad do potenciômetro
        bar_value = 0;    //valor da barra de progresso

char    txt1[10],         //texto para conversão e exibição
        txt2[10];

uint32_t ds_var;          //armazena o estado do botão


// ===============================================================================
// --- Configurações Iniciais ---
void setup()
{
  
    nexInit();  //inicializa o tft

    pinMode(led, OUTPUT);   //defino saída para o led
    digitalWrite(led, LOW); //inicia desligado
  
  
} //end setup


// ===============================================================================
// --- Loop Infinito ---
void loop()
{

 //Atualização dos valores do Nextion
    j1.setValue(bar_value);
    pot_val.setText(txt1);
  
    pot_value = analogRead(pot);
  
 
    //Converte inteiros para string
    memset(txt1, 0, sizeof(txt1));
    itoa(pot_value, txt1, 10);
  
 
    //Normaliza a barra animada de acordo com o valor do pot
    bar_value = map(pot_value, 0, 1023, 0, 100);


    bt0.getValue(&ds_var);
    if(ds_var>0) digitalWrite(led, HIGH);
    else digitalWrite(led, LOW);
  
} //end loop

