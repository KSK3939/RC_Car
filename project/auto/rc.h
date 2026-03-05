#ifndef INC_RC_H_
#define INC_RC_H_

#include "stm32f1xx_hal.h"

void front();
void back();
void left();
void right();
void stop();
void turn_right();
void turn_left();

void auto_front();
void auto_turn_right();
void auto_turn_left();
void auto_right();
void auto_left();

void move(uint8_t flag);

#endif /* INC_RC_H_ */
