
 
#ifndef _HS_UART_DRV_
#define _HS_UART_DRV_
#include "Arduino.h"
#include <stddef.h>
#include <stdint.h>

typedef enum UartNum
{
    ESP32_MDC_UART= 1,      
    // SERIAL_UART,        
    RF_UART                 
} _uartNum;

void UARTDrv_Init(_uartNum uartNum, int baud);
void UARTDrv_Print(_uartNum uartNum, String txt);
void UARTDrv_Write(_uartNum uartNum, uint8_t* txt, int size);
bool UARTDrv_Read(_uartNum uartNum, int* buff);

//Debug 
void RADIO_Print(String txt);
String UARTDrv_ReadStr(_uartNum uartNum);

#endif //_HS_UART_DRV_
