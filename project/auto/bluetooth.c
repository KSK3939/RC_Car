#include "bluetooth.h"
#include "usart.h"

static volatile uint8_t flags = 0;
static volatile uint8_t modes = 0;

static uint8_t rxData = 0;

void Bluetooth_Init()
{
  flags = 0;
  modes = 0;
}

void Bluetooth_Ctrl(uint8_t data)
{
  switch(data)
  {
    case 'A' :
      flags = 1;
      break;
    case 'B' :
      flags = 2;
      break;
    case 'C' :
      flags = 3;
      break;
    case 'D' :
      flags = 4;
      break;
    case 'L' :
      flags = 5;
      break;
    case 'R' :
      flags = 6;
      break;
    case '0' :
      flags = 0;
      break;

    case 'U' :
      modes = 1;
      break;
    case 'M' :
      modes = 0;
      break;

    default :
      break;
  }
}
// 받는 값에 따른 플래그 및 모드 설정

uint8_t Bluetooth_GetFlag()
{
  return flags;
}

uint8_t Bluetooth_GetMode()
{
  return modes;
}

void Bluetooth_Start()
{
  HAL_UART_Receive_IT(&huart1, &rxData, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance != USART1) return;

  Bluetooth_Ctrl(rxData);

  HAL_UART_Receive_IT(&huart1, &rxData, 1);
}


