# Temperature-Logger
Temperature logging program for the BeagleBone Black

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

