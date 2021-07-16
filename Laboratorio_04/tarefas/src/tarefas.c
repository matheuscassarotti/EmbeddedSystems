#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

/*========================
  Exerc�cio 2: CMSIS-RTOS
  ========================
  * Localize o arquivo RTX_Config.h na lista de depend�ncias do arquivo rtx_lib.c e analise-o. Quais s�o as configura��es para:
  � OS_TICK_FREQ? //R: 1000
  � OS_THREAD_NUM? //R: 1
  � OS_THREAD_DEF_STACK_NUM? //R: 1

  * Altere a configura��o OS_TICK_FREQ para 500. Qual � o efeito dessa altera��o na execu��o do programa do projeto �tarefas�?
  //R: O tempo entre cada tique do sistema operacional � dobrado, pois ao modificar este par�metro a frequ�ncia com que eles
 ocorrem � reduzida pela metade. Isso aumenta o atraso causado pela chamada das fun��es osDelay() e osDelayUntil().

  * Retorne a configura��o OS_TICK_FREQ para o seu valor original. Quais s�o os per�odos de ativa��o das tarefas thread1 e thread2?
  //R: O per�odo da tarefa thread1 � vari�vel de acordo com o tempo de execu��o de suas atividades internas, pois o atraso causado
  pela fun��o osDelay() � relativo ao momento em que ela � chamada, resultando num per�odo de: T(atividades) + 100 tiques.
    J� o per�odo da tarefa thread2 � aproximadamente constante pois o atraso causado pela fun��o osDelayUntil() � absoluto de acordo
  com o n�mero total de tiques do RTOS, e sempre ser� de (aproximadamente) 100 tiques.
*/

osThreadId_t thread1_id, thread2_id, thread3_id, thread4_id;

typedef struct{
  uint8_t led_number;
  uint32_t time;
} myLED;

void thread1(void *arg){
  uint8_t state = 0;
  
  while(1){
    state ^= LED1;
    LEDWrite(LED1, state);
    osDelay(100);
  } // while
} // thread1

void thread2(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  myLED *led = (myLED*)arg;
  
  while(1){
    tick = osKernelGetTickCount();
    state ^= (*led).led_number;
    LEDWrite((*led).led_number, state);
    osDelayUntil(tick + (*led).time);
  } // while
} // thread2

void main(void){
  LEDInit(LED4 | LED3 | LED2 | LED1);
  myLED led1, led2, led3, led4;

  led1.led_number = LED1;
  led1.time = 200;
  led2.led_number = LED2;
  led2.time = 300;
  led3.led_number = LED3;
  led3.time = 500;
  led4.led_number = LED4;
  led4.time = 700;
  
  osKernelInitialize();
  
  thread1_id = osThreadNew(thread2, &led1, NULL);
  thread2_id = osThreadNew(thread2, &led2, NULL);
  thread3_id = osThreadNew(thread2, &led3, NULL);
  thread4_id = osThreadNew(thread2, &led4, NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
