#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "esp32/ulp.h"
#include "ulp_main.h"
#include "driver/rtc_io.h"

extern const uint8_t bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t bin_end[]   asm("_binary_ulp_main_bin_end");

void app_main() {
  /* Init GPIO 4 (blue), which will be driven by the main core */
  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = GPIO_SEL_4;
  io_conf.pull_down_en = 0;
  io_conf.pull_up_en = 0;
  gpio_config(&io_conf);

  /* Init GPIO2 (RTC_GPIO 12, green) which will be driven by ULP */
  rtc_gpio_init(2);
  rtc_gpio_set_direction(2, RTC_GPIO_MODE_OUTPUT_ONLY);
  rtc_gpio_set_level(2,0);

  vTaskDelay(1000/portTICK_PERIOD_MS);

  /* Start ULP program */
  ESP_ERROR_CHECK( ulp_load_binary(0, bin_start, (bin_end - bin_start) / sizeof(uint32_t)));
  ESP_ERROR_CHECK( ulp_run(&ulp_entry - RTC_SLOW_MEM) );

  bool s = false;
  for (;;) {
    s = !s;
    gpio_set_level(4, (s ? 1 : 0));
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}
