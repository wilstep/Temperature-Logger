#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define av_N 20 // number of samples to average per read
#define tfile "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"

