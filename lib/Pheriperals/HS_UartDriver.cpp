
#include "Arduino.h"
#include "HS_UartDriver.h"
//#include "SoftwareSerialStm32.h" 
static bool ESP32_MDC_Flag = false;
static bool Radio_uFlag = false;
 

/* 
*------------------Arduino Serial is on UART2 on pin rx_PA3 tx_PA2)-----------------*
*                  & is moved to debugcli.h for debugging and CLI support  
*/

/*----------------------HardwareSerial Uart(RX, TX)----------------------*/
/*  
* HardwareSerial Uart2(PA10, PA9);  Do not work in Arduino Stm32@1.9.0 
* Fix provide under issue @ https://github.com/stm32duino/Arduino_Core_STM32/issues/1180#issuecomment-696786763
* Above fix untill next release.
*/ 
// #if MD_BOARD_VERSION == BOARD_VERSION_PRE_9
// //HardwareSerial Uart1(PA10, PA9); 
// #endif

// // V10 has Uart remapped to pin PA10, PA9
// #if MD_BOARD_VERSION >= BOARD_VERSION_V9
// #endif

HardwareSerial RADIO_UART(PA10, PA9);  


/* 
* HardwareSerial Uart4(PA1, PA0);  Do not work in Arduino Stm32@1.9.0, SOC stop working 
* Fix provide: https://github.com/stm32duino/Arduino_Core_STM32/pull/1189/commits
* Above fix until next release.
*/

#if MD_BOARD_VERSION == BOARD_VERSION_PRE_9
//HardwareSerial Uart4(PA1, PA0);
#endif


/*
* UART3 is reserved that can be use to communicate external module with MDC
* UART3 is on Pin RX_PB11 & TX_PB10 for MDC 
* NOTE: DISABLE External EEPROM I2C before enabling UART3 on these pins. (note we can shit UART3 pins to PB0 & PB2 in new revision)
* HardwareSerial Uart3(PB11, PB10);
*/

void UARTDrv_Init(_uartNum uartNum, int baud)
{
    switch (uartNum)
    {   

          case RF_UART/* constant-expression */:
            /* code */
            RADIO_UART.begin(baud); 
            Radio_uFlag = true; 
           // DBG_PRINT(DEBUG_BOARD_INIT, "\nRF UART Init------->"); 
            break;
            
       #if MD_BOARD_VERSION >= BOARD_VERSION_V9
       #else //#if MD_BOARD_VERSION == BOARD_VERSION_PRE_9
            case ESP32_MDC_UART/* constant-expression */:
            /* code */
            Uart1.begin(baud);
            ESP32_MDC_Flag = true;
            DBG_PRINT(DEBUG_BOARD_INIT, "\nES32 Debug Init------->"); 
            break; 
        
            // moved to debugcli.h for debugging and CLI support  
    
            case RF_UART/* constant-expression */:
            /* code */
            Uart4.begin(baud); 
            Radio_uFlag = true; 
            DBG_PRINT(DEBUG_BOARD_INIT, "\nRF UART Init------->"); 
            break;
       #endif 
    default:
        break;
    }    
} 

void RADIO_Print(String txt)
{
    if(Radio_uFlag) RADIO_UART.print(txt); 
    #if MD_BOARD_VERSION >= BOARD_VERSION_V9
    #endif
}

void UARTDrv_Print(_uartNum uartNum, String txt)
{
    switch (uartNum)
    {
        case RF_UART:
           // Serial.println("\nHello Serial RF");
            RADIO_UART.print(txt);
            break;   
        #if MD_BOARD_VERSION >= BOARD_VERSION_V9
        #else

        case ESP32_MDC_UART/* constant-expression */:
        /* code */
        if(ESP32_MDC_Flag) Uart1.print(txt);
        break;
        
         // moved to debugcli.h for debugging and CLI support  
        // case SERIAL_UART/* constant-expression */:
        // /* code */
        // Serial.print(txt);         
        // break;
        
        case RF_UART/* constant-expression */:
        /* code */
         Uart4.print(txt);   
        break; 

        #endif

        default:
        break;
    }
} 

void UARTDrv_Write(_uartNum uartNum, uint8_t* txt, int size)
{    
    switch (uartNum)
    {
        case RF_UART:
            RADIO_UART.write(txt, size);
            RADIO_UART.write(0x00); // null byte
            RADIO_UART.flush();
        break;
        #if MD_BOARD_VERSION >= BOARD_VERSION_V9
        #else
            case ESP32_MDC_UART/* constant-expression */:
            /* code */
            Uart1.write(txt, size);
            break; 

            case RF_UART/* constant-expression */:
            /* code */
            Uart4.write(txt, size);
            Uart4.write(0x00); // null byte
            Uart4.flush();
            break;
        #endif
    
    default:
        break;
    }
}


String UARTDrv_ReadStr(_uartNum uartNum)
{    
    String buff;
    char byt;
    
    switch (uartNum)
    {
        case RF_UART: 
            if(RADIO_UART.available() > 0)
            { 
                buff = RADIO_UART.readString();   
            }
            break;    
        #if MD_BOARD_VERSION >= BOARD_VERSION_V9
        #else
            case ESP32_MDC_UART: 
            if(Uart1.available() > 0)
            {
                byt = Uart1.read(); 
                buff += byt;
            }
            break;   
            
            case RF_UART: 
            if(Uart4.available() > 0)
            { 
                byt = Uart4.read(); 
                buff += byt;
            }
            break;    
        #endif 
        default:
            break;
    }

    return buff;
}


bool UARTDrv_Read(_uartNum uartNum, int* buff)
{    
    int byt;
    
    switch (uartNum)
    {
            case RF_UART: 
                if(RADIO_UART.available() > 0)
                { 
                    byt = RADIO_UART.read(); 
                    *buff = byt;
                    return true;
                }
            break;    
        #if MD_BOARD_VERSION >= BOARD_VERSION_V9
        #else
            case ESP32_MDC_UART: 
            if(Uart1.available() > 0)
            {
                
                byt = Uart1.read(); 
                *buff = byt;
                return true;
            }
            break;   
            
            case RF_UART: 
            if(Uart4.available() > 0)
            { 
                byt = Uart4.read(); 
                *buff = byt;
                return true;
            }
            break;    
        #endif 
        
        default:
            return false;
            break;
    }

    return false;
} 