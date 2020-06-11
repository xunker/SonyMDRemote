Sony RM-CDF7L Remote when used with MZ-NHF800
=========================================

## LCD

Although there is more space for other items, only some items on the display
come alive when plugged in to a NHF800 while playing:

  * Current Track Number
  * Current Battery Level
  * Indication of volume change
  * Repeat on/off
  * Repeat 1 track
  * Shuffle

## Buttons

All buttons function as expected, with the exception of "Radio On" button, which
is expected.

## Operation

Although the remote data line comes alive when you press "Stop" on the player
from an "off" state, that data stream does not seem to effect the display on the
remote in any way.

Can't wake up the player from "off" state with any remote buttons.

## Button Resistances

Multimeter wiring:

Remote pin 2 (RMT) disconnected from MD player, connected to negative of multimeter.
Remote pin 4 (VCC) remains connected to MD player, also connected to positive of multimeter.

Remote pins 1 and 3 remain connected to MD player.

Rest    0
Vol +   9.8k
Vol -   8.38k
Play    0.363k
Prev    1.037k
Next    3.67k
Stop    7.09k
Hold sw 0.5meg to 0.65meg (fluctuates)

## Voltage

Tested with Vcc of 2.1v and logic level of 2.1-to-2.3v.

## TODO

LCD signal protocol
