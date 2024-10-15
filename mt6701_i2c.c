#include "mt6701_i2c.h"

#include <stdlib.h>

#include "driver/i2c_master.h"
#include "esp_log.h"

#define MT6701_REG_ANGLE_14B 0x03
#define MT6701_READ_TIMEOUT 50

static char *TAG = "mt6701";
static i2c_master_bus_handle_t i2c_master_bus_handle;

static esp_err_t esp_mt6701_init() {
  i2c_master_bus_config_t i2c_mst_config = {
      .i2c_port = CONFIG_ESP_MT6701_I2C_PORT_NUM,
      .sda_io_num = CONFIG_ESP_MT6701_I2C_SDA_PIN_NUM,
      .scl_io_num = CONFIG_ESP_MT6701_I2C_SCL_PIN_NUM,
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .flags.enable_internal_pullup = true,
  };

  ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &i2c_master_bus_handle));
  ESP_LOGD(TAG, "Initialize i2c master bus success");

  return ESP_OK;
}

esp_err_t esp_mt6701_get_angle(esp_mt6701_dev_handle_t handle, int16_t *const angle, float *const angle_f) {
  esp_err_t ret = ESP_OK;
  uint8_t data[2] = {0, 0};

  uint8_t register_addr[1] = {MT6701_REG_ANGLE_14B};
  ESP_ERROR_CHECK(i2c_master_transmit_receive(handle->dev_handle, register_addr, sizeof(register_addr), data, sizeof(data), MT6701_READ_TIMEOUT));

  *angle = ((int16_t)data[0] << 6) | (data[1] >> 2);
  *angle_f = (float)*angle * 360 / 16384;
  return ret;
}

esp_err_t esp_mt6701_create(uint16_t dev_addr, esp_mt6701_dev_handle_t *handle) {
  esp_err_t err;
  if (i2c_master_bus_handle == NULL) {
    err = esp_mt6701_init();
    if (err != ESP_OK) {
      return err;
    }
  }

  esp_mt6701_dev_handle_t mt6701_handle = (esp_mt6701_dev_handle_t)calloc(1, sizeof(struct esp_mt6701_dev_t));

  ESP_LOGD(TAG, "Register mt6701 i2c device@%d", dev_addr);
  i2c_device_config_t dev_cfg = {
      .dev_addr_length = I2C_ADDR_BIT_LEN_7,
      .device_address = dev_addr,
      .scl_speed_hz = CONFIG_ESP_MT6701_I2C_SCL_SPEED,
  };
  i2c_master_dev_handle_t i2c_magnetic_dev_handle;
  ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_master_bus_handle, &dev_cfg, &i2c_magnetic_dev_handle));

  mt6701_handle->dev_handle = i2c_magnetic_dev_handle;
  *handle = mt6701_handle;
  ESP_LOGD(TAG, "Register knob device success");

  return ESP_OK;
}

esp_err_t esp_mt6701_delete(esp_mt6701_dev_handle_t handle) {
  i2c_master_bus_rm_device(handle->dev_handle);
  free(handle);
  return ESP_OK;
}
