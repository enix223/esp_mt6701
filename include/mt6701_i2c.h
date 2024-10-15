#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/i2c_master.h"

#define ESP_MT6701_I2C_ADDRESS_DEFAULT (0x6)

struct esp_mt6701_dev_t {
  i2c_master_dev_handle_t dev_handle;
};

typedef struct esp_mt6701_dev_t *esp_mt6701_dev_handle_t;

/**
 * @brief Create and init sensor object and return a sensor handle
 *
 * @param[in] dev_addr I2C device address of sensor, if equals -1, then default address is used
 * @param[out] handle MT6701 dev handle
 *
 * @return
 *     - NULL Fail
 *     - Others Success
 */
esp_err_t esp_mt6701_create(uint16_t dev_addr, esp_mt6701_dev_handle_t *handle);

/**
 * @brief Delete and release a sensor object
 *
 * @param[in] handle object handle of mt6701
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t esp_mt6701_delete(esp_mt6701_dev_handle_t handle);

/**
 * @brief Get angle from mt6701
 *
 * @param      handle object handle of mt6701
 * @param[out] angle angle value got from mt6701 in [int16_t]
 * @param[out] angle_f angle value got from mt6701 in [float]
 *
 * @return
 *     - ESP_OK Success
 *     - ESP_FAIL Fail
 */
esp_err_t esp_mt6701_get_angle(esp_mt6701_dev_handle_t handle, int16_t *const angle, float *const angle_f);

#ifdef __cplusplus
}
#endif
