#ifndef WS2812_CONTROL_H
#define WS2812_CONTROL_H

#include <stdint.h>
#include "led_strip.h"
#ifdef __cplusplus
extern "C" {
#endif

// led_color_t 结构体表示 LED 灯的颜色
typedef struct {
    uint32_t red;
    uint32_t green;
    uint32_t blue;
} led_color_t;

// led_effect_t 枚举类型表示 LED 灯的效果类型
typedef enum {
    // 关闭LED灯效果
    LED_EFFECT_OFF,
    // 使LED灯常亮效果
    LED_EFFECT_ON,
    // 使LED灯呼吸效果
    LED_EFFECT_BREATH,
    // 使LED灯淡入效果
    LED_EFFECT_FADE_IN,
    // 使LED灯慢闪烁效果
    LED_EFFECT_BLINK_SLOW,
    // 使LED灯快闪烁效果
    LED_EFFECT_BLINK_FAST,
    // 使LED灯彩虹效果
    LED_EFFECT_RAINBOW
} led_effect_t;


void ws2812_control_task(void *arg);    // WS2812控制任务
void led_set_on(led_strip_t *strip, led_color_t color); // LED 常亮
void led_set_off(led_strip_t *strip);       // LED 关闭
void led_set_breath(led_strip_t *strip, led_color_t color); // LED 呼吸效果
void led_set_fade_in(led_strip_t *strip, led_color_t color);    // LED 缓缓亮起
void led_set_blink_slow(led_strip_t *strip, led_color_t color); // LED 微微闪烁
void led_set_blink_fast(led_strip_t *strip, led_color_t color); // LED 快速闪烁
void led_set_rainbow(led_strip_t *strip);   // 彩虹效果


#ifdef __cplusplus
}
#endif

#endif // WS2812_CONTROL_H