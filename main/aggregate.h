// Duration in seconds of the aggragation window.
#define AGGREGATION_WINDOW_SIZE CONFIG_AGGREGATION_WINDOW_SIZE

/**
 * Performs value aggragation. Returns aggregated value.
*/
float get_aggregated_value(float *buffer, int samples);