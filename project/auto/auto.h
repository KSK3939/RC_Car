#ifndef INC_AUTO_H_
#define INC_AUTO_H_

#include "stm32f1xx_hal.h"
#include "rc.h"
#include "hcsr04.h"

typedef struct
{
  uint32_t trig_interval;
  uint32_t ctrl_interval;
  uint16_t lr_threshold;
  uint16_t front_threshold;

  uint32_t us_prevTick;
  uint8_t  us_state;

  uint32_t ctrl_prevTick;

  uint8_t  prev_max_idx;
  uint8_t  stop_pending;
  uint32_t stop_until_ms;

  uint32_t d0;
  uint32_t d1;
  uint32_t d2;
  uint32_t max_d;
  uint32_t max_idx;
}AMC;

// 필요한 변수들을 struct로 선언

void Auto_Init(AMC *ctx, uint32_t trig_interval, uint32_t ctrl_interval,
               uint16_t lr_threshold, uint16_t front_threshold);

void Auto_Update(AMC *ctx, const volatile uint16_t distance[3], volatile uint8_t dist_ready[3]);


#endif /* INC_AUTO_H_ */
