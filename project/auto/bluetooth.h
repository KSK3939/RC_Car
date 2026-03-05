#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#include "stm32f1xx_hal.h"

void Bluetooth_Init();
void Bluetooth_Ctrl(uint8_t data);

uint8_t Bluetooth_GetFlag();
uint8_t Bluetooth_GetMode();
void Bluetooth_Start();

#endif /* INC_BLUETOOTH_H_ */
