#include "rc.h"

uint32_t i = 7000;
static uint32_t prevMoveTime;

typedef enum
{
  CMD_STOP = 0,
  CMD_FRONT,
  CMD_BACK,
  CMD_LEFT,
  CMD_RIGHT,
  CMD_TURN_R,
  CMD_TURN_L
}Command;

// 각 동작을 enum으로 선언

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

// auto가 붙은것은 자율 주행 시에만 사용

void auto_front()
{
  TIM1->CCR1 = 4500;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 4500;
  TIM1->CCR4 = 0;
  // 지정한 속도로 움직이도록 설정.
}

void auto_right()
{
  TIM1->CCR1 = 1000;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 4250;
  TIM1->CCR4 = 0;
  // 직진하는 두 모터의 속도를 다르게 하여 실제 차와 비슷하게 회전.
}

void auto_left()
{
  TIM1->CCR1 = 4250;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 1000;
  TIM1->CCR4 = 0;
  // 직진하는 두 모터의 속도를 다르게 하여 실제 차와 비슷하게 회전.
}

void auto_turn_right()
{
  TIM1->CCR1 = 0;
  TIM1->CCR2 = 4000;
  TIM1->CCR3 = 4000;
  TIM1->CCR4 = 0;
  // 한쪽은 앞, 한쪽은 뒤로하여 제자리에서 돌도록 함. 조작의 용이성을 위하여 남겨둠
}

void auto_turn_left()
{
  TIM1->CCR1 = 4000;
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 0;
  TIM1->CCR4 = 4000;
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
  switch((Command)flag)
  {
      case CMD_STOP:
          stop();
          break;

      case CMD_FRONT:
          front();
          break;

      case CMD_BACK:
          back();
          break;

      case CMD_LEFT:
          left();
          break;

      case CMD_RIGHT:
          right();
          break;

      case CMD_TURN_R:
          turn_right();
          break;

      case CMD_TURN_L:
          turn_left();
          break;

      default:
          stop();   // 예외 상황 안전 처리
          break;
  }
}
// 명령에 따른 동작 설정
