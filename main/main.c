// Log management.
#include "esp_log.h"
// Task management.
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// Flash NVS (Nov-Volatile Storage).
#include "nvs_flash.h"
// Utility functions and program settings.
#include "sample.h"
#include "fft.h"
#include "station.h"
#include "aggregate.h"
#include "mqtts.h"

#define ONE_SECOND 1000

// Log message tag.
static const char* TAG = "main";

// Data for MQTT.
char data[8];

// Measurements buffer.
__attribute__((aligned(16)))
static float buffer[BUFFER_SIZE];

/**
 * Application entry point. 
*/
void app_main(void)
{
  float frequency;
  float adapted_frequency;
  int samples;
  float aggregated_value;

  //Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
  {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // Sample signal at maximum sampling frequency.
  sample_setup();
  ESP_LOGI(TAG, "Sampling signal at maximum sampling frequency: %i Hz.", MAX_SAMPLING_FREQUENCY);
  fill_buffer(buffer, BUFFER_SIZE, MAX_SAMPLING_FREQUENCY);

  // Perform fft on sampled signal.
  fft_setup();
  frequency = get_fft_highest_frequency(buffer);
  ESP_LOGI(TAG, "Found highest frequency: %f Hz.", frequency);

  // Re-sample signal at adapted sampling frequency.
  adapted_frequency = (2 * frequency) + 1;
  ESP_LOGI(TAG, "Sampling signal at adapted sampling frequency: %f Hz.", adapted_frequency);
  fill_buffer(buffer, BUFFER_SIZE, adapted_frequency);

  // Perform aggregation.
  samples = adapted_frequency * AGGREGATION_WINDOW_SIZE;
  if (samples > BUFFER_SIZE)
  {
    samples = BUFFER_SIZE;
    ESP_LOGW(TAG, "Signal frequency too high for given aggregation window size. \
    Aggregation window reduced to %f seconds.", samples / adapted_frequency);
  }
  aggregated_value = get_aggregated_value(buffer, samples);
  ESP_LOGI(TAG, "Calculated aggregated value: %f", aggregated_value);
  
  // Connect to wifi.
  wifi_init_sta();

  // Connect to mqtt.
  sprintf(data, "%d", (int)aggregated_value);
  mqtt_start(data);

  // Send aggregated value.

  ESP_LOGI(TAG, "Done.");
}
