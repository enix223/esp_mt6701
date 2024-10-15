#include <stdio.h>
#include "mt6701_i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

static char *TAG = "app_main";
static esp_mt6701_dev_handle_t handle;

static void knob_task(void *args) {
  int16_t angle_i;
  float angle_f;
  esp_err_t err;

  while(1) {
    err = esp_mt6701_get_angle(handle, &angle_i, &angle_f);
    if (err != ESP_OK) {
      ESP_LOGW(TAG, "failed to read knob angle: %d", err);
    } else {
      ESP_LOGI(TAG, "GOT ANGLE, int16 = %d, float = %.5f", angle_i, angle_f);
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void app_main(void)
{
  // init mt6701
  ESP_ERROR_CHECK(esp_mt6701_create(ESP_MT6701_I2C_ADDRESS_DEFAULT, &handle));

  // create read task
  xTaskCreatePinnedToCore(knob_task, "Knob task", 10000, NULL, 4, NULL, 1);
}
