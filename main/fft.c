/**
 * Source: https://github.com/espressif/esp-dsp/tree/master/examples/fft
*/

// Log management.
#include "esp_log.h"
// Math functions.
#include <math.h>
// Esp-dsp module.
#include "esp_dsp.h"
// Sensing settings.
#include "sample.h"

static const char *TAG = "fft";

// Hann window.
__attribute__((aligned(16)))
static float window[BUFFER_SIZE];

// Working complex array.
__attribute__((aligned(16)))
static float working_buffer[BUFFER_SIZE * 2];

/**
 * Setup fft module.
*/
void fft_setup()
{
    // Error check.
    esp_err_t ret;
    ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
    if (ret  != ESP_OK) 
    {
        ESP_LOGE(TAG, "Not possible to initialize FFT. Error = %i", ret);
        return;
    }
    // Generate hann window
    dsps_wind_hann_f32(window, BUFFER_SIZE);
}

/**
 * Calculates magnitudes on the fft output, performs outlier
 * detection and returns the highest frequency.
*/
float find_highest_frequency()
{
    int i;
    float magnitude;
    float sum;
    float sum1;
    float mean;
    float standard_deviation;
    float z_score;
    float frequency;
    float highest_frequency;

    // Calculate magnitude of fft output, and sum.
    sum = 0;
    for (i = 0; i < BUFFER_SIZE / 2; i++) 
    { 
        magnitude = sqrt(pow(working_buffer[i * 2 + 0], 2) + pow(working_buffer[i * 2 + 1], 2));
        working_buffer[i] = magnitude;
        sum += magnitude;
    }
    // Calulate mean and standard deviation.
    mean = sum / (BUFFER_SIZE / 2);
    sum1 = 0;
    for (i = 0; i < BUFFER_SIZE / 2; i++)
    {
        sum1 += pow((working_buffer[i] - mean), 2);
    }
    standard_deviation = sqrt(sum1 / (BUFFER_SIZE / 2));

    // Calculate Z-score, find outliers, and find highest frequency.
    highest_frequency = 0;
    for (i = 0; i < BUFFER_SIZE / 2; i++)
    {
        z_score = (working_buffer[i] - mean) / standard_deviation;
            frequency = ((float) i * MAX_SAMPLING_FREQUENCY) / BUFFER_SIZE;
        ESP_LOGI(TAG, "Frequency: %f Z-score: %f Mean: %f std: %f", frequency, z_score, mean, standard_deviation);
        if (z_score > Z_SCORE_THRESHOLD) 
        {
            if (frequency > highest_frequency && frequency >= 1) 
            {
                highest_frequency = frequency;
            }
        }
    }

    return highest_frequency;
}

/**
 * Performs fft on buffer. Returns highest frequency.
*/
float get_fft_highest_frequency(float *buffer)
{
    // Build complex vector for fft analysis. Perform Hann windowing as well.
    for (int i = 0 ; i < BUFFER_SIZE ; i++) 
    {
        working_buffer[i * 2 + 0] = buffer[i] * window[i];
        working_buffer[i * 2 + 1] = 0;
    }
    // FFT
    dsps_fft2r_fc32(working_buffer, BUFFER_SIZE);
    // Bit reverse
    dsps_bit_rev_fc32(working_buffer, BUFFER_SIZE);
    // Convert one complex vector to two complex vectors
    dsps_cplx2reC_fc32(working_buffer, BUFFER_SIZE);
    // What?
    // for (int i = 0 ; i < BUFFER_SIZE / 2 ; i++) {
    //     working_buffer[i] = 10 * log10f((pow(working_buffer[i * 2 + 0], 2)+ pow(working_buffer[i * 2 + 1], 2)) / BUFFER_SIZE);
    // }

    return find_highest_frequency();
}
