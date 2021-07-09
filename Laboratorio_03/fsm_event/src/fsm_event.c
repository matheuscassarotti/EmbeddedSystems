#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "driverlib/systick.h"
#include "driverleds.h" // Projects/drivers

// MEF com apenas 2 estados e 1 evento temporal que alterna entre eles
// Seleção por evento

int contador = 0;

volatile uint8_t Evento = 0;

void SysTick_Handler(void){
  Evento = 1;
} // SysTick_Handler

void grey_zero(){
  LEDOff(LED1);
  LEDOff(LED2); 
  LEDOff(LED3);
}

void grey_one(){
  LEDOff(LED1); 
  LEDOff(LED2); 
  LEDOn(LED3);
}

void grey_two(){
  LEDOff(LED1); 
  LEDOn(LED2); 
  LEDOn(LED3);
}

void grey_three(){
  LEDOff(LED1); 
  LEDOn(LED2); 
  LEDOff(LED3);
}

void grey_four(){
  LEDOn(LED1); 
  LEDOn(LED2); 
  LEDOff(LED3);
}

void grey_five(){
  LEDOn(LED1); 
  LEDOn(LED2); 
  LEDOn(LED3);
}

void grey_six(){
  LEDOn(LED1); 
  LEDOff(LED2); 
  LEDOn(LED3);
}

void grey_seven(){
  LEDOn(LED1); 
  LEDOff(LED2); 
  LEDOff(LED3);
}

void main(void){

  LEDInit(LED1);
  LEDInit(LED2);
  LEDInit(LED3);
    
  SysTickPeriodSet(12000000); // f = 1Hz para clock = 24MHz
  SysTickIntEnable();
  SysTickEnable();

  while(1){
    if(Evento){
      Evento = 0;
      switch(contador){
        case 0:
          grey_zero();
          break;
        case 1:
          grey_one();
          break;
        case 2:
          grey_two();
          break;
        case 3:
          grey_three();
          break;
        case 4:
          grey_four();
          break;
        case 5:
          grey_five();
          break;
        case 6:
          grey_six();
          break;
        case 7:
          grey_seven();
          break;
      } //switch
      contador++;
      if(contador > 7){
        contador = 0;
      } // if
    } // if
  } // while
} // main
