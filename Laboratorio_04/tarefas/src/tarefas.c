#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

/*========================
  Exercício 2: CMSIS-RTOS
  ========================
  * Localize o arquivo RTX_Config.h na lista de dependências do arquivo rtx_lib.c e analise-o. Quais são as configurações para:
  – OS_TICK_FREQ? //R: 1000
  – OS_THREAD_NUM? //R: 1
  – OS_THREAD_DEF_STACK_NUM? //R: 1

  * Altere a configuração OS_TICK_FREQ para 500. Qual é o efeito dessa alteração na execução do programa do projeto “tarefas”?
  //R: O tempo entre cada tique do sistema operacional é dobrado, pois ao modificar este parâmetro a frequência com que eles
 ocorrem é reduzida pela metade. Isso aumenta o atraso causado pela chamada das funções osDelay() e osDelayUntil().

  * Retorne a configuração OS_TICK_FREQ para o seu valor original. Quais são os períodos de ativação das tarefas thread1 e thread2?
  //R: O período da tarefa thread1 é variável de acordo com o tempo de execução de suas atividades internas, pois o atraso causado
  pela função osDelay() é relativo ao momento em que ela é chamada, resultando num período de: T(atividades) + 100 tiques.
    Já o período da tarefa thread2 é aproximadamente constante pois o atraso causado pela função osDelayUntil() é absoluto de acordo
  com o número total de tiques do RTOS, e sempre será de (aproximadamente) 100 tiques.
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
