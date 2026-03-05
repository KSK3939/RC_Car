#include "auto.h"


void Auto_Init(AMC *ctx, uint32_t trig_interval, uint32_t ctrl_interval,
               uint16_t lr_threshold, uint16_t front_threshold)
{
  ctx->trig_interval = trig_interval;
  ctx->ctrl_interval = ctrl_interval;
  ctx->lr_threshold = lr_threshold;
  ctx->front_threshold = front_threshold;

  ctx->us_prevTick = 0;
  ctx->us_state = 0;

  ctx->prev_max_idx = 0xFF;
  ctx->stop_pending = 0;
  ctx->stop_until_ms = 0;

  ctx->d0 = ctx->d1 = ctx->d2 = 0;
  ctx->max_idx = 0;
  ctx->max_d = 0;
}
// 초기화

void Auto_Update(AMC *ctx, const volatile uint16_t distance[3], volatile uint8_t dist_ready[3])
{
  uint32_t now = HAL_GetTick();

  if((now - ctx->us_prevTick) >= ctx->trig_interval)   // 논 블로킹 딜레이
  {
    ctx->us_prevTick = now;

    HCSR04_TRG(ctx->us_state);     // 현재 센서 트리거

    ctx->us_state++;               // 다음 센서로
    if(ctx->us_state > 2)
    {
      ctx->us_state = 0;
    }
  }

  if(dist_ready[0] && dist_ready[1] && dist_ready[2]) // 세개의 센서의 측정이 완료 될 때만 동작
  {
    dist_ready[0] = dist_ready[1] = dist_ready[2] = 0;

    ctx->d0 = distance[0];
    ctx->d1 = distance[1];
    ctx->d2 = distance[2];

    if(ctx->d0 > 100)
    {
      ctx->d0 = 100;
    }
    if(ctx->d2 > 100)
    {
      ctx->d2 = 100;
    }
    // 과도한 노이즈 필터링

    ctx->max_d = ctx->d0;
    ctx->max_idx = 0;
    if(ctx->d1 > ctx->max_d)
    {
      ctx->max_d = ctx->d1;
      ctx->max_idx = 1;
    }
    if(ctx->d2 > ctx->max_d)
    {
      ctx->max_d = ctx->d2;
      ctx->max_idx = 2;
    }
  }
  // 최대 거리 계산

  if((now - ctx->ctrl_prevTick) >= ctx->ctrl_interval)
  {
    ctx->ctrl_prevTick = now;
    if(ctx->d1 >= ctx->front_threshold)
    {
      if(ctx->d0 <= ctx->lr_threshold && ctx-> d2 > ctx->lr_threshold)
      {
        auto_turn_right();
      }
      else if(ctx->d2 <= ctx->lr_threshold && ctx-> d0 > ctx->lr_threshold)
      {
        auto_turn_left();
      }
      // 좌우 거리가 너무 작을 시 반대쪽으로 회전
      else if(ctx->d0 <= ctx->lr_threshold && ctx->d2 <= ctx->lr_threshold)
      {
        auto_front();
      }
      // 둘 다 작으면 직진
      else
      {
        auto_front();
      }
      // 좌우가 여유로우면 직진
    }
    else
    {
      if(ctx->d0 > ctx->d2)
      {
        auto_turn_left();
      }
      else
      {
        auto_turn_right();
      }
      // 더 넓은 곳으로 회전
    }
  }
}
