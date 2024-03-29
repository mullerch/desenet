/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : usb_prop.h
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : All processing related to Virtual COM Port Demo (Endpoint 0)
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usb_prop_H
#define __usb_prop_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
void Virtual_Com_Port_Init(void);
void Virtual_Com_Port_Reset(void);
void Virtual_Com_Port_Status_In (void);
void Virtual_Com_Port_Status_Out (void);
RESULT Virtual_Com_Port_Data_Setup(u8);
RESULT Virtual_Com_Port_NoData_Setup(u8);
RESULT Virtual_Com_Port_Get_Interface_Setting(u8 Interface, u8 AlternateSetting);
u8 *Virtual_Com_Port_GetDeviceDescriptor(u16 );
u8 *Virtual_Com_Port_GetConfigDescriptor(u16);
u8 *Virtual_Com_Port_GetStringDescriptor(u16);

u8 *Virtual_Com_Port_GetLineCoding(u16 Length);
u8 *Virtual_Com_Port_SetLineCoding(u16 Length);

/* Exported define -----------------------------------------------------------*/
typedef struct
{
  u32 bitrate;			// baud rate
  u8 format;			// 0: 1 stop bit, 1: 1.5 stop bits, 2: 2 stop bits
  u8 paritytype;		// 0: no parity, 1: even parity, 2: odd parity
  u8 datatype;			// 7: 7 data bits, 8: 8 data bits
}
LINE_CODING;

extern LINE_CODING linecoding;

#define Virtual_Com_Port_GetConfiguration          NOP_Process
#define Virtual_Com_Port_SetConfiguration          NOP_Process
#define Virtual_Com_Port_GetInterface              NOP_Process
#define Virtual_Com_Port_SetInterface              NOP_Process
#define Virtual_Com_Port_GetStatus                 NOP_Process
#define Virtual_Com_Port_ClearFeature              NOP_Process
#define Virtual_Com_Port_SetEndPointFeature        NOP_Process
#define Virtual_Com_Port_SetDeviceFeature          NOP_Process
#define Virtual_Com_Port_SetDeviceAddress          NOP_Process

#define SEND_ENCAPSULATED_COMMAND   0x00
#define GET_ENCAPSULATED_RESPONSE   0x01
#define SET_COMM_FEATURE            0x02
#define GET_COMM_FEATURE            0x03
#define CLEAR_COMM_FEATURE          0x04
#define SET_LINE_CODING             0x20
#define GET_LINE_CODING             0x21
#define SET_CONTROL_LINE_STATE      0x22
#define SEND_BREAK                  0x23

#endif /* __usb_prop_H */
/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/

