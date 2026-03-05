#include "hcsr04.h"

void HCSR04_INIT()
{
  for(int i = 0; i < 3; i++)
  {
    IC_Value1[i] = IC_Value2[i] = 0;
    echoTime[i] = 0;
    captureFlag[i] = 0;
    distance[i] = 0;
    dist_ready[i] = 0;
  }
}

// 초기화

void HCSR04_TRG(uint8_t idx)
{
  GPIO_TypeDef *port;
  uint16_t pin;
  uint16_t cc;

  if(idx == 0)
  {
    port = TRIG1_PORT;
    pin = TRIG1_PIN;
    cc = TIM_IT_CC1;
  }
  else if(idx == 1)
  {
    port = TRIG2_PORT;
    pin = TRIG2_PIN;
    cc = TIM_IT_CC2;
  }
  else if(idx == 2)
  {
    port = TRIG3_PORT;
    pin = TRIG3_PIN;
    cc = TIM_IT_CC3;
  }

  // 센서 및 채널 선택

  HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); // low에서
  delay_us(1);
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET); // high로 올려 트리거
  delay_us(10); // 10us 동안 유지
  HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET); // 다시 low로

  captureFlag[idx] = 0;
  __HAL_TIM_SET_CAPTUREPOLARITY(&htim2, cc, TIM_INPUTCHANNELPOLARITY_RISING);
  __HAL_TIM_SET_COUNTER(&htim2, 0);
  // 초기화

  if(idx==0)
  {
  __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC1);
  }
  else if(idx==1)
  {
  __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC2);
  }
  else
  {
  __HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_CC3);
  }
  __HAL_TIM_ENABLE_IT(&htim2, cc); // 인터럽트 활성화
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  uint8_t idx;
  uint32_t ch;
  uint32_t cc;

  if(htim->Instance != TIM2)
  {
    return;
  }

  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    idx = 0;
    ch = TIM_CHANNEL_1;
    cc = TIM_IT_CC1;
  }
  else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
    idx = 1;
    ch = TIM_CHANNEL_2;
    cc = TIM_IT_CC2;
  }
  else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
  {
    idx = 2;
    ch = TIM_CHANNEL_3;
    cc = TIM_IT_CC3;
  }

  // 채널 선택

  if(captureFlag[idx] == 0)
  {
    IC_Value1[idx] = HAL_TIM_ReadCapturedValue(htim, ch); // 시작 시간 저장
    captureFlag[idx] = 1;
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, ch, TIM_INPUTCHANNELPOLARITY_FALLING); // falling edge 모드로 전환
  }
  else if(captureFlag[idx] == 1)
  {
    IC_Value2[idx] = HAL_TIM_ReadCapturedValue(htim, ch); // 종료 시간 저장

    if(IC_Value2[idx] > IC_Value1[idx])
    {
      echoTime[idx] = IC_Value2[idx] - IC_Value1[idx]; // 평범하게 계산
    }
    else if(IC_Value1[idx] > IC_Value2[idx])
    {
      echoTime[idx] = (0xffff - IC_Value1[idx]) + IC_Value2[idx]; // ARR을 넘어가서 value2가 더 커졌을 때의 계산
    }
    distance[idx] = echoTime[idx] / 58; // 거리(cm) 계산
    dist_ready[idx] = 1;

    captureFlag[idx] = 0;
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, ch, TIM_INPUTCHANNELPOLARITY_RISING);
    __HAL_TIM_DISABLE_IT(htim, cc);
    // 초기화
  }
  else
  {
    return;
  }
}

const volatile uint16_t* HCSR04_GetDistance()
{
  return distance;
}

volatile uint8_t* HCSR04_GetReady()
{
  return dist_ready;
}


