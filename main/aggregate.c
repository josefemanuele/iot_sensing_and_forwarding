
/**
 * Performs value aggragation. Returns aggregated value.
*/
float get_aggregated_value(float *buffer, int samples)
{
    float sum;

    sum = 0;
    for (int i = 0; i < samples; i++) 
    {
        sum += buffer[i];
    }

    return sum / samples;
}