#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS
#include "driverbuttons.h"
#include <stdbool.h>
#include <math.h>

typedef struct{
  uint8_t ledNum;
  uint32_t intensidade;
  bool isLedAtual;
  osMessageQueueId_t idMessageQueue;
}led_pwm;

led_pwm leds[4]; //Array com 4 estruturas, uma para cada LED

void initLEDs(){ //Inicializa estruturas dos LEDs
  for(int i=0;i<4;i++){
    leds[i].ledNum = (int)pow(2,i); //Atribui número dos LEDs em expoentes de 2, conforme definido em "driverleds.h" (1,2,4,8)
    leds[i].intensidade = 1;
    leds[i].isLedAtual = false;
    leds[i].idMessageQueue = osMessageQueueNew(8, sizeof(uint8_t), NULL);
  }
}

osThreadId_t idControladora, idAcionadoraLED1, idAcionadoraLED2, idAcionadoraLED3, idAcionadoraLED4; //1 Controladora e 4 Acionadoras
osMutexId_t idMutex;

void GPIOJ_Handler(void){
  //Checa qual botão gerou a interrupção
  if (!ButtonRead(USW1)){
    ButtonIntClear(USW1);
    osThreadFlagsSet(idControladora, 0x0001); //Flag 0x01 = trocar LED atual
  } else if (!ButtonRead(USW2)){
    ButtonIntClear(USW2);
    osThreadFlagsSet(idControladora, 0x0002); //Flag 0x02 = mudar intensidade do LED
  } 
} 

void controladora(void *arg){
  uint32_t acionadoraAtual = 0, eventFlag = 0;
  leds[0].isLedAtual = true;
  osMessageQueueId_t msgQueueNum = leds[0].idMessageQueue;
  
  while(1){
    eventFlag = osThreadFlagsWait(0x0001 | 0x0002, osFlagsWaitAny, osWaitForever);
    if (eventFlag == 0x0001){ //Muda LED selecionado
      //Incrementa tarefa acionadora atual
      acionadoraAtual++;
      if(acionadoraAtual > 3){
        acionadoraAtual=0;
      }          
      //Determina LED atualmente selecionado e limpa flag de LED selecionado anteriormente
      leds[acionadoraAtual].isLedAtual = true;
      if(acionadoraAtual > 0){
        leds[(acionadoraAtual-1)].isLedAtual = false;
      } else if(acionadoraAtual == 0){
        leds[3].isLedAtual = false;
      }
      
      msgQueueNum = leds[acionadoraAtual].idMessageQueue;    
    }
    if (eventFlag == 0x0002){ //aumenta intensidade do LED atual
      if (leds[acionadoraAtual].intensidade < 11){
        leds[acionadoraAtual].intensidade++;
      } else {
        leds[acionadoraAtual].intensidade = 1; //Reseta para a intensidade inicial
      }
      osMessageQueuePut(msgQueueNum, &leds[acionadoraAtual].intensidade, 0, osWaitForever);
    }
  }
}

void acionadora(void *arg)
{
  led_pwm *led = (led_pwm *)arg;
  uint8_t estado = 0;
  uint32_t tempoKernel = osKernelGetTickCount();
  
  while(1){
    osMutexAcquire(idMutex, osWaitForever);
    // === !!! SEÇÃO CRÍTICA !!! ===
    tempoKernel = osKernelGetTickCount();
    estado ^= (*led).ledNum;
    LEDWrite((*led).ledNum, estado); 
    // ============ FIM ============
    osMutexRelease(idMutex);
    osDelayUntil(tempoKernel + ((*led).intensidade)); //Modulação do PWM
    
    osMutexAcquire(idMutex, osWaitForever);
    // === !!! SEÇÃO CRÍTICA !!! ===
    tempoKernel = osKernelGetTickCount(); 
    estado ^= (*led).ledNum;
    LEDWrite((*led).ledNum, estado); 
    // ============ FIM ============
    osMutexRelease(idMutex);
    osDelayUntil(tempoKernel + ((10-(*led).intensidade))); //Modulação do PWM
    
    if((*led).isLedAtual == true){
      osDelay(20); //Pisca o LED atualmente selecionado
    }
    
    osMessageQueueGet((*led).idMessageQueue, &(*led).intensidade, NULL, 0); 
  } // while   
}

void main(void){
  SystemInit();
  LEDInit(LED4 | LED3 | LED2 | LED1);
  ButtonInit(USW1 | USW2);
  ButtonIntEnable(USW1 | USW2);
  
  osKernelInitialize();
  
  initLEDs();
  idMutex = osMutexNew(NULL);
  idControladora = osThreadNew(controladora, NULL, NULL);
  idAcionadoraLED1 = osThreadNew(acionadora, &leds[0], NULL);
  idAcionadoraLED2 = osThreadNew(acionadora, &leds[1], NULL);
  idAcionadoraLED3 = osThreadNew(acionadora, &leds[2], NULL);
  idAcionadoraLED4 = osThreadNew(acionadora, &leds[3], NULL);
    
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main