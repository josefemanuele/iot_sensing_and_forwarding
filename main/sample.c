// Log management.
#include "esp_log.h"
// Task management.
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// ADC module.
#include "driver/adc.h"
#include "esp_adc_cal.h"
// SAMPLING settings.
#include "sample.h"

// TEST.
// Produce sin function
#include <math.h>

// ADC calibration structure.
static esp_adc_cal_characteristics_t adc1_chars;
// Log message tag.
static const char* TAG = "sample";

/**
 * Setup ADC module.
*/
void sample_setup() 
{
    // Set ADC calibration.
    esp_adc_cal_characterize(ADC_UNIT, ADC_ATTENUATION, ADC_BIT_WIDTH, 0, &adc1_chars);
    // Set ADC bit size and attenuation.
    ESP_ERROR_CHECK(adc1_config_width(ADC_BIT_WIDTH));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC_PIN, ADC_ATTENUATION));
}

/**
 * Sampling function. To be set as a task. Takes measurements into 
 * measuring buffer. Perform measurements in loop according to set
 * SAMPLING_RATE.
*/
void fill_buffer(float *buffer, int size, float sampling_frequency)
{
    int i;
    int sampling_interval;
    float t;

    // Fill measurements buffer.
    sampling_interval = (1.0 / sampling_frequency) * 1000;
    for (i = 0; i < size; i++)
    {
        buffer[i] = (float) esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC_PIN), &adc1_chars);
        //t = i * ((float) sampling_interval / 1000);
        //buffer[i] = DIGITAL_SIGNAL(t, AMPLITUDE_1, FREQUENCY_1, AMPLITUDE_2, FREQUENCY_2);
        //ESP_LOGI(TAG, "Read voltage: %f mV.", buffer[i]);
        // Sleep to sample according to set SAMPLING_FREQUENCY.
        vTaskDelay(pdMS_TO_TICKS(sampling_interval));
    }
}
