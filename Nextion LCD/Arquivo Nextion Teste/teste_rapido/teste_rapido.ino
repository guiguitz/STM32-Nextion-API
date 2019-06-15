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

// ===============================================================================
// --- Declaração de Objetos ---
                     //page id:0, id componente:1, nome do componente: "bt0"
NexProgressBar bat   = NexProgressBar(0, 1, "j0");
NexProgressBar motor = NexProgressBar(0, 3, "j1");
NexNumber vel        = NexNumber(0, 0, "n0");


// ===============================================================================
// --- Variáveis Globais ---
int     pot_value = 0,    //valor ad do potenciômetro
        bar_value = 0;    //valor da barra de progresso


int aux = 0;

char    txt1[10],         //texto para conversão e exibição
        txt2[10];

// ===============================================================================
// --- Configurações Iniciais ---
void setup()
{
  
    nexInit();  //inicializa o tft
  
} //end setup


// ===============================================================================
// --- Loop Infinito ---
void loop()
{

 //Atualização dos valores do Nextion
    bat.setValue(aux);
    motor.setValue(aux);
    vel.setValue(aux);
  
    //pot_value = analogRead(pot);
   
    //Converte inteiros para string
    //memset(txt1, 0, sizeof(txt1));
    //itoa(pot_value, txt1, 10);
  
    //Normaliza a barra animada de acordo com o valor do pot
    //bar_value = map(pot_value, 0, 1023, 0, 100);

    if(aux > 100) aux = 0;
    else  aux++;
  
} //end loop

