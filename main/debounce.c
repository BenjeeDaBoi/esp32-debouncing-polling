#include <stdio.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

#define BLINK_GPIO 8
#define BOOT_GPIO 9
#define POLLING_RATE_ms 30

static led_strip_t* pStrip_a;

static void poll_boot_button(void)
{

    if (gpio_get_level(BOOT_GPIO) == 0) {
        ESP_LOGI("[BUTTON]", "Button pressed");
        pStrip_a->set_pixel(pStrip_a, 0, 0, 255, 0);
    } else {
        ESP_LOGI("[BUTTON]", "Button released");
        pStrip_a->set_pixel(pStrip_a, 0, 255, 0, 0);
    }

    pStrip_a->refresh(pStrip_a, 50);

}

static void configure(void)
{

    pStrip_a = led_strip_init(CONFIG_BLINK_LED_RMT_CHANNEL, BLINK_GPIO, 1);
    pStrip_a->clear(pStrip_a, 50);

    gpio_reset_pin(BOOT_GPIO);
    gpio_set_direction(BOOT_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BOOT_GPIO, GPIO_PULLUP_ONLY);

}


void app_main(void)
{
    
    configure();
    while (true) {

        // Polling every 30ms
        poll_boot_button();
        vTaskDelay(POLLING_RATE_ms / portTICK_RATE_MS);

    }

}
