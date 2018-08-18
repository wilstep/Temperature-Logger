# Temperature-Logger
Temperature logging program for the BeagleBone Black

#### What you need
1 off BeagleBone, 1 off Breadboard, 1 off 1.5k resistor, 1 off LM335Z temperature sensor linear ic, 1 off multimeter

This was just the temperature sensor available at my local Jaycar store. One could easily addopt for a different temperature sensor (would require minor changes to the code) but be careful not to put more than 1.8v into the ADC

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

#### The Code

The code is a simple C program and straight forward to compile.

