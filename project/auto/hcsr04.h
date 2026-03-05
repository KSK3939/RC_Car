#ifndef INC_HCSR04_H_
#define INC_HCSR04_H_


#include "stm32f1xx_hal.h"
#include "tim.h"
#include "delay.h"
#include "gpio.h"

#define TRIG1_PORT GPIOC
#define TRIG1_PIN  GPIO_PIN_8

#define TRIG2_PORT GPIOC
#define TRIG2_PIN  GPIO_PIN_6

#define TRIG3_PORT GPIOC
#define TRIG3_PIN  GPIO_PIN_5

// 트리거 포트 및 핀 설정

static volatile uint16_t IC_Value1[3] = {0};
static volatile uint16_t IC_Value2[3] = {0};

static volatile uint16_t echoTime[3] = {0};
static volatile uint16_t captureFlag[3] = {0};
static volatile uint16_t  distance[3] = {0};
static volatile uint8_t dist_ready[3] = {0};

// 거리 계산에 필요한 변수들 선언

void HCSR04_INIT();
void HCSR04_TRG(uint8_t idx);

const volatile uint16_t* HCSR04_GetDistance();
volatile uint8_t* HCSR04_GetReady();

#endif /* INC_HCSR04_H_ */
