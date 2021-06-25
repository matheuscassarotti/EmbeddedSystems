#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

bool LED_D4 = 0;

void main(void){
  //SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_320, 24000000); // 24MHz
  SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_320, 120000000); // 120MHz

  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Habilita GPIO F (LED D3 = PF4, LED D4 = PF0)
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)); // Aguarda final da habilitação
    
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0); // LED D4 como saída
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 1); // LED D4 aceso
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);

  SysTickIntEnable();
  SysTickEnable();

  while(1){
      //Contador 24MHz: (1/24M) => 41,66 nanosegundos por ciclo -> (41,66 ns * 3 instruções * 4 ciclos por instrução) ^-1 = 2.000.000
      //Contador 120MHz: (1/120M) => 8,33 nanosegundos por ciclo -> (8,33 ns * 3 instruções * 4 ciclos por instrução) ^-1 = 10.000.000                
      for(uint32_t timer = 0;timer<10000000;timer++){
        if(timer > 9999998){
          LED_D4 = !LED_D4;
          GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, LED_D4);
          
          /* ###### ANÁLISE ########
          24MHz - Baixa Otimização - Tempo estimado para 5 ciclos = 10,21s
          120MHz - Baixa Otimização - Tempo estimado para 5 ciclos = 17,30s
          24MHz - Alta Otimização - Tempo estimado para 5 ciclos = 4,11s
          120MHz - Alta Otimização - Tempo estimado para 5 ciclos = 10,01s
          ##########################
          */
          
        } //end if
      } //end for
  } // while
} // main
