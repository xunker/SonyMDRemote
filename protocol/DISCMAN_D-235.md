Display Protocol for Discman D-235
==================================

Exploration of the display protocol for the Sony Discman D-235 (1994) and possibly others.

## Remote buttons

pin 4 is at 3.1v

connected to pin 2 via resistor network

## Display So far

3.2v ptp

logic unit is multiples of 2 milliseconds.

looks like there is no pause between messages, continuous bitstream so unsure if protocol is active low or active high

player continues to send some data even after built-in and remote display show no data

continuous data stream could be the disc spinning animation
