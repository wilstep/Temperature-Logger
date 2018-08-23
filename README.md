# Temperature-Logger
Temperature logging program and device circuit for the BeagleBone Black (BBB). The circuit makes use of a temperature sensor to send a voltage signal to the BBB which is proportional to the temperature. The software then causes the BBB to sample this signal 20 times every half a second and forms an average. This average is written to the file `tlog.csv` along with the time and date at regular intervals, as specified on the command line when the program is started. If the file `tlog.csv` already exists it will be appended to. Otherwise it will be made with the first line giving headings for the columns.

#### What you need
1 off BeagleBone, 1 off Breadboard, 1 off LM335Z temperature sensor linear ic \**, 1 off LM358 Low power dual op-amp linear ic, various resistors, 1 off 10 or 25 turn 10k trim pot, 1 off multimeter

\**This was just the temperature sensor available at my local Jaycar store. One could easily adopt for a different temperature sensor (would require minor changes to the circuit) but be careful not to put more than 1.8v into the ADC

#### The BBB ADC
This works on my BeagleBone Black, which is running Debian with kerel 4.1.15-ti-rt-r43

To enable the Analogue to Digital Converter (ADC) I issued the following from the root account

`sh -c "echo 'BB-ADC' > /sys/devices/platform/bone_capemgr/slots"`

To make this permanent (remains after a reboot) insert the line:
`cape_enable=bone_capemgr.enable_partno=BB-ADC`
into the file 
`/boot/uEnv.txt`

The sytem file:
`/sys/bus/iio/devices/iio:device0/in_voltage0_raw`
should now have the current value of the ADC we wish to use. 
This is a 12 bit ADC so I assume the value may be from 0 - 4095

#### The Circuit

The circuit (circuit.JPG) is a simple one, which makes use of two op-amps, the first as a voltage follower and the second to shift and amplify the signal. For the LM335Z (temperature sensor) leave the ADJ terminal unconnected. Then connect its +ve terminal to the resistor. Now connect the 5v source from the BBB to the other end of the resistor and the ground from the BBB to the -ve terminal of the LM335Z. Make a second connection from the +ve terminal of the LM335Z to the first op-amp.

The LM335Z when wired up this way will change voltage to the ADC at a gradient of approximately 10mV/deg Celsius which is then amplified to 15mV/deg. This lets us have slightly more than 30 quantised numbers from the ADC per degree. There is nothing to stop the signal going above 1.8v if the sensor gets too hot, so be careful. This will occur somewhere around 70 deg Celsius.

#### The Code

The code is a simple C program and straight forward to compile;
`gcc tlog.c -lm -o tlog`.
You will need to calibrate the software for your particular example of the measuring device. The code uses a least squares fit to the data which you enter in the the variables Ncal, T_cal[], and cnt_cal[] near the top of tlog.c.
  To obtain the calibration data, measure the temperature of the device (while it is not changing) (data for T_cal) and simultaneously run "tlog a" to obtain the adc value n (data for cnt_cal). In this way obtain a series of ordered pairs and enter them into tlog.c, after deleting the existing data (which is for my device not yours). Place the number of ordered pairs you have in the value for Ncal.
