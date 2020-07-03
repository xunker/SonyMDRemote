Sony WM-FX855 Cassette Walkman
==============================

## Remote Connector Differences

This player (and possibly other Cassette Walkman players) is slightly different than the average connector (year 1998 > MiniDisc or Discman):

* (pin 1) Ground
  - Same as average connector
* (pin 2) Remote In
  - this pin is *pulled-up* to about 2.7v, and remote control buttons pull it to ground through a resistor network
  - it's unlike the average connector, where it is *pulled-down* and actions are triggered by pulling it up to Vcc via a resistor network
* (pin 3) Data Out
  - Active-high, with high being voltage close to battery voltage and low being 0v
  - different than average connector, where this is active-low
* (pin 4) Battery positive via 1k resistor
  - 1.5v ideally, but ~1.45v max in the real world
  - different from average connector, where this pin is a regulated/boosted voltage usually from 2.1v to 2.9v

## Built-in buttons vs remote buttons

The service manual shows that the built-in buttons connect to the same pins as the remote buttons. The resistance values calculated are:

* Stop / Radio off
  - 0
* Play / Reverse / Repeat
  - 0.82k
* Fast-Forward / Next Preset
  - 1.82k
* Rewind / Previous Preset
  - 3.02k
* Band / Radio On
  - 4.52k

These resistances are approximate to the [RM-WMF5 Remote](../remotes/Sony%20RM-WMF5.md) that I have used successfully.

The "ASP" button and tuning buttons inside the tape deck do not connect through the same remote line.

### Other remote actions

* Bass Boost
  - 5.8k-6.8k
