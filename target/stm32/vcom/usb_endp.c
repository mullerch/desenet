#include "stm32f10x_lib.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "usb_hw.h"
#include "usb_istr.h"
#include "usb_endp.h"

static volatile bool tx_in_progress = FALSE;
static volatile u16 rx_pma_count = 0;
static u8 rx_buffer[VIRTUAL_COM_PORT_DATA_SIZE];
static u16 rx_buffer_count = 0;
static u16 rx_buffer_index = 0;

static inline void Enable_USB_Interrupts(void)
{
  //NVIC_RESETPRIMASK();
  const int c = USB_LP_CAN1_RX0_IRQn;
  NVIC->ISER[c >> 5] = 1 << (c & 0x1F);
}

static inline void Disable_USB_Interrupts(void)
{
  //NVIC_SETPRIMASK();
  const int c = USB_LP_CAN1_RX0_IRQn;
  NVIC->ICER[c >> 5] = 1 << (c & 0x1F);
}

// Transmit

void EP1_IN_Callback(void)									// on transmit
{
  tx_in_progress = FALSE;
}

u32 Virtual_Com_Port_Transmit_Available()
{
  return tx_in_progress ? 0 : VIRTUAL_COM_PORT_DATA_SIZE;
}

u32 Virtual_Com_Port_Transmit_Non_Blocking(const u8* data, u32 length)
{
  u32 l;
  if (!length || tx_in_progress) return 0;
  tx_in_progress = TRUE;
  l = length <= VIRTUAL_COM_PORT_DATA_SIZE ? length : VIRTUAL_COM_PORT_DATA_SIZE;
  UserToPMABufferCopy(data, ENDP1_TXADDR, l);
  Disable_USB_Interrupts();
  SetEPTxCount(ENDP1, l);
  SetEPTxValid(ENDP1);
  Enable_USB_Interrupts();
  return l;
}

void Virtual_Com_Port_Transmit(const u8* data, u32 length)
{
  while (length)
  {
    u32 l;
    l = Virtual_Com_Port_Transmit_Non_Blocking(data, length);
    data += l;
    length -= l;
  }
}

// Receive

void EP3_OUT_Callback(void)									// on receive
{
  u16 c;
  rx_pma_count = c = GetEPRxCount(ENDP3);
  if (!c) SetEPRxValid(ENDP3);
}

u32 Virtual_Com_Port_Receive_Available()
{
  return rx_buffer_count > rx_buffer_index ? rx_buffer_count - rx_buffer_index : rx_pma_count;
}

u32 Virtual_Com_Port_Receive_Non_Blocking(u8* data, u32 length)
{
  u32 l;
  if (!(rx_buffer_count > rx_buffer_index))
  {
    u16 c;
    c = rx_pma_count;
    if (!c) return 0;
    PMAToUserBufferCopy(rx_buffer, ENDP3_RXADDR, c);
    rx_buffer_count = c;
    rx_buffer_index = 0;
    rx_pma_count = 0;
    Disable_USB_Interrupts();
    SetEPRxValid(ENDP3);
    Enable_USB_Interrupts();
  }
  l = rx_buffer_count - rx_buffer_index;
  if (length > l) length = l;
  l = length;
  while (length--) *data++ = rx_buffer[rx_buffer_index++];
  return l;
}

u32 Virtual_Com_Port_Receive(u8* data, u32 length)
{
  u32 l;
  do l = Virtual_Com_Port_Receive_Non_Blocking(data, length);
  while (!l && length);
  return l;
}
