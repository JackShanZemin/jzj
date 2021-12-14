#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32g0xx_hal.h"
void Error_Handler(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void Uart1_SendStr(char*SendBuf);
void UART1_send_byte(char data);
void Uart2_SendDATA(char *data,unsigned char len);
void Uart1_SendDATA(char *data,unsigned char len);
extern uint8_t RxBuffer[400];
extern uint8_t TxBuffer[300];
extern int Rxcouter;
extern uint8_t Uart1_RxBuff[100];		//数据数据
extern uint8_t Uart1_Rx_Cnt ;		//长
extern unsigned char uart1_getok;
extern unsigned char uart2_getok; 
#endif


