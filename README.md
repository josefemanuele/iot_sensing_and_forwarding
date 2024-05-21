
# IoT sensing and forwarding

This project has been developed for the [esp32s3](https://www.espressif.com/en/products/socs/esp32) device. 
The aim of this projects is being able to sense an external signal, perform calculations such as fft and 
aggregation, connecting to the cloud and uploading aggregated values for further analysis.

# Sampling the signal

## Setup
To sample the signal we have used a 3.5 mm audio jack as external sensor. We have reproduced a voltage 
divider circuit to center the signal at half the operating voltage of the esp32 board, as well as to 
filter out the continuous current outputted by the audio card of the pc. [Reference](https://forum.arduino.cc/t/how-to-read-data-from-audio-jack/458301/3)

![Setup](img/setup.jpeg)
![Circuit](img/circuit.gif)

## Reproducing a signal
With such setup we are able to sense audio signal produced by the pc. With the python script provided under 
`scripts/`, we're able to setup the characteristics of the audio signal, like frequency and amplitude.

![Circuit](img/signal.png)

To reproduce the experiment above, edit the `main/sample.c` file, uncommenting the printf. Download [BetterSerial Plotter]
(https://hackaday.io/project/181686-better-serial-plotter) to plot the values received from the serial connection 
pc-to-esp32. Start the python script to send the audio signal.

Alternatively, for testing purpouses, we've also implemented the reproduction of a sinusoidal signal inside the c code. The reproduced signal
can be defined as a sum of multiples sinusoids, with varying amplitudes and frequencies. With the FFT operation, we want to identify these
different frequencies and find the highest frequency composing the signal.

To reproduce a composed signal from the code, edit the `main/sample.h` file, setting the signal parameters. Also, uncomment
the 

# Performing FFT

To compute FFT esp-idf comes with a handy library, the [esp-dsp](https://docs.espressif.com/projects/esp-dsp/en/latest/esp32/esp-dsp-examples.html).
We used the example on the documentation as a template, to compute the fft of the signal. From there we started computing magnitudes of the 
different frequencies tested, applying z-score evaluation, to find outlier values, hance identifying all of the different frequencies 
composing the audio signal. From those, we are only interested in the highest frequency. Because of the [Nyquist-Shannon sampling theorem]
(https://en.wikipedia.org/wiki/Nyquist–Shannon_sampling_theorem) we need to sample a signal at at least twice the frequency of the signal
to be able to correctly reconstruct it. By setting the sampling frequency at twice the maximum frequency composing the signal, we are sure to 
be able to safely reconstruct it.

You can find the implementation of the fft calculation and frequency extrapolation in `main/fft.c`.

# Adapting the sampling frequency

As per the Nayquist-Shannon theorem, once found the maximum frequency of the input signal, we set the adapted sampling frequency
at twice the found frequency + 1.

You can find the calculation of the adapted frequency in `main/fft.c`.

# Aggregation

For the purpouse of this project we used a mean function as the aggregation function. Having defined the aggregation window in 
`main/aggregation.h`, for example 5 seconds, we take 5 seconds of samples from the sampled buffer, apply the aggregation function, 
and obtain the aggregated value.

Aggregation function is in `main/aggregate.c`

# Connecting to the cloud

To connect to the cloud the devices needs internet connectivity. We used a WiFi access point to provide internet connectivity. Then, 
implemented MQTT connectivity to make the esp32 board communicate with an external MQTT broker. We installed [Mosquitto](https://mosquitto.org)
as local MQTT broker, used [esp-mqtt](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/protocols/mqtt.html) 
library to control connection from the device to the broker.

The MQTT implementation is in `main/mqtt.c`.

# System performances

## Energy consumption

## Data volume

## Network latency

To calculate network latency we measured round-trip time from the device and the MQTT broker. We queried the system time when sending 
the aggregated value, queried again when receiving confirmation from the broker. The difference between the two values gives us the 
time round-trip time, the half of rtt gives an estimation on network latency.

# Evaluating performances with different input signals

# Further improvements

# Annotations
