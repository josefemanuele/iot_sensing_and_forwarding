// Size of measurements buffer.
#define BUFFER_SIZE CONFIG_BUFFER_SIZE
// Sensing sampling rate in hertz.
#define MAX_SAMPLING_FREQUENCY CONFIG_MAX_SAMPLE_FREQUENCY
// Z-score threshold for outlier detection.
#define Z_SCORE_THRESHOLD 0.5
// ADC reading pin settings.
#define ADC_PIN ADC1_CHANNEL_1
#define ADC_UNIT ADC_UNIT_1
#define ADC_ATTENUATION ADC_ATTEN_DB_11
#define ADC_BIT_WIDTH ADC_WIDTH_BIT_DEFAULT
// Define composed signal parameters.
#define AMPLITUDE_1 1
#define FREQUENCY_1 100
#define AMPLITUDE_2 5
#define FREQUENCY_2 50
// Produce signal as a sum of sinousoids. Signal is centrated at 1600, and oscillates +/- 1100.
#define DIGITAL_SIGNAL(T, A1, F1, A2, B2) (2500 + ((A1 * sin(2 * M_PI * F1 * T) + A2 * sin(2 * M_PI * B2 * T)) / (A1 + A2) * 200))

/**
 * Setup ADC module.
*/
void sample_setup();

/**
 * Sampling function. To be set as a task. Takes measurements into 
 * measuring buffer. Perform measurements in loop according to set
 * SAMPLING_RATE.
*/
void fill_buffer(float *buffer, int size, float sampling_frequency);
