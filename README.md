# ESP32-S3 IDF框架 控制 WS2812 灯条：实现多种灯效

在这篇博客中，我们将介绍如何使用 ESP32-S3 开发板和 RMT 驱动程序来控制 WS2812 LED 灯条。我们会展示如何编写一个 `ws2812_control.c` 文件，该文件包含多种 LED 控制效果，包括常亮、呼吸、渐入、闪烁和彩虹效果。

## 环境准备
此项目我们会使用以下工具和库：
- **RMT (Remote Control)**：ESP32 的硬件外设，用于生成准确的时序信号来控制 WS2812。
- **LED Strip 库**：ESP-IDF 中用于操作 LED 灯条的库。
## 代码实现
### `ws2812_control.h`

头文件定义了 LED 颜色的结构体 `led_color_t` 和 LED 效果的枚举类型 `led_effect_t`，并声明了各种 LED 控制函数。

```c
#ifndef WS2812_CONTROL_H
#define WS2812_CONTROL_H

#include <stdint.h>
#include "led_strip.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t red;
    uint32_t green;
    uint32_t blue;
} led_color_t;

typedef enum {
    LED_EFFECT_OFF,
    LED_EFFECT_ON,
    LED_EFFECT_BREATH,
    LED_EFFECT_FADE_IN,
    LED_EFFECT_BLINK_SLOW,
    LED_EFFECT_BLINK_FAST,
    LED_EFFECT_RAINBOW
} led_effect_t;

void ws2812_control_task(void *arg);
void led_set_on(led_strip_t *strip, led_color_t color);
void led_set_off(led_strip_t *strip);
void led_set_breath(led_strip_t *strip, led_color_t color);
void led_set_fade_in(led_strip_t *strip, led_color_t color);
void led_set_blink_slow(led_strip_t *strip, led_color_t color);
void led_set_blink_fast(led_strip_t *strip, led_color_t color);
void led_set_rainbow(led_strip_t *strip);

#ifdef __cplusplus
}
#endif

#endif // WS2812_CONTROL_H
```

### `ws2812_control.c`

.c文件中实现了所有的 LED 效果函数。辅助函数 `led_strip_hsv2rgb()`是乐鑫官方的代码，用于将 HSV 颜色空间转换为 RGB 颜色空间，这是因为 WS2812 灯条使用 RGB 颜色模式。

#### 核心功能函数

- **led_set_on()**：设置 LED 常亮。
- **led_set_off()**：关闭 LED。
- **led_set_breath()**：实现呼吸效果。
- **led_set_fade_in()**：实现缓缓亮起的效果。
- **led_set_blink_slow()**：实现慢速闪烁。
- **led_set_blink_fast()**：实现快速闪烁。
- **led_set_rainbow()**：实现彩虹效果。

#### 任务函数

`ws2812_control_task()` 是WS2812任务函数，负责根据不同的 LED 效果类型调用相应的控制函数。

```c
void ws2812_control_task(void* arg) {
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(CONFIG_EXAMPLE_RMT_TX_GPIO, RMT_TX_CHANNEL);
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(CONFIG_EXAMPLE_STRIP_LED_NUMBER, (led_strip_dev_t)config.channel);
    led_strip_t *strip = led_strip_new_rmt_ws2812(&strip_config);
    if (!strip) {
        ESP_LOGE(TAG, "install WS2812 driver failed");
    }
    ESP_ERROR_CHECK(strip->clear(strip, 100));

    led_color_t color = {255, 0, 0}; // 设置颜色为红色
    led_effect_t effect = LED_EFFECT_BREATH; // 选择呼吸效果

    while (true) {
        switch (effect) {
            case LED_EFFECT_ON:
                led_set_on(strip, color);
                break;
            case LED_EFFECT_OFF:
                led_set_off(strip);
                break;
            case LED_EFFECT_BREATH:
                led_set_breath(strip, color);
                break;
            case LED_EFFECT_FADE_IN:
                led_set_fade_in(strip, color);
                break;
            case LED_EFFECT_BLINK_SLOW:
                led_set_blink_slow(strip, color);
                break;
            case LED_EFFECT_BLINK_FAST:
                led_set_blink_fast(strip, color);
                break;
            case LED_EFFECT_RAINBOW:
                led_set_rainbow(strip);
                break;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
```
