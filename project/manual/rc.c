#include "rc.h"

uint32_t i = 7000;
static uint32_t prevMoveTime;

void front()
{
  if(i < 9000)
  {
    uint32_t now = HAL_GetTick();
    if (now - prevMoveTime >= 10)
    {
      prevMoveTime = now;
      i = i + 20; // 가속 매커니즘. 논블로킹으로 시간을 측정하여 틱마다 속도 증가.
      if (i > 9000) i = 9000;
    }
  }
  TIM1->CCR1 = i;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = i;
  TIM1->CCR4 = 0;
  // 지정한 속도로 움직이도록 설정.
}

void back()
{
  TIM1->CCR1 = 0;
  TIM1->CCR2 = 6000;
  TIM1->CCR3 = 0;
  TIM1->CCR4 = 6000;
  // 후진
}

void right()
{
  TIM1->CCR1 = 2000;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 7000;
  TIM1->CCR4 = 0;
  // 직진하는 두 모터의 속도를 다르게 하여 실제 차와 비슷하게 회전.
}

void left()
{
  TIM1->CCR1 = 7000;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 2000;
  TIM1->CCR4 = 0;
  // 직진하는 두 모터의 속도를 다르게 하여 실제 차와 비슷하게 회전.
}

void turn_right()
{
  TIM1->CCR1 = 0;
  TIM1->CCR2 = 7000;
  TIM1->CCR3 = 7000;
  TIM1->CCR4 = 0;
  // 한쪽은 앞, 한쪽은 뒤로하여 제자리에서 돌도록 함. 조작의 용이성을 위하여 남겨둠
}

void turn_left()
{
  TIM1->CCR1 = 7000;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 0;
  TIM1->CCR4 = 7000;
  // 한쪽은 앞, 한쪽은 뒤로하여 제자리에서 돌도록 함. 조작의 용이성을 위하여 남겨둠
}

void stop()
{
  TIM1->CCR1 = 0;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 0;
  TIM1->CCR4 = 0;
  // 정지
  i = 7000;
  // 직진 초기 속도로 되돌림
  prevMoveTime = HAL_GetTick();
  // 약간의 텀 제거
}

void move(uint8_t flag)
{
  switch(flag)
  {
    case 0:
      stop();
      break;

    case 1:
      front();
      break;

    case 2:
      back();
      break;

    case 3:
      left();
      break;

    case 4:
      right();
      break;

    case 5:
      turn_right();
      break;

    case 6:
      turn_left();
      break;

  }
}
